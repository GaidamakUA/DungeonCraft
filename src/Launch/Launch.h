// Launch.h : main header file for the LAUNCH application
//

#if !defined(AFX_LAUNCH_H__9F9E8B8F_823B_46F8_9E78_334E2F0AD75F__INCLUDED_)
#define AFX_LAUNCH_H__9F9E8B8F_823B_46F8_9E78_334E2F0AD75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../Shared/FileParse.h"

/////////////////////////////////////////////////////////////////////////////
// CLaunchApp:
// See Launch.cpp for the implementation of this class
//

class CLaunchApp : public CWinApp
{
public:
	CLaunchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaunchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLaunchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCH_H__9F9E8B8F_823B_46F8_9E78_334E2F0AD75F__INCLUDED_)
