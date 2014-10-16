// MainFrm.h : Schnittstelle der Klasse CMainFrame
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__5247D2DB_7130_41CE_837D_8872C91AD023__INCLUDED_)
#define AFX_MAINFRM_H__5247D2DB_7130_41CE_837D_8872C91AD023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CFRAMEPARENT CMRCMDIFrameWndSizeDock

class CMainFrame : public CFRAMEPARENT
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attribute
public:

// Operationen
public:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementierung
public:
	BOOL AskCloseApplication();
	void SetActiveViewDirect( CView* pView );
	BOOL m_MainWndIsSizing;
	CStatusBar		m_wndStatusBar;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void GetSizeWorkspace( CSize &rect);
	BOOL IsClosingApplication() {return m_ClosingApplication;}

protected:  // Eingebundene Elemente der Steuerleiste
	CToolBar		m_wndToolBar;
	CReBar			m_wndReBar;
	CDialogBar      m_wndDlgBar;
	CToolBar		m_wndShowToolBar;

	BOOL			m_ClosingApplication;

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowGametree();
	afx_msg void OnUpdateShowGametree(CCmdUI* pCmdUI);
	afx_msg void OnErrorReport();
	afx_msg void OnUpdateErrorReport(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSpielTictactoespielen();
	afx_msg void OnUpdateSpielTictactoespielen(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnSetPaneInStatusbar (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterSizeMove (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitSizeMove (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTTTGameLoaded (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	// schaltet auf den TTT StatusBar, der außerhalb spielbarer Felder gezeigt wird
	void CMainFrame::SwitchToTTTStatusBar(void);
	// schaltet auf den TTT StatusBar, der bei spielbaren Feldern gezeigt wird
	void CMainFrame::SwitchToTTTFieldStatusBar(void);
	// schaltet auf den Game StatusBar, der während eines normalen Spieles gezeigt wird
	void CMainFrame::SwitchToGameStatusBar(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_MAINFRM_H__5247D2DB_7130_41CE_837D_8872C91AD023__INCLUDED_)
