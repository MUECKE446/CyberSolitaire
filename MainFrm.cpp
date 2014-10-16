// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "CyberSolitaire.h"

#include "CyberSolitaireFrame.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "SolEditDoc.h"
#include "SolEditView.h"
#include "DockViewBar.h"
#include "ErrorReportDialog.h"
#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFRAMEPARENT)

BEGIN_MESSAGE_MAP(CMainFrame, CFRAMEPARENT)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_SHOW_GAMETREE, OnShowGametree)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GAMETREE, OnUpdateShowGametree)
	ON_COMMAND(ID_ERROR_REPORT, OnErrorReport)
	ON_UPDATE_COMMAND_UI(ID_ERROR_REPORT, OnUpdateErrorReport)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SPIEL_TICTACTOESPIELEN, OnSpielTictactoespielen)
	ON_UPDATE_COMMAND_UI(ID_SPIEL_TICTACTOESPIELEN, OnUpdateSpielTictactoespielen)
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_USER_PANE_UPDATE, OnSetPaneInStatusbar)
	ON_MESSAGE (WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE (WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_MESSAGE (WM_USER_TTT_GAME_LOADED, OnTTTGameLoaded)
END_MESSAGE_MAP()

static UINT GameIndicators[] =
{
	ID_SEPARATOR,           // Statusleistenanzeige
	ID_INDICATOR_PLAYER,
	ID_INDICATOR_GAME_NAME,
	ID_INDICATOR_FREE1,
	ID_INDICATOR_FREE2,
	ID_INDICATOR_SCORE,
};

static UINT TTTIndicators[] =
{
	//ID_SEPARATOR,           // Statusleistenanzeige
	ID_INDICATOR_PLAYER,
	ID_INDICATOR_GAME_NAME,
	ID_INDICATOR_TTT_LEVEL,
	ID_INDICATOR_TTT_PLAYED_GAMES
};

static UINT TTTFieldIndicators[] =
{
	//ID_SEPARATOR,           // Statusleistenanzeige
	ID_INDICATOR_PLAYER,
	ID_INDICATOR_TTT_LEVEL,
	ID_INDICATOR_TTT_GAME1,
	ID_INDICATOR_TTT_GAME2,
	ID_INDICATOR_TTT_GAME3,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerstˆrung

CMainFrame::CMainFrame()
{
	// ZU ERLEDIGEN: Hier Code zur Member-Initialisierung einf¸gen
	m_MainWndIsSizing = FALSE;	
	m_ClosingApplication = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFRAMEPARENT::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}
	if (!m_wndShowToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndShowToolBar.LoadToolBar(IDR_SHOWTOOLBAR))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(GameIndicators,
		  sizeof(GameIndicators)/sizeof(UINT)))
	{
		TRACE0("Statusleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	m_wndToolBar.SetWindowText(_T("Common"));
	m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_RIGHT);

	m_wndShowToolBar.SetWindowText(_T("Show"));
	m_wndShowToolBar.SetBarStyle( m_wndShowToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndShowToolBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_RIGHT);

	EnableDocking(CBRS_ALIGN_ANY);

	// wie wars beim letzten Mal
	CControlBar* pBar;
	CPoint	TopLeftPoint;
	pBar = (CControlBar*)&m_wndToolBar;
	TopLeftPoint.x = GetApplication()->GetProfileInt("BARS","TOOLBAR_LEFT",0);
	TopLeftPoint.y = GetApplication()->GetProfileInt("BARS","TOOLBAR_TOP",0);
	if (GetApplication()->GetProfileInt("BARS","TOOLBAR_FLOATING",0))
		FloatControlBar(pBar,TopLeftPoint);
	else
		DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

	pBar = (CControlBar*)&m_wndShowToolBar;
	TopLeftPoint.x = GetApplication()->GetProfileInt("BARS","SHOWTOOLBAR_LEFT",0);
	TopLeftPoint.y = GetApplication()->GetProfileInt("BARS","SHOWTOOLBAR_TOP",0);
	if (GetApplication()->GetProfileInt("BARS","SHOWTOOLBAR_FLOATING",0))
		FloatControlBar(pBar,TopLeftPoint);
	else
		DockControlBarLeftOf(&m_wndShowToolBar,&m_wndToolBar);

	// wie wars beim letzten Mal
	BOOL	BarVisible;
	pBar = (CControlBar*)&m_wndToolBar;
	BarVisible = GetApplication()->GetProfileInt("BARS","TOOLBAR_VISIBLE",1);
	ShowControlBar(pBar,BarVisible, FALSE);

	pBar = (CControlBar*)&m_wndShowToolBar;
	BarVisible = GetApplication()->GetProfileInt("BARS","SHOWTOOLBAR_VISIBLE",1);
	ShowControlBar(pBar,BarVisible, FALSE);

	pBar = (CControlBar*)&m_wndStatusBar;
	BarVisible = GetApplication()->GetProfileInt("BARS","STATUSBAR_VISIBLE",1);
	ShowControlBar(pBar,BarVisible, FALSE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFRAMEPARENT::PreCreateWindow(cs) )
		return FALSE;
	// ZU ERLEDIGEN: ƒndern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFRAMEPARENT::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFRAMEPARENT::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Nachrichten-Handler

void CMainFrame::GetSizeWorkspace(CSize &rect)
{
	CRect rc, rc2;
	m_wndToolBar.GetWindowRect(&rc );
	m_wndStatusBar.GetWindowRect(rc2 );
	rect.cx = (rc.bottom - rc.top) + (rc2.bottom - rc2.top);
	rect.cy = (rc.right - rc.left) + (rc2.right - rc2.left);

}


LRESULT CMainFrame::OnSetPaneInStatusbar (WPARAM wParam, LPARAM lParam)
{
	CString *text1 = (CString *)lParam;
	int idx = (int) wParam;

	// eventuell muﬂ der StatusBar umgeschaltet werden
	if (m_wndStatusBar.CommandToIndex( idx ) == -1)
		SwitchToGameStatusBar();
	if (m_wndStatusBar.CommandToIndex( idx ) == -1)
		SwitchToTTTStatusBar();
	if (m_wndStatusBar.CommandToIndex( idx ) == -1)
		SwitchToTTTFieldStatusBar();

	m_wndStatusBar.SetPaneText( m_wndStatusBar.CommandToIndex( idx ), 
															text1->GetBuffer(text1->GetLength()) );
	m_wndStatusBar.RedrawWindow();
  return 0;
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}


void CMainFrame::OnShowGametree() 
{
	CControlBar* pBar = CFrameWnd::GetControlBar(0xE808);
	if (pBar != NULL)
	{
		ASSERT_KINDOF(CSizeViewBar, pBar);
		//ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, TRUE);
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
	}
	return;
}

void CMainFrame::OnUpdateShowGametree(CCmdUI* pCmdUI) 
{
	CControlBar* pBar = CFrameWnd::GetControlBar(0xE808);

	if (pBar != NULL)
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
	}
	else
	{
		pCmdUI->SetCheck( FALSE );
	}
}


void CMainFrame::OnErrorReport() 
{
	CString Description("");
	CErrorReportDialog ErrorReportDlg;
	if (ErrorReportDlg.DoModal() == IDOK)
	{
		GetApplication()->ErrorReport(ErrorReportDlg.m_ErrorReportDescription);
	}
	return;
}

void CMainFrame::OnUpdateErrorReport(CCmdUI* pCmdUI) 
{
	/*
	CControlBar* pBar = CFrameWnd::GetControlBar(0xE808);

	if (pBar != NULL)
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
	}
	else
	{
		pCmdUI->SetCheck( FALSE );
	}
	*/
}


LRESULT CMainFrame::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	//TRACE("OnExitSizeMove\n");
	ASSERT(m_MainWndIsSizing);
	CCyberSolitaireDoc* pCyberSolDoc = GetRunningSolitaireGame();
	if (pCyberSolDoc)
	{
		CCyberSolitaireView* pView = pCyberSolDoc->GetView();
		pView->UpdateBitmaps();
 		// alle Piles zeichnen
 		pCyberSolDoc->UpdateAllViews(NULL,HINT_DISPLAY_ALL_PILES,NULL);
	}
	CSolEditDoc* pSolEditDoc = GetRunningSolEditor();
	if (pSolEditDoc)
	{
		CSolEditView* pView = pSolEditDoc->GetView();
		pView->UpdateBitmaps();
 		// alle Piles zeichnen
 		pSolEditDoc->UpdateAllViews(NULL,HINT_DISPLAY_ALL,NULL);
	}
	m_MainWndIsSizing = FALSE;	
	return 0;
}

LRESULT CMainFrame::OnEnterSizeMove(WPARAM wParam, LPARAM lParam)
{
	//TRACE("OnEnterSizeMove\n");
	m_MainWndIsSizing = TRUE;	
	return 0;
}

void CMainFrame::SetActiveViewDirect(CView *pView)
{
	if( m_pViewActive != pView )
	{
	   m_pViewActive = pView;
	}
}

void CMainFrame::OnClose() 
{
	m_ClosingApplication = TRUE;
	// zu_tun
	// vielleicht kommen noch andere hinzu
	if (GetRunningSolEditor() || GetRunningSolitaireGame())
		m_ClosingApplication = AskCloseApplication();
	if (!m_ClosingApplication)
		return;

	if (GetRunningSolEditor())
		GetApplication()->pSolEditTemplate->CloseAllDocuments(FALSE);
	CCyberSolitaireDoc* pDoc = NULL;
	if (pDoc = GetRunningSolitaireGame())
	{
		pDoc->m_ExitGameCause = EXIT_CAUSE_EXITAPP;
		GetApplication()->pCyberSolitaireTemplate->CloseAllDocuments(FALSE);
	}
	GetApplication()->pSelectSolitaireTemplate->CloseAllDocuments(FALSE);
	CFRAMEPARENT::OnClose();
}

void CMainFrame::OnSpielTictactoespielen() 
{
	CTTTDoc* pDoc = GetRunningTTTGame();
	if (pDoc)
	{
		AfxMessageBox(IDS_TTT_IS_RUNNING, MB_YESNO | MB_ICONINFORMATION);
		return;
	}
	// zu tun
	//CString DocFileName = GetView()->m_pGameTree->m_SelectedGame;
	//if (DocFileName.IsEmpty())
	{
		pDoc = (CTTTDoc*) (GetApplication()->pTTTGameTemplate)->OpenDocumentFile(NULL);
	}
	/*
	else
	{
		pDoc = (CSolEditDoc*) (GetApplication()->pSolEditTemplate)->OpenDocumentFile(DocFileName);
	}
	*/
	if (!pDoc)
		return;
	//pDoc->GetView()->UpdateBitmaps();
	//pDoc->UpdateAllViews(NULL,HINT_DISPLAY_ALL,NULL);
}

void CMainFrame::OnUpdateSpielTictactoespielen(CCmdUI* pCmdUI) 
{
 	pCmdUI->Enable(!GetRunningTTTGame());
}

BOOL CMainFrame::AskCloseApplication()
{
	// soll Cybersolitaire wirklich beendet werden
	CString Question;
	Question.LoadString(IDS_ASK_GAME_END);
	switch (AfxMessageBox(Question, MB_YESNO | MB_ICONQUESTION))
	{
	case IDYES:
		return TRUE;
	case IDNO:
		return FALSE;
	default:
		ASSERT(FALSE);
		break;
	}
	return FALSE;
}

// schaltet auf den TTT StatusBar, der auﬂerhalb spielbarer Felder gezeigt wird
void CMainFrame::SwitchToTTTStatusBar(void)
{
	m_wndStatusBar.SetIndicators(TTTIndicators,sizeof(TTTIndicators)/sizeof(UINT));
}

// schaltet auf den TTT StatusBar, der bei spielbaren Feldern gezeigt wird
void CMainFrame::SwitchToTTTFieldStatusBar(void)
{
	m_wndStatusBar.SetIndicators(TTTFieldIndicators,sizeof(TTTFieldIndicators)/sizeof(UINT));
}

// schaltet auf den Game StatusBar, der w‰hrend eines normalen Spieles gezeigt wird
void CMainFrame::SwitchToGameStatusBar(void)
{
	m_wndStatusBar.SetIndicators(GameIndicators,sizeof(GameIndicators)/sizeof(UINT));
}

LRESULT CMainFrame::OnTTTGameLoaded (WPARAM wParam, LPARAM lParam)
{

	MDINext();
	return NULL;
}
