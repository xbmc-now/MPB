#pragma once

#include <windows.h>

#include "../Common/Util.h"

class CPacketInit
{
public:
	CPacketInit(void);
	~CPacketInit(void);

	//���̓o�b�t�@��188�o�C�g�P�ʂ�TS�ɕϊ����A188�̔{���ɂȂ�悤�ɂ��낦��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// inData			[IN]����TS�f�[�^
	// inSize			[IN]inData�̃T�C�Y�iBYTE�P�ʁj
	// outData			[OUT]188�o�C�g�ɐ��񂵂��o�b�t�@�i�Ăяo������delete����K�v����j
	// outSize			[OUT]outData�̃T�C�Y�iBYTE�P�ʁj
	BOOL GetTSData(
		BYTE* inData,
		DWORD inSize,
		BYTE** outData,
		DWORD* outSize
		);

	//�����o�b�t�@�̃N���A
	void ClearBuff();

protected:
	BYTE* nextStartBuff;
	DWORD nextStartSize;

	typedef enum TS_MODE{
		TS_NONE = 0,
		TS_188 = 188,
		TS_192 = 192,
		TS_204 = 204
	};
	TS_MODE TSMode;

	int lastOffset;

	BYTE* copyBuff;
	DWORD copyBuffSize;

protected:
	//���̓o�b�t�@��188�o�C�g�P�ʂ�TS�ɂ���offset�ʒu��1�p�P�b�g�̃T�C�Y���擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// data				[IN]����TS�f�[�^
	// size				[IN]data�̃T�C�Y�iBYTE�P�ʁj
	// offset			[OUT]nextStartBuff�̐擪��0�Ƃ����J�n�I�t�Z�b�g�ʒu
	// packetSize		[OUT]1�p�P�b�g�̃T�C�Y
	BOOL CheckSync(
		BYTE* data,
		DWORD size,
		DWORD* offset,
		DWORD* packetSize
		);
};
