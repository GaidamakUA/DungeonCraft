#if !defined(AFX_LOGICBLOCK_H__FF01C481_7B90_11D4_A03D_9E67440308EA__INCLUDED_)
#define AFX_LOGICBLOCK_H__FF01C481_7B90_11D4_A03D_9E67440308EA__INCLUDED_
/******************************************************************************
* Filename: LogicBlock.h
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogicBlock.h : header file
//
#include "gameevent.h"


/////////////////////////////////////////////////////////////////////////////
// CLogicBlock dialog

class CLogicBlock : public CDialog
{
// Construction
public:
	CLogicBlock(LOGIC_BLOCK_DATA& data, CWnd* pParent = NULL);   // standard constructor
  void GetData(LOGIC_BLOCK_DATA &data) { data = m_data; }
  void InitializeInput(CComboBox& inputType);
  void InitializeGate(CComboBox& gate);
  void InitializeAction(CComboBox& action);
  void SetControlStates(void);
  BYTE GetInputType(CComboBox& inputType, char blockName,const CString& oarameter);
  void UpdateInputParamControls(CComboBox& inputType, int controlId);
  BYTE GetGateType(CComboBox& gate);
  BYTE GetActionType(CComboBox& action);

// Dialog Data
	//{{AFX_DATA(CLogicBlock)
	enum { IDD = IDD_LOGICBLOCK };
	CButton	m_ParmGSelect;
	CButton	m_ParmFSelect;
	CButton	m_ParmDSelect;
	CButton	m_ParmBSelect;
	CButton	m_ParmASelect;
	CComboBox	m_InputTypeA;
	CComboBox	m_InputTypeB;
	CComboBox	m_InputTypeD;
	CComboBox	m_InputTypeF;
	CComboBox	m_InputTypeG;
	CString	m_InputParamA;
	CString	m_InputParamB;
	CString	m_InputParamD;
	CString	m_InputParamF;
	CString	m_InputParamG;
	CComboBox	m_GateTypeC;
	CComboBox	m_GateTypeE;
	CComboBox	m_GateTypeH;
	CComboBox	m_GateTypeI;
	CComboBox	m_GateTypeJ;
	CComboBox	m_GateTypeK;
	CComboBox	m_GateTypeL;
	BOOL	m_NotC;
	BOOL	m_NotE;
	BOOL	m_NotH;
	BOOL	m_NotI;
	BOOL	m_NotJ;
	BOOL	m_NotK;
	CButton	m_ChainIfTrueControl;
	CButton	m_ChainIfFalseControl;
	CComboBox	m_ActionType2;
	CComboBox	m_ActionType1;
	BOOL	m_ChainIfFalse;
	BOOL	m_ChainIfTrue;
	int		m_NoChain;
	BOOL	m_Debug;
	CString	m_ActionParam1;
	CString	m_ActionParam2;
	int		m_IfTrue1;
	int		m_IfTrue2;
	BOOL	m_Record;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogicBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  LOGIC_BLOCK_DATA m_data;
	// Generated message map functions
	//{{AFX_MSG(CLogicBlock)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNoChain();
	afx_msg void OnSelchangeInputTypeA();
	afx_msg void OnSelchangeInputTypeB();
	afx_msg void OnSelchangeInputTypeD();
	afx_msg void OnSelchangeInputTypeF();
	afx_msg void OnSelchangeInputTypeG();
	afx_msg void OnParamaselect();
	afx_msg void OnParambselect();
	afx_msg void OnParamdselect();
	afx_msg void OnParamfselect();
	afx_msg void OnParamgselect();
	afx_msg void OnAction1select2();
	afx_msg void OnAction2select2();
  afx_msg void OnLogicBlockQuestion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGICBLOCK_H__FF01C481_7B90_11D4_A03D_9E67440308EA__INCLUDED_)
