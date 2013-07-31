#pragma once

#include "../../BonCtrl/BonCtrl.h"
#include "../../Common/PathUtil.h"
#include "../../Common/TimeUtil.h"
#include "../../Common/PipeServer.h"

class CEpgDataCap_BonMain
{
public:
	CEpgDataCap_BonMain(void);
	~CEpgDataCap_BonMain(void);

	//�ʒm�pHWND��ݒ�
	void SetHwnd(HWND wnd);

	//�ݒ���s��
	void ReloadSetting();
	
	//BonDriver�t�H���_��BonDriver_*.dll���
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// bonList			[OUT]�����ł���BonDriver�ꗗ�imap�̃L�[ �����C���f�b�N�X�l�Amap�̒l BonDriver�t�@�C�����j
	DWORD EnumBonDriver(
		map<int, wstring>* bonList
		);

	//BonDriver�����[�h���ă`�����l�����Ȃǂ��擾�i�t�@�C�����Ŏw��j
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// bonDriverFile	[IN]EnumBonDriver�Ŏ擾���ꂽBonDriver�̃t�@�C����
	DWORD OpenBonDriver(
		LPCWSTR bonDriverFile
		);

	//���[�h���Ă���BonDriver�̊J��
	//�߂�l�F
	// �G���[�R�[�h
	DWORD CloseBonDriver();

	//�T�[�r�X�ꗗ���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// serviceList				[OUT]�T�[�r�X���̃��X�g
	DWORD GetServiceList(
		vector<CH_DATA4>* serviceList
		);

	//���[�h����BonDriver�̃t�@�C�������擾����i���[�h�������Ă��邩�̔���j
	//�߂�l�F
	// TRUE�i�����j�FFALSE�iOpen�Ɏ��s���Ă���j
	//�����F
	// bonDriverFile		[OUT]BonDriver�̃t�@�C����(NULL��)
	BOOL GetOpenBonDriver(
		wstring* bonDriverFile
		);

	//�`�����l���ύX
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// ONID			[IN]�ύX�`�����l����orignal_network_id
	// TSID			[IN]�ύX�`�����l����transport_stream_id
	// SID			[IN]�ύX�`�����l����service_id
	DWORD SetCh(
		WORD ONID,
		WORD TSID,
		WORD SID
		);

	//�`�����l���ύX
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// ONID			[IN]�ύX�`�����l����orignal_network_id
	// TSID			[IN]�ύX�`�����l����transport_stream_id
	// SID			[IN]�ύX�`�����l����service_id
	// space		[IN]�ύX�`�����l����space
	// ch			[IN]�ύX�`�����l����ch
	DWORD SetCh(
		WORD ONID,
		WORD TSID,
		WORD SID,
		DWORD space,
		DWORD ch
		);

	//���݂̃T�[�r�X�擾
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// ONID			[IN]���݂�orignal_network_id
	// TSID			[IN]���݂�transport_stream_id
	// SID			[IN]���݂�service_id
	void GetCh(
		WORD* ONID,
		WORD* TSID,
		WORD* SID
		);

	BOOL GetCh(
		DWORD* space,
		DWORD* ch
		);

	//�`�����l���ύX�����ǂ���
	//�߂�l�F
	// TRUE�i�ύX���j�AFALSE�i�����j
	BOOL IsChChanging(BOOL* chChgErr);

	//�O������Ȃǂ�CH�ύX���ꂽ�ꍇ��SID�̂ݐݒ�
	void SetSID(
		WORD SID
		);

	//UDP�ő��M���s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// enableFlag		[IN]TRUE�i�J�n�j�AFALSE�i��~�j
	BOOL SendUDP(
		BOOL enableFlag
		);

	//TCP�ő��M���s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// enableFlag		[IN]TRUE�i�J�n�j�AFALSE�i��~�j
	BOOL SendTCP(
		BOOL enableFlag
		);

	//UDP���M�̐ݒ萔���擾
	DWORD GetCountUDP();
	//TCP���M�̐ݒ萔���擾
	DWORD GetCountTCP();
	//UDP�̑��M����ꗗ�擾
	BOOL GetSendUDPList(
		vector<NW_SEND_INFO>* sendList
		);
	//TCP�̑��M����ꗗ�擾
	BOOL GetSendTCPList(
		vector<NW_SEND_INFO>* sendList
		);

	//�w��T�[�r�X�̌���or����EPG�����擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
	// epgInfo					[OUT]EPG���
	DWORD GetEpgInfo(
		BOOL nextFlag,
		CString* epgInfo
		);

	//�V�O�i�����x���̎擾
	//�߂�l�F
	// �V�O�i�����x��
	float GetSignalLevel();

	//�G���[�J�E���g���N���A����
	void ClearErrCount(
		);

	//�h���b�v�ƃX�N�����u���̃J�E���g���擾����
	//�����F
	// drop				[OUT]�h���b�v��
	// scramble			[OUT]�X�N�����u����
	void GetErrCount(
		ULONGLONG* drop,
		ULONGLONG* scramble
		);

	//�^����J�n����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL StartRec(
		);

	//�^����~����
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL StopRec();

	//�^�撆���ǂ������擾����
	// TRUE�i�^�撆�j�AFALSE�i�^�悵�Ă��Ȃ��j
	BOOL IsRec();

	//�\��^����~����
	void StopReserveRec();

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

	//EPG�擾���J�n����
	//�߂�l�F
	// �G���[�R�[�h
	DWORD StartEpgCap(
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
	DWORD GetEpgCapStatus(EPGCAP_SERVICE_INFO* info);

	//View�A�v���̋N�����s��
	void ViewAppOpen();

	void StartServer();
	void StopServer();

	void StartTimeShift();

	BOOL GetViewStatusInfo(
		float* signal,
		DWORD* space,
		DWORD* ch,
		ULONGLONG* drop,
		ULONGLONG* scramble,
		vector<NW_SEND_INFO>* sendUdpList,
		vector<NW_SEND_INFO>* sendTcpList
		);

protected:
	HWND msgWnd;

	CBonCtrl bonCtrl;
	CPipeServer pipeServer;

	vector<wstring> recFolderList;

	DWORD nwCtrlID;
	BOOL sendUdpFlag;
	BOOL sendTcpFlag;
	DWORD udpCount;
	DWORD tcpCount;
	vector<NW_SEND_INFO> udpSendList;
	vector<NW_SEND_INFO> tcpSendList;

	int startMargine;
	int endMargine;
	BOOL overWriteFlag;

	BOOL enableScrambleFlag;
	BOOL enableEMMFlag;

	BOOL allService;
	BOOL needCaption;
	BOOL needData;

	BOOL BSBasic;
	BOOL CS1Basic;
	BOOL CS2Basic;

	int openWait;

	wstring viewPath;
	wstring viewOpt;

	WORD lastONID;
	WORD lastTSID;
	WORD lastSID;

	DWORD recCtrlID;

	wstring currentBonDriver;
	int outCtrlID;
	map<DWORD, DWORD> ctrlMap;

protected:
	//�O������R�}���h�֌W
	static int CALLBACK CtrlCmdCallback(void* param, CMD_STREAM* cmdParam, CMD_STREAM* resParam);
};

