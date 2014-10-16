#if !defined(AFX_STATISTICDOC_H__F2638DC9_4801_11D7_9822_00A0CC562BEB__INCLUDED_)
#define AFX_STATISTICDOC_H__F2638DC9_4801_11D7_9822_00A0CC562BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticDoc.h : Header-Datei
//

class CStatistic;
class CSolLayout;


/////////////////////////////////////////////////////////////////////////////
// Dokument CStatisticDoc 

class CStatisticDoc : public CDocument
{
protected:
	CStatisticDoc();           // Dynamische Erstellung verwendet gesch�tzten Konstruktor
	DECLARE_DYNCREATE(CStatisticDoc)

// Attribute
public:

// Operationen
public:
	CString					m_GameName;
	CStatistic*				m_pStatistic;
	CSolLayout*				m_pSolLayout;

// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CStatisticDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);   // �berschrieben f�r Dokument-Ein-/Ausgabe
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementierung
public:
	CSelectDoc* GetSelectDoc();
	virtual ~CStatisticDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CStatisticDoc)
		// HINWEIS - Der Klassen-Assistent f�gt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_STATISTICDOC_H__F2638DC9_4801_11D7_9822_00A0CC562BEB__INCLUDED_
