// Piles.cpp: Implementierung der Klasse CPile.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "CyberSolitaireView.h"
#include "SolEditView.h"
#include "Piles.h"
#include "Do.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CPile, CObject, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(CStock, CPile, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(CFoundation, CPile, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(CTableau, CPile, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(CWaste, CPile, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(CMovePile, CPile, VERSIONABLE_SCHEMA | 1)


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CPile::CPile()
{
	m_PileVersion			= PILE_VERSION;
	m_Position				= NoPosition;
	m_Size					= NoSize;
	m_OverlappedFactor		= CCyberSolitaireApp::DefaultOverlappedFactor;
	m_IDDibEmpty			= -1;
	m_pdibEmpty				= NULL;
	m_KindOfOverlapped		= olmNotAvailable;
	m_KindOfSelecting		= selNotAvailable;
	m_KindOfPiling			= pilNotAvailable;
	m_FaceAtStart			= fasNotAvailable;
	m_NumberOnStart			= -1;
	m_NumberHiddenOnStart	= -1;
	m_NumberPerMove			= -1;
	m_PutIfEmpty			= pieNotAvailable;
	m_MaxCards				= -1;
	m_MaxSize				= CCyberSolitaireApp::DefaultCardSize;
	m_DealOrder				= 0;
	m_RedealOrder			= 0;
	m_DealTo				= dealNotAvailable;
	m_NumberPerDeal			= -1;
	m_PermittedPlay			= pppNotAvailable;
	m_UpdateOverlap			= TRUE;
	m_SpecialHandling		= FALSE;
	m_SpecialHandlingText	= "";
}

CPile::CPile(const CPile& p) 
{
	m_PileVersion			= p.m_PileVersion;
	ASSERT(m_Cards.IsEmpty());
	m_Cards					= p.m_Cards;
	m_Position				= p.m_Position;
	m_Size					= p.m_Size;				
	m_IDDibEmpty			= p.m_IDDibEmpty; 
	m_KindOfOverlapped		= p.m_KindOfOverlapped;	
	m_KindOfSelecting		= p.m_KindOfSelecting;	
	m_KindOfPiling			= p.m_KindOfPiling;		
	m_FaceAtStart			= p.m_FaceAtStart;		
	m_NumberOnStart			= p.m_NumberOnStart;	
	m_NumberHiddenOnStart	= p.m_NumberHiddenOnStart;	
	m_NumberPerMove			= p.m_NumberPerMove;
	m_PutIfEmpty			= p.m_PutIfEmpty;
	m_MaxCards				= p.m_MaxCards;
	m_MaxSize				= p.m_MaxSize;
	m_DealOrder				= p.m_DealOrder;
	m_RedealOrder			= p.m_RedealOrder;
	m_DealTo				= p.m_DealTo;
	m_NumberPerDeal			= p.m_NumberPerDeal;
	m_PermittedPlay			= p.m_PermittedPlay;
	m_UpdateOverlap			= p.m_UpdateOverlap;	
	m_SpecialHandling		= p.m_SpecialHandling;	
	m_SpecialHandlingText	= p.m_SpecialHandlingText;
	m_OverlappedFactor		= p.m_OverlappedFactor;
	
	m_dibEmpty.Load(AfxGetApp()->m_hInstance,p.m_IDDibEmpty);
	// die CDib, die eventuell in dem Original schon enthalten ist
	// wird nicht mit kopiert oder neu erzeugt. 
	// sollte dieser Stapel als leer einmal angezeigt werden müssen,
	// so wird die Bitmap neu erzeugt
	m_pdibEmpty				= NULL;
}

CPile::~CPile()
{
	if (m_pdibEmpty)
	{
		delete m_pdibEmpty;
		m_pdibEmpty = NULL;
	}
}

const CPile& CPile::operator =(const CPile& p)
{
	m_PileVersion		= p.m_PileVersion;
	ASSERT(m_Cards.IsEmpty());
	m_Cards					= p.m_Cards;
	m_Position				= p.m_Position;
	m_Size					= p.m_Size;				
	m_IDDibEmpty			= p.m_IDDibEmpty; 
	m_KindOfOverlapped		= p.m_KindOfOverlapped;	
	m_KindOfSelecting		= p.m_KindOfSelecting;	
	m_KindOfPiling			= p.m_KindOfPiling;		
	m_FaceAtStart			= p.m_FaceAtStart;		
	m_NumberOnStart			= p.m_NumberOnStart;	
	m_NumberHiddenOnStart	= p.m_NumberHiddenOnStart;	
	m_NumberPerMove			= p.m_NumberPerMove;
	m_PutIfEmpty			= p.m_PutIfEmpty;
	m_MaxCards				= p.m_MaxCards;
	m_MaxSize				= p.m_MaxSize;
	m_DealOrder				= p.m_DealOrder;
	m_RedealOrder			= p.m_RedealOrder;
	m_DealTo				= p.m_DealTo;
	m_NumberPerDeal			= p.m_NumberPerDeal;
	m_PermittedPlay			= p.m_PermittedPlay;
	m_UpdateOverlap			= p.m_UpdateOverlap;	
	m_SpecialHandling		= p.m_SpecialHandling;	
	m_SpecialHandlingText	= p.m_SpecialHandlingText;
	m_OverlappedFactor		= p.m_OverlappedFactor;
	
	m_dibEmpty.Load(AfxGetApp()->m_hInstance,p.m_IDDibEmpty);
	return *this;
}

const CPile& CPile::operator =(const CPile* p)
{
	m_PileVersion		= p->m_PileVersion;
	ASSERT(m_Cards.IsEmpty());
	m_Cards					= p->m_Cards;
	m_Position				= p->m_Position;
	m_Size					= p->m_Size;	
	m_IDDibEmpty			= p->m_IDDibEmpty; 
	m_KindOfOverlapped		= p->m_KindOfOverlapped;	
	m_KindOfSelecting		= p->m_KindOfSelecting;	
	m_KindOfPiling			= p->m_KindOfPiling;		
	m_FaceAtStart			= p->m_FaceAtStart;		
	m_NumberOnStart			= p->m_NumberOnStart;	
	m_NumberHiddenOnStart	= p->m_NumberHiddenOnStart;	
	m_NumberPerMove			= p->m_NumberPerMove;
	m_PutIfEmpty			= p->m_PutIfEmpty;
	m_MaxCards				= p->m_MaxCards;
	m_MaxSize				= p->m_MaxSize;
	m_DealOrder				= p->m_DealOrder;
	m_RedealOrder			= p->m_RedealOrder;
	m_DealTo				= p->m_DealTo;
	m_NumberPerDeal			= p->m_NumberPerDeal;
	m_PermittedPlay			= p->m_PermittedPlay;
	m_UpdateOverlap			= p->m_UpdateOverlap;	
	m_SpecialHandling		= p->m_SpecialHandling;	
	m_SpecialHandlingText	= p->m_SpecialHandlingText;
	m_OverlappedFactor		= p->m_OverlappedFactor;

	m_dibEmpty.Load(AfxGetApp()->m_hInstance,p->m_IDDibEmpty);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Diagnose

#ifdef _DEBUG
void CPile::AssertValid() const
{
	CObject::AssertValid();
}

void CPile::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// diese Funktion entnimmt dem Kartenstapel (this) ab der Karte pCard oder alle
// Karten (bei pCard == NULL) bis zum Ende und transportiert sie an das Ende von pDstPile
//
void CPile::Put(CPile *pDstPile, CCard *pCard, CRect& UpdateRect)
{
	//TRACE("Put from %s(%p) to %s(%p)\n",GetRuntimeClass()->m_lpszClassName,this,pDstPile->GetRuntimeClass()->m_lpszClassName,pDstPile);
	//GetApplication()->LogMessage("Put from %s(%p) to %s(%p)\n",GetRuntimeClass()->m_lpszClassName,this,
	//		pDstPile->GetRuntimeClass()->m_lpszClassName,pDstPile);
	CCard* pTmpCard;
	POSITION pos = NULL;
	POSITION lastpos = NULL;
	if (m_Cards.IsEmpty())
	{
		//TRACE("%s(%p) is empty\n",GetRuntimeClass()->m_lpszClassName,this);
		//GetApplication()->LogMessage("%s(%p) is empty\n",GetRuntimeClass()->m_lpszClassName,this);
		return;
	}
	if (pCard)	
		pos = m_Cards.Find(pCard);
	else
		pos = m_Cards.GetHeadPosition();
	ASSERT(pos);
	// hänge alle Karten an den Schwanz von pDstPile
	while (pos)
	{
		lastpos = pos;
		pTmpCard = m_Cards.GetNext(pos);
		pDstPile->m_Cards.AddTail(pTmpCard);
		//TRACE("\tAdd to %s(%p):%s\n",pDstPile->GetRuntimeClass()->m_lpszClassName,this,CardName);
		//GetApplication()->LogMessage("\tAdd to %s(%p):%s\n",pDstPile->GetRuntimeClass()->m_lpszClassName,
		//	pDstPile,pTmpCard->GetCardName());
		m_Cards.RemoveAt(lastpos);
	}
	// durch Neuberechnung des Overlappedfactors kann der Pile sogar größer werden
	UpdateRect.UnionRect(UpdateRect,pDstPile->GetRect());
	UpdateRect.UnionRect(UpdateRect,GetRect());
	UpdateCardsPositions();
	pDstPile->UpdateCardsPositions();
	UpdateRect.UnionRect(UpdateRect,pDstPile->GetRect());
	UpdateRect.UnionRect(UpdateRect,GetRect());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion entnimmt dem Kartenstapel (this) ab der Karte pCard oder alle
// Karten (bei pCard == NULL) bis zum Ende und transportiert sie an das Ende von pDstPile
//
/*
void CPile::Put(CPile *pDstPile, CCard *pCard, CListRect* pUpdateRectList)
{
	//TRACE("Put from %s(%p) to %s(%p)\n",GetRuntimeClass()->m_lpszClassName,this,pDstPile->GetRuntimeClass()->m_lpszClassName,pDstPile);
	//GetApplication()->LogMessage("Put from %s(%p) to %s(%p)\n",GetRuntimeClass()->m_lpszClassName,this,
	//		pDstPile->GetRuntimeClass()->m_lpszClassName,pDstPile);
	CCard* pTmpCard;
	POSITION pos = NULL;
	POSITION lastpos = NULL;
	if (m_Cards.IsEmpty())
	{
		//TRACE("%s(%p) is empty\n",GetRuntimeClass()->m_lpszClassName,this);
		//GetApplication()->LogMessage("%s(%p) is empty\n",GetRuntimeClass()->m_lpszClassName,this);
		return;
	}
	if (pCard)	
		pos = m_Cards.Find(pCard);
	else
		pos = m_Cards.GetHeadPosition();
	ASSERT(pos);
	// hänge alle Karten an den Schwanz von pDstPile
	while (pos)
	{
		lastpos = pos;
		pTmpCard = m_Cards.GetNext(pos);
		pDstPile->m_Cards.AddTail(pTmpCard);
		//TRACE("\tAdd to %s(%p):%s\n",pDstPile->GetRuntimeClass()->m_lpszClassName,this,CardName);
		//GetApplication()->LogMessage("\tAdd to %s(%p):%s\n",pDstPile->GetRuntimeClass()->m_lpszClassName,
		//	pDstPile,pTmpCard->GetCardName());
		m_Cards.RemoveAt(lastpos);
	}
	// durch Neuberechnung des Overlappedfactors kann der Pile sogar größer werden
	pUpdateRectList->AddTail(pDstPile->GetRect());
	pUpdateRectList->AddTail(GetRect());
	UpdateCardsPositions();
	pDstPile->UpdateCardsPositions();
	pUpdateRectList->AddTail(pDstPile->GetRect());
	pUpdateRectList->AddTail(GetRect());
}
*/

/////////////////////////////////////////////////////////////////////////////
// diese Funktion entnimmt dem Kartenstapel (pSrcPile) ab der Karte pCard oder alle
// Karten (bei pCard == NULL) bis zum Ende und transportiert sie an das Ende von this
//
void CPile::Get(CPile *pSrcPile, CCard *pCard, CRect& UpdateRect)
{
	//TRACE("Get from %s(%p) to %s(%p)\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile,GetRuntimeClass()->m_lpszClassName,this);
	//GetApplication()->LogMessage("Get from %s(%p) to %s(%p)\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile,
	//									GetRuntimeClass()->m_lpszClassName,this);
	CCard* pTmpCard;
	POSITION pos = NULL;
	POSITION lastpos = NULL;
	if(pSrcPile->m_Cards.IsEmpty())
	{
		//TRACE("%s(%p) is empty\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile);
		//GetApplication()->LogMessage("%s(%p) is empty\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile);
		return;
	}
	if (pCard)	
		pos = pSrcPile->m_Cards.Find(pCard);
	else
		pos = pSrcPile->m_Cards.GetHeadPosition();
	// hänge alle Karten an den Schwanz von this
	while (pos)
	{
		lastpos = pos;
		pTmpCard = pSrcPile->m_Cards.GetNext(pos);
		m_Cards.AddTail(pTmpCard);
		//TRACE("\tAdd to %s(%p):%s\n",GetRuntimeClass()->m_lpszClassName,this,CardName);
		//GetApplication()->LogMessage("\tAdd to %s(%p):%s\n",GetRuntimeClass()->m_lpszClassName,
		//	this,pTmpCard->GetCardName());
		pSrcPile->m_Cards.RemoveAt(lastpos);
	}
	// durch Neuberechnung des Overlappedfactors kann der Pile sogar größer werden
	UpdateRect.UnionRect(UpdateRect,pSrcPile->GetRect());
	UpdateRect.UnionRect(UpdateRect,GetRect());
	UpdateCardsPositions();
	pSrcPile->UpdateCardsPositions();
	UpdateRect.UnionRect(UpdateRect,pSrcPile->GetRect());
	UpdateRect.UnionRect(UpdateRect,GetRect());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion entnimmt dem Kartenstapel (pSrcPile) ab der Karte pCard oder alle
// Karten (bei pCard == NULL) bis zum Ende und transportiert sie an das Ende von this
//
/*
void CPile::Get(CPile *pSrcPile, CCard *pCard, CListRect* pUpdateRectList)
{
	//TRACE("Get from %s(%p) to %s(%p)\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile,GetRuntimeClass()->m_lpszClassName,this);
	//GetApplication()->LogMessage("Get from %s(%p) to %s(%p)\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile,
	//									GetRuntimeClass()->m_lpszClassName,this);
	CCard* pTmpCard;
	POSITION pos = NULL;
	POSITION lastpos = NULL;
	if(pSrcPile->m_Cards.IsEmpty())
	{
		//TRACE("%s(%p) is empty\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile);
		//GetApplication()->LogMessage("%s(%p) is empty\n",pSrcPile->GetRuntimeClass()->m_lpszClassName,pSrcPile);
		return;
	}
	if (pCard)	
		pos = pSrcPile->m_Cards.Find(pCard);
	else
		pos = pSrcPile->m_Cards.GetHeadPosition();
	// hänge alle Karten an den Schwanz von this
	while (pos)
	{
		lastpos = pos;
		pTmpCard = pSrcPile->m_Cards.GetNext(pos);
		m_Cards.AddTail(pTmpCard);
		//TRACE("\tAdd to %s(%p):%s\n",GetRuntimeClass()->m_lpszClassName,this,CardName);
		//GetApplication()->LogMessage("\tAdd to %s(%p):%s\n",GetRuntimeClass()->m_lpszClassName,
		//	this,pTmpCard->GetCardName());
		pSrcPile->m_Cards.RemoveAt(lastpos);
	}
	// durch Neuberechnung des Overlappedfactors kann der Pile sogar größer werden
	pUpdateRectList->AddTail(pSrcPile->GetRect());
	pUpdateRectList->AddTail(GetRect());
	UpdateCardsPositions();
	pSrcPile->UpdateCardsPositions();
	pUpdateRectList->AddTail(pSrcPile->GetRect());
	pUpdateRectList->AddTail(GetRect());
}
*/

/////////////////////////////////////////////////////////////////////////////
// diese Funktion testet, ob der angegebene point innerhalb des Pile Area
// von selektierten Karten liegt (point in logischen Koordinaten)
//
BOOL CPile::HitTestSelectedCards(CPoint point)
{
	CCard* pCard = NULL;
	if (m_Cards.IsEmpty())
		return FALSE;
	POSITION pos = m_Cards.GetHeadPosition();
	while (pos)
	{
		pCard = m_Cards.GetNext(pos);
		if (pCard->m_IsSelected && pCard->HitTest(point))
			return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion testet, ob der angegebene point innerhalb des Pile Area
// liegt (point in logischen Koordinaten)
//
BOOL CPile::HitTest(CPoint point)
{
	CRect PileRect(m_Position,m_Size);
	// Rechteck etwas verkleinern
	PileRect.InflateRect(-4,-4);
	if (PileRect.PtInRect(point))
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion testet, ob die beiden Piles überlappen
// dabei wird geprüft, ob bei pPile die linke obere Ecke 
// der obersten Karte im aufrufenden Pile liegt
//
BOOL CPile::HitTest(CPile* pPile)
{
	//pPile->UpdateCardsPositions();
	if (pPile->IsEmpty()) return FALSE;
	CRect PileRect1 = pPile->m_Cards.GetHead()->GetCardRect();
	CRect PileRect2 = GetRect();
	CRect IntersectRect((CRect&)ZeroRect);
	PileRect1.NormalizeRect(); PileRect2.NormalizeRect();
	IntersectRect.IntersectRect(PileRect1,PileRect2);
	if (m_KindOfOverlapped == olmNotOverlappedHalfDown)
	{
		return (IntersectRect.Width() > (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cx)) &&
				(IntersectRect.Height()> (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cy/2));
	}
	return (IntersectRect.Width() > (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cx)) &&
			(IntersectRect.Height()> (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cy));
	//IntersectRect.NormalizeRect();
	switch (m_KindOfOverlapped)
	{
	case olmNotOverlapped:
		return (IntersectRect.Width() > (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cx)) &&
			   (IntersectRect.Height()> (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cy));
	case olmDownOverlapped:
		return (IntersectRect.Width() > (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cx));
	case olmRigthOverlapped:
		return (IntersectRect.Height()> (int)(0.6 * CCyberSolitaireApp::DefaultCardSize.cy));
	default:
		break;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert einen Zeiger auf die Karte, die zu point gehört
// (point in logischen Koordinaten)
//
CCard* CPile::GetCard(CPoint point)
{
	CCard* pCard = NULL;
	//ASSERT(!m_Cards.IsEmpty());
	POSITION pos = m_Cards.GetHeadPosition();
	while (pos)
	{
		pCard = m_Cards.GetNext(pos);
		if (pCard->HitTest(point))
			return pCard;
	}
	return NULL;
}

void CPile::Display(CDC *pDC, CDibs& Dibs, BOOL WithMaxSize /* = FALSE */ , BOOL Transparent /* = TRUE */)
{
	// für Test
	//return;
	// Achtung pDC muß schon PrepareDC durchlaufen haben
	CCard* pCard = NULL;
	UpdateCardsPositions();
	if (m_Cards.IsEmpty() && !this->IsK(CMovePile))
	{
		// Umrechnungsfaktoren zwischen logischen Koordinaten und Kartengröße (Bitmap)
		double xFac = (double)m_Size.cx / (double)m_dibEmpty.GetSize().cx;
		double yFac = (double)m_Size.cy / (double)m_dibEmpty.GetSize().cy;
		// Umrechnung log. -> Bitmapkoordinaten
		CSize devVisibleSize((int)(CCyberSolitaireApp::DefaultCardSize.cx/xFac),(int)(CCyberSolitaireApp::DefaultCardSize.cy/yFac));
		CRect	devCardRect(ZeroPoint,devVisibleSize);		// das ist der Bereich in Pixeln, der die Kartenbitmap betrifft

		// die Bitmap denkt in Pixeln, deshalb umrechnen LP->DP
		CPoint Position(m_Position);
		pDC->LPtoDP(&Position);
		CSize VisibleSize(CCyberSolitaireApp::DefaultCardSize);
		pDC->LPtoDP(&VisibleSize);
		CRect	CardRect(Position,VisibleSize);
		if (!(m_PutIfEmpty == pieNoCard))
		{
			if (Transparent)
			{
				// sollte die CDib für diesen Stapel noch nicht vorhanden sein,
				// so wird sie jetzt erzeugt
				if (!m_pdibEmpty)
				{
					// erst mal alle TransPoints löschen
					m_dibEmpty.m_TransPoints.RemoveAll();
					// Karten Dib erzeugen
					CDib* pCardDib = new CDib;
					CSize CardSize(CCyberSolitaireApp::DefaultCardSize);
					pDC->LPtoDP(&CardSize);
					// CardRect hat jetzt die Größe des Piles, in der er tatsächlich dargestellt wird
					// in dev Koordinaten (Pixel)
					CRect	CardRect(ZeroPoint,CardSize);
					pCardDib->Create(*pDC,CardSize,Dibs.pBackgroundDib); // hiermit wird eine Bitmap derselben Farbtiefe wie
																		 // m_Dibs.pBackgroundDib erzeugt
					CRect	devCardRect(ZeroPoint,m_dibEmpty.GetSize());
					// jetzt wird die Bitmap des leeren Piles auf diese Größe reduziert
					pCardDib->CopyBitmap(*pDC,&m_dibEmpty,&CardRect,&devCardRect);
					// erzeuge eine Liste aller transparenten Punkte
					CDC memDC,backDC;
					if (!memDC.CreateCompatibleDC(pDC) || !backDC.CreateCompatibleDC(pDC)) 
					{
						TRACE0("Unable to create compatible DC's\n");
						return;
					}
					CBitmap* pOldBm = memDC.SelectObject(pCardDib);
					CBitmap* pOldBackBm = backDC.SelectObject(Dibs.pBackgroundDib);
					pCardDib->m_TransPoints.RemoveAll();
					// die Bitmap denkt in Pixeln, deshalb umrechnen LP->DP
					CPoint Position(GetPosition());
					pDC->LPtoDP(&Position);
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
					pCardDib->CopyTransPoints(&m_dibEmpty);
					memDC.SelectObject(pOldBm);
					backDC.SelectObject(pOldBackBm);
					m_pdibEmpty = pCardDib;
					pCardDib->CopyTransPoints(m_pdibEmpty);
				}	
				// wieder zurück
				//Dibs.pScreenDib->CopyBitmapTransparent(*pDC,&m_dibEmpty,Dibs.pBackgroundDib,&CardRect,&devCardRect);
				//Dibs.pScreenDib->CopyBitmap(*pDC,&m_dibEmpty,&CardRect,&devCardRect);
				Dibs.pScreenDib->CopyBitmap(*pDC,m_pdibEmpty,&CardRect);
			}
			else
				Dibs.pScreenDib->CopyBitmap(*pDC,&m_dibEmpty,&CardRect,&devCardRect);
		}
		else
			Dibs.pBackgroundDib->CopyBitmap(*pDC,Dibs.pBackgroundDib,&CardRect,&devCardRect);
	}
	else
	{
		CCard::LastDrawedCardRect.SetRectEmpty();
		POSITION pos = m_Cards.GetHeadPosition();
		while (pos)
		{
			pCard = m_Cards.GetNext(pos);
			pCard->Display(pDC,Dibs,Transparent);
			if (m_KindOfOverlapped == olmNotOverlapped)
				CCard::LastDrawedCardRect.SetRectEmpty();
		}
	}
	if ((this->IsK(CTableau) || this->IsK(CWaste) || this->IsK(CStock)) && WithMaxSize)
	//if (!this->IsK(CMovePile) && WithMaxSize)
	{
		CRect MaxPileRect(GetPosition(),m_MaxSize);
		pDC->LPtoDP(&MaxPileRect);

		// Create memory DC
		CDC CopyDC;
		if (!CopyDC.CreateCompatibleDC(pDC)) 
		{
			TRACE0("Unable to create compatible DC's\n");
			return;
		}
		CBitmap* pOldCopyBm = CopyDC.SelectObject(Dibs.pScreenDib);
		CBrush* pOldBrush = (CBrush*)CopyDC.SelectStockObject(NULL_BRUSH);

		GdiFlush();
		CopyDC.Rectangle(&MaxPileRect);
		CopyDC.SelectObject(pOldCopyBm);
		CopyDC.SelectObject(pOldBrush);
	}
}


void CPile::UpdateCardsPositions()
{
	// berechnet eventuell neuen m_OverlappedFactor
	UpdateOverlappedFactor();
	// berechnet die Geometrie des Piles
	CPoint NextPosition	= m_Position;
	m_Size = CCyberSolitaireApp::DefaultCardSize;
	CSize OverlappedSize(CCyberSolitaireApp::DefaultCardSize);
	int DeltaOverlapped = 0;
	switch (m_KindOfOverlapped)
	{
	case olmNotOverlapped:
		break;
	case olmDownOverlapped:
		DeltaOverlapped = (int)(m_OverlappedFactor * CCyberSolitaireApp::DefaultCardSize.cy);
		OverlappedSize.cy = DeltaOverlapped;
		m_Size.cy = 0;
		break;
	case olmRigthOverlapped:
		DeltaOverlapped = (int)(m_OverlappedFactor * CCyberSolitaireApp::DefaultCardSize.cx);
		OverlappedSize.cx = DeltaOverlapped;
		m_Size.cx = 0;
		break;
	case olmNotOverlappedHalfDown:
		// nur die Hälfte der Karte ist zu sehen
		// Korrektur der Stapelgröße
		m_Size = CCyberSolitaireApp::DefaultCardSize;
		m_Size.cy = m_Size.cy / 2;
		SetMaxSize(m_Size);
		break;
	default:
		break;
	}
	if (m_Cards.IsEmpty())
	{
		// zu tun
		// leeren Pile behandeln
		m_Size = CCyberSolitaireApp::DefaultCardSize;
		if (m_KindOfOverlapped == olmNotOverlappedHalfDown)
		{
			m_Size.cy = m_Size.cy / 2;
			SetMaxSize(m_Size);
		}
	}
	else
	{
		CCard* pCard = NULL;
		POSITION pos = m_Cards.GetHeadPosition();
		while (pos)
		{
			pCard = m_Cards.GetNext(pos);
			switch (m_KindOfOverlapped)
			{
			case olmNotOverlapped:
				// jede Karte bekommt die Position des Piles
				pCard->m_LogicalPosition = m_Position;
				pCard->m_LogicalVisibleSize = CCyberSolitaireApp::DefaultCardSize;

				// nur die letzte Karte wird angezeigt
				if (!pos)
					pCard->m_IsVisible = TRUE;
				else
					pCard->m_IsVisible = FALSE;
				break;
			case olmDownOverlapped:
				pCard->m_LogicalPosition = NextPosition;
				NextPosition.y += DeltaOverlapped;
				// die letzte Karte sieht man ganz
				if (!pos)
				{
					m_Size.cy += CCyberSolitaireApp::DefaultCardSize.cy;
					pCard->m_LogicalVisibleSize = CCyberSolitaireApp::DefaultCardSize;
				}
				else
				{
					m_Size.cy += OverlappedSize.cy;
					pCard->m_LogicalVisibleSize = OverlappedSize;
				}
				// alle Karten müssen angezeigt werden
				pCard->m_IsVisible = TRUE;
				break;
			case olmRigthOverlapped:
				pCard->m_LogicalPosition = NextPosition;
				NextPosition.x += DeltaOverlapped;
				if (!pos)
				{
					m_Size.cx += CCyberSolitaireApp::DefaultCardSize.cx;
					pCard->m_LogicalVisibleSize = CCyberSolitaireApp::DefaultCardSize;
				}
				else
				{
					m_Size.cx += OverlappedSize.cx;
					pCard->m_LogicalVisibleSize = OverlappedSize;
				}
				// alle Karten müssen angezeigt werden
				pCard->m_IsVisible = TRUE;
				break;
			case olmNotOverlappedHalfDown:
				// jede Karte bekommt die Position des Piles
				// nur die Hälfte der Karte ist zu sehen
				pCard->m_LogicalPosition = m_Position;
				pCard->m_LogicalVisibleSize = CCyberSolitaireApp::DefaultCardSize;
				pCard->m_LogicalVisibleSize.cy = pCard->m_LogicalVisibleSize.cy / 2;
				// nur die letzte Karte wird angezeigt
				if (!pos)
					pCard->m_IsVisible = TRUE;
				else
					pCard->m_IsVisible = FALSE;
				break;
			default:
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion mischt den Kartenstapel
//
void CPile::MixCards()
{

   srand((unsigned)time(NULL));
   // Kartenstapel mischen
	for (int i=0; i<GetApplication()->m_MaxMix; i++)
	{
		int Src = rand() % m_Cards.GetCount();	//[0..UpperBound] ???
		int Dst = rand() % m_Cards.GetCount();
		POSITION posSrc = m_Cards.FindIndex(Src);
		POSITION posDst = m_Cards.FindIndex(Dst);
		CCard* pCard1 = m_Cards.GetAt(posSrc);
		CCard* pCard2 = m_Cards.GetAt(posDst);
		m_Cards.SetAt(posDst,pCard1);
		m_Cards.SetAt(posSrc,pCard2);
	}
	//PlaySound("Shuffle.wav",NULL,SND_ASYNC | SND_FILENAME);
	PlaySound("Shuffle.wav",NULL,SND_SYNC | SND_FILENAME);
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion bearbeitet das Aussehen der Karten beim Start
//
void CPile::UpdateFaceAtStart()
{
	CCard* pCard = NULL;
	POSITION pos = m_Cards.GetHeadPosition();
	int cnt=1;
	while (pos)
	{
		pCard = m_Cards.GetNext(pos);
		switch (m_FaceAtStart) {
			case fasAllFaceDown:
				pCard->SetFaceUp(FALSE);
				break;
			case fasAllFaceUp:
				pCard->SetFaceUp(TRUE);
				break;
			case fasLastFaceUp:
				if (!pos)
					pCard->SetFaceUp(TRUE);
				else
					pCard->SetFaceUp(FALSE);
				break;
			case fasOddFaceUp:
				if ((cnt % 2) == 1)
					pCard->SetFaceUp(TRUE);
				else
					pCard->SetFaceUp(FALSE);
				break;
			case fasEvenFaceUp:
				if ((cnt % 2) == 0)
					pCard->SetFaceUp(TRUE);
				else
					pCard->SetFaceUp(FALSE);
				break;
			default:
				break;
		}
		if ((m_NumberHiddenOnStart != -1) && (cnt <= m_NumberHiddenOnStart))
				pCard->SetFaceUp(FALSE);
		cnt++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// selektiert oder deselektiert alle Karten ab pCard oder alle (pCard==NULL)
//
BOOL CPile::SelectCards(CCard *pCard, BOOL UnSelect /* = FALSE */)
{
	if (IsEmpty()) return TRUE;
	POSITION pos = NULL;
	// Deselektieren können wir jeden Pile
	if (UnSelect)
	{
		for (pos = m_Cards.GetHeadPosition();pos != 0;)
			m_Cards.GetNext(pos)->m_IsSelected = FALSE;
		return TRUE;
	}

	switch (m_KindOfSelecting)
	{
	case selAll:
		//ASSERT(pCard == m_Cards.GetHead());
		for (pos = m_Cards.GetHeadPosition();pos != 0;)
			m_Cards.GetNext(pos)->m_IsSelected = TRUE;
		return TRUE;
	// zu_tun
	case selAnySeqInSuit:
	case selAnySeqAlternate:
	case selAnySeqNoColor:
	case selAllSeqNoColorOrLast:
	case selAllSeqInSuitOrLast:
	case selAllSeqAlternateOrLast:
	case selOnlyLast:
	case selAllFaceUp:
	case selPairsEqualValue:
	case selPairsValue13:	


		if (pCard)
			pos = m_Cards.Find(pCard);
		else
			pos = m_Cards.GetHeadPosition();
		ASSERT(pos);
		while (pos)
		{
			pCard = m_Cards.GetNext(pos);
			if (UnSelect)
				pCard->m_IsSelected = FALSE;
			else
				pCard->m_IsSelected = TRUE;
		}
		break;
	case selExactOne:
		if (UnSelect)
			pCard->m_IsSelected = FALSE;
		else
			pCard->m_IsSelected = TRUE;
		break;
	default:
		break;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Sequenz
// in Farbfolge ist
//
BOOL CPile::AreCardsInSuit(CCard *pCard)
{
	CCard* pTmpCard = NULL;
	if (!pCard) return FALSE;
	
	CARD_SUIT Suit;
	POSITION pos = m_Cards.Find(pCard);
	// Farbe des ersten Karte feststellen 
	if (pos)
	{
		pTmpCard = m_Cards.GetNext(pos);
		Suit = pTmpCard->GetSuit();
	}
	// wenn nur eine Karte da, dann raus mit TRUE
	if (!pos) return TRUE;
	while (pos)
	{
		pTmpCard = m_Cards.GetNext(pos);
		if (pTmpCard->GetSuit() != Suit)
			return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Sequenz
// alternierend ist
//
BOOL CPile::AreCardsAlternate(CCard *pCard)
{
	CCard* pTmpCard = NULL;
	if (!pCard) return FALSE;
	
	CARD_COLOR Color;
	POSITION pos = m_Cards.Find(pCard);
	// Farbe des ersten Karte feststellen 
	if (pos)
	{
		pTmpCard = m_Cards.GetNext(pos);
		Color = pTmpCard->GetColor();
	}
	// wenn nur eine Karte da, dann raus mit TRUE
	if (!pos) return TRUE;
	while (pos)
	{
		pTmpCard = m_Cards.GetNext(pos);
		if (pTmpCard->GetColor() == Color)
			return FALSE;
		Color = pTmpCard->GetColor();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob pCard die erste sichtbare Karte hinter nicht
// sichtbaren Karten ist
//
BOOL CPile::IsFirstVisibleCard(CCard *pCard)
{
	CCard* pTmpCard = NULL;
	if (IsEmpty()) return FALSE;
	POSITION pos = m_Cards.Find(pCard);
	if (pos) VERIFY(pCard == m_Cards.GetPrev(pos));
	if (pos)
	{
		pTmpCard = m_Cards.GetPrev(pos);
		if (!pTmpCard->GetFaceUp())
			return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob pCard die letzte unsichtbare Karte vor
// den sichtbaren Karten ist
//
BOOL CPile::IsLastInvisibleCard(CCard *pCard)
{
	CCard* pTmpCard = NULL;
	if (IsEmpty()) return FALSE;
	if (pCard->GetFaceUp()) return FALSE;
	POSITION pos = m_Cards.Find(pCard);
	if (pos) VERIFY(pCard == m_Cards.GetNext(pos));
	if (pos)
	{
		pTmpCard = m_Cards.GetNext(pos);
		if (pTmpCard->GetFaceUp())
			return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Kartenstapel leer ist
//
BOOL CPile::IsEmpty()
{
	return m_Cards.IsEmpty();
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die Position des Kartenstapels
//
CPoint CPile::GetPosition()
{
	return m_Position;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion setzt die Position des Kartenstapels
//
CPoint CPile::SetPosition(CPoint Position)
{
	CPoint OldPosition(m_Position);
	m_Position = Position;
	return OldPosition;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die Größe des Kartenstapels
//
CSize CPile::GetSize()
{
	return m_Size;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert das Rechteck des Kartenstapels
//
CRect CPile::GetRect()
{
	return(CRect(m_Position,m_Size));
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion serialisiert den Kartenstapel
//
void CPile::Serialize(CArchive &ar)
{
	m_Cards.Serialize(ar);
	if (ar.IsStoring()) 
	{
		m_PileVersion = PILE_VERSION;
		ar << m_PileVersion;
		ar << m_IDDibEmpty << m_NumberOnStart << m_NumberPerMove << m_MaxCards 
		   << m_Position << m_Size << m_OverlappedFactor;
		ar.Write(&m_KindOfOverlapped,sizeof(m_KindOfOverlapped));
		ar.Write(&m_KindOfSelecting,sizeof(m_KindOfSelecting));
		ar.Write(&m_KindOfPiling,sizeof(m_KindOfPiling));
		ar.Write(&m_FaceAtStart,sizeof(m_FaceAtStart));
		ar.Write(&m_PutIfEmpty,sizeof(m_PutIfEmpty));
		ar << m_MaxSize;
		ar << m_NumberHiddenOnStart;
		ar << m_DealOrder;
		ar << m_NumberPerDeal;
		ar.Write(&m_DealTo,sizeof(m_DealTo));
		ar << m_RedealOrder;
		ar.Write(&m_PermittedPlay,sizeof(m_PermittedPlay));
		ar << m_SpecialHandling;
		ar << m_SpecialHandlingText;
		ar << m_UpdateOverlap;
	}
	else 
	{
		ar >> m_PileVersion;
		ar >> m_IDDibEmpty >> m_NumberOnStart >> m_NumberPerMove >> m_MaxCards
		   >> m_Position >> m_Size >> m_OverlappedFactor;
		ar.Read(&m_KindOfOverlapped,sizeof(m_KindOfOverlapped));
		ar.Read(&m_KindOfSelecting,sizeof(m_KindOfSelecting));
		ar.Read(&m_KindOfPiling,sizeof(m_KindOfPiling));
		ar.Read(&m_FaceAtStart,sizeof(m_FaceAtStart));
		ar.Read(&m_PutIfEmpty,sizeof(m_PutIfEmpty));
		m_dibEmpty.Load(AfxGetApp()->m_hInstance,m_IDDibEmpty);
		m_MaxSize = CCyberSolitaireApp::DefaultCardSize;
		if (m_PileVersion > 1)
			ar >> m_MaxSize;
		if (m_PileVersion > 2)
			ar >> m_NumberHiddenOnStart;
		else
			m_NumberHiddenOnStart = -1;
		if (m_PileVersion > 3)
		{
			ar >> m_DealOrder;
			ar >> m_NumberPerDeal;
			ar.Read(&m_DealTo,sizeof(m_DealTo));
		}
		else
			m_DealOrder = 0;
		if (m_PileVersion > 4)
		{
			ar >> m_RedealOrder;
		}
		else
			m_RedealOrder = 0;
		if (m_PileVersion > 5)
		{
			ar.Read(&m_PermittedPlay,sizeof(m_PermittedPlay));
		}
		else
			m_PermittedPlay = pppTableauOrFoundation;
		if (m_PileVersion > 6)
		{
			ar >> m_SpecialHandling;
			ar >> m_SpecialHandlingText;
		}
		else
		{
			m_SpecialHandling = FALSE;
			m_SpecialHandlingText = "";
		}
		if (m_PileVersion > 7)
		{
			ar >> m_UpdateOverlap;
		}
		else
		{
			m_UpdateOverlap = TRUE;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion verschiebt den Kartenstapel
//
void CPile::Move(CPoint FromPoint, CPoint ToPoint, CRect& UpdateRect)
{
	CRect PileRect(m_Position,m_Size);
	UpdateRect.UnionRect(UpdateRect,PileRect);
	SetPosition(GetPosition() + (ToPoint - FromPoint));
	UpdateCardsPositions();
	UpdateRect.UnionRect(UpdateRect,GetRect());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion verschiebt den Kartenstapel
//
/*
void CPile::Move(CPoint FromPoint, CPoint ToPoint, CListRect* pUpdateRectList)
{
	pUpdateRectList->AddTail(GetRect());
	SetPosition(GetPosition() + (ToPoint - FromPoint));
	UpdateCardsPositions();
	pUpdateRectList->AddTail(GetRect());
}
*/

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel absteigend und in Farbfolge ist
//
BOOL CPile::IsDownInSequenceAndSuit()
{
	return AreCardsInSuit(m_Cards.GetHead()) &&	AreCardsDownInSequence(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel absteigend und in Farbfolge ist
// mit Überlauf (As->König)
//
BOOL CPile::IsDownInSequenceAndSuitRolling()
{
	return AreCardsInSuit(m_Cards.GetHead()) &&	AreCardsDownInSequenceRolling(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel absteigend und alternierend ist
//
BOOL CPile::IsDownInSequenceAndAlternate()
{
	return AreCardsAlternate(m_Cards.GetHead()) &&	AreCardsDownInSequence(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel absteigend ist
// mit Überlauf (As->König)
//
BOOL CPile::IsDownInSequenceRolling()
{
	return AreCardsDownInSequenceRolling(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel aufsteigend und in Farbfolge ist
//
BOOL CPile::IsUpInSequenceAndSuit()
{
	return AreCardsInSuit(m_Cards.GetHead()) &&	AreCardsUpInSequence(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel aufsteigend und in Farbfolge ist
// mit Überlauf (König->As))
//
BOOL CPile::IsUpInSequenceAndSuitRolling()
{
	return AreCardsInSuit(m_Cards.GetHead()) &&	AreCardsUpInSequenceRolling(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel aufsteigend ist
// mit Überlauf (König->As))
//
BOOL CPile::IsUpInSequenceRolling()
{
	return AreCardsUpInSequenceRolling(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel aufsteigend und alternierend ist
//
BOOL CPile::IsUpInSequenceAndAlternate()
{
	return AreCardsAlternate(m_Cards.GetHead()) &&	AreCardsUpInSequence(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel absteigend ist
//
BOOL CPile::IsDownInSequence()
{
	return AreCardsDownInSequence(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel aufsteigend ist
//
BOOL CPile::IsUpInSequence()
{
	return AreCardsUpInSequence(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel aufsteigend und 
// gleiche Farbe wie BasicCardValue hat
//
BOOL CPile::IsUpInSequenceSameColor(int BasicCardValue)
{
	return HaveCardsSameColor(m_Cards.GetHead(),BasicCardValue) && AreCardsUpInSequence(m_Cards.GetHead());
}


/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Folge alle die gleiche 
// Farbe wie BasicCardValue haben
//
BOOL CPile::HaveCardsSameColor(CCard *pCard,int BasicCardValue)
{
	CARD_COLOR BasicCardColor;
	if ((BasicCardValue >= 1) &&	(BasicCardValue <= 13) || (BasicCardValue >= 40) &&	(BasicCardValue <= 52))
		BasicCardColor = BLACK;
	if ((BasicCardValue >= 14) &&	(BasicCardValue <= 26) || (BasicCardValue >= 27) &&	(BasicCardValue <= 39))
		BasicCardColor = RED;
	CCard* pPrevCard = NULL;
	CCard* pNextCard = NULL;
	if (!pCard) return FALSE;
	
	POSITION pos = m_Cards.Find(pCard);
	while (pos)
	{
		pNextCard = m_Cards.GetNext(pos);
		if (pNextCard->GetColor() != BasicCardColor)
			return FALSE;
	}
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob der Stapel aufsteigend 
// mit einer Differenz von 2 ist
//
BOOL CPile::IsUpInSequenceValueDiff2InSuit(int BasicCardValue)
{
	return HaveCardsSameSuit(m_Cards.GetHead(),BasicCardValue) && HaveCardsValueDiff2(m_Cards.GetHead());
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Folge alle die gleiche 
// Kartenfarbe (Pik,Kreuz,Karo,Herz) wie BasicCardValue haben
//
BOOL CPile::HaveCardsSameSuit(CCard *pCard,int BasicCardValue)
{
	CARD_SUIT	BasicCardSuit;
	if ((BasicCardValue >= 1) &&	(BasicCardValue <= 13))
		BasicCardSuit =  CLUB;
	if ((BasicCardValue >= 14) &&	(BasicCardValue <= 26))
		BasicCardSuit =  DIAMOND;
	if ((BasicCardValue >= 27) &&	(BasicCardValue <= 39))
		BasicCardSuit =  HEART;
	if ((BasicCardValue >= 40) &&	(BasicCardValue <= 52))
		BasicCardSuit =  SPADE;

	CCard* pPrevCard = NULL;
	CCard* pNextCard = NULL;
	if (!pCard) return FALSE;
	
	POSITION pos = m_Cards.Find(pCard);
	while (pos)
	{
		pNextCard = m_Cards.GetNext(pos);
		if (pNextCard->GetSuit() != BasicCardSuit)
			return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Folge alle den 
// Abstand 2 besitzen
//
BOOL CPile::HaveCardsValueDiff2(CCard *pCard)
{
	CCard* pPrevCard = NULL;
	CCard* pNextCard = NULL;
	int PrevValue = 0;
	int NextValue = 0;

	if (!pCard) return FALSE;
	
	POSITION pos = m_Cards.Find(pCard);
	pPrevCard = m_Cards.GetNext(pos);
	PrevValue = (pPrevCard->m_nCardValue-1) % 13;
	while (pos)
	{
		pNextCard = m_Cards.GetNext(pos);
		NextValue = (pNextCard->m_nCardValue-1) % 13;
		if (((PrevValue == 11) && (NextValue == 0)) || ((PrevValue == 12) && (NextValue == 1)))
			NextValue += 13;
		if ((PrevValue + 2) != NextValue)
			return FALSE;
		pPrevCard = pNextCard;
		PrevValue = (pNextCard->m_nCardValue-1) % 13;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Sequenz absteigend ist
//
BOOL CPile::AreCardsDownInSequence(CCard *pCard)
{
	CCard* pPrevCard = NULL;
	CCard* pNextCard = NULL;
	if (!pCard) return FALSE;
	
	POSITION pos = m_Cards.Find(pCard);
	pPrevCard = m_Cards.GetNext(pos);
	while (pos)
	{
		pNextCard = m_Cards.GetNext(pos);
		if (!pPrevCard->IsDownInSequence(pNextCard))
			return FALSE;
		pPrevCard = pNextCard;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Sequenz absteigend ist
// mit Überlauf (As->König)
//
BOOL CPile::AreCardsDownInSequenceRolling(CCard *pCard)
{
	CCard* pPrevCard = NULL;
	CCard* pNextCard = NULL;
	if (!pCard) return FALSE;
	
	POSITION pos = m_Cards.Find(pCard);
	pPrevCard = m_Cards.GetNext(pos);
	while (pos)
	{
		pNextCard = m_Cards.GetNext(pos);
		if (!pPrevCard->IsDownInSequenceRolling(pNextCard))
			return FALSE;
		pPrevCard = pNextCard;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Sequenz aufsteigend ist
//
BOOL CPile::AreCardsUpInSequence(CCard *pCard)
{
	CCard* pPrevCard = NULL;
	CCard* pNextCard = NULL;
	if (!pCard) return FALSE;
	
	POSITION pos = m_Cards.Find(pCard);
	pPrevCard = m_Cards.GetNext(pos);
	while (pos)
	{
		pNextCard = m_Cards.GetNext(pos);
		if (!pPrevCard->IsUpInSequence(pNextCard))
			return FALSE;
		pPrevCard = pNextCard;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob die mit pCard beginnende Sequenz aufsteigend ist
// mit Überlauf (König->As)
//
BOOL CPile::AreCardsUpInSequenceRolling(CCard *pCard)
{
	CCard* pPrevCard = NULL;
	CCard* pNextCard = NULL;
	if (!pCard) return FALSE;
	
	POSITION pos = m_Cards.Find(pCard);
	pPrevCard = m_Cards.GetNext(pos);
	while (pos)
	{
		pNextCard = m_Cards.GetNext(pos);
		if (!pPrevCard->IsUpInSequenceRolling(pNextCard))
			return FALSE;
		pPrevCard = pNextCard;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion berechnet den OverlappedFactor des Pile so, dass alle Karten 
// eines Piles in m_MaxSize liegen
//
void CPile::UpdateOverlappedFactor()
{
	int NumberOfCards = m_Cards.GetCount();
	CSize ComputedPileSize(0,0);
	BOOL UseDefaults = FALSE;

	// der MovePile braucht nicht untersucht zu werden
	if (this->IsK(CMovePile) || !m_UpdateOverlap)
	{
		m_OverlappedFactor	= CCyberSolitaireApp::DefaultOverlappedFactor;
		return;
	}

	// Überprüfung, ob der Pile mit den default-Werten in m_MaxSize reinpaßt
	if (m_Cards.IsEmpty() || (NumberOfCards == 1))
	{
		// die Größe ist genau eine Karte groß
		ComputedPileSize = CCyberSolitaireApp::DefaultCardSize;
		switch (m_KindOfOverlapped) {
		case olmNotOverlappedHalfDown:
			ComputedPileSize.cy = ComputedPileSize.cy / 2;
			break;
		default:
			break;
		}
		ASSERT((ComputedPileSize.cx <= m_MaxSize.cx) && (ComputedPileSize.cy <= m_MaxSize.cy));
		UseDefaults = TRUE;
	}
	else
	{
		switch (m_KindOfOverlapped) {
		case olmNotOverlapped:
			// die Größe ist genau eine Karte groß
			ComputedPileSize = CCyberSolitaireApp::DefaultCardSize;
			ASSERT((ComputedPileSize.cx <= m_MaxSize.cx) && (ComputedPileSize.cy <= m_MaxSize.cy));
			UseDefaults = TRUE;
			break;
		case olmDownOverlapped:
			ComputedPileSize.cy = (NumberOfCards - 1) *  CCyberSolitaireApp::DefaultDownOverlappedSize.cy +
								  CCyberSolitaireApp::DefaultCardSize.cy;
			ComputedPileSize.cx = CCyberSolitaireApp::DefaultCardSize.cx;
			UseDefaults = (ComputedPileSize.cy < m_MaxSize.cy);
			break;
		case olmRigthOverlapped:
			ComputedPileSize.cx = (NumberOfCards - 1) *  CCyberSolitaireApp::DefaultRightOverlappedSize.cx +
								  CCyberSolitaireApp::DefaultCardSize.cx;
			ComputedPileSize.cy = CCyberSolitaireApp::DefaultCardSize.cy;
			UseDefaults = (ComputedPileSize.cx < m_MaxSize.cx);
			break;
		case olmNotOverlappedHalfDown:
			ComputedPileSize = CCyberSolitaireApp::DefaultCardSize;
			ComputedPileSize.cy = ComputedPileSize.cy / 2;
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	// wir können die default Werte benutzen
	if (UseDefaults)
	{
		m_OverlappedFactor	= CCyberSolitaireApp::DefaultOverlappedFactor;
		return;
	}

	// es muß ein neuer Überlappungsfaktor berechnet werden
	ASSERT(NumberOfCards > 1);
	// um wieviel muß verkleinert werden
	int    TmpValue(0);
	switch (m_KindOfOverlapped) {
	case olmDownOverlapped:
		// Länge für die Überlappungen ( Gesamtlänge - 1 Karte)
		TmpValue = m_MaxSize.cy - CCyberSolitaireApp::DefaultCardSize.cy;
		m_OverlappedFactor =  (double)TmpValue / ((double)CCyberSolitaireApp::DefaultCardSize.cy * (NumberOfCards-1));
		break;
	case olmRigthOverlapped:
		// Länge für die Überlappungen ( Gesamtlänge - 1 Karte)
		TmpValue = m_MaxSize.cx - CCyberSolitaireApp::DefaultCardSize.cx;
		m_OverlappedFactor =  (double)TmpValue / ((double)CCyberSolitaireApp::DefaultCardSize.cx * (NumberOfCards-1));
		break;
	default:
		ASSERT(0);
		break;
	}
	//TRACE("OverlappedFactor: %4.4f\n",m_OverlappedFactor);
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion überprüft, ob pCard die letzte Karte des Pile ist
//
BOOL CPile::IsLastCard(CCard *pCard)
{
	if (IsEmpty())
		return FALSE;
	return m_Cards.GetTail() == pCard;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die letzte Karte des Pile
//
CCard* CPile::GetLastCard(CCard *pCard)
{
	if (IsEmpty())
		return NULL;
	return m_Cards.GetTail();
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die erste Karte des Pile oder NULL wenn leer
//
CCard* CPile::GetFirstCard(CCard *pCard)
{
	if (IsEmpty())
		return NULL;
	return m_Cards.GetHead();
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion selektiert die letzte sichtbare Sequenz des Pile
//
CCard* CPile::GetLastSequence()
{
	// bei leerem Pile ist nichts auszuwählen
	if (IsEmpty())
		return NULL;
	CCard* pPrevCard = NULL;
	POSITION pos = m_Cards.GetTailPosition();
	// wir beginnen mit der letzen Karte
	CCard* pNextCard = m_Cards.GetPrev(pos);
	while (pos)
	{
		pPrevCard = m_Cards.GetPrev(pos);
		// war es die letzte sichtbare Karte
		if (!pPrevCard->GetFaceUp())
			return pNextCard;
		// ist es noch eine Sequenz
		if (!pPrevCard->IsDownInSequence(pNextCard))
			return pNextCard;
		pNextCard = pPrevCard;
	}
	return pNextCard;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion selektiert die letzte sichtbare Sequenz in Farbfolge 
// endend bei pCard oder alle (bei pCard == NULL) des Pile
//
CCard* CPile::GetLastSequenceInSuit(CCard* pCard /* = NULL */)
{
	// bei leerem Pile ist nichts auszuwählen
	if (IsEmpty())
		return NULL;
	CCard* pPrevCard = NULL;
	POSITION pos = m_Cards.GetTailPosition();
	// wir beginnen mit der letzen Karte
	CCard* pNextCard = m_Cards.GetPrev(pos);
	while (pos)
	{
		pPrevCard = m_Cards.GetPrev(pos);
		// war es die letzte sichtbare Karte
		if (!pPrevCard->GetFaceUp())
			return pNextCard;
		// ist es noch eine Sequenz
		if ( m_KindOfPiling == pilDownInSuitRolling)
		{
			if (!pPrevCard->IsDownInSequenceRolling(pNextCard) || !pPrevCard->IsDownInSuitRolling(pNextCard))
				return pNextCard;
		}
		else
		{
			if (!pPrevCard->IsDownInSequence(pNextCard) || !pPrevCard->IsDownInSuit(pNextCard))
				return pNextCard;
		}
		if (pCard && (pNextCard == pCard))
			return pNextCard;
		pNextCard = pPrevCard;
	}
	return pNextCard;
}


/////////////////////////////////////////////////////////////////////////////
// diese Funktion selektiert die letzte sichtbare Sequenz im Farbwechsel 
// endend bei pCard oder alle (bei pCard == NULL) des Pile
//
CCard* CPile::GetLastSequenceAlternate(CCard* pCard /* = NULL */)
{
	// bei leerem Pile ist nichts auszuwählen
	if (IsEmpty())
		return NULL;
	CCard* pPrevCard = NULL;
	POSITION pos = m_Cards.GetTailPosition();
	// wir beginnen mit der letzen Karte
	CCard* pNextCard = m_Cards.GetPrev(pos);
	while (pos)
	{
		pPrevCard = m_Cards.GetPrev(pos);
		// war es die letzte sichtbare Karte
		if (!pPrevCard->GetFaceUp())
			return pNextCard;
		// ist es noch eine Sequenz
		if ( m_KindOfPiling == pilDownAlternateRolling)
		{
			if (!pPrevCard->IsDownInSequenceRolling(pNextCard) || !pPrevCard->IsDownAlternateRolling(pNextCard))
				return pNextCard;
		}
		else
		{
			if (!pPrevCard->IsDownInSequence(pNextCard) || !pPrevCard->IsDownAlternate(pNextCard))
				return pNextCard;
		}
		if (pCard && (pNextCard == pCard))
			return pNextCard;
		pNextCard = pPrevCard;
	}
	return pNextCard;
}


/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die angeklickte Karte, falls vorhanden
//
CCard* CPile::GetClickedCard(CPoint point, BOOL& RetWithErr)
{
	CCard* pCard = NULL;
	if (IsEmpty())
	{
		// gar keine Karte gefunden
		RetWithErr |= !pCard;
		if (!pCard) return pCard;
	}
	switch (m_KindOfOverlapped)
	{
	case olmNotOverlapped:
	case olmNotOverlappedHalfDown:
		// es muß die letzte(oberste) Karte sein
		pCard = m_Cards.GetTail();
		break;
	case olmDownOverlapped:
	case olmRigthOverlapped:
		pCard = GetCard(point);
		break;
	default:
		break;
	}
	// gar keine Karte gefunden
	RetWithErr |= !pCard;
	if (!pCard) return pCard;
	// unsichtbare Karten scheiden aus
	RetWithErr |= !pCard->m_IsVisible;
	// nicht aufgedeckte Karten scheiden aus
	RetWithErr |= !pCard->GetFaceUp();
	return pCard;
}

/////////////////////////////////////////////////////////////////////////////
// liefert die Startkarte der Sequenz
//
CCard* CPile::SelectSequence(CCard *pCard, BOOL &RetWithErr, 
							 CPile* pFirstSelectedPile /* = NULL */, CCard *pFirstSelectedCard /* = NULL */)
{
	CCard* pRetCard = NULL;
	switch (m_KindOfSelecting)
	{
	case selAnySeqInSuit:
		// es dürfen nur Sequenzen in Farbfolge ausgegewählt werden
		RetWithErr |= !AreCardsInSuit(pCard);
		RetWithErr |= !AreCardsDownInSequence(pCard);
		pRetCard = pCard;
		return pRetCard;
	case selAnySeqAlternate:
		// es dürfen nur Sequenzen im Farbwechsel ausgegewählt werden
		// 14.04.2004 Julchen meint, daß auch immer die letzte selektiert werden kann
		// ist es die letzte Karte ?
		//if (IsLastCard(pCard))
		//	return pCard;
		RetWithErr |= !AreCardsAlternate(pCard);
		pRetCard = pCard;
		return pRetCard;
	case selAnySeqNoColor:
		// es dürfen beliebige Sequenzen ohne Rücksicht auf Farbe ausgewählt werden
		// also auch Teile einer Sequenz
		RetWithErr |= !AreCardsDownInSequence(pCard);
		pRetCard = pCard;
		return pRetCard;
	case selAllSeqNoColorOrLast:
		// nur die letzte oder alle Karten in richtiger Reihenfolge dürfen bewegt werden
		// ist es die letzte Karte ?
		if (IsLastCard(pCard))
			return pCard;
		RetWithErr |= !(pRetCard = GetLastSequence());
		return pRetCard;
	case selAllSeqInSuitOrLast:
		// nur die letzte oder eine Sequenz in Farbfolge dürfen bewegt werden
		// ist es die letzte Karte ?
		if (IsLastCard(pCard))
		{
			pRetCard = pCard;
			return pRetCard;
		}
		RetWithErr |= !(pRetCard = GetLastSequenceInSuit(NULL));
		return pRetCard;
	case selAllSeqAlternateOrLast:
		// nur die letzte oder die geamte Sequenz im Farbwechsel dürfen bewegt werden
		// ist es die letzte Karte ?
		if (IsLastCard(pCard))
		{
			pRetCard = pCard;
			return pRetCard;
		}
		RetWithErr |= !(pRetCard = GetLastSequenceAlternate(NULL));
		return pRetCard;
	case selOnlyLast:
		// ist es die letzte Karte ?
		RetWithErr |= !(pRetCard = GetLastCard(pCard));
		break;
	case selAll:
		// alle ab der ersten Karte ?
		RetWithErr |= !(pRetCard = GetFirstCard(pCard));
		break;
	case selExactOne:
		pRetCard = pCard;
		return pRetCard;
	case selAllFaceUp:
		if (pCard->GetFaceUp())
			pRetCard = pCard;
		break;
	case selPairsEqualValue:
		if (!pFirstSelectedCard)
			RetWithErr |= !(pRetCard = GetLastCard(pCard));
		else
		{
			// es muß ein anderer Pile sein
			RetWithErr |= (pFirstSelectedPile == this);
			RetWithErr |= !(pRetCard = GetLastCard(pCard));
			RetWithErr |= pRetCard->GetValue() != pFirstSelectedCard->GetValue();
		}
		break;
	case selPairsValue13:
		if (!pFirstSelectedCard)
			RetWithErr |= !(pRetCard = GetLastCard(pCard));
		else
		{
			// es muß ein anderer Pile sein
			RetWithErr |= (pFirstSelectedPile == this);
			RetWithErr |= !(pRetCard = GetLastCard(pCard));
			RetWithErr |= (pRetCard->GetValue() + pFirstSelectedCard->GetValue()) != 13;
		}
		break;
	default:
		break;
	}
	return pRetCard;
}

void CPile::Revert()
{
	CCard* pCard = NULL;
	CCardList	TmpCards; // Hilfsfeld

	POSITION pos = m_Cards.GetTailPosition();
	while (pos)
	{
		pCard = m_Cards.GetPrev(pos);
		TmpCards.AddTail(pCard);
	}
	m_Cards.RemoveAll();
	m_Cards = TmpCards;
}


BOOL CPile::HasChanged(CPile* pCompPile)
{
	CCard* pCard;
	CCard* pCompCard;
	if (m_Cards.GetCount() != pCompPile->m_Cards.GetCount())
		return TRUE;
	POSITION cardpos = m_Cards.GetHeadPosition();
	POSITION cardposl = pCompPile->m_Cards.GetHeadPosition();
	while (cardpos)
	{
		pCard = m_Cards.GetNext(cardpos);
		pCompCard = m_Cards.GetNext(cardposl);
		if (*pCard != *pCompCard)
			return TRUE;
	}
	return FALSE;
}


// schreibt die Serialize Version ins LogFile
void CPile::ReportSerializeVersion(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("CPile Version: %d",m_PileVersion);
	GetApplication()->LogMessage(Msg);
}

// schreibt die EmptyBitmapID ins LogFile
void CPile::ReportEmptyBitmapID(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("EmptyBitmapID: %d",m_IDDibEmpty);
	GetApplication()->LogMessage(Msg);
}

// schreibt die Position ins LogFile
void CPile::ReportPilePosition(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Position: x=%d  y=%d",m_Position.x,m_Position.y);
	GetApplication()->LogMessage(Msg);
}

// schreibt die Anzahl der Karten bei Start ins LogFile
void CPile::ReportNumberOnStart(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Anzahl Karten bei Start: %d",m_NumberOnStart);
	GetApplication()->LogMessage(Msg);
}

// schreibt die Anzahl der verdeckten Karten bei Start ins LogFile
void CPile::ReportNumberHiddenOnStart(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Anzahl verdeckten Karten bei Start: %d",m_NumberHiddenOnStart);
	GetApplication()->LogMessage(Msg);
}

// schreibt die Anzahl der Karten, die pro Zug abgelegt werden dürfen, ins LogFile
void CPile::ReportNumberPerMove(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Anzahl wieviele Karten pro Zug abgelegt werden dürfen: %d",m_NumberPerMove);
	GetApplication()->LogMessage(Msg);
}

// schreibt die maximale Anzahl der Karten, die der Pile haben darf, ins LogFile
void CPile::ReportNumberOfMaxCards(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("maximale Anzahl Karten des Stapels: %d",m_MaxCards);
	GetApplication()->LogMessage(Msg);
}

// schreibt die maximale Größe des Piles ins LogFile
void CPile::ReportMaxSize(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("maximale Größe des Stapels: cx=%d cy=%d",m_MaxSize.cx,m_MaxSize.cy);
	GetApplication()->LogMessage(Msg);
}

// schreibt die OverlappedMethod ins LogFile
void CPile::ReportKindOfOverlapped(void)
{
	// zu_tun
	// als String Resource
	CString Msg("Art der Überlappung: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_OLM_NOTAVAILABLE + (int)m_KindOfOverlapped);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt die SelectingMethod ins LogFile
void CPile::ReportKindOfSelecting(void)
{
	// zu_tun
	// als String Resource
	CString Msg("Art der Selektion: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_SEL_NOTAVAILABLE + (int)m_KindOfSelecting);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt die PilingMethod ins LogFile
void CPile::ReportKindOfPiling(void)
{
	// zu_tun
	// als String Resource
	CString Msg("Art der Ablage: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_PIL_NOTAVAILABLE + (int)m_KindOfPiling);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt FaceAtStart ins LogFile
void CPile::ReportFaceAtStart(void)
{
	// zu_tun
	// als String Resource
	CString Msg("Aussehen bei Start: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_FAS_NOTAVAILABLE + (int)m_FaceAtStart);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt PutIfEmpty ins LogFile
void CPile::ReportPutIfEmpty(void)
{
	// zu_tun
	// als String Resource
	CString Msg("womit kann der leere Stapel belegt werden: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_PIE_NOTAVAILABLE + (int)m_PutIfEmpty);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt in welcher Reihenfolge dieser Stapel bei Geben (Deal) steht ins LogFile
void CPile::ReportDealOrder(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Reihenfolge beim Geben: %d",m_DealOrder);
	GetApplication()->LogMessage(Msg);
}

// schreibt in welcher Reihenfolge dieser Stapel bei Neu Geben (Redeal) steht ins LogFile
void CPile::ReportRedealOrder(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Reihenfolge beim Neu Geben: %d",m_RedealOrder);
	GetApplication()->LogMessage(Msg);
}

// schreibt wohin wird gegeben ins LogFile
void CPile::ReportDealing(void)
{
	// zu_tun
	// als String Resource
	CString Msg("wohin wird gegeben: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_DEAL_NOTAVAILABLE + (int)m_DealTo);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt wieviele Karten auf jeden Zielstapel gegeben werden ins LogFile
void CPile::ReportNumberPerDeal(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Anzahl Karten pro Geben: %d",m_NumberPerDeal);
	GetApplication()->LogMessage(Msg);
}

// schreibt wohin darf von diesem Stapel gespielt werden ins LogFile
void CPile::ReportPermittedPlay(void)
{
	// zu_tun
	// als String Resource
	CString Msg("wohin darf gespielt werden: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_PPP_NOTAVAILABLE + (int)m_PermittedPlay);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt ob eine Sonderbehandlung mit diesem Stapel durchgeführt wird ins LogFile
void CPile::ReportSpecialHandling(void)
{
	// zu_tun
	// als String Resource
	CString Msg("Sonderbehandlung: ");
	if (m_SpecialHandling)
	{
		Msg += "ja -> ";
		Msg += m_SpecialHandlingText;
	}
	else
		Msg += "nein";
	GetApplication()->LogMessage(Msg);
}

void CPile::ReportUpdateOverlap(void)
{
	// zu_tun
	// als String Resource
	CString Msg("Überlappung aktualisieren: ");
	if (m_UpdateOverlap)
	{
		Msg += "ja";
	}
	else
		Msg += "nein";
	GetApplication()->LogMessage(Msg);
}

void CPile::ReportProperties()
{
	ReportSerializeVersion();   
	ReportEmptyBitmapID();
	ReportPilePosition();       
	ReportMaxSize();            
	ReportNumberOnStart();      
	ReportNumberHiddenOnStart();
	ReportFaceAtStart();        
	ReportNumberPerMove();      
	ReportNumberOfMaxCards();   
	ReportKindOfOverlapped();   
	ReportKindOfSelecting();    
	ReportKindOfPiling();       
	ReportPutIfEmpty();         
	ReportDealOrder();          
	ReportDealing();            
	ReportNumberPerDeal();      
	ReportRedealOrder();        
	ReportPermittedPlay();
	ReportUpdateOverlap();
	ReportSpecialHandling();
}

// liefert die Karte, falls im Pile vorhanden
CCard* CPile::GetCard(CString& CardName)
{
	CCard* pCard = NULL;
	POSITION pos = m_Cards.GetHeadPosition();
	while (pos)
	{
		pCard = m_Cards.GetNext(pos);
		if (pCard->GetCardName() == CardName)
			return pCard;
	}
	return NULL;
}

// liefert die Karte, falls im Pile vorhanden
CCard* CPile::GetCard(int CardValue)
{
	CCard* pCard = NULL;
	POSITION pos = m_Cards.GetHeadPosition();
	while (pos)
	{
		pCard = m_Cards.GetNext(pos);
		if (pCard->m_nCardValue == CardValue)
			return pCard;
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// diese Funktion berechnet den OverlappedFactor des Pile so, dass die Anzahl
// NumberOfCards Karten eines Piles in m_MaxSize liegen
//
double CPile::ComputeOverlappedFactor(int NumberOfCards)
{
	CSize ComputedPileSize(0,0);
	BOOL UseDefaults = FALSE;

	// Überprüfung, ob der Pile mit den default-Werten in m_MaxSize reinpaßt
	if (NumberOfCards == 1)
	{
		// die Größe ist genau eine Karte groß
		ComputedPileSize = CCyberSolitaireApp::DefaultCardSize;
		ASSERT((ComputedPileSize.cx <= m_MaxSize.cx) && (ComputedPileSize.cy <= m_MaxSize.cy));
		UseDefaults = TRUE;
	}
	else
	{
		switch (m_KindOfOverlapped) {
		case olmNotOverlapped:
			// die Größe ist genau eine Karte groß
			ComputedPileSize = CCyberSolitaireApp::DefaultCardSize;
			ASSERT((ComputedPileSize.cx <= m_MaxSize.cx) && (ComputedPileSize.cy <= m_MaxSize.cy));
			UseDefaults = TRUE;
			break;
		case olmDownOverlapped:
			ComputedPileSize.cy = (NumberOfCards - 1) *  CCyberSolitaireApp::DefaultDownOverlappedSize.cy +
								  CCyberSolitaireApp::DefaultCardSize.cy;
			ComputedPileSize.cx = CCyberSolitaireApp::DefaultCardSize.cx;
			UseDefaults = (ComputedPileSize.cy < m_MaxSize.cy);
			break;
		case olmRigthOverlapped:
			ComputedPileSize.cx = (NumberOfCards - 1) *  CCyberSolitaireApp::DefaultRightOverlappedSize.cx +
								  CCyberSolitaireApp::DefaultCardSize.cx;
			ComputedPileSize.cy = CCyberSolitaireApp::DefaultCardSize.cy;
			UseDefaults = (ComputedPileSize.cx < m_MaxSize.cx);
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	// wir können die default Werte benutzen
	if (UseDefaults)
	{
		return CCyberSolitaireApp::DefaultOverlappedFactor;
	}

	// es muß ein neuer Überlappungsfaktor berechnet werden
	ASSERT(NumberOfCards > 1);
	// um wieviel muß verkleinert werden
	int    TmpValue(0);
	switch (m_KindOfOverlapped) {
	case olmDownOverlapped:
		// Länge für die Überlappungen ( Gesamtlänge - 1 Karte)
		TmpValue = m_MaxSize.cy - CCyberSolitaireApp::DefaultCardSize.cy;
		return (double)TmpValue / ((double)CCyberSolitaireApp::DefaultCardSize.cy * (NumberOfCards-1));
	case olmRigthOverlapped:
		// Länge für die Überlappungen ( Gesamtlänge - 1 Karte)
		TmpValue = m_MaxSize.cx - CCyberSolitaireApp::DefaultCardSize.cx;
		return (double)TmpValue / ((double)CCyberSolitaireApp::DefaultCardSize.cx * (NumberOfCards-1));
	default:
		ASSERT(0);
		return 0.0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Diese Funktion füllt die Bitmap für den leeren Stapel mit dem Hintergrund
//
void CPile::CopyBackgroundToEmptyAppearence(CDC *pDC, CDibs& Dibs)
{
	// Umrechnung log. -> Bitmapkoordinaten
	CSize devVisibleSize = GetBitmapSize(&m_dibEmpty);
	CRect	devCardRect(ZeroPoint,devVisibleSize);

	// die Bitmap denkt in Pixeln, deshalb umrechnen LP->DP
	CPoint Position(m_Position);
	pDC->LPtoDP(&Position);
	CSize VisibleSize(CCyberSolitaireApp::DefaultCardSize);
	pDC->LPtoDP(&VisibleSize);
	CRect	CardRect(Position,VisibleSize);
	// Background Dib erzeugen
	CDib* pBackDib = new CDib;
	// wieder zurück
	//pBackDib->Create(*pDC,devVisibleSize,&m_dibEmpty);
	pBackDib->Create(*pDC,devVisibleSize,Dibs.pBackgroundDib);
	pBackDib->CopyBitmap(*pDC,Dibs.pBackgroundDib,&devCardRect,&CardRect);
	CDC memDC,backDC;
	if (!memDC.CreateCompatibleDC(pDC) || !backDC.CreateCompatibleDC(pDC)) 
	{
		TRACE0("Unable to create compatible DC's\n");
		return;
	}
	CBitmap* pOldBm = memDC.SelectObject(&m_dibEmpty);
	CBitmap* pOldBack = backDC.SelectObject(pBackDib);
    GdiFlush();
	for (int i=0; i<m_dibEmpty.m_TransPoints.GetSize(); i++)
	{
		CPoint TransPoint = m_dibEmpty.m_TransPoints[i];
		memDC.SetPixel(TransPoint.x,TransPoint.y,backDC.GetPixel(TransPoint.x,TransPoint.y));
	}
	memDC.SelectObject(pOldBm);
	backDC.SelectObject(pOldBack);
	pBackDib->DeleteObject();
	delete pBackDib;
}


// schreibt die Serialize Version ins LogFile
void CStock::ReportSerializeVersion(void)
{
	// zu_tun
	// als String Resource
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	Msg.Format("CStock Version: %d",m_StockVersion);
	GetApplication()->LogMessage(Msg);
}

// schreibt woher kommen die Karten fürs Redeal ins LogFile
void CStock::ReportRedealing(void)
{
	// zu_tun
	// als String Resource
	CString Msg("woher stammen die Karten bei Neu geben: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_REDEAL_NOTAVAILABLE + (int)m_RedealMethod);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt wie oft Neu gegeben werden kann ins LogFile
void CStock::ReportNumberOfRedeal(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Wie oft kann Neu gegeben werden: %d",m_NumberOfRedeal);
	GetApplication()->LogMessage(Msg);
}

void CStock::ReportProperties()
{
	ReportSerializeVersion();
	ReportRedealing();       	
	ReportNumberOfRedeal();  	
	CPile::ReportProperties();   
}

// schreibt die Serialize Version ins LogFile
void CFoundation::ReportSerializeVersion(void)
{
	// zu_tun
	// als String Resource
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	Msg.Format("CFoundation Version: %d",m_FoundationVersion);
	GetApplication()->LogMessage(Msg);
}

// schreibt Auswahl der Basiskarte ins LogFile
void CFoundation::ReportKindOfBasicCard(void)
{
	// zu_tun
	// als String Resource
	CString Msg("Auswahl der Basiskarte: ");
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_BAS_NOTAVAILABLE + (int)m_KindOfBasicCard);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
}

// schreibt können Karten zurückgespielt werden ins LogFile
void CFoundation::ReportReplayable(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Kann zurückgespielt werden: %d",m_Replayable);
	GetApplication()->LogMessage(Msg);
}

// schreibt Basiskartenwert ins LogFile
void CFoundation::ReportBasicCardValue(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("Basiskarte: %d",m_BasicCardValue);
	GetApplication()->LogMessage(Msg);
}

// schreibt TRUE: nur Kartenwert nicht die Farbe ins LogFile
void CFoundation::ReportBasicCardOnlyVal(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("nur Kartenwert nicht die Farbe: %d",m_BasicCardOnlyVal);
	GetApplication()->LogMessage(Msg);
}

// schreibt TRUE: nur Farbe und zusätzlich Kartenwert ins LogFile
void CFoundation::ReportBasicCardOnlyColor(void)
{
	// zu_tun
	// als String Resource
	CString Msg;
	Msg.Format("nur Farbe und zusätzlich Kartenwert: %d",m_BasicCardOnlyColor);
	GetApplication()->LogMessage(Msg);
}

void CFoundation::ReportProperties()
{
	ReportSerializeVersion();
	ReportReplayable();      
	ReportKindOfBasicCard(); 	
	ReportBasicCardValue();  	
	ReportBasicCardOnlyVal();	
	ReportBasicCardOnlyColor();	
	CPile::ReportProperties();   
}


// schreibt die Serialize Version ins LogFile
void CTableau::ReportSerializeVersion(void)
{
	// zu_tun
	// als String Resource
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	Msg.Format("CTableau Version: %d",m_TableauVersion);
	GetApplication()->LogMessage(Msg);
}

void CTableau::ReportProperties()
{
	ReportSerializeVersion();
	CPile::ReportProperties();   
}

// schreibt die Serialize Version ins LogFile
void CWaste::ReportSerializeVersion(void)
{
	// zu_tun
	// als String Resource
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	Msg.Format("CWaste Version: %d",m_WasteVersion);
	GetApplication()->LogMessage(Msg);
}

void CWaste::ReportProperties()
{
	ReportSerializeVersion();
	CPile::ReportProperties();   
}



//////////////////////////////////////////////////////////////////////
// CMovePile Klasse
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMovePile::CMovePile()
{
	//TRACE("constructor (standard) :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
	m_MovePileVersion = MOVEPILE_VERSION;
}

CMovePile::CMovePile(const CMovePile& p) : CPile(p)
{
	//TRACE("constructor (copy)     :%s(%p)[%s(%p)]\n",GetRuntimeClass()->m_lpszClassName,this,p.GetRuntimeClass()->m_lpszClassName,&p);
	m_MovePileVersion		= p.m_MovePileVersion;
}

CMovePile::~CMovePile()
{
	//TRACE("destructor             :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
}

const CMovePile& CMovePile::operator =(const CMovePile& p)
{
	(CPile&)*this		= (CPile&)p;
	m_MovePileVersion		= p.m_MovePileVersion;
	return *this;
}

const CMovePile& CMovePile::operator =(const CMovePile* p)
{
	(CPile&)*this		= (CPile*)p;
	m_MovePileVersion	= p->m_MovePileVersion;
	return *this;
}

void CMovePile::Serialize(CArchive &ar)
{
	CPile::Serialize(ar);
	if (ar.IsStoring()) 
	{
		m_MovePileVersion = MOVEPILE_VERSION;
		ar << m_MovePileVersion;
	}
	else 
	{
		ar >> m_MovePileVersion;
	}
}

//////////////////////////////////////////////////////////////////////
// CStock Klasse
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CStock::CStock()
{
	//TRACE("constructor (standard) :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
	m_StockVersion	= STOCK_VERSION;
	m_NumberOfRedeal= 0;
	m_RedealMethod  = redealNotAvailable;
}

CStock::CStock(const CStock& p) : CPile(p)
{
	//TRACE("constructor (copy)     :%s(%p)[%s(%p)]\n",GetRuntimeClass()->m_lpszClassName,this,p.GetRuntimeClass()->m_lpszClassName,&p);
	m_StockVersion		= p.m_StockVersion;
	m_NumberOfRedeal	= p.m_NumberOfRedeal;
	m_RedealMethod		= p.m_RedealMethod;
}

CStock::~CStock()
{
	//TRACE("destructor             :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
}

const CStock& CStock::operator =(const CStock& p)
{
	(CPile&)*this		= (CPile&)p;
	m_StockVersion		= p.m_StockVersion;
	m_NumberOfRedeal	= p.m_NumberOfRedeal;
	m_RedealMethod		= p.m_RedealMethod;
	return *this;
}

const CStock& CStock::operator =(const CStock* p)
{
	(CPile&)*this		= (CPile*)p;
	m_StockVersion		= p->m_StockVersion;
	m_NumberOfRedeal	= p->m_NumberOfRedeal;
	m_RedealMethod		= p->m_RedealMethod;
	return *this;
}

void CStock::Serialize(CArchive &ar)
{
	CPile::Serialize(ar);
	if (ar.IsStoring()) 
	{
		m_StockVersion = STOCK_VERSION;
		ar << m_StockVersion;
		ar << m_NumberOfRedeal;
		ar.Write(&m_RedealMethod,sizeof(m_RedealMethod));

	}
	else 
	{
		ar >> m_StockVersion;
		if (m_StockVersion > 2)
			ar >> m_NumberOfRedeal;
		else
			m_NumberOfRedeal = 0;
		if (m_StockVersion > 3)
			ar.Read(&m_RedealMethod,sizeof(m_RedealMethod));
		else
			m_RedealMethod  = redealNotAvailable;
	}
}

//////////////////////////////////////////////////////////////////////
// CFoundation Klasse
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CFoundation::CFoundation()
: m_BasicCardOnlyColor(FALSE)
{
	//TRACE("constructor (standard) :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
	m_FoundationVersion = FOUNDATION_VERSION;
	m_Replayable		= FALSE;
	m_BasicCardValue	= 0;
	m_BasicCardOnlyVal	= FALSE;
	m_BasicCardOnlyColor= FALSE;
	m_KindOfBasicCard	= basNotAvailable;

}

CFoundation::CFoundation(const CFoundation& p) : CPile(p)
{
	//TRACE("constructor (copy)     :%s(%p)[%s(%p)]\n",GetRuntimeClass()->m_lpszClassName,this,p.GetRuntimeClass()->m_lpszClassName,&p);
	m_FoundationVersion		= p.m_FoundationVersion;
	m_Replayable			= p.m_Replayable;
	m_BasicCardValue		= p.m_BasicCardValue;
	m_BasicCardOnlyVal		= p.m_BasicCardOnlyVal;
	m_BasicCardOnlyColor	= p.m_BasicCardOnlyColor;
	m_KindOfBasicCard		= p.m_KindOfBasicCard;
}

CFoundation::~CFoundation()
{
	//TRACE("destructor             :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
}

const CFoundation& CFoundation::operator =(const CFoundation& p)
{
	(CPile&)*this		= (CPile&)p;
	m_FoundationVersion		= p.m_FoundationVersion;
	m_Replayable			= p.m_Replayable;
	m_BasicCardValue		= p.m_BasicCardValue;
	m_BasicCardOnlyVal		= p.m_BasicCardOnlyVal;
	m_BasicCardOnlyColor	= p.m_BasicCardOnlyColor;
	m_KindOfBasicCard		= p.m_KindOfBasicCard;
	return *this;
}

const CFoundation& CFoundation::operator =(const CFoundation* p)
{
	(CPile&)*this			= (CPile*)p;
	m_FoundationVersion		= p->m_FoundationVersion;
	m_Replayable			= p->m_Replayable;
	m_BasicCardValue		= p->m_BasicCardValue;
	m_BasicCardOnlyVal		= p->m_BasicCardOnlyVal;
	m_BasicCardOnlyColor	= p->m_BasicCardOnlyColor;
	m_KindOfBasicCard		= p->m_KindOfBasicCard;
	return *this;
}

void CFoundation::Serialize(CArchive &ar)
{
	CPile::Serialize(ar);
	if (ar.IsStoring()) 
	{
		m_FoundationVersion = FOUNDATION_VERSION;
		ar << m_FoundationVersion;
		ar << m_Replayable;
		// Version 2
		ar << m_BasicCardValue << m_BasicCardOnlyVal;
		ar.Write(&m_KindOfBasicCard,sizeof(m_KindOfBasicCard));
		// Version 3
		ar << m_BasicCardOnlyColor;
	}
	else 
	{
		ar >> m_FoundationVersion;
		ar >> m_Replayable;
		if (m_FoundationVersion > 1)
		{
			// Version 2
			ar >> m_BasicCardValue >> m_BasicCardOnlyVal;
			ar.Read(&m_KindOfBasicCard,sizeof(m_KindOfBasicCard));
		}
		if (m_FoundationVersion > 2)
		{
			// Version 3
			ar >> m_BasicCardOnlyColor;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// CTableau Klasse
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CTableau::CTableau()
{
	//TRACE("constructor (standard) :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
	m_TableauVersion = TABLEAU_VERSION;
}

CTableau::CTableau(const CTableau& p) : CPile(p)
{
	//TRACE("constructor (copy)     :%s(%p)[%s(%p)]\n",GetRuntimeClass()->m_lpszClassName,this,p.GetRuntimeClass()->m_lpszClassName,&p);
	m_TableauVersion		= p.m_TableauVersion;
}

CTableau::~CTableau()
{
	//TRACE("destructor             :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
}

const CTableau& CTableau::operator =(const CTableau& p)
{
	(CPile&)*this		= (CPile&)p;
	m_TableauVersion		= p.m_TableauVersion;
	return *this;
}

const CTableau& CTableau::operator =(const CTableau* p)
{
	(CPile&)*this		= (CPile*)p;
	m_TableauVersion	= p->m_TableauVersion;
	return *this;
}

void CTableau::Serialize(CArchive &ar)
{
	CPile::Serialize(ar);
	if (ar.IsStoring()) 
	{
		m_TableauVersion = TABLEAU_VERSION;
		ar << m_TableauVersion;
	}
	else 
	{
		ar >> m_TableauVersion;
	}
}


//////////////////////////////////////////////////////////////////////
// CWaste Klasse
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CWaste::CWaste()
{
	//TRACE("constructor (standard) :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
	m_WasteVersion = WASTE_VERSION;
}

CWaste::CWaste(const CWaste& p) : CPile(p)
{
	//TRACE("constructor (copy)     :%s(%p)[%s(%p)]\n",GetRuntimeClass()->m_lpszClassName,this,p.GetRuntimeClass()->m_lpszClassName,&p);
	m_WasteVersion		= p.m_WasteVersion;
}

CWaste::~CWaste()
{
	//TRACE("destructor             :%s(%p)\n",GetRuntimeClass()->m_lpszClassName,this);
}

const CWaste& CWaste::operator =(const CWaste& p)
{
	(CPile&)*this		= (CPile&)p;
	m_WasteVersion		= p.m_WasteVersion;
	return *this;
}

const CWaste& CWaste::operator =(const CWaste* p)
{
	(CPile&)*this		= (CPile*)p;
	m_WasteVersion	= p->m_WasteVersion;
	return *this;
}

void CWaste::Serialize(CArchive &ar)
{
	CPile::Serialize(ar);
	if (ar.IsStoring()) 
	{
		m_WasteVersion = WASTE_VERSION;
		ar << m_WasteVersion;
	}
	else 
	{
		ar >> m_WasteVersion;
	}
}

