#include "StdAfx.h"
#include "OneServiceUtil.h"


COneServiceUtil::COneServiceUtil(void)
{
	this->id = 0;
	this->SID = 0xFFFF;

	this->sendUdp = NULL;
	this->sendTcp = NULL;
	this->writeFile = NULL;

	this->buff = 0;
	this->buffSize = 0;
	this->buffWriteSize = 0;

	this->pmtPID = 0xFFFF;

	this->enableScramble = TRUE;
	this->epgUtil = NULL;

	this->pittariStart = FALSE;
	this->pittariEndChk = FALSE;
	this->maxBuffCount = -1;
}


COneServiceUtil::~COneServiceUtil(void)
{
	SAFE_DELETE(this->sendUdp);
	SAFE_DELETE(this->sendTcp);
	SAFE_DELETE(this->writeFile);

	SAFE_DELETE_ARRAY(this->buff);
}

void COneServiceUtil::SetEpgUtil(
	CEpgDataCap3Util* epgUtil
	)
{
	this->epgUtil = epgUtil;
}


//����ID�̐ݒ�
//�����F
// id			[IN]����ID
void COneServiceUtil::SetID(
	DWORD id
	)
{
	this->id = id;
}

//����ID�̎擾
//�߂�l�F
// ����ID
DWORD COneServiceUtil::GetID()
{
	return this->id;
}

//�����Ώ�ServiceID��ݒ�
//�����F
// SID			[IN]ServiceID
void COneServiceUtil::SetSID(
	WORD SID
)
{
	if( this->SID != SID ){
		this->pmtPID = 0xFFFF;
		this->emmPIDMap.clear();

		this->dropCount.Clear();
	}
	this->SID = SID;
}

//�ݒ肳��Ă鏈���Ώۂ�ServiceID���擾
//�߂�l�F
// ServiceID
WORD COneServiceUtil::GetSID()
{
	return this->SID;
}

//UDP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// id			[IN]���䎯��ID
// sendList		[IN/OUT]���M�惊�X�g�BNULL�Œ�~�BPort�͎��ۂɑ��M�Ɏg�p����Port���Ԃ�B
BOOL COneServiceUtil::SendUdp(
	vector<NW_SEND_INFO>* sendList
	)
{
	if( this->sendUdp != NULL ){
		this->sendUdp->CloseUpload();
	}

	if(udpPortMutex.size() != 0){
		for( int i=0; i<(int)udpPortMutex.size(); i++ ){
			::ReleaseMutex(udpPortMutex[i]);
			::CloseHandle(udpPortMutex[i]);
		}
		udpPortMutex.clear();
	}

	if( sendList != NULL ){
		if( this->sendUdp == NULL ){
			this->sendUdp = new CSendUDP;
		}
		for( size_t i=0; i<sendList->size(); i++ ){
			wstring key = L"";

			while(1){
				Format(key, L"%s%d_%d", MUTEX_UDP_PORT_NAME, (*sendList)[i].ip, (*sendList)[i].port );
				HANDLE mutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, key.c_str());
		
				if(mutex){
					::CloseHandle(mutex);
					(*sendList)[i].port++;
				}else{
					break;
				}
			}

			HANDLE portMutex = _CreateMutex( TRUE, key.c_str());
			_OutputDebugString(L"%s\r\n", key.c_str());
			udpPortMutex.push_back(portMutex);
		}

		this->sendUdp->StartUpload(sendList);
	}else{
		SAFE_DELETE(this->sendUdp);
	}

	return TRUE;
}

//TCP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// id			[IN]���䎯��ID
// sendList		[IN/OUT]���M�惊�X�g�BNULL�Œ�~�BPort�͎��ۂɑ��M�Ɏg�p����Port���Ԃ�B
BOOL COneServiceUtil::SendTcp(
	vector<NW_SEND_INFO>* sendList
	)
{
	if( this->sendTcp != NULL ){
		this->sendTcp->CloseUpload();
	}

	if(tcpPortMutex.size() != 0){
		for( int i=0; i<(int)tcpPortMutex.size(); i++ ){
			::ReleaseMutex(tcpPortMutex[i]);
			::CloseHandle(tcpPortMutex[i]);
		}
		tcpPortMutex.clear();
	}

	if( sendList != NULL ){
		if( this->sendTcp == NULL ){
			this->sendTcp = new CSendTCP;
		}
		for( size_t i=0; i<sendList->size(); i++ ){
			wstring key = L"";

			while(1){
				Format(key, L"%s%d_%d", MUTEX_TCP_PORT_NAME, (*sendList)[i].ip, (*sendList)[i].port );
				HANDLE mutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, key.c_str());
		
				if(mutex){
					::CloseHandle(mutex);
					(*sendList)[i].port++;
				}else{
					break;
				}
			}

			HANDLE portMutex = _CreateMutex( TRUE, key.c_str());
			_OutputDebugString(L"%s\r\n", key.c_str());
			tcpPortMutex.push_back(portMutex);
		}

		this->sendTcp->StartUpload(sendList);
	}else{
		SAFE_DELETE(this->sendTcp);
	}

	return TRUE;
}

//�o�͗pTS�f�[�^�𑗂�
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// data		[IN]TS�f�[�^
// size		[IN]data�̃T�C�Y
BOOL COneServiceUtil::AddTSBuff(
	BYTE* data,
	DWORD size
	)
{
	BOOL ret = TRUE;
	if( this->SID == 0xFFFF || this->sendTcp != NULL || this->sendUdp != NULL){
		//�S�T�[�r�X����
		ret = WriteData(data, size);
		for( DWORD i=0; i<size; i+=188 ){
			CTSPacketUtil packet;
			if( packet.Set188TS(data + i, 188) == TRUE ){
				if( packet.PID == this->pmtPID ){
					DWORD err = createPmt.AddData(&packet);
				}
			}
		}
	}else{
		if( size > this->buffSize ){
			SAFE_DELETE_ARRAY(this->buff);
			this->buff = new BYTE[size*2];
			this->buffSize = size*2;
		}
		this->buffWriteSize = 0;

		for( DWORD i=0; i<size; i+=188 ){
			CTSPacketUtil packet;
			if( packet.Set188TS(data + i, 188) == TRUE ){
				if( packet.PID == 0x0000 ){
					//PAT
					BYTE* patBuff = NULL;
					DWORD patBuffSize = 0;
					if( createPat.GetPacket(&patBuff, &patBuffSize) == TRUE ){
						memcpy(this->buff + this->buffWriteSize, patBuff, patBuffSize);
						this->buffWriteSize+=patBuffSize;
					}
				}else if( packet.PID == this->pmtPID ){
					//PMT
					DWORD err = createPmt.AddData(&packet);
					if( err == NO_ERR || err == ERR_NO_CHAGE ){
						BYTE* pmtBuff = NULL;
						DWORD pmtBuffSize = 0;
						if( createPmt.GetPacket(&pmtBuff, &pmtBuffSize) == TRUE ){
							memcpy(this->buff + this->buffWriteSize, pmtBuff, pmtBuffSize);
							this->buffWriteSize+=pmtBuffSize;
						}else{
							_OutputDebugString(L"createPmt.GetPacket Err");
							//���̂܂�
							memcpy(this->buff + this->buffWriteSize, data+i, 188);
							this->buffWriteSize+=188;
						}
					}else if( err == FALSE ){
						_OutputDebugString(L"createPmt.AddData Err");
						//���̂܂�
						memcpy(this->buff + this->buffWriteSize, data+i, 188);
						this->buffWriteSize+=188;
					}
				}else{
					//���̑�
					if( packet.PID < 0x0030 ){
						//���̂܂�
						memcpy(this->buff + this->buffWriteSize, data+i, 188);
						this->buffWriteSize+=188;
					}else{
						if( createPmt.IsNeedPID(packet.PID) == TRUE ){
							//PMT�Œ�`����Ă�
							memcpy(this->buff + this->buffWriteSize, data+i, 188);
							this->buffWriteSize+=188;
						}else{
							//EMM�Ȃ�K�v
							map<WORD,WORD>::iterator itr;
							itr = this->emmPIDMap.find(packet.PID);
							if( itr != this->emmPIDMap.end() ){
								memcpy(this->buff + this->buffWriteSize, data+i, 188);
								this->buffWriteSize+=188;
							}
						}
					}
				}
			}
		}

		WriteData(this->buff, this->buffWriteSize);
	}

	if( this->pittariStart == TRUE ){
		if( this->lastPMTVer == 0xFFFF ){
			this->lastPMTVer = createPmt.GetVersion();
		}else if(this->lastPMTVer != createPmt.GetVersion()){
			//�҂�����J�n
			StratPittariRec();
			this->lastPMTVer = createPmt.GetVersion();
		}
		if( this->epgUtil != NULL ){
			EPG_EVENT_INFO* epgInfo;
			if( this->epgUtil->GetEpgInfo( this->pittariONID, this->pittariTSID, this->pittariSID, FALSE, &epgInfo ) == NO_ERR ){
				if( epgInfo->event_id == this->pittariEventID ){
					//�҂�����J�n
					StratPittariRec();
					this->pittariStart = FALSE;
					this->pittariEndChk = TRUE;
				}
			}
		}
	}
	if( this->pittariEndChk == TRUE ){
		if( this->epgUtil != NULL ){
			EPG_EVENT_INFO* epgInfo;
			if( this->epgUtil->GetEpgInfo( this->pittariONID, this->pittariTSID, this->pittariSID, FALSE, &epgInfo ) == NO_ERR ){
				if( epgInfo->event_id != this->pittariEventID ){
					//�҂�����I��
					StopPittariRec();
					this->pittariEndChk = FALSE;
				}
			}
		}
	}

	return ret;
}

BOOL COneServiceUtil::WriteData(BYTE* data, DWORD size)
{
	if( this->sendUdp != NULL ){
		this->sendUdp->SendData(data, size);
	}
	if( this->sendTcp != NULL ){
		this->sendTcp->SendData(data, size);
	}
	if( this->writeFile != NULL ){
		this->writeFile->AddTSBuff(data, size);
	}

	dropCount.AddData(data, size);

	return TRUE;
}

void COneServiceUtil::SetPmtPID(
	WORD TSID,
	WORD pmtPID
	)
{
	if( this->pmtPID != pmtPID && this->SID != 0xFFFF){
		_OutputDebugString(L"COneServiceUtil::SetPmtPID 0x%04x => 0x%04x", this->pmtPID, pmtPID);
		map<WORD, CCreatePATPacket::PROGRAM_PID_INFO> PIDMap;

		CCreatePATPacket::PROGRAM_PID_INFO item;
		item.PMTPID = pmtPID;
		item.SID = this->SID;
		PIDMap.insert(pair<WORD, CCreatePATPacket::PROGRAM_PID_INFO>(item.PMTPID,item));
		
		item.PMTPID = 0x0010;
		item.SID = 0x00;
		PIDMap.insert(pair<WORD, CCreatePATPacket::PROGRAM_PID_INFO>(item.PMTPID,item));
		
		createPat.SetParam(TSID, &PIDMap);

		this->pmtPID = pmtPID;
	}
}

void COneServiceUtil::SetEmmPID(
	map<WORD,WORD>* PIDMap
	)
{
	this->emmPIDMap = *PIDMap;
}

//�t�@�C���ۑ����J�n����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
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
BOOL COneServiceUtil::StartSave(
	wstring fileName,
	BOOL overWriteFlag,
	BOOL pittariFlag,
	WORD pittariONID,
	WORD pittariTSID,
	WORD pittariSID,
	WORD pittariEventID,
	ULONGLONG createSize,
	vector<REC_FILE_SET_INFO>* saveFolder,
	vector<wstring>* saveFolderSub,
	int maxBuffCount
)
{
	this->maxBuffCount = maxBuffCount;
	if( pittariFlag == FALSE ){
		if( this->writeFile == NULL ){
			OutputDebugString(L"*:StartSave");
			this->writeFile = new CWriteTSFile;
			return this->writeFile->StartSave(fileName, overWriteFlag, createSize, saveFolder, saveFolderSub, this->maxBuffCount);
		}
	}else{
		if( this->writeFile == NULL ){
			OutputDebugString(L"*:StartSave pittariFlag");
			this->pittariRecFilePath = L"";
			this->fileName = fileName;
			this->overWriteFlag = overWriteFlag;
			this->createSize = createSize;
			this->saveFolder = *saveFolder;
			this->saveFolderSub = *saveFolderSub;
			this->pittariONID = pittariONID;
			this->pittariTSID = pittariTSID;
			this->pittariSID = pittariSID;
			this->pittariEventID = pittariEventID;

			this->lastPMTVer = 0xFFFF;

			this->pittariStart = TRUE;
			this->pittariEndChk = FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

void COneServiceUtil::StratPittariRec()
{
	if( this->writeFile == NULL ){
		OutputDebugString(L"*:StratPittariRec");
		this->writeFile = new CWriteTSFile;
		this->writeFile->StartSave(this->fileName, this->overWriteFlag, this->createSize, &this->saveFolder, &this->saveFolderSub, this->maxBuffCount);
	}
}

void COneServiceUtil::StopPittariRec()
{
	if( this->writeFile == NULL ){
		return ;
	}
	OutputDebugString(L"*:StopPittariRec");
	BOOL subRec;
	this->writeFile->GetSaveFilePath(&this->pittariRecFilePath, &subRec);
	BOOL ret = this->writeFile->EndSave();
}

//�t�@�C���ۑ����I������
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
BOOL COneServiceUtil::EndSave()
{
	if( this->writeFile == NULL ){
		return FALSE;
	}
	BOOL ret = this->writeFile->EndSave();
	SAFE_DELETE(this->writeFile);
	OutputDebugString(L"*:EndSave");
	return ret;
}

//�^�撆���ǂ���
//�߂�l�F
// TRUE�i�^�撆�j�AFALSE�i���Ă��Ȃ��j
BOOL COneServiceUtil::IsRec()
{
	if( this->writeFile == NULL ){
		if( this->pittariStart == TRUE || this->pittariEndChk == TRUE ){
			return TRUE;
		}
		return FALSE;
	}else{
		return TRUE;
	}
}

//�X�N�����u�����������̓���ݒ�
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// enable		[IN] TRUE�i��������j�AFALSE�i�������Ȃ��j
void COneServiceUtil::SetScramble(
	BOOL enable
	)
{
	this->enableScramble = enable;
}

//�X�N�����u�������������s�����ǂ���
//�߂�l�F
// TRUE�i��������j�AFALSE�i�������Ȃ��j
BOOL COneServiceUtil::GetScramble(
	)
{
	return this->enableScramble;
}

//�����ƃf�[�^�����܂߂邩�ǂ���
//�����F
// enableCaption		[IN]������ TRUE�i�܂߂�j�AFALSE�i�܂߂Ȃ��j
// enableData			[IN]�f�[�^������ TRUE�i�܂߂�j�AFALSE�i�܂߂Ȃ��j
void COneServiceUtil::SetServiceMode(
	BOOL enableCaption,
	BOOL enableData
	)
{
	createPmt.SetCreateMode(enableCaption, enableData);
}

//�G���[�J�E���g���N���A����
void COneServiceUtil::ClearErrCount()
{
	this->dropCount.Clear();
}

//�h���b�v�ƃX�N�����u���̃J�E���g���擾����
//�����F
// drop				[OUT]�h���b�v��
// scramble			[OUT]�X�N�����u����
void COneServiceUtil::GetErrCount(ULONGLONG* drop, ULONGLONG* scramble)
{
	this->dropCount.GetCount(drop, scramble);
}

//�^�撆�̃t�@�C���̃t�@�C���p�X���擾����
//�����F
// filePath			[OUT]�ۑ��t�@�C����
// subRecFlag		[OUT]�T�u�^�悪�����������ǂ���
void COneServiceUtil::GetSaveFilePath(
	wstring* filePath,
	BOOL* subRecFlag
	)
{
	if( this->writeFile != NULL ){
		this->writeFile->GetSaveFilePath(filePath, subRecFlag);
		if( filePath->size() == 0 ){
			*filePath = this->pittariRecFilePath;
		}
	}
}

//�h���b�v�ƃX�N�����u���̃J�E���g��ۑ�����
//�����F
// filePath			[IN]�ۑ��t�@�C����
void COneServiceUtil::SaveErrCount(
	wstring filePath
	)
{
	this->dropCount.SaveLog(filePath);
}


void COneServiceUtil::SetSignalLevel(
	float signalLv
	)
{
	this->dropCount.SetSignal(signalLv);
}

//�^�撆�̃t�@�C���̏o�̓T�C�Y���擾����
//�����F
// writeSize			[OUT]�ۑ��t�@�C����
void COneServiceUtil::GetRecWriteSize(
	__int64* writeSize
	)
{
	if( this->writeFile != NULL ){
		this->writeFile->GetRecWriteSize(writeSize);
	}
}

void COneServiceUtil::SetBonDriver(
	wstring bonDriver
	)
{
	this->dropCount.SetBonDriver(bonDriver);
}

void COneServiceUtil::SetPIDName(
	map<WORD, string>* pidName
	)
{
	this->dropCount.SetPIDName(pidName);
}
