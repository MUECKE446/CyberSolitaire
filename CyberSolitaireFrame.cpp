// CyberSolitaireFrame.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "MainFrm.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "CyberSolitaireFrame.h"
#include ".\cybersolitaireframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireFrame

IMPLEMENT_DYNCREATE(CCyberSolitaireFrame, CMDIChildWnd)

CCyberSolitaireFrame::CCyberSolitaireFrame()
{
}

CCyberSolitaireFrame::~CCyberSolitaireFrame()
{
	TRACE("Destruct CyberSolitaireFrame\n");
}


BEGIN_MESSAGE_MAP(CCyberSolitaireFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCyberSolitaireFrame)
	//}}AFX_MSG_MAP
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CCyberSolitaireFrame 

BOOL CCyberSolitaireFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// um dem CyberSolitaireDoc sein Icon zu bescheren
	//cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, NULL, NULL, AfxGetApp()->LoadIcon(IDI_CYBERSOLITAIREDOC));
	cs.style = WS_CHILD | WS_VISIBLE | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MAXIMIZE;
	return CMDIChildWnd::PreCreateWindow(cs);
}


void CCyberSolitaireFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	if (bActivate)
	{
		pMainFrame->SwitchToGameStatusBar();
		pActivateWnd->ShowWindow(SW_SHOWMAXIMIZED);
		CCyberSolitaireDoc* pDoc = (CCyberSolitaireDoc*)((CFrameWnd*)pActivateWnd)->GetActiveDocument();
		if (pDoc)
		{
			pDoc->SetTTTGameMenu();
			pDoc->ComputeScore();
		}
	}
}

