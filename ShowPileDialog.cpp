// ShowPileDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"

#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "ShowPileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CShowPileDialog 


CShowPileDialog::CShowPileDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShowPileDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowPileDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_CheatMode = FALSE;
	m_pPartPiles.SetSize(4);
}

CShowPileDialog::~CShowPileDialog()
{
	int i;
	for(i=0;i<4;i++)
		if (m_pPartPiles[i])
			delete m_pPartPiles[i];
	m_pPartPiles.RemoveAll();
}

void CShowPileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowPileDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowPileDialog, CDialog)
	//{{AFX_MSG_MAP(CShowPileDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CShowPileDialog 

BOOL CShowPileDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString Title;
	Title.Format(IDS_VIEW_PILE,m_pShowPile->m_Cards.GetCount());
	SetWindowText(Title);

	// der zu zeigende Pile wird auf maximal 4 partielle Piles aufgeteilt
	int i;
	for(i=0;i<4;i++)
		if (m_pPartPiles[i])
			delete m_pPartPiles[i];
	m_pPartPiles.RemoveAll();
	m_pPartPiles.SetSize(4);
	for(i=0;i<4;i++)
		m_pPartPiles[i] = new CPile;

	const int CardsPerLine = 26;
	int Cards = 0;
	int yPosition = 0;
	int xPosition = 0;
	CPoint Position(15,15);
	if (!m_pShowPile->m_Cards.IsEmpty())
	{
		CCard* pTmpCard = m_pShowPile->m_Cards.GetHead();
		POSITION pos = m_pShowPile->m_Cards.GetHeadPosition();
		for (i=0;i<4;i++)
		{
			while (pos)
			{
				m_pPartPiles[i]->m_Cards.AddTail(m_pShowPile->m_Cards.GetNext(pos));
				Cards++;
				if (Cards % CardsPerLine == 0)
				{
					// nächster partieller Pile
					break;
				}
			}
			if (!m_pPartPiles[i]->IsEmpty())
			{
				m_pPartPiles[i]->m_KindOfOverlapped = olmRigthOverlapped;
				m_pPartPiles[i]->SetPosition(Position);
				CSize MaxSize(CCyberSolitaireApp::DefaultCardSize);
				int NumberOfCards = m_pPartPiles[i]->GetCount();
				MaxSize.cx = int(CCyberSolitaireApp::DefaultOverlappedFactor * (double)(NumberOfCards-1) 
							* (double)CCyberSolitaireApp::DefaultCardSize.cx
							+ (double)CCyberSolitaireApp::DefaultCardSize.cx);
				m_pPartPiles[i]->SetMaxSize(MaxSize);
				m_pPartPiles[i]->UpdateCardsPositions();
				Position.y += pTmpCard->m_LogicalSize.cy + 15;
			}
		}
	}
	CenterWindow(AfxGetMainWnd());

	// jetzt brauche ich einen Pointer auf den Client-Bereich
	CRect ClientRect;
	this->GetClientRect(ClientRect);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CShowPileDialog::OnPaint() 
{
	int i;
	CPaintDC dc(this); // device context for painting
	// zeichne die partiellen Piles

	CRect PileRect(ZeroRect);
	CCard* pCard = NULL;
	for (i=0;i<4;i++)
	{
		if (m_pPartPiles[i]->IsEmpty() && (i==0))
		{
			// leeren Pile zeichnen
			m_pPartPiles[i]->m_dibEmpty.Draw((CDC&)dc);
		}
		else
		{
			POSITION pos = m_pPartPiles[i]->m_Cards.GetHeadPosition();
			while (pos)
			{
				pCard = m_pPartPiles[i]->m_Cards.GetNext(pos);
				// Umrechnung log. -> Bitmapkoordinaten
				CSize devVisibleSize((int)(pCard->m_LogicalVisibleSize.cx/pCard->m_xFac),(int)(pCard->m_LogicalVisibleSize.cy/pCard->m_yFac));
				CRect	devCardRect(ZeroPoint,devVisibleSize);

				CPoint Position(pCard->m_LogicalPosition);
				Position.x = (int)(Position.x/pCard->m_xFac);
				Position.y = (int)(Position.y/pCard->m_yFac);
				CRect	CardRect(Position,devVisibleSize);
				if (pCard->GetFaceUp() || m_CheatMode)
				{
					pCard->m_dibPicture.Draw((CDC&)dc,&CardRect,&devCardRect);
					if (!pCard->GetFaceUp() && m_CheatMode)
						((CCyberSolitaireView*)m_pParent)->m_dibCardSelected.Draw((CDC&)dc,&CardRect,&devCardRect,FALSE,SRCAND);
				}
				else
					GetApplication()->m_dibCardBack.Draw((CDC&)dc,&CardRect,&devCardRect);
				PileRect.UnionRect(PileRect,CardRect);
			}
			
		}
	}
	CRect ClientRect;
	GetClientRect(ClientRect);
	/*
	PileRect.top = (int)(PileRect.top/m_pShowPile->m_Cards.GetHead()->m_yFac);
	PileRect.bottom = (int)(PileRect.bottom/m_pShowPile->m_Cards.GetHead()->m_yFac);
	PileRect.left = (int)(PileRect.left/m_pShowPile->m_Cards.GetHead()->m_xFac);
	PileRect.right = (int)(PileRect.right/m_pShowPile->m_Cards.GetHead()->m_xFac);
	*/
	PileRect.InflateRect(10,10);
	CalcWindowRect(PileRect);
	ClientToScreen(PileRect);
	MoveWindow(PileRect);
	CenterWindow(AfxGetMainWnd());
	// Kein Aufruf von CDialog::OnPaint() für Zeichnungsnachrichten
}
