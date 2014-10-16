#if !defined(AFX_SELECTDOC_H__CAC24A79_8606_4452_AAC5_520FAE32F094__INCLUDED_)
#define AFX_SELECTDOC_H__CAC24A79_8606_4452_AAC5_520FAE32F094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectDoc.h : Header-Datei
//

#include "Statistic.h"
#include "Player.h"
//#define CMapGameNameToStatistic CTypedObjectMap<CMapStringToOb,CString,CStatistic*>  
//#define CMapGameNameToStatistic CTypedObjectMap<CMapStringToPtr,CString,CStatistic*>  
typedef CTypedPtrMap<CMapStringToOb,CString,CStatistic*> CMapGameNameToStatistic ;  

/////////////////////////////////////////////////////////////////////////////
// Dokument CSelectDoc 

class CTTTDoc;
class CSelectView;
class CSelectDoc : public COleDocument
{
protected:
	CSelectDoc();           // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CSelectDoc)

// Attribute
public:
	CStringList		m_GameList;
	CSolLayoutArray m_SolLayoutArr;			// Array mit allen Solitaire Spielen
	CPlayerArray	m_PlayerArr;			// Array mit allen angemeldeten Spielern
	CSolLayout*		m_pWorkingLayout;		// working Layout
	
	CMapGameNameToStatistic* m_pMapGameNameToStatistic; // Map mit den statistischen Werten
	CDaoWorkspace*	m_pDaoWorkSpace;		// WorkSpace Containing All DB
	CDaoDatabase*	m_pCybSolDB;			// Database
	CString			m_DBName;				// Pfad und Name der DB

// Operationen
public:
	BOOL OpenDataBase();

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSelectDoc)
	public:
	virtual void Serialize(CArchive& ar);   // Überschrieben für Dokument-Ein-/Ausgabe
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementierung
public:
	void UpdateScoreInStatusBar(int Score);
	void UpdateGameInStatusBar(CString& GameName);
	void UpdatePlayerInStatusBar(CPlayer& Player);
	void UpdateGameTree();
	int FindGame(CString& GameName);
	BOOL SaveSolLayoutArray(CString PathName);
	BOOL LoadSolLayoutArray(CString PathName);
	BOOL SaveMapGameNameToStatistic(CString PathName);
	BOOL LoadMapGameNameToStatistic(CString PathName);
	BOOL SavePlayerArray(CString PathName);
	BOOL LoadPlayerArray(CString PathName);
	CSelectView* m_pView;
	CSelectView* GetView();
	CDocument* NewGame(CString& GameName, CTTTDoc* pTTTDoc = NULL);
	void FillGameList();
	CSolLayout* GetWorkingLayout() {return m_pWorkingLayout; }
	void SetWorkingLayout(CSolLayout* pLayout) {m_pWorkingLayout = pLayout;}
	virtual ~CSelectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CSelectDoc)
	afx_msg void OnOpenGameeditor();
	afx_msg void OnUpdateOpenGameeditor(CCmdUI* pCmdUI);
	afx_msg void OnGamecontextDeleteGame();
	afx_msg void OnUpdateGamecontextDeleteGame(CCmdUI* pCmdUI);
	afx_msg void OnGamecontextStatistik();
	afx_msg void OnUpdateGamecontextStatistik(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// füllt die Map mit neuen Elementen, falls sie noch fehlen
	BOOL CompleteMapGameNameToStatistic(void);
	afx_msg void OnEinstellungenUebernehmetttwerte();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SELECTDOC_H__CAC24A79_8606_4452_AAC5_520FAE32F094__INCLUDED_
