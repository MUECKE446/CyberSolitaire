#if !defined(AFX_SHOWPILEDIALOG_H__3C2D0230_68F2_4919_A4B4_69673B6B1FEF__INCLUDED_)
#define AFX_SHOWPILEDIALOG_H__3C2D0230_68F2_4919_A4B4_69673B6B1FEF__INCLUDED_

#include "Piles.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowPileDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CShowPileDialog 

class CShowPileDialog : public CDialog
{
// Konstruktion
public:
	virtual ~CShowPileDialog();

public:
	CPileArray m_pPartPiles;
	CWnd* m_pParent;
	BOOL m_CheatMode;
	CShowPileDialog(CWnd* pParent = NULL);   // Standardkonstruktor

	CPile*		m_pShowPile;

// Dialogfelddaten
	//{{AFX_DATA(CShowPileDialog)
	enum { IDD = IDD_SHOW_PILE };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CShowPileDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CShowPileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SHOWPILEDIALOG_H__3C2D0230_68F2_4919_A4B4_69673B6B1FEF__INCLUDED_
