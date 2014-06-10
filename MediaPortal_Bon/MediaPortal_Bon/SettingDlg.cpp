// SettingDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MediaPortal_Bon.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg ダイアログ

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


// CSettingDlg メッセージ ハンドラー


BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	wstring path = L"";
	GetCommonIniPath(path);
	commonIniPath = path.c_str();
	GetModuleIniPath(path);
	appIniPath = path.c_str();

	basicDlg.SetIniPath(commonIniPath, appIniPath);
	serviceDlg.SetIniPath(commonIniPath, appIniPath);

	TCITEM Item;
	Item.mask = TCIF_TEXT;

	Item.pszText = L"基本設定";
	dlgTab.InsertItem(0, &Item);

	Item.pszText = L"サービス表示設定";
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
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSettingDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	basicDlg.SaveIni();
	serviceDlg.SaveIni();

	//CMediaPortal_BonDlg* parent = STATIC_DOWNCAST(CMediaPortal_BonDlg, GetParent());// GetParent(子ダイアログのhwnd)
	//ASSERT(parent);
	//parent->SendMessage(WM_APP_SENDDATA, 0, 0);
	GetParent()->SendMessage(WM_APP_SENDDATA, 0, 0);
	CDialog::OnOK();
}


void CSettingDlg::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
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
