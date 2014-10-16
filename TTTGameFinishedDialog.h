#pragma once
#include "afxwin.h"


// CTTTGameFinishedDialog-Dialogfeld

class CTTTGameFinishedDialog : public CDialog
{
	DECLARE_DYNAMIC(CTTTGameFinishedDialog)

public:
	CTTTGameFinishedDialog(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CTTTGameFinishedDialog();

// Dialogfelddaten
	enum { IDD = IDD_TTT_GAME_FINISHED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	// Name des Spiels
	CString m_GameName;
	// aktuell erreichte Punktzahl
	int m_ActualScore;
	// benötigte Punktzahl
	int m_NeededScore;
	// Ergebnis
	CString m_Result;

	CCyberSolitaireDoc*	m_pDoc;
	virtual BOOL OnInitDialog();
};
