// ErrorReportDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "cybersolitaire.h"
#include "ErrorReportDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CErrorReportDialog 


CErrorReportDialog::CErrorReportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorReportDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CErrorReportDialog)
	m_ErrorReportDescription = _T("");
	//}}AFX_DATA_INIT
}


void CErrorReportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorReportDialog)
	DDX_Text(pDX, IDC_ERROR_DESCRIPTION, m_ErrorReportDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorReportDialog, CDialog)
	//{{AFX_MSG_MAP(CErrorReportDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CErrorReportDialog 

BOOL CErrorReportDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}
