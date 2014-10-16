#if !defined(AFX_PLAYERDIALOGS_H__C274EBB5_38E8_11D7_9813_00A0CC562BEB__INCLUDED_)
#define AFX_PLAYERDIALOGS_H__C274EBB5_38E8_11D7_9813_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerDialogs.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSelectPlayerDialog 

class CSelectPlayerDialog : public CDialog
{
// Konstruktion
public:
	CSelectDoc* m_pSelectDoc;
	CFont* m_pNewFont;
	CSelectPlayerDialog(CWnd* pParent = NULL, int GreetingTextID = IDS_EMPTY_GREETING_TEXT, int OkButtonTextId = IDS_OK_TEXT);   // Standardkonstruktor

	CString	m_SelName;
	int		m_ArrIndex;
	BOOL FillPlayerList(BOOL WithTitleBar);

// Dialogfelddaten
	//{{AFX_DATA(CSelectPlayerDialog)
	enum { IDD = IDD_SELECT_PLAYER };
	CListCtrl	m_PlayerList;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSelectPlayerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	int			m_GreetingTextID;
	int			m_OkButtonTextId;

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSelectPlayerDialog)
	afx_msg void OnItemchangedPlayerList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclickPlayerList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDeletePlayer();
	afx_msg void OnInsertNewPlayer();
	afx_msg void OnOkDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertNewPlayerDialog 

class CInsertNewPlayerDialog : public CDialog
{
// Konstruktion
public:
	CInsertNewPlayerDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CInsertNewPlayerDialog)
	enum { IDD = IDD_INSERT_NEW_PLAYER };
	CString	m_NewPlayerNickName;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CInsertNewPlayerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CInsertNewPlayerDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PLAYERDIALOGS_H__C274EBB5_38E8_11D7_9813_00A0CC562BEB__INCLUDED_
