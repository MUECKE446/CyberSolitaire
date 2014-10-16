// SolEditDoc.h : Schnittstelle der Klasse CSolEditDoc
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLEDITDOC_H__3BA9180E_0AAB_4BEF_A890_D66BEBB106BF__INCLUDED_)
#define AFX_SOLEDITDOC_H__3BA9180E_0AAB_4BEF_A890_D66BEBB106BF__INCLUDED_

#include "CyberSolitaire.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSelectDoc;

class CSolEditDoc : public CDocument
{
	friend class CSolEditView;

protected: // Nur aus Serialisierung erzeugen
	CSolEditDoc();
	DECLARE_DYNCREATE(CSolEditDoc)

// Attribute
public:
	gameScoring			m_TypeOfScoring	;	// wie wird gezählt
	CSolLayout*			m_pLayout;

// Operationen
public:
	CString GetDateTimeVersion();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CSolEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementierung
public:
	CSelectDoc* GetSelectDoc();
	void InitNewGame(BOOL WithDialog = TRUE );
	void InitStockPile(CStock* pPile);
	void InitTableauPile(CTableau* pPile);
	void InitFoundationPile(CFoundation* pPile);
	void InitWastePile(CWaste* pPile);
	//void TestEmptyPiles();
	CStock* GetStock();
	void UpdatePiles();
	CSolEditView* GetView();
	virtual ~CSolEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSolEditView* m_pView;

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CSolEditDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCloseDocument();
	afx_msg void OnSortierenNachRedealOrder();
	afx_msg void OnFileClose();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_SOLEDITDOC_H__3BA9180E_0AAB_4BEF_A890_D66BEBB106BF__INCLUDED_)
