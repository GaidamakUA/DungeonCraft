// LaunchDlg.cpp : implementation file
//

#include "../Shared/stdafx.h"
#include "Launch.h"
#include <shlobj.h>

#include "LaunchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaunchDlg dialog

CLaunchDlg::CLaunchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaunchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaunchDlg)
	m_EnginePath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLaunchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaunchDlg)
	DDX_Control(pDX, IDC_DESIGNLIST, m_List);
	DDX_Text(pDX, IDC_ENGINEPATH, m_EnginePath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLaunchDlg, CDialog)
	//{{AFX_MSG_MAP(CLaunchDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_DESIGNLIST, OnDblclkDesignlist)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_ENGINESELECT, OnEngineselect)
	ON_BN_CLICKED(IDC_DESIGNSELECT, OnDesignselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaunchDlg message handlers

BOOL CLaunchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// insert columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "Name";
	lvc.cx = 125;
	lvc.fmt = LVCFMT_LEFT;
	m_List.InsertColumn(0,&lvc, CListSort::EDataType::dtSTRING, true);

	lvc.iSubItem = 1;
	lvc.pszText = "Version";
	lvc.cx = 65;
	lvc.fmt = LVCFMT_LEFT;
  m_List.InsertColumn(1,&lvc, CListSort::EDataType::dtSTRING, true);

	lvc.iSubItem = 2;
	lvc.pszText = "Path";
	lvc.cx = 160;
	lvc.fmt = LVCFMT_LEFT;
  m_List.InsertColumn(2,&lvc, CListSort::EDataType::dtSTRING, true);

	m_List.SetDefaultSortAttributes(0, CSortableListCtrl::ESortDirection::sdASCENDING);

  CString temp;
  if (!ConfigFile.FindToken("Game_Exec", temp))
    temp = "UAFWin.exe";
  m_EnginePath = temp;

  PopulateDesignList();

  UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLaunchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaunchDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLaunchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLaunchDlg::OnOK() 
{
  UpdateData(TRUE);

  POSITION pos = m_List.GetFirstSelectedItemPosition();

  if (pos != NULL)
  {
    int nItem = m_List.GetNextSelectedItem(pos);
    CString path;
    path = m_List.GetItemText(nItem,2);
    if (LaunchDesign(path))
      CDialog::OnOK();
  }		
}

void CLaunchDlg::OnDblclkDesignlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
  OnOK();	
	*pResult = 0;
}

void CLaunchDlg::PopulateDesignList()
{
  m_List.DeleteAllItems();

  char fullPath[_MAX_PATH+1];
  CString installDir;
  if (!ConfigFile.FindToken("Install_Dir", installDir))
    installDir = ".";

  if (installDir[installDir.GetLength()-1] != '\\')
    installDir += "\\";

  strcpy(fullPath, installDir);
  strcat(fullPath, "*.dsn");
 
  WIN32_FIND_DATA findData;
  HANDLE hSearch = FindFirstFile(fullPath, &findData);

  CString fullpath;

  if (hSearch != INVALID_HANDLE_VALUE)
  {
    do 
    {
      if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if (strlen(findData.cFileName) > 0)
        {
          fullpath = installDir;
          fullpath += findData.cFileName;

          AddDesign(fullpath);
        }
      }

    } while (FindNextFile(hSearch, &findData));
  }

  FindClose(hSearch);
	
  if (m_List.GetItemCount() > 0)
  {
    m_List.Sort();
    m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
    m_List.EnsureVisible(0, FALSE);  
  }
}

void CLaunchDlg::AddDesign(const char *path)
{
  int listIdx = m_List.GetItemCount();

  char name[_MAX_PATH+1];
  char dname[255];
  char version[50];

  strcpy(name, path);
  CString gamedat;
  gamedat.Format("%s\\game.dat", path);
  double ver;
  CString temp;

  GetDesignVersion(gamedat, ver, temp);
  if (ver == 0.0) return;

  sprintf(version, "%6.4f", ver);
  strncpy(dname, temp, 255);
  dname[254]='\0';

  LVITEM item;
	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.iItem = listIdx;
	item.iSubItem = 0;
	item.state = 0; 
	item.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	item.pszText = dname;
	item.cchTextMax = 0;
	item.iImage = 0;
	item.lParam = 0;

  m_List.InsertItem(&item);
  m_List.SetItemText(listIdx, 1, version);
  m_List.SetItemText(listIdx, 2, name);
}

void CLaunchDlg::GetDesignVersion(const char *fname, double &ver, CString &name)
{
  CFile myFile;
  ver=0.0;
  name="";
  
  try
  {
    if (!myFile.Open(fname, CFile::modeRead))
    {
      TRACE("Open Error: Unable to determine design version from file %s\n", fname);
      return;
    }   

    CArchive ar(&myFile, CArchive::load);  
    ar >> ver;
    ar >> name;
    if (name == "*")
      name="";
    ar.Close();
  }
  catch (...)
  {
    TRACE("Unable to determine design version from file %s\n", fname);
  }

  if (ver <= 0.0)
    TRACE("Error %u trying to determine design version from file %s\n", 
           GetLastError(), fname);
}

BOOL CLaunchDlg::LaunchDesign(const char *path)
{
  CString appName;
  char cmdLine[_MAX_PATH+1];
  char workDir[_MAX_PATH+1];

  //ConfigFile.FindToken("Game_Exec", appName);  
  UpdateData(TRUE);
  appName = m_EnginePath;

  strcpy(workDir, path);
  if (workDir[strlen(workDir)-1] == '\\')
    workDir[strlen(workDir)-1] = '\0';
  
  sprintf(cmdLine, "%s \"%s\"", 
          appName, workDir);

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  memset(&pi, 0, sizeof(pi));
  memset(&si, 0, sizeof(si));

  si.cb = sizeof(STARTUPINFO);
  si.dwFlags = STARTF_FORCEONFEEDBACK;

  BOOL success = CreateProcess(appName,
                               cmdLine,
                               NULL, NULL,
                               FALSE,
                               CREATE_NEW_PROCESS_GROUP,
                               NULL,
                               workDir,
                               &si,
                               &pi);
  if (!success)
  {
    CString msg;
    msg.Format("Unable to start \"%s\" and play \"%s\"",
               appName, path);
    ::MessageBox(NULL, msg, "CreateProcess Failure", MB_ICONERROR|MB_OK);
  }
  return success;
}

void CLaunchDlg::OnRefresh() 
{
  PopulateDesignList();	
}

BOOL GetFilename(CString& defaultFilename, CString Ext, BOOL LoadDlg)
{
  BOOL fileChosen = FALSE;

  int index;
  static CString lastDir;

  CString defaultDir;
  char szFilter[100];
  char ExtText[10];
  strncpy(ExtText, Ext, 3);
  ExtText[3] = '\0';

  sprintf(szFilter, "%s Files (*.%s)|*.%s||", ExtText, ExtText, ExtText);
  DWORD flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

  CString defFilename;
  index = defaultFilename.ReverseFind('\\');
  if (index > 1)
     defFilename = defaultFilename.Right(defaultFilename.GetLength() - index - 1);
  else
     defFilename = "";

   if (defaultFilename.IsEmpty())
   {
      if (!lastDir.IsEmpty())
         defaultDir = lastDir;
   }
   else
   {
      index = defaultFilename.ReverseFind('\\');
      if (index > 3)
         defaultDir = defaultFilename.Left(index);
   }

   //              True = Read, False = Write dialog
   CFileDialog dlg(LoadDlg, Ext, defFilename, flags, szFilter);

  dlg.m_ofn.lpstrInitialDir = (LPCSTR)defaultDir;

  DWORD result = dlg.DoModal();

  if (result == IDOK)
  {
     defaultFilename = dlg.GetPathName();

     index = defaultFilename.ReverseFind('\\');
     if (index > 3)
        lastDir = defaultFilename.Left(index);

     fileChosen = TRUE;
  }

  return fileChosen;
}

//*****************************************************************************
// NAME:    BrowseForFolder
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
CString BrowseForFolder(HWND hWnd, const char *pTitle)
{
  LPCSTR lpszTitle = pTitle;
  // We're going to use the shell to display a 
  // "Choose Directory" dialog box for the user.

  // set root at Desktop
  LPCITEMIDLIST pRoot = NULL;

  CString strResult = "";
  
  LPMALLOC lpMalloc;  // pointer to IMalloc
  // LpMalloc is a pointer to IMalloc, and IMalloc is the COM 
  // interface the Shell uses to allocate structures and pointers 
  // that it returns from most of its functions. To make the pointer
  // point to something, we call SHGetMalloc():
  
  if (::SHGetMalloc(&lpMalloc) != NOERROR)
    return strResult; // failed to get allocator
  
  char szDisplayName[_MAX_PATH];
  char szBuffer[_MAX_PATH];

  // The two buffers allocated after the call to SHGetMalloc() hold information 
  // that the Browse For Folder dialog box returns. szDisplayName will be put into 
  // the pszDisplayName member of the BROWSEINFO structure. This member receives 
  // the name only of the selected folder, not its path.
  
  BROWSEINFO browseInfo;
  browseInfo.hwndOwner = hWnd;
  
  browseInfo.pidlRoot = pRoot; 
  browseInfo.pszDisplayName = szDisplayName;
  browseInfo.lpszTitle = lpszTitle;   
  browseInfo.ulFlags = 0;   // also passed in
  browseInfo.lpfn = NULL;      // not used
  browseInfo.lParam = 0;      // not used   
  
  LPITEMIDLIST lpItemIDList;

  // Since we’re not having a callback function (look up BrowseCallbackProc in the 
  // documentation for the details), we simply NULL- and zero-out the lpfn and lParam 
  // members of the structure. The preceding LPITEMIDLIST variable will receive the 
  // return value of the SHBrowseForFolder() function. If the return value is NULL, 
  // we know SHBrowseForFolder() failed. So call it, testing for a non-NULL return
  // value:
  
  if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
  {
    // To actually get something useful with the LPITEMIDLIST pointer returned, 
    // we can use SHGetPathFromIDList(), passing our lpItemIDList and szBuffer 
    // variables as arguments. 
    // When the function returns, we should have a pathname to a folder in szBuffer. 
    // If szBuffer[0] is ‘\0’, the NULL-terminator, then we don’t have anything we 
    // can work with. Otherwise, we stuff szBuffer into strResult, using CString’s 
    // very useful assignment operator, and return the pathname to the caller as a 
    // reference to a CString, something that another of CString’s assignment 
    // operators accepts:
    
    // Get the path of the selected folder from the
    // item ID list.
    if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
    {
      // At this point, szBuffer contains the path 
      // the user chose.
      if (szBuffer[0] == '\0')
      {
        // SHGetPathFromIDList failed, or
        // SHBrowseForFolder failed.
        AfxMessageBox("Failed to get directory",
          MB_ICONSTOP|MB_OK);
      }
      
      // We have a path in szBuffer!
      // Return it.
      pRoot = lpItemIDList;
      strResult = szBuffer;
    }
    else
    {
      // The thing referred to by lpItemIDList 
      // might not have been a file system object.
      // For whatever reason, SHGetPathFromIDList
      // didn't work!
      AfxMessageBox("Failed to get directory",
        MB_ICONSTOP|MB_OK);
    }

    // Okay, now for the cleanup. The shell made use of its IMalloc interface 
    // (remember that LPMALLOC pointer we filled earlier) in order to allocate 
    // memory to hold our LPITEMIDLIST variable, which was filled by SHBrowseForFolder(). 
    // We need to clean up by freeing the memory through IMalloc::Free. Since IMalloc 
    // maintains a reference count to keep track of when it should do its own cleanup 
    // (as do all COM interfaces), we need to call the IUnknown::Release method. Memory
    // could leak if we don’t.    
    //
    lpMalloc->Free(lpItemIDList);
    lpMalloc->Release();      
  }
  
  // If we made it this far, SHBrowseForFolder failed.
  return strResult;
}

void CLaunchDlg::OnEngineselect() 
{
	UpdateData(TRUE);
  if (GetFilename(m_EnginePath, "exe", TRUE))
    UpdateData(FALSE);
}

void CLaunchDlg::OnDesignselect() 
{
	UpdateData(TRUE);
  CString des;
  des = BrowseForFolder(AfxGetMainWnd()->m_hWnd, "Browse for Design Folder");

  if (des != "")
  {
    bool found = false;
    for (int i=0;i<m_List.GetItemCount() && !found; i++)
    {
      if (m_List.GetItemText(i, 2) == des)
        found = true;
    }		
    if (!found)
      AddDesign(des);
  }
}
