// ProjView.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "CyberSolitaireView.h"
#include "CyberSolitaireDoc.h"
#include "MainFrm.h"
#include "GameTree.h"
#include "SelectDoc.h"
#include "SelectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectView

IMPLEMENT_DYNCREATE(CSelectView, CCtrlView)

CSelectView::CSelectView()
	: CCtrlView(_T ("SysTabControl32"), AFX_WS_DEFAULT_VIEW)
{
	//{{AFX_DATA_INIT(CSelectView)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	m_pGameTree = NULL;
	m_pTab = NULL;
	m_pApp = (CCyberSolitaireApp *)AfxGetApp();
	m_pDoc = (CSelectDoc *)GetDocument();
}

CSelectView::~CSelectView()
{
	/*
	delete m_FbTree;
	*/
	if (m_pGameTree)
		delete m_pGameTree;
	return;
}


BEGIN_MESSAGE_MAP(CSelectView, CCtrlView)
	//{{AFX_MSG_MAP(CSelectView)
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_DESCRIPTION, OnDescription)
	ON_COMMAND(ID_DESCRIPTION_SHORT, OnDescriptionShort)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_DESCRIPTION, OnUpdateDescription)
	ON_UPDATE_COMMAND_UI(ID_DESCRIPTION_SHORT, OnUpdateDescriptionShort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Diagnose CSelectView

#ifdef _DEBUG
void CSelectView::AssertValid() const
{
	CCtrlView::AssertValid();
}

void CSelectView::Dump(CDumpContext& dc) const
{
	CCtrlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSelectView 
//
//
BOOL CSelectView::PreCreateWindow(CREATESTRUCT& cs) 
{
	::InitCommonControls();
	cs.style |= TCS_BOTTOM;
	if( !CCtrlView::PreCreateWindow(cs) )
		return FALSE;
  return TRUE;	
}

//********************************************************************
//  CSelectView::OnInitialUpdate()
//
//  Parameter: -
//
//  Return   :	-
//
//  - - - - - - - - - - - - - - - - - - -
//  Description
//  Initialize the tab-view and create the corresponding tree controls
//
//
void CSelectView::OnInitialUpdate() 
{
	CString title;
	//---------- insert the menu
	CMainFrame* fr = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	//CMenu mymenu;
	//mymenu.LoadMenu( IDR_SELECTFRAME );
	//fr->MergeMenu(NULL,&mymenu, 1);

	m_pTab = (CTabCtrl *)this;
	m_pDoc = (CSelectDoc *)GetDocument();
	m_pDoc->m_pView = this;

	//------------------ calculate size of project window	
	CRect rec, irec;
	CSize rcb;
	AfxGetMainWnd()->GetClientRect( &rec );
	fr->GetSizeWorkspace(rcb);
	rec.right = rec.right/4;
	rec.bottom -= rcb.cx;
	GetOwner()->MoveWindow( &rec);
	//--------------- select the font
	LOGFONT lFont;
	CFont *pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lFont);
	lFont.lfWeight = FW_NORMAL;
	lFont.lfHeight = 15;
	lFont.lfWidth = 0;
	strcpy(lFont.lfFaceName,"Arial");

	m_selFont.CreateFontIndirect(&lFont);
	SetFont( &m_selFont);
	//
	//------------------- create the tab card
	TC_ITEM TabItem;
	TabItem.mask = TCIF_TEXT | TCIF_PARAM;
	CRect rc, rctab;
	m_pTab->GetClientRect (&rc); 
	m_pTab->GetItemRect( 0, &rctab );
	( rctab.top == 0 ) ?	rc.top += rctab.Height() : rc.bottom -= rctab.Height();
	//
	//------------------- add tab card with gameselection
	title.LoadString(IDS_GAME_SELECTION);
	TabItem.pszText = (LPSTR)(LPCSTR)title;
	TabItem.lParam = 0;
	m_pTab->InsertItem( 0, &TabItem );
	m_pTab->SetCurSel(0);

	m_pGameTree = new CGameTree( );
	m_pGameTree->Create(	TVS_HASLINES | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
										rc, m_pTab, IDD_GAME_TREE);
	m_pGameTree->ShowWindow(SW_SHOW);
	///*| TVS_LINESATROOT | TVS_TRACKSELECT*/ 
}



//********************************************************************
//  CSelectView::OnSize(UINT nType, int cx, int cy)
//
//  Parameter:	UINT nType
//							int cx
//							int cy
//
//  Return   :	-
//
//  - - - - - - - - - - - - - - - - - - -
//  Description
//  calculate the window size in case of resizing the window
//
//
//---------------------------------------------------------------------
void CSelectView::OnSize(UINT nType, int cx, int cy) 
{
	CCtrlView::OnSize(nType, cx, cy);

	if( m_pGameTree )
	{
		m_pGameTree->ResizeDialog( nType, cx, cy);
	}
}



//********************************************************************
//  CSelectView::OnDestroy()
//
//  Parameter:	
//
//  Return   :	-
//
//  - - - - - - - - - - - - - - - - - - -
//  Description
//  
//
//---------------------------------------------------------------------
void CSelectView::OnDestroy() 
{
	CMainFrame* fr = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	if (m_pGameTree) m_pGameTree->DestroyWindow(); 
	((CMainFrame *)AfxGetMainWnd())->SetActiveViewDirect(NULL);
	((CMainFrame *)AfxGetMainWnd())->SetActiveWindow();
	CCtrlView::OnDestroy();
}


//********************************************************************
//  CSelectView::DestroyWindow()
//
//  Parameter:	
//
//  Return   :	-
//
//  - - - - - - - - - - - - - - - - - - -
//  Description
//  
//
//---------------------------------------------------------------------
BOOL CSelectView::DestroyWindow() 
{
	/*
	CMainFrame* fr = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	//---------- insert the menu
	CMenu mymenu;
	mymenu.LoadMenu( IDR_PROJFRAME );
	fr->RemoveMenu(NULL,&mymenu);

	((CMainFrame *)AfxGetMainWnd())->ActivateFrame();

	delete m_WbTree;
	delete m_FbTree;
	*/

	return CCtrlView::DestroyWindow();
}




//********************************************************************
//  CSelectView::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
//
//  Parameter:	NMHDR* pNMHDR
//							LRESULT* pResult
//
//  Return   :	-
//
//  - - - - - - - - - - - - - - - - - - -
//  Description
//  the given tab card is the new selected card, so you have to show it.
//
//
//---------------------------------------------------------------------
/*
void CSelectView::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	switch( m_pTab->GetCurSel() )
	{
		case 0:  									// tab card workbench
			m_WbTree->ShowWindow(SW_SHOW);
			break;
		case 1: 									// tab card function blocks 
			m_FbTree->ShowWindow(SW_SHOW);
			break;
	}
	*pResult = 0;
}
*/
//********************************************************************
//  CSelectView::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
//
//  Parameter:	NMHDR* pNMHDR
//							LRESULT* pResult
//
//  Return   :	-
//
//  - - - - - - - - - - - - - - - - - - -
//  Description
//  the last selected tab card is no more actual. So you have to hide it.
//
//
//---------------------------------------------------------------------
/*
void CSelectView::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int tab = m_pTab->GetCurSel();
//	pDoc->EnableMenue( tab );

	switch( tab )
	{
		case 0:									// tab card workbench  
			m_WbTree->ShowWindow(SW_HIDE);
			break;
		case 1:									// tab card function blocks
			m_FbTree->ShowWindow(SW_HIDE);
			break;
	}
	*pResult = 0;
}
*/

//******************************************************************************
//******************************************************************************
//
// update methods
//
//******************************************************************************
//******************************************************************************


void CSelectView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
/*
	CString title;
	TC_ITEM TabItem;
	TabItem.mask = TCIF_TEXT;
	title.LoadString(IDS_TXT_DATABASE);
	TabItem.pszText = (LPSTR)(LPCSTR)title;
	m_pTab->SetItem( 0, &TabItem );
	title.LoadString(IDS_TXT_FB);
	TabItem.pszText = (LPSTR)(LPCSTR)title;
	m_pTab->SetItem( 1, &TabItem );


	if( m_WbTree )
	{
		int tab = m_pTab->GetCurSel();
		switch( tab )
		{
			case 0:									// tab card workbench  
				m_WbTree->ShowWindow(SW_HIDE);
				m_WbTree->ShowWindow(SW_SHOW);
				break;
			case 1:									// tab card function blocks
				m_FbTree->ShowWindow(SW_HIDE);
				m_FbTree->ShowWindow(SW_SHOW);
				break;
		}
		m_WbTree->SetLastItem(FALSE);
	}
*/
}

void CSelectView::PostNcDestroy() 
{
	CCtrlView::PostNcDestroy();
}

//*********************************************************************
// FUNTION        OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest,
//                                 UINT message)
//
// PARAMETER      CWnd* pDesktopWnd
//                UINT nHitTest
//                UINT message
//
// RETURN         -
//
// DESCRIPTION
//	prevents the active view beeing set in the frame window
//  if you close window than thereis no problem while closing
//---------------------------------------------------------------------
int CSelectView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	CMainFrame* pParentFrame = (CMainFrame *)GetTopLevelParent();
	pParentFrame->SetActiveViewDirect(this);
	return nResult;
}


void CSelectView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CCtrlView::OnShowWindow(bShow, nStatus);

}

BOOL CSelectView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	if ( pMsg->wParam == VK_ESCAPE )
		return TRUE;
	
	return CCtrlView::PreTranslateMessage(pMsg);
}



void CSelectView::OnDescription() 
{
	ASSERT(!m_pGameTree->m_SelectedGame.IsEmpty());
	CString HelpKey = m_pGameTree->m_SelectedGame;
	GetApplication()->WinHelp((DWORD)((LPCSTR)HelpKey),HELP_KEY);	
}

void CSelectView::OnDescriptionShort() 
{
	ASSERT(!m_pGameTree->m_SelectedGame.IsEmpty());
	CString TmpString;
	TmpString.LoadString(IDS_SHORTDESCRIPTOR);
	CString HelpKey = m_pGameTree->m_SelectedGame + TmpString;
	GetApplication()->WinHelp((DWORD)((LPCSTR)HelpKey),HELP_KEY);	
}



void CSelectView::OnUpdateDescription(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_pGameTree->m_SelectedGame.IsEmpty());
}

void CSelectView::OnUpdateDescriptionShort(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_pGameTree->m_SelectedGame.IsEmpty());
}
