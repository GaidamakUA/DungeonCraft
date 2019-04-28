// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C1554EB6_C808_4AA8_8B4C_59BA6A6FFF0E__INCLUDED_)
#define AFX_STDAFX_H__C1554EB6_C808_4AA8_8B4C_59BA6A6FFF0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//#include <afx.h>
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
//#include <afxtempl.h>
extern CString jsonPath;
#include "J.h"
void die(const char *msg);
void NotImplemented(int msg, bool);
void WriteLine(FILE *f, const char*key, const char *value);

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C1554EB6_C808_4AA8_8B4C_59BA6A6FFF0E__INCLUDED_)
