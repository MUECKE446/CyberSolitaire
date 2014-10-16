////////////////////////////////////////////////////////////////
// Copyright 1996 Microsoft Systems Journal. 
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CDib - Device Independent Bitmap.
// This implementation draws bitmaps using normal Win32 API functions,
// not DrawDib. CDib is derived from CBitmap, so you can use it with
// any other MFC functions that use bitmaps.
//
#include "StdAfx.h"
#include "CyberSolitaire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const  int		MAXPALCOLORS = 256;

IMPLEMENT_DYNAMIC(CDib, CObject)

CDib::CDib()
{
	memset(&m_bm, 0, sizeof(m_bm));
	m_hdd = NULL;
	//m_TransPoints.SetSize(1);

}

CDib::~CDib()
{
	DeleteObject();
}

//////////////////
// Delete Object. Delete DIB and palette.
//
BOOL CDib::DeleteObject()
{
	m_pal.DeleteObject();
	if (m_hdd) {
		DrawDibClose(m_hdd);
		m_hdd = NULL;
	}
	memset(&m_bm, 0, sizeof(m_bm));
	return CBitmap::DeleteObject();
}

//////////////////
// Read DIB from file.
//
BOOL CDib::Load(LPCTSTR lpszPathName)
{
	return Attach(::LoadImage(NULL, lpszPathName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
}

//////////////////
// Load bitmap resource. Never tested.
//
BOOL CDib::Load(HINSTANCE hInst, LPCTSTR lpResourceName)
{
	return Attach(::LoadImage(hInst, lpResourceName, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
}

//////////////////
// Creates a DIB with ClientRect size.
//
void CDib::Create(CDC& dc, CSize Size, CDib* pBitmap)
{
	// Size must be in pixel
	CPoint ZeroPnt(0,0);
	CRect ClientRect(ZeroPnt,Size);
	CDC memDC,CopyDC;
    if (!CopyDC.CreateCompatibleDC(&dc) || !memDC.CreateCompatibleDC(&dc)) 
    {
        TRACE0("Unable to create compatible DC's\n");
        return;
    }

	CBitmap* pOldBm = memDC.SelectObject(pBitmap);
	DeleteObject();
	HBITMAP hbm = ::CreateCompatibleBitmap(memDC.GetSafeHdc(),Size.cx,Size.cy);
	Attach(hbm);
    CBitmap* pOldCopyBm = CopyDC.SelectObject(this);
	CopyDC.SetStretchBltMode(COLORONCOLOR);
	CSize SrcSize = pBitmap->GetSize();
	CopyDC.StretchBlt(ClientRect.left, ClientRect.top, ClientRect.Width(),ClientRect.Height(), &memDC, 
		0, 0, SrcSize.cx,SrcSize.cy, SRCCOPY);
	memDC.SelectObject(pOldBm);
    CopyDC.SelectObject(pOldCopyBm);

}

//////////////////
// Attach is just like the CGdiObject version,
// except it also creates the palette
//
BOOL CDib::Attach(HGDIOBJ hbm)
{
	if (CBitmap::Attach(hbm)) {
		if (!GetBitmap(&m_bm))			// load BITMAP for speed
			return FALSE;
		m_pal.DeleteObject();			// in case one is already there
		return CreatePalette(m_pal);	// create palette
	}
	return FALSE;	
}

//////////////////
// Get size (width, height) of bitmap.
// extern fn works for ordinary CBitmap objects.
//
CSize GetBitmapSize(CBitmap* pBitmap)
{
	BITMAP bm;
	return pBitmap->GetBitmap(&bm) ?
		CSize(bm.bmWidth, bm.bmHeight) : CSize(0,0);
}

//////////////////
// You can use this static function to draw ordinary
// CBitmaps as well as CDibs
//
BOOL DrawBitmap(CDC& dc, CBitmap* pBitmap,
	const CRect* rcDst, const CRect* rcSrc, DWORD dwRop)
{
	// Compute rectangles where NULL specified
	CRect rc;
	if (!rcSrc) {
		// if no source rect, use whole bitmap
		rc = CRect(CPoint(0,0), GetBitmapSize(pBitmap));
		rcSrc = &rc;
	}

	if (!rcDst) {
		// if no destination rect, use source
		rcDst=rcSrc;
	}

	// Create memory DC
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap* pOldBm = memdc.SelectObject(pBitmap);

	// Blast bits from memory DC to target DC.
	// Use StretchBlt if size is different.
	//
	BOOL bRet = FALSE;
	if (rcDst->Size()==rcSrc->Size()) {
		bRet = dc.BitBlt(rcDst->left, rcDst->top, 
			rcDst->Width(), rcDst->Height(),
			&memdc, rcSrc->left, rcSrc->top, dwRop);
	} else {
		dc.SetStretchBltMode(COLORONCOLOR);
		bRet = dc.StretchBlt(rcDst->left, rcDst->top, rcDst->Width(),
			rcDst->Height(), &memdc, rcSrc->left, rcSrc->top, rcSrc->Width(),
			rcSrc->Height(), dwRop);
	}
	memdc.SelectObject(pOldBm);

	return bRet;
}

////////////////////////////////////////////////////////////////
// Draw DIB on caller's DC. Does stretching from source to destination
// rectangles. Generally, you can let the following default to zero/NULL:
//
//		bUseDrawDib = whether to use use DrawDib, default TRUE
//		pPal	      = palette, default=NULL, (use DIB's palette)
//		bForeground = realize in foreground (default FALSE)
//
// If you are handling palette messages, you should use bForeground=FALSE,
// since you will realize the foreground palette in WM_QUERYNEWPALETTE.
//
BOOL CDib::Draw(CDC& dc, const CRect* rcDst, const CRect* rcSrc,
	BOOL bUseDrawDib, DWORD dwRop, CPalette* pPal, BOOL bForeground)
{
	if (!m_hObject)
		return FALSE;

	// Select, realize palette
	if (pPal==NULL)				// no palette specified:
		pPal = GetPalette();		// use default
	CPalette* pOldPal = dc.SelectPalette(pPal, !bForeground);
	dc.RealizePalette();

	BOOL bRet = FALSE;
	if (bUseDrawDib) {
		// Compute rectangles where NULL specified
		//
		CRect rc(0,0,-1,-1);	// default for DrawDibDraw
		if (!rcSrc)
			rcSrc = &rc;
		if (!rcDst)
			rcDst=rcSrc;
		if (!m_hdd)
			VERIFY(m_hdd = DrawDibOpen());

		// Get BITMAPINFOHEADER/color table. I copy into stack object each time.
		// This doesn't seem to slow things down visibly.
		//
		DIBSECTION ds;
		VERIFY(GetObject(sizeof(ds), &ds)==sizeof(ds));
		char buf[sizeof(BITMAPINFOHEADER) + MAXPALCOLORS*sizeof(RGBQUAD)];
		BITMAPINFOHEADER& bmih = *(BITMAPINFOHEADER*)buf;
		RGBQUAD* colors = (RGBQUAD*)(&bmih+1);
		memcpy(&bmih, &ds.dsBmih, sizeof(bmih));
		GetColorTable(colors, MAXPALCOLORS);

		// Let DrawDib do the work!
		bRet = DrawDibDraw(m_hdd, dc,
			rcDst->left, rcDst->top, rcDst->Width(), rcDst->Height(),
			&bmih,			// ptr to BITMAPINFOHEADER + colors
			m_bm.bmBits,	// bits in memory
			rcSrc->left, rcSrc->top, rcSrc->Width(), rcSrc->Height(),
			bForeground ? 0 : DDF_BACKGROUNDPAL);

	} else {
		// Get BITMAPINFOHEADER/color table. I copy into stack object each time.
		// This doesn't seem to slow things down visibly.
		//
		DIBSECTION ds;
		VERIFY(GetObject(sizeof(ds), &ds)==sizeof(ds));
		char buf[sizeof(BITMAPINFOHEADER) + MAXPALCOLORS*sizeof(RGBQUAD)];
		BITMAPINFOHEADER& bmih = *(BITMAPINFOHEADER*)buf;
		RGBQUAD* colors = (RGBQUAD*)(&bmih+1);
		memcpy(&bmih, &ds.dsBmih, sizeof(bmih));
		GetColorTable(colors, MAXPALCOLORS);

		// use normal draw function
		bRet = DrawBitmap(dc, this, rcDst, rcSrc, dwRop);
	}
	if (pOldPal)
		dc.SelectPalette(pOldPal, TRUE);
	return bRet;
}

#define PALVERSION 0x300	// magic number for LOGPALETTE

//////////////////
// Create the palette. Use halftone palette for hi-color bitmaps.
//
BOOL CDib::CreatePalette(CPalette& pal)
{ 
	// should not already have palette
	ASSERT(pal.m_hObject==NULL);

	BOOL bRet = FALSE;
	RGBQUAD* colors = new RGBQUAD[MAXPALCOLORS];
	UINT nColors = GetColorTable(colors, MAXPALCOLORS);
	if (nColors > 0) {
		// Allocate memory for logical palette 
		int len = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColors;
		LOGPALETTE* pLogPal = (LOGPALETTE*)new char[len];
		if (!pLogPal)
			return NULL;

		// set version and number of palette entries
		pLogPal->palVersion = PALVERSION;
		pLogPal->palNumEntries = nColors;

		// copy color entries 
		for (UINT i = 0; i < nColors; i++) {
			pLogPal->palPalEntry[i].peRed   = colors[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = colors[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue  = colors[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}

		// create the palette and destroy LOGPAL
		bRet = pal.CreatePalette(pLogPal);
		delete [] (char*)pLogPal;
	} else {
		CWindowDC dcScreen(NULL);
		bRet = pal.CreateHalftonePalette(&dcScreen);
	}
	delete colors;
	return bRet;
}

//////////////////
// Helper to get color table. Does all the mem DC voodoo.
//
UINT CDib::GetColorTable(RGBQUAD* colorTab, UINT nColors)
{
	CWindowDC dcScreen(NULL);
	CDC memdc;
	memdc.CreateCompatibleDC(&dcScreen);
	CBitmap* pOldBm = memdc.SelectObject(this);
	nColors = GetDIBColorTable(memdc, 0, nColors, colorTab);
	memdc.SelectObject(pOldBm);
	return nColors;
}

//////////////////
// Use this function to copy a CDib bitmap to another (or a part of it)
//
//
BOOL CDib::CopyBitmap(CDC& dc, CBitmap* pBitmap, const CRect* rcDst, const CRect* rcSrc, DWORD dwRop)
{
	// Compute rectangles where NULL specified
	CRect rc;
	if (!rcSrc) {
		// if no source rect, use whole bitmap
		rc = CRect(CPoint(0,0), GetBitmapSize(pBitmap));
		rcSrc = &rc;
	}
	if (!rcDst) {
		// if no destination rect, use source
		rcDst=rcSrc;
	}

	// Create memory DC
    CDC memDC, CopyDC;
    if (!CopyDC.CreateCompatibleDC(&dc) || !memDC.CreateCompatibleDC(&dc)) 
    {
        TRACE0("Unable to create compatible DC's\n");
        return FALSE;
		//AfxThrowResourceException();
    }
	CBitmap* pOldBm = memDC.SelectObject(pBitmap);
    CBitmap* pOldCopyBm = CopyDC.SelectObject(this);

	// Blast bits from memory DC to target DC.
	// Use StretchBlt if size is different.
	//
    GdiFlush();
	BOOL bRet = FALSE;
	if (rcDst->Size()==rcSrc->Size()) {
		bRet = CopyDC.BitBlt(rcDst->left, rcDst->top, 
			rcDst->Width(), rcDst->Height(),
			&memDC, rcSrc->left, rcSrc->top, dwRop);
	} else {
		dc.SetStretchBltMode(COLORONCOLOR);
		bRet = CopyDC.StretchBlt(rcDst->left, rcDst->top, rcDst->Width(),
			rcDst->Height(), &memDC, rcSrc->left, rcSrc->top, rcSrc->Width(),
			rcSrc->Height(), dwRop);
	}

	memDC.SelectObject(pOldBm);
	CopyDC.SelectObject(pOldCopyBm);

	return bRet;
}

//////////////////
// Use this function to copy a CDib bitmap to another (or a part of it)
// the copy is made transparency. The color of the TopLeft point from rcDst is choosen
//
BOOL CDib::CopyBitmapTransparent(CDC &dc, CBitmap *pBitmap, CBitmap *pBackBitmap, const CRect *rcDst, const CRect *rcSrc, DWORD dwRop)
{
	// Compute rectangles where NULL specified
	CRect rc;
	if (!rcSrc) {
		// if no source rect, use whole bitmap
		rc = CRect(CPoint(0,0), GetBitmapSize(pBitmap));
		rcSrc = &rc;
	}
	if (!rcDst) {
		// if no destination rect, use source
		rcDst=rcSrc;
	}

	// Create memory DC
    CDC memDC, CopyDC,backDC;
    if (!CopyDC.CreateCompatibleDC(&dc) || !memDC.CreateCompatibleDC(&dc) || !backDC.CreateCompatibleDC(&dc)) 
    {
        TRACE0("Unable to create compatible DC's\n");
        return FALSE;
		//AfxThrowResourceException();
    }
	CBitmap* pOldBm = memDC.SelectObject(pBitmap);
	CBitmap* pOldBackBm = backDC.SelectObject(pBackBitmap);
    CBitmap* pOldCopyBm = CopyDC.SelectObject(this);

	// Blast bits from memory DC to target DC.
	// Use StretchBlt if size is different.
	//
    GdiFlush();
	BOOL bRet = FALSE;
	if (rcDst->Size()==rcSrc->Size()) {
		bRet = CopyDC.BitBlt(rcDst->left, rcDst->top, 
			rcDst->Width(), rcDst->Height(),
			&memDC, rcSrc->left, rcSrc->top, dwRop);
	} else {
		dc.SetStretchBltMode(COLORONCOLOR);
		bRet = CopyDC.StretchBlt(rcDst->left, rcDst->top, rcDst->Width(),
			rcDst->Height(), &memDC, rcSrc->left, rcSrc->top, rcSrc->Width(),
			rcSrc->Height(), dwRop);
	}

	for (int i=0; i<((CDib*)pBitmap)->m_TransPoints.GetSize(); i++)
	{
		CPoint TransPoint = ((CDib*)pBitmap)->m_TransPoints[i];
		TransPoint += rcDst->TopLeft();
		if (rcDst->PtInRect(TransPoint))
			CopyDC.SetPixel(TransPoint.x,TransPoint.y,backDC.GetPixel(TransPoint.x,TransPoint.y));
	}


	memDC.SelectObject(pOldBm);
	backDC.SelectObject(pOldBackBm);
	CopyDC.SelectObject(pOldCopyBm);


	return bRet;
}

void CDib::CopyTransPoints(CDib *pDstDib)
{
	pDstDib->m_TransPoints.RemoveAll();
	for (int i=0; i<m_TransPoints.GetSize(); i++)
	{
		pDstDib->m_TransPoints.Add(m_TransPoints[i]);
	}
}

// WriteDIB		- Writes a DIB to file
// Returns		- TRUE on success
// szFile		- Name of file to write to
// hDIB			- Handle of the DIB
//BOOL CDib::WriteDIB( LPTSTR szFile, HANDLE hDIB)
BOOL CDib::Write(LPTSTR szFile)
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;


	ASSERT( GetSafeHandle() );

	// The function has no arg for bitfields
	//if( dwCompression == BI_BITFIELDS )
	//	return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) m_pal.GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	//bi.biCompression	= dwCompression;
	bi.biCompression	= BI_RGB;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)this->GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		//if (dwCompression != BI_RGB)
		//	bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)this->GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	ReleaseDC(NULL,hDC);
	
/////////////////////////////////////////////////////////////////////////////////////////////////7	
	
	BITMAPFILEHEADER	hdr;
	//LPBITMAPINFOHEADER	lpbi;


	if (!hDIB)
		return FALSE;

	CFile file;
	if( !file.Open( szFile, CFile::modeWrite|CFile::modeCreate) )
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	nColors = 1 << lpbi->biBitCount;

	// Fill in the fields of the file header 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= GlobalSize (hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	// Write the file header 
	file.Write( &hdr, sizeof(hdr) );

	// Write the DIB header and the bits 
	file.Write( lpbi, GlobalSize(hDIB) );

	return TRUE;
}


