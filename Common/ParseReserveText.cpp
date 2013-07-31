#include "StdAfx.h"
#include "ParseReserveText.h"
#include "TimeUtil.h"
#include "StringUtil.h"

CParseReserveText::CParseReserveText(void)
{
	this->loadFilePath = L"";
	this->nextID = 1;
}

CParseReserveText::~CParseReserveText(void)
{
	multimap<wstring, RESERVE_DATA*>::iterator itr;
	for( itr = this->reserveMap.begin(); itr != this->reserveMap.end(); itr++ ){
		SAFE_DELETE(itr->second)
	}
}

BOOL CParseReserveText::ParseReserveText(LPCWSTR filePath)
{
	if( filePath == NULL ){
		return FALSE;
	}

	multimap<wstring, RESERVE_DATA*>::iterator itr;
	for( itr = this->reserveMap.begin(); itr != this->reserveMap.end(); itr++ ){
		SAFE_DELETE(itr->second)
	}
	this->reserveMap.clear();
	this->reserveIDMap.clear();
	this->nextID = 1;

	this->loadFilePath = filePath;

	HANDLE hFile = _CreateFile2( filePath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE ){
		return FALSE;
	}
	DWORD dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == 0 ){
		CloseHandle(hFile);
		return TRUE;
	}
	char* pszBuff = new char[dwFileSize+1];
	if( pszBuff == NULL ){
		CloseHandle(hFile);
		return FALSE;
	}
	ZeroMemory(pszBuff,dwFileSize+1);
	DWORD dwRead=0;
	ReadFile( hFile, pszBuff, dwFileSize, &dwRead, NULL );

	string strRead = pszBuff;

	CloseHandle(hFile);
	SAFE_DELETE_ARRAY(pszBuff)

	string parseLine="";
	size_t iIndex = 0;
	size_t iFind = 0;
	while( iFind != string::npos ){
		iFind = strRead.find("\r\n", iIndex);
		if( iFind == (int)string::npos ){
			parseLine = strRead.substr(iIndex);
			//strRead.clear();
		}else{
			parseLine = strRead.substr(iIndex,iFind-iIndex);
			//strRead.erase( 0, iIndex+2 );
			iIndex = iFind + 2;
		}
		//�擪�G�̓R�����g�s
		if( parseLine.find(";") != 0 ){
			//��s�H
			if( parseLine.find("\t") != string::npos ){
				RESERVE_DATA* item = new RESERVE_DATA;
				BOOL bRet = Parse1Line(parseLine, item);
				if( bRet == FALSE ){
					SAFE_DELETE(item)
				}else{
					wstring strKey;
					Format(strKey, L"%04d%02d%02d%02d%02d%02d%04X",
							item->startTime.wYear,
							item->startTime.wMonth,
							item->startTime.wDay,
							item->startTime.wHour,
							item->startTime.wMinute,
							item->startTime.wSecond,
							item->transportStreamID);

					item->reserveID = GetNextReserveID();
					this->reserveMap.insert( pair<wstring, RESERVE_DATA*>(strKey,item) );
					this->reserveIDMap.insert( pair<DWORD, RESERVE_DATA*>(item->reserveID,item) );
				}
			}
		}
	}
	SaveReserveText();

	return TRUE;
}

BOOL CParseReserveText::AddParseReserveText(LPCWSTR filePath)
{
	if( this->loadFilePath.empty() == TRUE && filePath == NULL ){
		return FALSE;
	}
	wstring load_filePath = L"";
	if( filePath != NULL ){
		load_filePath = filePath;
	}else{
		load_filePath = this->loadFilePath;
	}
	if( this->reserveMap.size() == 0 ){
		BOOL bRet = ParseReserveText(load_filePath.c_str());
		return bRet;
	}

	HANDLE hFile = CreateFileW( load_filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE ){
		return FALSE;
	}
	DWORD dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == 0 ){
		CloseHandle(hFile);
		return TRUE;
	}
	char* pszBuff = new char[dwFileSize+1];
	if( pszBuff == NULL ){
		CloseHandle(hFile);
		return FALSE;
	}
	ZeroMemory(pszBuff,dwFileSize+1);
	DWORD dwRead=0;
	ReadFile( hFile, pszBuff, dwFileSize, &dwRead, NULL );

	string strRead = pszBuff;

	CloseHandle(hFile);
	SAFE_DELETE_ARRAY(pszBuff)

	string parseLine="";
	size_t iIndex = 0;
	size_t iFind = 0;
	while( iFind != string::npos ){
		iFind = strRead.find("\r\n", iIndex);
		if( iFind == (int)string::npos ){
			parseLine = strRead.substr(iIndex);
			//strRead.clear();
		}else{
			parseLine = strRead.substr(iIndex,iFind-iIndex);
			//strRead.erase( 0, iIndex+2 );
			iIndex = iFind + 2;
		}
		//�擪�G�̓R�����g�s
		if( parseLine.find(";") != 0 ){
			//��s�H
			if( parseLine.find("\t") != string::npos ){
				RESERVE_DATA* item = new RESERVE_DATA;
				BOOL bRet = Parse1Line(parseLine, item);
				if( bRet == FALSE ){
					SAFE_DELETE(item)
				}else{
					wstring strKey;
					Format(strKey, L"%04d%02d%02d%02d%02d%02d%04X",
							item->startTime.wYear,
							item->startTime.wMonth,
							item->startTime.wDay,
							item->startTime.wHour,
							item->startTime.wMinute,
							item->startTime.wSecond,
							item->transportStreamID);

					if( item->reserveID == 0 ){
						item->reserveID = GetNextReserveID();
						this->reserveMap.insert( pair<wstring, RESERVE_DATA*>(strKey,item) );
						this->reserveIDMap.insert( pair<DWORD, RESERVE_DATA*>(item->reserveID,item) );
					}else{
						SAFE_DELETE(item);
					}
				}
			}
		}
	}
	SaveReserveText();

	return TRUE;
}

BOOL CParseReserveText::Parse1Line(string parseLine, RESERVE_DATA* item )
{
	if( parseLine.empty() == true || item == NULL ){
		return FALSE;
	}
	string strBuff="";

	Separate( parseLine, "\t", strBuff, parseLine);

	//���t����
	string strY;
	string strM;

	Separate( strBuff, "/", strY, strBuff);
	if( strY.empty() == false ){
		Separate( strBuff, "/", strM, strBuff);
		if( strM.empty() == false ){
			item->startTime.wYear = atoi(strY.c_str());
			item->startTime.wMonth = atoi(strM.c_str());
			item->startTime.wDay = atoi(strBuff.c_str());
		}else{
			return FALSE;
		}
	}else{
		return FALSE;
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//���ԕ���
	string strHour;
	string strMinute;

	Separate( strBuff, ":", strHour, strBuff);
	if( strHour.empty() == true ){
		return FALSE;
	}
	Separate( strBuff, ":", strMinute, strBuff);
	if( strHour.empty() == true ){
		return FALSE;
	}

	item->startTime.wHour = atoi(strHour.c_str());
	item->startTime.wMinute = atoi(strMinute.c_str());
	item->startTime.wSecond = atoi(strBuff.c_str());
	item->startTime.wMilliseconds = 0;

	Separate( parseLine, "\t", strBuff, parseLine);

	//������
	Separate( strBuff, ":", strHour, strBuff);
	if( strHour.empty() == true ){
		return FALSE;
	}
	Separate( strBuff, ":", strMinute, strBuff);
	if( strHour.empty() == true ){
		return FALSE;
	}

	item->durationSecond = atoi(strHour.c_str())*60*60 + atoi(strMinute.c_str())*60 + atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�^�C�g��
	AtoW(strBuff, item->title);

	Separate( parseLine, "\t", strBuff, parseLine);

	//�����ǖ�
	AtoW(strBuff, item->stationName);

	Separate( parseLine, "\t", strBuff, parseLine);

	//ONID
	item->originalNetworkID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//TSID
	item->transportStreamID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);
	
	//�T�[�r�X�h�c
	item->serviceID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�C�x���g�h�c
	item->eventID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);


	//�D��x
	item->recSetting.priority = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�Ǐ]
	item->recSetting.tuijyuuFlag = (BYTE)atoi(strBuff.c_str());
	if( item->eventID == 0xFFFF ){
		item->recSetting.tuijyuuFlag = 0;
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//ID
	item->reserveID = (DWORD)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�^�惂�[�h
	item->recSetting.recMode = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);
	
	//�����p1�@�҂�����^��
	item->recSetting.pittariFlag = (BYTE)atoi(strBuff.c_str());
	if( item->eventID == 0xFFFF ){
		item->recSetting.pittariFlag = 0;
	}
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�����p2�@Bat����
	if( strBuff.compare("0") == 0 ){
		item->recSetting.batFilePath = L"";
	}else{
		AtoW(strBuff, item->recSetting.batFilePath);
	}

	Separate( parseLine, "\t", strBuff, parseLine);
	//�����p3

	Separate( parseLine, "\t", strBuff, parseLine);

	//�R�����g����
	if( strBuff.length() > 2 ){
		AtoW(strBuff, item->comment);
	}else{
		item->comment = L"";
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//�^��t�H���_
	wstring wBuff = L"";
	AtoW(strBuff, wBuff);
	if( wBuff.size() > 0 ){
		wstring folder = L"";
		wstring plugin = L"";
		wstring recname = L"";
		Separate( wBuff, L"*", folder, wBuff);
		Separate( wBuff, L"*", plugin, recname);

		REC_FILE_SET_INFO folderItem;
		folderItem.recFolder = folder;
		ChkFolderPath(folderItem.recFolder);
		if( plugin.size() == 0 ){
			folderItem.writePlugIn = L"Write_Default.dll";
		}else{
			folderItem.writePlugIn = plugin;
		}
		folderItem.recNamePlugIn = recname;
		item->recSetting.recFolderList.push_back(folderItem);
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//�T�X�y���h
	if( strBuff.empty() == true ){
		item->recSetting.suspendMode = 4;
	}else{
		item->recSetting.suspendMode = (BYTE)atoi(strBuff.c_str());
	}
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�ċN��
	item->recSetting.rebootFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�^��ς݃t�@�C��
	AtoW(strBuff, item->recFilePath);
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�ʃ}�[�W��
	item->recSetting.useMargineFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�J�n�}�[�W��
	item->recSetting.startMargine = (INT)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�I���}�[�W��
	item->recSetting.endMargine = (INT)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�w��T�[�r�X���[�h
	item->recSetting.serviceMode = (DWORD)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�\��o�^���̊J�n����
	//���t����
	Separate( strBuff, "/", strY, strBuff);
	if( strY.empty() == false ){
		Separate( strBuff, "/", strM, strBuff);
		if( strM.empty() == false ){
			item->startTimeEpg.wYear = atoi(strY.c_str());
			item->startTimeEpg.wMonth = atoi(strM.c_str());
			item->startTimeEpg.wDay = atoi(strBuff.c_str());
		}else{
			return FALSE;
		}
	}else{
		return FALSE;
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//���ԕ���
	Separate( strBuff, ":", strHour, strBuff);
	if( strHour.empty() == true ){
		return FALSE;
	}
	Separate( strBuff, ":", strMinute, strBuff);
	if( strHour.empty() == true ){
		return FALSE;
	}

	item->startTimeEpg.wHour = atoi(strHour.c_str());
	item->startTimeEpg.wMinute = atoi(strMinute.c_str());
	item->startTimeEpg.wSecond = atoi(strBuff.c_str());
	item->startTimeEpg.wMilliseconds = 0;

	Separate( parseLine, "\t", strBuff, parseLine);

	//�����^��t�H���_
	DWORD recFolderNum = (DWORD)atoi(strBuff.c_str());

	for( DWORD i=0; i<recFolderNum; i++ ){
		Separate( parseLine, "\t", strBuff, parseLine);

		wstring wBuff = L"";
		AtoW(strBuff, wBuff);
		if( wBuff.size() > 0 ){
			wstring folder = L"";
			wstring plugin = L"";
			wstring recname = L"";
			Separate( wBuff, L"*", folder, wBuff);
			Separate( wBuff, L"*", plugin, recname);

			REC_FILE_SET_INFO folderItem;
			folderItem.recFolder = folder;
			ChkFolderPath(folderItem.recFolder);
			if( plugin.size() == 0 ){
				folderItem.writePlugIn = L"Write_Default.dll";
			}else{
				folderItem.writePlugIn = plugin;
			}
			folderItem.recNamePlugIn = recname;
			item->recSetting.recFolderList.push_back(folderItem);
		}
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//�A���^��
	item->recSetting.continueRecFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//������M����
	item->recSetting.partialRecFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�����I�g�pBonDrive
	item->recSetting.tunerID = (DWORD)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�\��ǉ����
	item->reserveStatus = (DWORD)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);
	
	//������M�����^��t�H���_
	DWORD recFolderNum2 = (DWORD)atoi(strBuff.c_str());

	for( DWORD i=0; i<recFolderNum2; i++ ){
		Separate( parseLine, "\t", strBuff, parseLine);

		wstring wBuff = L"";
		AtoW(strBuff, wBuff);
		if( wBuff.size() > 0 ){
			wstring folder = L"";
			wstring plugin = L"";
			wstring recname = L"";
			Separate( wBuff, L"*", folder, wBuff);
			Separate( wBuff, L"*", plugin, recname);

			REC_FILE_SET_INFO folderItem;
			folderItem.recFolder = folder;
			ChkFolderPath(folderItem.recFolder);
			if( plugin.size() == 0 ){
				folderItem.writePlugIn = L"Write_Default.dll";
			}else{
				folderItem.writePlugIn = plugin;
			}
			folderItem.recNamePlugIn = recname;
			item->recSetting.partialRecFolder.push_back(folderItem);
		}
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	item->recWaitFlag = FALSE;
	item->overlapMode = 0;

	return TRUE;
}

DWORD CParseReserveText::GetNextReserveID()
{
	DWORD reserveID = 0xFFFFFFFF;

	map<DWORD, RESERVE_DATA*>::iterator itr;
	itr = this->reserveIDMap.find(this->nextID);
	if( itr == this->reserveIDMap.end() ){
		reserveID = this->nextID;
		this->nextID++;
		if( this->nextID == 0 || this->nextID == 0xFFFFFFFF){
			this->nextID = 1;
		}
	}else{
		for( DWORD i=1; i<0xFFFFFFFF; i++ ){
			itr = this->reserveIDMap.find(this->nextID);
			if( itr == this->reserveIDMap.end() ){
				reserveID = this->nextID;
				this->nextID++;
				if( this->nextID == 0 || this->nextID == 0xFFFFFFFF){
					this->nextID = 1;
				}
				break;
			}else{
				this->nextID++;
			}
			if( this->nextID == 0 || this->nextID == 0xFFFFFFFF){
				this->nextID = 1;
			}
		}
	}

	return reserveID;
}

BOOL CParseReserveText::SaveReserveText(LPCWSTR filePath)
{
	if( this->loadFilePath.empty() == TRUE && filePath == NULL ){
		return FALSE;
	}

	wstring save_filePath = L"";
	if( filePath != NULL ){
		save_filePath = filePath;
	}else{
		save_filePath = this->loadFilePath;
	}

	HANDLE hFile = _CreateFile2( save_filePath.c_str(), GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE ){
		return FALSE;
	}

	multimap<wstring, RESERVE_DATA*>::iterator itr;
	for( itr=this->reserveMap.begin(); itr != this->reserveMap.end(); itr++ ){
		string strWrite="";
		string strBuff="";
		DWORD dwWrite;
		//���t
		Format(strBuff, "%04d/%02d/%02d",itr->second->startTime.wYear,itr->second->startTime.wMonth,itr->second->startTime.wDay);
		strWrite+=strBuff +"\t";
		//����
		Format(strBuff,"%02d:%02d:%02d",itr->second->startTime.wHour,itr->second->startTime.wMinute,itr->second->startTime.wSecond);
		strWrite+=strBuff +"\t";
		//������
		Format(strBuff,"%02d:%02d:%02d",itr->second->durationSecond/(60*60),(itr->second->durationSecond%(60*60))/60,itr->second->durationSecond%60);
		strWrite+=strBuff +"\t";
		//�^�C�g��
		WtoA(itr->second->title, strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//������
		WtoA(itr->second->stationName, strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//ONID
		Format(strBuff, "%d",itr->second->originalNetworkID);
		strWrite+=strBuff +"\t";
		//TSID
		Format(strBuff,"%d",itr->second->transportStreamID);
		strWrite+=strBuff +"\t";
		//ServiceID
		Format(strBuff,"%d",itr->second->serviceID);
		strWrite+=strBuff +"\t";
		//EventID
		Format(strBuff,"%d",itr->second->eventID);
		strWrite+=strBuff +"\t";
		//�D��x
		Format(strBuff,"%d",itr->second->recSetting.priority);
		strWrite+=strBuff +"\t";
		//�Ǐ]
		Format(strBuff,"%d",itr->second->recSetting.tuijyuuFlag);
		strWrite+=strBuff +"\t";
		//ID
		Format(strBuff,"%d",itr->second->reserveID);
		strWrite+=strBuff +"\t";
		//�^�惂�[�h
		Format(strBuff,"%d",itr->second->recSetting.recMode);
		strWrite+=strBuff +"\t";
		//�����p1 �҂�����^��
		Format(strBuff,"%d",itr->second->recSetting.pittariFlag);
		strWrite+=strBuff +"\t";
		//�����p2 Bat����
		if( itr->second->recSetting.batFilePath.empty() == true ){
			strWrite+="0\t";
		}else{
			WtoA(itr->second->recSetting.batFilePath,strBuff);
			strWrite+=strBuff;
			strWrite+="\t";
		}
		//�����p3
		strWrite+="0\t";
		//�R�����g
		if( itr->second->comment.length() > 0 ){
			WtoA(itr->second->comment,strBuff);
			strWrite+=strBuff;
		}
		strWrite+="\t";
		//�^��t�H���_
		if( itr->second->recSetting.recFolderList.size() > 0 ){
			wstring path = L"";
			path = itr->second->recSetting.recFolderList[0].recFolder;
			path += L"*";
			path += itr->second->recSetting.recFolderList[0].writePlugIn;
			path += L"*";
			path += itr->second->recSetting.recFolderList[0].recNamePlugIn;
			WtoA(path, strBuff);
			strWrite+=strBuff;
		}
		strWrite+="\t";
		//�T�X�y���h
		Format(strBuff,"%d",itr->second->recSetting.suspendMode);
		strWrite+=strBuff +"\t";
		//�ċN��
		Format(strBuff,"%d",itr->second->recSetting.rebootFlag);
		strWrite+=strBuff +"\t";
		//�^��σt�@�C���p�X
		WtoA(itr->second->recFilePath, strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//�ʃ}�[�W��
		Format(strBuff,"%d",itr->second->recSetting.useMargineFlag);
		strWrite+=strBuff +"\t";
		//�J�n�}�[�W��
		Format(strBuff,"%d",itr->second->recSetting.startMargine);
		strWrite+=strBuff +"\t";
		//�I���}�[�W��
		Format(strBuff,"%d",itr->second->recSetting.endMargine);
		strWrite+=strBuff +"\t";
		//�w��T�[�r�X���[�h
		Format(strBuff,"%d",itr->second->recSetting.serviceMode);
		strWrite+=strBuff +"\t";
		//�\��o�^���̊J�n����
		//���t
		Format(strBuff, "%04d/%02d/%02d",itr->second->startTimeEpg.wYear,itr->second->startTimeEpg.wMonth,itr->second->startTimeEpg.wDay);
		strWrite+=strBuff +"\t";
		//����
		Format(strBuff,"%02d:%02d:%02d",itr->second->startTimeEpg.wHour,itr->second->startTimeEpg.wMinute,itr->second->startTimeEpg.wSecond);
		strWrite+=strBuff +"\t";
		//�����^��t�H���_
		if( itr->second->recSetting.recFolderList.size() > 1 ){
			Format(strBuff,"%d",itr->second->recSetting.recFolderList.size()-1);
			strWrite+=strBuff +"\t";
			for( size_t i=1; i<itr->second->recSetting.recFolderList.size(); i++ ){
				wstring path = L"";
				path = itr->second->recSetting.recFolderList[i].recFolder;
				path += L"*";
				path += itr->second->recSetting.recFolderList[i].writePlugIn;
				path += L"*";
				path += itr->second->recSetting.recFolderList[i].recNamePlugIn;
				WtoA(path, strBuff);
				strWrite+=strBuff +"\t";
			}
		}else{
			strWrite+="0\t";
		}
		//�A���^��
		Format(strBuff,"%d",itr->second->recSetting.continueRecFlag);
		strWrite+=strBuff +"\t";
		//������M����
		Format(strBuff,"%d",itr->second->recSetting.partialRecFlag);
		strWrite+=strBuff +"\t";
		//�����I�g�pBonDrive
		Format(strBuff,"%d",itr->second->recSetting.tunerID);
		strWrite+=strBuff +"\t";
		//�\��ǉ����
		Format(strBuff,"%d",itr->second->reserveStatus);
		strWrite+=strBuff +"\t";
		//������M�T�[�r�X�^��̃t�H���_
		if( itr->second->recSetting.partialRecFolder.size() > 0 ){
			Format(strBuff,"%d",itr->second->recSetting.partialRecFolder.size());
			strWrite+=strBuff +"\t";
			for( size_t i=0; i<itr->second->recSetting.partialRecFolder.size(); i++ ){
				wstring path = L"";
				path = itr->second->recSetting.partialRecFolder[i].recFolder;
				path += L"*";
				path += itr->second->recSetting.partialRecFolder[i].writePlugIn;
				path += L"*";
				path += itr->second->recSetting.partialRecFolder[i].recNamePlugIn;
				WtoA(path, strBuff);
				strWrite+=strBuff +"\t";
			}
		}else{
			strWrite+="0\t";
		}
		strWrite+="\r\n";
		WriteFile(hFile, strWrite.c_str(), (DWORD)strWrite.length(), &dwWrite, NULL);
	}

	CloseHandle(hFile);

	return TRUE;
}

BOOL CParseReserveText::AddReserve(RESERVE_DATA* item, DWORD* reserveID)
{
	if( item == NULL ){
		return FALSE;
	}
	SYSTEMTIME Now;
	GetLocalTime(&Now);

	SYSTEMTIME EndTime;
	GetI64Time(item->startTime, item->durationSecond, NULL, NULL, &EndTime);
	if( IsBigTime(Now, EndTime ) == TRUE){
		return FALSE;
	}

	wstring strKey;
	Format(strKey, L"%04d%02d%02d%02d%02d%02d%04X",
		item->startTime.wYear,
		item->startTime.wMonth,
		item->startTime.wDay,
		item->startTime.wHour,
		item->startTime.wMinute,
		item->startTime.wSecond,
		item->transportStreamID);

	RESERVE_DATA* pSetItem = new RESERVE_DATA;
	*pSetItem = *item;

	pSetItem->reserveID = GetNextReserveID();
	if( reserveID != NULL ){
		*reserveID = pSetItem->reserveID;
	}
	this->reserveMap.insert( pair<wstring, RESERVE_DATA*>(strKey,pSetItem) );
	this->reserveIDMap.insert( pair<DWORD, RESERVE_DATA*>(pSetItem->reserveID,pSetItem));
	return TRUE;
}

BOOL CParseReserveText::ChgReserve(RESERVE_DATA* item)
{
	if( item == NULL ){
		return FALSE;
	}

	map<DWORD, RESERVE_DATA*>::iterator itr;
	itr = this->reserveIDMap.find(item->reserveID);
	if( itr == this->reserveIDMap.end() ){
		return FALSE;
	}
	*(itr->second) = *item;

	multimap<wstring, RESERVE_DATA*>::iterator itr2;
	for( itr2 = this->reserveMap.begin(); itr2 != this->reserveMap.end(); itr2++ ){
		if( itr2->second->reserveID == item->reserveID ){
			this->reserveMap.erase(itr2);

			wstring strKey;
			Format(strKey, L"%04d%02d%02d%02d%02d%02d%04X",
				item->startTime.wYear,
				item->startTime.wMonth,
				item->startTime.wDay,
				item->startTime.wHour,
				item->startTime.wMinute,
				item->startTime.wSecond,
				item->transportStreamID);

			this->reserveMap.insert( pair<wstring, RESERVE_DATA*>(strKey,itr->second) );

			break;
		}
	}

	return TRUE;
}

BOOL CParseReserveText::DelReserve(DWORD reserveID)
{
	multimap<wstring, RESERVE_DATA*>::iterator itr;
	for( itr = this->reserveMap.begin(); itr != this->reserveMap.end(); itr++ ){
		if( itr->second->reserveID == reserveID ){
			SAFE_DELETE(itr->second);
			this->reserveMap.erase(itr);
			break;
		}
	}
	map<DWORD, RESERVE_DATA*>::iterator itr2;
	itr2 = this->reserveIDMap.find(reserveID);
	if( itr2 != this->reserveIDMap.end() ){
		this->reserveIDMap.erase(itr2);
	}

	return TRUE;
}

void CParseReserveText::SwapMap()
{
	multimap<wstring, RESERVE_DATA*>(this->reserveMap).swap(this->reserveMap);
	map<DWORD, RESERVE_DATA*>(this->reserveIDMap).swap(this->reserveIDMap);
}

