
// MediaPortal_BonDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"

#include "MediaPortal_BonDef.h"
#include "MediaPortal_BonMain.h"
#include "SettingDlg.h"
#include "my_global.h"
#include "mysql.h"
#include "../../MpCtrl/DbCtrl.h"
#include "../../MpCtrl/DbCtrlDef.h"
#include <map>
#include <regex>
using namespace std;

// CMediaPortal_BonDlg ダイアログ
class CMediaPortal_BonDlg : public CDialogEx
{
// コンストラクション
public:
	CMediaPortal_BonDlg(CWnd* pParent = NULL);	// 標準コンストラクター


	void UnInit();
	void SetInitBon(CString bonFile);
	void SetIniMin(BOOL minFlag){ iniMin = minFlag; };

// ダイアログ データ
	enum { IDD = IDD_MediaPortal_BON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


protected:
	static UINT taskbarCreated;
protected:
	void BtnUpdate(DWORD guiMode);
	//タスクトレイ
	BOOL DeleteTaskBar(HWND wnd, UINT id);
	BOOL AddTaskBar(HWND wnd, UINT msg, UINT id, HICON icon, CString tips);
	BOOL ChgTipsTaskBar(HWND wnd, UINT id, HICON icon, CString tips);
	void ChgIconStatus();

	void ReloadBonDriver();
	DWORD SelectBonDriver(LPCWSTR fileName, BOOL ini = FALSE);
	DWORD SelectService(WORD ONID, WORD TSID, WORD SID);
	DWORD SelectService(WORD ONID, WORD TSID, WORD SID,	DWORD space, DWORD ch);
// 実装
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

	CMediaPortal_BonMain main;

	map<int, wstring> bonList;
	vector<CH_DATA4> serviceList;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	MYSQL mysql;
	MYSQL_RES *results;
	MYSQL_ROW record;

	CDbCtrl dbCtrl;

	DWORD mpServiceStat;
	CString mpLogPath;
	CFileFind mpLogFind;

	CString mpLogStr;
	fpos_t mpNowLogSz;
	fpos_t mpPreLogSz;
	CString mpStartTimeShifting;

public:
	CComboBox combTuner;
	CButton btnChScan;
	CButton btnSet;
	CButton btnCancel;
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
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonChscan();
	afx_msg void OnBnClickedButtonCancel();
	CEdit editStatus;
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnEndSession(BOOL bEnding);
};
