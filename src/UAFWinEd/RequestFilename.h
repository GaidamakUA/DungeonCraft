/******************************************************************************
* Filename: RequestFilename.h
* Copyright (c) 2000, UAF Development Team (email CocoaSpud@hotmail.com)
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
******************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CRequestDesignFilename dialog

class CRequestDesignFilename : public CDialog
{
// Construction
public:
	CRequestDesignFilename(CString file, BOOL AllowExisting=TRUE, CWnd* pParent=NULL);   // standard constructor
  CString m_file;

// Dialog Data
	//{{AFX_DATA(CRequestDesignFilename)
	enum { IDD = IDD_GETFILENAME };
	CString	m_Filename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRequestDesignFilename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  BOOL m_AllowExisting;
	// Generated message map functions
	//{{AFX_MSG(CRequestDesignFilename)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CRequestLevelFilename dialog

class CRequestLevelFilename : public CDialog
{
// Construction
public:
	CRequestLevelFilename(CString file, BOOL AllowExisting=TRUE, CWnd* pParent=NULL);   // standard constructor
  CString m_file;

// Dialog Data
	//{{AFX_DATA(CRequestLevelFilename)
	enum { IDD = IDD_GETFILENAME };
	CString	m_Filename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRequestLevelFilename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  BOOL m_AllowExisting;
	// Generated message map functions
	//{{AFX_MSG(CRequestLevelFilename)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
