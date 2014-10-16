// CyberSolitaireDoc.h : Schnittstelle der Klasse CCyberSolitaireDoc
//
/////////////////////////////////////////////////////////////////////////////

// das CyberSolitaireDoc stellt ein Solitairespiel dar

#if !defined(AFX_CYBERSOLITAIREDOC_H__2A6C1818_4B90_4A5C_8645_6DDAC7A8427C__INCLUDED_)
#define AFX_CYBERSOLITAIREDOC_H__2A6C1818_4B90_4A5C_8645_6DDAC7A8427C__INCLUDED_

#include "Piles.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ExitGameCauses
#define EXIT_CAUSE_NONE		0
#define EXIT_CAUSE_WINS		1
#define EXIT_CAUSE_ABORT	2
#define EXIT_CAUSE_EXITGAME	3
#define EXIT_CAUSE_EXITAPP	4
#define EXIT_CAUSE_TTTBACK	5

/////////////////////////////////////////////////////////////////////////////
// defines
//
// Versions for Serialize
#define CYBERSOLDOC_VERSION			3

class CSolitaireView;
class CDoControl;
class CSelectDoc;
class CStatistic;

class CCyberSolitaireDoc : public CDocument
{

	friend class CCyberSolitaireView;

protected: // Nur aus Serialisierung erzeugen
public:
	CCyberSolitaireDoc();
	DECLARE_DYNCREATE(CCyberSolitaireDoc)

// Attribute
public:
	CCardArray			m_Cards;			// Karten von 1 oder 2 Sets
	CMovePile*			m_pMovePile;		// der MovePile
	CPileList			m_PileList;			// enthält alle Piles außer den MovePile
	CPileArray			m_DealPileArr;		// enthält alle Piles von den gegeben wird
	CPileArray			m_RedealPileArr;	// enthält alle Piles von den beim Neu Geben der Stock gefüttert wird
	int					m_Score;			// Punkte Zähler
	gameScoring			m_TypeOfScoring;	// wie wird gezählt
	int					m_CountOfRedeals;	// Zähler für Neu Geben
	int					m_BasicCardValue;	// Basiskarte

	CPileList			m_LastPileList;		// enthält alle Piles des letzten Zuges außer den MovePile

	// Redo/Undo
	CDoControl*			m_pDo;				// Object zur Verwaltung von Redo/Undo

protected:
	CString				m_GameName;			// Name des Solitaire Spiels
	int					m_CyberSolVersion;	// Version für Serialisation
	int					m_MaxPoints;		// wann ist das Spiel gewonnen
	BOOL				m_WinsGame;			// TRUE, if Win Game
	CStatistic*			m_pStatistic;		// das Statistic Objekt

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CCyberSolitaireDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementierung
public:
	void CopyPileList();
	CString GetDateTimeVersion();
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	CString GetGameName() {return m_GameName;}
	void SetGameName(CString &GameName) {m_GameName	= GameName;}
	CSelectDoc* GetSelectDoc();
	void TestEmptyPiles();
	int ComputeScore();
	CStock* GetStock();
	CFoundation* GetFoundation();
	POSITION GetFirstPilePosition(pileDealing Destination);
	CPile* GetNextPile(POSITION& pos, pileDealing Destination);
	CTableau* GetNextTableau(POSITION& pos);
	POSITION GetFirstTableauPosition();
	CWaste* GetNextWaste(POSITION& pos);
	POSITION GetFirstWastePosition();
	void InitMovePile();
	void UpdatePiles();
	CCyberSolitaireView* GetView();
	void DeleteCards();
	void CreateCards(int NumSets);
	BOOL InitGame(CString& GameName);
	BOOL InitGame(CSolLayout& SolLayout,CStatistic* pStatistic, CTTTDoc* pTTTDoc = NULL);
	void ComputeStatistic();

	virtual ~CCyberSolitaireDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	CCyberSolitaireView* m_pView;
	//{{AFX_MSG(CCyberSolitaireDoc)
	afx_msg void OnButtonUndo();
	afx_msg void OnUpdateButtonUndo(CCmdUI* pCmdUI);
	afx_msg void OnButtonRedo();
	afx_msg void OnUpdateButtonRedo(CCmdUI* pCmdUI);
	afx_msg void OnSpielNeu();
	afx_msg void OnSpielNochmal();
	afx_msg void OnFileClose();
	afx_msg void OnSpielAbbrechen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void UpdateScoreInStatusBar(int Score);
	void UpdateGameInStatusBar(CString& GameName);
	void UpdatePlayerInStatusBar(CPlayer& Player);
	// spielt Töne zum Spielgeschehen
	BOOL PlaySound(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound);
	// befindet sich das Spiel in einer AutoPlay Phase
	BOOL m_InAutoPlay;
	// Testet, ob das Spiel gewonnen wurde
	BOOL WinsGame(void);
	// Zeiger auf das akuelle TTT Doc, falls das Spiel im Rahmen von TTT gespielt wird
	CTTTDoc* m_pTTTDoc;
	// behandelt das Verhalten beim Ende eines Spiels
	void HandleTheEndOfGame(void);
	// Grund, warum das Spiel beendet werden soll
	int m_ExitGameCause;
	afx_msg void OnGameExit();
	afx_msg void OnSpielBackToTttWindow();
	// überprüft, ob ein TT Spiel gewonnen oder verloren wurde
	BOOL WinsTTTGame(void);
	afx_msg void OnTTTSpielSpeichern();
	afx_msg void OnTTTSpielSpeichernUnter();
	// ersetzt das "Spiel" Menu bei einem Spiel, wenn es im TTT gespielt wird
	void SetTTTGameMenu(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CYBERSOLITAIREDOC_H__2A6C1818_4B90_4A5C_8645_6DDAC7A8427C__INCLUDED_)
