// sharetmp.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "CyberSolitaire.h"

//#include "ShareTmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSharedTemplate::CSharedTemplate(UINT nIDResource,
  CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
  CRuntimeClass* pViewClass) : CMultiDocTemplate(nIDResource, pDocClass,
                                 pFrameClass, pViewClass)
{
}

CSharedTemplate::~CSharedTemplate()
{
  // This prevents the base class virtual destructor from
  // destroying the menu resource (i.e. it checks to make
  // sure the handle isn't NULL)

  m_hMenuShared = NULL;
}
void CSharedTemplate::SetDefaultTitle(CDocument* pDocument)
{
	CString strDocName;
	if (GetDocString(strDocName, CDocTemplate::docName) &&
		!strDocName.IsEmpty())
	{
		if (strDocName == "CybSol")
		{
			strDocName = GetApplication()->GetGameName();
		}
		else
		{
			TCHAR szNum[8];
			wsprintf(szNum, _T("%d"), m_nUntitledCount+1);
			strDocName += szNum;
		}
	}
	else
	{
		// use generic 'untitled' - ignore untitled count
		VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
	}
	pDocument->SetTitle(strDocName);
}


IMPLEMENT_DYNAMIC(CSharedTemplate, CMultiDocTemplate)
