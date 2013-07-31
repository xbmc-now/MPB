#include "StdAfx.h"
#include "CmdLineUtil.h"

CCmdLineUtil::CCmdLineUtil(void)
{
	m_strOpt = L"";
	m_strOpt2 = L"";
}

CCmdLineUtil::~CCmdLineUtil(void)
{
}

void CCmdLineUtil::ParseParam( const TCHAR* pszParam, BOOL bFlag, BOOL bLast )
{
	if( bFlag == TRUE ){
		if( m_strOpt.IsEmpty() == false ){
			m_CmdList.insert(pair<CString, CString>(m_strOpt, m_strOpt2));
		}
		m_strOpt = L"";
		m_strOpt2 = L"";
	}
	if( bFlag == TRUE ){
		m_strOpt = pszParam;
	}else{
		if( m_strOpt2.IsEmpty() == true ){
			m_strOpt2 = pszParam;
		}
	}
	if( bLast == TRUE ){
		m_CmdList.insert(pair<CString, CString>(m_strOpt, m_strOpt2));
	}
}


