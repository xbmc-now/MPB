#pragma once

#include <windows.h>

#include "../Common/Util.h"
#include "../Common/PathUtil.h"
#include "../Common/StringUtil.h"
#include "../Common/ErrDef.h"

#include "IBonDriver.h"
#include "IBonDriver2.h"
#include "BonCtrlDef.h"

class CBonDriverUtil
{
public:
	CBonDriverUtil(void);
	~CBonDriverUtil(void);

	//�����ݒ�
	//�ݒ�t�@�C���ۑ����BonDriver�t�H���_���w��
	//�����F
	// settingFolderPath		[IN]�ݒ�t�@�C���ۑ��t�H���_�p�X
	// bonDriverFolderPath		[IN]BonDriver�t�H���_�p�X
	void SetSettingFolder(
		LPCWSTR settingFolderPath,
		LPCWSTR bonDriverFolderPath
		);

	//BonDriver�t�H���_��BonDriver_*.dll���
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// bonList			[OUT]�����ł���BonDriver�ꗗ�imap�̃L�[ �����C���f�b�N�X�l�Amap�̒l BonDriver�t�@�C�����j
	DWORD EnumBonDriver(
		map<int, wstring>* bonList
		);

	//BonDriver�̃��[�h
	//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�C���f�b�N�X�l�Ŏw��j
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// index			[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃C���f�b�N�X�l
	DWORD OpenBonDriver(
		int index,
		int openWait = 200
		);

	//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�t�@�C�����Ŏw��j
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// bonDriverFile	[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃t�@�C����
	DWORD OpenBonDriver(
		LPCWSTR bonDriverFile,
		int openWait = 200
		);

	//���[�h���Ă���BonDriver�̊J��
	//�߂�l�F
	// �G���[�R�[�h
	DWORD CloseBonDriver();

	//���[�h����BonDriver�̏��擾
	//Space��Ch�̈ꗗ���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// spaceMap			[OUT] Space��Ch�̈ꗗ�imap�̃L�[ Space�j
	DWORD GetOriginalChList(
		map<DWORD, BON_SPACE_INFO>* spaceMap
	);

	//BonDriver�̃`���[�i�[�����擾
	//�߂�l�F
	// �`���[�i�[��
	wstring GetTunerName();

	//�`�����l���ύX
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// space			[IN]�ύX�`�����l����Space
	// ch				[IN]�ύX�`�����l���̕���Ch
	DWORD SetCh(
		DWORD space,
		DWORD ch
		);

	//���݂̃`�����l���擾
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// space			[IN]���݂̃`�����l����Space
	// ch				[IN]���݂̃`�����l���̕���Ch
	DWORD GetNowCh(
		DWORD* space,
		DWORD* ch
		);

	BOOL GetSetCh(
		DWORD* space,
		DWORD* ch
		);

	//TS�X�g���[�����擾
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// data				[OUT]BonDriver�����o�b�t�@�̃|�C���^
	// size				[OUT]�擾�o�b�t�@�̃T�C�Y
	// remain			[OUT]���擾�o�b�t�@�̃T�C�Y
	BOOL GetTsStream(
		BYTE **data,
		DWORD *size,
		DWORD *remain
		);

	//�V�O�i�����x���̎擾
	//�߂�l�F
	// �V�O�i�����x��
	float GetSignalLevel();

	//Ch�ݒ�3�̃t�@�C���p�X���擾
	//�߂�l�F
	// Ch�ݒ�3�̃t�@�C���p�X
	wstring GetChSet4Path();

	//Ch�ݒ�4�̃t�@�C���p�X���擾
	//�߂�l�F
	// Ch�ݒ�4�̃t�@�C���p�X
	wstring GetChSet5Path();

	//Open����BonDriver�̃C���f�b�N�X�l���擾
	//�߂�l�F
	// �C���f�b�N�X�l�i-1�Ŗ�Open�j
	int GetOpenBonDriverIndex();

	//Open����BonDriver�̃t�@�C�������擾
	//�߂�l�F
	// BonDriver�̃t�@�C�����i�g���q�܂ށj
	wstring GetOpenBonDriverFileName();

	//�w�蕨���`�����l���̃`�����l�������擾
	//�߂�l�F
	// BonDriver�Œ�`����Ă��镨���`�����l����
	wstring GetChName(DWORD space, DWORD ch);

protected:
	HANDLE lockEvent;

	wstring settingFolderPath;
	wstring bonDriverFolderPath;

	map<int, BON_DRIVER_INFO> bonDllMap;

	int loadIndex;
	wstring loadTunerName;
	map<DWORD, BON_SPACE_INFO> loadChMap;
	BOOL initChSetFlag;
	IBonDriver* bonIF;
	IBonDriver2* bon2IF;
	HMODULE module;

	DWORD setSpace;
	DWORD setCh;
protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 5*1000);
	void UnLock(LPCWSTR log = NULL);

	//�t�@�C�����̍Ōオ�{���Ɂu.dll�v���m�F
	//�߂�l�F
	// TRUE�i�u.dll�v���Ō�j�AFALSE�i�u.dll�v���Ō�ł͂Ȃ��j
	//�����F
	// name						[IN]�m�F���镶����
	BOOL IsDllFile(wstring name);

	//BonDriver�����[�h���̖{��
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// bonDriverFilePath		[IN] ���[�h����BonDriver�̃t�@�C���p�X
	DWORD _OpenBonDriver(
		LPCWSTR bonDriverFilePath,
		int openWait
		);

	//���[�h���Ă���BonDriver�̊J���̖{��
	//�߂�l�F
	// �G���[�R�[�h
	DWORD _CloseBonDriver();
};

