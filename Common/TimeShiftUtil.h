#pragma once
#include "../BonCtrl/PacketInit.h"
#include "../BonCtrl/PMTUtil.h"
#include "../BonCtrl/SendUDP.h"
#include "../BonCtrl/SendTCP.h"
#include "../BonCtrl/CreatePATPacket.h"
#include "Util.h"
#include "TSPacketUtil.h"

class CTimeShiftUtil
{
public:
	CTimeShiftUtil(void);
	~CTimeShiftUtil(void);

	//UDP�ő��M���s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// udp		[IN]���M�N���X�BNULL�Œ�~�B
	BOOL SendUdp(
		CSendUDP* udp
		);

	//TCP�ő��M���s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// tcp		[IN]���M�N���X�BNULL�Œ�~�B
	BOOL SendTcp(
		CSendTCP* tcp
		);

	//�^�C���V�t�g�p�t�@�C�����J��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// filePath		[IN]�^�C���V�t�g�p�o�b�t�@�t�@�C���̃p�X
	// fileSize		[IN]�L���ȃt�@�C���T�C�Y�B-1�Ńt�@�C���T�C�Y���̂܂܂��L���B
	// fileMode		[IN]�^��ς݃t�@�C���Đ����[�h
	BOOL OpenTimeShift(
		LPCWSTR filePath,
		__int64 fileSize,
		BOOL fileMode
		);

	//�^�C���V�t�g���M���J�n����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL StartTimeShift();

	//�^�C���V�t�g���M���~����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL StopTimeShift();

	//���ݗL���ȃt�@�C���T�C�Y��ݒ肷��
	//�����F
	// fileSize		[IN]�L���ȃt�@�C���T�C�Y�B-1�Ńt�@�C���T�C�Y���̂܂܂��L���B
	void SetAvailableSize(__int64 fileSize);

	//���݂̑��M�t�@�C���ʒu���擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// filePos		[OUT]�t�@�C���ʒu
	BOOL GetCurrentFilePos(__int64* filePos);

	//���ݗL���ȃt�@�C���T�C�Y���擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// filePos		[OUT]�t�@�C���T�C�Y
	BOOL GetTotalFilePos(__int64* filePos);

	//���M�J�n�ʒu��ύX����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// filePos		[IN]�t�@�C���ʒu
	BOOL SetFilePos(__int64 filePos);

protected:
	HANDLE lockEvent;
	HANDLE lockBuffEvent;
	CPacketInit* packetInit;
	CSendUDP* sendUdp;
	CSendTCP* sendTcp;

	wstring filePath;
	WORD PCR_PID;

	BOOL fileMode;
	__int64 availableFileSize;
	__int64 currentFilePos;
	__int64 totalFileSize;

	HANDLE readThread;
	HANDLE readStopEvent;

	map<WORD, CPMTUtil*> pmtUtilMap; //�L�[PMT��PID
protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 5*1000);
	void UnLock(LPCWSTR log = NULL);
	BOOL LockBuff(LPCWSTR log = NULL, DWORD timeOut = 5*1000);
	void UnLockBuff(LPCWSTR log = NULL);

	static UINT WINAPI ReadThread(LPVOID param);
};

