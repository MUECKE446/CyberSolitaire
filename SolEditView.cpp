// SolEditView.cpp : Implementierung der Klasse CSolEditView
//

#include "stdafx.h"
#include "CyberSolitaire.h"

#include "MainFrm.h"
#include "SolEditDoc.h"
#include "InputLevelPointsDialog.h"
#include "SolEditView.h"
#include "PropertiesDialogs.h"
#include ".\soleditview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Macros

#define PREPARE_SOLEDIT_VIEW	CSolEditDoc* pDoc = GetDocument();\
								CClientDC dc(this);\
								OnPrepareDC(&dc);\
								CPoint LogPoint(point);\
								dc.DPtoLP(&LogPoint);\

/////////////////////////////////////////////////////////////////////////////
// CSolEditView

IMPLEMENT_DYNCREATE(CSolEditView, CView)

BEGIN_MESSAGE_MAP(CSolEditView, CView)
	//{{AFX_MSG_MAP(CSolEditView)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_INSERT_STOCK, OnInsertStock)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_SOLEDITPILECONTEXT_DELETE, OnSoleditpilecontextDelete)
	ON_UPDATE_COMMAND_UI(ID_SOLEDITPILECONTEXT_DELETE, OnUpdateSoleditpilecontextDelete)
	ON_COMMAND(ID_SOLEDITPILECONTEXT_PROPERTIES, OnSoleditpilecontextProperties)
	ON_UPDATE_COMMAND_UI(ID_SOLEDITPILECONTEXT_PROPERTIES, OnUpdateSoleditpilecontextProperties)
	ON_COMMAND(ID_SOLEDITGAMECONTEXT_PROPERTIES, OnSoleditgamecontextProperties)
	ON_UPDATE_COMMAND_UI(ID_SOLEDITGAMECONTEXT_PROPERTIES, OnUpdateSoleditgamecontextProperties)
	ON_COMMAND(ID_INSERT_FOUNDATION, OnInsertFoundation)
	ON_UPDATE_COMMAND_UI(ID_INSERT_STOCK, OnUpdateInsertPile)
	ON_COMMAND(ID_INSERT_TABLEAU, OnInsertTableau)
	ON_COMMAND(ID_INSERT_WASTE, OnInsertWaste)
	ON_COMMAND(ID_SOLEDITPILECONTEXT_COPY, OnSoleditpilecontextCopy)
	ON_COMMAND(ID_SOLEDITPILECONTEXT_MAXRECTMAXIMAL, OnSoleditpilecontextMaxrectmaximal)
	ON_COMMAND(ID_SOLEDITPILECONTEXT_MAXRECTINKREMENTIEREN, OnSoleditpilecontextMaxrectinkrementieren)
	ON_COMMAND(ID_SOLEDITPILECONTEXT_MAXRECTDEKREMENTIEREN, OnSoleditpilecontextMaxrectdekrementieren)
	ON_UPDATE_COMMAND_UI(ID_INSERT_FOUNDATION, OnUpdateInsertPile)
	ON_UPDATE_COMMAND_UI(ID_INSERT_TABLEAU, OnUpdateInsertPile)
	ON_UPDATE_COMMAND_UI(ID_INSERT_WASTE, OnUpdateInsertPile)
	ON_COMMAND(ID_DATEI_SPEICHERN, OnDateiSpeichern)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCORE, OnUpdatePanePosition )
	ON_COMMAND(ID_SOLEDITPILECONTEXT_MAXRECTLOESCHEN, OnSoleditpilecontextMaxrectloeschen)
	ON_COMMAND(ID_TTTLEVELPUNKTE, OnTTTLevelpunkte)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSolEditView Konstruktion/Destruktion

CSolEditView::CSolEditView()
{
	m_BackGround.Load(AfxGetApp()->m_hInstance,IDB_BACK_LITE_GREEN);
	m_Screen.Load(AfxGetApp()->m_hInstance,IDB_BACK_LITE_GREEN);
	m_dibCardSelected.Load(AfxGetApp()->m_hInstance,IDB_SELECTEDMASK);
	m_pScreen = NULL;
	m_pBackground = NULL;
	m_VisibleRect.top = 0;
	m_VisibleRect.left = 0;
	m_VisibleRect.right = TABLE_CX; 
	m_VisibleRect.bottom = TABLE_CY;
	m_PileMoving = FALSE;
	m_pSelectedPile = NULL;
	m_pContextPile = NULL;
	m_pSelectedCard = NULL;
	m_UpdateRect.SetRectEmpty();
	m_LastPoint.x = 0;
	m_LastPoint.y = 0;
	m_OldUseForAllPilesOfThisType = TRUE;
}

CSolEditView::~CSolEditView()
{
	if (m_pScreen)
		delete m_pScreen;
	if (m_pBackground)
		delete m_pBackground;
}

BOOL CSolEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSolEditView Zeichnen

void CSolEditView::OnDraw(CDC* pDC)
{
	
	CSolEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect UpdateRect =((CPaintDC*)pDC)->m_ps.rcPaint; 
	CRect UpdateRect1(UpdateRect);
	pDC->DPtoLP((CRect*)&UpdateRect);
	if (m_pScreen)
		m_pScreen->Draw(*pDC,&UpdateRect,&UpdateRect1,FALSE);
	
	
	DrawGrid(pDC,FALSE);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(m_VisibleRect);
}

/////////////////////////////////////////////////////////////////////////////
// CSolEditView Drucken

BOOL CSolEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CSolEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CSolEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einfügen
}

/////////////////////////////////////////////////////////////////////////////
// CSolEditView Diagnose

#ifdef _DEBUG
void CSolEditView::AssertValid() const
{
	CView::AssertValid();
}

void CSolEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSolEditDoc* CSolEditView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSolEditDoc)));
	return (CSolEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSolEditView Nachrichten-Handler

void CSolEditView::OnInitialUpdate() 
{
	CSolEditDoc* pDoc = GetDocument();
	// View pointer besetzen
	pDoc->m_pView = this;	
	pDoc->UpdatePiles();	// ???
	// initiaisieren des UpdateRect
	m_UpdateRect.SetRectEmpty();

	UpdateBitmaps();
	
	OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
}

void CSolEditView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
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

void CSolEditView::OnSize(UINT nType, int cx, int cy) 
{
	//TRACE("CSolEditView::OnSize\n");

	CView::OnSize(nType, cx, cy);
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

void CSolEditView::DrawGrid(CDC *pDC, BOOL Lines /* = TRUE */)
{
	CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
	//pDC->SelectStockObject(BLACK_BRUSH);
	int x,y;
	if (Lines)
	{
		for (x=0; x<=TABLE_CX; x+=5*GRID)
		{
			CPoint GridPoint1(x,0);
			pDC->MoveTo(GridPoint1);
			CPoint GridPoint2(x,TABLE_CY);
			pDC->LineTo(GridPoint2);
		}
		for (y=0; y<=TABLE_CY; y+=5*GRID)
		{
			CPoint GridPoint1(0,y);
			pDC->MoveTo(GridPoint1);
			CPoint GridPoint2(TABLE_CX,y);
			pDC->LineTo(GridPoint2);
		}
	}
	else
	{
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(BLACK_BRUSH);
		for (x=0; x<=TABLE_CX; x+=10*GRID)
			for (y=0; y<=TABLE_CY; y+=10*GRID)
			{
				CPoint GridPoint1(x-2,y-2);
				CPoint GridPoint2(x+3,y+3);
				CRect GridRect(GridPoint1,GridPoint2);
				pDC->Rectangle(GridRect);
			}
			if (pOldBrush) pDC->SelectObject(pOldBrush);
	}
	if (pOldPen) pDC->SelectObject(pOldPen);
}

void CSolEditView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CSolEditDoc* pDoc = GetDocument();
	if (!pDoc->m_pLayout) return;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPile* pPile;
	POSITION pos;
	switch (lHint) {
    case HINT_DISPLAY_ALL_PILES:
		// sofern es überhaupt Piles gibt
		if(!pDoc->m_pLayout->m_PileList.IsEmpty())
		{
			// das zu löschende Rechteck ist die Vereinigung aller Pile Rects
			m_UpdateRect.SetRectEmpty();
			pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
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
			pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
				pPile->Display(&dc,m_Dibs,TRUE,FALSE);
			}
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		}
		break;
	case HINT_DISPLAY_PILE:
		{
			pPile = (CPile*)pHint;
			if (!pPile)
				return;
			//CRect PileRect(pPile->GetPosition(),pPile->GetSize());
			CRect PileRect(pPile->GetPosition(),pPile->GetMaxSize());
			m_UpdateRect = PileRect;
			m_UpdateRect.InflateRect(2,2);
			dc.LPtoDP(&m_UpdateRect);
			m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
			// Pile des Documents zeichnen (in die Screen Bitmap)
			pPile->Display(&dc,m_Dibs,TRUE,FALSE);
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		}
		break;
    case HINT_DISPLAY_MOVEPILE:
	case HINT_UPDATE_PILES:
		{
			// sofern es überhaupt Piles gibt
			if(!pDoc->m_pLayout->m_PileList.IsEmpty())
			{
				if (m_pSelectedPile) 
				{
					CRect MaxRect(m_pSelectedPile->GetPosition(),m_pSelectedPile->GetMaxSize());
					m_UpdateRect.UnionRect(m_UpdateRect,MaxRect);
				}
				CPileList	PileList;
				pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
				while (pos)
				{
					pPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
					//CRect PileRect(pPile->GetPosition(),pPile->GetSize());
					if (pPile == m_pSelectedPile) continue;
					CRect PileRect(pPile->GetPosition(),pPile->GetMaxSize());
					CRect TmpRect(0,0,0,0);
					PileRect.NormalizeRect();
					m_UpdateRect.NormalizeRect();
					TmpRect.IntersectRect(&PileRect,&m_UpdateRect);
					if (!TmpRect.IsRectEmpty())
					{
						TmpRect.InflateRect(2,2);
						m_UpdateRect.UnionRect(m_UpdateRect,TmpRect);
						PileList.AddTail(pPile);
						//TRACE("Get pile %p\n",pPile);
					}
				}
				m_UpdateRect.NormalizeRect();
				//m_UpdateRect.InflateRect(GRID/2,GRID/2);
				//TRACE("OnUpdate HINT_DISPLAY_MOVEPILE DelRect(%d,%d,%d,%d)\n",m_UpdateRect.left,m_UpdateRect.top,m_UpdateRect.right,m_UpdateRect.bottom);
				dc.LPtoDP(&m_UpdateRect);
				//m_UpdateRect.InflateRect(2,2);
				m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
				// alle betroffenen Piles zeichnen (in die Screen Bitmap)
				pos = PileList.GetHeadPosition();
				while (pos)
				{
					pPile = PileList.GetNext(pos);
					//pPile->Display(&dc,m_Dibs,TRUE,FALSE);
					if (pPile == m_pSelectedPile) continue;
					pPile->Display(&dc,m_Dibs,TRUE);
					//TRACE("Display pile %p\n",pPile);
				}
			}
			if (m_pSelectedPile) m_pSelectedPile->Display(&dc,m_Dibs,TRUE,FALSE);
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		}
		break;
    case HINT_DISPLAY_ALL:
		GetClientRect(m_UpdateRect);
		m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect);
		// alle Pile des Documents zeichnen (in die Screen Bitmap)
		pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
			pPile->Display(&dc,m_Dibs,TRUE,FALSE);
		}
		RedrawWindow(NULL,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		break;
	default:
		Invalidate();
		//TRACE("OnUpdate");
		break;
	}
	m_UpdateRect.SetRectEmpty();
}

void CSolEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//TRACE("OnLButtonDown\n");
	PREPARE_SOLEDIT_VIEW
	SetPointToGrid (&LogPoint);
	PositionToStatusBar(LogPoint);
	if (pDoc->m_pLayout->m_PileList.IsEmpty()) return;

	// zunächst muß festgestellt werden, ob bzw. welcher Pile angeklickt wurde
	if (!m_pSelectedPile)
		m_pSelectedPile = HitPileTest(LogPoint);
	else
		m_pSelectedPile->SetPosition(LogPoint);
	if (m_pSelectedPile)
	{
		OnUpdate(NULL,HINT_UPDATE_PILES,NULL);
		//OnUpdate(NULL,HINT_DISPLAY_PILE,(CObject*)m_pSelectedPile);
		CPoint CursorPoint(m_pSelectedPile->GetPosition());
		m_LastPoint = CursorPoint;
		dc.LPtoDP (&CursorPoint);
		ClientToScreen (&CursorPoint);
		SetCursorPos(CursorPoint.x,CursorPoint.y);
		return;
	}
	m_LastPoint = LogPoint;
}

void CSolEditView::ReportUserError()
{
	//PlaySound("Incorrec.wav",NULL,SND_FILENAME);
	PlaySound("Incorrec.wav",NULL,SND_SYNC | SND_FILENAME);
	
	//MessageBeep(MB_ICONHAND);
}


void CSolEditView::OnMouseMove(UINT nFlags, CPoint point) 
{
	PREPARE_SOLEDIT_VIEW
	SetPointToGrid (&LogPoint);
	PositionToStatusBar(LogPoint);
	if (pDoc->m_pLayout->m_PileList.IsEmpty()) return;

	// ist der linke Mousbutton gedrückt
	if (nFlags & MK_LBUTTON)
	{
		if (!m_PileMoving)
		{
			// wir müssen eventuell mit einem Verschieben beginnen
			if (m_pSelectedPile)
			{
				m_pSelectedPile->SetPosition(LogPoint);
				m_pSelectedPile->UpdateCardsPositions();
				m_PileMoving = TRUE;
				m_LastPoint = LogPoint;
				if (GetApplication()->m_ClipCursor)
					ClipCursorToView(&dc);
				//OnUpdate(this,HINT_UPDATE_PILES,NULL);
				//CPoint CursorPoint(LogPoint);
				//pDC->LPtoDP (&CursorPoint);
				//ClientToScreen (&CursorPoint);
				//SetCursorPos(CursorPoint.x,CursorPoint.y);
			}
		}
		else
		{
			// wir sind mitten beim Verschieben
			if (m_LastPoint != LogPoint)
			{
				m_UpdateRect.UnionRect(m_UpdateRect,
										CRect(m_pSelectedPile->GetPosition(),m_pSelectedPile->GetMaxSize()));
				m_pSelectedPile->Move(m_LastPoint,LogPoint,m_UpdateRect);
				m_UpdateRect.UnionRect(m_UpdateRect,
										CRect(m_pSelectedPile->GetPosition(),m_pSelectedPile->GetMaxSize()));
				//OnUpdate(NULL,HINT_UPDATE_PILES,NULL);
				//OnUpdate(this,HINT_UPDATE_PILES,NULL);
				m_LastPoint = LogPoint;
				//pDC->LPtoDP (&LogPoint);
				//ClientToScreen (&LogPoint);
				//SetCursorPos(LogPoint.x,LogPoint.y);
			}
		}
		OnUpdate(this,HINT_UPDATE_PILES,NULL);
	}
	m_LastPoint = LogPoint;
	CView::OnMouseMove(nFlags, point);
}

void CSolEditView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//TRACE("OnLButtonUp\n");
	PREPARE_SOLEDIT_VIEW
	SetPointToGrid (&LogPoint);
	PositionToStatusBar(LogPoint);
	if (pDoc->m_pLayout->m_PileList.IsEmpty()) return;

	// wird gerade verschoben ?
	if (m_PileMoving)
	{
		SetPointToGrid(&LogPoint);
		m_pSelectedPile->SetPosition(LogPoint);
		// Cursor wieder freigeben
		ClipCursorToView(&dc,TRUE);
		m_PileMoving = FALSE;
		m_pSelectedPile = NULL;
		pDoc->SetModifiedFlag();
	}
	OnUpdate(this,HINT_UPDATE_PILES,NULL);
	pDoc->UpdatePiles();
	m_LastPoint = LogPoint;

	CView::OnLButtonUp(nFlags, point);
}

void CSolEditView::SetPointToGrid(CPoint* pPoint)
{
	int diff;
	
	diff = pPoint->x % GRID;
	if ( diff > (GRID / 2) )
		pPoint->x += ( GRID - diff );
	else
		pPoint->x -= diff;
	
	diff = pPoint->y % GRID;
	if ( diff > (GRID / 2) )
		pPoint->y += ( GRID - diff );
	else
		pPoint->y -= diff;
	
	return;

}

void CSolEditView::ClipCursorToView(CDC *pDC, BOOL Release /* = FALSE */)
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

CPile* CSolEditView::HitPileTest(CPoint LogPoint)
{
	CSolEditDoc* pDoc = GetDocument();
	//ASSERT(!pDoc->m_PileList.IsEmpty());
	CPile* pTmpPile = NULL;
	POSITION pos;
	// alle Pile des Documents durchsuchen
	pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
	while (pos)
	{
		pTmpPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
		if (pTmpPile->HitTest(LogPoint))
			return pTmpPile;
	}
	return NULL;
}

void CSolEditView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//dprintf("ContextMenu");
	// zunächst muß festgestellt werden, ob bzw. welcher Pile angeklickt wurde
	CSolEditDoc* pDoc = GetDocument();
	CPoint TmpPoint(point);
	ScreenToClient(&TmpPoint);
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint LogPoint(TmpPoint);
	dc.DPtoLP(&LogPoint);
	m_ContextPoint = LogPoint;
	//if (pDoc->m_PileList.IsEmpty()) return;
	if (m_pSelectedPile) return;
	if ((m_pContextPile = HitPileTest(LogPoint)))
	{
		// ein Pile wurde angeklickt -> setze den Cursor auf die linke obere Ecke
		CMenu Menu;
		Menu.LoadMenu(IDR_SOLEDIT_PILECONTEXT);
		Menu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON ,point.x,point.y,AfxGetMainWnd());
	}
	else
	{
		CMenu Menu;
		Menu.LoadMenu(IDR_SOLEDIT_GAMECONTEXT);
		Menu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON ,point.x,point.y,AfxGetMainWnd());
	}
}

BOOL CSolEditView::UpdateBitmaps()
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CRect ClientRect; 
	GetClientRect(&ClientRect);
	
	m_pScreen->m_TransPoints.RemoveAll();
	m_pBackground->m_TransPoints.RemoveAll();
	// die Arrays mit den Transparentpunkten der Karten aufbauen
	CCard* pFirstCard = NULL;
	CSolEditDoc* pDoc = GetDocument();
	return TRUE;

}

BOOL CSolEditView::IsMainWndSizing()
{
	return ((CMainFrame*)AfxGetMainWnd())->m_MainWndIsSizing;
}

/////////////////////////////////////////////////////////////////////////////
// löscht eine CDib, falls vorhanden
//
void CSolEditView::Delete(CDib *pDib)
{
	if (pDib)
	{
		pDib->DeleteObject();
		delete pDib;
		pDib = NULL;
	}
}





void CSolEditView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	PREPARE_SOLEDIT_VIEW
	SetPointToGrid (&LogPoint);
	if (pDoc->m_pLayout->m_PileList.IsEmpty()) return;

/*
	//dprintf("LButtonDbl");
	CSolEditDoc* pDoc = GetDocument();
	ASSERT(!pDoc->m_PileList.IsEmpty());
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint LogPoint(point);
	dc.DPtoLP(&LogPoint);

	// wird gerade verschoben ?
	if (m_PileMoving)
	{
		SetPointToGrid(&LogPoint);
		m_pSelectedPile->SetPosition(LogPoint);
		// Cursor wieder freigeben
		ClipCursorToView(&dc,TRUE);
		m_PileMoving = FALSE;
		m_pSelectedPile = NULL;
		pDoc->SetModifiedFlag();
	}
	pDoc->UpdatePiles();
*/
	m_LastPoint = LogPoint;
	CView::OnLButtonDblClk(nFlags, point);
}

BOOL CSolEditView::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->wParam == VK_ESCAPE )
		return TRUE;
	
	return CView::PreTranslateMessage(pMsg);
}

void CSolEditView::OnSoleditpilecontextDelete() 
{
	CSolEditDoc* pDoc = GetDocument();
	ASSERT(!pDoc->m_pLayout->m_PileList.IsEmpty());
	ASSERT(m_pContextPile);
	POSITION pos = pDoc->m_pLayout->m_PileList.Find(m_pContextPile);
	pDoc->m_pLayout->m_PileList.RemoveAt(pos);
	delete m_pContextPile;
	m_pContextPile = NULL;
	OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
}

void CSolEditView::OnUpdateSoleditpilecontextDelete(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	
}

void CSolEditView::OnSoleditpilecontextProperties() 
{
	CSolEditDoc* pDoc = GetDocument();
	CPilePropertiesDialog PilePropertiesDlg;
	PilePropertiesDlg.m_pPile =	m_pContextPile;
	PilePropertiesDlg.m_PilePosition = m_pContextPile->GetPosition();
	PilePropertiesDlg.m_PileMaxSize = m_pContextPile->GetMaxSize();
	int OldIDDibEmpty = m_pContextPile->m_IDDibEmpty;
	PilePropertiesDlg.m_IDDibEmpty = m_pContextPile->m_IDDibEmpty;
	PilePropertiesDlg.m_OverlappMethod.LoadString(IDS_OLM_NOTAVAILABLE + (int)m_pContextPile->m_KindOfOverlapped);
	PilePropertiesDlg.m_SelectMethod.LoadString(IDS_SEL_NOTAVAILABLE + (int)m_pContextPile->m_KindOfSelecting);
	PilePropertiesDlg.m_PilingMethod.LoadString(IDS_PIL_NOTAVAILABLE + (int)m_pContextPile->m_KindOfPiling);
	PilePropertiesDlg.m_FaceAtStart.LoadString(IDS_FAS_NOTAVAILABLE + (int)m_pContextPile->m_FaceAtStart);
	PilePropertiesDlg.m_PutIfEmpty.LoadString(IDS_PIE_NOTAVAILABLE + (int)m_pContextPile->m_PutIfEmpty);
	PilePropertiesDlg.m_NumberOnStart	= m_pContextPile->m_NumberOnStart;
	PilePropertiesDlg.m_NumberHiddenOnStart	= m_pContextPile->m_NumberHiddenOnStart;
	PilePropertiesDlg.m_NumberPerMove	= m_pContextPile->m_NumberPerMove;
	PilePropertiesDlg.m_MaxCards		= m_pContextPile->m_MaxCards;
	PilePropertiesDlg.m_DealOrder		= m_pContextPile->m_DealOrder;
	PilePropertiesDlg.m_RedealOrder		= m_pContextPile->m_RedealOrder;
	PilePropertiesDlg.m_DealTo.LoadString(IDS_DEAL_NOTAVAILABLE + (int)m_pContextPile->m_DealTo);
	PilePropertiesDlg.m_NumberPerDeal = m_pContextPile->m_NumberPerDeal;
	PilePropertiesDlg.m_PermittedPlay.LoadString(IDS_PPP_NOTAVAILABLE + (int)m_pContextPile->m_PermittedPlay);
	PilePropertiesDlg.m_UpdateOverlap	= m_pContextPile->m_UpdateOverlap;
	PilePropertiesDlg.m_SpecialHandling = m_pContextPile->m_SpecialHandling;
	PilePropertiesDlg.m_SpecialHandlingText = m_pContextPile->m_SpecialHandlingText;
	// für dieses Flag wird die Einstellung des letzten Aufrufes übernommen
	PilePropertiesDlg.m_UseForAllPilesOfThisType = m_OldUseForAllPilesOfThisType;
	
	if (m_pContextPile->IsK(CStock))
	{
		PilePropertiesDlg.m_NumberOfRedeal = ((CStock*)m_pContextPile)->m_NumberOfRedeal;
		PilePropertiesDlg.m_RedealMethod.LoadString(IDS_REDEAL_NOTAVAILABLE + (int)((CStock*)m_pContextPile)->m_RedealMethod);
		//PilePropertiesDlg.m_DealTo.LoadString(IDS_DEAL_NOTAVAILABLE + (int)((CStock*)m_pContextPile)->m_DealTo);
		//PilePropertiesDlg.m_NumberPerDeal = ((CStock*)m_pContextPile)->m_NumberPerDeal;
	}

	if (m_pContextPile->IsK(CFoundation))
	{
		PilePropertiesDlg.m_Replayable = ((CFoundation*)m_pContextPile)->m_Replayable;
		PilePropertiesDlg.m_BasicCardOnlyVal = ((CFoundation*)m_pContextPile)->m_BasicCardOnlyVal;
		PilePropertiesDlg.m_BasicCardOnlyColor = ((CFoundation*)m_pContextPile)->m_BasicCardOnlyColor;
		if (((CFoundation*)m_pContextPile)->m_BasicCardValue == 0)
			PilePropertiesDlg.m_IDDibBasicCard = IDB_EMPTY_CARD;
		else
			PilePropertiesDlg.m_IDDibBasicCard = IDB_START_CARD - 1 + ((CFoundation*)m_pContextPile)->m_BasicCardValue;
		PilePropertiesDlg.m_BasicCardMethod.LoadString(IDS_BAS_NOTAVAILABLE + (int)((CFoundation*)m_pContextPile)->m_KindOfBasicCard);
	}

	if (PilePropertiesDlg.DoModal() == IDOK)
	{
		// für den selektierten Pile wurden alle Werte übernommen
		// wir merken uns das Flag für den nächsten Aufruf
		m_OldUseForAllPilesOfThisType = PilePropertiesDlg.m_UseForAllPilesOfThisType;
		// sollen die Eigenschaften für alle Piles dieses Typs übernommen werden ?
		if (PilePropertiesDlg.m_UseForAllPilesOfThisType)
		{
			CPile* pPile = NULL;
			POSITION pos;
			// alle Pile des Documents durchsuchen
			pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
				if (pPile == m_pContextPile) continue;
				if (pPile->GetRuntimeClass() == m_pContextPile->GetRuntimeClass())
				{
					// allgemeine Werte übernehmen
					pPile->m_IDDibEmpty			= m_pContextPile->m_IDDibEmpty;			
					pPile->m_KindOfOverlapped	= m_pContextPile->m_KindOfOverlapped;	
					pPile->m_KindOfSelecting	= m_pContextPile->m_KindOfSelecting;	
					pPile->m_KindOfPiling		= m_pContextPile->m_KindOfPiling;		
					pPile->m_FaceAtStart		= m_pContextPile->m_FaceAtStart;		
					pPile->m_NumberOnStart		= m_pContextPile->m_NumberOnStart;		
					pPile->m_NumberHiddenOnStart= m_pContextPile->m_NumberHiddenOnStart;
					pPile->m_NumberPerMove		= m_pContextPile->m_NumberPerMove;		
					pPile->m_PutIfEmpty			= m_pContextPile->m_PutIfEmpty;			
					pPile->m_MaxCards			= m_pContextPile->m_MaxCards;			
					pPile->m_DealOrder			= m_pContextPile->m_DealOrder;			
					pPile->m_RedealOrder		= m_pContextPile->m_RedealOrder;			
					pPile->m_DealTo				= m_pContextPile->m_DealTo;		
					pPile->m_NumberPerDeal		= m_pContextPile->m_NumberPerDeal ;
					pPile->m_PermittedPlay		= m_pContextPile->m_PermittedPlay;	
					pPile->m_UpdateOverlap		= m_pContextPile->m_UpdateOverlap;
					pPile->m_SpecialHandling	= m_pContextPile->m_SpecialHandling;
					pPile->m_SpecialHandlingText= m_pContextPile->m_SpecialHandlingText;


					if (pPile->IsK(CStock) == m_pContextPile->IsK(CStock))
					{
						((CStock*)pPile)->m_NumberOfRedeal = ((CStock*)m_pContextPile)->m_NumberOfRedeal ;
						((CStock*)pPile)->m_RedealMethod	= ((CStock*)m_pContextPile)->m_RedealMethod;		
						//((CStock*)pPile)->m_DealTo		= ((CStock*)m_pContextPile)->m_DealTo;		
						//((CStock*)pPile)->m_NumberPerDeal = ((CStock*)m_pContextPile)->m_NumberPerDeal ;
					}
					if (pPile->IsK(CFoundation) == m_pContextPile->IsK(CFoundation))
					{
						((CFoundation*)pPile)->m_Replayable		= ((CFoundation*)m_pContextPile)->m_Replayable;
						((CFoundation*)pPile)->m_BasicCardOnlyVal = ((CFoundation*)m_pContextPile)->m_BasicCardOnlyVal; 
						((CFoundation*)pPile)->m_BasicCardOnlyColor = ((CFoundation*)m_pContextPile)->m_BasicCardOnlyColor; 
						((CFoundation*)pPile)->m_BasicCardValue	= ((CFoundation*)m_pContextPile)->m_BasicCardValue;	
						((CFoundation*)pPile)->m_KindOfBasicCard	= ((CFoundation*)m_pContextPile)->m_KindOfBasicCard;	
					}
				}
			}
		}

		pDoc->SetModifiedFlag();
	}	

	if (OldIDDibEmpty != m_pContextPile->m_IDDibEmpty)
	{
		m_pContextPile->m_dibEmpty.Detach();
		m_pContextPile->m_dibEmpty.LoadBitmap(m_pContextPile->m_IDDibEmpty);
	}
	UpdateBitmaps();
	OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
}

void CSolEditView::OnUpdateSoleditpilecontextProperties(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	
}

void CSolEditView::OnSoleditgamecontextProperties() 
{
	CSolEditDoc* pDoc = GetDocument();
	CGamePropertiesDialog	GamePropertiesDlg(this);
	GamePropertiesDlg.m_pDoc = pDoc;
	GamePropertiesDlg.m_GameName		= pDoc->m_pLayout->m_GameName;
	GamePropertiesDlg.m_GroupType		= pDoc->m_pLayout->m_GroupType;
	GamePropertiesDlg.m_NumberOfDecks	= pDoc->m_pLayout->m_NumberOfDecks;
	GamePropertiesDlg.m_strTypeOfScoring.LoadString(IDS_SCORE_NOTAVAILABLE + (int)pDoc->m_pLayout->m_TypeOfScoring);
	if (GamePropertiesDlg.DoModal() == IDOK)
	{
		// Werte wurden übernommen
		pDoc->SetModifiedFlag();
	}
}

void CSolEditView::OnUpdateSoleditgamecontextProperties(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	
}


void CSolEditView::OnInsertFoundation() 
{
	CSolEditDoc* pDoc = GetDocument();
	CPile* pPile =  new CFoundation;
	pDoc->InitFoundationPile((CFoundation*)pPile);
	pDoc->m_pLayout->m_PileList.AddTail(pPile);
	UpdateBitmaps();
	m_pSelectedPile = pPile;
	pDoc->SetModifiedFlag();
}

void CSolEditView::OnUpdateInsertPile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((BOOL)!m_pSelectedPile);
}

void CSolEditView::OnInsertTableau() 
{
	CSolEditDoc* pDoc = GetDocument();
	CPile* pPile =  new CTableau;
	pDoc->InitTableauPile((CTableau*)pPile);
	pDoc->m_pLayout->m_PileList.AddTail(pPile);
	UpdateBitmaps();
	m_pSelectedPile = pPile;
	pDoc->SetModifiedFlag();
}

void CSolEditView::OnInsertWaste() 
{
	CSolEditDoc* pDoc = GetDocument();
	CPile* pPile =  new CWaste;
	pDoc->InitWastePile((CWaste*)pPile);
	pDoc->m_pLayout->m_PileList.AddTail(pPile);
	UpdateBitmaps();
	m_pSelectedPile = pPile;
	pDoc->SetModifiedFlag();
}

void CSolEditView::OnInsertStock() 
{
	CSolEditDoc* pDoc = GetDocument();
	if (pDoc->GetStock())
	{
		AfxMessageBox(IDS_ONLY_ONE_STOCK_ALLOWED);
		return;
	}
	CPile* pPile =  new CStock;
	pDoc->InitStockPile((CStock*)pPile);
	pDoc->m_pLayout->m_PileList.AddHead(pPile);
	UpdateBitmaps();
	m_pSelectedPile = pPile;
	pDoc->SetModifiedFlag();
}


void CSolEditView::PositionToStatusBar(CPoint LPPoint)
{
	m_Position = LPPoint;
	CSolEditDoc* pDoc = GetDocument();
	CString strPosition("");
	strPosition.Format("(%d,%d)",LPPoint.x,LPPoint.y);
	::SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_SCORE, (long)&strPosition);
	//((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(4,(LPCSTR) strPosition);      
}

void CSolEditView::OnUpdatePanePosition(CCmdUI *pCmdUI) 
{
    pCmdUI->Enable(); 
	CString strPosition("");
	strPosition.Format("(%d,%d)",m_Position.x,m_Position.y);
    pCmdUI->SetText( strPosition ); 
}


void CSolEditView::OnSoleditpilecontextCopy() 
{
	CSolEditDoc* pDoc = GetDocument();
	CPile* pPile = NULL;
	if (m_pContextPile->IsK(CStock))
	{
		if (pDoc->GetStock())
		{
			AfxMessageBox(IDS_ONLY_ONE_STOCK_ALLOWED);
			return;
		}
		pPile =  new CStock((CStock&)*m_pContextPile);
	}
	if (m_pContextPile->IsK(CFoundation))
		pPile =  new CFoundation((CFoundation&)*m_pContextPile);
	if (m_pContextPile->IsK(CTableau))
		pPile =  new CTableau((CTableau&)*m_pContextPile);
	if (m_pContextPile->IsK(CWaste))
		pPile =  new CWaste((CWaste&)*m_pContextPile);
	pDoc->m_pLayout->m_PileList.AddHead(pPile);
	UpdateBitmaps();
	m_pSelectedPile = pPile;
}

void CSolEditView::SetDibs(CDib* pCardBackDib) 
{
	m_Dibs.pScreenDib = m_pScreen;
	m_Dibs.pBackgroundDib = m_pBackground;
	m_Dibs.pCardBackDib = pCardBackDib;
	m_Dibs.pCardSelectedDib = &m_dibCardSelected;
}


void CSolEditView::OnSoleditpilecontextMaxrectmaximal() 
{
	CSolEditDoc* pDoc = GetDocument();
	ASSERT(!pDoc->m_pLayout->m_PileList.IsEmpty());
	ASSERT(m_pContextPile);
	if (!m_pContextPile->IsK(CTableau) && !m_pContextPile->IsK(CWaste) && !m_pContextPile->IsK(CStock))
		return;
	//CRect MaxRect(m_pContextPile->GetPosition(),m_pContextPile->m_MaxSize);
	CRect MaxRect(m_pContextPile->GetPosition(),CCyberSolitaireApp::DefaultCardSize);
	CRect TableRectLP(0,0,TABLE_CX,TABLE_CY);
	POSITION pos = NULL;
	CPile* pPile;
	BOOL PileFound = FALSE;
	switch (m_pContextPile->m_KindOfOverlapped) {
	case olmNotOverlapped:
		// nichts zu tun
		return;
	case olmDownOverlapped:
		{
			PileFound = FALSE;
			pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
			//CClientDC dc(this);
			//OnPrepareDC(&dc);
			while (pos)
			{
				pPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
				if (pPile == m_pContextPile)
					continue;
				CRect PileRect(pPile->GetPosition(),pPile->GetSize());
				CRect TmpRect(0,0,0,0);
				PileRect.NormalizeRect();
				MaxRect.NormalizeRect();
				// kann der m_pContextPile in diesen überhaupt hineinwachsen ?
				if ((PileRect.top   >= MaxRect.bottom) && 
					((PileRect.left  <= MaxRect.left)  &&	(PileRect.right >= MaxRect.left) ||
					 (PileRect.left  <= MaxRect.right) &&	(PileRect.right >= MaxRect.right)))
				{
					PileFound = TRUE;
					while (!TmpRect.IntersectRect(&PileRect,&MaxRect) && (MaxRect.bottom < TableRectLP.bottom))
					{
						MaxRect.bottom += 2 * GRID;
					}
					MaxRect.bottom -= 4 * GRID;
					m_pContextPile->m_MaxSize = MaxRect.Size();
					pDoc->SetModifiedFlag();
					break;
				}
			}
			if (!PileFound)
			{
				MaxRect.bottom = TableRectLP.bottom - 2 * GRID;
				m_pContextPile->m_MaxSize = MaxRect.Size();
				pDoc->SetModifiedFlag();
			}
		break;
		}
	case olmRigthOverlapped:
		{
			PileFound = FALSE;
			pos = pDoc->m_pLayout->m_PileList.GetHeadPosition();
			//CClientDC dc(this);
			//OnPrepareDC(&dc);
			while (pos)
			{
				pPile = pDoc->m_pLayout->m_PileList.GetNext(pos);
				if (pPile == m_pContextPile)
					continue;
				CRect PileRect(pPile->GetPosition(),pPile->GetSize());
				CRect TmpRect(0,0,0,0);
				PileRect.NormalizeRect();
				MaxRect.NormalizeRect();
				// kann der m_pContextPile in diesen überhaupt hineinwachsen ?
				if ((PileRect.left   >= MaxRect.right) && 
					((PileRect.top   <= MaxRect.top)   &&	(PileRect.bottom >= MaxRect.top) ||
					 (PileRect.top   <= MaxRect.bottom)&&	(PileRect.bottom >= MaxRect.bottom)))
				{
					PileFound = TRUE;
					while (!TmpRect.IntersectRect(&PileRect,&MaxRect) && (MaxRect.bottom < TableRectLP.bottom))
					{
						MaxRect.right += 2 * GRID;
					}
					MaxRect.right -= 4 * GRID;
					m_pContextPile->m_MaxSize = MaxRect.Size();
					pDoc->SetModifiedFlag();
					break;
				}
			}
			if (!PileFound)
			{
				MaxRect.bottom = TableRectLP.bottom - 2 * GRID;
				m_pContextPile->m_MaxSize = MaxRect.Size();
				pDoc->SetModifiedFlag();
			}
		break;
		}
	default:
		ASSERT(0);
		break;
	}

	m_pContextPile = NULL;
	OnUpdate(this,HINT_DISPLAY_ALL,NULL);
}

void CSolEditView::OnSoleditpilecontextMaxrectinkrementieren() 
{
	CSolEditDoc* pDoc = GetDocument();
	ASSERT(!pDoc->m_pLayout->m_PileList.IsEmpty());
	ASSERT(m_pContextPile);
	if (!m_pContextPile->IsK(CTableau) && !m_pContextPile->IsK(CWaste) && !m_pContextPile->IsK(CStock))
		return;
	CRect MaxRect(m_pContextPile->GetPosition(),m_pContextPile->m_MaxSize);
	switch (m_pContextPile->m_KindOfOverlapped) {
	case olmNotOverlapped:
		// nichts zu tun
		return;
	case olmDownOverlapped:
		MaxRect.bottom += 2 * GRID;
		break;
	case olmRigthOverlapped:
		MaxRect.right += 2 * GRID;
		break;
	default:
		ASSERT(0);
		break;
	}
	m_pContextPile->m_MaxSize = MaxRect.Size();
	pDoc->SetModifiedFlag();
	m_pContextPile = NULL;
	OnUpdate(this,HINT_DISPLAY_ALL,NULL);
}

void CSolEditView::OnSoleditpilecontextMaxrectdekrementieren() 
{
	CSolEditDoc* pDoc = GetDocument();
	ASSERT(!pDoc->m_pLayout->m_PileList.IsEmpty());
	ASSERT(m_pContextPile);
	if (!m_pContextPile->IsK(CTableau) && !m_pContextPile->IsK(CWaste) && !m_pContextPile->IsK(CStock))
		return;
	CRect MaxRect(m_pContextPile->GetPosition(),m_pContextPile->m_MaxSize);
	switch (m_pContextPile->m_KindOfOverlapped) {
	case olmNotOverlapped:
		// nichts zu tun
		return;
	case olmDownOverlapped:
		MaxRect.bottom -= 2 * GRID;
		break;
	case olmRigthOverlapped:
		MaxRect.right -= 2 * GRID;
		break;
	default:
		ASSERT(0);
		break;
	}
	m_pContextPile->m_MaxSize = MaxRect.Size();
	pDoc->SetModifiedFlag();
	m_pContextPile = NULL;
	OnUpdate(this,HINT_DISPLAY_ALL,NULL);
}

void CSolEditView::OnSoleditpilecontextMaxrectloeschen()
{
	CSolEditDoc* pDoc = GetDocument();
	ASSERT(!pDoc->m_pLayout->m_PileList.IsEmpty());
	ASSERT(m_pContextPile);
	m_pContextPile->m_MaxSize = CCyberSolitaireApp::DefaultCardSize;
	pDoc->SetModifiedFlag();
	m_pContextPile = NULL;
	OnUpdate(this,HINT_DISPLAY_ALL,NULL);
}

void CSolEditView::OnDateiSpeichern() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	//CDib MyDib;
	//MyDib.Load("MYBITMAP.BMP");
	m_pScreen->Write("Versuch.bmp");

}

void CSolEditView::OnTTTLevelpunkte()
{
	// TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
	CSolEditDoc* pDoc = GetDocument();
	CInputLevelPointsDialog	InputLevelPointsDlg;

	// für Test
	/*
	if (pDoc->m_pLayout->m_LevelPointArray.GetSize() != 9)
	{
		pDoc->m_pLayout->m_LevelPointArray.SetSize(9);
		pDoc->SetModifiedFlag();
	}
	*/

	InputLevelPointsDlg.m_Level1_Points = pDoc->m_pLayout->m_LevelPointArray[0];
	InputLevelPointsDlg.m_Level2_Points = pDoc->m_pLayout->m_LevelPointArray[1];
	InputLevelPointsDlg.m_Level3_Points = pDoc->m_pLayout->m_LevelPointArray[2];
	InputLevelPointsDlg.m_Level4_Points = pDoc->m_pLayout->m_LevelPointArray[3];
	InputLevelPointsDlg.m_Level5_Points = pDoc->m_pLayout->m_LevelPointArray[4];
	InputLevelPointsDlg.m_Level6_Points = pDoc->m_pLayout->m_LevelPointArray[5];
	InputLevelPointsDlg.m_Level7_Points = pDoc->m_pLayout->m_LevelPointArray[6];
	InputLevelPointsDlg.m_Level8_Points = pDoc->m_pLayout->m_LevelPointArray[7];
	InputLevelPointsDlg.m_Level9_Points = pDoc->m_pLayout->m_LevelPointArray[8];

	if (InputLevelPointsDlg.DoModal() == IDOK)
	{
		pDoc->m_pLayout->m_LevelPointArray[0] = InputLevelPointsDlg.m_Level1_Points;
		pDoc->m_pLayout->m_LevelPointArray[1] = InputLevelPointsDlg.m_Level2_Points;
		pDoc->m_pLayout->m_LevelPointArray[2] = InputLevelPointsDlg.m_Level3_Points;
		pDoc->m_pLayout->m_LevelPointArray[3] = InputLevelPointsDlg.m_Level4_Points;
		pDoc->m_pLayout->m_LevelPointArray[4] = InputLevelPointsDlg.m_Level5_Points;
		pDoc->m_pLayout->m_LevelPointArray[5] = InputLevelPointsDlg.m_Level6_Points;
		pDoc->m_pLayout->m_LevelPointArray[6] = InputLevelPointsDlg.m_Level7_Points;
		pDoc->m_pLayout->m_LevelPointArray[7] = InputLevelPointsDlg.m_Level8_Points;
		pDoc->m_pLayout->m_LevelPointArray[8] = InputLevelPointsDlg.m_Level9_Points;
		pDoc->SetModifiedFlag();
	}
}
