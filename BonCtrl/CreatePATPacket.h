#pragma once

#include "../Common/Util.h"
#include "../Common/EpgTimerUtil.h"
#include "../Common/ErrDef.h"

class CCreatePATPacket
{
public:
	typedef struct _PROGRAM_PID_INFO{
		WORD SID;
		WORD PMTPID;
	}PROGRAM_PID_INFO;
public:
	CCreatePATPacket(void);
	~CCreatePATPacket(void);

	//�쐬PAT�̃p�����[�^��ݒ�
	//�����F
	// TSID				[IN]TransportStreamID
	// PIDMap			[IN]PMT�̃��X�g�i�L�[PMT��PID�j
	void SetParam(
		WORD TSID,
		map<WORD, PROGRAM_PID_INFO>* PIDMap
	);

	//�쐬PAT�̃o�b�t�@�|�C���^���擾
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// buff				[OUT]�쐬����PAT�p�P�b�g�ւ̃|�C���^�i����Ăяo�����܂ŗL���j
	// buffSize			[OUT]buff�̃T�C�Y
	// incrementFlag	[IN]TS�p�P�b�g��Counter���C���N�������g���邩�ǂ����iTRUE:����AFALSE�F���Ȃ��j
	BOOL GetPacket(
		BYTE** buff,
		DWORD* buffSize,
		BOOL incrementFlag = TRUE
	);

	//�쐬PAT�̃o�b�t�@���N���A
	void Clear();

protected:
	void CreatePAT();
	void CreatePacket();
	void IncrementCounter();

protected:
	BYTE version;
	BYTE counter;

	WORD TSID;
	map<WORD, PROGRAM_PID_INFO> PIDMap; //�L�[PMT��PID

	BYTE* packet;
	int packetSize;

	BYTE* PSI;
	int PSISize;
};
