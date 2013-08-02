
// MediaPortal_BonDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MediaPortal_Bon.h"
#include "MediaPortal_BonDlg.h"
#include "afxdialogex.h"
#include "my_global.h"
#include "mysql.h"

#include "../../Common/TimeUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMediaPortal_BonDlg �_�C�A���O


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
	this->iniUDP = FALSE;
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
	DDX_Control(pDX, IDC_COMBO_SERVICE, combService);
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
	ON_CBN_SELCHANGE(IDC_COMBO_SERVICE, &CMediaPortal_BonDlg::OnCbnSelchangeComboService)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CMediaPortal_BonDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_CHSCAN, &CMediaPortal_BonDlg::OnBnClickedButtonChscan)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CMediaPortal_BonDlg::OnBnClickedButtonCancel)
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
END_MESSAGE_MAP()


// CMediaPortal_BonDlg ���b�Z�[�W �n���h���[
void CMediaPortal_BonDlg::SetInitBon(CString bonFile)
{
	iniBonDriver = bonFile;
	if( GetPrivateProfileInt( iniBonDriver.GetBuffer(0), L"OpenFix", 0, this->moduleIniPath ) == 1){
		OutputDebugString(L"�����T�[�r�X�w�� �ݒ�l���[�h");
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon2, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	this->main.ReloadSetting();

	//BonDriver�̈ꗗ�擾
	ReloadBonDriver();

	//BonDriver�̃I�[�v��
	DWORD err = NO_ERR;
	if( this->iniBonDriver.IsEmpty() == false ){
		err = SelectBonDriver(this->iniBonDriver.GetBuffer(0), TRUE);
		this->log.Format(L"BonDriver�̃I�[�v����I�����܂����BIsEmptyfalse\r\n%s\r\n", this->iniBonDriver.GetBuffer(0));
		Sleep(this->initOpenWait);
	}else{
		map<int, wstring>::iterator itr;
		itr = this->bonList.begin();
		if( itr != this->bonList.end() ){
			err = SelectBonDriver(itr->second.c_str());
			this->log.Format(L"BonDriver�̃I�[�v����I�����܂����B\r\n%s\r\n", itr->second.c_str());
		}else{
			err = ERR_FALSE;
			this->log += L"BonDriver��������܂���ł���\r\n";
		}
	}

	// MediaPortal TV Server
	if((this->mpServiceStat = this->main.GetMpServiceStatus()) != 0 ) {
		switch(this->mpServiceStat){
			case 1:
				this->log.Format(L"�T�[�r�X�}�l�[�W�����N�����ł��܂���ł����B\r\n");
				break;
			case 2:
				this->log.Format(L"MediaPortal TV Server���C���X�g�[������Ă��܂���B\r\n");
				break;
			case 3:
				this->log.Format(L"TVService�T�[�r�X���N�����Ă��܂���\r\n");
				break;
			default:
				break;
		}
	}

	// MediaPortal���O�p�X�擾
	this->main.GetMpLogPath(this->mpLogPath);
	this->mpNowLogSz = 0;
	this->mpPreLogSz = 0;
	//this->log += this->mpLogPath;


	this->results = NULL;


	if (this->dbCtrl.Connect(&this->mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB) != 0) {
		this->log += L"DB�ڑ����s"; 
	} else {
		this->log += L"DB�ڑ�����"; 
		this->dbCtrl.Query(&this->mysql, "SELECT idChannel, displayName FROM channel;");
		
	}


	//mysql_query(&this->mysql, "SELECT idChannel, displayName FROM channel;");
	
	// MySQL�ւ̐ڑ���ؒf����
	mysql_close(&this->mysql);

	//this->dbCtrl.Query(mysql, "SELECT idChannel, displayName FROM channel;");
	//this->dbCtrl.Query(this->mysql, CT2A(L"SELECT idChannel, displayName FROM channel;",CP_UTF8));
/*
	// �����R�[�h��ݒ肵�Ă���
	//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "sjis");

	//mysql_set_character_set(&mysql, "utf8");
	//
	//mysql_set_character_set(&mysql, "utf8");
	//mysql_query(&mysql,"SET NAMES utf8");

	//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	
	// MySQL�ɐڑ�����
	mysql_real_connect(&mysql, NULL, user_name, passwd, db_name, 0, 0, 0);
	if (mysql_errno(&mysql) != 0) {
		_tprintf(L"�G���[����: %s\n", mysql_error(&mysql));
		return -1;
	}

	// MySQL�ւ̐ڑ���ؒf����
	mysql_close(&this->mysql);
*/
	//this->log += CW2T(CA2W(host)); 



	//this->dbCtrl.Query(this->mysql, CT2A(L"SELECT idChannel, displayName FROM channel;",CP_UTF8));
/*

	if (this->dbCtrl.Connect(this->mysql, host, user, passwd, db) != 0) {
		this->log += L"DB�ڑ����s"; 
	} else {
		this->log += L"DB�ڑ�����"; 
	}


	mysql_query(&this->mysql, "SELECT idChannel, displayName FROM channel;");

	if (this->dbCtrl.Query(this->mysql, "SELECT idChannel, displayName FROM channel") != 0) {
		this->log += L"�N�G�����s"; 
	} else {
		this->log += L"�N�G������"; 
	}
*/

/*
	const char host[]   = MYSQL_HOST;
	const char user[]   = MYSQL_USER;
	const char passwd[] = MYSQL_PASSWD;
	const char db[]     = MYSQL_DB;
	this->results = NULL;

	if (this->main.ConnectDb(&this->mysql, host, user, passwd, db) != 0) {
		this->log += L"DB�ڑ����s"; 
	} else {
		this->log += L"DB�ڑ�����"; 
	}

	if (mysql_errno(&this->mysql) != 0) {
		_tprintf(L"�G���[����: %s\n", mysql_error(&this->mysql));
		return -1;
	}

	// �������s
	mysql_query(&this->mysql, "SELECT idChannel, displayName FROM channel;");
	if (mysql_errno(&this->mysql) != 0) {
		_tprintf(L"�G���[����: %s\n", mysql_error(&this->mysql));
		return -2;
	}

	// �������ʎ擾
	this->results = mysql_store_result(&this->mysql);
	if (results == NULL || mysql_errno(&this->mysql) != 0) {
		_tprintf(L"�G���[����: %s\n", mysql_error(&this->mysql));
		return -3;
	}


	// ���R�[�h���Ȃ��Ȃ�܂Ō������ʂ�\������
	while (this->record = mysql_fetch_row(this->results)) {
		//this->log += _stprintf(L"%s\n", CW2T(CA2W(this->record[1], CP_UTF8))); 
		this->log += CW2T(CA2W(this->record[1], CP_UTF8)); 
	}

	// �������ʊi�[�G���A���
	mysql_free_result(this->results);

	// MySQL�ւ̐ڑ���ؒf����
	mysql_close(&this->mysql);
*/





	if( err == NO_ERR ){
		//�`�����l���ύX
		if( this->initONID != -1 && this->initTSID != -1 && this->initSID != -1 ){
			SelectService(this->initONID, this->initTSID, this->initSID);
			this->initONID = -1;
			this->initTSID = -1;
			this->initSID = -1;
			Sleep(this->initChgWait);
		}else{
			int sel = this->combService.GetCurSel();
			if( sel != CB_ERR ){
				DWORD index = (DWORD)this->combService.GetItemData(sel);
				SelectService(this->serviceList[index].originalNetworkID, this->serviceList[index].transportStreamID, this->serviceList[index].serviceID, this->serviceList[index].space, this->serviceList[index].ch );
			}
		}
	}

	//�E�C���h�E�̕���
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

	
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CMediaPortal_BonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CMediaPortal_BonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CMediaPortal_BonDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
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

	// TODO:  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	return 0;
}


void CMediaPortal_BonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( nID == SC_CLOSE ){
		if( this->main.IsRec() == TRUE ){
			if( AfxMessageBox( L"�^�撆�ł����I�����܂����H", MB_YESNO ) == IDNO ){
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
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
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
	int sel = this->combService.GetCurSel();
	if( sel != CB_ERR ){
		DWORD index = (DWORD)this->combService.GetItemData(sel);
		selONID = this->serviceList[index].originalNetworkID;
		selTSID = this->serviceList[index].transportStreamID;
		selSID = this->serviceList[index].serviceID;
	}

	strAdd.Format(L"%d", selONID );
	WritePrivateProfileString(L"SET", L"LastONID", strAdd.GetBuffer(0), this->moduleIniPath);
	strAdd.Format(L"%d", selTSID );
	WritePrivateProfileString(L"SET", L"LastTSID", strAdd.GetBuffer(0), this->moduleIniPath);
	strAdd.Format(L"%d", selSID );
	WritePrivateProfileString(L"SET", L"LastSID", strAdd.GetBuffer(0), this->moduleIniPath);

	WritePrivateProfileString(L"SET", L"LastBon", bon.GetBuffer(0), this->moduleIniPath);

	CDialogEx::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}


void CMediaPortal_BonDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
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
					udp = "UDP���M�F";
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
					tcp = "TCP���M�F";
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
/*
				// tv.log�Ď�
				DWORD err = 0;
				FILE *fp;
				TCHAR str[1024];
				if(this->mpServiceStat == 0){
					
					if((err = _tfopen_s(&fp,this->mpLogPath, L"r, ccs=UTF-8")) != 0 ) {
						this->log.Format(L"tv.log�I�[�v���G���[\r\n");
					} else {
						fseek(fp,0,SEEK_END); 
						fgetpos(fp,&this->mpNowLogSz); // ���݃t�@�C���T�C�Y�擾

						if(this->mpPreLogSz == 0 ){ // �����t�@�C���T�C�Y�擾
							this->mpPreLogSz = this->mpNowLogSz;
						}

						// ���݂̃t�@�C���T�C�Y���O������������ꍇ�͑O����[���ɂ���B
						if(this->mpPreLogSz > this->mpNowLogSz) this->mpPreLogSz = 0;

						if(this->mpPreLogSz < this->mpNowLogSz){
							fsetpos(fp, &this->mpPreLogSz); // �V�[�N
							this->log = L"";
							//this->log.Format(L"������%I64d����%I64d\r\n",this->mpPreLogSz,this->mpNowLogSz);
							while ( fgetws(str, 1024, fp) != NULL ) { // �ǉ�����\��
								this->log += str;
								this->log += L"\r\n";
							}
							this->mpPreLogSz = this->mpNowLogSz; // �t�@�C���T�C�Y����
						}

						fclose(fp);
					}
				}
				SetDlgItemText(IDC_EDIT_LOG, this->log);
*/

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
					this->log.Format(L"%s (%d/%d �c��� %d �b)\r\n", chName.c_str(), chkNum, totalNum, (totalNum - chkNum)*10);
					SetDlgItemText(IDC_EDIT_LOG, this->log);
					SetTimer(TIMER_CHSCAN_STATSU, 1000, NULL);
				}else if( status == ST_CANCEL ){
					KillTimer(TIMER_CHSCAN_STATSU);
					this->log = L"�L�����Z������܂���\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
				}else if( status == ST_COMPLETE ){
					KillTimer(TIMER_CHSCAN_STATSU);
					this->log = L"�I�����܂���\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
					ReloadServiceList();
					int sel = this->combService.GetCurSel();
					if( sel != CB_ERR ){
						DWORD index = (DWORD)this->combService.GetItemData(sel);
						SelectService(this->serviceList[index].originalNetworkID, this->serviceList[index].transportStreamID, this->serviceList[index].serviceID, this->serviceList[index].space, this->serviceList[index].ch );
					}
					BtnUpdate(GUI_NORMAL);
					ChgIconStatus();

					//�����T�[�r�X���ʂ̕����`�����l���ɂ��邩�`�F�b�N
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
						wstring log = L"����T�[�r�X�������̕����`�����l���Ō��o����܂����B\r\n��M���̂悢�����`�����l���̃T�[�r�X�̂ݎc���悤�ɐݒ���s���Ă��������B\r\n����ɘ^��ł��Ȃ��\�����o�Ă��܂��B\r\n\r\n";
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
					int sel = this->combService.GetCurSel();
					if( sel != CB_ERR ){
						DWORD index = (DWORD)this->combService.GetItemData(sel);
						if( info.ONID != this->serviceList[index].originalNetworkID ||
							info.TSID != this->serviceList[index].transportStreamID ||
							info.SID != this->serviceList[index].serviceID ){
						}
						this->initONID = info.ONID;
						this->initTSID = info.TSID;
						this->initSID = info.SID;
						ReloadServiceList();
						this->main.SetSID(info.SID);
					}

					this->log = L"EPG�擾��\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
					SetTimer(TIMER_EPGCAP_STATSU, 1000, NULL);
				}else if( status == ST_CANCEL ){
					KillTimer(TIMER_EPGCAP_STATSU);
					this->log = L"�L�����Z������܂���\r\n";
					SetDlgItemText(IDC_EDIT_LOG, this->log);
				}else if( status == ST_COMPLETE ){
					KillTimer(TIMER_EPGCAP_STATSU);
					this->log = L"�I�����܂���\r\n";
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
				CString strBuff2=L"";
				this->combService.GetWindowText(strBuff2);
				buff.Format(L"%s �F %s", bonFile.c_str(), strBuff2.GetBuffer(0));

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

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	if( nType == SIZE_MINIMIZED && this->minTask == TRUE){
		CString buff=L"";
		wstring bonFile = L"";
		this->main.GetOpenBonDriver(&bonFile);
		CString strBuff2=L"";
		this->combService.GetWindowText(strBuff2);
		buff.Format(L"%s �F %s", bonFile.c_str(), strBuff2.GetBuffer(0));

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
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
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
			ReloadServiceList();
			ChgIconStatus();
		}
		break;
	case WM_TRAY_PUSHICON:
		{
			//�^�X�N�g���C�֌W
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
		CString strBuff2=L"";
		this->combService.GetWindowText(strBuff2);
		buff.Format(L"%s �F %s", bonFile.c_str(), strBuff2.GetBuffer(0));

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
		CString strBuff2=L"";
		this->combService.GetWindowText(strBuff2);
		buff.Format(L"%s �F %s", bonFile.c_str(), strBuff2.GetBuffer(0));

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
			this->combService.EnableWindow(TRUE);
			this->btnChScan.EnableWindow(TRUE);
			this->btnSet.EnableWindow(TRUE);
			this->btnCancel.EnableWindow(FALSE);
			break;
		case GUI_CANCEL_ONLY:
			this->combTuner.EnableWindow(FALSE);
			this->combService.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_OPEN_FAIL:
			this->combTuner.EnableWindow(TRUE);
			this->combService.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(TRUE);
			this->btnCancel.EnableWindow(FALSE);
			break;
		case GUI_REC:
			this->combTuner.EnableWindow(FALSE);
			this->combService.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_REC_SET_TIME:
			this->combTuner.EnableWindow(FALSE);
			this->combService.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_OTHER_CTRL:
			this->combTuner.EnableWindow(FALSE);
			this->combService.EnableWindow(FALSE);
			this->btnChScan.EnableWindow(FALSE);
			this->btnSet.EnableWindow(FALSE);
			this->btnCancel.EnableWindow(TRUE);
			break;
		case GUI_REC_STANDBY:
			this->combTuner.EnableWindow(FALSE);
			this->combService.EnableWindow(FALSE);
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
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	KillTimer(TIMER_STATUS_UPDATE);
	CString buff=L"";
	this->combTuner.GetWindowText(buff);

	if( buff.IsEmpty() == false ){
		SelectBonDriver(buff.GetBuffer(0));

		int sel = this->combService.GetCurSel();
		if( sel != CB_ERR ){
			DWORD index = (DWORD)this->combService.GetItemData(sel);
			SelectService(this->serviceList[index].originalNetworkID, this->serviceList[index].transportStreamID, this->serviceList[index].serviceID, this->serviceList[index].space, this->serviceList[index].ch );
		}
	}
	ChgIconStatus();
	SetTimer(TIMER_STATUS_UPDATE, 1000, NULL);
}


void CMediaPortal_BonDlg::OnCbnSelchangeComboService()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int sel = this->combService.GetCurSel();
	if( sel != CB_ERR ){
		DWORD index = (DWORD)this->combService.GetItemData(sel);
		SelectService(this->serviceList[index].originalNetworkID, this->serviceList[index].transportStreamID, this->serviceList[index].serviceID, this->serviceList[index].space, this->serviceList[index].ch );
	}
	ChgIconStatus();
}


void CMediaPortal_BonDlg::OnBnClickedButtonSet()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
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
		ReloadServiceList();
		
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

void CMediaPortal_BonDlg::ReloadServiceList(BOOL ini)
{
	this->serviceList.clear();
	this->combService.ResetContent();

	DWORD ret = this->main.GetServiceList(&this->serviceList);
	if( ret != NO_ERR || this->serviceList.size() == 0 ){
		this->log += L"�`�����l�����̓ǂݍ��݂Ɏ��s���܂���\r\n";
		SetDlgItemText(IDC_EDIT_LOG, this->log);
	}else{
		int selectSel = 0;
		for( size_t i=0; i<this->serviceList.size(); i++ ){
			if( this->serviceList[i].useViewFlag == TRUE ){
				int index = this->combService.AddString(this->serviceList[i].serviceName.c_str());
				this->combService.SetItemData(index, (DWORD)i);
				if( this->serviceList[i].originalNetworkID == this->initONID &&
					this->serviceList[i].transportStreamID == this->initTSID &&
					this->serviceList[i].serviceID == this->initSID ){
						if( ini == FALSE ){
							this->initONID = -1;
							this->initTSID = -1;
							this->initSID = -1;
						}
						selectSel = index;
				}
			}
		}
		if( this->combService.GetCount() > 0 ){
			this->combService.SetCurSel(selectSel);
		}

	}

}

DWORD CMediaPortal_BonDlg::SelectBonDriver(LPCWSTR fileName, BOOL ini)
{
	this->main.CloseBonDriver();
	DWORD err = this->main.OpenBonDriver(fileName);
	if( err != NO_ERR ){
		this->log.Format(L"BonDriver�̃I�[�v�����ł��܂���ł���\r\n%s\r\n", fileName);
		SetDlgItemText(IDC_EDIT_LOG, this->log);
		BtnUpdate(GUI_OPEN_FAIL);
	}else{
		this->log = L"";
		SetDlgItemText(IDC_EDIT_LOG, this->log);
		BtnUpdate(GUI_NORMAL);
	}
	ReloadServiceList(ini);
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
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	if( this->main.StartChScan() != NO_ERR ){
		this->log.Format(L"�`�����l���X�L�������J�n�ł��܂���ł���\r\n");
		SetDlgItemText(IDC_EDIT_LOG, this->log);
		return;
	}
	SetTimer(TIMER_CHSCAN_STATSU, 1000, NULL);
	BtnUpdate(GUI_CANCEL_ONLY);
}

void CMediaPortal_BonDlg::OnBnClickedButtonCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	if( this->main.IsRec() == TRUE ){
		if( AfxMessageBox( L"�^����~���܂����H", MB_YESNO ) == IDNO ){
			return ;
		}
	}
	this->log = L"�L�����Z������܂���\r\n";
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

	// TODO:  �����ɓ���ȃN�G���̏I���Z�b�V���� �R�[�h��ǉ����Ă��������B
	if( this->main.IsRec() == TRUE ){
		ShowWindow(SW_SHOW);
		return FALSE;
	}
	return TRUE;
}


void CMediaPortal_BonDlg::OnEndSession(BOOL bEnding)
{
	CDialogEx::OnEndSession(bEnding);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	if( bEnding == TRUE ){
		if( this->main.IsRec() == TRUE ){
			this->main.StopReserveRec();
			this->main.StopRec();
		}
	}
}
