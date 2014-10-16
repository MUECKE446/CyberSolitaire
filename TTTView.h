#if !defined(AFX_TTTVIEW_H__5F1F52E4_0825_11D7_97ED_00A0CC562BEB__INCLUDED_)
#define AFX_TTTVIEW_H__5F1F52E4_0825_11D7_97ED_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTTView.h : Header-Datei
//


/////////////////////////////////////////////////////////////////////////////
// Ansicht CTTTView 

class	CTTTDoc;

class CTTTView : public CView
{
protected:
	CTTTView();           // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CTTTView)

// Attribute
public:
	CTTTDoc* GetDocument();

	CDib	m_BackGround;
	CDib	m_Screen;

	CDib*	m_pScreen;		// Bitmap, die gezeichnet wird
	CDib*	m_pBackground;	// Hintergrund Bitmap zum Löschen

	CDibs	m_Dibs;
	CRect	m_UpdateRect;
	CRect	m_DeviceRect;
	CRect	m_VisibleRect;



// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CTTTView)
	public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // Überschrieben zum Zeichnen dieser Ansicht
	//}}AFX_VIRTUAL

// Implementierung
public:
	void SetDibs(CDib* pCardBackDib);

protected:
	virtual ~CTTTView();

	void Delete(CDib* pDib);
	CPoint	m_LastPoint;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CTTTView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
	afx_msg LRESULT OnTTTGameFinished (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ein Merker für den letzten gefundenen Level beim MouseMove
	int m_OldFoundLevel;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG
inline CTTTDoc* CTTTView::GetDocument()
   { return (CTTTDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_TTTVIEW_H__5F1F52E4_0825_11D7_97ED_00A0CC562BEB__INCLUDED_
