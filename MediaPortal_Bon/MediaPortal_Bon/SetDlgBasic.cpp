// SetDlgBasic.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MediaPortal_Bon.h"
#include "SetDlgBasic.h"
#include "afxdialogex.h"

#include "../../Common/PathUtil.h"
#include "../../Common/StringUtil.h"

// CSetDlgBasic ダイアログ

IMPLEMENT_DYNAMIC(CSetDlgBasic, CDialog)

CSetDlgBasic::CSetDlgBasic(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlgBasic::IDD, pParent)
	, settingFolderPath(_T(""))
{

}

CSetDlgBasic::~CSetDlgBasic()
{
}

void CSetDlgBasic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SET_PATH, settingFolderPath);
	DDX_Control(pDX, IDC_IPADDRESS_UDP, udpIP);
	DDX_Text(pDX, IDC_EDIT_PORT_UDP, udpPort);
	DDX_Text(pDX, IDC_EDIT_WAIT_SEC, udpWaitSec);
	DDX_Text(pDX, IDC_EDIT_WAIT_PACKET, udpWaitPacket);
	DDX_Control(pDX, IDC_CHECK_ENABLE_DECODE, btnEnableScramble);
	DDX_Control(pDX, IDC_CHECK_ALL_SERVICE, btnAllService);
	DDX_Control(pDX, IDC_CHECK_EMM, btnEnableEMM);
	DDX_Control(pDX, IDC_CHECK_TASKMIN, btnTaskMin);
	DDX_Text(pDX, IDC_EDIT_SET_EPGPATH, epgFolderPath);
	DDX_Control(pDX, IDC_CHECK_EPGTIMER, btnEpgTimer);
	DDX_Control(pDX, IDC_COMBO_HOUR, cmbHour);
	DDX_Control(pDX, IDC_COMBO_MIN, cmbMin);
}


BEGIN_MESSAGE_MAP(CSetDlgBasic, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH, &CSetDlgBasic::OnBnClickedButtonSetPath)
END_MESSAGE_MAP()


// CSetDlgBasic メッセージ ハンドラー
BOOL CSetDlgBasic::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	wstring path;
	GetSettingPath(path);
	settingFolderPath = path.c_str();

	udpIP.SetAddress( GetPrivateProfileInt( L"SET", L"UDPIP", 2130706433, appIniPath ));
	udpPort = GetPrivateProfileInt( L"SET", L"UDPPort", 3456, appIniPath );
	udpWaitSec = GetPrivateProfileInt( L"SET", L"UDPWait", 4, appIniPath );
	udpWaitPacket = GetPrivateProfileInt( L"SET", L"UDPPacket", 128, appIniPath );

	btnAllService.SetCheck( GetPrivateProfileInt( L"SET", L"AllService", 0, appIniPath ) );
	btnEnableScramble.SetCheck( GetPrivateProfileInt( L"SET", L"Scramble", 1, appIniPath ) );
	btnEnableEMM.SetCheck( GetPrivateProfileInt( L"SET", L"EMM", 0, appIniPath ) );
	btnTaskMin.SetCheck( GetPrivateProfileInt( L"SET", L"MinTask", 0, appIniPath ) );
	btnEpgTimer.SetCheck( GetPrivateProfileInt( L"EPG_TIMER", L"ChkTimer", 1, appIniPath ) );

	WCHAR timeString[512]=L"";
	GetPrivateProfileString(L"EPG_TIMER", L"time", L"23:30", timeString, 512, appIniPath);

	// 指定時間を0時からの秒数に変換
	wstring left = L"";
	wstring right = L"";
	Separate(timeString, L":", left, right);

	cmbHour.SetCurSel(cmbHour.FindStringExact(-1,left.c_str()));
	cmbMin.SetCurSel(cmbMin.FindStringExact(-1,right.c_str()));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CSetDlgBasic::SaveIni()
{
	UpdateData(TRUE);
	WritePrivateProfileString(L"SET", L"DataSavePath", settingFolderPath.GetBuffer(0), commonIniPath);
	_CreateDirectory(settingFolderPath);

	CString val = L"";
	DWORD ip = 0;
	udpIP.GetAddress(ip);

	val.Format(L"%d",ip);
	WritePrivateProfileString( L"SET", L"UDPIP", val.GetBuffer(0), appIniPath );
	val.Format( L"%d", udpPort );
	WritePrivateProfileString(L"SET", L"UDPPort", val.GetBuffer(0), appIniPath);
	val.Format( L"%d", udpWaitSec );
	WritePrivateProfileString(L"SET", L"UDPWait", val.GetBuffer(0), appIniPath);
	val.Format( L"%d", udpWaitPacket );
	WritePrivateProfileString(L"SET", L"UDPPacket", val.GetBuffer(0), appIniPath);

	val.Format( L"%d", 1 );
	WritePrivateProfileString(L"SET_UDP", L"Count", val.GetBuffer(0), appIniPath);

	int i = 0;
	CString key = L"";
	key.Format(L"IP%d",i);
	val.Format(L"%d",ip);
	WritePrivateProfileString( L"SET_UDP", key.GetBuffer(0), val.GetBuffer(0), appIniPath );
	key.Format(L"Port%d",i);
	val.Format(L"%d",udpPort);
	WritePrivateProfileString( L"SET_UDP", key.GetBuffer(0), val.GetBuffer(0), appIniPath );
	key.Format(L"BroadCast%d",i);
	val.Format(L"%d",0);
	WritePrivateProfileString( L"SET_UDP", key.GetBuffer(0), val.GetBuffer(0), appIniPath );

	val.Format(L"%d",btnAllService.GetCheck());
	WritePrivateProfileString( L"SET", L"AllService", val.GetBuffer(0), appIniPath );
	val.Format(L"%d",btnEnableScramble.GetCheck());
	WritePrivateProfileString( L"SET", L"Scramble", val.GetBuffer(0), appIniPath );
	val.Format(L"%d",btnEnableEMM.GetCheck());
	WritePrivateProfileString( L"SET", L"EMM", val.GetBuffer(0), appIniPath );
	val.Format(L"%d",btnTaskMin.GetCheck());
	WritePrivateProfileString( L"SET", L"MinTask", val.GetBuffer(0), appIniPath );

	chSet.SaveUdpMp();

	UpdateData(FALSE);
}


void CSetDlgBasic::OnBnClickedButtonSetPath()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData(TRUE);

	BROWSEINFO bi;
	LPWSTR lpBuffer;
	LPITEMIDLIST pidlRoot = NULL;
	LPITEMIDLIST pidlBrowse;
	LPMALLOC lpMalloc = NULL;

	HRESULT hr = SHGetMalloc(&lpMalloc);
	if(FAILED(hr)) return;

	if ((lpBuffer = (LPWSTR) lpMalloc->Alloc(_MAX_PATH*2)) == NULL) {
		return;
	}
	if( settingFolderPath.IsEmpty() != 0 ){
		if (!SUCCEEDED(SHGetSpecialFolderLocation( m_hWnd,CSIDL_DESKTOP,&pidlRoot ) )){ 
			lpMalloc->Free(lpBuffer);
			return;
		}
	}

	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = lpBuffer;
	bi.lpszTitle = L"設定関係保存フォルダを選択してください";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = (LPARAM)settingFolderPath.GetBuffer(0);

	pidlBrowse = SHBrowseForFolder(&bi);
	if (pidlBrowse != NULL) {  
		if (SHGetPathFromIDList(pidlBrowse, lpBuffer)) {
			settingFolderPath = lpBuffer;
		}
		lpMalloc->Free(pidlBrowse);
	}
	if( pidlRoot != NULL ){
		lpMalloc->Free(pidlRoot); 
	}
	lpMalloc->Free(lpBuffer);
	lpMalloc->Release();
	
	UpdateData(FALSE);
}


BOOL CSetDlgBasic::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam  == VK_RETURN || pMsg->wParam  == VK_ESCAPE ){
			return FALSE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

