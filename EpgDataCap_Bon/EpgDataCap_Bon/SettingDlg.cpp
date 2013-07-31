// SettingDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "EpgDataCap_Bon.h"
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
	appDlg.SetIniPath(commonIniPath, appIniPath);
	epgDlg.SetIniPath(commonIniPath, appIniPath);
	networkDlg.SetIniPath(commonIniPath, appIniPath);
	appBtnDlg.SetIniPath(commonIniPath, appIniPath);
	serviceDlg.SetIniPath(commonIniPath, appIniPath);

	TCITEM Item;
	Item.mask = TCIF_TEXT;

	Item.pszText = L"��{�ݒ�";
	dlgTab.InsertItem(0, &Item);

	Item.pszText = L"����ݒ�";
	dlgTab.InsertItem(1, &Item);

	Item.pszText = L"EPG�擾�ݒ�";
	dlgTab.InsertItem(2, &Item);

	Item.pszText = L"�T�[�r�X�\���ݒ�";
	dlgTab.InsertItem(3, &Item);

	Item.pszText = L"�l�b�g���[�N�ݒ�";
	dlgTab.InsertItem(4, &Item);

	Item.pszText = L"�O���A�v���P�[�V�����ݒ�";
	dlgTab.InsertItem(5, &Item);

	basicDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_BASIC), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );
	appDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_APP), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );
	epgDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_EPG), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );
	networkDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_NW), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );
	appBtnDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_APPBTN), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );
	serviceDlg.Create( MAKEINTRESOURCE(IDD_DIALOG_SET_SERVICE), CWnd::FromHandle( dlgTab.GetSafeHwnd() ) );

	CRect rc;
	dlgTab.GetClientRect(rc);
	dlgTab.AdjustRect(FALSE, rc);

	basicDlg.MoveWindow(rc);
	appDlg.MoveWindow(rc);
	epgDlg.MoveWindow(rc);
	networkDlg.MoveWindow(rc);
	appBtnDlg.MoveWindow(rc);
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
	appDlg.SaveIni();
	epgDlg.SaveIni();
	networkDlg.SaveIni();
	appBtnDlg.SaveIni();
	serviceDlg.SaveIni();

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
			appDlg.ShowWindow(SW_HIDE);
			break;
		case 2:
			epgDlg.ShowWindow(SW_HIDE);
			break;
		case 3:
			serviceDlg.ShowWindow(SW_HIDE);
			break;
		case 4:
			networkDlg.ShowWindow(SW_HIDE);
			break;
		case 5:
			appBtnDlg.ShowWindow(SW_HIDE);
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
			appDlg.ShowWindow(SW_SHOW);
			break;
		case 2:
			epgDlg.ShowWindow(SW_SHOW);
			break;
		case 3:
			serviceDlg.ShowWindow(SW_SHOW);
			break;
		case 4:
			networkDlg.ShowWindow(SW_SHOW);
			break;
		case 5:
			appBtnDlg.ShowWindow(SW_SHOW);
			break;
		default:
			break;
	}

	*pResult = 0;
}
