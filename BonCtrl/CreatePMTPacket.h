#pragma once

#include "../Common/Util.h"
#include "../Common/ErrDef.h"
#include "../Common/EpgTimerUtil.h"
#include "../Common/TSPacketUtil.h"
#include "../Common/TSBuffUtil.h"

class CCreatePMTPacket
{
public:
	CCreatePMTPacket(void);
	~CCreatePMTPacket(void);

	//PMT�쐬���̃��[�h
	//�����F
	// needCaption			[IN]�����f�[�^���܂߂邩�ǂ����iTRUE:�܂߂�AFALSE�F�܂߂Ȃ��j
	// needData				[IN]�f�[�^�J���[�Z�����܂߂邩�ǂ����iTRUE:�܂߂�AFALSE�F�܂߂Ȃ��j
	void SetCreateMode(
		BOOL needCaption,
		BOOL needData
	);

	//�쐬���ƂȂ�PMT�̃p�P�b�g�����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// packet			//[IN] PMT�̃p�P�b�g
	DWORD AddData(
		CTSPacketUtil* packet
	);

	//�K�v��PID�����m�F
	//�߂�l�F
	// TRUE�i�K�v�j�AFALSE�i�s�K�v�j
	//�����F
	// PID				//[IN]�m�F����PID
	BOOL IsNeedPID(
		WORD PID
	);

	//�쐬PMT�̃o�b�t�@�|�C���^���擾
	//�߂�l�F
	// �쐬PMT�̃o�b�t�@�|�C���^
	//�����F
	// buff					[OUT]�쐬����PMT�p�P�b�g�ւ̃|�C���^�i����Ăяo�����܂ŗL���j
	// size					[OUT]buff�̃T�C�Y
	// incrementFlag		[IN]TS�p�P�b�g��Counter���C���N�������g���邩�ǂ����iTRUE:����AFALSE�F���Ȃ��j
	BOOL GetPacket(
		BYTE** buff,
		DWORD* size,
		BOOL incrementFlag = TRUE
	);

	//���������N���A
	void Clear();

	BYTE GetVersion();

protected:
	DWORD DecodePMT(BYTE* data, DWORD dataSize);
	void ClearSecondBuff();

	void CreatePMT();
	void CreatePacket();
	void IncrementCounter();
protected:
	CTSBuffUtil buffUtil;

	BOOL needCaption;
	BOOL needData;

	WORD lastPmtPID;
	WORD lastPcrPID;
	WORD lastPgNumber;
	BYTE lastVersion;

	BYTE* firstDescBuff;
	WORD firstDescBuffSize;

	typedef struct _SECOND_DESC_BUFF{
		BYTE stream_type;
		WORD elementary_PID;
		WORD ES_info_length;
		WORD descBuffSize;
		BYTE* descBuff;
		WORD quality;
		WORD qualityPID;
		_SECOND_DESC_BUFF(){
			stream_type = 0;
			elementary_PID = 0;
			ES_info_length = 0;
			descBuffSize = 0;
			descBuff = NULL;
			quality = 0;
			qualityPID = 0;
		}
		~_SECOND_DESC_BUFF(){
			SAFE_DELETE_ARRAY(descBuff);
		}
	}SECOND_DESC_BUFF;
	vector<SECOND_DESC_BUFF*> secondDescBuff;

	map<WORD,WORD> emmPIDMap;

	map<WORD,WORD> needPIDList;

	BYTE* createPSI;
	int createPSISize;
	
	BYTE* createPacket;
	int createPacketSize;

	BYTE createVer;
	BYTE createCounter;
};
