#if !defined(PILES_H_INCLUDED_)
#define PILES_H_INCLUDED_

#if !defined(AFX_CARDS_H__741739C5_7F1D_11D4_A917_0000B48B263B__INCLUDED_)
#include "Card.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// defines
//

#define MAX_MIX		1000

// Versions for Serialize
#define	PILE_VERSION			8
#define	STOCK_VERSION			4
#define	FOUNDATION_VERSION		3
#define	TABLEAU_VERSION			1
#define	WASTE_VERSION			1
#define	MOVEPILE_VERSION		1

//////////////////////////////////////////////////////////////////////
// Attribute der einzelnen Piles (Kartenstapel)
//////////////////////////////////////////////////////////////////////

// tableau (Stapel auf denen gespielt wird

// welche Karten sind zu Beginn des Spieles sichtbar
enum pileFaceAtStart {
	fasNotAvailable		= 0,		// nicht verfügbar/relevant
	fasAllFaceUp		= 1,		// alle Karten des Stapels sind sichtbar
	fasAllFaceDown		= 2,		// keine Karte des Stapels ist sichtbar         
	fasLastFaceUp		= 3,		// nur die letzte Karte des Stapels ist sichtbar 
	fasOddFaceUp		= 4,		// jede ungerade (1.,3.,..) Karte ist sichtbar   
	fasEvenFaceUp		= 5			// jede gerade (2.,4.,..) Karte ist sichtbar     
};

// welche Art der Überlappung hat dieser Stapel
enum pileOverlappedMethod {
	olmNotAvailable				= 0,	// nicht verfügbar/relevant
	olmNotOverlapped			= 1,	// alle Karten liegen übereinander
	olmDownOverlapped			= 2,	// Karten überlappen nach unten
	olmRigthOverlapped			= 3,	// Karten überlappen nach rechts
	olmNotOverlappedHalfDown	= 4		// alle Karten liegen übereinander, nur die obere Hälfte ist sichtbar
};

// welche Sequenzen dürfen bewegt (selektiert) werden
enum pileSelectingMethod {
	selNotAvailable			= 0,		// nicht verfügbar/relevant
										// beliebige Sequenz -> also auch Stücke einer längeren	Sequenz
	selAnySeqInSuit			= 1,		// beliebige Sequenz Farbfolge	 
	selAnySeqAlternate		= 2,		// beliebige Sequenz Farbwechsel
	selAnySeqNoColor		= 3,		// beliebige Sequenz ohne Farbfolge
										// gesamte   Sequenz -> keine Stücke einer längeren Sequenz
	selAllSeqNoColorOrLast	= 4,		// gesamte   Sequenz ohne Farbfolge oder letzte Karte
	selAllSeqInSuitOrLast	= 5,		// gesamte   Sequenz Farbfolge      oder letzte Karte
	selAllSeqAlternateOrLast= 6,		// gesamte   Sequenz Farbwechsel    oder letzte Karte
	selAll					= 7,		// alle Karten des Stapels
	selOnlyLast				= 8,		// nur letzte Karte
	selExactOne				= 9,		// genau eine Karte des Stapels
	selAllFaceUp			= 10,		// alle sichtbaren Karten
	selPairsEqualValue		= 11,		// Paare gleichen Werts
	selPairsValue13			= 12		// Paare mit Wert 13
};

// wie ist die Reihenfolge beim Stapeln (bei der Ablage durch Spieler)
enum pilePiling {
	pilNotAvailable				= 0,		// nicht verfügbar/relevant
	pilDownInSuit				= 1,		// abwärts  Farbfolge
	pilDownAlternate			= 2,		// abwärts  Farbwechsel
	pilDownNoColor				= 3,		// abwärts  ohne Farbfolge
	pilUpInSuit					= 4,		// aufwärts Farbfolge
	pilUpAlternate				= 5,		// aufwärts Farbwechsel
	pilUpNoColor				= 6,		// aufwärts ohne Farbfolge
	pilWithGaps					= 7,		// mit Lücken
	pilOnlyFromStock			= 8,		// beliebig, aber nur vom Stock
	pilNothing					= 9,		// auf diesen Stapel kann nicht abgelegt werden
	pilDownInSuitRolling		= 10,		// abwärts  Farbfolge (mit Überlauf As->König)
	pilUpInSuitRolling			= 11,		// aufwärts Farbfolge (mit Überlauf König->As)
	pilDownAlternateRolling		= 12,		// abwärts  Farbwechsel (mit Überlauf As->König)
	pilUpAlternateRolling		= 13,		// aufwärts Farbwechsel (mit Überlauf König->As)
	pilPairsEqualValue			= 14,		// Paare gleichen Wertes
	pilPairsValue13OrKing		= 15,		// Paare mit Wert 13 oder einzelner König
	pilUpDownNoColorRolling		= 16,		// aufwärts oder abwärts mit Überlauf ohne Farbfolge
	pilOneFromTableauOrWaste	= 17,		// nur 1 Karte vom Tableau oder Waste
	pilOnlyFromTableau			= 18,		// beliebig, aber nur von Tableaus
	pilUpSameColor				= 19,		// aufwärts gleiche Farbe wie Basiskarte
	pilDownSameColor			= 20,		// abwärts gleiche Farbe wie Basiskarte
	pilUpDiff2InSuit			= 21		// aufwärts im Abstand von 2 in Farbfolge
};

// womit kann ein leerer Stapel belegt werden
enum pilePutIfEmpty {
	pieNotAvailable			= 0,		// nicht verfügbar/relevant
	pieNoCard				= 1,		// nicht mehr belegbar
	pieBasicCard			= 2,		// mit einer Basiskarte
	pieBasicCardMinus1		= 3,		// mit Basiskarte - 1
	pieAce					= 4,		// As
	pieKing					= 5,		// König
	pieFromDealOrAny		= 6,		// wird automatisch belegt oder beliebige Karte, wenn keine Karten zum Geben
	pieAnyCard				= 7,		// beliebiger Karte
	pieOnlyFromStock		= 8,		// darf nur vom Stock belegt werden
	pieOnlyFromAutoDeal		= 9,			// wird ausschließlich automatisch von den Deal-Piles belegt
	pieOnlyFromTableau		= 10		// beliebig, aber nur von Tableaus
};

// auf welche Piles darf von diesem Stapel gespielt werden (durch eine Aktion des Users)
// hat nichts mit dem geben zu tun
enum pilePermittedPlay {
	pppNotAvailable				= 0,		// nicht verfügbar/relevant
	pppTableau					= 1,		// auf Tableau
	pppWaste					= 2,		// auf Waste
	pppFoundation				= 3,		// auf Foundation
	pppTableauOrFoundation		= 4,		// auf Tableau or Foundation
	pppWaste_Tableau_Foundation	= 5			// auf Waste, Tableau or Foundation
};

// welcher Art ist die Basiskarte eines Foundations
enum pileBasicCard {
	basNotAvailable		= 0,		// nicht verfügbar/relevant
	basFirstPlayed		= 1,		// erste abgelegte Karte wird Basiskarte
	basFixed			= 2,		// vorgegeben, liegt aber nicht
	basFixedLie			= 3,		// vorgegeben, liegt zu Beginn
	basFromStock		= 4			// wird bei Start vom Stock gezogen
};


// die Anzahl der Lücken muß vorgegeben werden
// ???

// woher darf ein leerer Kartenstapel gefüllt werden
// diese Werte können verodert sein
#define	FROM_STOCK		0x0001
#define	FROM_WASTE		0x0002
#define	FROM_USER		0x0004
#define	FROM_FOUNDATION	0x0008

// speziell stock
// wohin wird vom Stock gespielt
enum pileDealing {
	dealNotAvailable		= 0,	// nicht verfügbar/relevant
	dealTableaus			= 1,	// auf Spielstapel
	dealWaste				= 2		// in den Waste	(dann ist die Anzahl interessant)
};

enum pileRedealMethod {
	redealNotAvailable		= 0,	// nicht verfügbar/relevant
	redealTableaus			= 1,	// Spielstapel einsammeln
	redealTableausMix		= 2,	// Spielstapel einsammeln unsd mischen
	redealWaste				= 3,	// Waste einsammeln
	redealWasteMix			= 4		// Waste einsammeln unsd mischen
};


class CPile : public CObject  
{
    DECLARE_SERIAL(CPile)

public:
	CPile();
	CPile(const CPile& p);							// Copy constructor
	virtual ~CPile();

public:
	// Members
	// Information, die serialisiert werden muß
	int						m_PileVersion;			// Version für Serialize
	CCardList				m_Cards;				// alle zum Stapel gehörenden Karten
	int						m_IDDibEmpty;			// ID der Bitmap
	pileOverlappedMethod	m_KindOfOverlapped;		// Art der Überlappung
	pileSelectingMethod		m_KindOfSelecting;		// wie dürfen Karten selektiert und bewegt werden
	pilePiling				m_KindOfPiling;			// wie darf abgelegt werden
	pileFaceAtStart			m_FaceAtStart;			// Aussehen bei Start
	int						m_NumberOnStart;		// Anzahl der Karten am Start
	int						m_NumberHiddenOnStart;	// Anzahl der verdeckten Karten am Start
	int						m_NumberPerMove;		// wieviele Karten dürfen abgelegt werden
	pilePutIfEmpty			m_PutIfEmpty;			// womit kann der leere Stapel belegt werden
	int						m_MaxCards;				// wieviele Karten darf der Pile maximal haben
													// -1 keine Beschränkung
	CSize					m_MaxSize;				// maximale Größe des Stapels
	int						m_DealOrder;			// in welcher Reihenfolge steht dieser Stapel bei Geben (Deal)
													// 0 = von diesem Stapel wird nicht gegeben
													// 1 = von diesem Stapel zuerst (wenn nicht leer)
	int						m_RedealOrder;			// in welcher Reihenfolge steht dieser Stapel bei Neu Geben (Redeal)
													// 0 = von diesem Stapel wird nicht genommen
													// 1 = von diesem Stapel zuerst
	pileDealing				m_DealTo;				// wohin wird gegeben
	int						m_NumberPerDeal;		// wieviele Karten werden auf jeden Zielstapel gegeben
	
	pilePermittedPlay		m_PermittedPlay;		// wohin darf von diesem Stapel gespielt werden
	
	BOOL					m_UpdateOverlap;		// soll die Überlappung immer auf MaxSize angepaßt werden?
	BOOL					m_SpecialHandling;		// wird für diesen Pile eine Spezialbehandlung durchgeführt?
	CString					m_SpecialHandlingText;	// Beschreibung der Spezialbehandlung

	// Information die nach der Serialisation aufgebaut werden kann
	CDib					m_dibEmpty;				// DIP-Map des leeren Piles
	CDib*					m_pdibEmpty;			// Zeiger auf die Dip-Map des leeren Piles
													// schon umgerechnet auf die auszugebende Größe und 
													// mit den ausgefüllten transparenten Punkten
	double					m_OverlappedFactor;

protected:
	void UpdateOverlappedFactor();
	// Information, die serialisiert werden muß
	CPoint					m_Position;				// Position des Kartenstapels
	CSize					m_Size;					// sichtbares Rechteck des Kartenstapels

public:
// Operatoren
    const CPile& operator =(const CPile& p);
    const CPile& operator =(const CPile* p);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual double ComputeOverlappedFactor(int NumberOfCards);
	virtual BOOL HasChanged(CPile* pCompPile);
	virtual void Revert();
	virtual CCard* SelectSequence(CCard* pCard,BOOL& RetWithErr,
									CPile* pFirstSelectedPile = NULL, 
									CCard *pFirstSelectedCard = NULL);
	virtual CCard* GetClickedCard(CPoint point, BOOL& RetWithErr);
	virtual BOOL HitTestSelectedCards(CPoint point);
	virtual BOOL HitTest(CPile* pPile);
	virtual CCard* GetLastCard(CCard *pCard);
	virtual CCard* GetFirstCard(CCard *pCard);
	virtual CCard* GetLastSequence();
	virtual CCard* GetLastSequenceInSuit(CCard* pCard= NULL);
	virtual CCard* GetLastSequenceAlternate(CCard* pCard= NULL);
	virtual BOOL IsLastCard(CCard* pCard);
	virtual BOOL AreCardsDownInSequence(CCard* pCard);
	virtual BOOL AreCardsUpInSequence(CCard* pCard);
	virtual BOOL AreCardsDownInSequenceRolling(CCard* pCard);
	virtual BOOL AreCardsUpInSequenceRolling(CCard* pCard);
	virtual BOOL IsDownInSequenceAndSuit();
	virtual BOOL IsUpInSequenceAndSuitRolling();
	virtual BOOL IsDownInSequenceAndSuitRolling();
	virtual BOOL IsUpInSequenceRolling();
	virtual BOOL IsDownInSequenceRolling();
	virtual BOOL IsUpInSequenceAndSuit();
	virtual BOOL IsDownInSequenceAndAlternate();
	virtual BOOL IsUpInSequenceAndAlternate();
	virtual BOOL IsDownInSequence();
	virtual BOOL IsUpInSequence();
	virtual BOOL IsUpInSequenceSameColor(int BasicCardValue);
	virtual BOOL HaveCardsSameColor(CCard *pCard,int BasicCardValue);
	virtual BOOL IsUpInSequenceValueDiff2InSuit(int BasicCardValue);
	virtual BOOL HaveCardsSameSuit(CCard *pCard,int BasicCardValue);
	virtual BOOL HaveCardsValueDiff2(CCard *pCard);
	virtual CRect GetRect();
	virtual CSize GetSize();
	virtual CSize GetMaxSize(){return m_MaxSize;}
	virtual void SetMaxSize(CSize MaxSize){m_MaxSize = MaxSize;}
	virtual int GetCount() {return m_Cards.GetCount();}
	virtual CPoint SetPosition(CPoint Position);
	virtual CPoint GetPosition();
	virtual BOOL IsEmpty();
	virtual BOOL IsFirstVisibleCard(CCard* pCard);
	virtual BOOL IsLastInvisibleCard(CCard* pCard);
	virtual BOOL AreCardsInSuit(CCard* pCard);
	virtual BOOL AreCardsAlternate(CCard* pCard);
	virtual BOOL SelectCards(CCard* pCard, BOOL UnSelect = FALSE);
	virtual void UpdateFaceAtStart();
	virtual void MixCards();
	virtual void UpdateCardsPositions();
	virtual void Display(CDC* pDC, CDibs& Dibs, BOOL WithMaxSize = FALSE, BOOL Transparent = TRUE);
	virtual CCard* GetCard(CPoint point);
	virtual BOOL HitTest( CPoint point);
	virtual void CopyBackgroundToEmptyAppearence(CDC *pDC, CDibs& Dibs);
	virtual void Get(CPile* pSrcPile, CCard* pCard, CRect& UpdateRect);
	virtual void Put(CPile* pDstPile, CCard* pCard, CRect& UpdateRect);
	virtual void Move(CPoint FromPoint, CPoint ToPoint, CRect& UpdateRect);
	virtual void Serialize(CArchive& ar);

	// Report Funktionen
	void ReportSerializeVersion(void);
	void ReportEmptyBitmapID(void);
	void ReportPilePosition(void);
	void ReportNumberOnStart(void);
	void ReportNumberHiddenOnStart(void);
	void ReportNumberPerMove(void);
	void ReportNumberOfMaxCards(void);
	void ReportMaxSize(void);
	void ReportKindOfOverlapped(void);
	void ReportKindOfSelecting(void);
	void ReportKindOfPiling(void);
	void ReportFaceAtStart(void);
	void ReportPutIfEmpty(void);
	void ReportDealOrder(void);
	void ReportRedealOrder(void);
	void ReportDealing(void);
	void ReportNumberPerDeal(void);
	void ReportPermittedPlay(void);
	void ReportUpdateOverlap(void);
	void ReportSpecialHandling(void);
	void ReportProperties();

	// liefert die Karte, falls im Pile vorhanden
	CCard* GetCard(CString& CardName);
	CCard* GetCard(int CardValue);

};


class CStock : public CPile  
{
    DECLARE_SERIAL(CStock)
public:
	CStock();
	CStock(const CStock& p);							// Copy constructor
	virtual ~CStock();

public:
	int						m_StockVersion;			// Version für Serialize
	int						m_NumberOfRedeal;		// Anzahl wie oft neu gegeben werden kann
	pileRedealMethod		m_RedealMethod;			// woher und wie kommen die Karten beim neu geben

public:
// Operatoren
    const CStock& operator =(const CStock& p);
    const CStock& operator =(const CStock* p);

public:
	virtual void Serialize(CArchive& ar);
	// Report Funktionen
	void ReportSerializeVersion(void);
	void ReportRedealing(void);
	void ReportNumberOfRedeal(void);
	virtual void ReportProperties();
};


class CFoundation : public CPile  
{
    DECLARE_SERIAL(CFoundation)
public:
	CFoundation();
	CFoundation(const CFoundation& p);							// Copy constructor
	virtual ~CFoundation();

public:
	int						m_FoundationVersion;	// Version für Serialize
	BOOL					m_Replayable;			// können Karten zurückgespielt werden
	pileBasicCard			m_KindOfBasicCard;		// Auswahl der Basiskarte
	int						m_BasicCardValue;		// Basiskartenwert
													//  0 = leer
													//  1 = Kreuz As 
													// 14 = Karo As  
													// 27 = Herz As  
													// 40 = Pik As   
													// 53 = Pik König
	BOOL					m_BasicCardOnlyVal;		// TRUE: nur Kartenwert nicht die Farbe
	BOOL					m_BasicCardOnlyColor;	// TRUE: nur die Farbe zählt (und der Kartenwert)

public:
// Operatoren
    const CFoundation& operator =(const CFoundation& p);
    const CFoundation& operator =(const CFoundation* p);

public:
	virtual void Serialize(CArchive& ar);
	// Report Funktionen
	void ReportSerializeVersion(void);
	void ReportReplayable(void);
	void ReportKindOfBasicCard(void);
	void ReportBasicCardValue(void);
	void ReportBasicCardOnlyVal(void);
	void ReportBasicCardOnlyColor(void);
	virtual void ReportProperties();
	
};


class CTableau : public CPile  
{
    DECLARE_SERIAL(CTableau)
public:
	CTableau();
	CTableau(const CTableau& p);							// Copy constructor
	virtual ~CTableau();

public:
	int						m_TableauVersion;		// Version für Serialize

public:
// Operatoren
    const CTableau& operator =(const CTableau& p);
    const CTableau& operator =(const CTableau* p);

public:
	virtual void Serialize(CArchive& ar);
	// Report Funktionen
	void ReportSerializeVersion(void);
	virtual void ReportProperties();
};

class CWaste : public CPile  
{
    DECLARE_SERIAL(CWaste)
public:
	CWaste();
	CWaste(const CWaste& p);							// Copy constructor
	virtual ~CWaste();

public:
	int						m_WasteVersion;			// Version für Serialize

public:
// Operatoren
    const CWaste& operator =(const CWaste& p);
    const CWaste& operator =(const CWaste* p);

public:
	virtual void Serialize(CArchive& ar);
	// Report Funktionen
	void ReportSerializeVersion(void);
	virtual void ReportProperties();
};


class CMovePile : public CPile  
{
    DECLARE_SERIAL(CMovePile)
public:
	CMovePile();
	CMovePile(const CMovePile& p);							// Copy constructor
	virtual ~CMovePile();

public:
	int						m_MovePileVersion;		// Version für Serialize

public:
// Operatoren
    const CMovePile& operator =(const CMovePile& p);
    const CMovePile& operator =(const CMovePile* p);

public:
	virtual void Serialize(CArchive& ar);
};

class CPileList : public CTypedPtrList<CObList, CPile*>
{


public:
// Operatoren
    const CPileList& operator =(const CPileList& s) {
		RemoveAll();
		POSITION pos = s.GetHeadPosition();
		while (pos)
			AddTail(s.GetNext(pos));
        return *this;
    }

    const CPileList& operator =(const CPileList* s) {
		RemoveAll();
		POSITION pos = s->GetHeadPosition();
		while (pos)
			AddTail(s->GetNext(pos));
        return *this;
    }
};

class CPileArray : public CTypedPtrArray<CObArray, CPile*>
{


public:
// Operatoren
    const CPileArray& operator =(const CPileArray& s) {
		RemoveAll();
		if (s.GetSize() != 0)
			for (int i = 0; i < s.GetSize(); i++)
				Add(s.GetAt(i));

        return *this;
    }

    const CPileArray& operator =(const CPileArray* s) {
		RemoveAll();
		if (s->GetSize() != 0)
			for (int i = 0; i < s->GetSize(); i++)
				Add(s->GetAt(i));

        return *this;
    }
};

#endif // !defined(PILES_H_INCLUDED_)
