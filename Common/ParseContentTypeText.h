#ifndef __PARSE_CONTENT_TYPE_TEXT_H__
#define __PARSE_CONTENT_TYPE_TEXT_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////
//�����ϊ����t�@�C���́uSearchChg.txt�v�̓ǂݍ��݂��s�����߂̃N���X
//�r������Ȃǂ͍s���Ă��Ȃ����߁A�����X���b�h����̃A�N�Z�X�͏�ʑw�Ŕr����
//����s������
////////////////////////////////////////////////////////////////////////////
class CParseContentTypeText
{
public:
	CParseContentTypeText(void);
	~CParseContentTypeText(void);

	//SearchChg.txt�̓ǂݍ��݂��s��
	//�����F
	// file_path	SearchChg.txt�̃t���p�X
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ParseText(LPCWSTR filePath);

	//�ǂݍ��񂾕ϊ��������ɕ�����̕ϊ����s��
	//�����F
	// chgText		�ϊ����s��������
	//�߂�l�F
	void GetMimeType(wstring ext, wstring& mimeType);

protected:
	map<wstring,wstring> typeList;

protected:
	BOOL Parse1Line(string parseLine, wstring& oldText, wstring& newText);
};

#endif
