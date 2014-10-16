// PropertiesDialogs.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "cybersolitaire.h"
#include "SolEditDoc.h"
#include "PropertiesDialogs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CGamePropertiesDialog 


CGamePropertiesDialog::CGamePropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGamePropertiesDialog::IDD, pParent)
	, m_MaxPoints(0)
{
	//{{AFX_DATA_INIT(CGamePropertiesDialog)
	m_GroupType = _T("");
	m_GameName = _T("");
	m_NumberOfDecks = 2;
	m_strTypeOfScoring = _T("");
	//}}AFX_DATA_INIT
}


void CGamePropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGamePropertiesDialog)
	DDX_Control(pDX, IDC_SCORING, m_TypeOfScoringCombo);
	DDX_Control(pDX, IDC_GAME_GROUP, m_GroupTypeCombo);
	DDX_CBString(pDX, IDC_GAME_GROUP, m_GroupType);
	DDX_Text(pDX, IDC_GAME_NAME, m_GameName);
	DDX_Text(pDX, IDC_NUMBER_OF_DECKS, m_NumberOfDecks);
	DDV_MinMaxInt(pDX, m_NumberOfDecks, 1, 2);
	DDX_CBString(pDX, IDC_SCORING, m_strTypeOfScoring);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MAX_POINTS, m_MaxPoints);
	DDV_MinMaxInt(pDX, m_MaxPoints, 0, 104);
}


BEGIN_MESSAGE_MAP(CGamePropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(CGamePropertiesDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CGamePropertiesDialog 

BOOL CGamePropertiesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CenterWindow();
	ASSERT(m_pDoc);
	FillGroupTypeCombo(IDS_GROUP_NOTAVAILABLE);
	FillTypeOfScoringCombo(IDS_SCORE_NOTAVAILABLE);
	//GetDlgItem(IDC_NUMBER_OF_DECKS)->EnableWindow(!m_NumberOfDecks);
	if (m_MaxPoints == 0)
	{
		m_MaxPoints = m_NumberOfDecks * 52;
	}
	UpdateData(FALSE);
	// TODO: Zusätzliche Initialisierung hier einfügen
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CGamePropertiesDialog::FillGroupTypeCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_END_OF_GROUPS; i++)
	{
		CString GroupType;
		GroupType.LoadString(i);
		m_GroupTypeCombo.AddString(GroupType);
	}
}

void CGamePropertiesDialog::FillTypeOfScoringCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_END_OF_SCORING; i++)
	{
		CString strTypeOfScoring;
		strTypeOfScoring.LoadString(i);
		m_TypeOfScoringCombo.AddString(strTypeOfScoring);
	}
}

void CGamePropertiesDialog::OnOK() 
{
	// TODO: Zusätzliche Prüfung hier einfügen
	UpdateData();
	if (!m_NumberOfDecks)
	{
		AfxMessageBox(IDS_NUMBEROFDECKS_MUST_BE_GREATER_0);
		return;
	}
	m_pDoc->m_pLayout->m_GameName = m_GameName;
	m_pDoc->m_pLayout->m_GroupType = m_GroupType;
	m_pDoc->m_pLayout->m_GroupTypeID = m_GroupTypeCombo.FindStringExact(-1,m_GroupType);
	m_pDoc->m_pLayout->m_NumberOfDecks = m_NumberOfDecks;
	m_pDoc->m_pLayout->m_TypeOfScoring = (gameScoring)m_TypeOfScoringCombo.FindStringExact(-1,m_strTypeOfScoring);
	m_pDoc->m_pLayout->m_MaxPoints = m_MaxPoints;
	CDialog::OnOK();
}

void CGamePropertiesDialog::OnCancel() 
{
	UpdateData();
	if (!m_NumberOfDecks)
	{
		AfxMessageBox(IDS_NUMBEROFDECKS_MUST_BE_GREATER_0);
		return;
	}
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPilePropertiesDialog 


CPilePropertiesDialog::CPilePropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPilePropertiesDialog::IDD, pParent)
	, m_UseForAllPilesOfThisType(TRUE)
	, m_NumberHiddenOnStart(-1)
	, m_PermittedPlay(_T(""))
	, m_cx_MaxSize(_T(""))
	, m_cy_MaxSize(_T(""))
	, m_SpecialHandlingText(_T(""))
	, m_SpecialHandling(FALSE)
	, m_UpdateOverlap(TRUE)
	, m_BasicCardOnlyColor(FALSE)
{
	//{{AFX_DATA_INIT(CPilePropertiesDialog)
	m_xPosition = _T("");
	m_yPosition = _T("");
	m_FaceAtStart = _T("");
	m_OverlappMethod = _T("");
	m_PilingMethod = _T("");
	m_PutIfEmpty = _T("");
	m_SelectMethod = _T("");
	m_RedealMethod = _T("");
	m_MaxCards = 0;
	m_NumberOnStart = 0;
	m_NumberPerMove = 0;
	m_DealTo = _T("");
	m_NumberPerDeal = 0;
	m_Replayable = FALSE;
	m_BasicCardOnlyVal = FALSE;
	m_DealOrder = 0;
	m_RedealOrder = 0;
	m_NumberOfRedeal = 0;
	//}}AFX_DATA_INIT
}


void CPilePropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPilePropertiesDialog)
	DDX_Control(pDX, IDC_BASICCARD_METHOD, m_BasicCardCombo);
	DDX_Control(pDX, IDC_REDEAL_METHOD, m_RedealMethodCombo);
	DDX_Control(pDX, IDC_SPIN_BASICCARD, m_SpinBasicCardCtrl);
	DDX_Control(pDX, IDC_BMP_BASICCARD, m_BmpBasicCardCtrl);
	DDX_Control(pDX, IDC_DEAL_TO, m_DealToCombo);
	DDX_Control(pDX, IDC_SELECT_METHOD, m_SelectMethodCombo);
	DDX_Control(pDX, IDC_PUT_IF_EMPTY, m_PutIfEmptyCombo);
	DDX_Control(pDX, IDC_PILING_METHOD, m_PilingMethodCombo);
	DDX_Control(pDX, IDC_OVERLAPP_METHOD, m_OverlappMethodCombo);
	DDX_Control(pDX, IDC_FACE_AT_START, m_FaceAtStartCombo);
	DDX_Control(pDX, IDC_BMP_PILE_EMPTY, m_BmpEmptyPileCtrl);
	DDX_Control(pDX, IDC_SPIN_EMPTY_PILE, m_SpinEmptyCtrl);
	DDX_Text(pDX, IDC_X_POSITION, m_xPosition);
	DDX_Text(pDX, IDC_Y_POSITION, m_yPosition);
	DDX_CBString(pDX, IDC_FACE_AT_START, m_FaceAtStart);
	DDX_CBString(pDX, IDC_OVERLAPP_METHOD, m_OverlappMethod);
	DDX_CBString(pDX, IDC_PILING_METHOD, m_PilingMethod);
	DDX_CBString(pDX, IDC_PUT_IF_EMPTY, m_PutIfEmpty);
	DDX_CBString(pDX, IDC_SELECT_METHOD, m_SelectMethod);
	DDX_CBString(pDX, IDC_REDEAL_METHOD, m_RedealMethod);
	DDX_CBString(pDX, IDC_BASICCARD_METHOD, m_BasicCardMethod);
	DDX_Text(pDX, IDC_MAX_CARDS, m_MaxCards);
	DDX_Text(pDX, IDC_NUMBER_AT_START, m_NumberOnStart);
	DDX_Text(pDX, IDC_NUMBER_PER_MOVE, m_NumberPerMove);
	DDX_CBString(pDX, IDC_DEAL_TO, m_DealTo);
	DDX_Text(pDX, IDC_NUMBER_PER_DEAL, m_NumberPerDeal);
	DDX_Check(pDX, IDC_CHECK_REPLAYABLE, m_Replayable);
	DDX_Check(pDX, IDC_CHECK_BASICCARD_ONLY_VAL, m_BasicCardOnlyVal);
	DDX_Text(pDX, IDC_EDIT_DEALORDER, m_DealOrder);
	DDV_MinMaxUInt(pDX, m_DealOrder, 0, 10);
	DDX_Text(pDX, IDC_EDIT_REDEALORDER, m_RedealOrder);
	DDV_MinMaxUInt(pDX, m_RedealOrder, 0, 24);
	DDX_Text(pDX, IDC_EDIT_NUMBER_OF_REDEAL, m_NumberOfRedeal);
	DDV_MinMaxInt(pDX, m_NumberOfRedeal, -1, 3);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_USE_FOR_ALL_PILES, m_UseForAllPilesOfThisType);
	DDX_Text(pDX, IDC_NUMBER_HIDDEN_AT_START, m_NumberHiddenOnStart);
	DDX_Control(pDX, IDC_PERMITTED_PLAY, m_PermittedPlayCombo);
	DDX_CBString(pDX, IDC_PERMITTED_PLAY, m_PermittedPlay);
	DDX_Text(pDX, IDC_CX_MAXSIZE, m_cx_MaxSize);
	DDX_Text(pDX, IDC_CY_MAXSIZE, m_cy_MaxSize);
	DDX_Text(pDX, IDC_EDIT_SPECIAL_HANDLING, m_SpecialHandlingText);
	DDX_Check(pDX, IDC_CHECK_SPECIAL_HANDLING, m_SpecialHandling);
	DDX_Check(pDX, IDC_UPDATE_OVERLAP, m_UpdateOverlap);
	DDX_Check(pDX, IDC_CHECK_BASICCARD_ONLY_COLOR, m_BasicCardOnlyColor);
}


BEGIN_MESSAGE_MAP(CPilePropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(CPilePropertiesDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EMPTY_PILE, OnDeltaposSpinEmptyPile)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BASICCARD, OnDeltaposSpinBasiccard)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_BASICCARD_ONLY_COLOR, OnBnClickedCheckBasiccardOnlyColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPilePropertiesDialog 

BOOL CPilePropertiesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// zu_tun
	//ASSERT(m_pDoc);
	ASSERT(m_pPile);
	
	m_SpinEmptyCtrl.SetRange(IDB_EMPTY_START,IDB_EMPTY_END);
	m_SpinEmptyCtrl.SetPos(m_IDDibEmpty);
	m_SpinBasicCardCtrl.SetRange(IDB_START_CARD,IDB_END_CARD);
	m_SpinBasicCardCtrl.SetPos(m_IDDibBasicCard);
	m_xPosition.Format("%d",m_PilePosition.x);
	m_yPosition.Format("%d",m_PilePosition.y);
	m_cx_MaxSize.Format("%d",m_PileMaxSize.cx);
	m_cy_MaxSize.Format("%d",m_PileMaxSize.cy);
	FillFaceAtStartCombo(IDS_FAS_NOTAVAILABLE);   
	FillOverlappMethodCombo(IDS_OLM_NOTAVAILABLE);
	FillPilingMethodCombo(IDS_PIL_NOTAVAILABLE);  
	FillPutIfEmptyCombo(IDS_PIE_NOTAVAILABLE);    
	FillSelectMethodCombo(IDS_SEL_NOTAVAILABLE);  
	FillDealToCombo(IDS_DEAL_NOTAVAILABLE);
	FillPermittedPlayCombo(IDS_PPP_NOTAVAILABLE);
	m_BmpEmptyPile.LoadBitmap(m_IDDibEmpty);
	m_BmpEmptyPileCtrl.SetBitmap((HBITMAP)m_BmpEmptyPile);	
	m_BmpBasicCard.LoadBitmap(m_IDDibBasicCard);
	m_BmpBasicCardCtrl.SetBitmap((HBITMAP)m_BmpBasicCard);	

	// zunächst einmal alle Zusätze verbergen
	// CStock
	GetDlgItem(IDC_STOCK_SPECIFIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_NUMBER_OF_REDEAL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_NUMBER_OF_REDEAL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_REDEAL_METHOD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_REDEAL_METHOD)->ShowWindow(SW_HIDE);

	// CFoundation
	GetDlgItem(IDC_FOUNDATION_SPECIFIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_REPLAYABLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_REPLAYABLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_BASICCARD_ONLY_VAL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_BASICCARD_ONLY_COLOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_BASICCARD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_KIND_OF_BASIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BASICCARD_METHOD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SPIN_BASICCARD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BMP_BASICCARD)->ShowWindow(SW_HIDE);

	if (m_pPile->IsK(CStock))
	{
		FillRedealMethodCombo(IDS_REDEAL_NOTAVAILABLE);  
		// entsprechende Zusätze einblenden
		GetDlgItem(IDC_STOCK_SPECIFIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NUMBER_OF_REDEAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_NUMBER_OF_REDEAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_REDEAL_METHOD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_REDEAL_METHOD)->ShowWindow(SW_SHOW);
	}
	if (m_pPile->IsK(CFoundation))
	{
		FillBasicCardCombo(IDS_BAS_NOTAVAILABLE);  
		// entsprechende Zusätze einblenden
		GetDlgItem(IDC_FOUNDATION_SPECIFIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_REPLAYABLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_BASICCARD_ONLY_VAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_BASICCARD_ONLY_COLOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BASICCARD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_KIND_OF_BASIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BASICCARD_METHOD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPIN_BASICCARD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BMP_BASICCARD)->ShowWindow(SW_SHOW);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CPilePropertiesDialog::OnDeltaposSpinEmptyPile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_IDDibEmpty = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (m_IDDibEmpty < IDB_EMPTY_START)
		m_IDDibEmpty = IDB_EMPTY_END;
	if (m_IDDibEmpty > IDB_EMPTY_END)
		m_IDDibEmpty = IDB_EMPTY_START;
	m_BmpEmptyPile.Detach();
	m_BmpEmptyPile.LoadBitmap(m_IDDibEmpty);
	m_BmpEmptyPileCtrl.SetBitmap((HBITMAP)m_BmpEmptyPile);	
	*pResult = 0;
}

void CPilePropertiesDialog::FillFaceAtStartCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_FAS_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_FaceAtStartCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillOverlappMethodCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_OLM_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_OverlappMethodCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillPilingMethodCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_PIL_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_PilingMethodCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillPutIfEmptyCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_PIE_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_PutIfEmptyCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillSelectMethodCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_SEL_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_SelectMethodCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillDealToCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_DEAL_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_DealToCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillBasicCardCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_BAS_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_BasicCardCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillRedealMethodCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_REDEAL_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_RedealMethodCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::FillPermittedPlayCombo(int FirstID)
{
	for (int i=FirstID; i<IDS_PPP_END; i++)
	{
		CString TmpStr;
		TmpStr.LoadString(i);
		m_PermittedPlayCombo.AddString(TmpStr);
	}
}

void CPilePropertiesDialog::OnOK() 
{
	// TODO: Zusätzliche Prüfung hier einfügen
	UpdateData();
	//CPoint Position(atoi(m_xPosition),atoi(m_yPosition));
	// zu_tun
	// ist das ok ?
	//m_pPile->SetPosition(m_PilePosition);
	//m_pPile->m_Size			= 
	m_pPile->m_IDDibEmpty			= m_IDDibEmpty;
	m_pPile->m_KindOfOverlapped		= (pileOverlappedMethod)m_OverlappMethodCombo.FindStringExact(-1,m_OverlappMethod);
	m_pPile->m_KindOfSelecting		= (pileSelectingMethod)m_SelectMethodCombo.FindStringExact(-1,m_SelectMethod);
	m_pPile->m_KindOfPiling			= (pilePiling)m_PilingMethodCombo.FindStringExact(-1,m_PilingMethod);
	m_pPile->m_FaceAtStart			= (pileFaceAtStart)m_FaceAtStartCombo.FindStringExact(-1,m_FaceAtStart);
	m_pPile->m_NumberOnStart		= m_NumberOnStart;
	m_pPile->m_NumberHiddenOnStart	= m_NumberHiddenOnStart;
	m_pPile->m_NumberPerMove		= m_NumberPerMove;
	m_pPile->m_PutIfEmpty			= (pilePutIfEmpty)m_PutIfEmptyCombo.FindStringExact(-1,m_PutIfEmpty);
	m_pPile->m_MaxCards				= m_MaxCards;
	m_pPile->m_DealOrder			= m_DealOrder;
	m_pPile->m_RedealOrder			= m_RedealOrder;
	m_pPile->m_DealTo				= (pileDealing)m_DealToCombo.FindStringExact(-1,m_DealTo);
	m_pPile->m_NumberPerDeal		= m_NumberPerDeal;
	m_pPile->m_PermittedPlay		= (pilePermittedPlay)m_PermittedPlayCombo.FindStringExact(-1,m_PermittedPlay);
	m_pPile->m_UpdateOverlap		= m_UpdateOverlap;
	m_pPile->m_SpecialHandling		= m_SpecialHandling;
	m_pPile->m_SpecialHandlingText	= m_SpecialHandlingText;

	if (m_pPile->IsK(CStock))
	{
		((CStock*)m_pPile)->m_NumberOfRedeal = m_NumberOfRedeal;
		((CStock*)m_pPile)->m_RedealMethod  = (pileRedealMethod)m_RedealMethodCombo.FindStringExact(-1,m_RedealMethod);
	}

	if (m_pPile->IsK(CFoundation))
	{
		((CFoundation*)m_pPile)->m_Replayable = m_Replayable;
		((CFoundation*)m_pPile)->m_BasicCardOnlyVal = m_BasicCardOnlyVal;
		((CFoundation*)m_pPile)->m_BasicCardOnlyColor = m_BasicCardOnlyColor;
		if (m_IDDibBasicCard == IDB_EMPTY_CARD)
			((CFoundation*)m_pPile)->m_BasicCardValue = 0;
		else
			((CFoundation*)m_pPile)->m_BasicCardValue = m_IDDibBasicCard -IDB_START_CARD + 1;
		((CFoundation*)m_pPile)->m_KindOfBasicCard = (pileBasicCard)m_BasicCardCombo.FindStringExact(-1,m_BasicCardMethod);
	}

	CDialog::OnOK();
}

void CPilePropertiesDialog::OnCancel() 
{
	// TODO: Zusätzlichen Bereinigungscode hier einfügen
	
	CDialog::OnCancel();
}

void CPilePropertiesDialog::OnDeltaposSpinBasiccard(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_IDDibBasicCard = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (m_IDDibBasicCard < IDB_START_CARD)
		m_IDDibBasicCard = IDB_END_CARD;
	if (m_IDDibBasicCard > IDB_END_CARD)
		m_IDDibBasicCard = IDB_START_CARD;
	m_BmpBasicCard.Detach();
	m_BmpBasicCard.LoadBitmap(m_IDDibBasicCard);
	m_BmpBasicCardCtrl.SetBitmap((HBITMAP)m_BmpBasicCard);	
	*pResult = 0;
}


void CPilePropertiesDialog::OnBnClickedCheckBasiccardOnlyColor()
{
	// setzt automatisch auch nur Kartenwert
	UpdateData();
	if (m_BasicCardOnlyColor)
	{
		m_BasicCardOnlyVal = TRUE;
		UpdateData(FALSE);
	}
}
