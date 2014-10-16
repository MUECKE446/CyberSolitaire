// SolEditView.h : Schnittstelle der Klasse CSolEditView
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLEDITVIEW_H__006D9397_EF8C_4397_BD53_310424C11260__INCLUDED_)
#define AFX_SOLEDITVIEW_H__006D9397_EF8C_4397_BD53_310424C11260__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RETURN_WITH_ERROR	{ ReportUserError(); return; }


class CSolEditView : public CView
{
protected: // Nur aus Serialisierung erzeugen
	CSolEditView();
	DECLARE_DYNCREATE(CSolEditView)

// Attribute
public:
	CSolEditDoc* GetDocument();

	CDib	m_BackGround;
	CDib	m_Screen;
	CDib	m_dibCardSelected;	// Karte selektiert

	CDib*	m_pScreen;		// Bitmap, die gezeichnet wird
	CDib*	m_pBackground;	// Hintergrund Bitmap zum Löschen

	CRect	m_DeviceRect;
	CRect	m_VisibleRect;

	CPile*	m_pSelectedPile;
	CRect	m_UpdateRect;

	CDibs	m_Dibs;


// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CSolEditView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementierung
public:
	void SetDibs(CDib* pCardBackDib);
	void PositionToStatusBar(CPoint LPPoint);
	CPile* m_pContextPile;
	CPoint m_ContextPoint;
	void Delete(CDib *pDib);
	BOOL IsMainWndSizing();
	BOOL UpdateBitmaps();
	CPile* m_pShowPile;
	CPile* HitPileTest(CPoint LogPoint);
	void ClipCursorToView(CDC* pDC, BOOL Release = FALSE);
	void ReportUserError();
	void SetPointToGrid(CPoint* pPoint);
	void DrawGrid(CDC* pDC, BOOL Lines = TRUE);
	virtual ~CSolEditView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPoint m_LastPoint;
	CCard* m_pSelectedCard;
	BOOL m_PileMoving;

// Generierte Message-Map-Funktionen
protected:
	BOOL m_OldUseForAllPilesOfThisType;
	CPoint m_Position;
	//{{AFX_MSG(CSolEditView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInsertStock();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSoleditpilecontextDelete();
	afx_msg void OnUpdateSoleditpilecontextDelete(CCmdUI* pCmdUI);
	afx_msg void OnSoleditpilecontextProperties();
	afx_msg void OnUpdateSoleditpilecontextProperties(CCmdUI* pCmdUI);
	afx_msg void OnSoleditgamecontextProperties();
	afx_msg void OnUpdateSoleditgamecontextProperties(CCmdUI* pCmdUI);
	afx_msg void OnInsertFoundation();
	afx_msg void OnUpdateInsertPile(CCmdUI* pCmdUI);
	afx_msg void OnInsertTableau();
	afx_msg void OnInsertWaste();
	afx_msg void OnSoleditpilecontextCopy();
	afx_msg void OnSoleditpilecontextMaxrectmaximal();
	afx_msg void OnSoleditpilecontextMaxrectinkrementieren();
	afx_msg void OnSoleditpilecontextMaxrectdekrementieren();
	afx_msg void OnDateiSpeichern();
	//}}AFX_MSG
	afx_msg void OnUpdatePanePosition(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSoleditpilecontextMaxrectloeschen();
	afx_msg void OnTTTLevelpunkte();
};

#ifndef _DEBUG  // Testversion in SolEditView.cpp
inline CSolEditDoc* CSolEditView::GetDocument()
   { return (CSolEditDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_SOLEDITVIEW_H__006D9397_EF8C_4397_BD53_310424C11260__INCLUDED_)
