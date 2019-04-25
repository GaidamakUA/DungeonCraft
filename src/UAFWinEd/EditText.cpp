/******************************************************************************
* Filename: EditText.cpp
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

#include "UAFWinEd.h"
#include "Char.h"
#include "..\Shared\GPDLOpCodes.h"
#include "..\Shared\GPDLcomp.h"
#include "..\Shared\GPDLexec.h"
#include "EditText.h"
#include "Help.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CFont Font;

BOOL CEditText::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYDOWN && ::GetKeyState(VK_CONTROL) < 0)
  {
    switch (pMsg->wParam)
    {
    case 'C':
      m_textEditor.Copy();
      return TRUE;
    case 'A':
      m_textEditor.SetSel(0, -1);
      return TRUE;
    };
  };
  return CDialog::PreTranslateMessage(pMsg);

};


static CString CompileScript(const char *pText, const char *pFuncName, DWORD availableContexts)
{
  // don't need Actor context when compiling
  GPDLCOMP gpdlComp;
  const char *allowedEntry[2];
  allowedEntry[0] = pFuncName;
  allowedEntry[1] = NULL;
  CString binaryCode;

  CString script = pText;

  gpdlComp.m_availableContexts = availableContexts;
  binaryCode = gpdlComp.CompileScript(script, allowedEntry);
  return binaryCode;
}

static BOOL CompileAndExecuteScript(const char *pText, 
                                    const char *pFuncName, 
                                    CString *msg, 
                                    DWORD availableContexts)
{  
  CString binary = CompileScript(pText,pFuncName, availableContexts);
  if (binary[0] == 0)
  {
    GPDL tmpgpdl;
    if (msg != NULL)
    {
      tmpgpdl.interpretError = " ";  // Force interpret errors back here!
    };
    // might need Actor context when executing a script  
    CString answer = tmpgpdl.ExecuteScript(binary,1); 
    if (msg != NULL)
    {
      if (tmpgpdl.interpretError != " ")
      {
        *msg = tmpgpdl.interpretError;
        return FALSE;
      };
    };
    if (answer.IsEmpty())
    {
      if (msg == NULL)
      {
        MsgBoxInfo("This script is valid","Script Valid");
      }
      else
      {
        *msg = "";
        return FALSE;
      };
    }
    else
    {
      if (msg == NULL)
      {
        MsgBoxInfo(answer,"Script Valid - Results Below");
      }
      else
      {
        *msg = "";
        return TRUE;
      };
    };
    return TRUE;
  }
  else
  {
    if (msg == NULL)
    {
      binary.Replace("Called from", "\nCalled from");
      MsgBoxError(binary,"This Script Has Errors");
      return FALSE;
    }
    else
    {
      *msg = binary;
      return FALSE;
    };
  }
}


BOOL IsSyntaxAndSemanticsValid(const char *pText, const char *pFuncName, CString *pMsg, DWORD availableContext)
{
  CHARACTER tmpDude;
  ActorType actor;
  HOOK_PARAMETERS hookParameters;
  tmpDude.SetAlignment(ChaoticNeutral);
  //tmpDude.race(Human);
  tmpDude.RaceID(raceData.PeekRace(0)->RaceID());
  tmpDude.SetGender(Male);
  //tmpDude.SetClass(Fighter);
  tmpDude.SetClass(classData.PeekClass(0)->ClassID());
  tmpDude.generateNewCharacter(0,0);
  tmpDude.SetName("TmpScriptCheck");
  tmpDude.SetType(CHAR_TYPE);
  tmpDude.SetPartyMember();

  if (tmpDude.GetMaxHitPoints() == 0)
  {
    MsgBoxError("Failed to create character (used for testing script)\nRace/Class configuration files may have an error");
    return FALSE;
  }
  //tmpDude.GetContext(&actor, FighterFlag);
  tmpDude.GetContext(&actor);
  SetCharContext(&actor);
  SetTargetContext(&actor);
#ifdef UAFEngine
  xxxx  PRS 20120621  The Engine does not use this!!!!
  scriptContext.SetTargetContext(&tmpDude);
  scriptContext.SetCharacterContext(&tmpDude);
#endif
  BOOL result = FALSE;
  if (pText[0] == '$')
    result = CompileAndExecuteScript(pText, pFuncName, pMsg, availableContext);

  ClearTargetContext();
  ClearCharContext();
#ifdef UAFEngine
  xxxx  PRS 20120621  The Engine does not use this!!!!
  scriptContext.Clear();
#endif
  return result;
}

BOOL IsSyntaxValid(const char *pText, const char *pFuncName)
{
  CString abinaryCode = CompileScript(pText, pFuncName, 0xffffffff);
  return (abinaryCode[0]==0);
}

/////////////////////////////////////////////////////////////////////////////
// CEditText dialog


CEditText::CEditText(const CString &pre, 
                     const CString &post, 
                     const CString &func, 
                     bool  isScript,
                     ScriptList& list, CWnd* pParent /*=NULL*/)
	: CDialog(CEditText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditText)
	m_Text = _T("");
	m_ScriptName1 = _T("Activation Script");
	//}}AFX_DATA_INIT
  m_pre=pre;
  m_post=post;
  m_isScript = isScript;
  m_func=func;
  
  m_func.Remove(' '); // but remove spaces from name
  m_func.Replace('|', '_');
  
  m_list.RemoveAll();
  for (int i=0;i<list.GetSize();i++)
    m_list.Add( list[i] );
  availableContexts = 0xffffffff;
}

CEditText::~CEditText()
{
  Font.DeleteObject();
}

void CEditText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditText)
	DDX_Control(pDX, IDC_SCRIPTCOMBO, m_ScriptCombo);
	DDX_Control(pDX, IDC_TEXTEDITOR, m_textEditor);
	DDX_Control(pDX, IDC_SCRIPTFUNCTIONLIST, m_ScriptFunctionList);
	DDX_Text(pDX, IDC_TEXTEDITOR, m_Text);
	DDX_Text(pDX, IDC_SCRIPTNAME1, m_ScriptName1);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditText, CDialog)
	//{{AFX_MSG_MAP(CEditText)
	ON_BN_CLICKED(IDC_COMPILE, OnCompile)
	ON_BN_CLICKED(IDC_Help, OnHelp)
	ON_LBN_SELCHANGE(IDC_SCRIPTFUNCTIONLIST, OnSelchangeScriptfunctionlist)
	ON_LBN_DBLCLK(IDC_SCRIPTFUNCTIONLIST, OnDblclkScriptfunctionlist)
	ON_CBN_SELCHANGE(IDC_SCRIPTCOMBO, OnSelchangeScriptcombo)
	ON_CBN_DROPDOWN(IDC_SCRIPTCOMBO, OnDropdownScriptcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditText message handlers

BOOL CEditText::OnInitDialog() 
{
	CDialog::OnInitDialog();
  
  Font.CreatePointFont(100,"Courier",NULL);
  CWnd *pWnd = GetDlgItem(IDC_TEXTEDITOR);
  if (pWnd!=NULL)
    pWnd->SetFont(&Font);
  
  pWnd = GetDlgItem(IDC_TEXTEDITOR2);
  if (pWnd!=NULL)
    pWnd->SetFont(&Font);

  GPDLCOMP gpdlCompTemp;
  TEXTLIST list;
  switch (m_ScriptFilter)
  {
  case SPECIALABILITIES_USAGE:gpdlCompTemp.GetSpecialAbilityFunctionNames(list); break;
  case SPELL_USAGE:           gpdlCompTemp.GetSpellFunctionNames(list); break;
  case LOGICBLK_USAGE:        gpdlCompTemp.GetLogicBlockFunctionNames(list); break;
  case EVENT_USAGE:           gpdlCompTemp.GetEventFunctionNames(list); break;
  }
  
  POSITION pos = list.GetHeadPosition();  
  while (pos != NULL)
    m_ScriptFunctionList.AddString(list.GetNext(pos));
  
  
  // Find the longest string in the list box.
  CString      str;
  CSize      sz;
  int      dx = 0;
  TEXTMETRIC   tm;
  CDC*      pDC = m_ScriptFunctionList.GetDC();
  CFont*      pFont = m_ScriptFunctionList.GetFont();
  
  // Select the listbox font, save the old font
  CFont* pOldFont = pDC->SelectObject(pFont);
  // Get the text metrics for avg char width
  pDC->GetTextMetrics(&tm); 
  
  int i;
  for (i = 0; i < m_ScriptFunctionList.GetCount(); i++)
  {
    m_ScriptFunctionList.GetText(i, str);
    sz = pDC->GetTextExtent(str);
    
    // Add the avg width to prevent clipping
    sz.cx += tm.tmAveCharWidth;
    
    if (sz.cx > dx)
      dx = sz.cx;
  }
  // Select the old font back into the DC
  pDC->SelectObject(pOldFont);
  m_ScriptFunctionList.ReleaseDC(pDC);
  
  // Set the horizontal extent so every character of all strings 
  // can be scrolled to.
  m_ScriptFunctionList.SetHorizontalExtent(dx);


  m_ScriptCombo.ResetContent();
  for (i=0;i<m_list.GetSize();i++)
  {
    int index = m_ScriptCombo.AddString( m_list[i].name );
    m_ScriptCombo.SetItemData( index, i );
  }

  m_ScriptCombo.SetCurSel(0);
  if (m_list.GetSize() > 0)
  {
    m_Text = m_list[0].script;
    m_ScriptName1 = m_list[0].name;
  };


  GotoDlgCtrl(GetDlgItem(IDC_SCRIPTCOMBO));
  UpdateData(FALSE);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditText::OnCompile() 
{
  UpdateData(TRUE);
  CString script = m_pre + m_Text + m_post;
	IsSyntaxAndSemanticsValid(script, m_func, NULL, availableContexts);
}

void CEditText::OnOK() 
{
  // copy text from edit window to script before exiting
  UpdateData(TRUE);
	int index = m_ScriptCombo.GetCurSel();
  if (index < 0) return;
  int pos = m_ScriptCombo.GetItemData( index );
  m_list[pos].script = m_Text;

  if (m_isScript)
  {
    for (int i=0;i<m_list.GetSize();i++)
    {
      if (!m_list[i].script.IsEmpty())
      {
        // don't execute when exiting dialog, compile only
        CString script = m_pre + m_list[i].script + m_post;
        if (!IsSyntaxValid(script, m_func))
        {
          CString msg;
          msg.Format("Syntax for %s is not valid, Ignore?", m_list[i].name);
          if (MsgBoxYesNo(msg, "Ignore Syntax Error?")!=IDYES)
          {
            m_ScriptCombo.SetCurSel(i);
            OnSelchangeScriptcombo();
            return;
          }
        }
      }
    }
  };

	CDialog::OnOK();
}

CString ExtractFunction(CString& s)
{
  // The code that built these lines could very well have put them
  // directly into the ListBox along with a data word indicating
  // what kind of function each line represents.  That would save
  // us the work of parsing.  For now, I will try to parse the 
  // lines.  We will probably have to fix this later as different
  // kinds of functions are added.
  int start, op, cp;
  op = s.Find('(');
  if (op >= 0)
  {
    CString result;
    start = s.Find(' ') + 1;
    if (start > op) start = op;
    cp = s.Find(')');
    if (cp < op) cp = s.GetLength()-1;
    result = s.Mid(start,cp);
    return result;
  };
  return s;
}

void CEditText::OnSelchangeScriptfunctionlist() 
{
	// TODO: Add your control notification handler code here
	int i = m_ScriptFunctionList.GetCurSel();
  if (i < 0) return;
  if ( !OpenClipboard() )
  {
    AfxMessageBox( "Cannot open the Clipboard" );
    return;
  }
  // Remove the current Clipboard contents
  if( !EmptyClipboard() )
  {
    AfxMessageBox( "Cannot empty the Clipboard" );
    return;
  }
  // ...
  // Get the currently selected data
  // ...
  // For the appropriate data formats...
  CString r;
  char *str;
  m_ScriptFunctionList.GetText(i, r);
  r = ExtractFunction(r);
  HGLOBAL hglb =  GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,r.GetLength()+1);
  str = (char *)GlobalLock(hglb);
  strcpy(str,(LPCSTR)r);
  GlobalUnlock(hglb);
  if ( ::SetClipboardData( CF_TEXT, hglb ) == NULL )
  {
    AfxMessageBox( "Unable to set Clipboard data" );
  }
  // ...
  CloseClipboard();
	
}

void CEditText::OnDblclkScriptfunctionlist() 
{
  m_textEditor.Paste();
}


void CEditText::OnSelchangeScriptcombo() 
{
  // copy script to edit window
	int index = m_ScriptCombo.GetCurSel();
  if (index < 0) return;
  int pos = m_ScriptCombo.GetItemData( index );
  m_Text = m_list[pos].script;
  m_ScriptName1 = m_list[pos].name;
  UpdateData(FALSE);	
}

void CEditText::OnDropdownScriptcombo() 
{
  // copy text from edit window to script before selection changes
  UpdateData(TRUE);
	int index = m_ScriptCombo.GetCurSel();
  if (index < 0) return;
  int pos = m_ScriptCombo.GetItemData( index );
  m_list[pos].script = m_Text;
}

afx_msg void CEditText::OnHelp()
{
  CHelp dlg(m_helpKey);
  dlg.DoModal();
}
