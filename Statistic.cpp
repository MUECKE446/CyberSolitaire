// Statistic.cpp: Implementierung der Klasse CStatistic.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CyberSolitaire.h"

#include "Statistic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CSingleGameResult, CObject, VERSIONABLE_SCHEMA | 1)

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CSingleGameResult::CSingleGameResult()
{
	m_SingleGameResultVersion = SINGLEGAMERESULT_VERSION;
	m_PlayerName	= "";	
	m_Points		= 0;		
	m_Duration		= "";
	m_DateTimeStart	= "";
	m_DateTimeEnd	= "";	

}

CSingleGameResult::CSingleGameResult(const CSingleGameResult& p)
{
	m_SingleGameResultVersion	= p.m_SingleGameResultVersion;
	m_PlayerName				= p.m_PlayerName;	
	m_Points					= p.m_Points;		
	m_Duration					= p.m_Duration;		
	m_DateTimeStart				= p.m_DateTimeStart;	
	m_DateTimeEnd				= p.m_DateTimeEnd;	

}

CSingleGameResult::~CSingleGameResult()
{
}

const CSingleGameResult& CSingleGameResult::operator =(const CSingleGameResult& p)
{
	m_SingleGameResultVersion	= p.m_SingleGameResultVersion;
	m_PlayerName				= p.m_PlayerName;	
	m_Points					= p.m_Points;		
	m_Duration					= p.m_Duration;		
	m_DateTimeStart				= p.m_DateTimeStart;	
	m_DateTimeEnd				= p.m_DateTimeEnd;	
	return *this;
}

const CSingleGameResult& CSingleGameResult::operator =(const CSingleGameResult* p)
{
	m_SingleGameResultVersion	= p->m_SingleGameResultVersion;
	m_PlayerName				= p->m_PlayerName;	
	m_Points					= p->m_Points;		
	m_Duration					= p->m_Duration;		
	m_DateTimeStart				= p->m_DateTimeStart;	
	m_DateTimeEnd				= p->m_DateTimeEnd;	
	return *this;
}


void CSingleGameResult::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_SingleGameResultVersion = SINGLEGAMERESULT_VERSION;
		ar << m_SingleGameResultVersion;
		ar << m_PlayerName;	  
		ar << m_Points;		  
		ar << m_Duration;	
		ar << m_DateTimeStart;
		ar << m_DateTimeEnd;	  

    }
    else 
	{
		ar >> m_SingleGameResultVersion;
		// hier kann die Versionsverwaltung einsetzen
		ar >> m_PlayerName;	  
		ar >> m_Points;		  
		ar >> m_Duration;	
		ar >> m_DateTimeStart;
		ar >> m_DateTimeEnd;	  
    }
}

template <> void AFXAPI SerializeElements <CSingleGameResult> ( CArchive& ar, CSingleGameResult* pNewSingleGameResult, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewSingleGameResult++ )
    {
        // jedes CPlaceholder-Objekt serialisieren
        pNewSingleGameResult->Serialize( ar );
    }
}

void CSingleGameResult::ReportProperties()
{
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CStatistic, CObject, VERSIONABLE_SCHEMA | 1)

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////


	int						m_NumberOfPlayed;		// wie oft gespielt
	int						m_SumOfPoints;			// Summe aller erreichten Punkte
	int						m_Average;				// Durchschnitt der ereichten Punkte
	CSingleGameResultList	m_GameResultList;		// Liste der Resultate der einzelnen Spiele

CStatistic::CStatistic()
{
	m_StatisticVersion = STATISTIC_VERSION;
	m_NumberOfPlayed = 0;
	m_SumOfPoints = 0;	
	m_Average = 0;		
}

CStatistic::CStatistic(const CStatistic& p)
{
	m_StatisticVersion	= p.m_StatisticVersion;
	m_NumberOfPlayed	= p.m_NumberOfPlayed;
	m_SumOfPoints		= p.m_SumOfPoints;
	m_Average			= p.m_Average;		
	for (POSITION pos = m_GameResultList.GetHeadPosition();pos!= NULL;)
	{
		m_GameResultList.AddTail((CSingleGameResult&)p.m_GameResultList.GetNext(pos));
	}
}

CStatistic::~CStatistic()
{
}

const CStatistic& CStatistic::operator =(const CStatistic& p)
{
	m_StatisticVersion	= p.m_StatisticVersion;
	m_NumberOfPlayed	= p.m_NumberOfPlayed;
	m_SumOfPoints		= p.m_SumOfPoints;
	m_Average			= p.m_Average;		
	for (POSITION pos = m_GameResultList.GetHeadPosition();pos!= NULL;)
	{
		m_GameResultList.AddTail((CSingleGameResult&)p.m_GameResultList.GetNext(pos));
	}
	return *this;
}

const CStatistic& CStatistic::operator =(const CStatistic* p)
{
	m_StatisticVersion	= p->m_StatisticVersion;
	m_NumberOfPlayed	= p->m_NumberOfPlayed;
	m_SumOfPoints		= p->m_SumOfPoints;
	m_Average			= p->m_Average;		
	for (POSITION pos = m_GameResultList.GetHeadPosition();pos!= NULL;)
	{
		m_GameResultList.AddTail((CSingleGameResult&)p->m_GameResultList.GetNext(pos));
	}
	return *this;
}


void CStatistic::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_StatisticVersion = STATISTIC_VERSION;
		ar << m_StatisticVersion;
		ar << m_NumberOfPlayed;
    	ar << m_SumOfPoints;	
		ar << m_Average;		
	}
    else 
	{
		ar >> m_StatisticVersion;
		// hier kann die Versionsverwaltung einsetzen
		ar >> m_NumberOfPlayed;
    	ar >> m_SumOfPoints;	
		ar >> m_Average;		
    }
	m_GameResultList.Serialize(ar);
}

template <> void AFXAPI SerializeElements <CStatistic> ( CArchive& ar, CStatistic* pNewStatistic, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewStatistic++ )
    {
        // jedes CPlaceholder-Objekt serialisieren
        pNewStatistic->Serialize( ar );
    }
}

void CStatistic::ReportProperties()
{
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

BOOL CStatisticArray::DeleteAll()
{
	if (GetSize())
	{
		for (int i=0; i<GetSize(); i++)
		{
			CStatistic* pStatistic;
			pStatistic = &m_pData[i];
		}
		RemoveAll();
	}
	return TRUE;
}

void CStatisticArray::ReportProperties()
{
	if (GetSize())
		for (int i=0; i<GetSize(); i++)
			m_pData[i].ReportProperties();
}
