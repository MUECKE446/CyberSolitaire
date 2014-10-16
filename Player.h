// Player.h: Schnittstelle für die Klasse CPlayer.
//
//////////////////////////////////////////////////////////////////////

#if !defined(PLAYER_H_INCLUDED)
#define PLAYER_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLAYER_VERSION		1

//////////////////////////////////////////////////////////////////////

class CPlayer : public CObject  
{
    DECLARE_SERIAL(CPlayer)

public:
	CPlayer();
	CPlayer(const CPlayer& p);	// Copy constructor
	virtual ~CPlayer();

public:
	int					m_PlayerVersion;		// Version für Serialize
	CString				m_FirstName;			// Vorname
	CString				m_LastName;				// Nachname
	CString				m_NickName;				// Spitzname
	CString				m_Birthday;				// Geburtstag
	CString				m_Rank;					// Rang
	int					m_NumberOfGames;		// Anzahl gespielter Spiele
	// zu_tun
	//											// GameHistory

public:
// Operatoren
    const CPlayer& operator =(const CPlayer& p);
    const CPlayer& operator =(const CPlayer* p);

public:
	void ReportProperties();
	virtual void Serialize(CArchive& ar);
};


template <> void AFXAPI SerializeElements <CPlayer> ( CArchive& ar, CPlayer* pNewPlayer, int nCount );

class CPlayerArray : public CArray<CPlayer,CPlayer&>
{
public:
#ifdef _DEBUG
	void AssertValid() const;
	void Dump(CDumpContext& dc) const;
#endif //_DEBUG

	void ReportProperties();
	BOOL DeleteAll();
	// füllt das CPlayerArray aus der Datenbank
	int ReadDB(CDaoDatabase* pDB);
};

#endif
