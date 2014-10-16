#if !defined(AFX_CYBERSOLITAIREFRAME_H__35CAF4FD_F215_4D22_B7C2_12B780C5DB74__INCLUDED_)
#define AFX_CYBERSOLITAIREFRAME_H__35CAF4FD_F215_4D22_B7C2_12B780C5DB74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CyberSolitaireFrame.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Rahmen CCyberSolitaireFrame 

class CCyberSolitaireFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCyberSolitaireFrame)
protected:
	CCyberSolitaireFrame();           // Dynamische Erstellung verwendet geschützten Konstruktor

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CCyberSolitaireFrame)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CCyberSolitaireFrame();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CCyberSolitaireFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CYBERSOLITAIREFRAME_H__35CAF4FD_F215_4D22_B7C2_12B780C5DB74__INCLUDED_
