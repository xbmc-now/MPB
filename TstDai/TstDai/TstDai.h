
// TstDai.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#include "TstDaiMain.h"

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CTstDaiApp:
// ���̃N���X�̎����ɂ��ẮATstDai.cpp ���Q�Ƃ��Ă��������B
//

class CTstDaiApp : public CWinApp
{
public:
	CTstDaiApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CTstDaiApp theApp;