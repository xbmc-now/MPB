#include "StdAfx.h"
#include "EpgDBManager.h"
#include <process.h>

#include "../../Common/TimeUtil.h"

CEpgDBManager::CEpgDBManager(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);

    this->loadThread = NULL;
    this->loadStopEvent = _CreateEvent(FALSE, FALSE, NULL);

	wstring textPath;
	GetModuleFolderPath(textPath);
	textPath += L"\\ConvertText.txt";

	this->chgText.ParseReserveText(textPath.c_str() );

	::CoInitialize( NULL );
	HRESULT hr=regExp.CreateInstance(CLSID_RegExp);
	if(FAILED(hr)){
		regExp = NULL;
	}
}

CEpgDBManager::~CEpgDBManager(void)
{
	if( this->loadThread != NULL ){
		::SetEvent(this->loadStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->loadThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->loadThread, 0xffffffff);
		}
		CloseHandle(this->loadThread);
		this->loadThread = NULL;
	}
	if( this->loadStopEvent != NULL ){
		CloseHandle(this->loadStopEvent);
		this->loadStopEvent = NULL;
	}

	ClearEpgData();

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}

	if( regExp != NULL ){
		regExp.Release();
	}

	::CoUninitialize();
}

BOOL CEpgDBManager::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED ||
		dwRet == WAIT_TIMEOUT){
			OutputDebugString(L"��CEpgDBManager::Lock FALSE");
		return FALSE;
	}
	return TRUE;
}

void CEpgDBManager::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

void CEpgDBManager::ClearEpgData()
{
	try{
		map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itr;
		for( itr = this->epgMap.begin(); itr != this->epgMap.end(); itr++ ){
			SAFE_DELETE(itr->second);
		}
	}catch(...){
		_OutputDebugString(L"��CEpgDBManager::ClearEpgData() Exception");
	}
	this->epgMap.clear();
}

BOOL CEpgDBManager::ReloadEpgData()
{
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = TRUE;
	if( this->loadThread == NULL ){
		//��M�X���b�h�N��
		ClearEpgData();
		ResetEvent(this->loadStopEvent);
		this->loadThread = (HANDLE)_beginthreadex(NULL, 0, LoadThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
		SetThreadPriority( this->loadThread, THREAD_PRIORITY_NORMAL );
		ResumeThread(this->loadThread);
	}else{
		ret = FALSE;
	}

	UnLock();
	return ret;
}

UINT WINAPI CEpgDBManager::LoadThread(LPVOID param)
{
	CEpgDBManager* sys = (CEpgDBManager*)param;

	OutputDebugString(L"Start Load EpgData\r\n");
	DWORD time = GetTickCount();

	CEpgDataCap3Util epgUtil;
	if( epgUtil.Initialize(FALSE) == FALSE ){
		OutputDebugString(L"��EpgDataCap3.dll�̏������Ɏ��s���܂����B\r\n");
		return 0;
	}

	//EPG�t�@�C���̌���
	vector<wstring> epgFileList;
	wstring epgDataPath = L"";
	GetSettingPath(epgDataPath);
	epgDataPath += EPG_SAVE_FOLDER;

	wstring searchKey = epgDataPath;
	searchKey += L"\\*_epg.dat";

	WIN32_FIND_DATA findData;
	HANDLE find;

	//�w��t�H���_�̃t�@�C���ꗗ�擾
	find = FindFirstFile( searchKey.c_str(), &findData);
	if ( find == INVALID_HANDLE_VALUE ) {
		//�P�����݂��Ȃ�
		epgUtil.UnInitialize();
		return 0;
	}
	do{
		if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 ){
			//�{���Ɋg���qDLL?
			if( IsExt(findData.cFileName, L".dat" ) == TRUE ){
				//���������t�@�C�����ꗗ�ɒǉ�
				wstring epgFilePath = L"";
				Format(epgFilePath, L"%s\\%s", epgDataPath.c_str(), findData.cFileName);

				epgFileList.push_back(epgFilePath);
			}
		}
	}while(FindNextFile(find, &findData));

	FindClose(find);

	//EPG�t�@�C���̉��
	for( size_t i=0; i<epgFileList.size(); i++ ){
		HANDLE file = _CreateFile( epgFileList[i].c_str(), GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if( file != INVALID_HANDLE_VALUE ){
			FILETIME CreationTime;
			FILETIME LastAccessTime;
			FILETIME LastWriteTime;
			GetFileTime(file, &CreationTime, &LastAccessTime, &LastWriteTime);

			LONGLONG fileTime = ((LONGLONG)LastWriteTime.dwHighDateTime)<<32 | (LONGLONG)LastWriteTime.dwLowDateTime;
			if( fileTime + 7*24*60*60*I64_1SEC < GetNowI64Time() ){
				//1�T�Ԉȏ�O�̃t�@�C���Ȃ̂ō폜
				CloseHandle(file);
				DeleteFile( epgFileList[i].c_str() );
				_OutputDebugString(L"��delete %s", epgFileList[i].c_str());
			}else{

				DWORD fileSize = GetFileSize( file, NULL );
				BYTE readBuff[188*1024];
				DWORD readSize = 0;
				while( readSize < fileSize ){
					if( ::WaitForSingleObject(sys->loadStopEvent, 0) != WAIT_TIMEOUT ){
						//�L�����Z�����ꂽ
						CloseHandle(file);
						epgUtil.UnInitialize();
						return 0;
					}
					DWORD read=0;
					ReadFile( file, &readBuff, 188*1024, &read, NULL );
					for( DWORD i=0; i<read; i+=188 ){
						epgUtil.AddTSPacket(readBuff+i, 188);
					}
					readSize+=read;
					Sleep(0);
				}
				CloseHandle(file);
			}
		}
		Sleep(0);
	}

	//EPG�f�[�^���擾
	DWORD serviceListSize = 0;
	SERVICE_INFO* serviceList = NULL;
	if( epgUtil.GetServiceListEpgDB(&serviceListSize, &serviceList) == FALSE ){
		epgUtil.UnInitialize();
		return 0;
	}

	for( DWORD i=0; i<serviceListSize; i++ ){
		LONGLONG key = _Create64Key(serviceList[i].original_network_id, serviceList[i].transport_stream_id, serviceList[i].service_id);
		EPGDB_SERVICE_DATA* item = new EPGDB_SERVICE_DATA;
		item->serviceInfo.ONID = serviceList[i].original_network_id;
		item->serviceInfo.TSID = serviceList[i].transport_stream_id;
		item->serviceInfo.SID = serviceList[i].service_id;
		if( serviceList[i].extInfo != NULL ){
			item->serviceInfo.service_type = serviceList[i].extInfo->service_type;
			item->serviceInfo.partialReceptionFlag = serviceList[i].extInfo->partialReceptionFlag;
			if( serviceList[i].extInfo->service_provider_name != NULL ){
				item->serviceInfo.service_provider_name = serviceList[i].extInfo->service_provider_name;
			}
			if( serviceList[i].extInfo->service_name != NULL ){
				item->serviceInfo.service_name = serviceList[i].extInfo->service_name;
			}
			if( serviceList[i].extInfo->network_name != NULL ){
				item->serviceInfo.network_name = serviceList[i].extInfo->network_name;
			}
			if( serviceList[i].extInfo->ts_name != NULL ){
				item->serviceInfo.ts_name = serviceList[i].extInfo->ts_name;
			}
			item->serviceInfo.remote_control_key_id = serviceList[i].extInfo->remote_control_key_id;
		}
		sys->epgMap.insert(pair<LONGLONG, EPGDB_SERVICE_DATA*>(key, item));

		DWORD epgInfoListSize = 0;
		EPG_EVENT_INFO* epgInfoList = NULL;
		if( epgUtil.GetEpgInfoList(item->serviceInfo.ONID, item->serviceInfo.TSID, item->serviceInfo.SID, &epgInfoListSize, &epgInfoList) == TRUE ){
			for( DWORD j=0; j<epgInfoListSize; j++ ){
				EPGDB_EVENT_INFO* itemEvent = new EPGDB_EVENT_INFO;
				sys->ConvertEpgInfo(item->serviceInfo.ONID, item->serviceInfo.TSID, item->serviceInfo.SID, epgInfoList+j, itemEvent);
				item->eventMap.insert(pair<WORD, EPGDB_EVENT_INFO*>(itemEvent->event_id, itemEvent));
			}
		}
		Sleep(0);
	}

	_OutputDebugString(L"End Load EpgData %dmsec\r\n", GetTickCount()-time);
	epgUtil.UnInitialize();

	return 0;
}

//EPG�f�[�^���R�s�[����
BOOL CEpgDBManager::ConvertEpgInfo(WORD ONID, WORD TSID, WORD SID, EPG_EVENT_INFO* src, EPGDB_EVENT_INFO* dest)
{
	if( src == NULL || dest == NULL ){
		return FALSE;
	}

	dest->original_network_id = ONID;
	dest->transport_stream_id = TSID;
	dest->service_id = SID;
	dest->event_id = src->event_id;
	dest->StartTimeFlag = src->StartTimeFlag;
	dest->start_time = src->start_time;
	dest->DurationFlag = src->DurationFlag;
	dest->durationSec = src->durationSec;
	dest->freeCAFlag = src->freeCAFlag;

	if( src->shortInfo != NULL ){
		dest->shortInfo = new EPGDB_SHORT_EVENT_INFO;
		dest->shortInfo->event_name = src->shortInfo->event_name;
		dest->shortInfo->text_char = src->shortInfo->text_char;
		dest->shortInfo->search_event_name = src->shortInfo->event_name;
		dest->shortInfo->search_text_char = src->shortInfo->text_char;
		this->chgText.ChgText(dest->shortInfo->search_event_name);
		this->chgText.ChgText(dest->shortInfo->search_text_char);
	}
	if( src->extInfo != NULL ){
		dest->extInfo = new EPGDB_EXTENDED_EVENT_INFO;
		dest->extInfo->text_char = src->extInfo->text_char;
		dest->extInfo->search_text_char = src->extInfo->text_char;
		this->chgText.ChgText(dest->extInfo->search_text_char);
	}
	if( src->contentInfo != NULL ){
		dest->contentInfo = new EPGDB_CONTEN_INFO;
		for( BYTE i=0; i<src->contentInfo->listSize; i++ ){
			EPGDB_CONTENT_DATA item;
			item.content_nibble_level_1 = src->contentInfo->nibbleList[i].content_nibble_level_1;
			item.content_nibble_level_2 = src->contentInfo->nibbleList[i].content_nibble_level_2;
			item.user_nibble_1 = src->contentInfo->nibbleList[i].user_nibble_1;
			item.user_nibble_2 = src->contentInfo->nibbleList[i].user_nibble_2;
			dest->contentInfo->nibbleList.push_back(item);
		}
	}
	if( src->componentInfo != NULL ){
		dest->componentInfo = new EPGDB_COMPONENT_INFO;
		dest->componentInfo->stream_content = src->componentInfo->stream_content;
		dest->componentInfo->component_type = src->componentInfo->component_type;
		dest->componentInfo->component_tag = src->componentInfo->component_tag;
		dest->componentInfo->text_char = src->componentInfo->text_char;
	}
	if( src->audioInfo != NULL ){
		dest->audioInfo = new EPGDB_AUDIO_COMPONENT_INFO;
		for( WORD i=0; i<src->audioInfo->listSize; i++ ){
			EPGDB_AUDIO_COMPONENT_INFO_DATA item;
			item.stream_content = src->audioInfo->audioList[i].stream_content;
			item.component_type = src->audioInfo->audioList[i].component_type;
			item.component_tag = src->audioInfo->audioList[i].component_tag;
			item.simulcast_group_tag = src->audioInfo->audioList[i].simulcast_group_tag;
			item.ES_multi_lingual_flag = src->audioInfo->audioList[i].ES_multi_lingual_flag;
			item.main_component_flag = src->audioInfo->audioList[i].main_component_flag;
			item.quality_indicator = src->audioInfo->audioList[i].quality_indicator;
			item.sampling_rate = src->audioInfo->audioList[i].sampling_rate;
			item.text_char = src->audioInfo->audioList[i].text_char;
			dest->audioInfo->componentList.push_back(item);
		}
	}
	if( src->eventGroupInfo != NULL ){
		dest->eventGroupInfo = new EPGDB_EVENTGROUP_INFO;
		dest->eventGroupInfo->group_type = src->eventGroupInfo->group_type;
		for( BYTE i = 0; i<src->eventGroupInfo->event_count; i++ ){
			EPGDB_EVENT_DATA item;
			item.original_network_id = src->eventGroupInfo->eventDataList[i].original_network_id;
			item.transport_stream_id = src->eventGroupInfo->eventDataList[i].transport_stream_id;
			item.service_id = src->eventGroupInfo->eventDataList[i].service_id;
			item.event_id = src->eventGroupInfo->eventDataList[i].event_id;
			dest->eventGroupInfo->eventDataList.push_back(item);
		}
	}
	if( src->eventRelayInfo != NULL ){
		dest->eventRelayInfo = new EPGDB_EVENTGROUP_INFO;
		dest->eventRelayInfo->group_type = src->eventRelayInfo->group_type;
		for( BYTE i = 0; i<src->eventRelayInfo->event_count; i++ ){
			EPGDB_EVENT_DATA item;
			item.original_network_id = src->eventRelayInfo->eventDataList[i].original_network_id;
			item.transport_stream_id = src->eventRelayInfo->eventDataList[i].transport_stream_id;
			item.service_id = src->eventRelayInfo->eventDataList[i].service_id;
			item.event_id = src->eventRelayInfo->eventDataList[i].event_id;
			dest->eventRelayInfo->eventDataList.push_back(item);
		}
	}
	return TRUE;
}

BOOL CEpgDBManager::IsLoadingData()
{
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = _IsLoadingData();

	UnLock();
	return ret;
}

BOOL CEpgDBManager::_IsLoadingData()
{
	BOOL ret = FALSE;
	if( this->loadThread == NULL ){
		ret = FALSE;
	}else{
		if( WaitForSingleObject( this->loadThread, 0 ) == WAIT_TIMEOUT ){
			//���s��
			ret = TRUE;
		}else{
			//�I����Ă���
			CloseHandle(this->loadThread);
			this->loadThread = NULL;
			ret = FALSE;
		}
	}

	return ret;
}

BOOL CEpgDBManager::CancelLoadData()
{
	if( Lock() == FALSE ) return FALSE;

	if( this->loadThread != NULL ){
		::SetEvent(this->loadStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->loadThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->loadThread, 0xffffffff);
		}
		CloseHandle(this->loadThread);
		this->loadThread = NULL;
	}

	UnLock();
	return TRUE;
}

BOOL CEpgDBManager::SearchEpg(vector<EPGDB_SEARCH_KEY_INFO>* key, vector<EPGDB_EVENT_INFO*>* result)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = TRUE;

	map<ULONGLONG, SEARCH_RESULT_EVENT> resultMap;
	for( size_t i=0; i<key->size(); i++ ){
		SearchEvent( &(*key)[i], &resultMap );
	}

	map<ULONGLONG, SEARCH_RESULT_EVENT>::iterator itr;
	for( itr = resultMap.begin(); itr != resultMap.end(); itr++ ){
		result->push_back(itr->second.info);
	}

	UnLock();
	return ret;
}

BOOL CEpgDBManager::SearchEpg(EPGDB_SEARCH_KEY_INFO* key, vector<SEARCH_RESULT_EVENT>* result)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = TRUE;

	map<ULONGLONG, SEARCH_RESULT_EVENT> resultMap;
	SearchEvent( key, &resultMap );

	map<ULONGLONG, SEARCH_RESULT_EVENT>::iterator itr;
	for( itr = resultMap.begin(); itr != resultMap.end(); itr++ ){
		result->push_back(itr->second);
	}

	UnLock();
	return ret;
}

void CEpgDBManager::SearchEvent(EPGDB_SEARCH_KEY_INFO* key, map<ULONGLONG, SEARCH_RESULT_EVENT>* resultMap)
{
	if( key == NULL || resultMap == NULL ){
		return ;
	}
	
	if( key->andKey.size() == 0 && key->notKey.size() == 0 && key->contentList.size() == 0 && key->videoList.size() == 0 && key->audioList.size() == 0){
		//�L�[���[�h���W�������w����Ȃ��̂Ō������Ȃ�
		return ;
	}
	
	//�L�[���[�h����
	vector<wstring> andKeyList;
	vector<wstring> notKeyList;

	if( key->regExpFlag == FALSE ){
		//���K�\���ł͂Ȃ��̂ŃL�[���[�h�̕���
		wstring buff = L"";
		if( key->andKey.size() > 0 ){
			wstring andBuff = key->andKey;
			Replace(andBuff, L"�@", L" ");
			do{
				Separate(andBuff, L" ", buff, andBuff);
				chgText.ChgText(buff);
				if( buff.size() > 0 ){
					andKeyList.push_back(buff);
				}
			}while( andBuff.size() != 0 );
		}
		
		if( key->notKey.size() > 0 ){
			wstring notBuff = key->notKey;
			Replace(notBuff, L"�@", L" ");
			do{
				Separate(notBuff, L" ", buff, notBuff);
				chgText.ChgText(buff);
				if( buff.size() > 0 ){
					notKeyList.push_back(buff);
				}
			}while( notBuff.size() != 0 );
		}
	}else{
		if( key->andKey.size() > 0 ){
			andKeyList.push_back(key->andKey);
		}
		if( key->notKey.size() > 0 ){
			notKeyList.push_back(key->notKey);
		}
	}

	//���ԕ���
	vector<TIME_SEARCH> timeList;
	for( size_t i=0; i<key->dateList.size(); i++ ){
		DWORD start = key->dateList[i].startHour*60 + key->dateList[i].startMin;
		DWORD end = key->dateList[i].endHour*60 + key->dateList[i].endMin;
		if( key->dateList[i].startDayOfWeek == key->dateList[i].endDayOfWeek ){
			if( start < end ){
				//�ʏ�
				TIME_SEARCH item;
				item.week = key->dateList[i].startDayOfWeek;
				item.start = start;
				item.end = end;
				timeList.push_back(item);
			}else{
				//1�T�ԉ�
				for( BYTE j=0; j<7; j++ ){
					if( j== key->dateList[i].startDayOfWeek){
						TIME_SEARCH item1;
						item1.week = j;
						item1.start = 0;
						item1.end = end;
						timeList.push_back(item1);
						TIME_SEARCH item2;
						item2.week = j;
						item2.start = start;
						item2.end = 23*60+59;
						timeList.push_back(item2);
					}else{
						TIME_SEARCH item;
						item.week = j;
						item.start = 0;
						item.end = 23*60+59;
						timeList.push_back(item);
					}
				}
			}
		}else{
			BYTE chkWeek = key->dateList[i].startDayOfWeek;
			for( BYTE j=0; j<7; j++ ){
				if( chkWeek == key->dateList[i].startDayOfWeek ){
					TIME_SEARCH item;
					item.week = chkWeek;
					item.start = start;
					item.end = 23*60+59;
					timeList.push_back(item);
				}else if( chkWeek == key->dateList[i].endDayOfWeek ){
					TIME_SEARCH item;
					item.week = chkWeek;
					item.start = 0;
					item.end = end;
					timeList.push_back(item);
					break;
				}else{
					TIME_SEARCH item;
					item.week = chkWeek;
					item.start = 0;
					item.end = 23*60+59;
					timeList.push_back(item);
				}
				chkWeek++;
				if( chkWeek >= 7 ){
					chkWeek = 0;
				}
			}
		}
	}
	
	//�T�[�r�X���ƂɌ���
	for( size_t i=0; i<key->serviceList.size(); i++ ){
		map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itrService;
		itrService = this->epgMap.find(key->serviceList[i]);
		if( itrService != this->epgMap.end() ){
			//�T�[�r�X����
			map<WORD, EPGDB_EVENT_INFO*>::iterator itrEvent;
			for( itrEvent = itrService->second->eventMap.begin(); itrEvent != itrService->second->eventMap.end(); itrEvent++ ){
				wstring matchKey = L"";
				if( key->freeCAFlag == 1 ){
					//���������̂�
					if(itrEvent->second->freeCAFlag == 1 ){
						//�L������
						continue;
					}
				}else if( key->freeCAFlag == 2 ){
					//�L�������̂�
					if(itrEvent->second->freeCAFlag == 0 ){
						//��������
						continue;
					}
				}
				//�W�������m�F
				if( key->contentList.size() > 0 ){
					//�W�������w�肠��̂ŃW�������ōi�荞��
					if( itrEvent->second->contentInfo == NULL ){
						if( itrEvent->second->shortInfo == NULL ){
							//2�߂̃T�[�r�X�H�ΏۊO�Ƃ���
							continue;
						}
						//�W���������Ȃ�
						BOOL findNo = FALSE;
						for( size_t j=0; j<key->contentList.size(); j++ ){
							if( key->contentList[j].content_nibble_level_1 == 0xFF && 
								key->contentList[j].content_nibble_level_2 == 0xFF
								){
									//�W�������Ȃ��̎w�肠��
									findNo = TRUE;
									break;
							}
						}
						if( key->notContetFlag == 0 ){
							if( findNo == FALSE ){
								continue;
							}
						}else{
							//NOT��������
							if( findNo == TRUE ){
								continue;
							}
						}
					}else{
						BOOL equal = IsEqualContent(&(key->contentList), &(itrEvent->second->contentInfo->nibbleList));
						if( key->notContetFlag == 0 ){
							if( equal == FALSE ){
								//�W�������Ⴄ�̂őΏۊO
								continue;
							}
						}else{
							//NOT��������
							if( equal == TRUE ){
								continue;
							}
						}
					}
				}

				//�f���m�F
				if( key->videoList.size() > 0 ){
					if( itrEvent->second->componentInfo == NULL ){
						continue;
					}
					BOOL findContent = FALSE;
					WORD type = ((WORD)itrEvent->second->componentInfo->stream_content) << 8 | itrEvent->second->componentInfo->component_type;
					for( size_t j=0; j<key->videoList.size(); j++ ){
						if( type == key->videoList[j]){
							findContent = TRUE;
							break;
						}
					}
					if( findContent == FALSE ){
						continue;
					}
				}

				//�����m�F
				if( key->audioList.size() > 0 ){
					if( itrEvent->second->audioInfo == NULL ){
						continue;
					}
					BOOL findContent = FALSE;
					for( size_t j=0; j<itrEvent->second->audioInfo->componentList.size(); j++){
						WORD type = ((WORD)itrEvent->second->audioInfo->componentList[j].stream_content) << 8 | itrEvent->second->audioInfo->componentList[j].component_type;
						for( size_t k=0; k<key->audioList.size(); k++ ){
							if( type == key->audioList[k]){
								findContent = TRUE;
								break;
							}
						}
					}
					if( findContent == FALSE ){
						continue;
					}
				}

				//���Ԋm�F
				if( timeList.size() > 0 ){
					if( itrEvent->second->StartTimeFlag == FALSE ){
						//�J�n���ԕs���Ȃ̂őΏۊO
						continue;
					}
					BOOL inTime = IsInDateTime(&timeList, itrEvent->second->start_time);
					if( key->notDateFlag == 0 ){
						if( inTime == FALSE ){
							//���Ԕ͈͊O�Ȃ̂őΏۊO
							continue;
						}
					}else{
						//NOT��������
						if( inTime == TRUE ){
							continue;
						}
					}
				}

				//�L�[���[�h�m�F
				if( notKeyList.size() != 0 ){
					if( IsFindKeyword(key->regExpFlag, key->titleOnlyFlag, &notKeyList, itrEvent->second->shortInfo, itrEvent->second->extInfo, FALSE) == TRUE ){
						//not�L�[���[�h���������̂őΏۊO
						continue;
					}

					//if( key->regExpFlag == FALSE && key->aimaiFlag == 1){
					//	//�����܂�����
					//	if( IsFindLikeKeyword(key->titleOnlyFlag, &notKeyList, itrEvent->second->shortInfo, itrEvent->second->extInfo, FALSE) == TRUE ){
					//		//not�L�[���[�h���������̂őΏۊO
					//		continue;
					//	}
					//}else{
					//	if( IsFindKeyword(key->regExpFlag, key->titleOnlyFlag, &notKeyList, itrEvent->second->shortInfo, itrEvent->second->extInfo, FALSE) == TRUE ){
					//		//not�L�[���[�h���������̂őΏۊO
					//		continue;
					//	}
					//}
				}
				if( andKeyList.size() != 0 ){
					//if( IsFindKeyword(key->regExpFlag, key->titleOnlyFlag, &andKeyList, itrEvent->second->shortInfo, itrEvent->second->extInfo, TRUE) == FALSE ){
					//	//and�L�[���[�h������Ȃ������̂őΏۊO
					//	continue;
					//}
					if( key->regExpFlag == FALSE && key->aimaiFlag == 1){
						//�����܂�����
						if( IsFindLikeKeyword(key->titleOnlyFlag, &andKeyList, itrEvent->second->shortInfo, itrEvent->second->extInfo, TRUE, &matchKey) == FALSE ){
							//and�L�[���[�h������Ȃ������̂őΏۊO
							continue;
						}
					}else{
						if( IsFindKeyword(key->regExpFlag, key->titleOnlyFlag, &andKeyList, itrEvent->second->shortInfo, itrEvent->second->extInfo, TRUE, &matchKey) == FALSE ){
							//and�L�[���[�h������Ȃ������̂őΏۊO
							continue;
						}
					}
				}

				ULONGLONG mapKey = _Create64Key2(
					itrEvent->second->original_network_id,
					itrEvent->second->transport_stream_id,
					itrEvent->second->service_id,
					itrEvent->second->event_id);

				SEARCH_RESULT_EVENT addItem;
				addItem.findKey = matchKey;
				addItem.info = itrEvent->second;
				resultMap->insert(pair<ULONGLONG, SEARCH_RESULT_EVENT>(mapKey, addItem));

			}
		}
	}
/*
	for( itrService = this->epgMap.begin(); itrService != this->epgMap.end(); itrService++ ){
		map<WORD, EPGDB_EVENT_INFO*>::iterator itrEvent;
		for( itrEvent = itrService->second->eventMap.begin(); itrEvent != itrService->second->eventMap.end(); itrEvent++ ){
			if( itrEvent->second->shortInfo != NULL ){
				if( itrEvent->second->shortInfo->search_event_name.find(key->andKey) != string::npos ){
					ULONGLONG mapKey = _Create64Key2(
						itrEvent->second->original_network_id,
						itrEvent->second->transport_stream_id,
						itrEvent->second->service_id,
						itrEvent->second->event_id);

					resultMap->insert(pair<ULONGLONG, EPGDB_EVENT_INFO*>(mapKey, itrEvent->second));
				}
			}
		}
	}
*/
}

BOOL CEpgDBManager::IsEqualContent(vector<EPGDB_CONTENT_DATA>* searchKey, vector<EPGDB_CONTENT_DATA>* eventData)
{
	for( size_t i=0; i<searchKey->size(); i++ ){
		for( size_t j=0; j<eventData->size(); j++ ){
			if( (*searchKey)[i].content_nibble_level_1 == (*eventData)[j].content_nibble_level_1 ){
				if( (*searchKey)[i].content_nibble_level_2 != 0xFF ){
					if( (*searchKey)[i].content_nibble_level_2 == (*eventData)[j].content_nibble_level_2 ){
						return TRUE;
					}
				}else{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CEpgDBManager::IsInDateTime(vector<TIME_SEARCH>* timeList, SYSTEMTIME startTime)
{
	if( timeList == NULL ){
		return FALSE;
	}

	DWORD start = startTime.wHour*60 + startTime.wMinute;
	for( size_t i=0; i<timeList->size(); i++){
		if( (*timeList)[i].week == startTime.wDayOfWeek ){
			if( (*timeList)[i].start <= start && start <= (*timeList)[i].end ){
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CEpgDBManager::IsFindKeyword(BOOL regExpFlag, BOOL titleOnlyFlag, vector<wstring>* keyList, EPGDB_SHORT_EVENT_INFO* shortInfo, EPGDB_EXTENDED_EVENT_INFO* extInfo, BOOL andMode, wstring* findKey)
{
	if( shortInfo == NULL ){
		//��{���Ȃ��̂őΏۊO
		return FALSE;
	}

	//�����Ώۂ̕�����쐬
	wstring word = shortInfo->search_event_name;
	if( titleOnlyFlag == FALSE ){
		word += shortInfo->search_text_char;
		if( extInfo != NULL ){
			word += extInfo->search_text_char;
		}
	}

	if( regExpFlag == TRUE ){
		//���K�\�����[�h
		if( this->regExp != NULL && word.size() > 0 && keyList->size() > 0 ){
			try{
				_bstr_t target( word.c_str() );
				_bstr_t pattern( (*keyList)[0].c_str() );

				this->regExp->PutGlobal( VARIANT_TRUE );
				this->regExp->PutPattern( pattern );

				IMatchCollectionPtr pMatchCol( this->regExp->Execute( target ) );

				if( pMatchCol->Count > 0 ){
					if( findKey != NULL ){
						IMatch2Ptr pMatch( pMatchCol->Item[0] );
						_bstr_t value( pMatch->Value );

						*findKey = value;
					}
					return TRUE;
				}
			}catch(...){
			}
		}
		return FALSE;
	}else{
		//�ʏ�
		if( andMode == TRUE ){
			for( size_t i=0; i<keyList->size(); i++ ){
				if( word.find((*keyList)[i]) == string::npos ){
					//������Ȃ������̂ŏI��
					return FALSE;
				}else{
					if( findKey != NULL ){
						if( findKey->size() > 0 ){
							*findKey += L" ";
						}
						*findKey += (*keyList)[i];
					}
				}
			}
			return TRUE;
		}else{
			for( size_t i=0; i<keyList->size(); i++ ){
				if( word.find((*keyList)[i]) != string::npos ){
					//���������̂ŏI��
					return TRUE;
				}
			}
			return FALSE;
		}
	}
}

BOOL CEpgDBManager::IsFindLikeKeyword(BOOL titleOnlyFlag, vector<wstring>* keyList, EPGDB_SHORT_EVENT_INFO* shortInfo, EPGDB_EXTENDED_EVENT_INFO* extInfo, BOOL andMode, wstring* findKey)
{
	if( shortInfo == NULL ){
		//��{���Ȃ��̂őΏۊO
		return FALSE;
	}

	//�����Ώۂ̕�����쐬
	wstring word = shortInfo->search_event_name;
	if( titleOnlyFlag == FALSE ){
		word += shortInfo->search_text_char;
		if( extInfo != NULL ){
			word += extInfo->search_text_char;
		}
	}
	BOOL ret = FALSE;

	DWORD hitCount = 0;
	DWORD missCount = 0;
	for( size_t i=0; i<keyList->size(); i++ ){
		wstring key= L"";
		for( size_t j=0; j<(*keyList)[i].size(); j++ ){
			key += (*keyList)[i].at(j);
			if( word.find(key) == string::npos ){
				missCount+=1;
				key = (*keyList)[i].at(j);
				if( word.find(key) == string::npos ){
					missCount+=1;
					key = L"";
				}else{
					//hitCount+=1;
				}
			}else{
				hitCount+=(DWORD)key.size();
			}
		}
		if( andMode == FALSE ){
			DWORD totalCount = hitCount+missCount;
			DWORD per = (hitCount*100) / totalCount;
			if( per > 70 ){
				ret = TRUE;
				break;
			}
			hitCount = 0;
			missCount = 0;
		}else{
			if( findKey != NULL ){
				*findKey += (*keyList)[i];
			}
		}
	}
	if( andMode == TRUE ){
		DWORD totalCount = hitCount+missCount;
		DWORD per = (hitCount*100) / totalCount;
		if( per > 70 ){
			ret = TRUE;
		}else{
			ret = FALSE;
		}
	}
	return ret;
}

BOOL CEpgDBManager::GetServiceList(vector<EPGDB_SERVICE_INFO>* list)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = TRUE;
	map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itr;
	for( itr = this->epgMap.begin(); itr != this->epgMap.end(); itr++ ){
		list->push_back(itr->second->serviceInfo);
	}
	if( list->size() == 0 ){
		ret = FALSE;
	}

	UnLock();
	return ret;
}

BOOL CEpgDBManager::EnumEventInfo(LONGLONG serviceKey, vector<EPGDB_EVENT_INFO*>* result)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = TRUE;
	map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itr;
	itr = this->epgMap.find(serviceKey);
	if( itr != this->epgMap.end() ){
		map<WORD, EPGDB_EVENT_INFO*>::iterator itrInfo;
		for( itrInfo = itr->second->eventMap.begin(); itrInfo != itr->second->eventMap.end(); itrInfo++ ){
			result->push_back(itrInfo->second);
		}
	}
	if( result->size() == 0 ){
		ret = FALSE;
	}

	UnLock();
	return ret;
}

BOOL CEpgDBManager::EnumEventAll(vector<EPGDB_SERVICE_EVENT_INFO*>* result)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = TRUE;
	map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itr;
	for( itr = this->epgMap.begin(); itr != this->epgMap.end(); itr++ ){
		EPGDB_SERVICE_EVENT_INFO* item = new EPGDB_SERVICE_EVENT_INFO;
		item->serviceInfo = itr->second->serviceInfo;
		map<WORD, EPGDB_EVENT_INFO*>::iterator itrInfo;
		for( itrInfo = itr->second->eventMap.begin(); itrInfo != itr->second->eventMap.end(); itrInfo++ ){
			item->eventList.push_back(itrInfo->second);
		}
		result->push_back(item);
	}
	if( result->size() == 0 ){
		ret = FALSE;
	}

	UnLock();
	return ret;
}

BOOL CEpgDBManager::SearchEpg(
	WORD ONID,
	WORD TSID,
	WORD SID,
	WORD EventID,
	EPGDB_EVENT_INFO** result
	)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = FALSE;

	LONGLONG key = _Create64Key(ONID, TSID, SID);
	map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itr;
	itr = this->epgMap.find(key);
	if( itr != this->epgMap.end() ){
		map<WORD, EPGDB_EVENT_INFO*>::iterator itrInfo;
		itrInfo = itr->second->eventMap.find(EventID);
		if( itrInfo != itr->second->eventMap.end() ){
			*result = itrInfo->second;
			ret = TRUE;
		}
	}

	UnLock();
	return ret;
}

BOOL CEpgDBManager::SearchEpg(
	WORD ONID,
	WORD TSID,
	WORD SID,
	LONGLONG startTime,
	DWORD durationSec,
	EPGDB_EVENT_INFO** result
	)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = FALSE;

	LONGLONG key = _Create64Key(ONID, TSID, SID);
	map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itr;
	itr = this->epgMap.find(key);
	if( itr != this->epgMap.end() ){
		map<WORD, EPGDB_EVENT_INFO*>::iterator itrInfo;
		for( itrInfo = itr->second->eventMap.begin(); itrInfo != itr->second->eventMap.end(); itrInfo++ ){
			if( itrInfo->second->StartTimeFlag == 1 && itrInfo->second->DurationFlag == 1 ){
				if( startTime == ConvertI64Time(itrInfo->second->start_time) &&
					durationSec == itrInfo->second->durationSec
					){
						*result = itrInfo->second;
						ret = TRUE;
						break;
				}
			}
		}
	}

	UnLock();
	return ret;
}

BOOL CEpgDBManager::SearchServiceName(
	WORD ONID,
	WORD TSID,
	WORD SID,
	wstring& serviceName
	)
{
	if( Lock() == FALSE ) return FALSE;
	if( _IsLoadingData() == TRUE ){
		UnLock();
		return FALSE;
	}

	BOOL ret = FALSE;

	LONGLONG key = _Create64Key(ONID, TSID, SID);
	map<LONGLONG, EPGDB_SERVICE_DATA*>::iterator itr;
	itr = this->epgMap.find(key);
	if( itr != this->epgMap.end() ){
		serviceName = itr->second->serviceInfo.service_name;
		ret = TRUE;
	}

	UnLock();
	return ret;
}

