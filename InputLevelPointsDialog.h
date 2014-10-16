#pragma once


// CInputLevelPointsDialog-Dialogfeld

class CInputLevelPointsDialog : public CDialog
{
	DECLARE_DYNAMIC(CInputLevelPointsDialog)

public:
	CInputLevelPointsDialog(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CInputLevelPointsDialog();

// Dialogfelddaten
	enum { IDD = IDD_EDIT_TTT_LEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	// Anzahl der zu erreichenden Punkte im Level 1 von TTT
	UINT m_Level1_Points;
	UINT m_Level2_Points;
	UINT m_Level3_Points;
	UINT m_Level4_Points;
	UINT m_Level5_Points;
	UINT m_Level6_Points;
	UINT m_Level7_Points;
	UINT m_Level8_Points;
	UINT m_Level9_Points;
	afx_msg void OnBnClickedOk();
};
