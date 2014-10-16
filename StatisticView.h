#if !defined(AFX_STATISTICVIEW_H__F2638DC7_4801_11D7_9822_00A0CC562BEB__INCLUDED_)
#define AFX_STATISTICVIEW_H__F2638DC7_4801_11D7_9822_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticView.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Formularansicht CStatisticView 

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// constants
//

#define OFFSET_X	20
#define OFFSET_Y	20

const CPoint	DspWndOrg(50,50);
const CRect		DspWndRect(0,0,550,354);


#define	CResultArray CArray<int,int>

class CDispWnd;

#pragma once

enum DRAW_MODE {DRAW_POINT,DRAW_LINE,DRAW_BAR};

class CStatisticView : public CFormView
{
protected:
	CStatisticView();           // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CStatisticView)

// Formulardaten
public:
	//{{AFX_DATA(CStatisticView)
	enum { IDD = IDD_SHOW_STATISTIC };
	CScrollBar	m_ScrollBarResults;
	CListCtrl	m_PlayerList;
	int		m_ScrollBarValue;
	//}}AFX_DATA

// Attribute
public:
	CResultArray		m_Results;
	CString				m_PlayerName;

// Operationen
public:
	BOOL FillPlayerList(BOOL WithTitleBar);

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CStatisticView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CStatisticView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CStatisticView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnItemchangedPlayerListbox(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL BuildResults(CString& PlayerName, CResultArray& ResultArr);
	BOOL RemoveResults(CString& PlayerName);
	afx_msg void OnLvnItemchangedPlayerListbox(NMHDR *pNMHDR, LRESULT *pResult);
	CDispWnd*	m_pDispWnd;
	afx_msg void OnDarstellungPunkte();
	afx_msg void OnDarstellungLinien();
	afx_msg void OnDarstellungBalken();
	// Darstellungsmodus der Ergebnisse
	DRAW_MODE m_DrawMode;
	afx_msg void OnUpdateDarstellungPunkte(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDarstellungLinien(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDarstellungBalken(CCmdUI *pCmdUI);
	afx_msg void OnStatistikLoeschenActPlayer();
	afx_msg void OnStatistikLoeschenAllPlayer();
};

// CDispWnd

class CDispWnd : public CWnd
{
	DECLARE_DYNAMIC(CDispWnd)

public:
	CDispWnd();
	CDispWnd(CStatisticView* pView, CResultArray& ResultArr);
	virtual ~CDispWnd();

	CResultArray*			m_pResultArr;
	CStatisticView*			m_pView;



	static LOGFONT		SmallLogFont;

protected:
	DECLARE_MESSAGE_MAP()
public:
	// Resultate zeichnen
	void DrawResults(CDC& dc, int StartValue, int MaxValue);

	// Achsen und Beschriftungen zeichnen
	void DrawAxes(CDC& dc, int StartValue, int MaxValue);
	CFont* m_pFont;
	afx_msg void OnPaint();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_STATISTICVIEW_H__F2638DC7_4801_11D7_9822_00A0CC562BEB__INCLUDED_
