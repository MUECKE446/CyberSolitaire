// stdafxCLR.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//

#if !defined(AFX_STDAFX_H__C8BA607D_FD5B_4105_81E4_3CC3A9C4E38A__INCLUDED_)
#define AFX_STDAFX_H__C8BA607D_FD5B_4105_81E4_3CC3A9C4E38A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdao.h>			// MFC DAO Database support
#include <afxdisp.h>        // MFC Automatisierungsklassen
#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#include <afxdb.h>          // MFC database classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC-Socket-Erweiterungen
#include <afxtempl.h>		// Templates
#include <afxcoll.h>		// Collections
#include <afxdlgs.h>        // MFC OLE-Dialogfeldklassen
#include <afxrich.h>		// RichEditCtrl Klassen

#include <afxpriv.h>		// for WM_INITIALUPDATE
#include <afxcmn.h>			// for WM_INITIALUPDATE
#include <afxmt.h>
#include <vfw.h>

#include <algorithm>
#include <deque>

using namespace std;

#include "mrcext.h"			// MRCEXT headers
#include "SHARETMP.h"

// ********************* macro... *************************

#define IsK(_class_) IsKindOf(RUNTIME_CLASS(_class_))


// simple arrays and maps
#define CArrayInt		CArray<int, int>
#define	CArrayByte		CArray<BYTE, BYTE>
#define CArrayDouble	CArray<double, double>
#define CArrayPoint		CArray<CPoint, CPoint>
#define CArrayRect		CArray<CRect, CRect>

#include "Dib.h"

#pragma warning(disable : 4995)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__C8BA607D_FD5B_4105_81E4_3CC3A9C4E38A__INCLUDED_)
