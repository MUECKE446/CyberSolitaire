// SolLayout.cpp: Implementierung der Klasse CSolLayout.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "TTTField.h"
#include "TTTView.h"
#include "TTTDoc.h"

#include "SolLayout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CSolLayout, CObject, VERSIONABLE_SCHEMA | 1)

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CSolLayout::CSolLayout()
{
	m_SolLayoutVersion = SOL_LAYOUT_VERSION;
	m_Difficulty	    = 100;            
	m_MaxPoints			= 0;
	m_LevelPointArray.SetSize(TTT_LEVELS);
	for (int i=0;i<m_LevelPointArray.GetSize(); i++)
		m_LevelPointArray.SetAt(i,0);
}

CSolLayout::CSolLayout(const CSolLayout& p)
{
	m_SolLayoutVersion	= p.m_SolLayoutVersion;
	m_GameName			= p.m_GameName; 
	m_GroupType			= p.m_GroupType;
	m_GroupTypeID		= p.m_GroupTypeID;		
	m_NumberOfDecks		= p.m_NumberOfDecks;	
	m_TypeOfScoring		= p.m_TypeOfScoring;	
	m_Difficulty	    = p.m_Difficulty;            
	m_MaxPoints			= p.m_MaxPoints;
	m_LevelPointArray		= p.m_LevelPointArray;
	ASSERT(m_LevelPointArray.GetSize() == TTT_LEVELS);
	ASSERT(p.m_LevelPointArray.GetSize() == TTT_LEVELS);

	//m_Cards				= p.m_Cards;
	//m_PileList			= p.m_PileList;
	if (p.m_Cards.GetSize())
		// alle Karten kopieren
		for (int j = 1; j < p.m_Cards.GetSize(); j++)
			m_Cards.Add(new CCard(*p.m_Cards[j]));
	if (!p.m_PileList.IsEmpty())
	{
		CPile* pPile = NULL;
		CPile* pNewPile = NULL;
		POSITION pos = p.m_PileList.GetHeadPosition();
		// über den Copy Constructor werden hier neue Piles angelegt
		while (pos)
		{
			CPile* pPile = p.m_PileList.GetNext(pos);
			if (pPile->IsK(CStock))
				pNewPile = (CPile*)new CStock((CStock&)*pPile);
			if (pPile->IsK(CTableau))
				pNewPile = (CPile*)new CTableau((CTableau&)*pPile);
			if (pPile->IsK(CFoundation))
				pNewPile = (CPile*)new CFoundation((CFoundation&)*pPile);
			if (pPile->IsK(CWaste))
				pNewPile = (CPile*)new CWaste((CWaste&)*pPile);
			m_PileList.AddTail(pNewPile);
		}
	}
}

CSolLayout::~CSolLayout()
{
	if (m_Cards.GetSize() != 0)
		// alle Karten entsorgen
		for (int j = 1; j < m_Cards.GetSize(); j++)
			delete m_Cards[j];
	m_Cards.RemoveAll();
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
}

const CSolLayout& CSolLayout::operator =(const CSolLayout& p)
{
	m_SolLayoutVersion	= p.m_SolLayoutVersion;
	m_GameName			= p.m_GameName; 
	m_GroupType			= p.m_GroupType;
	m_GroupTypeID		= p.m_GroupTypeID;		
	m_NumberOfDecks		= p.m_NumberOfDecks;	
	m_TypeOfScoring		= p.m_TypeOfScoring;	
	m_Difficulty	    = p.m_Difficulty;            
	m_MaxPoints			= p.m_MaxPoints;
	m_LevelPointArray		= p.m_LevelPointArray;
	ASSERT(m_LevelPointArray.GetSize() == TTT_LEVELS);
	ASSERT(p.m_LevelPointArray.GetSize() == TTT_LEVELS);
	//m_Cards				= p.m_Cards;
	//m_PileList			= p.m_PileList;
	if (p.m_Cards.GetSize())
		// alle Karten kopieren
		for (int j = 1; j < p.m_Cards.GetSize(); j++)
			m_Cards.Add(new CCard(*p.m_Cards[j]));
	if (!p.m_PileList.IsEmpty())
	{
		CPile* pPile = NULL;
		CPile* pNewPile = NULL;
		POSITION pos = p.m_PileList.GetHeadPosition();
		// über den Copy Constructor werden hier neue Piles angelegt
		while (pos)
		{
			CPile* pPile = p.m_PileList.GetNext(pos);
			if (pPile->IsK(CStock))
				pNewPile = (CPile*)new CStock((CStock&)*pPile);
			if (pPile->IsK(CTableau))
				pNewPile = (CPile*)new CTableau((CTableau&)*pPile);
			if (pPile->IsK(CFoundation))
				pNewPile = (CPile*)new CFoundation((CFoundation&)*pPile);
			if (pPile->IsK(CWaste))
				pNewPile = (CPile*)new CWaste((CWaste&)*pPile);
			m_PileList.AddTail(pNewPile);
		}
	}
	return *this;
}

const CSolLayout& CSolLayout::operator =(const CSolLayout* p)
{
	m_SolLayoutVersion		= p->m_SolLayoutVersion;
	m_GameName			= p->m_GameName; 
	m_GroupType			= p->m_GroupType;
	m_GroupTypeID		= p->m_GroupTypeID;		
	m_NumberOfDecks		= p->m_NumberOfDecks;	
	m_TypeOfScoring		= p->m_TypeOfScoring;	
	m_Difficulty	    = p->m_Difficulty;            
	m_MaxPoints			= p->m_MaxPoints;
	m_LevelPointArray		= p->m_LevelPointArray;
	ASSERT(m_LevelPointArray.GetSize() == TTT_LEVELS);
	ASSERT(p->m_LevelPointArray.GetSize() == TTT_LEVELS);
	//m_Cards				= p->m_Cards;
	//m_PileList			= p->m_PileList;
	if (p->m_Cards.GetSize())
		// alle Karten kopieren
		for (int j = 1; j < p->m_Cards.GetSize(); j++)
			m_Cards.Add(new CCard(*p->m_Cards[j]));
	if (!p->m_PileList.IsEmpty())
	{
		CPile* pPile = NULL;
		CPile* pNewPile = NULL;
		POSITION pos = p->m_PileList.GetHeadPosition();
		// über den Copy Constructor werden hier neue Piles angelegt
		while (pos)
		{
			CPile* pPile = p->m_PileList.GetNext(pos);
			if (pPile->IsK(CStock))
				pNewPile = (CPile*)new CStock((CStock&)*pPile);
			if (pPile->IsK(CTableau))
				pNewPile = (CPile*)new CTableau((CTableau&)*pPile);
			if (pPile->IsK(CFoundation))
				pNewPile = (CPile*)new CFoundation((CFoundation&)*pPile);
			if (pPile->IsK(CWaste))
				pNewPile = (CPile*)new CWaste((CWaste&)*pPile);
			m_PileList.AddTail(pNewPile);
		}
	}
	return *this;
}


void CSolLayout::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_SolLayoutVersion = SOL_LAYOUT_VERSION;
		ar << m_SolLayoutVersion;
    }
    else 
	{
		ar >> m_SolLayoutVersion;
		// hier kann die Versionsverwaltung einsetzen
    }

	m_Cards.Serialize(ar);
	m_PileList.Serialize(ar);

	if (ar.IsStoring()) 
	{
		ar << m_GameName << m_GroupType << m_GroupTypeID << m_NumberOfDecks; 
		ar.Write(&m_TypeOfScoring,sizeof(m_TypeOfScoring));
		ar << m_Difficulty << m_MaxPoints;
		m_LevelPointArray.Serialize(ar);
    }
    else 
	{
		ar >> m_GameName >> m_GroupType >> m_GroupTypeID >> m_NumberOfDecks;
		ar.Read(&m_TypeOfScoring,sizeof(m_TypeOfScoring));
		if (m_SolLayoutVersion > 1)
		{
			ar >> m_Difficulty >> m_MaxPoints;
			m_LevelPointArray.Serialize(ar);
		}
    }
}

template <> void AFXAPI SerializeElements <CSolLayout> ( CArchive& ar, CSolLayout* pNewSolLayout, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewSolLayout++ )
    {
        // jedes CPlaceholder-Objekt serialisieren
        pNewSolLayout->Serialize( ar );
    }
}


BOOL CSolLayout::Save(CString lpszPathName)
{
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(lpszPathName,CFile::modeCreate|CFile::modeWrite);
		CArchive storeArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
		Serialize(storeArchive);     // store me
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
		ReportSaveLoadException(lpszPathName, e,FALSE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH_ALL
	
	return TRUE;
}

BOOL CSolLayout::Load(CString lpszPathName)
{
	CFile* pFile = NULL;
	TRY
	{
		pFile = new CFile(lpszPathName,CFile::modeRead);
		CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
		Serialize(loadArchive);     // load me
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

		ReportSaveLoadException(lpszPathName, e,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

void CSolLayout::ReportSaveLoadException(LPCTSTR lpszPathName,CException* e, BOOL bSaving, UINT nIDPDefault)
{
	UINT nIDP = nIDPDefault;
	UINT nHelpContext = nIDPDefault;
	CString prompt;

	if (e != NULL)
	{
		ASSERT_VALID(e);
		if (e->IsKindOf(RUNTIME_CLASS(CFileException)))
			return; // Nicht Fein aber was soll es.

		if (e->IsKindOf(RUNTIME_CLASS(CUserException)))
			return; // already reported

		if (e->IsKindOf(RUNTIME_CLASS(CArchiveException)))
		{
			switch (((CArchiveException*)e)->m_cause)
			{
			case CArchiveException::badSchema:
			case CArchiveException::badClass:
			case CArchiveException::badIndex:
			case CArchiveException::endOfFile:
				nIDP = AFX_IDP_FAILED_INVALID_FORMAT;
				break;
			default:
				break;
			}
		}
		if (prompt.IsEmpty())
		{
			AfxFormatString1(prompt, nIDP, lpszPathName);
		}

		AfxMessageBox(prompt, MB_ICONEXCLAMATION, nHelpContext);
	}

}


// sortiert alle Piles dieses Spiels in eine vorgegebene Reihenfolge
// jeweils falls vorhanden
// alle CTableau
// alle CFoundation
// alle CWaste
// den Stock
void CSolLayout::SortPiles(void)
{
	ASSERT(!m_PileList.IsEmpty());
	CPileList	SortedTableaus;
	CPileList	SortedFoundations;
	CPileList	SortedWastes;
	CPile* pStockPile = NULL;
	CPile* pPile;
	POSITION pos=NULL;
	for (pos = m_PileList.GetHeadPosition();pos != 0;)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile && pPile->IsK(CTableau))
			SortedTableaus.AddTail(pPile);
		if (pPile && pPile->IsK(CFoundation))
			SortedFoundations.AddTail(pPile);
		if (pPile && pPile->IsK(CWaste))
			SortedWastes.AddTail(pPile);
		if (pPile && pPile->IsK(CStock))
			pStockPile = pPile;
	}
	m_PileList.RemoveAll();
	if (!SortedTableaus.IsEmpty()) m_PileList.AddTail(&SortedTableaus);
	if (!SortedFoundations.IsEmpty()) m_PileList.AddTail(&SortedFoundations);
	if (!SortedWastes.IsEmpty()) m_PileList.AddTail(&SortedWastes);
	if (pStockPile) m_PileList.AddTail(pStockPile);
}

void CSolLayout::ReportGameProperties()
{
	// als String Resource
	CString Msg("---------------------------------------------------------------------");
	GetApplication()->LogMessage(Msg);
	Msg = "------------------------------ GAME ---------------------------------";
	GetApplication()->LogMessage(Msg);
	Msg = "---------------------------------------------------------------------";
	GetApplication()->LogMessage(Msg);
	Msg.Format("Name: %s",m_GameName);
	GetApplication()->LogMessage(Msg);
	Msg.Format("Gruppe: %s",m_GroupType);
	GetApplication()->LogMessage(Msg);
	Msg.Format("GruppenID: %d",m_GroupTypeID);
	GetApplication()->LogMessage(Msg);
	Msg.Format("Anzahl Kartenspiele: %d",m_NumberOfDecks);
	GetApplication()->LogMessage(Msg);
	Msg = "Art der Zählung: ";
	CString ValueMsg("");
	ValueMsg.LoadString(IDS_SCORE_NOTAVAILABLE + (int)m_TypeOfScoring);
	Msg += ValueMsg;
	GetApplication()->LogMessage(Msg);
	Msg.Format("Schwierigkeit: %d",m_Difficulty);
	GetApplication()->LogMessage(Msg);
	Msg.Format("maximal erreichbare Punkte: %d",m_MaxPoints);
	GetApplication()->LogMessage(Msg);
	Msg = "";
	// zu_tun
	// m_LevelPointArray ausgeben
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
}

void CSolLayout::ReportProperties()
{
	ReportGameProperties();
	CPile* pPile;
	POSITION pos=NULL;
	for (pos = m_PileList.GetHeadPosition();pos != 0;)
	{
		pPile = m_PileList.GetNext(pos);
		if (pPile && pPile->IsK(CTableau))
			((CTableau*)pPile)->ReportProperties();
		if (pPile && pPile->IsK(CFoundation))
			((CFoundation*)pPile)->ReportProperties();
		if (pPile && pPile->IsK(CWaste))
			((CWaste*)pPile)->ReportProperties();
		if (pPile && pPile->IsK(CStock))
			((CStock*)pPile)->ReportProperties();
	}
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

BOOL CSolLayoutArray::DeleteAll()
{
	if (GetSize())
	{
		for (int i=0; i<GetSize(); i++)
		{
			CSolLayout* pSolLayout;
			pSolLayout = &m_pData[i];
			if (pSolLayout->m_Cards.GetSize() != 0)
				// alle Karten entsorgen
				for (int j = 1; j < pSolLayout->m_Cards.GetSize(); j++)
					delete pSolLayout->m_Cards[j];
			pSolLayout->m_Cards.RemoveAll();
			CPile* pPile;
			if (!pSolLayout->m_PileList.IsEmpty())
			{
				POSITION pos = pSolLayout->m_PileList.GetHeadPosition();
				while (pos)
				{
					if (pPile = pSolLayout->m_PileList.GetNext(pos))
						delete pPile;
				}
				pSolLayout->m_PileList.RemoveAll();
			}
		}
		RemoveAll();
	}
	return TRUE;
}

// sortiert das Feld alphabetisch
void CSolLayoutArray::SortAlphabetical(void)
{
	//CSolLayout SolLayout;
	if (GetSize())
	{
		for (int i=0; i<GetSize()-1; i++)
		{
			//SolLayout = m_pData[i];
			for (int j=i+1; j<GetSize(); j++)
			{
				CString GameName1 = m_pData[i].m_GameName;
				CString GameName2 = m_pData[j].m_GameName;
				if (GameName1 >= GameName2)
				{
					// Elemente tauschen
					CSolLayout TmpLayout(m_pData[j]);
					m_pData[j] = m_pData[i];
					m_pData[i] = TmpLayout;
					break;
				}
			}
		}
	}
}


void CSolLayoutArray::ReportProperties()
{
	if (GetSize())
		for (int i=0; i<GetSize(); i++)
			m_pData[i].ReportProperties();
}
