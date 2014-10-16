// SolEditDoc.cpp : Implementierung der Klasse CSolEditDoc
//

#include "stdafx.h"
#include "CyberSolitaire.h"

#include "SelectDoc.h"
#include "PropertiesDialogs.h"
#include "SolEditDoc.h"
#include ".\soleditdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSolEditDoc

IMPLEMENT_DYNCREATE(CSolEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CSolEditDoc, CDocument)
	//{{AFX_MSG_MAP(CSolEditDoc)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_Menu300, OnSortierenNachRedealOrder)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSolEditDoc Konstruktion/Destruktion

CSolEditDoc::CSolEditDoc()
{
	m_pView = NULL;	// wird in OnInitialUpdate im View gesetzt
	m_pLayout = GetSelectDoc()->GetWorkingLayout();
}

CSolEditDoc::~CSolEditDoc()
{
	if (m_pLayout)
		delete m_pLayout;
	GetSelectDoc()->SetWorkingLayout(NULL);
}

BOOL CSolEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	InitNewGame();
	int Index = GetSelectDoc()->FindGame(m_pLayout->m_GameName);
	if (Index != -1)
	{
		AfxMessageBox(IDS_GAME_EXIST);
		SetModifiedFlag(TRUE);
	}
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSolEditDoc Serialisierung

void CSolEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Hier Code zum Speichern einfügen
	}
	else
	{
		// ZU ERLEDIGEN: Hier Code zum Laden einfügen
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSolEditDoc Diagnose

#ifdef _DEBUG
void CSolEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSolEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSolEditDoc Befehle

CSolEditView* CSolEditDoc::GetView()
{
	return m_pView;
}

void CSolEditDoc::UpdatePiles()
{
	CPile* pPile;
	if (!m_pLayout->m_PileList.IsEmpty())
	{
		POSITION pos = m_pLayout->m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = m_pLayout->m_PileList.GetNext(pos);
			// Geometrie der Karten und des Piles festlegen
			pPile->UpdateCardsPositions();
		}
	}
}

CStock* CSolEditDoc::GetStock()
{
	CPile* pPile = NULL;
	if (!m_pLayout->m_PileList.IsEmpty())
	{
		pPile = m_pLayout->m_PileList.GetHead();
		if (pPile->IsK(CStock))
			return (CStock*)pPile;
	}
	return NULL;
}

void CSolEditDoc::InitStockPile(CStock *pPile)
{
	ASSERT(pPile->IsK(CStock));
	pPile->m_MaxCards =	 1;
	pPile->m_KindOfOverlapped = olmNotOverlapped;
	pPile->m_KindOfPiling = pilNotAvailable;
	pPile->m_KindOfSelecting = selAll;
	pPile->m_FaceAtStart = fasAllFaceDown;
	pPile->m_NumberOnStart = 1;
	pPile->m_NumberHiddenOnStart = -1;
	pPile->m_IDDibEmpty = IDB_STOCK1;
	pPile->m_dibEmpty.Load(AfxGetApp()->m_hInstance,pPile->m_IDDibEmpty);
	pPile->m_DealTo = dealTableaus;
	pPile->m_NumberPerDeal = 1;
}

void CSolEditDoc::InitTableauPile(CTableau *pPile)
{
	ASSERT(pPile->IsK(CTableau));
	pPile->m_MaxCards =	 1;
	pPile->m_KindOfOverlapped = olmNotOverlapped;
	pPile->m_KindOfPiling = pilNotAvailable;
	pPile->m_KindOfSelecting = selAll;
	pPile->m_FaceAtStart = fasAllFaceDown;
	pPile->m_NumberOnStart = 1;
	pPile->m_NumberHiddenOnStart = -1;
	pPile->m_IDDibEmpty = IDB_TABLEAU1;
	pPile->m_dibEmpty.Load(AfxGetApp()->m_hInstance,pPile->m_IDDibEmpty);
	pPile->m_DealOrder = 1;
	pPile->m_RedealOrder = 0;
	pPile->m_DealTo = dealNotAvailable;
	pPile->m_NumberPerDeal = 0;
}

void CSolEditDoc::InitFoundationPile(CFoundation *pPile)
{
	ASSERT(pPile->IsK(CFoundation));
	pPile->m_MaxCards =	 1;
	pPile->m_KindOfOverlapped = olmNotOverlapped;
	pPile->m_KindOfPiling = pilNotAvailable;
	pPile->m_KindOfSelecting = selAll;
	pPile->m_FaceAtStart = fasAllFaceDown;
	pPile->m_NumberOnStart = 1;
	pPile->m_NumberHiddenOnStart = -1;
	pPile->m_IDDibEmpty = IDB_FOUNDATION1;
	pPile->m_dibEmpty.Load(AfxGetApp()->m_hInstance,pPile->m_IDDibEmpty);
	pPile->m_DealOrder = 0;
	pPile->m_RedealOrder = 0;
	pPile->m_DealTo = dealNotAvailable;
	pPile->m_NumberPerDeal = 0;
}

void CSolEditDoc::InitWastePile(CWaste *pPile)
{
	ASSERT(pPile->IsK(CWaste));
	pPile->m_MaxCards =	 1;
	pPile->m_KindOfOverlapped = olmNotOverlapped;
	pPile->m_KindOfPiling = pilNotAvailable;
	pPile->m_KindOfSelecting = selAll;
	pPile->m_FaceAtStart = fasAllFaceDown;
	pPile->m_NumberOnStart = 1;
	pPile->m_NumberHiddenOnStart = -1;
	pPile->m_IDDibEmpty = IDB_WASTE1;
	pPile->m_dibEmpty.Load(AfxGetApp()->m_hInstance,pPile->m_IDDibEmpty);
	pPile->m_DealOrder = 0;
	pPile->m_RedealOrder = 0;
	pPile->m_DealTo = dealNotAvailable;
	pPile->m_NumberPerDeal = 0;
}


void CSolEditDoc::InitNewGame(BOOL WithDialog /* = TRUE */)
{
	ASSERT(m_pLayout);
	m_pLayout->m_GameName.LoadString(IDS_NO_GAME_NAME);
	m_pLayout->m_GroupType.LoadString(IDS_GROUP_NOTAVAILABLE);
	m_pLayout->m_GroupTypeID = 0;
	m_pLayout->m_NumberOfDecks = 2;
	m_pLayout->m_TypeOfScoring = scoreNotAvailable;
	if (WithDialog)
	{
		CGamePropertiesDialog	GamePropertiesDlg((CWnd*)GetView());
		GamePropertiesDlg.m_GameName		= m_pLayout->m_GameName;
		GamePropertiesDlg.m_GroupType		= m_pLayout->m_GroupType;
		GamePropertiesDlg.m_NumberOfDecks	= m_pLayout->m_NumberOfDecks;
		GamePropertiesDlg.m_strTypeOfScoring.LoadString(IDS_SCORE_NOTAVAILABLE);
		GamePropertiesDlg.m_MaxPoints		= m_pLayout->m_MaxPoints;
		GamePropertiesDlg.m_pDoc = this;
		if (GamePropertiesDlg.DoModal() == IDOK)
		{
			// Werte wurden übernommen
			ASSERT (m_pLayout->m_NumberOfDecks > 0 && m_pLayout->m_NumberOfDecks < 3);
			//CreateCards(m_NumberOfDecks);
		}
		SetModifiedFlag();
	}
}

BOOL CSolEditDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CString GameName = lpszPathName;
	int Index = GetSelectDoc()->FindGame(GameName);
	if (Index == -1)
	{
		AfxMessageBox(IDS_GAME_NOT_FOUND);
		return FALSE;
	}
	return TRUE;
}

CSelectDoc* CSolEditDoc::GetSelectDoc()
{
	POSITION pos = GetApplication()->pSelectSolitaireTemplate->GetFirstDocPosition();
	// wir können davon ausgehen, daß es ein oder kein CProjDoc gibt
	CSelectDoc* pSelectDoc = NULL;
	if (pos)
		pSelectDoc = (CSelectDoc*)GetApplication()->pSelectSolitaireTemplate->GetNextDoc(pos);
	return pSelectDoc;
}

BOOL CSolEditDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	int Index = -1;
	Index = GetSelectDoc()->FindGame(m_pLayout->m_GameName);
	if (Index != -1)
	{
		//AfxMessageBox(IDS_GAME_EXIST);
		GetSelectDoc()->m_SolLayoutArr.RemoveAt(Index);
	}
	// die Piles des Spiels ordnen
	m_pLayout->SortPiles();
	// Spiel wird zur Liste hinzugefügt	und gespeichert
	GetSelectDoc()->m_SolLayoutArr.Add(*m_pLayout);
	CString PathName = GetApplication()->m_FullFilePath;
	PathName += "GAMES.DAT";
	// das Sortieren des Tree ist viel einfacher
	//GetSelectDoc()->m_SolLayoutArr.SortAlphabetical();
	GetSelectDoc()->SaveSolLayoutArray(PathName);
	GetSelectDoc()->FillGameList();
	PathName = GetApplication()->m_FullFilePath;
	PathName += "STATISTICS.DAT";
	if (GetSelectDoc()->CompleteMapGameNameToStatistic())
		GetSelectDoc()->SaveMapGameNameToStatistic(PathName);
	GetSelectDoc()->UpdateGameTree();
	return TRUE;
}

BOOL CSolEditDoc::SaveModified() 
{
	if (!IsModified())
		return TRUE;        // ok to continue

	// get name/title of document
	CString name;
	name = m_pLayout->m_GameName;
	/*
	if (m_strPathName.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
		if (name.IsEmpty())
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
	}
	else
	{
		// get name based on file title of path name
		//name = m_strPathName;
		name = m_pLayout->m_GameName;
	}
	*/

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		OnSaveDocument(name);
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	SetModifiedFlag(FALSE);
	if (m_pLayout) delete m_pLayout;
	m_pLayout = NULL;
	return TRUE;    // keep going
}

CString CSolEditDoc::GetDateTimeVersion()
{
	CString DateTimeVersion;
	COleDateTime Time = COleDateTime::GetCurrentTime();
	DateTimeVersion = Time.Format("%d$%m$%y_%H$%M$%S");
	return DateTimeVersion;
}

void CSolEditDoc::OnCloseDocument()
{
	SaveModified();
	CDocument::OnCloseDocument();
}

void CSolEditDoc::OnSortierenNachRedealOrder()
{
	CPileArray RedealPileArr;
	CPile* pPile = NULL;
	CPile* pNewPile = NULL;
	POSITION pos1 = NULL;
	POSITION pos2 = NULL;
	pos1 = m_pLayout->m_PileList.GetHeadPosition();
	while (pos1)
	{
		pos2 = pos1;
		pPile = m_pLayout->m_PileList.GetNext(pos1);
		if (pPile->m_RedealOrder != 0)
		{
			RedealPileArr.SetAtGrow(pPile->m_RedealOrder-1,pPile);
			// jetzt löschen wir den Pile in der Pile Liste
			m_pLayout->m_PileList.RemoveAt(pos2);
		}
	}
	// hier haben wir ein Array in der richtigen Reihenfolge
	// das hängen wir so an die Pile Liste an
	for (int i=0; i < RedealPileArr.GetSize(); i++)
	{
		m_pLayout->m_PileList.AddTail(RedealPileArr.GetAt(i));
	}
	SetModifiedFlag(TRUE);
}

void CSolEditDoc::OnFileClose()
{
	TRACE("Entry CSolEditDoc::OnFileClose\n");
	if (!SaveModified())
		return;

	// shut it down
	OnCloseDocument();
	// this should destroy the document
	TRACE("Exit  CSolEditDoc::OnFileClose\n");
}
