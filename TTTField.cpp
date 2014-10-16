// TTTField.cpp: Implementierung der Klasse CTTTField.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "TTTField.h"
//#include ".\tttfield.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CTTTField, CObject, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(CTTTLevel, CObject, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(CTTTGame,  CObject, VERSIONABLE_SCHEMA | 1)

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CTTTField::CTTTField()
{
	m_TTTFieldVersion = TTTFIELD_VERSION;
	m_Status = fieldEmpty;
	m_FieldPosition = CPoint(0,0);
	m_FieldRect.SetRectEmpty();
	m_pOwner = NULL;
	m_MouseOverField = FALSE;
	m_pFieldDisplayDib = NULL;
	m_TTTGameArray.SetSize(TTT_GAMES);
	VERIFY(m_FieldDibUp.Load(AfxGetApp()->m_hInstance,IDB_FIELD_UP));
	VERIFY(m_FieldDibDown.Load(AfxGetApp()->m_hInstance,IDB_FIELD_DOWN));
	VERIFY(m_FieldDibCross.Load(AfxGetApp()->m_hInstance,IDB_FIELD_CROSS));
	VERIFY(m_FieldDibCircle.Load(AfxGetApp()->m_hInstance,IDB_FIELD_CIRCLE));

}

CTTTField::CTTTField(const CTTTField& p)
{
	m_TTTFieldVersion			= p.m_TTTFieldVersion;
	m_Status					= p.m_Status;
	m_FieldPosition				= p.m_FieldPosition;
	m_FieldRect					= p.m_FieldRect;
	m_pOwner					= p.m_pOwner;
	m_MouseOverField			= p.m_MouseOverField;
	m_pFieldDisplayDib			= p.m_pFieldDisplayDib;
	for (int i=0; i<p.m_TTTGameArray.GetSize();i++)
		m_TTTGameArray[i] = p.m_TTTGameArray[i];
}

CTTTField::~CTTTField()
{

}

const CTTTField& CTTTField::operator =(const CTTTField& p)
{
	m_TTTFieldVersion			= p.m_TTTFieldVersion;
	m_Status					= p.m_Status;
	m_FieldPosition				= p.m_FieldPosition;
	m_FieldRect					= p.m_FieldRect;
	m_pOwner					= p.m_pOwner;
	m_MouseOverField			= p.m_MouseOverField;
	m_pFieldDisplayDib			= p.m_pFieldDisplayDib;
	for (int i=0; i<p.m_TTTGameArray.GetSize();i++)
		m_TTTGameArray[i] = p.m_TTTGameArray[i];
	return *this;
}

const CTTTField& CTTTField::operator =(const CTTTField* p)
{
	m_TTTFieldVersion			= p->m_TTTFieldVersion;
	m_Status					= p->m_Status;
	m_FieldPosition				= p->m_FieldPosition;
	m_FieldRect					= p->m_FieldRect;
	m_pOwner					= p->m_pOwner;
	m_MouseOverField			= p->m_MouseOverField;
	m_pFieldDisplayDib			= p->m_pFieldDisplayDib;
	for (int i=0; i<p->m_TTTGameArray.GetSize();i++)
		m_TTTGameArray[i] = p->m_TTTGameArray[i];
	return *this;
}

template <> void AFXAPI SerializeElements <CTTTField> ( CArchive& ar, CTTTField* pNewObj, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewObj++ )
    {
        // jedes Objekt serialisieren
        pNewObj->Serialize( ar );
    }
}


/////////////////////////////////////////////////////////////////////////////
// Diagnose

#ifdef _DEBUG
void CTTTField::AssertValid() const
{
	CObject::AssertValid();
}

void CTTTField::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// diese Funktion serialisiert das Feld
//
void CTTTField::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_TTTFieldVersion = TTTFIELD_VERSION;
		ar << m_TTTFieldVersion;
		ar << m_FieldPosition;
		ar << m_FieldRect;
		ar.Write(&m_Status,sizeof(m_Status));
	}
	else 
	{
		ar >> m_TTTFieldVersion;
		ar >> m_FieldPosition;
		ar >> m_FieldRect;
		ar.Read(&m_Status,sizeof(m_Status));
	}
	m_TTTGameArray.Serialize(ar);
}

void CTTTField::Display(CDC* pDC, CDibs& Dibs)
{
	//pDC->Rectangle(&m_FieldRect);
	CRect FieldRect(m_FieldRect);
	pDC->LPtoDP(&FieldRect);

	switch (m_Status)
	{
	case fieldEmpty:
		if (m_MouseOverField)
		{
			m_pFieldDisplayDib = &m_FieldDibUp;
		}
		else
		{
			m_pFieldDisplayDib = &m_FieldDibDown;
		}
		break;

	case fieldCross:
		m_pFieldDisplayDib = &m_FieldDibCross;
		break;

	case fieldCircle:
		m_pFieldDisplayDib = &m_FieldDibCircle;
		break;

	default:
		ASSERT(0);
	}

	Dibs.pScreenDib->CopyBitmap(*pDC,m_pFieldDisplayDib,&FieldRect);

	//pDC->Rectangle(&m_FieldRect);
	/*
	for (int i=0;i<m_TTTGameArray.GetSize();i++)
	{
		CRect TextRect(m_FieldRect);
		TextRect.top += i*20;
		pDC->DrawText(m_TTTGameArray[i].m_GameName,TextRect,DT_NOCLIP);
	}
	*/
}


void  CTTTField::SetFieldPosition(int LevelIndex)
{
	CPoint	LevelPosition = m_pOwner->GetLevelPosition();
	CSize	FieldSize(TTTLevelSize.cx/3,TTTLevelSize.cy/3);
	int xFactor = (LevelIndex)%3;
	int yFactor = (LevelIndex)/3;
	m_FieldPosition.x = LevelPosition.x + xFactor * FieldSize.cx;
	m_FieldPosition.y = LevelPosition.y + yFactor * FieldSize.cy;
	m_FieldRect.SetRect(m_FieldPosition.x,m_FieldPosition.y,
						m_FieldPosition.x+FieldSize.cx,m_FieldPosition.y+FieldSize.cy);
	TRACE("Field(%d): Pos(%d,%d)\tRect(%d,%d)\n",
		LevelIndex,m_FieldPosition.x,m_FieldPosition.y,
		m_FieldRect.Size().cx,m_FieldRect.Size().cy);
}

// Überprüft, ob der Mousezeiger sich über dem Feld befindet
BOOL CTTTField::HitTest(CPoint point)
{
	CRect FieldRect(m_FieldRect);
	// Rechteck etwas verkleinern
	FieldRect.InflateRect(-4,-4);
	if (FieldRect.PtInRect(point))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// Berechnung der Levelgröße (geht von TTT_LEVELS == 9 aus)
// Levelgröße ist immer quadratisch
#define FREE_EDGE	5	// Rand um den Level

#if TABLE_CX >= TABLE_CY
	const CSize TTTLevelSize((TABLE_CY/3)-2*FREE_EDGE,(TABLE_CY/3)-2*FREE_EDGE);
#else
	const CSize TTTLevelSize((TABLE_CX/3)-2*FREE_EDGE,(TABLE_CX/3)-2*FREE_EDGE);
#endif
	const CSize TTTLevelDistance((TABLE_CX - 3*TTTLevelSize.cx)/4,(TABLE_CY - 3*TTTLevelSize.cy)/4);

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CTTTLevel::CTTTLevel()
{
	m_TTTLevelVersion = TTTLEVEL_VERSION;
	m_LevelNumber = -1;
	m_LevelPosition = CPoint(0,0);
	// ein Level besitzt 9 Felder
	// die Felder werden über ihren Konstruktor initialisiert
	m_TTTFieldArray.SetSize(TTT_FIELDS);			
	// dieser Level wird Eigentümer seiner Felder
	for (int i=0; i<m_TTTFieldArray.GetSize();i++)
	{
		ASSERT(m_TTTFieldArray[i].m_pOwner == NULL);
		m_TTTFieldArray[i].m_pOwner = this;
	}
}

CTTTLevel::CTTTLevel(const CTTTLevel& p)
{
	m_TTTLevelVersion	= p.m_TTTLevelVersion;
	m_LevelNumber		= p.m_LevelNumber;
	m_LevelPosition		= p.m_LevelPosition;
	for (int i=0; i<p.m_TTTFieldArray.GetSize();i++)
		m_TTTFieldArray[i] = p.m_TTTFieldArray[i];
}

CTTTLevel::~CTTTLevel()
{

}

const CTTTLevel& CTTTLevel::operator =(const CTTTLevel& p)
{
	m_TTTLevelVersion	= p.m_TTTLevelVersion;
	m_LevelNumber		= p.m_LevelNumber;
	m_LevelPosition		= p.m_LevelPosition;
	for (int i=0; i<p.m_TTTFieldArray.GetSize();i++)
		m_TTTFieldArray[i] = p.m_TTTFieldArray[i];
	return *this;
}

const CTTTLevel& CTTTLevel::operator =(const CTTTLevel* p)
{
	m_TTTLevelVersion	= p->m_TTTLevelVersion;
	m_LevelNumber		= p->m_LevelNumber;
	m_LevelPosition		= p->m_LevelPosition;
	for (int i=0; i<p->m_TTTFieldArray.GetSize();i++)
		m_TTTFieldArray[i] = p->m_TTTFieldArray[i];
	return *this;
}

template <> void AFXAPI SerializeElements <CTTTLevel> ( CArchive& ar, CTTTLevel* pNewObj, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewObj++ )
    {
        // jedes Objekt serialisieren
        pNewObj->Serialize( ar );
    }
}


/////////////////////////////////////////////////////////////////////////////
// Diagnose

#ifdef _DEBUG
void CTTTLevel::AssertValid() const
{
	CObject::AssertValid();
}

void CTTTLevel::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	CString DumpString("");
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// diese Funktion setzt die logische Position des Level
//
void CTTTLevel::SetLevelPosition(CPoint Position)
{
	m_LevelPosition = Position;
	// damit verändern sich auch die Positionen aller Felder
	for (int i=0; i<m_TTTFieldArray.GetSize();i++)
	{
		m_TTTFieldArray[i].SetFieldPosition(i);
	}

}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion liefert die logische Position des Level
//
CPoint& CTTTLevel::GetLevelPosition()
{
	return m_LevelPosition;
}

/////////////////////////////////////////////////////////////////////////////
// diese Funktion serialisiert das Feld
//
void CTTTLevel::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_TTTLevelVersion = TTTLEVEL_VERSION;
		ar << m_TTTLevelVersion;
		ar << m_LevelPosition;
		ar << m_LevelNumber;
		ar.Write(&m_Status,sizeof(m_Status));
	}
	else 
	{
		ar >> m_TTTLevelVersion;
		ar >> m_LevelPosition;
		ar >> m_LevelNumber;
		ar.Read(&m_Status,sizeof(m_Status));
	}
	m_TTTFieldArray.Serialize(ar);
}



void CTTTLevel::Display(CDC *pDC, CDibs &Dibs)
{
	CRect	LevelRect(m_LevelPosition,TTTLevelSize);
	CDib	LevelDib;
	int		LevelResId = IDB_LEVEL2-2;

	if (m_Status == fieldEmpty)
	{
	}

	// die Felder werden nur bei spielbarem oder gespieltem Level angezeigt
	switch (m_Status)
	{
	case levelPermitted:				// spielbar
	case levelDone:						// gespielt
		VERIFY(LevelDib.Load(AfxGetApp()->m_hInstance,IDB_LEVEL_CLEAR));
		pDC->LPtoDP(&LevelRect);
		Dibs.pScreenDib->CopyBitmap(*pDC,&LevelDib,&LevelRect);
		for (int i=0; i<m_TTTFieldArray.GetSize(); i++)
		{
			m_TTTFieldArray[i].Display(pDC,Dibs);
		}
		break;
	case levelForbidden:
		ASSERT(m_LevelNumber >= 2);
		VERIFY(LevelDib.Load(AfxGetApp()->m_hInstance,LevelResId+m_LevelNumber));
		pDC->LPtoDP(&LevelRect);
		Dibs.pScreenDib->CopyBitmap(*pDC,&LevelDib,&LevelRect);
		//pDC->Rectangle(&LevelRect);
		break;
	default:
		ASSERT(0);
	}
}

// überprüft, ob die Mouse sich über dem Level befindet
BOOL CTTTLevel::HitTest(CPoint point)
{
	CRect LevelRect(m_LevelPosition,TTTLevelSize);
	// Rechteck etwas verkleinern
	LevelRect.InflateRect(-4,-4);
	if (LevelRect.PtInRect(point))
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CTTTGame::CTTTGame()
{
	m_TTTGameVersion = TTTGAME_VERSION;
	m_GameName = "";
	m_GameIndex = -1;
}

CTTTGame::CTTTGame(const CTTTGame& p)
{
	m_TTTGameVersion	= p.m_TTTGameVersion;
	m_GameName			= p.m_GameName;
	m_GameIndex			= p.m_GameIndex;
}

CTTTGame::~CTTTGame()
{

}

const CTTTGame& CTTTGame::operator =(const CTTTGame& p)
{
	m_TTTGameVersion	= p.m_TTTGameVersion;
	m_GameName			= p.m_GameName;
	m_GameIndex			= p.m_GameIndex;
	return *this;
}

const CTTTGame& CTTTGame::operator =(const CTTTGame* p)
{
	m_TTTGameVersion	= p->m_TTTGameVersion;
	m_GameName			= p->m_GameName;
	m_GameIndex			= p->m_GameIndex;
	return *this;
}


template <> void AFXAPI SerializeElements <CTTTGame> ( CArchive& ar, CTTTGame* pNewObj, int nCount )
{
    for ( int i = 0; i < nCount; i++, pNewObj++ )
    {
        // jedes Objekt serialisieren
        pNewObj->Serialize( ar );
    }
}


/////////////////////////////////////////////////////////////////////////////
// Diagnose

#ifdef _DEBUG
void CTTTGame::AssertValid() const
{
	CObject::AssertValid();
}

void CTTTGame::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// diese Funktion serialisiert das Feld
//
void CTTTGame::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		m_TTTGameVersion = TTTGAME_VERSION;
		ar << m_TTTGameVersion;
		ar << m_GameIndex;
		ar << m_GameName;
	}
	else 
	{
		ar >> m_TTTGameVersion;
		ar >> m_GameIndex;
		ar >> m_GameName;
	}
}


