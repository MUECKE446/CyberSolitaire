#if !defined(AFX_TTTDOC_H__5F1F52E3_0825_11D7_97ED_00A0CC562BEB__INCLUDED_)
#define AFX_TTTDOC_H__5F1F52E3_0825_11D7_97ED_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTTDoc.h : Header-Datei
//

#define TTTDOC_VERSION		1		// Version für Serialisierung
#define TTT_LEVELS			9		// Anzahl der Spielstufen

enum LevelChangeValues {
	PlayerWinsLevel		= 0,			// Spieler gewinnt den Level
	ComputerWinsLevel	= 1,			// Computer gewinnt den Level
	LevelNotChanged		= 2				// Level wurde noch nicht gewonnen
};

#define CIntArray			CArray<int,int>	

/////////////////////////////////////////////////////////////////////////////
// Dokument CTTTDoc 

class CTTTView;
class CSelectDoc;

class CTTTDoc : public CDocument
{
	friend class CTTTView;

protected:
	CTTTDoc();           // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CTTTDoc)

// Attribute
public:
	int						m_TTTDocVersion;			// Version für Serialisierung
	int						m_PlayedGames;				// Anzahl gespielter Spiele
	int						m_CurrentLevel;				// aktueller Level
	int						m_CurrentField;				// aktuelles Feld
	int						m_CurrentTTTGameArrayIndex;	// aktuell ausgewählter TTT GameArray Index
	CArrayTTTLevel			m_TTTLevelArray;			// die einzelnen Level (Spielstufen) (9)
	CCyberSolitaireDoc*		m_pCurrentSolitaireGame;	// Zeiger auf das aktuell laufende Solitaire Spiel
	CTTTField*				m_pCurrentTTTField;			// aktuell ausgewähltes TTT Field
	CSolLayout*				m_pCurrentSolLayout;		// Layout des aktuellen Spiels
	int						m_CurrentCyberSolitaireGameScore;	// aktuell erreichte Punkte bei dem ausgewählten Spiel
	BOOL					m_WithCurrentGameRunning;

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CTTTDoc)
	public:
	virtual void Serialize(CArchive& ar);   // Überschrieben für Dokument-Ein-/Ausgabe
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CTTTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	CTTTView* m_pView;

	//{{AFX_MSG(CTTTDoc)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString GetDateTimeVersion();
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual CSelectDoc* GetSelectDoc();
	virtual void GetRandomGamesIndexes(CIntArray& IntArray);
	virtual void UpdateGameInStatusBar(CString& GameName, int FieldNumber);
	virtual void UpdatePlayerInStatusBar(CPlayer& Player);
	virtual CTTTView* GetView() {return m_pView;}


	afx_msg void OnFileClose();
	// prüft, wer das gerade beendete Spiel gewonnen hat
	BOOL PlayerWinsCurrentSolitaireGame(void);
	// überprüft, ob der Level gewechselt hat
	enum LevelChangeValues ProofLevelChanged(void);
	// Füllt einen Level mit Auswahl-Spielen
	void FillLevelWithGames(void);
	afx_msg void OnSpielTTTSpeichern();
	afx_msg void OnSpielTTTSpeichernUnter();
	// // legt fest, ob automatisch eine Speicherung des TTT Spiels nach jedem Feld geschehen soll
	BOOL m_bAutoSave;
	// speichert das TTT Game (mit automatisch erzeugtem Namen
	BOOL AutoSaveTTTGame(void);
	// erzeugt einen automatischen Namen für das Spiel zum abspeichern
	void CreateAutoSaveGameName(CString& GameName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_TTTDOC_H__5F1F52E3_0825_11D7_97ED_00A0CC562BEB__INCLUDED_
