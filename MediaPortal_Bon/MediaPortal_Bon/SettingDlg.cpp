// SettingDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MediaPortal_Bon.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, dlgTab);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, &CSettingDlg::OnTcnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CSettingDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CSettingDlg ���b�Z�[�W �n���h���[


BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	wstring path = L"";
	GetCommonIniPath(path);
	commonIniPath = path.c_str();
	GetModuleIniPath(path);
	appIniPath = path.c_str();

	basicDlg.SetIniPath(commonIniPath, appIniPath);
	serviceDlg.SetIniPath(commonIniPath, appIniPath);

	TCITEM Item;
	Item.mask = TCIF_TEXT;

	Item.pszText = L"��{�ݒ�";
	dlgTab.InsertItem(0, &Item);

	Item.pszText = L"�T�[�r�X�\���ݒ�";
	dlgTab.InsertItem(1, &Item);


	basicDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_BASIC), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );
	serviceDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_SERVICE), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );

	CRect rc;
	dlgTab.GetClientRect(rc);
	dlgTab.AdjustRect(FALSE, rc);

	basicDlg.MoveWindow(rc);
	serviceDlg.MoveWindow(rc);

	dlgTab.SetCurSel(0);
	basicDlg.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CSettingDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	basicDlg.SaveIni();
	serviceDlg.SaveIni();

	//CMediaPortal_BonDlg* parent = STATIC_DOWNCAST(CMediaPortal_BonDlg, GetParent());// GetParent(�q�_�C�A���O��hwnd)
	//ASSERT(parent);
	//parent->SendMessage(WM_APP_SENDDATA, 0, 0);
	GetParent()->SendMessage(WM_APP_SENDDATA, 0, 0);
	CDialog::OnOK();
}


void CSettingDlg::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int index = dlgTab.GetCurSel();
	switch(index){
		case 0:
			basicDlg.ShowWindow(SW_HIDE);
			break;
		case 1:
			serviceDlg.ShowWindow(SW_HIDE);
			break;
		default:
			break;
	}
	*pResult = 0;
}


void CSettingDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int index = dlgTab.GetCurSel();
	switch(index){
		case 0:
			basicDlg.ShowWindow(SW_SHOW);
			break;
		case 1:
			serviceDlg.ShowWindow(SW_SHOW);
			break;
		default:
			break;
	}

	*pResult = 0;
}
