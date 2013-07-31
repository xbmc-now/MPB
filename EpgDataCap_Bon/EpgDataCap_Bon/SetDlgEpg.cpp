// SetDlgEpg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "EpgDataCap_Bon.h"
#include "SetDlgEpg.h"
#include "afxdialogex.h"

#include "../../Common/PathUtil.h"
// CSetDlgEpg �_�C�A���O

IMPLEMENT_DYNAMIC(CSetDlgEpg, CDialog)

CSetDlgEpg::CSetDlgEpg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlgEpg::IDD, pParent)
{

}

CSetDlgEpg::~CSetDlgEpg()
{
}

void CSetDlgEpg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICE, serviceList);
	DDX_Control(pDX, IDC_CHECK_BS, btnBS);
	DDX_Control(pDX, IDC_CHECK_CS1, btnCS1);
	DDX_Control(pDX, IDC_CHECK_CS2, btnCS2);
}


BEGIN_MESSAGE_MAP(CSetDlgEpg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALL_CHK, &CSetDlgEpg::OnBnClickedButtonAllChk)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO_CHK, &CSetDlgEpg::OnBnClickedButtonVideoChk)
	ON_BN_CLICKED(IDC_BUTTON_ALL_CLEAR, &CSetDlgEpg::OnBnClickedButtonAllClear)
END_MESSAGE_MAP()


// CSetDlgEpg ���b�Z�[�W �n���h���[


BOOL CSetDlgEpg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	btnBS.SetCheck( GetPrivateProfileInt( L"SET", L"BSBasicOnly", 1, commonIniPath ) );
	btnCS1.SetCheck( GetPrivateProfileInt( L"SET", L"CS1BasicOnly", 1, commonIniPath ) );
	btnCS2.SetCheck( GetPrivateProfileInt( L"SET", L"CS2BasicOnly", 1, commonIniPath ) );

	wstring path;
	GetSettingPath(path);
	path += L"\\ChSet5.txt";
	this->chSet.ParseText(path.c_str());

	map<LONGLONG, CH_DATA5>::iterator itr;
	for( itr = this->chSet.chList.begin(); itr != this->chSet.chList.end(); itr++ ){
		int index = this->serviceList.AddString(itr->second.serviceName.c_str());
		this->serviceList.SetCheck(index, itr->second.epgCapFlag);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CSetDlgEpg::SaveIni(void)
{
	UpdateData(TRUE);

	CString val = L"";
	val.Format(L"%d",btnBS.GetCheck());
	WritePrivateProfileString( L"SET", L"BSBasicOnly", val.GetBuffer(0), commonIniPath );
	val.Format(L"%d",btnCS1.GetCheck());
	WritePrivateProfileString( L"SET", L"CS1BasicOnly", val.GetBuffer(0), commonIniPath );
	val.Format(L"%d",btnCS2.GetCheck());
	WritePrivateProfileString( L"SET", L"CS2BasicOnly", val.GetBuffer(0), commonIniPath );

	for( int i=0; i<this->serviceList.GetCount(); i++ ){
		map<LONGLONG, CH_DATA5>::iterator itr;
		itr = this->chSet.chList.begin();
		advance(itr, i);
		this->chSet.SetEpgCapMode(
			itr->second.originalNetworkID,
			itr->second.transportStreamID,
			itr->second.serviceID,
			(BOOL)this->serviceList.GetCheck(i)
			);
	}
	this->chSet.SaveChText();
}

BOOL CSetDlgEpg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam  == VK_RETURN || pMsg->wParam  == VK_ESCAPE ){
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CSetDlgEpg::OnBnClickedButtonAllChk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	for( int i=0; i<this->serviceList.GetCount(); i++ ){
		this->serviceList.SetCheck(i, TRUE);
	}
}


void CSetDlgEpg::OnBnClickedButtonVideoChk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	for( int i=0; i<this->serviceList.GetCount(); i++ ){
		map<LONGLONG, CH_DATA5>::iterator itr;
		itr = this->chSet.chList.begin();
		advance(itr, i);
		if( itr->second.serviceType == 0x01 || itr->second.serviceType == 0xA5 ){
			this->serviceList.SetCheck(i, TRUE);
		}else{
			this->serviceList.SetCheck(i, FALSE);
		}
	}
}


void CSetDlgEpg::OnBnClickedButtonAllClear()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	for( int i=0; i<this->serviceList.GetCount(); i++ ){
		this->serviceList.SetCheck(i, FALSE);
	}
}
