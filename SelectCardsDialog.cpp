// SelectCardsDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "cybersolitaire.h"
#include "SelectCardsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSelectCardsDialog 


CSelectCardsDialog::CSelectCardsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectCardsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectCardsDialog)
	//}}AFX_DATA_INIT
}


void CSelectCardsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectCardsDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectCardsDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectCardsDialog)
	ON_BN_CLICKED(IDC_CARD001, OnCard001)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f�r Nachrichten CSelectCardsDialog 

BOOL CSelectCardsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zus�tzliche Initialisierung hier einf�gen
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zur�ckgeben
}

void CSelectCardsDialog::OnCard001() 
{
	// TODO: Code f�r die Behandlungsroutine der Steuerelement-Benachrichtigung hier einf�gen
	int a = 0;
}
