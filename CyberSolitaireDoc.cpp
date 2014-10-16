// CyberSolitaireDoc.cpp : Implementierung der Klasse CCyberSolitaireDoc
//

#include "stdafx.h"
#include <afxwin.h>
#include "CyberSolitaire.h"

#include "SelectDoc.h"
#include "TTTField.h"
#include "TTTDoc.h"
#include "TTTView.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "MainFrm.h"
#include "Do.h"
#include "Statistic.h"
#include "TTTGameFinishedDialog.h"
#include ".\cybersolitairedoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireDoc

IMPLEMENT_DYNCREATE(CCyberSolitaireDoc, CDocument)

BEGIN_MESSAGE_MAP(CCyberSolitaireDoc, CDocument)
	//{{AFX_MSG_MAP(CCyberSolitaireDoc)
	ON_COMMAND(ID_BUTTON_UNDO, OnButtonUndo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_UNDO, OnUpdateButtonUndo)
	ON_COMMAND(ID_BUTTON_REDO, OnButtonRedo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_REDO, OnUpdateButtonRedo)
	ON_COMMAND(ID_SPIEL_NEU, OnSpielNeu)
	ON_COMMAND(ID_SPIEL_NOCHMAL, OnSpielNochmal)
	ON_COMMAND(ID_EDIT_UNDO, OnButtonUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateButtonUndo)
	ON_COMMAND(ID_EDIT_REDO, OnButtonRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateButtonRedo)
	ON_COMMAND(ID_SPIEL_ABBRECHEN, OnSpielAbbrechen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_GAME_EXIT, OnGameExit)
	ON_COMMAND(ID_SPIEL_BACK_TO_TTT_WINDOW, OnSpielBackToTttWindow)
	ON_COMMAND(ID_SPIEL_SPEICHERN, OnTTTSpielSpeichern)
	ON_COMMAND(ID_SPIEL_SPEICHERNUNTER, OnTTTSpielSpeichernUnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireDoc Konstruktion/Destruktion

CCyberSolitaireDoc::CCyberSolitaireDoc()
: m_BasicCardValue(0)
, m_InAutoPlay(FALSE)
, m_pTTTDoc(NULL)
, m_ExitGameCause(0)
{
	m_pView = NULL;	// wird in OnInitialUpdate im View gesetzt
	m_pMovePile = NULL;
	m_BasicCardValue = 0;
	InitMovePile();
	m_CountOfRedeals = 0;
	m_WinsGame = FALSE; 
	m_pStatistic = NULL;
	m_Score = 0;
	// Score im Statusbar updaten
	UpdateScoreInStatusBar(m_Score);
	m_pDo = new CDoControl(this);
}

CCyberSolitaireDoc::~CCyberSolitaireDoc()
{
	m_Score = 0;
	// Score im Statusbar updaten
	UpdateScoreInStatusBar(m_Score);
	DeleteCards();
	// hier werden nur die Pointer auf die Piles entsorgt
	// das entsorgen der Piles selber geschieht dann bei der m_PileList
	m_DealPileArr.RemoveAll();
	m_RedealPileArr.RemoveAll();
	CPile* pPile;
	if (!m_PileList.IsEmpty())
	{
		POSITION pos = m_PileList.GetHeadPosition();
		while (pos)
		{
			if (pPile = m_PileList.GetNext(pos))
				delete pPile;
		}
		m_PileList.RemoveAll();
	}
	if (!m_LastPileList.IsEmpty())
	{
		POSITION pos = m_LastPileList.GetHeadPosition();
		while (pos)
		{
			if (pPile = m_LastPileList.GetNext(pos))
				delete pPile;
		}
		m_LastPileList.RemoveAll();
	}
	if (m_pMovePile)
	{
		delete m_pMovePile;
		m_pMovePile = NULL;
	}

	// Redo/Undo Object entsorgen
	delete m_pDo;
	TRACE("Destruct CyberSolitaireDoc\n");
}

BOOL CCyberSolitaireDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// ZU ERLEDIGEN: Hier Code zur Reinitialisierung einfügen
	// (SDI-Dokumente verwenden dieses Dokument)

	SetGameName(GetApplication()->GetGameName());
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireDoc Serialisierung

void CCyberSolitaireDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_CyberSolVersion = CYBERSOLDOC_VERSION;
		ar << m_CyberSolVersion;
		ar << m_GameName;
		ar << m_CountOfRedeals;

	}
	else
	{
		ar >> m_CyberSolVersion;
		// zu_tun
		/*
		if (m_CyberSolVersion  < CYBERSOLDOC_VERSION)
		{
			AfxMessageBox(IDS_WRONG_VERSION);
		}
		*/
		ar >> m_GameName;
		ar >> m_CountOfRedeals;
	}

	m_Cards.Serialize(ar);
	m_PileList.Serialize(ar);
	m_DealPileArr.Serialize(ar);
	m_RedealPileArr.Serialize(ar);

#ifdef _DEBUG
	//afxDump << m_PileList;
#endif
	m_pDo->Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_Score;
		ar.Write(&m_TypeOfScoring,sizeof(m_TypeOfScoring));
		ar << m_BasicCardValue;
	}
	else
	{
		ar >> m_Score;
		ar.Read(&m_TypeOfScoring,sizeof(m_TypeOfScoring));
		m_pDo->m_pDoc = this;
		if (m_CyberSolVersion  < 2)
			ar >> m_BasicCardValue;
		else
			m_CyberSolVersion = 0;
	}
	//UpdatePiles();		// ???
}

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireDoc Diagnose

#ifdef _DEBUG
void CCyberSolitaireDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCyberSolitaireDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireDoc Befehle

/////////////////////////////////////////////////////////////////////////////
// Initialisierung des Spiels
BOOL CCyberSolitaireDoc::InitGame(CString& GameName)
{
	ASSERT((m_pStatistic && !m_pTTTDoc) || (!m_pStatistic && m_pTTTDoc));
	// Find the Game
	return InitGame(GetSelectDoc()->m_SolLayoutArr[GetSelectDoc()->FindGame(GameName)],m_pStatistic);
}
/////////////////////////////////////////////////////////////////////////////
// Initialisierung des Spiels
BOOL CCyberSolitaireDoc::InitGame(CSolLayout& SolLayout, CStatistic* pStatistic, CTTTDoc* pTTTDoc /* = NULL */)
{
	m_GameName = SolLayout.m_GameName;
	if (pTTTDoc) ASSERT(!pStatistic);
	if (pStatistic) ASSERT(!pTTTDoc);
	m_pStatistic = pStatistic;
	m_pTTTDoc = pTTTDoc;

	SetTTTGameMenu();

	GetApplication()->LogMessage("Init Game %s",SolLayout.m_GameName);
	if (m_pTTTDoc)
		GetApplication()->LogMessage("Init Game from TTT Game");

	// Karten erstellen
	CreateCards(SolLayout.m_NumberOfDecks);
	
	m_Score = 0;
	// zu_tun
	//if (SolLayout.m_MaxPoints == 0)
	//	SolLayout.m_MaxPoints = SolLayout.m_NumberOfDecks * 52;
	m_MaxPoints = SolLayout.m_MaxPoints;
	m_TypeOfScoring = SolLayout.m_TypeOfScoring;

	ASSERT(m_PileList.IsEmpty());
	// diese PileList sollte alle Infos, bis auf die Karten, die zum Spiel gehören, in sich bergen
	ASSERT(!SolLayout.m_PileList.IsEmpty());
	// über den Copy Constructor werden hier neue Piles angelegt
	CPile* pPile = NULL;
	CPile* pNewPile = NULL;
	POSITION pos = NULL;
	pos = SolLayout.m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = SolLayout.m_PileList.GetNext(pos);
		if (pPile->IsK(CStock))
			pNewPile = (CPile*)new CStock((CStock&)*pPile);
		if (pPile->IsK(CTableau))
			pNewPile = (CPile*)new CTableau((CTableau&)*pPile);
		if (pPile->IsK(CFoundation))
			pNewPile = (CPile*)new CFoundation((CFoundation&)*pPile);
		if (pPile->IsK(CWaste))
			pNewPile = (CPile*)new CWaste((CWaste&)*pPile);
		m_PileList.AddTail(pNewPile);

		if (pNewPile->m_DealOrder != 0)
		{
			m_DealPileArr.SetAtGrow(pNewPile->m_DealOrder-1,pNewPile);
		}
		if (pNewPile->m_RedealOrder != 0)
		{
			m_RedealPileArr.SetAtGrow(pNewPile->m_RedealOrder-1,pNewPile);
		}
	}
	// erst mal alle Karten in den Stock
	pPile = GetStock();
	if (!pPile)
	{
		// dieses Spiel hat keinen Stock
		pPile = (CStock*)new CStock;
		// Init Stock Pile
		((CStock*)pPile)->m_MaxCards =	 1;
		((CStock*)pPile)->m_KindOfOverlapped = olmNotOverlapped;
		((CStock*)pPile)->m_KindOfPiling = pilNotAvailable;
		((CStock*)pPile)->m_KindOfSelecting = selAll;
		((CStock*)pPile)->m_FaceAtStart = fasAllFaceDown;
		((CStock*)pPile)->m_NumberOnStart = 1;
		((CStock*)pPile)->m_NumberHiddenOnStart = 1;
		((CStock*)pPile)->m_IDDibEmpty = IDB_STOCK1;
		((CStock*)pPile)->m_dibEmpty.Load(AfxGetApp()->m_hInstance,pPile->m_IDDibEmpty);
		((CStock*)pPile)->m_DealTo = dealTableaus;
		((CStock*)pPile)->m_NumberPerDeal = 1;
		((CStock*)pPile)->m_PermittedPlay = pppNotAvailable;

		m_PileList.AddTail(pPile);
	}
	for (int j=1; j<=m_Cards.GetUpperBound();j++)
	{
		pPile->m_Cards.AddTail(m_Cards[j]);
	}

	// Karten mischen
	pPile->MixCards();

	// müssen Sonderbehandlungen durchgeführt werden?
	pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile->m_SpecialHandling)
		{
			if (pPile->m_SpecialHandlingText.Find("belegt mit:")!=-1)
			{
				CString CardName;
				int Index = pPile->m_SpecialHandlingText.ReverseFind(':');
				CardName = pPile->m_SpecialHandlingText.Right(pPile->m_SpecialHandlingText.GetLength()-Index-1);
				CCard* pCard = GetStock()->GetCard(CardName);
				pPile->m_Cards.AddHead(pCard);
				GetStock()->m_Cards.RemoveAt(GetStock()->m_Cards.Find(pCard));
			}
		}
		if (!pPile->m_UpdateOverlap)
		{
			pPile->m_OverlappedFactor = pPile->ComputeOverlappedFactor(pPile->m_NumberOnStart);
		}
	}

	// Basiskarten der Foundation
	int OnlyOneBasicCardValue = 0;		// die Basiskarte darf nur einmal festgelegt werden
	BOOL BasicCardOnlyVal;
	pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile->IsK(CFoundation))
		{
			CCard* pCard;
			switch (((CFoundation*)pPile)->m_KindOfBasicCard) {
				case basFirstPlayed:
				case basFixed:
					break;
				case basFixedLie:
					{
						if (pPile->m_NumberOnStart == 1)
						{
							// hole Karte aus dem Stock
							m_BasicCardValue = ((CFoundation*)pPile)->m_BasicCardValue;
							CCard* pCard = GetStock()->GetCard(m_BasicCardValue);
							pPile->m_Cards.AddHead(pCard);
							GetStock()->m_Cards.RemoveAt(GetStock()->m_Cards.Find(pCard));
							BasicCardOnlyVal = ((CFoundation*)pPile)->m_BasicCardOnlyVal;
						}
					break;
					}
				case basFromStock:
					{
						if (pPile->m_NumberOnStart == 1)
						{
							// Karten mischen
							//pPile->MixCards();
							// hole Karte aus dem Stock
							pCard = GetStock()->m_Cards.GetTail();
							CRect UpdateRect(ZeroRect);
							pPile->Get(GetStock(),pCard,(CRect&)UpdateRect);
							((CFoundation*)pPile)->m_BasicCardValue = pCard->m_nCardValue;
							m_BasicCardValue = ((CFoundation*)pPile)->m_BasicCardValue;
							BasicCardOnlyVal = ((CFoundation*)pPile)->m_BasicCardOnlyVal;
							OnlyOneBasicCardValue++;
						}
					break;
					}
				default:
					break;
			}
		}
	}
	// Basiskarten festlegen
	pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile->IsK(CFoundation)&& (((CFoundation*)pPile)->m_KindOfBasicCard == basFromStock))
		{
			ASSERT(OnlyOneBasicCardValue < 2);
			((CFoundation*)pPile)->m_BasicCardValue = m_BasicCardValue;
			((CFoundation*)pPile)->m_BasicCardOnlyVal = BasicCardOnlyVal;
		}
	}

	pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		// Spezialitäten bearbeiten
		if (pPile->IsK(CTableau) || pPile->IsK(CWaste))
		{
			// für Test
			//pPile->m_FaceAtStart = fasAllFaceUp;
			// Tableau mit der Startformation belegen
			if (pPile->m_NumberOnStart != NA)
			{
				CCard* pCard;
				for (int j=1; j<=pPile->m_NumberOnStart; j++)
				{
					// hole Karte aus dem Stock
					pCard = GetStock()->m_Cards.GetTail();
					CRect UpdateRect(ZeroRect);
					pPile->Get(GetStock(),pCard,(CRect&)UpdateRect);
				}
			}
		}
	}
		
	m_CountOfRedeals = 0;
	m_Score = 0;
		
	// jetzt für alle Piles Geometrie und FaceAtStart bearbeiten
	//UpdatePiles();
	ASSERT(!m_PileList.IsEmpty());
	//POSITION pos = m_PileList.GetHeadPosition();
	pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		// FaceAtStart bearbeiten
		pPile->UpdateFaceAtStart();
	}
	ComputeScore();
	CopyPileList();
	GetView()->ReportGameStatus();
	// das Spiel wird nur auf Modified gesetzt, wenn schon gepunktet wurde
	if (m_Score)
		SetModifiedFlag(TRUE);
	return TRUE;
}

void CCyberSolitaireDoc::CreateCards(int NumSets)
{
	// m_AllCards[0] wird nicht verwendet
	ASSERT((NumSets>0) && (NumSets<3));
	m_Cards.SetSize(NumSets*52 + 1);
	int		i = 1;
	ASSERT((NumSets>0) && (NumSets<3));

	// erstes Kartenspiel
	for (i = 1; i <= NumSets * 52; i++)
	{
		m_Cards[i] = new CCard(i);
		m_Cards[i]->m_LogicalSize = CCyberSolitaireApp::DefaultCardSize;
	}
}

void CCyberSolitaireDoc::DeleteCards()
{
	// alle Karten entsorgen
	for (int i = 1; i < m_Cards.GetSize(); i++)
		delete m_Cards[i];
	m_Cards.RemoveAll();
}

CCyberSolitaireView* CCyberSolitaireDoc::GetView()
{
	return m_pView;
}

void CCyberSolitaireDoc::UpdatePiles()
{
	CPile* pPile;
	if (!m_PileList.IsEmpty())
	{
		POSITION pos = m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = m_PileList.GetNext(pos);
			// Geometrie der Karten und des Piles festlegen
			//pPile->UpdateCardsPositions();
		}
	}
}

void CCyberSolitaireDoc::InitMovePile()
{
	ASSERT(!m_pMovePile);
	m_pMovePile = new CMovePile;
	m_pMovePile->SetPosition(NoPosition);
	m_pMovePile->m_KindOfPiling = pilNotAvailable;
	m_pMovePile->m_KindOfSelecting = selNotAvailable;
	m_pMovePile->m_FaceAtStart = fasNotAvailable;
	m_pMovePile->m_NumberOnStart = 0;
	m_pMovePile->m_NumberHiddenOnStart = -1;
	m_pMovePile->m_dibEmpty.Load(AfxGetApp()->m_hInstance,IDB_BITMAP68);
	m_pMovePile->m_KindOfOverlapped = olmNotOverlapped;
}

void CCyberSolitaireDoc::OnButtonUndo() 
{
	m_pDo->Undo();	
	ComputeScore();
}

void CCyberSolitaireDoc::OnUpdateButtonUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_pDo->m_DoIndex > -1) && (m_pDo->m_DoArr.GetUpperBound() > -1));	
}

void CCyberSolitaireDoc::OnButtonRedo() 
{
	m_pDo->Redo();
	ComputeScore();
}

void CCyberSolitaireDoc::OnUpdateButtonRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDo->m_DoIndex < m_pDo->m_DoArr.GetUpperBound());	
}

POSITION CCyberSolitaireDoc::GetFirstPilePosition(pileDealing Destination)
{
	ASSERT(!m_PileList.IsEmpty());
	POSITION pos = m_PileList.GetHeadPosition();
	POSITION lastpos = NULL;
	while (pos)
	{
		lastpos = pos;
		switch (Destination) 
		{
		case dealTableaus:
			if (m_PileList.GetNext(pos)->IsK(CTableau))
				return lastpos;
			break;
		case dealWaste:
			if (m_PileList.GetNext(pos)->IsK(CWaste))
				return lastpos;
			break;
		default:
			ASSERT(0);
		}
	}
	return NULL;
}

CPile* CCyberSolitaireDoc::GetNextPile(POSITION &pos, pileDealing Destination)
{
	if (!pos)
		return NULL;
	CPile* pPile;
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		switch (Destination) 
		{
		case dealTableaus:
			if (pPile->IsK(CTableau))
				return pPile;
			break;
		case dealWaste:
			if (pPile->IsK(CWaste))
				return pPile;
			break;
		default:
			ASSERT(0);
		}
	}
	return NULL;
}

POSITION CCyberSolitaireDoc::GetFirstTableauPosition()
{
	ASSERT(!m_PileList.IsEmpty());
	POSITION pos = m_PileList.GetHeadPosition();
	POSITION lastpos = NULL;
	while (pos)
	{
		lastpos = pos;
		if (m_PileList.GetNext(pos)->IsK(CTableau))
			return lastpos;
	}
	return NULL;
}

CTableau* CCyberSolitaireDoc::GetNextTableau(POSITION &pos)
{
	if (!pos)
		return NULL;
	CPile* pPile;
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile->IsK(CTableau))
			return (CTableau*)pPile;
	}
	return NULL;
}

POSITION CCyberSolitaireDoc::GetFirstWastePosition()
{
	ASSERT(!m_PileList.IsEmpty());
	POSITION pos = m_PileList.GetHeadPosition();
	POSITION lastpos = NULL;
	while (pos)
	{
		lastpos = pos;
		if (m_PileList.GetNext(pos)->IsK(CWaste))
			return lastpos;
	}
	return NULL;
}

CWaste* CCyberSolitaireDoc::GetNextWaste(POSITION &pos)
{
	if (!pos)
		return NULL;
	CPile* pPile;
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile->IsK(CWaste))
			return (CWaste*)pPile;
	}
	return NULL;
}


CStock* CCyberSolitaireDoc::GetStock()
{
	ASSERT(!m_PileList.IsEmpty());
	// weil jetzt eine feste Folge im Layout eistiert
	CPile* pPile = m_PileList.GetTail();
	if (pPile->IsK(CStock))
		return (CStock*)pPile;
	else 
		return NULL;
}

// für Spiele mit nur einem Ablagestapel (Foundation)
CFoundation* CCyberSolitaireDoc::GetFoundation()
{
	ASSERT(!m_PileList.IsEmpty());
	POSITION pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		CPile* pPile = m_PileList.GetNext(pos);
		if (pPile->IsK(CFoundation))
			return (CFoundation*)pPile;
	}
	return NULL;
}

int CCyberSolitaireDoc::ComputeScore()
{
	if(m_PileList.IsEmpty())
	{
		m_Score = 0;
		return m_Score;
	}
	int NewScore = 0;
	switch (m_TypeOfScoring) {
	case scoreSequenceInSuitAndFoundation:
	case scoreSequenceInSuitAndKingUp:
		{
			CPile* pPile;
			CCard* pNextCard;
			CCard* pPrevCard;

			// alle Piles durchsuchen
			POSITION pos = m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = m_PileList.GetNext(pos);
				// alle Karten des Piles durchsuchen
				if (!pPile->IsEmpty())
				{
					// für jeden im Foundation abgelegten Stapel gibts einen Extrapunkt
					if (pPile->IsK(CFoundation) && (m_TypeOfScoring == scoreSequenceInSuitAndFoundation))
						NewScore++;
					POSITION cardpos = pPile->m_Cards.GetHeadPosition();
					pPrevCard = pPile->m_Cards.GetNext(cardpos);
					if ((pPrevCard->IsKing()) && (m_TypeOfScoring == scoreSequenceInSuitAndKingUp))
						NewScore++;
					//int cnt = 1;
					while (cardpos)
					{
						pNextCard = pPile->m_Cards.GetNext(cardpos);
						// gezählt werden nur sichtbare Karten
						if (pPrevCard->GetFaceUp())
						{
							//if (!pPile->IsK(CFoundation))
							//{
								if (pPrevCard->IsDownInSequence(pNextCard) && pPrevCard->IsDownInSuit(pNextCard))
								{
									//cnt++;
									NewScore++;
								}
							/*
							}
							else
							{
								// Foundation ist UpInSequence
								if (pNextCard->IsUpInSequence(pPrevCard) && pNextCard->IsUpInSuit(pPrevCard))
									NewScore++;
							}
							*/
						}
						pPrevCard = pNextCard;
					}
				}
			}
		}
		break;
	case scoreSequenceNoColorAndFoundation:
		{
			CPile* pPile;
			CCard* pNextCard;
			CCard* pPrevCard;

			// alle Piles durchsuchen
			POSITION pos = m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = m_PileList.GetNext(pos);
				// alle Karten des Piles durchsuchen
				if (!pPile->IsEmpty())
				{
					// für jeden im Foundation abgelegten Stapel gibts einen Extrapunkt
					if (pPile->IsK(CFoundation))
						NewScore++;
					POSITION cardpos = pPile->m_Cards.GetHeadPosition();
					pPrevCard = pPile->m_Cards.GetNext(cardpos);
					while (cardpos)
					{
						pNextCard = pPile->m_Cards.GetNext(cardpos);
						// gezählt werden nur sichtbare Karten
						if (pPrevCard->GetFaceUp())
						{
							if (pPrevCard->IsDownInSequence(pNextCard))
								NewScore++;
						}
						pPrevCard = pNextCard;
					}
				}
			}
		}
		break;
	case scoreCardOnFoundation:
		{
			CPile* pPile;
			// alle Piles durchsuchen
			POSITION pos = m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = m_PileList.GetNext(pos);
				// alle Karten des Piles durchsuchen
				if (!pPile->IsEmpty())
				{
					// für jeden im Foundation abgelegten Stapel gibts einen Extrapunkt
					if (pPile->IsK(CFoundation))
					{
						NewScore += pPile->m_Cards.GetCount();
					}
				}
			}
		}
		break;
	default:
		break;
	}

	if (NewScore != m_Score)
	{
		// zu_tun
		// Geräusche machen
		if (NewScore > m_Score)
			//PlaySound("Plus.wav",NULL,SND_ASYNC | SND_FILENAME);
			PlaySound("Plus.wav",NULL,SND_SYNC | SND_FILENAME);
		else
			//PlaySound("Minus.wav",NULL,SND_ASYNC | SND_FILENAME);
			PlaySound("Minus.wav",NULL,SND_SYNC | SND_FILENAME);

		m_Score = NewScore;
	}

	// Score im Statusbar updaten
	UpdateScoreInStatusBar(m_Score);
	UpdateGameInStatusBar(m_GameName);
	WinsGame();
	return m_Score;
}

void CCyberSolitaireDoc::TestEmptyPiles()
{
	CPile* pPile;
	// alle Piles durchsuchen
	POSITION pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile->IsEmpty())
		{
			CCard* pCard = NULL;
			if (pPile->IsK(CTableau))
			{
				switch (pPile->m_PutIfEmpty)
				{
				case pieNoCard:
				case pieAnyCard:
					break;
				case pieFromDealOrAny:
				case pieOnlyFromAutoDeal:
					{
						// hole Karte aus dem DealPile falls noch vorhanden
						CPile* pDealPile;
						for (int i=0; i< m_DealPileArr.GetSize();i++)
						{
							pDealPile = m_DealPileArr.GetAt(i);
							PROGRAM_FAILURE(pDealPile);
							if (!pDealPile->m_Cards.IsEmpty())
							{
								pCard = pDealPile->m_Cards.GetTail();
								CRect UpdateRect(ZeroRect);
								pPile->Get(pDealPile,pCard,(CRect&)UpdateRect);
								BOOL ChangeLastCardFaceUp = FALSE;
								//PROGRAM_FAILURE(!pCard->GetFaceUp());
								CCard* pNewLastDealCard = NULL;
								m_pDo->InsertStartDo();
								if (!pCard->GetFaceUp())
								{
									pCard->SetFaceUp(TRUE);
									ChangeLastCardFaceUp = TRUE;
								}
								else
								{
									// die Karte auf dem DealPile war schon zu sehen
									// deshalb muß die letzte Karte des DealPile umgedreht werden
									// falls sie verdeckt ist
									if (!pDealPile->m_Cards.IsEmpty())
									{
										pNewLastDealCard = pDealPile->m_Cards.GetTail();
										if (!pNewLastDealCard->GetFaceUp())
										{
											pNewLastDealCard->SetFaceUp(TRUE);
											m_pDo->CreateDo(CHANGE_CARD_FACEUP,pDealPile,NULL,pNewLastDealCard,TRUE);
										}
									}
								}
								m_pDo->CreateDo(GET_DEAL,pDealPile,pPile,pCard,ChangeLastCardFaceUp);
								m_pDo->CreateDo(END,NULL,NULL,NULL,NULL);
								UpdateAllViews(NULL,HINT_DISPLAY_PILE,(CObject*)pPile);
								UpdateAllViews(NULL,HINT_DISPLAY_PILE,(CObject*)pDealPile);
								break;
							}
						}
					}
					break;
				case pieBasicCard:		
				case pieBasicCardMinus1:
				case pieKing:			
				default:
					break;
				}
			}
			if (pPile->IsK(CStock))
			{
				if ((((CStock*)pPile)->m_NumberOfRedeal > 0 &&((CStock*)pPile)->m_NumberOfRedeal > m_CountOfRedeals) || 
					  (((CStock*)pPile)->m_NumberOfRedeal == -1))
				{
					// es kann neu gegeben werden
				}
				else
				{
					// die Bitmap dür den leeren Pile wechselt
					if (pPile->m_IDDibEmpty != IDB_EMPTY3)
					{
						pPile->m_IDDibEmpty = IDB_EMPTY3;
						pPile->m_dibEmpty.Detach();
						pPile->m_dibEmpty.Load(AfxGetApp()->m_hInstance,pPile->m_IDDibEmpty);
						GetView()->UpdateBitmaps();
						UpdateAllViews(NULL,HINT_DISPLAY_PILE,(CObject*)pPile);
					}
				}
			}
		}
	}
}



CSelectDoc* CCyberSolitaireDoc::GetSelectDoc()
{
	POSITION pos = GetApplication()->pSelectSolitaireTemplate->GetFirstDocPosition();
	// wir können davon ausgehen, daß es ein oder kein CProjDoc gibt
	CSelectDoc* pSelectDoc = NULL;
	if (pos)
		pSelectDoc = (CSelectDoc*)GetApplication()->pSelectSolitaireTemplate->GetNextDoc(pos);
	return pSelectDoc;

}

void CCyberSolitaireDoc::OnSpielNeu() 
{
	if (GetView())
		GetView()->ResetToolTip();
	ComputeStatistic();
	DeleteContents();
	InitGame(m_GameName);
	GetView()->UpdateBitmaps();
	UpdateAllViews(NULL,HINT_DISPLAY_ALL,NULL);
}

void CCyberSolitaireDoc::OnSpielNochmal() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	m_pDo->UndoAll();
	ComputeScore();
}

void CCyberSolitaireDoc::DeleteContents() 
{
	m_Score = 0;
	// Score im Statusbar updaten
	UpdateScoreInStatusBar(m_Score);
	m_BasicCardValue = 0;
	DeleteCards();
	// hier werden nur die Pointer auf die Piles entsorgt
	// das entsorgen der Piles selber geschieht dann bei der m_PileList
	m_DealPileArr.RemoveAll();
	m_RedealPileArr.RemoveAll();
	CPile* pPile;
	if (!m_PileList.IsEmpty())
	{
		POSITION pos = m_PileList.GetHeadPosition();
		while (pos)
		{
			if (pPile = m_PileList.GetNext(pos))
				delete pPile;
		}
		m_PileList.RemoveAll();
	}
	if (!m_LastPileList.IsEmpty())
	{
		POSITION pos = m_LastPileList.GetHeadPosition();
		while (pos)
		{
			if (pPile = m_LastPileList.GetNext(pos))
				delete pPile;
		}
		m_LastPileList.RemoveAll();
	}
	// Redo/Undo Object entsorgen
	if (m_pDo)
	{
		delete m_pDo;
		m_pDo = NULL;
	}
	m_pDo = new CDoControl(this);
	CDocument::DeleteContents();
}

void CCyberSolitaireDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	
	CDocument::SetTitle(lpszTitle);
}

void CCyberSolitaireDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	
	CDocument::SetPathName(lpszPathName, bAddToMRU);
}

BOOL CCyberSolitaireDoc::SaveModified() 
{
	BOOL RetVal;
	if (!IsModified())
		RetVal = TRUE;        // ok to continue

	// get name/title of document
	CString name;
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
		name = m_strPathName;
		AfxGetFileTitle(m_strPathName, name.GetBuffer(_MAX_PATH), _MAX_PATH);
		name.ReleaseBuffer();
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	int Ret;
	
	if (((CMainFrame *)AfxGetMainWnd())->IsClosingApplication())
		Ret = AfxMessageBox(prompt, MB_YESNO | MB_ICONQUESTION, AFX_IDP_ASK_TO_SAVE);
	else
		Ret = AfxMessageBox(prompt, MB_YESNOCANCEL | MB_ICONQUESTION, AFX_IDP_ASK_TO_SAVE);

	switch (Ret)
	{
	case IDCANCEL:
		RetVal = FALSE;       // don't continue
		break;
	case IDYES:
		// If so, either Save or Update, as appropriate
		RetVal = DoFileSave();
		break;

	case IDNO:
		// If not saving changes, revert the document
		RetVal = FALSE;
		break;

	default:
		ASSERT(FALSE);
		RetVal = TRUE;
		break;
	}

	if (RetVal)
		SetModifiedFlag(FALSE);
	return RetVal;
}

BOOL CCyberSolitaireDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Speziellen Erstellungscode hier einfügen
	GetApplication()->LogMessage("Load Game %s",m_GameName);
	CSolLayout SolLayout = GetSelectDoc()->m_SolLayoutArr[GetSelectDoc()->FindGame(m_GameName)];
	CStatistic TmpStatistic;
	m_pStatistic = &TmpStatistic;
	if (!GetSelectDoc()->m_pMapGameNameToStatistic->Lookup(m_GameName,m_pStatistic))
	{
		// Fehler
		ASSERT(0);
	}
	m_Score = 0;
	m_MaxPoints = SolLayout.m_MaxPoints;
	m_TypeOfScoring = SolLayout.m_TypeOfScoring;

	CopyPileList();
	return TRUE;
}

BOOL CCyberSolitaireDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	TRACE("Entry CyberSolitaireDoc::OnSaveDocument\n");
	if (!CDocument::OnSaveDocument(lpszPathName))
	{
		TRACE("Exit  CyberSolitaireDoc::OnSaveDocument\n");
		return FALSE;
	}
	
	// TODO: Speziellen Erstellungscode hier einfügen
	TRACE("Exit  CyberSolitaireDoc::OnSaveDocument\n");
	return TRUE;
}

BOOL CCyberSolitaireDoc::DoFileSave()
{
	DWORD dwAttrib = GetFileAttributes(m_strPathName);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		// we do not have read-write access or the file does not (now) exist
		if (!DoSave(NULL))
		{
			TRACE0("Warning: File save with new name failed.\n");
			return FALSE;
		}
	}
	else
	{
		if (!DoSave(m_strPathName))
		{
			TRACE0("Warning: File save failed.\n");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCyberSolitaireDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			//newName = m_strTitle;
			newName = m_strTitle + "_" + GetDateTimeVersion();
			// check for dubious filename
			//int iBad = newName.FindOneOf(_T(" #%;/\\"));
			int iBad = newName.FindOneOf(_T("#%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			try
			{
				CFile::Remove(newName);
			}
			catch (CException* err)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				err->Delete();
			}
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}



CString CCyberSolitaireDoc::GetDateTimeVersion()
{
	CString DateTimeVersion;
	COleDateTime Time = COleDateTime::GetCurrentTime();
	DateTimeVersion = Time.Format("%d$%m$%y_%H$%M$%S");
	return DateTimeVersion;
}

void CCyberSolitaireDoc::OnCloseDocument() 
{
	TRACE("Entry CyberSolitaireDoc::OnCloseDocument\n");
	/*
	// kann irgendwann mal raus
	ComputeStatistic();
	if (!m_pTTTDoc && IsModified())
	{
		SaveModified();
		SetModifiedFlag(FALSE);
	}
	// TTT benachrichtigen, daß das Spiel beendet wurde
	if (m_pTTTDoc)
		SendMessage(m_pTTTDoc->GetView()->m_hWnd,WM_USER_TTT_GAME_FINISHED,0,0L);

	CDocument::OnCloseDocument();
	*/
	ASSERT(m_ExitGameCause == EXIT_CAUSE_EXITAPP);
	HandleTheEndOfGame();
	TRACE("Exit  CyberSolitaireDoc::OnCloseDocument\n");

}

void CCyberSolitaireDoc::CopyPileList()
{
	CPile* pPile;
	// erst mal die Liste leeren
	if (!m_LastPileList.IsEmpty())
	{
		POSITION pos = m_LastPileList.GetHeadPosition();
		while (pos)
		{
			if (pPile = m_LastPileList.GetNext(pos))
				delete pPile;
		}
		m_LastPileList.RemoveAll();
	}
	// und jetzt kopieren
	POSITION pos = m_PileList.GetHeadPosition();
	while (pos)
	{
		pPile = new CPile(*m_PileList.GetNext(pos));
		m_LastPileList.AddTail(pPile);
	}
}


// spielt Töne zum Spielgeschehen
BOOL CCyberSolitaireDoc::PlaySound(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	if (!m_InAutoPlay)
	{
		return ::PlaySound(pszSound,hmod,fdwSound);
	}
	else
	{
		// in der AutoPlay-Phase werden keine Töne ausgegeben
		return FALSE;
	}
	return FALSE;
}

void CCyberSolitaireDoc::OnSpielAbbrechen() 
{
	m_ExitGameCause = EXIT_CAUSE_ABORT;
	HandleTheEndOfGame();
}

void CCyberSolitaireDoc::OnFileClose()
{
	TRACE("Entry CyberSolitaireDoc::OnFileClose\n");
	ASSERT(0);	// sollte nicht mehr aufgerufen werden
	/*
	// wurde das Spiel gewonnen ?
	if (m_WinsGame)
	{
		// zu_tun
		// einen anderen Dialog einführen
		if (!m_pTTTDoc)
			AfxMessageBox(IDS_WINS_GAME,MB_OK);
	}

	if (!m_pTTTDoc && !SaveModified())
		return;

	// shut it down
	OnCloseDocument();
	// this should destroy the document
	*/
	TRACE("Exit  CyberSolitaireDoc::OnFileClose\n");

}

void CCyberSolitaireDoc::ComputeStatistic()
{
	// zu_tun
	//return;
	ASSERT((m_pStatistic && !m_pTTTDoc) || (!m_pStatistic && m_pTTTDoc));
	// Statistik wird nur bei normalen Spielen geführt (nicht im TTT)
	if (!m_pTTTDoc)
	{
		m_pStatistic->m_NumberOfPlayed++;
		m_pStatistic->m_SumOfPoints += m_Score;
		m_pStatistic->m_Average = m_pStatistic->m_SumOfPoints / m_pStatistic->m_NumberOfPlayed;
		CSingleGameResult GameResult;
		GameResult.m_PlayerName = GetApplication()->m_CurrentPlayerName;	
		GameResult.m_Points = m_Score;
		m_pStatistic->m_GameResultList.AddTail(GameResult);
	}
}

// Testet, ob das Spiel gewonnen wurde
BOOL CCyberSolitaireDoc::WinsGame(void)
{
	// Die Berechnung, ob ein Spiel gewonnen wurde unterscheidet sich beim normalen Spiel
	// und einem Spiel im TTT
	/*
	if (m_pTTTDoc)
	{
		// wir befinden uns im TTT Modus
		CSolLayout SolLayout = GetSelectDoc()->m_SolLayoutArr[GetSelectDoc()->FindGame(m_GameName)];
		ASSERT(SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel] > 0);
		m_WinsGame = m_Score >= SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel];
	}
	else
	{
		m_WinsGame = (m_Score == m_MaxPoints);
	}
	if (!m_InAutoPlay && m_WinsGame && !m_pTTTDoc) 
	{
		// wurde das Spiel gewonnen ?
		// zu_tun
		// einen anderen Dialog einführen
		PlaySound("Clapping.wav",NULL,SND_ASYNC | SND_FILENAME);
		AfxMessageBox(IDS_WINS_GAME,MB_OK);
	}
	*/
	// Diese Funktion überprüft nur noch, ob ein Spiel gewonnen wurde.
	m_WinsGame = (m_Score == m_MaxPoints);
	if (m_WinsGame)
		m_ExitGameCause = EXIT_CAUSE_WINS;
	return m_WinsGame;
}

// behandelt das Verhalten beim Ende eines Spiels
void CCyberSolitaireDoc::HandleTheEndOfGame(void)
{
	switch(m_ExitGameCause)
	{
	case EXIT_CAUSE_WINS:
		{
			if (!m_pTTTDoc)
			{
				// zu_tun
				// einen anderen Dialog einführen
				PlaySound("Clapping.wav",NULL,SND_ASYNC | SND_FILENAME);
				AfxMessageBox(IDS_WINS_GAME,MB_OK);
				ComputeStatistic();
			}
			else
			{
				// zu_tun
				// ein gewonnenes TTT Spiel auswerten
				ASSERT(m_pTTTDoc);
				CTTTGameFinishedDialog TTTGameFinishedDlg;
				TTTGameFinishedDlg.m_pDoc = this;
				TTTGameFinishedDlg.m_GameName = m_GameName;
				TTTGameFinishedDlg.m_ActualScore = m_Score;
				CSolLayout SolLayout = GetSelectDoc()->m_SolLayoutArr[GetSelectDoc()->FindGame(m_GameName)];
				ASSERT(SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel] > 0);
				TTTGameFinishedDlg.m_NeededScore = SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel];
				TTTGameFinishedDlg.DoModal();
				// aktuellen Spielstand dem TTT mitteilen
				m_pTTTDoc->m_CurrentCyberSolitaireGameScore = m_Score;
				// TTT benachrichtigen, daß das Spiel beendet wurde
				SendMessage(m_pTTTDoc->GetView()->m_hWnd,WM_USER_TTT_GAME_FINISHED,0,0L);
			}
			CDocument::OnCloseDocument();
			break;
		}
	case EXIT_CAUSE_ABORT:
		{
			// zu_tun
			// überlegen was bei TT Spielen geschieht
			if(AfxMessageBox(IDS_DO_YOU_WANT_CANCEL_GAME, MB_YESNO | MB_ICONQUESTION) == IDNO )
				return;

			CDocument::OnCloseDocument();
			break;
		}
	case EXIT_CAUSE_EXITGAME:
		{
			BOOL RetVal=TRUE;	// so vorbesetzt, damit Spiele, die gar nicht angefangen wurden
								// auch nicht in die Statistik kommen
			if (!m_pTTTDoc)
			{
				if (IsModified())
				{
					RetVal = SaveModified();
					SetModifiedFlag(FALSE);
				}
				// das Spiel kommt nur in die Statistik, wenn es nicht gespeichert
				// wurde. Andernfalls ist davon auszugehen, daß es irgendwann mal weitergespielt wird!
				if (!RetVal)
				{
					ComputeStatistic();
				}
			}
			// zu_tun
			// überlegen was bei TTT Spielen geschieht
			CDocument::OnCloseDocument();
			break;
		}
	case EXIT_CAUSE_EXITAPP:
		{
			BOOL RetVal=TRUE;	// so vorbesetzt, damit Spiele, die gar nicht angefangen wurden
								// auch nicht in die Statistik kommen
			if (!m_pTTTDoc)
			{
				if (IsModified())
				{
					RetVal = SaveModified();
					SetModifiedFlag(FALSE);
				}
				// das Spiel kommt nur in die Statistik, wenn es nicht gespeichert
				// wurde. Andernfalls ist davon auszugehen, daß es irgendwann mal weitergespielt wird!
				if (!RetVal)
				{
					ComputeStatistic();
				}
			}
			// zu_tun
			// überlegen was bei TT Spielen geschieht
			CDocument::OnCloseDocument();
			break;
		}
	case EXIT_CAUSE_TTTBACK:
		{
			ASSERT(m_pTTTDoc);
			CTTTGameFinishedDialog TTTGameFinishedDlg;
			TTTGameFinishedDlg.m_pDoc = this;
			TTTGameFinishedDlg.m_GameName = m_GameName;
			TTTGameFinishedDlg.m_ActualScore = m_Score;
			CSolLayout SolLayout = GetSelectDoc()->m_SolLayoutArr[GetSelectDoc()->FindGame(m_GameName)];
			ASSERT(SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel] > 0);
			TTTGameFinishedDlg.m_NeededScore = SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel];
			if (TTTGameFinishedDlg.DoModal() == IDOK)
			{
				// zu_tun
				// aktuellen Spielstand dem TTT mitteilen
				m_pTTTDoc->m_CurrentCyberSolitaireGameScore = m_Score;
				// TTT benachrichtigen, daß das Spiel beendet wurde
				SendMessage(m_pTTTDoc->GetView()->m_hWnd,WM_USER_TTT_GAME_FINISHED,0,0L);
				CDocument::OnCloseDocument();
			}
			else
			{
				return;
			}
			break;
		}

	default:
		break;
	}
}

void CCyberSolitaireDoc::OnGameExit()
{
	m_ExitGameCause = EXIT_CAUSE_EXITGAME;
	HandleTheEndOfGame();
}

void CCyberSolitaireDoc::OnSpielBackToTttWindow()
{
	m_ExitGameCause = EXIT_CAUSE_TTTBACK;
	HandleTheEndOfGame();
}

// überprüft, ob ein TT Spiel gewonnen oder verloren wurde
BOOL CCyberSolitaireDoc::WinsTTTGame(void)
{
	// wir befinden uns im TTT Modus
	ASSERT(m_pTTTDoc);
	BOOL bWinsTTTGame = FALSE;
	CSolLayout SolLayout = GetSelectDoc()->m_SolLayoutArr[GetSelectDoc()->FindGame(m_GameName)];
	ASSERT(SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel] > 0);
	bWinsTTTGame = m_Score >= SolLayout.m_LevelPointArray[m_pTTTDoc->m_CurrentLevel];
	return bWinsTTTGame;
}

void CCyberSolitaireDoc::OnTTTSpielSpeichern()
{
	ASSERT(m_pTTTDoc);
	m_pTTTDoc->m_WithCurrentGameRunning = TRUE;
	m_pTTTDoc->DoFileSave();
	CString CyberSolitaireFileName = m_pTTTDoc->GetPathName();
	int Index = CyberSolitaireFileName.ReverseFind('.');
	CyberSolitaireFileName = CyberSolitaireFileName.Left(Index+1);
	CyberSolitaireFileName += "CYS";
	this->SetPathName(CyberSolitaireFileName,0);
	if (!DoSave(m_strPathName))
	{
		TRACE0("Warning: File save failed.\n");
	}
}

void CCyberSolitaireDoc::OnTTTSpielSpeichernUnter()
{
	ASSERT(m_pTTTDoc);
	m_pTTTDoc->m_WithCurrentGameRunning = TRUE;
	if(!m_pTTTDoc->DoSave(NULL))
		TRACE("Warning: File save-as failed.\n");
	CString CyberSolitaireFileName = m_pTTTDoc->GetPathName();
	int Index = CyberSolitaireFileName.ReverseFind('.');
	CyberSolitaireFileName = CyberSolitaireFileName.Left(Index+1);
	CyberSolitaireFileName += "CYS";
	this->SetPathName(CyberSolitaireFileName,0);
	if (!DoSave(m_strPathName))
	{
		TRACE0("Warning: File save failed.\n");
	}
}

// ersetzt das "Spiel" Menu bei einem Spiel, wenn es im TTT gespielt wird
void CCyberSolitaireDoc::SetTTTGameMenu(void)
{
	// das Menu "Spiel" ersetzen
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu* pMenu = NULL;
	if (pFrame && m_pTTTDoc)
	{
		pMenu = pFrame->GetMenu();
		ASSERT(m_pView);
		CMenu* pNewSubMenu = m_pView->m_TTTGameMenu.GetSubMenu(0);
		CString MenuText;
		pMenu->GetMenuString(0,MenuText,MF_BYPOSITION);
		BOOL bRet = pMenu->ModifyMenu(0,MF_BYPOSITION | MF_POPUP,(UINT)pNewSubMenu->GetSafeHmenu(),MenuText);
 		pFrame->DrawMenuBar();
		return;
	}	    
	if (pFrame && !m_pTTTDoc)
	{
		pMenu = pFrame->GetMenu();
		ASSERT(m_pView);
		CMenu* pNewSubMenu = m_pView->m_GameMenu.GetSubMenu(0);
		CString MenuText;
		pMenu->GetMenuString(0,MenuText,MF_BYPOSITION);
		BOOL bRet = pMenu->ModifyMenu(0,MF_BYPOSITION | MF_POPUP,(UINT)pNewSubMenu->GetSafeHmenu(),MenuText);
 		pFrame->DrawMenuBar();
		return;
	}	    

}

// Score im Statusbar updaten
//
void CCyberSolitaireDoc::UpdateScoreInStatusBar(int Score)
{
	CString strScore;
	if (m_pTTTDoc)
	{
		int TTTLevelPoints = m_pTTTDoc->m_pCurrentSolLayout->m_LevelPointArray[m_pTTTDoc->m_CurrentLevel];
		strScore.Format("Ergebnis:%d von %d",Score,TTTLevelPoints);
	}
	else
	{
		strScore.Format("Ergebnis:%d von %d",Score,m_MaxPoints);
	}
	SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_SCORE, (long)&strScore);
}

// GameName im Statusbar updaten
//
void CCyberSolitaireDoc::UpdateGameInStatusBar(CString& GameName)
{
	CString strName;
	if (m_pTTTDoc)
	{
		int TTTLevelPoints = m_pTTTDoc->m_pCurrentSolLayout->m_LevelPointArray[m_pTTTDoc->m_CurrentLevel];
		strName.Format("%s / TicTacToe",GameName);
	}
	else
	{
		strName = GameName;
	}
	SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_GAME_NAME, (long)&strName);
}

// Spieler im Statusbar updaten
//
void CCyberSolitaireDoc::UpdatePlayerInStatusBar(CPlayer& Player)
{
	CString strText;
	strText.Format("%s/%s",Player.m_NickName,Player.m_Rank);
	SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_PLAYER, (long)&strText);
}

