/******************************************************************************
* Filename: QButtonOptions.h
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
#if !defined(AFX_QBUTTONOPTIONS_H__15CB36C2_558F_11D3_BF9A_00A0CC25685D__INCLUDED_)
#define AFX_QBUTTONOPTIONS_H__15CB36C2_558F_11D3_BF9A_00A0CC25685D__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// CQButtonOptions dialog

class CQButtonOptions : public CDialog
{
// Construction
public:
	CQButtonOptions(QUESTION_BUTTON_DATA& event, CWnd* pParent = NULL);   // standard constructor
  void GetData(QUESTION_BUTTON_DATA& event);

// Dialog Data
	//{{AFX_DATA(CQButtonOptions)
	enum { IDD = IDD_QBUTTONS };
	CButton	m_SeeArt;
	CComboBox	m_AfterChain5;
	CComboBox	m_AfterChain4;
	CComboBox	m_AfterChain3;
	CComboBox	m_AfterChain2;
	CComboBox	m_AfterChain1;
	CString	m_Label1;
	CString	m_Label2;
	CString	m_Label3;
	CString	m_Label4;
	CString	m_Label5;
	BOOL	m_Present1;
	BOOL	m_Present2;
	BOOL	m_Present3;
	BOOL	m_Present4;
	BOOL	m_Present5;
	CString	m_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQButtonOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  QUESTION_BUTTON_DATA m_event;

	// Generated message map functions
	//{{AFX_MSG(CQButtonOptions)
	afx_msg void OnSeeart();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QBUTTONOPTIONS_H__15CB36C2_558F_11D3_BF9A_00A0CC25685D__INCLUDED_)
