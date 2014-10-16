#if !defined(AFX_ERRORREPORTDIALOG_H__A1B15DD6_29FF_11D7_9804_00A0CC562BEB__INCLUDED_)
#define AFX_ERRORREPORTDIALOG_H__A1B15DD6_29FF_11D7_9804_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorReportDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CErrorReportDialog 

class CErrorReportDialog : public CDialog
{
// Konstruktion
public:
	CErrorReportDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CErrorReportDialog)
	enum { IDD = IDD_ERROR_REPORT };
	CString	m_ErrorReportDescription;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CErrorReportDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CErrorReportDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ERRORREPORTDIALOG_H__A1B15DD6_29FF_11D7_9804_00A0CC562BEB__INCLUDED_
