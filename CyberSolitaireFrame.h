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
	CCyberSolitaireFrame();           // Dynamische Erstellung verwendet gesch�tzten Konstruktor

// Attribute
public:

// Operationen
public:

// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
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
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_CYBERSOLITAIREFRAME_H__35CAF4FD_F215_4D22_B7C2_12B780C5DB74__INCLUDED_
