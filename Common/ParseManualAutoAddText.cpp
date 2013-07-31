#include "StdAfx.h"
#include "ParseManualAutoAddText.h"
#include "TimeUtil.h"
#include "StringUtil.h"

CParseManualAutoAddText::CParseManualAutoAddText(void)
{
	this->loadFilePath = L"";
	this->nextID = 1;
}

CParseManualAutoAddText::~CParseManualAutoAddText(void)
{
	map<DWORD, MANUAL_AUTO_ADD_DATA*>::iterator itr;
	for( itr = this->dataIDMap.begin(); itr != this->dataIDMap.end(); itr++ ){
		SAFE_DELETE(itr->second)
	}
}

BOOL CParseManualAutoAddText::ParseText(LPCWSTR filePath)
{
	if( filePath == NULL ){
		return FALSE;
	}

	map<DWORD, MANUAL_AUTO_ADD_DATA*>::iterator itr;
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
				MANUAL_AUTO_ADD_DATA* item = new MANUAL_AUTO_ADD_DATA;
				BOOL bRet = Parse1Line(parseLine, item);
				if( bRet == FALSE ){
					SAFE_DELETE(item)
				}else{
					item->dataID = GetNextID();
					this->dataIDMap.insert( pair<DWORD, MANUAL_AUTO_ADD_DATA*>(item->dataID,item) );
				}
			}
		}
	}

	return TRUE;
}

BOOL CParseManualAutoAddText::Parse1Line(string parseLine, MANUAL_AUTO_ADD_DATA* item )
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
	//�Ώۗj��
	item->dayOfWeekFlag = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);
	
	//�^��J�n���ԁi00:00��0�Ƃ��ĕb�P�ʁj
	item->startTime = (DWORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�^�摍����
	item->durationSecond = (DWORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�ԑg��
	AtoW(strBuff, item->title);

	Separate( parseLine, "\t", strBuff, parseLine);

	//�T�[�r�X��
	AtoW(strBuff, item->stationName);

	Separate( parseLine, "\t", strBuff, parseLine);

	//ONID
	item->originalNetworkID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//TSID
	item->transportStreamID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//SID
	item->serviceID = (WORD)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);


	//�^��ݒ�
	//�^�惂�[�h
	item->recSetting.recMode = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�D��x
	item->recSetting.priority = (BYTE)atoi(strBuff.c_str());

	Separate( parseLine, "\t", strBuff, parseLine);

	//�Ǐ]
	item->recSetting.tuijyuuFlag = 0;

	Separate( parseLine, "\t", strBuff, parseLine);

	//�w��T�[�r�X���[�h
	item->recSetting.serviceMode = (DWORD)atoi(strBuff.c_str());
	
	Separate( parseLine, "\t", strBuff, parseLine);

	//�҂�����^��
	item->recSetting.pittariFlag = 0;
	
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

	return TRUE;
}

DWORD CParseManualAutoAddText::GetNextID()
{
	DWORD dataID = 0xFFFFFFFF;

	map<DWORD, MANUAL_AUTO_ADD_DATA*>::iterator itr;
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

BOOL CParseManualAutoAddText::SaveText(LPCWSTR filePath)
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

	map<DWORD, MANUAL_AUTO_ADD_DATA*>::iterator itr;
	for( itr=this->dataIDMap.begin(); itr != this->dataIDMap.end(); itr++ ){
		string strWrite="";
		string strBuff="";
		DWORD dwWrite;
		//dataID
		Format(strBuff, "%d",itr->second->dataID);
		strWrite+=strBuff +"\t";
		//�����L�[
		//�Ώۗj��
		Format(strBuff, "%d",itr->second->dayOfWeekFlag);
		strWrite+=strBuff +"\t";
		//�^��J�n���ԁi00:00��0�Ƃ��ĕb�P�ʁj
		Format(strBuff, "%d",itr->second->startTime);
		strWrite+=strBuff +"\t";
		//�^�摍����
		Format(strBuff, "%d",itr->second->durationSecond);
		strWrite+=strBuff +"\t";
		//�ԑg��
		WtoA(itr->second->title, strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//�T�[�r�X��
		WtoA(itr->second->stationName, strBuff);
		strWrite+=strBuff;
		strWrite+="\t";
		//ONID
		Format(strBuff, "%d",itr->second->originalNetworkID);
		strWrite+=strBuff +"\t";
		//TSID
		Format(strBuff, "%d",itr->second->transportStreamID);
		strWrite+=strBuff +"\t";
		//SID
		Format(strBuff, "%d",itr->second->serviceID);
		strWrite+=strBuff +"\t";

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

BOOL CParseManualAutoAddText::AddData(MANUAL_AUTO_ADD_DATA* item)
{
	if( item == NULL ){
		return FALSE;
	}

	MANUAL_AUTO_ADD_DATA* setItem = new MANUAL_AUTO_ADD_DATA;
	*setItem = *item;
	setItem->recSetting.pittariFlag = 0;
	setItem->recSetting.tuijyuuFlag = 0;

	setItem->dataID = GetNextID();

	this->dataIDMap.insert( pair<DWORD, MANUAL_AUTO_ADD_DATA*>(setItem->dataID, setItem) );

	return TRUE;
}

BOOL CParseManualAutoAddText::ChgData(MANUAL_AUTO_ADD_DATA* item)
{
	if( item == NULL ){
		return FALSE;
	}

	map<DWORD, MANUAL_AUTO_ADD_DATA*>::iterator itr;
	itr = this->dataIDMap.find(item->dataID);
	if( itr == this->dataIDMap.end() ){
		return FALSE;
	}
	*(itr->second) = *item;
	itr->second->recSetting.pittariFlag = 0;
	itr->second->recSetting.tuijyuuFlag = 0;


	return TRUE;
}

BOOL CParseManualAutoAddText::DelData(DWORD dataID)
{
	map<DWORD, MANUAL_AUTO_ADD_DATA*>::iterator itr;
	itr = this->dataIDMap.find(dataID);
	if( itr != this->dataIDMap.end() ){
		this->dataIDMap.erase(itr);
	}

	return TRUE;
}
