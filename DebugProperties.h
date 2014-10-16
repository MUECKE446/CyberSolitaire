#pragma once


// CDebugPropertiesDialog-Dialogfeld

class CDebugPropertiesDialog : public CDialog
{
	DECLARE_DYNAMIC(CDebugPropertiesDialog)

public:
	CDebugPropertiesDialog(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CDebugPropertiesDialog();

// Dialogfelddaten
	enum { IDD = IDD_DEBUG_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	// schaltet das CursorClipping ein/aus
	BOOL m_ClipCursor;
	// schaltet das Ausgeben der Game Layouts ins Log-File ein/aus
	BOOL m_ReportGameLayouts;
};
