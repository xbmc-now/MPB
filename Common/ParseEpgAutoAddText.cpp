#include "StdAfx.h"
#include "ParseEpgAutoAddText.h"
#include "TimeUtil.h"
#include "StringUtil.h"

CParseEpgAutoAddText::CParseEpgAutoAddText(void)
{
	this->loadFilePath = L"";
	this->nextID = 1;
}

CParseEpgAutoAddText::~CParseEpgAutoAddText(void)
{
	map<DWORD, EPG_AUTO_ADD_DATA*>::iterator itr;
	for( itr = this->dataIDMap.begin(); itr != this->dataIDMap.end(); itr++ ){
		SAFE_DELETE(itr->second)
	}
}

BOOL CParseEpgAutoAddText::ParseText(LPCWSTR filePath)
{
	if( filePath == NULL ){
		return FALSE;
	}

	map<DWORD, EPG_AUTO_ADD_DATA*>::iterator itr;
	for( itr = this->dataIDMap.begin(); itr != this->dataIDMap.end(); itr++ ){
		SAFE_DELETE(itr->second)
	}
	this->dataIDMap.clear();
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
				EPG_AUTO_ADD_DATA* item = new EPG_AUTO_ADD_DATA;
				BOOL bRet = Parse1Line(parseLine, item);
				if( bRet == FALSE ){
					SAFE_DELETE(item)
				}else{
					item->dataID = GetNextID();
					this->dataIDMap.insert( pair<DWORD, EPG_AUTO_ADD_DATA*>(item->dataID,item) );
				}
			}
		}
	}

	return TRUE;
}

BOOL CParseEpgAutoAddText::Parse1Line(string parseLine, EPG_AUTO_ADD_DATA* item )
{
	if( parseLine.empty() == true || item == NULL ){
		return FALSE;
	}
	string strBuff="";

	Separate( parseLine, "\t", strBuff, parseLine);

	//dataID
	item->dataID = (DWORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�����L�[
	//and�L�[
	AtoW(strBuff, item->searchInfo.andKey);

	Separate( parseLine, "\t", strBuff, parseLine);

	//not�L�[
	AtoW(strBuff, item->searchInfo.notKey);

	Separate( parseLine, "\t", strBuff, parseLine);

	//���K�\�����[�h
	item->searchInfo.regExpFlag = (BOOL)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�^�C�g���̂�
	item->searchInfo.titleOnlyFlag = (BOOL)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�ΏۃW������
	if( strBuff.size() > 0 ){
		while(strBuff.find(",") != string::npos){
			string val = "";
			Separate( strBuff, ",", val, strBuff);

			DWORD flag = (DWORD)atoi(val.c_str());

			EPGDB_CONTENT_DATA addItem;
			addItem.content_nibble_level_1 = (BYTE)((flag&0xFF000000)>>24);
			addItem.content_nibble_level_2 = (BYTE)((flag&0x00FF0000)>>16);
			addItem.user_nibble_1 = (BYTE)((flag&0x0000FF00)>>8);
			addItem.user_nibble_2 = (BYTE)(flag&0x000000FF);

			item->searchInfo.contentList.push_back(addItem);
		}
		DWORD flag = (DWORD)atoi(strBuff.c_str());
		EPGDB_CONTENT_DATA addItem;
		addItem.content_nibble_level_1 = (BYTE)((flag&0xFF000000)>>24);
		addItem.content_nibble_level_2 = (BYTE)((flag&0x00FF0000)>>16);
		addItem.user_nibble_1 = (BYTE)((flag&0x0000FF00)>>8);
		addItem.user_nibble_2 = (BYTE)(flag&0x000000FF);

		item->searchInfo.contentList.push_back(addItem);
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//�Ώۊ���
	if( strBuff.size() > 0 ){
		while(strBuff.find(",") != string::npos){
			string val = "";
			Separate( strBuff, ",", val, strBuff);

			string weekS = "";
			string timeS = "";
			string weekE = "";
			string timeE = "";
			Separate( val, "-", weekS, val);
			Separate( val, "-", timeS, val);
			Separate( val, "-", weekE, timeE);

			DWORD timeStart = (DWORD)atoi(timeS.c_str());
			DWORD timeEnd = (DWORD)atoi(timeE.c_str());

			EPGDB_SEARCH_DATE_INFO addItem;
			addItem.startDayOfWeek = (BYTE)atoi(weekS.c_str());
			addItem.startHour = (WORD)((timeStart&0xFFFF0000)>>16);
			addItem.startMin = (WORD)(timeStart&0x0000FFFF);
			addItem.endDayOfWeek = (BYTE)atoi(weekE.c_str());
			addItem.endHour = (WORD)((timeEnd&0xFFFF0000)>>16);
			addItem.endMin = (WORD)(timeEnd&0x0000FFFF);

			item->searchInfo.dateList.push_back(addItem);
		}
		string weekS = "";
		string timeS = "";
		string weekE = "";
		string timeE = "";
		Separate( strBuff, "-", weekS, strBuff);
		Separate( strBuff, "-", timeS, strBuff);
		Separate( strBuff, "-", weekE, timeE);

		DWORD timeStart = (DWORD)atoi(timeS.c_str());
		DWORD timeEnd = (DWORD)atoi(timeE.c_str());

		EPGDB_SEARCH_DATE_INFO addItem;
		addItem.startDayOfWeek = (BYTE)atoi(weekS.c_str());
		addItem.startHour = (WORD)((timeStart&0xFFFF0000)>>16);
		addItem.startMin = (WORD)(timeStart&0x0000FFFF);
		addItem.endDayOfWeek = (BYTE)atoi(weekE.c_str());
		addItem.endHour = (WORD)((timeEnd&0xFFFF0000)>>16);
		addItem.endMin = (WORD)(timeEnd&0x0000FFFF);

		item->searchInfo.dateList.push_back(addItem);
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//�ΏۃT�[�r�X
	if( strBuff.size() > 0 ){
		while(strBuff.find(",") != string::npos){
			string val = "";
			Separate( strBuff, ",", val, strBuff);

			string strH;
			string strL;

			strH = val.substr(0,4);
			strL = val.substr(4);

			__int64 i64Ch=0;
			CHAR *endstr;
			i64Ch = ((__int64)strtol(strH.c_str(), &endstr, 16))<<32 |
				((__int64)strtoul(strL.c_str(), &endstr, 16));

			item->searchInfo.serviceList.push_back(i64Ch);
		}
		string strH;
		string strL;

		strH = strBuff.substr(0,4);
		strL = strBuff.substr(4);

		__int64 i64Ch=0;
		CHAR *endstr;
		i64Ch = ((__int64)strtol(strH.c_str(), &endstr, 16))<<32 |
			((__int64)strtoul(strL.c_str(), &endstr, 16));

		item->searchInfo.serviceList.push_back(i64Ch);
	}

	Separate( parseLine, "\t", strBuff, parseLine);

	//�^��ݒ�
	//�^�惂�[�h
	item->recSetting.recMode = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�D��x
	item->recSetting.priority = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�Ǐ]
	item->recSetting.tuijyuuFlag = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�w��T�[�r�X���[�h
	item->recSetting.serviceMode = (DWORD)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�҂�����^��
	item->recSetting.pittariFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//Bat����
	AtoW(strBuff, item->recSetting.batFilePath);

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

	//�ʃ}�[�W��
	item->recSetting.useMargineFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�J�n�}�[�W��
	item->recSetting.startMargine = (INT)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�I���}�[�W��
	item->recSetting.endMargine = (INT)atoi(strBuff.c_str());
	
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
			if( plugin.size() == 0 ){
				folderItem.writePlugIn = L"Write_Default.dll";
			}else{
				folderItem.writePlugIn = plugin;
			}
			folderItem.writePlugIn = plugin;
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

	//�����܂�����
	item->searchInfo.aimaiFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�W������NOT����
	item->searchInfo.notContetFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//����NOT����
	item->searchInfo.notDateFlag = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//FreeCA���[�h
	item->searchInfo.freeCAFlag = (BYTE)atoi(strBuff.c_str());
	
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

	//�^��ς��̃`�F�b�N����
	item->searchInfo.chkRecEnd = (BYTE)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�^��ς��̃`�F�b�N�Ώۊ���
	item->searchInfo.chkRecDay = (WORD)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	return TRUE;
}

DWORD CParseEpgAutoAddText::GetNextID()
{
	DWORD dataID = 0xFFFFFFFF;

	map<DWORD, EPG_AUTO_ADD_DATA*>::iterator itr;
	itr = this->dataIDMap.find(this->nextID);
	if( itr == this->dataIDMap.end() ){
		dataID = this->nextID;
		this->nextID++;
		if( this->nextID == 0 || this->nextID == 0xFFFFFFFF){
			this->nextID = 1;
		}
	}else{
		for( DWORD i=1; i<0xFFFFFFFF; i++ ){
			itr = this->dataIDMap.find(this->nextID);
			if( itr == this->dataIDMap.end() ){
				dataID = this->nextID;
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

	return dataID;
}

BOOL CParseEpgAutoAddText::SaveText(LPCWSTR filePath)
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

	map<DWORD, EPG_AUTO_ADD_DATA*>::iterator itr;
	for( itr=this->dataIDMap.begin(); itr != this->dataIDMap.end(); itr++ ){
		string strWrite="";
		string strBuff="";
		DWORD dwWrite;
		//dataID
		Format(strBuff, "%d",itr->second->dataID);
		strWrite+=strBuff +"\t";
		//�����L�[
		//and�L�[
		WtoA(itr->second->searchInfo.andKey, strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//not�L�[
		WtoA(itr->second->searchInfo.notKey, strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//���K�\�����[�h
		Format(strBuff, "%d",itr->second->searchInfo.regExpFlag);
		strWrite+=strBuff +"\t";
		//�^�C�g���̂�
		Format(strBuff, "%d",itr->second->searchInfo.titleOnlyFlag);
		strWrite+=strBuff +"\t";
		//�ΏۃW������
		for(size_t i=0; i<itr->second->searchInfo.contentList.size(); i++ ){
			DWORD val = ((DWORD)itr->second->searchInfo.contentList[i].content_nibble_level_1) << 24 |
				((DWORD)itr->second->searchInfo.contentList[i].content_nibble_level_2) << 16 |
				((DWORD)itr->second->searchInfo.contentList[i].user_nibble_1) << 8 |
				((DWORD)itr->second->searchInfo.contentList[i].user_nibble_2);
			Format(strBuff, "%d",val);
			strWrite+=strBuff;
			if( i+1 < itr->second->searchInfo.contentList.size() ){
				strWrite+=",";
			}
		}
		strWrite+="\t";
		//�Ώۊ���
		for(size_t i=0; i<itr->second->searchInfo.dateList.size(); i++ ){
			DWORD start = ((DWORD)itr->second->searchInfo.dateList[i].startHour) << 16 |
				((DWORD)itr->second->searchInfo.dateList[i].startMin);
			DWORD end = ((DWORD)itr->second->searchInfo.dateList[i].endHour) << 16 |
				((DWORD)itr->second->searchInfo.dateList[i].endMin);

			Format(strBuff, "%d-%d-%d-%d",itr->second->searchInfo.dateList[i].startDayOfWeek, start,
				itr->second->searchInfo.dateList[i].endDayOfWeek, end);

			strWrite+=strBuff;
			if( i+1 < itr->second->searchInfo.dateList.size() ){
				strWrite+=",";
			}
		}
		strWrite+="\t";
		//�ΏۃT�[�r�X
		for(size_t i=0; i<itr->second->searchInfo.serviceList.size(); i++ ){
			DWORD dwH = 0;
			DWORD dwL = 0;
			dwH = (DWORD)(itr->second->searchInfo.serviceList[i]>>32);
			dwL = (DWORD)(itr->second->searchInfo.serviceList[i]&0xFFFFFFFF);
			Format(strBuff, "%04X%08X",dwH,dwL);
			strWrite+=strBuff;

			if( i+1 < itr->second->searchInfo.serviceList.size() ){
				strWrite+=",";
			}
		}
		strWrite+="\t";
		//�^��ݒ�
		//�^�惂�[�h
		Format(strBuff,"%d",itr->second->recSetting.recMode);
		strWrite+=strBuff +"\t";
		//�D��x
		Format(strBuff,"%d",itr->second->recSetting.priority);
		strWrite+=strBuff +"\t";
		//�Ǐ]
		Format(strBuff,"%d",itr->second->recSetting.tuijyuuFlag);
		strWrite+=strBuff +"\t";
		//�w��T�[�r�X���[�h
		Format(strBuff,"%d",itr->second->recSetting.serviceMode);
		strWrite+=strBuff +"\t";
		//�҂�����^��
		Format(strBuff,"%d",itr->second->recSetting.pittariFlag);
		strWrite+=strBuff +"\t";
		//Bat����
		WtoA(itr->second->recSetting.batFilePath,strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//�T�X�y���h
		Format(strBuff,"%d",itr->second->recSetting.suspendMode);
		strWrite+=strBuff +"\t";
		//�ċN��
		Format(strBuff,"%d",itr->second->recSetting.rebootFlag);
		strWrite+=strBuff +"\t";
		//�ʃ}�[�W��
		Format(strBuff,"%d",itr->second->recSetting.useMargineFlag);
		strWrite+=strBuff +"\t";
		//�J�n�}�[�W��
		Format(strBuff,"%d",itr->second->recSetting.startMargine);
		strWrite+=strBuff +"\t";
		//�I���}�[�W��
		Format(strBuff,"%d",itr->second->recSetting.endMargine);
		strWrite+=strBuff +"\t";
		//�����^��t�H���_
		Format(strBuff,"%d",itr->second->recSetting.recFolderList.size());
		strWrite+=strBuff +"\t";
		for( size_t i=0; i<itr->second->recSetting.recFolderList.size(); i++ ){
			wstring path = L"";
			path = itr->second->recSetting.recFolderList[i].recFolder;
			path += L"*";
			path += itr->second->recSetting.recFolderList[i].writePlugIn;
			path += L"*";
			path += itr->second->recSetting.recFolderList[i].recNamePlugIn;
			WtoA(path, strBuff);
			strWrite+=strBuff +"\t";
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
		//�����܂�����
		Format(strBuff,"%d",itr->second->searchInfo.aimaiFlag);
		strWrite+=strBuff +"\t";
		//�W������NOT����
		Format(strBuff,"%d",itr->second->searchInfo.notContetFlag);
		strWrite+=strBuff +"\t";
		//����NOT����
		Format(strBuff,"%d",itr->second->searchInfo.notDateFlag);
		strWrite+=strBuff +"\t";
		//FreeCA���[�h
		Format(strBuff,"%d",itr->second->searchInfo.freeCAFlag);
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
		//�^��ς��̃`�F�b�N����
		Format(strBuff,"%d",itr->second->searchInfo.chkRecEnd);
		strWrite+=strBuff +"\t";
		//�^��ς��̃`�F�b�N�Ώۊ���
		Format(strBuff,"%d",itr->second->searchInfo.chkRecDay);
		strWrite+=strBuff +"\t";

		strWrite+="\r\n";
		WriteFile(hFile, strWrite.c_str(), (DWORD)strWrite.length(), &dwWrite, NULL);
	}

	CloseHandle(hFile);

	return TRUE;
}

BOOL CParseEpgAutoAddText::AddData(EPG_AUTO_ADD_DATA* item)
{
	if( item == NULL ){
		return FALSE;
	}

	item->dataID = GetNextID();
	EPG_AUTO_ADD_DATA* setItem = new EPG_AUTO_ADD_DATA;
	*setItem = *item;

	this->dataIDMap.insert( pair<DWORD, EPG_AUTO_ADD_DATA*>(setItem->dataID, setItem) );

	return TRUE;
}

BOOL CParseEpgAutoAddText::ChgData(EPG_AUTO_ADD_DATA* item)
{
	if( item == NULL ){
		return FALSE;
	}

	map<DWORD, EPG_AUTO_ADD_DATA*>::iterator itr;
	itr = this->dataIDMap.find(item->dataID);
	if( itr == this->dataIDMap.end() ){
		return FALSE;
	}
	*(itr->second) = *item;


	return TRUE;
}

BOOL CParseEpgAutoAddText::DelData(DWORD dataID)
{
	map<DWORD, EPG_AUTO_ADD_DATA*>::iterator itr;
	itr = this->dataIDMap.find(dataID);
	if( itr != this->dataIDMap.end() ){
		this->dataIDMap.erase(itr);
	}

	return TRUE;
}
