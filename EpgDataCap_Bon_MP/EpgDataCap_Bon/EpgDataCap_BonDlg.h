
// EpgDataCap_BonDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"

#include "EpgDataCap_BonDef.h"
#include "EpgDataCap_BonMain.h"
#include "SettingDlg.h"

// CEpgDataCap_BonDlg �_�C�A���O
class CEpgDataCap_BonDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CEpgDataCap_BonDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[


	void UnInit();
	void SetInitBon(CString bonFile);
	void SetIniMin(BOOL minFlag){ iniMin = minFlag; };
	void SetIniNW(BOOL networkFlag){ iniNetwork = networkFlag; };
	void SetIniView(BOOL viewFlag){ iniView = viewFlag; };
	void SetIniNWUDP(BOOL udpFlag){ iniUDP = udpFlag; };
	void SetIniNWTCP(BOOL tcpFlag){ iniTCP = tcpFlag; };

// �_�C�A���O �f�[�^
	enum { IDD = IDD_EPGDATACAP_BON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


protected:
	static UINT taskbarCreated;
protected:
	void BtnUpdate(DWORD guiMode);
	//�^�X�N�g���C
	BOOL DeleteTaskBar(HWND wnd, UINT id);
	BOOL AddTaskBar(HWND wnd, UINT msg, UINT id, HICON icon, CString tips);
	BOOL ChgTipsTaskBar(HWND wnd, UINT id, HICON icon, CString tips);
	void ChgIconStatus();

	void ReloadBonDriver();
	void ReloadServiceList(BOOL ini = FALSE);
	void ReloadNWSet();
	DWORD SelectBonDriver(LPCWSTR fileName, BOOL ini = FALSE);
	DWORD SelectService(WORD ONID, WORD TSID, WORD SID);
	DWORD SelectService(WORD ONID, WORD TSID, WORD SID,	DWORD space, DWORD ch);
// ����
protected:
	HICON m_hIcon;
	HICON m_hIcon2;

	HICON iconRed;
	HICON iconBlue;
	HICON iconGreen;
	HICON iconGray;
	BOOL minTask;

	CString moduleIniPath;
	CString commonIniPath;
	CString timerSrvIniPath;

	CString iniBonDriver;
	int initONID;
	int initTSID;
	int initSID;
	int initOpenWait;
	int initChgWait;
	BOOL iniMin;
	BOOL iniView;
	BOOL iniNetwork;
	BOOL iniUDP;
	BOOL iniTCP;
	int openLastCh;

	CEpgDataCap_BonMain main;

	map<int, wstring> bonList;
	vector<CH_DATA4> serviceList;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox combTuner;
	CComboBox combService;
	CButton btnChScan;
	CButton btnEpg;
	CButton btnSet;
	CButton btnRec;
	CComboBox combRecH;
	CComboBox combRecM;
	CButton chkRecSet;
	CButton btnCancel;
	CButton btnView;
	CButton chkUDP;
	CButton chkTCP;
	CString log;
	CString statusLog;
	CString pgInfo;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTaskbarCreated(WPARAM, LPARAM);
	afx_msg void OnCbnSelchangeComboTuner();
	afx_msg void OnCbnSelchangeComboService();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonChscan();
	afx_msg void OnBnClickedButtonEpg();
	afx_msg void OnBnClickedButtonRec();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonView();
	afx_msg void OnBnClickedCheckUdp();
	afx_msg void OnBnClickedCheckTcp();
	afx_msg void OnBnClickedCheckRecSet();
	CEdit editStatus;
	CButton btnPgNext;
	afx_msg void OnBnClickedCheckNextpg();
	CButton btnTimeShift;
	afx_msg void OnBnClickedButtonTimeshift();
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnEndSession(BOOL bEnding);
};
