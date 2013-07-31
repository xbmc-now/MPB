#ifndef __PARSE_MANUAL_AUTO_ADD_TEXT_H__
#define __PARSE_MANUAL_AUTO_ADD_TEXT_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////
//�\����t�@�C���́uManualAutoAdd.txt�v�̓ǂݍ��݂ƕۑ��������s�����߂̃N���X
//�r������Ȃǂ͍s���Ă��Ȃ����߁A�����X���b�h����̃A�N�Z�X�͏�ʑw�Ŕr����
//����s������
////////////////////////////////////////////////////////////////////////////
class CParseManualAutoAddText
{
public:
	//�\��ꗗ��map
	//�L�[�͒ʂ��ԍ�
	//Read�݂̂Œǉ��Ȃǂ̑���̓����o�֐��g�p���邱��
	map<DWORD, MANUAL_AUTO_ADD_DATA*> dataIDMap;

public:
	CParseManualAutoAddText(void);
	~CParseManualAutoAddText(void);

	//ManualAutoAdd.txt�̓ǂݍ��݂��s��
	//�����F
	// file_path	ManualAutoAdd.txt�̃t���p�X
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ParseText(LPCWSTR filePath);
	//���݂̏���ManualAutoAdd.txt�ɏ㏑���ۑ�����
	//�����F
	// file_path	ManualAutoAdd.txt�̃t���p�X(NULL�œǂݍ��ݎ��̃t�@�C���p�X�g�p)
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL SaveText(LPCWSTR filePath = NULL);

	//����ǉ�����
	//�����F
	// item			�ǉ�������
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL AddData(MANUAL_AUTO_ADD_DATA* item);
	//����ύX����
	//�����F
	// item			�ύX������
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ChgData(MANUAL_AUTO_ADD_DATA* item);
	//�����폜����
	//�����F
	// dataID	�폜�������ID
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL DelData(DWORD dataID);

protected:
	wstring loadFilePath;

	DWORD nextID;
protected:
	BOOL Parse1Line(string parseLine, MANUAL_AUTO_ADD_DATA* item );
	DWORD GetNextID();

};

#endif
