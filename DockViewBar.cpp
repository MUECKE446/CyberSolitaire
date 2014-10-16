// SizeViewBar.cpp : implementation file
//

#include "stdafx.h"
//#include "LogiCAD6.h"
#include "DockViewBar.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizeViewBar

IMPLEMENT_DYNCREATE(CSizeViewBar, CMRCSizeControlBar)


CSizeViewBar::CSizeViewBar( UINT width, UINT height, CString type) : CMRCSizeControlBar(/*SZBARF_DESTROY_ON_CLOSE |*/ SZBARF_AUTOTIDY /*| SZBARF_STDMOUSECLICKS*/ )
{

	m_pChildView = NULL;
	m_height = height;
	m_width = width;
	m_HorzDockSize.cy = m_height;
	m_VertDockSize.cx = m_width;
	m_type = type;
}


CSizeViewBar::~CSizeViewBar()
{
	CString Param;

	if (IsProbablyFloating())
  {
		Param.Format("%sposx",m_type);
		Param.MakeUpper();
		AfxGetApp()->WriteProfileInt( "WINDOWS", Param, winRect.left);			
		Param.Format("%sposy",m_type);
		Param.MakeUpper();
		AfxGetApp()->WriteProfileInt( "WINDOWS", Param, winRect.top);			
		Param.Format("%sdocked",m_type);
		Param.MakeUpper();
		AfxGetApp()->WriteProfileInt( "WINDOWS", Param, 0);			
	}
	else
	{
		Param.Format("%sposx",m_type);
		Param.MakeUpper();
		AfxGetApp()->WriteProfileInt( "WINDOWS", Param, 0);			
		Param.Format("%sposy",m_type);
		Param.MakeUpper();
		AfxGetApp()->WriteProfileInt( "WINDOWS", Param, 0);			
		Param.Format("%sdocked",m_type);
		Param.MakeUpper();
		AfxGetApp()->WriteProfileInt( "WINDOWS", Param, 1);			
  	Param.Format("%sheight",m_type);
	  Param.MakeUpper();
  	AfxGetApp()->WriteProfileInt("WINDOWS",Param, m_HorzDockSize.cy);
  	Param.Format("%swidth",m_type);
  	Param.MakeUpper();
	  AfxGetApp()->WriteProfileInt("WINDOWS",Param, m_VertDockSize.cx);
	}

}


BEGIN_MESSAGE_MAP(CSizeViewBar, CMRCSizeControlBar)
	//{{AFX_MSG_MAP(CSizeViewBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSizeViewBar message handlers

int CSizeViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	lpCreateStruct->style |= (/*SZBARF_DESTROY_ON_CLOSE |*/ SZBARF_AUTOTIDY /*| SZBARF_STDMOUSECLICKS */);
	if (CMRCSizeControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//------------------ calculate size of project window	
	m_HorzDockSize.cy = m_height;
	m_VertDockSize.cx = m_width;

	return 0;
}


//-------------------------------------------------------------------
void CSizeViewBar::OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags)
// respond to this event as we need to override it
//-------------------------------------------------------------------
{
	CRect rect(0, 0, cx, cy);		// rectangle for client area
	
	BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	
	// shrink rectangle if we're docked
	if (IsProbablyFloating())
  {
     rect.InflateRect(-2, -2);		// shrink for border when floating
  }
  else
		rect.InflateRect(-6, -6);

	if (m_pChildView != NULL && m_pChildView->m_hWnd != NULL)
	{
		m_pChildView->MoveWindow(&rect);
		m_pChildView->GetWindowRect((LPRECT)winRect);
	}
	m_height = rect.bottom;
	m_width  = rect.right;
}


IMPLEMENT_DYNAMIC(CSizeBarDocTemplate, CSharedTemplate)

CSizeBarDocTemplate::CSizeBarDocTemplate(UINT nID, CRuntimeClass* pDocClass,
										 CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass,
										 DWORD style, DWORD Id, UINT width, UINT height, 
										 BOOL dockit, CPoint position)
										 :CSharedTemplate(nID, pDocClass, pFrameClass, pViewClass)
{
	m_Style = style;
	m_Id    = Id;
	m_barwidth = width;
	m_barheight = height;
	m_dockit = dockit;
	m_position = position;
}



CDocument* CSizeBarDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName,
																									BOOL bMakeVisible)
{
	CDocument* pDocument = CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE0("CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	
	// Create the bar, and put the view in it
	CString typ;
	typ.Format("DOC%d", m_Id);
	pBar = new CSizeViewBar( m_barwidth, m_barheight, typ );
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainFrame != NULL);

	DWORD dwStyle = m_Style | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN ;
	VERIFY(pBar->Create(NULL, NULL, dwStyle, CFrameWnd::rectDefault,
											pMainFrame, m_Id));
	
 	// Create the view 
	CView* pView = (CView*)m_pViewClass->CreateObject();
	if (pView == NULL)
	{
		TRACE1("Warning: Dynamic create of view type %hs failed.\n",
			m_pViewClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CWnd, pView);

	// views are always created with a border!
	CCreateContext ccontext;
	ccontext.m_pCurrentDoc = pDocument;
	
	if(!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
										CRect(0,0,0,0), pBar, -1 /*AFX_IDW_PANE_FIRST*/ , &ccontext))
	{
		TRACE0("Warning: could not create view for frame.\n");
		return NULL;        // can't continue without a view
	}
	
	pBar->m_pChildView = pView;
	pBar->EnableDocking(CBRS_ALIGN_ANY);
    pMainFrame->DockControlBar(pBar);

	pDocument->m_bAutoDelete = bAutoDelete;

	if (lpszPathName == NULL)
	{
		// create a new document - with default document name
		SetDefaultTitle(pDocument);

		// avoid creating temporary compound file when starting up invisible
		if (!bMakeVisible)
			pDocument->m_bEmbedded = TRUE;

		if (!pDocument->OnNewDocument())
		{
			// user has be alerted to what failed in OnNewDocument
			TRACE0("CDocument::OnNewDocument returned FALSE.\n");
			//pFrame->DestroyWindow();
			return NULL;
		}

		// it worked, now bump untitled count
		m_nUntitledCount++;
	}
	else
	{
		// open an existing document
		CWaitCursor wait;
		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE.\n");
			//pFrame->DestroyWindow();
			return NULL;
		}
		pDocument->SetPathName(lpszPathName);
	}

  // set title for control bar
  pBar->SetWindowText(pDocument->GetTitle());

   //InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	if (bMakeVisible)
	{
		// send initial update to all views (and other controls) in the frame
		pBar->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

		// give view a chance to save the focus (CFormView needs this)
		//if (pView != NULL)
		//	pView->OnActivateFrame(WA_INACTIVE, this);

		//if (pView != NULL)
		//	pView->OnActivateView(TRUE, pView, pView);
	}

	// update frame counts and frame title (may already have been visible)
	if (pDocument != NULL)
		pDocument->UpdateFrameCounts();

	if( m_dockit != TRUE )
	{
    pMainFrame->FloatControlBar(pBar, m_position );
		bMakeVisible=TRUE;
	}


	return pDocument; 
}


CFrameWnd* CSizeBarDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther)
// window NEW can't work for us.
{

	return NULL;
}








