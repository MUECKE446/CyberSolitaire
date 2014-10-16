// CardToolTip.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "CardToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardToolTip

CCardToolTip::CCardToolTip()
{
}

CCardToolTip::~CCardToolTip()
{
	if (m_hWnd) Activate(FALSE);
	if (m_hWnd) DelTool(m_pWnd,(UINT) m_pWnd);
}


BEGIN_MESSAGE_MAP(CCardToolTip, CToolTipCtrl)
	//{{AFX_MSG_MAP(CCardToolTip)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CCardToolTip 

CCardToolTip::CCardToolTip(CWnd* pWnd, CDC* pDC, CPile *pPile, CCard *pCard)
{
	ASSERT(pWnd);
	ASSERT(pPile);
	ASSERT(pCard);
	CRect rr = pCard->GetVisibleRect();
	pDC->LPtoDP(&rr);
	m_pPile = pPile;	
	m_pCard = pCard;
	m_pWnd = pWnd;
	m_bShowing = FALSE;
	m_ToolTipText.Format("%s",pCard->GetCardName());
	Create( pWnd, TTS_ALWAYSTIP|TTS_NOPREFIX );
	AddTool( pWnd, LPSTR_TEXTCALLBACK, &rr, (UINT) pWnd );
	Activate( TRUE );
	TRACE("CreateToolTip:%s\n",m_ToolTipText);
	::SendMessage( m_hWnd, TTM_SETDELAYTIME, TTDT_INITIAL, MAKELONG(750,0) );
	::SendMessage( m_hWnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELONG(1000,0) );
	::SendMessage( m_hWnd, TTM_SETDELAYTIME, TTDT_RESHOW, MAKELONG(1000,0) );
	/*
	SendMessage( TTM_SETDELAYTIME, TTDT_INITIAL, 750 );
	SendMessage( TTM_SETDELAYTIME, TTDT_AUTOPOP, 1000 );
	SendMessage( TTM_SETDELAYTIME, TTDT_RESHOW, 1000 );
	*/
}

void CCardToolTip::Show( BOOL bShow)
{
	m_pCard->SetFaceUp(bShow);
	m_bShowing = bShow;
	//TRACE("%s\tface changed to %d\n",m_pCard->GetCardName(),m_pCard->GetFaceUp());
}

