// Statistic.h: Schnittstelle für die Klasse CStatistic.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STATISTIC_H_INCLUDED)
#define STATISTIC_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATISTIC_VERSION		1
#define SINGLEGAMERESULT_VERSION		1

class CSingleGameResult : public CObject
{
    DECLARE_SERIAL(CSingleGameResult)

public:
	CSingleGameResult();
	CSingleGameResult(const CSingleGameResult& p);	// Copy constructor
	virtual ~CSingleGameResult();

public:
	int					m_SingleGameResultVersion;		// Version für Serialize
	CString				m_PlayerName;					// Name des Spielers
	int					m_Points;						// Anzahl der erreichten Punkte
	CString				m_Duration;						// Dauer des Spiels
	CString				m_DateTimeStart;				// Tag und Uhrzeit des Spielbeginnes
	CString				m_DateTimeEnd;					// Tag und Uhrzeit des Spielendes

public:
// Operatoren
    const CSingleGameResult& operator =(const CSingleGameResult& p);
    const CSingleGameResult& operator =(const CSingleGameResult* p);

public:
	void ReportProperties();
	virtual void Serialize(CArchive& ar);

};

template <> void AFXAPI SerializeElements <CSingleGameResult> ( CArchive& ar, CSingleGameResult* pNewSingleGameResult, int nCount );

typedef CArray<CSingleGameResult,CSingleGameResult&>	CSingleGameResultArray;
typedef CList<CSingleGameResult,CSingleGameResult&>		CSingleGameResultList;

//////////////////////////////////////////////////////////////////////

class CStatistic : public CObject  
{
    DECLARE_SERIAL(CStatistic)

public:
	CStatistic();
	CStatistic(const CStatistic& p);	// Copy constructor
	virtual ~CStatistic();

public:
	int						m_StatisticVersion;		// Version für Serialize
	int						m_NumberOfPlayed;		// wie oft gespielt
	int						m_SumOfPoints;			// Summe aller erreichten Punkte
	int						m_Average;				// Durchschnitt der ereichten Punkte
	CSingleGameResultList	m_GameResultList;		// Liste der Resultate der einzelnen Spiele


public:
// Operatoren
    const CStatistic& operator =(const CStatistic& p);
    const CStatistic& operator =(const CStatistic* p);

public:
	void ReportProperties();
	virtual void Serialize(CArchive& ar);
};


template <> void AFXAPI SerializeElements <CStatistic> ( CArchive& ar, CStatistic* pNewStatistic, int nCount );

class CStatisticArray : public CArray<CStatistic,CStatistic&>
{
public:
	void ReportProperties();
	BOOL DeleteAll();
};



#endif
