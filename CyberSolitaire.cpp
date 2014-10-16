// CyberSolitaire.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafxCLR.h"
#include "CyberSolitaire.h"

#include "DockViewBar.h"
#include "MainFrm.h"
#include "CyberSolitaireFrame.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "SolEditFrame.h"
#include "SolEditDoc.h"
#include "SolEditView.h"
#include "TTTField.h"
#include "TTTFrame.h"
#include "TTTDoc.h"
#include "TTTView.h"
#include "SelectDoc.h"
#include "SelectView.h"
#include "HTMLHelp.h"
#include "BackgroundDialog.h"
#include "DebugProperties.h"
#include "PlayerDialogs.h"
#include "StatisticFrame.h"
#include "StatisticDoc.h"
#include "StatisticView.h"
#include "TTTDoc.h"
#include ".\cybersolitaire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// constants
//

const CPoint	NoPosition(-1000,-1000);
const CSize		NoSize(-1,-1);
const CRect		NoRect(NoPosition,NoSize);
const CPoint	ZeroPoint(0,0);
const CSize		ZeroSize(0,0);
const CRect		ZeroRect(0,0,0,0);

// für die ganze Application konstante Default Werte
const	CSize	CCyberSolitaireApp::DefaultCardSize(CARD_CX,CARD_CY); 
const	double	CCyberSolitaireApp::DefaultOverlappedFactor = 0.2; 
const	CSize	CCyberSolitaireApp::DefaultRightOverlappedSize((int)(DefaultOverlappedFactor * CARD_CX),CARD_CY); 
const	CSize	CCyberSolitaireApp::DefaultDownOverlappedSize (CARD_CX,(int)(DefaultOverlappedFactor * CARD_CY)); 
int				CCyberSolitaireApp::m_CouldNotOpenFile = 0;

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireApp

BEGIN_MESSAGE_MAP(CCyberSolitaireApp, CWinApp)
	//{{AFX_MSG_MAP(CCyberSolitaireApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_DATEI_GAMEEDITOR, OnDateiGameEditor)
	ON_UPDATE_COMMAND_UI(ID_DATEI_GAMEEDITOR, OnUpdateDateiGameEditor)
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	// Standard-Druckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_EINSTELLUNGEN_KARTENUNDHINTERGRUND, OnEinstellungenKartenundhintergrund)
	ON_COMMAND(ID_EINSTELLUNGEN_DEBUGEINSTELLUNGEN, OnEinstellungenDebugeinstellungen)
	ON_COMMAND(ID_SPIEL_TICTACTOELADEN, OnSpielTictactoeLaden)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireApp Konstruktion

CCyberSolitaireApp::CCyberSolitaireApp()
: m_GameName(_T(""))
, m_ClipCursor(FALSE)
, m_ReportGameLayouts(FALSE)
, m_FullFilePath(_T(""))
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
	m_pSemaphore = new CSemaphore(1,1,"CyberSol");
	m_pLock = new CSingleLock(m_pSemaphore);
#if (_MFC_VER >= 0x0700)
	EnableHtmlHelp( );
#endif
}

CCyberSolitaireApp::~CCyberSolitaireApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CCyberSolitaireApp-Objekt

CCyberSolitaireApp theApp;
/*
#ifdef _DEBUG
CDbgWnd theDbgWnd;      // debug window
#endif // _DEBUG
int theDbgLevel;        // debug level
*/

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireApp Initialisierung

BOOL CCyberSolitaireApp::InitInstance()
{
	if (!m_pLock->Lock(100))
	{
		AfxMessageBox(IDS_ONLY_ONE_INSTANCE,MB_OK | MB_ICONSTOP);
		return FALSE;
	}
#ifdef _DEBUG
    // Create the debug window
    //theDbgWnd.Create();
    //dprintf("Started");
	afxDump.SetDepth(1);
	// zu_tun
	/*
	HWND hTraceWnd = FindWindow("MfxTraceWindow","TRACEWIN");
	if (!hTraceWnd)
		WinExec("TRACEWIN.EXE",SW_SHOW);
	else
		ShowWindow(hTraceWnd,SW_SHOW);
	*/
#endif // _DEBUG

	AfxEnableControlContainer();

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Größe Ihrer fertigen 
	//  ausführbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht benötigen, entfernen.

#ifdef _AFXDLL
	//Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif

	//----------- in dieses File werden alle Benutzeraktivitäten (für Fehlersuche) geschrieben
	m_LogFileName =  "CyberSolitaire.log";
	m_LogFileSize = 1400000L;
	
	// Ändern des Registrierungsschlüssels, unter dem unsere Einstellungen gespeichert sind.
	// ZU ERLEDIGEN: Sie sollten dieser Zeichenfolge einen geeigneten Inhalt geben
	// wie z.B. den Namen Ihrer Firma oder Organisation.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	char FilePath[_MAX_PATH];
	CString IniFile;
	GetModuleFileName(NULL,FilePath,_MAX_PATH);
	m_FullFilePath = FilePath;
	int Index = m_FullFilePath.ReverseFind('\\');
	m_FullFilePath = m_FullFilePath.Left(Index+1);
	IniFile = FilePath;
	Index = IniFile.ReverseFind('.');
	IniFile = IniFile.Left(Index+1);
	IniFile += "INI";
	m_pszProfileName=_tcsdup(IniFile);
	LoadStdProfileSettings();  // Standard INI-Dateioptionen laden (einschließlich MRU)

	// bestimmte, für alle Spiele gleichbleibende Eigenschaften festlegen
	//m_CardSize.cx = CARD_CX;							// logische Grösse der Karten
	//m_CardSize.cy = CARD_CY;
	VERIFY(m_dibCardBack.Load(m_hInstance,IDB_BACK1));	// Kartenrücken
	//m_OverlappedFactor = 0.2;							// Faktor für Überlappung
	
	m_MainMenu.LoadMenu(IDR_MAINFRAME);

	CString INISection;
	INISection.LoadString(IDS_SECTION_PROPERTIES);
	m_CardBackID   = GetProfileInt( INISection, "CARD_BACK", IDB_BACK1);			
	m_BackgroundID = GetProfileInt( INISection, "GAME_BACKGROUND", IDB_BACKGROUND1);			
	INISection.LoadString(IDS_SECTION_DEBUG);
	m_ClipCursor   = GetProfileInt( INISection, "CLIPCURSOR", 1);			
	m_ReportGameLayouts   = GetProfileInt( INISection, "REPORTGAMELAYOUTS", 0);			

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.

	//---------------------------------- register select card
	UINT bw, bh, px, py, type, docked;
	CString Param;
	type = 0xE808;
	Param.Format("DOC%dheight",type);
	Param.MakeUpper();
	bh	= GetProfileInt( "WINDOWS", Param, 100);			
	Param.Format("DOC%dwidth",type);
	Param.MakeUpper();
	bw	= GetProfileInt( "WINDOWS", Param, 300);			
	Param.Format("DOC%dposx",type);
	Param.MakeUpper();
	px	= GetProfileInt( "WINDOWS", Param, 50);			
	Param.Format("DOC%dposy",type);
	Param.MakeUpper();
	py	= GetProfileInt( "WINDOWS", Param, 150);			
	Param.Format("DOC%ddocked",type);
	Param.MakeUpper();
	docked	= GetProfileInt( "WINDOWS", Param,1 );			
	
	pSelectSolitaireTemplate = new CSizeBarDocTemplate(	IDR_SELECTFRAME,
		RUNTIME_CLASS(CSelectDoc) ,
		//RUNTIME_CLASS(CChildFrame),
		NULL,
		RUNTIME_CLASS(CSelectView),
		CBRS_LEFT ,
		0xE808, bw, bh, 
		docked, CPoint( px,py) );
	pSelectSolitaireTemplate->m_hMenuShared = m_MainMenu;
	AddDocTemplate(pSelectSolitaireTemplate);

	//---------------------------------- register CyberSolitaire document
	pCyberSolitaireTemplate = new CSharedTemplate(	IDR_CYBSOLTYPE,
		RUNTIME_CLASS(CCyberSolitaireDoc) ,
		RUNTIME_CLASS(CCyberSolitaireFrame),
		RUNTIME_CLASS(CCyberSolitaireView)		);
	AddDocTemplate(pCyberSolitaireTemplate);
	
	//---------------------------------- register SolEdit document
	pSolEditTemplate = new CSharedTemplate(	IDR_SOLEDITTYPE,
		RUNTIME_CLASS(CSolEditDoc) ,
		RUNTIME_CLASS(CSolEditFrame),
		RUNTIME_CLASS(CSolEditView)		);
	AddDocTemplate(pSolEditTemplate);
	
	//---------------------------------- register TTT document
	pTTTGameTemplate = new CSharedTemplate(	IDR_TTT_TYPE,
		RUNTIME_CLASS(CTTTDoc) ,
		RUNTIME_CLASS(CTTTFrame),
		RUNTIME_CLASS(CTTTView)		);
	AddDocTemplate(pTTTGameTemplate);

	//---------------------------------- register Statistic document
	pStatisticTemplate = new CSingleDocTemplate(	IDR_STATISTICTYPE,
		RUNTIME_CLASS(CStatisticDoc) ,
		RUNTIME_CLASS(CStatisticFrame),
		RUNTIME_CLASS(CStatisticView));
	AddDocTemplate(pStatisticTemplate);
	
	// Haupt-MDI-Rahmenfenster erzeugen
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Öffnen per DragDrop aktivieren
	m_pMainWnd->DragAcceptFiles();

	// DDE-Execute-Open aktivieren
	//EnableShellOpen();
	//RegisterShellFileTypes(TRUE);

	// Befehlszeile parsen, um zu prüfen auf Standard-Umgebungsbefehle DDE, Datei offen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ohne öffnen des ersten Game Dokumentes starten
	if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileOpen)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Verteilung der in der Befehlszeile angegebenen Befehle
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Das Hauptfenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	//pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	
	pSelectSolitaireTemplate->OpenDocumentFile( NULL );

	INISection.LoadString(IDS_SECTION_CURRENTS);
	m_CurrentPlayerName = GetProfileString( INISection, "PLAYER", "unbekannt");			
	m_GameName          = GetProfileString( INISection, "GAME", NULL);	
	m_MaxMix			= GetProfileInt( INISection, "MAXMIX",MAX_MIX);

	CSelectPlayerDialog SelectPlayerDlg(NULL,IDS_GREETING_TEXT,IDS_OK_PLAY);
	SelectPlayerDlg.m_pSelectDoc = GetSelectDoc();
	SelectPlayerDlg.m_SelName = m_CurrentPlayerName;
	SelectPlayerDlg.DoModal();

	m_CurrentPlayerName = SelectPlayerDlg.m_SelName;
	ASSERT(SelectPlayerDlg.m_ArrIndex != -1);
	m_CurrentPlayer = GetSelectDoc()->m_PlayerArr[SelectPlayerDlg.m_ArrIndex];
	ASSERT(m_CurrentPlayerName == m_CurrentPlayer.m_NickName);
	GetSelectDoc()->UpdatePlayerInStatusBar(m_CurrentPlayer);
	WriteProfileString( INISection, "PLAYER", m_CurrentPlayerName);	
	return TRUE;
}


CSelectDoc* CCyberSolitaireApp::GetSelectDoc()
{
	POSITION pos = pSelectSolitaireTemplate->GetFirstDocPosition();
	// wir können davon ausgehen, daß es ein oder kein CProjDoc gibt
	CSelectDoc* pSelectDoc = NULL;
	if (pos)
		pSelectDoc = (CSelectDoc*)pSelectSolitaireTemplate->GetNextDoc(pos);
	return pSelectDoc;

}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialog für Info über Anwendung

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogdaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_Version;
	//}}AFX_DATA

	// Überladungen für virtuelle Funktionen, die vom Anwendungs-Assistenten erzeugt wurden
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_Version = _T("CyberSolitaire ");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate)
	{
#ifdef DEV_VERSION
		m_Version =	_T("CyberSolitaire Dev ");
#else
		m_Version =	_T("CyberSolitaire ");
#endif
		CString Version;
		Version.LoadString(IDS_PROGRAM_VERSION);
		m_Version += Version;
	}
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VERSION, m_Version);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CCyberSolitaireApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireApp-Nachrichtenbehandlungsroutinen

int CCyberSolitaireApp::ExitInstance() 
{
#ifdef _DEBUG
    //theDbgWnd.DestroyWindow();
	HWND hTraceWnd = FindWindow("MfxTraceWindow","TRACEWIN");
	if (hTraceWnd)
		ShowWindow(hTraceWnd,SW_SHOWMINIMIZED);
#endif // _DEBUG

	delete m_pLock;
	delete m_pSemaphore;
	// Kartenrücken löschen
	m_dibCardBack.DeleteObject();
	return CWinApp::ExitInstance();
}

///////////////////////////////////////////////////////////////////////////////

#pragma unmanaged

bool CCyberSolitaireApp::LogMessage(UINT MessageID,...)
{
	CString	FormatText;
	CString MessageText;
	if (!FormatText.LoadString(MessageID))
	{
		MessageBeep(-1L);
		TRACE("Text not found in resource ID = %ld\n",MessageID);
		MessageText.LoadString(IDS_TEXT_NOT_FOUND);
	}
	else
	{
		va_list	argp;
		va_start (argp, MessageID);
		MessageText.FormatV((LPCTSTR)FormatText,argp);
		va_end (argp);
	}
	return LogMessage(MessageText);
}

///////////////////////////////////////////////////////////////////////////////

bool CCyberSolitaireApp::LogMessage(LPCSTR Message,...)
{
	CString	FormatText;
	CString MessageText;
	FormatText = Message;
	va_list	argp;
	va_start (argp, Message);
	MessageText.FormatV((LPCTSTR)FormatText,argp);
	va_end (argp);
	return LogMessage(MessageText);
}	

///////////////////////////////////////////////////////////////////////////////

bool CCyberSolitaireApp::LogMessage(CString Message,...)
{
	CFile			LogFile;
	CFileException	e;
	CString			Buffer;
	//DWORD			FileSize;
	ULONGLONG		FileSize;
	DWORD			DelCharNumber;
			                    
	CString	FormatText;
	CString MessageText;
	FormatText = Message;
	va_list	argp;
	va_start (argp, Message);
	MessageText.FormatV((LPCTSTR)FormatText,argp);
	va_end (argp);

	// ersetzen von Zeilenumbrüchen	
	MessageText.Replace('\n',' ');
	CString OutStr = MessageText;
	
	// es wird ohne File gearbeitet
	if (m_LogFileName.IsEmpty())
		return TRUE;

	//----------- open the file for read/write and if it doesn't exist create it
	if( LogFile.Open( m_LogFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite,&e ) == FALSE )
	{
		#ifdef _DEBUG
		   afxDump << "File could not be opened " << e.m_cause << "\n";
		#endif
		m_CouldNotOpenFile++;
		//OutStr = "Could not create Log-File " + m_LogFileName;
		//AfxMessageBox(OutStr, MB_OK, 0);
		return FALSE;
	}

	
	// Text zusammenbauen
	CTime timeWrite;   
	timeWrite = CTime::GetCurrentTime();
	// write the time out
	CString DateStr = timeWrite.Format("%d.%m.%Y %H:%M:%S : ");
	OutStr = DateStr + OutStr + "\r\n";
	FileSize = LogFile.GetLength() + OutStr.GetLength();        // get length of file after write

	// check if maximum size of file is reached ( default = Floppyspace )
	// if reached delete the first part of the file ( 30 % )
	DelCharNumber = m_LogFileSize / 3;
	if (FileSize > m_LogFileSize)
	{
		// wir lesen ca. 1/3 des alten LogFiles
		DelCharNumber = m_LogFileSize / 3;
		LogFile.Seek(DelCharNumber,CFile::begin);
		LPSTR pBuffer = Buffer.GetBufferSetLength( m_LogFileSize );
		// finde das nächste Stringende
		LogFile.Read( (void *)(LPCSTR)Buffer, 2048 );		                               
        int CharCnt = 0;
        while (*pBuffer != '\n')
        {
        	pBuffer++;
        	CharCnt++;
        }
       	CharCnt++;
		LogFile.Seek(DelCharNumber+CharCnt,CFile::begin);
		// obsolete
		//DWORD BytesRead = LogFile.ReadHuge( (void *)(LPCSTR)Buffer, m_LogFileSize );		                               
		DWORD BytesRead = LogFile.Read( (void *)(LPCSTR)Buffer, m_LogFileSize );		                               
		// nun kopieren wir um
		LogFile.Seek( 0, CFile::begin);
		// obsolete
		//LogFile.WriteHuge( Buffer, BytesRead );		                               
		LogFile.Write( Buffer, BytesRead );		                               
		LogFile.Flush();
		LogFile.SetLength(BytesRead);
	}

	// Text rausschreiben
	FileSize = LogFile.GetLength();        // get length of file
	LogFile.Seek(0,CFile::end);
	LogFile.Write( OutStr, OutStr.GetLength() );		                               
	if (m_CouldNotOpenFile)
	{
		OutStr = "Error-->kein Zugriff auf Log-File Anzahl: ";
		OutStr.Format("%s%d",OutStr,m_CouldNotOpenFile);
		OutStr = DateStr + OutStr + "\r\n";
		m_CouldNotOpenFile = 0;
		LogFile.Write( OutStr, OutStr.GetLength() );
	}

	LogFile.Close();
	
	return TRUE;
}	

#pragma managed

void CCyberSolitaireApp::OnFileOpen()
{
	if (GetRunningSolitaireGame())
	{
		// zuerst muß das laufende Spiel beendet werden
		CString Message;
		Message.LoadString(IDS_EXIT_RUNNING_GAME);
		AfxMessageBox(IDS_EXIT_RUNNING_GAME,MB_OK | MB_ICONINFORMATION);
		return; // open cancelled
	}

	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, pCyberSolitaireTemplate))
		return; // open cancelled

	AfxGetApp()->OpenDocumentFile(newName);
		// if returns NULL, the user has already been alerted
}

/////////////////////////////////////////////////////////////////////////////
// Helpers
//
extern "C" CCyberSolitaireApp* GetApplication(void)
{
	return (CCyberSolitaireApp*)AfxGetApp();
}


extern "C" CCyberSolitaireDoc* GetRunningSolitaireGame(void)
{
	CCyberSolitaireDoc* pCyberSolitaireDoc = NULL;
	POSITION pos = GetApplication()->pCyberSolitaireTemplate->GetFirstDocPosition();
	// es darf nur ein Spiel aktiv sein
	int OnlyOneActive = 0;
	while (pos != NULL)
	{
		pCyberSolitaireDoc = (CCyberSolitaireDoc*)GetApplication()->pCyberSolitaireTemplate->GetNextDoc(pos);
		OnlyOneActive++;
	}
	ASSERT(OnlyOneActive <= 1);
	return pCyberSolitaireDoc;
}

extern "C" CSolEditDoc* GetRunningSolEditor(void)
{
	CSolEditDoc* pSolEditDoc = NULL;
	POSITION pos = GetApplication()->pSolEditTemplate->GetFirstDocPosition();
	// es darf nur ein Spiel aktiv sein
	int OnlyOneActive = 0;
	while (pos != NULL)
	{
		pSolEditDoc = (CSolEditDoc*)GetApplication()->pSolEditTemplate->GetNextDoc(pos);
		OnlyOneActive++;
	}
	ASSERT(OnlyOneActive <= 1);
	return pSolEditDoc;
}

extern "C" CTTTDoc* GetRunningTTTGame(void)
{
	// zu tun
	//return NULL;
	CTTTDoc* pTTTDoc = NULL;
	POSITION pos = GetApplication()->pTTTGameTemplate->GetFirstDocPosition();
	// es darf nur ein Spiel aktiv sein
	int OnlyOneActive = 0;
	while (pos != NULL)
	{
		pTTTDoc = (CTTTDoc*)GetApplication()->pTTTGameTemplate->GetNextDoc(pos);
		OnlyOneActive++;
	}
	ASSERT(OnlyOneActive <= 1);
	return pTTTDoc;
}


void CCyberSolitaireApp::WinHelp(DWORD dwData, UINT nCmd) 
{
#if (_MFC_VER >= 0x0700)
	CString HTMLHelpFile = GetApplication()->m_pszHelpFilePath;
	int Index = HTMLHelpFile.ReverseFind('.');
	HTMLHelpFile = HTMLHelpFile.Left(Index+1);
	HTMLHelpFile += "CHM";
#endif

	switch (nCmd)
	{
	case HELP_KEY:
		HH_AKLINK link;
		
		link.cbStruct =     sizeof(HH_AKLINK) ;
		link.fReserved =    FALSE ;
		link.pszKeywords =  (char *)dwData ; 
		link.pszUrl =       NULL ; 
		link.pszMsgText =   NULL ; 
		link.pszMsgTitle =  NULL ; 
		link.pszWindow =    NULL ;
		link.fIndexOnFail = TRUE ;

#if (_MFC_VER < 0x0700)
		HtmlHelp(
			GetDesktopWindow(),
			HTMLHelpFile,
			HH_KEYWORD_LOOKUP,
			(DWORD) &link) ;
#else
		HtmlHelp(
			(DWORD) &link,
			HH_KEYWORD_LOOKUP) ;
#endif
		break;
	
	case HELP_CONTEXT: 
#if (_MFC_VER < 0x0700)
		HtmlHelp(
			GetDesktopWindow(),
			HTMLHelpFile,
			HH_HELP_CONTEXT,
			dwData) ;
#else
		HtmlHelp(
			dwData,
			HH_HELP_CONTEXT) ;
#endif
		break;

	case HELP_INDEX:
	default:
		// ACHTUNG !!! Die API Beschreibung stimmt nicht !!!
		// HH_DISPLAY_TOC und HH_DISPLAY_TOPIC haben n i c h t dieselbe Funktionalität
#if (_MFC_VER < 0x0700)
		CString Topic;
		Topic = HTMLHelpFile + "::/IDH_1main_inhalt.htm";
		HtmlHelp(
			GetDesktopWindow(),
			HTMLHelpFile,
			HH_DISPLAY_TOC,
			NULL);

		// aud die Startseite schalten
		HtmlHelp(
			GetDesktopWindow(),
			(LPCSTR)Topic,
			HH_DISPLAY_TOPIC,
			NULL) ;
#else
		HtmlHelp(
			NULL,
			HH_DISPLAY_TOC);

		// auf die Startseite schalten
		HtmlHelp(
			HH_DISPLAY_TOPIC,
			NULL) ;
#endif
		break;
	}
}

void CCyberSolitaireApp::OnDateiGameEditor() 
{
	CSolEditDoc* pDoc = GetRunningSolEditor();
	if (pDoc)
	{
		AfxMessageBox(IDS_EDITOR_IS_RUNNING,MB_OK | MB_ICONINFORMATION);
		return;
	}
	pDoc = (CSolEditDoc*) (GetApplication()->pSolEditTemplate)->OpenDocumentFile(NULL);
	pDoc->GetView()->UpdateBitmaps();
	pDoc->UpdateAllViews(NULL,HINT_DISPLAY_ALL_PILES,NULL);
	/*
	CString newName("");
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, pSolEditTemplate))
		return; // open cancelled

	AfxGetApp()->OpenDocumentFile(newName);
	*/
	// if returns NULL, the user has already been alerted
}

void CCyberSolitaireApp::OnUpdateDateiGameEditor(CCmdUI* pCmdUI) 
{
 	//pCmdUI->Enable(!GetRunningSolitaireGame());
}


// baut einen GameName zusammen
void CCyberSolitaireApp::SetGameName(CString& GameName)
{
	/*
	CString TmpName(GameName);
	CString DateTime;
	COleDateTime Time = COleDateTime::GetCurrentTime();
	DateTime = Time.Format("%d%m%y_%H%M%S");
	m_GameName = TmpName + "_" + DateTime;
	*/
	m_GameName = GameName;
	GetSelectDoc()->UpdateGameInStatusBar(GameName);
}

void CCyberSolitaireApp::OnEinstellungenKartenundhintergrund()
{
	CBackgroundDialog BackgroundDlg;
	m_CardBackID   = GetProfileInt( "PROPERTIES", "CARD_BACK", IDB_BACK1);			
	m_BackgroundID = GetProfileInt( "PROPERTIES", "GAME_BACKGROUND", IDB_BACKGROUND1);			
	BackgroundDlg.m_IDCardBackground = m_CardBackID;
	BackgroundDlg.m_IDGameBackground = m_BackgroundID;
	if (BackgroundDlg.DoModal() == IDOK)
	{
		m_CardBackID   = BackgroundDlg.m_IDCardBackground;
		m_BackgroundID = BackgroundDlg.m_IDGameBackground;
		m_dibCardBack.DeleteObject();
		m_dibCardBack.Load(m_hInstance,m_CardBackID);	// Kartenrücken
		CCyberSolitaireDoc* pDoc = GetRunningSolitaireGame();
		if (pDoc)
		{
			pDoc->GetView()->m_BackGround.DeleteObject();
			pDoc->GetView()->m_Screen.DeleteObject();
			pDoc->GetView()->m_BackGround.Load(m_hInstance,m_BackgroundID);
			pDoc->GetView()->m_Screen.Load(m_hInstance,m_BackgroundID);
			pDoc->GetView()->UpdateScreenAndBackground();
			WriteProfileInt( "PROPERTIES", "CARD_BACK", m_CardBackID);			
			WriteProfileInt( "PROPERTIES", "GAME_BACKGROUND", m_BackgroundID);			
		}
	}
}

void CCyberSolitaireApp::RenameLogFileInErrorCase()
{
	CString DateTimeVersion;
	COleDateTime Time = COleDateTime::GetCurrentTime();
	DateTimeVersion = Time.Format("%d$%m$%y_%H$%M$%S");
	CString NewLogFileName(m_LogFileName);
	int Index = NewLogFileName.ReverseFind('.');
	NewLogFileName = NewLogFileName.Left(Index);
	NewLogFileName += DateTimeVersion;
	NewLogFileName += ".LOG";
	CFile			LogFile;
	try
	{
		CFile::Rename( m_LogFileName, NewLogFileName );
	}
	catch( CFileException* err )
	{
		#ifdef _DEBUG
			afxDump << "File " << m_LogFileName << " not found, cause = "
				<< err->m_cause << "\n";
		#endif
		err->Delete();
	}
}

CDocument* CCyberSolitaireApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	CDocument* pDoc = CWinApp::OpenDocumentFile(lpszFileName);
	if (pDoc->IsK(CTTTDoc))
	{
		SendMessage(AfxGetMainWnd()->m_hWnd,WM_USER_TTT_GAME_LOADED,0,0L);
	}
	return pDoc;
}

void CCyberSolitaireApp::ErrorReport(CString &ErrorDescription)
{
	CString DateTimeVersion;
	COleDateTime Time = COleDateTime::GetCurrentTime();
	DateTimeVersion = Time.Format("%d$%m$%y_%H$%M$%S");
	CString ErrorFileName(_T("Error_"));
	ErrorFileName += DateTimeVersion;
	LogMessage("%s:\t%s",ErrorFileName,ErrorDescription);
	CString Version;
	Version.LoadString(IDS_PROGRAM_VERSION);
	LogMessage("CyberSolitaire %s",Version);
	// läuft gerade ein Spiel ???
	CCyberSolitaireDoc* pDoc = GetRunningSolitaireGame();
	if (pDoc)
	{
		CString ErrorGameName(ErrorFileName);
		ErrorGameName += "_";
		ErrorGameName += GetGameName();
		ErrorGameName += ".CYS";
		CString Title = pDoc->GetTitle();
		pDoc->DoSave(ErrorGameName,FALSE);
		pDoc->SetTitle(Title);
	}
	// rename the log-file
	ErrorFileName += ".LOG";
	CFile			LogFile;
	try
	{
		CFile::Rename( m_LogFileName, ErrorFileName );
	}
	catch( CFileException* err )
	{
		#ifdef _DEBUG
			afxDump << "File " << m_LogFileName << " not found, cause = "
				<< err->m_cause << "\n";
		#endif
		err->Delete();
	}
}

void CCyberSolitaireApp::OnEinstellungenDebugeinstellungen()
{
	CDebugPropertiesDialog DebugPropertiesDlg;
	CString INISection;
	INISection.LoadString(IDS_SECTION_DEBUG);
	m_ClipCursor   = GetProfileInt( INISection, "CLIPCURSOR", 1);			
	m_ReportGameLayouts   = GetProfileInt( INISection, "REPORTGAMELAYOUTS", 0);			
	DebugPropertiesDlg.m_ClipCursor = m_ClipCursor;
	DebugPropertiesDlg.m_ReportGameLayouts = m_ReportGameLayouts;
	if (DebugPropertiesDlg.DoModal() == IDOK)
	{
		m_ClipCursor = DebugPropertiesDlg.m_ClipCursor;
		WriteProfileInt( INISection, "CLIPCURSOR", m_ClipCursor);			
		m_ReportGameLayouts = DebugPropertiesDlg.m_ReportGameLayouts;
		WriteProfileInt( INISection, "REPORTGAMELAYOUTS", m_ReportGameLayouts);	
		// die Layouts werden hier auch gleich geschrieben
		if (m_ReportGameLayouts)
			GetSelectDoc()->m_SolLayoutArr.ReportProperties();
	}
}

void CCyberSolitaireApp::OnSpielTictactoeLaden()
{
	if (GetRunningSolitaireGame())
	{
		// zuerst muß das laufende Spiel beendet werden
		CString Message;
		Message.LoadString(IDS_EXIT_RUNNING_GAME);
		AfxMessageBox(IDS_EXIT_RUNNING_GAME,MB_OK | MB_ICONINFORMATION);
		return; // open cancelled
	}

	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, pTTTGameTemplate))
		return; // open cancelled

	CTTTDoc* pTTTDoc = (CTTTDoc*)AfxGetApp()->OpenDocumentFile(newName);
	// if returns NULL, the user has already been alerted
	SendMessage(AfxGetMainWnd()->m_hWnd,WM_USER_TTT_GAME_LOADED,0,0L);

}
void DisplayDaoException(CDaoException* pDaoException)
{
	CString szMsg;
	if (pDaoException->m_pErrorInfo!=NULL)
	{
		szMsg.Format(
			_T("%s: %s (%d)\n\n")
			_T("Would you like to see help?"),
			(LPCTSTR)pDaoException->m_pErrorInfo->m_strDescription,
			(LPCTSTR)pDaoException->m_pErrorInfo->m_strSource,
			pDaoException->m_pErrorInfo->m_lErrorCode);

		if (AfxMessageBox(szMsg, MB_YESNO) == IDYES)
		{
			WinHelp(GetDesktopWindow(),
					pDaoException->m_pErrorInfo->m_strHelpFile,
					HELP_CONTEXT,
					pDaoException->m_pErrorInfo->m_lHelpContext);
		}
	}
	else
	{
		switch(pDaoException->m_nAfxDaoError)
		{
		case AFX_DAO_ERROR_ENGINE_INITIALIZATION:
//			MFC could not initialize the Microsoft Jet database engine. OLE might have failed to initialize,
//			or it might have been impossible to create an instance of the DAO database engine object. 
//			These problems usually suggest a bad installation of either DAO or OLE.

			szMsg = "MFC could not initialize the Microsoft Jet database engine!\n"
					"Please Reinstall DAO to correct this problem";
			break;
		case AFX_DAO_ERROR_DFX_BIND:
//			An address used in a DAO record field exchange (DFX) function call does not exist or is invalid (the address was not used to bind data). You might have passed a bad address in a DFX call, 
//			or the address might have become invalid between DFX operations.

			szMsg = "MFC could not initialize the Microsoft Jet database engine!\n"
					"Please Reinstall DAO to correct this problem";
			break;
		case AFX_DAO_ERROR_OBJECT_NOT_OPEN:
			szMsg = "You attempted to open a recordset based on a querydef or a tabledef object that was not in an open state";
			break;		
		case NO_AFX_DAO_ERROR:
//			The most recent operation did not result in an MFC extended error. However, the operation could have produced other errors from DAO or OLE, 
//			so you should check m_pErrorInfo and possibly m_scode.
		default:
			szMsg.Format(
			_T("ERROR:CDaoException\n\n")
			_T("SCODE_CODE		=%d\n")	
			_T("SCODE_FACILITY	=%d\n")	
			_T("SCODE_SEVERITY	=%d\n")	
			_T("ResultFromScode	=%d\n"),
			SCODE_CODE		(pDaoException->m_scode),
			SCODE_FACILITY	(pDaoException->m_scode),
			SCODE_SEVERITY	(pDaoException->m_scode),
			ResultFromScode (pDaoException->m_scode));
		}
		AfxMessageBox(szMsg);
	}
}

