// CyberSolitaireView.cpp : Implementierung der Klasse CCyberSolitaireView
//

#include "stdafx.h"
#include "CyberSolitaire.h"

#include "MainFrm.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "Do.h"
#include "ShowPileDialog.h"
#include ".\cybersolitaireview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// zu_tun
static clock_t	avetime = 0;
static clock_t	sumtime = 0;
static long		loops = 0;

/////////////////////////////////////////////////////////////////////////////
// Macros

#define PREPARE_CYBERSOLITAIRE_VIEW	CCyberSolitaireDoc* pDoc = GetDocument();\
								CClientDC dc(this);\
								OnPrepareDC(&dc);\
								CPoint LogPoint(point);\
								dc.DPtoLP(&LogPoint);\

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireView

IMPLEMENT_DYNCREATE(CCyberSolitaireView, CView)

BEGIN_MESSAGE_MAP(CCyberSolitaireView, CView)
	//{{AFX_MSG_MAP(CCyberSolitaireView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SHOW_PILE, OnShowPile)
	ON_COMMAND(ID_SHOW_PILE_CHEATMODE, OnShowPileCheatmode)
	ON_COMMAND(ID_SHOW_CARD_CHEAT, OnShowCardCheat)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTip)
	ON_NOTIFY_EX( TTN_POP, 0, OnToolTipPop)
	ON_NOTIFY_EX( TTN_SHOW, 0,OnToolTipShow)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_PILECONTEXT_KARTEUMDREHEN, OnChangeCardFaceUp)
	ON_COMMAND(ID_AUTOPLAY, OnAutoplay)
	ON_UPDATE_COMMAND_UI(ID_AUTOPLAY, OnUpdateAutoplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireView Konstruktion/Destruktion

CCyberSolitaireView::CCyberSolitaireView()
{
	//VERIFY(m_BackGround.Load("Splsh16.bmp"));
	//VERIFY(m_Screen.Load("Splsh16.bmp"));
	//m_BackGround.Load(AfxGetApp()->m_hInstance,IDB_BACKGROUND1);
	//m_Screen.Load(AfxGetApp()->m_hInstance,IDB_BACKGROUND1);
	//m_BackGround.Load(AfxGetApp()->m_hInstance,IDB_BACKGROUND2);
	//m_Screen.Load(AfxGetApp()->m_hInstance,IDB_BACKGROUND2);
	m_BackGround.Load(AfxGetApp()->m_hInstance,GetApplication()->m_BackgroundID);
	m_Screen.Load(AfxGetApp()->m_hInstance,GetApplication()->m_BackgroundID);
	m_dibCardSelected.Load(AfxGetApp()->m_hInstance,IDB_SELECTEDMASK);
	m_pScreen = NULL;
	m_pBackground = NULL;
	m_VisibleRect.top = 0;
	m_VisibleRect.left = 0;
	m_VisibleRect.right = TABLE_CX; 
	m_VisibleRect.bottom = TABLE_CY;
	m_CardsMoving = FALSE;
	m_pSelectedPile = NULL;
	m_pSelectedCard = NULL;
	m_pSecondSelectedPile = NULL;
	m_pSecondSelectedCard = NULL;
	m_UpdateRect.SetRectEmpty();
	m_pClickedCard = NULL;
	m_ShowCardCheated = FALSE;
	m_pCardToolTip = NULL;
	m_TTTGameMenu.LoadMenu( IDR_TTT_CYBSOL_GAME );
	m_GameMenu.LoadMenu( IDR_CYBSOLTYPE );

}


CCyberSolitaireView::~CCyberSolitaireView()
{
	// zu_tun
	TRACE("loops   = %d\n",loops);
	TRACE("sumtime = %d\n",sumtime);
	TRACE("avetime = %d\n",avetime);
	ResetToolTip();
	if (m_pScreen)
		delete m_pScreen;
	if (m_pBackground)
		delete m_pBackground;
	TRACE("Destruct CyberSolitaireView\n");
}

BOOL CCyberSolitaireView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireView Zeichnen

void CCyberSolitaireView::OnDraw(CDC* pDC)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect UpdateRect =((CPaintDC*)pDC)->m_ps.rcPaint; 
	CRect UpdateRect1(UpdateRect);
	pDC->DPtoLP((CRect*)&UpdateRect);
	if (m_pScreen)
		m_pScreen->Draw(*pDC,&UpdateRect,&UpdateRect1,FALSE);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);
	//pDC->Rectangle(m_VisibleRect);
	//if (m_CardsMoving)
	//	TRACE("OnDraw CardsMoving\n");

}

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireView Drucken

BOOL CCyberSolitaireView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CCyberSolitaireView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CCyberSolitaireView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einfügen
}

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireView Diagnose

#ifdef _DEBUG
void CCyberSolitaireView::AssertValid() const
{
	CView::AssertValid();
}

void CCyberSolitaireView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCyberSolitaireDoc* CCyberSolitaireView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCyberSolitaireDoc)));
	return (CCyberSolitaireDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireView Nachrichten-Handler

void CCyberSolitaireView::OnInitialUpdate() 
{
	CCyberSolitaireDoc* pDoc = GetDocument();
  
	CView::OnInitialUpdate();

	// Enable ToolTips
	EnableToolTips( TRUE );

	// View pointer besetzen
	pDoc->m_pView = this;	
	//pDoc->UpdatePiles();	// ???
	// initiaisieren des UpdateRect
	m_UpdateRect.SetRectEmpty();

	UpdateBitmaps();
	pDoc->ComputeScore();
	OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
}

void CCyberSolitaireView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	pDC->SetMapMode(MM_ISOTROPIC);
    m_VisibleRect.NormalizeRect();
	GetClientRect(&m_DeviceRect);
    m_DeviceRect.NormalizeRect();

	pDC->SetWindowOrg(m_VisibleRect.left,m_VisibleRect.top);
	pDC->SetWindowExt(m_VisibleRect.right - m_VisibleRect.left,m_VisibleRect.bottom - m_VisibleRect.top);
	
	pDC->SetViewportOrg(m_DeviceRect.left,m_DeviceRect.top);
	if(!m_DeviceRect.IsRectEmpty())
		pDC->SetViewportExt(m_DeviceRect.right - m_DeviceRect.left,m_DeviceRect.bottom - m_DeviceRect.top);
}

void CCyberSolitaireView::OnSize(UINT nType, int cx, int cy) 
{
	//TRACE("CCyberSolitaireView::OnSize\n");
	//ShowWindow(SW_HIDE);
	CWaitCursor wait;
	//CView::OnSize(nType, cx, cy);
	// eventuell vorhandene Screen und Background Bitmaps löschen
	Delete(m_pScreen);
	Delete(m_pBackground);

	CClientDC dc(this);
	OnPrepareDC(&dc);
	CRect ClientRect; GetClientRect(&ClientRect);
	
	// Screen und Background Dib mit der aktuellen Größe erzeugen
	m_pScreen = new CDib;
	m_pScreen->Create(dc,ClientRect.Size(),&m_BackGround);
	m_pScreen->m_TransPoints.RemoveAll();
	m_pBackground = new CDib;
	m_pBackground->Create(dc,ClientRect.Size(),&m_BackGround);
	m_pBackground->m_TransPoints.RemoveAll();
	SetDibs(&GetApplication()->m_dibCardBack);
	if (IsMainWndSizing())
		return;
	else
	{
		UpdateBitmaps();
		OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
	}
}


void CCyberSolitaireView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// zu_tun
	clock_t start, finish, difftime;
	start = clock();
	CCyberSolitaireDoc* pDoc = GetDocument();
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPile* pPile;
	POSITION pos;
	switch (lHint) {
    case HINT_DISPLAY_ALL_PILES:
		// sofern es überhaupt Piles gibt
		if(!pDoc->m_PileList.IsEmpty())
		{
			// das zu löschende Rechteck ist die Vereinigung aller Pile Rects
			m_UpdateRect.SetRectEmpty();
			pos = pDoc->m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = pDoc->m_PileList.GetNext(pos);
				//CRect PileRect(pPile->GetPosition(),pPile->GetSize());
				CRect PileRect(pPile->GetPosition(),pPile->GetMaxSize());
				if (m_UpdateRect.IsRectNull())
					m_UpdateRect = PileRect;
				else
					m_UpdateRect.UnionRect(m_UpdateRect,PileRect);
			}
			m_UpdateRect.InflateRect(2,2);
			dc.LPtoDP(&m_UpdateRect);
			m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
			// alle Pile des Documents zeichnen (in die Screen Bitmap)
			pos = pDoc->m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = pDoc->m_PileList.GetNext(pos);
				pPile->Display(&dc,m_Dibs);
			}
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		}
		break;
	case HINT_DISPLAY_PILE:
		{
			pPile = (CPile*)pHint;
			if (!pPile)
				return;
			// zu_tun wieder raus
			// so könnte es für halbe Piles aussehen
			//CRect PileRect(pPile->GetPosition(),pPile->GetSize());
			CRect PileRect(pPile->GetPosition(),pPile->GetMaxSize());
			m_UpdateRect = PileRect;
			m_UpdateRect.InflateRect(2,2);
			dc.LPtoDP(&m_UpdateRect);
			m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
			// Pile des Documents zeichnen (in die Screen Bitmap)
			pPile->Display(&dc,m_Dibs);
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		}
		break;
    case HINT_DISPLAY_MOVEPILE:
	case HINT_UPDATE_PILES:
		{
			// sofern es überhaupt Piles gibt
			if(!pDoc->m_PileList.IsEmpty())
			{
				CPileList	PileList;
				pos = pDoc->m_PileList.GetHeadPosition();
				while (pos)
				{
					pPile = pDoc->m_PileList.GetNext(pos);
					CRect PileRect(pPile->GetPosition(),pPile->GetSize());
					CRect TmpRect(0,0,0,0);
					PileRect.NormalizeRect();
					m_UpdateRect.NormalizeRect();
					TmpRect.IntersectRect(&PileRect,&m_UpdateRect);
					if (!TmpRect.IsRectEmpty())
					{
						//m_UpdateRect.UnionRect(m_UpdateRect,PileRect);
						PileList.AddTail(pPile);
						//TRACE("Get pile %p\n",pPile);
					}
				}
				m_UpdateRect.NormalizeRect();
				if (!m_UpdateRect.IsRectEmpty())
				{
					m_UpdateRect.InflateRect(2,2);
					//if (!this->IsK(CMovePile))
					//	TRACE("(%d,%d,%d,%d)\n",m_UpdateRect.left,m_UpdateRect.top,m_UpdateRect.right,m_UpdateRect.bottom);
					dc.LPtoDP(&m_UpdateRect);
					m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
				}
				// alle betroffenen Piles zeichnen (in die Screen Bitmap)
				pos = PileList.GetHeadPosition();
				while (pos)
				{
					pPile = PileList.GetNext(pos);
					pPile->Display(&dc,m_Dibs);
					//TRACE("Display pile %p\n",pPile);
				}
			}
			pDoc->m_pMovePile->Display(&dc,m_Dibs);
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
			m_UpdateRect.SetRectEmpty();

		}
		break;
    case HINT_DISPLAY_ALL:
		GetClientRect(m_UpdateRect);
		m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect);
		// alle Pile des Documents zeichnen (in die Screen Bitmap)
		pos = pDoc->m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = pDoc->m_PileList.GetNext(pos);
			pPile->Display(&dc,m_Dibs);
		}
		RedrawWindow(NULL,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		break;
	default:
		Invalidate();
		//TRACE("OnUpdate");
		break;
	}
	m_UpdateRect.SetRectEmpty();
	// zu_tun
	loops++;
	finish = clock();
	difftime = finish - start;
	sumtime += difftime;
	avetime = sumtime / loops;

}

void CCyberSolitaireView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//TRACE("OnLButtonDown\n");
	PREPARE_CYBERSOLITAIRE_VIEW

	if (pDoc->m_ExitGameCause == EXIT_CAUSE_WINS)
	{
		pDoc->HandleTheEndOfGame();
		return;
	}

	// zunächst muß festgestellt werden, ob bzw. welcher Pile angeklickt wurde
	CPile* pClickedPile = NULL;
	pClickedPile = HitPileTest(LogPoint); 
	
	// wenn in die Landschaft oder auf den zuvor selektierten Pile geklickt wurde
	if (!pClickedPile || (pClickedPile && m_pSelectedPile && (pClickedPile == m_pSelectedPile))) 
	{
		// eine bestehende Selektion wird dann aufgehoben
		if (m_pSelectedPile)
		{
			// deselect
			m_pSelectedPile->SelectCards(NULL,TRUE);
			OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pSelectedPile);
			m_pSelectedPile = NULL;
			m_pSelectedCard = NULL;
			m_pSecondSelectedPile = NULL;
			m_pSecondSelectedCard = NULL;
			//pDoc->PlaySound("gallop.wav",NULL,SND_ASYNC | SND_FILENAME);
			pDoc->PlaySound("gallop.wav",NULL,SND_SYNC | SND_FILENAME);
		}
		m_LastPoint = LogPoint;
		return;
	}
	
	// wird eventuell durch Klicken verschoben
	if (!m_CardsMoving && m_pSelectedPile && (pClickedPile != m_pSelectedPile) &&
		(m_pSelectedPile->m_KindOfSelecting != selPairsEqualValue) &&
		(m_pSelectedPile->m_KindOfSelecting != selPairsValue13))
	{
		//TRACE("PutSelectedPileToMovePile from (%d,%d) to (%d,%d)\n",
		//	m_pSelectedCard->m_LogicalPosition.x,m_pSelectedCard->m_LogicalPosition.y,LogPoint.x,LogPoint.y);
		PutSelectedPileToMovePile(m_pSelectedCard->m_LogicalPosition);			
		//pDoc->m_pMovePile->Move(m_pSelectedCard->m_LogicalPosition,LogPoint,m_UpdateRect);
		CPoint ToPosition;
		if (pClickedPile->IsEmpty())
			ToPosition = pClickedPile->GetPosition();
		else
			ToPosition = pClickedPile->m_Cards.GetTail()->m_LogicalPosition;

		pDoc->m_pMovePile->Move(m_pSelectedCard->m_LogicalPosition,ToPosition,m_UpdateRect);
		OnUpdate(NULL,HINT_DISPLAY_MOVEPILE,NULL);
		m_LastPoint = LogPoint;
		return;
	}

	// ein Pile wurde angeklickt
	//pDoc->PlaySound("Select.wav",NULL,SND_ASYNC | SND_FILENAME);
	pDoc->PlaySound("Select.wav",NULL,SND_SYNC | SND_FILENAME);
	
	/////////////////////////////////////////////////////////////////////////////
	// Überprüfung der Selektion von Karten entsprechend der Vorgaben des Kartenstapels
	// bei Rückgabe von TRUE wird die weitere Verarbeitung des Mausklicks beendet
	CCard* pCard = NULL;
	m_pClickedCard = NULL;
	BOOL RetWithErr = FALSE;
	
	// soll eine Selektion durchgeführt werden ?
	// das Selektieren von Karten ist bei allen Arten von Piles bekannt
	if (pClickedPile->m_KindOfSelecting != selNotAvailable)
	{
		if (SelectCards(pClickedPile,LogPoint,RetWithErr))
		{
			if (RetWithErr) RETURN_WITH_ERROR;
			m_LastPoint = LogPoint;
			return;
		}
	}
	

	/////////////////////////////////////////////////////////////////////////////
	// Sollen neue Karten gegeben werden (bisher nur für CStock bekannt)

	///////////////////// CStock ///////////////////////////////////////////////
	if (pClickedPile->IsK(CStock))
	{
		// zu_tun
		// wenn leer Redeal untersuchen
		BOOL Redeal = FALSE;
		if (pClickedPile->IsEmpty())
		{
			if (!TestRedeal())
				RETURN_WITH_ERROR;
			Redeal = TRUE;
		}
		DealCards(pDoc->GetStock(),Redeal);
		ReportGameStatus();
		pDoc->TestEmptyPiles();
		//TRACE("Dealt\n");
	}
	m_LastPoint = LogPoint;
	//TRACE("Left Mouse Button down");
}

void CCyberSolitaireView::ReportUserError()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	//pDoc->PlaySound("Incorrec.wav",NULL,SND_ASYNC | SND_FILENAME);
	pDoc->PlaySound("Incorrec.wav",NULL,SND_SYNC | SND_FILENAME);
	GetApplication()->LogMessage("MOVE CANCELED BY USER ERROR");
}


void CCyberSolitaireView::OnMouseMove(UINT nFlags, CPoint point) 
{
	//TRACE("OnMouseMove\n");
	PREPARE_CYBERSOLITAIRE_VIEW

	if (pDoc->m_ExitGameCause == EXIT_CAUSE_WINS)
	{
		pDoc->HandleTheEndOfGame();
		return;
	}

	// Nichts tun wenn keine Bewegung über Gridpunkt hinweg
	// könnte das moven beschleunigen
	if ( abs(LogPoint.x - m_LastPoint.x) < GRID && abs (LogPoint.y - m_LastPoint.y) < GRID )
	{
		return;
	}

	// ist der linke Mousbutton gedrückt
	if (nFlags & MK_LBUTTON)
	{
		if (!m_CardsMoving)
		{
			// wir müssen eventuell mit einem Verschieben beginnen
			if (!m_pSelectedPile)
				return;	// keine Karten selektiert
			else
			{
				if (m_pClickedCard == m_pSelectedCard)
				{
					//TRACE("move started\n");
					PutSelectedPileToMovePile(m_pSelectedCard->m_LogicalPosition);			
					pDoc->m_pMovePile->Move(m_LastPoint,LogPoint,m_UpdateRect);
					//pDoc->m_pMovePile->Move(pDoc->m_pMovePile->GetPosition(),LogPoint,m_UpdateRect);
					OnUpdate(NULL,HINT_DISPLAY_MOVEPILE,NULL);
				}
			}
		}
		else
		{
			// wir sind mitten beim Verschieben
			if (m_LastPoint != LogPoint)
			{
				pDoc->m_pMovePile->Move(m_LastPoint,LogPoint,m_UpdateRect);
				//TRACE("MouseMove move\n");
				OnUpdate(NULL,HINT_DISPLAY_MOVEPILE,NULL);
			}
		}
	}
	else
	{
		BOOL DummyErr = FALSE;
		CCard* pCard = NULL;
		CPile* pPile = NULL;;
		CString msg(_T(""));
		if ((pPile = HitPileTest(LogPoint)) && (pCard = pPile->GetClickedCard(LogPoint,DummyErr)) &&
			(pPile->IsLastInvisibleCard(pCard))) 
		{
			//TRACE("IsLastInvisible\n");
			if (!m_pCardToolTip || 
				(m_pCardToolTip && !m_pCardToolTip->GetShowing() && (m_pCardToolTip->m_pCard != pCard)))
			{
				if (ResetToolTip())
					m_pCardToolTip = new CCardToolTip(this,&dc,pPile,pCard);
			}
		}
	}
	m_LastPoint = LogPoint;
	//TRACE("MouseMove\n");
}

void CCyberSolitaireView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//TRACE("OnLButtonUp\n");
	PREPARE_CYBERSOLITAIRE_VIEW

	if (pDoc->m_ExitGameCause == EXIT_CAUSE_WINS)
	{
		pDoc->HandleTheEndOfGame();
		return;
	}

	ASSERT(!pDoc->m_PileList.IsEmpty());
	//pDoc->UpdatePiles();
	CPile* pClickedPile = NULL;
	BOOL	Found = FALSE;
	POSITION pos;

	//pClickedPile = HitPileTest(LogPoint);

	// wird gerade verschoben ?
	if (m_CardsMoving)
	{
		//TRACE("Try to end moving\n");
		// Cursor wieder freigeben
		ClipCursorToView(&dc,TRUE);
		ResetToolTip();
		// alle Pile des Documents durchsuchen
		pos = pDoc->m_PileList.GetHeadPosition();
		while (pos)
		{
			pClickedPile = pDoc->m_PileList.GetNext(pos);
			Found =  pClickedPile->HitTest(LogPoint) || pClickedPile->HitTest(pDoc->m_pMovePile);
			//Found |= pClickedPile->HitTest(pDoc->m_pMovePile);
			if (Found)
				break;
		}

		// wenn in die Landschaft oder auf den zuvor selektierten Pile geklickt wurde
		if (!pClickedPile || (pClickedPile && m_pSelectedPile && (pClickedPile == m_pSelectedPile))) 
		{
			PutMovePileBack();
			RETURN_WITH_ERROR;
		}
		// ein Pile wurde gefunden
		BOOL RetWithErr = FALSE;
		if (PutToTableauOrWaste(pClickedPile,RetWithErr))
		{
			if (RetWithErr) RETURN_WITH_ERROR;
			// falls ein leerer Piles entstanden ist, testen ob damit etwas zu geschehen hat
			pDoc->TestEmptyPiles();
			pDoc->ComputeScore();
			ReportGameDifferenceStatus();
			return;
		}
		
		if (PutToFoundation(pClickedPile,RetWithErr))
		{
			if (RetWithErr) RETURN_WITH_ERROR;
			// falls ein leerer Piles entstanden ist, testen ob damit etwas zu geschehen hat
			pDoc->TestEmptyPiles();
			pDoc->ComputeScore();
			ReportGameDifferenceStatus();
			return;
		}
		
		
		if (pClickedPile->IsK(CStock))
		{
			PutMovePileBack();
			RETURN_WITH_ERROR;
		}

		// falls ein leerer Piles entstanden ist, testen ob damit etwas zu geschehen hat
		ReportGameDifferenceStatus();
		pDoc->TestEmptyPiles();
	}
	else
	{
		BOOL RetWithErr = FALSE;
		// werden vielleicht Paare abgelegt
		if (m_pSelectedCard && m_pSecondSelectedCard)
		{
			PutBothSelectedPilesToMovePile();
			if (PutToFoundation((CPile*)pDoc->GetFoundation(),RetWithErr))
			{
				if (RetWithErr) RETURN_WITH_ERROR;
				// falls ein leerer Piles entstanden ist, testen ob damit etwas zu geschehen hat
				pDoc->TestEmptyPiles();
				pDoc->m_pDo->CreateDo(END,NULL,NULL,NULL,NULL);
				pDoc->ComputeScore();
				ReportGameDifferenceStatus();
				return;
			}
		}
	}
	pDoc->ComputeScore();
	m_LastPoint = LogPoint;
	//CView::OnLButtonUp(nFlags, point);
}


/////////////////////////////////////////////////////////////////////////////
// löscht eine CDib, falls vorhanden
//
void CCyberSolitaireView::Delete(CDib *pDib)
{
	if (pDib)
	{
		pDib->DeleteObject();
		delete pDib;
		pDib = NULL;
	}
}



/////////////////////////////////////////////////////////////////////////////
// tut den MovePile zurück
//
void CCyberSolitaireView::PutMovePileBack()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	// MovePile zurück packen
	// falls die letzte Karte umgedreht war, rückgängig machen !!!
	pDoc->m_pMovePile->Put(m_pSelectedPile,m_pSelectedCard,m_UpdateRect);
	m_pSelectedPile->SelectCards(NULL,TRUE);
	m_pSelectedPile = NULL;
	m_pSelectedCard = NULL;
	m_pSecondSelectedPile = NULL;
	m_pSecondSelectedCard = NULL;
	m_CardsMoving = FALSE;
	OnUpdate(NULL,HINT_UPDATE_PILES,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// hängt den MovePile an pPile
//
void CCyberSolitaireView::PutMovePileToPile(CPile *pPile)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	CCard* pFirstMovePileCard = pDoc->m_pMovePile->m_Cards.GetHead();
	// MovePile dem Pile hinzugeben
	pDoc->m_pMovePile->Put(pPile,pFirstMovePileCard,m_UpdateRect);
	pPile->SelectCards(pFirstMovePileCard,TRUE);
	// nur dann hat sich was geändert
	if (pPile != m_pSelectedPile)
	{
		// was ist mit der letzten Karte -> umdrehen, falls nicht sichtbar
		BOOL ChangeLastCardFaceUp = FALSE;
		if (!m_pSelectedPile->IsEmpty())
		{
			if (!m_pSelectedPile->m_Cards.GetTail()->GetFaceUp())
			{
				// die letzte Karte wird sichtbar gesetzt
				ChangeLastCardFaceUp = TRUE;
				m_pSelectedPile->m_Cards.GetTail()->SetFaceUp(TRUE);
			}
			CRect PileRect(m_pSelectedPile->GetPosition(),m_pSelectedPile->GetSize());
			m_UpdateRect.UnionRect(m_UpdateRect,PileRect);
		}
		pDoc->m_pDo->CreateDo(GET,m_pSelectedPile,pPile,m_pSelectedCard,ChangeLastCardFaceUp);
	}

	m_pSelectedPile = NULL;
	m_pSelectedCard = NULL;
	m_pSecondSelectedPile = NULL;
	m_pSecondSelectedCard = NULL;
	m_CardsMoving = FALSE;
	/*
	if (!pPile->IsK(CFoundation))
		pDoc->PlaySound("put.wav",NULL,SND_SYNC | SND_FILENAME);
	*/
	//TRACE("PutMovePileToPile\n");
	// zu_tun
	// ist es nötig hier alles neu zu zeichnen ???
	OnUpdate(NULL,HINT_UPDATE_PILES,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// tut den MovePile zurück
//
void CCyberSolitaireView::PutMovePilePairBack()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	// umgekehrte Reihenfolge als beim Reintun
	pDoc->m_pMovePile->Put(m_pSecondSelectedPile,m_pSecondSelectedCard,m_UpdateRect);
	pDoc->m_pMovePile->Put(m_pSelectedPile,m_pSelectedCard,m_UpdateRect);
	m_pSelectedPile->SelectCards(NULL,TRUE);
	m_pSelectedPile = NULL;
	m_pSelectedCard = NULL;
	m_pSecondSelectedPile->SelectCards(NULL,TRUE);
	m_pSecondSelectedPile = NULL;
	m_pSecondSelectedCard = NULL;
	m_CardsMoving = FALSE;
	OnUpdate(NULL,HINT_UPDATE_PILES,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// hängt den MovePile an pPile
//
void CCyberSolitaireView::PutMovePilePairToPile(CPile *pPile)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	// MovePile dem Pile hinzugeben
	pDoc->m_pMovePile->Put(pPile,m_pSecondSelectedCard,m_UpdateRect);
	pDoc->m_pDo->CreateDo(GET,m_pSecondSelectedPile,pPile,m_pSecondSelectedCard,FALSE);
	pDoc->m_pMovePile->Put(pPile,m_pSelectedCard,m_UpdateRect);
	pDoc->m_pDo->CreateDo(GET,m_pSelectedPile,pPile,m_pSelectedCard,FALSE);
	pPile->SelectCards(m_pSecondSelectedCard,TRUE);

	m_pSelectedPile = NULL;
	m_pSelectedCard = NULL;
	m_pSecondSelectedPile = NULL;
	m_pSecondSelectedCard = NULL;
	m_CardsMoving = FALSE;
	//pDoc->PlaySound("put.wav",NULL,SND_SYNC | SND_FILENAME);
	//TRACE("PutMovePilePairToPile\n");
	// zu_tun
	// ist es nötig hier alles neu zu zeichnen ???
	OnUpdate(NULL,HINT_UPDATE_PILES,NULL);
}

void CCyberSolitaireView::ClipCursorToView(CDC *pDC, BOOL Release /* = FALSE */)
{
	CRect TmpRect;
	if (!Release)
	{
		GetClientRect(&TmpRect);
		MapWindowPoints(NULL,&TmpRect);
		ClipCursor(&TmpRect);
		SetCapture ();
	}
	else
	{
		ClipCursor (NULL);
		ReleaseCapture();
	}
}

void CCyberSolitaireView::PutSelectedPileToMovePile(CPoint LogPoint)
{
	// den MovePile füllen
	CCyberSolitaireDoc* pDoc = GetDocument();
	CClientDC dc(this);
	OnPrepareDC(&dc);
	// verifizieren, daß der MovePile leer ist
	ASSERT(pDoc->m_pMovePile->m_Cards.IsEmpty());
	pDoc->m_pMovePile->SetPosition(LogPoint);
	pDoc->m_pMovePile->m_KindOfOverlapped = m_pSelectedPile->m_KindOfOverlapped;
	// Karten übertragen
	if ((m_pSelectedPile->m_KindOfSelecting == selExactOne) && (m_pSelectedCard != m_pSelectedPile->m_Cards.GetTail()))
	{
		// wir haben den ganz speziellen Fall, daß aus der Mitte raus genommen werden muß
		CMovePile* pTmpPile = new CMovePile(*pDoc->m_pMovePile);
		CCard* pNextCard;
		POSITION pos = m_pSelectedPile->m_Cards.Find(m_pSelectedCard);
		pNextCard = m_pSelectedPile->m_Cards.GetNext(pos);
		pNextCard = m_pSelectedPile->m_Cards.GetNext(pos);
		pTmpPile->Get(m_pSelectedPile,pNextCard,m_UpdateRect);
		pDoc->m_pMovePile->Get(m_pSelectedPile,m_pSelectedCard,m_UpdateRect);
		m_pSelectedPile->Get(pTmpPile,pNextCard,m_UpdateRect);
		delete pTmpPile;
	}
	else
		pDoc->m_pMovePile->Get(m_pSelectedPile,m_pSelectedCard,m_UpdateRect);
	m_CardsMoving = TRUE;
	OnUpdate(NULL,HINT_DISPLAY_MOVEPILE,NULL);
	if (GetApplication()->m_ClipCursor)
		ClipCursorToView(&dc);
}

void CCyberSolitaireView::PutBothSelectedPilesToMovePile()
{
	// den MovePile füllen
	CCyberSolitaireDoc* pDoc = GetDocument();
	// verifizieren, daß der MovePile leer ist
	ASSERT(pDoc->m_pMovePile->m_Cards.IsEmpty());
	// Karten übertragen
	pDoc->m_pMovePile->Get(m_pSelectedPile,m_pSelectedCard,m_UpdateRect);
	pDoc->m_pMovePile->Get(m_pSecondSelectedPile,m_pSecondSelectedCard,m_UpdateRect);
}

CPile* CCyberSolitaireView::HitPileTest(CPoint LogPoint)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	if (pDoc->m_PileList.IsEmpty()) return NULL;
	CPile* pTmpPile = NULL;
	POSITION pos;
	// alle Pile des Documents durchsuchen
	pos = pDoc->m_PileList.GetHeadPosition();
	while (pos)
	{
		pTmpPile = pDoc->m_PileList.GetNext(pos);
		if (pTmpPile->HitTest(LogPoint))
			return pTmpPile;
	}
	return NULL;
}

void CCyberSolitaireView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_ShowCardCheated) return;
	ScreenToClient(&point);
	PREPARE_CYBERSOLITAIRE_VIEW

	// zunächst muß festgestellt werden, ob bzw. welcher Pile angeklickt wurde
	if (!(m_pShowPile = HitPileTest(LogPoint))) return;
	// ein Pile wurde angeklickt
	if (m_pShowPile->IsEmpty()) return;

	m_pClickedCard = NULL;
	// finde die Karte, die angeklickt wurde
	switch (m_pShowPile->m_KindOfOverlapped)
	{
	case olmNotOverlapped:
		// es muß die letzte(oberste) Karte sein
		m_pClickedCard = m_pShowPile->m_Cards.GetTail();
		break;
	case olmDownOverlapped:
	case olmRigthOverlapped:
		m_pClickedCard = m_pShowPile->GetCard(LogPoint);
		break;
	default:
		break;
	}
	dc.LPtoDP (&LogPoint);
	ClientToScreen(&LogPoint);
	CMenu Menu;
	Menu.LoadMenu(IDR_PILECONTEXT);
	Menu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON ,LogPoint.x,LogPoint.y,AfxGetMainWnd());

}

void CCyberSolitaireView::OnShowPile() 
{
	CShowPileDialog	ShowPileDialog(this);
	ASSERT(m_pShowPile);
	ShowPileDialog.m_pShowPile = new CPile(*m_pShowPile);
	ShowPileDialog.m_CheatMode = FALSE;
	ShowPileDialog.DoModal();
	delete ShowPileDialog.m_pShowPile;
	// jetzt müssen wir die Karten wieder richtig hinlegen
	m_pShowPile->UpdateCardsPositions();
}


BOOL CCyberSolitaireView::UpdateBitmaps()
{
	//TRACE("CCyberSolitaireView::UpdateBitmaps started\n");
	CPoint point(ZeroPoint);
	PREPARE_CYBERSOLITAIRE_VIEW

	m_pScreen->m_TransPoints.RemoveAll();
	m_pBackground->m_TransPoints.RemoveAll();
	// die Arrays mit den Transparentpunkten der Karten aufbauen
	CCard* pFirstCard = NULL;
	if (pDoc->m_Cards.GetSize() !=0 )
	{
		// Karten Dib erzeugen
		CDib* pCardDib = new CDib;
		CCard* pCard = pDoc->m_Cards[1];
		CSize CardSize(pCard->m_LogicalSize);
		CRect	logCardRect(ZeroPoint,CardSize);
		dc.LPtoDP(&CardSize);
		CRect	CardRect(ZeroPoint,CardSize);
		pCardDib->Create(dc,CardSize,&pDoc->m_Cards[1]->m_dibPicture);
		CRect	devCardRect(ZeroPoint,pCard->m_PixelSize);
		pCardDib->CopyBitmap(dc,&pDoc->m_Cards[1]->m_dibPicture,&CardRect,&devCardRect);
		// erzeuge eine Liste aller transparenten Punkte
		// Create memory DC
		CDC memDC;
		if (!memDC.CreateCompatibleDC(&dc)) 
		{
			TRACE0("Unable to create compatible DC's\n");
			return FALSE;
		}
		CBitmap* pOldBm = memDC.SelectObject(pCardDib);
		pCardDib->m_TransPoints.RemoveAll();
		// now evaluate the transparency
		COLORREF TransColor = memDC.GetPixel(ZeroPoint);
		int xPos,yPos;
		//for (xPos=logCardRect.left;xPos<=logCardRect.right;xPos++)
		//	for (yPos=logCardRect.top;yPos<=logCardRect.bottom;yPos++)
		for (xPos=CardRect.left;xPos<=CardRect.right;xPos++)
			for (yPos=CardRect.top;yPos<=CardRect.bottom;yPos++)
			{
				// GetPixel benutzt logische Koordinaten nach der Beschreibung
				// nach dem Debuggen sind es allerdings Pixel (Device Koordinaten)
				if (memDC.GetPixel(xPos,yPos) == TransColor)
				{
					CPoint Point(xPos,yPos);
					pCardDib->m_TransPoints.Add(Point);
				}
			}
		memDC.SelectObject(pOldBm);
		pCardDib->CopyTransPoints(&pCard->m_dibPicture);
		Delete(pCardDib);
		// diese Liste der transparenten Punkte gilt für jede Karte im Spiel
		pFirstCard = pCard;
		for (int j=2; j<=pDoc->m_Cards.GetUpperBound();j++)
		{
			pCard = pDoc->m_Cards[j];
			pFirstCard->m_dibPicture.CopyTransPoints(&pCard->m_dibPicture);
		}

		// diese Liste der transparenten Punkte gilt auch für Kartenrücken und selektiert Maske
		pFirstCard->m_dibPicture.CopyTransPoints(&m_dibCardSelected);
		pFirstCard->m_dibPicture.CopyTransPoints(&GetApplication()->m_dibCardBack);

		// jetzt müssen noch alle Piles mit den Bitmaps für leer bearbeitet werden
		CPile* pPile;
		POSITION pos;
		// sofern es überhaupt Piles gibt
		if(!pDoc->m_PileList.IsEmpty())
		{
			pos = pDoc->m_PileList.GetHeadPosition();
			// für alle Piles
			while (pos)
			{
				pPile = pDoc->m_PileList.GetNext(pos);
				if (!pPile->IsK(CMovePile))
				{
					// erst mal alle TransPoints löschen
					pPile->m_dibEmpty.m_TransPoints.RemoveAll();
					// Karten Dib erzeugen
					CDib* pCardDib = new CDib;
					CSize CardSize(CCyberSolitaireApp::DefaultCardSize);
					dc.LPtoDP(&CardSize);
					// CardRect hat jetzt die Größe des Piles, in der er tatsächlich dargestellt wird
					// in dev Koordinaten (Pixel)
					CRect	CardRect(ZeroPoint,CardSize);
					pCardDib->Create(dc,CardSize,m_Dibs.pBackgroundDib); // hiermit wird eine Bitmap derselben Farbtiefe wie
																		 // m_Dibs.pBackgroundDib erzeugt
					CRect	devCardRect(ZeroPoint,pPile->m_dibEmpty.GetSize());
					// jetzt wird die Bitmap des leeren Piles auf diese Größe reduziert
					pCardDib->CopyBitmap(dc,&pPile->m_dibEmpty,&CardRect,&devCardRect);
					// erzeuge eine Liste aller transparenten Punkte
					CDC memDC,backDC;
					if (!memDC.CreateCompatibleDC(&dc) || !backDC.CreateCompatibleDC(&dc)) 
					{
						TRACE0("Unable to create compatible DC's\n");
						return FALSE;
					}
					CBitmap* pOldBm = memDC.SelectObject(pCardDib);
					CBitmap* pOldBackBm = backDC.SelectObject(m_Dibs.pBackgroundDib);
					pCardDib->m_TransPoints.RemoveAll();
					// die Bitmap denkt in Pixeln, deshalb umrechnen LP->DP
					CPoint Position(pPile->GetPosition());
					dc.LPtoDP(&Position);
					// NewCardRect hat dieselbe Größe wie CardRect, liegt aber an der Position, 
					// an der der leere Pile tatsächlich dargestellt wird
					CRect NewCardRect(Position,CardSize);
					// now evaluate the transparency
					// Get/SetPixel arbeitet mit (dev) Pixelkoordinaten, nicht mit logischen wie in Doku steht
					COLORREF TransColor = memDC.GetPixel(ZeroPoint);
					int xPos,yPos;
					for (xPos=CardRect.left;xPos<=CardRect.right;xPos++)
						for (yPos=CardRect.top;yPos<=CardRect.bottom;yPos++)
						{
							if (memDC.GetPixel(xPos,yPos) == TransColor)
							{
								CPoint Point(xPos,yPos);
								pCardDib->m_TransPoints.Add(Point);
								CPoint TransPoint(Point);
								TransPoint += NewCardRect.TopLeft();
								if (NewCardRect.PtInRect(TransPoint))
								{
									COLORREF TmpColor = memDC.GetPixel(Point.x,Point.y);
									memDC.SetPixel(Point.x,Point.y,backDC.GetPixel(TransPoint.x,TransPoint.y));
									TmpColor = memDC.GetPixel(Point.x,Point.y);
								}
							}
						}
					pCardDib->CopyTransPoints(&pPile->m_dibEmpty);
					memDC.SelectObject(pOldBm);
					backDC.SelectObject(pOldBackBm);
					if (pPile->m_pdibEmpty)
					{
						Delete(pPile->m_pdibEmpty);
						pPile->m_pdibEmpty = NULL;
					}
					pPile->m_pdibEmpty = pCardDib;
					pCardDib->CopyTransPoints(pPile->m_pdibEmpty);
				}
			}
		}
	}
	else
	{
		return FALSE;
	}
	//TRACE("CCyberSolitaireView::UpdateBitmaps ended\n");
	return TRUE;

}

void CCyberSolitaireView::OnShowPileCheatmode() 
{
	CShowPileDialog	ShowPileDialog(this);
	ASSERT(m_pShowPile);
	ShowPileDialog.m_pShowPile = new CPile(*m_pShowPile);
	ShowPileDialog.m_CheatMode = TRUE;
	ShowPileDialog.DoModal();
	delete ShowPileDialog.m_pShowPile;
	// jetzt müssen wir die Karten wieder richtig hinlegen
	m_pShowPile->UpdateCardsPositions();
}

BOOL CCyberSolitaireView::IsMainWndSizing()
{
	//TRACE("CCyberSolitaireView::m_MainWndSizing=%d\n",((CMainFrame*)AfxGetMainWnd())->m_MainWndIsSizing);
	return ((CMainFrame*)AfxGetMainWnd())->m_MainWndIsSizing;
}





void CCyberSolitaireView::OnShowCardCheat() 
{
	if(m_ShowCardCheated) return;
	if (!m_pClickedCard) return;
	if (m_pClickedCard->GetFaceUp()) return;
	m_ShowCardCheated = TRUE;
	PROGRAM_FAILURE(!m_pClickedCard->GetFaceUp());
	m_pClickedCard->SetFaceUp(TRUE);
	// zeichne
	OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pShowPile);
	m_Timer = SetTimer(1,1500,0);
}

void CCyberSolitaireView::OnTimer(UINT nIDEvent) 
{
	m_ShowCardCheated = FALSE;
	PROGRAM_FAILURE(m_pClickedCard->GetFaceUp());
	m_pClickedCard->SetFaceUp(FALSE);
	// zeichne
	OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pShowPile);
	KillTimer(m_Timer);
	CView::OnTimer(nIDEvent);
}

void CCyberSolitaireView::SetDibs(CDib* pCardBackDib) 
{
	m_Dibs.pScreenDib = m_pScreen;
	m_Dibs.pBackgroundDib = m_pBackground;
	m_Dibs.pCardBackDib = pCardBackDib;
	m_Dibs.pCardSelectedDib = &m_dibCardSelected;
}


void CCyberSolitaireView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//TRACE("OnLButtonDblClk %p\n",m_pSelectedPile);
	PREPARE_CYBERSOLITAIRE_VIEW

	if (pDoc->m_ExitGameCause == EXIT_CAUSE_WINS)
	{
		pDoc->HandleTheEndOfGame();
		return;
	}

	// zunächst muß festgestellt werden, ob bzw. welcher Pile angeklickt wurde
	CPile* pClickedPile = NULL;
	pClickedPile = HitPileTest(LogPoint); 
	
	CPile* pPile;
	// wenn auf den zuvor selektierten Pile geklickt wurde
	if ((pClickedPile && m_pSelectedPile && (pClickedPile == m_pSelectedPile))) 
	{
		// im Normalfall wurde vorher durch LButtonDown ein Pile selektiert
		// es darf nur die letzte Karte sein oder ein Pile in dem jede beliebige Karte selektiert werden darf
		if (!m_pSelectedPile->IsLastCard(m_pSelectedCard) && (m_pSelectedPile->m_KindOfSelecting != selExactOne))
			RETURN_WITH_ERROR;
		// nun muß untersucht werden, ob die Karte auf einem Foundation 
		// abgelegt werden kann
		if (m_pSelectedPile->IsK(CFoundation))
			RETURN_WITH_ERROR;
		POSITION pos;
		// alle Pile des Documents durchsuchen
		pos = pDoc->m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = pDoc->m_PileList.GetNext(pos);
			if (!pPile->IsK(CFoundation))
				continue;
			if (TestPutToFoundation(pPile))
			{
				//pDoc->PlaySound("Incorrec.wav",NULL,SND_FILENAME);
				CPoint Point = pPile->GetPosition();
				CSize CardSize(CCyberSolitaireApp::DefaultCardSize);
				//CardSize.cx = CardSize.cx / 2;
				//CardSize.cy = CardSize.cy / 2;
				//Point += CardSize;
				Point += CPoint(5,5);
				//dc.LPtoDP(&Point);
				if (!m_CardsMoving)
				{
					PutSelectedPileToMovePile(m_pSelectedCard->m_LogicalPosition);			
					pDoc->m_pMovePile->Move(m_pSelectedCard->m_LogicalPosition,Point,m_UpdateRect);
					OnUpdate(NULL,HINT_DISPLAY_MOVEPILE,NULL);
				}
				//OnMouseMove(MK_LBUTTON,Point);
				dc.LPtoDP(&Point);
				OnLButtonUp(nFlags,Point);
				m_LastPoint = Point;
				break;
				//MessageBeep(-1);
			}
		
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}

BOOL CCyberSolitaireView::TestPutToFoundation(CPile *pPile)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	// es dürfen nur einzelne Karten abgelegt werden
	if (pPile->m_NumberPerMove == 1)
	{

		// mit welcher Karte wird gestartet ?
		if (pPile->IsEmpty())
		{
			// es darf nur eine Karte im MovePile sein
			if (m_pSelectedPile->m_Cards.GetTail() != m_pSelectedCard)
				return FALSE;
			switch (pPile->m_PutIfEmpty)
			{
			case pieNoCard:
			case pieAnyCard:
			case pieFromDealOrAny:
			case pieOnlyFromAutoDeal:
			case pieBasicCard:		
				if (!m_pSelectedCard->IsBasicCard(((CFoundation*)pPile)->m_BasicCardValue,
												 ((CFoundation*)pPile)->m_BasicCardOnlyVal,
												 ((CFoundation*)pPile)->m_BasicCardOnlyColor))
				{
					return FALSE;
				}
				else
				{
					return TRUE;
				}
				break;
			case pieBasicCardMinus1:
			case pieKing:
				return FALSE;
				break;
			case pieAce:
				return m_pSelectedCard->IsAce();
				break;
			default:
				return FALSE;
				break;
			}
		}
		else
		{
			// ist die maximale Zahl von Karten im Pile erreicht ?
			if ((pPile->m_MaxCards != NA) && (pPile->m_Cards.GetCount() >= pPile->m_MaxCards))
				return FALSE;

			CCard* pLastCard = pPile->m_Cards.GetTail();
			BOOL Ok = FALSE;
			CPile* pTmpPile = new CPile(*pPile);
			CMovePile* pRestPile = NULL;
			CCard* pNextCard;
			CRect UpdateRect(0,0,0,0);
			if ((m_pSelectedPile->m_KindOfSelecting == selExactOne) && (m_pSelectedCard != m_pSelectedPile->m_Cards.GetTail()))
			{
				// wir haben den ganz speziellen Fall, daß aus der Mitte raus genommen werden muß
				ASSERT(pDoc->m_pMovePile->IsEmpty());
				pRestPile = new CMovePile(*pDoc->m_pMovePile);
				POSITION pos = m_pSelectedPile->m_Cards.Find(m_pSelectedCard);
				pNextCard = m_pSelectedPile->m_Cards.GetNext(pos);
				pNextCard = m_pSelectedPile->m_Cards.GetNext(pos);
				pRestPile->Get(m_pSelectedPile,pNextCard,m_UpdateRect);
			}
			pTmpPile->Get(m_pSelectedPile,m_pSelectedCard,UpdateRect);
			switch (pPile->m_KindOfPiling)
			{
			case pilDownInSuit:
				// sind die Karten absteigend und in Farbfolge
				Ok = pTmpPile->IsDownInSequenceAndSuit();
				break;
			case pilDownNoColor:
				// sind die Karten absteigend
				Ok = pTmpPile->IsDownInSequence();
				break;
			case pilUpInSuit:
				// sind die Karten aufsteigend und in Farbfolge
				Ok = pTmpPile->IsUpInSequenceAndSuit();
				break;
			case pilDownAlternate:
				// sind die Karten absteigend und alternierend
				Ok = pTmpPile->IsDownInSequenceAndAlternate();
				break;
			case pilUpAlternate:
				// sind die Karten aufsteigend und alternierend
				Ok = pTmpPile->IsUpInSequenceAndAlternate();
				break;
			case pilUpNoColor:
				// sind die Karten aufsteigend
				Ok = pTmpPile->IsUpInSequence();
				break;
			case pilWithGaps:
				// zu_tun
				break;
			case pilDownInSuitRolling:
				// sind die Karten absteigend und in Farbfolge mit Überlauf (As->König)
				Ok = pTmpPile->IsDownInSequenceAndSuitRolling();
				break;
			case pilUpInSuitRolling:
				// sind die Karten absteigend und in Farbfolge mit Überlauf (König->As)
				Ok = pTmpPile->IsUpInSequenceAndSuitRolling();
				break;
			case pilUpDownNoColorRolling:
				// sind die Karten auf- oder absteigend mit Überlauf
				Ok = pLastCard->IsDownInSequenceRolling(m_pSelectedCard) || 
					 pLastCard->IsUpInSequenceRolling(m_pSelectedCard);
				break;
			case pilUpSameColor:
				// sind die Karten aufsteigend und gleiche Farbe wie Basiskarte
				Ok = pTmpPile->IsUpInSequenceSameColor(((CFoundation*)pPile)->m_BasicCardValue);
				break;
			case pilDownSameColor:
				// zu_tun
				Ok = FALSE;
				break;
			case pilUpDiff2InSuit:
				// sind die Karten aufsteigend um 2 auseinander
				Ok = pTmpPile->IsUpInSequenceValueDiff2InSuit(((CFoundation*)pPile)->m_BasicCardValue);
				break;
			default:
				ASSERT(0);
				break;
			}
			pTmpPile->Put(m_pSelectedPile,m_pSelectedCard,UpdateRect);
			delete pTmpPile;
			if (pRestPile)
			{
				m_pSelectedPile->Get(pRestPile,pNextCard,m_UpdateRect);
				delete pRestPile;
			}
			return Ok;
		}
	}
	else
	{
		// was anderes ist icht bekannt
		PROGRAM_FAILURE(pPile->m_NumberPerMove == 13 || pPile->m_NumberPerMove == 2);
		BOOL Ok = FALSE;
		CPile* pTmpPile = new CPile(*pPile);
		if (!pTmpPile->IsEmpty())
		{
			delete pTmpPile;
			return FALSE;
		}
		CMovePile* pRestPile = NULL;
		CRect UpdateRect(0,0,0,0);
		pTmpPile->Get(m_pSelectedPile,m_pSelectedCard,UpdateRect);
		if (pTmpPile->m_Cards.GetCount() != 13)
		{
			// so nich
			pTmpPile->Put(m_pSelectedPile,m_pSelectedCard,UpdateRect);
			delete pTmpPile;
			return FALSE;
		}
		else
		{
			switch (pPile->m_KindOfPiling)
			{
			case pilDownInSuit:
				// sind die Karten absteigend und in Farbfolge
				Ok = pTmpPile->IsDownInSequenceAndSuit();
				break;
			case pilDownNoColor:
				// sind die Karten absteigend
				Ok = pTmpPile->IsDownInSequence();
				break;
			case pilUpInSuit:
				// sind die Karten aufsteigend und in Farbfolge
				Ok = pTmpPile->IsUpInSequenceAndSuit();
				break;
			case pilDownAlternate:
				// sind die Karten absteigend und alternierend
				Ok = pTmpPile->IsDownInSequenceAndAlternate();
				break;
			case pilUpAlternate:
				// sind die Karten aufsteigend und alternierend
				Ok = pTmpPile->IsUpInSequenceAndAlternate();
				break;
			case pilUpNoColor:
				// sind die Karten aufsteigend
				Ok = pTmpPile->IsUpInSequence();
				break;
			case pilWithGaps:
				// zu_tun
				break;
			case pilDownInSuitRolling:
				// sind die Karten absteigend und in Farbfolge mit Überlauf (As->König)
				Ok = pTmpPile->IsDownInSequenceAndSuitRolling();
				break;
			case pilUpInSuitRolling:
				// sind die Karten absteigend und in Farbfolge mit Überlauf (König->As)
				Ok = pTmpPile->IsUpInSequenceAndSuitRolling();
				break;
			case pilUpSameColor:
				// sind die Karten aufsteigend und gleiche Farbe wie Basiskarte
				Ok = pTmpPile->IsUpInSequenceSameColor(((CFoundation*)pPile)->m_BasicCardValue);
				break;
			case pilDownSameColor:
				// zu_tun
				Ok = FALSE;
				break;
			case pilUpDiff2InSuit:
				// sind die Karten aufsteigend um 2 auseinander
				Ok = pTmpPile->IsUpInSequenceValueDiff2InSuit(((CFoundation*)pPile)->m_BasicCardValue);
				break;
			default:
				ASSERT(0);
				break;
			}
			pTmpPile->Put(m_pSelectedPile,m_pSelectedCard,UpdateRect);
			delete pTmpPile;
			return Ok;
		}
	}
	return FALSE;
}


BOOL CCyberSolitaireView::ResetToolTip()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	if ( m_pCardToolTip != NULL ) 
	{
		if (m_pCardToolTip->GetShowing())
			return FALSE;
		//TRACE("ResetToolTip %s\n",m_pCardToolTip->m_pCard->GetCardName());
		//GetApplication()->LogMessage("ResetToolTip %s",m_pCardToolTip->m_pCard->GetCardName());
		delete m_pCardToolTip ;
		m_pCardToolTip = NULL ;
	}
	return TRUE;
}


// Tooltip management
BOOL CCyberSolitaireView::OnToolTip( UINT id, NMHDR *pTTT, LRESULT *pResult )
{
	if (!m_pCardToolTip)
	{
		GetApplication()->LogMessage("OnToolTip with m_pCardToolTip == NULL");
		return FALSE;
	}
	TOOLTIPTEXT *pt = (TOOLTIPTEXT*) pTTT;
	strncpy( pt->szText, m_pCardToolTip->GetText() ,sizeof(pt->szText) );
	pt->lpszText	= pt->szText;
	pt->hinst		= NULL;
	//TRACE("OnToolTip:%s\n",pt->lpszText);
	//GetApplication()->LogMessage("OnToolTip:%s",pt->lpszText);
	return TRUE;
}

// Tooltip will be hidden
BOOL CCyberSolitaireView::OnToolTipPop( UINT id, NMHDR *pTTT, LRESULT *pResult )
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	if ((pTTT->idFrom == (UINT)this))
	{
		//TRACE("OnToolTipPop %s\n",m_pCardToolTip->m_pCard->GetCardName());
		//GetApplication()->LogMessage("OnToolTipPop %s",m_pCardToolTip->m_pCard->GetCardName());
		PROGRAM_FAILURE(m_pCardToolTip->m_pCard->GetFaceUp());
		m_pCardToolTip->Show(FALSE);
		//pDoc->m_pDo->CreateDo(CHANGE_CARD_FACEUP,m_pToolTipPile,NULL,m_pToolTipCard,FALSE);
		OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pCardToolTip->m_pPile);
	}
	return FALSE;
}

// Tooltip will be shown
BOOL CCyberSolitaireView::OnToolTipShow( UINT id, NMHDR *pTTT, LRESULT *pResult )
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	if ((pTTT->idFrom == (UINT)this))
	{
		//TRACE("OnToolTipShow %s\n",m_pCardToolTip->m_pCard->GetCardName());
		//GetApplication()->LogMessage("OnToolTipShow %s",m_pCardToolTip->m_pCard->GetCardName());
		PROGRAM_FAILURE(!m_pCardToolTip->m_pCard->GetFaceUp());
		m_pCardToolTip->Show(TRUE);
		//pDoc->m_pDo->CreateDo(CHANGE_CARD_FACEUP,m_pToolTipPile,NULL,m_pToolTipCard,TRUE);
		OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pCardToolTip->m_pPile);
	}
	return FALSE;
}

BOOL CCyberSolitaireView::PreTranslateMessage(MSG* pMsg)
{
	// Tooltip management
	if ( m_pCardToolTip != NULL )
	{
		if (	(pMsg->message == WM_LBUTTONDOWN) ||
				(pMsg->message == WM_LBUTTONUP) || 
				(pMsg->message == WM_MBUTTONDOWN) ||
				(pMsg->message == WM_MBUTTONUP) ||
				(pMsg->message == WM_MOUSEMOVE) ||
				(pMsg->message == WM_RBUTTONDOWN) ||
				(pMsg->message == WM_RBUTTONUP) )
		{
			if (m_pCardToolTip->m_hWnd)
			{
				m_pCardToolTip->Activate(TRUE);
				m_pCardToolTip->RelayEvent(pMsg);
			}
		}
	}
   return CView::PreTranslateMessage(pMsg);
}
 


/////////////////////////////////////////////////////////////////////////////
// Hiermit werden bei einem Mausklick links entsprechend der Vorgaben des 
// Kartenstapels Karten selektiert
// bei Rückgabe von TRUE wird die weitere Verarbeitung des Mausklicks beendet
//
BOOL CCyberSolitaireView::SelectCards(CPile* pPile, CPoint LogPoint, BOOL& RetWithErr)
{
	CCard* pCard = NULL;
	m_pClickedCard = NULL;

	if (pPile->m_KindOfSelecting != selNotAvailable)
	{
		if ((pPile->m_KindOfSelecting == selPairsValue13 || pPile->m_KindOfSelecting == selPairsEqualValue) &&
			m_pSelectedCard)
		{
			// selektiere Paare
			// finde die Karte, die angeklickt wurde
			pCard = pPile->GetClickedCard(LogPoint,RetWithErr);
			m_pClickedCard = pCard;
			if (RetWithErr) 
			{
				m_pSelectedPile->SelectCards(NULL,TRUE);
				OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pSelectedPile);
				m_pSelectedPile = NULL;
				m_pSelectedCard = NULL;
				return TRUE;
			}
			// pCard kann sich ändern
			pCard = pPile->SelectSequence(pCard,RetWithErr,m_pSelectedPile,m_pSelectedCard);
			if (RetWithErr) 
			{
				m_pSelectedPile->SelectCards(NULL,TRUE);
				OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pSelectedPile);
				m_pSelectedPile = NULL;
				m_pSelectedCard = NULL;
				return TRUE;
			}

			if (!m_pSecondSelectedPile)
			{
				// selektieren
				//TRACE("Second selected\n");
				pPile->SelectCards(pCard);
				m_pSecondSelectedCard = pCard;
				m_pSecondSelectedPile = pPile;
			}
			else
			{
				// hier ist m_pSecondSelectedPile != NULL
				if (pPile == m_pSecondSelectedPile)
				{
					// deselektieren
					//TRACE("deselected\n");
					m_pSelectedPile->SelectCards(NULL,TRUE);
					m_pSecondSelectedPile->SelectCards(NULL,TRUE);
					m_pSelectedPile = NULL;
					m_pSelectedCard = NULL;
					m_pSecondSelectedPile = NULL;
					m_pSecondSelectedCard = NULL;
				}
			}

			// zeichne
			OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)pPile);
			//m_LastPoint = LogPoint;
			return TRUE;

		}
		else
		{
			// selektiere einzeln
			// finde die Karte, die angeklickt wurde
			pCard = pPile->GetClickedCard(LogPoint,RetWithErr);
			m_pClickedCard = pCard;
			if (RetWithErr) return TRUE;
			// pCard kann sich ändern
			pCard = pPile->SelectSequence(pCard,RetWithErr);
			if (RetWithErr) return TRUE;

			if (!m_pSelectedPile)
			{
				// selektieren
				//TRACE("selected\n");
				pPile->SelectCards(pCard);
				m_pSelectedCard = pCard;
				m_pSelectedPile = pPile;
			}
			else
			{
				// hier ist m_pSelectedPile != NULL
				if (pPile == m_pSelectedPile)
				{
					// deselektieren
					//TRACE("deselected\n");
					m_pSelectedPile->SelectCards(NULL,TRUE);
					m_pSelectedPile = NULL;
					m_pSelectedCard = NULL;
				}
			}

			// zeichne
			OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)pPile);
			//m_LastPoint = LogPoint;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Diese Funktion versucht Karten vom angegebenen SourcePile zu geben
//
void CCyberSolitaireView::DealCards(CPile *pSrcPile,BOOL Redeal)
{
	if (pSrcPile->m_DealTo == dealNotAvailable)
		return;

	CCyberSolitaireDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// auf welche Kartenstapel soll gegeben werden
	CCard* pCard;
	if (!Redeal)
		pDoc->m_pDo->CreateDo(START,NULL,NULL,NULL,NULL);
	POSITION pos = pDoc->GetFirstPilePosition(pSrcPile->m_DealTo);
	CPile* pPile = pDoc->GetNextPile(pos,pSrcPile->m_DealTo);
	while (pPile)
	{ 
		// falls hier überhaupt Karten abgelegt werden dürfen
		if (pPile->m_KindOfPiling != pilNothing)
		{
			switch (pPile->m_KindOfPiling) 
			{
			case pilOneFromTableauOrWaste:
				break;
			default:
				for (int j=1; j<=pSrcPile->m_NumberPerDeal; j++)
				{
					// hole Karte(n) aus dem SrcPile falls noch vorhanden
					if (!pSrcPile->m_Cards.IsEmpty())
					{
						pCard = pSrcPile->m_Cards.GetTail();
						CRect UpdateRect(ZeroRect);
						pPile->Get(pSrcPile,pCard,(CRect&)UpdateRect);
						PROGRAM_FAILURE(!pCard->GetFaceUp());
						pCard->SetFaceUp(TRUE);
						BOOL ChangeLastCardFaceUp = TRUE;
						pDoc->m_pDo->CreateDo(GET_DEAL,pSrcPile,pPile,pCard,ChangeLastCardFaceUp);
						if (pSrcPile->IsK(CStock))
						{
							if ((((CStock*)pSrcPile)->m_NumberOfRedeal > 0 &&((CStock*)pSrcPile)->m_NumberOfRedeal > pDoc->m_CountOfRedeals) || 
								(((CStock*)pSrcPile)->m_NumberOfRedeal == -1))
							{
								// es kann neu gegeben werden
							}
							else
							{
								// die Bitmap dür den leeren Pile wechselt
								if (pSrcPile->m_IDDibEmpty != IDB_EMPTY3)
								{
									pSrcPile->m_IDDibEmpty = IDB_EMPTY3;
									pSrcPile->m_dibEmpty.Detach();
									pSrcPile->m_dibEmpty.Load(AfxGetApp()->m_hInstance,pSrcPile->m_IDDibEmpty);
									UpdateBitmaps();
								}
							}
						}
						OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)pPile);
						OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)pSrcPile);
					}
					else
					{
						return;
					}
				}
			}
		}
		pPile = pDoc->GetNextPile(pos,pSrcPile->m_DealTo);
	}
	pDoc->m_pDo->CreateDo(END,NULL,NULL,NULL,NULL);
	//OnUpdate(NULL,HINT_DISPLAY_ALL_PILES,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Hiermit wird versucht auf  einem Foundation abzulegen
// bei Rückgabe von TRUE wird die weitere Verarbeitung des Mausklicks beendet
//
BOOL CCyberSolitaireView::PutToFoundation(CPile* pPile, BOOL& RetWithErr)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pPile->IsK(CFoundation))
	{
		// es dürfen nur ganze Stapel abgelegt werden
		if (pPile->m_NumberPerMove == 13)
		{
			if (pDoc->m_pMovePile->m_Cards.GetCount() != pPile->m_NumberPerMove)
			{
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
			}

			if (!pPile->IsEmpty())
			{
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
			}
			else
			{
				// der Pile ist leer
				CCard* pMovePileCard = pDoc->m_pMovePile->m_Cards.GetHead();
				switch (pPile->m_PutIfEmpty)
				{
				case pieNoCard:
				case pieAnyCard:
				case pieOnlyFromAutoDeal:
				case pieFromDealOrAny:
				case pieBasicCardMinus1:
				case pieKing:
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
					break;
				case pieBasicCard:	
					if (!pMovePileCard->IsBasicCard(((CFoundation*)pPile)->m_BasicCardValue,
													((CFoundation*)pPile)->m_BasicCardOnlyVal,
													((CFoundation*)pPile)->m_BasicCardOnlyColor))
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					//else
					//{
					//	PutMovePileToPile(pPile);
					//}
					break;
				case pieAce:
					if (!pMovePileCard->IsAce())
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					//else
					//{
					//	PutMovePileToPile(pPile);
					//}
					break;
				default:
					break;
				}
				// da nur ganze Stapel abgelegt werden können, reicht die Untersuchung des MovePiles
				switch (pPile->m_KindOfPiling)
				{
				case pilDownInSuit:
					// sind die Karten absteigend und in Farbfolge
					if (!pDoc->m_pMovePile->IsDownInSequenceAndSuit())
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					else
					{
						PutMovePileToPile(pPile);
					}
					break;
				case pilDownNoColor:
					// sind die Karten absteigend
					if (!pDoc->m_pMovePile->IsDownInSequence())
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					else
					{
						PutMovePileToPile(pPile);
					}
					break;
				case pilDownAlternate:
					// sind die Karten absteigend und in Farbfolge
					if (!pDoc->m_pMovePile->IsDownInSequenceAndAlternate())
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					else
					{
						PutMovePileToPile(pPile);
					}
					break;
				case pilUpInSuit:
				case pilUpAlternate:
				case pilUpNoColor:
				case pilWithGaps:
				case pilDownSameColor:
					// zu_tun
					break;
				case pilUpSameColor:
					// sind die Karten aufsteigend und gleiche Farbe wie Basiskarte
					if (!pDoc->m_pMovePile->IsUpInSequenceSameColor(((CFoundation*)pPile)->m_BasicCardValue))
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					else
					{
						PutMovePileToPile(pPile);
					}
					break;
				case pilUpDiff2InSuit:
					// sind die Karten aufsteigend um 2 auseinander
					if (!pDoc->m_pMovePile->IsUpInSequenceValueDiff2InSuit(((CFoundation*)pPile)->m_BasicCardValue))
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					else
					{
						PutMovePileToPile(pPile);
					}
					break;
				default:
					break;
				}

			}
			return TRUE;
		}

		// es dürfen nur einzelne Karten abgelegt werden
		if (pPile->m_NumberPerMove == 1)
		{
			// falls mehr als 1 Karte im MovePile -> nicht gut
			if (pDoc->m_pMovePile->m_Cards.GetCount() != pPile->m_NumberPerMove)
			{
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
			}

			CCard* pMovePileCard = pDoc->m_pMovePile->m_Cards.GetHead();
			// mit welcher Karte wird gestartet ?
			if (pPile->IsEmpty())
			{
				switch (pPile->m_PutIfEmpty)
				{
				case pieNoCard:
				case pieAnyCard:
				case pieOnlyFromAutoDeal:
				case pieFromDealOrAny:
				case pieBasicCardMinus1:
				case pieKing:
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
					break;
				case pieBasicCard:	
					if (!pMovePileCard->IsBasicCard(((CFoundation*)pPile)->m_BasicCardValue,
													((CFoundation*)pPile)->m_BasicCardOnlyVal,
													((CFoundation*)pPile)->m_BasicCardOnlyColor))
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					else
					{
						PutMovePileToPile(pPile);
					}
					break;
				case pieAce:
					if (!pMovePileCard->IsAce())
					{
						PutMovePileBack();
						RetWithErr = TRUE;
						return TRUE;
					}
					else
					{
						PutMovePileToPile(pPile);
					}
					break;
				default:
					break;
				}
			}
			else
			{
				// wir brauchen einen temporären Pile zur Untersuchung
				CCard* pLastCard = pPile->m_Cards.GetTail();
				BOOL Ok = FALSE;
				CPile* pTmpPile = new CPile(*pPile);
				CPile* pMovePile = new CPile(*pDoc->m_pMovePile);
				CRect UpdateRect(0,0,0,0);
				pTmpPile->Get(pMovePile,NULL,UpdateRect);

				// ist die maximale Zahl von Karten im Pile erreicht ?
				if ((pTmpPile->m_MaxCards != NA) && (pTmpPile->m_Cards.GetCount() > pPile->m_MaxCards))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}

				switch (pPile->m_KindOfPiling)
				{
				case pilDownInSuit:
					// sind die Karten absteigend und in Farbfolge
					Ok = pTmpPile->IsDownInSequenceAndSuit();
					break;
				case pilDownNoColor:
					// sind die Karten absteigend
					Ok = pTmpPile->IsDownInSequence();
					break;
				case pilUpInSuit:
					// sind die Karten aufsteigend und in Farbfolge
					Ok = pTmpPile->IsUpInSequenceAndSuit();
					break;
				case pilDownAlternate:
					// sind die Karten absteigend und alternierend
					Ok = pTmpPile->IsDownInSequenceAndAlternate();
					break;
				case pilUpAlternate:
					// sind die Karten aufsteigend und alternierend
					Ok = pTmpPile->IsUpInSequenceAndAlternate();
					break;
				case pilUpNoColor:
					// sind die Karten aufsteigend
					Ok = pTmpPile->IsUpInSequence();
					break;
				case pilWithGaps:
					// zu_tun
					break;
				case pilDownInSuitRolling:
					// sind die Karten absteigend und in Farbfolge mit Überlauf (As->König)
					Ok = pTmpPile->IsDownInSequenceAndSuitRolling();
					break;
				case pilUpInSuitRolling:
					// sind die Karten aufsteigend und in Farbfolge mit Überlauf (König->As)
					Ok = pTmpPile->IsUpInSequenceAndSuitRolling();
					break;
				case pilUpDownNoColorRolling:
					// sind die Karten auf- oder absteigend mit Überlauf
					Ok = pLastCard->IsDownInSequenceRolling(m_pSelectedCard) || 
						pLastCard->IsUpInSequenceRolling(m_pSelectedCard);
					break;
				case pilUpSameColor:
					// sind die Karten aufsteigend und gleiche Farbe wie Basiskarte
					Ok = pTmpPile->IsUpInSequenceSameColor(((CFoundation*)pPile)->m_BasicCardValue);
					break;
				case pilDownSameColor:
					// zu_tun
					Ok = FALSE;
					break;
				case pilUpDiff2InSuit:
					// sind die Karten aufsteigend um 2 auseinander
					Ok = pTmpPile->IsUpInSequenceValueDiff2InSuit(((CFoundation*)pPile)->m_BasicCardValue);
					break;
				default:
					ASSERT(0);
					break;
				}
				delete pTmpPile;
				delete pMovePile;
				if (Ok)
				{
					PutMovePileToPile(pPile);
					return TRUE;
				}
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
			}
		}
		// es dürfen nur Paare von Karten abgelegt werden
		if (pPile->m_NumberPerMove == 2)
		{
			// falls mehr als 2 Karten im MovePile -> nicht gut
			/*
			if (pDoc->m_pMovePile->m_Cards.GetCount() != pPile->m_NumberPerMove)
			{
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
			}
			*/

			CCard* pMovePileCard = pDoc->m_pMovePile->m_Cards.GetHead();
			// wir brauchen einen temporären Pile zur Untersuchung
			BOOL Ok = FALSE;
			CPile* pTmpPile = new CPile(*pPile);
			CPile* pMovePile = new CPile(*pDoc->m_pMovePile);
			CRect UpdateRect(0,0,0,0);
			pTmpPile->Get(pMovePile,NULL,UpdateRect);

			// ist die maximale Zahl von Karten im Pile erreicht ?
			/*
			 if ((pTmpPile->m_MaxCards != NA) && (pTmpPile->m_Cards.GetCount() > pPile->m_MaxCards))
			{
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
			}
			*/

			switch (pPile->m_KindOfPiling)
			{
			case pilPairsEqualValue:
				// sind die Karten absteigend und in Farbfolge
				Ok = (pDoc->m_pMovePile->m_Cards.GetCount() == 2) &&
					 (pDoc->m_pMovePile->m_Cards.GetHead()->GetValue() == pDoc->m_pMovePile->m_Cards.GetTail()->GetValue());
				break;
			case pilPairsValue13OrKing:
				// sind die Karten absteigend
				Ok = (pDoc->m_pMovePile->m_Cards.GetCount() == 2) ||
					((pDoc->m_pMovePile->m_Cards.GetCount() == 1) && pDoc->m_pMovePile->m_Cards.GetHead()->IsKing());
				break;
			default:
				ASSERT(0);
				break;
			}
			delete pTmpPile;
			delete pMovePile;
			if (Ok)
			{
				pDoc->m_pDo->CreateDo(START,NULL,NULL,NULL,NULL);
				PutMovePilePairToPile(pPile);
				return TRUE;
			}
			else
			{
				PutMovePilePairBack();
				RetWithErr = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Hiermit wird versucht auf einen Tableau oder Waste abzulegen
// bei Rückgabe von TRUE wird die weitere Verarbeitung des Mausklicks beendet
//
BOOL CCyberSolitaireView::PutToTableauOrWaste(CPile* pPile, BOOL& RetWithErr)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CCard* pCard = NULL;
	if (pPile->IsK(CTableau) || pPile->IsK(CWaste))
	{
		// dürfen die Karten hier abgelegt werden ?
		if (pPile->IsEmpty())
		{
			// zu_tun
			// leeren Stapel bearbeiten
			CCard* pFirstMovePileCard = pDoc->m_pMovePile->m_Cards.GetHead();
			switch (pPile->m_PutIfEmpty)
			{
			case pieNoCard:				// vom Anwender dürfen keine Karten abgelegt werden
			case pieOnlyFromAutoDeal:	// es dürfen nur automatisch Karten hier abgelegt werden, nicht aber vom Anwender	
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
			case pieAnyCard:
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pieFromDealOrAny:
				if (pDoc->m_DealPileArr.GetAt(pDoc->m_DealPileArr.GetUpperBound())->IsEmpty())
					PutMovePileToPile(pPile);
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;
			case pieOnlyFromStock:
				// darf nur vom Stock belegt werden
				if (m_pSelectedPile->IsK(CStock))
					PutMovePileToPile(pPile);
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;
			case pieOnlyFromTableau:
				// darf nur vom Stock belegt werden
				if (m_pSelectedPile->IsK(CTableau))
					PutMovePileToPile(pPile);
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;

			// zu_tun
			case pieBasicCard:		
			case pieBasicCardMinus1:
			case pieKing:
			{
				CCard* pFirstMovePileCard = pDoc->m_pMovePile->m_Cards.GetHead();
				if (pFirstMovePileCard->IsKing())
					PutMovePileToPile(pPile);
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;
			}	
			default:
				break;
			}
		}
		else
		{
			// der Kartenstapel ist nicht leer
			// ist die maximale Zahl von Karten im Pile erreicht ?
			if ((pPile->m_MaxCards != NA) && (pPile->m_Cards.GetCount() >= pPile->m_MaxCards))
			{
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
			}
			CCard* pFirstMovePileCard = pDoc->m_pMovePile->m_Cards.GetHead();
			pCard = pPile->m_Cards.GetTail();

			switch (pPile->m_KindOfPiling)
			{
			case pilDownNoColor:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsDownInSequence(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilDownInSuit:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsDownInSuit(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilOnlyFromStock:
				// darf nur vom Stock belegt werden
				if (m_pSelectedPile->IsK(CStock))
					PutMovePileToPile(pPile);
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;
			case pilOnlyFromTableau:
				// darf nur von Tableaus belegt werden
				if (m_pSelectedPile->IsK(CTableau))
					PutMovePileToPile(pPile);
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;
			case pilDownAlternate:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsDownAlternate(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilUpInSuit:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsUpInSuit(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilUpAlternate:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsUpAlternate(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilUpNoColor:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsUpInSequence(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilWithGaps:
				// zu_tun
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
				break;
			case pilDownInSuitRolling:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsDownInSuitRolling(pFirstMovePileCard) || pCard->IsBasicCard(pDoc->m_BasicCardValue,TRUE,FALSE))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilUpInSuitRolling:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsUpInSuitRolling(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilDownAlternateRolling:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				RetWithErr |= !pCard->IsDownAlternateRolling(pFirstMovePileCard);
				if (pPile->m_MaxCards != 26)	// != Gloucestershire
					RetWithErr |= pCard->IsBasicCard(pDoc->m_BasicCardValue,TRUE,FALSE);

				if (RetWithErr)
				{
					PutMovePileBack();
					//RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilUpAlternateRolling:
				// paßt die erste Karte des MovePiles in der Reihenfolge
				if (!pCard->IsUpAlternateRolling(pFirstMovePileCard))
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				else
				{
					PutMovePileToPile(pPile);
				}
				break;
			case pilUpDownNoColorRolling:
				// sind die Karten auf- oder absteigend mit Überlauf
				if (pCard->IsDownInSequenceRolling(pFirstMovePileCard) || 
					pCard->IsUpInSequenceRolling(pFirstMovePileCard))
				{
					PutMovePileToPile(pPile);
				}
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;
			case pilOneFromTableauOrWaste:
				// es kann genau eine Karte von einem Tableau oder Waste hier abgelegt werden
				if (pPile->IsEmpty() && (pDoc->m_pMovePile->m_Cards.GetCount() == 1))
				{
					PutMovePileToPile(pPile);
				}
				else
				{
					PutMovePileBack();
					RetWithErr = TRUE;
					return TRUE;
				}
				break;
			default:
				PutMovePileBack();
				RetWithErr = TRUE;
				return TRUE;
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}

// Testet den Stock ob noch mal gegeben werden kann
BOOL CCyberSolitaireView::TestRedeal()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CStock* pStock = pDoc->GetStock();
	ASSERT(pStock->IsEmpty());	
	if (pStock->m_NumberOfRedeal == 0)
		return FALSE;

	if ((pStock->m_NumberOfRedeal > 0 && pStock->m_NumberOfRedeal > pDoc->m_CountOfRedeals) || (pStock->m_NumberOfRedeal == -1))
	{
		// es kann neu gegeben werden
		pDoc->m_CountOfRedeals++;
		switch(pStock->m_RedealMethod) {
			case redealTableaus:		
			case redealTableausMix:	
			case redealWasteMix:
			case redealWaste:
				DoRedeal();
				return TRUE;
			case redealNotAvailable:	
			default:
				ASSERT(0);
				return FALSE;
		}
	}
	return FALSE;
}

// sammelt Waste ein, dreht die Karten um und weist sie dem Stock zum erneuten Geben zu
void CCyberSolitaireView::DoRedeal()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CStock* pStock = pDoc->GetStock();
	CPile* pPile;

	if (!pDoc->m_RedealPileArr.GetSize())
		return;

	pDoc->m_pDo->CreateDo(START,NULL,NULL,NULL,NULL);
	pDoc->m_pDo->CreateDo(REDEAL,NULL,NULL,NULL,NULL);
	// alle Pile des Documents durchsuchen
	//for (int i=0; i < pDoc->m_RedealPileArr.GetSize(); i++)
	//for (int i=pDoc->m_RedealPileArr.GetCount()-1; i >=0; i--)
	for (int i=pDoc->m_RedealPileArr.GetSize()-1; i >=0; i--)
	{
		pPile = pDoc->m_RedealPileArr.GetAt(i);
		PutPileToStock(pPile);
	}
	if ((pStock->m_RedealMethod == redealTableausMix) || (pStock->m_RedealMethod == redealWasteMix))
	{
		// zu_tun
		// MixCards
	}
}

void CCyberSolitaireView::PutPileToStock(CPile* pPile)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	//pDoc->m_pDo->CreateDo(START,NULL,NULL,NULL,NULL);
	//pDoc->m_pDo->CreateDo(REDEAL,NULL,NULL,NULL,NULL);
	// für das Undo alle Karten FaceUp protokollieren
	CCard* pCard = NULL;
	POSITION pos = pPile->m_Cards.GetHeadPosition();
	while (pos)
	{
		pCard = pPile->m_Cards.GetNext(pos);
		if (pCard->GetFaceUp())
		{
			pCard->SetFaceUp(FALSE);
			pDoc->m_pDo->CreateDo(CHANGE_CARD_FACEUP,pPile,NULL,pCard,FALSE);
		}
	}
	pPile->Revert();
	pDoc->m_pDo->CreateDo(REVERT,pPile,NULL,NULL,NULL);
	pCard = pPile->m_Cards.GetHead();
	pDoc->GetStock()->Get(pPile,NULL,m_UpdateRect);
	//pDoc->m_pDo->CreateDo(GET,pPile,pDoc->GetStock(),NULL,FALSE);
	pDoc->m_pDo->CreateDo(GET,pPile,pDoc->GetStock(),pCard,FALSE);
	// wird vom nachfolgendem DealCards übernommen
	//pDoc->m_pDo->CreateDo(END,NULL,NULL,NULL,NULL);
	OnUpdate(NULL,HINT_UPDATE_PILES,NULL);
}


void CCyberSolitaireView::UpdateScreenAndBackground()
{
	Delete(m_pScreen);
	Delete(m_pBackground);

	CClientDC dc(this);
	OnPrepareDC(&dc);
	CRect ClientRect; GetClientRect(&ClientRect);
	
	// Screen und Background Dib mit der aktuellen Größe erzeugen
	m_pScreen = new CDib;
	m_pScreen->Create(dc,ClientRect.Size(),&m_BackGround);
	m_pScreen->m_TransPoints.RemoveAll();
	m_pBackground = new CDib;
	m_pBackground->Create(dc,ClientRect.Size(),&m_BackGround);
	m_pBackground->m_TransPoints.RemoveAll();
	SetDibs(&GetApplication()->m_dibCardBack);
	UpdateBitmaps();
	//OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
	OnUpdate(this,HINT_DISPLAY_ALL,NULL);
}

void CCyberSolitaireView::OnChangeCardFaceUp()
{
	if (!m_pClickedCard) return;
	PROGRAM_FAILURE(!m_pClickedCard->GetFaceUp());
	m_pClickedCard->SetFaceUp(!m_pClickedCard->GetFaceUp());
	// zeichne
	OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pShowPile);
}

void CCyberSolitaireView::ReportGameStatus()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	GetApplication()->LogMessage("GAME STATUS (complete)");
	CPile* pPile;
	CCard* pCard;
	// alle Pile bearbeiten
	POSITION pos = pDoc->m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = pDoc->m_PileList.GetNext(pos);
		int NumberOfCards = pPile->m_Cards.GetCount(); 
		GetApplication()->LogMessage("Pile :%s(%p) %d Cards",pPile->GetRuntimeClass()->m_lpszClassName,pPile,NumberOfCards);
		// alle Karten des Stapels bearbeiten
		POSITION pos1 = pPile->m_Cards.GetHeadPosition();
		while (pos1)
		{
			pCard = pPile->m_Cards.GetNext(pos1);
			CString CardName = pCard->GetCardName();
			if (CardName.GetLength() > 6)
				GetApplication()->LogMessage("\t %s\t Face %d\t Display %d",CardName,pCard->GetFaceUp(),pCard->m_IsVisible);
			else
				GetApplication()->LogMessage("\t %s\t\t Face %d\t Display %d",CardName,pCard->GetFaceUp(),pCard->m_IsVisible);
		}
	}
}

void CCyberSolitaireView::ReportGameDifferenceStatus()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	GetApplication()->LogMessage("GAME STATUS (difference)");
	CPile* pPile;
	CPile* pLastPile;
	CCard* pCard;
	// alle Pile bearbeiten
	POSITION pos  = pDoc->m_PileList.GetHeadPosition();
	POSITION posl = pDoc->m_LastPileList.GetHeadPosition();
	while (pos)
	{
		pPile = pDoc->m_PileList.GetNext(pos);
		pLastPile = pDoc->m_LastPileList.GetNext(posl);
		if (pPile->HasChanged(pLastPile))
		{
			int NumberOfCards = pPile->m_Cards.GetCount(); 
			GetApplication()->LogMessage("Pile :%s(%p) %d Cards",pPile->GetRuntimeClass()->m_lpszClassName,pPile,NumberOfCards);
			// alle Karten des Stapels bearbeiten
			POSITION pos1 = pPile->m_Cards.GetHeadPosition();
			while (pos1)
			{
				pCard = pPile->m_Cards.GetNext(pos1);
				CString CardName = pCard->GetCardName();
				if (CardName.GetLength() > 6)
					GetApplication()->LogMessage("\t %s\t Face %d\t Display %d",CardName,pCard->GetFaceUp(),pCard->m_IsVisible);
				else
					GetApplication()->LogMessage("\t %s\t\t Face %d\t Display %d",CardName,pCard->GetFaceUp(),pCard->m_IsVisible);
			}
		}
	}
	pDoc->CopyPileList();
}

/*
// automatische Ablage aller Karten, die abgelegt werden können
{
}
	

	*/
// automatische Ablage aller Karten, die abgelegt werden können
//
void CCyberSolitaireView::OnAutoplay()
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	GetApplication()->LogMessage("AutoPlay activated");
	// es darf keine Karte selektiert sein
	// zu_tun
	// überlegen, ob nicht automatisch deselektiert werden kann
	if (m_pSelectedPile || m_pSelectedCard)
		RETURN_WITH_ERROR;
	pDoc->m_InAutoPlay = TRUE;
	int NumberOfAutoPlayedCards = 0;

	CPile* pPile;
	CCard* pCard;
	BOOL	CardsToFoundation = TRUE;
	while (CardsToFoundation)
	{
		CardsToFoundation = FALSE;
		// alle Piles bearbeiten
		POSITION pos = pDoc->m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = pDoc->m_PileList.GetNext(pos);
			if (!pPile->IsK(CFoundation) && !pPile->IsEmpty() && ((pPile->m_PermittedPlay == pppTableauOrFoundation) || 
									 (pPile->m_PermittedPlay == pppFoundation) ||
									 (pPile->m_PermittedPlay == pppWaste_Tableau_Foundation)))
			{
				BOOL PutCardToFoundation = TRUE;
				while (PutCardToFoundation && !pPile->IsEmpty())
				{
					PutCardToFoundation = FALSE;
					// zu_tun
					// piles, die zulassen, beliebige Karten auszuwählen, müssen hier anders behandelt werden
					// wähle die letzte Karte aus
					pCard = pPile->m_Cards.GetTail();
					m_pSelectedPile = pPile;
					m_pSelectedCard = m_pClickedCard = pCard;
					// teste, ob letzte Karte auf einen Foundation abgelegt werden kann
					// durchsuche dafür alle Foundation
					CPile* pFoundation = NULL;
					POSITION pos2 = pDoc->m_PileList.GetHeadPosition();
					while (pos2)
					{
						pFoundation = pDoc->m_PileList.GetNext(pos2);
						if (!pFoundation->IsK(CFoundation))
							continue;
						// berechne den Ablagepunkt
						CPoint Point = pFoundation->GetPosition();
						CSize CardSize(CCyberSolitaireApp::DefaultCardSize);
						//CardSize.cx = CardSize.cx / 2;
						//CardSize.cy = CardSize.cy / 2;
						//Point += CardSize;
						Point += CPoint(5,5);
						if (pFoundation->m_NumberPerMove == 13)
						{
							// dann muß anders selektiert werden
							if (m_pSelectedPile->m_Cards.GetCount() < 13)
							{
								// kann nicht sein
								PutCardToFoundation = FALSE;
							}
							else
							{
								pCard = m_pSelectedPile->m_Cards.GetAt(m_pSelectedPile->m_Cards.FindIndex(m_pSelectedPile->m_Cards.GetCount()-13));
								m_pSelectedCard = m_pClickedCard = pCard;
								PutCardToFoundation = TestPutToFoundation(pFoundation);
							}
						}
						else
							PutCardToFoundation = TestPutToFoundation(pFoundation);
						if (PutCardToFoundation)
						{
							// wenn ja, dann lege ab (wie bei doppelklick) und setze CardsToFoundation = TRUE
							if (!m_CardsMoving)
							{
								if (!NumberOfAutoPlayedCards)
									pDoc->m_pDo->CreateDo(START_AUTOPLAY,NULL,NULL,NULL,NULL);
								PutSelectedPileToMovePile(m_pSelectedCard->m_LogicalPosition);			
								NumberOfAutoPlayedCards += pDoc->m_pMovePile->m_Cards.GetCount();
								//pDoc->m_pMovePile->Move(m_LastPoint,Point,m_UpdateRect);
								pDoc->m_pMovePile->Move(m_pSelectedCard->m_LogicalPosition,Point,m_UpdateRect);
								OnUpdate(NULL,HINT_DISPLAY_MOVEPILE,NULL);
							}
							//OnMouseMove(MK_LBUTTON,Point);
							CClientDC dc(this);
							OnPrepareDC(&dc);
							dc.LPtoDP(&Point);
							OnLButtonUp(0,Point);
							m_LastPoint = Point;
							CardsToFoundation = TRUE;
							break;
						}
					}
				}
			}
		}
	}
	if (NumberOfAutoPlayedCards)
	{		
		pDoc->m_pDo->CreateDo(END_AUTOPLAY,NULL,NULL,NULL,NULL);
		pDoc->m_InAutoPlay = FALSE;
		pDoc->ComputeScore();
		ReportGameStatus();
		//pDoc->PlaySound("Plus.wav",NULL,SND_ASYNC | SND_FILENAME);
		//pDoc->PlaySound("Plus.wav",NULL,SND_SYNC | SND_FILENAME);
	}
	else
	{
		// zu_tun
		// Meldung durch Ton, daß es nicht möglich ist
		//::PlaySound("Bing.wav",NULL,SND_ASYNC | SND_FILENAME);
		pDoc->m_InAutoPlay = FALSE;
		::PlaySound("Bing.wav",NULL,SND_SYNC | SND_FILENAME);
	}
	m_pSelectedPile = NULL;
	m_pSelectedCard = m_pClickedCard = NULL;
	//pDoc->ComputeScore();
}

void CCyberSolitaireView::OnUpdateAutoplay(CCmdUI *pCmdUI)
{
	CCyberSolitaireDoc* pDoc = GetDocument();
	CPile* pPile;
	CPile* pOldSelectedPile = m_pSelectedPile;
	CCard* pOldClickedCard  = m_pClickedCard;
	CCard* pOldSelectedCard = m_pSelectedCard;
	CCard* pCard;
	BOOL	CardsToFoundation = TRUE;
	while (CardsToFoundation)
	{
		CardsToFoundation = FALSE;
		// alle Piles bearbeiten
		POSITION pos = pDoc->m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = pDoc->m_PileList.GetNext(pos);
			if (!pPile->IsK(CFoundation) && !pPile->IsEmpty() && ((pPile->m_PermittedPlay == pppTableauOrFoundation) || 
									 (pPile->m_PermittedPlay == pppFoundation) ||
									 (pPile->m_PermittedPlay == pppWaste_Tableau_Foundation)))
			{
				BOOL PutCardToFoundation = TRUE;
				while (PutCardToFoundation && !pPile->IsEmpty())
				{
					PutCardToFoundation = FALSE;
					// wähle die letzte Karte aus
					pCard = pPile->m_Cards.GetTail();
					m_pSelectedPile = pPile;
					m_pSelectedCard = m_pClickedCard = pCard;
					// teste, ob letzte Karte auf einen Foundation abgelegt werden kann
					// durchsuche dafür alle Foundation
					CPile* pFoundation = NULL;
					POSITION pos2 = pDoc->m_PileList.GetHeadPosition();
					while (pos2)
					{
						pFoundation = pDoc->m_PileList.GetNext(pos2);
						if (!pFoundation->IsK(CFoundation))
							continue;
						// berechne den Ablagepunkt
						CPoint Point = pFoundation->GetPosition();
						Point += CPoint(5,5);
						if (pFoundation->m_NumberPerMove == 13)
						{
							// dann muß anders selektiert werden
							if (m_pSelectedPile->m_Cards.GetCount() < 13)
							{
								// kann nicht sein
								PutCardToFoundation = FALSE;
							}
							else
							{
								pCard = m_pSelectedPile->m_Cards.GetAt(m_pSelectedPile->m_Cards.FindIndex(m_pSelectedPile->m_Cards.GetCount()-13));
								m_pSelectedCard = m_pClickedCard = pCard;
								PutCardToFoundation = TestPutToFoundation(pFoundation);
							}
						}
						else
							PutCardToFoundation = TestPutToFoundation(pFoundation);
						if (PutCardToFoundation)
						{
							pCmdUI->Enable();
							m_pSelectedPile = pOldSelectedPile;
							m_pSelectedCard = pOldSelectedCard; 
							m_pClickedCard  = pOldClickedCard;
							return;
						}
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
	m_pSelectedPile = pOldSelectedPile;
	m_pSelectedCard = pOldSelectedCard; 
	m_pClickedCard  = pOldClickedCard;
}


