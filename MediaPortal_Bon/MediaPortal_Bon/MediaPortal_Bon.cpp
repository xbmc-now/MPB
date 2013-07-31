
// MediaPortal_Bon.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "MediaPortal_Bon.h"
#include "MediaPortal_BonDlg.h"

#include "CmdLineUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMediaPortal_BonApp

BEGIN_MESSAGE_MAP(CMediaPortal_BonApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMediaPortal_BonApp コンストラクション

CMediaPortal_BonApp::CMediaPortal_BonApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CMediaPortal_BonApp オブジェクトです。

CMediaPortal_BonApp theApp;


// CMediaPortal_BonApp 初期化

BOOL CMediaPortal_BonApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	SetProcessShutdownParameters(0x300, 0);

	AfxEnableControlContainer();

	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	CShellManager *pShellManager = new CShellManager;

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	CCmdLineUtil cCmdUtil;
	ParseCommandLine(cCmdUtil);

	CMediaPortal_BonDlg dlg;

	map<CString, CString>::iterator itr;
	dlg.SetIniMin(FALSE);
	for( itr = cCmdUtil.m_CmdList.begin(); itr != cCmdUtil.m_CmdList.end(); itr++ ){
		if( itr->first.CompareNoCase(L"d") == 0 ){
			dlg.SetInitBon(itr->second);
			OutputDebugString(itr->second);
		}else if( itr->first.CompareNoCase(L"min") == 0 ){
			dlg.SetIniMin(TRUE);
		}
	}


	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}

	// 上で作成されたシェル マネージャーを削除します。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

