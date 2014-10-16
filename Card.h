// Cards.h: Schnittstelle f�r die Klasse CCard.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDS_H__741739C5_7F1D_11D4_A917_0000B48B263B__INCLUDED_)
#define AFX_CARDS_H__741739C5_7F1D_11D4_A917_0000B48B263B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Farben des Kartenspiels
enum CARD_SUIT {
	CLUB,					// Kreuz
	SPADE,					// Pik
	HEART,					// Herz
	DIAMOND					// Karo
};

// Farben rot oder schwarz
enum CARD_COLOR {
	BLACK,					// schwarz
	RED						// rot
};

// Versionen f�r Serialize
#define	CARD_VERSION			2


// Vorw�rtsdeklarationen
class CPile;							
class CCyberSolitaireView;
class CSolEditView;

struct CDibs;

/*
	es gibt 3 verschiedene Koordinaten
	z.B.:	m_LogicalSize		logische Koordinaten (const)
			m_dpSize	device	 Koordinaten (abh�ngig von der Fenstergr��e)
			m_PixelSize	pixel    Koordinaten (gegeben durch die Ursprungsbitmap)
*/
class CCard : public CObject  
{
    DECLARE_SERIAL(CCard)
public:
	CCard();
    CCard(const CCard& s) {
		m_CardVersion		= s.m_CardVersion;
		m_LogicalPosition			= s.m_LogicalPosition;		
		m_LogicalSize				= s.m_LogicalSize;		
		m_IsFaceUp			= s.m_IsFaceUp;		
		m_IsVisible			= s.m_IsVisible;		
		m_LogicalVisibleSize		= s.m_LogicalVisibleSize;	
		m_PixelVisibleSize	= s.m_PixelVisibleSize;	
		m_xFac				= s.m_xFac;	
		m_yFac				= s.m_yFac;	
		m_CardName			= s.m_CardName;
    }
	CCard(int CardID);
	virtual ~CCard();

public:

static CRect	LastDrawedCardRect;		// letztes gezeichnetes CardRect
										// um Gr��en�nderungen wegen Rundungsfehlern zu vermeiden

	// Members
	int			m_CardVersion;			// Version f�r Serialize
	int			m_InGameID;				// Kartenwert + SetNummer * 52
	int			m_nCardValue;			// Kartenwert	 1 = Kreuz As 
										//				14 = Karo As 
										//				27 = Herz As
										//				40 = Pik As
	CPoint		m_LogicalPosition;		// linke obere Ecke der Karte (logisch)
	CSize		m_LogicalSize;			// Gr��e der Karte (logisch)
	CSize		m_PixelSize;			// Gr��e der Karte (pixel)
	double		m_xFac;					// Umrechnungsfaktoren zwischen logischen Koordinaten und Kartengr��e (Bitmap)
	double		m_yFac;
	BOOL		m_IsFaceUp;				// Bild sichtbar
	BOOL		m_IsVisible;			// muss diese Karte �berhaupt gezeichnet werden
	BOOL		m_IsSelected;
	CDib		m_dibPicture;			// DIP-Map des Kartenbildes
	CSize		m_LogicalVisibleSize;	// was ist von der Karte sichtbar -> wird auch f�r HitTest verwendet
	CSize		m_PixelVisibleSize;		// was ist von der Karte sichtbar (pixel)
	CString		m_CardName;				// Kartenname

// Operatoren
    const CCard& operator =(const CCard& s) {
		m_CardVersion		= s.m_CardVersion;
		m_LogicalPosition			= s.m_LogicalPosition;		
		m_LogicalSize				= s.m_LogicalSize;		
		m_IsFaceUp			= s.m_IsFaceUp;		
		m_IsVisible			= s.m_IsVisible;		
		m_LogicalVisibleSize		= s.m_LogicalVisibleSize;	
		m_PixelVisibleSize	= s.m_PixelVisibleSize;	
		m_xFac				= s.m_xFac;	
		m_yFac				= s.m_yFac;	
		m_CardName			= s.m_CardName;
        return *this;
    }
    const CCard& operator =(const CCard* s) {
		m_CardVersion		= s->m_CardVersion;
		m_LogicalPosition			= s->m_LogicalPosition;		
		m_LogicalSize				= s->m_LogicalSize;		
		m_IsFaceUp			= s->m_IsFaceUp;		
		m_IsVisible			= s->m_IsVisible;		
		m_LogicalVisibleSize		= s->m_LogicalVisibleSize;	
		m_PixelVisibleSize	= s->m_PixelVisibleSize;	
		m_xFac				= s->m_xFac;	
		m_yFac				= s->m_yFac;	
		m_CardName			= s->m_CardName;
        return *this;
    }

    BOOL operator !=(const CCard& s) {
		if ((m_CardVersion		!= s.m_CardVersion) ||
		    (m_IsFaceUp			!= s.m_IsFaceUp) ||
		    (m_IsVisible			!= s.m_IsVisible) ||
		    (m_CardName			!= s.m_CardName))
			return TRUE;
        return FALSE;
    }

    BOOL operator !=(const CCard* s) {
		if ((m_CardVersion		!= s->m_CardVersion) ||
		    (m_IsFaceUp			!= s->m_IsFaceUp) ||
		    (m_IsVisible			!= s->m_IsVisible) ||
		    (m_CardName			!= s->m_CardName))
			return TRUE;
        return FALSE;
    }

public:
	BOOL IsKing();
	BOOL IsAce();
	BOOL IsDownInSuit(CCard* pCard);
	BOOL IsDownAlternate(CCard* pCard);
	BOOL IsUpInSuit(CCard* pCard);
	BOOL IsUpAlternate(CCard* pCard);
	BOOL IsDownInSuitRolling(CCard* pCard);
	BOOL IsDownAlternateRolling(CCard* pCard);
	BOOL IsUpInSuitRolling(CCard* pCard);
	BOOL IsUpAlternateRolling(CCard* pCard);
	void GetCardName(CString& CardName);
	CString GetCardName() {return m_CardName;}
	BOOL IsDownInSequence(CCard* pCard);
	BOOL IsDownInSequenceRolling(CCard* pCard);
	BOOL IsUpInSequence(CCard* pCard);
	BOOL IsUpInSequenceRolling(CCard* pCard);
	CARD_SUIT GetSuit();
	CARD_COLOR GetColor();
	void Display(CDC* pDC, CDibs& Dibs, BOOL Transparent = TRUE);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void Serialize(CArchive& ar);

	BOOL GetFaceUp(void);
	void SetFaceUp(BOOL FaceUp);
	CRect GetCardRect(void);
	CRect GetVisibleRect(void) {CRect TmpRect(m_LogicalPosition,m_LogicalVisibleSize); return TmpRect;}
	BOOL HitTest(CPoint point);

	BOOL IsBasicCard(int BasicCardValue, BOOL OnlyBasicCardVal = TRUE, BOOL OnlyBasicCardColor = FALSE);
	// gibt den Kartenwert zur�ck
	int GetValue(void);
};

template <> void AFXAPI SerializeElements <CCard> ( CArchive& ar, CCard* pNewCard, int nCount );

class  CCardList : public CTypedPtrList<CObList, CCard*>
{


public:
// Operatoren
    const CCardList& operator =(const CCardList& s) {
		RemoveAll();
		POSITION pos = s.GetHeadPosition();
		while (pos)
			AddTail(s.GetNext(pos));
        return *this;
    }

    const CCardList& operator =(const CCardList* s) {
		RemoveAll();
		POSITION pos = s->GetHeadPosition();
		while (pos)
			AddTail(s->GetNext(pos));
        return *this;
    }
};

class CCardArray : public CTypedPtrArray<CObArray, CCard*>
{


public:
// Operatoren
    const CCardArray& operator =(const CCardArray& s) {
		RemoveAll();
		if (s.GetSize() != 0)
			for (int i = 0; i < s.GetSize(); i++)
				Add(s.GetAt(i));

        return *this;
    }

    const CCardArray& operator =(const CCardArray* s) {
		RemoveAll();
		if (s->GetSize() != 0)
			for (int i = 0; i < s->GetSize(); i++)
				Add(s->GetAt(i));

        return *this;
    }
};

#endif // !defined(AFX_CARDS_H__741739C5_7F1D_11D4_A917_0000B48B263B__INCLUDED_)
