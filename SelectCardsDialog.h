#if !defined(AFX_SELECTCARDSDIALOG_H__62AD01D3_B8B8_11D6_97BB_00A0CC562BEB__INCLUDED_)
#define AFX_SELECTCARDSDIALOG_H__62AD01D3_B8B8_11D6_97BB_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectCardsDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSelectCardsDialog 

class CSelectCardsDialog : public CDialog
{
// Konstruktion
public:
	CSelectCardsDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSelectCardsDialog)
	enum { IDD = IDD_SELECT_CARDS };
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSelectCardsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSelectCardsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnCard001();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SELECTCARDSDIALOG_H__62AD01D3_B8B8_11D6_97BB_00A0CC562BEB__INCLUDED_
