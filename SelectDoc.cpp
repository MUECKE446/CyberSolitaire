// SelectDoc.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "SolEditDoc.h"
#include "SolEditView.h"
#include "TTTField.h"
#include "TTTFrame.h"
#include "TTTDoc.h"
#include "TTTView.h"
#include "GameTree.h"
#include "StatisticDoc.h"
#include "StatisticView.h"
//#include "Statistic.h"

#include "SelectCardsDialog.h"

#include "SelectView.h"
#include "SelectDoc.h"
#include ".\selectdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDoc

IMPLEMENT_DYNCREATE(CSelectDoc, COleDocument)

CSelectDoc::CSelectDoc()
{
	m_pDaoWorkSpace = new CDaoWorkspace;
	m_pCybSolDB		= NULL;
	m_DBName = GetApplication()->m_FullFilePath;
	m_DBName += "CYBERSOL1.MDB";

	m_pView = NULL;
	m_pWorkingLayout = NULL;
	m_pMapGameNameToStatistic = NULL;

	
	// für Test
	/*
	CPlayer Player;
	Player.m_NickName = "Name3";
	Player.m_Rank = "Expert3";
	m_PlayerArr.Add(Player);
	Player.m_NickName = "Name4";
	Player.m_Rank = "Expert4";
	m_PlayerArr.Add(Player);
	Player.m_NickName = "Name1";
	Player.m_Rank = "Rookie1";
	m_PlayerArr.Add(Player);
	Player.m_NickName = "Name2";
	Player.m_Rank = "Rookie2";
	m_PlayerArr.Add(Player);
	*/
	
}

BOOL CSelectDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;

	CString PathName = GetApplication()->m_FullFilePath;
	PathName += "GAMES.DAT";
	LoadSolLayoutArray(PathName);
	FillGameList();
	PathName = GetApplication()->m_FullFilePath;
	PathName += "CYBERSOLITAIRE.DAT";
	LoadPlayerArray(PathName);
	PathName = GetApplication()->m_FullFilePath;
	PathName += "STATISTICS.DAT";
	LoadMapGameNameToStatistic(PathName);
	if (CompleteMapGameNameToStatistic())
		SaveMapGameNameToStatistic(PathName);
	
	//m_SolLayoutArr.SortAlphabetical();

	// für Test
	/*
	BOOL SaveAgain = FALSE;
	for (int i=0;i<m_SolLayoutArr.GetSize();i++)
	{
		CSolLayout& SolLayout = m_SolLayoutArr[i];
		if ((SolLayout.m_GameName == "Big Forty") || (SolLayout.m_GameName == "Black Widow") || (SolLayout.m_GameName == "Napoleons Square"))
		{
			CPile* pPile;
			POSITION pos=NULL;
			for (pos = SolLayout.m_PileList.GetHeadPosition();pos != 0;)
			{
				pPile = SolLayout.m_PileList.GetNext(pos);
				if (pPile->m_KindOfSelecting == selAllSeqInSuitOrLast)
				{
					pPile->m_KindOfSelecting = selAnySeqInSuit;
					//TRACE("KindOfSelecting changed\n");
					SaveAgain = TRUE;
				}
			}
		}
		SolLayout.m_MaxPoints = SolLayout.m_NumberOfDecks * 52;
		SaveAgain = TRUE;
	}
	PathName = "GAMES.DAT";
	if (SaveAgain)
		SaveSolLayoutArray(PathName);
	*/

	if (GetApplication()->m_ReportGameLayouts)
		m_SolLayoutArr.ReportProperties();
	
	return TRUE;
}

CSelectDoc::~CSelectDoc()
{
	CString PathName = GetApplication()->m_FullFilePath;
	PathName += "CYBERSOLITAIRE.DAT";
	SavePlayerArray(PathName);
	// nun müssen alle dynamisch erzeugten Elemente entsorgt werden
	m_SolLayoutArr.DeleteAll();
	m_PlayerArr.DeleteAll();
	POSITION	pos;
	CStatistic*	pStatistic;
	CString		key; 
	// Iterate through the entire map
	for( pos = m_pMapGameNameToStatistic->GetStartPosition(); pos != NULL; )
	{
		m_pMapGameNameToStatistic->GetNextAssoc( pos, key, pStatistic );
		delete pStatistic;
	}
	m_pMapGameNameToStatistic->RemoveAll();
	delete m_pMapGameNameToStatistic;
	if (m_pCybSolDB) 
	{
		m_pCybSolDB->Close();
		delete m_pCybSolDB;
		m_pCybSolDB = NULL;
	}

	if (m_pDaoWorkSpace)
	{
		m_pDaoWorkSpace->Close();
		delete m_pDaoWorkSpace;
		m_pDaoWorkSpace = NULL;
	}

}


BEGIN_MESSAGE_MAP(CSelectDoc, COleDocument)
	//{{AFX_MSG_MAP(CSelectDoc)
	ON_COMMAND(ID_OPEN_GAMEEDITOR, OnOpenGameeditor)
	ON_UPDATE_COMMAND_UI(ID_OPEN_GAMEEDITOR, OnUpdateOpenGameeditor)
	ON_COMMAND(ID_GAMECONTEXT_DELETE_GAME, OnGamecontextDeleteGame)
	ON_UPDATE_COMMAND_UI(ID_GAMECONTEXT_DELETE_GAME, OnUpdateGamecontextDeleteGame)
	ON_COMMAND(ID_GAMECONTEXT_STATISTIK, OnGamecontextStatistik)
	ON_UPDATE_COMMAND_UI(ID_GAMECONTEXT_STATISTIK, OnUpdateGamecontextStatistik)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EINSTELLUNGEN_UEBERNEHMETTTWERTE, OnEinstellungenUebernehmetttwerte)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Diagnose CSelectDoc

#ifdef _DEBUG
void CSelectDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CSelectDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Serialisierung CSelectDoc 
void CSelectDoc::Serialize(CArchive& ar)
{
	// es wird nicht das ganze Document gespeichert/geladen
	// sondern nur das m_SolLayoutArr, welches alle Spielbeschreibungen enthält
	//m_SolLayoutArr.Serialize(ar);

	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Code zum Speichern hier einfügen
	}
	else
	{
		// ZU ERLEDIGEN: Code zum Laden hier einfügen
	}
}

/////////////////////////////////////////////////////////////////////////////
// Befehle CSelectDoc 

void CSelectDoc::OnCloseDocument() 
{
	// Statistic speichern
	CString PathName = GetApplication()->m_FullFilePath;
	PathName += "STATISTICS.DAT";
	SaveMapGameNameToStatistic(PathName);

	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;  // don't destroy document while closing views

	while (!m_viewList.IsEmpty())
	{
		// get frame attached to the view
		CView* pView = (CView*)m_viewList.GetHead();
		ASSERT_VALID(pView);
		CControlBar * pBar = (CControlBar *) pView->GetParent();
	    ASSERT_KINDOF(CControlBar, pBar);

		pBar->DestroyWindow();
        delete pBar;
        // will destroy the view as well
	}

	m_bAutoDelete = bAutoDelete;
	// clean up contents of document before destroying the document itself
	DeleteContents();
	// delete the document if necessary
	if (m_bAutoDelete)
		delete this;
}

void CSelectDoc::FillGameList()
{
	int i=0;
	m_GameList.RemoveAll();
	if (m_SolLayoutArr.GetSize())
	{
		for (i=0;i<m_SolLayoutArr.GetSize();i++)
		{
			m_GameList.AddTail(m_SolLayoutArr[i].m_GameName);
			// für Test
			/*
			CString GameName = m_SolLayoutArr[i].m_GameName;
			ASSERT(m_SolLayoutArr[i].m_LevelPointArray.GetSize() == TTT_LEVELS);
			*/

		}
	}
}

CDocument* CSelectDoc::NewGame(CString &GameName,CTTTDoc* pTTTDoc /* = NULL */)
{
	CCyberSolitaireDoc* pDoc = GetRunningSolitaireGame();
	if (pDoc)
	{
		AfxMessageBox(IDS_GAME_IS_RUNNING,MB_OK | MB_ICONINFORMATION);
		return NULL;
	}
	GetApplication()->SetGameName(GameName);

	//CStatistic TmpStatistic;
	//CStatistic* pStatistic = &TmpStatistic;
	CStatistic* pStatistic = NULL;
	// die Statistik wird bei TTT Spielen nicht mitgeführt
	if (!pTTTDoc)
	{
		if (!m_pMapGameNameToStatistic->Lookup(GameName,pStatistic))
		{
			// Fehler
			ASSERT(0);
		}
	}
	if (!GameName.IsEmpty())
	{
		pDoc = (CCyberSolitaireDoc*) (GetApplication()->pCyberSolitaireTemplate)->OpenDocumentFile(NULL);
		pDoc->InitGame(m_SolLayoutArr[FindGame(GameName)],pStatistic,pTTTDoc);
		pDoc->GetView()->UpdateBitmaps();
		pDoc->UpdateAllViews(NULL,HINT_DISPLAY_ALL,NULL);
	}
	else
	{
		pDoc = (CCyberSolitaireDoc*) (GetApplication()->pCyberSolitaireTemplate)->OpenDocumentFile(NULL,FALSE);
	}

	return (CDocument*)pDoc;
}


void CSelectDoc::OnOpenGameeditor() 
{
	CSolEditDoc* pDoc = GetRunningSolEditor();
	if (pDoc)
	{
		AfxMessageBox(IDS_EDITOR_IS_RUNNING,MB_OK | MB_ICONINFORMATION);
		return;
	}
	ASSERT(!m_pWorkingLayout);
	CString DocFileName = GetView()->m_pGameTree->m_SelectedGame;
	if (DocFileName.IsEmpty())
	{
		m_pWorkingLayout = new CSolLayout();
		pDoc = (CSolEditDoc*) (GetApplication()->pSolEditTemplate)->OpenDocumentFile(NULL);
	}
	else
	{
		m_pWorkingLayout = new CSolLayout(m_SolLayoutArr[FindGame(DocFileName)]);
		pDoc = (CSolEditDoc*) (GetApplication()->pSolEditTemplate)->OpenDocumentFile(DocFileName);
	}
	if (!pDoc)
		return;
	pDoc->GetView()->UpdateBitmaps();
	pDoc->UpdateAllViews(NULL,HINT_DISPLAY_ALL,NULL);
}

void CSelectDoc::OnUpdateOpenGameeditor(CCmdUI* pCmdUI) 
{
 	pCmdUI->Enable(!GetRunningSolitaireGame());
}

CSelectView* CSelectDoc::GetView()
{
	return m_pView;
}

BOOL CSelectDoc::LoadSolLayoutArray(CString PathName)
{
	ASSERT(!m_SolLayoutArr.GetSize());
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(PathName,CFile::modeRead);
		CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
		m_SolLayoutArr.Serialize(loadArchive);
		//Serialize(loadArchive);     // load me
		loadArchive.Close();
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		if (pFile)
		{
			pFile->Close();
			delete pFile;
		}

		ReportSaveLoadException(PathName, e,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

BOOL CSelectDoc::SaveSolLayoutArray(CString PathName)
{
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(PathName,CFile::modeCreate|CFile::modeWrite);
		CArchive storeArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
		m_SolLayoutArr.Serialize(storeArchive);
		//Serialize(storeArchive);     // store me
		storeArchive.Close();
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		if (pFile)
		{
			pFile->Close();
			delete pFile;
		}
		ReportSaveLoadException(PathName, e,FALSE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH_ALL
	
	return TRUE;
}

BOOL CSelectDoc::LoadPlayerArray(CString PathName)
{
	ASSERT(!m_PlayerArr.GetSize());
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(PathName,CFile::modeRead);
		CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
		m_PlayerArr.Serialize(loadArchive);
		//Serialize(loadArchive);     // load me
		loadArchive.Close();
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		if (pFile)
		{
			pFile->Close();
			delete pFile;
		}

		ReportSaveLoadException(PathName, e,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	END_CATCH_ALL
	return TRUE;
	/*
	// wird erst mal wieder fallengelassen
	if (OpenDataBase())
	{
		m_PlayerArr.ReadDB(m_pCybSolDB);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	*/
}

BOOL CSelectDoc::SavePlayerArray(CString PathName)
{
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(PathName,CFile::modeCreate|CFile::modeWrite);
		CArchive storeArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
		m_PlayerArr.Serialize(storeArchive);
		//Serialize(storeArchive);     // store me
		storeArchive.Close();
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		if (pFile)
		{
			pFile->Close();
			delete pFile;
		}
		ReportSaveLoadException(PathName, e,FALSE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH_ALL
	
	return TRUE;
}

BOOL CSelectDoc::LoadMapGameNameToStatistic(CString PathName)
{
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(PathName,CFile::modeRead);
		CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
		loadArchive >> (CMapStringToOb*&)m_pMapGameNameToStatistic;
		//m_MapGameNameToStatistic.Serialize(loadArchive);
		//Serialize(loadArchive);     // load me
		loadArchive.Close();
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		if (pFile)
		{
			pFile->Close();
			delete pFile;
		}

		ReportSaveLoadException(PathName, e,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

BOOL CSelectDoc::SaveMapGameNameToStatistic(CString PathName)
{
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(PathName,CFile::modeCreate|CFile::modeWrite);
		CArchive storeArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
		storeArchive << m_pMapGameNameToStatistic;
		//m_MapGameNameToStatistic.Serialize(storeArchive);
		//Serialize(storeArchive);     // store me
		storeArchive.Close();
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		if (pFile)
		{
			pFile->Close();
			delete pFile;
		}
		ReportSaveLoadException(PathName, e,FALSE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH_ALL
	
	return TRUE;
}

int CSelectDoc::FindGame(CString &GameName)
{
	int i = -1;;
	if (m_SolLayoutArr.GetSize())
	{
		for (i=0;i<m_SolLayoutArr.GetSize();i++)
		{
			if (m_SolLayoutArr[i].m_GameName == GameName)
				return i;
		}
	}
	return -1;
}

void CSelectDoc::UpdateGameTree()
{
	m_GameList.RemoveAll();
	FillGameList();
	m_pView->m_pGameTree->InitGameTree();
	m_pView->Invalidate();
}

void CSelectDoc::OnGamecontextDeleteGame() 
{
	CString GameName = GetView()->m_pGameTree->m_SelectedGame;
	if (AfxMessageBox(IDS_DO_YOU_WANT_REALLY_DELETE,MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
	m_SolLayoutArr.RemoveAt(FindGame(GameName));
	CString PathName = GetApplication()->m_FullFilePath;
	PathName += "GAMES.DAT";
	SaveSolLayoutArray(PathName);
	UpdateGameTree();
}

void CSelectDoc::OnUpdateGamecontextDeleteGame(CCmdUI* pCmdUI) 
{
	CString GameName = GetView()->m_pGameTree->m_SelectedGame;
	pCmdUI->Enable((GameName != _T("")) && !GetRunningSolitaireGame() && !GetRunningSolEditor());	
}


// füllt die Map mit neuen Elementen, falls sie noch fehlen
BOOL CSelectDoc::CompleteMapGameNameToStatistic(void)
{
	BOOL Completed = FALSE;
	POSITION pos = NULL;
	pos = m_GameList.GetHeadPosition();
	CString GameName("");
	//CStatistic TmpStatistic;
	//CStatistic* pStatistic = &TmpStatistic;
	CStatistic* pStatistic = NULL;
	if (!m_pMapGameNameToStatistic) m_pMapGameNameToStatistic = new CMapGameNameToStatistic;

	while (pos)
	{
		GameName = m_GameList.GetNext(pos);
		//pStatistic = &TmpStatistic;
		if (!m_pMapGameNameToStatistic->Lookup(GameName,pStatistic))
		{
			pStatistic = new CStatistic;
			m_pMapGameNameToStatistic->SetAt(GameName,pStatistic);
			Completed = TRUE;
		}
	}
	return Completed;
}

// Score im Statusbar updaten
//
void CSelectDoc::UpdateScoreInStatusBar(int Score)
{
	CString strScore;
	strScore.Format("Ergebnis:%d",Score);
	SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_SCORE, (long)&strScore);
}

// GameName im Statusbar updaten
//
void CSelectDoc::UpdateGameInStatusBar(CString& GameName)
{
	SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_GAME_NAME, (long)&GameName);
}

// Spieler im Statusbar updaten
//
void CSelectDoc::UpdatePlayerInStatusBar(CPlayer& Player)
{
	CString strText;
	strText.Format("%s/%s",Player.m_NickName,Player.m_Rank);
	SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_PLAYER, (long)&strText);
}


void CSelectDoc::OnGamecontextStatistik() 
{
	CStatisticDoc* pDoc = NULL;
	// hier wird festgestellt, ob schon ein Statistik Fenster geöffnet ist
	POSITION pos = (GetApplication()->pStatisticTemplate)->GetFirstDocPosition();
	// es gibt maximal 1 Statistic Fenster -> if und nicht while
	if (pos != NULL)
		(GetApplication()->pStatisticTemplate)->GetNextDoc(pos)->OnCloseDocument();
	CString DocFileName = GetView()->m_pGameTree->m_SelectedGame;
	pDoc = (CStatisticDoc*)GetApplication()->pStatisticTemplate->OpenDocumentFile(DocFileName);
}

void CSelectDoc::OnUpdateGamecontextStatistik(CCmdUI* pCmdUI) 
{
	CString GameName = GetView()->m_pGameTree->m_SelectedGame;
	//pCmdUI->Enable((GameName != _T("")) && !GetRunningSolitaireGame() && !GetRunningSolEditor());	
	pCmdUI->Enable((GameName != _T("")) && !GetRunningSolEditor());	
}
// zu_tun muß wieder raus

struct MEGASOL_DATA {
	int				NbrChars;
	char			GameName[40];
	int				Cnt1;
	int				Cnt2;
	int				Nbr1;
	int				Nbr2;
	int				LevelPoint[9];
};

void CSelectDoc::OnEinstellungenUebernehmetttwerte()
{
	return;

	int NbrGames;
	int Nbr0;
	MEGASOL_DATA	Data;
	// File MEGASOL_GAMES.DAT auslesen
	CString FileName = GetApplication()->m_FullFilePath;
	FileName += "MEGASOL_GAMES.DAT";
	CString GameName("");
	CFile theFile;
	if ( theFile.Open(FileName,CFile::modeRead) != 0 )
	{
		CArchive archive(&theFile, CArchive::load);
		//Serialize(archive);
		archive >> NbrGames;
		archive >> Nbr0;
		for (int i=0; i<NbrGames; i++)
		{
			int n=0;
			archive >> Data.NbrChars;
			for (n=0; n<Data.NbrChars;n++)
				archive >> Data.GameName[n];
			Data.GameName[n] ='\0';
			GameName = Data.GameName;
			archive >> Data.Cnt1;
			archive >> Data.Cnt2;
			archive >> Data.Nbr1;
			archive >> Data.Nbr2;
			for (int j=0; j<9; j++)
			{
				archive >> Data.LevelPoint[j];
			}
			// suche das entsprechende Spiel in Cybersolitaire
			if (FindGame(GameName) != -1)
			{
				CSolLayout* pSolLayout = &m_SolLayoutArr[FindGame(GameName)];
				for (int j=0; j<9; j++)
				{
					pSolLayout->m_LevelPointArray[j] = Data.LevelPoint[j];
				}
				//m_SolLayoutArr[FindGame(GameName)] = SolLayout;
			}
		}
		archive.Close();
		CString PathName = GetApplication()->m_FullFilePath;
		PathName += "GAMES.DAT";
		SaveSolLayoutArray(PathName);

		theFile.Close();
	}

}

BOOL CSelectDoc::OpenDataBase()
{
	try
	{
		m_pCybSolDB = new CDaoDatabase(m_pDaoWorkSpace);
		m_pCybSolDB->Open(m_DBName);

	}
	catch (CDaoException* err)
	{
		DisplayDaoException(err);
		err->Delete();
		delete m_pCybSolDB;
		m_pCybSolDB = NULL;
		return FALSE;
	}
	return TRUE;
}

