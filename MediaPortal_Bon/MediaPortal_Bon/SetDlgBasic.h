#pragma once
#include "afxwin.h"

#include "../../Common/ParseChText4Mp.h"

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
	CParseChText4 chSet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetPath();
	afx_msg void OnBnClickedButtonSetEpgPath();
	CString settingFolderPath;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	CIPAddressCtrl udpIP;
	UINT udpPort;
	UINT udpWaitSec;
	UINT udpWaitPacket;
	CButton btnEnableScramble;
	CButton btnAllService;
	CButton btnTaskMin;
	CButton btnEnableEMM;
	CButton btnEpgTimer;
	CString epgFolderPath;
	CComboBox cmbHour;
	CComboBox cmbMin;
};
