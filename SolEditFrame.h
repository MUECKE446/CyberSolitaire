#if !defined(AFX_SOLEDIT_H__INCLUDED_)
#define AFX_SOLEDIT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CyberSolitaireFrame.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Rahmen CSolEditFrame 

class CSolEditFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSolEditFrame)
protected:
	CSolEditFrame();           // Dynamische Erstellung verwendet gesch�tzten Konstruktor

// Attribute
public:

// Operationen
public:

// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CSolEditFrame)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CSolEditFrame();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSolEditFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_SOLEDIT_H__INCLUDED_
