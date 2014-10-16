// InputLevelPointsDialog.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "InputLevelPointsDialog.h"
#include ".\inputlevelpointsdialog.h"


// CInputLevelPointsDialog-Dialogfeld

IMPLEMENT_DYNAMIC(CInputLevelPointsDialog, CDialog)
CInputLevelPointsDialog::CInputLevelPointsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputLevelPointsDialog::IDD, pParent)
	, m_Level1_Points(1)
	, m_Level2_Points(1)
	, m_Level3_Points(1)
	, m_Level4_Points(1)
	, m_Level5_Points(1)
	, m_Level6_Points(1)
	, m_Level7_Points(1)
	, m_Level8_Points(1)
	, m_Level9_Points(1)
{
}

CInputLevelPointsDialog::~CInputLevelPointsDialog()
{
}

void CInputLevelPointsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEVEL1, m_Level1_Points);
	DDV_MinMaxUInt(pDX, m_Level1_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL2, m_Level2_Points);
	DDV_MinMaxUInt(pDX, m_Level2_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL3, m_Level3_Points);
	DDV_MinMaxUInt(pDX, m_Level3_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL4, m_Level4_Points);
	DDV_MinMaxUInt(pDX, m_Level4_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL5, m_Level5_Points);
	DDV_MinMaxUInt(pDX, m_Level5_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL6, m_Level6_Points);
	DDV_MinMaxUInt(pDX, m_Level6_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL7, m_Level7_Points);
	DDV_MinMaxUInt(pDX, m_Level7_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL8, m_Level8_Points);
	DDV_MinMaxUInt(pDX, m_Level8_Points, 1, 104);
	DDX_Text(pDX, IDC_EDIT_LEVEL9, m_Level9_Points);
	DDV_MinMaxUInt(pDX, m_Level9_Points, 1, 104);
}


BEGIN_MESSAGE_MAP(CInputLevelPointsDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CInputLevelPointsDialog-Meldungshandler

void CInputLevelPointsDialog::OnBnClickedOk()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	OnOK();
}
