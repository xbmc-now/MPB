#include "StdAfx.h"
#include "EpgDataCap_BonMain.h"

#include "EpgDataCap_BonDef.h"
#include "../../Common/CtrlCmdDef.h"
#include "../../Common/CtrlCmdUtil.h"

CEpgDataCap_BonMain::CEpgDataCap_BonMain(void)
{
	this->msgWnd = NULL;
	this->nwCtrlID = 0;
	this->sendUdpFlag = FALSE;
	this->sendTcpFlag = FALSE;

	this->startMargine = 5;
	this->endMargine = 5;
	this->overWriteFlag = FALSE;
	this->enableScrambleFlag = TRUE;
	this->enableEMMFlag = FALSE;

	this->allService = FALSE;
	this->needCaption = TRUE;
	this->needData = FALSE;

	this->BSBasic = TRUE;
	this->CS1Basic = TRUE;
	this->CS2Basic = TRUE;

	this->viewPath = L"";
	this->viewOpt = L"";

	this->lastONID = 0xFFFF;
	this->lastTSID = 0xFFFF;
	this->lastSID = 0xFFFF;

	this->recCtrlID = 0;

	this->currentBonDriver = L"";
	this->outCtrlID = -1;

	this->openWait = 200;
}


CEpgDataCap_BonMain::~CEpgDataCap_BonMain(void)
{
	this->pipeServer.StopServer();
}

void CEpgDataCap_BonMain::SetHwnd(HWND wnd)
{
	this->msgWnd = wnd;
}

//�ݒ���s��
void CEpgDataCap_BonMain::ReloadSetting()
{
	wstring commonIniPath = L"";
	GetCommonIniPath(commonIniPath);

	wstring appIniPath = L"";
	GetModuleIniPath(appIniPath);

	wstring settingPath = L"";
	GetSettingPath(settingPath);

	wstring bonDriverPath = L"";
	GetModuleFolderPath(bonDriverPath);
	bonDriverPath += BON_DLL_FOLDER;

	this->bonCtrl.SetSettingFolder( settingPath.c_str(), bonDriverPath.c_str() );

	this->recFolderList.clear();
	int iNum = GetPrivateProfileInt( L"SET", L"RecFolderNum", 0, commonIniPath.c_str() );
	if( iNum == 0 ){
		this->recFolderList.push_back( settingPath );
	}else{
		for( int i = 0; i < iNum; i++ ){
			CString key = L"";
			key.Format(L"RecFolderPath%d", i );
			WCHAR wBuff[512]=L"";
			GetPrivateProfileString( L"SET", key, L"", wBuff, 512, commonIniPath.c_str() );

			this->recFolderList.push_back(wBuff);
		}
	}

	this->enableScrambleFlag = GetPrivateProfileInt( L"SET", L"Scramble", 1, appIniPath.c_str() );
	this->enableEMMFlag = GetPrivateProfileInt( L"SET", L"EMM", 0, appIniPath.c_str() );
	this->allService = GetPrivateProfileInt( L"SET", L"AllService", 0, appIniPath.c_str() );
	this->needCaption = GetPrivateProfileInt( L"SET", L"Caption", 1, appIniPath.c_str() );
	this->needData = GetPrivateProfileInt( L"SET", L"Data", 0, appIniPath.c_str() );

	this->startMargine = GetPrivateProfileInt( L"SET", L"StartMargine", 5, appIniPath.c_str() );
	this->endMargine = GetPrivateProfileInt( L"SET", L"EndMargine", 5, appIniPath.c_str() );

	this->overWriteFlag = GetPrivateProfileInt( L"SET", L"OverWrite", 0, appIniPath.c_str() );

	this->BSBasic = GetPrivateProfileInt( L"SET", L"BSBasicOnly", 1, commonIniPath.c_str() );
	this->CS1Basic = GetPrivateProfileInt( L"SET", L"CS1BasicOnly", 1, commonIniPath.c_str() );
	this->CS2Basic = GetPrivateProfileInt( L"SET", L"CS2BasicOnly", 1, commonIniPath.c_str() );

	WCHAR buff[512]=L"";
	GetPrivateProfileString( L"SET", L"ViewPath", L"", buff, 512, appIniPath.c_str() );
	this->viewPath = buff;
	GetPrivateProfileString( L"SET", L"ViewOption", L"", buff, 512, appIniPath.c_str() );
	this->viewOpt = buff;

	this->udpCount = (DWORD)GetPrivateProfileInt( L"SET_UDP", L"Count", 0, appIniPath.c_str() );
	this->tcpCount = (DWORD)GetPrivateProfileInt( L"SET_TCP", L"Count", 0, appIniPath.c_str() );

	if( this->nwCtrlID != 0 ){
		if( this->allService == TRUE ){
			this->bonCtrl.SetServiceID(this->nwCtrlID, 0xFFFF);
		}else{
			this->bonCtrl.SetServiceID(this->nwCtrlID, this->lastSID);
		}
	}

	BOOL epgCapLive = (BOOL)GetPrivateProfileInt( L"SET", L"EpgCapLive", 1, appIniPath.c_str() );
	BOOL epgCapRec = (BOOL)GetPrivateProfileInt( L"SET", L"EpgCapRec", 1, appIniPath.c_str() );
	DWORD epgCapBackStartWaitSec = (DWORD)GetPrivateProfileInt( L"SET", L"EpgCapBackStartWaitSec", 30, appIniPath.c_str() );

	this->bonCtrl.SetBackGroundEpgCap(epgCapLive, epgCapRec, this->BSBasic, this->CS1Basic, this->CS2Basic, epgCapBackStartWaitSec);
	if( this->sendTcpFlag == FALSE && this->sendUdpFlag == FALSE ){
		this->bonCtrl.SetScramble(this->nwCtrlID, this->enableScrambleFlag);
	}
	this->bonCtrl.SetEMMMode(this->enableEMMFlag);

	DWORD tsBuffMaxCount = (DWORD)GetPrivateProfileInt( L"SET", L"TsBuffMaxCount", 5000, appIniPath.c_str() );
	int writeBuffMaxCount = GetPrivateProfileInt( L"SET", L"WriteBuffMaxCount", -1, appIniPath.c_str() );
	this->bonCtrl.SetTsBuffMaxCount(tsBuffMaxCount, writeBuffMaxCount);

	this->openWait = (DWORD)GetPrivateProfileInt( L"SET", L"OpenWait", 200, appIniPath.c_str() );
}

//BonDriver�t�H���_��BonDriver_*.dll���
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonList			[OUT]�����ł���BonDriver�ꗗ�imap�̃L�[ �����C���f�b�N�X�l�Amap�̒l BonDriver�t�@�C�����j
DWORD CEpgDataCap_BonMain::EnumBonDriver(
	map<int, wstring>* bonList
)
{
	return this->bonCtrl.EnumBonDriver(bonList);
}

//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�t�@�C�����Ŏw��j
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonDriverFile	[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃t�@�C����
DWORD CEpgDataCap_BonMain::OpenBonDriver(
	LPCWSTR bonDriverFile
)
{
	DWORD ret = this->bonCtrl.OpenBonDriver(bonDriverFile, this->openWait);
	if( ret == NO_ERR ){
		this->lastONID = 0xFFFF;
		this->lastTSID = 0xFFFF;
		this->lastSID = 0xFFFF;
		this->currentBonDriver = bonDriverFile;
		if( this->nwCtrlID == 0 ){
			if( this->bonCtrl.CreateServiceCtrl(&this->nwCtrlID) == TRUE ){
				this->bonCtrl.SetScramble(this->nwCtrlID, this->enableScrambleFlag);
				this->bonCtrl.SetServiceMode(this->nwCtrlID, this->needCaption, this->needData);
			}
		}else{
			this->bonCtrl.ClearErrCount(this->nwCtrlID);
		}
	}else{
		this->currentBonDriver = L"";
	}
	return ret;
}

//���[�h���Ă���BonDriver�̊J��
//�߂�l�F
// �G���[�R�[�h
DWORD CEpgDataCap_BonMain::CloseBonDriver()
{
	this->currentBonDriver = L"";
	return this->bonCtrl.CloseBonDriver();
}

//�T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceList				[OUT]�T�[�r�X���̃��X�g
DWORD CEpgDataCap_BonMain::GetServiceList(
	vector<CH_DATA4>* serviceList
	)
{
	return this->bonCtrl.GetServiceList(serviceList);
}

//���[�h����BonDriver�̃t�@�C�������擾����i���[�h�������Ă��邩�̔���j
//�߂�l�F
// TRUE�i�����j�FFALSE�iOpen�Ɏ��s���Ă���j
//�����F
// bonDriverFile		[OUT]BonDriver�̃t�@�C����(NULL��)
BOOL CEpgDataCap_BonMain::GetOpenBonDriver(
	wstring* bonDriverFile
	)
{
	return this->bonCtrl.GetOpenBonDriver(bonDriverFile);
}

//�`�����l���ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// ONID			[IN]�ύX�`�����l����orignal_network_id
// TSID			[IN]�ύX�`�����l���̕���transport_stream_id
// SID			[IN]�ύX�`�����l���̕���service_id
DWORD CEpgDataCap_BonMain::SetCh(
	WORD ONID,
	WORD TSID,
	WORD SID
	)
{
	DWORD err = ERR_FALSE;
	if( this->bonCtrl.IsRec() == FALSE ){
		if( this->bonCtrl.GetEpgCapStatus(NULL) == ST_WORKING ){
			this->bonCtrl.StopEpgCap();
		}
		err = this->bonCtrl.SetCh(ONID, TSID, SID);
		if( err == NO_ERR ){
			this->lastONID = ONID;
			this->lastTSID = TSID;
			this->lastSID = SID;

			if( this->nwCtrlID != 0 ){
				if( this->allService == TRUE ){
					this->bonCtrl.SetServiceID(this->nwCtrlID, 0xFFFF);
				}else{
					this->bonCtrl.SetServiceID(this->nwCtrlID, this->lastSID);
				}
			}
		}
	}
	return err;
}

//�`�����l���ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// SID			[IN]�ύX�`�����l����service_id
// SID			[IN]�ύX�`�����l����space
// SID			[IN]�ύX�`�����l����ch
DWORD CEpgDataCap_BonMain::SetCh(
	WORD ONID,
	WORD TSID,
	WORD SID,
	DWORD space,
	DWORD ch
	)
{
	DWORD err = ERR_FALSE;
	if( this->bonCtrl.IsRec() == FALSE ){
		if( this->bonCtrl.GetEpgCapStatus(NULL) == ST_WORKING ){
			this->bonCtrl.StopEpgCap();
		}
		err = this->bonCtrl.SetCh(space, ch, SID);
		if( err == NO_ERR ){
			this->lastONID = ONID;
			this->lastTSID = TSID;
			this->lastSID = SID;

			if( this->nwCtrlID != 0 ){
				if( this->allService == TRUE ){
					this->bonCtrl.SetServiceID(this->nwCtrlID, 0xFFFF);
				}else{
					this->bonCtrl.SetServiceID(this->nwCtrlID, this->lastSID);
				}
			}
		}
	}
	return err;
}

//���݂̃T�[�r�X�擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// ONID			[IN]���݂�orignal_network_id
// TSID			[IN]���݂�transport_stream_id
// SID			[IN]���݂�service_id
void CEpgDataCap_BonMain::GetCh(
	WORD* ONID,
	WORD* TSID,
	WORD* SID
	)
{
	*ONID = this->lastONID;
	*TSID = this->lastTSID;
	*SID = this->lastSID;
}

BOOL CEpgDataCap_BonMain::GetCh(
	DWORD* space,
	DWORD* ch
	)
{
	return this->bonCtrl.GetCh(space, ch);
}

//�`�����l���ύX�����ǂ���
//�߂�l�F
// TRUE�i�ύX���j�AFALSE�i�����j
BOOL CEpgDataCap_BonMain::IsChChanging(BOOL* chChgErr)
{
	return this->bonCtrl.IsChChanging(chChgErr);
}

void CEpgDataCap_BonMain::SetSID(
	WORD SID
	)
{
	this->lastSID = SID;
}

//UDP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// enableFlag		[IN]TRUE�i�J�n�j�AFALSE�i��~�j
BOOL CEpgDataCap_BonMain::SendUDP(
	BOOL enableFlag
	)
{
	this->udpSendList.clear();
	if( enableFlag == TRUE ){
		wstring appIniPath = L"";
		GetModuleIniPath(appIniPath);

		int udpCount = GetPrivateProfileInt( L"SET_UDP", L"Count", 0, appIniPath.c_str() );
		for( int i = 0; i < udpCount; i++ ){
			NW_SEND_INFO item;

			CString key = L"";
			key.Format(L"IP%d",i);
			item.ip = GetPrivateProfileInt( L"SET_UDP", key.GetBuffer(0), 2130706433, appIniPath.c_str() );
			key.Format(L"Port%d",i);
			item.port = GetPrivateProfileInt( L"SET_UDP", key.GetBuffer(0), 1234, appIniPath.c_str() );
			Format(item.ipString, L"%d.%d.%d.%d",
				(item.ip&0xFF000000)>>24,
				(item.ip&0x00FF0000)>>16,
				(item.ip&0x0000FF00)>>8,
				(item.ip&0x000000FF) );
			key.Format(L"BroadCast%d",i);
			item.broadcastFlag = GetPrivateProfileInt( L"SET_UDP", key.GetBuffer(0), 0, appIniPath.c_str() );

			udpSendList.push_back(item);
		}
	}

	BOOL ret = TRUE;
	if( udpSendList.size() == 0 ){
		this->sendUdpFlag = FALSE;
		if( this->nwCtrlID != 0 ){
			this->bonCtrl.SendUdp(this->nwCtrlID,NULL);
/*			if( this->sendUdpFlag == FALSE && this->sendTcpFlag == FALSE ){
				this->bonCtrl.DeleteServiceCtrl(this->nwCtrlID);
				this->nwCtrlID = 0;
			}*/
		}
	}else{
		this->sendUdpFlag = TRUE;
		if( this->nwCtrlID == 0 ){
			if( this->bonCtrl.CreateServiceCtrl(&this->nwCtrlID) == FALSE ){
				this->sendTcpFlag = FALSE;
				return FALSE;
			}
		}
		if( this->nwCtrlID != 0 ){
			if( this->allService == TRUE ){
				this->bonCtrl.SetServiceID(this->nwCtrlID, 0xFFFF);
			}else{
				this->bonCtrl.SetServiceID(this->nwCtrlID, this->lastSID);
			}
		}
		this->bonCtrl.SetScramble(this->nwCtrlID, this->enableScrambleFlag);
		this->bonCtrl.SetServiceMode(this->nwCtrlID, this->needCaption, this->needData);
		ret = this->bonCtrl.SendUdp(this->nwCtrlID,&this->udpSendList);
	}
	return ret;
}

//TCP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// enableFlag		[IN]TRUE�i�J�n�j�AFALSE�i��~�j
BOOL CEpgDataCap_BonMain::SendTCP(
	BOOL enableFlag
	)
{
	this->tcpSendList.clear();
	if( enableFlag == TRUE ){
		wstring appIniPath = L"";
		GetModuleIniPath(appIniPath);

		int tcpCount = GetPrivateProfileInt( L"SET_TCP", L"Count", 0, appIniPath.c_str() );
		for( int i = 0; i < tcpCount; i++ ){
			NW_SEND_INFO item;

			CString key = L"";
			key.Format(L"IP%d",i);
			item.ip = GetPrivateProfileInt( L"SET_TCP", key.GetBuffer(0), 2130706433, appIniPath.c_str() );
			key.Format(L"Port%d",i);
			item.port = GetPrivateProfileInt( L"SET_TCP", key.GetBuffer(0), 2230, appIniPath.c_str() );
			Format(item.ipString, L"%d.%d.%d.%d",
				(item.ip&0xFF000000)>>24,
				(item.ip&0x00FF0000)>>16,
				(item.ip&0x0000FF00)>>8,
				(item.ip&0x000000FF) );
			item.broadcastFlag = 0;

			tcpSendList.push_back(item);
		}
	}

	BOOL ret = TRUE;
	if( tcpSendList.size() == 0 ){
		this->sendTcpFlag = FALSE;
		if( this->nwCtrlID != 0 ){
			this->bonCtrl.SendTcp(this->nwCtrlID,NULL);
/*			if( this->sendUdpFlag == FALSE && this->sendTcpFlag == FALSE ){
				this->bonCtrl.DeleteServiceCtrl(this->nwCtrlID);
				this->nwCtrlID = 0;
			}*/
		}
	}else{
		this->sendTcpFlag = TRUE;
		if( this->nwCtrlID == 0 ){
			if( this->bonCtrl.CreateServiceCtrl(&this->nwCtrlID) == FALSE ){
				this->sendTcpFlag = FALSE;
				return FALSE;
			}
		}
		if( this->nwCtrlID != 0 ){
			if( this->allService == TRUE ){
				this->bonCtrl.SetServiceID(this->nwCtrlID, 0xFFFF);
			}else{
				this->bonCtrl.SetServiceID(this->nwCtrlID, this->lastSID);
			}
		}
		this->bonCtrl.SetScramble(this->nwCtrlID, this->enableScrambleFlag);
		this->bonCtrl.SetServiceMode(this->nwCtrlID, this->needCaption, this->needData);
		ret = this->bonCtrl.SendTcp(this->nwCtrlID,&this->tcpSendList);
	}
	return ret;
}

//UDP���M�̐ݒ萔���擾
DWORD CEpgDataCap_BonMain::GetCountUDP()
{
	return this->udpCount;
}

//TCP���M�̐ݒ萔���擾
DWORD CEpgDataCap_BonMain::GetCountTCP()
{
	return this->tcpCount;
}

//UDP�̑��M����ꗗ�擾
BOOL CEpgDataCap_BonMain::GetSendUDPList(
	vector<NW_SEND_INFO>* sendList
	)
{
	if( this->udpSendList.size() == 0 ){
		return FALSE;
	}
	*sendList = this->udpSendList;

	return TRUE;
}

//TCP�̑��M����ꗗ�擾
BOOL CEpgDataCap_BonMain::GetSendTCPList(
	vector<NW_SEND_INFO>* sendList
	)
{
	if( this->tcpSendList.size() == 0 ){
		return FALSE;
	}
	*sendList = this->tcpSendList;

	return TRUE;
}

//�w��T�[�r�X�̌���or����EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceID				[IN]�擾�Ώۂ�ServiceID
// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap_BonMain::GetEpgInfo(
	BOOL nextFlag,
	CString* epgInfo
	)
{
	WORD ONID = 0;
	WORD TSID = 0;
	if( this->bonCtrl.IsChChanging(NULL) == TRUE ){
		return FALSE;
	}
	if( this->bonCtrl.GetStreamID(&ONID, &TSID) == FALSE ){
		return FALSE;
	}
	EPGDB_EVENT_INFO info;

	DWORD ret = this->bonCtrl.GetEpgInfo(ONID, TSID, this->lastSID, nextFlag, &info);

	if(epgInfo != NULL ){
		*epgInfo = L"";
		if( ret == NO_ERR ){
			wstring epgText = L"";
			_ConvertEpgInfoText(&info, epgText);
			*epgInfo = epgText.c_str();
		}
	}

	return ret;
}

//�V�O�i�����x���̎擾
//�߂�l�F
// �V�O�i�����x��
float CEpgDataCap_BonMain::GetSignalLevel()
{
	return this->bonCtrl.GetSignalLevel();
}

//�G���[�J�E���g���N���A����
void CEpgDataCap_BonMain::ClearErrCount(
	)
{
	if( this->nwCtrlID != 0 ){
		this->bonCtrl.ClearErrCount(this->nwCtrlID);
	}
}

//�h���b�v�ƃX�N�����u���̃J�E���g���擾����
//�����F
// drop				[OUT]�h���b�v��
// scramble			[OUT]�X�N�����u����
void CEpgDataCap_BonMain::GetErrCount(
	ULONGLONG* drop,
	ULONGLONG* scramble
	)
{
	if( this->nwCtrlID != 0 ){
		this->bonCtrl.GetErrCount(this->nwCtrlID, drop, scramble);
	}

}

//�^����J�n����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
BOOL CEpgDataCap_BonMain::StartRec(
	)
{
	if( this->bonCtrl.IsRec() == TRUE || this->recCtrlID != 0 ){
		return FALSE;
	}

	if( this->bonCtrl.CreateServiceCtrl(&this->recCtrlID) == FALSE ){
		return FALSE;
	}
	if( this->allService == TRUE ){
		this->bonCtrl.SetServiceID(this->recCtrlID, 0xFFFF);
	}else{
		this->bonCtrl.SetServiceID(this->recCtrlID, this->lastSID);
	}
	this->bonCtrl.SetScramble(this->recCtrlID, this->enableScrambleFlag);
	this->bonCtrl.SetServiceMode(this->recCtrlID, this->needCaption, this->needData);

	vector<CH_DATA4> serviceList;
	this->bonCtrl.GetServiceList(&serviceList);
	wstring serviceName = L"";
	Format(serviceName, L"%04X", this->lastSID);
	for( size_t i=0; i<serviceList.size(); i++){
		if( serviceList[i].originalNetworkID == this->lastONID &&
			serviceList[i].transportStreamID == this->lastTSID &&
			serviceList[i].serviceID == this->lastSID ){
				serviceName = serviceList[i].serviceName;
		}
	}

	wstring fileName = L"";
	SYSTEMTIME now;
	GetLocalTime(&now);
	Format(fileName, L"%04d%02d%02d-%02d%02d%02d-%s.ts",
		now.wYear,
		now.wMonth,
		now.wDay,
		now.wHour,
		now.wMinute,
		now.wSecond,
		serviceName.c_str()
		);
	ChkFileName(fileName);

	vector<REC_FILE_SET_INFO> saveFolder;
	REC_FILE_SET_INFO forderItem;
	forderItem.recFolder = this->recFolderList[0];
	saveFolder.push_back(forderItem);

	this->bonCtrl.StartSave(this->recCtrlID, fileName, this->overWriteFlag, FALSE, 0,0,0,0, 0, &saveFolder, &this->recFolderList);

	return TRUE;
}

//�^����~����
// TRUE�i�����j�AFALSE�i���s�j
BOOL CEpgDataCap_BonMain::StopRec()
{
	if( this->recCtrlID != 0 ){
		this->bonCtrl.EndSave(this->recCtrlID);
		this->bonCtrl.DeleteServiceCtrl(this->recCtrlID);
		this->recCtrlID = 0;
	}
	return TRUE;
}

//�^�撆���ǂ������擾����
// TRUE�i�^�撆�j�AFALSE�i�^�悵�Ă��Ȃ��j
BOOL CEpgDataCap_BonMain::IsRec()
{
	return this->bonCtrl.IsRec();
}

//�\��^����~����
void CEpgDataCap_BonMain::StopReserveRec()
{
	map<DWORD,DWORD>::iterator itr;
	for( itr = this->ctrlMap.begin(); itr != this->ctrlMap.end(); itr++ ){
		this->bonCtrl.DeleteServiceCtrl(itr->second);
	}
	this->ctrlMap.clear();
}

//�`�����l���X�L�������J�n����
//�߂�l�F
// �G���[�R�[�h
DWORD CEpgDataCap_BonMain::StartChScan()
{
	return this->bonCtrl.StartChScan();
}

//�`�����l���X�L�������L�����Z������
//�߂�l�F
// �G���[�R�[�h
DWORD CEpgDataCap_BonMain::StopChScan()
{
	return this->bonCtrl.StopChScan();
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
DWORD CEpgDataCap_BonMain::GetChScanStatus(
	DWORD* space,
	DWORD* ch,
	wstring* chName,
	DWORD* chkNum,
	DWORD* totalNum
	)
{
	return this->bonCtrl.GetChScanStatus(space, ch, chName, chkNum, totalNum);
}

//EPG�擾���J�n����
//�߂�l�F
// �G���[�R�[�h
DWORD CEpgDataCap_BonMain::StartEpgCap(
	)
{
	vector<EPGCAP_SERVICE_INFO> chList;
	this->bonCtrl.GetEpgCapService(&chList);
	if( chList.size() == 0 ){
		return ERR_FALSE;
	}
	return this->bonCtrl.StartEpgCap(&chList, this->BSBasic, this->CS1Basic, this->CS2Basic);
}

//EPG�擾���~����
//�߂�l�F
// �G���[�R�[�h
DWORD CEpgDataCap_BonMain::StopEpgCap(
	)
{
	return this->bonCtrl.StopEpgCap();
}

//EPG�擾�̃X�e�[�^�X���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// info			[OUT]�擾���̃T�[�r�X
DWORD CEpgDataCap_BonMain::GetEpgCapStatus(
	EPGCAP_SERVICE_INFO* info
	)
{
	return this->bonCtrl.GetEpgCapStatus(info);
}

//View�A�v���̋N�����s��
void CEpgDataCap_BonMain::ViewAppOpen()
{
	if( this->viewPath.size() > 0 ){
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);

		CString strOpen;
		strOpen.Format(L"\"%s\" %s", this->viewPath.c_str(), this->viewOpt.c_str());

		CreateProcess( NULL, strOpen.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}

void CEpgDataCap_BonMain::StartServer()
{
	wstring pipeName = L"";
	wstring eventName = L"";

	Format(pipeName, L"%s%d", CMD2_VIEW_CTRL_PIPE, GetCurrentProcessId());
	Format(eventName, L"%s%d", CMD2_VIEW_CTRL_WAIT_CONNECT, GetCurrentProcessId());

	OutputDebugString(pipeName.c_str());
	OutputDebugString(eventName.c_str());
	this->pipeServer.StartServer(eventName.c_str(), pipeName.c_str(), CtrlCmdCallback, this, 0, GetCurrentProcessId());
}

void CEpgDataCap_BonMain::StopServer()
{
	this->pipeServer.StopServer();
}

void CEpgDataCap_BonMain::StartTimeShift()
{
	wstring saveFile = L"";
	DWORD ctrlID = 0;
	if( this->recCtrlID != 0 ){
		BOOL subRec = FALSE;
		this->bonCtrl.GetSaveFilePath(this->recCtrlID, &saveFile, &subRec);
		ctrlID = this->recCtrlID;
	}else if(this->bonCtrl.IsRec() == TRUE){
		map<DWORD,DWORD>::iterator itr;
		itr = this->ctrlMap.begin();
		BOOL subRec = FALSE;
		this->bonCtrl.GetSaveFilePath(itr->second, &saveFile, &subRec);
		ctrlID = itr->second;
	}
	if( saveFile.size() > 0 ){
		wstring appPath = L"";
		GetModuleFolderPath(appPath);
		appPath += L"\\FilePlay.exe";

		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);

		CString strOpen;
		strOpen.Format(L"\"%s\" \"%s\" -pid %d -ctrlid %d", appPath.c_str(), saveFile.c_str(), GetCurrentProcessId(), ctrlID);

		CreateProcess( NULL, strOpen.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}

BOOL CEpgDataCap_BonMain::GetViewStatusInfo(
	float* signal,
	DWORD* space,
	DWORD* ch,
	ULONGLONG* drop,
	ULONGLONG* scramble,
	vector<NW_SEND_INFO>* sendUdpList,
	vector<NW_SEND_INFO>* sendTcpList
	)
{
	*sendUdpList = this->udpSendList;
	*sendTcpList = this->tcpSendList;
	return this->bonCtrl.GetViewStatusInfo(this->nwCtrlID, signal, space, ch, drop, scramble);
}

int CALLBACK CEpgDataCap_BonMain::CtrlCmdCallback(void* param, CMD_STREAM* cmdParam, CMD_STREAM* resParam)
{
	CEpgDataCap_BonMain* sys = (CEpgDataCap_BonMain*)param;

	resParam->dataSize = 0;
	resParam->param = CMD_ERR;

	switch( cmdParam->param ){
	case CMD2_VIEW_APP_SET_BONDRIVER:
		OutputDebugString(L"CMD2_VIEW_APP_SET_BONDRIVER");
		{
			wstring val;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				sys->CloseBonDriver();
				if( sys->OpenBonDriver(val.c_str()) == NO_ERR ){
					resParam->param = CMD_SUCCESS;
					PostMessage(sys->msgWnd, WM_CHG_TUNER, 0, 0);
				}
			}
		}
		break;
	case CMD2_VIEW_APP_GET_BONDRIVER:
		OutputDebugString(L"CMD2_VIEW_APP_GET_BONDRIVER");
		{
			if( sys->currentBonDriver.size() > 0 ){
				resParam->dataSize = GetVALUESize(sys->currentBonDriver);
				resParam->data = new BYTE[resParam->dataSize];
				if( WriteVALUE(sys->currentBonDriver, resParam->data, resParam->dataSize, NULL) == TRUE ){
					resParam->param = CMD_SUCCESS;
				}
			}
		}
		break;
	case CMD2_VIEW_APP_SET_CH:
		OutputDebugString(L"CMD2_VIEW_APP_SET_CH");
		{
			if( sys->bonCtrl.IsRec() == FALSE ){
				if( sys->bonCtrl.GetEpgCapStatus(NULL) == ST_WORKING ){
					sys->bonCtrl.StopEpgCap();
				}
				SET_CH_INFO val;
				if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
					if( sys->bonCtrl.IsRec() == FALSE ){
						if( val.useSID == TRUE ){
							if(sys->SetCh(val.ONID, val.TSID, val.SID) == TRUE){
								sys->lastONID = val.ONID;
								sys->lastTSID = val.TSID;
								sys->lastSID = val.SID;

								resParam->param = CMD_SUCCESS;
								PostMessage(sys->msgWnd, WM_CHG_CH, 0, 0);
							}
						}else if( val.useBonCh == TRUE ){
							if(sys->bonCtrl.SetCh(val.space, val.ch) == TRUE){

								resParam->param = CMD_SUCCESS;
								PostMessage(sys->msgWnd, WM_CHG_CH, 0, 0);
							}
						}
					}
				}
			}
		}
		break;
	case CMD2_VIEW_APP_GET_DELAY:
		{
			int val = sys->bonCtrl.GetTimeDelay();

			resParam->dataSize = GetVALUESize(val);
			resParam->data = new BYTE[resParam->dataSize];
			if( WriteVALUE(val, resParam->data, resParam->dataSize, NULL) == TRUE ){
				resParam->param = CMD_SUCCESS;
			}
		}
		break;
	case CMD2_VIEW_APP_GET_STATUS:
		{
			DWORD val = VIEW_APP_ST_NORMAL;
			if( sys->currentBonDriver.size() == 0 ){
				val = VIEW_APP_ST_ERR_BON;
			}else if( sys->bonCtrl.IsRec() == TRUE ){
				val = VIEW_APP_ST_REC;
			}else if( sys->bonCtrl.GetEpgCapStatus(NULL) == ST_WORKING ){
				val = VIEW_APP_ST_GET_EPG;
			}else{
				//VIEW_APP_ST_NORMAL
				BOOL chChgErr = FALSE;
				if(sys->IsChChanging(&chChgErr) == FALSE ){
					if( chChgErr == TRUE ){
						val = VIEW_APP_ST_ERR_CH_CHG;
					}
				}
			}
			resParam->dataSize = GetVALUESize(val);
			resParam->data = new BYTE[resParam->dataSize];
			if( WriteVALUE(val, resParam->data, resParam->dataSize, NULL) == TRUE ){
				resParam->param = CMD_SUCCESS;
			}
		}
		break;
	case CMD2_VIEW_APP_CLOSE:
		OutputDebugString(L"CMD2_VIEW_APP_CLOSE");
		{
			map<DWORD,DWORD>::iterator itr;
			for( itr = sys->ctrlMap.begin(); itr != sys->ctrlMap.end(); itr++ ){
				sys->bonCtrl.DeleteServiceCtrl(itr->second);
			}
			sys->ctrlMap.clear();
			if( sys->recCtrlID != 0 ){
				sys->bonCtrl.DeleteServiceCtrl(sys->recCtrlID);
				sys->recCtrlID = 0;
			}
			if( sys->nwCtrlID != 0 ){
				sys->bonCtrl.DeleteServiceCtrl(sys->nwCtrlID);
				sys->nwCtrlID = 0;
			}
			resParam->param = CMD_SUCCESS;
			PostMessage(sys->msgWnd, WM_CLOSE, 0, 0);
		}
		break;
	case CMD2_VIEW_APP_SET_ID:
		OutputDebugString(L"CMD2_VIEW_APP_SET_ID");
		{
			if( ReadVALUE(&sys->outCtrlID, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				resParam->param = CMD_SUCCESS;
			}
		}
		break;
	case CMD2_VIEW_APP_GET_ID:
		OutputDebugString(L"CMD2_VIEW_APP_GET_ID");
		{
			resParam->dataSize = GetVALUESize(sys->outCtrlID);
			resParam->data = new BYTE[resParam->dataSize];
			if( WriteVALUE(sys->outCtrlID, resParam->data, resParam->dataSize, NULL) == TRUE ){
				resParam->param = CMD_SUCCESS;
			}
		}
		break;
	case CMD2_VIEW_APP_SET_STANDBY_REC:
		OutputDebugString(L"CMD2_VIEW_APP_SET_STANDBY_REC");
		{
			DWORD val = 0;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				resParam->param = CMD_SUCCESS;
				PostMessage(sys->msgWnd, WM_RESERVE_REC_STANDBY, val, 0);
			}
		}
		break;
	case CMD2_VIEW_APP_CREATE_CTRL:
		OutputDebugString(L"CMD2_VIEW_APP_CREATE_CTRL");
		{
			DWORD val = 0;
			if( sys->bonCtrl.CreateServiceCtrl(&val) == TRUE ){
				sys->ctrlMap.insert(pair<DWORD,DWORD>(val, val));
				resParam->dataSize = GetVALUESize(val);
				resParam->data = new BYTE[resParam->dataSize];
				if( WriteVALUE(val, resParam->data, resParam->dataSize, NULL) == TRUE ){
					resParam->param = CMD_SUCCESS;
				}
			}
		}
		break;
	case CMD2_VIEW_APP_DELETE_CTRL:
		OutputDebugString(L"CMD2_VIEW_APP_DELETE_CTRL");
		{
			DWORD val = 0;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				if( sys->bonCtrl.DeleteServiceCtrl(val) == TRUE ){
					map<DWORD,DWORD>::iterator itr;
					itr = sys->ctrlMap.find(val);
					if( itr != sys->ctrlMap.end() ){
						sys->ctrlMap.erase(itr);
					}
					resParam->param = CMD_SUCCESS;

					if( sys->ctrlMap.size() > 0 ){
						WORD sid = 0xFFFF;
						sys->bonCtrl.GetServiceID(sys->ctrlMap.begin()->first, &sid);
						sys->bonCtrl.SetServiceID(sys->nwCtrlID, sid);
						if( sid != 0xFFFF ){
							sys->lastSID = sid;
							PostMessage(sys->msgWnd, WM_CHG_CH, 0, 0);
						}
					}
				}
			}
		}
		break;
	case CMD2_VIEW_APP_SET_CTRLMODE:
		OutputDebugString(L"CMD2_VIEW_APP_SET_CTRLMODE");
		{
			SET_CTRL_MODE val;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				if(val.enableScramble == 0 && sys->nwCtrlID != 0){
					sys->bonCtrl.SetScramble(sys->nwCtrlID, FALSE);
				}
				sys->bonCtrl.SetScramble(val.ctrlID, val.enableScramble);
				sys->bonCtrl.SetServiceMode(val.ctrlID, val.enableCaption, val.enableData);
				sys->bonCtrl.SetServiceID(val.ctrlID, val.SID);
				
				resParam->param = CMD_SUCCESS;
			}
		}
		break;
	case CMD2_VIEW_APP_REC_START_CTRL:
		OutputDebugString(L"CMD2_VIEW_APP_REC_START_CTRL");
		{
			SET_CTRL_REC_PARAM val;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				BOOL overWrite = sys->overWriteFlag;
				if( val.overWriteFlag != 2 ){
					overWrite = val.overWriteFlag;
				}
				sys->bonCtrl.ClearErrCount(val.ctrlID);
				if(sys->bonCtrl.StartSave(val.ctrlID, val.fileName, overWrite, val.pittariFlag, val.pittariONID, val.pittariTSID, val.pittariSID, val.pittariEventID, val.createSize, &val.saveFolder, &sys->recFolderList) == TRUE){
					resParam->param = CMD_SUCCESS;
					PostMessage(sys->msgWnd, WM_RESERVE_REC_START, 0, 0);
				}
			}
		}
		break;
	case CMD2_VIEW_APP_REC_STOP_CTRL:
		OutputDebugString(L"CMD2_VIEW_APP_REC_STOP_CTRL");
		{
			SET_CTRL_REC_STOP_PARAM val;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				wstring saveFile = L"";
				BOOL subRec = FALSE;
				sys->bonCtrl.GetSaveFilePath(val.ctrlID, &saveFile, &subRec);
				if( saveFile.size() > 0 && val.saveErrLog == 1 ){
					wstring iniCommonPath = L"";
					GetCommonIniPath(iniCommonPath);

					WCHAR buff[512] = L"";
					GetPrivateProfileString(L"SET", L"RecInfoFolder", L"", buff, 512, iniCommonPath.c_str());
					wstring infoFolder = buff;
					ChkFolderPath(infoFolder);

					if( infoFolder.size() > 0 ){
						wstring tsFileName = L"";
						GetFileName(saveFile, tsFileName);
						wstring saveFileErr = L"";
						Format(saveFileErr, L"%s\\%s.err", infoFolder.c_str(), tsFileName.c_str());
						sys->bonCtrl.SaveErrCount(val.ctrlID, saveFileErr);
					}else{
						wstring saveFileErr = saveFile;
						saveFileErr += L".err";
						sys->bonCtrl.SaveErrCount(val.ctrlID, saveFileErr);
					}
				}
				SET_CTRL_REC_STOP_RES_PARAM resVal;
				resVal.recFilePath = saveFile;
				resVal.drop = 0;
				resVal.scramble = 0;
				resVal.subRecFlag = (BYTE)subRec;
				sys->bonCtrl.GetErrCount(val.ctrlID, &resVal.drop, &resVal.scramble);
				if(sys->bonCtrl.EndSave(val.ctrlID) == TRUE){
					resParam->dataSize = GetVALUESize(&resVal);
					resParam->data = new BYTE[resParam->dataSize];
					if( WriteVALUE(&resVal, resParam->data, resParam->dataSize, NULL) == TRUE ){
						resParam->param = CMD_SUCCESS;
					}
					if( sys->ctrlMap.size() == 1 ){
						PostMessage(sys->msgWnd, WM_RESERVE_REC_STOP, 0, 0);
					}
				}
			}
		}
		break;
	case CMD2_VIEW_APP_REC_FILE_PATH:
		OutputDebugString(L"CMD2_VIEW_APP_REC_FILE_PATH");
		{
			DWORD val = 0;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				wstring saveFile = L"";
				BOOL subRec;
				sys->bonCtrl.GetSaveFilePath(val, &saveFile, &subRec);
				if( saveFile.size() > 0 ){
					resParam->dataSize = GetVALUESize(saveFile);
					resParam->data = new BYTE[resParam->dataSize];
					if( WriteVALUE(saveFile, resParam->data, resParam->dataSize, NULL) == TRUE ){
						resParam->param = CMD_SUCCESS;
					}
				}
			}
		}
		break;
	case CMD2_VIEW_APP_EPGCAP_START:
		OutputDebugString(L"CMD2_VIEW_APP_EPGCAP_START");
		{
			vector<SET_CH_INFO> val;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				vector<EPGCAP_SERVICE_INFO> chList;
				for( size_t i=0; i<val.size(); i++ ){
					EPGCAP_SERVICE_INFO item;
					item.ONID = val[i].ONID;
					item.TSID = val[i].TSID;
					item.SID = val[i].SID;
					chList.push_back(item);
				}
				if( sys->bonCtrl.StartEpgCap(&chList, sys->BSBasic, sys->CS1Basic, sys->CS2Basic) == NO_ERR ){
					PostMessage(sys->msgWnd, WM_RESERVE_EPGCAP_START, 0, 0);
					
					resParam->param = CMD_SUCCESS;
				}
			}
		}
		break;
	case CMD2_VIEW_APP_EPGCAP_STOP:
		OutputDebugString(L"CMD2_VIEW_APP_EPGCAP_STOP");
		{
			sys->bonCtrl.StopEpgCap();
			resParam->param = CMD_SUCCESS;
			PostMessage(sys->msgWnd, WM_RESERVE_EPGCAP_STOP, 0, 0);
		}
		break;
	case CMD2_VIEW_APP_REC_STOP_ALL:
		OutputDebugString(L"CMD2_VIEW_APP_REC_STOP_ALL");
		{
			DWORD ret = CMD_SUCCESS;
			if( sys->recCtrlID != 0 ){
				if(sys->bonCtrl.EndSave(sys->recCtrlID) == FALSE ){
					ret = CMD_ERR;
				}
				sys->bonCtrl.DeleteServiceCtrl(sys->recCtrlID);
				sys->recCtrlID = 0;
			}
			map<DWORD,DWORD>::iterator itr;
			for( itr = sys->ctrlMap.begin(); itr != sys->ctrlMap.end(); itr++ ){
				sys->bonCtrl.DeleteServiceCtrl(itr->second);
			}
			sys->ctrlMap.clear();

			resParam->param = ret;
			PostMessage(sys->msgWnd, WM_RESERVE_REC_STOP, 0, 0);
		}
		break;
	case CMD2_VIEW_APP_SEARCH_EVENT:
		{
			SEARCH_EPG_INFO_PARAM key;
			if( ReadVALUE(&key, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				EPGDB_EVENT_INFO epgInfo;
				if( sys->bonCtrl.SearchEpgInfo(key.ONID, key.TSID, key.SID, key.eventID, key.pfOnlyFlag, &epgInfo) == TRUE ){
					resParam->dataSize = GetVALUESize(&epgInfo);
					resParam->data = new BYTE[resParam->dataSize];
					if( WriteVALUE(&epgInfo, resParam->data, resParam->dataSize, NULL) == TRUE ){
						resParam->param = CMD_SUCCESS;
					}
				}
			}
		}
		break;
	case CMD2_VIEW_APP_GET_EVENT_PF:
		{
			GET_EPG_PF_INFO_PARAM key;
			if( ReadVALUE(&key, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				EPGDB_EVENT_INFO epgInfo;
				if( sys->bonCtrl.GetEpgInfo(key.ONID, key.TSID, key.SID, key.pfNextFlag, &epgInfo) == TRUE ){
					resParam->dataSize = GetVALUESize(&epgInfo);
					resParam->data = new BYTE[resParam->dataSize];
					if( WriteVALUE(&epgInfo, resParam->data, resParam->dataSize, NULL) == TRUE ){
						resParam->param = CMD_SUCCESS;
					}
				}
			}
		}
		break;
	case CMD2_VIEW_APP_EXEC_VIEW_APP:
		{
			sys->ViewAppOpen();
			resParam->param = CMD_SUCCESS;
		}
		break;
	case CMD2_VIEW_APP_REC_WRITE_SIZE:
		{
			DWORD val = 0;
			if( ReadVALUE(&val, cmdParam->data, cmdParam->dataSize, NULL ) == TRUE ){
				__int64 writeSize = -1;
				sys->bonCtrl.GetRecWriteSize(val, &writeSize);
				resParam->dataSize = GetVALUESize(writeSize);
				resParam->data = new BYTE[resParam->dataSize];
				if( WriteVALUE(writeSize, resParam->data, resParam->dataSize, NULL) == TRUE ){
					resParam->param = CMD_SUCCESS;
				}
			}
		}
		break;
	default:
		_OutputDebugString(L"err default cmd %d\r\n", cmdParam->param);
		resParam->param = CMD_NON_SUPPORT;
		break;
	}

	return 0;
}
