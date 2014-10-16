// TTTField.h: Schnittstelle für die Klasse CTTTField.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTTFIELD_H__5F1F52E6_0825_11D7_97ED_00A0CC562BEB__INCLUDED_)
#define AFX_TTTFIELD_H__5F1F52E6_0825_11D7_97ED_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TTT_FIELDS		9		// Anzahl der Felder
#define TTT_GAMES		3		// Anzahl der Auswahlspiele

#define TTTFIELD_VERSION		1	// Version für Serialisierung
#define TTTGAME_VERSION			1	// Version für Serialisierung
#define TTTLEVEL_VERSION		1	// Version für Serialisierung

#define CArrayTTTLevel			CArray<CTTTLevel,CTTTLevel&>	
#define CArrayTTTField			CArray<CTTTField,CTTTField&>	
#define CArrayTTTGame			CArray<CTTTGame,CTTTGame&>	

extern const CSize TTTLevelSize;
extern const CSize TTTLevelDistance;

// Vorwärtsdeklarationen
class CTTTLevel;
class CTTTGame;

	// Status des Feldes
enum fieldStatus {
	fieldEmpty				= 0,	// leer
	fieldCross				= 1,	// Kreuz
	fieldCircle				= 2,	// Kreis
	fieldNotVisible			= 3		// Inhalt nicht sichtbar
};


class CTTTField : public CObject  
{
   DECLARE_SERIAL(CTTTField)


public:
	CTTTField();
	CTTTField(const CTTTField& p);	// Copy constructor
	virtual ~CTTTField();

public:
	// Members
	// Information, die serialisiert werden muß
	int					m_TTTFieldVersion;	// Version für Serialisierung
	fieldStatus			m_Status;			// Status des Feldes
	CPoint				m_FieldPosition;	// obere linke Ecke des Feldes
	CRect				m_FieldRect;		// Rechteck des Feldes
	CTTTLevel*			m_pOwner;			// Eigentümer ist ein CTTTLevel
	CArrayTTTGame		m_TTTGameArray;		// enthält die einzelnen Auswahlspiele dieses Feldes (3)

	BOOL				m_MouseOverField;	// TRUE, wenn die Mouse über diesem Feld ist
	CDib*				m_pFieldDisplayDib;	// Zeiger auf die anzuzeigende Bitmap
	CDib				m_FieldDibUp;
	CDib				m_FieldDibDown;
	CDib				m_FieldDibCross;
	CDib				m_FieldDibCircle;


public:
// Operatoren
    const CTTTField& operator =(const CTTTField& p);
    const CTTTField& operator =(const CTTTField* p);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual CRect GetRect() {return m_FieldRect;}
	virtual CSize GetSize() {return m_FieldRect.Size();}
	virtual void  SetFieldPosition(int LevelIndex);
	virtual fieldStatus  GetFieldStatus(){return m_Status;}
	void Display(CDC* pDC, CDibs& Dibs);
	virtual void Serialize(CArchive& ar);

	// Überprüft, ob der Mousezeiger sich über dem Feld befindet
	BOOL HitTest(CPoint point);
};

template <> void AFXAPI SerializeElements <CTTTField> ( CArchive& ar, CTTTField* pNewObj, int nCount );



// Status des Level
enum levelStatus {
	levelPermitted			= 0,	// spielbar (praktisch der aktuelle Level)
	levelForbidden			= 1,	// verboten zu spielen
	levelDone				= 2		// der Level wurde schon gespielt
};


class CTTTLevel : public CObject  
{
   DECLARE_SERIAL(CTTTLevel)


public:
	CTTTLevel();
	CTTTLevel(const CTTTLevel& p);	// Copy constructor
	virtual ~CTTTLevel();

public:
	// Members
	// Information, die serialisiert werden muß
	int					m_TTTLevelVersion;	// Version für Serialisierung
	CPoint				m_LevelPosition;	// Position des Levels (logisch)
	CArrayTTTField		m_TTTFieldArray;	// enthält die einzelnen Felder des Level (9)
	levelStatus			m_Status;			// Status des Level
	int					m_LevelNumber;		// aktuelle Nummer des Levels

public:
// Operatoren
    const CTTTLevel& operator =(const CTTTLevel& p);
    const CTTTLevel& operator =(const CTTTLevel* p);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void Display(CDC* pDC, CDibs& Dibs);

	void SetLevelPosition(CPoint Position);
	CPoint& GetLevelPosition();
	virtual void Serialize(CArchive& ar);

	// überprüft, ob die Mouse sich über dem Level befindet
	BOOL HitTest(CPoint point);
};

template <> void AFXAPI SerializeElements <CTTTLevel> ( CArchive& ar, CTTTLevel* pNewObj, int nCount );


class CTTTGame : public CObject  
{
   DECLARE_SERIAL(CTTTGame)


public:
	CTTTGame();
	CTTTGame(const CTTTGame& p);	// Copy constructor
	virtual ~CTTTGame();

public:
	// Members
	// Information, die serialisiert werden muß
	int					m_TTTGameVersion;	// Version für Serialisierung
	CString				m_GameName;
	int					m_GameIndex;

public:
// Operatoren
    const CTTTGame& operator =(const CTTTGame& p);
    const CTTTGame& operator =(const CTTTGame* p);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual void Serialize(CArchive& ar);

};

template <> void AFXAPI SerializeElements <CTTTGame> ( CArchive& ar, CTTTGame* pNewObj, int nCount );




#endif // !defined(AFX_TTTFIELD_H__5F1F52E6_0825_11D7_97ED_00A0CC562BEB__INCLUDED_)
