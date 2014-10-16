#if !defined(AFX_GAMETREE_H__47972053_3B08_11D6_975D_00A0CC562BEB__INCLUDED_)
#define AFX_GAMETREE_H__47972053_3B08_11D6_975D_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameTree.h : Header-Datei
//

#define ROOT		0
#define ALL_GAMES	1
#define GAMES		2


/////////////////////////////////////////////////////////////////////////////
// Fenster CGameTree 

class CGameTree : public CTreeCtrl
{
// Konstruktion
public:
	CGameTree();

// Attribute
public:

// Operationen
public:
	void ResizeDialog( UINT nType, int cx, int cy);

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CGameTree)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementierung
public:
	CString m_SelectedGame;
	void InitGameTree();
	virtual ~CGameTree();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CGameTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_GAMETREE_H__47972053_3B08_11D6_975D_00A0CC562BEB__INCLUDED_
