#ifndef __PARSE_CH_TEXT_4_H__
#define __PARSE_CH_TEXT_4_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////
//�`�����l�����t�@�C���́uChSet4.txt�v�̓ǂݍ��݂ƕۑ��������s�����߂̃N���X
//�r������Ȃǂ͍s���Ă��Ȃ����߁A�����X���b�h����̃A�N�Z�X�͏�ʑw�Ŕr����
//����s������
////////////////////////////////////////////////////////////////////////////
class CParseChText4
{
public:
	//�`�����l������multimap
	//�L�[��ONID<<32 | TSID<<16 | SID
	//Read�݂̂Œǉ��Ȃǂ̑���̓����o�֐��g�p���邱��
	multimap<LONGLONG, CH_DATA4> chList;

public:
	CParseChText4(void);
	~CParseChText4(void);

	//ChSet4.txt�̓ǂݍ��݂��s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// file_path		[IN]ChSet4.txt�̃t���p�X
	BOOL ParseText(
		LPCWSTR filePath
		);
	
	//�`�����l������ǉ�����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// ch_info			[IN]�ǉ�����`�����l�����
	BOOL AddCh(
		CH_DATA4 chInfo
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
		WORD serviceID);

	//�`�����l�������폜
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// space		[IN]�����`�����l��
	// ch			[IN]�����`�����l��
	BOOL DelCh(
		WORD space,
		WORD ch);

	//�`�����l�������폜
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// space		[IN]�����`�����l��
	// ch			[IN]�����`�����l��
	// serviceID				[IN]ServiceID
	BOOL DelChService(
		WORD space,
		WORD ch,
		WORD serviceID);

	//���݂̏���ChSet4.txt�ɏ㏑���ۑ�����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// file_path				[IN]ChSet4.txt�̃t���p�X(NULL�œǂݍ��ݎ��̃t�@�C���p�X�g�p)
	BOOL SaveChText(
		LPCWSTR filePath = NULL
		);
	
	CParseChText4& operator=(const CParseChText4& o)
	{
		chList = o.chList;
		return *this;
	};

protected:
	wstring filePath;

protected:
	BOOL Parse1Line(string parseLine, CH_DATA4* chInfo );
};

#endif
