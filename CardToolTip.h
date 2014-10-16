#if !defined(AFX_CARDTOOLTIP_H__86865826_0046_11D7_97E5_00A0CC562BEB__INCLUDED_)
#define AFX_CARDTOOLTIP_H__86865826_0046_11D7_97E5_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CardToolTip.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CCardToolTip 

class CCardToolTip : public CToolTipCtrl
{
// Konstruktion
public:
	CCardToolTip();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CCardToolTip)
	//}}AFX_VIRTUAL

// Implementierung
public:
	CCardToolTip(CWnd* pWnd, CDC* pDC, CPile* pPile,CCard* pCard);
	BOOL m_bShowing;
	CCard* m_pCard;
	CPile* m_pPile;
	CWnd*  m_pWnd;
	CString	m_ToolTipText;
	virtual ~CCardToolTip();

	BOOL GetShowing() {return m_bShowing;}
	void SetShowing(BOOL bShowing) {m_bShowing = bShowing;}
	void Show( BOOL bShow);
	CString GetText(){ return m_ToolTipText;}


	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CCardToolTip)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CARDTOOLTIP_H__86865826_0046_11D7_97E5_00A0CC562BEB__INCLUDED_
