#if !defined(AFX_PROJVIEW_H__70ACB338_0CF5_11D3_8DAC_0000CB73095F__INCLUDED_)
#define AFX_PROJVIEW_H__70ACB338_0CF5_11D3_8DAC_0000CB73095F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjView.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Formularansicht CSelectView 

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSelectDoc;
class CGameTree;

class CSelectView : public CCtrlView
{
protected:
	DECLARE_DYNCREATE(CSelectView)

public:
	CSelectView(); 
	CTabCtrl& GetTabCtrl() const{ return *(CTabCtrl*)this; };

// Formulardaten
	//{{AFX_DATA(CSelectView)
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA

// Attribute
public:

// Operationen
public:
	CFont										m_selFont;
	CSelectDoc*							m_pDoc;
	CGameTree*							m_pGameTree;
	CTabCtrl*								m_pTab;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSelectView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CSelectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSelectView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDescription();
	afx_msg void OnDescriptionShort();
	afx_msg void OnUpdateDescription(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDescriptionShort(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CCyberSolitaireApp* m_pApp;
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROJVIEW_H__70ACB338_0CF5_11D3_8DAC_0000CB73095F__INCLUDED_
