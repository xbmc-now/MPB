#pragma once
#include "afxwin.h"


// CSetDlgApp �_�C�A���O

class CSetDlgApp : public CDialog
{
	DECLARE_DYNAMIC(CSetDlgApp)

public:
	CSetDlgApp(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSetDlgApp();
	
	void SetIniPath(CString commonIniPath, CString appIniPath){
		this->commonIniPath = commonIniPath;
		this->appIniPath = appIniPath;
	};
	void SaveIni(void);

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SET_APP };

protected:
	CString commonIniPath;
	CString appIniPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton btnEnableScramble;
	CButton btnNeedCaption;
	CButton btnNeedData;
	int startMargine;
	int endMargine;
	CButton btnOverWrite;
	CButton btnAllService;
	CButton btnEpgCapLive;
	CButton btnEpgCapRec;
	CButton btnTaskMin;
	CButton btnEnableEMM;
	CButton btnOpenLast;
	UINT epgCapBackStartWaitSec;
};
