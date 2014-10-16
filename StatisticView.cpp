// StatisticView.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "cybersolitaire.h"
#include "Player.h"
#include "SelectDoc.h"
#include "StatisticDoc.h"
#include "StatisticView.h"
#include ".\statisticview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticView

IMPLEMENT_DYNCREATE(CStatisticView, CFormView)


CStatisticView::CStatisticView()
	: CFormView(CStatisticView::IDD)
{
	//{{AFX_DATA_INIT(CStatisticView)
	m_ScrollBarValue = 0;
	//}}AFX_DATA_INIT
	m_DrawMode = DRAW_POINT;
}

CStatisticView::~CStatisticView()
{
	delete m_pDispWnd;
}

void CStatisticView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticView)
	DDX_Control(pDX, IDC_SCROLLBAR_RESULTS, m_ScrollBarResults);
	DDX_Control(pDX, IDC_PLAYER_LISTBOX, m_PlayerList);
	DDX_Scroll(pDX, IDC_SCROLLBAR_RESULTS, m_ScrollBarValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticView, CFormView)
	//{{AFX_MSG_MAP(CStatisticView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PLAYER_LISTBOX, OnItemchangedPlayerListbox)
	//}}AFX_MSG_MAP
ON_NOTIFY(LVN_ITEMCHANGED, IDC_PLAYER_LISTBOX, OnLvnItemchangedPlayerListbox)
ON_COMMAND(ID_DARSTELLUNG_PUNKTE, OnDarstellungPunkte)
ON_COMMAND(ID_DARSTELLUNG_LINIEN, OnDarstellungLinien)
ON_COMMAND(ID_DARSTELLUNG_BALKEN, OnDarstellungBalken)
ON_UPDATE_COMMAND_UI(ID_DARSTELLUNG_PUNKTE, OnUpdateDarstellungPunkte)
ON_UPDATE_COMMAND_UI(ID_DARSTELLUNG_LINIEN, OnUpdateDarstellungLinien)
ON_UPDATE_COMMAND_UI(ID_DARSTELLUNG_BALKEN, OnUpdateDarstellungBalken)
ON_COMMAND(ID_STATISTIK_LOESCHEN_ACT_PLAYER, OnStatistikLoeschenActPlayer)
ON_COMMAND(ID_STATISTIK_LOESCHEN_ALL_PLAYER, OnStatistikLoeschenAllPlayer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Diagnose CStatisticView

#ifdef _DEBUG
void CStatisticView::AssertValid() const
{
	CFormView::AssertValid();
}

void CStatisticView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CStatisticView 

void CStatisticView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
	
}


void CStatisticView::OnDraw(CDC* pDC) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	//return;
	CRect DrawRect(DspWndRect);
	DrawRect += DspWndOrg;
	
	//pDC->Rectangle(&DspWndRect);
	
}

int CStatisticView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}


void CStatisticView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	/*
	CFont* pFont = pDispWnd->GetFont();
	LOGFONT font;
	pFont->GetLogFont( &font );
	font.lfWeight = FW_EXTRABOLD;
	font.lfHeight = 48;
	CFont* pNewFont = new CFont;
	pNewFont->CreateFontIndirect( &font );
	pDispWnd->SetFont( pNewFont );
*/
	DWORD Style = m_PlayerList.GetExtendedStyle();
	Style |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	//Style |= LVS_EX_CHECKBOXES;
	m_PlayerList.SetExtendedStyle(Style);
	m_PlayerList.SetBkColor(RGB(255, 255, 210));
	m_PlayerList.SetTextBkColor(RGB(255, 255, 210));
	
	//--------- fill list
	FillPlayerList(TRUE);

	LVFINDINFO info;
	int nIndex;
	//info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.flags = LVFI_STRING;
	info.psz = GetApplication()->m_CurrentPlayerName;

	if ((nIndex = m_PlayerList.FindItem(&info)) != -1)
	{
		m_PlayerList.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
		m_PlayerName = info.psz;
		//m_ArrIndex = m_PlayerList.GetItemData(nIndex);
	}

	//
	BuildResults(m_PlayerName,m_Results);

	//CRect ClientRect;
	CRect WindowRect;
	CWnd* pDispWnd = this->GetDlgItem( IDC_DISP_WINDOW );
	pDispWnd->GetWindowRect(WindowRect);
	ScreenToClient(WindowRect);
	//pDispWnd->GetClientRect(ClientRect);
	//CRect DrawRect(DspWndRect);
	//DrawRect += DspWndOrg;
	m_pDispWnd = new CDispWnd(this,m_Results);
	m_pDispWnd->Create(_T("STATIC"), "Hi", WS_CHILD | WS_VISIBLE,WindowRect, this, IDC_DISP_WINDOW);
	int ScrollMax =	 m_Results.GetCount()/25;
	if (m_Results.GetCount()%25 != 0)
		ScrollMax++;
	m_ScrollBarResults.SetScrollRange(1,ScrollMax);
	UpdateData(FALSE);
	//UpdateData();
}


/*
LVS_EX_CHECKBOXES		The control supplies check boxes for each item. You can retrieve the state of the check box 
						by using the ListView_GetCheckState macro. 
LVS_EX_FULLROWSELECT	When an item is selected, all of its subitems are also displayed as elected. Clicking on any 
						subitem will select the entire row. This extended style is only effective in conjunction with the LVS_REPORT style. 
LVS_EX_GRIDLINES		Dashed gridlines are displayed around all items and subitems. This extended style is only 
						effective in conjunction with the LVS_REPORT style. 
LVS_EX_HEADERDRAGDROP	Enables drag-and-drop re-ordering of the columns in the ListView. This extended style is only 
						effective in conjunction with the LVS_REPORT style. 
LVS_EX_SUBITEMIMAGES	Allows images to be displayed for subitems. This extended style is only effective in conjunction 
						with the LVS_REPORT style. 
LVS_EX_TRACKSELECT		Enables hot tracking of items in a ListView control. Hot Tracking, also known as Hover Selection, 
						means that an item is automatically selected when the mouse pointer is over it for more than 1 second. This style applies to all styles of the ListView control. 
*/

BOOL CStatisticView::FillPlayerList(BOOL WithTitleBar)
{
	int				iItem = 0;
	int				iActualItem;
	LV_ITEM			item;
	LV_COLUMN		column;
	CString			title;

	//--------------------- build titlebar of list
	if(WithTitleBar)
	{
		title.LoadString(IDS_TITLE_PLAYER_NAME);
		column.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		column.fmt	= LVCFMT_LEFT;
		column.cx		= 90;
		column.pszText	= (LPSTR)(LPCSTR)title;
		column.iSubItem = 1;
		m_PlayerList.InsertColumn(0, &column);
		
		column.cx		= 70;
		title.LoadString(IDS_TITLE_PLAYER_RANK);
		column.pszText	= (LPSTR)(LPCSTR)title;
		column.iSubItem = 1;
		m_PlayerList.InsertColumn(1, &column);
		
		column.cx		= 70;
		title.LoadString(IDS_TITLE_PLAYER_NUMBER_OF_GAMES);
		column.pszText	= (LPSTR)(LPCSTR)title;
		column.iSubItem = 1;
		m_PlayerList.InsertColumn(2, &column);
		
		column.cx		= 90;
		title.LoadString(IDS_TITLE_PLAYER_AVERAGE);
		column.pszText	= (LPSTR)(LPCSTR)title;
		column.iSubItem = 1;
		m_PlayerList.InsertColumn(3, &column);
		
		column.cx		= 90;
		title.LoadString(IDS_TITLE_PLAYER_AVERAGE_PERCENT);
		column.pszText	= (LPSTR)(LPCSTR)title;
		column.iSubItem = 1;
		m_PlayerList.InsertColumn(4, &column);
		
		column.cx		= 110;
		title.LoadString(IDS_TITLE_PLAYER_AVERAGE_STANDARD);
		column.pszText	= (LPSTR)(LPCSTR)title;
		column.iSubItem = 1;
		m_PlayerList.InsertColumn(5, &column);
	}
	m_PlayerList.DeleteAllItems();
	//--------------------- fill list with information
	int i;
	CPlayer Player;
	for (i=0;i<=GetApplication()->GetSelectDoc()->m_PlayerArr.GetUpperBound();i++)
	{
		Player = GetApplication()->GetSelectDoc()->m_PlayerArr.GetAt(i);
		//----------- get players nickname
		item.mask = LVIF_TEXT | LVIF_PARAM;
		item.iItem = iItem++;
		item.iSubItem = 0;
		item.pszText = (LPSTR)(LPCSTR) Player.m_NickName;
		item.lParam = (DWORD)iItem;
		iActualItem = m_PlayerList.InsertItem(&item);
		//----------- get players rank
		item.mask = LVIF_TEXT ;
		item.pszText = (LPSTR)(LPCSTR) Player.m_Rank;
		item.iItem = iActualItem;
		item.iSubItem = 1;
		m_PlayerList.SetItem(&item);
		// Index im Array
		m_PlayerList.SetItemData( iActualItem, (DWORD)i );
	}
	#ifdef _DEBUG
		//afxDump.SetDepth( 1 );
		//afxDump << "m_PlayerArr: " << &GetSelectDoc()->->m_PlayerArr << "\n";
	#endif
	return TRUE;
}

void CStatisticView::OnLvnItemchangedPlayerListbox(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	if (pNMLV->uOldState == 0 && pNMLV->uNewState == 0)
		return;	// No change

	BOOL bPrevState = (BOOL)(((pNMLV->uOldState & 
				LVIS_STATEIMAGEMASK)>>12)-1);   // Old check box state
	if (bPrevState < 0)	// On startup there's no previous state 
		bPrevState = 0; // so assign as false (unchecked)

	// New check box state
	BOOL bChecked=(BOOL)(((pNMLV->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
	if (bChecked < 0) // On non-checkbox notifications assume false
		bChecked = 0; 

	if (bPrevState == bChecked) // No change in check box
		return;
	
	// Now bChecked holds the new check box state

	// ....
}

BOOL CStatisticView::BuildResults(CString& PlayerName, CResultArray& ResultArr)
{
	CStatisticDoc* pDoc = (CStatisticDoc*)GetDocument();
	ResultArr.RemoveAll();
	CSingleGameResult SingleGameResult;
	for (POSITION pos = pDoc->m_pStatistic->m_GameResultList.GetHeadPosition();pos!= NULL;)
	{
		SingleGameResult = pDoc->m_pStatistic->m_GameResultList.GetNext(pos);
		if (SingleGameResult.m_PlayerName == PlayerName)
		{
			ResultArr.Add(SingleGameResult.m_Points);
		}
	}
	return !(ResultArr.GetUpperBound() == -1);
}

// CDispWnd

IMPLEMENT_DYNAMIC(CDispWnd, CStatic)

//LOGFONT CDispWnd::SmallLogFont = {12,9,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,0,0,DRAFT_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"ARIAL"};
//LOGFONT CDispWnd::SmallLogFont = {10,7,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,0,0,DRAFT_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"ARIAL"};
LOGFONT CDispWnd::SmallLogFont = {7,6,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,0,0,DRAFT_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"ARIAL"};


CDispWnd::CDispWnd()
{
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(&SmallLogFont);
	m_pView = NULL;
}

CDispWnd::CDispWnd(CStatisticView* pView, CResultArray& ResultArr)
{
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(&SmallLogFont);
	m_pView = pView;
	m_pResultArr = &ResultArr;
}

CDispWnd::~CDispWnd()
{
	delete m_pFont;
}


BEGIN_MESSAGE_MAP(CDispWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CDispWnd-Meldungshandler


// Achsen und Beschriftungen zeichnen
void CDispWnd::DrawAxes(CDC& dc, int StartValue, int MaxValue)
{
	CRect ClientRect;
	GetClientRect(ClientRect);
	
	int x,y;
	int i;
	int	xs = (ClientRect.right - 2*OFFSET_X);
	int	dx = xs / 24;
	int NewMaxValue = ((MaxValue+10) / 10) * 10;
	int StartValueIntern;
	if (StartValue < 1)
		StartValueIntern = 1;
	else
		StartValueIntern = StartValue; 
	dc.SetTextAlign(TA_CENTER);
	x = 1 * dx;
	CString txt;
	txt.Format("%d",StartValueIntern);
	dc.TextOut(x,0,txt);
	for (i=5;i<=25;i+=5)
	{
		x = i * dx;
		CString txt;
		txt.Format("%d",StartValueIntern+i-1);
		dc.TextOut(x,0,txt);
	}
	//dc.SetTextAlign(TA_TOP | TA_RIGHT);
	dc.SetTextAlign(TA_TOP | TA_LEFT);
	for ( i=20;i<=NewMaxValue;i+=20)
	{
		y = i * (-ClientRect.bottom + 2*OFFSET_Y) / NewMaxValue;
		y -=5;
		CString txt;
		txt.Format("%d",i);
		//dc.TextOut(-OFFSET_Y,y,txt);
		dc.TextOut(0,y,txt);
	}
	// Axen
	int	x1 = 1*dx;
	dc.MoveTo(x1,0);
	dc.LineTo(25*dx,0);
	//dc.LineTo(ClientRect.right-2*OFFSET_X,0);
	dc.MoveTo(x1,0);
	dc.LineTo(x1,-ClientRect.bottom+2*OFFSET_Y);
	// Gitternetz
	CPen* pOldPen = (CPen*)dc.SelectStockObject(WHITE_PEN);
	for (i=5;i<=25;i+=5)
	{
		x = i * dx;
		dc.MoveTo(x,-1);
		y = MaxValue * (-ClientRect.bottom + 2*OFFSET_Y) / NewMaxValue;
		//y = -ClientRect.bottom + OFFSET_Y;
		dc.LineTo(CPoint(x,y));
	}
	//for ( i=20;i<=NewMaxValue;i+=20)
	for ( i=10;i<NewMaxValue;i+=10)
	{
		y = i * (-ClientRect.bottom + 2*OFFSET_Y) / NewMaxValue;
		dc.MoveTo(1*dx+1,y);
		dc.LineTo(CPoint(25*dx,y));
	}
	y = MaxValue * (-ClientRect.bottom + 2*OFFSET_Y) / NewMaxValue;
	dc.MoveTo(1*dx+1,y);
	//x = ClientRect.right - 2*OFFSET_X;
	dc.LineTo(CPoint(25*dx,y));
	dc.SelectObject(pOldPen);
}

void CDispWnd::DrawResults(CDC& dc,int StartValue,int MaxValue)
{
	CRect ClientRect;
	GetClientRect(ClientRect);
	
	int x,y;
	int result;
	int	xs = (ClientRect.right - 2*OFFSET_X);
	int	dx = xs / 24;
	int NewMaxValue = ((MaxValue+10) / 10) * 10;
	if (m_pResultArr->GetUpperBound() == -1)
		return;
	// zeichne die 25 Resultate ab StartValue (sofern vorhanden)
	x = 1;
	for (int i=StartValue ;i<= StartValue + 24; i++)
	{
		if (i<=m_pResultArr->GetUpperBound())
			result = m_pResultArr->GetAt(i);
		else
			break;
		y = result * (-ClientRect.bottom + 2*OFFSET_Y) / NewMaxValue;
		switch (m_pView->m_DrawMode)
		{
		case DRAW_POINT:
			dc.MoveTo(CPoint(x*dx,y));
			// zeichne ein kleines Kästchen
			dc.FillSolidRect(CRect(x*dx-2,y-2,x*dx+2,y+2),RED_CLR);
			break;
		case DRAW_LINE:
			if (x==1)
				dc.MoveTo(CPoint(x*dx,y));
			else
				dc.LineTo(CPoint(x*dx,y));
			// zeichne ein kleines Kästchen
			dc.FillSolidRect(CRect(x*dx-2,y-2,x*dx+2,y+2),RED_CLR);
			break;
		case DRAW_BAR:
			dc.MoveTo(CPoint(x*dx,y));
			// zeichne ein kleines Kästchen
			dc.FillSolidRect(CRect(x*dx-2,0,x*dx+2,y),BLUE);
			break;
		default:
			ASSERT(0);
		}
		x++;
	}
}

void CDispWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect ClientRect;
	GetClientRect(ClientRect);
	CRect DeleteRect(ClientRect);
	DeleteRect.InflateRect(-2,-2);
	dc.FillSolidRect(DeleteRect,LIGHT_GRAY);
	//////////dc.SetWindowOrg(CPoint(ClientRect.left,-ClientRect.bottom));
	//dc.SetWindowOrg(CPoint(-OFFSET_X,-ClientRect.bottom+OFFSET_Y));
	int	xs = (ClientRect.right - 2*OFFSET_X);
	int	dx = xs / 24;
	dc.SetViewportOrg(CPoint(OFFSET_X-dx,ClientRect.bottom-OFFSET_Y));
	//////////dc.SetWindowOrg(CPoint(10,10));
	CFont* pOldFont = (CFont*)dc.SelectObject(m_pFont);
	dc.SetBkColor(LIGHT_GRAY);
	//DrawAxes(dc,0,104);
	CStatisticDoc* pDoc = (CStatisticDoc*)m_pView->GetDocument();
	int StartValue = ((m_pView->m_ScrollBarResults.GetScrollPos()-1) * 25) + 1;
	//DrawAxes(dc,1,pDoc->m_pSolLayout->m_MaxPoints);
	DrawAxes(dc,StartValue,pDoc->m_pSolLayout->m_MaxPoints);
	DrawResults(dc,StartValue,pDoc->m_pSolLayout->m_MaxPoints);
	//dc.TextOut(0,0,"Hello World");
	if (pOldFont)
		dc.SelectObject(pOldFont);
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}



void CStatisticView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	if (pScrollBar == &m_ScrollBarResults)
	{
		// Get the minimum and maximum scroll-bar positions.
		int minpos;
		int maxpos;
		pScrollBar->GetScrollRange(&minpos, &maxpos); 
		maxpos = pScrollBar->GetScrollLimit();

		// Get the current position of scroll box.
		int curpos = pScrollBar->GetScrollPos();

		// Determine the new position of scroll box.
		switch (nSBCode)
		{
		case SB_LEFT:      // Scroll to far left.
			curpos = minpos;
			break;

		case SB_RIGHT:      // Scroll to far right.
			curpos = maxpos;
			break;

		case SB_ENDSCROLL:   // End scroll.
			break;

		case SB_LINELEFT:      // Scroll left.
			if (curpos > minpos)
				curpos--;
			break;

		case SB_LINERIGHT:   // Scroll right.
			if (curpos < maxpos)
				curpos++;
			break;

		case SB_PAGELEFT:    // Scroll one page left.
			{
				// Get the page size. 
				SCROLLINFO   info;
				pScrollBar->GetScrollInfo(&info, SIF_ALL);

				if (curpos > minpos)
					curpos = max(minpos, curpos - (int) info.nPage);
			}
			break;

		case SB_PAGERIGHT:      // Scroll one page right.
			{
				// Get the page size. 
				SCROLLINFO   info;
				pScrollBar->GetScrollInfo(&info, SIF_ALL);

				if (curpos < maxpos)
					curpos = min(maxpos, curpos + (int) info.nPage);
			}
			break;

		case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
			curpos = nPos;      // of the scroll box at the end of the drag operation.
			break;

		case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
			curpos = nPos;     // position that the scroll box has been dragged to.
			break;
		}

		// Set the new position of the thumb (scroll box).
		pScrollBar->SetScrollPos(curpos);
		m_pDispWnd->RedrawWindow();
	}
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CStatisticView::OnItemchangedPlayerListbox(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;

	if( pNMLV->uNewState == 3 )
	{
		m_PlayerName = m_PlayerList.GetItemText( pNMLV->iItem, 0 );
		//m_ArrIndex = m_PlayerList.GetItemData(pNMLV->iItem);
		BuildResults(m_PlayerName,m_Results);
		m_pDispWnd->m_pResultArr = &m_Results;
		m_pDispWnd->Invalidate();

	}
	else
	{
		m_PlayerName = _T("");
		//m_ArrIndex = -1;
	}
	UpdateData(FALSE);
	//TRACE("ItemChanged\n");
	*pResult = 0;
}

void CStatisticView::OnDarstellungPunkte()
{
	m_DrawMode = DRAW_POINT;
	m_pDispWnd->Invalidate();
}

void CStatisticView::OnDarstellungLinien()
{
	m_DrawMode = DRAW_LINE;
	m_pDispWnd->Invalidate();
}

void CStatisticView::OnDarstellungBalken()
{
	m_DrawMode = DRAW_BAR;
	m_pDispWnd->Invalidate();
}

void CStatisticView::OnUpdateDarstellungPunkte(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DrawMode == DRAW_POINT);
}

void CStatisticView::OnUpdateDarstellungLinien(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DrawMode == DRAW_LINE);
}

void CStatisticView::OnUpdateDarstellungBalken(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DrawMode == DRAW_BAR);
}

void CStatisticView::OnStatistikLoeschenActPlayer()
{
	// TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
	RemoveResults(m_PlayerName);
	BuildResults(m_PlayerName,m_Results);
	m_pDispWnd->Invalidate();

}

void CStatisticView::OnStatistikLoeschenAllPlayer()
{
	// TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
	int i;
	CPlayer Player;
	for (i=0;i<=GetApplication()->GetSelectDoc()->m_PlayerArr.GetUpperBound();i++)
	{
		Player = GetApplication()->GetSelectDoc()->m_PlayerArr.GetAt(i);
		RemoveResults(Player.m_NickName);
		BuildResults(Player.m_NickName,m_Results);
	}
	m_pDispWnd->Invalidate();
}
BOOL CStatisticView::RemoveResults(CString& PlayerName)
{
	BOOL Success = FALSE;
	CStatisticDoc* pDoc = (CStatisticDoc*)GetDocument();
	CSingleGameResult SingleGameResult;
	for (POSITION pos = pDoc->m_pStatistic->m_GameResultList.GetHeadPosition();pos!= NULL;)
	{
		POSITION Oldpos = pos;
		SingleGameResult = pDoc->m_pStatistic->m_GameResultList.GetNext(pos);
		if (SingleGameResult.m_PlayerName == PlayerName)
		{
			Success = TRUE;
			pDoc->m_pStatistic->m_NumberOfPlayed--;
			pDoc->m_pStatistic->m_SumOfPoints -= SingleGameResult.m_Points;
			pDoc->m_pStatistic->m_Average = pDoc->m_pStatistic->m_SumOfPoints / pDoc->m_pStatistic->m_NumberOfPlayed;
			pDoc->m_pStatistic->m_GameResultList.RemoveAt(Oldpos);
		}
	}
	return Success;
}

