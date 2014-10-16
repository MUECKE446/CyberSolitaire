////////////////////////////////////////////////////////////////
// Copyright 1996 Microsoft Systems Journal. 
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// See dib.cpp

#ifndef _INC_VFW
#pragma message ("NOTE: You can speed compilation by including <vfw.h> in stdafx.h")
#include <vfw.h>
#endif


// global functions for ordinary CBitmap too
//
extern CSize GetBitmapSize(CBitmap* pBitmap);
extern BOOL  DrawBitmap(CDC& dc, CBitmap* pBitmap,
	const CRect* rcDst=NULL, const CRect* rcSrc=NULL, DWORD dwRop=SRCCOPY);

////////////////
// CDib implements Device Independent Bitmaps as a form of CBitmap. 
//
class CDib : public CBitmap {
protected:
	DECLARE_DYNAMIC(CDib)
	BITMAP	m_bm;		// stored for speed
	CPalette m_pal;		// palette
	HDRAWDIB m_hdd;		// for DrawDib

public:
	CDib();
	~CDib();

public:
	CArrayPoint	m_TransPoints;			// Array mit den Pixeln, die transparent erscheinen sollen

public:
	//BOOL WriteDIB( LPTSTR szFile, HANDLE hDIB);
	BOOL Write( LPTSTR szFile);
	void CopyTransPoints(CDib* pDstDib);
	CSize	GetSize() { return CSize(m_bm.bmWidth, m_bm.bmHeight); }
	BOOL Attach(HGDIOBJ hbm);
	BOOL Load(LPCTSTR szPathName);
	BOOL Load(HINSTANCE hInst, LPCTSTR lpResourceName);
	BOOL Load(HINSTANCE hInst, UINT uID)
		{ return Load(hInst, MAKEINTRESOURCE(uID)); }
	void Create(CDC& dc, CSize Size, CDib* pBitmap);

	BOOL CopyBitmap(CDC& dc, CBitmap* pBitmap, 
		const CRect* rcDst=NULL, const CRect* rcSrc=NULL, DWORD dwRop=SRCCOPY);

	BOOL CopyBitmapTransparent(CDC& dc, CBitmap* pBitmap, CBitmap* pBackBitmap, 
		const CRect* rcDst=NULL, const CRect* rcSrc=NULL, DWORD dwRop=SRCCOPY);

	// Universal Draw function can use DrawDib or not.
	BOOL Draw(CDC& dc, const CRect* rcDst=NULL, const CRect* rcSrc=NULL,
		BOOL bUseDrawDib=TRUE, DWORD dwRop=SRCCOPY, CPalette* pPal=NULL, BOOL bForeground=FALSE);

	BOOL DeleteObject();
	BOOL CreatePalette(CPalette& pal);
	CPalette* GetPalette()  { return &m_pal; }

	UINT GetColorTable(RGBQUAD* colorTab, UINT nColors);
};
