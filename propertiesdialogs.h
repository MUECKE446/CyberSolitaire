#if !defined(AFX_PROPERTIESDIALOGS_H__3274EF73_B36D_11D6_97B6_00A0CC562BEB__INCLUDED_)
#define AFX_PROPERTIESDIALOGS_H__3274EF73_B36D_11D6_97B6_00A0CC562BEB__INCLUDED_

#include "SolEditDoc.h"	// Hinzugefügt von der Klassenansicht
#include "afxwin.h"
//#include "atltypes.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesDialogs.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CGamePropertiesDialog 

class CGamePropertiesDialog : public CDialog
{
// Konstruktion
public:
	CSolEditDoc* m_pDoc;
	CGamePropertiesDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CGamePropertiesDialog)
	enum { IDD = IDD_GAME_PROPERTIES };
	CComboBox	m_TypeOfScoringCombo	;
	CComboBox	m_GroupTypeCombo		;
	CString		m_GroupType				;
	CString		m_GameName				;
	int			m_NumberOfDecks			;
	CString		m_strTypeOfScoring		;
	//}}AFX_DATA

protected:
	void FillGroupTypeCombo(int FirstID);
	void FillTypeOfScoringCombo(int FirstID);
// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CGamePropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CGamePropertiesDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// Gewinnpunkte
	int m_MaxPoints;
};

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPilePropertiesDialog 

class CPilePropertiesDialog : public CDialog
{
// Konstruktion
public:
	CBitmap m_BmpEmptyPile;
	int m_IDDibEmpty;
	CBitmap m_BmpBasicCard;
	int m_IDDibBasicCard;
	CPile* m_pPile;
	CSolEditDoc* m_pDoc;
	void FillFaceAtStartCombo(int FirstID);
	void FillOverlappMethodCombo(int FirstID);
	void FillPilingMethodCombo(int FirstID);
	void FillPutIfEmptyCombo(int FirstID);
	void FillSelectMethodCombo(int FirstID);
	void FillDealToCombo(int FirstID);
	void FillBasicCardCombo(int FirstID);
	void FillRedealMethodCombo(int FirstID);
	void FillPermittedPlayCombo(int FirstID);

	CPoint m_PilePosition;
	CPilePropertiesDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CPilePropertiesDialog)
	enum { IDD = IDD_PILE_PROPERTIES };
	CComboBox	m_BasicCardCombo;
	CComboBox	m_RedealMethodCombo;
	CSpinButtonCtrl	m_SpinBasicCardCtrl;
	CStatic	m_BmpBasicCardCtrl;
	CComboBox	m_DealToCombo;
	CComboBox	m_SelectMethodCombo;
	CComboBox	m_PutIfEmptyCombo;
	CComboBox	m_PilingMethodCombo;
	CComboBox	m_OverlappMethodCombo;
	CComboBox	m_FaceAtStartCombo;
	CComboBox	m_PermittedPlayCombo;
	CStatic	m_BmpEmptyPileCtrl;
	CSpinButtonCtrl	m_SpinEmptyCtrl;
	CString	m_xPosition;
	CString	m_yPosition;
	CString	m_FaceAtStart;
	CString	m_OverlappMethod;
	CString	m_PilingMethod;
	CString	m_PutIfEmpty;
	CString	m_SelectMethod;
	CString	m_RedealMethod;
	CString m_BasicCardMethod;
	int		m_MaxCards;
	int		m_NumberOnStart;
	int		m_NumberPerMove;
	CString	m_DealTo;
	int		m_NumberPerDeal;
	BOOL	m_Replayable;
	BOOL	m_BasicCardOnlyVal;
	UINT	m_DealOrder;
	UINT	m_RedealOrder;
	int		m_NumberOfRedeal;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPilePropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPilePropertiesDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinEmptyPile(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDeltaposSpinBasiccard(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// die Eigenschaften werden für alle Piles dieses Typs übernommen
	BOOL m_UseForAllPilesOfThisType;
	// Anzahl der verdeckten Karten bei Start
	int m_NumberHiddenOnStart;

	CString m_PermittedPlay;
	CString m_cx_MaxSize;
	CString m_cy_MaxSize;
	CSize m_PileMaxSize;
	// Beschreibung der Spezialbehandlung
	CString m_SpecialHandlingText;
	// wird für diesen Pile eine Spezialbehandlung durchgeführt?
	BOOL m_SpecialHandling;
	// Überlappung aktualisieren
	BOOL m_UpdateOverlap;
	// nur die Farbe zählt
	BOOL m_BasicCardOnlyColor;
	afx_msg void OnBnClickedCheckBasiccardOnlyColor();
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROPERTIESDIALOGS_H__3274EF73_B36D_11D6_97B6_00A0CC562BEB__INCLUDED_
