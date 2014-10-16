// TTTFrame.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "MainFrm.h"
#include "TTTField.h"
#include "TTTDoc.h"
#include "TTTView.h"
#include "TTTFrame.h"
#include ".\tttframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTTFrame

IMPLEMENT_DYNCREATE(CTTTFrame, CMDIChildWnd)

CTTTFrame::CTTTFrame()
{
}

CTTTFrame::~CTTTFrame()
{
}


BEGIN_MESSAGE_MAP(CTTTFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CTTTFrame)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CTTTFrame 

int CTTTFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Fügen Sie Ihren spezialisierten Erstellcode hier ein.
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CStatusBar* pStatusBar = &pMainFrame->m_wndStatusBar;
	// zu_tun
	// muß wieder raus
	/*
	if (!pStatusBar->SetIndicators(TTTIndicators,sizeof(TTTIndicators)/sizeof(UINT)))
	{
		TRACE0("Statusleiste konnte nicht umgestellt werden\n");
		return -1;      // Fehler bei Erstellung
	}
	*/

	//CControlBar* pBar = (CControlBar*)&pMainFrame->m_wndStatusBar;
	//ShowControlBar(pBar,FALSE, FALSE);


	return 0;
}

BOOL CTTTFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_CHILD | WS_VISIBLE | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MAXIMIZE;
	return CMDIChildWnd::PreCreateWindow(cs);
}

void CTTTFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	if (bActivate)
	{
		pActivateWnd->ShowWindow(SW_SHOWMAXIMIZED);
	}
}


