#if !defined(AFX_TTTFRAME_H__5F1F52E5_0825_11D7_97ED_00A0CC562BEB__INCLUDED_)
#define AFX_TTTFRAME_H__5F1F52E5_0825_11D7_97ED_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTTFrame.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Rahmen CTTTFrame 

class CTTTFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CTTTFrame)
protected:
	CTTTFrame();           // Dynamische Erstellung verwendet gesch�tzten Konstruktor

// Attribute
public:
	CStatusBar		m_pStatusBar;

// Operationen
public:

// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CTTTFrame)
	public:
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CTTTFrame();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CTTTFrame)
		// HINWEIS - Der Klassen-Assistent f�gt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_TTTFRAME_H__5F1F52E5_0825_11D7_97ED_00A0CC562BEB__INCLUDED_
