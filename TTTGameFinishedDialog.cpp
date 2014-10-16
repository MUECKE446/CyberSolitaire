// TTTGameFinishedDialog.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "CyberSolitaireDoc.h"
#include "TTTGameFinishedDialog.h"
#include ".\tttgamefinisheddialog.h"


// CTTTGameFinishedDialog-Dialogfeld

IMPLEMENT_DYNAMIC(CTTTGameFinishedDialog, CDialog)
CTTTGameFinishedDialog::CTTTGameFinishedDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTTTGameFinishedDialog::IDD, pParent)
	, m_GameName(_T(""))
	, m_ActualScore(0)
	, m_NeededScore(0)
	, m_Result(_T(""))
{
	m_pDoc = NULL;
}

CTTTGameFinishedDialog::~CTTTGameFinishedDialog()
{
}

void CTTTGameFinishedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GAME_NAME, m_GameName);
	DDX_Text(pDX, IDC_ACTUAL_SCORE, m_ActualScore);
	DDX_Text(pDX, IDC_NEEDED_SCORE, m_NeededScore);
	DDX_Text(pDX, IDC_GAME_RESULT, m_Result);
}


BEGIN_MESSAGE_MAP(CTTTGameFinishedDialog, CDialog)
END_MESSAGE_MAP()


// CTTTGameFinishedDialog-Meldungshandler

BOOL CTTTGameFinishedDialog::OnInitDialog()
{
	ASSERT(m_pDoc);
	if (m_pDoc->WinsTTTGame())
		m_Result.LoadString(IDS_GAME_WIN);
	else
		m_Result.LoadString(IDS_GAME_LOOSE);
	CDialog::OnInitDialog();

	// TODO:  hier zusätzliche Initialisierung hinzufügen.

	return TRUE;  // return TRUE unless you set the focus to a control
	// AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}
