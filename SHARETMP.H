// sharetmp.h : interface of the CSharedTemplate class
//
/////////////////////////////////////////////////////////////////////////////

class CSharedTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CSharedTemplate)
 public:
	CSharedTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
           CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

    virtual ~CSharedTemplate();
	void SetDefaultTitle(CDocument* pDocument);
};
