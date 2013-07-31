#pragma once


// CSetDlgAppBtn �_�C�A���O

class CSetDlgAppBtn : public CDialog
{
	DECLARE_DYNAMIC(CSetDlgAppBtn)

public:
	CSetDlgAppBtn(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSetDlgAppBtn();
	
	void SetIniPath(CString commonIniPath, CString appIniPath){
		this->commonIniPath = commonIniPath;
		this->appIniPath = appIniPath;
	};
	void SaveIni(void);

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SET_APPBTN };

protected:
	CString commonIniPath;
	CString appIniPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString viewExe;
	CString viewOpt;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonViewExe();
};
