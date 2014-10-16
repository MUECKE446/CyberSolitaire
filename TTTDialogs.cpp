// TTTDialogs.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "TTTDialogs.h"
#include ".\tttdialogs.h"


// CTTTChooseGameDialog-Dialogfeld

IMPLEMENT_DYNAMIC(CTTTChooseGameDialog, CDialog)
CTTTChooseGameDialog::CTTTChooseGameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTTTChooseGameDialog::IDD, pParent)
	, m_ChoosedGame(1)
	, m_Game1(_T(""))
	, m_Game2(_T(""))
	, m_Game3(_T(""))
	, m_ChoosedGameName(_T(""))
	, m_strNeededScore1(_T(""))
	, m_strNeededScore2(_T(""))
	, m_strNeededScore3(_T(""))
{
}

CTTTChooseGameDialog::~CTTTChooseGameDialog()
{
}

void CTTTChooseGameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEEDED_SCORE1, m_NeededScore1);
	DDX_Control(pDX, IDC_NEEDED_SCORE2, m_NeededScore2);
	DDX_Control(pDX, IDC_NEEDED_SCORE3, m_NeededScore3);
}


BEGIN_MESSAGE_MAP(CTTTChooseGameDialog, CDialog)
	ON_BN_CLICKED(IDC_RADIO_GAME1, OnBnClickedRadioGame1)
	ON_BN_CLICKED(IDC_RADIO_GAME2, OnBnClickedRadioGame2)
	ON_BN_CLICKED(IDC_RADIO_GAME3, OnBnClickedRadioGame3)
	ON_BN_CLICKED(IDC_DESCRIPTION, OnBnClickedDescription)
	ON_BN_CLICKED(IDC_DESCRIPTION_SHORT, OnBnClickedDescriptionShort)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTTTChooseGameDialog-Meldungshandler

void CTTTChooseGameDialog::OnBnClickedRadioGame1()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	m_ChoosedGame = 1;
}

void CTTTChooseGameDialog::OnBnClickedRadioGame2()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	m_ChoosedGame = 2;
}

void CTTTChooseGameDialog::OnBnClickedRadioGame3()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	m_ChoosedGame = 3;
}

BOOL CTTTChooseGameDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  hier zusätzliche Initialisierung hinzufügen.
	m_ChoosedGame = 1;
	m_pRadioButton = (CButton*)GetDlgItem(IDC_RADIO_GAME1); 
	m_pRadioButton->SetCheck(BST_CHECKED);
	// zu_tun
	// die Games anzeigen
	GetDlgItem(IDC_RADIO_GAME1)->SetWindowText(m_Game1);
	GetDlgItem(IDC_RADIO_GAME2)->SetWindowText(m_Game2);
	GetDlgItem(IDC_RADIO_GAME3)->SetWindowText(m_Game3);
	m_NeededScore1.SetWindowText(m_strNeededScore1);
	m_NeededScore2.SetWindowText(m_strNeededScore2);
	m_NeededScore3.SetWindowText(m_strNeededScore3);

	return TRUE;  // return TRUE unless you set the focus to a control
	// AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

void CTTTChooseGameDialog::OnBnClickedDescription()
{
	CString HelpKey;
	switch (m_ChoosedGame)
	{
	case 1:
		HelpKey = m_Game1;
		break;
	case 2:
		HelpKey = m_Game2;
		break;
	case 3:
		HelpKey = m_Game3;
		break;
	default:
		ASSERT(0);
	}
	GetApplication()->WinHelp((DWORD)((LPCSTR)HelpKey),HELP_KEY);	
}

void CTTTChooseGameDialog::OnBnClickedDescriptionShort()
{
	CString HelpKey;
	switch (m_ChoosedGame)
	{
	case 1:
		HelpKey = m_Game1;
		break;
	case 2:
		HelpKey = m_Game2;
		break;
	case 3:
		HelpKey = m_Game3;
		break;
	default:
		ASSERT(0);
	}
	CString TmpString;
	TmpString.LoadString(IDS_SHORTDESCRIPTOR);
	HelpKey += TmpString;
	GetApplication()->WinHelp((DWORD)((LPCSTR)HelpKey),HELP_KEY);	
}

void CTTTChooseGameDialog::OnBnClickedOk()
{
	OnOK();
	switch (m_ChoosedGame)
	{
	case 1:
		m_ChoosedGameName = m_Game1;
		break;
	case 2:
		m_ChoosedGameName = m_Game2;
		break;
	case 3:
		m_ChoosedGameName = m_Game3;
		break;
	default:
		ASSERT(0);
	}
}
