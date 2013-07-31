#include "StdAfx.h"
#include "WriteTSFile.h"
#include <process.h>

#include "../Common/PathUtil.h"

CWriteTSFile::CWriteTSFile(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);
	this->buffLockEvent = _CreateEvent(FALSE, TRUE, NULL);

    this->outThread = NULL;
    this->outStopEvent = _CreateEvent(FALSE, FALSE, NULL);
	this->writeTotalSize = 0;
	this->maxBuffCount = -1;
	this->buffOverErr = FALSE;
}

CWriteTSFile::~CWriteTSFile(void)
{
	if( this->outThread != NULL ){
		::SetEvent(this->outStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->outThread, 10000) == WAIT_TIMEOUT ){
			::TerminateThread(this->outThread, 0xffffffff);
		}
		CloseHandle(this->outThread);
		this->outThread = NULL;
	}
	if( this->outStopEvent != NULL ){
		CloseHandle(this->outStopEvent);
		this->outStopEvent = NULL;
	}


	if( this->buffLockEvent != NULL ){
		CloseHandle(this->buffLockEvent);
		this->buffLockEvent = NULL;
	}
	for( size_t i=0; i<this->TSBuff.size(); i++ ){
		SAFE_DELETE(this->TSBuff[i])
	}
	this->TSBuff.clear();

	for( size_t i=0; i<this->fileList.size(); i++ ){
		SAFE_DELETE(this->fileList[i]);
	}
	this->fileList.clear();

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
}

BOOL CWriteTSFile::Lock(LPCWSTR log, DWORD timeOut)
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
				_OutputDebugString(L"��CWriteTSFile::Lock FALSE : %s", log);
			}else{
				OutputDebugString(L"��CWriteTSFile::Lock FALSE");
			}
		return FALSE;
	}
	return TRUE;
}

void CWriteTSFile::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

//�t�@�C���ۑ����J�n����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// fileName				[IN]�ۑ��t�@�C����
// overWriteFlag		[IN]����t�@�C�������ݎ��ɏ㏑�����邩�ǂ����iTRUE�F����AFALSE�F���Ȃ��j
// createSize			[IN]�t�@�C���쐬���Ƀf�B�X�N�ɗ\�񂷂�e��
// saveFolder			[IN]�g�p����t�H���_�ꗗ
// saveFolderSub		[IN]HDD�̋󂫂��Ȃ��Ȃ����ꍇ�Ɉꎞ�I�Ɏg�p����t�H���_
BOOL CWriteTSFile::StartSave(
	wstring fileName,
	BOOL overWriteFlag,
	ULONGLONG createSize,
	vector<REC_FILE_SET_INFO>* saveFolder,
	vector<wstring>* saveFolderSub,
	int maxBuffCount
)
{
	if( Lock(L"StartSave") == FALSE ) return FALSE;
	BOOL ret = TRUE;

	this->exceptionErr = FALSE;
	this->buffOverErr = FALSE;
	this->maxBuffCount = maxBuffCount;

	if( saveFolder->size() == 0 ){
		UnLock();
		_OutputDebugString(L"CWriteTSFile::StartSave Err saveFolder 0");
		return FALSE;
	}
	
	if( this->outThread == NULL || this->fileList.size() == 0 ){
		this->writeTotalSize = 0;
		this->subRecFlag = FALSE;
		this->saveFileName = fileName;
		this->overWriteFlag = overWriteFlag;
		this->saveFolder = *saveFolder;
		this->saveFolderSub = *saveFolderSub;
		BOOL firstFreeChek = TRUE;
		for( size_t i=0; i<this->saveFolder.size(); i++ ){
			SAVE_INFO* item = new SAVE_INFO;
			if( this->saveFolder[i].writePlugIn.size() == 0 ){
				this->saveFolder[i].writePlugIn = L"Write_Default.dll";
			}
			wstring plugInPath = L"";
			GetModuleFolderPath(plugInPath);
			plugInPath += L"\\Write\\";
			plugInPath += this->saveFolder[i].writePlugIn.c_str();

			item->writeUtil = new CWritePlugInUtil;
			if(item->writeUtil->Initialize(plugInPath.c_str() ) == FALSE ){
				_OutputDebugString(L"CWriteTSFile::StartSave Err 3");
				SAFE_DELETE(item);
			}else{
				wstring folderPath = this->saveFolder[i].recFolder;
				if( CompareNoCase(this->saveFolder[i].writePlugIn, L"Write_Default.dll" ) == 0 ){
					//�f�t�H���g�̏ꍇ�͋󂫗e�ʂ����炩���߃`�F�b�N
					if( createSize > 0 ){
						if( ChkFreeFolder(createSize, this->saveFolder[i].recFolder) == FALSE ){
							if( GetFreeFolder(createSize, folderPath) == TRUE ){
								//�󂫂Ȃ������̂ŃT�u�t�H���_�ɘ^��
								this->subRecFlag = TRUE;
							}
						}
					}
				}
				ChkFolderPath(folderPath);

				wstring recPath;
				recPath = folderPath;
				recPath += L"\\";
				if( this->saveFolder[i].recFileName.size() == 0 ){
					recPath += fileName;
					item->recFileName = fileName;
				}else{
					recPath += this->saveFolder[i].recFileName;
					item->recFileName = this->saveFolder[i].recFileName;
				}
				//�J�n
				BOOL startRes = item->writeUtil->StartSave(recPath.c_str(), this->overWriteFlag, createSize);
				if( startRes == FALSE ){
					_OutputDebugString(L"CWriteTSFile::StartSave Err 2");
					//�G���[���T�u�t�H���_�Ń��g���C
					if( GetFreeFolder(createSize, folderPath) == TRUE ){
						//�󂫂Ȃ������̂ŃT�u�t�H���_�ɘ^��
						this->subRecFlag = TRUE;
					}
					ChkFolderPath(folderPath);
					recPath = folderPath;
					recPath += L"\\";
					if( this->saveFolder[i].recFileName.size() == 0 ){
						recPath += fileName;
						item->recFileName = fileName;
					}else{
						recPath += this->saveFolder[i].recFileName;
						item->recFileName = this->saveFolder[i].recFileName;
					}
					startRes = item->writeUtil->StartSave(recPath.c_str(), this->overWriteFlag, createSize);
				}
				if( startRes == TRUE ){
					WCHAR saveFilePath[512] = L"";
					DWORD saveFilePathSize = 512;
					item->writeUtil->GetSaveFilePath(saveFilePath, &saveFilePathSize);

					item->recFilePath = saveFilePath;
					item->freeChk = firstFreeChek;
					item->overWriteFlag = overWriteFlag;
					this->fileList.push_back(item);
					if( i==0 ){
						this->mainSaveFilePath = saveFilePath;
					}
					firstFreeChek = FALSE;
				}else{
					SAFE_DELETE(item);
				}
			}
		}

		if( this->fileList.size() > 0 ){
			//��M�X���b�h�N��
			ResetEvent(this->outStopEvent);
			this->outThread = (HANDLE)_beginthreadex(NULL, 0, OutThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
			SetThreadPriority( this->outThread, THREAD_PRIORITY_NORMAL );
			ResumeThread(this->outThread);
		}else{
			_OutputDebugString(L"CWriteTSFile::StartSave Err fileList 0");
			ret = FALSE;
		}
	}else{
		_OutputDebugString(L"CWriteTSFile::StartSave Err 1");
		ret = FALSE;
	}

	UnLock();
	return ret;
}

//�ۑ��T�u�t�H���_����󂫂̂���t�H���_�p�X���擾
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// needFreeSize			[IN]�Œ�K�v�ȋ󂫃T�C�Y
// freeFolderPath		[OUT]���������t�H���_
BOOL CWriteTSFile::GetFreeFolder(
	ULONGLONG needFreeSize,
	wstring& freeFolderPath
)
{
	BOOL ret = FALSE;

	for( int i = 0; i < (int)this->saveFolderSub.size(); i++ ){
		ULARGE_INTEGER stFree;
		ULARGE_INTEGER stTotal;
		ULARGE_INTEGER stTotalFree;
		if( _GetDiskFreeSpaceEx( this->saveFolderSub[i].c_str(), &stFree, &stTotal, &stTotalFree ) == TRUE ){
			if( stFree.QuadPart > (ULONGLONG)needFreeSize ){
				freeFolderPath = this->saveFolderSub[i];
				ChkFolderPath(freeFolderPath);
				ret = TRUE;
				break;
			}
		}
	}
	return ret;
}

//�w��t�H���_�̋󂫂����邩�`�F�b�N
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// needFreeSize			[IN]�Œ�K�v�ȋ󂫃T�C�Y
// chkFolderPath		[IN]�w��t�H���_
BOOL CWriteTSFile::ChkFreeFolder(
	ULONGLONG needFreeSize,
	wstring chkFolderPath
)
{
	BOOL ret = FALSE;

	ULARGE_INTEGER stFree;
	ULARGE_INTEGER stTotal;
	ULARGE_INTEGER stTotalFree;
	if( _GetDiskFreeSpaceEx( chkFolderPath.c_str(), &stFree, &stTotal, &stTotalFree ) == TRUE ){
		if( stFree.QuadPart > (ULONGLONG)needFreeSize ){
			ret = TRUE;
		}
	}
	return ret;
}

//�t�@�C���ۑ����I������
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
BOOL CWriteTSFile::EndSave()
{
	if( Lock(L"EndSave") == FALSE ) return FALSE;
	BOOL ret = TRUE;

	if( this->outThread != NULL ){
		::SetEvent(this->outStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->outThread, 10000) == WAIT_TIMEOUT ){
			::TerminateThread(this->outThread, 0xffffffff);
		}
		CloseHandle(this->outThread);
		this->outThread = NULL;
	}

	//�c���Ă���o�b�t�@�������o��
	if( WaitForSingleObject( this->buffLockEvent, 500 ) == WAIT_OBJECT_0 ){
		for( size_t i=0; i<this->TSBuff.size(); i++ ){
			for( size_t j=0; j<this->fileList.size(); j++ ){
				if( this->fileList[j]->writeUtil != NULL ){
					DWORD write = 0;
					this->fileList[j]->writeUtil->AddTSBuff( this->TSBuff[i]->data, this->TSBuff[i]->size, &write);
				}
			}
			SAFE_DELETE(this->TSBuff[i]);
		}
		this->TSBuff.clear();
		if( this->buffLockEvent != NULL ){
			SetEvent(this->buffLockEvent);
		}
	}

	for( size_t i=0; i<this->fileList.size(); i++ ){
		if( this->fileList[i]->writeUtil != NULL ){
			this->fileList[i]->writeUtil->StopSave();
			this->fileList[i]->writeUtil->UnInitialize();
			SAFE_DELETE(this->fileList[i]);
		}
	}
	this->fileList.clear();

	if( this->buffOverErr == TRUE ){
		ret = FALSE;
	}
	if( this->exceptionErr == TRUE ){
		ret = FALSE;
	}

	UnLock();
	return ret;
}

//�o�͗pTS�f�[�^�𑗂�
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// data		[IN]TS�f�[�^
// size		[IN]data�̃T�C�Y
BOOL CWriteTSFile::AddTSBuff(
	BYTE* data,
	DWORD size
	)
{
	if( Lock(L"AddTSBuff") == FALSE ) return FALSE;

	if( data == NULL || size == 0 || this->outThread == NULL){
		UnLock();
		return FALSE;
	}

	BOOL ret = TRUE;

	TS_DATA* item = new TS_DATA;
	item->size = size;
	item->data = new BYTE[size];
	memcpy(item->data, data, size);
	if( WaitForSingleObject( this->buffLockEvent, 500 ) == WAIT_OBJECT_0 ){
		if( this->maxBuffCount > 0 ){
			if(this->TSBuff.size() > (unsigned)this->maxBuffCount){
				_OutputDebugString(L"��writeBuffList MaxOver");
				this->buffOverErr = TRUE;
				size_t startIndex = this->maxBuffCount;
				if( this->maxBuffCount < 1000 ){
					startIndex = 0;
				}else{
					startIndex -= 1000;
				}
				for( size_t i=startIndex; i<this->TSBuff.size(); i++ ){
					SAFE_DELETE(this->TSBuff[i]);
				}
				vector<TS_DATA*>::iterator itr;
				itr = this->TSBuff.begin();
				advance(itr,startIndex);
				this->TSBuff.erase( itr, this->TSBuff.end() );
			}
		}
		this->TSBuff.push_back(item);
		if( this->buffLockEvent != NULL ){
			SetEvent(this->buffLockEvent);
		}
	}
	UnLock();
	return ret;
}

UINT WINAPI CWriteTSFile::OutThread(LPVOID param)
{
	CWriteTSFile* sys = (CWriteTSFile*)param;
	while(1){
		if( ::WaitForSingleObject(sys->outStopEvent, 0) != WAIT_TIMEOUT ){
			//�L�����Z�����ꂽ
			break;
		}
		//�o�b�t�@����f�[�^���o��
		TS_DATA* data = NULL;
		try{
			if( WaitForSingleObject( sys->buffLockEvent, 500 ) == WAIT_OBJECT_0 ){
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
			_OutputDebugString(L"����CWriteTSFile::OutThread Exception1");
			sys->exceptionErr = TRUE;
			continue ;
		}

		if( data != NULL ){
			for( size_t i=0; i<sys->fileList.size(); i++ ){
				try{
					if( sys->fileList[i]->writeUtil != NULL ){
						DWORD write = 0;
						if( sys->fileList[i]->writeUtil->AddTSBuff( data->data, data->size, &write) == FALSE ){
							//�󂫂��Ȃ��Ȃ���
							sys->writeTotalSize = -1;
							sys->fileList[i]->writeUtil->StopSave();

							if( sys->fileList[i]->freeChk == TRUE ){
								//���̋󂫂�T��
								wstring freeFolderPath = L"";
								if( sys->GetFreeFolder(200*1024*1024, freeFolderPath) == TRUE ){
									wstring recFilePath = freeFolderPath;
									recFilePath += L"\\";
									recFilePath += sys->fileList[i]->recFileName;

									//�J�n
									if( sys->fileList[i]->writeUtil->StartSave(recFilePath.c_str(), sys->fileList[i]->overWriteFlag, 0) == FALSE ){
										//���s�����̂ŏI���
										SAFE_DELETE(sys->fileList[i]->writeUtil);
									}else{
										WCHAR saveFilePath[512] = L"";
										DWORD saveFilePathSize = 512;
										sys->fileList[i]->writeUtil->GetSaveFilePath(saveFilePath, &saveFilePathSize);
										sys->fileList[i]->subRecPath.push_back(saveFilePath);
										sys->subRecFlag = TRUE;

										if( data->size > write ){
											sys->fileList[i]->writeUtil->AddTSBuff( data->data+write, data->size-write, &write);
										}
									}
								}
							}
						}
					}
				}catch(...){
					sys->fileList[i]->writeUtil = NULL;
					sys->exceptionErr = TRUE;
					_OutputDebugString(L"����CWriteTSFile::OutThread Exception2");
				}
			}

			sys->writeTotalSize += data->size;

			SAFE_DELETE(data);
		}else{
			Sleep(10);
		}
	}
	return 0;
}

//�^�撆�̃t�@�C���̃t�@�C���p�X���擾����
//�����F
// filePath			[OUT]�ۑ��t�@�C����
// subRecFlag		[OUT]�T�u�^�悪�����������ǂ���
void CWriteTSFile::GetSaveFilePath(
	wstring* filePath,
	BOOL* subRecFlag
	)
{
	if( Lock(L"GetSaveFilePath") == FALSE ) return ;

	*filePath = this->mainSaveFilePath;
	*subRecFlag = this->subRecFlag;

	UnLock();
}

//�^�撆�̃t�@�C���̏o�̓T�C�Y���擾����
//�����F
// writeSize			[OUT]�ۑ��t�@�C����
void CWriteTSFile::GetRecWriteSize(
	__int64* writeSize
	)
{
	if( Lock(L"GetRecWriteSize") == FALSE ) return ;

	if( writeSize != NULL ){
		*writeSize = this->writeTotalSize;
	}
	UnLock();
}
