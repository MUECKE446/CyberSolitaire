// DebugPropertiers.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "DebugProperties.h"


// CDebugPropertiesDialog-Dialogfeld

IMPLEMENT_DYNAMIC(CDebugPropertiesDialog, CDialog)
CDebugPropertiesDialog::CDebugPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugPropertiesDialog::IDD, pParent)
	, m_ClipCursor(FALSE)
	, m_ReportGameLayouts(FALSE)
{
}

CDebugPropertiesDialog::~CDebugPropertiesDialog()
{
}

void CDebugPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CLIPCURSOR, m_ClipCursor);
	DDX_Check(pDX, IDC_REPORTGAMELAYOUTS, m_ReportGameLayouts);
}


BEGIN_MESSAGE_MAP(CDebugPropertiesDialog, CDialog)
END_MESSAGE_MAP()


// CDebugPropertiesDialog-Meldungshandler

