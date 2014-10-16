// TTTView.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "MainFrm.h"
#include "SelectDoc.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "TTTDialogs.h"
#include "TTTField.h"
#include "TTTDoc.h"
#include "TTTView.h"
#include ".\tttview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Macros

#define PREPARE_TTT_VIEW	CTTTDoc* pDoc = GetDocument();\
								CClientDC dc(this);\
								OnPrepareDC(&dc);\
								CPoint LogPoint(point);\
								dc.DPtoLP(&LogPoint);\

/////////////////////////////////////////////////////////////////////////////
// CTTTView

IMPLEMENT_DYNCREATE(CTTTView, CView)

CTTTView::CTTTView()
: m_OldFoundLevel(0)
{
	m_BackGround.Load(AfxGetApp()->m_hInstance,IDB_BACKGROUND1);
	m_Screen.Load(AfxGetApp()->m_hInstance,IDB_BACKGROUND1);
	m_pScreen = NULL;
	m_pBackground = NULL;
	m_VisibleRect.top = 0;
	m_VisibleRect.left = 0;
	m_VisibleRect.right = TABLE_CX; 
	m_VisibleRect.bottom = TABLE_CY;
	m_UpdateRect.SetRectEmpty();
}

CTTTView::~CTTTView()
{
	if (m_pScreen)
		delete m_pScreen;
	if (m_pBackground)
		delete m_pBackground;
}


BEGIN_MESSAGE_MAP(CTTTView, CView)
	//{{AFX_MSG_MAP(CTTTView)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_USER_TTT_GAME_FINISHED, OnTTTGameFinished)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Zeichnung CTTTView 

void CTTTView::OnDraw(CDC* pDC)
{
	CTTTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect UpdateRect =((CPaintDC*)pDC)->m_ps.rcPaint; 
	CRect UpdateRect1(UpdateRect);
	pDC->DPtoLP((CRect*)&UpdateRect);
	// zu_tun
	// ZU ERLEDIGEN: Code zum Zeichnen hier einfügen
	//CDibs Dibs;
	for (int i=0; i<TTT_LEVELS; i++)
	{
		pDoc->m_TTTLevelArray[i].Display(pDC,m_Dibs);
	}
	if (m_pScreen)
		m_pScreen->Draw(*pDC,&UpdateRect,&UpdateRect1,FALSE);
	//pDC->Rectangle(m_VisibleRect);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);
	/*
	pDC->Rectangle(m_VisibleRect);
	*/
}

/////////////////////////////////////////////////////////////////////////////
// Diagnose CTTTView

#ifdef _DEBUG
void CTTTView::AssertValid() const
{
	CView::AssertValid();
}

void CTTTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTTTDoc* CTTTView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTTTDoc)));
	return (CTTTDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CTTTView 

void CTTTView::OnInitialUpdate() 
{
	CTTTDoc* pDoc = GetDocument();
	// View pointer besetzen
	pDoc->m_pView = this;	
	// initiaisieren des UpdateRect
	m_UpdateRect.SetRectEmpty();
	CView::OnInitialUpdate();
	if (pDoc->m_WithCurrentGameRunning)
	{
		//ShowWindow(SW_HIDE);
		//pDoc->m_pCurrentSolitaireGame->GetView()->ShowWindow(SW_SHOW);
		//pDoc->m_pCurrentSolitaireGame->UpdateAllViews(NULL,HINT_DISPLAY_ALL,NULL);
		//return;
	}

}

void CTTTView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	pDC->SetMapMode(MM_ISOTROPIC);
    m_VisibleRect.NormalizeRect();
	GetClientRect(&m_DeviceRect);
    m_DeviceRect.NormalizeRect();

	pDC->SetWindowOrg(m_VisibleRect.left,m_VisibleRect.top);
	pDC->SetWindowExt(m_VisibleRect.right - m_VisibleRect.left,m_VisibleRect.bottom - m_VisibleRect.top);
	
	pDC->SetViewportOrg(m_DeviceRect.left,m_DeviceRect.top);
	if(!m_DeviceRect.IsRectEmpty())
		pDC->SetViewportExt(m_DeviceRect.right - m_DeviceRect.left,m_DeviceRect.bottom - m_DeviceRect.top);
}

void CTTTView::SetDibs(CDib* pCardBackDib) 
{
	m_Dibs.pScreenDib = m_pScreen;
	m_Dibs.pBackgroundDib = m_pBackground;
	m_Dibs.pCardBackDib = NULL;
	m_Dibs.pCardSelectedDib = NULL;
}

void CTTTView::OnSize(UINT nType, int cx, int cy) 
{
	//TRACE("CTTTView::OnSize\n");
	CWaitCursor wait;
	// eventuell vorhandene Screen und Background Bitmaps löschen
	Delete(m_pScreen);
	Delete(m_pBackground);

	CClientDC dc(this);
	OnPrepareDC(&dc);
	CRect ClientRect; GetClientRect(&ClientRect);
	
	// Screen und Background Dib mit der aktuellen Größe erzeugen
	m_pScreen = new CDib;
	m_pScreen->Create(dc,ClientRect.Size(),&m_BackGround);
	m_pBackground = new CDib;
	m_pBackground->Create(dc,ClientRect.Size(),&m_BackGround);
	SetDibs(&GetApplication()->m_dibCardBack);
	/*
	if (IsMainWndSizing())
		return;
	else
	{
		UpdateBitmaps();
		OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
	}
	OnUpdate(this,HINT_DISPLAY_ALL_PILES,NULL);
	*/
}

/////////////////////////////////////////////////////////////////////////////
// löscht eine CDib, falls vorhanden
//
void CTTTView::Delete(CDib *pDib)
{
	if (pDib)
	{
		pDib->DeleteObject();
		delete pDib;
		pDib = NULL;
	}
}


void CTTTView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// zu_tun
	CTTTDoc* pDoc = GetDocument();
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CTTTField* pField = NULL;
	CTTTLevel* pLevel = NULL;
	CRect LevelRect;
	switch (lHint) 
	{
    case HINT_DISPLAY_TTT_FIELD:
		pField = (CTTTField*)pHint;
		
		// das zu löschende Rechteck berechnen
		m_UpdateRect = pField->m_FieldRect;
		m_UpdateRect.InflateRect(2,2);
		dc.LPtoDP(&m_UpdateRect);
		m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
		pField->Display(&dc,m_Dibs);
		RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		break;

    case HINT_DISPLAY_TTT_LEVEL:
		pLevel = (CTTTLevel*)pHint;
		
		// das zu löschende Rechteck berechnen
		LevelRect.SetRect(pLevel->m_LevelPosition,pLevel->m_LevelPosition+TTTLevelSize);
		m_UpdateRect = LevelRect;
		m_UpdateRect.InflateRect(2,2);
		dc.LPtoDP(&m_UpdateRect);
		m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
		//pField->Display(&dc,m_Dibs);
		RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		break;

	default:
		Invalidate();
		break;
	}

	/*
	for (int i=0; i<TTT_LEVELS; i++)
	{
		pDoc->m_TTTLevelArray[i].Display(&dc,m_Dibs);
	}
	RedrawWindow(NULL,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
	*/
	/*
	CPile* pPile;
	POSITION pos;
	switch (lHint) {
    case HINT_DISPLAY_ALL_PILES:
		// sofern es überhaupt Piles gibt
		if(!pDoc->m_PileList.IsEmpty())
		{
			// das zu löschende Rechteck ist die Vereinigung aller Pile Rects
			m_UpdateRect.SetRectEmpty();
			pos = pDoc->m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = pDoc->m_PileList.GetNext(pos);
				//CRect PileRect(pPile->GetPosition(),pPile->GetSize());
				CRect PileRect(pPile->GetPosition(),pPile->GetMaxSize());
				if (m_UpdateRect.IsRectNull())
					m_UpdateRect = PileRect;
				else
					m_UpdateRect.UnionRect(m_UpdateRect,PileRect);
			}
			m_UpdateRect.InflateRect(2,2);
			dc.LPtoDP(&m_UpdateRect);
			m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
			// alle Pile des Documents zeichnen (in die Screen Bitmap)
			pos = pDoc->m_PileList.GetHeadPosition();
			while (pos)
			{
				pPile = pDoc->m_PileList.GetNext(pos);
				pPile->Display(&dc,m_Dibs);
			}
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		}
		break;
	case HINT_DISPLAY_PILE:
		{
			pPile = (CPile*)pHint;
			if (!pPile)
				return;
			//CRect PileRect(pPile->GetPosition(),pPile->GetSize());
			CRect PileRect(pPile->GetPosition(),pPile->GetMaxSize());
			m_UpdateRect = PileRect;
			m_UpdateRect.InflateRect(2,2);
			dc.LPtoDP(&m_UpdateRect);
			m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
			// Pile des Documents zeichnen (in die Screen Bitmap)
			pPile->Display(&dc,m_Dibs);
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		}
		break;
    case HINT_DISPLAY_MOVEPILE:
	case HINT_UPDATE_PILES:
		{
			// sofern es überhaupt Piles gibt
			if(!pDoc->m_PileList.IsEmpty())
			{
				CPileList	PileList;
				pos = pDoc->m_PileList.GetHeadPosition();
				while (pos)
				{
					pPile = pDoc->m_PileList.GetNext(pos);
					CRect PileRect(pPile->GetPosition(),pPile->GetSize());
					CRect TmpRect(0,0,0,0);
					PileRect.NormalizeRect();
					m_UpdateRect.NormalizeRect();
					TmpRect.IntersectRect(&PileRect,&m_UpdateRect);
					if (!TmpRect.IsRectEmpty())
					{
						//m_UpdateRect.UnionRect(m_UpdateRect,PileRect);
						PileList.AddTail(pPile);
						//TRACE("Get pile %p\n",pPile);
					}
				}
				m_UpdateRect.NormalizeRect();
				if (!m_UpdateRect.IsRectEmpty())
				{
					m_UpdateRect.InflateRect(2,2);
					//if (!this->IsK(CMovePile))
					//	TRACE("(%d,%d,%d,%d)\n",m_UpdateRect.left,m_UpdateRect.top,m_UpdateRect.right,m_UpdateRect.bottom);
					dc.LPtoDP(&m_UpdateRect);
					m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect,&m_UpdateRect);
				}
				// alle betroffenen Piles zeichnen (in die Screen Bitmap)
				pos = PileList.GetHeadPosition();
				while (pos)
				{
					pPile = PileList.GetNext(pos);
					pPile->Display(&dc,m_Dibs);
					//TRACE("Display pile %p\n",pPile);
				}
			}
			pDoc->m_pMovePile->Display(&dc,m_Dibs);
			RedrawWindow(&m_UpdateRect,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
			m_UpdateRect.SetRectEmpty();

		}
		break;
    case HINT_DISPLAY_ALL:
		GetClientRect(m_UpdateRect);
		m_pScreen->CopyBitmap(dc,m_pBackground,&m_UpdateRect);
		// alle Pile des Documents zeichnen (in die Screen Bitmap)
		pos = pDoc->m_PileList.GetHeadPosition();
		while (pos)
		{
			pPile = pDoc->m_PileList.GetNext(pos);
			pPile->Display(&dc,m_Dibs);
		}
		RedrawWindow(NULL,NULL,RDW_UPDATENOW | RDW_INVALIDATE);
		break;
	default:
		Invalidate();
		//TRACE("OnUpdate");
		break;
	}
	m_UpdateRect.SetRectEmpty();
	// zu_tun
	loops++;
	finish = clock();
	difftime = finish - start;
	sumtime += difftime;
	avetime = sumtime / loops;
	*/
}

void CTTTView::OnMouseMove(UINT nFlags, CPoint point)
{
	//TRACE("OnMouseMove\n");
	PREPARE_TTT_VIEW

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CStatusBar* pStatusBar = &pMainFrame->m_wndStatusBar;

	// Nichts tun wenn keine Bewegung über Gridpunkt hinweg
	// könnte das moven beschleunigen
	if ( abs(LogPoint.x - m_LastPoint.x) < GRID && abs (LogPoint.y - m_LastPoint.y) < GRID )
	{
		return;
	}

	BOOL	FieldFound = FALSE;
	BOOL	LevelFound = FALSE;
	int		FoundLevel = -1;
	CTTTField* pFoundField = NULL;
	for (int i=0; i<TTT_LEVELS; i++)
	{
		// in den Feldern m_MouseOverField zurücksetzen
		for (int j=0; j<TTT_FIELDS; j++)
			pDoc->m_TTTLevelArray[i].m_TTTFieldArray[j].m_MouseOverField = FALSE;

		if ((pDoc->m_TTTLevelArray[i].m_Status != levelForbidden) && pDoc->m_TTTLevelArray[i].HitTest(LogPoint))
		{
			LevelFound = TRUE;
			FoundLevel = i;

			for (int j=0; j<TTT_FIELDS; j++)
			{
				if (pDoc->m_TTTLevelArray[i].m_TTTFieldArray[j].HitTest(LogPoint))
				{
					//TRACE("Mouse über Level,Feld %d,%d\n",i,j);
					FieldFound = TRUE;
					pFoundField = &pDoc->m_TTTLevelArray[i].m_TTTFieldArray[j];
					pFoundField->m_MouseOverField = TRUE;
					break;
				}
			}
			if (FieldFound)
				break;
		}
	}

	if (LevelFound)
	{
		if (FoundLevel != -1)
			OnUpdate(this,HINT_DISPLAY_TTT_LEVEL,&pDoc->m_TTTLevelArray[FoundLevel]);
		if (FieldFound)
		{
			for (int j=0; j<pFoundField->m_TTTGameArray.GetSize();j++)
			{
				CTTTGame* pTTTGame = &pFoundField->m_TTTGameArray[j];
				CString Text("");
				// Text um die erforderlichen Punkte erweitern
				int LevelPoints = pDoc->GetSelectDoc()->m_SolLayoutArr.GetAt(pTTTGame->m_GameIndex).m_LevelPointArray[pDoc->m_CurrentLevel];
				Text.Format("%s (%d)",pTTTGame->m_GameName,LevelPoints);
				pDoc->UpdateGameInStatusBar(Text,j+1);
			}
		}
		else
		{
			TRACE("UpdateGameInStatusBar leer\n");
			for (int j=0; j<TTT_GAMES;j++)
				pDoc->UpdateGameInStatusBar(CString(""),j+1);
		}
	}
	else
	{
		for (int i=0; i<TTT_LEVELS; i++)
			OnUpdate(this,HINT_DISPLAY_TTT_LEVEL,&pDoc->m_TTTLevelArray[i]);
		pMainFrame->SwitchToTTTStatusBar();
	}

	m_LastPoint = LogPoint;

}

void CTTTView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.
	PREPARE_TTT_VIEW

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CStatusBar* pStatusBar = &pMainFrame->m_wndStatusBar;

	BOOL	FieldFound = FALSE;
	BOOL	LevelFound = FALSE;
	int		CurrentField = -1;
	int		CurrentLevel = -1;
	CTTTField* pFoundField = NULL;
	for (int i=0; i<TTT_LEVELS; i++)
	{
		if (pDoc->m_TTTLevelArray[i].HitTest(LogPoint))
		{
			LevelFound = TRUE;
			for (int j=0; j<TTT_FIELDS; j++)
			{
				if (pDoc->m_TTTLevelArray[i].m_TTTFieldArray[j].HitTest(LogPoint))
				{
					if (pDoc->m_CurrentLevel == i)
					{
						//TRACE("Mouse über Level,Feld %d,%d\n",i,j);
						FieldFound = TRUE;
						pFoundField = &pDoc->m_TTTLevelArray[i].m_TTTFieldArray[j];
						// prüfen,ob dieses Feld frei bespielbar ist
						if (pFoundField->m_Status != fieldEmpty)
						{
							MessageBeep(0);
							return;
						}
						CurrentLevel = i;
						CurrentField = j;
					}
					break;
				}
			}
			if (FieldFound)
				break;
		}
	}

	CTTTGame* pTTTGame = NULL;
	if (FieldFound)
	{
		GetApplication()->LogMessage("TTT Choose Game Dialog started");
		CTTTChooseGameDialog TTTChooseGameDlg;
		pTTTGame = &pFoundField->m_TTTGameArray[0];
		TTTChooseGameDlg.m_Game1 = pTTTGame->m_GameName;
		TTTChooseGameDlg.m_strNeededScore1.Format("%d",
			pDoc->GetSelectDoc()->m_SolLayoutArr.GetAt(pTTTGame->m_GameIndex).m_LevelPointArray[pDoc->m_CurrentLevel]);
		pTTTGame = &pFoundField->m_TTTGameArray[1];
		TTTChooseGameDlg.m_Game2 = pTTTGame->m_GameName;
		TTTChooseGameDlg.m_strNeededScore2.Format("%d",
			pDoc->GetSelectDoc()->m_SolLayoutArr.GetAt(pTTTGame->m_GameIndex).m_LevelPointArray[pDoc->m_CurrentLevel]);
		pTTTGame = &pFoundField->m_TTTGameArray[2];
		TTTChooseGameDlg.m_Game3 = pTTTGame->m_GameName;
		TTTChooseGameDlg.m_strNeededScore3.Format("%d",
			pDoc->GetSelectDoc()->m_SolLayoutArr.GetAt(pTTTGame->m_GameIndex).m_LevelPointArray[pDoc->m_CurrentLevel]);
		if (TTTChooseGameDlg.DoModal() == IDOK)
		{
			// zu_tun
			// Überprüfen, ob das Spiel für TTT geeignet ist
			pDoc->m_pCurrentSolLayout = &pDoc->GetSelectDoc()->m_SolLayoutArr[pDoc->GetSelectDoc()->FindGame(TTTChooseGameDlg.m_ChoosedGameName)];
			BOOL TTTSuitable = (pDoc->m_pCurrentSolLayout->m_LevelPointArray[pDoc->m_CurrentLevel] > 0);
			if (!TTTSuitable)
			{
				AfxMessageBox(IDS_NOT_TTT_SUITABLE,MB_YESNO|MB_ICONSTOP);
				CView::OnLButtonDown(nFlags, point);
				pDoc->m_pCurrentSolLayout = NULL;
				pDoc->m_CurrentTTTGameArrayIndex = -1;
				pDoc->m_pCurrentTTTField = NULL;
				pDoc->m_CurrentField = -1;
				pDoc->m_CurrentCyberSolitaireGameScore = -1;
				return;
			}
			pMainFrame->SwitchToGameStatusBar();

			pDoc->m_pCurrentTTTField = pFoundField;
			pDoc->m_CurrentTTTGameArrayIndex = TTTChooseGameDlg.m_ChoosedGame -1;
			pDoc->m_CurrentCyberSolitaireGameScore = -1;
			ShowWindow(SW_HIDE);
			pDoc->m_pCurrentSolitaireGame = 
				(CCyberSolitaireDoc*)pDoc->GetSelectDoc()->NewGame(TTTChooseGameDlg.m_ChoosedGameName,pDoc);
			ASSERT(pDoc->m_CurrentLevel == CurrentLevel);
			pDoc->m_CurrentField = CurrentField;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

LRESULT CTTTView::OnTTTGameFinished (WPARAM wParam, LPARAM lParam)
{
	CTTTDoc* pDoc = GetDocument();
	// diese Benachrichtigung wird beim Schließen des aktuellen TTT Spiels empfangen
	// ab hier ist dann wieder das TTT Fenster zu sehen
	ShowWindow(SW_SHOW);
	pDoc->m_pCurrentSolitaireGame = NULL;
	ASSERT(pDoc->m_pCurrentSolLayout != NULL);
	ASSERT(pDoc->m_CurrentTTTGameArrayIndex != -1);
	ASSERT(pDoc->m_pCurrentTTTField != NULL);
	ASSERT(pDoc->m_CurrentField != -1);
	ASSERT(pDoc->m_CurrentCyberSolitaireGameScore != -1);
	// zu_tun
	// hier kann jetzt also die Auswertung des gespielten Spiels geschehen
	// die Auswertung muß feststellen, ob das Spiel gewonnen oder verloren wurde
	// davon abhängig muß das gespielte Feld ein o oder ein x bekommen
	// danach muß überprüft werden, ob eventuell der gespielte Level beendet wurde
	// dadurch kann sich eventuell der Level erhöhen oder das Spiel ist verloren
	// zu_tun Versuch
	BOOL PlayerWins = pDoc->PlayerWinsCurrentSolitaireGame();
	if (PlayerWins)
	{
		pDoc->m_pCurrentTTTField->m_Status = fieldCross;
	}
	else
	{
		pDoc->m_pCurrentTTTField->m_Status = fieldCircle;
	}
	// eventuell das TTT Spiel automatisch speichern
	if (pDoc->m_bAutoSave)
	{
		pDoc->AutoSaveTTTGame();
	}

	enum LevelChangeValues WinsLevel = pDoc->ProofLevelChanged();
	switch (WinsLevel)
	{
	case PlayerWinsLevel:
		//AfxMessageBox("Player wins Level",MB_OK);
		if (pDoc->m_CurrentLevel < TTT_LEVELS-1)
		{
			// der Level muß um 1 erhöht werden
			pDoc->m_TTTLevelArray[pDoc->m_CurrentLevel].m_Status = levelDone;
			pDoc->m_CurrentLevel++;
			pDoc->m_TTTLevelArray[pDoc->m_CurrentLevel].m_Status = levelPermitted;
			pDoc->FillLevelWithGames();
		}
		else
		{
			// zu_tun
			AfxMessageBox("Glückwunsch! Sie habendas Tic-Tac_Toe gewonnen",MB_OK);
		}

		break;
	case ComputerWinsLevel:	
		//AfxMessageBox("Computer wins Level",MB_OK);
		break;
	case LevelNotChanged:
		//AfxMessageBox("Level unchanged",MB_OK);
		break;
	default:
		ASSERT(0);
	}
	return NULL;
}
void CTTTView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// zu_tun
	// muß wieder raus
	// diese Funktion simuliert ein ausgewähltes Spiel
	// und dient zum Test des TTT
	//return;
	PREPARE_TTT_VIEW

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CStatusBar* pStatusBar = &pMainFrame->m_wndStatusBar;

	BOOL	FieldFound = FALSE;
	BOOL	LevelFound = FALSE;
	int		CurrentField = -1;
	int		CurrentLevel = -1;
	CTTTField* pFoundField = NULL;
	for (int i=0; i<TTT_LEVELS; i++)
	{
		if (pDoc->m_TTTLevelArray[i].HitTest(LogPoint))
		{
			LevelFound = TRUE;
			for (int j=0; j<TTT_FIELDS; j++)
			{
				if (pDoc->m_TTTLevelArray[i].m_TTTFieldArray[j].HitTest(LogPoint))
				{
					if (pDoc->m_CurrentLevel == i)
					{
						//TRACE("Mouse über Level,Feld %d,%d\n",i,j);
						FieldFound = TRUE;
						pFoundField = &pDoc->m_TTTLevelArray[i].m_TTTFieldArray[j];
						// prüfen,ob dieses Feld frei bespielbar ist
						if (pFoundField->m_Status != fieldEmpty)
						{
							MessageBeep(0);
							return;
						}
						CurrentLevel = i;
						CurrentField = j;
					}
					break;
				}
			}
			if (FieldFound)
				break;
		}
	}

	CTTTGame* pTTTGame = NULL;
	if (FieldFound)
	{

		// Überprüfen, ob das Spiel für TTT geeignet ist
		pDoc->m_pCurrentSolLayout = &pDoc->GetSelectDoc()->m_SolLayoutArr[pDoc->GetSelectDoc()->FindGame(CString("test"))];
		for (int i=0; i<pDoc->m_pCurrentSolLayout->m_LevelPointArray.GetSize();i++)
			pDoc->m_pCurrentSolLayout->m_LevelPointArray[i] = i+1;

		if (AfxMessageBox("Spiel gewonnen?",MB_YESNO) == IDYES)
			pDoc->m_CurrentCyberSolitaireGameScore = pDoc->m_CurrentLevel+1;
		else
			pDoc->m_CurrentCyberSolitaireGameScore = 0;

		pMainFrame->SwitchToGameStatusBar();

		pDoc->m_pCurrentTTTField = pFoundField;
		pDoc->m_CurrentTTTGameArrayIndex = 1;
		//ShowWindow(SW_HIDE);
		pDoc->m_pCurrentSolitaireGame = NULL;
		ASSERT(pDoc->m_CurrentLevel == CurrentLevel);
		pDoc->m_CurrentField = CurrentField;
		// TTT benachrichtigen, daß das Spiel beendet wurde
		SendMessage(WM_USER_TTT_GAME_FINISHED,0,0L);
		return;
	}

	CView::OnRButtonDown(nFlags, point);
}
