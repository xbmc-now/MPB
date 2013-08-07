#pragma once

#include <windows.h>

#include "../Common/Util.h"
#include "../Common/StructDef.h"
#include "../Common/EpgTimerUtil.h"
#include "../Common/StringUtil.h"

#include "BonDriverUtil.h"
#include "PacketInit.h"
#include "TSOut.h"
#include "ChSetUtil.h"

class CBonCtrl
{
public:
	CBonCtrl(void);
	~CBonCtrl(void);

	//�����ݒ�
	//�ݒ�t�@�C���ۑ����BonDriver�t�H���_���w��
	//�����F
	// settingFolderPath		[IN]�ݒ�t�@�C���ۑ��t�H���_�p�X
	// bonDriverFolderPath		[IN]BonDriver�t�H���_�p�X
	void SetSettingFolder(
		LPCWSTR settingFolderPath,
		LPCWSTR bonDriverFolderPath
		);

	void SetEMMMode(BOOL enable);

	void SetTsBuffMaxCount(DWORD tsBuffMaxCount, int writeBuffMaxCount);

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

	//���[�h����BonDriver�̃t�@�C�������擾����i���[�h�������Ă��邩�̔���j
	//�߂�l�F
	// TRUE�i�����j�FFALSE�iOpen�Ɏ��s���Ă���j
	//�����F
	// bonDriverFile		[OUT]BonDriver�̃t�@�C����(NULL��)
	BOOL GetOpenBonDriver(
		wstring* bonDriverFile
		);

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

	//�`�����l���ύX
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// space			[IN]�ύX�`�����l����Space
	// ch				[IN]�ύX�`�����l���̕���Ch
	// SID			[IN]�ύX�`�����l���̕���service_id
	DWORD SetCh(
		DWORD space,
		DWORD ch,
		WORD SID
		);

	//�`�����l���ύX
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// ONID			[IN]�ύX�`�����l����orignal_network_id
	// TSID			[IN]�ύX�`�����l���̕���transport_stream_id
	// SID			[IN]�ύX�`�����l���̕���service_id
	DWORD SetCh(
		WORD ONID,
		WORD TSID,
		WORD SID
		);

	BOOL GetCh(
		DWORD* space,
		DWORD* ch
		);

	//�`�����l���ύX�����ǂ���
	//�߂�l�F
	// TRUE�i�ύX���j�AFALSE�i�����j
	BOOL IsChChanging(BOOL* chChgErr);

	//���݂̃X�g���[����ID���擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// ONID		[OUT]originalNetworkID
	// TSID		[OUT]transportStreamID
	BOOL GetStreamID(
		WORD* ONID,
		WORD* TSID
		);

	//�V�O�i�����x���̎擾
	//�߂�l�F
	// �V�O�i�����x��
	float GetSignalLevel();

	//EPG�f�[�^�̒~�Ϗ�Ԃ����Z�b�g����
	void ClearSectionStatus();

	//EPG�f�[�^�̒~�Ϗ�Ԃ��擾����
	//�߂�l�F
	// �X�e�[�^�X
	//�����F
	// l_eitFlag		[IN]L-EIT�̃X�e�[�^�X���擾
	EPG_SECTION_STATUS GetSectionStatus(
		BOOL l_eitFlag
		);

	//���X�g���[���̃T�[�r�X�ꗗ���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// serviceList				[OUT]�T�[�r�X���̃��X�g
	DWORD GetServiceListActual(
		vector<TS_SERVICE_INFO>* serviceList
		);

	//�T�[�r�X�ꗗ���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// serviceList				[OUT]�T�[�r�X���̃��X�g
	DWORD GetServiceList(
		vector<CH_DATA4>* serviceList
		);

	//TS�X�g���[������p�R���g���[�����쐬����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// id			[OUT]���䎯��ID
	BOOL CreateServiceCtrl(
		DWORD* id
		);

	//TS�X�g���[������p�R���g���[�����쐬����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// id			[IN]���䎯��ID
	BOOL DeleteServiceCtrl(
		DWORD id
		);

	//����Ώۂ̃T�[�r�X��ݒ肷��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s
	//�����F
	// id			[IN]���䎯��ID
	// serviceID	[IN]�ΏۃT�[�r�XID�A0xFFFF�őS�T�[�r�X�Ώ�
	BOOL SetServiceID(
		DWORD id,
		WORD serviceID
		);

	BOOL GetServiceID(
		DWORD id,
		WORD* serviceID
		);

	//UDP�ő��M���s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// id			[IN]���䎯��ID
	// sendList		[IN/OUT]���M�惊�X�g�BNULL�Œ�~�BPort�͎��ۂɑ��M�Ɏg�p����Port���Ԃ�B
	BOOL SendUdp(
		DWORD id,
		vector<NW_SEND_INFO>* sendList
		);

	//TCP�ő��M���s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// id			[IN]���䎯��ID
	// sendList		[IN/OUT]���M�惊�X�g�BNULL�Œ�~�BPort�͎��ۂɑ��M�Ɏg�p����Port���Ԃ�B
	BOOL SendTcp(
		DWORD id,
		vector<NW_SEND_INFO>* sendList
		);

	//�t�@�C���ۑ����J�n����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// id					[IN]���䎯��ID
	// fileName				[IN]�ۑ��t�@�C���p�X
	// overWriteFlag		[IN]����t�@�C�������ݎ��ɏ㏑�����邩�ǂ����iTRUE�F����AFALSE�F���Ȃ��j
	// pittariFlag			[IN]�҂����胂�[�h�iTRUE�F����AFALSE�F���Ȃ��j
	// pittariONID			[IN]�҂����胂�[�h�Ř^�悷��ONID
	// pittariTSID			[IN]�҂����胂�[�h�Ř^�悷��TSID
	// pittariSID			[IN]�҂����胂�[�h�Ř^�悷��SID
	// pittariEventID		[IN]�҂����胂�[�h�Ř^�悷��C�x���gID
	// createSize			[IN]�t�@�C���쐬���Ƀf�B�X�N�ɗ\�񂷂�e��
	// saveFolder			[IN]�g�p����t�H���_�ꗗ
	// saveFolderSub		[IN]HDD�̋󂫂��Ȃ��Ȃ����ꍇ�Ɉꎞ�I�Ɏg�p����t�H���_
	BOOL StartSave(
		DWORD id,
		wstring fileName,
		BOOL overWriteFlag,
		BOOL pittariFlag,
		WORD pittariONID,
		WORD pittariTSID,
		WORD pittariSID,
		WORD pittariEventID,
		ULONGLONG createSize,
		vector<REC_FILE_SET_INFO>* saveFolder,
		vector<wstring>* saveFolderSub
	);

	//�t�@�C���ۑ����I������
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// id			[IN]���䎯��ID
	BOOL EndSave(
		DWORD id
		);

	//�X�N�����u�����������̓���ݒ�
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// id			[IN]���䎯��ID
	// enable		[IN] TRUE�i��������j�AFALSE�i�������Ȃ��j
	BOOL SetScramble(
		DWORD id,
		BOOL enable
		);

	//�����ƃf�[�^�����܂߂邩�ǂ���
	//�����F
	// id					[IN]���䎯��ID
	// enableCaption		[IN]������ TRUE�i�܂߂�j�AFALSE�i�܂߂Ȃ��j
	// enableData			[IN]�f�[�^������ TRUE�i�܂߂�j�AFALSE�i�܂߂Ȃ��j
	void SetServiceMode(
		DWORD id,
		BOOL enableCaption,
		BOOL enableData
		);

	//�G���[�J�E���g���N���A����
	//�����F
	// id					[IN]���䎯��ID
	void ClearErrCount(
		DWORD id
		);

	//�h���b�v�ƃX�N�����u���̃J�E���g���擾����
	//�����F
	// id					[IN]���䎯��ID
	// drop					[OUT]�h���b�v��
	// scramble				[OUT]�X�N�����u����
	void GetErrCount(
		DWORD id,
		ULONGLONG* drop,
		ULONGLONG* scramble
		);

	//�^�撆�̃t�@�C���̃t�@�C���p�X���擾����
	//�����F
	// id					[IN]���䎯��ID
	// filePath				[OUT]�ۑ��t�@�C����
	// subRecFlag			[OUT]�T�u�^�悪�����������ǂ���
	void GetSaveFilePath(
		DWORD id,
		wstring* filePath,
		BOOL* subRecFlag
		);

	//�h���b�v�ƃX�N�����u���̃J�E���g��ۑ�����
	//�����F
	// id					[IN]���䎯��ID
	// filePath				[IN]�ۑ��t�@�C����
	void SaveErrCount(
		DWORD id,
		wstring filePath
		);

	//�^�撆�̃t�@�C���̏o�̓T�C�Y���擾����
	//�����F
	// id					[IN]���䎯��ID
	// writeSize			[OUT]�ۑ��t�@�C����
	void GetRecWriteSize(
		DWORD id,
		__int64* writeSize
		);

	//�w��T�[�r�X�̌���or����EPG�����擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
	// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
	// serviceID				[IN]�擾�Ώۂ�ServiceID
	// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
	// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
	DWORD GetEpgInfo(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID,
		BOOL nextFlag,
		EPGDB_EVENT_INFO* epgInfo
		);

	//�w��C�x���g��EPG�����擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
	// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
	// serviceID				[IN]�擾�Ώۂ�ServiceID
	// eventID					[IN]�擾�Ώۂ�EventID
	// pfOnlyFlag				[IN]p/f����̂݌������邩�ǂ���
	// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
	DWORD SearchEpgInfo(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID,
		WORD eventID,
		BYTE pfOnlyFlag,
		EPGDB_EVENT_INFO* epgInfo
		);
	
	//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
	//�߂�l�F
	// ���̕b��
	int GetTimeDelay(
		);

	//�^�撆���ǂ������擾����
	// TRUE�i�^�撆�j�AFALSE�i�^�悵�Ă��Ȃ��j
	BOOL IsRec();

	//�`�����l���X�L�������J�n����
	//�߂�l�F
	// �G���[�R�[�h
	DWORD StartChScan();

	//�`�����l���X�L�������L�����Z������
	//�߂�l�F
	// �G���[�R�[�h
	DWORD StopChScan();

	//�`�����l���X�L�����̏�Ԃ��擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// space		[OUT]�X�L�������̕���CH��space
	// ch			[OUT]�X�L�������̕���CH��ch
	// chName		[OUT]�X�L�������̕���CH�̖��O
	// chkNum		[OUT]�`�F�b�N�ς݂̐�
	// totalNum		[OUT]�`�F�b�N�Ώۂ̑���
	DWORD GetChScanStatus(
		DWORD* space,
		DWORD* ch,
		wstring* chName,
		DWORD* chkNum,
		DWORD* totalNum
		);

	//EPG�擾�Ώۂ̃T�[�r�X�ꗗ���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// chList		[OUT]EPG�擾����`�����l���ꗗ
	DWORD GetEpgCapService(
		vector<EPGCAP_SERVICE_INFO>* chList
		);


	//EPG�擾���J�n����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// chList		[IN]EPG�擾����`�����l���ꗗ
	// BSBasic		[IN]BS�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
	// CS1Basic		[IN]CS1�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
	// CS2Basic		[IN]CS2�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
	DWORD StartEpgCap(
		vector<EPGCAP_SERVICE_INFO>* chList,
		BOOL BSBasic,
		BOOL CS1Basic,
		BOOL CS2Basic
		);

	//EPG�擾���~����
	//�߂�l�F
	// �G���[�R�[�h
	DWORD StopEpgCap(
		);

	//EPG�擾�̃X�e�[�^�X���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// info			[OUT]�擾���̃T�[�r�X
	DWORD GetEpgCapStatus(
		EPGCAP_SERVICE_INFO* info
		);

	//�o�b�N�O���E���h�ł�EPG�擾�ݒ�
	//�����F
	// enableLive	[IN]�������Ɏ擾����
	// enableRec	[IN]�^�撆�Ɏ擾����
	// enableRec	[IN]EPG�擾����`�����l���ꗗ
	// BSBasic		[IN]BS�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
	// CS1Basic		[IN]CS1�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
	// CS2Basic		[IN]CS2�łP�`�����l�������{���̂ݎ擾���邩�ǂ���
	// backStartWaitSec	[IN]Ch�؂�ւ��A�^��J�n��A�o�b�N�O���E���h�ł�EPG�擾���J�n����܂ł̕b��
	void SetBackGroundEpgCap(
		BOOL enableLive,
		BOOL enableRec,
		BOOL BSBasic,
		BOOL CS1Basic,
		BOOL CS2Basic,
		DWORD backStartWaitSec
		);

	BOOL GetViewStatusInfo(
		DWORD id,
		float* signal,
		DWORD* space,
		DWORD* ch,
		ULONGLONG* drop,
		ULONGLONG* scramble
		);

protected:
	CBonDriverUtil bonUtil;
	CPacketInit packetInit;
	CTSOut tsOut;
	CChSetUtil chUtil;

	HANDLE lockEvent;

	HANDLE buffLockEvent;
	vector<TS_DATA*> TSBuff;

	HANDLE recvThread;
	HANDLE recvStopEvent;
	HANDLE analyzeThread;
	HANDLE analyzeStopEvent;

	//�`�����l���X�L�����p
	HANDLE chScanThread;
	HANDLE chScanStopEvent;
	DWORD chSt_space;
	DWORD chSt_ch;
	wstring chSt_chName;
	DWORD chSt_chkNum;
	DWORD chSt_totalNum;
	DWORD chSt_err;
	typedef struct _CHK_CH_INFO{
		DWORD space;
		DWORD ch;
		wstring spaceName;
		wstring chName;
	}CHK_CH_INFO;

	//EPG�擾�p
	HANDLE epgCapThread;
	HANDLE epgCapStopEvent;
	vector<EPGCAP_SERVICE_INFO> epgCapChList;
	BOOL BSBasic;
	BOOL CS1Basic;
	BOOL CS2Basic;
	EPGCAP_SERVICE_INFO epgSt_ch;
	DWORD epgSt_err;

	HANDLE epgCapBackThread;
	HANDLE epgCapBackStopEvent;
	BOOL enableLiveEpgCap;
	BOOL enableRecEpgCap;
	WORD lastSID;

	DWORD epgCapBackStartWaitSec;
	DWORD tsBuffMaxCount;
	int writeBuffMaxCount;
protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 15*1000);
	void UnLock(LPCWSTR log = NULL);

	//BonDriver�����[�h��̏���������
	//�߂�l�F
	// �G���[�R�[�h
	DWORD _OpenBonDriver();

	//���[�h���Ă���BonDriver�̊J���{��
	//�߂�l�F
	// �G���[�R�[�h
	DWORD _CloseBonDriver();

	DWORD _SetCh(
		DWORD space,
		DWORD ch,
		BOOL chScan = FALSE
		);

	void GetEpgDataFilePath(WORD ONID, WORD TSID, wstring& epgDataFilePath);

	static UINT WINAPI RecvThread(LPVOID param);
	static UINT WINAPI AnalyzeThread(LPVOID param);

	static UINT WINAPI ChScanThread(LPVOID param);
	static UINT WINAPI EpgCapThread(LPVOID param);

	void StartBackgroundEpgCap();
	void StopBackgroundEpgCap();
	static UINT WINAPI EpgCapBackThread(LPVOID param);
};

