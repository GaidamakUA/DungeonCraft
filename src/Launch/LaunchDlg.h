// LaunchDlg.h : header file
//

#if !defined(AFX_LAUNCHDLG_H__D992D7E8_98C6_48D6_9F3E_3D459CA5D716__INCLUDED_)
#define AFX_LAUNCHDLG_H__D992D7E8_98C6_48D6_9F3E_3D459CA5D716__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../UAFWinEd/SortableListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CLaunchDlg dialog

class CLaunchDlg : public CDialog
{
// Construction
public:
	CLaunchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLaunchDlg)
	enum { IDD = IDD_LAUNCH_DIALOG };
	CSortableListCtrl	m_List;
	CString	m_EnginePath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaunchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

  void PopulateDesignList();
  void AddDesign(const char *path);
  void GetDesignVersion(const char *fname, double &ver, CString &name);
  BOOL LaunchDesign(const char *path);

	// Generated message map functions
	//{{AFX_MSG(CLaunchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDblclkDesignlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRefresh();
	afx_msg void OnEngineselect();
	afx_msg void OnDesignselect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHDLG_H__D992D7E8_98C6_48D6_9F3E_3D459CA5D716__INCLUDED_)
