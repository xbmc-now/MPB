#ifndef __PARSE_RECINFO_TEXT_H__
#define __PARSE_RECINFO_TEXT_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////
//�^��ςݏ��t�@�C���́uRecInfo.txt�v�̓ǂݍ��݂ƕۑ��������s�����߂̃N���X
//�r������Ȃǂ͍s���Ă��Ȃ����߁A�����X���b�h����̃A�N�Z�X�͏�ʑw�Ŕr����
//����s������
////////////////////////////////////////////////////////////////////////////
class CParseRecInfoText
{
public:
	//�^��ς݈ꗗ��multimap
	//�L�[�͘^��J�n����
	//Read�݂̂Œǉ��Ȃǂ̑���̓����o�֐��g�p���邱��
	multimap<wstring, REC_FILE_INFO*> recInfoMap;
	//�^��ς݈ꗗ��map
	//�L�[�͒ʂ��ԍ�
	//Read�݂̂Œǉ��Ȃǂ̑���̓����o�֐��g�p���邱��
	map<DWORD, REC_FILE_INFO*> recIDMap;

public:
	CParseRecInfoText(void);
	~CParseRecInfoText(void);

	//RecInfo.txt�̓ǂݍ��݂��s��
	//�����F
	// file_path	RecInfo.txt�̃t���p�X
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ParseRecInfoText(LPCWSTR filePath);
	//���݂̏���Reserve.txt�ɏ㏑���ۑ�����
	//�����F
	// file_path	RecInfo.txt�̃t���p�X(NULL�œǂݍ��ݎ��̃t�@�C���p�X�g�p)
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL SaveRecInfoText(LPCWSTR filePath = NULL);

	//�^��ςݏ���ǉ�����
	//�����F
	// item			�ǉ�����^��ςݏ��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL AddRecInfo(REC_FILE_INFO* item);
	//�^��ςݏ����폜����
	//�����F
	// id	�폜����^��ςݏ���ID
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL DelRecInfo(DWORD id);
	//�v���e�N�g����ύX����
	//�����F
	// id	�ύX����^��ςݏ���ID
	// flag	�v���e�N�g�t���O
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ChgProtectRecInfo(DWORD id, BYTE flag);

	void GetProtectFiles(map<wstring, wstring>* fileMap);

	//�ꗗ�̎����폜���s�����̐ݒ�
	//�����F
	// keep_count		�c���Ă������̌�
	// auto_del_flag	�����폜���s�����ǂ����iTRUE:�s���AFALSE�F�s��Ȃ��j
	void SetAutoDel(DWORD keepCount, BOOL autoDelFlag){ this->keepCount = keepCount; this->autoDelFlag = autoDelFlag; };

protected:
	wstring loadFilePath;
	DWORD keepCount;
	BOOL autoDelFlag;

	DWORD nextID;

	map<wstring, wstring> protectFileList;
protected:
	BOOL Parse1Line(string parseLine, REC_FILE_INFO* item );
	void AutoDelInfo(DWORD keepCount);
	DWORD GetNextReserveID();

	void DelTS_InfoFile(wstring tsFilePath);
	void AddPtotectFileList(wstring tsFilePath);
	void DelPtotectFileList(wstring tsFilePath);
};

#endif
