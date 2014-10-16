// SolLayout.h: Schnittstelle für die Klasse CSolLayout.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLLAYOUT_H__DCF8D173_B4E6_11D6_97B8_00A0CC562BEB__INCLUDED_)
#define AFX_SOLLAYOUT_H__DCF8D173_B4E6_11D6_97B8_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIntArray : public CArray<int,int>
{
public:
// Operatoren
    const CIntArray& operator =(const CIntArray& s) {
		SetSize(s.GetSize());
		if (s.GetSize() != 0)
			for (int i = 0; i < s.GetSize(); i++)
				SetAt(i,s.GetAt(i));

        return *this;
    }

    const CIntArray& operator =(const CIntArray* s) {
		SetSize(s->GetSize());
		if (s->GetSize() != 0)
			for (int i = 0; i < s->GetSize(); i++)
				SetAt(i,s->GetAt(i));

        return *this;
    }
};

//#define SOL_LAYOUT_VERSION		1
#define SOL_LAYOUT_VERSION		2

//////////////////////////////////////////////////////////////////////

class CSolLayout : public CObject  
{
    DECLARE_SERIAL(CSolLayout)

public:
	CSolLayout();
	CSolLayout(const CSolLayout& p);	// Copy constructor
	virtual ~CSolLayout();

public:
	int					m_SolLayoutVersion;		// Version für Serialize
	CString				m_GameName;				// Name des Spiels
	CString				m_GroupType;			// Spielgruppe
	int					m_GroupTypeID;			// ID der Gruppe
	int					m_NumberOfDecks;		// Anzahl der Kartenspiele
	gameScoring			m_TypeOfScoring;		// wie wird gezählt
	int					m_Difficulty;			// Schwierigkeit in % (100 ganz leicht, 0 ganz schwer)
	int					m_MaxPoints;			// maximal erreichbare Punkte
	CIntArray			m_LevelPointArray;		// zu erreichende Punkte in den einzelnen
												// Stufen (z.B. TIC TAC)

	CCardArray			m_Cards;				// Karten
	CPileList			m_PileList;				// enthält alle Piles außer den MovePile


public:
// Operatoren
    const CSolLayout& operator =(const CSolLayout& p);
    const CSolLayout& operator =(const CSolLayout* p);

public:
	void ReportGameProperties();
	void ReportProperties();
	virtual void Serialize(CArchive& ar);
	BOOL Save(CString lpszPathName);
	BOOL Load(CString lpszPathName);
	void ReportSaveLoadException(LPCTSTR lpszPathName,CException* e, BOOL bSaving, UINT nIDPDefault);

	// sortiert alle Piles dieses Spiels in eine vorgegebene Reihenfolge
	void SortPiles(void);
};


template <> void AFXAPI SerializeElements <CSolLayout> ( CArchive& ar, CSolLayout* pNewSolLayout, int nCount );

class CSolLayoutArray : public CArray<CSolLayout,CSolLayout&>
{
public:
	void ReportProperties();
	BOOL DeleteAll();
	// sortiert das Feld alphabetisch
	void SortAlphabetical(void);
};

#endif // !defined(AFX_SOLLAYOUT_H__DCF8D173_B4E6_11D6_97B8_00A0CC562BEB__INCLUDED_)
