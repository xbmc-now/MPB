#include "StdAfx.h"
#include "TimeShiftUtil.h"
#include <process.h>

CTimeShiftUtil::CTimeShiftUtil(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);
	this->lockBuffEvent = _CreateEvent(FALSE, TRUE, NULL);

    this->readThread = NULL;
    this->readStopEvent = _CreateEvent(FALSE, FALSE, NULL);

	this->packetInit = NULL;
	this->sendUdp = NULL;
	this->sendTcp = NULL;

	this->PCR_PID = 0xFFFF;
	this->fileMode = FALSE;
	this->availableFileSize = 0;
	this->currentFilePos = 0;
	this->totalFileSize = 0;
}


CTimeShiftUtil::~CTimeShiftUtil(void)
{
	if( this->readThread != NULL ){
		::SetEvent(this->readStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->readThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->readThread, 0xffffffff);
		}
		CloseHandle(this->readThread);
		this->readThread = NULL;
	}
	if( this->readStopEvent != NULL ){
		CloseHandle(this->readStopEvent);
		this->readStopEvent = NULL;
	}

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
	if( this->lockBuffEvent != NULL ){
		UnLockBuff();
		CloseHandle(this->lockBuffEvent);
		this->lockBuffEvent = NULL;
	}

	SAFE_DELETE(this->packetInit);

	map<WORD, CPMTUtil*>::iterator itrPmt;
	for( itrPmt = this->pmtUtilMap.begin(); itrPmt != this->pmtUtilMap.end(); itrPmt++ ){
		SAFE_DELETE(itrPmt->second);
	}
	this->pmtUtilMap.clear();
}

BOOL CTimeShiftUtil::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED ||
		dwRet == WAIT_TIMEOUT){
			OutputDebugString(L"��CTimeShiftUtil::Lock FALSE");
		return FALSE;
	}
	return TRUE;
}

void CTimeShiftUtil::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

BOOL CTimeShiftUtil::LockBuff(LPCWSTR log, DWORD timeOut)
{
	if( this->lockBuffEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockBuffEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED ||
		dwRet == WAIT_TIMEOUT){
			OutputDebugString(L"��CTimeShiftUtil::LockBuff FALSE");
		return FALSE;
	}
	return TRUE;
}

void CTimeShiftUtil::UnLockBuff(LPCWSTR log)
{
	if( this->lockBuffEvent != NULL ){
		SetEvent(this->lockBuffEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

//UDP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// udp		[IN]���M�N���X�BNULL�Œ�~�B
BOOL CTimeShiftUtil::SendUdp(
	CSendUDP* udp
	)
{
	if( Lock() == FALSE ) return FALSE;
	BOOL ret = TRUE;

	if( LockBuff() == TRUE ){
		this->sendUdp = udp;
		UnLockBuff();
	}

	UnLock();
	return ret;
}

//TCP�ő��M���s��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// tcp		[IN]���M�N���X�BNULL�Œ�~�B
BOOL CTimeShiftUtil::SendTcp(
	CSendTCP* tcp
	)
{
	if( Lock() == FALSE ) return FALSE;
	BOOL ret = TRUE;

	if( LockBuff() == TRUE ){
		this->sendTcp = tcp;
		UnLockBuff();
	}

	UnLock();
	return ret;
}

BOOL CTimeShiftUtil::OpenTimeShift(
	LPCWSTR filePath,
	__int64 fileSize,
	BOOL fileMode
	)
{
	if( Lock() == FALSE ) return FALSE;

	if( this->readThread != NULL ){
		::SetEvent(this->readStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->readThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->readThread, 0xffffffff);
		}
		CloseHandle(this->readThread);
		this->readThread = NULL;
	}

	map<WORD, CPMTUtil*>::iterator itrPmt;
	for( itrPmt = this->pmtUtilMap.begin(); itrPmt != this->pmtUtilMap.end(); itrPmt++ ){
		SAFE_DELETE(itrPmt->second);
	}
	this->pmtUtilMap.clear();

	this->PCR_PID = 0xFFFF;
	this->availableFileSize = fileSize;

	BOOL ret = TRUE;
	if( filePath == NULL ){
		UnLock();
		return FALSE;
	}
	if( this->packetInit == NULL ){
		this->packetInit = new CPacketInit;
	}
	HANDLE file = _CreateFile2( filePath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( file == INVALID_HANDLE_VALUE ){
		UnLock();
		return FALSE;
	}
	DWORD lenH = 0;
	DWORD lenL = GetFileSize(file, &lenH);
	__int64 totlaFileSize = ((__int64)lenH)<<32 | lenL;
	if( this->availableFileSize != -1 ){
		if( totlaFileSize > this->availableFileSize ){
			totlaFileSize = this->availableFileSize;
		}
	}

	//PCR��PID����͂���
	BYTE buff[188*256];
	DWORD totalReadSize = 0;
	while(this->PCR_PID == 0xFFFF && (totalReadSize < 1024*1024*40 || (__int64)totalReadSize < totlaFileSize)){
		DWORD readSize = 0;
		DWORD buffSize = 188*256;
		if( totalReadSize + buffSize > totlaFileSize ){
			buffSize = (DWORD)(totlaFileSize-totalReadSize);
		}
		if( ReadFile( file, buff, buffSize, &readSize, NULL ) == FALSE ){
			CloseHandle(file);
			UnLock();
			return FALSE;
		}
		if( readSize < buffSize){
			CloseHandle(file);
			UnLock();
			return FALSE;
		}
		totalReadSize += readSize;

		BYTE* data = NULL;
		DWORD dataSize = 0;
		if( this->packetInit->GetTSData(buff, readSize, &data, &dataSize) == TRUE ){
			for( DWORD i=0; i<dataSize; i+=188 ){
				CTSPacketUtil packet;
				if( packet.Set188TS(data + i, 188) == TRUE ){
					if( packet.transport_scrambling_control == 0 ){
						//PMT
						if( packet.payload_unit_start_indicator == 1 && packet.data_byteSize > 0){
							BYTE pointer = packet.data_byte[0];
							if( pointer+1 < packet.data_byteSize ){
								if( packet.data_byte[1+pointer] == 0x02 ){
									//PMT
									map<WORD, CPMTUtil*>::iterator itrPmt;
									itrPmt = this->pmtUtilMap.find(packet.PID);
									if( itrPmt == this->pmtUtilMap.end() ){
										CPMTUtil* util = new CPMTUtil;
										this->pmtUtilMap.insert(pair<WORD, CPMTUtil*>(packet.PID, util));
										if( util->AddPacket(&packet) == TRUE ){
											this->PCR_PID = util->PCR_PID;
											break;
										}
									}else{
										if( itrPmt->second->AddPacket(&packet) == TRUE ){
											this->PCR_PID = itrPmt->second->PCR_PID;
											break;
										}
									}
								}
							}
						}else{
							//PMT��2�p�P�b�g�ڂ��`�F�b�N
							map<WORD, CPMTUtil*>::iterator itrPmt;
							itrPmt = this->pmtUtilMap.find(packet.PID);
							if( itrPmt != this->pmtUtilMap.end() ){
								if( itrPmt->second->AddPacket(&packet) == TRUE ){
									this->PCR_PID = itrPmt->second->PCR_PID;
									break;
								}
							}
						}
					}
				}
			}
			SAFE_DELETE_ARRAY(data);
		}
	}

	if( this->PCR_PID == 0xFFFF ){
		ret = FALSE;
	}else{
		this->filePath = filePath;
		this->fileMode = fileMode;
		this->currentFilePos = 0;
		this->totalFileSize = totlaFileSize;
	}

	CloseHandle(file);

	UnLock();
	return ret;
}

//�^�C���V�t�g���M���J�n����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
BOOL CTimeShiftUtil::StartTimeShift()
{
	if( Lock() == FALSE ) return FALSE;
	BOOL ret = TRUE;
	if( this->filePath.size() == 0 ){
		ret = FALSE;
	}else{
		if( this->readThread == NULL ){
			//��M�X���b�h�N��
			ResetEvent(this->readStopEvent);
			this->readThread = (HANDLE)_beginthreadex(NULL, 0, ReadThread, (LPVOID)this, CREATE_SUSPENDED, NULL);
			SetThreadPriority( this->readThread, THREAD_PRIORITY_NORMAL );
			ResumeThread(this->readThread);
		}
	}

	UnLock();
	return ret;
}

BOOL CTimeShiftUtil::StopTimeShift()
{
	if( Lock() == FALSE ) return FALSE;
	BOOL ret = TRUE;

	if( this->readThread != NULL ){
		::SetEvent(this->readStopEvent);
		// �X���b�h�I���҂�
		if ( ::WaitForSingleObject(this->readThread, 15000) == WAIT_TIMEOUT ){
			::TerminateThread(this->readThread, 0xffffffff);
		}
		CloseHandle(this->readThread);
		this->readThread = NULL;
	}
	UnLock();
	return ret;
}

UINT WINAPI CTimeShiftUtil::ReadThread(LPVOID param)
{
	CTimeShiftUtil* sys = (CTimeShiftUtil*)param;
	BYTE buff[188*256];

	HANDLE file = _CreateFile2( sys->filePath.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( file == INVALID_HANDLE_VALUE ){
		return FALSE;
	}

	DWORD lenH = 0;
	DWORD lenL = GetFileSize(file, &lenH);
	__int64 totlaFileSize = ((__int64)lenH)<<32 | lenL;
	if( totlaFileSize < sys->currentFilePos ){
		sys->currentFilePos = totlaFileSize;
	}
	sys->totalFileSize = totlaFileSize;

	LONG setH = (LONG)(sys->currentFilePos>>32);
	LONG setL = (LONG)(sys->currentFilePos & 0x00000000FFFFFFFF);
	SetFilePointer(file, setL, &setH, FILE_BEGIN);

	__int64 initTime = -1;
	__int64 initTick = 0;

	DWORD errCount = 0;
	__int64 pcr_offset = 0;
	__int64 tick_offset = 0;

	while(1){
		if( ::WaitForSingleObject(sys->readStopEvent, 0) != WAIT_TIMEOUT ){
			//�L�����Z�����ꂽ
			break;
		}
		__int64 totalReadSize = sys->currentFilePos;
		DWORD readSize = 0;
		DWORD buffSize = 188*256;
		if( sys->availableFileSize != -1 && sys->fileMode == FALSE ){
			if( totalReadSize + buffSize > sys->availableFileSize ){
				buffSize = (DWORD)(sys->availableFileSize-totalReadSize);
			}
		}
		if( totalReadSize + buffSize > sys->totalFileSize && sys->fileMode == FALSE){
			//�t�@�C���T�C�Y�ς���Ă����Ă�͂��Ȃ̂ŊJ������
			CloseHandle(file);
			file = _CreateFile2( sys->filePath.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
			if( file == INVALID_HANDLE_VALUE ){
				return FALSE;
			}
			LONG setH = (LONG)(sys->currentFilePos>>32);
			LONG setL = (LONG)(sys->currentFilePos & 0x00000000FFFFFFFF);
			SetFilePointer(file, setL, &setH, FILE_BEGIN);

			lenH = 0;
			lenL = GetFileSize(file, &lenH);
			__int64 newSize = ((__int64)lenH)<<32 | lenL;

			sys->totalFileSize = newSize;
			Sleep(50);
			errCount++;
			if( errCount > (1000/50)*10){
				//10�b�ԃt�@�C���T�C�Y�̍X�V�Ȃ�����I�[�̂͂�
				CloseHandle(file);
				return FALSE;
			}
			continue;
		}
		BOOL ret = ReadFile( file, buff, buffSize, &readSize, NULL );
		if( ret == FALSE || readSize < buffSize){
			if( sys->fileMode == TRUE ){
				//�t�@�C�����[�h������G���[�͏I�[�̂͂�
				CloseHandle(file);
				return FALSE;
			}else{
				Sleep(50);
				errCount++;
				if( errCount > (1000/50)*10){
					//10�b�ԃt�@�C���T�C�Y�̍X�V�Ȃ�����I�[�̂͂�
					CloseHandle(file);
					return FALSE;
				}
			}
		}else{
			errCount = 0;
		}
		if( readSize == 0 ){
			Sleep(50);
			continue;
		}
		BYTE* data = NULL;
		DWORD dataSize = 0;
		__int64 base = -1;
		if( sys->packetInit->GetTSData(buff, readSize, &data, &dataSize) == TRUE ){
			if( sys->LockBuff() == TRUE ){
				for( DWORD i=0; i<dataSize; i+=188 ){
					CTSPacketUtil packet;
					if( packet.Set188TS(data + i, 188) == TRUE ){
						if( packet.transport_scrambling_control == 0 ){
							//PMT
							if( packet.payload_unit_start_indicator == 1 && packet.data_byteSize > 0){
								BYTE pointer = packet.data_byte[0];
								if( pointer+1 < packet.data_byteSize ){
									if( packet.data_byte[1+pointer] == 0x02 ){
										//PMT
										map<WORD, CPMTUtil*>::iterator itrPmt;
										itrPmt = sys->pmtUtilMap.find(packet.PID);
										if( itrPmt == sys->pmtUtilMap.end() ){
											CPMTUtil* util = new CPMTUtil;
											sys->pmtUtilMap.insert(pair<WORD, CPMTUtil*>(packet.PID, util));
											if( util->AddPacket(&packet) == TRUE ){
												if( sys->PCR_PID != util->PCR_PID ){
													//�`�����l���ς�����H
													initTime = -1;
													initTick = 0;
												}
												sys->PCR_PID = util->PCR_PID;
											}
										}else{
											if( itrPmt->second->AddPacket(&packet) == TRUE ){
												if( sys->PCR_PID != itrPmt->second->PCR_PID ){
													//�`�����l���ς�����H
													initTime = -1;
													initTick = 0;
												}
												sys->PCR_PID = itrPmt->second->PCR_PID;
											}
										}
									}
								}
							}else{
								//PMT��2�p�P�b�g�ڂ��`�F�b�N
								map<WORD, CPMTUtil*>::iterator itrPmt;
								itrPmt = sys->pmtUtilMap.find(packet.PID);
								if( itrPmt != sys->pmtUtilMap.end() ){
									if( itrPmt->second->AddPacket(&packet) == TRUE ){
										if( sys->PCR_PID != itrPmt->second->PCR_PID ){
											//�`�����l���ς�����H
											initTime = -1;
											initTick = 0;
										}
										sys->PCR_PID = itrPmt->second->PCR_PID;
									}
								}
							}

							if( packet.PID == sys->PCR_PID && packet.adaptation_field_length > 0 ){
								if( packet.PCR_flag == 1 ){
									base = (__int64)packet.program_clock_reference_base/90;
									if( initTime == -1 ){
										initTime = base;
										initTick = GetTickCount();
									}
								}
							}
						}
					}
				}
				sys->UnLockBuff();

				if( sys->sendUdp != NULL ){
					sys->sendUdp->SendData(data, dataSize);
				}
				if( sys->sendTcp != NULL ){
					sys->sendTcp->SendData(data, dataSize);
				}
			}
			SAFE_DELETE_ARRAY(data);
			sys->currentFilePos += readSize;
		}
		

		while(1){
			if( initTime != -1 && base != -1){
				if( base+pcr_offset<initTime ){
					//PCR�����߂����H
					pcr_offset = 0x1FFFFFFFF;
				}
				__int64 tick = GetTickCount();
				if( tick+tick_offset<initTick ){
					tick_offset = 0xFFFFFFFF;
				}
				if( (base+pcr_offset)-initTime >tick+tick_offset-initTick+100){
					if( ::WaitForSingleObject(sys->readStopEvent, 20) != WAIT_TIMEOUT ){
						//�L�����Z�����ꂽ
						goto Err_End;
					}
				}else{
					break;
				}
			}else{
				break;
			}
		}
	}

Err_End:
	CloseHandle(file);

	//����PAT�����Ď��񑗐M���Ƀ��Z�b�g�����悤�ɂ���
	BYTE endBuff[188*512];
	memset(endBuff, 0xFF, 188*512);
	map<WORD, CCreatePATPacket::PROGRAM_PID_INFO> PIDMap;
	CCreatePATPacket patUtil;
	patUtil.SetParam(1, &PIDMap);
	BYTE* patBuff;
	DWORD patSize=0;
	patUtil.GetPacket(&patBuff, &patSize);

	memcpy(endBuff, patBuff, patSize);
	for( int i=patSize; i<188*512; i+=188){
		endBuff[i] = 0x47;
		endBuff[i+1] = 0x1F;
		endBuff[i+2] = 0xFF;
		endBuff[i+3] = 0x10;
	}

	if( sys->sendUdp != NULL ){
		sys->sendUdp->SendData(endBuff, 188*512);
	}
	if( sys->sendTcp != NULL ){
		sys->sendTcp->SendData(endBuff, 188*512);
	}

	return 0;
}

//���ݗL���ȃt�@�C���T�C�Y��ݒ肷��
//�����F
// fileSize		[IN]�L���ȃt�@�C���T�C�Y�B-1�Ńt�@�C���T�C�Y���̂܂܂��L���B
void CTimeShiftUtil::SetAvailableSize(__int64 fileSize)
{
	if( Lock() == FALSE ) return ;

	this->availableFileSize = fileSize;

	if( fileSize == -1 ){
		HANDLE file = _CreateFile2( this->filePath.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if( file != INVALID_HANDLE_VALUE ){
			DWORD lenH = 0;
			DWORD lenL = GetFileSize(file, &lenH);
			__int64 newSize = ((__int64)lenH)<<32 | lenL;

			this->totalFileSize = newSize;
			CloseHandle(file);
		}
	}
	UnLock();
	return ;
}

//���݂̑��M�t�@�C���ʒu���擾����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// filePos		[OUT]�t�@�C���ʒu
BOOL CTimeShiftUtil::GetCurrentFilePos(__int64* filePos)
{
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = FALSE;
	if( filePos != NULL ){
		*filePos = this->currentFilePos;
		ret = TRUE;
	}
	//_OutputDebugString(L"currentFilePos::%I64d", this->currentFilePos);
	UnLock();
	return ret;
}

//���ݗL���ȃt�@�C���T�C�Y���擾����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// filePos		[OUT]�t�@�C���T�C�Y
BOOL CTimeShiftUtil::GetTotalFilePos(__int64* filePos)
{
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = FALSE;
	if( filePos != NULL ){
		if( this->fileMode == TRUE ){
			*filePos = this->totalFileSize;
		}else{
			if( this->availableFileSize != -1 ){
				*filePos = this->availableFileSize;
			}else{
				*filePos = this->totalFileSize;
			}
		}
		ret = TRUE;
	}
	//_OutputDebugString(L"totalFileSize::%I64d", *filePos);

	UnLock();
	return ret;
}

//���M�J�n�ʒu��ύX����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// filePos		[IN]�t�@�C���ʒu
BOOL CTimeShiftUtil::SetFilePos(__int64 filePos)
{
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = FALSE;
	this->currentFilePos = filePos;
	if( this->packetInit != NULL ){
		this->packetInit->ClearBuff();
	}

	UnLock();
	return ret;
}
