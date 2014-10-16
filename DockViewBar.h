#if !defined(AFX_SIZEVIEWBAR_H__14EC9881_72DD_11D1_BA8A_006097DA5345__INCLUDED_)
#define AFX_SIZEVIEWBAR_H__14EC9881_72DD_11D1_BA8A_006097DA5345__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SizeViewBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSizeViewBar window

class CSizeViewBar : public CMRCSizeControlBar
{
	DECLARE_DYNCREATE(CSizeViewBar)
	// Construction
public:
	CSizeViewBar(){};
	CSizeViewBar(UINT width, UINT height, CString type);

// Attributes
public:
	CView	* m_pChildView;
	UINT m_menuId;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizeViewBar)
	public:
	//}}AFX_VIRTUAL
	void OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags);

// Implementation
public:
	CRect winRect;
	CString m_type;
	UINT m_width;
	UINT m_height;
	virtual ~CSizeViewBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSizeViewBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//class CSizeBarDocTemplate : public CMultiDocTemplate
class CSizeBarDocTemplate : public CSharedTemplate
{
	DWORD m_Style;
	DWORD m_Id;
	UINT m_menuID;

public:
	CPoint m_position;
	BOOL m_dockit;
	UINT m_barheight;
	UINT m_barwidth;
	CSizeViewBar * pBar;
	DECLARE_DYNAMIC(CSizeBarDocTemplate)
	CSizeBarDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
										 CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass,
										 DWORD style, DWORD Id, UINT width, UINT height, 
										 BOOL dockit, CPoint position);
	//CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther);
	CDocument* OpenDocumentFile(LPCTSTR lpszPathName,	BOOL bMakeVisible);


	CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIZEVIEWBAR_H__14EC9881_72DD_11D1_BA8A_006097DA5345__INCLUDED_)
