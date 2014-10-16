// SolEditFrame.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "MainFrm.h"
#include "SolEditFrame.h"
#include ".\soleditframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolEditFrame

IMPLEMENT_DYNCREATE(CSolEditFrame, CMDIChildWnd)

CSolEditFrame::CSolEditFrame()
{
}

CSolEditFrame::~CSolEditFrame()
{
}


BEGIN_MESSAGE_MAP(CSolEditFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSolEditFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSolEditFrame 

BOOL CSolEditFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// um dem CyberSolitaireDoc sein Icon zu bescheren
	//cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, NULL, NULL, AfxGetApp()->LoadIcon(IDI_CYBERSOLITAIREDOC));
	cs.style = WS_CHILD | WS_VISIBLE | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MAXIMIZE;
	return CMDIChildWnd::PreCreateWindow(cs);
}

