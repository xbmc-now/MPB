#pragma once

#include "../Common/Util.h"
#include "../Common/StructDef.h"
#include "../Common/ErrDef.h"
#include "../Common/StringUtil.h"
#include "../Common//WritePlugInUtil.h"

#include "BonCtrlDef.h"

class CWriteTSFile
{
public:
	CWriteTSFile(void);
	~CWriteTSFile(void);

	//�t�@�C���ۑ����J�n����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// fileName				[IN]�ۑ��t�@�C���p�X
	// overWriteFlag		[IN]����t�@�C�������ݎ��ɏ㏑�����邩�ǂ����iTRUE�F����AFALSE�F���Ȃ��j
	// createSize			[IN]�t�@�C���쐬���Ƀf�B�X�N�ɗ\�񂷂�e��
	// saveFolder			[IN]�g�p����t�H���_�ꗗ
	// saveFolderSub		[IN]HDD�̋󂫂��Ȃ��Ȃ����ꍇ�Ɉꎞ�I�Ɏg�p����t�H���_
	BOOL StartSave(
		wstring fileName,
		BOOL overWriteFlag,
		ULONGLONG createSize,
		vector<REC_FILE_SET_INFO>* saveFolder,
		vector<wstring>* saveFolderSub,
		int maxBuffCount
	);

	//�t�@�C���ۑ����I������
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL EndSave();

	//�o�͗pTS�f�[�^�𑗂�
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// data		[IN]TS�f�[�^
	// size		[IN]data�̃T�C�Y
	BOOL AddTSBuff(
		BYTE* data,
		DWORD size
		);

	//�^�撆�̃t�@�C���̃t�@�C���p�X���擾����
	//�����F
	// filePath			[OUT]�ۑ��t�@�C����
	// subRecFlag		[OUT]�T�u�^�悪�����������ǂ���
	void GetSaveFilePath(
		wstring* filePath,
		BOOL* subRecFlag
		);

	//�^�撆�̃t�@�C���̏o�̓T�C�Y���擾����
	//�����F
	// writeSize			[OUT]�ۑ��t�@�C����
	void GetRecWriteSize(
		__int64* writeSize
		);

protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 30*1000);
	void UnLock(LPCWSTR log = NULL);
	/*
	HANDLE OpenFile(
		wstring recFolderPath,
		wstring recFileName,
		BOOL overWriteFlag,
		ULONGLONG createSize,
		BOOL chkFree,
		wstring& recFilePath
		);
*/
	//�ۑ��T�u�t�H���_����󂫂̂���t�H���_�p�X���擾
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// needFreeSize			[IN]�Œ�K�v�ȋ󂫃T�C�Y
	// freeFolderPath		[OUT]���������t�H���_
	BOOL GetFreeFolder(
		ULONGLONG needFreeSize,
		wstring& freeFolderPath
	);

	//�w��t�H���_�̋󂫂����邩�`�F�b�N
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// needFreeSize			[IN]�Œ�K�v�ȋ󂫃T�C�Y
	// chkFolderPath		[IN]�w��t�H���_
	BOOL ChkFreeFolder(
		ULONGLONG needFreeSize,
		wstring chkFolderPath
	);

	static UINT WINAPI OutThread(LPVOID param);

	BOOL GetNextFileName(wstring recFolder, wstring fileName, wstring& recPath);

protected:
	HANDLE lockEvent;

	HANDLE buffLockEvent;
	vector<TS_DATA*> TSBuff;

	HANDLE outThread;
	HANDLE outStopEvent;

	typedef struct _SAVE_INFO{
		CWritePlugInUtil* writeUtil;
		BOOL freeChk;
		BOOL overWriteFlag;
		wstring recFilePath;
		vector<wstring> subRecPath;
		wstring recFileName;
		_SAVE_INFO(void){
			freeChk = FALSE;
			overWriteFlag = FALSE;
			recFilePath = L"";
			writeUtil = NULL;
			recFileName = L"";
		};
		~_SAVE_INFO(void){
			if( writeUtil != NULL ){
				writeUtil->UnInitialize();
				SAFE_DELETE(writeUtil);
			}
		};
	}SAVE_INFO;
	vector<SAVE_INFO*> fileList;

	wstring saveFileName;
	BOOL overWriteFlag;
	vector<REC_FILE_SET_INFO> saveFolder;
	vector<wstring> saveFolderSub;

	BOOL subRecFlag;
	__int64 writeTotalSize;
	wstring mainSaveFilePath;

	BOOL exceptionErr;
	int maxBuffCount;
	BOOL buffOverErr;
};

