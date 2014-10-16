#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CBackgroundDialog-Dialogfeld

class CBackgroundDialog : public CDialog
{
	DECLARE_DYNAMIC(CBackgroundDialog)

public:
	CBackgroundDialog(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CBackgroundDialog();

// Dialogfelddaten
	enum { IDD = IDD_CARDS_AND_BACKGROUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposCardsBackground(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposGameBackground(NMHDR *pNMHDR, LRESULT *pResult);
	// Bild für Kartenrückseite
	CBitmap m_BmpCardBackground;
	CStatic m_BmpCardBackgroundCtrl;
	int		m_IDCardBackground;
	CSpinButtonCtrl m_SpinCardBackgroundCtrl;
	// Spielhintergrund
	CBitmap m_BmpGameBackground;
	CStatic m_BmpGameBackgroundCtrl;
	int		m_IDGameBackground;
	CSpinButtonCtrl m_SpinGameBackgroundCtrl;
	virtual BOOL OnInitDialog();
};
