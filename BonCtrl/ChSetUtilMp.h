#pragma once

#include "../Common/Util.h"
#include "../Common/StructDef.h"
#include "../Common/EpgDataCap3Def.h"
#include "BonCtrlDef.h"

#include "../Common/ParseChText4.h"
#include "../Common/ParseChText5.h"

class CChSetUtil
{
public:
	CChSetUtil(void);
	~CChSetUtil(void);

	//�`�����l���ݒ�t�@�C����ǂݍ���
	BOOL LoadChSet(
		wstring chSet4FilePath,
		wstring chSet5FilePath
		);

	//�`�����l���ݒ�t�@�C����ۑ�����
	BOOL SaveChSet(
		wstring chSet4FilePath,
		wstring chSet5FilePath
		);

	//�`�����l���X�L�����p�ɃN���A����
	BOOL Clear();

	//�`�����l������ǉ�����
	BOOL AddServiceInfo(
		DWORD space,
		DWORD ch,
		wstring chName,
		SERVICE_INFO* serviceInfo
		);

	//�T�[�r�X�ꗗ���擾����
	BOOL GetEnumService(
		vector<CH_DATA4>* serviceList
		);

	//ID���畨���`�����l������������
	BOOL GetCh(
		WORD ONID,
		WORD TSID,
		DWORD& space,
		DWORD& ch
		);

	//EPG�擾�Ώۂ̃T�[�r�X�ꗗ���擾����
	//�����F
	// chList		[OUT]EPG�擾����`�����l���ꗗ
	void GetEpgCapService(
		vector<EPGCAP_SERVICE_INFO>* chList
		);

	//EPG�擾�Ώۂ̕����`�����l�����ǂ���
	BOOL IsEpgCapService(
		WORD ONID,
		WORD TSID
		);

	//������M�T�[�r�X���ǂ���
	BOOL IsPartial(
		WORD ONID,
		WORD TSID,
		WORD SID
		);


protected:
	CParseChText4 chText4;
	CParseChText5 chText5;
};

