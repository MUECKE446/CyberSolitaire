// PlayerDialogs.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "cybersolitaire.h"
#include "Player.h"
#include "SelectDoc.h"
#include "PlayerDialogs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSelectPlayerDialog 


CSelectPlayerDialog::CSelectPlayerDialog(CWnd* pParent /*=NULL*/, int GreetingTextID /*=IDS_EMPTY_GREETING_TEXT*/, int OkButtonTextId /*=IDS_OK_TEXT*/)
	: CDialog(CSelectPlayerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectPlayerDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	m_GreetingTextID = GreetingTextID;
	m_OkButtonTextId = OkButtonTextId;
	m_ArrIndex = -1;
}


void CSelectPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectPlayerDialog)
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	//}}AFX_DATA_MAP
	GetDlgItem(IDC_DELETE_PLAYER)->EnableWindow(m_pSelectDoc->m_PlayerArr.GetSize());
}


BEGIN_MESSAGE_MAP(CSelectPlayerDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectPlayerDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PLAYER_LIST, OnItemchangedPlayerList)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_PLAYER_LIST, OnItemclickPlayerList)
	ON_BN_CLICKED(IDC_DELETE_PLAYER, OnDeletePlayer)
	ON_BN_CLICKED(IDC_INSERT_NEW_PLAYER, OnInsertNewPlayer)
	ON_BN_CLICKED(IDC_OK_DIALOG, OnOkDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSelectPlayerDialog 

void CSelectPlayerDialog::OnItemchangedPlayerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;

	if( pNMLV->uNewState == 3 )
	{
		m_SelName = m_PlayerList.GetItemText( pNMLV->iItem, 0 );
		m_ArrIndex = m_PlayerList.GetItemData(pNMLV->iItem);
	}
	else
	{
		m_SelName = _T("");
		m_ArrIndex = -1;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CSelectPlayerDialog::OnItemclickPlayerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	
	*pResult = 0;
}

BOOL CSelectPlayerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CenterWindow();
	/*
	CRect rec_exp, rec_org;
	GetWindowRect( &rec_org );
	CWnd *stat = GetDlgItem( IDC_EXPAND );
	stat->GetWindowRect( &rec_exp );
	if( m_isExpanded == FALSE )
	{
		rec_org.bottom -= rec_exp.Height() + 20;
		MoveWindow(rec_org);
	}
	*/
	CWnd* pGreetingWnd = this->GetDlgItem( IDC_GREETING_TEXT );
	pGreetingWnd->GetDC()->SetTextColor( RGB(255,0,0) );
	CFont* pFont = pGreetingWnd->GetFont();
	LOGFONT font;
	pFont->GetLogFont( &font );
	font.lfWeight = FW_EXTRABOLD;
	font.lfHeight = 18;
	m_pNewFont = new CFont;
	m_pNewFont->CreateFontIndirect( &font );
	pGreetingWnd->SetFont( m_pNewFont );

	DWORD Style = m_PlayerList.GetExtendedStyle();
	Style |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	//Style |= LVS_EX_CHECKBOXES;
	m_PlayerList.SetExtendedStyle(Style);
	m_PlayerList.SetBkColor(RGB(255, 255, 210));
	m_PlayerList.SetTextBkColor(RGB(255, 255, 210));
	
	CString Text;
	Text.LoadString(m_GreetingTextID);
	pGreetingWnd->SetWindowText(Text);
	pGreetingWnd->RedrawWindow();

	Text.LoadString(m_OkButtonTextId);
	GetDlgItem(IDC_OK_DIALOG)->SetWindowText(Text);
	GetDlgItem(IDC_OK_DIALOG)->RedrawWindow();

	CString MemoName = m_SelName;

	//--------- fill list
	FillPlayerList(TRUE);

	// für Test
	// so wird selektiert
	//m_PlayerList.SetItemState(2, LVIS_SELECTED, LVIS_SELECTED);
	LVFINDINFO info;
	int nIndex;

	//info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.flags = LVFI_STRING;
	info.psz = MemoName;

	if ((nIndex = m_PlayerList.FindItem(&info)) != -1)
	{
		m_PlayerList.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
		m_SelName = MemoName;
		m_ArrIndex = m_PlayerList.GetItemData(nIndex);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CSelectPlayerDialog::OnDeletePlayer() 
{
	CString TmpStr;
	DWORD	i,index;
	UpdateData();

	POSITION pos = m_PlayerList.GetFirstSelectedItemPosition();
	if(  pos == NULL )						// Das war wohl nichts
	{
		AfxMessageBox(IDS_CHOOSE_PLAYER, MB_OK | MB_ICONEXCLAMATION );
		return;
	}

	TmpStr.Format(IDS_DO_YOU_WANT_DELETE_PLAYER, m_SelName);
	if(AfxMessageBox(TmpStr, MB_YESNO | MB_ICONQUESTION) == IDYES )
	{	
		index = m_PlayerList.GetNextSelectedItem(pos);
		// dies ist der Arrayindex
		i = m_PlayerList.GetItemData(index);
		m_pSelectDoc->m_PlayerArr.RemoveAt(i);
		FillPlayerList(FALSE);
		UpdateData(FALSE);
	}
}

void CSelectPlayerDialog::OnInsertNewPlayer() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	CInsertNewPlayerDialog InsertNewPlayerDlg;
	if (InsertNewPlayerDlg.DoModal() == IDOK)
	{
		// zu_tun
		// Überprüfung auf doppelte Namen
		CPlayer Player;
		Player.m_NickName = InsertNewPlayerDlg.m_NewPlayerNickName;;
		Player.m_Rank = "Rookie";
		m_pSelectDoc->m_PlayerArr.Add(Player);
		FillPlayerList(FALSE);
	}
}

void CSelectPlayerDialog::OnOkDialog() 
{
	UpdateData();
	if (m_SelName.IsEmpty())
	{
		AfxMessageBox(IDS_CHOOSE_PLAYER, MB_OK | MB_ICONEXCLAMATION );
		return;
	}

	CDialog::OnOK();
}

void CSelectPlayerDialog::PostNcDestroy() 
{
	delete m_pNewFont;
	CDialog::PostNcDestroy();
}

BOOL CSelectPlayerDialog::FillPlayerList(BOOL WithTitleBar)
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
		
		column.cx		= 110;
		title.LoadString(IDS_TITLE_PLAYER_RANK);
		column.pszText	= (LPSTR)(LPCSTR)title;
		column.iSubItem = 1;
		m_PlayerList.InsertColumn(1, &column);
		
	}
	m_PlayerList.DeleteAllItems();
	//--------------------- fill list with information
	int i;
	CPlayer Player;
	for (i=0;i<=m_pSelectDoc->m_PlayerArr.GetUpperBound();i++)
	{
		Player = m_pSelectDoc->m_PlayerArr.GetAt(i);
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
		//afxDump << "m_PlayerArr: " << &m_pSelectDoc->m_PlayerArr << "\n";
	#endif
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertNewPlayerDialog 


CInsertNewPlayerDialog::CInsertNewPlayerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertNewPlayerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertNewPlayerDialog)
	m_NewPlayerNickName = _T("");
	//}}AFX_DATA_INIT
}


void CInsertNewPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertNewPlayerDialog)
	DDX_Text(pDX, IDC_EDIT_NEW_PLAYER, m_NewPlayerNickName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertNewPlayerDialog, CDialog)
	//{{AFX_MSG_MAP(CInsertNewPlayerDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CInsertNewPlayerDialog 

void CInsertNewPlayerDialog::OnOK() 
{
	// TODO: Zusätzliche Prüfung hier einfügen
	UpdateData();
	if (m_NewPlayerNickName.IsEmpty())
	{
		AfxMessageBox(IDS_STRING_IS_EMPTY,MB_OK || MB_ICONWARNING);
		return;
	}
	CDialog::OnOK();
}
