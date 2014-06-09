#pragma once


#include "../../Common/PathUtil.h"

#include "SetDlgBasic.h"
#include "SetDlgService.h"

#include "afxcmn.h"

//#include "MediaPortal_BonDef.h"

// CSettingDlg �_�C�A���O

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSettingDlg();


// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	CString commonIniPath;
	CString appIniPath;

	CSetDlgBasic basicDlg;
	CSetDlgService serviceDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl dlgTab;
};
