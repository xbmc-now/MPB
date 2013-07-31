
// MediaPortal_Bon.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMediaPortal_BonApp:
// このクラスの実装については、MediaPortal_Bon.cpp を参照してください。
//

class CMediaPortal_BonApp : public CWinApp
{
public:
	CMediaPortal_BonApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMediaPortal_BonApp theApp;