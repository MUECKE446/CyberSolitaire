// StatisticDoc.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "cybersolitaire.h"
#include "SelectDoc.h"
#include "SelectView.h"
#include "StatisticDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticDoc

IMPLEMENT_DYNCREATE(CStatisticDoc, CDocument)

CStatisticDoc::CStatisticDoc()
{
	m_GameName	 = "";
	m_pStatistic = NULL;
	m_pSolLayout = NULL;
}

BOOL CStatisticDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CStatisticDoc::~CStatisticDoc()
{
	if (m_pSolLayout)
	{
		delete m_pSolLayout;
	}
}


BEGIN_MESSAGE_MAP(CStatisticDoc, CDocument)
	//{{AFX_MSG_MAP(CStatisticDoc)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Diagnose CStatisticDoc

#ifdef _DEBUG
void CStatisticDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStatisticDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Serialisierung CStatisticDoc 

void CStatisticDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Code zum Speichern hier einfügen
	}
	else
	{
		// ZU ERLEDIGEN: Code zum Laden hier einfügen
	}
}

BOOL CStatisticDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	m_GameName = lpszPathName;
	CSelectDoc* pSelectDoc = GetSelectDoc();
	if (!pSelectDoc) return FALSE;
	int Index = pSelectDoc->FindGame(m_GameName);
	if (Index == -1)
	{
		AfxMessageBox(IDS_GAME_NOT_FOUND);
		return FALSE;
	}
	if (!pSelectDoc->m_pMapGameNameToStatistic->Lookup(m_GameName,m_pStatistic))
	{
		// Fehler
		ASSERT(0);
		return FALSE;
	}
	m_pSolLayout = new CSolLayout(pSelectDoc->m_SolLayoutArr[Index]);
	ASSERT(m_pSolLayout);
	return TRUE;
}

CSelectDoc* CStatisticDoc::GetSelectDoc()
{
	POSITION pos = GetApplication()->pSelectSolitaireTemplate->GetFirstDocPosition();
	// wir können davon ausgehen, daß es ein oder kein CProjDoc gibt
	CSelectDoc* pSelectDoc = NULL;
	if (pos)
		pSelectDoc = (CSelectDoc*)GetApplication()->pSelectSolitaireTemplate->GetNextDoc(pos);
	return pSelectDoc;
}

/////////////////////////////////////////////////////////////////////////////
// Befehle CStatisticDoc 
