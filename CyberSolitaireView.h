// CyberSolitaireView.h : Schnittstelle der Klasse CCyberSolitaireView
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYBERSOLITAIREVIEW_H__853AAB60_2098_4081_A14B_9C1E972E2A49__INCLUDED_)
#define AFX_CYBERSOLITAIREVIEW_H__853AAB60_2098_4081_A14B_9C1E972E2A49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CardToolTip.h"

// Macros

#define RETURN_WITH_ERROR	{ ReportUserError(); return; }

class	CCyberSolitaireDoc;

class CCyberSolitaireView : public CView
{

protected: // Nur aus Serialisierung erzeugen
	CCyberSolitaireView();
	DECLARE_DYNCREATE(CCyberSolitaireView)

// Attribute
public:
	CCyberSolitaireDoc* GetDocument();

	CDib	m_BackGround;
	CDib	m_Screen;
	CDib	m_dibCardSelected;	// Karte selektiert

	CDib*	m_pScreen;		// Bitmap, die gezeichnet wird
	CDib*	m_pBackground;	// Hintergrund Bitmap zum Löschen

	CRect	m_DeviceRect;
	CRect	m_VisibleRect;

	CPile*	m_pSelectedPile;
	CPile*	m_pSecondSelectedPile;
	
	CDibs	m_Dibs;

	CCardToolTip*	m_pCardToolTip;
	
	BOOL	m_ShowCardCheated;
	CRect	m_UpdateRect;
	CPile*	m_pShowPile;
	CCard*	m_pClickedCard;
	UINT	m_Timer;

	CMenu	m_TTTGameMenu;
	CMenu	m_GameMenu;

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CCyberSolitaireView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementierung
public:
	BOOL PutToTableauOrWaste(CPile* pPile, BOOL& RetWithErr);
	BOOL PutToFoundation(CPile* pPile, BOOL& RetWithErr);
	void DealCards(CPile* pSrcPile, BOOL Redeal);
	BOOL SelectCards(CPile* pPile, CPoint LogPoint, BOOL& RetWithErr);
	BOOL TestPutToFoundation(CPile* pPile);
	void SetDibs(CDib* pCardBackDib);
	BOOL IsMainWndSizing();
	BOOL UpdateBitmaps();
	CPile* HitPileTest(CPoint LogPoint);
	void PutSelectedPileToMovePile(CPoint LogPoint);
	void PutBothSelectedPilesToMovePile();
	void ClipCursorToView(CDC* pDC, BOOL Release = FALSE);
	void PutMovePileToPile(CPile* pPile);
	void PutMovePileBack();
	void PutMovePilePairToPile(CPile* pPile);
	void PutMovePilePairBack();
	void ReportUserError();
	virtual ~CCyberSolitaireView();

	// Tooltip management
    BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL ResetToolTip();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	void Delete(CDib* pDib);
	
	CPoint	m_LastPoint;
	CCard*	m_pSelectedCard;				// selektierte Karte (bei einer Sequenz: die erste selektierte)
	CCard*	m_pSecondSelectedCard;				// selektierte Karte (bei einer Sequenz: die erste selektierte)
	BOOL	m_CardsMoving;
	
	//{{AFX_MSG(CCyberSolitaireView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnShowPile();
	afx_msg void OnShowPileCheatmode();
	afx_msg void OnShowCardCheat();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg BOOL OnToolTip(UINT id, NMHDR *pTTT, LRESULT *pResult);
	afx_msg BOOL OnToolTipPop(UINT id, NMHDR *pTTT, LRESULT *pResult);
	afx_msg BOOL OnToolTipShow(UINT id, NMHDR *pTTT, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
public:
	void ReportGameStatus();
	void ReportGameDifferenceStatus();
	void UpdateScreenAndBackground();
	// Testet den Stock ob noch mal gegeben werden kann
	BOOL TestRedeal();
	void DoRedeal();
	void PutPileToStock(CPile* pPile);

	afx_msg void OnChangeCardFaceUp();
	afx_msg void OnAutoplay();
	afx_msg void OnUpdateAutoplay(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // Testversion in CyberSolitaireView.cpp
inline CCyberSolitaireDoc* CCyberSolitaireView::GetDocument()
   { return (CCyberSolitaireDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CYBERSOLITAIREVIEW_H__853AAB60_2098_4081_A14B_9C1E972E2A49__INCLUDED_)
