// TTTDoc.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "MainFrm.h"
#include "CyberSolitaireDoc.h"
#include "SelectDoc.h"
#include "TTTField.h"
#include "TTTView.h"
#include "TTTDoc.h"
#include ".\tttdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTTDoc

IMPLEMENT_DYNCREATE(CTTTDoc, CDocument)

CTTTDoc::CTTTDoc()
: m_pCurrentSolitaireGame(NULL)
, m_pCurrentSolLayout(NULL)
, m_CurrentTTTGameArrayIndex(-1)
, m_pCurrentTTTField(NULL)
, m_CurrentCyberSolitaireGameScore(0)
, m_bAutoSave(TRUE)
{
	m_PlayedGames = 0;
	m_CurrentLevel = 0;
	m_CurrentField = -1;	// noch kein aktuelles Feld
	m_WithCurrentGameRunning = FALSE;
	// Anzahl der Level bestimmt die Größe des Feldes
	m_TTTLevelArray.SetSize(TTT_LEVELS);

	// obere linke Ecke des ersten Level
	CPoint TmpPoint = CPoint(TTTLevelDistance.cx,TTTLevelDistance.cy);

	for (int i=0; i<m_TTTLevelArray.GetSize();i++)
	{
		// alle Level sind erst mal verboten
		m_TTTLevelArray[i].m_Status = levelForbidden;
		// LevelNumber setzen
		m_TTTLevelArray[i].m_LevelNumber = i+1;	// die Levelnummer zählt von 1 bis 9
		// Position des Level setzen
		TRACE("TTT Level Position(%d): (%d,%d)\n",i,TmpPoint.x,TmpPoint.y);
		m_TTTLevelArray[i].SetLevelPosition(TmpPoint);
		// Position des nächsten Level errechnen
		TmpPoint.x += (TTTLevelSize.cx + TTTLevelDistance.cx);     // nach rechts
		// jeweils 3 Level in einer Zeile
		if (((i+1) % 3) == 0)
		{
			TmpPoint.x = TTTLevelDistance.cx;
			TmpPoint.y += (TTTLevelSize.cy + TTTLevelDistance.cy);
		}
/*
#ifdef _DEBUG
	afxDump << "Dumping TTTLevel(";	afxDump << i; afxDump << "):\n";
	m_TTTLevelArray[i].Dump( afxDump );
	afxDump << "\n";
#endif
*/
	}
	// der erste Level darf immer gespielt werden
	m_TTTLevelArray[0].m_Status = levelPermitted;
}

BOOL CTTTDoc::OnNewDocument()
{
	srand((unsigned)time(NULL));
	if (!CDocument::OnNewDocument())
		return FALSE;
	// hier wird ein neues Tic Tac Toe vorbereitet
	// Anzahl der gespielten Spiele ist sicher 0
	m_PlayedGames = 0;
	// der erste Level darf immer gespielt werden
	m_TTTLevelArray[0].m_Status = levelPermitted;
	// nun brauchen wir für jedes freie Feld 3 Auswahlspiele
	// dazu erzeugen wir 3 verschiedene Indizes aus der Anzahl der Spiele
	CIntArray	RandomGamesIndex;
	RandomGamesIndex.SetSize(TTT_GAMES);

	GetApplication()->LogMessage("New TTT Game started");
	for (int i=0; i<m_TTTLevelArray[0].m_TTTFieldArray.GetSize();i++)
	{
		CTTTField* pTTTField = &m_TTTLevelArray[0].m_TTTFieldArray[i];
		ASSERT(pTTTField->GetFieldStatus() == fieldEmpty);
		GetRandomGamesIndexes(RandomGamesIndex);
		CTTTGame* pTTTGame = NULL;
		for (int j=0; j<pTTTField->m_TTTGameArray.GetSize();j++)
		{
			pTTTGame = &m_TTTLevelArray[0].m_TTTFieldArray[i].m_TTTGameArray[j];
			pTTTGame->m_GameIndex = RandomGamesIndex[j];
			pTTTGame->m_GameName = GetSelectDoc()->m_SolLayoutArr.GetAt(pTTTGame->m_GameIndex).m_GameName;
			TRACE("Game für Feld %d = %s\n",i,pTTTGame->m_GameName);
			GetApplication()->LogMessage("Game für Feld %d = %s\n",i,pTTTGame->m_GameName);
		}
	}
	m_CurrentLevel = 0;		// ein neues TTT beginnt mit dem 1. Level


	return TRUE;
}

void CTTTDoc::GetRandomGamesIndexes(CIntArray& IntArray)
{
	CSelectDoc* pSelectDoc = GetSelectDoc();
	int NbAllGames = pSelectDoc->m_GameList.GetSize();
	int i = 0; int j = 0;
	for (i=0; i<IntArray.GetSize();i++)
	{
		DoRepeat:
		IntArray[i] = rand() % NbAllGames;
		if (GetSelectDoc()->m_SolLayoutArr.GetAt(i).m_LevelPointArray[m_CurrentLevel] == 0)
			goto DoRepeat;
		for (j=0; j<i;j++)
		{
			if (IntArray[j] == IntArray[i])
				goto DoRepeat;
		}
	}
}
CTTTDoc::~CTTTDoc()
{
}


BEGIN_MESSAGE_MAP(CTTTDoc, CDocument)
	//{{AFX_MSG_MAP(CTTTDoc)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_SPIEL_TTTSPEICHERN, OnSpielTTTSpeichern)
	ON_COMMAND(ID_SPIEL_TTTSPEICHERNUNTER, OnSpielTTTSpeichernUnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Diagnose CTTTDoc

#ifdef _DEBUG
void CTTTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTTTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Serialisierung CTTTDoc 

void CTTTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_TTTDocVersion = TTTDOC_VERSION;
		ar << m_TTTDocVersion;
		ar << m_PlayedGames;
		ar << m_CurrentLevel;				
		ar << m_CurrentField;				
		ar << m_CurrentTTTGameArrayIndex;
		ar << m_CurrentCyberSolitaireGameScore;
		ar << m_WithCurrentGameRunning;
	}
	else
	{
		ar >> m_TTTDocVersion;
		ar >> m_PlayedGames;
		ar >> m_CurrentLevel;				
		ar >> m_CurrentField;				
		ar >> m_CurrentTTTGameArrayIndex;
		ar >> m_CurrentCyberSolitaireGameScore;
		ar >> m_WithCurrentGameRunning;
	}
	m_TTTLevelArray.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// Befehle CTTTDoc 

BOOL CTTTDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Fügen Sie Ihren spezialisierten Erstellcode hier ein.
	if (m_WithCurrentGameRunning)
	{
		CString newName = lpszPathName;
		int Index = newName.ReverseFind('.');
		newName = newName.Left(Index+1);
		newName += "CYS";
		m_pCurrentSolitaireGame = (CCyberSolitaireDoc*)AfxGetApp()->OpenDocumentFile(newName);
		m_pCurrentSolitaireGame->m_pTTTDoc = this;
		// besetzen der nicht serialisierten Werte des TTTDoc
		m_pCurrentSolLayout = 
			&GetSelectDoc()->m_SolLayoutArr[GetSelectDoc()->FindGame(m_pCurrentSolitaireGame->GetGameName())];
		m_pCurrentTTTField = &m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[m_CurrentField];
	}

	return TRUE;
}

CSelectDoc* CTTTDoc::GetSelectDoc()
{
	POSITION pos = GetApplication()->pSelectSolitaireTemplate->GetFirstDocPosition();
	// wir können davon ausgehen, daß es ein oder kein CProjDoc gibt
	CSelectDoc* pSelectDoc = NULL;
	if (pos)
		pSelectDoc = (CSelectDoc*)GetApplication()->pSelectSolitaireTemplate->GetNextDoc(pos);
	return pSelectDoc;
}

// GameName im TTT Field Statusbar updaten
//
void CTTTDoc::UpdateGameInStatusBar(CString& GameName, int FieldNumber)
{
	switch (FieldNumber)
	{
	case 1:
		SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_TTT_GAME1, (long)&GameName);
		break;
	case 2:
		SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_TTT_GAME2, (long)&GameName);
		break;
	case 3:
		SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_TTT_GAME3, (long)&GameName);
		break;
	default:
		ASSERT(0);
		break;
	}
}

// Spieler im Statusbar updaten
//
void CTTTDoc::UpdatePlayerInStatusBar(CPlayer& Player)
{
	CString strText;
	strText.Format("%s/%s",Player.m_NickName,Player.m_Rank);
	SendMessage( AfxGetMainWnd()->m_hWnd, WM_USER_PANE_UPDATE, ID_INDICATOR_PLAYER, (long)&strText);
}


void CTTTDoc::OnFileClose()
{
	TRACE("Entry CTTTDoc::OnFileClose\n");

	if (!SaveModified())
		return;

	// shut it down
	OnCloseDocument();
	// this should destroy the document
	TRACE("Exit  CTTTDoc::OnFileClose\n");
}

// prüft, wer das gerade beendete Spiel gewonnen hat
BOOL CTTTDoc::PlayerWinsCurrentSolitaireGame(void)
{
	BOOL bWinsTTTGame = FALSE;
	ASSERT(m_pCurrentSolLayout->m_LevelPointArray[m_CurrentLevel] > 0);
	bWinsTTTGame = m_CurrentCyberSolitaireGameScore >= m_pCurrentSolLayout->m_LevelPointArray[m_CurrentLevel];
	return bWinsTTTGame;
}

// überprüft, ob der Level gewechselt hat
enum LevelChangeValues CTTTDoc::ProofLevelChanged(void)
{
	enum LevelChangeValues WinsLevel = LevelNotChanged;
	if	(
		// Zeilen
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[0].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[1].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[2].m_Status == fieldCross) ||
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[3].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[5].m_Status == fieldCross) ||
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[6].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[7].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[8].m_Status == fieldCross) ||
		// Spalten
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[0].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[3].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[6].m_Status == fieldCross) ||
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[1].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[7].m_Status == fieldCross) ||
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[2].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[5].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[8].m_Status == fieldCross) ||
		// Diagonalen
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[0].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[8].m_Status == fieldCross) ||
		(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[2].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCross &&
		m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[6].m_Status == fieldCross)
		)
	{
		WinsLevel = PlayerWinsLevel;
	}
	else
		if (
			// Zeilen
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[0].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[1].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[2].m_Status == fieldCircle) ||
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[3].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[5].m_Status == fieldCircle) ||
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[6].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[7].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[8].m_Status == fieldCircle) ||
			// Spalten
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[0].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[3].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[6].m_Status == fieldCircle) ||
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[1].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[7].m_Status == fieldCircle) ||
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[2].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[5].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[8].m_Status == fieldCircle) ||
			// Diagonalen
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[0].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[8].m_Status == fieldCircle) ||
			(m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[2].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[4].m_Status == fieldCircle &&
			m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[6].m_Status == fieldCircle)
			)
			{
				WinsLevel = ComputerWinsLevel;
			}
	return WinsLevel;
}

// Füllt einen Level mit Auswahl-Spielen
void CTTTDoc::FillLevelWithGames(void)
{
	// nun brauchen wir für jedes freie Feld 3 Auswahlspiele
	// dazu erzeugen wir 3 verschiedene Indizes aus der Anzahl der Spiele
	CIntArray	RandomGamesIndex;
	RandomGamesIndex.SetSize(TTT_GAMES);

	GetApplication()->LogMessage("Fill LevelWithGames started");
	for (int i=0; i<m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray.GetSize();i++)
	{
		CTTTField* pTTTField = &m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[i];
		ASSERT(pTTTField->GetFieldStatus() == fieldEmpty);
		GetRandomGamesIndexes(RandomGamesIndex);
		CTTTGame* pTTTGame = NULL;
		for (int j=0; j<pTTTField->m_TTTGameArray.GetSize();j++)
		{
			pTTTGame = &m_TTTLevelArray[m_CurrentLevel].m_TTTFieldArray[i].m_TTTGameArray[j];
			pTTTGame->m_GameIndex = RandomGamesIndex[j];
			pTTTGame->m_GameName = GetSelectDoc()->m_SolLayoutArr.GetAt(pTTTGame->m_GameIndex).m_GameName;
			TRACE("Game für Feld %d = %s\n",i,pTTTGame->m_GameName);
			GetApplication()->LogMessage("Game für Feld %d = %s\n",i,pTTTGame->m_GameName);
		}
	}
}

BOOL CTTTDoc::DoFileSave()
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

BOOL CTTTDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
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

CString CTTTDoc::GetDateTimeVersion()
{
	CString DateTimeVersion;
	COleDateTime Time = COleDateTime::GetCurrentTime();
	DateTimeVersion = Time.Format("%d$%m$%y_%H$%M$%S");
	return DateTimeVersion;
}



void CTTTDoc::OnSpielTTTSpeichern()
{
	m_WithCurrentGameRunning = m_pCurrentSolitaireGame != NULL;
	ASSERT(!m_WithCurrentGameRunning);
	DoFileSave();
}

void CTTTDoc::OnSpielTTTSpeichernUnter()
{
	m_WithCurrentGameRunning = m_pCurrentSolitaireGame != NULL;
	ASSERT(!m_WithCurrentGameRunning);
	if(!DoSave(NULL))
		TRACE("Warning: File save-as failed.\n");
}

// speichert das TTT Game (mit automatisch erzeugtem Namen
BOOL CTTTDoc::AutoSaveTTTGame(void)
{
	CString GameName(_T(""));
	CreateAutoSaveGameName(GameName);
	return DoSave(GameName);
}

// erzeugt einen automatischen Namen für das Spiel zum abspeichern
void CTTTDoc::CreateAutoSaveGameName(CString& GameName)
{
	if (!GameName.IsEmpty())
		return;
	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT(pTemplate != NULL);
	GameName = m_strTitle;
	int Index = GameName.Find('_');
	if (Index != -1)
		GameName = GameName.Left(Index+1);
	GameName = GameName + "_" + GetDateTimeVersion();
	// check for dubious filename
	int iBad = GameName.FindOneOf(_T("#%;/\\"));
	if (iBad != -1)
		GameName.ReleaseBuffer(iBad);

	// append the default suffix if there is one
	CString strExt;
	if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
		!strExt.IsEmpty())
	{
		ASSERT(strExt[0] == '.');
		GameName += strExt;
	}
}
