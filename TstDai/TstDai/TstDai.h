
// TstDai.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#include "TstDaiMain.h"

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CTstDaiApp:
// このクラスの実装については、TstDai.cpp を参照してください。
//

class CTstDaiApp : public CWinApp
{
public:
	CTstDaiApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CTstDaiApp theApp;