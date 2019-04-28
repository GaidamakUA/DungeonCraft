/******************************************************************************
* Filename: GPDLexec.h
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

#ifndef __GPDLEXEC_H__
#define __GPDLEXEC_H__

// 19 Apr PRS #include "GPDLCode.h"
#include "regexp.h"

#define DOWN_ARROW      0x28
#define UP_ARROW        0x26
#define CARRIAGE_RETURN 0x0d
#define BACKSPACE       0x08
#define SPACE           0x20


enum AURA_FUNC
{
  $AURA_Create,
  $AURA_Destroy,
  $AURA_AddSA,
  $AURA_GetSA,
  $AURA_RemoveSA,
  $AURA_Attach,
  $AURA_Location,
  $AURA_Combatant,
  $AURA_Size,
  $AURA_Shape,
  $AURA_Spell,
  $AURA_Wavelength,


  $AURA_ATTACH_Combatant,
  $AURA_ATTACH_CombatantFacing,
  $AURA_ATTACH_None,

};


class GPDL_EVENT;
class GPDLCOMP;

enum GPDL_STATE { // Also used for event return values.

	// Internal states.  These are the only possible states that
	//  will be returned by State().  
	GPDL_UNINITIALIZED=1,
	GPDL_IDLE,
	GPDL_WAIT_INPUT,
	GPDL_WAIT_ACK,

	// Return status values
	GPDL_OK,
	GPDL_ACCEPTED,
	GPDL_IGNORED,
	GPDL_NOSUCHNAME,
	GPDL_EVENT_ERROR,
	GPDL_READ_ERROR,
	GPDL_OVER_RP,
	GPDL_UNDER_RP,
	GPDL_OVER_SP,
	GPDL_UNDER_SP,
	GPDL_GREPERROR,
	GPDL_ILLPARAM,
	GPDL_BADINTEGER,
	GPDL_ILLFUNC,     // Illegal cell at entry to function
	GPDL_ILLCHARNUM,
  GPDL_EXCESSCPU

};


class GLOBAL_ITEM_ID;
class ITEM_DATA;
struct SPELL_EFFECTS_DATA;

class GPDL {
#define DBG_messageBox	  1
#define DBG_functionTrace 2
	class ERRORLOG {
	private:
		FILE   *m_file;
		bool	m_messagePrinted;
	public:
		ERRORLOG(void);
		~ERRORLOG(void);
		void log(CString& msg);
		void log(const char *msg);
	};
  class GLOBALS {
  private:
	  CString			*m_value;
	  unsigned int	 m_allocatedSize;
  public:
	  GLOBALS (void);
	  ~GLOBALS(void);
	  int read(CArchive& ar);
#ifdef UAFEDITOR
    int read(GPDLCOMP *pGPDL);
#endif
	  CString &peek(unsigned int index) {return m_value[index];};
    void Set(unsigned int index, const CString value){m_value[index] = value;};
  };
  class INDEX {
  private:
	  CString			*m_name;
	  unsigned int	*m_value;
	  unsigned int	 m_allocatedSize;
  public:
	  INDEX (void);
	  ~INDEX(void);
	  int read(CArchive& ar);
#ifdef UAFEDITOR
    int read(GPDLCOMP *pGPDL);
#endif
	  unsigned int numEntry(void) { return m_allocatedSize;};
	  CString &name(unsigned int index) {return m_name[index];};
	  unsigned int lookup(const CString& funcName);
  };
  class DISCOURSETEXT {
  private:
	  class LINE {
	  public:
		  CString		m_text;
		  LINE	   *m_nextLine;  // or NULL
		  LINE	   *m_prevLine;  // Or NULL
		  LINE(const char *line);
		  ~LINE(void);
		  void		deleteLastChar(unsigned int backspaceLimit);
		  void		addOneChar(UINT c);
	  };
	  //unsigned int	m_MAXLINES; // Set in constructor
	  unsigned int	m_MAXCHARS; // Set in constructor
	  unsigned int	m_promptLen;  // To avoid backspacing over prompt
	  bool			m_listening;
	  LINE		   *m_firstLine;
	  unsigned int	m_numLine;
	  unsigned int	m_topLine;
	  void			m_format(CString& text, unsigned int width, bool colorTags=true);
	  void			m_addLine(const char *line);
  public:
	  DISCOURSETEXT(void);
	  ~DISCOURSETEXT(void);
	  void			Display(void);
	  void			SetText(CString& text, unsigned int width);
	  void			SetNewTop(void); // Set top to text that follows.
	  void			StartInput(void);
	  void			downArrow(void);
	  void			upArrow(void);
	  GPDL_STATE		Char(UINT c); // Return GPDL_ACCEPTED if character accepted
	  void			Clear(void);
	  CString			GetInput(void); // Get result of input operation;
  };
private:
  unsigned int *m_program;
	GLOBALS	  	  m_globals;
	INDEX			    m_index;
	unsigned int	m_PC;
	unsigned int	m_FP;
	unsigned int	m_SP;
	unsigned int	m_RP;
	unsigned int	m_RP0;
	unsigned int	m_SP0;
  unsigned int  m_H;
  unsigned int  m_programAllocated; // Allocated for program
  unsigned int  m_executionAddress; // latest Opcode fetch address
	GPDL_STATE		m_state;
	GPDL_STATE		m_status;
	unsigned int	m_debugLevel;
  unsigned int  m_interpretCount;
	//unsigned int	m_char_pixels_per_line; // Initialized in constructor and
											// modified by $TEXTWIDTH(n);
  GPDL_EVENT   *m_pGPDLevent;
	ERRORLOG	  	m_errorLog;
	GPDL_STATE		m_interpret(void);
  GPDL_STATE    ReadProgram(CArchive& ar);
  void          SA_Name(void);
  void          SA_Param(const SPECIAL_ABILITIES *pSA);
  void          SA_Param(void);
  void          SA_Delete(SPECIAL_ABILITIES *pSA);
  void          SA_SetParam(SPECIAL_ABILITIES *pSA);
  void          AURA_FUNCTION(AURA_FUNC func);
#ifdef UAFEDITOR
  GPDL_STATE    ReadProgram(GPDLCOMP *pGPDL);
#endif
  unsigned int Peek(unsigned int address)
  {
    ASSERT(address < m_H);
    return m_program[address];
  };
	void			m_pushRP(unsigned int n);
	unsigned int	m_popRP(void);
	void			m_pushSP(const CString& val);
	CString			m_popSP (void);
	void			m_popPC(void);
	void			m_pushPC(void);
	void			m_popFP(void);
	void			m_pushFP(void);
	void			m_pushString1(void);
	void			m_popString1(void);
	void			m_pushString2(void);
	void			m_popString2(void);  
	void			m_pushString3(void);
  void      m_popString3(void);
	void			m_pushString4(void);
  void      m_popString4(void);
  void      m_popString5(void);
  void      m_popString6(void);
  void      m_pushEmptyString(void);
	GPDL_STATE		m_popInteger(bool& neg, unsigned int& n);
  int       m_popInteger(void);
	void			m_popInteger1();
	void			m_popInteger2();
	void			m_popInteger3();
	void			m_popInteger4();
	void			m_popInteger5();
	void			m_pushInteger1(void);
	void			m_pushInteger2(void);
	void			m_pushInteger3(void);
	void			m_pushUInteger1(void);
	void			m_popUInteger1();
	void			m_pushUInteger2(void);
	void			m_popUInteger2();
  void      m_popInts(int n);
	void			m_IllegalStateMessageAndQuit(void);
	void			m_interpretError(const char *msg);
	void			m_error_message(char *msg);
//  void      m_getCharacterValueWithActor(int keyindex, ActorType actor);
//  void      m_setCharacterValueWithActor(int keyIndex, ActorType actor, const CString& value);
//  void      m_getCharacterValue(int keyindex);
//  void      m_setCharacterValue(int keyindex);
//  void      m_getPartyValue(int keyindex);
//  void      m_setPartyValue(int keyindex);
//  void      m_getGameValue(int keyindex);
//  void      m_setGameValue(int keyindex);

  CHARACTER *Dude(void);

  void      m_GetLiteralInt(int v);
  void      m_GetCharInt(int (CHARACTER::*f)() const);
  void      m_GetCharInt(int (CHARACTER::*f)(DWORD) const);
  void      m_GetCharInt(genderType (CHARACTER::*f)(DWORD) const);
  void      m_GetCharString(const CString& (CHARACTER::*f)() const);
  void      m_GetActorBOOL(BOOL (CHARACTER::*f)() const);
  void      m_GetCharInt(BYTE (CHARACTER::*f)(DWORD) const);
  void      m_GetCharInt(BYTE (CHARACTER::*f)() const);
  void      m_GetCharInt(int (CHARACTER::*f)(const ITEM_ID&, int, DWORD) const);

  void      m_SetCharInt(void (CHARACTER::*f)(int));
  void      m_SetCharInt(void (CHARACTER::*f)(int,int,bool));
  void      m_SetLiteralInt(int *v);
  void      m_SetCharString(void (CHARACTER::*f)(const CString&));
  void      m_SetCharBOOL(void (CHARACTER::*f)(BOOL));
  void      m_SetActorBOOL(void (CHARACTER::*f)(BOOL b));

  void      m_GetCharActor(ActorType *pActor);
  

  //
  // Spell and AI script functions
  //

  // convert between ActorType and string
  ActorType  m_StringToActor(const char *data);
  CString    m_ActorToString(const ActorType *data);
  //CString    m_giIDToString(GLOBAL_ITEM_ID& giID);
  CString    m_giIDToString(const ITEM_ID& itemID);
  //CString    m_gsIDToString(GLOBAL_SPELL_ID& gsID);
  CString    m_gsIDToString(const SPELL_ID& itemID);
  //GLOBAL_SPELL_ID m_StringTogsID(const CString& str);
  CString m_StringTogsID(const CString& str);
  //ITEM_DATA *m_StringToItemData(const CString& str);
  const ITEM_DATA *m_GetItemData(void);

  // Convert Index values to data pointers
  COMBATANT *m_IndexToCombatant(int index);
  CHARACTER *m_IndexToCharacter(int index);
  CHARACTER *m_IndexedCharacter(int index);
  COMBATANT *m_popCombatant(void)        {return m_IndexToCombatant(m_popInteger());};
  CHARACTER *m_popCharacter(void)        {return m_IndexToCharacter(m_popInteger());};
  //CHARACTER *m_popIndexedCharacter(void) {return m_IndexedCharacter(m_popInteger());};

  CHARACTER *m_popCharacterActor(void);
  COMBATANT *m_popCombatantActor(void);

  // returns an ActorType in string format
  CString m_Myself(void);    // based on current character ActorType
  CString m_Name(const char *name); // search based on name that is passed in
  CString m_Target(void);    // based on current target ActorType
  void    m_Attacker(void);  // from current ATTACKER_CONTEXT
  void    m_Combatant(void); // from current COMBATANT_CONTEXT
  CString m_Item(void);      // from current ITEM_CONTEXT
  CString m_Spell(void);     // From current SPELL_CONTEXT
  SPELL_ID m_Spell(const CString& string);

  // returns a string representing the requested value
  // input parameter is a string containing an ActorType
  CString m_Gender(CString &data);
  CString m_Class(CString &data); 
  CString m_Race(CString &data);
  CString m_Status(CString &data);
  CString m_Alignment(CString &data);
  CString m_HitPoints(CString &data);

  // returns "1" or "0" (true or false)
  // input parameter is a string containing an ActorType
  CString m_AlignmentGood(CString &data);
  CString m_AlignmentEvil(CString &data);
  CString m_AlignmentLawful(CString &data);
  CString m_AlignmentNeutral(CString &data);
  CString m_AlignmentChaotic(CString &data);  
  CString m_InParty(CString &data);
  CString m_IsUndead(CString &data);
  CString m_IndexOf(const CString &data);
  CString m_IsCharAffectedBySpell(const CString &ActorString, const SPELL_ID &SpellID);
  CString m_IsCharAffectedBySpellAttribute(const CString &ActorString, const CString &SpellAttribName);

	unsigned int   *m_returnStack;
//	void			m_attributeInsert(ATTRIBUTES *atr, 
//							   const CString& key,
//						       const CString& val);
	CString        *m_dataStack;
	CString		    	m_listenText;
	DISCOURSETEXT   m_discourseText;
	//
	// Some temporary variables for the interpreter
	CString		    	m_false;
	CString			    m_true;
	unsigned int    m_bincode;   // The 32-bit code at m_PC
	unsigned int    m_opcode;    // The top byte of m_bincode;
	unsigned int    m_subop;     // The bottom 24 bits of m_bincode;
	CString		    	m_string1;
	CString	    		m_string2;
  CString         m_string3;
  CString         m_string4;
  CString         m_string5;
  CString         m_string6;
	int			      	m_Integer1;
	int				      m_Integer2;
	int				      m_Integer3;
	int				      m_Integer4;
	int				      m_Integer5;
	unsigned int    m_uInteger1;
	unsigned int	  m_uInteger2;
  int             m_intArray[5];
	bool			      m_bool1;
	GPDL_STATE		  m_interpStatus;
	re_pattern_buffer m_grepPat;
	const char	   *m_grepCResult;
  double          m_IntermediateResult; // holds results between scripts
  SPELL_EFFECTS_DATA *pTimedSA;

public:
	GPDL(void);
	~GPDL(void);
  CString interpretError;
	GPDL_STATE State(void);
	GPDL_STATE Load(CArchive& ar);
#ifdef UAFEDITOR
  GPDL_STATE Load(GPDLCOMP *pGPDL);
#endif
	unsigned int numIndex(void); // Number of entries in index
	CString indexName(unsigned int i); // Name of ith function in index.
  int GetFunctionEntryPoint(const CString& name); //-1 if interpreter not initialized
                                                  // 0 if entry point not found
	bool IsActive(void);
	GPDL_STATE BreakExecute(void);
	GPDL_STATE BeginExecute(CString& funcName, GPDL_EVENT *event);
  CString    BeginExecute(int entryPoint);
  CString    ExecuteScript(const CString& code, int entryPointOrdinal);
  CString    ExecuteScript(const CString& code, int entryPointOrdinal,CString *par, int numPar);
	GPDL_STATE Display(void);  // If nothing to display...no harm.
	GPDL_STATE Char(UINT c); // Returns GPDL_IGNORED if keystroke ignored.
  void SetIntermediateResult(double val) { m_IntermediateResult = val; }
  double GetIntermediateResult(void) const { return m_IntermediateResult; }
};

const int GPDL_STACKSIZE = 4;
void die(CString errorMsg);


class GPDL_STACK
{
  GPDL m_stack[GPDL_STACKSIZE];
  int  m_activeGPDL;
public:
  GPDL_STACK(void)
  {
    m_activeGPDL = 0;
  };
  void Push(void)
  {
    m_activeGPDL++;
    if (m_activeGPDL >= GPDL_STACKSIZE)
    {
      die("GPDL recursion limit");
    };
  };
  void Pop(void)
  {
    if (m_activeGPDL == -1)
    {
      die("GPDL recursion underflow");
    };
    m_activeGPDL--;
  };
  GPDL *activeGPDL(void)
  {
    return m_stack+m_activeGPDL;
  };
};

extern GPDL_STACK gpdlStack;

extern CArray<CString, CString> discourseLine;
//extern CString discourseLine[5]; //DLD 12/20/00 //extern CString discourseLine[TEXTBOX_LINES]; 



int LongCompare(const CString& x, const CString& y);          //ARITHMETIC
CString LongAdd(const CString& x, const CString& y);          //ARITHMETIC
CString LongSubtract(const CString& x, const CString& y);     //ARITHMETIC
CString LongMultiply(const CString& s1, const CString& s2);   //ARITHMETIC
void LongDivide(const CString& dividend,                      //ARITHMETIC
                const CString& divisor,                       //ARITHMETIC
                CString& quotient,                            //ARITHMETIC
                CString& remainder);                          //ARITHMETIC
int CleanNumber(const CString& num, char *digits);            //ARITHMETIC


class RDREXEC                                                           //RDR
{                                                                       //RDR
  char *m_code;                                                         //RDR
  int   m_getInteger(const char **str);                                 //RDR
  int   m_getChar(const char **str);                                    //RDR
public:                                                                 //RDR
  RDREXEC(void);                                                        //RDR
  ~RDREXEC(void);                                                       //RDR
  int InterpretExpression(const CString& expr,                          //RDR
                          int (*interpretRDR)(int *),                   //RDR
                          int (*interpretRDRB)(const char *array));     //RDR
};                                                                      //RDR

#endif
