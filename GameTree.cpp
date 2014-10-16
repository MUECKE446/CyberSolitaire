// GameTree.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "SelectDoc.h"
#include "SelectView.h"
#include "GameTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameTree

CGameTree::CGameTree()
{
}

CGameTree::~CGameTree()
{
}


BEGIN_MESSAGE_MAP(CGameTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CGameTree)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CGameTree 

int CGameTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	InitGameTree();
	return 0;
}

BOOL CGameTree::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle =	WS_EX_NOPARENTNOTIFY;
	return CTreeCtrl::PreCreateWindow(cs);
}

void CGameTree::ResizeDialog(UINT nType, int cx, int cy)
{
	OnSize( nType,  cx,  cy);
}

void CGameTree::OnSize(UINT nType, int cx, int cy) 
{
	CTreeCtrl::OnSize(nType, cx, cy);
	CRect rc, rctab;
	GetParent()->GetClientRect (&rc); 
	((CTabCtrl *)GetParent())->GetItemRect(0, &rctab);
	( rctab.top == 0 ) ?	rc.top += rctab.Height() : rc.bottom -= rctab.Height();
	MoveWindow (&rc);
}

void CGameTree::InitGameTree()
{
	CSelectDoc* pDoc = (CSelectDoc*)((CSelectView*)GetParent())->GetDocument();
	HTREEITEM  hTreeItem, hTreeItem1, hTreeItem2;
	TV_INSERTSTRUCT TreeCtrlItem;
	CString subtitle;
	//CImageList *pimgl;
	
	//------- clear existing tree
	if( GetCount() > 0 )
		DeleteAllItems();
	//if( GetImageList(TVSIL_NORMAL) == NULL )
	//{
	//	pimgl = SetImageList( phIml, TVSIL_NORMAL );
	//}
	{
		TreeCtrlItem.hParent				= TVI_ROOT;
		TreeCtrlItem.hInsertAfter			= TVI_LAST;
		//TreeCtrlItem.item.mask				= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TreeCtrlItem.item.mask				= TVIF_TEXT | TVIF_PARAM;
		TreeCtrlItem.item.pszText			= (char *)"CyberSolitaire";
		TreeCtrlItem.item.lParam			= ROOT;
		TreeCtrlItem.item.iImage			= ROOT;
		TreeCtrlItem.item.iSelectedImage	= ROOT;
		hTreeItem							= InsertItem(&TreeCtrlItem);

		subtitle = "alle Spiele";
		TreeCtrlItem.hParent				= hTreeItem;
		TreeCtrlItem.hInsertAfter			= TVI_FIRST;
		TreeCtrlItem.item.mask				= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TreeCtrlItem.item.pszText			= (char *)(LPCTSTR)subtitle;
		TreeCtrlItem.item.lParam			= ALL_GAMES;
		TreeCtrlItem.item.iImage			= ALL_GAMES;
		TreeCtrlItem.item.iSelectedImage	= ALL_GAMES;
		hTreeItem1							= InsertItem(&TreeCtrlItem);

		CString	GameName;
		POSITION pos = pDoc->m_GameList.GetHeadPosition();
		while (pos)
		{
			GameName = pDoc->m_GameList.GetNext(pos);
			TreeCtrlItem.hParent				= hTreeItem1;
			TreeCtrlItem.hInsertAfter			= TVI_LAST;
			TreeCtrlItem.item.mask				= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			TreeCtrlItem.item.pszText			= (char *)(LPCTSTR)GameName;
			TreeCtrlItem.item.lParam			= GAMES;
			TreeCtrlItem.item.iImage			= GAMES;
			TreeCtrlItem.item.iSelectedImage	= GAMES;
			hTreeItem2							= InsertItem(&TreeCtrlItem);
		}
		//-------- expand tree so you will see the games
		SortChildren(hTreeItem1);
		Expand(GetRootItem(),TVE_EXPAND);
		Expand(hTreeItem1,TVE_EXPAND);
	}
}

void CGameTree::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString GameName;
	HTREEITEM iGame,iCategory;
	UINT flag;
	CPoint pt;

	CSelectDoc* pDoc = (CSelectDoc*)((CSelectView*)GetParent())->GetDocument();

	//-------------- make the marked item selected (use info for new item in the
	//               NM_TREEVIEW structure and interpret image information as CPoint
	GetCursorPos(&pt);
	ScreenToClient( &pt );
	HTREEITEM CurItem = HitTest( pt, &flag );
	DWORD typ = GetItemData( GetSelectedItem() );
	switch ( typ )
	{
		case GAMES:			//---------- level page
					iGame = GetSelectedItem();
					GameName = GetItemText(iGame);
					iCategory = GetParentItem( iGame );
					SetCheck( iCategory, TRUE );
					pDoc->NewGame(GameName);
					m_SelectedGame = GameName;
					break;
	}
	*pResult = 0;
}

// ruft das entsprechende Contextmenu auf
//
void CGameTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
CMenu Men;
CMenu *Popup;
UINT flag;
CPoint pt;
bool ShowIt = FALSE;

	m_SelectedGame = _T("");
	//-------------- make the marked item selected (use info for new item in the
	//               NM_TREEVIEW structure and interpret image information as CPoint
	GetCursorPos(&pt);
	ScreenToClient( &pt );
	HTREEITEM CurItem = HitTest( pt, &flag );
	//-------------- get size of tree window
	CRect r;
	GetWindowRect( r );
	if( CurItem )
	{
		//-------------- get position of selected item
		CRect Sel;
		GetItemRect(CurItem , Sel, TRUE );
		//-------------- get type of selected item
		DWORD typ = GetItemData( CurItem );
		//-------------- call popup menues corresponding to tree level
		switch ( typ )
		{
			case ROOT:			//---------- level CyberSoiltaire
					Men.LoadMenu(IDR_GAME_CONTEXT);
					m_SelectedGame = "";
					ShowIt = TRUE;
					break;
			case ALL_GAMES:		//---------- level Gruppe ALL_GAMES
					ShowIt = FALSE;
					break;
			case GAMES:			//---------- level GAMES
					Men.LoadMenu(IDR_GAME_CONTEXT);
					m_SelectedGame = GetItemText(CurItem);
					//dprintf("Seletcted Game:%s",m_SelectedGame);
					ShowIt = TRUE;
					break;
		}
		if( ShowIt )
		{
			Popup = Men.GetSubMenu(0);
			ASSERT(Popup != NULL);
			Popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON,r.right, Sel.top+r.top, AfxGetMainWnd(), NULL);
		}
	}
	*pResult = 0;
}

