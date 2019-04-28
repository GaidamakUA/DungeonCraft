/******************************************************************************
* Filename: FormattedText.h
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
#ifndef __FORMATTED_TEXT_H__
#define __FORMATTED_TEXT_H__

#include "globaldata.h"

// num pixels across box where text msgs are displayed
extern int TEXTBOX_WIDTH;
extern int TEXTBOX_HEIGHT;

// max lines of text that will be displayed at same time
extern int TEXTBOX_LINES; //DLD 12/20/00 //const int TEXTBOX_LINES = 5;  


//const int MAX_TEXT_LINES = 20;

struct TEXT_LINE
{
  TEXT_LINE(const CString&str){text=str; waitForReturn = FALSE;};
  TEXT_LINE(void){waitForReturn=FALSE;};
  CString text;
  BOOL    waitForReturn;
};

struct TEXT_DISPLAY_DATA 
{
   TEXT_DISPLAY_DATA();
   ~TEXT_DISPLAY_DATA() { text.RemoveAll(); }
   
   BOOL NeedsFrontBuffer();
   BOOL UseSlowText();
   void Quit(void);
   void Add(const CString& str){TEXT_LINE t(str); text.Add(t);};
   void RemoveAll(void){text.RemoveAll();};
   
   BOOL IsFirstBox(void);
   BOOL IsLastBox(void);   
   void PrevBox(void);
   void NextBox(void);
   BOOL WaitForReturn(void);
   
   // uses different max display line count
   BOOL IsFirstJournalBox(void);
   BOOL IsLastJournalBox(void);
   void PrevJournalBox(void);
   void NextJournalBox(void);
   void FirstJournalBox(void);
   void LastJournalBox(void);
   
   BOOL highlightAll; // inverse text colors?
   BOOL slowText; // pause after each letter is displayed?
   BOOL InitialBoxDisplay; // true when box full of text is displayed for first time
   BOOL pauseText;

   int numLines;
   int currLine;
   DWORD startTime;
   BOOL eventRequiresReturn;
private:
   mCArray<TEXT_LINE, TEXT_LINE&> text;
public:
   DEFINE_mCARRAY_ACCESS_FUNCTIONS(TextLine,                 /* array data name      */ \
                                   CString,                  /* Search variable type */ \
                                   txt,                      /* search variable name */ \
                                   text,                     /* array name           */ \
                                   TEXT_LINE,                /* array data type      */ \
                                   TEXT_DISPLAY_DATA)        /* type of *this        */

};

void FormatDisplayText(TEXT_DISPLAY_DATA &data, 
                       const char *pText, 
                       BOOL slowText=TRUE, 
                       BOOL HighlightAll=FALSE,
                       BOOL requireEnter=FALSE);
void FormatPausedText(TEXT_DISPLAY_DATA &data, const char *pText);

void DisplayFormattedText(TEXT_DISPLAY_DATA &data, COLORREF color=White);
void DisplayPausedText(TEXT_DISPLAY_DATA &data, COLORREF color=White);

void FormatJournalText(TEXT_DISPLAY_DATA &data, JOURNAL_DATA &jdata);
void DisplayJournalText(TEXT_DISPLAY_DATA &data, COLORREF color=White);

void FormatCombatText(TEXT_DISPLAY_DATA &data, const char *pText);

void ClearFormattedText(TEXT_DISPLAY_DATA &data);
BOOL TextIsMultiBox(TEXT_DISPLAY_DATA& data);

unsigned int GetTextBoxCharWidth();
// height of textbox area in pixels
unsigned int GetTextBoxCharHeight(void);

void PreProcessText(const char *pIn, char **pOut);
int NumDigits(int val);
void StripInvalidChars(char *pIn);

extern TEXT_DISPLAY_DATA textData;
extern TEXT_DISPLAY_DATA errorTextData;
extern TEXT_DISPLAY_DATA pausedTextData;
extern TEXT_DISPLAY_DATA journalTextData;
extern TEXT_DISPLAY_DATA combatTextData;

struct DisplayStatusPkt
{
  CString **m_pStrings;
  int      m_numString;
  DisplayStatusPkt(void){m_numString=0; m_pStrings=NULL;};
  ~DisplayStatusPkt(void);
  void AddString(const CString& str);
};



#endif
