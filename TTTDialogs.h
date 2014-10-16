#pragma once
#include "afxwin.h"


// CTTTChooseGameDialog-Dialogfeld

class CTTTChooseGameDialog : public CDialog
{
	DECLARE_DYNAMIC(CTTTChooseGameDialog)

public:
	CTTTChooseGameDialog(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CTTTChooseGameDialog();

// Dialogfelddaten
	enum { IDD = IDD_TTT_CHOOSE_GAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioGame1();
	afx_msg void OnBnClickedRadioGame2();
	afx_msg void OnBnClickedRadioGame3();
	CStatic m_NeededScore1;
	CStatic m_NeededScore2;
	CStatic m_NeededScore3;
	virtual BOOL OnInitDialog();
	int m_ChoosedGame;
	CString m_Game1;
	CString m_Game2;
	CString m_Game3;
	CButton* m_pRadioButton;
	afx_msg void OnBnClickedDescription();
	afx_msg void OnBnClickedDescriptionShort();
	CString m_ChoosedGameName;
	afx_msg void OnBnClickedOk();
	CString m_strNeededScore1;
	CString m_strNeededScore2;
	CString m_strNeededScore3;
};
