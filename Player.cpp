// Player.cpp: Implementierung der Klasse CPlayer.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "Crack.h"

#include "Player.h"
#include ".\player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CPlayer, CObject, VERSIONABLE_SCHEMA | 1)

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CPlayer::CPlayer()
{
	m_PlayerVersion		= PLAYER_VERSION;
	m_FirstName			= _T("");
	m_LastName			= _T("");
	m_NickName			= _T("");
	m_Birthday			= _T("");
	m_Rank				= _T("");
	m_NumberOfGames		= 0;
}

CPlayer::CPlayer(const CPlayer& p)
{
	m_PlayerVersion		= p.m_PlayerVersion	;
	m_FirstName			= p.m_FirstName		;
	m_LastName			= p.m_LastName		;		
	m_NickName			= p.m_NickName		;		
	m_Birthday			= p.m_Birthday		;		
	m_Rank				= p.m_Rank			;			
	m_NumberOfGames		= p.m_NumberOfGames	;
}

CPlayer::~CPlayer()
{
}

const CPlayer& CPlayer::operator =(const CPlayer& p)
{
	m_PlayerVersion		= p.m_PlayerVersion	;
	m_FirstName			= p.m_FirstName		;
	m_LastName			= p.m_LastName		;		
	m_NickName			= p.m_NickName		;		
	m_Birthday			= p.m_Birthday		;		
	m_Rank				= p.m_Rank			;			
	m_NumberOfGames		= p.m_NumberOfGames	;
	return *this;
}

const CPlayer& CPlayer::operator =(const CPlayer* p)
{
	m_PlayerVersion		= p->m_PlayerVersion;
	m_FirstName			= p->m_FirstName		;
	m_LastName			= p->m_LastName			;		
	m_NickName			= p->m_NickName			;		
	m_Birthday			= p->m_Birthday			;		
	m_Rank				= p->m_Rank				;			
	m_NumberOfGames		= p->m_NumberOfGames	;
	return *this;
}


void CPlayer::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_PlayerVersion = SOL_LAYOUT_VERSION;
		ar << m_PlayerVersion;
		ar << m_FirstName	 ;
    	ar << m_LastName	 ;
		ar << m_NickName	 ;
		ar << m_Birthday	 ;
		ar << m_Rank		 ;
		ar << m_NumberOfGames;
	}
    else 
	{
		ar >> m_PlayerVersion;
		// hier kann die Versionsverwaltung einsetzen
		ar >> m_FirstName	 ;
    	ar >> m_LastName	 ;
		ar >> m_NickName	 ;
		ar >> m_Birthday	 ;
		ar >> m_Rank		 ;
		ar >> m_NumberOfGames;
    }
}


void CPlayer::ReportProperties()
{
	CString Msg("");
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
	GetApplication()->LogMessage(Msg);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <> void AFXAPI SerializeElements <CPlayer> ( CArchive& ar, CPlayer* pNewPlayer, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewPlayer++ )
    {
        // jedes CPlaceholder-Objekt serialisieren
        pNewPlayer->Serialize( ar );
    }
}

//////////////////////////////////////////////////////////////////////

BOOL CPlayerArray::DeleteAll()
{
	RemoveAll();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPlayerArray Diagnose

#ifdef _DEBUG
void CPlayerArray::AssertValid() const
{
	CObject::AssertValid();
}

void CPlayerArray::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	for (int i=0; i<= GetUpperBound(); i++)
	{
		dc << "Name[" << i <<"]: "<< GetAt(i).m_NickName <<"\n";
	}
}

#endif //_DEBUG

void CPlayerArray::ReportProperties()
{
	if (GetSize())
		for (int i=0; i<GetSize(); i++)
			m_pData[i].ReportProperties();
}

// füllt das CPlayerArray aus der Datenbank
int CPlayerArray::ReadDB(CDaoDatabase* pDB)
{
	int Cnt = 0;
	CPlayer	Player;
	CDaoTableDef Table(pDB);
	Table.Open("Player");
	/*
	CDaoFieldInfo FieldInfo;
	Table.GetFieldInfo("Player-ID",FieldInfo);
	Table.GetFieldInfo("PlayerName",FieldInfo);
	Table.GetFieldInfo("NickName",FieldInfo);
	Table.GetFieldInfo("Rank",FieldInfo);
	*/
	CDaoRecordset rs(pDB);
	CString strSelect(_T(""));
	CString FieldValue(_T(""));
	try
	{
		strSelect = _T("Select * From [Player]");
		rs.Open(dbOpenDynaset,strSelect);

		while (!rs.IsEOF()) 
		{
			COleVariant var;
			Cnt++;
			var = rs.GetFieldValue("FirstName");
			FieldValue = CCrack::strVARIANT(var);
			if (FieldValue == "NULL")
				FieldValue = "";
			Player.m_FirstName = FieldValue;
			var = rs.GetFieldValue("LastName");
			FieldValue = CCrack::strVARIANT(var);
			if (FieldValue == "NULL")
				FieldValue = "";
			Player.m_LastName = FieldValue;
			var = rs.GetFieldValue("NickName");
			FieldValue = CCrack::strVARIANT(var);
			if (FieldValue == "NULL")
				FieldValue = "";
			Player.m_NickName = FieldValue;
			var = rs.GetFieldValue("Birthday");
			FieldValue = CCrack::strVARIANT(var);
			if (FieldValue == "NULL")
				FieldValue = "";
			Player.m_Birthday = FieldValue;
			var = rs.GetFieldValue("NumberOfGames");
			Player.m_NumberOfGames = var.uintVal;

			this->Add(Player);
			rs.MoveNext();
		}
		rs.Close();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}
	return Cnt;
}
