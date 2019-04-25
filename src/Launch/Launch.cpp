// Launch.cpp : Defines the class behaviors for the application.
//

#include "..\Shared\stdafx.h"
#include <sys/types.h> 
#include <sys/stat.h>
#include "Launch.h"
#include "LaunchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLaunchApp

BEGIN_MESSAGE_MAP(CLaunchApp, CWinApp)
	//{{AFX_MSG_MAP(CLaunchApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaunchApp construction


CLaunchApp theApp;

CLaunchApp::CLaunchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLaunchApp object


/////////////////////////////////////////////////////////////////////////////
// CLaunchApp initialization

BOOL CLaunchApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  char path[_MAX_PATH+1];
  GetModuleFileName(NULL, path, sizeof(path));

  char *ptr = strrchr(path, '\\');

  if (ptr != NULL)
  {
     ptr++;
     *ptr = '\0';
  }

  CString filename;
  filename.Format("%sconfig.txt", path);
  ConfigFile.Parse(filename);

	CLaunchDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL FileExists(const char *file)
{
   struct _tstat st;
   BOOL success;

   if (file == NULL)
     return FALSE;

   if (_tstat(file, &st) != 0)
      success = FALSE;
   else
     success = TRUE;

   return success;
}