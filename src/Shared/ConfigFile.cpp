/******************************************************************************
* Filename: ConfigFile.cpp
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
//#include "..\Shared\Version.h"

//#include "afxtempl.h"
#include "externs.h"
#include "char.h"
#include "monster.h"
#include "class.h"
#include "ConfigFile.h"

void *My_malloc(int size);
void *My_realloc(void *buf, int size);
void My_free(void *addr);

extern CString emptyString;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

inline bool whitespace(const char c)
{
  return ((c==' ') || (c=='\t'));
}



CONFIG_FILE::LINE_ITEM::LINE_ITEM(void) 
{
  m_status=CONFIG_STAT_intError;
  m_toBeDeleted=false;
  m_linenumber=0;
}

CONFIG_FILE::LINE_ITEM::~LINE_ITEM(void) 
{
}

void CONFIG_FILE::LINE_ITEM::CreateComment(char *txt)
{
    m_line = txt;
    m_key = "";
    m_value = "";
    m_errorMsg = "";
    m_status = CONFIG_STAT_comment;
    m_toBeDeleted = false;
}

void CONFIG_FILE::LINE_ITEM::CreateDelimiter(char *txt)
{
    m_line = txt;
    m_key = "";
    m_value = "";
    m_errorMsg = "";
    m_status = CONFIG_STAT_delimiter;
    m_toBeDeleted = false;
}


CONFIG_FILE::CONFIG_FILE(void) 
{
  m_objectFunction=NULL;
  m_ids=NULL;
  m_numErrors=0;
}

CONFIG_FILE::~CONFIG_FILE(void)
{
  m_ids=NULL;
  m_objectFunction=NULL;
}


CONFIG_ITEM_STATUS CONFIG_FILE::split(
    LINE_ITEM *lineitem,
//    const CString& src,
    CString& key,
    CString& value,
    int *valueColumn)
{
  int len, incol, outcol;
  char *temp=NULL;
  CString& src = lineitem->m_line;
  // Skip leading whitespace.
  len=src.GetLength();
  temp=(char *) My_malloc(len+10); // Cannot get longer...only shorter
  if (temp==NULL)
  {
    MsgBoxError("Memory allocation error");
    return CONFIG_STAT_syntax;
  };
  key="";
  value="";
  if (valueColumn!=NULL) *valueColumn = -1;
  for (incol=0; (incol < len) && (whitespace(src[incol])); incol++){};
  if (incol==len) 
  {
    My_free(temp);
    return CONFIG_STAT_ok; // Blank line is ok.
  };
  outcol=0;
  // Now we look through the line for a non-escaped
  // equal sign.
  while (incol < len)
  {
    if (src[incol]=='\\')
    {
      incol++;
      if (incol>=len) 
      {
        My_free (temp);
        return CONFIG_STAT_syntax; // trailing escape-char
      };
      temp[outcol++]=src[incol++]; // copy escaped character
      continue;
    };
    if (src[incol]=='=') break;
    temp[outcol++]=src[incol++];
  };
  // So, did we find an equal sign?
  if ((incol>=len) || (src[incol]!='=')) 
  {
    My_free (temp);
    PerhapsLogError(lineitem,"No equal sign found in line");
    return CONFIG_STAT_syntax;
  };
  // That's nice.  We found an equals.  Now back off and
  // discard any trailing blanks from the key.
  while (whitespace(temp[--outcol])){};
  key=CString(temp,outcol+1);
  // Now to skip whitespace again.
  incol++;  // Bypass equal sign
  outcol=0;
  while ((incol<len) && (whitespace(src[incol]))) incol++;
  if (incol>=len) 
  {
    My_free(temp);
    return CONFIG_STAT_ok; // value is blank
  };
  if (valueColumn!=NULL) *valueColumn=incol;
  while (incol < len) 
  {
    if (src[incol]=='\\')
    {      
      incol++;
      if (incol>=len)
      {
        My_free (temp);
        return CONFIG_STAT_syntax;
      };
      temp[outcol++]=src[incol++]; // Copy escaped character
      continue;
    };
    temp[outcol++]=src[incol++]; // Copy non-escaped character
  };
  // Now delete trailing blanks from value
  while (whitespace(temp[--outcol])){};
  value=CString(temp,outcol+1);
  My_free (temp);
  return CONFIG_STAT_ok;
}


CONFIG_ITEM_STATUS CONFIG_FILE::ReadFile(CArchive& ar)
{
  // Read all the lines into memory and set their item numbers.
  LINE_ITEM lineItem;
  int objectNumber;
//  const CString emptyString="";
  //CString line;
  int linenumber=1; // Because title line already has been read.
  //CString errorMsg;
  objectNumber=0;
  m_filePath=ar.GetFile()->GetFilePath();
  CString cmp("\\(BEGIN)");

  while (ar.ReadString(lineItem.m_line))
  {
    linenumber++;
    lineItem.m_linenumber=linenumber;
    lineItem.m_status=CONFIG_STAT_intError; // Someone better fix it.
    if (lineItem.m_line.Left(8).CompareNoCase(cmp)==0)
    {
      lineItem.m_status=CONFIG_STAT_delimiter;
      objectNumber++;
    };
    m_objects.AddTail(objectNumber,lineItem);
  };
  return CONFIG_STAT_ok;
}


CONFIG_ITEM_STATUS CONFIG_FILE::ReadFile(CAR& car)
{
  // Read all the lines into memory and set their item numbers.
  LINE_ITEM lineItem;
  int objectNumber;
//  const CString emptyString="";
  //CString line;
  int linenumber=1; // Because title line already has been read.
  //CString errorMsg;
  objectNumber=0;
  m_filePath=car.GetFile()->GetFilePath();
  CString cmp("\\(BEGIN)");

  while (car.ReadString(lineItem.m_line))
  {
    linenumber++;
    lineItem.m_linenumber=linenumber;
    lineItem.m_status=CONFIG_STAT_intError; // Someone better fix it.
    if (lineItem.m_line.Left(8).CompareNoCase(cmp)==0)
    {
      lineItem.m_status=CONFIG_STAT_delimiter;
      objectNumber++;
    };
    m_objects.AddTail(objectNumber,lineItem);
  };
  return CONFIG_STAT_ok;
}


void CONFIG_FILE::PerhapsLogError(int objNum, int lineNum, const char *msg)
{
  LINE_ITEM *lineItem;
  lineItem = m_objects.GetLine(objNum,lineNum);
  PerhapsLogError(lineItem, msg);
}

void CONFIG_FILE::PerhapsLogError(LINE_ITEM *pLine, const CString& errorMsg)
{
  CString msg;
  if (LogDatabaseErrors > 0)
  {
    msg.Format("Error in \"%s\" - Line %d\n  %s  %s\n",
                m_filePath, pLine->m_linenumber,
                pLine->m_line,
                errorMsg);
      WriteDebugString("%s\n", msg);
  }
  else if (LogDatabaseErrors==0)
  {
    WriteDebugString("Additional errors not reported\n");
  };
  LogDatabaseErrors--;
}

CONFIG_ITEM_STATUS CONFIG_FILE::DecodeObject(
     int objectNumber,
     CONFIGID *ids,
     CString& objectName)  // Returns name of object
{
  int itemNumber;
  CONFIG_ITEM_STATUS worstStatus;
  CONFIG_PKT pkt;
  int i;
  LINE_ITEM *pLine;
  pkt.objectName="";
  worstStatus=CONFIG_STAT_ok; 
  CString cmp("\\(END)");
  for (itemNumber=0; 
     ((pLine=m_objects.GetLine(objectNumber,itemNumber)) != NULL); 
     itemNumber++)  // For each line in this object
  {
    if (pLine->m_status==CONFIG_STAT_delimiter) continue;
    if (pLine->m_line.Left(2)=="//") 
    {
      pLine->m_status=CONFIG_STAT_comment;
      continue; // Comments don't get decoded.
    };
    if (pLine->m_line.Left(6).CompareNoCase(cmp)==0)
    {
      pLine->m_status=CONFIG_STAT_delimiter;
      objectName=pkt.objectName;
      m_objects.SetName(objectNumber, objectName);
      for (itemNumber++; 
           ((pLine=m_objects.GetLine(objectNumber,itemNumber))!=NULL);
           itemNumber++)
      {
        pLine->m_status=CONFIG_STAT_comment; // after \(END) considered comment
      };
      return worstStatus;
    };
    pkt.status=split(pLine,pkt.key,pkt.value);
    pLine->m_status=pkt.status;
    if (pkt.status < CONFIG_STAT_syntax)
    {
      pLine->m_key=pkt.key;
      pLine->m_value=pkt.value;
    };
    if (pkt.status>worstStatus)
    {
      worstStatus=pkt.status;
    };
    pLine->m_status=worstStatus;
    if (pkt.status!=CONFIG_STAT_ok) continue;
    if (pkt.key.IsEmpty()) 
    { 
      pLine->m_status=CONFIG_STAT_comment;
      continue; // Blank line is ok and is considered a comment.
    };

    pkt.keyNum = 0;
    int blankidx = pkt.key.ReverseFind(' ');
    if (blankidx >= 2)
    {
      CString valtxt = pkt.key.Right(pkt.key.GetLength()-blankidx-1);
      pkt.keyNum = atoi(valtxt);
      if (pkt.keyNum > 0) // if group nbr present, strip from key
        pkt.key = pkt.key.Left(blankidx);
    }

    // Convert the key to an ID.
    for (i=0; ids[i].m_id.m_dataType!=CONFIG_DATA_end; i++)
    {
      if (pkt.key.CompareNoCase(ids[i].m_name)==0) 
        break;
    };

    if (ids[i].m_id.m_dataType==CONFIG_DATA_end) 
    {
      pkt.status=CONFIG_STAT_key;
      PerhapsLogError(pLine,"No such Keyword");
      pLine->m_status=pkt.status;
      if (pkt.status > worstStatus) 
        worstStatus=pkt.status;
      continue;
    };
    //
    // Send the item back to user for decoding and checking
    pkt.func=CONFIG_FUNC_checkValue;
//    pkt.itemNumber=itemNumber;
    pkt.IsStoring=false;
    (*ids[i].m_f)(&pkt);
    if (pkt.status!=CONFIG_STAT_ok) 
    {
      m_numErrors++;
      PerhapsLogError(pLine,pkt.errorMsg);
    };
    pLine->m_status=pkt.status;
    if (pkt.status>worstStatus) 
    {
      worstStatus=pkt.status;
    };
  };
  objectName=pkt.objectName;
  return worstStatus;
}

CONFIG_ITEM_STATUS CONFIG_FILE::DecodeFile(CONFIGID *ids, bool commit)
{
  CONFIGID *pID;
  int objectNumber;
  CString objectName;
  CONFIG_PKT pkt;
  CONFIG_ITEM_STATUS status;
  m_ids=ids;
  pID=ids;
  CONFIG_ITEM_STATUS objWorstStatus, fileWorstStatus;

  // Find the object manipulation function.
  while (pID->m_id.m_dataType!=CONFIG_DATA_object)
  {
    if (pID->m_id.m_dataType==0) 
    {
      return CONFIG_STAT_intError; // No object manipulation function
    };
    pID++;
  };
  m_objectFunction=pID->m_f;

  // The worst problem in the entire file.
  fileWorstStatus=CONFIG_STAT_ok;

  // Now we interate through the games objects
  for (objectNumber=0; 
     objectNumber < m_objects.NumberOfObjects(); 
     objectNumber++) 
  {
    // Clear the object.
    pkt.func=CONFIG_FUNC_beginObject;     
    (*m_objectFunction)(&pkt);
    //
    // We are starting a new item.
    objWorstStatus=CONFIG_STAT_ok;
    //
    // Check and set values in the object.
    status=DecodeObject(objectNumber, ids, objectName);
    if (status > objWorstStatus) 
    {
      objWorstStatus = status;
    };
    //
    // Store the object permanently.if requested
    if (commit)
    {
      pkt.func=CONFIG_FUNC_storeObject;
      pkt.status=CONFIG_STAT_ok;
      (*m_objectFunction)(&pkt);
      if (pkt.status > objWorstStatus) 
      {
        objWorstStatus = pkt.status;
      };
    };
    //
    // Remember worst status in entire file
    if (objWorstStatus > fileWorstStatus) 
    {
      fileWorstStatus=objWorstStatus;
    };
  };
  return fileWorstStatus;
}

CONFIG_ITEM_STATUS CONFIG_FILE::WriteFile(CArchive& ar, CONFIGID *ids)
{
  // You must first have read a file so that we can replace the
  // appropriate lines.  We would like to retain comments and ordering
  // of objects and their keyword/value pairs.  There is no way we can
  // retain formatting of individual lines like   "abc      =   3"
  // We will always put a single space around the equal sign and no
  // unnecessary spaces in the value string.
  // Our strategy:
  //  Decode the old file to get the keywords
  //  Mark all objects as 'to be deleted'
  //  While there are more objects
  //  {
  //    Get an object and its name
  //    If no such name in old file then create one
  //    Clear the 'to be deleted' flag
  //    Mark all proper keword lines 'to-be-deleted'
  //    For each keyword in the ids list
  //    {
  //      Delete any items with the same keyword and remember position.
  //      Call encoding function.  Insert at remembered position.
  //    }
  //  }
  //  Write all objects.  Those that are 'to-be-deleted' get commented out.
  // That's all Folks.
  //
  CONFIG_PKT pkt;
  int objectNumber;
  CONFIGID *pID;
  
  DecodeFile(ids, false);  // Get keywords and values separated.  Get object names.

  m_objects.MarkAllToDelete();  // Mark all objects to be deleted.
  m_objects.ClearToDelete(0);   // Object 0 always gets written

  for (pkt.status=CONFIG_STAT_ok, 
        pkt.func=CONFIG_FUNC_getFirstObject, 
         (*m_objectFunction)(&pkt);

       pkt.status==CONFIG_STAT_ok;

       pkt.func=CONFIG_FUNC_getNextObject, 
         (*m_objectFunction)(&pkt) )
  {
    if (pkt.objectName=="") continue;  // Unnamed objects are ignored.
    objectNumber=m_objects.FindObject(pkt.objectName);
    if (objectNumber==-1)
    { // We need to insert a new object
      LINE_ITEM line;
      objectNumber=m_objects.AddObject();
      line.CreateDelimiter("\\(BEGIN)");
      m_objects.AddTail(objectNumber, line);
      line.CreateDelimiter("\\(END)");
      m_objects.AddTail(objectNumber, line);
      line.CreateComment("");
      m_objects.AddTail(objectNumber, line);
    };
    m_objects.ClearToDelete(objectNumber); // Don't delete this one.
    m_objects.MarkItemsToDelete(objectNumber);  // All items marked to delete.
    // Now go through the keyword table, delete old entries, encode new entries.
    for (pID=ids; pID->m_id.m_dataType!=CONFIG_DATA_end; pID++)
    {
      // Delete all lines with this key.  Remember position.
      if (pID->m_id.m_dataType==CONFIG_DATA_object) continue;
      pkt.insertPoint=m_objects.DeleteKeys(objectNumber, pID->m_name);
      pkt.objectNumber=objectNumber;
      pkt.key=pID->m_name;
      pkt.IsStoring=true;
      pkt.status=CONFIG_STAT_more;
      pkt.listPos=NULL;
      for (int listNum=0; pkt.status==CONFIG_STAT_more; listNum++)
      {
        pkt.status=CONFIG_STAT_ok;
        pkt.listNum=listNum; // In case there are several.
        
        (*pID->m_f)(&pkt);  // Let it do the encoding.

        switch (pkt.status)
        {
        case CONFIG_STAT_skip:
          pkt.status = CONFIG_STAT_more;
          break;
        case CONFIG_STAT_ok:
        case CONFIG_STAT_more:
          if (pID->m_id.m_dataType == CONFIG_DATA_LONG)
          {
            CONFIG_ENCODE_long(pkt.value, pkt.longValue);
          };
          if (pID->m_id.m_dataType == CONFIG_DATA_DOUBLE)
          {
            CONFIG_ENCODE_float(pkt.value, pkt.doubleValue);
          };
          if (pID->m_readOnly == false)
          {
            if (pID->m_grouped)
            {
              if (pkt.value.GetLength() > 0)
              {
                // if using grouping, add sequential numbering to key
                CString tmpKey;
                tmpKey.Format(" %i", pkt.listNum+1);
                tmpKey = pkt.key + tmpKey;

                // A single line of output
                m_objects.InsertKeyValue(
                               objectNumber,
                               pkt.insertPoint, // Updated by InsertKeyValue
                               tmpKey,//pkt.key,
                               pkt.value);
              }
            }
            else
            {
              // A single line of output
              m_objects.InsertKeyValue(
                             objectNumber,
                             pkt.insertPoint, // Updated by InsertKeyValue
                             pkt.key,
                             pkt.value);
            }
          }
          break;
        case CONFIG_STAT_end: pkt.status=CONFIG_STAT_ok; break; // Nothing to do
        default:
          ASSERT(0);  // Should not happen
        };
      };
      if (pkt.status==CONFIG_STAT_more) pkt.status=CONFIG_STAT_ok;
    };
    // Now delete all items marked 'to-be-deleted'
    m_objects.RemoveDeletedLines(objectNumber);
  };
  m_objects.WriteFile(ar);
  if (pkt.status==CONFIG_STAT_intError) return pkt.status;
  return CONFIG_STAT_ok;
}
 
   
bool getToken(int& incol, const CString& value, CString& token)
{ // Skip leading and trailing whitespace so you can always look
  // at the next character after getting a token.
  int startcol;
  char c;
  int len=value.GetLength();
  while ( (incol<len) && whitespace(value[incol])) incol++;
  if (incol>=len) return false;
  startcol=incol;
  c=value[incol];
  if (   ((c>='A')&&(c<='Z'))
       ||((c>='a')&&(c<='z'))
       ||(c=='$')
     ) 
  {
    incol++;
    while (incol<len) 
    {  
      c=value[incol];
      if (   ((c>='A')&&(c<='Z'))
           ||((c>='a')&&(c<='z'))
           ||((c>='0')&&(c<='9'))
           ||(c=='$')
         )
      {
        incol++;
      }
      else break;
    };
    token=value.Mid(startcol,incol-startcol);
  }
  else
  { 
    token=c;
    incol++;
  };
  while ( (incol<len) && whitespace(value[incol])) incol++;
  return true;
}

int GetBLT (int& incol, const CString& value, CString& token, CString& errorMsg)
{ // Get Blank-Limited-Token.   Simpler than getToken
  // Skip leading and trailing whitespace so you can always look
  // at the next character after getting a token.
  // Double quotes around token are allowed.  
  int startcol, tlen;
  int len=value.GetLength();
  while ( (incol<len) && whitespace(value[incol])) incol++;
  if (incol>=len) return 0;
  int origcol = incol;
  if (value[incol]=='"') 
  {
    // collect characters up to closing double-quote
    incol++;
    startcol=incol;
    while (incol<len)
    {
      if (value[incol]=='"') break;
      incol++;
    }
    if (incol>=len) 
    {
      errorMsg="Missing closing double-quotes";
      incol = origcol; // restore position
      return 1;
    };
    tlen=incol-startcol;
    incol++;
  }
  else
  {
    startcol=incol;
    incol++;
    while (incol<len) 
    {  
      if (whitespace(value[incol])) break;
      incol++;
    };
    tlen=incol-startcol;
  };
  token=value.Mid(startcol,tlen);
  while ( (incol<len) && whitespace(value[incol])) incol++;
  return 2;
}


bool token2int (const CString& token, int& rslt)
{
  int len=token.GetLength();
  int i=0, sign =1;
  char c;
  int base=10;
  rslt=0;
  if (len>1) 
  {
    if (token[0]=='-')
    {
      sign=-1;
      i=1;
    }
    else if (token[0]=='+')
    {
      i=1;
    };
  }
  else if ((len>=2) && (token[0]=='0') && ((token[1]=='x')||(token[1]=='X')))
  {
    i=2;
    base=16;
  };
  for (; i< len; i++)
  {
    c=token[i];
    if ((c>='a')&&(c<='z')) c+='A'-'a';
    if ((c>='0')&&(c<='9')) rslt=10*rslt+c-'0';
    else if ((base==16)&&(c>='A')&&(c<='F')) rslt=16*rslt+c-'A'+10;
    else return false;
  };
  rslt *= sign;
  return true;
}



CONFIG_ITEM_STATUS GetInteger (
  int& column,
  const CString& value,
  int& result,
  CString& errorMsg)
{
  // Skips over leading and trailing whitespace
  // The string must not be all whitespace.
  int origColumn = column;
  int len;
  int sign=1;
  len=value.GetLength();
  result=0;
  while (column < len)
  {
    if (whitespace(value[column]))
    {
      column++;
      continue;
    };
    break;
  };

  // We have found first character, which
  // might be a +/- as in +3 or '- 4'
  if (column < len)
  {
    if (value[column]=='-')
    {
      sign=-1;
      // skip to next token
      column++;
      while ((column < len) && whitespace(value[column])) column++;
    }
    else if (value[column]=='+')    
    {
      sign=1;
      // skip to next token
      column++;
      while ((column < len) && whitespace(value[column])) column++;
    }
  }

  if (column>=len)
  {
    errorMsg="Looking for integer.  Found only blanks";
    column=origColumn;
    return CONFIG_STAT_value;
  };
  if ((value[column]<'0') || (value[column]>'9'))
    // There must be at least one digit!
  {
    errorMsg="Non-decimal digit in integer";
    column=origColumn;
    return CONFIG_STAT_value;
  };
  while (column < len) 
  {
    if (whitespace(value[column])) break;
    if ((value[column]<'0') || (value[column]>'9')) break;
    result=result*10+value[column]-'0';
    column++;
  };
  // We either found the end of line or a whitespace or a non-digit.
  // If it is whitespace, we promised to skip over it.
  while ((column < len) && whitespace(value[column])) column++;
  result*=sign;
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_flags (
    const CString& value,
    CString& errorMsg,
    CONFIGID *bitnames,
    unsigned int& result)
{
  static CString name;
  CONFIGID *pID;
  int len;
  int incol, startcol, endcol;
  char c=' ';  // Makes Lint happy
  bool plusok=false;
  len=value.GetLength();
  result=0;
  incol=0;
  while (incol<len)
  {
    startcol=-1;      // No identifier seen yet
    while (incol<len)
    {
      c=value[incol];
      if (!whitespace(c)) break;
      incol++;
    };
    if (incol>=len) break; // Nothing to do.
    if (plusok) {
      if (c!='+') 
      {
        errorMsg="Expected plus sign between identifiers";
        return CONFIG_STAT_value;
      };
      plusok=false;
      incol++;
      continue;
    };
    if (c=='+')
    {
      errorMsg="Unexpected plus sign";
      return CONFIG_STAT_value;
    };
    // c is the first character of an identifier.
    // Collect the rest.
    startcol=incol;  // Remember start.
    incol++;
    while (incol<len)
    {
      c=value[incol];
      if (c=='+') break;
      incol++;
    };
    // We have an identifier between startcol and incol.
    endcol=incol;
    // Get rid of trailing whitespace
    while (whitespace(value[--endcol])){};
    name=value.Mid(startcol, endcol-startcol+1);
    // Now we need to search for this identifier in the 
    //  list provided by the caller
    pID=bitnames;
    while (1)
    {
      if (name.CompareNoCase(LPCTSTR (pID->m_name))==0)
      {
        result |= pID->m_id.m_enumVal;
        break;
      };
     if (pID->m_id.m_enumVal == 0) 
       {
        errorMsg.Format("Unrecognized identifier = %s",name);
        return CONFIG_STAT_value;
      };
      pID++;
    };    
    plusok=true;
  };
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_enumList (
    const CString& value,
    CString& errorMsg,
    CONFIGID *bitnames,
    CList <int, int> **list)
{
  static CString name;
  static CList<int,int> List;
  CONFIGID *pID;
  int len;
  int incol, startcol, endcol;
  char c=' ';  // Make Lint happy
  bool plusok=false, found;
  List.RemoveAll();
  *list=&List;
  len=value.GetLength();
  incol=0;
  while (incol<len)
  {
    startcol=-1;      // No identifier seen yet
    while (incol<len)
    {
      c=value[incol];
      if (!whitespace(c)) break;
      incol++;
    };
    if (incol>=len) break; // Nothing to do.
    if (plusok) {
      if (c!='+') 
      {
        errorMsg="Expected plus sign between identifiers";
        return CONFIG_STAT_value;
      };
      plusok=false;
      incol++;
      continue;
    };
    if (c=='+')
    {
      errorMsg="Unexpected plus sign";
      return CONFIG_STAT_value;
    };
    // c is the first character of an identifier.
    // Collect the rest.
    startcol=incol;  // Remember start.
    incol++;
    while (incol<len)
    {
      c=value[incol];
      if (c=='+') break;
      incol++;
    };
    // We have an identifier between startcol and incol.
    endcol=incol;
    // Get rid of trailing whitespace
    while (whitespace(value[--endcol])){};
    name=value.Mid(startcol, endcol-startcol+1);
    // Now we need to search for this identifier in the 
    //  list provided by the caller
    pID=bitnames;
    found=false;
    while (!found)
    {
      if (pID->m_name == "") 
      {
        errorMsg.Format("Unrecognized identifier = %s",name);
        return CONFIG_STAT_value;
      };
      if (name.CompareNoCase(LPCTSTR (pID->m_name))==0)
      {
        List.AddTail(pID->m_id.m_enumVal);
        found=true;
      };
      pID++;
    };    
    plusok=true;
  };
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_integer (
    const CString& value,
    CString& errorMsg,
    int min,
    int max,
    int& result)
{
  int len;
  int i;
  unsigned char c;
  len=value.GetLength();
  result=0;
  int sign=1;
  for (i=0; i<len; i++)
  {
    if ((i==0) && (value[0]=='-')) 
    {
      sign=-1;
      continue;
    };
    c=value[i];
    if ((c<'0') || (c>'9'))
    {
      errorMsg="Non-digit in integer";
      return CONFIG_STAT_value;
    };
    result=result*10+c-'0';
  };
  result*=sign;
  if (result<min)
  {
    errorMsg="Integer less than minimum value";
    return CONFIG_STAT_value;
  };
  if (result>max)
  {
    errorMsg="Integer greater than maximum value";
    return CONFIG_STAT_value;
  };
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_long (
    const CString& value,
    CString& errorMsg,
    int min,
    int max,
    long& result,
    long DEFAULT)
{
  int i;
  CONFIG_ITEM_STATUS status;
  if (value=="")
  {
    result=DEFAULT;
    return CONFIG_STAT_ok;
  };
  status=CONFIG_DECODE_integer(value,errorMsg,min,max,i);
  if (status==CONFIG_STAT_ok) result=i;
  return status;
}

bool decodeNdM(int& incol, const CString& value, int& n, int& m)
{  // If it does not decode properly, leave incol as is.
  int len;
  CString errorMsg; // Discard error message
  int col=incol;
  len=value.GetLength();
  while ( (col<len) && whitespace(value[col])) col++;
  if (GetInteger(col,value,n,errorMsg)!=CONFIG_STAT_ok) return false;
  if ( (col>=len) || ( (value[col]!='D')&&(value[col]!='d') ) ) return false;
  col++;
  if (GetInteger(col,value,m,errorMsg)!=CONFIG_STAT_ok) return false;
  incol=col;
  return true;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_dice (
    const CString& value,
    CString& errorMsg,
    unsigned int& nDice,
    unsigned int& nSides,
    int& nBonus,
    int *levelBonus,
    int *maximum)
{
  CONFIG_ITEM_STATUS status;
  int temp;
  int len;
  len=value.GetLength();
  if (len==0)
  {
    nDice=0;
    nSides=0;
    nBonus=0;
    return CONFIG_STAT_ok;
  };
  // Expect value of form "diceDsides+-bonus"
  int incol=0;
  status=GetInteger(incol,value,temp,errorMsg);
  if (status!=CONFIG_STAT_ok) return status;
  if ((incol>=len) || 
      ((value[incol]!='*')&&(value[incol]!='d')&&(value[incol]!='D')))
  {
    errorMsg="Expected 'd' or 'D' or '*' after number of dice";
    return CONFIG_STAT_value;
  };
  incol++;
  if (temp>255)
  {
    errorMsg="Number of dice cannot be greater than 255";
    return CONFIG_STAT_value;
  };
  if (temp < 0) 
  {
    errorMsg="Number of dice cannot be negative";
    return CONFIG_STAT_value;
  };
  nDice=(unsigned int)temp;

  status=GetInteger(incol,value,temp,errorMsg);
  if (status!=CONFIG_STAT_ok) return status;
  if ((incol>=len) || ((value[incol]!='+') && (value[incol]!='-')))
  {
    errorMsg="Expected + or - after number of sides";
    return CONFIG_STAT_value;
  };
  //incol++;
  if (temp > 255) {
    errorMsg="Dice can have at most 255 sides";
    return CONFIG_STAT_value;
  };
  if (temp < 0)
  {
    errorMsg="Dice cannot have negative number of sides";
    return CONFIG_STAT_value;
  };
  nSides=(unsigned int)temp;

  status=GetInteger(incol,value,nBonus,errorMsg);
  if (status!=CONFIG_STAT_ok) return status;
  if (incol==value.GetLength()) return status;
  // Perhaps we are expecting a level bonus or maximum.
  if ((levelBonus==NULL) && (maximum==NULL))
  {
    errorMsg="Unexpected characters after bonus";
    return CONFIG_STAT_value;
  };
  // OK.  Is it level bonus or the word 'max' that comes next?
  // Level bonus is preceeded by plus
  if (value[incol]=='+')
  {
    // Level bonus!  Level bonus!
    incol++;
    status=GetInteger(incol,value,temp,errorMsg);
    if (status!=CONFIG_STAT_ok) return status;
    if ((incol>=len) ||
        (value[incol]!='*') )
    {
      errorMsg="Expected '*' following level bonus";
      return CONFIG_STAT_value;
    };
    incol++;
    while ((incol<len)&&(whitespace(value[incol]))) incol++;
    if (value.Mid(incol,5).CompareNoCase("level")!=0)
    {
      errorMsg="Expected \"*level\" following level bonus";
      return CONFIG_STAT_value;
    };
    incol+=5; // bypass 'level'
    if (levelBonus!=NULL) *levelBonus=temp;
  };
  // Now let us check for a 'max'
  while ((incol<len)&&(whitespace(value[incol]))) incol++;
  if (value.Mid(incol,3).CompareNoCase("max")!=0)
  {
    errorMsg="Expected the word \"max\" after \"*level\"";
    return CONFIG_STAT_value;
  };
  incol+=3; // bypass "max"
  status=GetInteger(incol,value,temp,errorMsg);
  if (status!=CONFIG_STAT_ok)
  {
    errorMsg+="...decoding max field";
    return status;
  };
  if (maximum!=NULL) *maximum=temp;
  return CONFIG_STAT_ok;
}

/*
#define DPT_NONE 0
#define DPT_INTEGER 1
#define DPT_OPERAND 2
#define DPT_TOKEN 3

struct DicePlusTokenParseData
{
  DicePlusTokenParseData() { type=DPT_NONE;val=""; }
  int type;
  CString val;
};

typedef CArray<DicePlusTokenParseData,DicePlusTokenParseData&> DPTokenListType;

struct DicePlusOperatorPrecedenceData
{
  const char *m_op;
  int m_prec;
};

#define MAX_DP_OPERATORS 13
#define MAX_DP_PRECEDENCE 8
DicePlusOperatorPrecedenceData DPOpPrecedence[MAX_DP_OPERATORS] =
{
  { "*",  2 },
  { "/",  2 },
  { "+",  3 },
  { "-",  3 },
  { "==", 1 },
  { ">",  1 },
  { ">=", 1 },
  { "<",  1 },
  { "<=", 1 },
  { "&&", 1 },
  { "||", 1 },
  { "|",  1 },
  { "&",  1 }  
};
*/

CString ConvertAdjEnumToToken(char op)
{
  switch (op)
  {
  //case AJO_none: return "";
  case AJO_gear:     return "*";
  case AJO_plus:     return "+";
  case AJO_equality: return "==";
  case AJO_greater:  return ">";
  case AJO_greaterEqual: return ">=";
  case AJO_andBit:       return "&";
  case AJO_slash:        return "/";
  case AJO_andLogical:   return "&&";
  //case AJO_continue:     return "";
  case AJO_minus:        return "-";
  case AJO_less:         return "<";
  case AJO_lessEqual:    return "<=";
  case AJO_orBit:        return "|";
  case AJO_orLogical:    return "||";
  }
  return "";
}

char ConvertTokenToAdjEnum(const CString& value)
{  
  //
  // must be careful not parse tokens like "-1" as 
  // AJO_minus
  //
  if (value.GetLength()==1)
  {
    if (value=="*") return AJO_gear;
    if (value=="+") return AJO_plus;
    if (value=="/") return AJO_slash;
    if (value==">") return AJO_greater;
    if (value=="&") return AJO_andBit;
    if (value=="-") return AJO_minus;
    if (value=="<") return AJO_less;
    if (value=="|") return AJO_orBit;
  }
  else if (value.GetLength()==2)
  {
    if (value=="==") return AJO_equality;  
    if (value==">=") return AJO_greaterEqual;  
    if (value=="&&") return AJO_andLogical;
    if (value=="<=") return AJO_lessEqual;
    if (value=="||") return AJO_orLogical;
  }  
  return AJO_none;
}
/*
BOOL GetDicePlusOperand (
    const CString& value,
    int &column,
    CString &token,
    CString& errorMsg)
{
  token = ConvertAdjEnumToToken(ConvertTokenToAdjEnum(value));
  return (!token.IsEmpty());
}

int GetNextDicePlusToken(
    const CString& value,
    int &column,
    CString &result,
    CString &errorMsg)
{
  int num;
  CString oper;
  CString token;
  int col=0;
  result="";

  switch (GetBLT(column, value, token, errorMsg))
  {
  case 0: // no more data
  case 1: // error
    return DPT_NONE;
  }

  if (GetDicePlusOperand(token,col,oper,errorMsg))
  {
    result = oper;
    return DPT_OPERAND;
  }
  else if (GetInteger(col,token,num,errorMsg)==CONFIG_STAT_ok)
  {
    result.Format("%i", num);
    return DPT_INTEGER;
  }
  else// if (getToken(col,token,token))
  {
    result = token;
    return DPT_TOKEN;
  }
  return DPT_NONE;
}
*/
/*
bool IsMinMaxOperator(DicePlusTokenParseData &data)
{
  if (data.type!=DPT_TOKEN) return false;
  if (data.val.CompareNoCase("min")==0) return true;
  if (data.val.CompareNoCase("max")==0) return true;
  return false;
}
*/
/*
bool IsOperatorWithPrecedence(DicePlusTokenParseData &data, int prec)
{
  if (data.type!=DPT_OPERAND) return false;
  for (int i=0;i<MAX_DP_OPERATORS;i++)
  {
    if (strncmp(DPOpPrecedence[i].m_op, data.val, strlen(DPOpPrecedence[i].m_op))==0)
      return (DPOpPrecedence[i].m_prec==prec);
  }
  return false;
}

BOOL ParseDicePlusAdjustmentTokenBinary(DPTokenListType &list, int index, ADJUSTMENT &adjData)
{
  // only one of tok1,tok2 should be a token,
  // the other should be a DPT_INTEGER
  if (list[index+1].type == DPT_TOKEN)
  {
    // 3*level
    ASSERT(list[index-1].type != DPT_TOKEN);
    adjData.m_parameter[0] = atoi(list[index-1].val); // param
    adjData.m_reference = list[index+1].val; // reference
    adjData.m_operator[0] = ConvertTokenToAdjEnum(list[index].val); // operator
  }
  else if (list[index-1].type == DPT_TOKEN)
  {
    // level*3
    ASSERT(list[index+1].type != DPT_TOKEN);
    adjData.m_parameter[0] = atoi(list[index+1].val); // param
    adjData.m_reference = list[index-1].val; // reference
    adjData.m_operator[0] = ConvertTokenToAdjEnum(list[index].val); // operator
  }
  else
  {
    // should be '3*2'          
    if (list[index+1].type == DPT_NONE)
    {
      ASS ERT(FALSE);
      return FALSE;
    }
    else
    {
      adjData.m_reference.m_refName = "__const__"; // will be LITERAL_DB
      adjData.m_reference.m_refKey = atoi(list[index-1].val); // and the actual literal
      adjData.m_parameter[0] = atoi(list[index+1].val);
      adjData.m_operator[0] = ConvertTokenToAdjEnum(list[index].val); // operator
    }
  }  

  // don't parse these tokens again
  list[index-1].type = DPT_NONE;
  list[index].type = DPT_NONE;
  list[index+1].type = DPT_NONE;
  return TRUE;
}

BOOL ParseDicePlusAdjustmentTokenUnaryLeft(DPTokenListType &list, int index, ADJUSTMENT &adjData)
{
  if (list[index-1].type == DPT_TOKEN)
  {
    // level*
    adjData.m_reference = list[index-1].val; // reference
    adjData.m_operator[0] = ConvertTokenToAdjEnum(list[index].val); // operator
    adjData.m_operator[1] = AJO_continue;
  }
  else
  {
    // should be '3*'          
    adjData.m_reference.m_refName = "__const__"; // will be LITERAL_DB
    adjData.m_reference.m_refKey = atoi(list[index-1].val); // and the actual literal
    adjData.m_operator[0] = ConvertTokenToAdjEnum(list[index].val); // operator
    adjData.m_operator[1] = AJO_continue;
  }  

  // don't parse these tokens again
  list[index-1].type = DPT_NONE;
  list[index].type = DPT_NONE;
  return TRUE;
}

BOOL ParseDicePlusAdjustmentTokenUnaryRight(DPTokenListType &list, int index, ADJUSTMENT &adjData)
{
  if (list[index+1].type == DPT_TOKEN)
  {
    // *level
    adjData.m_reference = list[index+1].val; // reference
    adjData.m_operator[0] = ConvertTokenToAdjEnum(list[index].val); // operator
    adjData.m_operator[1] = AJO_continue;
  }
  else
  {
    // should be '*3'
    adjData.m_reference.m_refName = "__const__"; // will be LITERAL_DB
    adjData.m_reference.m_refKey = atoi(list[index+1].val); // and the actual literal
    adjData.m_operator[0] = ConvertTokenToAdjEnum(list[index].val); // operator
    adjData.m_operator[1] = AJO_continue;
  }  

  // don't parse these tokens again
  list[index+1].type = DPT_NONE;
  list[index].type = DPT_NONE;
  return TRUE;
}

BOOL ParseDicePlusAdjustmentTokens(DPTokenListType &list, int index, ADJUSTMENT &adjData)
{
  if ((index < 0) || (index >= list.GetSize())) return FALSE;

  adjData.Clear(); 

  char tokcount = 0;
  if (list[index-1].type!=DPT_NONE) tokcount++;
  if (list[index+1].type!=DPT_NONE) tokcount++;

  if (tokcount == 2)
  {
    return ParseDicePlusAdjustmentTokenBinary(list,index,adjData); // two operands, 3*2
  }
  else if (tokcount == 1)
  {
    if (list[index-1].type!=DPT_NONE)
      return ParseDicePlusAdjustmentTokenUnaryLeft(list,index,adjData); // left operand, 3*
    else
      return ParseDicePlusAdjustmentTokenUnaryRight(list,index,adjData); // right operand, *3
  }
  return FALSE;
}

BOOL ParseDicePlusMinMaxTokens(DPTokenListType &list, int index, DICEPLUS *dice)
{
  if ((index < 0) || (index >= list.GetSize())) return FALSE;
  if (dice==NULL) return FALSE;

  if (list[index].val.CompareNoCase("min")==0)
  {
    if (index+1 < list.GetSize())
    {
      if (list[index+1].type == DPT_INTEGER)
      {
        dice->m_min = atoi(list[index+1].val);
        list[index+1].type = DPT_NONE;
      }
    }
    list[index].type = DPT_NONE;
  }
  else if (list[index].val.CompareNoCase("max")==0)
  {
    if (index+1 < list.GetSize())
    {
      if (list[index+1].type == DPT_INTEGER)
      {
        dice->m_max = atoi(list[index+1].val);
        list[index+1].type = DPT_NONE;
      }
    }
    list[index].type = DPT_NONE;
  }
  return FALSE;
}

void ProcessZeroOperatorTokens(DPTokenListType &list, DICEPLUS *dice)
{
  //no operators, as in 1d4+3 or 1d4+level

  if (list.GetSize()==0) return;
  if (dice==NULL) return;
  
  for (int i=0;i<list.GetSize();i++)
  {
    if (list[i].type==DPT_INTEGER)
    {
      // there should not be any tokens on
      // either side of this one by now
      if (i+1 < list.GetSize())
      {
        if (list[i+1].type != DPT_NONE)
          return; 
      }
      if (i-1 >= 0)
      {
        if (list[i-1].type != DPT_NONE)
          return; 
      }

      ADJUSTMENT adjData;
      adjData.m_reference.m_refName = "__const__"; // will be LITERAL_DB
      adjData.m_reference.m_refKey = atoi(list[i].val); // and the actual literal
      adjData.m_operator[0] = AJO_continue;
      dice->AddAdjust(adjData);
      list[i].type = DPT_NONE;
    }
    else if (list[i].type==DPT_TOKEN)
    {
      // there should not be any tokens on
      // either side of this one by now
      if (i+1 < list.GetSize())
      {
        if (list[i+1].type != DPT_NONE)
          return; 
      }
      if (i-1 >= 0)
      {
        if (list[i-1].type != DPT_NONE)
          return; 
      }

      ADJUSTMENT adjData;
      adjData.m_reference = list[i].val; // reference
      adjData.m_operator[0] = AJO_continue;
      dice->AddAdjust(adjData);
      list[i].type = DPT_NONE;
    }
  }
}

void ProcessMinMaxModifiers(DPTokenListType &list, DICEPLUS *dice)
{
  if (list.GetSize()==0) return;
  if (dice==NULL) return;

  // Min/Max tokens
  //  min 0, max 1000

  for (int i=list.GetSize()-2;i>=0;i--)
  {
    if (IsMinMaxOperator(list[i]))
      ParseDicePlusMinMaxTokens(list, i, dice);
  }
}

// return 1 for forward, -1 for backward
int ScanForwardOrBackward(DPTokenListType &list)
{
  // look for an operator on either end of
  // list that doesn't is preceeded or trailed
  // by DPT_NONE. Default to forward scan.
  //
  if (list.GetSize() <= 3) return 1; // doesn't matter if 3 or less tokens

  int last = list.GetUpperBound();
  if (list[last].type!=DPT_NONE)
  {
    // can't be a trailing operator
    return 1; // scan forward
  }

  // start at the end and skip over processed
  // tokens (DPT_NONE)
  while ((last > 0) && (list[last].type==DPT_NONE))
    last--;

  // We've located the last unprocessed token in
  // the list. If it's an operator, we need
  // scan backwards
  if (list[last].type==DPT_OPERAND)
  {
    return -1;
  }
  return 1; // scan forward
}

void ProcessLevelPrecedence(DPTokenListType &list, DICEPLUS *dice, int prec)
{
  if (list.GetSize()==0) return;
  if (dice==NULL) return;

  if (ScanForwardOrBackward(list) < 0)
  {
    for (int i=list.GetSize()-2;i>=0;i--)
    {
      if (IsOperatorWithPrecedence(list[i], prec))
      {
        ADJUSTMENT adjData;
        if (ParseDicePlusAdjustmentTokens(list, i, adjData))
          dice->AddAdjust(adjData);
      }
    }
  }
  else
  {
    for (int i=0;i<list.GetSize()-1;i++)
    {
      if (IsOperatorWithPrecedence(list[i], prec))
      {
        ADJUSTMENT adjData;
        if (ParseDicePlusAdjustmentTokens(list, i, adjData))
          dice->AddAdjust(adjData);
      }
    }
  }
}
*/


CONFIG_ITEM_STATUS CONFIG_DECODE_dicePlus (
    const CString& value,
    CString& errorMsg,
    DICEPLUS *dice,
    int numdice, int numside, int bonus, int min, int max)
{
  //CONFIG_ITEM_STATUS status;
  /*
  CString token;
  int temp;
  int len;
  //int sign;
  int n, m;
  int fieldNum;
  int incol=0;
  bool signlocated=false;
  bool diceoplocated=false;
  */
  dice->Clear();

  // init using default values provided by caller
  /*
  dice->m_numDice=numdice;
  dice->m_numSides=numside;
  dice->m_bonus=bonus;
  dice->m_min=min;
  dice->m_max=max;
  */
  dice->m_Text = value;
  dice->m_Text.TrimLeft();
  dice->m_Text.TrimRight();
  dice->m_Text.Remove(' ');
  dice->m_Text.Remove('\n');
  dice->m_Text.Remove('\r');
  dice->m_Text.Remove('\t');

  //len=value.GetLength();
  //if (len==0) return CONFIG_STAT_ok;

  if (!dice->Compile()) return CONFIG_STAT_value;
  return CONFIG_STAT_ok;
  
  //DPTokenListType tokenlist;

  // Now comes a series of one of the following:
  // +-ndm 
  // +/- bonus
  // +-bonus/characteristic
  // min n
  // max n

  /*
  for (fieldNum=0; incol<len; fieldNum++)
  {
    if (fieldNum==0)
    {
      // check for +/- as in
      // - 2d4, or + 2d4
      //
      // sign defaults to +1
      int oldcol = incol;
      if (getToken(incol, value, token))
      {
        if (token=="-")
        {
          dice->m_sign = -1;
          signlocated=true;
        }
        else if (token=="+")
        {
          dice->m_sign = 1;
          signlocated=true;
        }
        else
          incol = oldcol;
      }
      else
        incol = oldcol;

      // What have we here?
      // could still have sign prefix, as in
      // -2d4 or +2d4 (sign comes back with numDice
      //
      if (decodeNdM(incol,value,n,m))
      {
        if (!signlocated)
        {
          if (n >= 0) dice->m_sign=1;
          else dice->m_sign=-1;
          signlocated=true;
        }
        dice->m_numDice=abs(n);
        dice->m_numSides=m;
        continue; // to next specification
      }
      else if (GetInteger(incol,value,temp,errorMsg)==CONFIG_STAT_ok)
      {
        // The line starts with an integer.
        dice->m_bonus=temp;
        continue;
      }
      else
      {
        errorMsg.Format("DicePlus must start with integer or dice expression: \'%s\'", token);
        return CONFIG_STAT_value;
      }
    }

    DicePlusTokenParseData tokendata;
    tokendata.type = GetNextDicePlusToken(value,incol,tokendata.val,errorMsg);
    if (tokendata.type!=DPT_NONE)
    {
      if ((!diceoplocated)&&(tokendata.type==DPT_OPERAND))
      {
        // first adjustment is operator following dice.
        // for 3d4 + 2 * level==4, this will be the '+' operator
        diceoplocated=true;
        ADJUSTMENT adjData;
        adjData.m_operator[0] = ConvertTokenToAdjEnum(tokendata.val);
        dice->AddAdjust(adjData);
      }
      else
        tokenlist.Add(tokendata);
    }
  };

  if (tokenlist.GetSize()==0) return CONFIG_STAT_ok;

  ProcessMinMaxModifiers(tokenlist, dice);
  for (int p=1;p<=MAX_DP_PRECEDENCE;p++)
    ProcessLevelPrecedence(tokenlist, dice, p);


  //Process1stLevelPrecedence(tokenlist, dice);
  //Process2ndLevelPrecedence(tokenlist, dice);
  //Process3rdLevelPrecedence(tokenlist, dice);    
  ProcessZeroOperatorTokens(tokenlist, dice);

  // list should be empty if all tokens were recognized
  CString baseerr;
  baseerr.Format("Some DicePlus tokens not recognized in \'%s\':\n", value);
  bool tokerror=false;
  for (int i=0;i<tokenlist.GetSize();i++)
  {
    if (tokenlist[i].type != DPT_NONE)
    {      
      tokerror=true;
      baseerr += tokenlist[i].val;
      baseerr += ",";
    }
  }

  if (tokerror)
  {
    errorMsg = baseerr;
    return CONFIG_STAT_value;
  }

  return CONFIG_STAT_ok;
  */
}

/*
bool RefFirstChar(char c)
{
  if ( (c>='A')&&(c<='Z') ) return true;
  if ( (c>='a')&&(c<='z') ) return true;
  if ( (c=='$')||(c=='_') ) return true;
  return false;
}
int adjustGetOp(int& col, const CString& value, char& op, CString& errorMsg)
{
  int result;
  CString token;
  result=GetBLT(col, value, token,errorMsg);
  if (result==0) return 0; // Nothing left to get
  if (result==1) return 1; // error  
  op = ConvertTokenToAdjEnum(token);
  if (op==AJO_none)
  {
    errorMsg.Format("Unknown arithmetic operator = \"%s\"", token);
    return 1;
  };
  return 2; // All is well
}
*/

CONFIG_ITEM_STATUS CONFIG_DECODE_adjust(
    const CString& value,
    CString& errorMsg,
    ADJUSTMENTListType &list)
    //ADJUSTMENT *adjust)
{
  // reference op1 A op2 B op3 [C]
  //int col=0;
  //int incol=0;
  int len = value.GetLength();
  if (len==0) return CONFIG_STAT_ok;
  //int temp;
  //int result;
  //CString token;
  //adjust->Clear();
  list.RemoveAll();
  return CONFIG_STAT_ok;

  /*
  DPTokenListType tokenlist; 
  
  while (incol<len)
  { 
    DicePlusTokenParseData tokendata;
    tokendata.type = GetNextDicePlusToken(value,incol,tokendata.val,errorMsg);
    if (tokendata.type!=DPT_NONE)
      tokenlist.Add(tokendata);
  }

  if (tokenlist.GetSize()==0) return CONFIG_STAT_ok;

  DICEPLUS dice; // this just let's us use the same token parsing functions
  for (int p=1;p<=MAX_DP_PRECEDENCE;p++)
    ProcessLevelPrecedence(tokenlist, &dice, p);

  //Process1stLevelPrecedence(tokenlist, &dice);
  //Process2ndLevelPrecedence(tokenlist, &dice);
  //Process3rdLevelPrecedence(tokenlist, &dice);  
  ProcessZeroOperatorTokens(tokenlist, &dice);

  // list should be empty if all tokens were recognized
  CString baseerr;
  baseerr.Format("Some ADJUSTMENT tokens not recognized in %s: ", value);
  bool tokerror=false;
  int i;
  for (i=0;i<tokenlist.GetSize();i++)
  {
    if (tokenlist[i].type != DPT_NONE)
    {      
      tokerror=true;
      baseerr += tokenlist[i].val;
      baseerr += ",";
    }
  }

  if (tokerror)
  {
    errorMsg = baseerr;
    return CONFIG_STAT_value;
  }

  for (i=0;i<dice.m_adjust.GetSize();i++)
    list.Add(dice.m_adjust[i]);
  */

  /*
  for (int i=0; i<4; i++)
  {
    if (i==0)
    { 
      result=GetBLT(col,value,token,errorMsg); 
      if (result==0) return CONFIG_STAT_ok;
      if (result==1) return CONFIG_STAT_value;
      if (token2int(token,temp))
      { // We accept an integer in place of the reference
        adjust->m_reference.m_refName="__const__";
        adjust->m_reference.m_refKey=temp;
      }
      else if (RefFirstChar(token[0]))  // Alpha, dollar, or underscore.
      {
        adjust->m_reference.m_refName=token;
      }
      else
      {
        errorMsg = "First term of adjust must be integer or named reference";
        return CONFIG_STAT_value;
      };
    }
    else  // if not first term
    {
      result=GetBLT(col,value,token, errorMsg);
      if (result==1) return CONFIG_STAT_value;
      if (result==0)
      {
        // Trailing opcode is rather special
        // We will use the opcode field to remember that this
        // is a special case and use the third parameter to
        // remember the actual opcode.
        adjust->m_parameter[i-1]=adjust->m_operator[i-1];
        adjust->m_operator[i-1]=AJO_continue;
        return CONFIG_STAT_ok;
      }
      else if(token2int(token, temp))
      {
        adjust->m_parameter[i-1]=temp;
      }
      else 
      {
        errorMsg="Undecodeable numeric term in adjust expression";
        return CONFIG_STAT_value;
      };
    };
    // Now we go for operation
    if (i<3)
    {
      result=adjustGetOp(col,value,adjust->m_operator[i],errorMsg);
      switch (result)
      {
      case 0: return CONFIG_STAT_ok; // end of line
      case 1: return CONFIG_STAT_value; // Error
      }
    }    
  }
  */
  //return CONFIG_STAT_ok;
}


CONFIG_ITEM_STATUS CONFIG_DECODE_file(
    const CString& value,
    CString& /* errorMsg */,
    CString& result,
    const CString& Default)
{
  // All we do is convert forward slashes to back slashes.
  int col;
  if ( value=="") 
  {
      result=Default;
      return CONFIG_STAT_ok;
  };
  result=value;
  while ((col=result.Find('/'))>=0)
  {
    result.SetAt(col,'\\');
  };
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_enum (
    const CString& value,
    CString *errorMsg,
    const CONFIGID *possibilities,
    int& result)
{
  // The first item in the list is default for the case value == ""
  if (value=="") 
  {
    result=possibilities->m_id.m_enumVal;
    return CONFIG_STAT_ok;
  };
  while (possibilities->m_name!="")
  {
    if (value==possibilities->m_name) 
    {
      result = possibilities->m_id.m_enumVal;
      return CONFIG_STAT_ok;
    };
    possibilities++;
  };
  if (errorMsg != NULL)
  errorMsg->Format("No such enumerated possibility as %s",value);
  return CONFIG_STAT_value;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_string (
    const CString& value,
    CString& /*errorMsg*/,
    CString& result)
{
  // Strip leading and trailing double-quotes
  result=value;
  result.TrimLeft();
  result.TrimRight();
  int len = result.GetLength();
  if ((len>1)
      && (result[0]=='"')
      && (result[len-1]=='"'))
  {
    result=result.Mid(1,result.GetLength()-2);
  };
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_float (
    const CString& value,
    CString& errorMsg,
    double& result,
    double DEFAULT)
{
  char *end;
  if (value.IsEmpty())
  {
    result=DEFAULT;
    return CONFIG_STAT_ok;
  };
  result=strtod(value,&end);
  if (end-LPCTSTR(value)!=value.GetLength()) 
  {
    errorMsg="Extra cgaracters after floating point number";
    return CONFIG_STAT_value;
  };
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_BOOL (
    const CString& value,
    CString& errorMsg,
    BOOL& result,
    BOOL DEFAULT)
{
  if (value=="")
  {
    result=DEFAULT;
    return CONFIG_STAT_ok;
  };
  if ( (value.CompareNoCase("true")==0) ||
     (value.CompareNoCase("yes")==0) )
  {
    result=TRUE;
    return CONFIG_STAT_ok;
  };
  if ( (value.CompareNoCase("false")==0) ||
     (value.CompareNoCase("no")==0) )
  {
    result=FALSE;
    return CONFIG_STAT_ok;
  };
  errorMsg="Boolean value must be 'true', 'yes', 'false', or 'no'";
  return CONFIG_STAT_value;
}

//CONFIG_ITEM_STATUS CONFIG_FILE::Edit(void) 
//{
//  ConfigFileDlg dlg;
//  dlg.m_infile=this;
//  dlg.DoModal();
//  return CONFIG_STAT_ok;
//}

CONFIG_ITEM_STATUS CONFIG_FILE::GetObjectInfo(
      int objectNumber, 
      CString &objectName)
{
  // You provide an object number.  We will return the name.
  CONFIGID *pID;
  CONFIG_PKT pkt;
  CONFIG_ITEM_STATUS status;
  if (m_ids==NULL) return CONFIG_STAT_intError;
  pID=m_ids;
  void (*objectFunction)(CONFIG_PKT *pkt);

  // Find the object manipulation function.
  while (pID->m_id.m_dataType!=CONFIG_DATA_object)
  {
    if (pID->m_id.m_dataType==CONFIG_DATA_end) 
    {
      return CONFIG_STAT_intError; // No object manipulation function
    };
    pID++;
  };
  objectFunction=pID->m_f;

  if (objectNumber>=m_objects.NumberOfObjects()) 
  {
    return CONFIG_STAT_intError;
  };
  // Clear the object.
  pkt.func=CONFIG_FUNC_beginObject;     
  (*objectFunction)(&pkt);
  // Decode this object.
  // Check and set values in the object and get its name.
  status=DecodeObject(objectNumber, m_ids, objectName);
  return status;

}

      
bool CONFIG_FILE::Exists(int objectNumber) const
{
  return m_objects.NumberOfObjects() > objectNumber;
}

CString CONFIG_FILE::GetFileName(void) 
{
  return m_filePath;
}

CONFIG_FILE::OBJECTS::OBJECTS(void) {
  m_numberOfObjects=0;
  m_allocatedSize=10;
  m_objectList=(OBJECT **)My_malloc(m_allocatedSize*sizeof (*m_objectList));
  if (m_objectList==NULL)
  {
    MsgBoxError("OBJECTS memory allocation error");
    _exit(1);
  };
  for (int i=0; i<m_allocatedSize; i++) m_objectList[i]=NULL;
}

int CONFIG_FILE::OBJECTS::FindObject(const CString& name) 
{
  for (int i=0; i<m_numberOfObjects; i++)
  {
    if (m_objectList[i]==NULL) continue;
    if ((*m_objectList[i]).m_objectName==name) return i;
  };
  return -1;
}

void CONFIG_FILE::OBJECTS::InsertKeyValue(
                int objectNumber,
                int& insertPoint,
                const CString& key,
                const CString& value)
{
  ASSERT (m_objectList!=NULL);
  ASSERT (objectNumber < m_numberOfObjects);
  m_objectList[objectNumber]->InsertKeyValue(
            insertPoint,
            key,
            value);
}

void CONFIG_FILE::OBJECTS::SetName(int objectNumber, const CString& name)
{
  ASSERT (m_objectList!=NULL);
  ASSERT (objectNumber < m_numberOfObjects);
  m_objectList[objectNumber]->m_objectName=name;
}

void CONFIG_FILE::OBJECTS::WriteFile(CArchive& ar)
{
  ASSERT(m_objectList!=NULL);
  for (int i=0; i<m_numberOfObjects; i++)
  {
    if (m_objectList[i]==NULL) continue;
    m_objectList[i]->WriteFile(ar);
  };
}


void CONFIG_FILE::OBJECTS::MarkAllToDelete(void)
{
  ASSERT(m_objectList!=NULL);
  for (int i=0; i<m_numberOfObjects; i++)
  {
    (*(m_objectList[i])).m_toBeDeleted=true;
  };
}

void CONFIG_FILE::OBJECTS::MarkItemsToDelete(int objectNumber)
{
  ASSERT(m_objectList!=NULL);
  ASSERT (objectNumber < m_numberOfObjects);
  (*m_objectList[objectNumber]).MarkItemsToDelete();
}

int CONFIG_FILE::OBJECTS::AddObject (int objectNumber)
{
  if (objectNumber<0) objectNumber=m_numberOfObjects;
  if (objectNumber>=m_allocatedSize)
  {
    int newSize=m_allocatedSize*3/2+5;
    if (newSize <= objectNumber) newSize=objectNumber+5;
    m_objectList=(OBJECT **)My_realloc(m_objectList, newSize*sizeof (*m_objectList));
    if (m_objectList==NULL)
    {
      MsgBoxError("OBJECTS memory allocation error");
      _exit(1);
    };
    for (int i=m_allocatedSize; i<newSize; i++) m_objectList[i]=NULL;
    m_allocatedSize=newSize;
    m_numberOfObjects=objectNumber+1;
  };
  ASSERT(m_objectList!=NULL);
  if (m_objectList[objectNumber]==NULL)
  {
    if (objectNumber>=m_numberOfObjects) 
        m_numberOfObjects=objectNumber+1;
    OBJECT *p = new OBJECT;
    m_objectList[objectNumber]=p;
  };
  return objectNumber;
}


void CONFIG_FILE::OBJECTS::ClearToDelete(int objectNumber)
{
  ASSERT(m_objectList!=NULL);
  // check for no leading comments.
  if ((objectNumber==0) && (m_numberOfObjects==0)) return;
  ASSERT (objectNumber < m_numberOfObjects);
  (*(m_objectList[objectNumber])).m_toBeDeleted=false;
}

void CONFIG_FILE::OBJECTS::RemoveDeletedLines(int objectNumber)
{
  ASSERT(m_objectList!=NULL);
  ASSERT( objectNumber < m_numberOfObjects);
  (*m_objectList[objectNumber]).RemoveDeletedLines();
}

int CONFIG_FILE::OBJECTS::DeleteKeys(int objectNumber, const CString& key)
{
  ASSERT(m_objectList!=NULL);
  ASSERT (objectNumber < m_numberOfObjects);
  return (*m_objectList[objectNumber]).DeleteKeys(key);
}

int CONFIG_FILE::NumberOfItems(int objNum) const
{
  if (!Exists(objNum)) return 0;
  return m_objects.NumberOfItems(objNum);
}

CString CONFIG_FILE::GetLineText(int objectNumber, int lineNumber)
{
  LINE_ITEM *lineItem;
  if (!Exists(objectNumber)) return emptyString;
  lineItem=m_objects.GetLine(objectNumber,lineNumber);
  if (lineItem==NULL) return "";
  return lineItem->m_line;
}

bool CONFIG_FILE::GetLineInfo(
    int objectNumber,
    int lineNumber,
    CString& line,
    CONFIG_ITEM_STATUS& status,
    int *helpMsgID,
    CString *errorMsg,
    int *valueColumn)
    // Return true if such a line exists.
    // valueColumn set to first character of value or -1.
{
  LINE_ITEM *lineItem;
  CONFIG_PKT pkt;
  CONFIG_ITEM_STATUS stat;
  if (!Exists(objectNumber)) return false;
  lineItem=m_objects.GetLine(objectNumber,lineNumber);
  if (lineItem==NULL) return false;
  line=lineItem->m_line;
  status=lineItem->m_status;
  if ((helpMsgID!=NULL) || (errorMsg!=NULL))
  {
    // We must call the decoding function to get more information.
    CString key, value;
    stat=split(lineItem,key,value,valueColumn);
    if (stat != CONFIG_STAT_ok) {
      if (helpMsgID!=NULL) *helpMsgID=0;
      if (errorMsg!=NULL) *errorMsg="Syntax error";
      status=stat;
      lineItem->m_status=stat;
      return true;
    };
    if ((m_objectFunction==NULL) || (m_ids==NULL))
    {
      MsgBoxError("GetLineInfo internal Error");
      if (helpMsgID!=NULL) *helpMsgID=0;
      if (errorMsg!=NULL) *errorMsg="No error info available";
      return true;
    };
    pkt.func=CONFIG_FUNC_beginObject;
	int i;
    for (i=0; m_ids[i].m_id.m_dataType!=CONFIG_DATA_end;i++)
    {
      if (m_ids[i].m_name.CompareNoCase(key)==0) break;
    };
    if (m_ids[i].m_id.m_dataType==CONFIG_DATA_end)
    {
      lineItem->m_status=CONFIG_STAT_key;
      PerhapsLogError(lineItem,"No such keyword");
      status=lineItem->m_status;
      if (errorMsg!=NULL)*errorMsg="No such keyword defined";
      if (helpMsgID!=NULL) *helpMsgID=0;
      return true;
    }
    else
    {
      pkt.key=key;
      pkt.value=value;
      pkt.status=CONFIG_STAT_ok;
      pkt.IsStoring=false;
      m_ids[i].m_f(&pkt);
      if (errorMsg!=NULL) *errorMsg=pkt.errorMsg;
      if (helpMsgID!=NULL) *helpMsgID=pkt.helpMsgID;
      status=pkt.status;
      return true;
    }
  }
  return true;
}


int CONFIG_FILE::OBJECTS::NumberOfItems(int objNum) const
{
  return m_objectList[objNum]->m_numberOfItems;
}

CONFIG_FILE::OBJECTS::~OBJECTS(void) {
  ASSERT(m_objectList!=NULL);
  for (int i=0; i<m_numberOfObjects; i++)
  {
    if (m_objectList[i]!=NULL)
    {
      delete(m_objectList[i]);
      m_objectList[i]=NULL;
    };
  };
  if (m_objectList!=NULL) My_free (m_objectList);
}

void CONFIG_FILE::OBJECTS::AddTail(int objectNumber, LINE_ITEM& item) {
  if (objectNumber>=m_allocatedSize)
  {
    int newSize=m_allocatedSize*3/2+5;
    if (newSize <= objectNumber) newSize=objectNumber+5;
    m_objectList=(OBJECT **)My_realloc(m_objectList, newSize*sizeof (*m_objectList));
    if (m_objectList==NULL)
    {
      MsgBoxError("OBJECTS memory allocation error");
      _exit(1);
    };
    for (int i=m_allocatedSize; i<newSize; i++) m_objectList[i]=NULL;
    m_allocatedSize=newSize;
    m_numberOfObjects=objectNumber+1;
  };
  ASSERT(m_objectList!=NULL);
  if (m_objectList[objectNumber]==NULL)
  {
    if (objectNumber>=m_numberOfObjects) 
        m_numberOfObjects=objectNumber+1;
    OBJECT *p = new OBJECT;
    m_objectList[objectNumber]=p;
  };
  m_objectList[objectNumber]->AddTail(item);
}
  
CONFIG_FILE::LINE_ITEM *CONFIG_FILE::OBJECTS::GetLine
      (int objectNum, int itemNum)
{ 
  ASSERT (m_objectList!=NULL);
  if ((objectNum>=m_numberOfObjects) ||
      (m_objectList[objectNum]==NULL))
  {
    MsgBoxError("OBJECTS::GetLine internal error");
    return NULL;
  };
  return m_objectList[objectNum]->GetLine(itemNum);
}

CONFIG_FILE::OBJECT::OBJECT(void) {
  m_allocatedSize=10;
  m_numberOfItems=0;
  m_itemList=(LINE_ITEM **)My_malloc(m_allocatedSize*sizeof (*m_itemList));
  if (m_itemList==NULL)
  {
    MsgBoxError("OBJECT memory allocation failure");
    _exit(1);
  };
  m_toBeDeleted=false;
}

CONFIG_FILE::OBJECT::~OBJECT(void) 
{
  for (int i=0; i<m_numberOfItems; i++)
  {
    delete m_itemList[i];
  };
  m_numberOfItems=0;
  if (m_itemList!=NULL) My_free(m_itemList);
  m_itemList=NULL;
  m_allocatedSize=0;
}

void CONFIG_FILE::OBJECT::InsertKeyValue(
            int& insertPoint,
            const CString& key,
            const CString& value)
{
  LINE_ITEM item;
  int index;
  item.m_key=key;
  item.m_value=value;
  item.m_status=CONFIG_STAT_ok;
  item.m_toBeDeleted=false;
  index=insertPoint;
  if (index<=0) index=1; // after the \(BEGIN)
  if (index > m_numberOfItems) index=m_numberOfItems;
  insertPoint=index+1; // In case there are more to come.
  m_numberOfItems++;
  if (m_numberOfItems>m_allocatedSize) 
  {
    m_allocatedSize=m_allocatedSize*3/2+5;
    m_itemList=(LINE_ITEM **)My_realloc(m_itemList,
      m_allocatedSize*sizeof (*m_itemList));
    if (m_itemList==NULL)
    {
      MsgBoxError("OBJECT memory allocation failure");
      _exit(1);
    };
  };
  for (int i=m_numberOfItems-1; i>index ; i--)
    m_itemList[i]=m_itemList[i-1];
  LINE_ITEM *p = new LINE_ITEM(item);
  m_itemList[index]=p;
}

void CONFIG_FILE::OBJECT::WriteFile(CArchive& ar)
{
  CONFIG_ITEM_STATUS lineStatus;
  CString line;
  CString *pKey, *pValue;
  CString nKey, nValue;
  for (int i=0; i<m_numberOfItems; i++)
  {
    if (m_itemList[i]->m_toBeDeleted) continue; // Skip this line
    line="";
    if (m_toBeDeleted) 
    {
      line="//";  // Deleted objects are commented out
    };
    lineStatus=m_itemList[i]->m_status;
    if ((lineStatus==CONFIG_STAT_comment)  ||
        (lineStatus==CONFIG_STAT_syntax)   ||
        (lineStatus==CONFIG_STAT_delimiter)||
        (lineStatus==CONFIG_STAT_intError))
    {
      line+=m_itemList[i]->m_line;
    }
    else
    {
      pKey=&m_itemList[i]->m_key;
      pValue=&m_itemList[i]->m_value;
      if (pKey->FindOneOf("\\=")>=0)
      {
        nKey=*pKey;
        nKey.Replace("\\","\\\\");
        nKey.Replace("=","\\=");
        pKey=&nKey;
      };
      if (pValue->FindOneOf("\\=")>=0)
      {
        nValue=*pValue;
        nValue.Replace("\\","\\\\");
        nValue.Replace("=","\\=");
        pValue=&nValue;
      };
      line+=(*pKey) + " = " + (*pValue);
    };
    line += "\r\n";
    ar.WriteString(line);
  };
}

void CONFIG_FILE::OBJECT::MarkItemsToDelete(void)
{
  for (int i=0; i<m_numberOfItems; i++)
  {
    if ((*m_itemList[i]).m_status==CONFIG_STAT_comment) 
      (*m_itemList[i]).m_toBeDeleted=false;
    else if ((*m_itemList[i]).m_status==CONFIG_STAT_delimiter)
      (*m_itemList[i]).m_toBeDeleted=false;
    else (*m_itemList[i]).m_toBeDeleted=true;
  };
}

void CONFIG_FILE::OBJECT::RemoveDeletedLines(void) 
{
  for (int i=0; i<m_numberOfItems; i++)
  {
    if ((*m_itemList[i]).m_toBeDeleted)
    {
      delete m_itemList[i];
      m_numberOfItems--;
      for (int j=i; j<m_numberOfItems; j++)
      {
        m_itemList[j]=m_itemList[j+1];
      };
      i--;
    };
  };
}

int CONFIG_FILE::OBJECT::DeleteKeys(const CString& key)
{
  int result=-1;  // No deleted lines yet
  for (int i=0; i<m_numberOfItems; i++)
  {
    if ((*m_itemList[i]).m_status==CONFIG_STAT_delimiter)
      if ((*m_itemList[i]).m_line.Left(6) =="\\(END)")
        if (result==-1)
        {
          result=i;
          break;
        };
    if ((*m_itemList[i]).m_key.CompareNoCase(key)==0)
    {
      result=i;
      delete m_itemList[i];
      m_numberOfItems--;
      for (int j=i; j<m_numberOfItems; j++)
      {
        m_itemList[j]=m_itemList[j+1];
      };
      i--;
    };
  };
  return result;
}


void CONFIG_FILE::OBJECT::AddTail(LINE_ITEM& item) 
{
  m_numberOfItems++;
  if (m_numberOfItems>m_allocatedSize) 
  {
    m_allocatedSize=m_allocatedSize*3/2+5;
    m_itemList=(LINE_ITEM **)My_realloc(m_itemList,
      m_allocatedSize*sizeof (*m_itemList));
    if (m_itemList==NULL)
    {
      MsgBoxError("OBJECT memory allocation failure");
      _exit(1);
    };
  };
  LINE_ITEM *p = new LINE_ITEM(item);
  m_itemList[m_numberOfItems-1]=p;
}

CONFIG_FILE::LINE_ITEM *CONFIG_FILE::OBJECT::GetLine(int itemNum)
{
  if (itemNum>=m_numberOfItems) return NULL;
  return m_itemList[itemNum];
}




CONFIG_ITEM_STATUS GetStringUntilComma (
	int& column,
	const CString& value,
	CString& result,
	CString& errorMsg,
  BOOL preserveWhitespace=FALSE) // skips internal whitespace, not leading/trailing
{	
	// The string must not be all whitespace.
	int len;
	len=value.GetLength();
	result="";

  // if already pointing at comma, advance to next char
  if ((column < len) && (value[column] == ','))
    column++;

  // Skips over leading whitespace
	while (column < len)
	{
		if (whitespace(value[column]))
		{
			column++;
			continue;
		}
		break;
	}
	
  // We have found first character.
	if (column>=len)
	{
		errorMsg="Looking for string between commas.  Found only blanks";
		return CONFIG_STAT_value;
	}

  // nothing but whitespace between commas?
  if (value[column] == ',')
  { 
    // This is not an error, at least not for the text files
    // I am aware of. Spell scripts, being optional, might
    // have nothing between two commas on a line with other data.
    
		//errorMsg="Looking for string between commas.  Found only blanks";
		return CONFIG_STAT_ok;    
  }
  
  // collect chars in value until EOL or ',' found
	while ((column < len) && (value[column] != ','))
	{
    if (!preserveWhitespace)
    {
      if (!whitespace(value[column]))
		    result += value[column];
    }
    else
      result += value[column];
		column++;
	}

	return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_MonsterAttack(
		  const CString& value,
		  CString& errorMsg,
      MONSTER_ATTACK_DETAILS *data)
{
	CONFIG_ITEM_STATUS status;
	CString temp;
	int len;
	len=value.GetLength();
  data->Clear();
	if (len==0) return CONFIG_STAT_ok;

	// decode value of form "sides,nbr,bonus,message"
	int incol = 0;

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;
  data->sides = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;
  data->nbr = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;
  data->bonus = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg, TRUE); // preserve whitespace
	if (status != CONFIG_STAT_ok) return status;
  data->attackMsg = temp;

  status = GetStringUntilComma(incol,value, temp, errorMsg, TRUE); // preserve whitespace
	if (status == CONFIG_STAT_ok)
  {
    //data->spelldbkey = spellData.FindName(temp);
    if (temp == "No Spell") temp.Empty();
    data->spellID = temp;
  };

  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS CONFIG_DECODE_SpriteData (
		const CString& value,
		CString& errorMsg,
    CString &filename,
    int &nTimeDelay,
    int &nNumFrames,
    int &nFrameWidth,
    int &nFrameHeight,
		int &nStyle,
    DWORD &nMaxLoops,
    DWORD &nFlags,
    int   &nAllowCentering,
    int   &nUseAlpha,
    WORD  &nAlphaValue)
{
	CONFIG_ITEM_STATUS status;
	CString temp;
	int len;
	len=value.GetLength();

  filename="DEFAULT";
  nTimeDelay=0;
  nNumFrames=0;
  nFrameWidth=0;
  nFrameHeight=0;
  nStyle=0;
  nMaxLoops=1;
  nFlags=6;  // loop once

	if (len==0)
		return CONFIG_STAT_ok;

	// Expect value of form "filename,delay,frames,width,height,style,maxloops,flags,allowcentering,usealpha,alphavalue"
	int incol = 0;
  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  filename=temp;

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  nTimeDelay = atoi(temp);
  
  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  nNumFrames = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  nFrameWidth = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  nFrameHeight = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  nStyle = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok)
  {
    // could be an older version of the config file
    if (nNumFrames==0)
    {
      nMaxLoops=0;
      nFlags=0;
      nAllowCentering = 0;
      nUseAlpha = 0;
      nAlphaValue = 0;
    }
    return status;
  }

  nMaxLoops = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;
  nFlags = atoi(temp);


  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;
  nAllowCentering = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;
  nUseAlpha = atoi(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;
  nAlphaValue = atoi(temp);
  
  return CONFIG_STAT_ok;
}

static CONFIG_ITEM_STATUS decodeAbilityValue (
        int *col,
        const CString& value,
        short *rslt,
        short *modifier,
        CString *errorMsg)
{
  int integer;
  int len;
  len=value.GetLength();
  *modifier=0;
  if (GetInteger(*col, value, integer, *errorMsg) != CONFIG_STAT_ok)
  {
    *errorMsg="Expected an integer for ability value";
    return CONFIG_STAT_value;
  };
  *rslt=integer;
  if ((*col)>=len) return CONFIG_STAT_ok;
  if (value[*col]!='(') return CONFIG_STAT_ok;
  (*col)++;
  if (GetInteger(*col, value, integer, *errorMsg) != CONFIG_STAT_ok)
  {
    *errorMsg="Expected an integer ability modifier in parenthese";
    return CONFIG_STAT_value;
  };
  *modifier=integer;
  if (((*col)>=len) || (value[*col]!=')'))
  {
    *errorMsg="Expected close parenthesis after ability modifier";
    return CONFIG_STAT_value;
  };
  (*col)++;
  return CONFIG_STAT_ok;
}


CONFIG_ITEM_STATUS CONFIG_DECODE_AbilityRequirement (
  const CString& value,
  CString& errorMsg,
  ABILITY_REQ *abreq)
{
  CONFIG_ITEM_STATUS result=CONFIG_STAT_ok;
  CString token;
  int col=0;
  int len;
  len=value.GetLength();
  if (getToken(col, value, token)) // Get the ability name
  {
    //abreq->m_ability.m_abilityName=token;
    abreq->m_abilityID=token;
    result = decodeAbilityValue(&col, 
                                value, 
                                &abreq->m_min, 
                                &abreq->m_minMod, 
                                &errorMsg);
    if (result==CONFIG_STAT_ok)
    {
      if ( (col>=len) || (value[col]!='/') )
      {
        errorMsg="Expected '/' between min and max valuesFbase_class";
        result=CONFIG_STAT_value;
      } 
      else
      {
        col++;
        result = decodeAbilityValue(&col, 
                                    value, 
                                    &abreq->m_max, 
                                    &abreq->m_maxMod, 
                                    &errorMsg);
      };
    };
  }
  else 
  {
    errorMsg="Expected an ability name";
    result = CONFIG_STAT_value;
  };
  return result;
}

void CONFIG_ENCODE_integer (
      CString& result,
      int integer)
{
  result.Format("%d",integer);
}

void CONFIG_ENCODE_long (
      CString& result,
      long value)
{
  result.Format("%d",value);
}

void CONFIG_ENCODE_flags (
      CString& result,
      const CONFIGID *ids,
      unsigned int flags)
{
  result="";
  for (;ids->m_name!=""; ids++)
  {
    if ((ids->m_id.m_enumVal & flags)==
      (unsigned int)ids->m_id.m_enumVal)
    {
      if (result!="") result+=" + ";
      result+=ids->m_name;
    };
  };
}

void CONFIG_ENCODE_flags (
      CString& result,
      const CONFIGID *ids,
      long flags)
{
  CONFIG_ENCODE_flags(result,ids, (unsigned int)(flags));
}

void CONFIG_ENCODE_flags (
      CString& result,
      const CONFIGID *ids,
      unsigned char flags)
{
  CONFIG_ENCODE_flags(result,ids, (unsigned int)(flags));
}

void CONFIG_ENCODE_flags (
      CString& result,
      const CONFIGID *ids,
      unsigned long flags)
{
  CONFIG_ENCODE_flags(result,ids, (unsigned int)(flags));
}

void CONFIG_ENCODE_flags (
      CString& result,
      const CONFIGID *ids,
      unsigned short flags)
{
  CONFIG_ENCODE_flags(result,ids, (unsigned int)(flags));
}



void CONFIG_ENCODE_BOOL (
      CString& result,
      BOOL value)
{
  result=value?"yes":"no";
}

void CONFIG_ENCODE_float(
      CString& result,
      double value)
{
  result.Format("%f",value);
}

void CONFIG_ENCODE_dice (
      CString& result,
      int nDice,
      int nSides,
      int bonus,
      int levelBonus,
      int max)
{
  char str[80];
  if (bonus >= 0)
    sprintf(str,"%uD%u + %d",nDice,nSides,bonus);
  else
    sprintf(str,"%uD%u - %d",nDice,nSides,abs(bonus));

  if (levelBonus!=0x40000000)
    sprintf(str+strlen(str)," + %d*level",levelBonus);
  if (max!=0x40000000)
    sprintf(str+strlen(str)," max %d",max);
  result=str;
}


void CONFIG_ENCODE_dicePlus (
      CString& result,
      const DICEPLUS& dice)
{
  result = dice.m_Text;
  if (result.IsEmpty()) result = "0";
  /*
  CString temp;
  if ((dice.m_numSides==0)&&(dice.m_numDice==0))
  {
    // literal only, such as '10'
    if (dice.m_sign < 0)
      result.Format("-%i", dice.m_bonus);
    else
      result.Format("%i", dice.m_bonus);

    int num=dice.m_adjust.GetSize();
    if (num > 0)
    {
      result += dice.m_adjust[0].EncodeOperator(dice.m_adjust[0].m_operator[0]);

      // for things like 3*level    
      for (int i=1; i<num; i++)
      {
        ADJUSTMENT adj = dice.m_adjust[i];
        temp += adj.EncodeText();
        //temp += " ";
        result += temp;
      }

      // no need for min/max if just a literal used
      if ((dice.m_bonus!=0)||(num>0))
      {
        // for upper/lower limits
        if (dice.m_min != -999)
        {      
          temp.Format(" min %d",
                        dice.m_min);
          result += temp;
        }
        if ((dice.m_max != 999)&&(dice.m_max!=0))
        {      
          temp.Format(" max %d",
                        dice.m_max);
          result += temp;
        }
      }
    }
  }
  else
  {
    // 1d4
    if (dice.m_sign < 0)
    {
      result.Format("-%dd%d",
                dice.m_numDice,
                dice.m_numSides);                
    }
    else
    {
      result.Format("%dd%d",
                dice.m_numDice,
                dice.m_numSides);
    }

    int num=dice.m_adjust.GetSize();
    if (num > 0)
    {
      result += dice.m_adjust[0].EncodeOperator(dice.m_adjust[0].m_operator[0]);

      // for things like 3*level
      for (int i=1; i<num; i++)
      {
        ADJUSTMENT adj = dice.m_adjust[i];
        temp += adj.EncodeText();
        //temp += " ";
        result += temp;
      }
    }

    // for upper/lower limit
    if (dice.m_min != -999)
    {      
      temp.Format(" min %d",
                    dice.m_min);
      result += temp;
    }
    if ((dice.m_max != 999)&&(dice.m_max!=0))
    {      
      temp.Format(" max %d",
                    dice.m_max);
      result += temp;
    }
  }
  */
}


void CONFIG_ENCODE_file (
      CString& result,
      const CString& filename) 
{
  result=filename;
  result.Replace('\\','/');
}

void CONFIG_ENCODE_enum (
    CString& result,
    const CONFIGID *ids,
    int value)
{
  for (;ids->m_name!=""; ids++)
  {
    if (ids->m_id.m_enumVal==value)
    {
      result=ids->m_name;
      return;
    };
  };
  result.Format("unknown enumerated value = 0x%08x",value);
}

void CONFIG_ENCODE_MonsterAttack(
            CString &result,
            const MONSTER_ATTACK_DETAILS *data)
{
	// Produce value of form "sides,nbr,bonus,message,spellName"
  SPELL_ID spellID;
  //if (data->spelldbkey.IsValidSpell())
  if (data->spellID.IsValidSpell())
  {
    //spellName = spellData.GetSpellName(data->spelldbkey);
    spellID = spellData.GetSpellName(data->spellID);
  };
	result.Format("%d,%d,%d,%s,%s",
                data->sides,
                data->nbr,
                data->bonus,
                data->attackMsg,
                spellID);
}

void CONFIG_ENCODE_SpriteData (
		CString& result,
    const CString &filename,
    int &nTimeDelay,
    int &nNumFrames,
    int &nFrameWidth,
    int &nFrameHeight,
		int &nStyle,
    DWORD &nMaxLoops,
    DWORD &nFlags,
    int   &nAllowCentering,
    int   &nUseAlpha,
    WORD  &nAlphaValue)
{

	// //Produce value of form "filename,delay,frames,width,height,style"
	// Produce value of form "filename,delay,frames,width,height,style,maxLoops,flags,allowCentering,useAlpha,alphaValue"
	result.Format("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                    filename,
                    nTimeDelay,
                    nNumFrames,
                    nFrameWidth,
                    nFrameHeight,
                    nStyle,
                    nMaxLoops,
                    nFlags,
                    nAllowCentering,
                    nUseAlpha,
                    nAlphaValue);
}

void CONFIG_ENCODE_SpellEffect (
		CString& result,
    SPELL_EFFECTS_DATA *data)
{
  //  $CHAR_AC, 1d4+2, delta,   target,         yes
  //                   percent, alltargettypes, no
  //                            targeter

  CString units;
  CString targeting;
  CString cumulative;
  CString dplus;

  CONFIG_ENCODE_dicePlus(dplus, data->changeData);
  if (data->flags & SPELL_EFFECTS_DATA::EFFECT_TARGET)
  {
    if (data->flags & SPELL_EFFECTS_DATA::EFFECT_TARGETER)
      targeting="alltargettypes";
    else
      targeting="target";
  }
  else
  {
    if (data->flags & SPELL_EFFECTS_DATA::EFFECT_TARGETER)
      targeting="targeter";
    else // error
      targeting="target"; // use default
  }
  if (data->flags & SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE)
    cumulative="yes";
  else
    cumulative="no";

  if (data->flags & SPELL_EFFECTS_DATA::EFFECT_PCNT)
    units="percent";
  else if (data->flags & SPELL_EFFECTS_DATA::EFFECT_ABS)
    units="absolute";
  else if (data->flags & SPELL_EFFECTS_DATA::EFFECT_REMOVEALL)
    units="remove";
  else
    units="delta";

	// Produce value of form "$Attrib,diceplus,units,targeting,cumulative,script,script,script"
	result.Format("%s,%s,%s,%s,%s",
                data->AffectedAttr(),
                dplus,
                units,
                targeting,
                cumulative);

  //
  // these scripts are now put on a different line
  //

  /*
  if (data->activationScript!="")
  {
    CString tmp;
    tmp.Format(",%s", data->activationScript);
    tmp.Remove('\n');
    tmp.Remove('\r');
    tmp.Remove('\t');
    tmp.Remove('\v');
    result += tmp;
  }
  else
  {
    result += ",";
  }

  if (data->modificationScript!="")
  {
    CString tmp;
    tmp.Format(",%s", data->modificationScript);
    tmp.Remove('\n');
    tmp.Remove('\r');
    tmp.Remove('\t');
    tmp.Remove('\v');
    result += tmp;
  }
  if (data->savingThrowScript!="")
  {
    CString tmp;
    tmp.Format(",%s", data->savingThrowScript);
    tmp.Remove('\n');
    tmp.Remove('\r');
    tmp.Remove('\t');
    tmp.Remove('\v');
    result += tmp;
  }
  */
}
CONFIG_ITEM_STATUS CONFIG_DECODE_SpellEffect (
		const CString& value,
		CString& errorMsg,
    SPELL_EFFECTS_DATA *data)
{
	CONFIG_ITEM_STATUS status;
	CString temp;
	int len;
	len=value.GetLength();

	if (len==0)
	{
    data->Clear();
		return CONFIG_STAT_ok;
	}

	// Expect value of form 
  //
  //  $CHAR_AC, 1d4+2, delta,   target,         yes
  //                   percent, alltargettypes, no
  //                            targeter

	int incol = 0;
  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  data->AffectedAttr(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg,TRUE); // leave whitespace
	if (status != CONFIG_STAT_ok) return status;

  status = CONFIG_DECODE_dicePlus(temp, errorMsg, &data->changeData,0,0,0,0,0);
  if (status != CONFIG_STAT_ok) return status;
  
  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  if (temp.CompareNoCase("percent")==0)
    data->flags |= SPELL_EFFECTS_DATA::EFFECT_PCNT;
  else if (temp.CompareNoCase("absolute")==0)
    data->flags |= SPELL_EFFECTS_DATA::EFFECT_ABS;
  else if (temp.CompareNoCase("remove")==0)
    data->flags |= SPELL_EFFECTS_DATA::EFFECT_REMOVEALL;
  // else if (temp.CompareNoCase("delta")==0)
  //  -- not needed since delta is the default (no flags for it)

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  if (temp.CompareNoCase("target")==0)
    data->flags |= SPELL_EFFECTS_DATA::EFFECT_TARGET;
  else if (temp.CompareNoCase("targeter")==0)
    data->flags |= SPELL_EFFECTS_DATA::EFFECT_TARGETER;
  else if (temp.CompareNoCase("alltargettypes")==0)
    data->flags |= (SPELL_EFFECTS_DATA::EFFECT_TARGET|SPELL_EFFECTS_DATA::EFFECT_TARGETER);
  else
    data->flags |= SPELL_EFFECTS_DATA::EFFECT_TARGET; // default

  status = GetStringUntilComma(incol,value, temp, errorMsg);
	if (status != CONFIG_STAT_ok) return status;

  if (temp.CompareNoCase("yes")==0)
    data->flags |= SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE;

  // these scripts are now stored on a separate line, but we need to 
  // handle older files that still put them on this line
  status = GetStringUntilComma(incol,value, temp, errorMsg, TRUE);
	if (status != CONFIG_STAT_ok) 
  {
    // the scripts are optional
    return CONFIG_STAT_ok;
  }

  // these scripts should be empty if loading files using the current format,
  // but might not be if loading older format files.

  if (!temp.IsEmpty())
    data->ActivationScript(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg, TRUE);
  if (status != CONFIG_STAT_ok) 
  {
    // the scripts are optional
    return CONFIG_STAT_ok;
  }

  if (!temp.IsEmpty())
    data->ModificationScript(temp);

  status = GetStringUntilComma(incol,value, temp, errorMsg, TRUE);
  if (status != CONFIG_STAT_ok) 
  {
    // the scripts are optional
    return CONFIG_STAT_ok;
  }
  
  if (!temp.IsEmpty())
    data->SavingThrowScript(temp); 

  return CONFIG_STAT_ok;
}
