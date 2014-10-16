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
	CStatisticDoc();           // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CStatisticDoc)

// Attribute
public:

// Operationen
public:
	CString					m_GameName;
	CStatistic*				m_pStatistic;
	CSolLayout*				m_pSolLayout;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CStatisticDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);   // Überschrieben für Dokument-Ein-/Ausgabe
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
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_STATISTICDOC_H__F2638DC9_4801_11D7_9822_00A0CC562BEB__INCLUDED_
