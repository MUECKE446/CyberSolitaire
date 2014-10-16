
#ifndef __CRACK_H__
#define __CRACK_H__

class CCrack
{
public:
	static LPCTSTR strFieldType(short sType);
	static LPCTSTR strQueryDefType(short sType);
	static LPCTSTR strBOOL(BOOL bFlag);
	static CString strVARIANT(const COleVariant& var);
};
#endif