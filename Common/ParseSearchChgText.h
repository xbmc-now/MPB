#ifndef __PARSE_SEARCH_CHG_TEXT_H__
#define __PARSE_SEARCH_CHG_TEXT_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////
//�����ϊ����t�@�C���́uSearchChg.txt�v�̓ǂݍ��݂��s�����߂̃N���X
//�r������Ȃǂ͍s���Ă��Ȃ����߁A�����X���b�h����̃A�N�Z�X�͏�ʑw�Ŕr����
//����s������
////////////////////////////////////////////////////////////////////////////
class CParseSearchChgText
{
public:
	CParseSearchChgText(void);
	~CParseSearchChgText(void);

	//SearchChg.txt�̓ǂݍ��݂��s��
	//�����F
	// file_path	SearchChg.txt�̃t���p�X
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ParseReserveText(LPCWSTR filePath);
	//�ǂݍ��񂾕ϊ��������ɕ�����̕ϊ����s��
	//�����F
	// chgText		�ϊ����s��������
	//�߂�l�F
	void ChgText(wstring& chgText);

protected:
	typedef struct _CHG_INFO{
		wstring oldText;	//�ϊ��Ώۂ̕�����
		wstring newText;	//�ϊ����镶����
	} CHG_INFO;
	vector<CHG_INFO> chgKey;

protected:
	BOOL Parse1Line(string parseLine, wstring& oldText, wstring& newText);
};

#endif
