#include "stdafx.h"
#include "ParseChText4Mp.h"
#include "StringUtil.h"
#include "EpgTimerUtil.h"
#include "PathUtil.h"

#include "../MpCtrl/DbCtrl.h"

CParseChText4::CParseChText4(void)
{
	filePath = L"";
}

CParseChText4::~CParseChText4(void)
{
}

BOOL CParseChText4::ParseText(LPCWSTR filePath)
{
	if( filePath == NULL ){
		return FALSE;
	}

	this->chList.clear();
	this->filePath = filePath;

	HANDLE hFile = _CreateFile2( filePath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
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
		CH_DATA4 Item;
		if( Parse1Line(parseLine, &Item) == TRUE ){
			LONGLONG iKey = _Create64Key(Item.originalNetworkID, Item.transportStreamID, Item.serviceID );
			this->chList.insert( pair<LONGLONG, CH_DATA4>(iKey,Item) );
		}
	}

	return TRUE;
}

BOOL CParseChText4::Parse1Line(string parseLine, CH_DATA4* chInfo )
{
	if( parseLine.empty() == true || chInfo == NULL ){
		return FALSE;
	}
	string strBuff="";

	Separate( parseLine, "\t", strBuff, parseLine);

	//Ch��
	AtoW(strBuff, chInfo->chName);

	Separate( parseLine, "\t", strBuff, parseLine);

	//�T�[�r�X��
	AtoW(strBuff, chInfo->serviceName);

	Separate( parseLine, "\t", strBuff, parseLine);

	//ts_name or network_name
	AtoW(strBuff, chInfo->networkName);

	Separate( parseLine, "\t", strBuff, parseLine);

	//Space
	chInfo->space = atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//Ch
	chInfo->ch = atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//ONID
	chInfo->originalNetworkID = atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//TSID
	chInfo->transportStreamID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//SID
	chInfo->serviceID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//Service Type
	chInfo->serviceType = atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//Partial Flag
	chInfo->partialFlag = atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//Use
	chInfo->useViewFlag = atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�����R��ID
	chInfo->remoconID = atoi(strBuff.c_str());

	return TRUE;
}

BOOL CParseChText4::AddCh(CH_DATA4 chInfo )
{
	LONGLONG iKey = _Create64Key(chInfo.originalNetworkID, chInfo.transportStreamID, chInfo.serviceID );
	multimap<LONGLONG, CH_DATA4>::iterator itrF;
	itrF = this->chList.find(iKey);
	if( itrF == this->chList.end() ){
		this->chList.insert( pair<LONGLONG, CH_DATA4>(iKey,chInfo) );
	}else{
		if( itrF->second.ch == chInfo.ch && itrF->second.space == chInfo.space ){
			return FALSE;
		}else{
			this->chList.insert( pair<LONGLONG, CH_DATA4>(iKey,chInfo) );
		}
	}
	return TRUE;
}

BOOL CParseChText4::DelCh(WORD originalNetworkID, WORD transportStreamID, WORD serviceID)
{
	LONGLONG iKey = _Create64Key(originalNetworkID, transportStreamID, serviceID );
	multimap<LONGLONG, CH_DATA4>::iterator itrF;
	itrF = this->chList.find(iKey);
	if( itrF != this->chList.end() ){
		this->chList.erase(itrF);
	}
	return TRUE;
}

BOOL CParseChText4::DelCh(
	WORD space,
	WORD ch)
{
	multimap<LONGLONG, CH_DATA4>::iterator itrF;
	for( itrF = this->chList.begin(); itrF != this->chList.end(); itrF++ ){
		if( itrF->second.space == space && itrF->second.ch == ch ){
			this->chList.erase(itrF);
			break;
		}
	}
	return TRUE;
}

BOOL CParseChText4::DelChService(
	WORD space,
	WORD ch,
	WORD serviceID)
{
	multimap<LONGLONG, CH_DATA4>::iterator itrF;
	for( itrF = this->chList.begin(); itrF != this->chList.end(); itrF++ ){
		if( itrF->second.space == space && itrF->second.ch == ch && itrF->second.serviceID == serviceID){
			this->chList.erase(itrF);
			break;
		}
	}
	return TRUE;
}

BOOL CParseChText4::SaveChText(LPCWSTR filePath)
{
	wstring loadFilePath = L"";
	wstring loadTunerName = L"";
	if( filePath == NULL ){
		loadFilePath = this->filePath;
		loadTunerName = this->tunerName;
	}else{
		loadFilePath = filePath;
		wregex re(L".+\\\\(.+)\\(.+\\)\\.ChSet4\\.txt$");
		wstring text(filePath);
		wsmatch m;
		if( regex_search(text, m, re) ){ 
			loadTunerName = m[1];
			loadTunerName += L".dll";
		}
	}

	if( loadFilePath.size() == 0 ){
		return FALSE;
	}

	if( loadTunerName.size() == 0 ){
		return FALSE;
	}

	multimap<LONGLONG, CH_DATA4> sortList;
	multimap<LONGLONG, CH_DATA4>::iterator itr;
	for( itr = this->chList.begin(); itr != this->chList.end(); itr++ ){
		LONGLONG Key = ((LONGLONG)itr->second.space)<<32 | ((LONGLONG)itr->second.ch)<<16 | (LONGLONG)itr->second.serviceID;
		sortList.insert(pair<LONGLONG, CH_DATA4>(Key, itr->second));
	}


	// �t�@�C���o��
	HANDLE hFile = _CreateFile2( loadFilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE ){
		return FALSE;
	}

	for( itr = sortList.begin(); itr != sortList.end(); itr++ ){
		string chName="";
		WtoA(itr->second.chName, chName);
		string serviceName="";
		WtoA(itr->second.serviceName, serviceName);
		string networkName="";
		WtoA(itr->second.networkName, networkName);

		string strBuff;
		Format(strBuff, "%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
			chName.c_str(),
			serviceName.c_str(),
			networkName.c_str(),
			itr->second.space,
			itr->second.ch,
			itr->second.originalNetworkID,
			itr->second.transportStreamID,
			itr->second.serviceID,
			itr->second.serviceType,
			itr->second.partialFlag,
			itr->second.useViewFlag,
			itr->second.remoconID
			);
		DWORD dwWrite = 0;
		WriteFile(hFile, strBuff.c_str(), (DWORD)strBuff.length(), &dwWrite, NULL);
	}

	CloseHandle(hFile);

	wstring appIniPath = L"";
	GetModuleIniPath(appIniPath);

	wstring ipString;
	DWORD ip;
	DWORD port;

	ip = GetPrivateProfileInt(L"SET_UDP", L"IP0", 2130706433, appIniPath.c_str());
	Format(ipString, L"%d.%d.%d.%d", 
	(ip&0xFF000000)>>24, 
	(ip&0x00FF0000)>>16, 
	(ip&0x0000FF00)>>8, 
	(ip&0x000000FF) );
	port = GetPrivateProfileInt( L"SET_UDP", L"Port0", 3456, appIniPath.c_str() );

	// MediaPortal TV Server�̃f�[�^�x�[�X�֓o�^
	if (this->dbCtrl.Connect(&this->mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB) != 0) {
		return FALSE;
	}

	this->results = NULL;
	CString sql = L"";
	wstring wsql = L"";
	int chkNum = 0;

	this->dbCtrl.Begin(&this->mysql);

	map<CString, int> lockTable;
	lockTable[L"channelgroup"] = 2;
	lockTable[L"tuningdetail"] = 2;
	lockTable[L"groupmap"    ] = 2;
	lockTable[L"channel"     ] = 2;
	lockTable[L"channelmap"  ] = 2;
	if (this->dbCtrl.LockTable(&this->mysql, lockTable) != 0) goto ESC;


	// channelgroup�̓o�^�������邩���ׂ�
	sql  = L"SELECT idGroup FROM channelgroup WHERE idGroup < 2;";
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
	this->dbCtrl.StoreResult(&this->mysql, &this->results);
	chkNum = this->dbCtrl.NumRows(&this->results);
	this->dbCtrl.FreeResult(&this->results);

	int maxNum;
	sql  = L"SELECT MAX(idGroup) FROM channelgroup;";
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
	this->dbCtrl.StoreResult(&this->mysql, &this->results);
	this->record = this->dbCtrl.FetchRow(&this->results);
	maxNum = atoi(this->record[0]);
	this->dbCtrl.FreeResult(&this->results);

	switch(chkNum){
		case 0: // �o�^��0�Ȃ�idGroup��0�i�n��g�EBS�j��1�iCS�j��o�^����
			// idGroup��0�̓I�[�g�i���o�[�̂���INSERT�̒i�K�ł͓o�^�ł��Ȃ��̂�INSERT���Ă���ύX����
			sql.Format(_T("INSERT INTO channelgroup VALUES(%d,'�n��g�EBS',0);"), maxNum + 1);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			sql.Format(_T("UPDATE channelgroup SET idGroup = 0 WHERE idGroup = %d;"), maxNum + 1);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			sql  = L"INSERT INTO channelgroup VALUES(1,'CS',1);";
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			break;
		case 1: // �o�^��1�Ȃ�idGroup��1�𖖔ԂɕύX���āAidGroup��0�i�n��g�EBS�j��1�iCS�j��o�^����
			// 1�Ƃ������Ƃ�MediaPortal_Bon���O��MediaPortal TV Server�̃`�����l���ݒ�ŏ����l���������B
			sql.Format(_T("UPDATE channelgroup SET idGroup = %d WHERE idGroup = 1;"), maxNum + 1);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			sql.Format(_T("UPDATE groupmap SET idGroup = %d WHERE idGroup = 1;"), maxNum + 1);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			sql  = L"SELECT MAX(idGroup) FROM channelgroup;";
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			this->record = this->dbCtrl.FetchRow(&this->results);
			maxNum = atoi(this->record[0]);
			this->dbCtrl.FreeResult(&this->results);

			sql.Format(_T("INSERT INTO channelgroup VALUES(%d,'�n��g�EBS',0);"), maxNum + 1);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			sql.Format(_T("UPDATE channelgroup SET idGroup = 0 WHERE idGroup = %d;"), maxNum + 1);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			sql  = L"INSERT INTO channelgroup VALUES(1,'CS',1);";
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			break;
	}

	// �����`���[�i�[�̊����̓o�^�ɑ΂��ă`���[�i�[����ύX����B
	sql.Format(_T("UPDATE tuningdetail SET provider = '@_%s' WHERE provider = '%s';"), loadTunerName.c_str(), loadTunerName.c_str());
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

	//sql.Format(_T("iterator = '%d' '%d';"), sortList.begin(), sortList.end());
	//AfxMessageBox(sql, NULL, MB_OK);

	int tmpCh;  // �o�^�p�`�����l��
	for( itr = sortList.begin(); itr != sortList.end(); itr++ ){

		wsql  = L"";
		wsql += L"SELECT idChannel FROM tuningdetail WHERE ";
		wsql += L"provider = '@_%s' AND ";
		wsql += L"channelNumber = %d AND ";
		wsql += L"networkId     = %d AND ";
		wsql += L"transportId   = %d AND ";
		wsql += L"serviceId     = %d;";
		sql.Format(wsql.c_str(),
			loadTunerName.c_str(),
			itr->second.ch,
			itr->second.originalNetworkID,
			itr->second.transportStreamID,
			itr->second.serviceID
			);
		if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
		this->dbCtrl.StoreResult(&this->mysql, &this->results);

		//AfxMessageBox(loadTunerName.c_str(), NULL, MB_OK);

		if(this->dbCtrl.NumRows(&this->results)){ // �����̃`�����l���͑ޔ�����
			this->record = this->dbCtrl.FetchRow(&this->results);
			tmpCh = atoi(this->record[0]);
			this->dbCtrl.FreeResult(&this->results);

			wsql  = L"";
			wsql += L"UPDATE tuningdetail SET provider = '%s' WHERE ";
			wsql += L"provider = '@_%s' AND ";
			wsql += L"channelNumber = %d AND ";
			wsql += L"networkId     = %d AND ";
			wsql += L"transportId   = %d AND ";
			wsql += L"serviceId     = %d;";
			sql.Format(wsql.c_str(),
				loadTunerName.c_str(),
				loadTunerName.c_str(),
				itr->second.ch,
				itr->second.originalNetworkID,
				itr->second.transportStreamID,
				itr->second.serviceID
				);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			// �����̃`�����l���o�^�������Ă�groupmap.idGroup���K�؂Ȓl�i0:�n��g�EBS, 1:CS�j�ɂȂ��Ă��邩�𒲂ׂ�B
			sql.Format(_T("SELECT idMap FROM groupmap WHERE idChannel = %d AND idGroup = %d;"), tmpCh, itr->second.ch);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			if(!this->dbCtrl.NumRows(&this->results)){
				this->dbCtrl.FreeResult(&this->results);
				sql  = L"SELECT SortOrder FROM groupmap;";
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
				this->dbCtrl.StoreResult(&this->mysql, &this->results);
				maxNum = this->dbCtrl.NumRows(&this->results);

				// groupmap.idGroup�ɓK�؂Ȓl�i0:�n��g�EBS, 1:CS�j�̓o�^���s���B
				sql.Format(_T("INSERT INTO groupmap VALUES(0, %d, %d, %d);"), itr->second.ch, tmpCh, maxNum);
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			}
			this->dbCtrl.FreeResult(&this->results);

			// �`�����l���T�v�����邩
			sql.Format(_T("SELECT displayName FROM channel WHERE idChannel = %d;"), tmpCh);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			if(!this->dbCtrl.NumRows(&this->results)){
				// �`�����l���̓o�^���s���B
				sql.Format(_T("INSERT INTO channel VALUES(%d,0,1,0,'2000-01-01 00:00:00',0,'2000-01-01 00:00:00',0,1,'','%s',0,%d);"), 
					tmpCh, 
					itr->second.serviceName.c_str(),
					itr->second.ch
				);
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			}
			this->dbCtrl.FreeResult(&this->results);

			// �`�����l���}�b�v�����邩
			sql.Format(_T("SELECT idChannelMap FROM channelmap WHERE idChannel = %d AND idCard = 1;"), tmpCh);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			if(!this->dbCtrl.NumRows(&this->results)){
				// �`�����l���}�b�v�̓o�^���s���B
				sql.Format(_T("INSERT INTO channelmap VALUES(0,%d,1,0);"), tmpCh);
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			}
			this->dbCtrl.FreeResult(&this->results);

		} else { // �`�����l����V�K�o�^
			this->dbCtrl.FreeResult(&this->results);

			// BON�h���C�o�[������Ă������`�����l�������邩�ǂ���
			wsql  = L"";
			wsql += L"SELECT idChannel FROM tuningdetail WHERE ";
			wsql += L"provider <> '@_%s' AND ";
			wsql += L"channelNumber = %d AND ";
			wsql += L"networkId     = %d AND ";
			wsql += L"transportId   = %d AND ";
			wsql += L"serviceId     = %d;";
			sql.Format(wsql.c_str(),
				loadTunerName.c_str(),
				itr->second.ch,
				itr->second.originalNetworkID,
				itr->second.transportStreamID,
				itr->second.serviceID
				);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			
			// �����`�����l��������ꍇ�͂��̃`�����l�����g��
			// �Ȃ��ꍇ�̓`�����l���T�v�̎��ԍ��𓾂�B
			if(!this->dbCtrl.NumRows(&this->results)){
				sql  = L"SELECT MAX(idChannel) FROM channel;";
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
				this->dbCtrl.StoreResult(&this->mysql, &this->results);
			}
			this->record = this->dbCtrl.FetchRow(&this->results);
			//if(CA2T(this->record[0], CP_UTF8) == L"") maxNum = 0;
			if(this->record[0] == 0x00000000) maxNum = 0;
			else maxNum = atoi(this->record[0]);
			tmpCh  = maxNum + 1;
			this->dbCtrl.FreeResult(&this->results);

			// �`�����l���T�v�����邩
			sql.Format(_T("SELECT displayName FROM channel WHERE idChannel = %d;"), tmpCh);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			if(!this->dbCtrl.NumRows(&this->results)){
				// �`�����l���̓o�^���s���B
				sql.Format(_T("INSERT INTO channel VALUES(%d,0,1,0,'2000-01-01 00:00:00',0,'2000-01-01 00:00:00',0,1,'','%s',0,%d);"), 
					tmpCh, 
					itr->second.serviceName.c_str(),
					itr->second.ch
				);
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			}
			this->dbCtrl.FreeResult(&this->results);

			// �`�����l���ڍׂ̎��ԍ��𓾂�B
			int maxTuNum;
			sql  = L"SELECT MAX(idTuning) FROM tuningdetail;";
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			this->record = this->dbCtrl.FetchRow(&this->results);
			if(this->record[0] == 0x00000000) maxTuNum = 0;
			else maxTuNum = atoi(this->record[0]);
			this->dbCtrl.FreeResult(&this->results);

			// �`�����l���ڍדo�^
			sql.Format(L"INSERT INTO tuningdetail VALUES(%d,%d,'%s','%s',7,%d,0,31,0,1,%d,%d,%d,496,0,0,0,0,1,0,8,-1,-1,0,0,0,-1,-1,-1,-1,'udp://%s:%d',0,0,0);",
				maxTuNum + 1,
				tmpCh,
				itr->second.serviceName.c_str(),
				loadTunerName.c_str(),
				itr->second.ch,
				itr->second.originalNetworkID,
				itr->second.transportStreamID,
				itr->second.serviceID,
				ipString.c_str(),
				port
			);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

			//AfxMessageBox(sql, NULL, MB_OK);

			// �O���[�v�����邩
			sql.Format(_T("SELECT idChannel FROM groupmap WHERE idGroup = %d AND idChannel = %d;"), itr->second.space, tmpCh);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			if(!this->dbCtrl.NumRows(&this->results)){
				this->dbCtrl.FreeResult(&this->results);
				sql  = L"SELECT SortOrder FROM groupmap;";
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
				this->dbCtrl.StoreResult(&this->mysql, &this->results);
				maxNum = this->dbCtrl.NumRows(&this->results);

				// �O���[�v�̓o�^���s���B
				sql.Format(_T("INSERT INTO groupmap VALUES(0, %d, %d, %d);"), itr->second.space, tmpCh, maxNum);
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			}
			this->dbCtrl.FreeResult(&this->results);
			
			// �`�����l���}�b�v�����邩
			sql.Format(_T("SELECT idChannelMap FROM channelmap WHERE idChannel = %d AND idCard = 2;"), tmpCh);
			if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			this->dbCtrl.StoreResult(&this->mysql, &this->results);
			if(!this->dbCtrl.NumRows(&this->results)){
				// �`�����l���}�b�v�̓o�^���s���B
				sql.Format(_T("INSERT INTO channelmap VALUES(0,%d,2,0);"), tmpCh);
				if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
			}
			this->dbCtrl.FreeResult(&this->results);
		}
	}

	// �c�����`�����l�����폜����
	sql.Format(_T("DELETE FROM tuningdetail WHERE provider = '@_%s';"), loadTunerName.c_str());
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

	// ���łɃ`���[�i�[�̂Ȃ��`�����l�����폜����
	sql  = L"DELETE channel FROM channel LEFT JOIN tuningdetail ON channel.idChannel = tuningdetail.idChannel WHERE tuningdetail.idChannel IS NULL;";
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

	// ���łɃ`�����l���̂Ȃ��O���[�v���폜����B
	sql  = L"DELETE groupmap FROM groupmap LEFT JOIN channel ON groupmap.idChannel = channel.idChannel WHERE channel.idChannel IS NULL;";
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

	// ���łɃ`�����l���̂Ȃ��`�����l���}�b�v���폜����
	sql  = L"DELETE channelmap FROM channelmap LEFT JOIN channel ON channelmap.idChannel = channel.idChannel WHERE channel.idChannel IS NULL;";
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

	this->dbCtrl.Commit(&this->mysql);
	this->dbCtrl.UnlockTable(&this->mysql);
	this->dbCtrl.Close(&this->mysql);
	return TRUE;

	ESC: 
	wstring err = L"";
	Format(err, L"ERROR SQL:%s", sql);
	AfxMessageBox(err.c_str(), NULL, MB_OK);
	this->dbCtrl.Rollback(&this->mysql);
	this->dbCtrl.UnlockTable(&this->mysql);
	this->dbCtrl.Close(&this->mysql);
	return FALSE;
}

BOOL CParseChText4::SaveUdpMp()
{
	
	wstring appIniPath = L"";
	GetModuleIniPath(appIniPath);

	wstring ipString;
	DWORD ip;
	DWORD port;
	
	// MediaPortal TV Server�̃f�[�^�x�[�X�֓o�^
	if (this->dbCtrl.Connect(&this->mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB) != 0) {
		return FALSE;
	}

	this->results = NULL;
	CString sql = L"";
	wstring wsql = L"";

	this->dbCtrl.Begin(&this->mysql);

	map<CString, int> lockTable;
	lockTable[L"tuningdetail"] = 2;
	lockTable[L"groupmap"] = 2;
	if (this->dbCtrl.LockTable(&this->mysql, lockTable) != 0) goto ESC;

	ip = GetPrivateProfileInt(L"SET_UDP", L"IP0", 2130706433, appIniPath.c_str());
	Format(ipString, L"%d.%d.%d.%d", 
	(ip&0xFF000000)>>24, 
	(ip&0x00FF0000)>>16, 
	(ip&0x0000FF00)>>8, 
	(ip&0x000000FF) );
	port = GetPrivateProfileInt( L"SET_UDP", L"Port0", 3456, appIniPath.c_str() );

	sql.Format(_T("UPDATE tuningdetail LEFT JOIN groupmap ON tuningdetail.idChannel = groupmap.idChannel SET tuningdetail.url= 'udp://%s:%d' WHERE groupmap.idGroup IN(0,1);"), ipString.c_str(), port);
	if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;

	this->dbCtrl.Commit(&this->mysql);
	this->dbCtrl.UnlockTable(&this->mysql);
	this->dbCtrl.Close(&this->mysql);
	return TRUE;

	ESC: 
	wstring err = L"";
	Format(err, L"ERROR SQL:%s", sql);
	AfxMessageBox(err.c_str(), NULL, MB_OK);
	this->dbCtrl.Rollback(&this->mysql);
	this->dbCtrl.UnlockTable(&this->mysql);
	this->dbCtrl.Close(&this->mysql);
	return FALSE;

}
