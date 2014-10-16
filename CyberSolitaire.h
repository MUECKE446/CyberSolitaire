// CyberSolitaire.h : Haupt-Header-Datei für die Anwendung CYBERSOLITAIRE
//

#if !defined(AFX_CYBERSOLITAIRE_H__3D0B27BA_67E4_4343_8322_980BBC85A042__INCLUDED_)
#define AFX_CYBERSOLITAIRE_H__3D0B27BA_67E4_4343_8322_980BBC85A042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Hauptsymbole
//#include "dbgwnd.h"

#include "Piles.h"	// Hinzugefügt von der Klassenansicht

class CCyberSolitaireDoc;
class CSolEditDoc;
class CTTTDoc;
class CCard;
class CPile;
class CSelectDoc;
class CDo;
typedef CTypedPtrArray<CObArray, CDo*> CDoArray;

/////////////////////////////////////////////////////////////////////////////
// Macros


#ifndef _DEBUG
#define	PROGRAM_FAILURE(Bool) {\
		if (!(Bool)) {\
			CString TmpStr;\
			CString File(__FILE__); int i = File.ReverseFind('\\'); File = File.Right(File.GetLength()-i-1);\
			CString Version;\
			Version.LoadString(IDS_PROGRAM_VERSION);\
			TmpStr.Format(IDS_PROGRAM_FAILED, File, __LINE__, Version );\
			GetApplication()->LogMessage(TmpStr);\
			GetApplication()->RenameLogFileInErrorCase();\
			AfxMessageBox(TmpStr,MB_OK);\
		}\
	}
#else
#define	PROGRAM_FAILURE(Bool) {\
		if (!(Bool)) {\
			CString TmpStr;\
			CString File(__FILE__); int i = File.ReverseFind('\\'); File = File.Right(File.GetLength()-i-1);\
			CString Version;\
			Version.LoadString(IDS_PROGRAM_VERSION);\
			TmpStr.Format(IDS_PROGRAM_FAILED, File, __LINE__, Version );\
			GetApplication()->LogMessage(TmpStr);\
			GetApplication()->RenameLogFileInErrorCase();\
			ASSERT(Bool);\
		}\
	}
#endif

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants
//

extern const CPoint	NoPosition;
extern const CSize	NoSize;
extern const CRect	NoRect;
extern const CPoint	ZeroPoint;
extern const CSize	ZeroSize;
extern const CRect	ZeroRect;

#define WM_USER_PROGRESS_START		WM_USER+0x100
#define WM_USER_PROGRESS_UPDATE		WM_USER+0x101
#define WM_USER_PROGRESS_FINISHED	WM_USER+0x102
#define WM_USER_PANE_UPDATE			WM_USER+0x103

#define WM_USER_TTT_GAME_FINISHED	WM_USER+0x200
#define WM_USER_TTT_GAME_LOADED		WM_USER+0x201


// Hints für CCyberSolitaireDoc & ..View
#define HINT_DISPLAY_ALL_PILES		1
#define HINT_DISPLAY_PILE			2
#define HINT_DISPLAY_MOVEPILE		3
#define HINT_UPDATE_PILES			4
#define HINT_DISPLAY_ALL			5
#define HINT_DISPLAY_TTT_FIELD		6
#define HINT_DISPLAY_TTT_LEVEL		7

// logische Grösse des Tischs
#define TABLE_CX	1400	
#define TABLE_CY	1050	
//#define TABLE_CX	1600	
//#define TABLE_CY	1300	
//#define TABLE_CY	750	

// logische Grösse der Karten
#define CARD_CX		125
#define CARD_CY		180

// Grid Größe für den Game Editor
//#define	GRID		50
#define	GRID		5

#define	NA			-1
#define	ANY			0

// BASE COLORS (the 20 static colors of the default palette)
// Note: CLR_INVALID is defined in WinGdi.h as 0xffffffff
#define BLACK_CLR		PALETTEINDEX(0)
#define DARK_RED		PALETTEINDEX(1)
#define DARK_GREEN		PALETTEINDEX(2)
#define DARK_YELLOW		PALETTEINDEX(3)
#define DARK_BLUE		PALETTEINDEX(4)
#define DARK_MAGENTA	PALETTEINDEX(5)
#define DARK_CYAN		PALETTEINDEX(6)
#define LIGHT_GRAY		PALETTEINDEX(7)
#define MONEY_GREEN		PALETTEINDEX(8)
#define SKY_BLUE		PALETTEINDEX(9)
#define CREAM			PALETTEINDEX(10)
#define MEDIUM_GRAY		PALETTEINDEX(11)
#define DARK_GRAY		PALETTEINDEX(12)
#define RED_CLR			PALETTEINDEX(13)
#define GREEN			PALETTEINDEX(14)
#define YELLOW			PALETTEINDEX(15)
#define BLUE			PALETTEINDEX(16)
#define MAGENTA			PALETTEINDEX(17)
#define CYAN			PALETTEINDEX(18)
#define WHITE			PALETTEINDEX(19)

// wie soll gezählt werden
enum gameScoring {
	scoreNotAvailable					= 0,		// nicht verfügbar/relevant
	scoreSequenceInSuitAndFoundation	= 1,		// jede Karte in Farbfolge und ablegen zählt einen Punkt + Ablage auf Foundation
	scoreSequenceNoColorAndFoundation	= 2,		// jede Karte in Folge und ablegen zählt einen Punkt + Ablage auf Foundation
	scoreCardOnFoundation				= 3,		// jede Karte auf einem Ablagestapel zählt einen Punkt
	scoreSequenceInSuitAndKingUp		= 4			// jede Karte in Farbfolge und König nach oben legen zählt einen Punkt
};

//**********************************************************

UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);

/////////////////////////////////////////////////////////////////////////////
// typedefs
//
typedef	struct CDibs {
	CDib*	pScreenDib;
	CDib*	pBackgroundDib;
	CDib*	pCardBackDib;
	CDib*	pCardSelectedDib;
} CDibs;


/////////////////////////////////////////////////////////////////////////////
// weitere includes
//
#include "SolLayout.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////
// CCyberSolitaireApp:
// Siehe CyberSolitaire.cpp für die Implementierung dieser Klasse
//

class CCyberSolitaireApp : public CWinApp
{
public:
	CCyberSolitaireApp();
	~CCyberSolitaireApp();

public:
	CMultiDocTemplate*	pSelectSolitaireTemplate;
	CMultiDocTemplate*	pCyberSolitaireTemplate;
	CMultiDocTemplate*	pSolEditTemplate;
	CMultiDocTemplate*	pTTTGameTemplate;
	CSingleDocTemplate*	pStatisticTemplate;

	CMenu						m_MainMenu;

	// diese Werte sind default und damit constant
	static const CSize			DefaultCardSize;			// logische Kartengröße
	static const double			DefaultOverlappedFactor;	// Überlappungsfaktor (z.B. 0.2) (in % / 100)
	static const CSize			DefaultRightOverlappedSize;	// logische Überlappungsgröße einer Karte (nach rechts)
	static const CSize			DefaultDownOverlappedSize;	// logische Überlappungsgröße einer Karte (nach unten)
	
	CDib						m_dibCardBack;				// Kartenrücken
	
	CString						m_GameName;					// Name des aktuellen Spiels
	CString						m_CurrentPlayerName;		// Name des aktuellen Spielers
	CPlayer						m_CurrentPlayer;			// aktueller Spieler

	CString						m_FullFilePath;				// aktueller Full Path
	int							m_MaxMix;					// Anzahl beim Mischen (default 1000)

	// Debug Hilfen
	// schaltet das CursorClipping ein/aus
	BOOL m_ClipCursor;
	// schaltet das Ausgeben der Game Layouts ins Log-File ein/aus
	BOOL m_ReportGameLayouts;

protected:
	CString			m_LogFileName;
	UINT			m_LogFileSize;


public:
	void ErrorReport(CString& ErrorDescription);
	void RenameLogFileInErrorCase();
	int m_BackgroundID;
	int m_CardBackID;
	void OnFileOpen();
	bool LogMessage(UINT MessageID,...);
	bool LogMessage(LPCSTR Message,...);
	bool LogMessage(CString Message,...);
	CSelectDoc* GetSelectDoc(void);

	// baut einen GameName zusammen
	void SetGameName(CString& GameName);
	CString GetGameName() {return m_GameName;}
	CPlayer& GetCurrentPlayer() {return m_CurrentPlayer;}
	CString GetCurrentPlayerName() {return m_CurrentPlayerName;}

private:
	CSemaphore*			m_pSemaphore;
	CSingleLock*		m_pLock;

	static int			m_CouldNotOpenFile;

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CCyberSolitaireApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementierung
	//{{AFX_MSG(CCyberSolitaireApp)
	afx_msg void OnAppAbout();
	afx_msg void OnDateiGameEditor();
	afx_msg void OnUpdateDateiGameEditor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEinstellungenKartenundhintergrund();
	afx_msg void OnEinstellungenDebugeinstellungen();
	afx_msg void OnSpielTictactoeLaden();
};

/////////////////////////////////////////////////////////////////////////////
// Helpers
//
extern "C" CCyberSolitaireApp* GetApplication(void);
extern "C" CCyberSolitaireDoc* GetRunningSolitaireGame(void);
extern "C" CSolEditDoc* GetRunningSolEditor(void);
extern "C" CTTTDoc* GetRunningTTTGame(void);

void DisplayDaoException(CDaoException* pDaoException);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CYBERSOLITAIRE_H__3D0B27BA_67E4_4343_8322_980BBC85A042__INCLUDED_)
