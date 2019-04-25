/******************************************************************************
* Filename: LogicBlock.cpp
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

#include "..\Shared\stdafx.h"
#include "resource.h"

#include "class.h"
#include "LogicBlock.h"
#include "AttributeSelect.h"
#include "GPDLOpCodes.h"
#include "GPDLcomp.h"
#include "EditText.h"
#include "Help.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct {
  char *text;
  LOGIC_BLOCK_GATE_TYPE type;} logicGateText[] = {
    {"Copy from Top",    LBGT_top},
    {"Copy from Side",   LBGT_side},
    {"Logical AND",      LBGT_land},
    {"Logical OR",       LBGT_lor},
    {"Numeric Plus",     LBGT_aplus},
    {"Numeric Minus",    LBGT_aminus},
    {"Numeric Multiply", LBGT_agear},
    {"Numeric Divide",   LBGT_aslash},
    {"Numeric Modulo",   LBGT_amod},
    {"Numeric Greater",  LBGT_agreater},
    {"GREP",             LBGT_grep},
    {"Force True",       LBGT_true},
    {"Force False",      LBGT_false},
    {"String Equal",     LBGT_stringEqual},
    {"What Do You Need?",LBGT_NotImplemented}
};

int GateIndex(char type)
{
  int i;
  for (i=0; i<sizeof logicGateText/sizeof logicGateText[0]; i++)
    if (type == logicGateText[i].type) return i;
  return i-1;
}

struct {
  char *text;
  LOGIC_BLOCK_INPUT_TYPE type;} logicInputText[] = {
    {"Literal",            LBIT_literal},
    {"Global ASL",         LBIT_globalASL},
    {"Party Size",         LBIT_partySize},
    {"Character Info",     LBIT_charInfo},
    {"Direction Facing",   LBIT_dirFacing},
    {"Level ASL",          LBIT_levelASL},
    {"Quest Stage",        LBIT_questStage},
    {"Item List",          LBIT_itemList},
    {"NPC List",           LBIT_NPCList},
    {"Run Time Vars",      LBIT_RunTimeIf},
    {"Character ASL",      LBIT_charASL},
    {"Party ASL",          LBIT_partyASL},
    {"Wiggle (Grep field)",LBIT_Wiggle},
    {"GPDL Function",      LBIT_SourceGPDL},
    {"Temporary ASL",      LBIT_tempASL},
    {"What Do You Need?",  LBIT_NotImplemented}
};


struct {
  char *text;
  LOGIC_BLOCK_ACTION_TYPE type;} logicActionText[] = {
    {"Do Nothing",             LBAT_nothing},
    {"Set Global ASL",         LBAT_setGlobalASL},
    {"Set Level ASL",          LBAT_setLevelASL},
    {"Remove Global ASL",      LBAT_removeGlobalASL},
    {"Remove Level ASL",       LBAT_removeLevelASL},
    {"Set Quest Stage",        LBAT_setQuestStage},
    {"Set Temporary ASL",      LBAT_tempASL},
    {"Set Icon Index By Name", LBAT_setIconIndexByName},
    {"Set Character ASL",      LBAT_setCharASL},
    {"Set Party ASL",          LBAT_setPartyASL},
    {"Remove Party ASL",       LBAT_removePartyASL},
    {"GPDL Function",          LBAT_SourceGPDL},
    {"What Do You Need?",      LBAT_NotImplemented}
};

/////////////////////////////////////////////////////////////////////////////
// CLogicBlock dialog


CLogicBlock::CLogicBlock(LOGIC_BLOCK_DATA& data, CWnd* pParent /*=NULL*/)
	: CDialog(CLogicBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogicBlock)
	m_InputParamA = _T("");
	m_InputParamB = _T("");
	m_InputParamD = _T("");
	m_InputParamF = _T("");
	m_InputParamG = _T("");
	m_NotC = FALSE;
	m_NotE = FALSE;
	m_NotH = FALSE;
	m_NotI = FALSE;
	m_NotJ = FALSE;
	m_NotK = FALSE;
	m_ChainIfFalse = FALSE;
	m_ChainIfTrue = FALSE;
	m_NoChain = -1;
	m_Debug = FALSE;
	m_ActionParam1 = _T("");
	m_ActionParam2 = _T("");
	m_IfTrue1 = -1;
	m_IfTrue2 = -1;
	m_Record = FALSE;
	//}}AFX_DATA_INIT
  m_data = data;
}


void CLogicBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogicBlock)
	DDX_Control(pDX, IDC_PARAMGSELECT, m_ParmGSelect);
	DDX_Control(pDX, IDC_PARAMFSELECT, m_ParmFSelect);
	DDX_Control(pDX, IDC_PARAMDSELECT, m_ParmDSelect);
	DDX_Control(pDX, IDC_PARAMBSELECT, m_ParmBSelect);
	DDX_Control(pDX, IDC_PARAMASELECT, m_ParmASelect);
	DDX_Control(pDX, IDC_InputTypeA, m_InputTypeA);
	DDX_Control(pDX, IDC_InputTypeB, m_InputTypeB);
	DDX_Control(pDX, IDC_InputTypeD, m_InputTypeD);
	DDX_Control(pDX, IDC_InputTypeF, m_InputTypeF);
	DDX_Control(pDX, IDC_InputTypeG, m_InputTypeG);
	DDX_Text(pDX, IDC_InputParamA, m_InputParamA);
	DDX_Text(pDX, IDC_InputParamB, m_InputParamB);
	DDX_Text(pDX, IDC_InputParamD, m_InputParamD);
	DDX_Text(pDX, IDC_InputParamF, m_InputParamF);
	DDX_Text(pDX, IDC_InputParamG, m_InputParamG);
	DDX_Control(pDX, IDC_GateTypeC, m_GateTypeC);
	DDX_Control(pDX, IDC_GateTypeE, m_GateTypeE);
	DDX_Control(pDX, IDC_GateTypeH, m_GateTypeH);
	DDX_Control(pDX, IDC_GateTypeI, m_GateTypeI);
	DDX_Control(pDX, IDC_GateTypeJ, m_GateTypeJ);
	DDX_Control(pDX, IDC_GateTypeK, m_GateTypeK);
	DDX_Control(pDX, IDC_GateTypeL, m_GateTypeL);
	DDX_Check(pDX, IDC_NotC, m_NotC);
	DDX_Check(pDX, IDC_NotE, m_NotE);
	DDX_Check(pDX, IDC_NotH, m_NotH);
	DDX_Check(pDX, IDC_NotI, m_NotI);
	DDX_Check(pDX, IDC_NotJ, m_NotJ);
	DDX_Check(pDX, IDC_NotK, m_NotK);
	DDX_Control(pDX, IDC_ChainIfTrue, m_ChainIfTrueControl);
	DDX_Control(pDX, IDC_ChainIfFalse, m_ChainIfFalseControl);
	DDX_Control(pDX, IDC_ActionType2, m_ActionType2);
	DDX_Control(pDX, IDC_ActionType1, m_ActionType1);
	DDX_Check(pDX, IDC_ChainIfFalse, m_ChainIfFalse);
	DDX_Check(pDX, IDC_ChainIfTrue, m_ChainIfTrue);
	DDX_Radio(pDX, IDC_NoChain, m_NoChain);
	DDX_Check(pDX, IDC_Debug, m_Debug);
	DDX_Text(pDX, IDC_ActionParam1, m_ActionParam1);
	DDX_Text(pDX, IDC_ActionParam2, m_ActionParam2);
	DDX_Radio(pDX, IDC_IfTrue_1, m_IfTrue1);
	DDX_Radio(pDX, IDC_IfTrue_2, m_IfTrue2);
	DDX_Check(pDX, IDC_Record, m_Record);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogicBlock, CDialog)
	//{{AFX_MSG_MAP(CLogicBlock)
	ON_BN_CLICKED(IDC_NoChain, OnNoChain)
	ON_CBN_SELCHANGE(IDC_InputTypeA, OnSelchangeInputTypeA)
	ON_CBN_SELCHANGE(IDC_InputTypeB, OnSelchangeInputTypeB)
	ON_CBN_SELCHANGE(IDC_InputTypeD, OnSelchangeInputTypeD)
	ON_CBN_SELCHANGE(IDC_InputTypeF, OnSelchangeInputTypeF)
	ON_CBN_SELCHANGE(IDC_InputTypeG, OnSelchangeInputTypeG)
	ON_BN_CLICKED(IDC_PARAMASELECT, OnParamaselect)
	ON_BN_CLICKED(IDC_PARAMBSELECT, OnParambselect)
	ON_BN_CLICKED(IDC_PARAMDSELECT, OnParamdselect)
	ON_BN_CLICKED(IDC_PARAMFSELECT, OnParamfselect)
	ON_BN_CLICKED(IDC_PARAMGSELECT, OnParamgselect)
	ON_BN_CLICKED(IDC_AlwaysChain, OnNoChain)
	ON_BN_CLICKED(IDC_ConditionallyChain, OnNoChain)
	ON_BN_CLICKED(IDC_ACTION1SELECT2, OnAction1select2)
	ON_BN_CLICKED(IDC_ACTION2SELECT2, OnAction2select2)
  ON_BN_CLICKED(IDC_LOGIC_BLOCK, OnLogicBlockQuestion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogicBlock message handlers

void CLogicBlock::InitializeInput(CComboBox& inputType)
{
  int i, index;
  inputType.ResetContent();
  for (i=0; i<sizeof logicInputText/sizeof logicInputText[0]; i++)
  {
    index=inputType.AddString(logicInputText[i].text);
    inputType.SetItemData(index,logicInputText[i].type);
  };
}

BYTE CLogicBlock::GetInputType(CComboBox& inputType, 
                               char blockName,
                               const CString& parameter)
{
  int index;
  BYTE result;
  index=inputType.GetCurSel();
  result = inputType.GetItemData(index);
  if (result != LBIT_NotImplemented)
  {
    if (result == LBIT_SourceGPDL)
    {
      const char *entries[2] = {"input",NULL};
      CString sourcePgm, binaryProgram;
      GPDLCOMP gpdlComp;
      sourcePgm = "$PUBLIC $FUNC input(A,B,C,D,E,F){"
                  + parameter
                  + "}input;";
      binaryProgram = gpdlComp.CompileScript(sourcePgm,entries);
      if (binaryProgram[0] == 0) return result;
      {
        CString errMsg;
        errMsg.Format("Error compiling Input Block %c\n%s",blockName,binaryProgram);
        MessageBox(errMsg);
      };
      return result;
    };
    return result;
  };
  CString txt, msg;
  inputType.GetLBText(index, txt);
  msg.Format("Sorry\n%s\nis not implemented",txt);
  MessageBox(msg);
  return LBIT_literal;
}

void CLogicBlock::InitializeGate(CComboBox& gate)
{
  int i, index;
  gate.ResetContent();
  for (i=0; i<sizeof logicGateText/sizeof logicGateText[0]; i++)
  {
    index = gate.AddString(logicGateText[i].text);
    gate.SetItemData(index, logicGateText[i].type);
  };
}

BYTE CLogicBlock::GetGateType(CComboBox& gate)
{
  int index;
  BYTE result;
  index=gate.GetCurSel();
  result = gate.GetItemData(index);
  if (result != LBGT_NotImplemented) return result;
  CString txt, msg;
  gate.GetLBText(index, txt);
  msg.Format("Sorry\n%s\nis not implemented",txt);
  MessageBox(msg);
  return LBGT_top;
}


void CLogicBlock::InitializeAction(CComboBox& action)
{
  int i, index;
  action.ResetContent();
  for (i=0; i<sizeof logicActionText/sizeof logicActionText[0]; i++)
  {
    index = action.AddString(logicActionText[i].text);
    action.SetItemData(index, logicActionText[i].type);
  };
}

BYTE CLogicBlock::GetActionType(CComboBox& action)
{
  int index;
  BYTE result;
  index=action.GetCurSel();
  result = action.GetItemData(index);
  if (result != LBAT_NotImplemented) return result;
  CString txt, msg;
  action.GetLBText(index, txt);
  msg.Format("Sorry\n%s\nis not implemented",txt);
  MessageBox(msg);
  return LBAT_nothing;
}


BOOL CLogicBlock::OnInitDialog() 
{
	CDialog::OnInitDialog();
  InitializeInput(m_InputTypeA);
  InitializeInput(m_InputTypeB);
  InitializeInput(m_InputTypeD);
  InitializeInput(m_InputTypeF);
  InitializeInput(m_InputTypeG);
  InitializeGate(m_GateTypeC);
  InitializeGate(m_GateTypeE);
  InitializeGate(m_GateTypeH);
  InitializeGate(m_GateTypeI);
  InitializeGate(m_GateTypeJ);
  InitializeGate(m_GateTypeK);
  InitializeGate(m_GateTypeL);
  InitializeAction(m_ActionType1);
  InitializeAction(m_ActionType2);
	m_InputParamA=m_data.m_InputParamA;
	m_InputParamB=m_data.m_InputParamB;
	m_InputParamD=m_data.m_InputParamD;
	m_InputParamF=m_data.m_InputParamF;
	m_InputParamG=m_data.m_InputParamG;
	m_ActionParam1=m_data.m_ActionParam1;
	m_ActionParam2=m_data.m_ActionParam2;
	m_GateTypeC.SetCurSel(GateIndex(m_data.m_GateTypeC));
	m_GateTypeE.SetCurSel(GateIndex(m_data.m_GateTypeE));
	m_GateTypeH.SetCurSel(GateIndex(m_data.m_GateTypeH));
	m_GateTypeI.SetCurSel(GateIndex(m_data.m_GateTypeI));
	m_GateTypeJ.SetCurSel(GateIndex(m_data.m_GateTypeJ));
	m_GateTypeK.SetCurSel(GateIndex(m_data.m_GateTypeK));
	m_GateTypeL.SetCurSel(GateIndex(m_data.m_GateTypeL));
	m_InputTypeA.SetCurSel(m_data.m_InputTypeA);
	m_InputTypeB.SetCurSel(m_data.m_InputTypeB);
	m_InputTypeD.SetCurSel(m_data.m_InputTypeD);
	m_InputTypeF.SetCurSel(m_data.m_InputTypeF);
	m_InputTypeG.SetCurSel(m_data.m_InputTypeG);
  UpdateInputParamControls(m_InputTypeA, IDC_PARAMASELECT);
	UpdateInputParamControls(m_InputTypeB, IDC_PARAMBSELECT);	
	UpdateInputParamControls(m_InputTypeD, IDC_PARAMDSELECT);	
	UpdateInputParamControls(m_InputTypeF, IDC_PARAMFSELECT);	
	UpdateInputParamControls(m_InputTypeG, IDC_PARAMGSELECT);	
	m_ActionType1.SetCurSel(m_data.m_ActionType1);
	m_ActionType2.SetCurSel(m_data.m_ActionType2);
	m_ChainIfFalse=m_data.m_ChainIfFalse;
	m_ChainIfTrue=m_data.m_ChainIfTrue;
	m_NoChain=m_data.m_NoChain;
	m_NotC=m_data.m_NotC;
	m_NotE=m_data.m_NotE;
	m_NotH=m_data.m_NotH;
	m_NotI=m_data.m_NotI;
	m_NotJ=m_data.m_NotJ;
	m_NotK=m_data.m_NotK;
  m_IfTrue1=m_data.m_IfTrue1;
  m_IfTrue2=m_data.m_IfTrue2;
  m_Debug=(m_data.m_Flags & LBF_RUNTIME_DEBUG)!=0;
  m_Record=(m_data.m_Flags & LBF_RECORD_VALUES)!=0;
  SetControlStates();
  UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogicBlock::OnOK() 
{
	// TODO: Add extra validation here
  UpdateData(true);
	m_data.m_InputParamA=m_InputParamA;
	m_data.m_InputParamB=m_InputParamB;
	m_data.m_InputParamD=m_InputParamD;
	m_data.m_InputParamF=m_InputParamF;
	m_data.m_InputParamG=m_InputParamG;
	m_data.m_ActionParam1=m_ActionParam1;
	m_data.m_ActionParam2=m_ActionParam2;
	m_data.m_GateTypeC=GetGateType(m_GateTypeC);
	m_data.m_GateTypeE=GetGateType(m_GateTypeE);
	m_data.m_GateTypeH=GetGateType(m_GateTypeH);
	m_data.m_GateTypeI=GetGateType(m_GateTypeI);
	m_data.m_GateTypeJ=GetGateType(m_GateTypeJ);
	m_data.m_GateTypeK=GetGateType(m_GateTypeK);
	m_data.m_GateTypeL=GetGateType(m_GateTypeL);
	m_data.m_InputTypeA=GetInputType(m_InputTypeA,'A',m_InputParamA);
	m_data.m_InputTypeB=GetInputType(m_InputTypeB,'B',m_InputParamB);
	m_data.m_InputTypeD=GetInputType(m_InputTypeD,'D',m_InputParamD);
	m_data.m_InputTypeF=GetInputType(m_InputTypeF,'F',m_InputParamF);
	m_data.m_InputTypeG=GetInputType(m_InputTypeG,'G',m_InputParamG);
	m_data.m_ActionType1=GetActionType(m_ActionType1);
	m_data.m_ActionType2=GetActionType(m_ActionType2);
	m_data.m_ChainIfFalse=m_ChainIfFalse;
	m_data.m_ChainIfTrue=m_ChainIfTrue;
	m_data.m_NoChain=m_NoChain;
	m_data.m_NotC=m_NotC;
	m_data.m_NotE=m_NotE;
	m_data.m_NotH=m_NotH;
	m_data.m_NotI=m_NotI;
	m_data.m_NotJ=m_NotJ;
	m_data.m_NotK=m_NotK;
  m_data.m_IfTrue1=m_IfTrue1;
  m_data.m_IfTrue2=m_IfTrue2;
  m_data.m_Flags = 0;
  m_data.m_Flags|=m_Debug?LBF_RUNTIME_DEBUG:0;
  m_data.m_Flags|=m_Record?LBF_RECORD_VALUES:0;

	CDialog::OnOK();
}

void CLogicBlock::SetControlStates(void)
{
  switch (m_NoChain)
  {
  case 0:  // No chain
  case 1:  // Chain Always
    m_ChainIfFalseControl.EnableWindow(false);
    m_ChainIfTrueControl.EnableWindow(false);
    break;
  case 2:  // Chain conditionally
    m_ChainIfFalseControl.EnableWindow(true);
    m_ChainIfTrueControl.EnableWindow(true);
    break;
  };
}

void CLogicBlock::OnNoChain() 
{
	UpdateData(true);
  SetControlStates();
}

void CLogicBlock::UpdateInputParamControls(CComboBox& inputType, int controlId)
{
  CWnd *pWnd = GetDlgItem(controlId);
  if (pWnd == NULL) return;
  switch (inputType.GetItemData(inputType.GetCurSel()))
  {
  case LBIT_RunTimeIf:
  case LBIT_SourceGPDL:
    pWnd->EnableWindow(TRUE);
    pWnd->ShowWindow(SW_SHOW);
    break;
  default:
    pWnd->EnableWindow(FALSE);
    pWnd->ShowWindow(SW_HIDE);
    break;
  }
}

void CLogicBlock::OnSelchangeInputTypeA() 
{
  UpdateInputParamControls(m_InputTypeA, IDC_PARAMASELECT);
}

void CLogicBlock::OnSelchangeInputTypeB() 
{
	UpdateInputParamControls(m_InputTypeB, IDC_PARAMBSELECT);	
}

void CLogicBlock::OnSelchangeInputTypeD() 
{
	UpdateInputParamControls(m_InputTypeD, IDC_PARAMDSELECT);	
}

void CLogicBlock::OnSelchangeInputTypeF() 
{
	UpdateInputParamControls(m_InputTypeF, IDC_PARAMFSELECT);	
}

void CLogicBlock::OnSelchangeInputTypeG() 
{
	UpdateInputParamControls(m_InputTypeG, IDC_PARAMGSELECT);	
}

void CLogicBlock::OnParamaselect() 
{
  UpdateData(TRUE);
  if (m_InputTypeA.GetItemData(m_InputTypeA.GetCurSel())==LBIT_SourceGPDL)
  {
    CString start("$PUBLIC $FUNC logicblocktest() { ");
    CString end(" } logicblocktest;");
    CString func = "logicblocktest";

    ScriptList list;
    list.SetSize(1);
    list[0].name = "Logic Block Input A Script";
    list[0].script = m_InputParamA;

    CEditText dlg(start, end, func, true, list);
    dlg.m_ScriptFilter = LOGICBLK_USAGE;

    if (dlg.DoModal()==IDOK)
      m_InputParamA = dlg.m_list[0].script;
  }
  else
  {
	  CAttributeSelect dlg;
    dlg.DoModal();
    m_InputParamA = dlg.attribute;
  }
  UpdateData(FALSE);
}

void CLogicBlock::OnParambselect() 
{
  UpdateData(TRUE);
  if (m_InputTypeB.GetItemData(m_InputTypeB.GetCurSel())==LBIT_SourceGPDL)
  {
    CString start("$PUBLIC $FUNC logicblocktest() { ");
    CString end(" } logicblocktest;");
    CString func = "logicblocktest";

    ScriptList list;
    list.SetSize(1);
    list[0].name = "Logic Block Input B Script";
    list[0].script = m_InputParamB;

    CEditText dlg(start, end, func, true, list);
    dlg.m_ScriptFilter = LOGICBLK_USAGE;

    if (dlg.DoModal()==IDOK)
      m_InputParamB = dlg.m_list[0].script;
  }
  else
  {
	  CAttributeSelect dlg;
    dlg.DoModal();
    m_InputParamB = dlg.attribute;
  }
  UpdateData(FALSE);
}

void CLogicBlock::OnParamdselect() 
{
  UpdateData(TRUE);
  if (m_InputTypeD.GetItemData(m_InputTypeD.GetCurSel())==LBIT_SourceGPDL)
  {
    CString start("$PUBLIC $FUNC logicblocktest() { ");
    CString end(" } logicblocktest;");
    CString func = "logicblocktest";

    ScriptList list;
    list.SetSize(1);
    list[0].name = "Logic Block Input D Script";
    list[0].script = m_InputParamD;

    CEditText dlg(start, end, func, true, list);
    dlg.m_ScriptFilter = LOGICBLK_USAGE;

    if (dlg.DoModal()==IDOK)
      m_InputParamD = dlg.m_list[0].script;
  }
  else
  {
	  CAttributeSelect dlg;
    dlg.DoModal();
    m_InputParamD = dlg.attribute;
  }
  
  UpdateData(FALSE);
}

void CLogicBlock::OnParamfselect() 
{
  UpdateData(TRUE);
  if (m_InputTypeF.GetItemData(m_InputTypeF.GetCurSel())==LBIT_SourceGPDL)
  {
    CString start("$PUBLIC $FUNC logicblocktest() { ");
    CString end(" } logicblocktest;");
    CString func = "logicblocktest";

    ScriptList list;
    list.SetSize(1);
    list[0].name = "Logic Block Input F Script";
    list[0].script = m_InputParamF;

    CEditText dlg(start, end, func, true, list);
    dlg.m_ScriptFilter = LOGICBLK_USAGE;

    if (dlg.DoModal()==IDOK)
      m_InputParamF = dlg.m_list[0].script;
  }
  else
  {
	  CAttributeSelect dlg;
    dlg.DoModal();
    m_InputParamF = dlg.attribute;
  }
  UpdateData(FALSE);
}

void CLogicBlock::OnParamgselect() 
{
  UpdateData(TRUE);
  if (m_InputTypeG.GetItemData(m_InputTypeG.GetCurSel())==LBIT_SourceGPDL)
  {
    CString start("$PUBLIC $FUNC logicblocktest() { ");
    CString end(" } logicblocktest;");
    CString func = "logicblocktest";

    ScriptList list;
    list.SetSize(1);
    list[0].name = "Logic Block Input G Script";
    list[0].script = m_InputParamG;

    CEditText dlg(start, end, func, true, list);
    dlg.m_ScriptFilter = LOGICBLK_USAGE;

    if (dlg.DoModal()==IDOK)
      m_InputParamG = dlg.m_Text;
  }
  else
  {
	  CAttributeSelect dlg;
    dlg.DoModal();
    m_InputParamG = dlg.attribute;
  }
  UpdateData(FALSE);
}

void CLogicBlock::OnAction1select2() 
{
	// TODO: Add your control notification handler code here
	
  UpdateData(TRUE);
  if (m_ActionType1.GetItemData(m_ActionType1.GetCurSel())==LBAT_SourceGPDL)
  {
    CString start("$PUBLIC $FUNC logicblocktest() { ");
    CString end(" } logicblocktest;");
    CString func = "logicblocktest";

    ScriptList list;
    list.SetSize(1);
    list[0].name = "Logic Block Action 1 Script";
    list[0].script = m_ActionParam1;

    CEditText dlg(start, end, func, true, list);
    dlg.m_ScriptFilter = LOGICBLK_USAGE;

    if (dlg.DoModal()==IDOK)
      m_ActionParam1 = dlg.m_list[0].script;
  };
  UpdateData(FALSE);
}

void CLogicBlock::OnAction2select2() 
{
	// TODO: Add your control notification handler code here
	
  UpdateData(TRUE);
  if (m_ActionType2.GetItemData(m_ActionType2.GetCurSel())==LBAT_SourceGPDL)
  {
    CString start("$PUBLIC $FUNC logicblocktest() { ");
    CString end(" } logicblocktest;");
    CString func = "logicblocktest";

    ScriptList list;
    list.SetSize(1);
    list[0].name = "Logic Block Action 2 Script";
    list[0].script = m_ActionParam2;

    CEditText dlg(start, end, func, true, list);
    dlg.m_ScriptFilter = LOGICBLK_USAGE;

    if (dlg.DoModal()==IDOK)
      m_ActionParam2 = dlg.m_list[0].script;
  };
  UpdateData(FALSE);
}

void CLogicBlock::OnLogicBlockQuestion() 
{
  CHelp dlg("LOGIC_BLOCK");
  dlg.DoModal();
}
