// SetDlgBasic.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "EpgDataCap_Bon.h"
#include "SetDlgBasic.h"
#include "afxdialogex.h"

#include "../../Common/PathUtil.h"
#include "../../Common/StringUtil.h"

// CSetDlgBasic �_�C�A���O

IMPLEMENT_DYNAMIC(CSetDlgBasic, CDialog)

CSetDlgBasic::CSetDlgBasic(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlgBasic::IDD, pParent)
	, settingFolderPath(_T(""))
	, recFolderPath(_T(""))
{

}

CSetDlgBasic::~CSetDlgBasic()
{
}

void CSetDlgBasic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SET_PATH, settingFolderPath);
	DDX_Text(pDX, IDC_EDIT_REC_FOLDER, recFolderPath);
	DDX_Control(pDX, IDC_LIST_REC_FOLDER, recFolderList);
}


BEGIN_MESSAGE_MAP(CSetDlgBasic, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REC_PATH, &CSetDlgBasic::OnBnClickedButtonRecPath)
	ON_BN_CLICKED(IDC_BUTTON_REC_ADD, &CSetDlgBasic::OnBnClickedButtonRecAdd)
	ON_BN_CLICKED(IDC_BUTTON_REC_DEL, &CSetDlgBasic::OnBnClickedButtonRecDel)
	ON_BN_CLICKED(IDC_BUTTON_REC_UP, &CSetDlgBasic::OnBnClickedButtonRecUp)
	ON_BN_CLICKED(IDC_BUTTON_REC_DOWN, &CSetDlgBasic::OnBnClickedButtonRecDown)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH, &CSetDlgBasic::OnBnClickedButtonSetPath)
END_MESSAGE_MAP()


// CSetDlgBasic ���b�Z�[�W �n���h���[
BOOL CSetDlgBasic::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	wstring path;
	GetSettingPath(path);
	settingFolderPath = path.c_str();

	int iNum = GetPrivateProfileInt( L"SET", L"RecFolderNum", 0, commonIniPath );
	if( iNum == 0 ){
		GetDefSettingPath(path);
		recFolderList.AddString( path.c_str() );
	}else{
		for( int i = 0; i < iNum; i++ ){
			CString key = L"";
			key.Format(L"RecFolderPath%d", i );
			WCHAR buff[512]=L"";
			GetPrivateProfileString( L"SET", key, L"", buff, 512, commonIniPath );
			recFolderList.AddString( buff );
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CSetDlgBasic::SaveIni()
{
	UpdateData(TRUE);
	WritePrivateProfileString(L"SET", L"DataSavePath", settingFolderPath.GetBuffer(0), commonIniPath);
	_CreateDirectory(settingFolderPath);

	int iNum = recFolderList.GetCount();
	CString strVal;
	strVal.Format( L"%d", iNum );
	WritePrivateProfileString(L"SET", L"RecFolderNum", strVal.GetBuffer(0), commonIniPath);
	for( int i = 0; i < iNum; i++ ){
		CString strKey = L"";
		strKey.Format(L"RecFolderPath%d", i );
		CString strFolder = L"";
		recFolderList.GetText( i, strFolder );
		WritePrivateProfileString(L"SET", strKey, strFolder, commonIniPath);
	}

	UpdateData(FALSE);
}

void CSetDlgBasic::OnBnClickedButtonRecPath()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
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
	if( recFolderPath.IsEmpty() != 0 ){
		if (!SUCCEEDED(SHGetSpecialFolderLocation( m_hWnd,CSIDL_DESKTOP,&pidlRoot ) )){ 
			lpMalloc->Free(lpBuffer);
			return;
		}
	}

	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = lpBuffer;
	bi.lpszTitle = L"�^��t�@�C���ۑ��t�H���_��I�����Ă�������";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = (LPARAM)recFolderPath.GetBuffer(0);

	pidlBrowse = SHBrowseForFolder(&bi);
	if (pidlBrowse != NULL) {  
		if (SHGetPathFromIDList(pidlBrowse, lpBuffer)) {
			recFolderPath = lpBuffer;
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


void CSetDlgBasic::OnBnClickedButtonRecAdd()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	if( recFolderPath.IsEmpty() == true ){
		return ;
	}
	wstring addPath = recFolderPath.GetBuffer(0);
	ChkFolderPath( addPath );

	//����t�H���_�����łɂ��邩�`�F�b�N
	int iNum = recFolderList.GetCount();
	BOOL findFlag = FALSE;
	for( int i = 0; i < iNum; i++ ){
		CString folder = L"";
		recFolderList.GetText( i, folder );
		wstring strPath = folder.GetBuffer(0);
		ChkFolderPath( strPath );

		if( CompareNoCase( addPath, strPath ) == 0 ){
			findFlag = TRUE;
			break;
		}
	}
	if( findFlag == FALSE ){
		recFolderList.AddString( addPath.c_str() );
	}

	UpdateData(FALSE);
}


void CSetDlgBasic::OnBnClickedButtonRecDel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int sel = recFolderList.GetCurSel();
	if( sel == LB_ERR ){
		return ;
	}
	recFolderList.DeleteString( sel );
}


void CSetDlgBasic::OnBnClickedButtonRecUp()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int sel = recFolderList.GetCurSel();
	if( sel == LB_ERR || sel == 0){
		return ;
	}
	CString folder = L"";
	recFolderList.GetText( sel, folder );
	recFolderList.DeleteString( sel );
	recFolderList.InsertString( sel-1, folder );
	recFolderList.SetCurSel( sel-1 );
}


void CSetDlgBasic::OnBnClickedButtonRecDown()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int sel = recFolderList.GetCurSel();
	if( sel == LB_ERR || sel == recFolderList.GetCount() - 1 ){
		return ;
	}
	CString folder = L"";
	recFolderList.GetText( sel, folder );
	recFolderList.DeleteString( sel );
	recFolderList.InsertString( sel+1, folder );
	recFolderList.SetCurSel( sel+1 );
}


void CSetDlgBasic::OnBnClickedButtonSetPath()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
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
	bi.lpszTitle = L"�ݒ�֌W�ۑ��t�H���_��I�����Ă�������";
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
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam  == VK_RETURN || pMsg->wParam  == VK_ESCAPE ){
			return FALSE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

