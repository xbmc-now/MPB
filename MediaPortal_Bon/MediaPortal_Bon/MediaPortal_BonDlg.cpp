
// MediaPortal_BonDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MediaPortal_Bon.h"
#include "MediaPortal_BonDlg.h"
#include "afxdialogex.h"


#include "../../Common/TimeUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMediaPortal_BonDlg ダイアログ


UINT CMediaPortal_BonDlg::taskbarCreated = 0;

CMediaPortal_BonDlg::CMediaPortal_BonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMediaPortal_BonDlg::IDD, pParent)
	, log(_T(""))
	, statusLog(_T(""))
	, pgInfo(_T(""))
{
	m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_ICON_BLUE ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIcon2 = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_ICON_BLUE ), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	iconRed = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_ICON_RED ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	iconBlue = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_ICON_BLUE ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	iconGreen = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_ICON_GREEN ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	iconGray = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_ICON_GRAY ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	wstring strPath = L"";
	GetModuleIniPath(strPath);
	this->moduleIniPath = strPath.c_str();
	GetCommonIniPath(strPath);
	this->commonIniPath = strPath.c_str();
	GetEpgTimerSrvIniPath(strPath);
	this->timerSrvIniPath = strPath.c_str();

	this->initONID = GetPrivateProfileInt( L"Set", L"LastONID", -1, this->moduleIniPath );
	this->initTSID = GetPrivateProfileInt( L"Set", L"LastTSID", -1, this->moduleIniPath );
	this->initSID = GetPrivateProfileInt( L"Set", L"LastSID", -1, this->moduleIniPath );
	WCHAR buff[512]=L"";
	GetPrivateProfileString( L"Set", L"LastBon", L"", buff, 512, this->moduleIniPath );
	this->iniBonDriver = buff;

	iniView = FALSE;
	iniNetwork = TRUE;
	iniMin = FALSE;
	this->iniUDP = TRUE;
	this->iniTCP = FALSE;
	
	this->minTask = GetPrivateProfileInt( L"Set", L"MinTask", 0, this->moduleIniPath );
	this->openLastCh = GetPrivateProfileInt( L"Set", L"OpenLast", 1, this->moduleIniPath );
	if( this->openLastCh == 0 ){
		if( GetPrivateProfileInt( L"Set", L"OpenFix", 0, this->moduleIniPath ) == 1){
			this->initONID = GetPrivateProfileInt( L"Set", L"FixONID", -1, this->moduleIniPath );
			this->initTSID = GetPrivateProfileInt( L"Set", L"FixTSID", -1, this->moduleIniPath );
			this->initSID = GetPrivateProfileInt( L"Set", L"FixSID", -1, this->moduleIniPath );
			GetPrivateProfileString( L"Set", L"FixBon", L"", buff, 512, this->moduleIniPath );
			this->iniBonDriver = buff;
		}else{
			this->initONID = -1;
			this->initTSID = -1;
			this->initSID = -1;
			this->iniBonDriver = L"";
		}
	}
	this->initOpenWait = 0;
	this->initChgWait = 0;
}

void CMediaPortal_BonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TUNER, combTuner);
	DDX_Control(pDX, IDC_BUTTON_CHSCAN, btnChScan);
	DDX_Control(pDX, IDC_BUTTON_SET, btnSet);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, btnCancel);
	DDX_Text(pDX, IDC_EDIT_LOG, log);
	DDX_Text(pDX, IDC_EDIT_STATUS, statusLog);
	DDX_Control(pDX, IDC_EDIT_STATUS, editStatus);
}

BEGIN_MESSAGE_MAP(CMediaPortal_BonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(CMediaPortal_BonDlg::taskbarCreated, OnTaskbarCreated)
	ON_CBN_SELCHANGE(IDC_COMBO_TUNER, &CMediaPortal_BonDlg::OnCbnSelchangeComboTuner)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CMediaPortal_BonDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_CHSCAN, &CMediaPortal_BonDlg::OnBnClickedButtonChscan)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CMediaPortal_BonDlg::OnBnClickedButtonCancel)
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
END_MESSAGE_MAP()


// CMediaPortal_BonDlg メッセージ ハンドラー
void CMediaPortal_BonDlg::SetInitBon(CString bonFile)
{
	iniBonDriver = bonFile;
	if( GetPrivateProfileInt( iniBonDriver.GetBuffer(0), L"OpenFix", 0, this->moduleIniPath ) == 1){
		OutputDebugString(L"強制サービス指定 設定値ロード");
		this->initONID = GetPrivateProfileInt( iniBonDriver.GetBuffer(0), L"FixONID", -1, this->moduleIniPath );
		this->initTSID = GetPrivateProfileInt( iniBonDriver.GetBuffer(0), L"FixTSID", -1, this->moduleIniPath );
		this->initSID = GetPrivateProfileInt( iniBonDriver.GetBuffer(0), L"FixSID", -1, this->moduleIniPath );
		this->initOpenWait = GetPrivateProfileInt( iniBonDriver.GetBuffer(0), L"OpenWait", 0, this->moduleIniPath );
		this->initChgWait = GetPrivateProfileInt( iniBonDriver.GetBuffer(0), L"ChgWait", 0, this->moduleIniPath );
		_OutputDebugString(L"%d,%d,%d,%d,%d",initONID,initTSID,initSID,initOpenWait,initChgWait );
	}
}

BOOL CMediaPortal_BonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon2, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	this->main.ReloadSetting();

	//BonDriverの一覧取得
	ReloadBonDriver();

	//BonDriverのオープン
	DWORD err = NO_ERR;
	if( this->iniBonDriver.IsEmpty() == false ){
		err = SelectBonDriver(this->iniBonDriver.GetBuffer(0), TRUE);
		this->log.Format(L"BonDriverのオープンを選択しました。\r\n%s\r\n", this->iniBonDriver.GetBuffer(0));
		Sleep(this->initOpenWait);
	}else{
		map<int, wstring>::iterator itr;
		itr = this->bonList.begin();
		if( itr != this->bonList.end() ){
			err = SelectBonDriver(itr->second.c_str());
			this->log.Format(L"BonDriverのオープンを選択しました。\r\n%s\r\n", itr->second.c_str());
		}else{
			err = ERR_FALSE;
			this->log += L"BonDriverが見つかりませんでした\r\n";
		}
	}

	// MediaPortal TV Server
	if((this->mpServiceStat = this->main.GetMpServiceStatus()) != 0 ) {
		switch(this->mpServiceStat){
			case 1:
				this->log.Format(L"サービスマネージャが起動ができませんでした。\r\n");
				break;
			case 2:
				this->log.Format(L"MediaPortal TV Serverがインストールされていません。\r\n");
				break;
			case 3:
				this->log.Format(L"TVServiceサービスが起動していません\r\n");
				break;
			default:
				break;
		}
	}

	// MediaPortalログパス取得
	this->main.GetMpLogPath(this->mpLogPath);
	this->mpNowLogSz = 0;
	this->mpPreLogSz = 0;
	//this->log += this->mpLogPath;

	this->results = NULL;
	if (this->dbCtrl.Connect(&this->mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB) != 0) {
		this->log += _T("DB接続失敗");
	} else {
		this->log += _T("DB接続成功");
		this->dbCtrl.Close(&this->mysql);
	}

	if( err == NO_ERR ){
		//チャンネル変更
		if( this->initONID != -1 && this->initTSID != -1 && this->initSID != -1 ){
			SelectService(this->initONID, this->initTSID, this->initSID);
			this->initONID = -1;
			this->initTSID = -1;
			this->initSID = -1;
			Sleep(this->initChgWait);
		}
		this->main.SendUDP(TRUE);
	}

	//ウインドウの復元
	WINDOWPLACEMENT Pos;
	Pos.length = sizeof(WINDOWPLACEMENT);
	Pos.flags = NULL;
	if( this->iniMin == FALSE ){
		Pos.showCmd = SW_SHOW;
	}else{
		Pos.showCmd = SW_SHOWMINNOACTIVE;
	}
	Pos.rcNormalPosition.left = GetPrivateProfileInt(L"SET_WINDOW", L"left", 0, this->moduleIniPath);
	Pos.rcNormalPosition.right = GetPrivateProfileInt(L"SET_WINDOW", L"right", 0, this->moduleIniPath);
	Pos.rcNormalPosition.top = GetPrivateProfileInt(L"SET_WINDOW", L"top", 0, this->moduleIniPath);
	Pos.rcNormalPosition.bottom = GetPrivateProfileInt(L"SET_WINDOW", L"bottom", 0, this->moduleIniPath);
	if( Pos.rcNormalPosition.left != 0 &&
		Pos.rcNormalPosition.right != 0 &&
		Pos.rcNormalPosition.top != 0 &&
		Pos.rcNormalPosition.bottom != 0 ){
		SetWindowPlacement(&Pos);
	}
	SetTimer(TIMER_STATUS_UPDATE, 1000, NULL);
	SetTimer(TIMER_INIT_DLG, 1, NULL);
	this->main.SetHwnd(GetSafeHwnd());

	UpdateData(FALSE);

	this->main.StartServer();

	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMediaPortal_BonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMediaPortal_BonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CMediaPortal_BonDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam  == VK_RETURN || pMsg->wParam  == VK_ESCAPE ){
			return FALSE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


int CMediaPortal_BonDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ここに特定な作成コードを追加してください。

	return 0;
}


void CMediaPortal_BonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if( nID == SC_CLOSE ){
		if( this->main.IsRec() == TRUE ){
			if( AfxMessageBox( L"録画中ですが終了しますか？", MB_YESNO ) == IDNO ){
				return ;
			}
			this->main.StopReserveRec();
			this->main.StopRec();
		}
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


void CMediaPortal_BonDlg::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	KillTimer(TIMER_STATUS_UPDATE);
	this->main.StopServer();
	this->main.CloseBonDriver();

	CDialogEx::OnClose();
}


void CMediaPortal_BonDlg::OnDestroy()
{
	UpdateData(TRUE);
	this->main.StopServer();
	this->main.CloseBonDriver();
	KillTimer(TIMER_STATUS_UPDATE);

	KillTimer(RETRY_ADD_TRAY);
	DeleteTaskBar(GetSafeHwnd(), TRAYICON_ID);

	WINDOWPLACEMENT Pos;
	GetWindowPlacement(&Pos);
	CString strAdd;

	strAdd.Format(L"%d", Pos.rcNormalPosition.top );
	WritePrivateProfileString(L"SET_WINDOW", L"top", strAdd.GetBuffer(0), this->moduleIniPath);
	strAdd.Format(L"%d", Pos.rcNormalPosition.left );
	WritePrivateProfileString(L"SET_WINDOW", L"left", strAdd.GetBuffer(0), this->moduleIniPath);
	strAdd.Format(L"%d", Pos.rcNormalPosition.bottom );
	WritePrivateProfileString(L"SET_WINDOW", L"bottom", strAdd.GetBuffer(0), this->moduleIniPath);
	strAdd.Format(L"%d", Pos.rcNormalPosition.right );
	WritePrivateProfileString(L"SET_WINDOW", L"right", strAdd.GetBuffer(0), this->moduleIniPath);

	int selONID = -1;
	int selTSID = -1;
	int selSID = -1;
	CString bon = L"";

	this->combTuner.GetWindowText(bon);

	WritePrivateProfileString(L"SET", L"LastBon", bon.GetBuffer(0), this->moduleIniPath);

	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


void CMediaPortal_BonDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	switch(nIDEvent){
		case TIMER_INIT_DLG:
			{
				KillTimer( TIMER_INIT_DLG );
				if( this->iniMin == TRUE && this->minTask == TRUE){
				    ShowWindow(SW_HIDE);
				}
			}
			break;
		case TIMER_STATUS_UPDATE:
			{
				KillTimer( TIMER_STATUS_UPDATE );
				SetThreadExecutionState(ES_SYSTEM_REQUIRED);

				int iLine = this->editStatus.GetFirstVisibleLine();
				float signal = 0;
				DWORD space = 0;
				DWORD ch = 0;
				ULONGLONG drop = 0;
				ULONGLONG scramble = 0;
				vector<NW_SEND_INFO> udpSendList;
				vector<NW_SEND_INFO> tcpSendList;

				BOOL ret = this->main.GetViewStatusInfo(&signal, &space, &ch, &drop, &scramble, &udpSendList, &tcpSendList);

				if(ret==TRUE){
					this->statusLog.Format(L"Signal: %.02f Drop: %I64d Scramble: %I64d  space: %d ch: %d",signal, drop, scramble, space, ch);
				}else{
					this->statusLog.Format(L"Signal: %.02f Drop: %I64d Scramble: %I64d",signal, drop, scramble);
				}
				this->statusLog += L"\r\n";

				CString udp = L"";
				if( udpSendList.size() > 0 ){
					udp = "UDP送信：";
					for( size_t i=0; i<udpSendList.size(); i++ ){
						CString buff;
						if( udpSendList[i].broadcastFlag == FALSE ){
							buff.Format(L"%s:%d ",udpSendList[i].ipString.c_str(), udpSendList[i].port);
						}else{
							buff.Format(L"%s:%d(Broadcast) ",udpSendList[i].ipString.c_str(), udpSendList[i].port);
						}
						udp += buff;
					}
					udp += L"\r\n";
				}
				this->statusLog += udp;

				CString tcp = L"";
				if( tcpSendList.size() > 0 ){
					tcp = "TCP送信：";
					for( size_t i=0; i<tcpSendList.size(); i++ ){
						CString buff;
						buff.Format(L"%s:%d ",tcpSendList[i].ipString.c_str(), tcpSendList[i].port);
						tcp += buff;
					}
					tcp += L"\r\n";
				}
				this->statusLog += tcp;

				SetDlgItemText(IDC_EDIT_STATUS, this->statusLog);
				editStatus.LineScroll(iLine);

				//--------------------------------------------------------------
				// TVService.log監視
				DWORD err = 0;
				FILE *fp;
				//wstring str[1024];
				TCHAR str[1024];
				//WCHAR *str = L"";
				if(this->mpServiceStat == 0){ // TVServiceサービス起動中

					if((err = _tfopen_s(&fp,this->mpLogPath, L"r, ccs=UTF-8")) != 0 ) {
						this->log.Format(L"TVService.logオープンエラー\r\n");
					} else {

						fseek(fp,0,SEEK_END); 
						fgetpos(fp,&this->mpNowLogSz); // 現在ファイルサイズ取得

						if(this->mpPreLogSz == 0 ) this->mpPreLogSz = this->mpNowLogSz; // 初期ファイルサイズ取得

						// 現在のファイルサイズが前回よりも小さい場合は前回をゼロにする。
						if(this->mpPreLogSz > this->mpNowLogSz) this->mpPreLogSz = 0;

						if(this->mpPreLogSz < this->mpNowLogSz){
							fsetpos(fp, &this->mpPreLogSz); // シーク
							//this->log = L"";
							//this->log.Format(L"増えた%I64dから%I64d\r\n",this->mpPreLogSz,this->mpNowLogSz);

							wregex re(L"\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\,[0-9]{3}\\] \\[.{7}\\] \\[.{9}\\] \\[.{5}\\] - Controller: StartTimeShifting (?!started on card:|failed:).* ([0-9]+)$");

							this->mpStartTimeShifting = L"";

							while ( _fgetts(str, 1024, fp) != NULL ) { // 追加分を表示
								//this->log += str;
								//this->log += L"\r\n";
								wstring text(str);
								wsmatch m;
								if( regex_search(text, m, re) ) this->mpStartTimeShifting = m[1];
							}
							this->mpPreLogSz = this->mpNowLogSz; // ファイルサイズを代入

							if (this->mpStartTimeShifting != L""){
								//this->log = L"";
								//this->log += text;
								this->log.Format(L"チャンネルヒット%s\r\n",this->mpStartTimeShifting.c_str());

								this->results = NULL;
								CString sql = L"";
								wstring wsql = L"";
								int chkNum = 0;

								// MediaPortal TV Serverのデータベース接続
								if (this->dbCtrl.Connect(&this->mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB) == 0){

									// groupmapからidGroupを得る
									sql.Format(_T("SELECT idGroup FROM groupmap WHERE idChannel = %s AND  idGroup < 2;"), 
										this->mpStartTimeShifting.c_str());
									if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
									this->dbCtrl.StoreResult(&this->mysql, &this->results);
									chkNum = this->dbCtrl.NumRows(&this->results);
									this->dbCtrl.FreeResult(&this->results);

									if(chkNum){
										this->mpServiceList.clear();
										// チャンネルの詳細情報を得る
										sql.Format(_T("SELECT tuningdetail.provider, tuningdetail.networkId, tuningdetail.transportId, tuningdetail.serviceId, groupmap.idGroup, tuningdetail.channelNumber FROM tuningdetail LEFT JOIN groupmap ON tuningdetail.idChannel = groupmap.idChannel WHERE groupmap.idGroup IN(0,1) AND tuningdetail.idChannel = %s;"), 
											this->mpStartTimeShifting.c_str());
										if (this->dbCtrl.Query(&this->mysql, sql) != 0) goto ESC;
										this->dbCtrl.StoreResult(&this->mysql, &this->results);
										chkNum = this->dbCtrl.NumRows(&this->results);

										// 現在開いているBonDriver
										wstring bonFile = L"";
										this->main.GetOpenBonDriver(&bonFile);

										while (this->record = this->dbCtrl.FetchRow(&this->results)) {
											CH_DATAMP item;
											this->log = L"";
											this->log += CA2T(this->record[0], CP_UTF8);
											this->log += this->record[1];
											item.bonName           = CA2T(this->record[0], CP_UTF8);
											item.originalNetworkID = atoi(this->record[1]);
											item.transportStreamID = atoi(this->record[2]);
											item.serviceID         = atoi(this->record[3]);
											item.space             = atoi(this->record[4]);
											item.ch                = atoi(this->record[5]);
											this->mpServiceList.push_back(item);
											//goto ESC;
										}

										// BonDriverが同じか
										if(bonFile != this->mpServiceList[0].bonName.c_str()){
											KillTimer(TIMER_STATUS_UPDATE);
											SelectBonDriver(this->mpServiceList[0].bonName.c_str());
											this->iniBonDriver = this->mpServiceList[0].bonName.c_str();
											ReloadBonDriver();
											ChgIconStatus();
											SetTimer(TIMER_STATUS_UPDATE, 3000, NULL);

											SelectService(
												this->mpServiceList[0].originalNetworkID, 
												this->mpServiceList[0].transportStreamID, 
												this->mpServiceList[0].serviceID,
												this->mpServiceList[0].space,
												this->mpServiceList[0].ch
											);

										} else {
											SelectService(
												this->mpServiceList[0].originalNetworkID, 
												this->mpServiceList[0].transportStreamID, 
												this->mpServiceList[0].serviceID
											);
											this->initONID = -1;
											this->initTSID = -1;
											this->initSID = -1;

										}

										// チャンネル変更
/*
										SelectService(
											this->mpServiceList[0].originalNetworkID, 
											this->mpServiceList[0].transportStreamID, 
											this->mpServiceList[0].serviceID,
											this->mpServiceList[0].space,
											this->mpServiceList[0].ch
										);

										SelectService(
											this->mpServiceList[0].originalNetworkID, 
											this->mpServiceList[0].transportStreamID, 
											this->mpServiceList[0].serviceID
										);
										this->initONID = -1;
										this->initTSID = -1;
										this->initSID = -1;
*/
										this->log = L"チャンネル変更";
										Sleep(this->initChgWait);
									}
									ESC:
									this->dbCtrl.FreeResult(&this->results);
									this->dbCtrl.Close(&this->mysql);
								}
							}
						}
						fclose(fp);
					}
				}
				//--------------------------------------------------------------

				SetDlgItemText(IDC_EDIT_LOG, this->log);
				SetTimer(TIMER_STATUS_UPDATE, 1000, NULL);
			}
			break;
		case TIMER_CHSCAN_STATSU:
			{
				KillTimer( TIMER_CHSCAN_STATSU );
				DWORD space = 0;
				DWORD ch = 0;
				wstring chName = L"";
				DWORD chkNum = 0;
				DWORD totalNum = 0;
				DWORD status = this->main.GetChScanStatus(&space, &ch, &chName, &chkNum, &totalNum);
				if( status == ST_WORKING ){
					this->log.Format(L"%s (%d/%d 残り約 %d 秒)\r\n", chName.c_str(), chkNum, totalNum, (totalNum - chkNum)*10);
					SetDlgItemText(IDC_EDIT_LOG, this->log);
					SetTimer(TIMER_CHSCAN_STATSU, 1000, NULL);
				}else if( status == ST_CANCEL ){
					KillTimer(TIMER_CHSCAN_STATSU);
					this->log = L"キャンセルされました\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
				}else if( status == ST_COMPLETE ){
					KillTimer(TIMER_CHSCAN_STATSU);
					this->log = L"終了しました\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);

					BtnUpdate(GUI_NORMAL);
					ChgIconStatus();

					//同じサービスが別の物理チャンネルにあるかチェック
					wstring msg = L"";
					for( size_t i=0; i<this->serviceList.size(); i++ ){
						for( size_t j=i+1; j<this->serviceList.size(); j++ ){
							if( this->serviceList[i].originalNetworkID == this->serviceList[j].originalNetworkID &&
								this->serviceList[i].transportStreamID == this->serviceList[j].transportStreamID &&
								this->serviceList[i].serviceID == this->serviceList[j].serviceID ){
									wstring log = L"";
									Format(log, L"%s space:%d ch:%d <=> %s space:%d ch:%d\r\n",
										this->serviceList[i].serviceName.c_str(),
										this->serviceList[i].space,
										this->serviceList[i].ch,
										this->serviceList[j].serviceName.c_str(),
										this->serviceList[j].space,
										this->serviceList[j].ch);
									msg += log;
									break;
							}
						}
					}
					if( msg.size() > 0){
						wstring log = L"同一サービスが複数の物理チャンネルで検出されました。\r\n受信環境のよい物理チャンネルのサービスのみ残すように設定を行ってください。\r\n正常に録画できない可能性が出てきます。\r\n\r\n";
						log += msg;
						MessageBox(log.c_str());
					}
				}
			}
			break;
		case TIMER_EPGCAP_STATSU:
			{
				KillTimer( TIMER_EPGCAP_STATSU );
				EPGCAP_SERVICE_INFO info;
				DWORD status = this->main.GetEpgCapStatus(&info);
				if( status == ST_WORKING ){


					this->log = L"EPG取得中\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
					SetTimer(TIMER_EPGCAP_STATSU, 1000, NULL);
				}else if( status == ST_CANCEL ){
					KillTimer(TIMER_EPGCAP_STATSU);
					this->log = L"キャンセルされました\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
				}else if( status == ST_COMPLETE ){
					KillTimer(TIMER_EPGCAP_STATSU);
					this->log = L"終了しました\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
					BtnUpdate(GUI_NORMAL);
					ChgIconStatus();
				}
			}
			break;
		case RETRY_ADD_TRAY:
			{
				KillTimer(RETRY_ADD_TRAY);
				CString buff=L"";
				wstring bonFile = L"";
				this->main.GetOpenBonDriver(&bonFile);


				HICON setIcon = this->iconBlue;
				if( this->main.IsRec() == TRUE ){
					setIcon = this->iconRed;
				}else if( this->main.GetEpgCapStatus(NULL) == ST_WORKING ){
					setIcon = this->iconGreen;
				}else if( this->main.GetOpenBonDriver(NULL) == FALSE ){
					setIcon = this->iconGray;
				}
		
				if( AddTaskBar( GetSafeHwnd(),
						WM_TRAY_PUSHICON,
						TRAYICON_ID,
						setIcon,
						buff ) == FALSE ){
							SetTimer(RETRY_ADD_TRAY, 5000, NULL);
				}
			}
			break;
		default:
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMediaPortal_BonDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if( nType == SIZE_MINIMIZED && this->minTask == TRUE){
		CString buff=L"";
		wstring bonFile = L"";
		this->main.GetOpenBonDriver(&bonFile);

		HICON setIcon = this->iconBlue;
		if( this->main.IsRec() == TRUE ){
			setIcon = this->iconRed;
		}else if( this->main.GetEpgCapStatus(NULL) == ST_WORKING ){
			setIcon = this->iconGreen;
		}else if( this->main.GetOpenBonDriver(NULL) == FALSE ){
			setIcon = this->iconGray;
		}
		
		if( AddTaskBar( GetSafeHwnd(),
				WM_TRAY_PUSHICON,
				TRAYICON_ID,
				setIcon,
				buff ) == FALSE ){
					SetTimer(RETRY_ADD_TRAY, 5000, NULL);
		}
		if(!this->iniMin) ShowWindow(SW_HIDE);
	}
}


LRESULT CMediaPortal_BonDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	switch(message){
	case WM_CHG_TUNER:
		{
			wstring bonDriver = L"";
			this->main.GetOpenBonDriver(&bonDriver);
			this->iniBonDriver = bonDriver.c_str();
			ReloadBonDriver();
			ChgIconStatus();
		}
		break;
	case WM_CHG_CH:
		{
			WORD ONID;
			WORD TSID;
			WORD SID;
			this->main.GetCh(&ONID, &TSID, &SID);
			this->initONID = ONID;
			this->initTSID = TSID;
			this->initSID = SID;
			ChgIconStatus();
		}
		break;
	case WM_TRAY_PUSHICON:
		{
			//タスクトレイ関係
			switch(LOWORD(lParam)){
				case WM_LBUTTONDOWN:
					{
						this->iniMin = FALSE;
						ShowWindow(SW_RESTORE);
						SetForegroundWindow();
						KillTimer(RETRY_ADD_TRAY);
						DeleteTaskBar(GetSafeHwnd(), TRAYICON_ID);
					}
					break ;
				default :
					break ;
				}
		}
		break;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


BOOL CMediaPortal_BonDlg::AddTaskBar(HWND wnd, UINT msg, UINT id, HICON icon, CString tips)
{ 
	BOOL ret=TRUE;
	NOTIFYICONDATA data;
	ZeroMemory(&data, sizeof(NOTIFYICONDATA));

	data.cbSize = sizeof(NOTIFYICONDATA); 
	data.hWnd = wnd; 
	data.uID = id; 
	data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	data.uCallbackMessage = msg; 
	data.hIcon = icon; 

	if( tips.IsEmpty() == false ){
		wcsncpy_s(data.szTip, sizeof(data.szTip), tips.GetBuffer(0), sizeof(data.szTip) );
	}
 
	ret = Shell_NotifyIcon(NIM_ADD, &data);
  
	return ret; 
}

BOOL CMediaPortal_BonDlg::ChgTipsTaskBar(HWND wnd, UINT id, HICON icon, CString tips)
{ 
	BOOL ret=TRUE;
	NOTIFYICONDATA data;
	ZeroMemory(&data, sizeof(NOTIFYICONDATA));

	data.cbSize = sizeof(NOTIFYICONDATA); 
	data.hWnd = wnd; 
	data.uID = id; 
	data.hIcon = icon; 
	data.uFlags = NIF_ICON | NIF_TIP; 

	if( tips.IsEmpty() == false ){
		wcsncpy_s(data.szTip, sizeof(data.szTip), tips.GetBuffer(0), sizeof(data.szTip) );
	}
 
	ret = Shell_NotifyIcon(NIM_MODIFY, &data); 
 
	return ret; 
}

BOOL CMediaPortal_BonDlg::DeleteTaskBar(HWND wnd, UINT id)
{ 
	BOOL ret=TRUE; 
	NOTIFYICONDATA data; 
	ZeroMemory(&data, sizeof(NOTIFYICONDATA));
 
	data.cbSize = sizeof(NOTIFYICONDATA); 
	data.hWnd = wnd; 
	data.uID = id; 
         
	ret = Shell_NotifyIcon(NIM_DELETE, &data); 

	return ret; 
}

void CMediaPortal_BonDlg::ChgIconStatus(){
	if( this->minTask == TRUE){
		CString buff=L"";
		wstring bonFile = L"";
		this->main.GetOpenBonDriver(&bonFile);

		HICON setIcon = this->iconBlue;
		if( this->main.IsRec() == TRUE ){
			setIcon = this->iconRed;
		}else if( this->main.GetEpgCapStatus(NULL) == ST_WORKING ){
			setIcon = this->iconGreen;
		}else if( this->main.GetOpenBonDriver(NULL) == FALSE ){
			setIcon = this->iconGray;
		}

		ChgTipsTaskBar( GetSafeHwnd(),
				TRAYICON_ID,
				setIcon,
				buff );
	}
}

LRESULT CMediaPortal_BonDlg::OnTaskbarCreated(WPARAM, LPARAM)
{
	if( IsWindowVisible() == FALSE && this->minTask == TRUE){
		CString buff=L"";
		wstring bonFile = L"";
		this->main.GetOpenBonDriver(&bonFile);

		HICON setIcon = this->iconBlue;
		if( this->main.IsRec() == TRUE ){
			setIcon = this->iconRed;
		}else if( this->main.GetEpgCapStatus(NULL) == ST_WORKING ){
			setIcon = this->iconGreen;
		}else if( this->main.GetOpenBonDriver(NULL) == FALSE ){
			setIcon = this->iconGray;
		}
		
		if( AddTaskBar( GetSafeHwnd(),
				WM_TRAY_PUSHICON,
				TRAYICON_ID,
				setIcon,
				buff ) == FALSE ){
					SetTimer(RETRY_ADD_TRAY, 5000, NULL);
		}
	}

	return 0;
}

void CMediaPortal_BonDlg::BtnUpdate(DWORD guiMode)
{
	switch(guiMode){
		case GUI_NORMAL:
			this->combTuner.EnableWindow(TRUE);
			this->btnChScan.EnableWindow(TRUE);
			this->btnSet.EnableWindow(TRUE);
			this->btnCancel.EnableWindow(FALSE);
			break;
		case GUI_CANCEL_ONLY:
			this->combTuner.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_OPEN_FAIL:
			this->combTuner.EnableWindow(TRUE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(TRUE);
			this->btnCancel.EnableWindow(FALSE);
			break;
		case GUI_REC:
			this->combTuner.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_REC_SET_TIME:
			this->combTuner.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_OTHER_CTRL:
			this->combTuner.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_REC_STANDBY:
			this->combTuner.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(FALSE);
			break;
		default:
			break;
	}
}



void CMediaPortal_BonDlg::OnCbnSelchangeComboTuner()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	KillTimer(TIMER_STATUS_UPDATE);
	CString buff=L"";
	//this->combTuner.GetWindowText(buff);
	this->combTuner.GetLBText(this->combTuner.GetCurSel(), buff);

	if( buff.IsEmpty() == false ){
		SelectBonDriver(buff.GetBuffer(0));
		//AfxMessageBox(buff.GetBuffer(0), NULL, MB_OK);
	}
	SetTimer(TIMER_STATUS_UPDATE, 1000, NULL);
}


void CMediaPortal_BonDlg::OnBnClickedButtonSet()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CSettingDlg setDlg;
	if( setDlg.DoModal() == IDOK ){

		this->main.ReloadSetting();

		WORD ONID;
		WORD TSID;
		WORD SID;
		this->main.GetCh(&ONID, &TSID, &SID);
		this->initONID = ONID;
		this->initTSID = TSID;
		this->initSID = SID;
		

		wstring ipString;
		DWORD ip;
		DWORD port;

		ip = GetPrivateProfileInt(L"SET_UDP", L"IP0", 2130706433, moduleIniPath);
		Format(ipString, L"%d.%d.%d.%d", 
		(ip&0xFF000000)>>24, 
		(ip&0x00FF0000)>>16, 
		(ip&0x0000FF00)>>8, 
		(ip&0x000000FF) );
		port = GetPrivateProfileInt( L"SET_UDP", L"Port0", 3456, moduleIniPath );

		float signal = 0;
		DWORD space = 0;
		DWORD ch = 0;
		ULONGLONG drop = 0;
		ULONGLONG scramble = 0;
		vector<NW_SEND_INFO> udpSendList;
		vector<NW_SEND_INFO> tcpSendList;

		BOOL ret = this->main.GetViewStatusInfo(&signal, &space, &ch, &drop, &scramble, &udpSendList, &tcpSendList);

		if( udpSendList[0].ipString.c_str() != ipString || udpSendList[0].port != port ){
			this->main.SendUDP(FALSE);
			this->main.SendUDP(TRUE);
		}

		this->minTask = GetPrivateProfileInt( L"Set", L"MinTask", 0, this->moduleIniPath );
	}
}

void CMediaPortal_BonDlg::ReloadBonDriver()
{
	this->bonList.clear();
	this->combTuner.ResetContent();

	this->main.EnumBonDriver(&bonList);

	int selectIndex = 0;
	map<int, wstring>::iterator itr;
	for( itr = this->bonList.begin(); itr != this->bonList.end(); itr++ ){
		int index = this->combTuner.AddString(itr->second.c_str());
		if( this->iniBonDriver.IsEmpty() == false ){
			if( this->iniBonDriver.Compare(itr->second.c_str()) == 0 ){
				selectIndex = index;
			}
		}
	}
	if( this->bonList.size() > 0){
		this->combTuner.SetCurSel(selectIndex);
	}
}


DWORD CMediaPortal_BonDlg::SelectBonDriver(LPCWSTR fileName, BOOL ini)
{
	this->main.CloseBonDriver();
	DWORD err = this->main.OpenBonDriver(fileName);
	if( err != NO_ERR ){
		this->log.Format(L"BonDriverのオープンができませんでした\r\n%s\r\n", fileName);
		SetDlgItemText(IDC_EDIT_LOG, this->log);
		BtnUpdate(GUI_OPEN_FAIL);
	}else{
		this->log = L"";
		SetDlgItemText(IDC_EDIT_LOG, this->log);
		BtnUpdate(GUI_NORMAL);
	}
	return err;
}

DWORD CMediaPortal_BonDlg::SelectService(WORD ONID, WORD TSID, WORD SID)
{
	DWORD err = this->main.SetCh(ONID, TSID, SID);
	return err;
}

DWORD CMediaPortal_BonDlg::SelectService(WORD ONID, WORD TSID, WORD SID,	DWORD space, DWORD ch)
{
	DWORD err = this->main.SetCh(ONID, TSID, SID, space, ch);
	return err;
}

void CMediaPortal_BonDlg::OnBnClickedButtonChscan()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if( this->main.StartChScan() != NO_ERR ){
		this->log.Format(L"チャンネルスキャンを開始できませんでした\r\n");
		SetDlgItemText(IDC_EDIT_LOG, this->log);
		return;
	}
	SetTimer(TIMER_CHSCAN_STATSU, 1000, NULL);
	BtnUpdate(GUI_CANCEL_ONLY);
}

void CMediaPortal_BonDlg::OnBnClickedButtonCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if( this->main.IsRec() == TRUE ){
		if( AfxMessageBox( L"録画を停止しますか？", MB_YESNO ) == IDNO ){
			return ;
		}
	}
	this->log = L"キャンセルされました\r\n";
	SetDlgItemText(IDC_EDIT_LOG, this->log);

	this->main.StopChScan();
	KillTimer(TIMER_CHSCAN_STATSU);
	this->main.StopEpgCap();
	KillTimer(TIMER_EPGCAP_STATSU);
	this->main.StopRec();
	KillTimer(TIMER_REC_END);
	this->main.StopReserveRec();


	BtnUpdate(GUI_NORMAL);
	ChgIconStatus();
}

BOOL CMediaPortal_BonDlg::OnQueryEndSession()
{
	if (!CDialogEx::OnQueryEndSession())
		return FALSE;

	// TODO:  ここに特定なクエリの終了セッション コードを追加してください。
	if( this->main.IsRec() == TRUE ){
		ShowWindow(SW_SHOW);
		return FALSE;
	}
	return TRUE;
}


void CMediaPortal_BonDlg::OnEndSession(BOOL bEnding)
{
	CDialogEx::OnEndSession(bEnding);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if( bEnding == TRUE ){
		if( this->main.IsRec() == TRUE ){
			this->main.StopReserveRec();
			this->main.StopRec();
		}
	}
}
