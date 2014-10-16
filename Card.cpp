// Cards.cpp: Implementierung der Klasse CCard.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "CyberSolitaireView.h"
#include "SolEditView.h"
#include "Piles.h"
#include "card.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRect	CCard::LastDrawedCardRect(0,0,0,0);

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CCard, CObject, 1)

// Achtung !!! dieser Konstruktor wird beim Lesen des Archivs benutzt !!!
CCard::CCard()
{
	int a=1;
}

CCard::CCard(int CardID)
{
	m_CardVersion = CARD_VERSION;
	m_InGameID			= CardID;
	if (m_InGameID <= 52)
	{
		m_nCardValue	= m_InGameID;
	}
	else
	{
		m_nCardValue	= m_InGameID-52;
	}

	//VERIFY(m_dibPicture.Load(AfxGetApp()->m_hInstance,IDB_BITMAP1-1+m_nCardValue));
	VERIFY(m_dibPicture.Load(AfxGetApp()->m_hInstance,IDB_BITMAP70-1+m_nCardValue));

	m_IsFaceUp			= FALSE;		
	m_IsVisible			= FALSE;
	m_IsSelected		= FALSE;
	m_LogicalPosition			= NoPosition;
	m_LogicalSize				= CCyberSolitaireApp::DefaultCardSize;
	m_PixelSize			= m_dibPicture.GetSize();
	// Umrechnungsfaktoren zwischen logischen Koordinaten und Kartengröße (Bitmap)
	m_xFac = (double)m_LogicalSize.cx / (double)m_PixelSize.cx;
	m_yFac = (double)m_LogicalSize.cy / (double)m_PixelSize.cy;
	m_LogicalVisibleSize		= m_LogicalSize;
	GetCardName(m_CardName);
}	

CCard::~CCard()
{

}

/////////////////////////////////////////////////////////////////////////////
// CCard Diagnose

#ifdef _DEBUG
void CCard::AssertValid() const
{
	CObject::AssertValid();
}

void CCard::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	CString CardName("no name");
	((CCard&)*this).GetCardName(CardName);
	CRect CardRect = ((CCard&)*this).GetCardRect();
	CString strCardRect;
	strCardRect.Format("log. Rect(%d,%d,%d,%d)",CardRect.left,CardRect.top,CardRect.right,CardRect.bottom);
	CString DbgOut;
	DbgOut.Format("Id:%d %s %s\n",m_InGameID,CardName,strCardRect);
	//TRACE("%s",DbgOut);
	dc << "Id:" << m_InGameID <<"\t"<< CardName <<"\t"<< strCardRect <<"\n";
}

#endif //_DEBUG


void CCard::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_CardVersion = CARD_VERSION;
		ar << m_CardVersion;
		ar << m_InGameID << m_nCardValue << m_LogicalPosition << m_LogicalSize << m_PixelSize	<< m_xFac << m_yFac
		   << m_IsFaceUp << m_IsVisible << m_IsSelected << m_LogicalVisibleSize  << m_PixelVisibleSize 
		   << m_CardName; 
	}
	else 
	{
		ar >> m_CardVersion;
		ar >> m_InGameID >> m_nCardValue >> m_LogicalPosition >> m_LogicalSize >> m_PixelSize	>> m_xFac >> m_yFac
		   >> m_IsFaceUp >> m_IsVisible >> m_IsSelected >> m_LogicalVisibleSize >> m_PixelVisibleSize; 
		if (m_CardVersion >= 2)
			ar >> m_CardName;
		else
			GetCardName(m_CardName);
		// wie komme ich jetzt an die Bitmap
		VERIFY(m_dibPicture.Load(AfxGetApp()->m_hInstance,IDB_BITMAP70-1+m_nCardValue));
	}
	//Dump(afxDump);
}

BOOL CCard::HitTest(CPoint point)
{
	// point in log. Koordinaten
	if (!m_IsVisible)
		return FALSE;
	CRect VisibleRect(m_LogicalPosition,m_LogicalVisibleSize);
	return VisibleRect.PtInRect(point);
}

CRect CCard::GetCardRect()
{
	CRect CardRect(m_LogicalPosition,m_LogicalSize);
	return CardRect;
}


BOOL CCard::GetFaceUp()
{
	return m_IsFaceUp;
}

void CCard::SetFaceUp(BOOL FaceUp)
{
	m_IsFaceUp = FaceUp;
	//GetApplication()->LogMessage("%s\tface changed to %d\n",GetCardName(),m_IsFaceUp);

}

template <> void AFXAPI SerializeElements <CCard> ( CArchive& ar, CCard* pNewCard, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewCard++ )
    {
        // jedes Objekt serialisieren
        pNewCard->Serialize( ar );
    }
}


void CCard::Display(CDC* pDC, CDibs& Dibs, BOOL Transparent /* = TRUE */)
{
	// für Test
	//return;
	// Achtung pDC muß schon PrepareDC durchlaufen haben
	// muß überhaupt gezeichnet werden?
	if(!m_IsVisible) return;
	// Umrechnung log. -> Bitmapkoordinaten
	CSize devVisibleSize((int)(m_LogicalVisibleSize.cx/m_xFac),(int)(m_LogicalVisibleSize.cy/m_yFac));
	CRect	devCardRect(ZeroPoint,devVisibleSize);
	
	// die Bitmap denkt in Pixeln, deshalb umrechnen LP->DP
	CPoint Position(m_LogicalPosition);
	pDC->LPtoDP(&Position);
	CSize VisibleSize(m_LogicalVisibleSize);
	pDC->LPtoDP(&VisibleSize);
	CRect	CardRect(Position,VisibleSize);
	// Ungenauigkeiten durch Rundungen beseitigen
	if (LastDrawedCardRect.IsRectNull())
	{
		LastDrawedCardRect = CardRect;
	}
	else
	{
		if (m_LogicalVisibleSize.cy < CCyberSolitaireApp::DefaultCardSize.cy)
			CardRect.top = LastDrawedCardRect.bottom;
		if (m_LogicalVisibleSize.cx < CCyberSolitaireApp::DefaultCardSize.cx)
			CardRect.left = LastDrawedCardRect.right;
		if (m_LogicalVisibleSize == CCyberSolitaireApp::DefaultCardSize)
		{
			if (CardRect.right == LastDrawedCardRect.right)
			{
				CardRect.top = LastDrawedCardRect.bottom;
			}
			if (CardRect.top == LastDrawedCardRect.top)
			{
				CardRect.left = LastDrawedCardRect.right;
			}
		}
		LastDrawedCardRect = CardRect;
	}
	if (m_IsFaceUp)
	{
		if (Transparent)
			Dibs.pScreenDib->CopyBitmapTransparent(*pDC,&m_dibPicture,Dibs.pBackgroundDib,&CardRect,&devCardRect);
		else
			Dibs.pScreenDib->CopyBitmap(*pDC,&m_dibPicture,&CardRect,&devCardRect);
	}
	else
	{
		if (Transparent)
			Dibs.pScreenDib->CopyBitmapTransparent(*pDC,Dibs.pCardBackDib,Dibs.pBackgroundDib,&CardRect,&devCardRect);
		else
			Dibs.pScreenDib->CopyBitmap(*pDC,Dibs.pCardBackDib,&CardRect,&devCardRect);
	}
	if (m_IsSelected)
	{
		Dibs.pScreenDib->CopyBitmap(*pDC,Dibs.pCardSelectedDib,&CardRect,&devCardRect,SRCAND);
	}
	CString CardName("");
	GetCardName(CardName);
	//TRACE("Display Card:%s \tat Position(%d,%d)\n",CardName,m_LogicalPosition.x,m_LogicalPosition.y);
	//TRACE("CardRect (%d,%d,%d,%d)\n",CardRect.left,CardRect.top,CardRect.right,CardRect.bottom);
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die Farbe (Kreuz,Pik,Herz,Karo) der Karte
//
CARD_SUIT CCard::GetSuit()
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As   
	if ((m_nCardValue >= 1) &&	(m_nCardValue <= 13))
		return CLUB;
	if ((m_nCardValue >= 14) &&	(m_nCardValue <= 26))
		return DIAMOND;
	if ((m_nCardValue >= 27) &&	(m_nCardValue <= 39))
		return HEART;
	if ((m_nCardValue >= 40) &&	(m_nCardValue <= 52))
		return SPADE;
	ASSERT(0);
	return (CARD_SUIT)NA;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die Farbe (rot oder schwarz) der Karte
//
CARD_COLOR CCard::GetColor()
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As   
	if ((m_nCardValue >= 1) &&	(m_nCardValue <= 13) || (m_nCardValue >= 40) &&	(m_nCardValue <= 52))
		return BLACK;
	if ((m_nCardValue >= 14) &&	(m_nCardValue <= 26) || (m_nCardValue >= 27) &&	(m_nCardValue <= 39))
		return RED;
	ASSERT(0);
	return (CARD_COLOR)NA;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion prüft, ob die Karte im Wert genau 1 hinter pCard liegt
//
BOOL CCard::IsDownInSequence(CCard *pCard)
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As
	int Value1 = (pCard->m_nCardValue-1) % 13;
	int Value2 = (m_nCardValue-1) % 13;
	return (Value2 == (Value1 + 1));
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion prüft, ob die Karte im Wert genau 1 hinter pCard liegt
// mit Überlauf (As->König)
//
BOOL CCard::IsDownInSequenceRolling(CCard *pCard)
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As
	int Value1 = (pCard->m_nCardValue-1) % 13;
	int Value2 = (m_nCardValue-1) % 13;
	if (Value2 == 0)
		return ((Value2 + 13) == (Value1 + 1));
	else
		return (Value2 == (Value1 + 1));
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion prüft, ob die Karte im Wert genau 1 nach pCard liegt
//
BOOL CCard::IsUpInSequence(CCard *pCard)
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As
	int Value1 = (pCard->m_nCardValue-1) % 13;
	int Value2 = (m_nCardValue-1) % 13;
	return ((Value2 + 1) == Value1);
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion prüft, ob die Karte im Wert genau 1 nach pCard liegt
// mit Überlauf (König->As)
//
BOOL CCard::IsUpInSequenceRolling(CCard *pCard)
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As
	int Value1 = (pCard->m_nCardValue-1) % 13;
	int Value2 = (m_nCardValue-1) % 13;
	if (Value1 == 0)
		return ((Value2 + 1) == (Value1 + 13));
	else
		return ((Value2 + 1) == Value1);
}

//////////////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die Farbe (Kreuz,Pik,Herz,Karo) & Wert der Karte als String
//
void CCard::GetCardName(CString & CardName)
{
	// Farbe bestimmmen
	CARD_SUIT Color = GetSuit();
	switch (Color) {
	case CLUB:
		CardName = "Kreuz ";
		break;
	case DIAMOND:
		CardName = "Karo ";
		break;
	case HEART:
		CardName = "Herz ";
		break;
	case SPADE:
		CardName = "Pik ";
		break;
	default:
		ASSERT(0);
		break;
	}
	int Value = (m_nCardValue) % 13;
	switch (Value) {
	case 0:
		CardName += "König";
		break;
	case 1:
		CardName += "As";
		break;
	case 2:
		CardName += "2";
		break;
	case 3:
		CardName += "3";
		break;
	case 4:
		CardName += "4";
		break;
	case 5:
		CardName += "5";
		break;
	case 6:
		CardName += "6";
		break;
	case 7:
		CardName += "7";
		break;
	case 8:
		CardName += "8";
		break;
	case 9:
		CardName += "9";
		break;
	case 10:
		CardName += "10";
		break;
	case 11:
		CardName += "Bube";
		break;
	case 12:
		CardName += "Dame";
		break;
	default:
		ASSERT(0);
		break;
	}
}

BOOL CCard::IsDownInSuit(CCard *pCard)
{
	CARD_SUIT Suit1 = pCard->GetSuit();
	CARD_SUIT Suit2 = GetSuit();
	return IsDownInSequence(pCard) && (Suit1 == Suit2);
}

BOOL CCard::IsDownAlternate(CCard *pCard)
{
	CARD_COLOR Color1 = pCard->GetColor();
	CARD_COLOR Color2 = GetColor();
	return IsDownInSequence(pCard) && (Color1 != Color2);
}

BOOL CCard::IsDownInSuitRolling(CCard *pCard)
{
	CARD_SUIT Suit1 = pCard->GetSuit();
	CARD_SUIT Suit2 = GetSuit();
	return IsDownInSequenceRolling(pCard) && (Suit1 == Suit2);
}

BOOL CCard::IsDownAlternateRolling(CCard *pCard)
{
	CARD_COLOR Color1 = pCard->GetColor();
	CARD_COLOR Color2 = GetColor();
	return IsDownInSequenceRolling(pCard) && (Color1 != Color2);
}

BOOL CCard::IsUpInSuit(CCard *pCard)
{
	CARD_SUIT Suit1 = pCard->GetSuit();
	CARD_SUIT Suit2 = GetSuit();
	return IsUpInSequence(pCard) && (Suit1 == Suit2);
}

BOOL CCard::IsUpInSuitRolling(CCard *pCard)
{
	CARD_SUIT Suit1 = pCard->GetSuit();
	CARD_SUIT Suit2 = GetSuit();
	return IsUpInSequenceRolling(pCard) && (Suit1 == Suit2);
}

BOOL CCard::IsUpAlternate(CCard *pCard)
{
	CARD_COLOR Color1 = pCard->GetColor();
	CARD_COLOR Color2 = GetColor();
	return IsUpInSequence(pCard) && (Color1 != Color2);
}

BOOL CCard::IsUpAlternateRolling(CCard *pCard)
{
	CARD_COLOR Color1 = pCard->GetColor();
	CARD_COLOR Color2 = GetColor();
	return IsUpInSequenceRolling(pCard) && (Color1 != Color2);
}

BOOL CCard::IsAce()
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As
	int Value = m_nCardValue % 13;
	return (Value == 1);
}


BOOL CCard::IsKing()
{
// Kartenwert	 1 = Kreuz As 
//				14 = Karo As  
//				27 = Herz As  
//				40 = Pik As
	int Value = m_nCardValue % 13;
	return (Value == 0);
}

BOOL CCard::IsBasicCard(int BasicCardValue, BOOL OnlyBasicCardVal /* = TRUE */, BOOL OnlyBasicCardColor /* = FALSE */)
{
	BOOL RetVal1,RetVal2;
	if (!OnlyBasicCardVal)
		return  (m_nCardValue == BasicCardValue);
	else
		RetVal1 = ((m_nCardValue % 13) == (BasicCardValue % 13));
	CARD_COLOR BasicCardColor;
	if ((BasicCardValue >= 1) &&	(BasicCardValue <= 13) || (BasicCardValue >= 40) &&	(BasicCardValue <= 52))
		BasicCardColor = BLACK;
	if ((BasicCardValue >= 14) &&	(BasicCardValue <= 26) || (BasicCardValue >= 27) &&	(BasicCardValue <= 39))
		BasicCardColor = RED;
	CARD_COLOR Color = GetColor();
	if (OnlyBasicCardColor)
		RetVal2 = (BasicCardColor == Color);
	else
		RetVal2 = TRUE;
	return RetVal1 && RetVal2;
}

// gibt den Kartenwert zurück
int CCard::GetValue(void)
{
	return m_nCardValue % 13;
}
