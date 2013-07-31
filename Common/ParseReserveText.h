#ifndef __PARSE_RESERVE_TEXT_H__
#define __PARSE_RESERVE_TEXT_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////
//�\����t�@�C���́uReserve.txt�v�̓ǂݍ��݂ƕۑ��������s�����߂̃N���X
//�r������Ȃǂ͍s���Ă��Ȃ����߁A�����X���b�h����̃A�N�Z�X�͏�ʑw�Ŕr����
//����s������
////////////////////////////////////////////////////////////////////////////
class CParseReserveText
{
public:
	//�\��ꗗ��multimap
	//�L�[�͘^��J�n����
	//Read�݂̂Œǉ��Ȃǂ̑���̓����o�֐��g�p���邱��
	multimap<wstring, RESERVE_DATA*> reserveMap;
	//�\��ꗗ��map
	//�L�[�͒ʂ��ԍ�
	//Read�݂̂Œǉ��Ȃǂ̑���̓����o�֐��g�p���邱��
	map<DWORD, RESERVE_DATA*> reserveIDMap;

public:
	CParseReserveText(void);
	~CParseReserveText(void);

	//Reserve.txt�̓ǂݍ��݂��s��
	//�����F
	// file_path	Reserve.txt�̃t���p�X
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ParseReserveText(LPCWSTR filePath);
	//Reserve.txt�̒ǉ��ǂݍ��݂��s��(ID��0�̂��̂����ǉ��ǂݍ���)
	//�����F
	// file_path	Reserve.txt�̃t���p�X(NULL�œǂݍ��ݎ��̃t�@�C���p�X�g�p)
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL AddParseReserveText(LPCWSTR filePath = NULL);
	//���݂̏���Reserve.txt�ɏ㏑���ۑ�����
	//�����F
	// file_path	Reserve.txt�̃t���p�X(NULL�œǂݍ��ݎ��̃t�@�C���p�X�g�p)
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL SaveReserveText(LPCWSTR filePath = NULL);

	//�\�����ǉ�����
	//�����F
	// item			�ǉ�����\����
	// reserveID	�ǉ�����ID
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL AddReserve(RESERVE_DATA* item, DWORD* reserveID);
	//�\�����ύX����
	//�����F
	// item			�ύX����\����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ChgReserve(RESERVE_DATA* item);
	//�\������폜����
	//�����F
	// reserve_id	�폜����\�����ID
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL DelReserve(DWORD reserveID);

	void SwapMap();
protected:
	wstring loadFilePath;

	DWORD nextID;
protected:
	BOOL Parse1Line(string parseLine, RESERVE_DATA* item );
	DWORD GetNextReserveID();

};

#endif
