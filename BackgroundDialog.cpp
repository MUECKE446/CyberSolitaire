// BackgroundDialog.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "BackgroundDialog.h"


// CBackgroundDialog-Dialogfeld

IMPLEMENT_DYNAMIC(CBackgroundDialog, CDialog)
CBackgroundDialog::CBackgroundDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBackgroundDialog::IDD, pParent)
{
}

CBackgroundDialog::~CBackgroundDialog()
{
}

void CBackgroundDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BMP_CARDSBACKGROUND, m_BmpCardBackgroundCtrl);
	DDX_Control(pDX, IDC_BMP_GAME_BACKGROUND, m_BmpGameBackgroundCtrl);
	DDX_Control(pDX, IDC_CARDS_BACKGROUND, m_SpinCardBackgroundCtrl);
	DDX_Control(pDX, IDC_GAME_BACKGROUND, m_SpinGameBackgroundCtrl);
}


BEGIN_MESSAGE_MAP(CBackgroundDialog, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CARDS_BACKGROUND, OnDeltaposCardsBackground)
	ON_NOTIFY(UDN_DELTAPOS, IDC_GAME_BACKGROUND, OnDeltaposGameBackground)
END_MESSAGE_MAP()


// CBackgroundDialog-Meldungshandler

void CBackgroundDialog::OnDeltaposCardsBackground(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	//NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_IDCardBackground = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (m_IDCardBackground < IDB_BACK1)
		m_IDCardBackground = IDB_BACK14;
	if (m_IDCardBackground > IDB_BACK14)
		m_IDCardBackground = IDB_BACK1;
	m_BmpCardBackground.Detach();
	m_BmpCardBackground.LoadBitmap(m_IDCardBackground);
	m_BmpCardBackgroundCtrl.SetBitmap((HBITMAP)m_BmpCardBackground);	
	*pResult = 0;
}

void CBackgroundDialog::OnDeltaposGameBackground(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	//NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_IDGameBackground = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (m_IDGameBackground < IDB_BACKGROUND_START)
		m_IDGameBackground = IDB_BACKGROUND_END;
	if (m_IDGameBackground > IDB_BACKGROUND_END)
		m_IDGameBackground = IDB_BACKGROUND_START;
	m_BmpGameBackground.Detach();
	m_BmpGameBackground.LoadBitmap(m_IDGameBackground);
	m_BmpGameBackgroundCtrl.SetBitmap((HBITMAP)m_BmpGameBackground);	
	*pResult = 0;
}

BOOL CBackgroundDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// für Test
	//m_IDCardBackground = IDB_BACK1;
	//m_IDGameBackground = IDB_BACKGROUND_START;

	m_SpinCardBackgroundCtrl.SetRange(IDB_BACK1,IDB_BACK14);
	m_SpinCardBackgroundCtrl.SetPos(m_IDCardBackground);
	m_BmpCardBackground.LoadBitmap(m_IDCardBackground);
	m_BmpCardBackgroundCtrl.SetBitmap((HBITMAP)m_BmpCardBackground);	
	
	m_SpinGameBackgroundCtrl.SetRange(IDB_BACKGROUND_START,IDB_BACKGROUND_END);
	m_SpinGameBackgroundCtrl.SetPos(m_IDGameBackground);
	m_BmpGameBackground.LoadBitmap(m_IDGameBackground);
	m_BmpGameBackgroundCtrl.SetBitmap((HBITMAP)m_BmpGameBackground);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}
