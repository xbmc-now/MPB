#include "stdafx.h"
#include "BonCtrl.h"
#include <process.h>

#include "../Common/ErrDef.h"
#include "../Common/TimeUtil.h"
#include "../Common/SendCtrlCmd.h"

CBonCtrl::CBonCtrl(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);
	this->buffLockEvent = _CreateEvent(FALSE, TRUE, NULL);

    this->recvThread = NULL;
    this->recvStopEvent = _CreateEvent(FALSE, FALSE, NULL);
    this->analyzeThread = NULL;
    this->analyzeStopEvent = _CreateEvent(FALSE, FALSE, NULL);

    this->chScanThread = NULL;
    this->chScanStopEvent = _CreateEvent(FALSE, FALSE, NULL);
	this->chSt_space = 0;
	this->chSt_ch = 0;
	this->chSt_chName = L"";
	this->chSt_chkNum = 0;
	this->chSt_totalNum = 0;
	this->chSt_err = ST_STOP;

	this->epgCapThread = NULL;
	this->epgCapStopEvent = _CreateEvent(FALSE, FALSE, NULL);
	this->BSBasic = TRUE;
	this->CS1Basic = TRUE;
	this->CS2Basic = TRUE;
	this->epgSt_err = ST_STOP;

	this->epgCapBackThread = NULL;
	this->epgCapBackStopEvent = _CreateEvent(FALSE, FALSE, NULL);
	this->enableLiveEpgCap = FALSE;
	this->enableRecEpgCap = FALSE;

	this->epgCapBackStartWaitSec = 30;
	this->tsBuffMaxCount = 5000;
	this->writeBuffMaxCount = -1;
}


CBonCtrl::~CBonCtrl(void)
{
	_CloseBonDriver();

	if( this->chScanThread != NULL ){
		::SetEvent(this->chScanStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->chScanThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->chScanThread, 0xffffffff);
		}
		CloseHandle(this->chScanThread);
		this->chScanThread = NULL;
	}
	if( this->chScanStopEvent != NULL ){
		CloseHandle(this->chScanStopEvent);
		this->chScanStopEvent = NULL;
	}

	if( this->epgCapThread != NULL ){
		::SetEvent(this->epgCapStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->epgCapThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->epgCapThread, 0xffffffff);
		}
		CloseHandle(this->epgCapThread);
		this->epgCapThread = NULL;
	}
	if( this->epgCapStopEvent != NULL ){
		CloseHandle(this->epgCapStopEvent);
		this->epgCapStopEvent = NULL;
	}

	if( this->epgCapBackThread != NULL ){
		::SetEvent(this->epgCapBackStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->epgCapBackThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->epgCapBackThread, 0xffffffff);
		}
		CloseHandle(this->epgCapBackThread);
		this->epgCapBackThread = NULL;
	}
	if( this->epgCapBackStopEvent != NULL ){
		CloseHandle(this->epgCapBackStopEvent);
		this->epgCapBackStopEvent = NULL;
	}

	if( this->analyzeThread != NULL ){
		::SetEvent(this->analyzeStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->analyzeThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->analyzeThread, 0xffffffff);
		}
		CloseHandle(this->analyzeThread);
		this->analyzeThread = NULL;
	}
	if( this->analyzeStopEvent != NULL ){
		CloseHandle(this->analyzeStopEvent);
		this->analyzeStopEvent = NULL;
	}

	if( this->recvThread != NULL ){
		::SetEvent(this->recvStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->recvThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->recvThread, 0xffffffff);
		}
		CloseHandle(this->recvThread);
		this->recvThread = NULL;
	}
	if( this->recvStopEvent != NULL ){
		CloseHandle(this->recvStopEvent);
		this->recvStopEvent = NULL;
	}
	
	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
	if( this->buffLockEvent != NULL ){
		CloseHandle(this->buffLockEvent);
		this->buffLockEvent = NULL;
	}
}

BOOL CBonCtrl::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	//if( log != NULL ){
	//	OutputDebugString(log);
	//}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED ||
		dwRet == WAIT_TIMEOUT){
			if( log != NULL ){
				_OutputDebugString(L"��CBonCtrl::Lock FALSE : %s", log);
			}else{
				OutputDebugString(L"��CBonCtrl::Lock FALSE");
			}
		return FALSE;
	}
	return TRUE;
}

void CBonCtrl::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

//�����ݒ�
//�ݒ�t�@�C���ۑ����BonDriver�t�H���_���w��
//�����F
// settingFolderPath		[IN]�ݒ�t�@�C���ۑ��t�H���_�p�X
// bonDriverFolderPath		[IN]BonDriver�t�H���_�p�X
void CBonCtrl::SetSettingFolder(
	LPCWSTR settingFolderPath,
	LPCWSTR bonDriverFolderPath
)
{
	if( Lock(L"SetSettingFolder") == FALSE ) return ;
	this->bonUtil.SetSettingFolder(settingFolderPath, bonDriverFolderPath);

	if( this->bonUtil.GetOpenBonDriverIndex() != -1 ){
		this->chUtil.LoadChSet( this->bonUtil.GetChSet4Path(), this->bonUtil.GetChSet5Path() );
	}

	UnLock();
}

void CBonCtrl::SetEMMMode(BOOL enable)
{
	if( Lock(L"SetEMMMode") == FALSE ) return ;

	this->tsOut.SetEmm(enable);

	UnLock();
}

void CBonCtrl::SetTsBuffMaxCount(DWORD tsBuffMaxCount, int writeBuffMaxCount)
{
	if( Lock(L"SetTsBuffMaxCount") == FALSE ) return ;

	this->tsBuffMaxCount = tsBuffMaxCount;
	this->writeBuffMaxCount = writeBuffMaxCount;

	UnLock();
}

//BonDriver�t�H���_��BonDriver_*.dll���
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonList			[OUT]�����ł���BonDriver�ꗗ�imap�̃L�[ �����C���f�b�N�X�l�Amap�̒l BonDriver�t�@�C�����j
DWORD CBonCtrl::EnumBonDriver(
	map<int, wstring>* bonList
)
{
	if( Lock(L"EnumBonDriver") == FALSE ) return ERR_FALSE;
	DWORD ret = this->bonUtil.EnumBonDriver(bonList);
	UnLock();
	return ret;
}

//BonDriver�̃��[�h
//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�C���f�b�N�X�l�Ŏw��j
//�߂�l�F
// �G���[�R�[�h
//�����F
// iIndex			[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃C���f�b�N�X�l
DWORD CBonCtrl::OpenBonDriver(
	int index,
	int openWait
)
{
	if( Lock(L"OpenBonDriver") == FALSE ) return ERR_FALSE;
	_CloseBonDriver();
	DWORD ret = this->bonUtil.OpenBonDriver(index, openWait);
	if( ret == NO_ERR ){
		ret = _OpenBonDriver();
		this->tsOut.ResetChChange();

		wstring bonFile;
		bonFile = this->bonUtil.GetOpenBonDriverFileName();
		this->tsOut.SetBonDriver(bonFile);
	}
	if( this->bonUtil.GetOpenBonDriverIndex() != -1 ){
		this->chUtil.LoadChSet( this->bonUtil.GetChSet4Path(), this->bonUtil.GetChSet5Path() );
	}

	UnLock();
	return ret;
}

//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�t�@�C�����Ŏw��j
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonDriverFile	[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃t�@�C����
DWORD CBonCtrl::OpenBonDriver(
	LPCWSTR bonDriverFile,
	int openWait
)
{
	if( Lock(L"OpenBonDriver-2") == FALSE ) return ERR_FALSE;
	_CloseBonDriver();
	DWORD ret = this->bonUtil.OpenBonDriver(bonDriverFile, openWait);
	if( ret == NO_ERR ){
		ret = _OpenBonDriver();
		this->tsOut.ResetChChange();

		wstring bonFile;
		bonFile = this->bonUtil.GetOpenBonDriverFileName();
		this->tsOut.SetBonDriver(bonFile);
	}
	if( this->bonUtil.GetOpenBonDriverIndex() != -1 ){
		this->chUtil.LoadChSet( this->bonUtil.GetChSet4Path(), this->bonUtil.GetChSet5Path() );
	}

	UnLock();
	return ret;
}

//���[�h���Ă���BonDriver�̊J��
//�߂�l�F
// �G���[�R�[�h
DWORD CBonCtrl::CloseBonDriver()
{
	if( Lock(L"CloseBonDriver") == FALSE ) return ERR_FALSE;
	StopBackgroundEpgCap();
	DWORD ret = _CloseBonDriver();
	UnLock();
	return ret;
}

//���[�h����BonDriver�̃t�@�C�������擾����i���[�h�������Ă��邩�̔���j
//�߂�l�F
// TRUE�i�����j�FFALSE�iOpen�Ɏ��s���Ă���j
//�����F
// bonDriverFile		[OUT]BonDriver�̃t�@�C����(NULL��)
BOOL CBonCtrl::GetOpenBonDriver(
	wstring* bonDriverFile
	)
{
	if( Lock(L"GetOpenBonDriver") == FALSE ) return ERR_FALSE;

	BOOL ret = FALSE;

	if( this->bonUtil.GetOpenBonDriverIndex() != -1 ){
		ret = TRUE;
		if( bonDriverFile != NULL ){
			*bonDriverFile = this->bonUtil.GetOpenBonDriverFileName();
		}
	}

	UnLock();

	return ret;
}
//���[�h����BonDriver�̏��擾
//Space��Ch�̈ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// spaceMap			[OUT] Space��Ch�̈ꗗ�imap�̃L�[ Space�j
DWORD CBonCtrl::GetOriginalChList(
	map<DWORD, BON_SPACE_INFO>* spaceMap
)
{
	if( Lock(L"GetOriginalChList") == FALSE ) return ERR_FALSE;
	DWORD ret = this->bonUtil.GetOriginalChList(spaceMap);
	UnLock();
	return ret;
}

//BonDriver�̃`���[�i�[�����擾
//�߂�l�F
// �`���[�i�[��
wstring CBonCtrl::GetTunerName()
{
	if( Lock(L"GetTunerName") == FALSE ) return L"err";
	wstring ret = this->bonUtil.GetTunerName();
	UnLock();
	return ret;
}

//�`�����l���ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// space			[IN]�ύX�`�����l����Space
// ch				[IN]�ύX�`�����l���̕���Ch
DWORD CBonCtrl::SetCh(
	DWORD space,
	DWORD ch
)
{
	if( Lock(L"SetCh") == FALSE ) return ERR_FALSE;

	if( this->tsOut.IsRec() == TRUE ){
		UnLock();
		return ERR_FALSE;
	}

	DWORD ret = _SetCh(space, ch);

	UnLock();
	return ret;
}

//�`�����l���ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// space			[IN]�ύX�`�����l����Space
// ch				[IN]�ύX�`�����l���̕���Ch
// SID			[IN]�ύX�`�����l���̕���service_id
DWORD CBonCtrl::SetCh(
	DWORD space,
	DWORD ch,
	WORD SID
)
{
	if( Lock(L"SetCh-2") == FALSE ) return ERR_FALSE;

	if( this->tsOut.IsRec() == TRUE ){
		UnLock();
		return ERR_FALSE;
	}

	DWORD ret = ERR_FALSE;
	ret = _SetCh(space, ch);
	this->lastSID = SID;

	UnLock();
	return ret;
}

//�`�����l���ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// ONID			[IN]�ύX�`�����l����orignal_network_id
// TSID			[IN]�ύX�`�����l���̕���transport_stream_id
// SID			[IN]�ύX�`�����l���̕���service_id
DWORD CBonCtrl::SetCh(
	WORD ONID,
	WORD TSID,
	WORD SID
)
{
	if( Lock(L"SetCh-3") == FALSE ) return ERR_FALSE;

	if( this->tsOut.IsRec() == TRUE ){
		UnLock();
		return ERR_FALSE;
	}

	DWORD space=0;
	DWORD ch=0;

	DWORD ret = ERR_FALSE;
	if( this->chUtil.GetCh( ONID, TSID, space, ch ) == TRUE ){
		ret = _SetCh(space, ch);
		this->lastSID = SID;
	}

	UnLock();
	return ret;
}

DWORD CBonCtrl::_SetCh(
	DWORD space,
	DWORD ch,
	BOOL chScan
	)
{
	DWORD spaceNow=0;
	DWORD chNow=0;

	DWORD ret = ERR_FALSE;
	if( this->bonUtil.GetNowCh(&spaceNow, &chNow) == TRUE ){
		ret = NO_ERR;
		if( space != spaceNow || ch != chNow ){
			this->tsOut.SetChChangeEvent(chScan);
			_OutputDebugString(L"SetCh space %d, ch %d", space, ch);
			ret = this->bonUtil.SetCh(space, ch);

			StartBackgroundEpgCap();
		}else{
			BOOL chChgErr = FALSE;
			if( this->tsOut.IsChChanging(&chChgErr) == TRUE ){
				if( chChgErr == TRUE ){
					//�G���[�̎��͍Đݒ�
					this->tsOut.SetChChangeEvent();
					_OutputDebugString(L"SetCh space %d, ch %d", space, ch);
					ret = this->bonUtil.SetCh(space, ch);

					StartBackgroundEpgCap();
				}
			}else{
				if( chChgErr == TRUE ){
					//�G���[�̎��͍Đݒ�
					this->tsOut.SetChChangeEvent();
					_OutputDebugString(L"SetCh space %d, ch %d", space, ch);
					ret = this->bonUtil.SetCh(space, ch);

					StartBackgroundEpgCap();
				}
			}
		}
	}else{
		OutputDebugString(L"Err GetNowCh");
	}
	return ret;
}

BOOL CBonCtrl::GetCh(
	DWORD* space,
	DWORD* ch
	)
{
	if( Lock(L"GetCh") == FALSE ) return FALSE;
	BOOL ret = FALSE;
	if( this->bonUtil.GetSetCh(space, ch) == TRUE ){
		ret = TRUE;
	}
	UnLock();
	return ret;
}


//�`�����l���ύX�����ǂ���
//�߂�l�F
// TRUE�i�ύX���j�AFALSE�i�����j
BOOL CBonCtrl::IsChChanging(BOOL* chChgErr)
{
	if( Lock(L"IsChChanging") == FALSE ) return 0;
	BOOL ret = this->tsOut.IsChChanging(chChgErr);
	UnLock();
	return ret;
}

//���݂̃X�g���[����ID���擾����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// ONID		[OUT]originalNetworkID
// TSID		[OUT]transportStreamID
BOOL CBonCtrl::GetStreamID(
	WORD* ONID,
	WORD* TSID
	)
{
	if( Lock(L"GetStreamID") == FALSE ) return 0;
	BOOL ret = this->tsOut.GetStreamID(ONID, TSID);
	UnLock();
	return ret;
}

//�V�O�i�����x���̎擾
//�߂�l�F
// �V�O�i�����x��
float CBonCtrl::GetSignalLevel()
{
	if( Lock(L"GetSignalLevel") == FALSE ) return 0;
	float ret = this->bonUtil.GetSignalLevel();
	this->tsOut.SetSignalLevel(ret);
	UnLock();
	return ret;
}

//BonDriver�����[�h��̏���������
//�߂�l�F
// �G���[�R�[�h
DWORD CBonCtrl::_OpenBonDriver()
{
	if( this->recvThread == NULL ){
		//��M�X���b�h�N��
		ResetEvent(this->recvStopEvent);
		this->recvThread = (HANDLE)_beginthreadex(NULL, 0, RecvThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
		SetThreadPriority( this->recvThread, THREAD_PRIORITY_NORMAL );
		ResumeThread(this->recvThread);
	}
	if( this->analyzeThread == NULL ){
		//��̓X���b�h�N��
		ResetEvent(this->analyzeStopEvent);
		this->analyzeThread = (HANDLE)_beginthreadex(NULL, 0, AnalyzeThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
		SetThreadPriority( this->analyzeThread, THREAD_PRIORITY_NORMAL );
		ResumeThread(this->analyzeThread);
	}

	return NO_ERR;
}

//���[�h���Ă���BonDriver�̊J���{��
//�߂�l�F
// �G���[�R�[�h
DWORD CBonCtrl::_CloseBonDriver()
{
	if( this->epgCapBackThread != NULL ){
		::SetEvent(this->epgCapBackStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->epgCapBackThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->epgCapBackThread, 0xffffffff);
		}
		CloseHandle(this->epgCapBackThread);
		this->epgCapBackThread = NULL;
	}

	if( this->epgCapThread != NULL ){
		::SetEvent(this->epgCapStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->epgCapThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->epgCapThread, 0xffffffff);
		}
		CloseHandle(this->epgCapThread);
		this->epgCapThread = NULL;
	}

	if( this->recvThread != NULL ){
		::SetEvent(this->recvStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->recvThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->recvThread, 0xffffffff);
		}
		CloseHandle(this->recvThread);
		this->recvThread = NULL;
	}
	if( this->analyzeThread != NULL ){
		::SetEvent(this->analyzeStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->analyzeThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->analyzeThread, 0xffffffff);
		}
		CloseHandle(this->analyzeThread);
		this->analyzeThread = NULL;
	}

	DWORD ret = this->bonUtil.CloseBonDriver();

	if( WaitForSingleObject( this->buffLockEvent, 1000 ) == WAIT_OBJECT_0 ){
		for( size_t i=0; i<this->TSBuff.size(); i++ ){
			SAFE_DELETE(this->TSBuff[i])
		}
		this->TSBuff.clear();
	}
	if( this->buffLockEvent != NULL ){
		SetEvent(this->buffLockEvent);
	}
	this->packetInit.ClearBuff();

	return ret;
}

UINT WINAPI CBonCtrl::RecvThread(LPVOID param)
{
	CBonCtrl* sys = (CBonCtrl*)param;
	while(1){
		if( ::WaitForSingleObject(sys->recvStopEvent, 0) != WAIT_TIMEOUT ){
			//�L�����Z�����ꂽ
			break;
		}
		BYTE *data = NULL;
		DWORD size = 0;
		DWORD remain = 0;
		try{
			if( sys->bonUtil.GetTsStream(&data,&size,&remain) == TRUE ){
				if( size != 0 && data != NULL){
					TS_DATA* item = new TS_DATA;
					try{
						if( sys->packetInit.GetTSData(data, size, &item->data, &item->size) == TRUE ){
							if( WaitForSingleObject( sys->buffLockEvent, 50000 ) == WAIT_OBJECT_0 ){
								if(sys->TSBuff.size() > sys->tsBuffMaxCount){
									size_t startIndex = sys->tsBuffMaxCount;
									if( sys->tsBuffMaxCount < 1000 ){
										startIndex = 0;
									}else{
										startIndex -= 1000;
									}
									for( size_t i=startIndex; i<sys->TSBuff.size(); i++ ){
										SAFE_DELETE(sys->TSBuff[i]);
									}
									vector<TS_DATA*>::iterator itr;
									itr = sys->TSBuff.begin();
									advance(itr,startIndex);
									sys->TSBuff.erase( itr, sys->TSBuff.end() );
								}
								sys->TSBuff.push_back(item);
								if( sys->buffLockEvent != NULL ){
									SetEvent(sys->buffLockEvent);
								}
							}else{
								delete item;
								_OutputDebugString(L"����Buff Write TimeOut");
							}
						}else{
							delete item;
						}
					}catch(...){
						delete item;
						_OutputDebugString(L"����RecvThread Exception2");
					}
				}else{
					Sleep(10);
				}
			}else{
				Sleep(10);
			}
		}catch(...){
			_OutputDebugString(L"����RecvThread Exception1");
		}
	}
	return 0;
}

UINT WINAPI CBonCtrl::AnalyzeThread(LPVOID param)
{
	CBonCtrl* sys = (CBonCtrl*)param;

	while(1){
		if( ::WaitForSingleObject(sys->analyzeStopEvent, 0) != WAIT_TIMEOUT ){
			//�L�����Z�����ꂽ
			break;
		}

		//�o�b�t�@����f�[�^���o��
		TS_DATA* data = NULL;
		try{
			if( WaitForSingleObject( sys->buffLockEvent, 5000 ) == WAIT_OBJECT_0 ){
				if(sys->TSBuff.size() > sys->tsBuffMaxCount){
					size_t startIndex = sys->tsBuffMaxCount;
					if( sys->tsBuffMaxCount < 1000 ){
						startIndex = 0;
					}else{
						startIndex -= 1000;
					}
					for( size_t i=startIndex; i<sys->TSBuff.size(); i++ ){
						SAFE_DELETE(sys->TSBuff[i]);
					}
					vector<TS_DATA*>::iterator itr;
					itr = sys->TSBuff.begin();
					advance(itr,startIndex);
					sys->TSBuff.erase( itr, sys->TSBuff.end() );
				}
				if( sys->TSBuff.size() != 0 ){
					data = sys->TSBuff[0];
					sys->TSBuff.erase( sys->TSBuff.begin() );
				}
				if( sys->buffLockEvent != NULL ){
					SetEvent(sys->buffLockEvent);
				}
			}else{
				Sleep(10);
				continue ;
			}
		}catch(...){
			_OutputDebugString(L"����AnalyzeThread Exception1");
			if( data != NULL ){
				sys->tsOut.AddTSBuff(data);
				SAFE_DELETE(data);
			}
			continue ;
		}
		try{
			if( data != NULL ){
				sys->tsOut.AddTSBuff(data);
				SAFE_DELETE(data);
			}else{
				Sleep(5);
			}
		}catch(...){
			_OutputDebugString(L"����AnalyzeThread Exception2");
		}
	}
	return 0;
}

//EPG�f�[�^�̒~�Ϗ�Ԃ����Z�b�g����
void CBonCtrl::ClearSectionStatus()
{
	if( Lock(L"ClearSectionStatus") == FALSE ) return ;
	this->tsOut.ClearSectionStatus();

	UnLock();
	return ;
}

//EPG�f�[�^�̒~�Ϗ�Ԃ��擾����
//�߂�l�F
// �X�e�[�^�X
//�����F
// l_eitFlag		[IN]L-EIT�̃X�e�[�^�X���擾
EPG_SECTION_STATUS CBonCtrl::GetSectionStatus(
	BOOL l_eitFlag
	)
{
	if( Lock(L"GetSectionStatus") == FALSE ) return EpgNoData;
	EPG_SECTION_STATUS status = this->tsOut.GetSectionStatus(l_eitFlag);

	UnLock();
	return status;
}

//���X�g���[���̃T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceList				[OUT]�T�[�r�X���̃��X�g
DWORD CBonCtrl::GetServiceListActual(
	vector<TS_SERVICE_INFO>* serviceList
	)
{
	if( Lock(L"GetServiceListActual") == FALSE ) return EpgNoData;

	DWORD _serviceListSize = 0;
	SERVICE_INFO* _serviceList = NULL;
	DWORD err = this->tsOut.GetServiceListActual(&_serviceListSize, &_serviceList);
	if( err == NO_ERR ){
		for( DWORD i=0; i<_serviceListSize; i++ ){
			TS_SERVICE_INFO item;
			item.ONID = _serviceList[i].original_network_id;
			item.TSID = _serviceList[i].transport_stream_id;
			item.SID = _serviceList[i].service_id;
			if( _serviceList[i].extInfo != NULL ){
				item.serviceType = _serviceList[i].extInfo->service_type;
				item.partialFlag = _serviceList[i].extInfo->partialReceptionFlag;
				if( _serviceList[i].extInfo->service_name != NULL ){
					item.serviceName = _serviceList[i].extInfo->service_name;
				}else{
					item.serviceName = L"";
				}
				if( _serviceList[i].extInfo->network_name != NULL ){
					item.networkName = _serviceList[i].extInfo->network_name;
				}else{
					item.networkName = L"";
				}
				item.remoteControlKeyID = _serviceList[i].extInfo->remote_control_key_id;
			}else{
				item.serviceType = 0;
				item.partialFlag = 0;
				item.serviceName = L"";
				item.networkName = L"";
				item.remoteControlKeyID = 0;
			}
			serviceList->push_back(item);
		}
	}

	UnLock();
	return err;
}

//�T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceList				[OUT]�T�[�r�X���̃��X�g
DWORD CBonCtrl::GetServiceList(
	vector<CH_DATA4>* serviceList
	)
{
	if( Lock(L"GetServiceList") == FALSE ) return FALSE;
	BOOL ret = this->chUtil.GetEnumService(serviceList);

	UnLock();
	return ret;
}

//TS�X�g���[������p�R���g���[�����쐬����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id			[OUT]���䎯��ID
BOOL CBonCtrl::CreateServiceCtrl(
	DWORD* id
	)
{
	if( Lock(L"CreateServiceCtrl") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.CreateServiceCtrl(id);

	UnLock();
	return ret;
}

//TS�X�g���[������p�R���g���[�����쐬����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id			[IN]���䎯��ID
BOOL CBonCtrl::DeleteServiceCtrl(
	DWORD id
	)
{
	if( Lock(L"DeleteServiceCtrl") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.DeleteServiceCtrl(id);

	UnLock();
	return ret;
}

//����Ώۂ̃T�[�r�X��ݒ肷��
BOOL CBonCtrl::SetServiceID(
	DWORD id,
	WORD serviceID
	)
{
	if( Lock(L"SetServiceID") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.SetServiceID(id,serviceID);

	UnLock();
	return ret;
}

BOOL CBonCtrl::GetServiceID(
	DWORD id,
	WORD* serviceID
	)
{
	if( Lock(L"SetServiceID") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.GetServiceID(id,serviceID);

	UnLock();
	return ret;
}

//UDP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// id			[IN]���䎯��ID
// sendList		[IN/OUT]���M�惊�X�g�BNULL�Œ�~�BPort�͎��ۂɑ��M�Ɏg�p����Port���Ԃ�B�B
BOOL CBonCtrl::SendUdp(
	DWORD id,
	vector<NW_SEND_INFO>* sendList
	)
{
	if( Lock(L"SendUdp") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.SendUdp(id,sendList);

	UnLock();
	return ret;
}

//TCP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// id			[IN]���䎯��ID
// sendList		[IN/OUT]���M�惊�X�g�BNULL�Œ�~�BPort�͎��ۂɑ��M�Ɏg�p����Port���Ԃ�B
BOOL CBonCtrl::SendTcp(
	DWORD id,
	vector<NW_SEND_INFO>* sendList
	)
{
	if( Lock(L"SendTcp") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.SendTcp(id,sendList);

	UnLock();
	return ret;
}

//�t�@�C���ۑ����J�n����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// id					[IN]���䎯��ID
// fileName				[IN]�ۑ��t�@�C���p�X
// overWriteFlag		[IN]����t�@�C�������ݎ��ɏ㏑�����邩�ǂ����iTRUE�F����AFALSE�F���Ȃ��j
// pittariFlag			[IN]�҂����胂�[�h�iTRUE�F����AFALSE�F���Ȃ��j
// pittariONID			[IN]�҂����胂�[�h�Ř^�悷��ONID
// pittariTSID			[IN]�҂����胂�[�h�Ř^�悷��TSID
// pittariSID			[IN]�҂����胂�[�h�Ř^�悷��SID
// pittariEventID		[IN]�҂����胂�[�h�Ř^�悷��C�x���gID
// createSize			[IN]�t�@�C���쐬���Ƀf�B�X�N�ɗ\�񂷂�e��
// saveFolder			[IN]�g�p����t�H���_�ꗗ
// saveFolderSub		[IN]HDD�̋󂫂��Ȃ��Ȃ����ꍇ�Ɉꎞ�I�Ɏg�p����t�H���_
BOOL CBonCtrl::StartSave(
	DWORD id,
	wstring fileName,
	BOOL overWriteFlag,
	BOOL pittariFlag,
	WORD pittariONID,
	WORD pittariTSID,
	WORD pittariSID,
	WORD pittariEventID,
	ULONGLONG createSize,
	vector<REC_FILE_SET_INFO>* saveFolder,
	vector<wstring>* saveFolderSub
)
{
	if( Lock(L"StartSave") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.StartSave(id, fileName, overWriteFlag, pittariFlag, pittariONID, pittariTSID, pittariSID, pittariEventID, createSize, saveFolder, saveFolderSub, writeBuffMaxCount);

	StartBackgroundEpgCap();

	UnLock();
	return ret;
}

//�t�@�C���ۑ����I������
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// id			[IN]���䎯��ID
BOOL CBonCtrl::EndSave(
	DWORD id
	)
{
	if( Lock(L"EndSave") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.EndSave(id);

	UnLock();
	return ret;
}

//�X�N�����u�����������̓���ݒ�
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// enable		[IN] TRUE�i��������j�AFALSE�i�������Ȃ��j
BOOL CBonCtrl::SetScramble(
	DWORD id,
	BOOL enable
	)
{
	if( Lock(L"SetScramble") == FALSE ) return FALSE;
	BOOL ret = this->tsOut.SetScramble(id, enable);

	UnLock();
	return ret;
}

//�����ƃf�[�^�����܂߂邩�ǂ���
//�����F
// id					[IN]���䎯��ID
// enableCaption		[IN]������ TRUE�i�܂߂�j�AFALSE�i�܂߂Ȃ��j
// enableData			[IN]�f�[�^������ TRUE�i�܂߂�j�AFALSE�i�܂߂Ȃ��j
void CBonCtrl::SetServiceMode(
	DWORD id,
	BOOL enableCaption,
	BOOL enableData
	)
{
	if( Lock(L"SetServiceMode") == FALSE ) return ;
	this->tsOut.SetServiceMode(id, enableCaption, enableData);

	UnLock();
}

//�G���[�J�E���g���N���A����
void CBonCtrl::ClearErrCount(
	DWORD id
	)
{
	if( Lock(L"ClearErrCount") == FALSE ) return ;
	this->tsOut.ClearErrCount(id);

	UnLock();
}

//�h���b�v�ƃX�N�����u���̃J�E���g���擾����
//�����F
// drop				[OUT]�h���b�v��
// scramble			[OUT]�X�N�����u����
void CBonCtrl::GetErrCount(
	DWORD id,
	ULONGLONG* drop,
	ULONGLONG* scramble
	)
{
	if( Lock(L"GetErrCount") == FALSE ) return ;
	this->tsOut.GetErrCount(id, drop, scramble);

	UnLock();
}

//�w��T�[�r�X�̌���or����EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CBonCtrl::GetEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	BOOL nextFlag,
	EPGDB_EVENT_INFO* epgInfo
	)
{
	if( Lock(L"GetEpgInfo") == FALSE ) return FALSE;
	DWORD ret = this->tsOut.GetEpgInfo(originalNetworkID, transportStreamID, serviceID, nextFlag, epgInfo);

	UnLock();
	return ret;
}

//�w��C�x���g��EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// eventID					[IN]�擾�Ώۂ�EventID
// pfOnlyFlag				[IN]p/f����̂݌������邩�ǂ���
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CBonCtrl::SearchEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	WORD eventID,
	BYTE pfOnlyFlag,
	EPGDB_EVENT_INFO* epgInfo
	)
{
	if( Lock(L"SearchEpgInfo") == FALSE ) return FALSE;
	DWORD ret = this->tsOut.SearchEpgInfo(originalNetworkID, transportStreamID, serviceID, eventID, pfOnlyFlag, epgInfo);

	UnLock();
	return ret;
}

//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
//�߂�l�F
// ���̕b��
int CBonCtrl::GetTimeDelay(
	)
{
	if( Lock(L"GetTimeDelay") == FALSE ) return 0;
	int delay = this->tsOut.GetTimeDelay();

	UnLock();
	return delay;
}

//�^�撆���ǂ������擾����
// TRUE�i�^�撆�j�AFALSE�i�^�悵�Ă��Ȃ��j
BOOL CBonCtrl::IsRec()
{
	if( Lock(L"IsRec") == FALSE ) return FALSE;

	BOOL ret = this->tsOut.IsRec();

	UnLock();
	return ret;
}
//�`�����l���X�L�������J�n����
//�߂�l�F
// �G���[�R�[�h
DWORD CBonCtrl::StartChScan()
{
	if( Lock(L"StartChScan") == FALSE ) return ERR_FALSE;

	if( this->tsOut.IsRec() == TRUE ){
		UnLock();
		return ERR_FALSE;
	}
	if( this->epgSt_err == ST_WORKING ){
		UnLock();
		return ERR_FALSE;
	}

	if( ::WaitForSingleObject(this->chScanThread, 0) == WAIT_OBJECT_0 ){
		CloseHandle(this->chScanThread);
		this->chScanThread = NULL;
	}

	DWORD ret = ERR_FALSE;
	if( this->chScanThread == NULL ){
		if( this->bonUtil.GetOpenBonDriverIndex() != -1 ){
			ret = NO_ERR;
			this->chSt_space = 0;
			this->chSt_ch = 0;
			this->chSt_chName = L"";
			this->chSt_chkNum = 0;
			this->chSt_totalNum = 0;
			this->chSt_err = ST_WORKING;

			//��M�X���b�h�N��
			ResetEvent(this->chScanStopEvent);
			this->chScanThread = (HANDLE)_beginthreadex(NULL, 0, ChScanThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
			SetThreadPriority( this->chScanThread, THREAD_PRIORITY_NORMAL );
			ResumeThread(this->chScanThread);
		}
	}

	UnLock();
	return ret;
}

//�`�����l���X�L�������L�����Z������
//�߂�l�F
// �G���[�R�[�h
DWORD CBonCtrl::StopChScan()
{
	if( Lock(L"StopChScan") == FALSE ) return ERR_FALSE;

	if( this->chScanThread != NULL ){
		::SetEvent(this->chScanStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->chScanThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->chScanThread, 0xffffffff);
		}
		CloseHandle(this->chScanThread);
		this->chScanThread = NULL;
	}

	UnLock();
	return NO_ERR;
}

//�`�����l���X�L�����̏�Ԃ��擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// space		[OUT]�X�L�������̕���CH��space
// ch			[OUT]�X�L�������̕���CH��ch
// chName		[OUT]�X�L�������̕���CH�̖��O
// chkNum		[OUT]�`�F�b�N�ς݂̐�
// totalNum		[OUT]�`�F�b�N�Ώۂ̑���
DWORD CBonCtrl::GetChScanStatus(
	DWORD* space,
	DWORD* ch,
	wstring* chName,
	DWORD* chkNum,
	DWORD* totalNum
	)
{
	if( Lock(L"GetChScanStatus") == FALSE ) return ERR_FALSE;

	DWORD ret;
	if( space != NULL ){
		*space = this->chSt_space;
	}
	if( ch != NULL ){
		*ch = this->chSt_ch;
	}
	if( chName != NULL ){
		*chName = this->chSt_chName;
	}
	if( chkNum != NULL ){
		*chkNum = this->chSt_chkNum;
	}
	if( totalNum != NULL ){
		*totalNum = this->chSt_totalNum;
	}
	ret = this->chSt_err;

	UnLock();
	return ret;
}

UINT WINAPI CBonCtrl::ChScanThread(LPVOID param)
{
	CBonCtrl* sys = (CBonCtrl*)param;

	sys->chUtil.Clear();

	wstring chSet4 = sys->bonUtil.GetChSet4Path();
	wstring chSet5 = sys->bonUtil.GetChSet5Path();

	vector<CHK_CH_INFO> chkList;
	map<DWORD, BON_SPACE_INFO> spaceMap;
	if( sys->bonUtil.GetOriginalChList(&spaceMap) != NO_ERR ){
		sys->chSt_err = ST_COMPLETE;
		sys->chUtil.SaveChSet(chSet4, chSet5);
		return 0;
	}
	map<DWORD, BON_SPACE_INFO>::iterator itrSpace;
	for( itrSpace = spaceMap.begin(); itrSpace != spaceMap.end(); itrSpace++ ){
		sys->chSt_totalNum += (DWORD)itrSpace->second.chMap.size();
		map<DWORD, BON_CH_INFO>::iterator itrCh;
		for( itrCh = itrSpace->second.chMap.begin(); itrCh != itrSpace->second.chMap.end(); itrCh++ ){
			CHK_CH_INFO item;
			item.space = itrSpace->second.space;
			item.spaceName = itrSpace->second.spaceName;
			item.ch = itrCh->second.ch;
			item.chName = itrCh->second.chName;
			chkList.push_back(item);
		}
	}

	if( sys->chSt_totalNum == 0 ){
		sys->chSt_err = ST_COMPLETE;
		sys->chUtil.SaveChSet(chSet4, chSet5);
		return 0;
	}

	wstring folderPath;
	GetModuleFolderPath( folderPath );
	wstring iniPath = folderPath;
	iniPath += L"\\BonCtrl.ini";

	DWORD chChgTimeOut = GetPrivateProfileInt(L"CHSCAN", L"ChChgTimeOut", 9, iniPath.c_str());
	DWORD serviceChkTimeOut = GetPrivateProfileInt(L"CHSCAN", L"ServiceChkTimeOut", 8, iniPath.c_str());


	DWORD wait = 0;
	BOOL chkNext = TRUE;
	LONGLONG startTime = 0;
	DWORD chkWait = 0;
	DWORD chkCount = 0;
	BOOL firstChg = FALSE;

	while(1){
		if( ::WaitForSingleObject(sys->chScanStopEvent, wait) != WAIT_TIMEOUT ){
			//�L�����Z�����ꂽ
			sys->chSt_err = ST_CANCEL;
			break;
		}
		if( chkNext == TRUE ){
			sys->chSt_space = chkList[chkCount].space;
			sys->chSt_ch = chkList[chkCount].ch;
			sys->chSt_chName = chkList[chkCount].chName;
			sys->_SetCh(chkList[chkCount].space, chkList[chkCount].ch, TRUE);
			if( firstChg == FALSE ){
				firstChg = TRUE;
				sys->tsOut.ResetChChange();
			}
			startTime = GetTimeCount();
			chkNext = FALSE;
			wait = 1000;
			chkWait = chChgTimeOut;
		}else{
			BOOL chChgErr = FALSE;
			if( sys->tsOut.IsChChanging(&chChgErr) == TRUE ){
				if( startTime + chkWait < GetTimeCount() ){
					//�`�����l���؂�ւ���8�b�ȏォ�����Ă�̂Ŗ��M���Ɣ��f
					OutputDebugString(L"��AutoScan Ch Change timeout\r\n");
					chkNext = TRUE;
				}
			}else{
				if( startTime + chkWait+serviceChkTimeOut < GetTimeCount() || chChgErr == TRUE){
					//�`�����l���؂�ւ������������ǃT�[�r�X�ꗗ�Ƃ�Ȃ��̂Ŗ��M���Ɣ��f
					OutputDebugString(L"��AutoScan GetService timeout\r\n");
					chkNext = TRUE;
				}else{
					//�T�[�r�X�ꗗ�̎擾���s��
					DWORD serviceListSize;
					SERVICE_INFO* serviceList;
					if( sys->tsOut.GetServiceListActual(&serviceListSize, &serviceList) == NO_ERR ){
						if( serviceListSize > 0 ){
							//�ꗗ�̎擾���ł���
							for( DWORD i=0 ;i<serviceListSize; i++ ){
								if( serviceList[i].extInfo != NULL ){
									if( serviceList[i].extInfo->service_name != NULL ){
										if( wcslen(serviceList[i].extInfo->service_name) > 0 ){
											sys->chUtil.AddServiceInfo(chkList[chkCount].space, chkList[chkCount].ch, chkList[chkCount].chName, &(serviceList[i]));
										}
									}
								}
							}
							chkNext = TRUE;
						}
					}
				}
			}
			if( chkNext == TRUE ){
				//���̃`�����l����
				chkCount++;
				sys->chSt_chkNum++;
				if( sys->chSt_totalNum <= chkCount ){
					//�S���`�F�b�N�I������̂ŏI��
					sys->chSt_err = ST_COMPLETE;
					sys->chUtil.SaveChSet(chSet4, chSet5);
					break;
				}
			}
		}
	}

	sys->chUtil.LoadChSet(chSet4, chSet5);

	return 0;
}

//EPG�擾�Ώۂ̃T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// chList		[OUT]EPG�擾����`�����l���ꗗ
DWORD CBonCtrl::GetEpgCapService(
	vector<EPGCAP_SERVICE_INFO>* chList
	)
{
	if( Lock(L"GetEpgCapService") == FALSE ) return ERR_FALSE;

	DWORD ret = NO_ERR;
	this->chUtil.GetEpgCapService(chList);
	UnLock();
	return ret;
}

//EPG�擾���J�n����
//�߂�l�F
// �G���[�R�[�h
//�����F
// chList		[IN]EPG�擾����`�����l���ꗗ
// BSBasic		[IN]BS�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
// CS1Basic		[IN]CS1�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
// CS2Basic		[IN]CS2�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
DWORD CBonCtrl::StartEpgCap(
	vector<EPGCAP_SERVICE_INFO>* chList,
	BOOL BSBasic,
	BOOL CS1Basic,
	BOOL CS2Basic
	)
{
	if( Lock(L"StartEpgCap") == FALSE ) return ERR_FALSE;

	if( this->tsOut.IsRec() == TRUE ){
		UnLock();
		return ERR_FALSE;
	}
	if( this->chSt_err == ST_WORKING ){
		UnLock();
		return ERR_FALSE;
	}

	StopBackgroundEpgCap();

	if( ::WaitForSingleObject(this->epgCapThread, 0) == WAIT_OBJECT_0 ){
		CloseHandle(this->epgCapThread);
		this->epgCapThread = NULL;
	}

	DWORD ret = ERR_FALSE;
	if( this->epgCapThread == NULL ){
		if( this->bonUtil.GetOpenBonDriverIndex() != -1 ){
			ret = NO_ERR;
			this->epgCapChList = *chList;
			this->BSBasic = BSBasic;
			this->CS1Basic = CS1Basic;
			this->CS2Basic = CS2Basic;
			this->epgSt_err = ST_WORKING;
			this->epgSt_ch.ONID = 0xFFFF;
			this->epgSt_ch.TSID = 0xFFFF;
			this->epgSt_ch.SID = 0xFFFF;

			//��M�X���b�h�N��
			ResetEvent(this->epgCapStopEvent);
			this->epgCapThread = (HANDLE)_beginthreadex(NULL, 0, EpgCapThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
			SetThreadPriority( this->epgCapThread, THREAD_PRIORITY_NORMAL );
			ResumeThread(this->epgCapThread);
		}
	}

	UnLock();
	return ret;
}

//EPG�擾���~����
//�߂�l�F
// �G���[�R�[�h
DWORD CBonCtrl::StopEpgCap(
	)
{
	if( Lock(L"StopEpgCap") == FALSE ) return ERR_FALSE;

	if( this->epgCapThread != NULL ){
		::SetEvent(this->epgCapStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->epgCapThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->epgCapThread, 0xffffffff);
		}
		CloseHandle(this->epgCapThread);
		this->epgCapThread = NULL;
	}

	UnLock();
	return NO_ERR;
}

//EPG�擾�̃X�e�[�^�X���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// info			[OUT]�擾���̃T�[�r�X
DWORD CBonCtrl::GetEpgCapStatus(
	EPGCAP_SERVICE_INFO* info
	)
{
	if( Lock(L"GetEpgCapStatus") == FALSE ) return ERR_FALSE;

	if( info != NULL ){
		*info = this->epgSt_ch;
	}
	DWORD ret = this->epgSt_err;

	UnLock();
	return ret;
}

UINT WINAPI CBonCtrl::EpgCapThread(LPVOID param)
{
	CBonCtrl* sys = (CBonCtrl*)param;

	BOOL chkNext = TRUE;
	BOOL startCap = FALSE;
	DWORD wait = 0;
	LONGLONG startTime = 0;
	DWORD chkCount = 0;
	DWORD chkWait = 8;

	BOOL chkBS = FALSE;
	BOOL chkCS1 = FALSE;
	BOOL chkCS2 = FALSE;

	wstring folderPath;
	GetModuleFolderPath( folderPath );
	wstring iniPath = folderPath;
	iniPath += L"\\BonCtrl.ini";

	DWORD timeOut = GetPrivateProfileInt(L"EPGCAP", L"EpgCapTimeOut", 15, iniPath.c_str());
	BOOL saveTimeOut = GetPrivateProfileInt(L"EPGCAP", L"EpgCapSaveTimeOut", 0, iniPath.c_str());

	while(1){
		if( ::WaitForSingleObject(sys->epgCapStopEvent, wait) != WAIT_TIMEOUT ){
			//�L�����Z�����ꂽ
			sys->epgSt_err = ST_CANCEL;
			sys->tsOut.StopSaveEPG(FALSE);
			break;
		}
		if( chkNext == TRUE ){
			if( sys->tsOut.IsChChanging(NULL) == TRUE ){
				Sleep(200);
				continue;
			}
			DWORD space = 0;
			DWORD ch = 0;
			sys->chUtil.GetCh(sys->epgCapChList[chkCount].ONID, sys->epgCapChList[chkCount].TSID, space, ch);
			sys->_SetCh(space, ch);
			startTime = GetTimeCount();
			chkNext = FALSE;
			startCap = FALSE;
			wait = 1000;
			if( sys->epgCapChList[chkCount].ONID == 4 ){
				chkBS = TRUE;
			}else if( sys->epgCapChList[chkCount].ONID == 6 ){
				chkCS1 = TRUE;
			}else if( sys->epgCapChList[chkCount].ONID == 7 ){
				chkCS2 = TRUE;
			}
			sys->epgSt_ch = sys->epgCapChList[chkCount];
		}else{
			BOOL chChgErr = FALSE;
			if( sys->tsOut.IsChChanging(&chChgErr) == TRUE ){
				if( startTime + chkWait < GetTimeCount() ){
					//�`�����l���؂�ւ���10�b�ȏォ�����Ă�̂Ŗ��M���Ɣ��f
					chkNext = TRUE;
				}
			}else{
				if( (startTime + chkWait + timeOut*60 < GetTimeCount()) || chChgErr == TRUE){
					//15���ȏォ�����Ă���Ȃ��~
					sys->tsOut.StopSaveEPG(saveTimeOut);
					chkNext = TRUE;
					wait = 0;
					_OutputDebugString(L"++%d����EPG�擾�������� or Ch�ύX�ŃG���[", timeOut);
				}else if(startTime + chkWait < GetTimeCount() ){
					//�؂�ւ�����15�b�ȏ�߂��Ă���̂Ŏ擾����
					if( startCap == FALSE ){
						//�擾�J�n
						startCap = TRUE;
						wstring epgDataPath = L"";
						sys->GetEpgDataFilePath(sys->epgCapChList[chkCount].ONID, sys->epgCapChList[chkCount].TSID, epgDataPath);
						sys->tsOut.StartSaveEPG(epgDataPath);
						sys->tsOut.ClearSectionStatus();
						wait = 60*1000;
					}else{
						//�~�Ϗ�ԃ`�F�b�N
						BOOL leitFlag = sys->chUtil.IsPartial(sys->epgCapChList[chkCount].ONID, sys->epgCapChList[chkCount].TSID, sys->epgCapChList[chkCount].SID);
						EPG_SECTION_STATUS status = sys->tsOut.GetSectionStatus(leitFlag);
						if( sys->epgCapChList[chkCount].ONID == 4 && sys->BSBasic == TRUE ){
							if( status == EpgBasicAll || status == EpgHEITAll ){
								chkNext = TRUE;
							}
						}else if( sys->epgCapChList[chkCount].ONID == 6 && sys->CS1Basic == TRUE ){
							if( status == EpgBasicAll || status == EpgHEITAll ){
								chkNext = TRUE;
							}
						}else if( sys->epgCapChList[chkCount].ONID == 7 && sys->CS2Basic == TRUE ){
							if( status == EpgBasicAll || status == EpgHEITAll ){
								chkNext = TRUE;
							}
						}else{
							if( leitFlag == FALSE && status == EpgHEITAll ){
								chkNext = TRUE;
							}else if( leitFlag == TRUE && status == EpgLEITAll ){
								chkNext = TRUE;
							}
						}
						if( chkNext == TRUE ){
							sys->tsOut.StopSaveEPG(TRUE);
							wait = 0;
						}else{
							wait = 10*1000;
						}
					}
				}
			}
			if( chkNext == TRUE ){
				//���̃`�����l����
				chkCount++;
				if( sys->epgCapChList.size() <= chkCount ){
					//�S���`�F�b�N�I������̂ŏI��
					sys->epgSt_err = ST_COMPLETE;
					return 0;
				}
				//BS 1�`�����l���̂݁H
				if( sys->epgCapChList[chkCount].ONID == 4 && sys->BSBasic == TRUE && chkBS == TRUE){
					while(chkCount<(DWORD)sys->epgCapChList.size()){
						if( sys->epgCapChList[chkCount].ONID != 4 ){
							break;
						}
						chkCount++;
						if( sys->epgCapChList.size() <= chkCount ){
							//�S���`�F�b�N�I������̂ŏI��
							sys->epgSt_err = ST_COMPLETE;
							return 0;
						}
					}
				}
				//CS1 1�`�����l���̂݁H
				if( sys->epgCapChList[chkCount].ONID == 6 && sys->CS1Basic == TRUE && chkCS1 == TRUE ){
					while(chkCount<(DWORD)sys->epgCapChList.size()){
						if( sys->epgCapChList[chkCount].ONID != 6 ){
							break;
						}
						chkCount++;
						if( sys->epgCapChList.size() <= chkCount ){
							//�S���`�F�b�N�I������̂ŏI��
							sys->epgSt_err = ST_COMPLETE;
							return 0;
						}
					}
				}
				//CS2 1�`�����l���̂݁H
				if( sys->epgCapChList[chkCount].ONID == 7 && sys->CS2Basic == TRUE && chkCS2 == TRUE ){
					while(chkCount<(DWORD)sys->epgCapChList.size()){
						if( sys->epgCapChList[chkCount].ONID != 7 ){
							break;
						}
						chkCount++;
						if( sys->epgCapChList.size() <= chkCount ){
							//�S���`�F�b�N�I������̂ŏI��
							sys->epgSt_err = ST_COMPLETE;
							return 0;
						}
					}
				}
			}
		}
	}
	return 0;
}

void CBonCtrl::GetEpgDataFilePath(WORD ONID, WORD TSID, wstring& epgDataFilePath)
{
	wstring epgDataFolderPath = L"";
	GetSettingPath(epgDataFolderPath);
	epgDataFolderPath += EPG_SAVE_FOLDER;

	if( ONID == 4 && this->BSBasic == TRUE ){
		Format(epgDataFilePath, L"%s\\%04XFFFF_epg.dat", epgDataFolderPath.c_str(), ONID);
	}else if( ONID == 6 && this->CS1Basic == TRUE ){
		Format(epgDataFilePath, L"%s\\%04XFFFF_epg.dat", epgDataFolderPath.c_str(), ONID);
	}else if( ONID == 7 && this->CS2Basic == TRUE ){
		Format(epgDataFilePath, L"%s\\%04XFFFF_epg.dat", epgDataFolderPath.c_str(), ONID);
	}else{
		Format(epgDataFilePath, L"%s\\%04X%04X_epg.dat", epgDataFolderPath.c_str(), ONID, TSID);
	}
}

//�^�撆�̃t�@�C���̃t�@�C���p�X���擾����
//�����F
// id					[IN]���䎯��ID
// filePath				[OUT]�ۑ��t�@�C����
// subRecFlag			[OUT]�T�u�^�悪�����������ǂ���
void CBonCtrl::GetSaveFilePath(
	DWORD id,
	wstring* filePath,
	BOOL* subRecFlag
	)
{
	if( Lock(L"GetSaveFilePath") == FALSE ) return ;
	this->tsOut.GetSaveFilePath(id, filePath, subRecFlag);

	UnLock();
}

//�h���b�v�ƃX�N�����u���̃J�E���g��ۑ�����
//�����F
// id					[IN]���䎯��ID
// filePath				[IN]�ۑ��t�@�C����
void CBonCtrl::SaveErrCount(
	DWORD id,
	wstring filePath
	)
{
	if( Lock(L"SaveErrCount") == FALSE ) return ;
	this->tsOut.SaveErrCount(id, filePath);

	UnLock();
}

//�^�撆�̃t�@�C���̏o�̓T�C�Y���擾����
//�����F
// id					[IN]���䎯��ID
// writeSize			[OUT]�ۑ��t�@�C����
void CBonCtrl::GetRecWriteSize(
	DWORD id,
	__int64* writeSize
	)
{
	if( Lock(L"GetRecWriteSize") == FALSE ) return ;
	this->tsOut.GetRecWriteSize(id, writeSize);

	UnLock();
}

//�o�b�N�O���E���h�ł�EPG�擾�ݒ�
//�����F
// enableLive	[IN]�������Ɏ擾����
// enableRec	[IN]�^�撆�Ɏ擾����
// enableRec	[IN]EPG�擾����`�����l���ꗗ
// BSBasic		[IN]BS�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
// CS1Basic		[IN]CS1�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
// CS2Basic		[IN]CS2�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
// backStartWaitSec	[IN]Ch�؂�ւ��A�^��J�n��A�o�b�N�O���E���h�ł�EPG�擾���J�n����܂ł̕b��
void CBonCtrl::SetBackGroundEpgCap(
	BOOL enableLive,
	BOOL enableRec,
	BOOL BSBasic,
	BOOL CS1Basic,
	BOOL CS2Basic,
	DWORD backStartWaitSec
	)
{
	if( Lock(L"SetBackGroundEpgCap") == FALSE ) return ;

	this->enableLiveEpgCap = enableLive;
	this->enableRecEpgCap = enableRec;
	this->BSBasic = BSBasic;
	this->CS1Basic = CS1Basic;
	this->CS2Basic = CS2Basic;
	this->epgCapBackStartWaitSec = backStartWaitSec;

	StartBackgroundEpgCap();

	UnLock();
}

void CBonCtrl::StartBackgroundEpgCap()
{
	StopBackgroundEpgCap();
	if( this->epgCapBackThread == NULL && this->epgCapThread == NULL ){
		if( this->bonUtil.GetOpenBonDriverIndex() != -1 ){
			//��M�X���b�h�N��
			ResetEvent(this->epgCapBackStopEvent);
			this->epgCapBackThread = (HANDLE)_beginthreadex(NULL, 0, EpgCapBackThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
			SetThreadPriority( this->epgCapBackThread, THREAD_PRIORITY_NORMAL );
			ResumeThread(this->epgCapBackThread);
		}
	}
}

void CBonCtrl::StopBackgroundEpgCap()
{
	if( this->epgCapBackThread != NULL ){
		::SetEvent(this->epgCapBackStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->epgCapBackThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->epgCapBackThread, 0xffffffff);
		}
		CloseHandle(this->epgCapBackThread);
		this->epgCapBackThread = NULL;
	}
}

UINT WINAPI CBonCtrl::EpgCapBackThread(LPVOID param)
{
	wstring folderPath;
	GetModuleFolderPath( folderPath );
	wstring iniPath = folderPath;
	iniPath += L"\\BonCtrl.ini";

	DWORD timeOut = GetPrivateProfileInt(L"EPGCAP", L"EpgCapTimeOut", 15, iniPath.c_str());
	BOOL saveTimeOut = GetPrivateProfileInt(L"EPGCAP", L"EpgCapSaveTimeOut", 0, iniPath.c_str());

	CBonCtrl* sys = (CBonCtrl*)param;
	if( ::WaitForSingleObject(sys->epgCapBackStopEvent, sys->epgCapBackStartWaitSec*1000) != WAIT_TIMEOUT ){
		//�L�����Z�����ꂽ
		return 0;
	}

	if( sys->tsOut.IsRec() == TRUE ){
		if( sys->enableRecEpgCap == FALSE ){
			return 0;
		}
	}else{
		if( sys->enableLiveEpgCap == FALSE ){
			return 0;
		}
	}

	LONGLONG startTime = GetTimeCount();

	wstring epgDataPath = L"";
	WORD ONID;
	WORD TSID;
	sys->tsOut.GetStreamID(&ONID, &TSID);

	if( sys->chUtil.IsEpgCapService(ONID, TSID) == FALSE ){
		return 0;
	}

	sys->GetEpgDataFilePath(ONID, TSID, epgDataPath);
	sys->tsOut.StartSaveEPG(epgDataPath);
	sys->tsOut.ClearSectionStatus();

	if( ::WaitForSingleObject(sys->epgCapBackStopEvent, 60*1000) != WAIT_TIMEOUT ){
		//�L�����Z�����ꂽ
		sys->tsOut.StopSaveEPG(FALSE);
		return 0;
	}
	while(1){
		//�~�Ϗ�ԃ`�F�b�N
		BOOL chkNext = FALSE;
		BOOL leitFlag = sys->chUtil.IsPartial(ONID, TSID, sys->lastSID);
		EPG_SECTION_STATUS status = sys->tsOut.GetSectionStatus(leitFlag);
		if( ONID == 4 && sys->BSBasic == TRUE ){
			if( status == EpgBasicAll || status == EpgHEITAll ){
				chkNext = TRUE;
			}
		}else if( ONID == 6 && sys->CS1Basic == TRUE ){
			if( status == EpgBasicAll || status == EpgHEITAll ){
				chkNext = TRUE;
			}
		}else if( ONID == 7 && sys->CS2Basic == TRUE ){
			if( status == EpgBasicAll || status == EpgHEITAll ){
				chkNext = TRUE;
			}
		}else{
			if( leitFlag == FALSE && status == EpgHEITAll ){
				chkNext = TRUE;
			}else if( leitFlag == TRUE && status == EpgLEITAll ){
				chkNext = TRUE;
			}
		}

		if( chkNext == TRUE ){
			sys->tsOut.StopSaveEPG(TRUE);
			CSendCtrlCmd cmd;
			cmd.SetConnectTimeOut(1000);
			cmd.SendReloadEpg();
			break;
		}else{
			if( (startTime + timeOut*60 < GetTimeCount()) ){
				//15���ȏォ�����Ă���Ȃ��~
				sys->tsOut.StopSaveEPG(saveTimeOut);
				CSendCtrlCmd cmd;
				cmd.SetConnectTimeOut(1000);
				cmd.SendReloadEpg();
				_OutputDebugString(L"++%d����EPG�擾�������� or Ch�ύX�ŃG���[", timeOut);
				break;
			}
		}

		if( ::WaitForSingleObject(sys->epgCapBackStopEvent, 10*1000) != WAIT_TIMEOUT ){
			//�L�����Z�����ꂽ
			sys->tsOut.StopSaveEPG(FALSE);
			break;
		}
	}

	return 0;
}

BOOL CBonCtrl::GetViewStatusInfo(
	DWORD id,
	float* signal,
	DWORD* space,
	DWORD* ch,
	ULONGLONG* drop,
	ULONGLONG* scramble
	)
{
	if( Lock(L"GetViewStatusInfo") == FALSE ) return FALSE;
	BOOL ret = FALSE;

	this->tsOut.GetErrCount(id, drop, scramble);

	*signal = this->bonUtil.GetSignalLevel();
	this->tsOut.SetSignalLevel(*signal);

	if( this->bonUtil.GetSetCh(space, ch) == TRUE ){
		ret = TRUE;
	}

	UnLock();
	return ret;
}
