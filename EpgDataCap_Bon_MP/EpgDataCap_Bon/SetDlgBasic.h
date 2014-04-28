#pragma once
#include "afxwin.h"


// CSetDlgBasic �_�C�A���O

class CSetDlgBasic : public CDialog
{
	DECLARE_DYNAMIC(CSetDlgBasic)

public:
	CSetDlgBasic(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSetDlgBasic();

	void SetIniPath(CString commonIniPath, CString appIniPath){
		this->commonIniPath = commonIniPath;
		this->appIniPath = appIniPath;
	};
	void SaveIni(void);


// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SET_BASIC };

protected:
	CString commonIniPath;
	CString appIniPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRecPath();
	afx_msg void OnBnClickedButtonRecAdd();
	afx_msg void OnBnClickedButtonRecDel();
	afx_msg void OnBnClickedButtonRecUp();
	afx_msg void OnBnClickedButtonRecDown();
	afx_msg void OnBnClickedButtonSetPath();
	CString settingFolderPath;
	CString recFolderPath;
	CListBox recFolderList;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
