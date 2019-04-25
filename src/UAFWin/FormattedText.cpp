/******************************************************************************
* Filename: FormattedText.cpp
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

//#include "externs.h"
#include "Dungeon.h"
#include "Graphics.h"
#include "Party.h"
#include "Viewport.h"
#include "MouseThread.h"
#include "Path.h"
#include "DrawTile.h"

#include "FormattedText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// holds multi-line text data that will be displayed to 
// the user, with 'HIT RETURN' prompted after each
// box-full is displayed.
//
int TEXTBOX_LINES = 5;                            //DLD 12/20/00
int TEXTBOX_WIDTH = SCREEN_WIDTH - (textboxX*2);
int TEXTBOX_HEIGHT = 5 * 16;

int JOURNAL_TEXTBOX_LINES = 20;
TEXT_DISPLAY_DATA textData;
TEXT_DISPLAY_DATA errorTextData;
TEXT_DISPLAY_DATA pausedTextData;
TEXT_DISPLAY_DATA journalTextData;
TEXT_DISPLAY_DATA combatTextData;

static BOOL ftHighlightOn = FALSE;
static BOOL ftWhiteOn = FALSE;
static BOOL ftYellowOn = FALSE;
static BOOL ftRedOn = FALSE;
static BOOL ftGreenOn = FALSE;
static BOOL ftBlueOn = FALSE;
static BOOL ftCustom1On = FALSE;


//*****************************************************************************
//    NAME: BOOL IsWhiteSpace
//
// PURPOSE: 
//
//*****************************************************************************
static inline BOOL IsWhiteSpace(const char *Char)
{
   if (Char[0] == ' ')  return true;
   if (Char[0] == '\n') return true;
   if (Char[0] == '\r') return true;
   if (Char[0] == '\t') return true;
   if ( (Char[0] == '/') && (Char[1] == 'n') ) return true;
   return false;
}

//*****************************************************************************
//    NAME: BOOL IsNewLine
//
// PURPOSE: 
//
//*****************************************************************************
static inline BOOL IsNewLine(const char *Char)
{
   if ( (Char[0] == '\n') || (Char[0] == '\r') ) return true;
   if ( (Char[0] == '/' ) && (Char[1] == 'n' ) ) return true;
   return false;
}

//*****************************************************************************
//    NAME: ResetFontColorTags
//
// PURPOSE: 
//
//*****************************************************************************
static void ResetFontColorTags(void)
{
  ftWhiteOn = FALSE;
  ftYellowOn = FALSE;
  ftRedOn = FALSE;
  ftGreenOn = FALSE;
  ftBlueOn = FALSE;
  ftCustom1On=FALSE;
  ftHighlightOn = FALSE;
}

//*****************************************************************************
//    NAME: HighlightText
//
// PURPOSE: 
//
//*****************************************************************************
static void HighlightText(BOOL enable)
{
  ftHighlightOn = enable;

  ftYellowOn = FALSE;
  ftWhiteOn = FALSE;
  ftRedOn = FALSE;
  ftGreenOn = FALSE;
  ftBlueOn = FALSE;
  ftCustom1On=FALSE;
}

//*****************************************************************************
//    NAME: WhiteText
//
// PURPOSE: 
//
//*****************************************************************************
static void WhiteText(BOOL enable)
{
  ftWhiteOn = enable;

  ftHighlightOn = FALSE;
  ftYellowOn = FALSE;
  ftRedOn = FALSE;
  ftGreenOn = FALSE;
  ftBlueOn = FALSE;
  ftCustom1On=FALSE;
}
//*****************************************************************************
//    NAME: YellowText
//
// PURPOSE: 
//
//*****************************************************************************
static void YellowText(BOOL enable)
{
  ftYellowOn = enable;

  ftHighlightOn = FALSE;
  ftWhiteOn = FALSE;
  ftRedOn = FALSE;
  ftGreenOn = FALSE;
  ftBlueOn = FALSE;
  ftCustom1On=FALSE;
}
//*****************************************************************************
//    NAME: RedText
//
// PURPOSE: 
//
//*****************************************************************************
static void RedText(BOOL enable)
{
  ftRedOn = enable;

  ftHighlightOn = FALSE;
  ftWhiteOn = FALSE;
  ftYellowOn = FALSE;
  ftGreenOn = FALSE;
  ftBlueOn = FALSE;
  ftCustom1On=FALSE;
}
//*****************************************************************************
//    NAME: GreenText
//
// PURPOSE: 
//
//*****************************************************************************
static void GreenText(BOOL enable)
{
  ftGreenOn = enable;

  ftHighlightOn = FALSE;
  ftWhiteOn = FALSE;
  ftYellowOn = FALSE;
  ftRedOn = FALSE;
  ftBlueOn = FALSE;
  ftCustom1On=FALSE;
}
//*****************************************************************************
//    NAME: BlueText
//
// PURPOSE: 
//
//*****************************************************************************
static void BlueText(BOOL enable)
{
  ftBlueOn = enable;

  ftHighlightOn = FALSE;
  ftWhiteOn = FALSE;
  ftYellowOn = FALSE;
  ftRedOn = FALSE;
  ftGreenOn = FALSE;
  ftCustom1On=FALSE;
}
//*****************************************************************************
//    NAME: Custom1Text
//
// PURPOSE: 
//
//*****************************************************************************
static void Custom1Text(BOOL enable)
{
  ftCustom1On = enable;

  ftHighlightOn = FALSE;
  ftWhiteOn = FALSE;
  ftYellowOn = FALSE;
  ftRedOn = FALSE;
  ftGreenOn = FALSE;
  ftBlueOn=FALSE;
}

//*****************************************************************************
//    NAME: CheckColorTag
//
// PURPOSE: 
//
//*****************************************************************************
static void CheckColorTag(const char *pText)
{
  if (pText==NULL) return;

  if (*pText != '/') return;
  pText++;
  if (*pText=='\0') return;
  char color = tolower(*pText);
  switch (color)
  {
  case 'h': HighlightText(!ftHighlightOn); break;
  case 'w': WhiteText(!ftWhiteOn); break;
  case 'y': YellowText(!ftYellowOn); break;
  case 'r': RedText(!ftRedOn); break;
  case 'g': GreenText(!ftGreenOn); break;
  case 'b': BlueText(!ftBlueOn); break;
  case 'c': Custom1Text(!ftCustom1On);break;
  default: return;
  }
}
//*****************************************************************************
//    NAME: ScanForColorTags
//
// PURPOSE: 
//
//*****************************************************************************
static void ScanForColorTags(const char *pText)
{
  while ((pText!=NULL) && (*pText!='\0'))
  {
    CheckColorTag(pText);
    pText++;
  }
}

//*****************************************************************************
//    NAME: CheckJournalColorTag
//
// PURPOSE: 
//
//*****************************************************************************
static BOOL CheckJournalColorTag(const char *pText)
{
  if (pText==NULL) return FALSE;  
  //CheckColorTag(pText);
  return (*pText == '\b');
}

//*****************************************************************************
//    NAME: CurrentColor
//
// PURPOSE: 
//
//*****************************************************************************
static CString CurrentColor()
{
  if (ftHighlightOn) return "/h";
  if (ftWhiteOn) return "/w";
  if (ftYellowOn) return "/y";
  if (ftRedOn) return "/r";
  if (ftGreenOn) return "/g";
  if (ftBlueOn) return "/b";
  if (ftCustom1On) return "/c";
  return "";
}

//*****************************************************************************
//    NAME: NeedEndingColorTag
//
// PURPOSE: 
//
//*****************************************************************************
static BOOL NeedEndingColorTag(CString &str, CString &curr)
{
  bool found=false;
  char *pChar = str.GetBuffer(2);
  int i=str.GetLength()-2;
  while ((i>=0)&&(!found))
  {
    if (strncmp(&pChar[i],curr,2)==0)
      found=true;
    else if (pChar[i]!=' ')
      break;
    i--;
  }
  str.ReleaseBuffer();
  return (!found);
}
//*****************************************************************************
//    NAME: NeedStartingColorTag
//
// PURPOSE: 
//
//*****************************************************************************
static BOOL NeedStartingColorTag(CString &str, CString &curr)
{
  bool found=false;
  char *pChar = str.GetBuffer(2);
  int i=0;
  while ((i<str.GetLength()-1)&&(!found))
  {
    if (strncmp(&pChar[i],curr,2)==0)
      found=true;
    else if (pChar[i]!=' ')
      break;
    i++;
  }
  str.ReleaseBuffer();
  return (!found);
}

//*****************************************************************************
//    NAME: AddColorTags
//
// PURPOSE: 
//
//*****************************************************************************
static CString AddColorTags(CString &str)
{
  CString curr = CurrentColor();
  if (curr=="") return "";
  if (str.GetLength()==0) return curr;  
  //if (NeedEndingColorTag(str, curr))
  //  str += curr;
  return curr;
}

//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::TEXT_DISPLAY_DATA
//
// PURPOSE: 
//
//*****************************************************************************
TEXT_DISPLAY_DATA::TEXT_DISPLAY_DATA() 
{ 
 highlightAll=FALSE;
 slowText=FALSE;
 InitialBoxDisplay=TRUE;
 numLines=0;
 currLine=0;
 startTime=0;
 pauseText=FALSE;
 text.RemoveAll();
}

//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::NeedsFrontBuffer
//
// PURPOSE: 
//
//*****************************************************************************
BOOL TEXT_DISPLAY_DATA::NeedsFrontBuffer()
{
  return ( (UseSlowText()) || (pauseText) );
}

//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::UseSlowText
//
// PURPOSE: 
//
//*****************************************************************************
BOOL TEXT_DISPLAY_DATA::UseSlowText()
{
  return ((InitialBoxDisplay) && (slowText));
}


//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::IsFirstBox
//
// PURPOSE: 
//
//*****************************************************************************
BOOL TEXT_DISPLAY_DATA::IsFirstBox()
{
  return (currLine == 0);
}


BOOL TEXT_DISPLAY_DATA::WaitForReturn()
{
  int i;
  for (i=0; i<TEXTBOX_LINES; i++)
  {
    if (currLine+i >= numLines) break;
    if (PeekTextLine(currLine+i)->waitForReturn) return true;
  };
  return false;
}
//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::IsLastBox
//
// PURPOSE: 
//
//*****************************************************************************
BOOL TEXT_DISPLAY_DATA::IsLastBox()
{
  int i;
  for (i=0; i<TEXTBOX_LINES; i++)
  {
    if (currLine+i+1 >= numLines) return true;
    if (PeekTextLine(currLine+i)->waitForReturn) return false;
  };
  return true;
}
//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::NextBox
//
// PURPOSE: 
//
//*****************************************************************************
void TEXT_DISPLAY_DATA::NextBox()
{
  int i;
  for (i=0; i<TEXTBOX_LINES; i++)
  {
    if (currLine+i > numLines) 
    {
      currLine = numLines;
      break;
    };
    if (PeekTextLine(currLine+i)->waitForReturn)
    {
      currLine = currLine+i+1;
      if (currLine > numLines)
      {
        currLine = numLines;
      };
      break;
    };
  };
  InitialBoxDisplay = TRUE;
}
//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::PrevBox
//
// PURPOSE: 
//
//*****************************************************************************
void TEXT_DISPLAY_DATA::PrevBox()
{
  int i;
  if (currLine > 0)
  {
    currLine -= 2;
    for (i=2; i<TEXTBOX_LINES; i++)
    {
      if (currLine < 0) break;
      if (PeekTextLine(currLine)->waitForReturn)
      {
        currLine = currLine+1;
        break;
      }
      else
      {
        currLine--;
      };
    };
    if (currLine < 0) currLine = 0;
    InitialBoxDisplay = TRUE;
  }
}


//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::IsFirstJournalBox
//
// PURPOSE: 
//
//*****************************************************************************
BOOL TEXT_DISPLAY_DATA::IsFirstJournalBox()
{
  return (currLine == 0);
}
//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::IsLastJournalBox
//
// PURPOSE: 
//
//*****************************************************************************
BOOL TEXT_DISPLAY_DATA::IsLastJournalBox()
{
  return (currLine+JOURNAL_TEXTBOX_LINES >= numLines);
}
//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::PrevJournalBox
//
// PURPOSE: 
//
//*****************************************************************************
void TEXT_DISPLAY_DATA::PrevJournalBox()
{
  if (currLine > 0)
  {
    currLine -= JOURNAL_TEXTBOX_LINES;
    if (currLine < 0) currLine = 0;
    InitialBoxDisplay = TRUE;
  }
}

//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::NextJournalBox
//
// PURPOSE: 
//
//*****************************************************************************
void TEXT_DISPLAY_DATA::NextJournalBox()
{
  if (currLine < numLines)
  {
    currLine += JOURNAL_TEXTBOX_LINES;
    if (currLine > numLines) currLine = numLines;
    InitialBoxDisplay = TRUE;
  }
}
//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::FirstJournalBox
//
// PURPOSE: 
//
//*****************************************************************************
void TEXT_DISPLAY_DATA::FirstJournalBox()
{
  currLine = 0;
}
//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::LastJournalBox
//
// PURPOSE: 
//
//*****************************************************************************
void TEXT_DISPLAY_DATA::LastJournalBox()
{
  currLine = numLines - JOURNAL_TEXTBOX_LINES;

  if (currLine < 0) currLine = 0;
  if (currLine >= numLines) currLine = numLines-JOURNAL_TEXTBOX_LINES;
}


//*****************************************************************************
//    NAME: TEXT_DISPLAY_DATA::Quit
//
// PURPOSE: 
//
//*****************************************************************************
void TEXT_DISPLAY_DATA::Quit()
{
  while (currLine < numLines)
    currLine += TEXTBOX_LINES;

  currLine -= TEXTBOX_LINES;
  if (currLine < 0) currLine = 0;
  if (currLine >= numLines) currLine = numLines-TEXTBOX_LINES;
}

// width of textbox area in pixels
//*****************************************************************************
//    NAME: GetTextBoxCharWidth
//
// PURPOSE: 
//
//*****************************************************************************
unsigned int GetTextBoxCharWidth(void)
{  
  unsigned int width = 0;

  int maxCharWidth = GraphicsMgr.GetMaxCharacterWidth();
  ASSERT( maxCharWidth > 0 );

  //TEXTBOX_WIDTH = SCREEN_WIDTH - (textboxX*2); // actual width (used elsewhere)
  width = TEXTBOX_WIDTH - (maxCharWidth/2); // usable width for text
  ASSERT( width > 0 );
  return width;
}

// height of textbox area in pixels
//*****************************************************************************
//    NAME: GetTextBoxCharHeight
//
// PURPOSE: 
//
//*****************************************************************************
unsigned int GetTextBoxCharHeight(void)
{  
  //return (HorzBarBotY - textboxY - GraphicsMgr.GetMaxCharacterHeight() - 16);
  unsigned int height = 0;

  int maxCharHeight = GraphicsMgr.GetMaxCharacterHeight();
  ASSERT( maxCharHeight > 0 );
  
  height = TEXTBOX_HEIGHT;// - maxCharHeight; 
  TEXTBOX_LINES = height / maxCharHeight;

  return (height);
}

//*****************************************************************************
//    NAME: ClearFormattedTextScreenArea
//
// PURPOSE: 
//
//*****************************************************************************
void ClearFormattedTextScreenArea(BOOL front)
{
  RECT rect;
  rect.left = textboxX;
  rect.top = textboxY;
  rect.right = textboxX+GetTextBoxCharWidth();
  rect.bottom = textboxY+GetTextBoxCharHeight();

  if (front)
    GraphicsMgr.ClearSurface(FRONTBUFFERNUM, BACKGROUND_FILL, &rect);
  else
    GraphicsMgr.ClearSurface(BACKBUFFERNUM, BACKGROUND_FILL, &rect);
}

//*****************************************************************************
//    NAME: ClearFormattedText
//
// PURPOSE: 
//
//*****************************************************************************
void ClearFormattedText(TEXT_DISPLAY_DATA &data)
{   
   GraphicsMgr.HighlightText(FALSE);
   GraphicsMgr.ResetFontColorTags();
   GraphicsMgr.EnableFontColorTags(FALSE);
   data.InitialBoxDisplay = TRUE;
   data.slowText = TRUE;
   data.numLines = 0;
   data.currLine = 0;
   data.pauseText = FALSE;
   //data.text.RemoveAll();
   data.RemoveAll();
}

//*****************************************************************************
//    NAME: TextIsMultiBox
//
// PURPOSE: 
//
//*****************************************************************************
BOOL TextIsMultiBox(TEXT_DISPLAY_DATA& data)
{
  int i;
  if (data.numLines > TEXTBOX_LINES) return true;
  for (i=0; i<data.numLines-1; i++)
  {
    if (data.PeekTextLine(i)->waitForReturn) return true;
  };
  return false;
}

//*****************************************************************************
//    NAME: StripInvalidChars
//
// PURPOSE: 
//
//*****************************************************************************
void StripInvalidChars(char *pIn)
{
  char *pChar=pIn;
  while ((pChar != NULL) && (*pChar != '\0'))
  {
    if ((*pChar < 0) || (*pChar > 127))
      *pChar = ' ';
    pChar++;
  }   
}

//*****************************************************************************
//    NAME: NumDigits
//
// PURPOSE: 
//
//*****************************************************************************
int NumDigits(int val)
{
  int sign = (val < 0);
  val = abs(val);

  if (val < 10) return (1+sign);
  if (val < 100) return (2+sign);
  if (val < 1000) return (3+sign);
  if (val < 10000) return (4+sign);
  if (val < 100000) return (5+sign);
  if (val < 1000000) return (6+sign);
  if (val < 10000000) return (7+sign);
  if (val < 100000000) return (8+sign);
  if (val < 1000000000) return (9+sign);
  return (10+sign);
}

// strips illegal ascii from text, and performs party
// name substitution into text.
//
// ^ = active party member name
// ^1..^8 = name of party member 1..8
// ^D = current game day
// ^H = current game hour
// ^M = current game minute
// ^a - ^z = temp ASL named 'a' - 'z'
//
//*****************************************************************************
//    NAME: PreProcessText
//
// PURPOSE: 
//
//*****************************************************************************
void PreProcessText(const char *pIn, char **pNew)
{
  CString out;
   const char TokenStartMarker = '^';
   if (pIn == NULL)
     return;
  if (pNew == NULL)
     return;

  //int TokenCount=0;
  //int SubstituteTextLength=0;
   const char *pChar=pIn;
/*
   while ((pChar != NULL) && (*pChar != '\0'))
   {
      if (*pChar == TokenStartMarker)
      {
        int partyOrdinal;
        partyOrdinal = 0;
        TokenCount++;
        // check to see if we have ^ tokens, and figure
        // out the required length of the new string
        if (   (pChar != NULL)
            && (*(pChar+1) != '\0'))
        {
          if ( (*(pChar+1) >= '1') && (*(pChar+1) <= '8') )
          {
            pChar++; // inc past ^
            char tmp[2];
            tmp[0] = *pChar;
            tmp[1] = '\0';
            int dude = atoi(tmp);
            dude -= 1; // zero-based in party data
            if ((dude < 0) || (dude >= party.numCharacters))
              dude = party.numCharacters-1;
            SubstituteTextLength += party.characters[dude].GetName().GetLength();
          }
          else if (*(pChar+1) == 'D')
          {
            pChar++; // inc past ^
            SubstituteTextLength += NumDigits(party.days);
          }
          else if (*(pChar+1) == 'H')
          {
            pChar++; // inc past ^
            SubstituteTextLength += 2; 
          }
          else if (*(pChar+1) == 'M')
          {
            pChar++; // inc past ^
            SubstituteTextLength += 2; 
          }
          else if ( (*(pChar+1) >= 'a') && (*(pChar+1) <= 'z') )
          {
            pChar++; // inc past ^
            CString key = *pChar;
            CString val = globalData.temp_asl.Lookup((LPCSTR)key);
            SubstituteTextLength += val.GetLength();
          }
          else // just regular ^
            SubstituteTextLength += party.getActiveChar().GetName().GetLength();
        }
        else // ^ is last in string
          SubstituteTextLength += party.getActiveChar().GetName().GetLength();
      }
      pChar++;
   }   
   if (TokenCount == 0)
   {    
     *pOut = new char[strlen(pIn)+1];
     strcpy(*pOut, pIn);
     StripInvalidChars(*pOut);
     return;
   }
*/

   // create new string with appropriate length

   pChar = pIn;
   out = CString(' ', strlen(pIn) + 200); 
   // Make some room available so we don't alloc new buffer for each character.
   out = "";
   // copy old string to new string
   while (*pChar != '\0')
   {
      if (*pChar == TokenStartMarker)
      {
        // check to see if we have ^ and substitute replacement
        // text for ^ token
        if (pChar[1] != '\0')
        {
          int dude = -1;
          if ( (pChar[1]=='1') && (pChar[2]>='0') && (pChar[2]<='2') )
        {
            dude = 10 + pChar[2] - '0' - 1;
            pChar += 2;
          }
          else if ( (pChar[1] >= '1') && (pChar[1] <= '8') )
          {
            dude = pChar[1] - '0' - 1;
            pChar++;
          }
          else if (pChar[1] == 'D')
          {
            pChar++; // inc past ^
            CString num;
            num.Format("%i", party.days);
            out += num;            
          }
          else if (pChar[1] == 'H')
          {
            pChar++; // inc past ^
            CString num;
            num.Format("%02i", party.hours);
            out += num;            
          }
          else if (pChar[1] == 'M')
          {
            pChar++; // inc past ^
            CString num;
            num.Format("%02i", party.minutes);
            out +=num;            
          }
          else if ( (*(pChar+1) >= 'a') && (*(pChar+1) <= 'z') )
          {
            pChar++; // inc past ^
            CString key = *pChar;
            CString val = globalData.global_asl.Lookup((LPCSTR)key);
            out += val; 
          }
          else // just regular ^
            out += party.getActiveChar().GetName();

          if (dude >= party.numCharacters)
          {
             dude = party.numCharacters-1; 
          };
          if (dude >= 0)
          {      
            out += party.characters[dude].GetName();            
          };
        }
        else // ^ is last in string
          out += party.getActiveChar().GetName();        
      }
      else
        out += pChar[0];

      pChar++;
   }
  //*pOut = new char[strlen(pIn) + SubstituteTextLength + 1];
  *pNew = new char[out.GetLength() + 1];
  memcpy(*pNew, (LPCSTR)out, out.GetLength());
  (*pNew)[out.GetLength()] = 0;
   // remove unprintable characters
  StripInvalidChars(*pNew);
}

//*****************************************************************************
//    NAME: PostProcessText
//
// PURPOSE: 
//
//*****************************************************************************
void PostProcessText(TEXT_DISPLAY_DATA &data)
{
  // not likely to be a character set that is less than
  // 5 pixels in width on average
  WORD MaxOneLineWidthInChars = SCREEN_WIDTH/5;
  char ch[2];
  ch[1] = '\0';

  char *temp;
  temp = new char[MaxOneLineWidthInChars+1];

	for (int i=0;i<data.numLines;i++)
	{
    temp[0] = '\0';

    //int max = data.text[i].GetLength();
    int max = data.PeekTextLine(i)->text.GetLength();
    max = min( max, MaxOneLineWidthInChars );
    for (int l=0; l < max; l++)
    {
      if (   (data.PeekTextLine(i)->text[l] != '\n')
          && (data.PeekTextLine(i)->text[l] != '\r')
          && (data.PeekTextLine(i)->text[l] != '\b')
          && (data.PeekTextLine(i)->text[l] != '\t'))
      {
        ch[0] = data.PeekTextLine(i)->text[l];
        strncat(temp, ch, 1);
      }
    }

    //data.text[i] = temp;
    data.GetTextLine(i)->text = temp;
	}

  if (temp != NULL)
    delete [] temp;
}

// keep it short, you only get 1 box of text when using paused text
//*****************************************************************************
//    NAME: FormatPausedText
//
// PURPOSE: 
//
//*****************************************************************************
void FormatPausedText(TEXT_DISPLAY_DATA &data, const char *pText)
{
  ResetFontColorTags();
  ClearFormattedText(data);
  
  if (pText == NULL)
    return;
  if (strlen(pText) == 0)
    return;
  
  char *buffer=NULL;
  PreProcessText(pText, &buffer);
  
  data.slowText = FALSE;
  data.highlightAll = FALSE;
  data.pauseText = TRUE;
  
  int char_pixels_per_line = GetTextBoxCharWidth();
  if (char_pixels_per_line <= 0)
    return;
  
  BOOL done = FALSE;
  const char *start = buffer;
  const char *end = buffer;
  int currLineWidth = 0;
  
  while ((!done) && (start != NULL) && (*start != '\0'))
  {
    currLineWidth = 0;
    
    while ( (!done) && (currLineWidth < char_pixels_per_line) && (!IsNewLine(end)))
    {
      currLineWidth += GraphicsMgr.GetTextWidth(end, 1);
      end++;      
      if (*end == '\0') done = TRUE;
    }
    
    CString NewString("");        

    if (done)
    {
       NewString+=start;
    }
    else
    {
       CString tmpNewString("");
       const char *temp = end;

       while ((!IsWhiteSpace(temp)) && (temp != start))
          temp--;

       if (temp != start)
       {              
          tmpNewString.Format("%.*s", temp-start, start);
          end = temp;
       }
       else
       {
         tmpNewString.Format("%.*s", end-start, start);
       }
       NewString += tmpNewString;
    }

    // text wrapped to next line doesn't need to start with a space
    if (NewString[0]==' ') NewString.Delete(0);
    data.Add(NewString);
    
    if (!done)
    {
      while ( (*end == '\r') || (*end == '\n') )
        end++;
    }
    
    data.numLines++;
    start = end;
  }
  
  if (buffer != NULL)
    delete [] buffer;
  
  PostProcessText(data);
  data.startTime = virtualGameTime;
}

//*****************************************************************************
//    NAME: FormatJournalText
//
// PURPOSE: 
//
//*****************************************************************************
void FormatJournalText(TEXT_DISPLAY_DATA &data, JOURNAL_DATA &jdata)
{
   ResetFontColorTags();
   ClearFormattedText(data);
   if (jdata.GetCount() <= 0) return;

   CString text="";
   int count=0;
   POSITION pos = jdata.jlist.GetHeadPosition();
   while (pos != NULL)
   {
     if (jdata.jlist.PeekAtPos(pos).text.GetLength() > 0)
     {
       count++;
       text += jdata.jlist.PeekAtPos(pos).text;
       if (count < jdata.GetCount()) 
         text += "\b\n\n";
     }
     jdata.jlist.GetNext(pos);
   }

   if (text.GetLength() <= 0) return;

   char *pText = new char[text.GetLength()+1];
   strcpy(pText, text);
   pText[text.GetLength()]='\0';

    data.slowText = FALSE;
    data.highlightAll = FALSE;
    int char_pixels_per_line = GetTextBoxCharWidth();
    if (char_pixels_per_line <= 0)
      return;

    BOOL done = FALSE;
    const char *start = pText;
    const char *end = pText;
    int currLineWidth = 0;
    CString CurrColor("");
    BOOL ResetColor=FALSE;

    while ((!done) && (start != NULL) && (*start != '\0'))
    {
       ResetColor=FALSE;
       currLineWidth = 0;

        while ( (!done) && (currLineWidth < char_pixels_per_line) && (!IsNewLine(end)))
        {
          currLineWidth += GraphicsMgr.GetTextWidth(end, 1);
          ResetColor = CheckJournalColorTag(end);
          end++;
          if (*end == '\0') done = TRUE;
        }

        CString NewString("");

        if (done)
        {
          NewString+=start;
        }
        else
        {
           CString tmpNewString("");
           const char *temp = end;

           while ((!IsWhiteSpace(temp)) && (temp != start))
              temp--;

           if (temp != start)
           {
              tmpNewString.Format("%.*s", temp-start, start);
              end = temp;
           }
           else
           {
              tmpNewString.Format("%.*s", end-start, start);
           }
           NewString += tmpNewString;
        }

        // text wrapped to next line doesn't need to start with a space
        if (NewString[0]==' ') NewString.Delete(0);

        ScanForColorTags(NewString);

        if ((CurrColor!="")&&(NeedStartingColorTag(NewString, CurrColor)))
          NewString = CurrColor+NewString;

        CurrColor=AddColorTags(NewString); 
        if (NewString[0]==' ') NewString.Delete(0);
        //data.text.Add(NewString);
        data.Add(NewString);

        if (ResetColor)
        {
          CurrColor="";
          ResetFontColorTags();
        }

        if (!done)
        {
          while ( (*end == '\r') || (*end == '\n') )
          {
            //data.text.Add(" ");
            data.Add(" ");
            data.numLines++;

            char curr = *end;
            end++;
            
            // edit boxes use "\r\n" to skip lines
            // but only need to add one blank line to
            // represent that sequence
            if ( (*end == '\r') || (*end == '\n') )
            {
              if (curr != *end)
                end++;
            }
          }
        }

        data.numLines++;
        start = end;
    }

   PostProcessText(data);
   data.startTime = virtualGameTime;
   if (pText != NULL)
     delete [] pText;
}

//*****************************************************************************
//    NAME: FormatDisplayText
//
// PURPOSE: 
//
//*****************************************************************************
void FormatDisplayText(TEXT_DISPLAY_DATA &data,
                       const char *pText, 
                       BOOL slowText, 
                       BOOL HighlightAll,
                       BOOL requireEnter)
{ 
   ResetFontColorTags();
   ClearFormattedText(data);

   data.eventRequiresReturn = requireEnter;

   if (pText == NULL)
      return;
   if (strlen(pText) == 0)
     return;

   char *buffer=NULL;
   PreProcessText(pText, &buffer); // Handle special character '^'

   if (INTER_CHARACTER_DELAY > 0)
     data.slowText = slowText;
   else
     data.slowText = FALSE;

    data.highlightAll = HighlightAll;
    int char_pixels_per_line = GetTextBoxCharWidth(); //DLD 12/20/00 now a changable var //unsigned int char_pixels_per_line = GetTextBoxCharWidth();
    if (char_pixels_per_line <= 0)
      return;

    BOOL done = FALSE;
    const char *start = buffer;
    const char *end = buffer;
    int currLineWidth = 0;
    int waitForReturn;
    CString CurrColor("");

    while ((!done) && (start != NULL) && (*start != '\0'))
    {
       currLineWidth = 0;
       waitForReturn = FALSE;

        while ( (!done) && (currLineWidth < char_pixels_per_line) && (!IsNewLine(end)))
        {
          currLineWidth += GraphicsMgr.GetTextWidth(end, 1);
          end++;
          if (*end == '\0') done = TRUE;
        }

        CString NewString("");        

        if (done)
        {
           NewString+=start;
        }
        else
        {
           CString tmpNewString("");
           const char *temp = end;


           while ((!IsWhiteSpace(temp)) && (temp != start))
              temp--;

           if (temp != start)
           {              
              tmpNewString.Format("%.*s", temp-start, start);
              end = temp;
           }
           else
           {
             tmpNewString.Format("%.*s", end-start, start);
           }
           NewString += tmpNewString;
        }

        // text wrapped to next line doesn't need to start with a space
        if (NewString[0]==' ') NewString.Delete(0);

        ScanForColorTags(NewString);  // Process special character '/'

        if ((CurrColor!="")&&(NeedStartingColorTag(NewString, CurrColor)))
          NewString = CurrColor+NewString;

        CurrColor=AddColorTags(NewString);
        if (NewString[0]==' ') NewString.Delete(0);
        //data.text.Add(NewString);
        data.Add(NewString);
        if ( (end[0] == '/') && (end[1] == 'n') )
        {
          data.GetTextLine(data.GetTextLineCount()-1)->waitForReturn = TRUE;
          end = end+2;
        }

        if (!done)
        {
          //while ( (*end == '\r') || (*end == '\n') )
          //  end++;
          {
            while ( (end[0] == '\r') || (end[0] == '\n') )
            {            
              //data.text.Add(" ");
              data.Add(" ");
              data.numLines++;

              char curr = *end;
              end++;
            
              // edit boxes use "\r\n" to skip lines
              // but only need to add one blank line to
              // represent that sequence
              if ( (*end == '\r') || (*end == '\n') )
              {
                if (curr != *end)
                  end++;
              }
            }
          };
        }

        data.numLines++;
        start = end;
    }

   if (buffer != NULL)
     delete [] buffer;

   PostProcessText(data);
   data.startTime = virtualGameTime;
}

//*****************************************************************************
//    NAME: FormatCombatText
//
// PURPOSE: 
//
//*****************************************************************************
void FormatCombatText(TEXT_DISPLAY_DATA &data, const char *pText)
{
   ResetFontColorTags();
   ClearFormattedText(data);

   if (pText == NULL) return;
   if (strlen(pText) == 0) return;

   data.slowText = FALSE;
   data.highlightAll = FALSE;
   
   int char_pixels_per_line = COMBAT_STAT_BOX_WIDTH - 
                              //GraphicsMgr.GetMaxCharacterWidth() - 
                              (VertBarSrc.right-VertBarSrc.left) - 16;
    if (char_pixels_per_line <= 0)
    {
      char_pixels_per_line = 240;
    };
    BOOL done = FALSE;
    const char *start = pText;
    const char *end = pText;
    int currLineWidth = 0;

    while ((!done) && (start != NULL) && (*start != '\0'))
    {
       currLineWidth = 0;

        while ( (!done) && (currLineWidth < char_pixels_per_line) && (*end != '\n') && (*end != '\r'))
        {
          currLineWidth += GraphicsMgr.GetTextWidth(end, 1);
          end++;
          if (*end == '\0') done = TRUE;
        }

        CString NewString("");        

        if (done)
        {
           NewString+=start;
        }
        else
        {
           CString tmpNewString("");
           const char *temp = end;

           while ((*temp != ' ') && (*temp != '\n') && (*temp != '\r') && (temp != start))
              temp--;

           if (temp != start)
           {             
              tmpNewString.Format("%.*s", temp-start, start);
              end = temp;
           }
           else
           {
             tmpNewString.Format("%.*s", end-start, start);
           }
           NewString += tmpNewString;
        }

        if (NewString[0]==' ') NewString.Delete(0);
        //data.text.Add(NewString);
        data.Add(NewString);

        if (!done)
        {
          while ( (*end == '\r') || (*end == '\n') )
            end++;
        }

        data.numLines++;
        start = end;
    }
}

// keep it short, you only get 1 box of text when using paused text
//*****************************************************************************
//    NAME: DisplayPausedText
//
// PURPOSE: 
//
//*****************************************************************************
void DisplayPausedText(TEXT_DISPLAY_DATA &data, COLORREF color)
{
  if (data.numLines <= 0) return;

  int x = textboxX;
  int y = textboxY + (GetTextBoxCharHeight()/2); // middle of box height
  int curr = data.currLine;
  int last = curr + TEXTBOX_LINES;
  if (last > data.numLines)
    last = data.numLines;
  BOOL useFrontbuffer = data.NeedsFrontBuffer();
  BOOL useSlowText = data.UseSlowText();

  if (useFrontbuffer) MouseRender.ShowMouseCursor(FALSE);

  // clear text area on screen
  ClearFormattedTextScreenArea(useFrontbuffer);

  GraphicsMgr.ResetFontColorTags();
  //GraphicsMgr.EnableFontColorTags(TRUE);

  while (curr < last)
  {
    // to allow for slow text, formatted text is drawn directly to the front
    // buffer, so this function must be called after the back buffer is flipped
    //displayText(x,y, data.text[curr], color, data.highlightAll, useSlowText, !useFrontbuffer);
    displayText(x,y, data.PeekTextLine(curr)->text, color, data.highlightAll, useSlowText, !useFrontbuffer);
    y += 18;
    curr++;
  }

  if (useFrontbuffer) MouseRender.ShowMouseCursor(TRUE);

  //GraphicsMgr.EnableFontColorTags(FALSE);
  data.InitialBoxDisplay = FALSE;
  theApp.AppSleep(GetMsgTimeDelay());
}

//*****************************************************************************
//    NAME: DisplayFormattedText
//
// PURPOSE: 
//
//*****************************************************************************
void DisplayFormattedText(TEXT_DISPLAY_DATA &data, COLORREF color)
{
   if (data.numLines <= 0) return;
   int x = textboxX;
   int y = textboxY;

   int curr = data.currLine;
   int last = curr + TEXTBOX_LINES;

   if (last > data.numLines)
      last = data.numLines;

   BOOL useFrontbuffer = data.NeedsFrontBuffer();
   BOOL useSlowText = data.UseSlowText();

   if (useFrontbuffer) MouseRender.ShowMouseCursor(FALSE);

   GraphicsMgr.ResetFontColorTags();
   GraphicsMgr.EnableFontColorTags(TRUE);
   while (curr < last)
   {
      // to allow for slow text, formatted text is drawn directly to the front
      // buffer, so this function must be called after the back buffer is flipped
      if (data.PeekTextLine(curr)->waitForReturn)
      {
        last = curr+1;
      };
      GraphicsMgr.ResetFontColorTags();
      //displayText(x,y, data.text[curr], color, data.highlightAll, useSlowText, !useFrontbuffer);
      displayText(x,y, data.PeekTextLine(curr)->text, color, data.highlightAll, useSlowText, !useFrontbuffer);
      //y += 18;
      y += GraphicsMgr.GetMaxCharacterHeight();
      curr++;
   }

   GraphicsMgr.EnableFontColorTags(FALSE);
   if (useFrontbuffer) MouseRender.ShowMouseCursor(TRUE);

   data.InitialBoxDisplay = FALSE;
}

//*****************************************************************************
//    NAME: DisplayJournalText
//
// PURPOSE: 
//
//*****************************************************************************
void DisplayJournalText(TEXT_DISPLAY_DATA &data, COLORREF color)
{  
   if (data.numLines <= 0) return;

   int x = 18;
   int y = 38;

   int curr = data.currLine;
   int last = curr + JOURNAL_TEXTBOX_LINES;

   if (last > data.numLines)
      last = data.numLines;

   BOOL useFrontbuffer = data.NeedsFrontBuffer();
   BOOL useSlowText = data.UseSlowText();

   if (useFrontbuffer) MouseRender.ShowMouseCursor(FALSE);
   
   GraphicsMgr.EnableFontColorTags(TRUE);

   while (curr < last)
   {
      // to allow for slow text, formatted text is drawn directly to the front
      // buffer, so this function must be called after the back buffer is flipped
      GraphicsMgr.ResetFontColorTags();
      //displayText(x,y, data.text[curr], color, data.highlightAll, useSlowText, !useFrontbuffer);
      displayText(x,y, data.PeekTextLine(curr)->text, color, data.highlightAll, useSlowText, !useFrontbuffer);
      y += 18;
      curr++;
   }

   GraphicsMgr.EnableFontColorTags(FALSE);

   if (useFrontbuffer) MouseRender.ShowMouseCursor(TRUE);

   data.InitialBoxDisplay = FALSE;
}
