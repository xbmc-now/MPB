#include "stdafx.h"
#include "BonDriverUtil.h"

CBonDriverUtil::CBonDriverUtil(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL );

	this->settingFolderPath = L"";
	GetSettingPath(this->settingFolderPath);

	this->bonDriverFolderPath = L"";
	GetModuleFolderPath(this->bonDriverFolderPath);
	this->bonDriverFolderPath += BON_DLL_FOLDER;
	
	this->loadIndex = -1;
	this->loadTunerName = L"";
	this->initChSetFlag = FALSE;
	this->bonIF = NULL;
	this->bon2IF = NULL;
	this->module = NULL;

	this->setSpace = 0;
	this->setCh = 0;
}

CBonDriverUtil::~CBonDriverUtil(void)
{
	_CloseBonDriver();

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
}

BOOL CBonDriverUtil::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED){
		return FALSE;
	}
	return TRUE;
}

void CBonDriverUtil::UnLock(LPCWSTR log)
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
void CBonDriverUtil::SetSettingFolder(
	LPCWSTR settingFolderPath,
	LPCWSTR bonDriverFolderPath
)
{
	if( Lock() == FALSE ) return ;

	this->settingFolderPath = settingFolderPath;
	this->bonDriverFolderPath = bonDriverFolderPath;

	ChkFolderPath(this->settingFolderPath);
	ChkFolderPath(this->bonDriverFolderPath);

	bonDllMap.clear();

	wstring searchKey = L"";
	searchKey += this->bonDriverFolderPath;
	searchKey += L"\\BonDriver*.dll";

	WIN32_FIND_DATA findData;
	HANDLE find;

	//�w��t�H���_�̃t�@�C���ꗗ�擾
	find = FindFirstFile( searchKey.c_str(), &findData);
	if ( find == INVALID_HANDLE_VALUE ) {
		UnLock();
		return ;
	}
	do{
		if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 ){
			//�{���Ɋg���qDLL?
			if( IsDllFile(findData.cFileName) == TRUE ){
				//��������DLL���ꗗ�ɒǉ�
				BON_DRIVER_INFO Item;
				Format(Item.filePath, L"%s\\%s", this->bonDriverFolderPath.c_str(), findData.cFileName);
				Item.fileName = findData.cFileName;
				GetFileTitle( Item.fileName, Item.fileTitle );

				int index = (int)bonDllMap.size();

				this->bonDllMap.insert(pair<int, BON_DRIVER_INFO>(index, Item));
			}
		}
	}while(FindNextFile(find, &findData));

	FindClose(find);

	UnLock();
}

BOOL CBonDriverUtil::IsDllFile(wstring name)
{
	if( name.empty() == true ){
		return FALSE;
	}
	WCHAR szDrive[_MAX_DRIVE];
	WCHAR szDir[_MAX_DIR];
	WCHAR szFname[_MAX_FNAME];
	WCHAR szExt[_MAX_EXT];
	_wsplitpath_s( name.c_str(), szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT );

	if( CompareNoCase( szExt, L".dll" ) != 0 ){
		return FALSE;
	}

	return TRUE;
}

//BonDriver�t�H���_��BonDriver_*.dll���
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonList			[OUT]�����ł���BonDriver�ꗗ�imap�̃L�[ �����C���f�b�N�X�l�Amap�̒l BonDriver�t�@�C�����j
DWORD CBonDriverUtil::EnumBonDriver(
	map<int, wstring>* bonList
)
{
	if( Lock() == FALSE ) return ERR_FALSE;

	map<int, BON_DRIVER_INFO>::iterator itr;
	for( itr = this->bonDllMap.begin(); itr != this->bonDllMap.end(); itr++ ){
		bonList->insert(pair<int, wstring>(itr->first, itr->second.fileName));
	}

	UnLock();

	return NO_ERR;
}

//BonDriver�̃��[�h
//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�C���f�b�N�X�l�Ŏw��j
//�߂�l�F
// �G���[�R�[�h
//�����F
// index			[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃C���f�b�N�X�l
DWORD CBonDriverUtil::OpenBonDriver(
	int index,
	int openWait
)
{
	if( Lock() == FALSE ) return ERR_OPEN_TUNER;
	DWORD err = ERR_OPEN_TUNER;
	
	map<int, BON_DRIVER_INFO>::iterator itrF;
	itrF = this->bonDllMap.find(index);
	if( itrF != this->bonDllMap.end() ){
		err = _OpenBonDriver(itrF->second.filePath.c_str(), openWait);
		if( err == NO_ERR ){
			this->loadIndex = index;
		}
	}else{
		_OutputDebugString(L"��Open����BonDriver��������܂���");
		err = ERR_FIND_TUNER;
	}
	
	UnLock();
	return err;
}

//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�t�@�C�����Ŏw��j
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonDriverFile	[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃t�@�C����
DWORD CBonDriverUtil::OpenBonDriver(
	LPCWSTR bonDriverFile,
	int openWait
	)
{
	if( Lock() == FALSE ) return ERR_OPEN_TUNER;
	DWORD err = ERR_FIND_TUNER;
	map<int, BON_DRIVER_INFO>::iterator itrF;
	for( itrF = this->bonDllMap.begin(); itrF != this->bonDllMap.end(); itrF++ ){
		if( CompareNoCase(bonDriverFile, itrF->second.fileName) == 0 ){
			err = _OpenBonDriver(itrF->second.filePath.c_str(), openWait);
			if( err == NO_ERR ){
				this->loadIndex = itrF->first;
			}
			break;
		}
	}
	if( err == ERR_FIND_TUNER ){
		_OutputDebugString(L"��Open����BonDriver��������܂���");
		_OutputDebugString(bonDriverFile);
	}

	UnLock();
	return err;
}

//BonDriver�����[�h���̖{��
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonDriverFilePath		[IN] ���[�h����BonDriver�̃t�@�C���p�X
DWORD CBonDriverUtil::_OpenBonDriver(
	LPCWSTR bonDriverFilePath,
	int openWait
	)
{
	DWORD err = ERR_OPEN_TUNER;

	if( this->module != NULL ){
		_CloseBonDriver();
	}
	this->loadChMap.clear();
	this->loadIndex = -1;
	this->initChSetFlag = FALSE;
	this->loadTunerName = L"";

	this->module = ::LoadLibrary(bonDriverFilePath);
	if( this->module == NULL ){
		OutputDebugString(L"��BonDriver�����[�h�ł��܂���");
		OutputDebugString(bonDriverFilePath);
		return ERR_LOAD_MODULE;
	}
	IBonDriver* (*func)();
	func = (IBonDriver* (*)())::GetProcAddress( this->module , "CreateBonDriver");
	if( !func ){
		OutputDebugString(L"��GetProcAddress�Ɏ��s���܂���");
		err = ERR_INIT;
		goto ERR_END;
	}
	this->bonIF = func();
	try{
		this->bon2IF = dynamic_cast<IBonDriver2 *>(bonIF);
		BOOL open = this->bonIF->OpenTuner();
		if( open == FALSE ){
			Sleep(1000);
			open = this->bonIF->OpenTuner();
		}

		if( open == FALSE ){
			//Open���s
			OutputDebugString(L"��OpenTuner�Ɏ��s���܂���");
			this->bonIF->CloseTuner();
			this->bonIF->Release();
			this->bonIF = NULL;
			this->bon2IF = NULL;
			err = ERR_OPEN_TUNER;
		}else{
			//Open����
			//�`���[�i�[���̎擾
			this->loadTunerName = this->bon2IF->GetTunerName();
			Replace(this->loadTunerName, L"(",L"�i");
			Replace(this->loadTunerName, L")",L"�j");
			//�`�����l���ꗗ�̎擾
			DWORD countSpace = 0;
			while(1){
				if( this->bon2IF->EnumTuningSpace(countSpace) != NULL ){
					BON_SPACE_INFO spaceItem;
					spaceItem.space = countSpace;
					spaceItem.spaceName = this->bon2IF->EnumTuningSpace(countSpace);
					DWORD countCh = 0;

					while(1){
						if( this->bon2IF->EnumChannelName(countSpace, countCh) != NULL ){
							BON_CH_INFO chItem;
							chItem.ch = countCh;
							chItem.chName = this->bon2IF->EnumChannelName(countSpace, countCh);
							if( chItem.chName.empty() == false ){
								spaceItem.chMap.insert(pair<DWORD,BON_CH_INFO>(countCh, chItem));
							}
						}else{
							break;
						}
						countCh++;
					}
					this->loadChMap.insert(pair<DWORD, BON_SPACE_INFO>(countSpace, spaceItem));
				}else{
					break;
				}
				countSpace++;
			}
			Sleep(openWait);
			this->initChSetFlag = FALSE;
			err = NO_ERR;
		}
	}catch(...){
		if( this->bonIF != NULL ){
			this->bonIF->CloseTuner();
			this->bonIF->Release();
			this->bonIF = NULL;
			this->bon2IF = NULL;
		}
		err = ERR_OPEN_TUNER;
	}
ERR_END:
	if( err != NO_ERR ){
		if( this->module != NULL ){
			::FreeLibrary( this->module );
			this->module=NULL;
		}
	}

	return err;
}

//���[�h���Ă���BonDriver�̊J��
//�߂�l�F
// �G���[�R�[�h
DWORD CBonDriverUtil::CloseBonDriver()
{
	if( Lock() == FALSE ) return ERR_FALSE;
	DWORD err = NO_ERR;
	err = _CloseBonDriver();
	UnLock();
	return err;
}

DWORD CBonDriverUtil::_CloseBonDriver()
{
	DWORD err = NO_ERR;
	if( this->bonIF != NULL ){
		this->bonIF->CloseTuner();
		this->bonIF->Release();
		this->bonIF = NULL;
		this->bon2IF = NULL;
	}
	if( this->module != NULL ){
		::FreeLibrary( this->module );
		this->module = NULL;
	}
	return err;
}

//���[�h����BonDriver�̏��擾
//Space��Ch�̈ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// spaceMap			[OUT] Space��Ch�̈ꗗ�imap�̃L�[ Space�j
DWORD CBonDriverUtil::GetOriginalChList(
	map<DWORD, BON_SPACE_INFO>* spaceMap
)
{
	if( Lock() == FALSE ) return ERR_FALSE;

	if( spaceMap == NULL || this->bon2IF == NULL ){
		UnLock();
		return ERR_INVALID_ARG;
	}

	*spaceMap = this->loadChMap;

	UnLock();
	return NO_ERR;
}

//BonDriver�̃`���[�i�[�����擾
//�߂�l�F
// �`���[�i�[��
wstring CBonDriverUtil::GetTunerName()
{
	wstring name = L"";
	if( Lock() == FALSE ) return name;

	name = this->loadTunerName;

	UnLock();
	return name;
}

//�`�����l���ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// space			[IN]�ύX�`�����l����Space
// ch				[IN]�ύX�`�����l���̕���Ch
DWORD CBonDriverUtil::SetCh(
	DWORD space,
	DWORD ch
	)
{
	if( Lock() == FALSE ) return FALSE;
	if( this->bon2IF == NULL ){
		UnLock();
		return ERR_NOT_INIT;
	}
	this->setSpace = space;
	this->setCh = ch;
	//����͏�Ƀ`�����l���ݒ�s��
	if( this->initChSetFlag == TRUE ){
		//�Q��ڈȍ~�͕ύX�������ꍇ�ɍs��
		if( space == this->bon2IF->GetCurSpace() &&
			ch == this->bon2IF->GetCurChannel() )
		{
			UnLock();
			return NO_ERR;
		}
	}
	if( this->bon2IF->SetChannel(space, ch) == FALSE ){
		Sleep(500);
		if( this->bon2IF->SetChannel(space, ch) == FALSE ){
			UnLock();
			return ERR_FALSE;
		}
	}
	this->initChSetFlag = TRUE;
	UnLock();
	return NO_ERR;
}

//���݂̃`�����l���擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// space			[IN]���݂̃`�����l����Space
// ch				[IN]���݂̃`�����l���̕���Ch
DWORD CBonDriverUtil::GetNowCh(
	DWORD* space,
	DWORD* ch
	)
{
	if( Lock() == FALSE ) return FALSE;
	if( this->bon2IF == NULL ){
		UnLock();
		return FALSE;
	}
	if( this->initChSetFlag == FALSE ){
		*space = 0xFFFFFFFF;
		*ch = 0xFFFFFFFF;
	}else{
		*space = this->bon2IF->GetCurSpace();
		*ch = this->bon2IF->GetCurChannel();
		this->setSpace = *space;
		this->setCh = *ch;
	}
	UnLock();
	return TRUE;
}

BOOL CBonDriverUtil::GetSetCh(
	DWORD* space,
	DWORD* ch
	)
{
	*space = this->setSpace;
	*ch = this->setCh;
	return TRUE;
}

//TS�X�g���[�����擾
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// data				[OUT]BonDriver�����o�b�t�@�̃|�C���^
// size				[OUT]�擾�o�b�t�@�̃T�C�Y
// remain			[OUT]���擾�o�b�t�@�̃T�C�Y
BOOL CBonDriverUtil::GetTsStream(
	BYTE **data,
	DWORD *size,
	DWORD *remain
	)
{
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = FALSE;
	if( this->bonIF == NULL ){
		UnLock();
		return FALSE;
	}
	try{
		ret = this->bonIF->GetTsStream(data, size, remain);
	}catch(...){
		ret = FALSE;
	}
	UnLock();
	return ret;
}

//�V�O�i�����x���̎擾
//�߂�l�F
// �V�O�i�����x��
float CBonDriverUtil::GetSignalLevel()
{
	if( Lock() == FALSE ) return 0;
	if( this->bonIF == NULL ){
		UnLock();
		return 0;
	}
	float fLevel = this->bonIF->GetSignalLevel();
	UnLock();
	return fLevel;
}

//Ch�ݒ�3�̃t�@�C���p�X���擾
//�߂�l�F
// Ch�ݒ�3�̃t�@�C���p�X
wstring CBonDriverUtil::GetChSet4Path()
{
	wstring ret = L"";
	if( Lock() == FALSE ) return ret;

	wstring fileTitle = L"";
	map<int, BON_DRIVER_INFO>::iterator itrF;
	itrF = this->bonDllMap.find(this->loadIndex);
	if( itrF != this->bonDllMap.end() ){
		fileTitle = itrF->second.fileTitle;
	}

	wstring tunerName = this->loadTunerName;
	CheckFileName(tunerName);
	Format(ret, L"%s\\%s(%s).ChSet4.txt", this->settingFolderPath.c_str(), fileTitle.c_str(), tunerName.c_str() );


	UnLock();
	return ret;
}

//Ch�ݒ�4�̃t�@�C���p�X���擾
//�߂�l�F
// Ch�ݒ�4�̃t�@�C���p�X
wstring CBonDriverUtil::GetChSet5Path()
{
	wstring ret = L"";
	if( Lock() == FALSE ) return ret;

	wstring fileTitle = L"";
	map<int, BON_DRIVER_INFO>::iterator itrF;
	itrF = this->bonDllMap.find(this->loadIndex);
	if( itrF != this->bonDllMap.end() ){
		fileTitle = itrF->second.fileTitle;
	}

	Format(ret, L"%s\\ChSet5.txt", this->settingFolderPath.c_str() );

	UnLock();
	return ret;
}

//Open����BonDriver�̃C���f�b�N�X�l���擾
//�߂�l�F
// �C���f�b�N�X�l�i-1�Ŗ�Open�j
int CBonDriverUtil::GetOpenBonDriverIndex()
{
	if( Lock() == FALSE ) return -1;
	int index = this->loadIndex;
	UnLock();
	return index;
}

//Open����BonDriver�̃t�@�C�������擾
//�߂�l�F
// BonDriver�̃t�@�C�����i�g���q�܂ށj
wstring CBonDriverUtil::GetOpenBonDriverFileName()
{
	wstring ret = L"";
	if( Lock() == FALSE ) return ret;

	map<int, BON_DRIVER_INFO>::iterator itrF;
	itrF = this->bonDllMap.find(this->loadIndex);
	if( itrF != this->bonDllMap.end() ){
		ret = itrF->second.fileName;
	}

	UnLock();
	return ret;
}

//�w�蕨���`�����l���̃`�����l�������擾
//�߂�l�F
// BonDriver�Œ�`����Ă��镨���`�����l����
wstring CBonDriverUtil::GetChName(DWORD space, DWORD ch)
{
	wstring ret = L"";
	if( Lock() == FALSE ) return ret;

	map<DWORD, BON_SPACE_INFO>::iterator itrSpace;
	itrSpace = this->loadChMap.find(space);
	if( itrSpace != this->loadChMap.end() ){
		map<DWORD, BON_CH_INFO>::iterator itrCh;
		itrCh = itrSpace->second.chMap.find(ch);
		if( itrCh != itrSpace->second.chMap.end() ){
			ret = itrCh->second.chName;
		}
	}

	UnLock();
	return ret;
}
