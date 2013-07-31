#ifndef __PARSE_CH_TEXT_5_H__
#define __PARSE_CH_TEXT_5_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////
//�`�����l�����t�@�C���́uChSet5.txt�v�̓ǂݍ��݂ƕۑ��������s�����߂̃N���X
//�r������Ȃǂ͍s���Ă��Ȃ����߁A�����X���b�h����̃A�N�Z�X�͏�ʑw�Ŕr����
//����s������
////////////////////////////////////////////////////////////////////////////
class CParseChText5
{
public:
	//�`�����l������multimap
	//�L�[��ONID<<32 | TSID<<16 | SID
	//Read�݂̂Œǉ��Ȃǂ̑���̓����o�֐��g�p���邱��
	map<LONGLONG, CH_DATA5> chList;

public:
	CParseChText5(void);
	~CParseChText5(void);

	//ChSet5.txt�̓ǂݍ��݂��s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// file_path		[IN]ChSet5.txt�̃t���p�X
	BOOL ParseText(
		LPCWSTR filePath
		);
		
	//�`�����l������ǉ�����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// ch_info			[IN]�ǉ�����`�����l�����
	BOOL AddCh(
		CH_DATA5 chInfo
		);
		
	//�`�����l�������폜
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// originalNetworkID		[IN]ONID
	// transportStreamID		[IN]TSID
	// serviceID				[IN]ServiceID
	BOOL DelCh(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID
		);

	//���݂̏���ChSet5.txt�ɏ㏑���ۑ�����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// file_path				[IN]ChSet5.txt�̃t���p�X(NULL�œǂݍ��ݎ��̃t�@�C���p�X�g�p)
	BOOL SaveChText(
		LPCWSTR filePath = NULL
		);
		
	//ONID��ServiceID����TSID���擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// originalNetworkID		[IN]ONID
	// serviceID				[IN]ServiceID
	WORD GetTSID(
		WORD originalNetworkID,
		WORD serviceID
		);

	//EPG�f�[�^�̎擾�Ώۂ���ݒ肷��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// originalNetworkID		[IN]ONID
	// transportStreamID		[IN]TSID
	// serviceID				[IN]ServiceID
	// enable					[IN]EPG�f�[�^�̎擾�Ώۂ��ǂ���
	BOOL SetEpgCapMode(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID,
		BOOL enable
		);

protected:
	wstring filePath;

protected:
	BOOL Parse1Line(string parseLine, CH_DATA5* chInfo );
};

#endif
