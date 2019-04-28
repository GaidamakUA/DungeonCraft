// Note......No precompiled headers!

#include <afxtempl.h>
#include <malloc.h>

#include "../Shared/GPDLOpCodes.h"

void WriteDebugString(const char *ptext, ... );
extern int debugSeverity;

void *My_malloc(int size);
void *My_realloc(void *addr, int size);
void My_free(void *addr);

#define MAX_FUNC_PARAMETERS 7


// **********************************************************************
//
// INPUTFILE   INPUTFILE   INPUTFILE   INPUTFILE   INPUTFILE   INPUTFILE   
                                                              //INPUTFILE
enum TOKENTYPE {TKN_NONE=0,                                   //INPUTFILE
        TKN_NAME,                                             //INPUTFILE
        TKN_PLUS,                                             //INPUTFILE
        TKN_MINUS,                                            //INPUTFILE
        TKN_OPENBRACE,                                        //INPUTFILE
        TKN_CLOSEBRACE,                                       //INPUTFILE
        TKN_INTEGER,                                          //INPUTFILE
        TKN_OPENPAREN,                                        //INPUTFILE
        TKN_CLOSEPAREN,                                       //INPUTFILE
        TKN_COLON,                                            //INPUTFILE
        TKN_SEMICOLON,                                        //INPUTFILE
        TKN_GEAR,                                             //INPUTFILE
        TKN_SLASH,                                            //INPUTFILE
        TKN_DOUBLESLASH,                                      //INPUTFILE
        TKN_STRING,                                           //INPUTFILE
        TKN_COMMA,                                            //INPUTFILE
        TKN_EQUAL,                                            //INPUTFILE
        TKN_POUND,                                            //INPUTFILE
        TKN_OPENBRACKET,                                      //INPUTFILE
        TKN_CLOSEBRACKET,                                     //INPUTFILE
        TKN_LESS,                                             //INPUTFILE
        TKN_nLESS,                                            //INPUTFILE
        TKN_GREATER,                                          //INPUTFILE
        TKN_nPLUS,                                            //INPUTFILE
        TKN_nMINUS,                                           //INPUTFILE
        TKN_nGEAR,                                            //INPUTFILE
        TKN_ISEQUAL,                                          //INPUTFILE
        TKN_NOTEQUAL,                                         //INPUTFILE
        TKN_nISEQUAL,                                         //INPUTFILE
        TKN_nNOTEQUAL,                                        //INPUTFILE
        TKN_LOR,                                              //INPUTFILE
        TKN_LAND,                                             //INPUTFILE
        TKN_nOR,                                              //INPUTFILE
        TKN_nXOR,                                             //INPUTFILE
        TKN_nAND,                                             //INPUTFILE
        TKN_LESSEQUAL,                                        //INPUTFILE
        TKN_nLESSEQUAL,                                       //INPUTFILE
        TKN_nGREATER,                                         //INPUTFILE
        TKN_GREATEREQUAL,                                     //INPUTFILE
        TKN_nGREATEREQUAL,                                    //INPUTFILE
        TKN_nSLASH,                                           //INPUTFILE
        TKN_nPERCENT,                                         //INPUTFILE
        TKN_PERCENT,                                          //INPUTFILE
        TKN_NOT,                                              //INPUTFILE
        TKN_PRAGMA,   // like #PUBLIC                         //INPUTFILE
        TKN_nEQUAL    // =#                                   //INPUTFILE
       };                                                     //INPUTFILE
                                                              //INPUTFILE
class INPUTFILE {                                             //INPUTFILE
private:                                                      //INPUTFILE
  int     m_MAXLINELEN;                                       //INPUTFILE
  CFile   *m_file;                                            //INPUTFILE
  int    (*m_ProcessErrorMessage)(const CString& errMsg       //INPUTFILE
                                             ,bool wait);     //INPUTFILE
  const char  *(*m_lineFetcher)(int n);                       //INPUTFILE
  const char *m_line;                                         //INPUTFILE
  int         m_lineIndex;                                    //INPUTFILE
  int         m_lineNumber;                                   //INPUTFILE
  int         m_errorCount;                                   //INPUTFILE
  char    m_prevChar;                                         //INPUTFILE
  char    m_savedChar;                                        //INPUTFILE
  int     m_eof;                                              //INPUTFILE
  bool    m_skipLine;  // Skip to end of line.                //INPUTFILE
  int     m_integer;                                          //INPUTFILE
  INPUTFILE  *m_prevFile;                                     //INPUTFILE
  CString     m_token;                                        //INPUTFILE
  TOKENTYPE   m_backspaceTkn;                                 //INPUTFILE
  TOKENTYPE m_latestTkn;                                      //INPUTFILE
  void        m_pop(void);                                    //INPUTFILE
  void        m_push();                                       //INPUTFILE
  char    m_nextChar(void);                                   //INPUTFILE
  void    m_convertDecimal(char c);                           //INPUTFILE
  void        m_convertHex(void);                             //INPUTFILE
  TOKENTYPE   m_getRawToken(void);                            //INPUTFILE
  TOKENTYPE m_getString(void);                                //INPUTFILE
  void        m_backspace(void);                              //INPUTFILE
public:                                                       //INPUTFILE
  INPUTFILE(void);                                            //INPUTFILE
  ~INPUTFILE(void);                                           //INPUTFILE
  int Open(char *filename);                                   //INPUTFILE
  int Initialize(                                             //INPUTFILE
        const char *GetOneLine(int n),                        //INPUTFILE
        int ProcessErrorMessage(const CString &errMsg, bool wait));//INPUTFILE
  TOKENTYPE NextToken(void);                                  //INPUTFILE
  CString token(void) {return m_token;};                      //INPUTFILE
  int integer(void) {return m_integer;};                      //INPUTFILE
  void backspaceToken(void); // Backspace one token.          //INPUTFILE
  void error(char *txt, bool printLineNumbers=true)           //INPUTFILE
      {error(CString(txt),printLineNumbers);};                //INPUTFILE
  void error(CString txt, bool printLineNumbers=true);        //INPUTFILE
};                                                            //INPUTFILE
                                                              //INPUTFILE
int INPUTFILE::Initialize(                                    //INPUTFILE
        const char *GetOneLine(int n),                        //INPUTFILE
        int ProcessErrorMessage(const CString &errMsg, bool wait))//INPUTFILE
{                                                             //INPUTFILE
  m_MAXLINELEN=80;                                            //INPUTFILE
  m_file=NULL;                                                //INPUTFILE
  m_lineNumber = 1;                                           //INPUTFILE
  m_lineIndex = 0;                                            //INPUTFILE
  m_lineFetcher = GetOneLine;                                 //INPUTFILE
  m_ProcessErrorMessage = ProcessErrorMessage;                //INPUTFILE
  m_prevFile=NULL;                                            //INPUTFILE
  m_backspaceTkn=TKN_NONE;                                    //INPUTFILE
  m_latestTkn=TKN_NONE;                                       //INPUTFILE
  m_eof=0;                                                    //INPUTFILE
  m_prevChar=0;                                               //INPUTFILE
  m_savedChar=0;                                              //INPUTFILE
  m_errorCount = 0;                                           //INPUTFILE
  //m_line=(char *) malloc(m_MAXLINELEN+1);                   //INPUTFILE
  //if (m_line==NULL) {                                       //INPUTFILE
  //  error("Cannot allocate input file buffer");             //INPUTFILE
  //  Sleep(3000);                                            //INPUTFILE
  //  exit(1);                                                //INPUTFILE
  //};                                                        //INPUTFILE
  m_line="";                                                  //INPUTFILE
  return 0;                                                   //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
                                                              //INPUTFILE
INPUTFILE::INPUTFILE(void)                                    //INPUTFILE
{                                                             //INPUTFILE
  m_file=NULL;                                                //INPUTFILE
  m_lineFetcher = NULL;                                       //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
INPUTFILE::~INPUTFILE(void) {                                 //INPUTFILE
  do {                                                        //INPUTFILE
    if (m_file!=NULL) {                                       //INPUTFILE
      m_file->Close();                                        //INPUTFILE
      delete m_file;                                          //INPUTFILE
      m_file=NULL;                                            //INPUTFILE
    };                                                        //INPUTFILE
    if (m_prevFile!=NULL) m_pop();                            //INPUTFILE
  } while (m_file!=NULL);                                     //INPUTFILE
  if ((m_line!=NULL) && (m_lineFetcher==NULL))                //INPUTFILE
  {                                                           //INPUTFILE
    My_free((char *)m_line);                                  //INPUTFILE
  };                                                          //INPUTFILE
  m_line=NULL;                                                //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
int INPUTFILE::Open(char *filename) {                         //INPUTFILE
  if (m_file!=NULL) {                                         //INPUTFILE
    error("Internal error - SOURCEFILE already open");        //INPUTFILE
    Sleep(3000);                                              //INPUTFILE
    _exit(1);                                                 //INPUTFILE
  };                                                          //INPUTFILE
  m_file=new CFile();                                         //INPUTFILE
  if (m_file->Open(filename,CFile::modeRead)==0) {            //INPUTFILE
    delete m_file;                                            //INPUTFILE
    m_file=NULL;                                              //INPUTFILE
    return 0;                                                 //INPUTFILE
  };                                                          //INPUTFILE
  m_lineNumber=1;                                             //INPUTFILE
//  m_filename=filename;                                      //INPUTFILE
  m_lineIndex=0;                                              //INPUTFILE
//  m_line="";  // We are at end of line zero.                //INPUTFILE
  m_eof=0;                                                    //INPUTFILE
  m_prevChar=0;                                               //INPUTFILE
  m_savedChar=0;                                              //INPUTFILE
  m_backspaceTkn=TKN_NONE;                                    //INPUTFILE
  m_latestTkn=TKN_NONE;                                       //INPUTFILE
  return 1;                                                   //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
void INPUTFILE::m_pop(void) {                                 //INPUTFILE
  INPUTFILE *oldsource;                                       //INPUTFILE
  oldsource=m_prevFile;                                       //INPUTFILE
  if (m_file!=NULL) {                                         //INPUTFILE
    m_file->Close();                                          //INPUTFILE
    delete m_file;                                            //INPUTFILE
    m_file=NULL;                                              //INPUTFILE
  };                                                          //INPUTFILE
  if (m_line!=NULL) My_free ((char *)m_line);                    //INPUTFILE
  // copy everything needed to return                         //INPUTFILE
  m_file        =oldsource->m_file;        // CFile *         //INPUTFILE
  m_lineNumber  =oldsource->m_lineNumber;  // int             //INPUTFILE
  m_prevFile    =oldsource->m_prevFile;    // INPUTFILE *     //INPUTFILE
  m_lineIndex   =oldsource->m_lineIndex;                      //INPUTFILE
  m_line        =oldsource->m_line;                           //INPUTFILE
  oldsource->m_file=NULL;                                     //INPUTFILE
  oldsource->m_line=NULL;                                     //INPUTFILE
  delete oldsource;                                           //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
void INPUTFILE::m_push(void) {                                //INPUTFILE
  INPUTFILE *oldsource; // Where we will save old state       //INPUTFILE
  const char *newline;                                        //INPUTFILE
  oldsource = new INPUTFILE;                                  //INPUTFILE
  oldsource->m_prevFile   =m_prevFile;                        //INPUTFILE
  m_prevFile=oldsource;                                       //INPUTFILE
  m_prevFile->m_file       =m_file;                           //INPUTFILE
  m_file=NULL;                                                //INPUTFILE
  m_prevFile->m_lineNumber =m_lineNumber;                     //INPUTFILE
  m_lineNumber=1;                                             //INPUTFILE
  m_prevFile->m_lineIndex  =m_lineIndex;                      //INPUTFILE
  m_lineIndex=0;                                              //INPUTFILE
  newline=m_prevFile->m_line;                                 //INPUTFILE
  m_prevFile->m_line       =m_line;                           //INPUTFILE
  m_line=(char *)newline;  // Swap input buffers.             //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
static bool whitespace(char c) {                              //INPUTFILE
  if (c==' ')  return true;                                   //INPUTFILE
  if (c==0x0d) return true;                                   //INPUTFILE
  if (c=='\n') return true;                                   //INPUTFILE
  if (c=='\t') return true;                                   //INPUTFILE
  if (c==0x10) return true;                                   //INPUTFILE
  if (c==0x12) return true;                                   //INPUTFILE
  return false;                                               //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
char INPUTFILE::m_nextChar(void) {                            //INPUTFILE
//  char *newfilename, *pChar;                                //INPUTFILE
  char temp;                                                  //INPUTFILE
  int result;                                                 //INPUTFILE
  if (m_savedChar != 0) {                                     //INPUTFILE
    temp=m_savedChar;                                         //INPUTFILE
    m_savedChar=0;                                            //INPUTFILE
    return temp;                                              //INPUTFILE
  };                                                          //INPUTFILE
  if (m_eof) return 0;                                        //INPUTFILE
  while (1) {                                                 //INPUTFILE
    if (m_line[m_lineIndex]==0) {                             //INPUTFILE
      if (m_file != NULL)                                     //INPUTFILE
      {                                                       //INPUTFILE
        result=m_file->Read((char *)m_line,m_MAXLINELEN-1);   //INPUTFILE
        if (result==0)                                        //INPUTFILE
        {                                                     //INPUTFILE
          m_line = NULL;                                      //INPUTFILE
        }                                                     //INPUTFILE
        else                                                  //INPUTFILE
        {                                                     //INPUTFILE
           ((char *)m_line)[result]=0;                        //INPUTFILE
        }                                                     //INPUTFILE
      }                                                       //INPUTFILE
      else if (m_lineFetcher != NULL)                         //INPUTFILE
      {                                                       //INPUTFILE
        m_line = (char *)(*m_lineFetcher)(0);                 //INPUTFILE
      }                                                       //INPUTFILE
      else                                                    //INPUTFILE
      {                                                       //INPUTFILE
        error("No input specified",false);                    //INPUTFILE
        m_line = NULL;                                        //INPUTFILE
        return 0;                                             //INPUTFILE
      };                                                      //INPUTFILE
      if (m_line == NULL)                                     //INPUTFILE
      {                                                       //INPUTFILE
        if (m_prevFile==NULL)                                 //INPUTFILE
        {                                                     //INPUTFILE
          m_eof=1;                                            //INPUTFILE
          return 0;                                           //INPUTFILE
        } else {                                              //INPUTFILE
          // Close this file and continue where               //INPUTFILE
          // we left off in the previous file.                //INPUTFILE
          m_pop();                                            //INPUTFILE
          //m_line[0]=0;  // Ignore end of #include line      //INPUTFILE
          m_lineIndex=0;                                      //INPUTFILE
          continue;                                           //INPUTFILE
        };                                                    //INPUTFILE
      };                                                      //INPUTFILE
      m_lineIndex=0;                                          //INPUTFILE
    };                                                        //INPUTFILE
    m_prevChar=m_line[m_lineIndex++];                         //INPUTFILE
    if (m_prevChar=='\n') {                                   //INPUTFILE
      m_lineNumber++;                                         //INPUTFILE
      return m_prevChar;                                      //INPUTFILE
    };                                                        //INPUTFILE
    return m_prevChar;                                        //INPUTFILE
  };                                                          //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
void INPUTFILE::m_backspace(void) {                           //INPUTFILE
  m_savedChar=m_prevChar;                                     //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
void INPUTFILE::backspaceToken(void) {                        //INPUTFILE
  if (m_backspaceTkn!=TKN_NONE) {                             //INPUTFILE
    MessageBox(NULL,                                          //INPUTFILE
             "Internal error in backspaceToken",              //INPUTFILE
           NULL,                                              //INPUTFILE
           MB_OK|MB_TOPMOST);                                 //INPUTFILE
    exit(1);                                                  //INPUTFILE
  };                                                          //INPUTFILE
  m_backspaceTkn=m_latestTkn;                                 //INPUTFILE
};                                                            //INPUTFILE
                                                              //INPUTFILE
void INPUTFILE::error(CString text, bool printLineNumbers)    //INPUTFILE
{                                                             //INPUTFILE
  INPUTFILE *file;                                            //INPUTFILE
  CString message;                                            //INPUTFILE
  m_errorCount++;                                             //INPUTFILE
  if (m_errorCount>12)                                        //INPUTFILE
  {                                                           //INPUTFILE
    TRACE(".");                                               //INPUTFILE
    (*m_ProcessErrorMessage)(".", false);                     //INPUTFILE
    return;                                                   //INPUTFILE
  };                                                          //INPUTFILE
  TRACE("%s\n",text);                                         //INPUTFILE
  message = text;                                             //INPUTFILE
  if (printLineNumbers)                                       //INPUTFILE
  {                                                           //INPUTFILE
    message = "%" + message;                                  //INPUTFILE
  };                                                          //INPUTFILE
  (*m_ProcessErrorMessage)(message, false);                   //INPUTFILE
  if (printLineNumbers==false) return;                        //INPUTFILE
  for (file=this; file!=NULL; file=file->m_prevFile)          //INPUTFILE
  {                                                           //INPUTFILE
    if (file==this)                                           //INPUTFILE
    {                                                         //INPUTFILE
      TRACE(" Error in ");                                    //INPUTFILE
      (*m_ProcessErrorMessage)(" Error in ", false);          //INPUTFILE
    }                                                         //INPUTFILE
    else                                                      //INPUTFILE
    {                                                         //INPUTFILE
      TRACE("  Called from ");                                //INPUTFILE
      (*m_ProcessErrorMessage)("  Called from ", false);      //INPUTFILE
    };                                                        //INPUTFILE                 
    if (m_file != NULL)                                       //INPUTFILE
    {                                                         //INPUTFILE
      TRACE("file  %s",file->m_file->GetFilePath());          //INPUTFILE
      message.Format("file  %s",file->m_file->GetFilePath()); //INPUTFILE
      (*m_ProcessErrorMessage)(message, false);               //INPUTFILE
    }                                                         //INPUTFILE
    else                                                      //INPUTFILE
    {                                                         //INPUTFILE
      TRACE(" main program");                                 //INPUTFILE
      (*m_ProcessErrorMessage)("main program", false);        //INPUTFILE
    };                                                        //INPUTFILE
    TRACE("\n   at line number %d\n",file->m_lineNumber);     //INPUTFILE
    message.Format(                                           //INPUTFILE
              "\n   at line number %d\n",file->m_lineNumber); //INPUTFILE
    (*m_ProcessErrorMessage)(message, false);                 //INPUTFILE
    TRACE("The last token encountered was \'%s\'\n",m_token); //INPUTFILE
    message.Format(                                           //INPUTFILE
               "The last token encountered was \'%s\'\n",m_token);//INPUTFILE
    (*m_ProcessErrorMessage)(message, false);                 //INPUTFILE
    //TRACE("The line being parsed was \'%s\'\n",m_line);     //INPUTFILE
    message.Format("The line being parsed was \'%s\'\n",m_line);//INPUTFILE
    (*m_ProcessErrorMessage)(message, true);                  //INPUTFILE
  };                                                          //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
static bool initialChar(char c) {                             //INPUTFILE
  if (c=='$') return true;                                    //INPUTFILE
  if (c=='_') return true;                                    //INPUTFILE
  if (c<'A') return false;                                    //INPUTFILE
  if (c<='Z') return true;                                    //INPUTFILE
  if (c<'a') return false;                                    //INPUTFILE
  if (c<='z') return true;                                    //INPUTFILE
  return false;                                               //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
static bool moreChar(char c) {                                //INPUTFILE
  if ((c>='0') && (c<='9')) return true;                      //INPUTFILE
  if (c=='@') return true;                                    //INPUTFILE
  return initialChar(c);                                      //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
void INPUTFILE::m_convertHex(void) {                          //INPUTFILE
  char c;                                                     //INPUTFILE
  m_integer=0;                                                //INPUTFILE
  while (1) {                                                 //INPUTFILE
    c=m_nextChar();                                           //INPUTFILE
    if ((c>='0')&&(c<='9')) {                                 //INPUTFILE
      m_integer=m_integer*16+c-'0';                           //INPUTFILE
    } else if ((c>='A')&&(c<='F')) {                          //INPUTFILE
      m_integer=m_integer*16+c-'A'+10;                        //INPUTFILE
    } else if ((c>='a')&&(c<='f')) {                          //INPUTFILE
      m_integer=m_integer*16+c-'a'+10;                        //INPUTFILE
    } else {                                                  //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return;                                                 //INPUTFILE
    }                                                         //INPUTFILE
  };                                                          //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
void INPUTFILE::m_convertDecimal(char c) {                    //INPUTFILE
  m_integer=0;                                                //INPUTFILE
  while((c>='0')&&(c<='9')) {                                 //INPUTFILE
    m_integer=m_integer*10+c-'0';                             //INPUTFILE
    c=m_nextChar();                                           //INPUTFILE
  };                                                          //INPUTFILE
  m_backspace();                                              //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
                                                              //INPUTFILE
TOKENTYPE INPUTFILE::m_getString(void) {                      //INPUTFILE
  // A " was encountered.                                     //INPUTFILE
  // Scan m_line for the closing quote, copying               //INPUTFILE
  // characters to m_token as we go.                          //INPUTFILE
//  int tIndex=0;                                             //INPUTFILE
  char c;                                                     //INPUTFILE
  m_token="";                                                 //INPUTFILE
  while((c=m_nextChar())!='\n') { // Strings cannot span lines//INPUTFILE 
    if (c==0) break;                                          //INPUTFILE
    if (c=='\\') { // If escape character                     //INPUTFILE
      c=m_nextChar();                                         //INPUTFILE
      if (c=='\n') return TKN_NONE;                           //INPUTFILE
      if (c==0) return TKN_NONE;                              //INPUTFILE
      if (c=='n')                                             //INPUTFILE
      {                                                       //INPUTFILE
        m_token+='\n';                                        //INPUTFILE
        continue;                                             //INPUTFILE
      };                                                      //INPUTFILE
      m_token+=c;                                             //INPUTFILE
      continue;                                               //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='"') {                                             //INPUTFILE
      return TKN_STRING;                                      //INPUTFILE
    };                                                        //INPUTFILE
    m_token+=c;                                               //INPUTFILE
  };                                                          //INPUTFILE
  return TKN_NONE; // Ran into end-of-line                    //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
TOKENTYPE INPUTFILE::m_getRawToken(void) {                    //INPUTFILE
  // returns NOOP if no token available                       //INPUTFILE
//  int tIndex=0;                                             //INPUTFILE
  char c, nextc;                                              //INPUTFILE
  m_token="";                                                 //INPUTFILE
  if (m_skipLine) {                                           //INPUTFILE
    m_skipLine=false;                                         //INPUTFILE
    while (1) {                                               //INPUTFILE
      c=m_nextChar();                                         //INPUTFILE
      if (c==0) return TKN_NONE;                              //INPUTFILE
      if (c=='\n') break;                                     //INPUTFILE
//      if (c==0x0d) break;                                   //INPUTFILE
    };                                                        //INPUTFILE
    m_backspace();                                            //INPUTFILE
  };                                                          //INPUTFILE
  while (1) {                                                 //INPUTFILE
    c=m_nextChar();                                           //INPUTFILE
    if (c==0) return TKN_NONE; // no token available          //INPUTFILE
    if (whitespace(c)) continue;                              //INPUTFILE
    m_token+=c;                                               //INPUTFILE
    // If alpha, dollar, or underscore then this is           //INPUTFILE
    //   a name.                                              //INPUTFILE
    if (initialChar(c)) {                                     //INPUTFILE
      while (1) {                                             //INPUTFILE
        c=m_nextChar();                                       //INPUTFILE
        if (c==0) { // no more available                      //INPUTFILE
          return TKN_NAME;                                    //INPUTFILE
        };                                                    //INPUTFILE
        if (moreChar(c)) {                                    //INPUTFILE
          m_token+=c;                                         //INPUTFILE
        } else {                                              //INPUTFILE
          m_backspace();                                      //INPUTFILE
          return TKN_NAME;                                    //INPUTFILE
        };                                                    //INPUTFILE
      };                                                      //INPUTFILE
    };                                                        //INPUTFILE
    if ((c>='0')&&(c<='9')) { // might be 0x...               //INPUTFILE
      if (c=='0') {                                           //INPUTFILE
        c=m_nextChar();                                       //INPUTFILE
        if ((c=='X') || (c=='x')) {                           //INPUTFILE
          m_convertHex();                                     //INPUTFILE
        } else {                                              //INPUTFILE
          m_backspace();                                      //INPUTFILE
          m_convertDecimal('0');                              //INPUTFILE
        }                                                     //INPUTFILE
      } else {                                                //INPUTFILE
        m_convertDecimal(c);                                  //INPUTFILE
      };                                                      //INPUTFILE
      return TKN_INTEGER;                                     //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='(') return TKN_OPENPAREN;                         //INPUTFILE
    if (c==')') return TKN_CLOSEPAREN;                        //INPUTFILE
    if (c==';') return TKN_SEMICOLON;                         //INPUTFILE
    if (c=='*')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token = "*#";                                       //INPUTFILE
        return TKN_nGEAR;                                     //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_GEAR;                                        //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='/') {                                             //INPUTFILE
      nextc=m_nextChar();                                     //INPUTFILE
      if (nextc=='/') {                                       //INPUTFILE
        m_token+='/';                                         //INPUTFILE
        return TKN_DOUBLESLASH;                               //INPUTFILE
      };                                                      //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '#';                                       //INPUTFILE
        return TKN_nSLASH;                                    //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_SLASH;                                       //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='%') {                                             //INPUTFILE
      nextc=m_nextChar();                                     //INPUTFILE
      if (nextc=='#') {                                       //INPUTFILE
        m_token+='#';                                         //INPUTFILE
        return TKN_nPERCENT;                                  //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_PERCENT;                                     //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='"') {                                             //INPUTFILE
      return m_getString();                                   //INPUTFILE
//      while (1) {                                           //INPUTFILE
//        nextc=m_nextChar();                                 //INPUTFILE
//        if (whitespace(nextc)) continue;                    //INPUTFILE
//          if (nextc == '"') {                               //INPUTFILE
//          CString temp;                                     //INPUTFILE
//          temp=m_token;                                     //INPUTFILE
//          if (m_getString()!=TKN_STRING) return TKN_NONE;   //INPUTFILE
//          m_token=temp+m_token;                             //INPUTFILE
//        } else break;                                       //INPUTFILE
//      };                                                    //INPUTFILE
//      m_backspace();                                        //INPUTFILE
//      return TKN_STRING;                                    //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='+')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token = "+#";                                       //INPUTFILE
        return TKN_nPLUS;                                     //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_PLUS;                                        //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='-')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token = "-#";                                       //INPUTFILE
        return TKN_nMINUS;                                    //INPUTFILE
      };                                                      //INPUTFILE
      if ((nextc >= '0') && (nextc <='9'))                    //INPUTFILE
      {                                                       //INPUTFILE
        m_token = "-";                                       //INPUTFILE
        m_backspace();                                        //INPUTFILE
        return TKN_nMINUS;                                    //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_MINUS;                                       //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='{') return TKN_OPENBRACE;                         //INPUTFILE
    if (c=='}') return TKN_CLOSEBRACE;                        //INPUTFILE
    if (c==',') return TKN_COMMA;                             //INPUTFILE
    if (c=='&')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '#';                                       //INPUTFILE
        return TKN_nAND;                                      //INPUTFILE
      };                                                      //INPUTFILE
      if (nextc == '&')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '&';                                       //INPUTFILE
        return TKN_LAND;                                      //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='!')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '=')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '=';                                       //INPUTFILE
        nextc = m_nextChar();                                 //INPUTFILE
        if (nextc == '#')                                     //INPUTFILE
        {                                                     //INPUTFILE
          m_token += '#';                                     //INPUTFILE
          return TKN_nNOTEQUAL;                               //INPUTFILE
        };                                                    //INPUTFILE
        m_backspace();                                        //INPUTFILE
        return TKN_NOTEQUAL;                                  //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_NOT;                                         //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='|')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '#';                                       //INPUTFILE
        return TKN_nOR;                                       //INPUTFILE
      };                                                      //INPUTFILE
      if (nextc == '|')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '|';                                       //INPUTFILE
        return TKN_LOR;                                       //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_NOT;                                         //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='^')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '#';                                       //INPUTFILE
        return TKN_nXOR;                                      //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='=')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc=m_nextChar();                                     //INPUTFILE
      if (nextc == '=')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += "=";                                       //INPUTFILE
        nextc = m_nextChar();                                 //INPUTFILE
        if (nextc == '#')                                     //INPUTFILE
        {                                                     //INPUTFILE
          m_token += '#';                                     //INPUTFILE
          return TKN_nISEQUAL;                                //INPUTFILE
        };                                                    //INPUTFILE
        m_backspace();                                        //INPUTFILE
        return TKN_ISEQUAL;                                   //INPUTFILE
      };                                                      //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '#';                                       //INPUTFILE
        return TKN_nEQUAL;                                    //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_EQUAL;                                       //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='[') return TKN_OPENBRACKET;                       //INPUTFILE
    if (c==']') return TKN_CLOSEBRACKET;                      //INPUTFILE
    if (c=='<')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '#';                                       //INPUTFILE
        return TKN_nLESS;                                     //INPUTFILE
      }                                                       //INPUTFILE
      if (nextc == '=')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '=';                                       //INPUTFILE
        nextc = m_nextChar();                                 //INPUTFILE
        if (nextc == '#')                                     //INPUTFILE
        {                                                     //INPUTFILE
          m_token += '#';                                     //INPUTFILE
          return TKN_nLESSEQUAL;                              //INPUTFILE
        };                                                    //INPUTFILE
        m_backspace();                                        //INPUTFILE
        return TKN_LESSEQUAL;                                 //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_LESS;                                        //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='>')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      if (nextc == '#')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += "#";                                       //INPUTFILE
        return TKN_nGREATER;                                  //INPUTFILE
      };                                                      //INPUTFILE
      if (nextc == '=')                                       //INPUTFILE
      {                                                       //INPUTFILE
        m_token += '=';                                       //INPUTFILE
        nextc = m_nextChar();                                 //INPUTFILE
        if (nextc == '#')                                     //INPUTFILE
        {                                                     //INPUTFILE
          m_token += '#';                                     //INPUTFILE
          return TKN_nGREATEREQUAL;                           //INPUTFILE
        };                                                    //INPUTFILE
        m_backspace();                                        //INPUTFILE
        return TKN_GREATEREQUAL;                              //INPUTFILE
      };                                                      //INPUTFILE
      m_backspace();                                          //INPUTFILE
      return TKN_GREATER;                                     //INPUTFILE
    };                                                        //INPUTFILE
    if (c=='#')                                               //INPUTFILE
    {                                                         //INPUTFILE
      nextc = m_nextChar();                                   //INPUTFILE
      m_backspace();                                          //INPUTFILE
      if (!initialChar(nextc))                                //INPUTFILE
      {                                                       //INPUTFILE
        m_backspace();                                        //INPUTFILE
        return TKN_POUND;                                     //INPUTFILE
      };                                                      //INPUTFILE
      while (1) {                                             //INPUTFILE
        c=m_nextChar();                                       //INPUTFILE
        if (c==0) { // no more available                      //INPUTFILE
          return TKN_PRAGMA;                                  //INPUTFILE
        };                                                    //INPUTFILE
        if (moreChar(c)) {                                    //INPUTFILE
          m_token+=c;                                         //INPUTFILE
        } else {                                              //INPUTFILE
          m_backspace();                                      //INPUTFILE
          return TKN_PRAGMA;                                  //INPUTFILE
        };                                                    //INPUTFILE
      };                                                      //INPUTFILE
    };                                                        //INPUTFILE
    if (c==':') return TKN_COLON;                             //INPUTFILE
    return TKN_NONE;                                          //INPUTFILE
  };                                                          //INPUTFILE
}                                                             //INPUTFILE
                                                              //INPUTFILE
TOKENTYPE INPUTFILE::NextToken(void)                          //INPUTFILE
{                                                             //INPUTFILE
  if (m_backspaceTkn!=TKN_NONE) {                             //INPUTFILE
    m_latestTkn=m_backspaceTkn;                               //INPUTFILE
    m_backspaceTkn=TKN_NONE;                                  //INPUTFILE
    return m_latestTkn;                                       //INPUTFILE
  };                                                          //INPUTFILE
  // Skip comments                                            //INPUTFILE
  do {                                                        //INPUTFILE
    m_latestTkn=m_getRawToken();                              //INPUTFILE
    if (m_latestTkn==TKN_DOUBLESLASH) {                       //INPUTFILE
      m_skipLine=true;                                        //INPUTFILE
    };                                                        //INPUTFILE
  } while (m_latestTkn==TKN_DOUBLESLASH);                     //INPUTFILE
  return m_latestTkn;                                         //INPUTFILE
}                                                             //INPUTFILE

static INPUTFILE infile;

//******************************************************************
//
//  code    code    code    code    code    code    code    code  
enum {                                                        //code
  STRING = 0,                                                 //code
  ACTOR = 1,                                                  //code
};                                                            //code
                                                              //code
class CODE {                                                  //code
private:                                                      //code
  unsigned int *m_code;                                       //code
  unsigned int m_H;                                           //code
  unsigned int m_allocated;                                   //code
public:                                                       //code
  CODE(void);                                                 //code
  ~CODE(void);                                                //code
  void Initialize(void);
  void poke(unsigned int addr, unsigned int value);           //code
  unsigned int here(void) {return m_H;};                      //code
  unsigned int peek(unsigned int addr);                       //code
  unsigned int *codeAddress(void) {return m_code;};           //code
  void comma(unsigned int binCode);                           //code
  void comma(BINOPS binCode);                                 //code
  int write(CArchive& ar);                                    //code
  int read(CArchive& ar);                                     //code
};                                                            //code
                                                              //code

struct SYSTEMFUNCTION {                                       //code
  char name[30];                                              //code
  int  numParam;                                              //code
  SUBOPS binaryCode;                                          //code
  BYTE   UsageType;                                           //code
  unsigned char types[MAX_FUNC_PARAMETERS+1];                 //code
};                                                            //code
                                                              //code
void CODE::Initialize(void)                                   //code
{                                                             //code
  if (m_code != NULL) My_free(m_code);                           //code
  m_H=0;                                                      //code
  m_allocated=0;                                              //code
  m_code=NULL;                                                //code
}                                                             //code
                                                              //code
CODE::CODE(void)                                              //code
{                                                             //code
  m_code = NULL;                                              //code
  Initialize();                                               //code
}                                                             //code
                                                              //code
CODE::~CODE(void) {                                           //code
  if (m_code!=NULL) My_free (m_code);                            //code
  m_code=NULL;                                                //code
}                                                             //code
                                                              //code
unsigned int CODE::peek(unsigned int addr) {                  //code
  return m_code[addr];                                        //code
}                                                             //code
                                                              //code
void CODE::poke(unsigned int addr, unsigned int value) {      //code
  if (addr==0) {                                              //code
    if (m_H!=0) { //  Allow initial NOOP to be placed here.   //code
      MessageBox(NULL,                                        //code
                 "Somone is tampering with address zero",     //code
                 NULL, MB_OK|MB_TOPMOST);                     //code
    };                                                        //code
  };                                                          //code
  m_code[addr]=value;                                         //code
}                                                             //code
                                                              //code
void CODE::comma(unsigned int binCode) {                      //code
  if (m_H >= m_allocated)                                     //code
  {                                                           //code
    ASSERT(m_allocated==m_H); // Must grow one at a time!     //code
    m_allocated=m_allocated*3/2+1;                            //code
    m_code=(unsigned int *)My_realloc(m_code,m_allocated*sizeof (int));
    if (m_code==NULL)                                         //code
    {                                                         //code
      MessageBox(NULL,"Cannot allocate code memory",          //code
                       NULL,MB_OK|MB_TOPMOST|MB_TASKMODAL);   //code
      _exit(1);                                               //code
    };                                                        //code
  };                                                          //code
  m_code[m_H++]=binCode;                                      //code
}                                                             //code
                                                              //code
void CODE::comma(BINOPS binCode) {                            //code
  comma((unsigned int)binCode)  ;                             //code
}                                                             //code
                                                              //code
int CODE::write(CArchive& ar) {                               //code
  unsigned int k;                                             //code
  ar << here();                                               //code
  for (k=0; k < here(); k++) {                                //code
    ar << peek(k);                                            //code
  };                                                          //code
  return 0;                                                   //code
}                                                             //code
                                                              //code
int CODE::read(CArchive& ar) {                                //code
  unsigned int i;                                             //code
  ar >> m_H;                                                  //code
  for (i=0; i<m_H; i++) {                                     //code
    ar>>m_code[i];                                            //code
  };                                                          //code
  return 0;                                                   //code
}                                                             //code
                                                              //code
CODE code;                                                    //code

//******************************************************************
//
// DICTIONARY   DICTIONARY   DICTIONARY   DICTIONARY   DICTIONARY   
//
class DEFINITION {                                            //DICTIONARY
  friend class DICTIONARY;                                    //DICTIONARY
private:                                                      //DICTIONARY
  char         m_id[4];            // "def"                   //DICTIONARY
  DEFINITION  *m_next;                                        //DICTIONARY
  CString      m_name;                                        //DICTIONARY
  bool     m_FLG_proto;        // If function prototype definition
  bool     m_FLG_function;     // Function rather than variable
  bool     m_FLG_FPrelative;   // Relative to Frame pointer 
  bool     m_FLG_LocalVariable;// A variable declared local to the function
  bool     m_FLG_System;       // System-defined function
  bool     m_FLG_Public;       // If function is declared $PUBLIC
  bool     m_FLG_GlobalVariable; // $VAR outside function
  DEFINITION  *m_formalParams; // Formal parameter list
  DEFINITION  *m_localVariable;// Local variable list
  CString      m_charValue;    //                             //DICTIONARY
  unsigned int m_intValue;                                    //DICTIONARY
public:
  DWORD    m_requiredContexts;
private:
  unsigned char m_types[MAX_FUNC_PARAMETERS+1];    //If system function then this //DICTIONARY
                               // is return type and 6 parameter types.
public:                                                       //DICTIONARY
  DEFINITION(void);                                           //DICTIONARY
  ~DEFINITION(void);                                          //DICTIONARY
  bool FLG_proto(void) {return m_FLG_proto;};                 //DICTIONARY
  bool FLG_function (void) {return m_FLG_function;};          //DICTIONARY
  bool FLG_localVar (void) {return m_FLG_LocalVariable;};     //DICTIONARY
  bool FLG_System(void) {return m_FLG_System;};               //DICTIONARY
  bool FLG_FPrelative(void) {return m_FLG_FPrelative;};       //DICTIONARY
  bool FLG_GlobalVariable(void){return m_FLG_GlobalVariable;};//DICTIONARY
  void setFPrelative(void) {m_FLG_FPrelative=true;};          //DICTIONARY
  void setLocalVar(void) {m_FLG_LocalVariable=true;};         //DICTIONARY
  void setSystem(void) {m_FLG_System=true;};                  //DICTIONARY
  void setPublic(void) {m_FLG_Public=true;};                  //DICTIONARY
  void setProto(void) {m_FLG_proto=true;};                    //DICTIONARY
  void setGlobalVariable(void) {m_FLG_GlobalVariable=true;};       //DICTIONARY
  const CString& name(void) const {return m_name;};           //DICTIONARY
  int checkProtoParam(int n);  // Check that this parameter exists in prototype.
  DEFINITION *addFormalParam(CString& pname);                 //DICTIONARY
  DEFINITION *addLocalVariable(CString& pname);               //DICTIONARY
  void setDefaultValue(CString& value) {m_charValue=value;};  //DICTIONARY
  unsigned int intValue(void) {return m_intValue;};           //DICTIONARY
  void setIntValue(unsigned int val) {m_intValue=val;};       //DICTIONARY
  DEFINITION *firstParam(void) {return m_formalParams;};      //DICTIONARY
  int numParam(void); // Number of formal parameters.         //DICTIONARY
  int numLocals(void); // Number of locally declared variables; 
  DEFINITION *next(void) {return m_next;};                    //DICTIONARY
  bool publicUserFunc(void);  // Is this a public, user, function definition?
  void type(int index, unsigned char dtype) {m_types[index]=dtype;};
  unsigned char type(int index) {return m_types[index];};     //DICTIONARY
};                                                            //DICTIONARY
                                                              //DICTIONARY
class DICTIONARY {                                            //DICTIONARY
private:                                                      //DICTIONARY
  char    m_id[4];  // "dic"                                  //DICTIONARY
  CString     m_name;                                         //DICTIONARY
  DICTIONARY *m_parent;                                       //DICTIONARY
  DICTIONARY *m_next;  // siblings                            //DICTIONARY
  DICTIONARY *m_offspring;                                    //DICTIONARY
  DEFINITION *m_definitions;                                  //DICTIONARY
  unsigned int m_countFunctions(void);                        //DICTIONARY
  int m_writeFunctionIndex(CString& prefix, CArchive& ar);    //DICTIONARY
public:                                                       //DICTIONARY
  DICTIONARY(CString name);                                   //DICTIONARY
  ~DICTIONARY(void);                                          //DICTIONARY
  DEFINITION *Lookup(CString& name);                          //DICTIONARY
  DEFINITION *localLookup(CString& name);   // Look in this dictionary only
  DICTIONARY *findDictionary(CString& name) const;            //DICTIONARY
  int m_findPublicFunction(CString& prefix, CString& name);   //DICTIONARY
  DEFINITION *addFunction(CString& name);   // Add function to dictionary
  DEFINITION *addDefinition(const CString& name); // Add arbitrary definition
  DICTIONARY *addDictionary(CString& name); // Add an offspring dictionary.
  DICTIONARY *parent(void) { return m_parent;};               //DICTIONARY

  void parent(DICTIONARY *parent){m_parent=parent;};          //DICTIONARY
  CString findUserFunc(unsigned int address); // function starting at address
  int write(CArchive& ar);                                    //DICTIONARY
  unsigned int GetPublicValue(int n);                         //DICTIONARY
  CString GetPublicName(const CString& prefix, int n);        //DICTIONARY
  int CountPublic(void);                                      //DICTIONARY
};                                                            //DICTIONARY
                                                              //DICTIONARY
class GLOBAL {                                                //DICTIONARY
  friend class GLOBALS;                                       //DICTIONARY
private:                                                      //DICTIONARY
  char        m_type;  // 'C'onstant or 'V'ariable            //DICTIONARY
  CString     m_value; // Constant value or variable name     //DICTIONARY                                     //DICTIONARY
  unsigned int  m_iIndex;  // My index in the GLOBALS array   //DICTIONARY
public:                                                       //DICTIONARY
  GLOBAL (void);                                              //DICTIONARY
  ~GLOBAL (void);                                             //DICTIONARY
  void addCodeReference();                                    //DICTIONARY
  int Index(void) {return m_iIndex;};                         //DICTIONARY
};                                                            //DICTIONARY
                                                              //DICTIONARY
class GLOBALS {                                               //DICTIONARY
  friend class GPDLCOMP;                                      //DICTIONARY
private:                                                      //DICTIONARY
  char     m_id[4];      // "con"                             //DICTIONARY
  GLOBAL    *m_values;                                        //DICTIONARY
  unsigned int m_allocatedSize;                               //DICTIONARY
  unsigned int m_used;                                        //DICTIONARY
public:                                                       //DICTIONARY
  GLOBALS (void);                                             //DICTIONARY
  ~GLOBALS (void);                                            //DICTIONARY
  GLOBAL *SearchConstant(const CString& value);               //DICTIONARY
  GLOBAL *SearchVariable(const CString& value);               //DICTIONARY
  GLOBAL *InsertConstant(const CString& value); // Uses existing entry if possible
  GLOBAL *InsertVariable(const CString& name);                //DICTIONARY
  CString getValue(unsigned int index);                       //DICTIONARY
  char getType(unsigned int index);                           //DICTIONARY
  int write(CArchive& ar);                                    //DICTIONARY
};                                                            //DICTIONARY
                                                              //DICTIONARY
                                                              //DICTIONARY
DEFINITION::DEFINITION (void) {                               //DICTIONARY
  int i;                                                      //DICTIONARY
  m_next=NULL;                                                //DICTIONARY
  m_formalParams=NULL;                                        //DICTIONARY
  m_localVariable=NULL;                                       //DICTIONARY
  m_FLG_proto=false;                                          //DICTIONARY
  m_FLG_FPrelative=false;                                     //DICTIONARY
  m_FLG_LocalVariable=false;                                  //DICTIONARY
  m_FLG_function=false;                                       //DICTIONARY
  m_FLG_System=false;                                         //DICTIONARY
  m_FLG_Public=false;                                         //DICTIONARY
  m_intValue=0xffffffff;                                      //DICTIONARY
  m_requiredContexts=0;                                       //DICTIONARY
  strcpy(m_id,"def");                                         //DICTIONARY

  for (i=0; i<MAX_FUNC_PARAMETERS+1; i++) m_types[i] = 0;     //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DEFINITION::~DEFINITION(void) {                               //DICTIONARY
  DEFINITION *fp;                                             //DICTIONARY
  if (m_next!=NULL) {                                         //DICTIONARY
    MessageBox(NULL,"DEFINITION destructor error",            //DICTIONARY
                       NULL,MB_OK|MB_TOPMOST);                //DICTIONARY
    return;                                                   //DICTIONARY
  };                                                          //DICTIONARY
  while (m_formalParams!=NULL) {                              //DICTIONARY
    fp=m_formalParams;                                        //DICTIONARY
    m_formalParams=fp->m_next;                                //DICTIONARY
    fp->m_next=NULL;                                          //DICTIONARY
    delete fp;                                                //DICTIONARY
  };                                                          //DICTIONARY
  while (m_localVariable != NULL)                             //DICTIONARY
  {                                                           //DICTIONARY
    fp = m_localVariable;                                     //DICTIONARY
    m_localVariable = fp->m_next;                             //DICTIONARY
    fp->m_next = NULL;                                        //DICTIONARY
    delete fp;                                                //DICTIONARY
  };                                                          //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DICTIONARY::DICTIONARY(CString name) {                        //DICTIONARY
  m_name=name;                                                //DICTIONARY
  m_parent=NULL;                                              //DICTIONARY
  m_offspring=NULL;                                           //DICTIONARY
  m_definitions=NULL;                                         //DICTIONARY
  m_next=NULL;                                                //DICTIONARY
  strcpy(m_id,"dic");                                         //DICTIONARY

}                                                             //DICTIONARY
                                                              //DICTIONARY
DICTIONARY::~DICTIONARY(void) {                               //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  while (m_next!=NULL) {                                      //DICTIONARY
    dict=m_next;                                              //DICTIONARY
    m_next=dict->m_next;                                      //DICTIONARY
    dict->m_next=NULL;                                        //DICTIONARY
    delete dict;                                              //DICTIONARY
  };                                                          //DICTIONARY
  while (m_definitions!=NULL) {                               //DICTIONARY
    def=m_definitions;                                        //DICTIONARY
    m_definitions=def->m_next;                                //DICTIONARY
    def->m_next=NULL;                                         //DICTIONARY
    delete def;                                               //DICTIONARY
  };                                                          //DICTIONARY
  while (m_offspring!=NULL) {                                 //DICTIONARY
    dict=m_offspring;                                         //DICTIONARY
    m_offspring=dict->m_offspring;                            //DICTIONARY
    dict->m_offspring=NULL;                                   //DICTIONARY
    delete dict;                                              //DICTIONARY
  };                                                          //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
                                                              //DICTIONARY
DEFINITION *DEFINITION::addFormalParam(CString& name) {       //DICTIONARY
  DEFINITION *param;                                          //DICTIONARY
  param=new DEFINITION;                                       //DICTIONARY
  param->m_next=m_formalParams;                               //DICTIONARY
  m_formalParams=param;                                       //DICTIONARY
  param->m_name=name;                                         //DICTIONARY
  return param;                                               //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DEFINITION *DEFINITION::addLocalVariable(CString& name) {     //DICTIONARY
  DEFINITION *variable;                                       //DICTIONARY
  variable=new DEFINITION;                                    //DICTIONARY
  variable->m_next=m_localVariable;                           //DICTIONARY
  m_localVariable=variable;                                   //DICTIONARY
  variable->m_name=name;                                      //DICTIONARY
  return variable;                                            //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
int DEFINITION::checkProtoParam(int n) {                      //DICTIONARY
  // make sure the zero-based nth parameter exists            //DICTIONARY
  DEFINITION *param;                                          //DICTIONARY
  for (param=m_formalParams;n>=0; n--, param=param->m_next) { //DICTIONARY 
    if (param==NULL) return 1;                                //DICTIONARY
  };                                                          //DICTIONARY
  return 0;                                                   //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
int DEFINITION::numParam(void) {                              //DICTIONARY
  // Count formal parameters.                                 //DICTIONARY
  int result=0;                                               //DICTIONARY
  DEFINITION *param;                                          //DICTIONARY
  for (param=m_formalParams;param!=NULL;param=param->m_next)  //DICTIONARY
    result++;                                                 //DICTIONARY
  return result;                                              //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
int DEFINITION::numLocals(void) {                             //DICTIONARY
  // Count local variables.                                   //DICTIONARY
  int result=0;                                               //DICTIONARY
  DEFINITION *variable;                                       //DICTIONARY
  for (variable=m_localVariable;                              //DICTIONARY
       variable!=NULL;                                        //DICTIONARY
       variable=variable->m_next)                             //DICTIONARY
    result++;                                                 //DICTIONARY
  return result;                                              //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DICTIONARY *DICTIONARY::addDictionary(CString& name) {        //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  dict=new DICTIONARY(name);                                  //DICTIONARY
  dict->m_next=m_offspring;                                   //DICTIONARY
  m_offspring=dict;                                           //DICTIONARY
  dict->m_parent=this;                                        //DICTIONARY
  return dict;                                                //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DEFINITION *DICTIONARY::addFunction(CString& name) {          //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  def=new DEFINITION;                                         //DICTIONARY
  def->m_name=name;                                           //DICTIONARY
  def->m_next=m_definitions;                                  //DICTIONARY
  m_definitions=def;                                          //DICTIONARY
  def->m_FLG_function=true;                                   //DICTIONARY
  return def;                                                 //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DEFINITION *DICTIONARY::addDefinition(const CString& name) {  //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  def=new DEFINITION;                                         //DICTIONARY
  def->m_name=name;                                           //DICTIONARY
  def->m_next=m_definitions;                                  //DICTIONARY
  m_definitions=def;                                          //DICTIONARY
  return def;                                                 //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DICTIONARY *DICTIONARY::findDictionary(CString& name) const { //DICTIONARY
  const DICTIONARY *parent;                                   //DICTIONARY
  DICTIONARY *offspring;                                      //DICTIONARY
  parent=this;                                                //DICTIONARY
  while (parent!=NULL) {                                      //DICTIONARY
    offspring=parent->m_offspring;                            //DICTIONARY
    while (offspring!=NULL) {                                 //DICTIONARY
      if (offspring->m_name==name) // If correct name         //DICTIONARY
            return offspring;                                 //DICTIONARY
      offspring=offspring->m_next;                            //DICTIONARY
    };                                                        //DICTIONARY
    parent=parent->m_parent;                                  //DICTIONARY
  };                                                          //DICTIONARY
  return NULL;                                                //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
bool DEFINITION::publicUserFunc(void) {                       //DICTIONARY
  return m_FLG_function &&                                    //DICTIONARY
       (!m_FLG_System)&&                                      //DICTIONARY
       m_FLG_Public;                                          //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
unsigned int DICTIONARY::m_countFunctions(void) {             //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  unsigned int count=0;                                       //DICTIONARY
  for (def=m_definitions; def!=NULL; def=def->m_next) {       //DICTIONARY
    if (def->publicUserFunc()) count++;                       //DICTIONARY
  };                                                          //DICTIONARY
  for (dict=m_offspring; dict!=NULL; dict=dict->m_next) {     //DICTIONARY
    count+=dict->m_countFunctions();                          //DICTIONARY
  };                                                          //DICTIONARY
  return count;                                               //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
int DICTIONARY::m_writeFunctionIndex(CString& prefix, CArchive& ar) {
  DICTIONARY *dict;                                           //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  CString newprefix;                                          //DICTIONARY
  for (def=m_definitions; def!=NULL; def=def->m_next) {       //DICTIONARY
    if (def->publicUserFunc()) {                              //DICTIONARY
      ar << prefix + def->m_name;                             //DICTIONARY
      ar << def->m_intValue;                                  //DICTIONARY
    };                                                        //DICTIONARY
  };                                                          //DICTIONARY
  for (dict=m_offspring; dict!=NULL; dict=dict->m_next) {     //DICTIONARY
    newprefix=prefix+dict->m_name+'@';                        //DICTIONARY
    dict->m_writeFunctionIndex(newprefix, ar);                //DICTIONARY
  };                                                          //DICTIONARY
  return 0;                                                   //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
int DICTIONARY::m_findPublicFunction(CString& prefix,         //DICTIONARY
                                     CString& name)   {       //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  CString newprefix;                                          //DICTIONARY
  for (def=m_definitions; def!=NULL; def=def->m_next) {       //DICTIONARY
    if (   (def->publicUserFunc())                            //DICTIONARY
         &&(prefix + def->name() == name)) {                  //DICTIONARY
      return  def->m_intValue;                                //DICTIONARY
    };                                                        //DICTIONARY
  };                                                          //DICTIONARY
  for (dict=m_offspring; dict!=NULL; dict=dict->m_next) {     //DICTIONARY
    newprefix=prefix+dict->m_name+'@';                        //DICTIONARY
    dict->m_findPublicFunction(newprefix,name);               //DICTIONARY
  };                                                          //DICTIONARY
  return 0;                                                   //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
int DICTIONARY::write(CArchive& ar) {                         //DICTIONARY
  unsigned int numdef;                                        //DICTIONARY
  CString prefix="";                                          //DICTIONARY
  numdef=m_countFunctions();                                  //DICTIONARY
  ar << numdef;                                               //DICTIONARY
  m_writeFunctionIndex(prefix,ar);                            //DICTIONARY
  return 0;                                                   //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
DEFINITION *DICTIONARY::Lookup(CString& name) {               //DICTIONARY
  // Search only myself.                                      //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  def=m_definitions;                                          //DICTIONARY
  while (def!=NULL) {                                         //DICTIONARY
    if (def->m_name==name) return def;                        //DICTIONARY
    def=def->m_next;                                          //DICTIONARY
  };                                                          //DICTIONARY
  return NULL;                                                //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
CString DICTIONARY::findUserFunc(unsigned int address) {      //DICTIONARY
  // Find a function that starts at the given address.        //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  CString result;                                             //DICTIONARY
  for (def=m_definitions; def!=NULL; def=def->m_next) {       //DICTIONARY
    if (def->m_intValue==address                              //DICTIONARY
      && def->m_FLG_function                                  //DICTIONARY
      && !def->m_FLG_System) {                                //DICTIONARY
      return def->m_name;                                     //DICTIONARY
    };                                                        //DICTIONARY
  };                                                          //DICTIONARY
  for (dict=m_offspring; dict!=NULL; dict=dict->m_next) {     //DICTIONARY
    result=dict->findUserFunc(address);                       //DICTIONARY
    if (result!="") return result;                            //DICTIONARY
  };                                                          //DICTIONARY
  return CString("");                                         //DICTIONARY
}                                                             //DICTIONARY


#include "GPDLcomp.h"

//
// Public Function Name, Nbr Input Parameters, OpCode, ReturnType, ParameterType1..ParameterTypeN
//
SYSTEMFUNCTION systemfunctions[] = 
{
  {"$LISTEN"                   ,0,SUBOP_LISTEN                  ,TALK_USAGE,0},
  {"$DEBUG"                    ,1,SUBOP_DEBUG                   ,ALL_USAGE,0,0},
  {"$DebugWrite"               ,1,SUBOP_DebugWrite              ,ALL_USAGE,0,0},
  {"$PLUS"                     ,2,SUBOP_iPLUS                   ,ALL_USAGE,0,0,0},
  {"$MINUS"                    ,2,SUBOP_iMINUS                  ,ALL_USAGE,0,0,0},
  {"$TIMES"                    ,2,SUBOP_iTIMES                  ,ALL_USAGE,0,0,0},
  {"$DIV"                      ,2,SUBOP_iDIV                    ,ALL_USAGE,0,0,0},
  {"$MOD"                      ,2,SUBOP_iMOD                    ,ALL_USAGE,0,0,0},
  {"$EQUAL"                    ,2,SUBOP_iEQUAL                  ,ALL_USAGE,0,0,0},
  {"$LESS"                     ,2,SUBOP_iLESS                   ,ALL_USAGE,0,0,0},
  {"$GREATER"                  ,2,SUBOP_iGREATER                ,ALL_USAGE,0,0,0},
  {"$TESTKEY"                  ,1,SUBOP_TESTKEY                 ,TALK_USAGE,0,0},
  {"$SAY"                      ,1,SUBOP_SAY                     ,TALK_USAGE,0,0},
  {"$NUMERIC"                  ,1,SUBOP_NUMERIC                 ,ALL_USAGE,0,0},
  {"$LISTENTEXT"               ,0,SUBOP_LISTENTEXT              ,TALK_USAGE,0},
  {"$RANDOM"                   ,1,SUBOP_RANDOM                  ,ALL_USAGE,0,0},
  {"$GREP"                     ,2,SUBOP_GREP                    ,ALL_USAGE,0,0,0},
  {"$WIGGLE"                   ,1,SUBOP_Wiggle                  ,ALL_USAGE,STRING},
  {"$SET_GLOBAL_ASL"           ,2,SUBOP_SET_GLOBAL_ASL          ,ALL_USAGE,0,0,0},
  {"$GET_GLOBAL_ASL"           ,1,SUBOP_GET_GLOBAL_ASL          ,ALL_USAGE,0,0},
  {"$NOT"                      ,1,SUBOP_NOT                     ,ALL_USAGE,0,0},
  {"$DelimitedStringCount"     ,1,SUBOP_DelimitedStringCount    ,ALL_USAGE,STRING,STRING},
  {"$DelimitedStringSubstring" ,2,SUBOP_DelimitedStringSubstring,ALL_USAGE,STRING,STRING,STRING},
  {"$DelimitedStringHead"      ,1,SUBOP_DelimitedStringHead     ,ALL_USAGE,STRING,STRING},
  {"$DelimitedStringTail"      ,1,SUBOP_DelimitedStringTail     ,ALL_USAGE,STRING,STRING},
  {"$DelimitedStringFilter"    ,3,SUBOP_DelimitedStringFilter   ,ALL_USAGE,STRING,STRING,STRING,STRING},
  {"$DelimitedStringAdd"       ,3,SUBOP_DelimitedStringAdd      ,ALL_USAGE,STRING,STRING,STRING,STRING},
  {"$LENGTH"                   ,1,SUBOP_LENGTH                  ,ALL_USAGE,0,0},
  {"$SET_CHAR_ASL"             ,3,SUBOP_SET_CHAR_ASL            ,ALL_USAGE,0,0,0,0},
  {"$PARTYSIZE"                ,0,SUBOP_PARTYSIZE               ,ALL_USAGE,0},
  {"$GET_CHAR_SEX"             ,1,SUBOP_GET_CHAR_SEX            ,ALL_USAGE,0,0},
  {"$SET_CHAR_SEX"             ,2,SUBOP_SET_CHAR_SEX            ,ALL_USAGE,STRING,STRING},
  {"$GET_CHAR_ASL"             ,2,SUBOP_GET_CHAR_ASL            ,ALL_USAGE,0,0,0},
  {"$IF_CHAR_ASL"              ,2,SUBOP_IF_CHAR_ASL             ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_NAME"            ,1,SUBOP_GET_CHAR_NAME           ,ALL_USAGE,0,0},
  // Deprecated {"$SET_LEVEL_ASL"            ,2,SUBOP_SET_LEVEL_ASL           ,ALL_USAGE,0,0,0},
  // Deprecated {"$DELETE_LEVEL_ASL"         ,1,SUBOP_DELETE_LEVEL_ASL        ,ALL_USAGE,0,0},
  {"$SET_LEVEL_STATS_ASL"      ,3,SUBOP_SET_LEVEL_STATS_ASL     ,ALL_USAGE,0,0,0},
  {"$DELETE_LEVEL_STATS_ASL"   ,2,SUBOP_DELETE_LEVEL_STATS_ASL  ,ALL_USAGE,0,0},
  {"$SET_PARTY_ASL"            ,2,SUBOP_SET_PARTY_ASL           ,ALL_USAGE,0,0,0},
  {"$GET_PARTY_ASL"            ,1,SUBOP_GET_PARTY_ASL           ,ALL_USAGE,0,0},
  {"$IF_PARTY_ASL"             ,1,SUBOP_IF_PARTY_ASL            ,ALL_USAGE,0,0},
  {"$DELETE_PARTY_ASL"         ,1,SUBOP_DELETE_PARTY_ASL        ,ALL_USAGE,0,0},
  {"$SMALL_PICTURE"            ,1,SUBOP_SMALL_PICTURE           ,TALK_USAGE,0,0},
  {"$SET_QUEST"                ,2,SUBOP_SET_QUEST               ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_AC"              ,1,SUBOP_GET_CHAR_AC             ,ALL_USAGE,0,0},   // int GET_CHAR_AC( party_index_for_char )
  {"$GET_CHAR_ADJAC"           ,1,SUBOP_GET_CHAR_ADJAC          ,ALL_USAGE,0,0},   // int GET_CHAR_AC( party_index_for_char )
  {"$GET_CHAR_EFFAC"           ,1,SUBOP_GET_CHAR_EFFAC          ,ALL_USAGE,0,0},   // int GET_CHAR_AC( party_index_for_char )
  {"$SET_CHAR_AC"              ,2,SUBOP_SET_CHAR_AC             ,ALL_USAGE,0,0,0}, // void SET_CHAR_AC( party_index_for_char, new_value)
  {"$GET_CHAR_HITPOINTS"       ,1,SUBOP_GET_CHAR_HITPOINTS      ,ALL_USAGE,0,0},
  {"$SET_CHAR_HITPOINTS"       ,2,SUBOP_SET_CHAR_HITPOINTS      ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_MAXHITPOINTS"    ,1,SUBOP_GET_CHAR_MAXHITPOINTS   ,ALL_USAGE,0,0},
  {"$SET_CHAR_MAXHITPOINTS"    ,2,SUBOP_SET_CHAR_MAXHITPOINTS   ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_THAC0"           ,1,SUBOP_GET_CHAR_THAC0          ,ALL_USAGE,0,0},
  {"$GET_CHAR_ADJTHAC0"        ,1,SUBOP_GET_CHAR_ADJTHAC0       ,ALL_USAGE,0,0},
  {"$SET_CHAR_THAC0"           ,2,SUBOP_SET_CHAR_THAC0          ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_RDYTOTRAIN"      ,1,SUBOP_GET_CHAR_RDYTOTRAIN     ,ALL_USAGE,0,0},
  {"$SET_CHAR_RDYTOTRAIN"      ,2,SUBOP_SET_CHAR_RDYTOTRAIN     ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_Exp"             ,2,SUBOP_GET_CHAR_Exp            ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_RACE"            ,1,SUBOP_GET_CHAR_RACE           ,ALL_USAGE,0,0},
  {"$SET_CHAR_RACE"            ,2,SUBOP_SET_CHAR_RACE           ,ALL_USAGE,0,0,0},
  {"$SET_CHAR_Exp"             ,3,SUBOP_SET_CHAR_Exp            ,ALL_USAGE,0,0,0,0},

  //Begin Deprecated
  //{"$GET_CHAR_CLERICEXP"       ,1,SUBOP_GET_CHAR_CLERICEXP      ,ALL_USAGE,0,0},
  //{"$SET_CHAR_CLERICEXP"       ,2,SUBOP_SET_CHAR_CLERICEXP      ,ALL_USAGE,0,0,0},
  //{"$GET_CHAR_MAGICUSEREXP"    ,1,SUBOP_GET_CHAR_MAGICUSEREXP   ,ALL_USAGE,0,0},
  //{"$SET_CHAR_MAGICUSEREXP"    ,2,SUBOP_SET_CHAR_MAGICUSEREXP   ,ALL_USAGE,0,0,0},
  //{"$GET_CHAR_FIGHTEREXP"      ,1,SUBOP_GET_CHAR_FIGHTEREXP     ,ALL_USAGE,0,0},
  //{"$SET_CHAR_FIGHTEREXP"      ,2,SUBOP_SET_CHAR_FIGHTEREXP     ,ALL_USAGE,0,0,0},
  //{"$GET_CHAR_RANGEREXP"       ,1,SUBOP_GET_CHAR_RANGEREXP      ,ALL_USAGE,0,0},
  //{"$SET_CHAR_RANGEREXP"       ,2,SUBOP_SET_CHAR_RANGEREXP      ,ALL_USAGE,0,0,0},
  //{"$GET_CHAR_THIEFEXP"        ,1,SUBOP_GET_CHAR_THIEFEXP       ,ALL_USAGE,0,0},
  //{"$SET_CHAR_THIEFEXP"        ,2,SUBOP_SET_CHAR_THIEFEXP       ,ALL_USAGE,0,0,0},
  //{"$GET_CHAR_DRUIDEXP"        ,1,SUBOP_GET_CHAR_DRUIDEXP       ,ALL_USAGE,0,0},
  //{"$SET_CHAR_DRUIDEXP"        ,2,SUBOP_SET_CHAR_DRUIDEXP       ,ALL_USAGE,0,0,0},
  //{"$GET_CHAR_PALADINEXP"      ,1,SUBOP_GET_CHAR_PALADINEXP     ,ALL_USAGE,0,0},
  //{"$SET_CHAR_PALADINEXP"      ,2,SUBOP_SET_CHAR_PALADINEXP     ,ALL_USAGE,0,0,0},
  //End Deprecated


  {"$GET_CHAR_AGE"             ,1,SUBOP_GET_CHAR_AGE            ,ALL_USAGE,0,0},
  {"$SET_CHAR_AGE"             ,2,SUBOP_SET_CHAR_AGE            ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_MAXAGE"          ,1,SUBOP_GET_CHAR_MAXAGE         ,ALL_USAGE,0,0},
  {"$SET_CHAR_MAXAGE"          ,2,SUBOP_SET_CHAR_MAXAGE         ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_MAXMOVE"         ,1,SUBOP_GET_CHAR_MAXMOVE        ,ALL_USAGE,0,0},
  {"$SET_CHAR_MAXMOVE"         ,2,SUBOP_SET_CHAR_MAXMOVE        ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_LIMITED_STR"     ,1,SUBOP_GET_CHAR_LIMITED_STR    ,ALL_USAGE,0,0},
  {"$GET_CHAR_PERM_STR"        ,1,SUBOP_GET_CHAR_PERM_STR       ,ALL_USAGE,0,0},
  {"$SET_CHAR_PERM_STR"        ,2,SUBOP_SET_CHAR_PERM_STR       ,ALL_USAGE,0,0,0},
  {"$MODIFY_CHAR_ATTRIBUTE"    ,7,SUBOP_MODIFY_CHAR_ATTRIBUTE   ,ALL_USAGE,0,0,0,0,0,0,0,0},
  {"$REMOVE_CHAR_MODIFICATION" ,2,SUBOP_REMOVE_CHAR_MODIFICATION,ALL_USAGE,0,0,0},
  {"$GET_CHAR_LIMITED_STRMOD"  ,1,SUBOP_GET_CHAR_LIMITED_STRMOD ,ALL_USAGE,0,0},
  {"$GET_CHAR_PERM_STRMOD"     ,1,SUBOP_GET_CHAR_PERM_STRMOD    ,ALL_USAGE,0,0},
  {"$SET_CHAR_PERM_STRMOD"     ,2,SUBOP_SET_CHAR_PERM_STRMOD    ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_LIMITED_INT"     ,1,SUBOP_GET_CHAR_LIMITED_INT    ,ALL_USAGE,0,0},
  {"$GET_CHAR_PERM_INT"        ,1,SUBOP_GET_CHAR_PERM_INT       ,ALL_USAGE,0,0},
  {"$SET_CHAR_PERM_INT"        ,2,SUBOP_SET_CHAR_PERM_INT       ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_LIMITED_WIS"     ,1,SUBOP_GET_CHAR_LIMITED_WIS    ,ALL_USAGE,0,0},
  {"$GET_CHAR_PERM_WIS"        ,1,SUBOP_GET_CHAR_PERM_WIS       ,ALL_USAGE,0,0},
  {"$SET_CHAR_PERM_WIS"        ,2,SUBOP_SET_CHAR_PERM_WIS       ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_LIMITED_DEX"     ,1,SUBOP_GET_CHAR_LIMITED_DEX    ,ALL_USAGE,0,0},
  {"$GET_CHAR_PERM_DEX"        ,1,SUBOP_GET_CHAR_PERM_DEX       ,ALL_USAGE,0,0},
  {"$SET_CHAR_PERM_DEX"        ,2,SUBOP_SET_CHAR_PERM_DEX       ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_LIMITED_CON"     ,1,SUBOP_GET_CHAR_LIMITED_CON    ,ALL_USAGE,0,0},
  {"$GET_CHAR_PERM_CON"        ,1,SUBOP_GET_CHAR_PERM_CON       ,ALL_USAGE,0,0},
  {"$SET_CHAR_PERM_CON"        ,2,SUBOP_SET_CHAR_PERM_CON       ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_LIMITED_CHA"     ,1,SUBOP_GET_CHAR_LIMITED_CHA    ,ALL_USAGE,0,0},
  {"$GET_CHAR_PERM_CHA"        ,1,SUBOP_GET_CHAR_PERM_CHA       ,ALL_USAGE,0,0},
  {"$SET_CHAR_PERM_CHA"        ,2,SUBOP_SET_CHAR_PERM_CHA       ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_MAXENC"          ,1,SUBOP_GET_CHAR_MAXENC         ,ALL_USAGE,0,0},
  {"$SET_CHAR_MAXENC"          ,2,SUBOP_SET_CHAR_MAXENC         ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_ENC"             ,1,SUBOP_GET_CHAR_ENC            ,ALL_USAGE,0,0},
  {"$GET_CHAR_GENDER"          ,1,SUBOP_GET_CHAR_GENDER         ,ALL_USAGE,0,0},
  {"$SET_CHAR_GENDER"          ,2,SUBOP_SET_CHAR_GENDER         ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_CLASS"           ,1,SUBOP_GET_CHAR_CLASS          ,ALL_USAGE,0,0},
  {"$SET_CHAR_CLASS"           ,2,SUBOP_SET_CHAR_CLASS          ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_ALIGNMENT"       ,1,SUBOP_GET_CHAR_ALIGNMENT      ,ALL_USAGE,0,0},
  {"$SET_CHAR_ALIGNMENT"       ,2,SUBOP_SET_CHAR_ALIGNMENT      ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_STATUS"          ,1,SUBOP_GET_CHAR_STATUS         ,ALL_USAGE,0,0},
  {"$SET_CHAR_STATUS"          ,2,SUBOP_SET_CHAR_STATUS         ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_UNDEAD"          ,1,SUBOP_GET_CHAR_UNDEAD         ,ALL_USAGE,0,0},
  {"$SET_CHAR_UNDEAD"          ,2,SUBOP_SET_CHAR_UNDEAD         ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_SIZE"            ,1,SUBOP_GET_CHAR_SIZE           ,ALL_USAGE,0,0},
  {"$SET_CHAR_SIZE"            ,2,SUBOP_SET_CHAR_SIZE           ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_MAGICRESIST"     ,1,SUBOP_GET_CHAR_MAGICRESIST    ,ALL_USAGE,0,0},
  {"$SET_CHAR_MAGICRESIST"     ,2,SUBOP_SET_CHAR_MAGICRESIST    ,ALL_USAGE,0,0,0},
  {"$GET_CHAR_Lvl"             ,2,SUBOP_GET_CHAR_Lvl            ,ALL_USAGE,0,0,0},
  {"$SET_CHAR_Lvl"             ,3,SUBOP_SET_CHAR_Lvl            ,ALL_USAGE,0,0,0,0},


  {"$GET_CHAR_NBRHITDICE"      ,1,SUBOP_GET_CHAR_NBRHITDICE     ,ALL_USAGE,0,0},
  {"$GET_CHAR_NBRATTACKS"      ,1,SUBOP_GET_CHAR_NBRATTACKS     ,ALL_USAGE,0,0},
  {"$GET_CHAR_MORALE"          ,1,SUBOP_GET_CHAR_MORALE         ,ALL_USAGE,0,0},
  {"$SET_CHAR_MORALE"          ,2,SUBOP_SET_CHAR_MORALE         ,ALL_USAGE,0,0,0},
  {"$GET_PARTY_FACING"         ,0,SUBOP_GET_PARTY_FACING        ,ALL_USAGE,0},
  {"$GET_PARTY_LOCATION"       ,0,SUBOP_GET_PARTY_LOCATION      ,ALL_USAGE,0},
  {"$SET_PARTY_FACING"         ,1,SUBOP_SET_PARTY_FACING        ,ALL_USAGE,0,0},
  {"$GET_PARTY_DAYS"           ,0,SUBOP_GET_PARTY_DAYS          ,ALL_USAGE,0},
  {"$SET_PARTY_DAYS"           ,1,SUBOP_SET_PARTY_DAYS          ,ALL_USAGE,0,0},
  {"$GET_PARTY_HOURS"          ,0,SUBOP_GET_PARTY_HOURS         ,ALL_USAGE,0},
  {"$SET_PARTY_HOURS"          ,1,SUBOP_SET_PARTY_HOURS         ,ALL_USAGE,0,0},
  {"$GET_PARTY_MINUTES"        ,0,SUBOP_GET_PARTY_MINUTES       ,ALL_USAGE,0},
  {"$SET_PARTY_MINUTES"        ,1,SUBOP_SET_PARTY_MINUTES       ,ALL_USAGE,0,0},
  {"$GET_PARTY_TIME"           ,0,SUBOP_GET_PARTY_TIME          ,ALL_USAGE,0},
  {"$SET_PARTY_TIME"           ,1,SUBOP_SET_PARTY_TIME          ,ALL_USAGE,0,0},
  {"$GET_PARTY_ACTIVECHAR"     ,0,SUBOP_GET_PARTY_ACTIVECHAR    ,ALL_USAGE,0},
  {"$SET_PARTY_ACTIVECHAR"     ,1,SUBOP_SET_PARTY_ACTIVECHAR    ,ALL_USAGE,0,0},
  {"$GET_GAME_CURRLEVEL"       ,0,SUBOP_GET_GAME_CURRLEVEL      ,ALL_USAGE,0},
  {"$GET_GAME_VERSION"         ,0,SUBOP_GET_GAME_VERSION        ,ALL_USAGE,0},
  {"$COMBATANT_AVAILATTACKS"   ,3,SUBOP_COMBATANT_AVAILATTACKS  ,COMBAT_USAGE,0,ACTOR,0,0},
  {"$Myself"                   ,0,SUBOP_Myself                  ,ALL_USAGE,ACTOR},   // Actor Myself()
  {"$Gender"                   ,1,SUBOP_Gender                  ,ALL_USAGE,0,ACTOR}, // GenderType Gender(Actor)
  {"$Name"                     ,1,SUBOP_Name                    ,ALL_USAGE,ACTOR,0}, // Actor Name(charname)
  {"$ClassContext"             ,0,SUBOP_ClassContext            ,ALL_USAGE,STRING},  // context ClassName
  {"$RaceContext"              ,0,SUBOP_RaceContext             ,ALL_USAGE,STRING},  // context Race
  {"$ItemContext"              ,0,SUBOP_ItemContext             ,ALL_USAGE,STRING},
  {"$SpellContext"             ,0,SUBOP_SpellContext            ,ALL_USAGE,STRING},
  {"$SpellgroupContext"        ,0,SUBOP_SpellgroupContext       ,ALL_USAGE,STRING},
  {"$MonsterTypeContext"       ,0,SUBOP_MonsterTypeContext      ,ALL_USAGE,STRING},
  {"$AbilityContext"           ,0,SUBOP_AbilityContext          ,ALL_USAGE,STRING},
  {"$TraitContext"             ,0,SUBOP_TraitContext            ,ALL_USAGE,STRING},
  {"$TargetContext"            ,0,SUBOP_TargetContext           ,ALL_USAGE,ACTOR},   // Actor $Target()
  {"$AttackerContext"          ,0,SUBOP_AttackerContext         ,ALL_USAGE,ACTOR},  
  {"$CombatantContext"         ,0,SUBOP_CombatantContext        ,ALL_USAGE,ACTOR},
  {"$CharacterContext"         ,0,SUBOP_CharacterContext        ,ALL_USAGE,ACTOR},
  {"$NextCreatureIndex"        ,2,SUBOP_NextCreatureIndex       ,ALL_USAGE,0,0,0},
  {"$Status"                   ,1,SUBOP_Status                  ,ALL_USAGE,0,ACTOR}, // StatusType Status(Actor)
  {"$Alignment"                ,1,SUBOP_Alignment               ,ALL_USAGE,0,ACTOR}, // AlignmentType Alignment(Actor)
  {"$AlignmentGood"            ,1,SUBOP_AlignmentGood           ,ALL_USAGE,0,ACTOR}, // BOOL AlignmentGood(Actor)
  {"$AlignmentEvil"            ,1,SUBOP_AlignmentEvil           ,ALL_USAGE,0,ACTOR},
  {"$AlignmentLawful"          ,1,SUBOP_AlignmentLawful         ,ALL_USAGE,0,ACTOR},
  {"$AlignmentNeutral"         ,1,SUBOP_AlignmentNeutral        ,ALL_USAGE,0,ACTOR},
  {"$AlignmentChaotic"         ,1,SUBOP_AlignmentChaotic        ,ALL_USAGE,0,ACTOR},
  {"$HitPoints"                ,1,SUBOP_HitPoints               ,ALL_USAGE,0,ACTOR}, // int HitPoints(Actor)
  {"$InParty"                  ,1,SUBOP_InParty                 ,ALL_USAGE,0,ACTOR}, // BOOL InParty(Actor)
  {"$IsUndead"                 ,1,SUBOP_IsUndead                ,ALL_USAGE,0,ACTOR}, // BOOL IsUndead(Actor)
  {"$MIDDLE"                   ,3,SUBOP_MIDDLE                  ,ALL_USAGE,0,0,0,0},
  {"$IndexOf"                  ,1,SUBOP_IndexOf                 ,ALL_USAGE,0,ACTOR}, // int IndexOf(Actor)
  {"$GET_CHAR_TYPE"            ,1,SUBOP_GET_CHAR_TYPE             ,ALL_USAGE,0,0},     // int GET_CHAR_TYPE( party_index_for_char )
  {"$COINNAME"                  ,1,SUBOP_COINNAME                 ,ALL_USAGE,0,0},   // CString COINNAME( coin_ordinal )
  {"$COINCOUNT"                 ,2,SUBOP_COINCOUNT                ,ALL_USAGE,0,0,0}, // int COINCOUNT(coin_ordinal, party_index_for_char);
  {"$COINRATE"                  ,1,SUBOP_COINRATE                 ,ALL_USAGE,STRING,STRING},   // double COINRATE(coin_ordinal)
  {"$GET_PARTY_MONEYAVAILABLE"  ,1,SUBOP_GET_PARTY_MONEYAVAILABLE ,ALL_USAGE,STRING,STRING},   // int GET_PARTY_MONEYAVAILABLE(convert_to_coin_type) 0=no_convert
  {"$GET_VAULT_MONEYAVAILABLE"  ,1,SUBOP_GET_VAULT_MONEYAVAILABLE ,ALL_USAGE,STRING,STRING},    // int GET_VAULT_MONEYAVAILABLE(convert_to_coin_type) 0=no_convert
  {"$LAST_ATTACKER_OF"          ,1,SUBOP_LAST_ATTACKER_OF         ,ALL_USAGE,ACTOR,ACTOR}, // Actor LAST_ATTACKER_OF(Actor)
  {"$LAST_HITTER_OF"            ,0,SUBOP_NOT_USED_FOR_ANYTHING1   ,ALL_USAGE,STRING}, // Actor LAST_HITTER_OF(Actor)
  {"$LAST_TARGETER_OF"          ,0,SUBOP_NOT_USED_FOR_ANYTHING2   ,ALL_USAGE,STRING}, // Actor LAST_TARGETER_OF(Actor)
  {"$LEAST_DAMAGED_FRIENDLY"    ,0,SUBOP_LEAST_DAMAGED_FRIENDLY   ,ALL_USAGE,ACTOR}, // Actor LEAST_DAMAGED_FRIENDLY()
  {"$MOST_DAMAGED_FRIENDLY"     ,0,SUBOP_MOST_DAMAGED_FRIENDLY    ,ALL_USAGE,ACTOR}, // Actor MOST_DAMAGED_FRIENDLY()
  {"$NEAREST_TO"                ,1,SUBOP_NEAREST_TO               ,ALL_USAGE,ACTOR,ACTOR}, // Actor NEAREST_TO(Actor)
  {"$NEAREST_ENEMY_TO"          ,1,SUBOP_NEAREST_ENEMY_TO         ,ALL_USAGE,ACTOR,ACTOR}, // Actor NEAREST_ENEMY_TO(Actor)
  {"$LEAST_DAMAGED_ENEMY"       ,0,SUBOP_LEAST_DAMAGED_ENEMY      ,ALL_USAGE,ACTOR}, // Actor LEAST_DAMAGED_ENEMY()
  {"$MOST_DAMAGED_ENEMY"        ,0,SUBOP_MOST_DAMAGED_ENEMY       ,ALL_USAGE,ACTOR}, // Actor MOST_DAMAGED_ENEMY()      
  {"$IS_AFFECTED_BY_SPELL"      ,2,SUBOP_IS_AFFECTED_BY_SPELL     ,ALL_USAGE,STRING,ACTOR,STRING}, // String IS_AFFECTED_BY_SPELL(Actor, spell_name)        
  {"$IS_AFFECTED_BY_SPELL_ATTR" ,2,SUBOP_IS_AFFECTED_BY_SPELL_ATTR,ALL_USAGE,STRING,ACTOR,STRING}, // String IS_AFFECTED_BY_SPELL_ATTR(Actor, spell_attrib_name)
  {"$CURR_CHANGE_BY_VAL"        ,0,SUBOP_CURR_CHANGE_BY_VAL       ,SPELL_USAGE,STRING},
  {"$GET_ISMAMMAL"              ,1,SUBOP_GET_ISMAMMAL             ,ALL_USAGE,STRING,ACTOR},
  {"$GET_ISANIMAL"              ,1,SUBOP_GET_ISANIMAL             ,ALL_USAGE,STRING,ACTOR},
  {"$GET_ISSNAKE"               ,1,SUBOP_GET_ISSNAKE              ,ALL_USAGE,STRING,ACTOR},
  {"$GET_ISGIANT"               ,1,SUBOP_GET_ISGIANT              ,ALL_USAGE,STRING,ACTOR},
  {"$GET_ISALWAYSLARGE"         ,1,SUBOP_GET_ISALWAYSLARGE        ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASDWARFACPENALTY"     ,1,SUBOP_GET_HASDWARFACPENALTY    ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASGNOMEACPENALTY"     ,1,SUBOP_GET_HASGNOMEACPENALTY    ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASDWARFTHAC0PENALTY"  ,1,SUBOP_GET_HASDWARFTHAC0PENALTY ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASGNOMETHAC0PENALTY"  ,1,SUBOP_GET_HASGNOMETHAC0PENALTY ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASRANGERDMGPENALTY"   ,1,SUBOP_GET_HASRANGERDMGPENALTY  ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASPOISONIMMUNITY"     ,1,SUBOP_GET_HASPOISONIMMUNITY    ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASDEATHIMMUNITY"      ,1,SUBOP_GET_HASDEATHIMMUNITY     ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASCONFUSIONIMMUNITY"  ,1,SUBOP_GET_HASCONFUSIONIMMUNITY ,ALL_USAGE,STRING,ACTOR},
  {"$GET_HASVORPALIMMUNITY"     ,1,SUBOP_GET_HASVORPALIMMUNITY    ,ALL_USAGE,STRING,ACTOR},
  {"$GET_CANBEHELDORCHARMED"    ,1,SUBOP_GET_CANBEHELDORCHARMED   ,ALL_USAGE,STRING,ACTOR},
  {"$GET_AFFECTEDBYDISPELEVIL"  ,1,SUBOP_GET_AFFECTEDBYDISPELEVIL ,ALL_USAGE,STRING,ACTOR},
  {"$SET_AFFECTEDBYDISPELEVIL"  ,2,SUBOP_SET_AFFECTEDBYDISPELEVIL ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$GIVE_CHAR_ITEM"            ,2,SUBOP_GIVE_CHAR_ITEM           ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$TAKE_CHAR_ITEM"            ,2,SUBOP_TAKE_CHAR_ITEM           ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$LOGIC_BLOCK_VALUE"         ,1,SUBOP_LOGIC_BLOCK_VALUE        ,ALL_USAGE,STRING},
  {"$GET_CHAR_DAMAGEBONUS"      ,1,SUBOP_GET_CHAR_DAMAGEBONUS     ,ALL_USAGE,STRING,STRING},
  {"$SET_CHAR_DAMAGEBONUS"      ,2,SUBOP_SET_CHAR_DAMAGEBONUS     ,ALL_USAGE,STRING,STRING,STRING},  
  {"$GET_CHAR_HITBONUS"         ,1,SUBOP_GET_CHAR_HITBONUS        ,ALL_USAGE,STRING,STRING},
  {"$SET_CHAR_HITBONUS"         ,2,SUBOP_SET_CHAR_HITBONUS        ,ALL_USAGE,STRING,STRING,STRING},
  {"$CHAR_REMOVEALLSPELLS"      ,2,SUBOP_CHAR_REMOVEALLSPELLS     ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$MyIndex"                   ,0,SUBOP_MYINDEX                  ,ALL_USAGE,STRING},
  {"$CastSpellOnTarget"         ,2,SUBOP_CASTSPELLONTARGET        ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$CastSpellOnTargetAs"       ,3,SUBOP_CASTSPELLONTARGETAS      ,ALL_USAGE,STRING,ACTOR,STRING,ACTOR},
  {"$AddCombatant"              ,2,SUBOP_AddCombatant             ,COMBAT_USAGE,STRING,STRING,STRING},
  {"$CHAR_DISPELMAGIC"          ,2,SUBOP_CHAR_DISPELMAGIC         ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$CHAR_REMOVEALLITEMCURSE"   ,1,SUBOP_CHAR_REMOVEALLITEMCURSE  ,ALL_USAGE,STRING,ACTOR},

  {"$GET_CHARACTER_SA"          ,2,SUBOP_GET_CHARACTER_SA         ,ALL_USAGE,0,ACTOR,0},
  {"$SET_CHARACTER_SA"          ,3,SUBOP_SET_CHARACTER_SA         ,ALL_USAGE,0,ACTOR,0,0},
  {"$DELETE_CHARACTER_SA"       ,2,SUBOP_DELETE_CHARACTER_SA      ,ALL_USAGE,0,ACTOR,STRING},
  {"$SA_CHARACTER_GET"          ,1,SUBOP_SA_CHARACTER_GET         ,ALL_USAGE,STRING,STRING},

  {"$GET_COMBATANT_SA"          ,2,SUBOP_GET_COMBATANT_SA         ,COMBAT_USAGE,0,ACTOR,0},
  {"$SET_COMBATANT_SA"          ,3,SUBOP_SET_COMBATANT_SA         ,COMBAT_USAGE,0,ACTOR,0,0},
  {"$DELETE_COMBATANT_SA"       ,2,SUBOP_DELETE_COMBATANT_SA      ,ALL_USAGE,0,ACTOR,STRING},
  {"$SA_COMBATANT_GET"          ,1,SUBOP_SA_COMBATANT_GET         ,COMBAT_USAGE,STRING,STRING},

  {"$GET_ITEM_SA"               ,2,SUBOP_GET_ITEM_SA              ,ALL_USAGE,STRING,STRING,STRING},
  {"$SA_ITEM_GET"               ,1,SUBOP_SA_ITEM_GET              ,ALL_USAGE,STRING,STRING},

  {"$GET_SPELL_SA"              ,2,SUBOP_GET_SPELL_SA             ,ALL_USAGE,STRING,STRING,STRING},
  {"$SA_SPELL_GET"              ,1,SUBOP_SA_SPELL_GET             ,ALL_USAGE,STRING,STRING},


  {"$GET_MONSTERTYPE_SA"        ,2,SUBOP_GET_MONSTERTYPE_SA       ,ALL_USAGE,STRING,STRING,STRING},
  {"$SA_MONSTERTYPE_GET"        ,1,SUBOP_SA_MONSTERTYPE_GET       ,ALL_USAGE,STRING,STRING},

  {"$GET_RACE_SA"               ,2,SUBOP_GET_RACE_SA              ,ALL_USAGE,STRING,STRING,STRING},
  {"$SA_RACE_GET"               ,1,SUBOP_SA_RACE_GET              ,ALL_USAGE,STRING,STRING},

  {"$GET_ABILITY_SA"            ,2,SUBOP_GET_ABILITY_SA           ,ALL_USAGE,STRING,STRING,STRING},
  {"$SA_ABILITY_GET"            ,1,SUBOP_SA_ABILITY_GET           ,ALL_USAGE,STRING,STRING},


  {"$GET_CLASS_SA"              ,2,SUBOP_GET_CLASS_SA             ,ALL_USAGE,STRING,STRING,STRING},
  {"$GET_BASECLASS_SA"          ,2,SUBOP_GET_BASECLASS_SA         ,ALL_USAGE,STRING,STRING,STRING},
  {"$SA_CLASS_GET"              ,1,SUBOP_SA_CLASS_GET             ,ALL_USAGE,STRING,STRING},
  {"$SA_BASECLASS_GET"          ,1,SUBOP_SA_BASECLASS_GET         ,ALL_USAGE,STRING,STRING},

  {"$GET_SPELL_Level"           ,1,SUBOP_GET_SPELL_Level          ,ALL_USAGE,STRING,STRING},
  {"$GET_SPELL_CanBeDispelled"  ,1,SUBOP_GET_SPELL_CanBeDispelled ,ALL_USAGE,STRING,STRING},
  {"$GET_HOOK_PARAM"            ,1,SUBOP_GET_HOOK_PARAM           ,ALL_USAGE,STRING,STRING},
  {"$SET_HOOK_PARAM"            ,2,SUBOP_SET_HOOK_PARAM           ,ALL_USAGE,STRING,STRING,STRING},
  {"$GET_CHAR_Ready"            ,3,SUBOP_GET_CHAR_Ready           ,ALL_USAGE,STRING,ACTOR,STRING,STRING},
  {"$SET_CHAR_Ready"            ,3,SUBOP_SET_CHAR_Ready           ,ALL_USAGE,STRING,ACTOR,STRING,STRING},
  {"$SA_PARAM_GET"              ,0,SUBOP_SA_PARAM_GET             ,ALL_USAGE,STRING},
  {"$SA_PARAM_SET"              ,1,SUBOP_SA_PARAM_SET             ,ALL_USAGE,STRING,STRING},
  {"$SA_NAME"                   ,0,SUBOP_SA_NAME                  ,ALL_USAGE,STRING},
  {"$SA_SOURCE_TYPE"            ,0,SUBOP_SA_SOURCE_TYPE           ,ALL_USAGE,STRING},
  {"$SA_SOURCE_NAME"            ,0,SUBOP_SA_SOURCE_NAME           ,ALL_USAGE,STRING},
  {"$SA_REMOVE"                 ,0,SUBOP_SA_REMOVE                ,ALL_USAGE,STRING},
  {"$RUN_CHAR_SCRIPTS"          ,2,SUBOP_RUN_CHAR_SCRIPTS         ,ALL_USAGE,0,0,0},
  {"$SET_PARTY_XY"              ,2,SUBOP_SET_PARTY_XY             ,ALL_USAGE,0,0,0},
  {"$CALL_GLOBAL_SCRIPT"        ,2,SUBOP_CALL_GLOBAL_SCRIPT       ,ALL_USAGE,0,0,0},
  {"$VisualDistance"            ,2,SUBOP_VisualDistance           ,COMBAT_USAGE,0,0,0},
  {"$TeleportCombatant"         ,3,SUBOP_TeleportCombatant        ,COMBAT_USAGE,STRING,STRING,STRING,STRING},
  {"$CombatantLocation"         ,2,SUBOP_CombatantLocation        ,COMBAT_USAGE,STRING,STRING,STRING},
  {"$IsLineOfSight"             ,5,SUBOP_IsLineOfSight            ,COMBAT_USAGE,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$SetFriendly"               ,2,SUBOP_SetFriendly              ,COMBAT_USAGE,STRING,STRING,STRING},
  {"$GetFriendly"               ,2,SUBOP_GetFriendly              ,COMBAT_USAGE,STRING,STRING,STRING},
  {"$GetCombatRound"            ,0,SUBOP_GetCombatRound           ,COMBAT_USAGE,STRING},
  {"$DAT_Baseclass_Experience"  ,2,SUBOP_DAT_Baseclass_Experience ,ALL_USAGE,STRING,STRING,STRING},
  {"$DAT_Baseclass_Level"       ,2,SUBOP_DAT_Baseclass_Level      ,ALL_USAGE,STRING,STRING,STRING},
  {"$DAT_Race_Weight"           ,1,SUBOP_DAT_Race_Weight          ,ALL_USAGE,STRING,ACTOR},
  {"$DAT_Race_Height"           ,1,SUBOP_DAT_Race_Height          ,ALL_USAGE,STRING,ACTOR},
  {"$DAT_Item_CommonName"       ,1,SUBOP_DAT_Item_CommonName      ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_IDName"           ,1,SUBOP_DAT_Item_IDName          ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_MaxRange"         ,1,SUBOP_DAT_Item_MaxRange        ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_MediumRange"      ,1,SUBOP_DAT_Item_MediumRange     ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_ShortRange"       ,1,SUBOP_DAT_Item_ShortRange      ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_Priority"         ,1,SUBOP_DAT_Item_Priority        ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_DamageSmall"      ,1,SUBOP_DAT_Item_DamageSmall     ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_DamageLarge"      ,1,SUBOP_DAT_Item_DamageLarge     ,ALL_USAGE,STRING,STRING},
  {"$DAT_Item_AttackBonus"      ,1,SUBOP_DAT_Item_AttackBonus     ,ALL_USAGE,STRING,STRING},
  {"$DAT_Class_Baseclasses"     ,1,SUBOP_DAT_Class_Baseclasses    ,ALL_USAGE,STRING,STRING},
  {"$GET_CONFIG"                ,1,SUBOP_GET_CONFIG               ,ALL_USAGE,STRING,STRING},
  {"$ListAdjacentCombatants"    ,1,SUBOP_ListAdjacentCombatants   ,COMBAT_USAGE,STRING,STRING},
  {"$ComputeAttackDamage"       ,2,SUBOP_ComputeAttackDamage      ,COMBAT_USAGE,STRING,STRING,STRING},
//  {"$GET_SPELL_NAME"            ,1,SUBOP_GetSpellName             ,ALL_USAGE,STRING,SPELL},
  {"$UpCase"                    ,1,SUBOP_UpCase                   ,ALL_USAGE,STRING,STRING},
  {"$Capitalize"                ,1,SUBOP_Capitalize               ,ALL_USAGE,STRING,STRING},
  {"$DownCase"                  ,1,SUBOP_DownCase                 ,ALL_USAGE,STRING,STRING},
  //Deprecated {"$GET_SPELLBOOK"             ,2,SUBOP_GetSpellbook             ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$SelectSpell"               ,2,SUBOP_SelectSpell              ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$Memorize"                  ,1,SUBOP_Memorize                 ,ALL_USAGE,STRING,ACTOR},
  {"$MonsterPlacement"          ,1,SUBOP_MonsterPlacement         ,COMBAT_USAGE,STRING,STRING},
  {"$SetWall"                   ,5,SUBOP_SetWall                  ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$SetBackground"             ,5,SUBOP_SetBackground            ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$SetOverlay"                ,5,SUBOP_SetOverlay               ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$SetDoor"                   ,5,SUBOP_SetDoor                  ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$SetBlockage"               ,5,SUBOP_SetBlockage              ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$GetWall"                   ,4,SUBOP_GetWall                  ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING},
  {"$GetBackground"             ,4,SUBOP_GetBackground            ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING},
  {"$GetOverlay"                ,4,SUBOP_GetOverlay               ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING},
  {"$GetDoor"                   ,4,SUBOP_GetDoor                  ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING},
  {"$GetBlockage"               ,4,SUBOP_GetBlockage              ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING},
  {"$RUN_CHAR_SE_SCRIPTS"       ,2,SUBOP_RUN_CHAR_SE_SCRIPTS      ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$REMOVE_SPELL_EFFECT"       ,2,SUBOP_REMOVE_SPELL_EFFECT      ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$RUN_AREA_SE_SCRIPTS"       ,2,SUBOP_RUN_AREA_SE_SCRIPTS      ,COMBAT_USAGE,STRING,STRING,STRING},
  {"$RUN_CHAR_PS_SCRIPTS"       ,2,SUBOP_RUN_CHAR_PS_SCRIPTS      ,COMBAT_USAGE,STRING,ACTOR,STRING},
  {"$IntegerTable"              ,4,SUBOP_IntegerTable             ,ALL_USAGE,STRING,STRING,STRING,STRING,STRING},
  {"$ForEachPartyMember"        ,2,SUBOP_ForEachPartyMember       ,ALL_USAGE,STRING,STRING,STRING},
  {"$ForEachPossession"         ,2,SUBOP_ForEachPossession        ,ALL_USAGE,STRING,ACTOR,STRING},
  {"$GetCombatantState"         ,1,SUBOP_GetCombatantState        ,COMBAT_USAGE,STRING,ACTOR},
  {"$IsIdentified"              ,3,SUBOP_IsIdentified             ,ALL_USAGE,STRING,ACTOR,STRING,STRING},
  {"$SkillAdj"                  ,5,SUBOP_SkillAdj                 ,ALL_USAGE,STRING,ACTOR,STRING,STRING,STRING,STRING},
  {"$SpellAdj"                  ,7,SUBOP_SpellAdj                 ,ALL_USAGE,STRING,ACTOR,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$SetMemorizeCount"          ,3,SUBOP_SetMemorizeCount         ,ALL_USAGE,STRING,STRING,STRING,STRING},
  {"$GetHighestLevelBaseclass"  ,1,SUBOP_GetHighestLevelBaseclass ,ALL_USAGE,STRING,STRING},
  {"$GetBaseclassLevel"         ,2,SUBOP_GetBaseclassLevel        ,ALL_USAGE,STRING,STRING,STRING},
  {"$RollHitPointDice"          ,3,SUBOP_RollHitPointDice         ,ALL_USAGE,STRING,STRING,STRING,STRING},
  {"$AURA_Create"               ,5,SUBOP_AURA_Create              ,COMBAT_USAGE|SPELL_USAGE,STRING,STRING,STRING,STRING,STRING,STRING},
  {"$AURA_Destroy"              ,0,SUBOP_AURA_Destroy             ,COMBAT_USAGE,STRING},
  {"$AURA_AddSA"                ,2,SUBOP_AURA_AddSA               ,COMBAT_USAGE,STRING,STRING,STRING},
  {"$AURA_GetSA"                ,1,SUBOP_AURA_GetSA               ,COMBAT_USAGE,STRING,STRING},
  {"$AURA_RemoveSA"             ,1,SUBOP_AURA_RemoveSA            ,COMBAT_USAGE,STRING,STRING},
  {"$AURA_Size"                 ,4,SUBOP_AURA_Size                ,COMBAT_USAGE,STRING,STRING,STRING,STRING,STRING},
  {"$AURA_Shape"                ,1,SUBOP_AURA_Shape               ,COMBAT_USAGE,STRING,STRING},
  {"$AURA_Attach"               ,1,SUBOP_AURA_Attach              ,COMBAT_USAGE,STRING,STRING},
  {"$AURA_Location"             ,2,SUBOP_AURA_Location            ,COMBAT_USAGE,STRING,STRING,STRING},
  {"$AURA_Spell"                ,1,SUBOP_AURA_Spell               ,COMBAT_USAGE,STRING,STRING},
  {"$AURA_Combatant"            ,1,SUBOP_AURA_Combatant           ,COMBAT_USAGE,STRING,STRING},
  {"$AURA_Wavelength"           ,1,SUBOP_AURA_Wavelength          ,COMBAT_USAGE,STRING,STRING},
  {"$GrSet"                     ,3,SUBOP_GrSet                    ,GRAPHICS_USAGE,STRING,STRING,STRING,STRING},
  {"$GrSetLinefeed"             ,1,SUBOP_GrSetLinefeed            ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrMoveTo"                  ,1,SUBOP_GrMoveTo                 ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrFormat"                  ,1,SUBOP_GrFormat                 ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrColor"                   ,1,SUBOP_GrColor                  ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrPrint"                   ,1,SUBOP_GrPrint                  ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrPrtLF"                   ,1,SUBOP_GrPrtLF                  ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrMark"                    ,1,SUBOP_GrMark                   ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrMove"                    ,1,SUBOP_GrMove                   ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrTab"                     ,1,SUBOP_GrTab                    ,GRAPHICS_USAGE,STRING,STRING},
  {"$GrPic"                     ,1,SUBOP_GrPic                    ,GRAPHICS_USAGE,STRING,STRING},
  {"$ToHitComputation_Roll"     ,0,SUBOP_ToHitComputation_Roll    ,ALL_USAGE,STRING},
};

static const unsigned int numSysFunc = sizeof systemfunctions/sizeof systemfunctions[0];

struct REQUIRED_CONTEXT
{
  SUBOPS binaryCode;
  DWORD  contexts;
};

REQUIRED_CONTEXT requiredContexts[] =
{
  {SUBOP_Myself                  ,CTX_Myself},
  {SUBOP_ClassContext            ,CTX_Class},
  {SUBOP_RaceContext             ,CTX_Race},
  {SUBOP_ItemContext             ,CTX_Item},
  {SUBOP_SpellContext            ,CTX_Spell},
  {SUBOP_SpellgroupContext       ,CTX_SpellGroup},
  {SUBOP_MonsterTypeContext      ,CTX_MonsterType},
  {SUBOP_AbilityContext          ,CTX_Ability},
  {SUBOP_TraitContext            ,CTX_Trait},
  {SUBOP_TargetContext           ,CTX_Target},
  {SUBOP_AttackerContext         ,CTX_Attacker},  
  {SUBOP_CombatantContext        ,CTX_Combatant},
  {SUBOP_CharacterContext        ,CTX_Character},
};

static const unsigned int numRequiredContexts = sizeof requiredContexts/sizeof requiredContexts[0];






                                                              //DICTIONARY
/* *****************************************************      //DICTIONARY
 *                                                            //DICTIONARY
 * The system dictionary will contain the system functions    //DICTIONARY
 * from the table just above.  We used to compile them into   //DICTIONARY
 * the root dictionary every time we initialized the          //DICTIONARY
 * compiler.  But that happened every time we compiled a      //DICTIONARY
 * spell, etc.  So now we have a global system dictionary     //DICTIONARY
 * that is the parent of the root dictionary and we only      //DICTIONARY
 * compile a system function into this dictionary when we     //DICTIONARY
 * find it referenced and not already there.                  //DICTIONARY
 *                                                            //DICTIONARY
 */                                                           //DICTIONARY
DICTIONARY systemDictionary("system");                        //DICTIONARY
DEFINITION *DICTIONARY::localLookup(CString& vname) {         //DICTIONARY
  // Search dictionary heirarchy.  Formal parameters          //DICTIONARY
  //  will be found only at the local level.                  //DICTIONARY
  bool local=true;  // We are still in local dictionary.      //DICTIONARY
  CString name;                                               //DICTIONARY
  CString temp;                                               //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  int at;                                                     //DICTIONARY
  name=vname;                                                 //DICTIONARY
  dict=this; // We start with us.                             //DICTIONARY
  if ((at=name.Find('@'))<0) {                                //DICTIONARY
    while (dict!=NULL) {                                      //DICTIONARY
      def=dict->Lookup(name);                                 //DICTIONARY
      if (def!=NULL) {  // If we found it                     //DICTIONARY
        if (!local) {                                         //DICTIONARY
          if (def->m_FLG_FPrelative) {                        //DICTIONARY
            return NULL;                                      //DICTIONARY
          };                                                  //DICTIONARY
        };                                                    //DICTIONARY
        return def;                                           //DICTIONARY
      };                                                      //DICTIONARY
      // Go search our ancestors.  If we are a                //DICTIONARY
      //  function-level dictionary then set local to false.  //DICTIONARY
//      if (dict->m_FLG_function) local=false;                //DICTIONARY
      if (dict == &systemDictionary)                          //DICTIONARY
      {                                                       //DICTIONARY
        // Perhaps we need to add a system function to        //DICTIONARY
        // the system dictionary.                             //DICTIONARY
        int i;                                                //DICTIONARY
        for (i=0; i<numSysFunc; i++)                          //DICTIONARY
        {                                                     //DICTIONARY
          int j;                                              //DICTIONARY
          if (name == systemfunctions[i].name)                //DICTIONARY
          {                                                   //DICTIONARY
            CString pname;                                    //DICTIONARY
            def = systemDictionary.addFunction(name);         //DICTIONARY
            def->setIntValue(  (SHIFTED_BINOP)                //DICTIONARY
                              |systemfunctions[i].binaryCode);//DICTIONARY
            def->setSystem();                                 //DICTIONARY
            {                                                 //DICTIONARY
              int k;                                          //DICTIONARY
              for (k=0; k<numRequiredContexts; k++)           //DICTIONARY
              {                                               //DICTIONARY
                if (requiredContexts[k].binaryCode            //DICTIONARY
                           == systemfunctions[i].binaryCode)  //DICTIONARY
                {                                             //DICTIONARY
                  def->m_requiredContexts =                   //DICTIONARY
                            requiredContexts[k].contexts;     //DICTIONARY
                  break;                                      //DICTIONARY
                };                                            //DICTIONARY
              };                                              //DICTIONARY
            };                                                //DICTIONARY
            pname="a";                                        //DICTIONARY
            for (j=0; j<systemfunctions[i].numParam; j++)     //DICTIONARY
            {                                                 //DICTIONARY  
              def->addFormalParam(pname);                     //DICTIONARY
              pname+="a";                                     //DICTIONARY
            };                                                //DICTIONARY
            for (j=0; j<6; j++)                               //DICTIONARY
            {                                                 //DICTIONARY
              def->type(j, systemfunctions[i].types[j]);      //DICTIONARY
            };                                                //DICTIONARY
            return def;                                       //DICTIONARY
          };                                                  //DICTIONARY
        };                                                    //DICTIONARY
                                                              //DICTIONARY
        return NULL;                                          //DICTIONARY
      };                                                      //DICTIONARY
      dict=dict->m_parent;                                    //DICTIONARY
    };                                                        //DICTIONARY
    return NULL;                                              //DICTIONARY
  } else {                                                    //DICTIONARY
    temp=name.Left(at);                                       //DICTIONARY
    dict=findDictionary(temp);                                //DICTIONARY
    if (dict==NULL) return NULL;                              //DICTIONARY
    name=name.Right(name.GetLength()-at-1);                   //DICTIONARY
    return dict->localLookup(name);                           //DICTIONARY
  };                                                          //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBALS::GLOBALS(void) {                                      //DICTIONARY
  unsigned int i;                                             //DICTIONARY
  m_allocatedSize=4;  // The absolute minimum!                //DICTIONARY
  m_used=1;  // We don't use the zeroth entry.                //DICTIONARY
  m_values=new GLOBAL[m_allocatedSize];                       //DICTIONARY
  for (i=1; i<m_allocatedSize; i++) m_values[i].m_iIndex=i;   //DICTIONARY
  strcpy(m_id,"con");                                         //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBALS::~GLOBALS(void) {                                     //DICTIONARY
  delete [] m_values;                                         //DICTIONARY
  m_allocatedSize=0;                                          //DICTIONARY
  m_values=NULL;                                              //DICTIONARY
  m_used=0;                                                   //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBAL::GLOBAL (void)                                         //DICTIONARY
{                                                             //DICTIONARY
  m_iIndex=0xffffffff;                                        //DICTIONARY
  m_type = ' ';                                               //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBAL::~GLOBAL (void) {                                      //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
void GLOBAL::addCodeReference(void) {                         //DICTIONARY
  code.comma((BINOP_ReferenceGLOBAL<<24)|m_iIndex);           //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBAL *GLOBALS::InsertConstant(const CString& value) {       //DICTIONARY
  GLOBAL *result;                                             //DICTIONARY
  result=SearchConstant(value);                               //DICTIONARY
  if (result!=NULL) return result;                            //DICTIONARY
  if (m_used >= m_allocatedSize)                              //DICTIONARY
  {                                                           //DICTIONARY
    // We need to expand the GLOBAL array.                    //DICTIONARY
    unsigned int newsize;                                     //DICTIONARY
    unsigned int i;                                           //DICTIONARY
    newsize=(m_allocatedSize*5)/4;                            //DICTIONARY
    result=new GLOBAL[newsize];                               //DICTIONARY
    for (i=0; i<newsize; i++) {                               //DICTIONARY
      if (i<m_allocatedSize) result[i]=m_values[i];           //DICTIONARY
      else result[i].m_iIndex=i;                              //DICTIONARY
    };                                                        //DICTIONARY
    delete []m_values;                                        //DICTIONARY
    m_values=result;                                          //DICTIONARY
    m_allocatedSize=newsize;                                  //DICTIONARY
  };                                                          //DICTIONARY
  result=m_values+m_used;                                     //DICTIONARY
  m_used++;                                                   //DICTIONARY
  result->m_value=value;                                      //DICTIONARY
  result->m_type = 'C';                                       //DICTIONARY
  return result;                                              //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBAL *GLOBALS::InsertVariable(const CString& name) {        //DICTIONARY
  GLOBAL *result;                                             //DICTIONARY
  result=SearchVariable(name);                                //DICTIONARY
  if (result!=NULL) return result;                            //DICTIONARY
  if (m_used >= m_allocatedSize)                              //DICTIONARY
  {                                                           //DICTIONARY
    // We need to expand the GLOBAL array.                    //DICTIONARY
    unsigned int newsize;                                     //DICTIONARY
    unsigned int i;                                           //DICTIONARY
    newsize=(m_allocatedSize*5)/4;                            //DICTIONARY
    result=new GLOBAL[newsize];                               //DICTIONARY
    for (i=0; i<newsize; i++) {                               //DICTIONARY
      if (i<m_allocatedSize) result[i]=m_values[i];           //DICTIONARY
      else result[i].m_iIndex=i;                              //DICTIONARY
    };                                                        //DICTIONARY
    delete []m_values;                                        //DICTIONARY
    m_values=result;                                          //DICTIONARY
    m_allocatedSize=newsize;                                  //DICTIONARY
  };                                                          //DICTIONARY
  result=m_values+m_used;                                     //DICTIONARY
  m_used++;                                                   //DICTIONARY
  result->m_value=name;                                       //DICTIONARY
  result->m_type = 'V';                                       //DICTIONARY
  return result;                                              //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBAL *GLOBALS::SearchConstant(const CString& value) {       //DICTIONARY
  GLOBAL *result;                                             //DICTIONARY
  for (result=m_values+1;                                     //DICTIONARY
       result!=m_values+m_used;                               //DICTIONARY
       result=result++) {                                     //DICTIONARY
    if (      (result->m_value==value)                        //DICTIONARY
          &&  (result->m_type == 'C')                         //DICTIONARY
       ) return result;                                       //DICTIONARY
  };                                                          //DICTIONARY
  return NULL;                                                //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
GLOBAL *GLOBALS::SearchVariable(const CString& name) {        //DICTIONARY
  GLOBAL *result;                                             //DICTIONARY
  for (result=m_values+1;                                     //DICTIONARY
       result!=m_values+m_used;                               //DICTIONARY
       result=result++) {                                     //DICTIONARY
    if (      (result->m_value==name)                         //DICTIONARY
          &&  (result->m_type == 'V')                         //DICTIONARY
       ) return result;                                       //DICTIONARY
  };                                                          //DICTIONARY
  return NULL;                                                //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
CString GLOBALS::getValue(unsigned int index) {               //DICTIONARY
  if (index<m_used) return m_values[index].m_value;           //DICTIONARY
  return "Undefined";                                         //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
                                                              //DICTIONARY
char GLOBALS::getType(unsigned int index) {                   //DICTIONARY
  if (index<m_used) return m_values[index].m_type;            //DICTIONARY
  return ' ';                                                 //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
                                                              //DICTIONARY
int GLOBALS::write(CArchive& ar) {                            //DICTIONARY
  unsigned int i;                                             //DICTIONARY
  ar << m_used;                                               //DICTIONARY
  for (i=0; i<m_used; i++) {                                  //DICTIONARY
    if (m_values[i].m_type == 'V')                            //DICTIONARY
    {                                                         //DICTIONARY
      ar << CString("");                                      //DICTIONARY
    }                                                         //DICTIONARY
    else                                                      //DICTIONARY
    {                                                         //DICTIONARY
      ar << m_values[i].m_value;                              //DICTIONARY
    };                                                        //DICTIONARY
  };                                                          //DICTIONARY
  return 0;                                                   //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
unsigned int DICTIONARY::GetPublicValue(int n)                //DICTIONARY
{                                                             //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  CString newprefix;                                          //DICTIONARY
  for (def=m_definitions; def!=NULL; def=def->m_next)         //DICTIONARY
  {                                                           //DICTIONARY
    if (def->publicUserFunc())                                //DICTIONARY
    {                                                         //DICTIONARY
      if (n==0)                                               //DICTIONARY
      {                                                       //DICTIONARY
        return def->m_intValue;                               //DICTIONARY
      };                                                      //DICTIONARY
      n--;                                                    //DICTIONARY
    };                                                        //DICTIONARY
  };                                                          //DICTIONARY
  //for (dict=m_offspring; dict!=NULL; dict=dict->m_next)     //DICTIONARY
  for (dict=m_offspring; dict!=NULL; )                        //DICTIONARY
  {                                                           //DICTIONARY
    return dict->GetPublicValue(n);                           //DICTIONARY
  };                                                          //DICTIONARY
  return 0;                                                   //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
CString  DICTIONARY::GetPublicName(const CString& prefix,     //DICTIONARY
                                   int n)                     //DICTIONARY
{                                                             //DICTIONARY
  DICTIONARY *dict;                                           //DICTIONARY
  DEFINITION *def;                                            //DICTIONARY
  CString newprefix;                                          //DICTIONARY
  for (def=m_definitions; def!=NULL; def=def->m_next)         //DICTIONARY
  {                                                           //DICTIONARY
    if (def->publicUserFunc())                                //DICTIONARY
    {                                                         //DICTIONARY
      if (n==0)                                               //DICTIONARY
      {                                                       //DICTIONARY
        return prefix + def->m_name;                          //DICTIONARY
      };                                                      //DICTIONARY
      n--;                                                    //DICTIONARY
    };                                                        //DICTIONARY
  };                                                          //DICTIONARY
  //for (dict=m_offspring; dict!=NULL; dict=dict->m_next)     //DICTIONARY
  for (dict = m_offspring; dict != NULL;)                     //DICTIONARY
  {                                                           //DICTIONARY
    newprefix=prefix+dict->m_name+'@';                        //DICTIONARY
    return dict->GetPublicName(newprefix, n);                 //DICTIONARY
  };                                                          //DICTIONARY
  return CString("");                                         //DICTIONARY
}                                                             //DICTIONARY
                                                              //DICTIONARY
                                                              //DICTIONARY
int DICTIONARY::CountPublic(void)                             //DICTIONARY
{                                                             //DICTIONARY
  return m_countFunctions();                                  //DICTIONARY
}                                                             //DICTIONARY

//GLOBALS  globals;
bool AllFunctionsArePublic = false;





//********************************************************************
//
// Little  Little  Little  Little  Little  Little  Little  Little  
//
// Returns address of jump to be resolved later.              //Little
//static unsigned int compileJump(void);                      //Little
// Compile a jump for which destination is known;             //Little
//static void compileJump(unsigned int destination);          //Little
// Resolve a prior jump address.                              //Little
//static void resolveJump(unsigned int address);              //Little
                // Where the jump was located                 //Little
                                                              //Little
                                                              //Little
void GPDLCOMP::compileFalse(void) {                           //Little
  // Cause a false to appear on the top of the execution      //Little
  // stack.  Handy when you are about to call a               //Little
  // function and you want to put the default answer          //Little
  // on the stack before pushing the parameters.              //Little
  //static CString False="";                                  //Little
  //globals.insert(False)->addCodeReference();                //Little
  code.comma((SHIFTED_BINOP)|SUBOP_FALSE);                    //Little
}                                                             //Little
                                                              //Little
//static void compileOVER(void) {                             //Little
//  // Fetch second item on stack to stack.                   //Little
//  code.comma((SHIFTED_BINOP)|SUBOP_OVER);                   //Little
//}                                                           //Little
                                                              //Little
                                                              //Little
void GPDLCOMP::switchFillPrevCase(int& prevJumpCase) {        //Little
  // The previous $CASE statement could fail.                 //Little
  // If so, it needs to jump around the code                  //Little
  // associated with that case and go try                     //Little
  // the next case.  So at the end of each                    //Little
  // case-test a jump is left unresolved.                     //Little
  // Here we are going to                                     //Little
  // resolve the jump from the previous case                  //Little
  // and cause it to jump to the current case.                //Little
  if (prevJumpCase==0) return; // Nothing to do!              //Little
  resolveJump(prevJumpCase);                                  //Little
  prevJumpCase=0;                                             //Little
}                                                             //Little
                                                              //Little
int GPDLCOMP::switchCheckNoDefault(bool& Default) {           //Little
  // A case statement should have no $DEFAULT                 //Little
  // or $CASE following a $DEFAULT.  So if                    //Little
  // Default is true we will cause an error                   //Little
  // and return non-zero.                                     //Little
  if (Default) {                                              //Little
    infile.error("A $CASE or $DEFAULT follows a $DEFAULT");   //Little
    infile.error("Called from compileSwitchStatement",false); //Little
    return 1;                                                 //Little
  };                                                          //Little
  return 0;                                                   //Little
}                                                             //Little
                                                              //Little
unsigned int GPDLCOMP::switchCompileJumpOver() {              //Little
  // When a $CASE is encountered we have to make provision    //Little
  // for any code preceeding the $CASE to 'fall-through'      //Little
  // and continue executing.  This will occur whenever        //Little
  // a $BREAK is omitted either intentionally or (more        //Little
  // likely!) accidentally.  We compile the jump and          //Little
  // return its address so that you can resolve it            //Little
  // later when the $CASE testing is completed.               //Little
  return compileJump();                                       //Little
}                                                             //Little
                                                              //Little
void GPDLCOMP::switchCompileDUP(void) {                       //Little
  // Eash $CASE must test the switch variable against         //Little
  // a case expression.  Before each such test, we must       //Little
  // duplicate the switch variable on the stack.  That        //Little
  // is our task.                                             //Little
  code.comma((SHIFTED_BINOP)|SUBOP_DUP);                    //Little
        // It's an easy task!                                 //Little
}                                                             //Little
                                                              //Little
int GPDLCOMP::switchCompileExpression(void)                   //Little
{                                                             //Little
  // After the word '$CASE' there should appear an            //Little
  // expression giving the value for which we should          //Little
  // test.  It is our job to compile the code that            //Little
  // will leave the value of the expression on the            //Little
  // stack.                                                   //Little
  // Return non-zero if error.                                //Little
  int result;                                                 //Little
  result=compileExpression();                                 //Little
  if (result) {                                               //Little
    infile.error("Called from compileSwitchStatement");       //Little
    return result;                                            //Little
  };                                                          //Little
  return 0;                                                   //Little
}                                                             //Little
                                                              //Little
int GPDLCOMP::switchCheckForColon (void) {                    //Little
  // When 'compileSwitchStatement' expects a                  //Little
  // colon in the source code it calls us to                  //Little
  // do the checking.  We gobble up the colon                 //Little
  // and return zero if all is well.                          //Little
  // Otherwise we print an error message.                     //Little
  TOKENTYPE tokenType;                                        //Little
  tokenType=GetToken();                                       //Little
  if (tokenType==TKN_COLON) return 0;                         //Little
  infile.error("Expected colon following $CASE expression");  //Little
  return 1;                                                   //Little
}                                                             //Little
                                                              //Little
unsigned int GPDLCOMP::switchCompileJNE (void) {              //Little
  // At the end of each $CASE-test we want to                 //Little
  // jump to the next $CASE-test if this test fails.          //Little
  // Our job is to see if this test succeeds or               //Little
  // fails and, if it fails, to jump to the next              //Little
  // $CASE.  We don't know where the next $CASE will          //Little
  // be so we compile a jump and return its address           //Little
  // so you can resolve it when the next                      //Little
  // $CASE is encountered.                                    //Little
  unsigned int result;                                        //Little
  code.comma((SHIFTED_BINOP)|SUBOP_ISEQUAL);                  //Little
  result=code.here();                                         //Little
  code.comma(BINOP_JUMPFALSE<<24);                            //Little
  return result;                                              //Little
}                                                             //Little
                                                              //Little
unsigned int GPDLCOMP::switchCompileJumpGREP  (void) {        //Little
  // At the end of each $GCASE-test we want to                //Little
  // jump to the next $CASE-test if this test fails.          //Little
  // Our job is to see if this test succeeds or               //Little
  // fails and, if it fails, to jump to the next              //Little
  // $CASE.  We don't know where the next $CASE will          //Little
  // be so we compile a jump and return its address           //Little
  // so you can resolve it when the next                      //Little
  // $CASE is encountered.                                    //Little
  unsigned int result;                                        //Little
  code.comma((SHIFTED_BINOP)|SUBOP_SWAP);                     //Little
           // $GREP(pattern,string);                          //Little
  code.comma((SHIFTED_BINOP)|SUBOP_GREP);                     //Little
  result=code.here();                                         //Little
  code.comma(BINOP_JUMPFALSE<<24);                            //Little
  return result;                                              //Little
}                                                             //Little
                                                              //Little
void GPDLCOMP::switchFillCodeJump (int& prevCodeJump) {       //Little
  // Each section of code in a switch statement               //Little
  // may not have a $BREAK statement.  It should              //Little
  // 'fall-through' to the code in the following              //Little
  // $CASE section.  But it must jump around the              //Little
  // test for the following $CASE.  After the code for that   //Little
  // $CASE is compiled we must go back and resolve            //Little
  // the forward reference of the jump that was               //Little
  // compiled to bypass the $CASE testing.  See               //Little
  // compileJumpOver() to see how the jump was                //Little
  // constructed.                                             //Little
  if (prevCodeJump!=0) resolveJump(prevCodeJump);             //Little
  prevCodeJump=0;                                             //Little
}                                                             //Little
                                                              //Little
void GPDLCOMP::resolveJump(unsigned int address) {            //Little
  // A previous jump was in a forward direction               //Little
  // and the destination was not known at the time            //Little
  // the jump was compiled.  We now know that the destination //Little
  // is 'here'.  'address' is the address of the              //Little
  // jump's opcode.                                           //Little
  code.poke(address,code.peek(address)|code.here());          //Little
}                                                             //Little
                                                              //Little
int GPDLCOMP::respondGetPattern(void)                         //Little
{                                                             //Little
  // Called during compilation of $RESPOND to get the         //Little
  // pattern parameter.  It is the first parameter and        //Little
  // we make sure the open parenthsis is present.             //Little
  int result;                                                 //Little
  TOKENTYPE tokenType;                                        //Little
  tokenType=GetToken();                                       //Little
  if (tokenType!=TKN_OPENPAREN) {                             //Little
    infile.error(                                             //Little
      "Expected open parenthesis following '$RESPOND'");      //Little
    return 1;                                                 //Little
  };                                                          //Little
  result=compileExpression();                                 //Little
  if (result) {                                               //Little
    infile.error(                                             //Little
      "Called while expecting pattern in $RESPOND",false);    //Little
    return result;                                            //Little
  };                                                          //Little
  return 0;                                                   //Little
}                                                             //Little
                                                              //Little
int GPDLCOMP::respondCloseParen(void) {                       //Little
  // Make sure a close parenthesis comes at the end           //Little
  // of $RESPOND's parameter list.                            //Little
  TOKENTYPE tokenType;                                        //Little
  tokenType=GetToken();                                       //Little
  if (tokenType!=TKN_CLOSEPAREN) {                            //Little
    infile.error("Expected close parenthesis after $RESPOND's two parameters");
    return 1;                                                 //Little
  };                                                          //Little
  return 0;                                                   //Little
}                                                             //Little
                                                              //Little
int GPDLCOMP::respondSayText(void)                            //Little
{                                                             //Little
  // Called when we want to compile the expression            //Little
  // representing the text of a message to be sent            //Little
  // to the user as part of a $RESPOND command.               //Little
  int result;                                                 //Little
  result=compileExpression();                                 //Little
  if (result) {                                               //Little
    infile.error("Called from compileRespond...while expecting message text",false);
    return result;                                            //Little
  };                                                          //Little
  return 0;                                                   //Little
}                                                             //Little
                                                              //Little
int GPDLCOMP::respondCheckComma(void) {                       //Little
  // Called during compilation of $RESPOND when the comma     //Little
  // between the pattern and the text is expected.            //Little
  TOKENTYPE tokenType;                                        //Little
  tokenType=GetToken();                                       //Little
  if (tokenType!=TKN_COMMA) {                                 //Little
    infile.error("Expected a comma between pattern and text of $RESPOND arguments");
    return 1;                                                 //Little
  };                                                          //Little
  return 0;                                                   //Little
}                                                             //Little
                                                              //Little
unsigned int GPDLCOMP::compileJumpFalse(void) {               //Little
  // Compile a jump and remember the address of the           //Little
  // jump's destination word for later resolution.            //Little
  unsigned int address;                                       //Little
  address=code.here();                                        //Little
  code.comma(BINOP_JUMPFALSE<<24);                            //Little
  return address;                                             //Little
}                                                             //Little
                                                              //Little
unsigned int GPDLCOMP::compileJump(void) {                    //Little
  // Compile a jump and remember the address of the           //Little
  // jump's destination word for later resolution.            //Little
  unsigned int address;                                       //Little
  address=code.here();                                        //Little
  code.comma(BINOP_JUMP<<24);                                 //Little
  return address;                                             //Little
}                                                             //Little
                                                              //Little
void GPDLCOMP::compileJump(unsigned int destination) {        //Little
  // Compile a jump for which the dstination is already known.//Little
  code.comma((BINOP_JUMP<<24)|destination);                   //Little
}                                                             //Little
                                                              //Little

/*
#define ALL_USAGE      255
#define TALK_USAGE     1
#define SPELL_USAGE    2
#define LOGICBLK_USAGE 4
#define EVENT_USAGE    8
#define INTERNAL_USAGE 16

struct SYSTEMFUNCTION 
{
  char name[30];         
  int  numParam;         
  SUBOPS binaryCode;     
  unsigned char types[4]; (types[0] is the return type)
};   
*/





void GPDLCOMP::InitializeGPDLcompiler(void)
{
  //unsigned int i;
  //int j;
  //m_numSysFunc = sizeof systemfunctions/sizeof systemfunctions[0];
  CString fname, pname;
  //DEFINITION *def;
  m_compilingScript = false;
  code.Initialize();
  m_activeFunction=NULL;
  if (m_root != NULL) delete m_root;
  m_root = new DICTIONARY("root");
  if (m_globals != NULL) delete m_globals;
  m_globals = new GLOBALS;
  m_breaklist=0xffffffff;
  m_continuelist=0xffffffff;
  m_current=m_root;
  m_context=m_root;
  m_root->parent(&systemDictionary);
  /*
  for (i=0; i<m_numSysFunc; i++) 
  {
    fname=CString(systemfunctions[i].name);
    def=m_root->addFunction(fname);
    def->setIntValue((SHIFTED_BINOP)|systemfunctions[i].binaryCode);
    def->setSystem();
    pname="a";
    for (j=0; j<systemfunctions[i].numParam; j++) {
      def->addFormalParam(pname);
      pname+="a";
    };
    for (j=0; j<4; j++) def->type(j, systemfunctions[i].types[j]);
  };
  */
}


GPDLCOMP::GPDLCOMP(void)
{
  m_root = NULL;
  m_globals = NULL;
  m_availableContexts = 0xffffffff;
  InitializeGPDLcompiler();
}

GPDLCOMP::~GPDLCOMP(void)
{
  if (m_root != NULL) delete m_root;
  m_root = NULL;
  if (m_globals != NULL) delete m_globals;
  m_globals = NULL;
}


TOKENTYPE GPDLCOMP::GetToken(void)
{
  TOKENTYPE result;
  while ((result = infile.NextToken())  == TKN_PRAGMA)
  {
    if (infile.token() == "#PUBLIC")
    {
      AllFunctionsArePublic = true;
    };
  };
  return result;
}

//void compileBinaryCall(DEFINITION *def) {
//  code.comma(def->intValue());
//}

void GPDLCOMP::compilePOP(void) 
{
  code.comma((SHIFTED_BINOP)|SUBOP_POP);
}

void GPDLCOMP::addToBreakList (void) 
{
  // compile a cell and put it on the break list.
  code.poke(code.here()-1,code.peek(code.here()-1)|m_breaklist);
  m_breaklist=code.here()-1;
}

void GPDLCOMP::addToContinueList (void) 
{
  // compile a cell and put it on the continue list.
  code.poke(code.here()-1,code.peek(code.here()-1)|m_continuelist);
  m_continuelist=code.here()-1;
}

//inline UINT AFXAPI HashKey(CString& key)
//{
//  int len=key.GetLength();
//  if (len<5) {
//    int result=0;
//    for (int i=0; i<len; i++) result=result*23+key[i];
//    return result;
//  };
//  return ((((key[0]*23+key[1])*29+key[len-1])*47+key[len-2])*53+key[2])*57+len;
//};

void GPDLCOMP::discardCurrent(void) { // Pop the current dictionary and throw it away.
  DICTIONARY *temp;
  temp=m_current;
  m_current=m_current->parent();
  delete temp;
}


void GPDLCOMP::resolveBreaks(unsigned int addr) 
{
  // Fix up the entries in breaklist to point at addr
  unsigned int temp;
  while (m_breaklist!=0) {
    temp=m_breaklist;
    m_breaklist=code.peek(temp)&0xffffff;
    code.poke(temp,(code.peek(temp)&0xff000000)|addr);
  };
}

void GPDLCOMP::resolveContinues(unsigned int addr) 
{
  // Fix up the entries in continuelist to point at addr
  unsigned int temp;
  while (m_continuelist!=0) {
    temp = m_continuelist;
    m_continuelist=code.peek(temp)&0xffffff;
    code.poke(temp,(code.peek(temp)&0xff000000)|addr);
  };
}

int GPDLCOMP::compileVariableReference(DEFINITION *def) 
{
  if (def->FLG_FPrelative()) 
  {
    code.comma((BINOP_FETCH_FP<<24)|(def->intValue()&0xffffff));
    return 0;
  };
  if (def->FLG_GlobalVariable())
  {
    code.comma((BINOP_ReferenceGLOBAL<<24)|(def->intValue()&0xffffff));
    return 0;
  };
  infile.error("Reference to variable that is not Frame-Relative");
  return 1;
}

int GPDLCOMP::compileVariableStore(DEFINITION *def) 
{
  if (def->FLG_FPrelative())
  {
    code.comma((BINOP_STORE_FP<<24)|(def->intValue() & 0xffffff));
    return 0;
  }; 
  if (def->FLG_GlobalVariable())
  {
    code.comma((BINOP_ReferenceGLOBAL<<24)|(def->intValue() & 0xffffff) | 0x800000);
    return 0;
  }; 
  infile.error("Reference to variable that is not Frame-Relative");
  return 1;
}


int GPDLCOMP::compileForceNumeric(void) 
{
  code.comma((BINOP_SUBOP<<24)|SUBOP_FORCENUMERIC);
  return 0;
}




int GPDLCOMP::compileVariableDecl(void) {
  DEFINITION *newVar;
  int numvar;
  CString string;
  TOKENTYPE tokenType;
  if (infile.token() != "$VAR") 
  {
    infile.error("Internal compileVariableDecl error");
    return 1;
  };
  if (m_activeFunction == NULL)
  {  // We have a global variable in our midst.
     // We will treat it very much like a constant.
     // In fact, we will rename the 'constant' class to be the 'globals' class.
     // It will include constants AND global variables.
    GLOBAL *pGlobal;
    tokenType = GetToken();
    if (tokenType != TKN_NAME)
    {
      infile.error("Expected a variable name following '$VAR'");
      return 1;
    };
    newVar = m_context->addDefinition(infile.token());
    newVar->setGlobalVariable();
    pGlobal = m_globals->InsertVariable(newVar->name());
    newVar->setIntValue(pGlobal->Index());
    //m_activeFunction->addLocalVariable(string);
    //numvar = code.peek(code.here()-1) & 0x3ff;
    //newVar->setIntValue(-numvar);
    //newVar->setFPrelative();
    //newVar->setLocalVar();  // Declare it a local variable declaration
    return 0;
    //infile.error("compileVariableDecl not implemented outside of function definitions");
    //return 1;
  };
  if (code.here() == m_activeFunction->intValue()+1)
  { // The first variable to be defined in this function.  Plant a "BINOP_LOCALS"
    // to tell the runtime engine to allocate space on the stack.
    code.comma(BINOP_LOCALS<<24); // count of zero for now.
  };
  if (    (code.here() != m_activeFunction->intValue()+2)
       || (code.peek(code.here()-1) >> 24 != BINOP_LOCALS) )
  {
    infile.error("Variables must be declared before any executable code in a function");
    return 1;
  };
  code.poke(code.here()-1, code.peek(code.here()-1) + 1); // Add to local count
  tokenType=GetToken();
  if (tokenType != TKN_NAME)
  {
    infile.error("Expected a variable name following '$VAR'");
    return 1;
  };
  newVar = m_context->addDefinition(infile.token());
  string = infile.token();
  m_activeFunction->addLocalVariable(string);
  numvar = code.peek(code.here()-1) & 0x3ff;
  newVar->setIntValue(-numvar);
  newVar->setFPrelative();
  newVar->setLocalVar();  // Declare it a local variable declaration
  return 0;
}


int GPDLCOMP::compileAtomicElement(void) {
  // One element of an expression.
  // We allow implicit concatenation of two consecutive literal strings.
  TOKENTYPE tokenType;
  int result;
  DEFINITION *def;
  CString token;
  SUBOPS unaryOperator = SUBOP_ILLEGAL;
  tokenType = GetToken();
  // We allow only one unary operator.  Do you want more?
  if (tokenType == TKN_nMINUS)
  {
    unaryOperator = SUBOP_nNEGATE;
    tokenType = GetToken();
  };
  if (tokenType == TKN_OPENPAREN)
  {
    result = compileExpression();
    if (result != 0)
    {
      infile.error("Called from compileExpression",false);
      return result;
    };
    tokenType = GetToken();
    if (tokenType != TKN_CLOSEPAREN)
    {
      infile.error("Expected close parenthesis",true);
      return 1;
    };
  }
  else if (tokenType==TKN_STRING) {
    token="";
    while (tokenType==TKN_STRING) {
      token+=infile.token();
      tokenType=GetToken();
    };
    infile.backspaceToken();
    m_globals->InsertConstant(token)->addCodeReference();
  }
  else if (tokenType==TKN_INTEGER) {
    token.Format("%d",infile.integer());
    m_globals->InsertConstant(token)->addCodeReference();
  }
  else if (tokenType==TKN_NAME) {
    token=infile.token();
    def=m_context->localLookup(token);
    if (def==NULL) 
    {
      // Perhaps it is "true" or "false"
      if (token == "true")
      {
        token = "1";
        m_globals->InsertConstant(token)->addCodeReference();
      }
      else if (token == "false")
      {
        token = "";
        m_globals->InsertConstant(token)->addCodeReference();
      }
      else
      {
        infile.error("Undefined symbol in expression term");
        return 1;
      };
    }
    else if (def->FLG_function()) 
    {
      if (def->FLG_System())
      {
        if (def->type(0) != 0)
        {
          infile.error("Expected a function that returns a string value");
          return 1;
        };
      };
      result=compileFunctionCall(def);
      if (result) 
      {
        infile.error("Called from compileAtomicElement");
        return result;
      };
    }
    else
    {
      result=compileVariableReference(def);
      if (result) {
        infile.error("Called from compileAtomicElement");
        return result;
      };
    };
  }
  else
  {
    infile.error("Unrecognized syntax for a term in an expression");
    return 1;
  };
  if (unaryOperator != SUBOP_ILLEGAL)
  {
    code.comma ((SHIFTED_BINOP) | unaryOperator);
  };
  return 0;
}

struct OPERDEF
{
  int priority;
  TOKENTYPE tokenType;
  unsigned int opcode;
};

static OPERDEF operDef[] =
{
    {5, TKN_LOR,          (SHIFTED_BINOP)|SUBOP_LOR},
    {10,TKN_LAND,         (SHIFTED_BINOP)|SUBOP_LAND},
    {15,TKN_nOR,          (SHIFTED_BINOP)|SUBOP_nOR},
    {20,TKN_nXOR,         (SHIFTED_BINOP)|SUBOP_nXOR},
    {25,TKN_nAND,         (SHIFTED_BINOP)|SUBOP_nAND},
    {30,TKN_ISEQUAL,      (SHIFTED_BINOP)|SUBOP_ISEQUAL},
    {30,TKN_nISEQUAL,     (SHIFTED_BINOP)|SUBOP_nISEQUAL},
    {30,TKN_NOTEQUAL,     (SHIFTED_BINOP)|SUBOP_NOTEQUAL},
    {30,TKN_nNOTEQUAL,    (SHIFTED_BINOP)|SUBOP_nNOTEQUAL},
    {35,TKN_LESS,         (SHIFTED_BINOP)|SUBOP_LESS},
    {35,TKN_nLESS,        (SHIFTED_BINOP)|SUBOP_nLESS},
    {35,TKN_LESSEQUAL,    (SHIFTED_BINOP)|SUBOP_LESSEQUAL},
    {35,TKN_nLESSEQUAL,   (SHIFTED_BINOP)|SUBOP_nLESSEQUAL},
    {35,TKN_GREATER,      (SHIFTED_BINOP)|SUBOP_GREATER},
    {35,TKN_nGREATER,     (SHIFTED_BINOP)|SUBOP_nGREATER},
    {35,TKN_GREATEREQUAL, (SHIFTED_BINOP)|SUBOP_GREATEREQUAL},
    {35,TKN_nGREATEREQUAL,(SHIFTED_BINOP)|SUBOP_nGREATEREQUAL},
    {40,TKN_PLUS,         (SHIFTED_BINOP)|SUBOP_PLUS},
    {40,TKN_nPLUS,        (SHIFTED_BINOP)|SUBOP_nPLUS},
    {40,TKN_nMINUS,       (SHIFTED_BINOP)|SUBOP_nMINUS},
    {45,TKN_nGEAR,        (SHIFTED_BINOP)|SUBOP_nTIMES},
    {45,TKN_nSLASH,       (SHIFTED_BINOP)|SUBOP_nSLASH},
    {45,TKN_nPERCENT,     (SHIFTED_BINOP)|SUBOP_nPERCENT},

};


int GPDLCOMP::compileExpression(void)
{
  unsigned int opcodes[MAXSTACK];
  int priority[MAXSTACK];
  TOKENTYPE tokenTypes[MAXSTACK];
  int stackLen=0; // No entries yet.
  TOKENTYPE tokenType;
  int j, result;
  // Our strategy is:
  // 1) Compile an atomic element  (might be parenthesized expression)
  // 2) If next operator has higher priority than the top
  //    operator on the stack then put the new operator on the
  //    stack and go to 1)
  // 3) Perform the operation on the top of the stack.  Go to 2)
  //
  // First we must check for leading, unary operators.
  tokenType = GetToken();
  if (tokenType == TKN_NOT)
  {
    priority[stackLen] = 999;
    tokenTypes[stackLen] = tokenType;
    opcodes[stackLen] = (SHIFTED_BINOP)|SUBOP_NOT;
    stackLen++;
  }
  else if (tokenType == TKN_nMINUS)
  {
    priority[stackLen] = 999;
    tokenTypes[stackLen] = tokenType;
    opcodes[stackLen] = (SHIFTED_BINOP)|SUBOP_nNEGATE;
    stackLen++;
  }
  else
  {
    infile.backspaceToken();
  };
  for (;;)
  {
    result = compileAtomicElement();
    if (result != 0) break; // Prints error message.
    tokenType = GetToken();
    if (tokenType == TKN_COMMA) {infile.backspaceToken(); break;};
    if (tokenType == TKN_CLOSEPAREN) {infile.backspaceToken(); break;};
    if (tokenType == TKN_SEMICOLON) {infile.backspaceToken(); break;};
    if (tokenType == TKN_COLON) {infile.backspaceToken(); break;};
    // See if this is a legal operator.
    for (j=0; j<sizeof (operDef)/sizeof (operDef[0]); j++)
    {
      if (tokenType == operDef[j].tokenType) break;
    };
    if (j == sizeof(operDef)/sizeof(operDef[0]))
    {
      infile.error("Unknown operator symbol", true);
      return 1;
    };
    if (stackLen >= MAXSTACK-1)
    {
      infile.error("compileExpression stack overflow",true);
      return 1;
    };
    priority[stackLen] = operDef[j].priority;
    tokenTypes[stackLen] = operDef[j].tokenType;
    opcodes[stackLen] = operDef[j].opcode;
    stackLen++;
    // Now....while the penultimate operation on the stack
    // is greater or equal in priority to the top entry, perform
    // the penultimate operation and drop it from the  stack.
    while (   (stackLen > 1)
           && (priority[stackLen-2] >= priority[stackLen-1]) )
    {
      code.comma(opcodes[stackLen-2]);
      stackLen--;
      priority[stackLen-1] = priority[stackLen];
      opcodes[stackLen-1] = opcodes[stackLen];
      tokenTypes[stackLen-1] = tokenTypes[stackLen];
    };
  };
  // tokenType is the type of token that terminated the expression.
  if (result == 0)
  { //Empty the stack of any old pending operations
    while (stackLen > 0)
    {
      stackLen--;
      code.comma(opcodes[stackLen]);
    };
  }
  else
  {
    infile.error("Called from compileExpression",false);
  };
  return result;
}


int GPDLCOMP::compileVarAssignment(void)
{
  TOKENTYPE tokenType;
  DEFINITION *def;
  CString string;
  int result;
  tokenType = GetToken();
  if (tokenType!=TKN_NAME)
  {
    infile.error("Assignment statement starts badly");
    return 1;
  };
  string = infile.token();
  def = m_context->localLookup(string);
  if (def == NULL)
  {
    infile.error("Assignment statement starts with undefined variable name");
    return 1;
  };
  if (!def->FLG_FPrelative()  && !def->FLG_GlobalVariable())
  {
    infile.error("Attempt to assign value to non-variable");
    return 1;
  };
  tokenType = GetToken();
  if ((tokenType != TKN_EQUAL)  && (tokenType != TKN_nEQUAL))
  {
    infile.error("Assignment statement missing an equal sign");
    return 1;
  };
  result = compileExpression();
  if (result == 0)
  {
    if (tokenType == TKN_nEQUAL)
    {
      result = compileForceNumeric();
    }
    result = compileVariableStore(def);
  };
  if (result != 0)
  {
    infile.error("Called from CompileVarAssignment", false);
  };
  return result;
}


int GPDLCOMP::compileTypedSystemFunctionCall(int type)
{ // We are supposed to compile a single function call 
  // that returns data of the specified type.
  TOKENTYPE tokenType;
  tokenType = GetToken();
  CString emsg;
  CString token;
  DEFINITION *def;
  int result = 1;
  for (;;) // So we can easily have one place to print an error
  {
    if (tokenType != TKN_NAME)
    {
      emsg = "Expected a system function name";
      break;
    };
    token=infile.token();
    def=m_context->localLookup(token);
    if (def==NULL) {
      emsg = "Undefined symbol in system function parameter";
      break;
    };
    if ((def->m_requiredContexts & m_availableContexts) != def->m_requiredContexts)
    {
      emsg = "System Function expects a context that is not provided by this hook";
      break;
    };
    if (!def->FLG_function())
    {
      emsg = "Expected system function parameter to be a function call";
      break;
    };
    if (!def->FLG_System())
    {
      emsg = "Expected system function parameter to be a system function call";
      break;
    };
    if (def->type(0) != type)
    {
      emsg = "System function parameter not of correct data type";
      break;
    };
    result=compileFunctionCall(def);
    if (result) 
    {
      emsg = "Error compiling parameter to system function";
      break;
    };
    return 0;
  };
  CString msg;
  infile.error("Attempting to compile a parameter of a System Function.",false);

  switch (type)
  {
  case STRING:
    msg.Format("The parameter must be of type String");
    break;
  case ACTOR:
    msg.Format("The parameter must be of type Actor");
    break;
  default:
    msg.Format("The parameter must be of type %i", type);
    break;
  }
  
  infile.error(msg, false);
  infile.error(emsg,true);
  return result;
}

int GPDLCOMP::compileActualParameters(DEFINITION *def ) 
{
  int result;
  int parameterNumber;
  TOKENTYPE tokenType;
  DEFINITION *pactual;
  tokenType=GetToken();
  if (tokenType!=TKN_OPENPAREN) {
    infile.error("Expected open parenthesis before actual parameters");
    return 1;
  };
  pactual=def->firstParam();
  parameterNumber = 0;
  while(1) 
  {
    tokenType=GetToken();
    
    if (tokenType==TKN_CLOSEPAREN) 
      break;

    if (pactual==NULL) 
    {
      infile.error("Too many parameters");
      return 1;
    };

    infile.backspaceToken();

    if (def->FLG_System() && (def->type(parameterNumber+1) != 0))
    { 
      // We are compiling a system function with a requirement for
      // a particular type of parameter.  The parameter *MUST* be
      // a single system function call that returns the proper type.
      result = compileTypedSystemFunctionCall(def->type(parameterNumber+1));
    }
    else
    { 
      //Any old expression that results in a string will be OK.
      result=compileExpression();
    };

    if (result) 
    {
      infile.error("Called from compileActualParameters",false);
      return result;
    };

    pactual=pactual->next();
    tokenType=GetToken();

    if (tokenType==TKN_COMMA) 
    {
      parameterNumber++;
      continue;
    };
    
    if (tokenType==TKN_CLOSEPAREN) 
      break;

    infile.error("Unexpected actual parameter separator");
    return 1;
  };
  if (pactual!=NULL) {
    infile.error("Not enough parameters");
    return 1;
  };
  return 0;
}


int GPDLCOMP::compileWhile(void) 
{
  // We expect "(expression) { .... } .
  // We will save the old breaklist and continuelist
  //  and restore them when we are done.
  TOKENTYPE tokenType;
  int result;
  unsigned int doneJump;
  unsigned int continueAddr;
  CString blockname;
  SAVECONTINUELIST savecontinuelist(&m_continuelist);;
  SAVEBREAKLIST savebreaklist(&m_breaklist);
  tokenType=GetToken();
  if (tokenType!=TKN_OPENPAREN) {
    infile.error("Expected open parenthesis after $WHILE");
    return 1;
  };
  continueAddr=code.here();
  result=compileExpression();
  if (result) {
    infile.error("Called from compileWhile",false);
    return 1;
  };
  doneJump=compileJumpFalse();
  addToBreakList();
  tokenType=GetToken();
  if (tokenType!=TKN_CLOSEPAREN) {
    infile.error("Expected close parenthesis after $WHILE expression");
    return 1;
  };
  blockname=CString("");
  result=compileBlock(blockname);
  if (result) {
    infile.error("Called from compileWhile",false);
    return 1;
  };
  compileJump(continueAddr);
  resolveBreaks(code.here());
  resolveContinues(continueAddr);
  resolveJump(doneJump);
  return 0;
}

int GPDLCOMP::compileIf(void) 
{
  // We expect "(expression) { .... } .
  unsigned int jumpaddr, elseaddr; // To be filled in later
  TOKENTYPE tokenType;
  int result;
  CString blockname;
  tokenType=GetToken();
  if (tokenType!=TKN_OPENPAREN) {
    infile.error("Expected open parenthesis after $IF");
    return 1;
  };
  result=compileExpression();
  if (result) {
    infile.error("Called from compileIf",false);
    return 1;
  };
  jumpaddr=compileJumpFalse(); // Remember where the jump is!
  tokenType=GetToken();
  if (tokenType!=TKN_CLOSEPAREN) {
    infile.error("Expected close parenthesis after $IF expression");
    return 1;
  };
  tokenType=GetToken();
  if (tokenType!=TKN_OPENBRACE)
  {
    infile.error("Block following $IF must be enclosed in braces");
    return 1;
  };
  infile.backspaceToken();
  blockname=CString("");
  result=compileBlock(blockname);
  if (result) 
  {
    infile.error("Called from compileIf",false);
    return 1;
  };
  tokenType = GetToken();
  if (tokenType == TKN_NAME)
  {
    if (infile.token() == "$ELSE")
    {
      tokenType=GetToken();
      if (tokenType!=TKN_OPENBRACE)
      {
        infile.error("Block following $ELSE must be enclosed in braces");
        return 1;
      };
      infile.backspaceToken();
      blockname=CString("");
      elseaddr = compileJump(); //To jump around the else-part
      resolveJump(jumpaddr); //around the if-part
      result=compileBlock(blockname);
      if (result) 
      {
        infile.error("Called from compileIf - $ELSE block",false);
        return 1;
      };
      resolveJump(elseaddr);
      return 0;
    };
  };
  infile.backspaceToken();
  resolveJump(jumpaddr);
  return 0;
}

int GPDLCOMP::compileContinue(void) 
{
  compileJump();
  addToContinueList();
  return 0;
}

int GPDLCOMP::compileBreak(void) 
{
  compileJump();
  addToBreakList();
  return 0;
}

int GPDLCOMP::compileFunctionCall(DEFINITION *func)
{
  CString token;
  int result;
  if (!func->FLG_function()) {
    infile.error(CString("Name does not represent a function - ")+token);
    return 1;
  };
  if (!func->FLG_System()) compileFalse(); // Default result
  result=compileActualParameters(func);
  if (result) {
    infile.error("Called from compileFunctionCall",false);
    return result;
  };
  if (func->FLG_System()) 
  {
    code.comma(func->intValue());  // The binary opcode.
  } 
  else 
  {
    code.comma(func->intValue()|(BINOP_CALL<<24));  // The binary opcode.
  };
  return 0;
}



int GPDLCOMP::compileSwitch(void) 
{
  // First we expect to find the switch expression
  // buried inside of parentheses.
  SAVEBREAKLIST saveBreak(&m_breaklist);
  TOKENTYPE tokenType;
  int prevCaseJump, prevCodeJump;
  int result;
  CString token;
  bool Default, CaseSeen;
  tokenType=GetToken();
  if (tokenType!=TKN_OPENPAREN) {
    infile.error("Expected open parenthesis following '$SWITCH'");
    return 1;
  };
  result=compileExpression();  // The switch expression
  if (result) {
    infile.error("Called from compileSwitchStatement",false);
    return result;
  };
  tokenType=GetToken();
  if (tokenType!=TKN_CLOSEPAREN) {
    infile.error("Expected close parenthesis after switch expression");
    return 1;
  };
  // Now we compile each of the $CASE (or $GCASE) statements, and a 
  // possible $DEFAULT statement. 
  prevCaseJump=0;
  tokenType=GetToken();
  if (tokenType!=TKN_OPENBRACE) {
    infile.error("Expected open brace to start $CASE body");
    return 1;
  };
  Default=false;
  CaseSeen=false;
  while (1) {
    tokenType=GetToken();
    if (tokenType==TKN_CLOSEBRACE) break;
    if (tokenType==TKN_NAME) {
      token=infile.token();
      if (token=="$CASE" || token=="$GCASE") {
        // All of the steps are implemented 
        // in subroutines so there is some 
        // hope of seeing the big picture.
        //

        // $CASE cannot follow $DEFAULT
        result=switchCheckNoDefault(Default);   
        if (result) return result;

        // 'fall-through' jumps over test
        if (CaseSeen) // If there is possibly code active.
          prevCodeJump=switchCompileJumpOver();
        else prevCodeJump=0;

        // Make previous test jump to this one.
        switchFillPrevCase(prevCaseJump);

        // Duplicate the switch value
        switchCompileDUP();              

        // Compute case value
        result=switchCompileExpression();       
        if (result) return result;

        // Should be a colon after expression
        result=switchCheckForColon();           
        if (result) return result;

        // Test and jump to next case if this one fails.
        if (token=="$CASE") {
          prevCaseJump=switchCompileJNE();
        } else  { // Must ge a $GCASE
          prevCaseJump=switchCompileJumpGREP();
        };

        // So fallthrough can get around the $CASE test code
        switchFillCodeJump(prevCodeJump);
        CaseSeen=true;
        continue;
      };
      if (token=="$DEFAULT") {
        // Check that no default already exists.
        result=switchCheckNoDefault(Default);
        if (result) return result;
        // Arrange for previous test to jump to here if it fails.
        switchFillPrevCase(prevCaseJump);
        // Check that a colon follows '$DEFAULT'
        result=switchCheckForColon();
        if (result) return result;
        Default=true;
        continue;
      };
    };
    // Must be an executable statement.
    if (!CaseSeen) {
      infile.error("Switch statement should begin with $CASE or $GCASE");
      return 1;
    };
    infile.backspaceToken();
    result=compileStatement();
    if (result) {
      infile.error("Called from compileSwitchStatement",false);
      return result;
    };
  };
  // The sentinel '$ENDSWITCH' should come next.
  tokenType=GetToken();
  if (tokenType!=TKN_NAME
    || infile.token()!="$ENDSWITCH") {
    infile.error("Expected '$ENDSWITCH' sentinel at end of $SWITCH statement");
    return 1;
  };
  switchFillPrevCase(prevCaseJump); // In case there was no default.
  resolveBreaks(code.here());
  return 0;
}

int GPDLCOMP::compileReturn(void)
{
  TOKENTYPE tokenType;
  int result;
  int numParam, numLocals;
  if (m_activeFunction==NULL) {
    infile.error("$RETURN outside function body");
    return 1;
  };
  numParam=m_activeFunction->numParam();
  numLocals=m_activeFunction->numLocals();
  tokenType=GetToken(); // Is there a value to return?
  infile.backspaceToken();
  if (tokenType==TKN_SEMICOLON) {
    code.comma((BINOP_RETURN<<24)|(numLocals<<12)|numParam);  // All that is needed.
    return 0;
  };
  result=compileExpression();
  if (result) {
    infile.error("Called from compileReturn",false);
    return result;
  };
  code.comma((BINOP_STORE_FP<<24)|m_activeFunction->numParam()); // Store result.
  code.comma((BINOP_RETURN<<24)|(numLocals<<12)|numParam);
  return 0;
}

int GPDLCOMP::compileRespond(void) 
{
  int result;
  unsigned int jump;
  result=respondGetPattern(); // Open paren and the pattern to look for
  if (result) return result;
  code.comma((SHIFTED_BINOP)|SUBOP_LISTENTEXT);    // Fetch Listen Text
  code.comma((SHIFTED_BINOP)|SUBOP_GREP);          // See if pattern is in text
  jump=compileJumpFalse();// Save address for later resolution
  result=respondCheckComma(); // Comma between patter and text
  if (result) return result;
  result=respondSayText();    // Get text to $SAY
  code.comma((SHIFTED_BINOP)|SUBOP_SAY);           // Say the text
  code.comma((SHIFTED_BINOP)|SUBOP_POP);           // Discard $SAY's response.                     
  compileContinue();          // And add to continueList
  resolveJump(jump);          // Resolve the $IF jump
  result=respondCloseParen(); // End of parameters.
  if (result) return result;
  return 0;
}

int GPDLCOMP::compileStatement()
{
  TOKENTYPE tokenType;
  DEFINITION *def;
  CString token;
  CString tmperr;
  int result;
  // What kind of statement are we faced with?
  //   - Variable declaration    $VAR xxx, yyy, zzz;
  //   - Function declaration    $FUNC smoothie(k) {
  //   - Function call           doit("abc");
  //   - Assignment statement    x = 3;
  tokenType=GetToken();
  if (tokenType==TKN_NAME) {
    token=infile.token();
    if (token=="$VAR") result=compileVariableDecl();
    else 
    if (token=="$FUNC") result=compileFunctionDecl();
    else
    if (infile.token()=="$PUBLIC") result=compilePublicDecl();
    else
    if (token=="$BREAK") result=compileBreak();
    else
    if (token=="$WHILE") result=compileWhile();
    else
    if (token=="$CONTINUE") result=compileContinue();
    else
    if (token=="$SWITCH") result=compileSwitch();
    else 
    if (token=="$RETURN") result=compileReturn();
    else
    if (token=="$IF") result=compileIf();
    else
    if (token=="$RESPOND") result=compileRespond();
    else {
      // Perhaps is a function call or an assignment
      def=m_context->localLookup(token);
      if (def==NULL) 
      {
        CString msg;
        msg.Format("Undefined name \'%s\' at start of statement", token);
        infile.error(msg);
        return 1;
      };
      if (def->FLG_function())
      {
        result=compileFunctionCall(def);
        if (result==0) compilePOP();  // Remove result from the stack.
      }
      else // Maybe it is a parameter or variable assignment
      if (def->FLG_FPrelative())
      {
        infile.backspaceToken();
        result = compileVarAssignment();
      }
      else
      if (def->FLG_GlobalVariable())
      {
        infile.backspaceToken();
        result = compileVarAssignment();
      }
      else
      {
        tmperr.Format("Unknown token \'%s\' at start of statement", token);
        infile.error(tmperr);
        return 1;
      };
    };
    if (result) {
      infile.error("Called from compileStatement",false);
      return result;
    };

    tokenType=GetToken();
    if (tokenType!=TKN_SEMICOLON) 
    {
      tmperr.Format("Expected semi-colon at end of statement: \'%s\'", token);
      infile.error(tmperr);
      return 1;
    };
    return 0;
  } 
  else 
  {    
    tmperr.Format("Statement starts badly: \'%s\'", token);
    infile.error(tmperr);
    return 1;
  };
}

int GPDLCOMP::compileBlock(CString& blockname) 
{
  TOKENTYPE tokenType;
  int result;
  tokenType=GetToken();
  if (tokenType!=TKN_OPENBRACE) {
    infile.error("Expected open brace to start a code block");
    return 1;
  };
  while ((tokenType=GetToken())!=TKN_CLOSEBRACE) {
    infile.backspaceToken();
    result=compileStatement();
    if (result) {
      infile.error("Called from compileBlock",false);
      return result;
    };
  };
  if (blockname!="") {
    tokenType=GetToken();
    if ((tokenType!=TKN_NAME) || (infile.token()!=blockname)) {
      infile.error(CString("Expected ending block name - ")+blockname);
      return 1;
    };
  };
  return 0;
}

void GPDLCOMP::defineFormalParameters(DEFINITION *func, DICTIONARY *dict) 
{
  // Each of the formal parameters in the function definition
  // must be defined in the dictionary *dict.
  int paramOffset=0;//func->numParam();
  DEFINITION *param;
  DEFINITION *def;
  for (param=func->firstParam(); param!=NULL; param=param->next()) {
    def=dict->addDefinition(param->name());
    def->setFPrelative();
    def->setIntValue(paramOffset++);
  };
}

int GPDLCOMP::compilePublicDecl(void)
{
  TOKENTYPE tokenType;
  CString token;
  tokenType=GetToken();
  if (tokenType!=TKN_NAME  ||
    infile.token()!="$FUNC") {
    infile.error("Expected \"$FUNC\" following \"$PUBLIC\"");
    return 1;
  };
  return compileFunctionDecl(ATTRIBUTE_PUBLIC);
}

int GPDLCOMP::compileFunctionDecl(unsigned int attributes) 
{
  TOKENTYPE tokenType;
  CString token;
  unsigned int jump=0xffffffff;
  bool proto; // does a previous prototype exist?
  int param;  // number of parameters in parameter list.
  DEFINITION *newFunc;
  DEFINITION *def;
  DEFINITION *formalParam=NULL;
  SAVECURRENT saveCurrent(&m_current);  // In case we change these,
  SAVECONTEXT saveContext(&m_context);  //  we want them put back!
  int result;
  if (AllFunctionsArePublic) attributes |= ATTRIBUTE_PUBLIC;
  // The first thing we expect to find is the function name.
  tokenType=GetToken();
  token=infile.token();
  if (tokenType!=TKN_NAME) {
    infile.error ("Expected to find the name of a function");
    return 1;
  };
  def=m_current->localLookup(token);
  if (def==NULL) proto=false; // No previous prototype exists.
  else {
    if (def->FLG_proto()) proto=true; // Previous prototype does exist;
    else {
      infile.error("Function already defined");
      return 1;
    };
  };
  newFunc=m_current->addFunction(token); // Add function to current dictionary.
  m_current=m_current->addDictionary(token);
  tokenType=GetToken();
  if (tokenType!=TKN_OPENPAREN) {
    infile.error("Expected open parenthesis after function name");
    return 1;
  };
  // Add a formal parameter for each comma-delimited parameter between
  // parentheses.
  param=0;
  while (1) {
    tokenType=GetToken();
    if (tokenType==TKN_CLOSEPAREN) break;
    if (tokenType!=TKN_NAME) {
      infile.error("Expected a formal parameter name");
      return 1;
    };
    token=infile.token();
    if (proto) 
    {
      if (newFunc->checkProtoParam(param)) 
      {
        infile.error("prototype has fewer parameters");
        return 1;
      };
    }
    else formalParam=newFunc->addFormalParam(token);
    param++;
    tokenType=GetToken();
    if (tokenType==TKN_COMMA) continue;
    if (tokenType==TKN_EQUAL) {
      if (proto) {
        infile.error("Default value should be specified in prototype");
        return 1;
      };
      tokenType=GetToken();
      if (tokenType!=TKN_STRING) {
        infile.error("Expected literal string as default value of parameter");
        return 1;
      };
      token=infile.token();
      ASSERT(formalParam != NULL);
      formalParam->setDefaultValue(token);
      tokenType=GetToken();
      if (tokenType==TKN_COMMA) continue;
    };
    if (tokenType==TKN_CLOSEPAREN) break;
    infile.error("Expected comma or close parenthesis at end of formal parameters");
    return 1;
  };
  if (attributes & ATTRIBUTE_PUBLIC) newFunc->setPublic();
  // Now should come an open brace or a semi-colon;
  if (proto) {
    if (newFunc->checkProtoParam(param)==0) {
      infile.error("Prototype had more parameters");
      return 1;
    };
  };
  tokenType=GetToken();
  if (tokenType==TKN_SEMICOLON) {
    if (proto) {
      infile.error("Function prototype already declared");
      return 1;
    };
    newFunc->setProto();
    discardCurrent();  // Get rid of dictionary for now.
    return 0;
  };
  if (tokenType!=TKN_OPENBRACE) {
    infile.error("Expected open brace or semi-colon following function parameters");
    return 1;
  };
  infile.backspaceToken(); 
  if (m_activeFunction!=NULL) 
    jump=compileJump(); // Save address for later resolution
  token=newFunc->name();
  newFunc->setIntValue(code.here());
  {
    // We need to move the formal parameter names to 
    // the current dictionary.
    defineFormalParameters(newFunc, m_current);
    SAVEACTIVEFUNCTION saveActiveFunction (newFunc, &m_activeFunction);
    m_context=m_current;
    CString entryDebug; // This is a debugging marker just below FP.
                        // The name of the function and (number of parameters)
    if (m_compilingScript)
    {
      entryDebug = "";
    }
    else
    {
      entryDebug.Format("%s(%d)",newFunc->name(),newFunc->numParam());
    };
    m_globals->InsertConstant(entryDebug)->addCodeReference();
    result=compileBlock(token);
    if (result) {
      infile.error("Called from compileFunctionDecl",false);
      return result;
    };
    // Put a default $RETURN at the end.
    compileReturn();
  };
  if (m_activeFunction!=NULL) 
  {
    ASSERT(jump != 0xffffffff);
    resolveJump(jump);
  };
  if (result) {
    infile.error("Called from compileFunction",false); // Compile block for this function.
    return result;
  };
  return 0;
}

int GPDLCOMP::CompileProgram
      (const char *GetOneLine(int n), 
       int ProcessErrorMessage(const CString &errMsg, bool wait),
       bool compileScript)
{
  TOKENTYPE tokenType;
  int result;
  InitializeGPDLcompiler();
  m_compilingScript = compileScript;
  infile.Initialize(GetOneLine, ProcessErrorMessage);
  code.comma((SHIFTED_BINOP)|SUBOP_NOOP);  // So program will not start at zero!
  while ((tokenType=GetToken())!=TKN_NONE) 
  {
    if (tokenType==TKN_NAME) { // Must be a function declaration or a 
                           //  variable declaration.
      if (infile.token()=="$PUBLIC") {
        result=compilePublicDecl();
        if (result==0) {
          tokenType=GetToken();
          if (tokenType!=TKN_SEMICOLON) {
            infile.error("Expected semi-colon after $PUBLIC statement");
            result=1;
          };
        };
      } else 
      if (infile.token()=="$FUNC") {
        result=compileFunctionDecl();
        if (result==0) {
          tokenType=GetToken();
          if (tokenType!=TKN_SEMICOLON) {
            infile.error("Expected semi-colon after function definition");
            result=1;
          };
        };
      } else 
      if (infile.token()=="$VAR") {
        result=compileVariableDecl();
        if (result==0) {
          tokenType=GetToken();
          if (tokenType!=TKN_SEMICOLON) {
            infile.error("Expected semi-colon after variable declaration");
            result=1;
          };
        };
      } else {
        infile.error("Illegal statement at global level");
        result=1;
      }
    } else {
      infile.error("Illegal statement type at global level");
      result=1;
    };
    if (result) 
    {
      return 1;
    }
  };
  return 0;
}

static const char *scriptText = "";
static CString scriptError;

static const char *GetScriptLine(int /*n*/)
{
  const char *result;
  result = scriptText;
  scriptText = "";
  return result;
}

static int ProcessScriptError(const CString& msg, bool /*wait*/)
{
  scriptError += msg;
  scriptError += '\n';
  return 0;
}
  

struct BUILTINLIT  // Built-in Literal
{
  char    *lit;
  SUBOPS  subop;
};

BUILTINLIT builtinLit[] =
{
  {"", SUBOP_FALSE},  //also used for false
  {"0", SUBOP_ZERO},
  {"1", SUBOP_ONE}    //also used for true
};


CString GPDLCOMP::CompileScript(const CString& script, const char *entryPoints[])
{
  //If the first byte of the result is non-zero then it is an error message.
  int i, j;
  int headerOffset, entryPointOffset, codeOffset, constantsOffset;
  int numEntry;
  int numGlobalVar, numNeededGlobalVar;
  int totalSizeOfGlobalVars;
  int binaryCodeSize;
  int pGlobal;
  int entryAddress;
  int codeLen;
  int xlate, opcode;
  char *binaryCode;
  CString globalVar, entryName, emptyString;
  emptyString = "";
  scriptError = "";
  scriptText = LPCTSTR(script);
  CompileProgram(GetScriptLine, ProcessScriptError, true);
  if (scriptError.GetLength() > 0)
  {
    if (scriptError[0] == 0)
    {
      scriptError = CString("%") + scriptError;
    };
    return scriptError;
  };
  numGlobalVar = m_globals->m_used;
  numNeededGlobalVar = 0;
  // Actually there are one fewer than this because zero is not used.
  totalSizeOfGlobalVars = 0;
  for (i=1; i<numGlobalVar; i++)
  {
    globalVar = m_globals->getValue(i);
    for (j=0; j<sizeof (builtinLit)/sizeof (BUILTINLIT); j++)
    {
      if (    builtinLit[j].lit == globalVar) break;
    };
    if (j == sizeof (builtinLit)/sizeof (BUILTINLIT))
    {
      totalSizeOfGlobalVars += globalVar.GetLength() + 1;
      numNeededGlobalVar++;
    };
  };
  for (numEntry=0; entryPoints[numEntry]!=NULL; numEntry++);
  binaryCodeSize = 
         4*numGlobalVar        // Constant translation table..not part of binary
       + 1                     // a leading zero
       + 3                     // Number of entry points
       + 4                     // Size of code (# of ints)
       + 4                     // Size of data (# of bytes)
       + 4*numEntry            // Entry Point Table 
       + 4*code.here()         // The code itself
       + totalSizeOfGlobalVars // The data itself
       ;
  binaryCode = (char *)alloca(binaryCodeSize);
  headerOffset = 4*numGlobalVar;
  entryPointOffset = headerOffset + 12;
  codeOffset = entryPointOffset + 4*numEntry;
  constantsOffset = codeOffset + 4*code.here();
  binaryCode[headerOffset] = 0;  // This ain't no error message, fella!
  *(int *)(binaryCode+headerOffset+1) = numEntry;
  for (i=0; i<numEntry; i++)
  {
    entryName = CString(entryPoints[i]);
    entryAddress = m_root->m_findPublicFunction(emptyString,entryName);
    if (entryAddress <= 0)
    {
      scriptError = "Undefined function name = ";
      scriptError += entryPoints[i];
      return scriptError;
    };
    ((int *)(binaryCode+entryPointOffset))[i] = entryAddress;
  };
  //Build the translation table to convert CString references
  // to text string references.
  // Convert the CStrings to text strings.
  pGlobal = 4*code.here();
  for (i=0; i<numGlobalVar; i++)
  {
    globalVar = m_globals->getValue(i);
    for (j=0; j<sizeof (builtinLit)/sizeof (BUILTINLIT); j++)
    {
      if (builtinLit[j].lit == globalVar) break;
    };
    if (j == sizeof (builtinLit)/sizeof (BUILTINLIT))
    {
      strcpy(binaryCode + codeOffset + pGlobal, LPCTSTR(globalVar));
      ((int *)(binaryCode))[i] = pGlobal;
      pGlobal += globalVar.GetLength() + 1;
    }
    else
    {
      ((int *)(binaryCode))[i] 
                 = builtinLit[j].subop + (SHIFTED_BINOP);
    };
  };
  *(int *)(binaryCode + headerOffset + 8) = totalSizeOfGlobalVars;
  codeLen = code.here();
  *(int *)(binaryCode + headerOffset + 4) = codeLen;
  // Now we go through the code and change all BINOP_FETCHCONSTANT
  // to either a SystemFunction call or a BINOP_FETCHTEXT.
  for (i=0; i<codeLen; i++)
  {
    opcode = code.peek(i);
    if (((opcode>>24) & 0xff) == BINOP_ReferenceGLOBAL)
    {
      // Look in the translate table to see what to do.
      xlate = ((int *)(binaryCode))[opcode&0xffffff]; 
      if (((xlate >> 24) & 0xff) == BINOP_SUBOP)
      {
        ((int *)(binaryCode+codeOffset))[i] = xlate;
      }
      else
      {
        ((int *)(binaryCode+codeOffset))[i] 
                = (BINOP_FETCHTEXT<<24) + xlate;
      };
    }
    else
    { // Just copy it as it is.
      ((int *)(binaryCode+codeOffset))[i] = opcode;
    };
  };
  return CString(binaryCode+headerOffset,
                 12 + 4*(codeLen+numEntry) + totalSizeOfGlobalVars);
}

void GPDLCOMP::WriteDictionary(CArchive& outarchive)
{
  m_root->write(outarchive);
}

void GPDLCOMP::WriteConstants(CArchive& outarchive)
{
  m_globals->write(outarchive);
}

void GPDLCOMP::WriteCode(CArchive& outarchive)
{
  if (code.peek(0)!=((SHIFTED_BINOP)|SUBOP_NOOP)) 
  {
    infile.error("Internal error.  Address zero has been tampered with.");
    //Sleep(3000);
    exit(1);
  };
  code.write(outarchive);
}

unsigned int GPDLCOMP::GetCodeSize(void)
{
  return code.here();
}

unsigned int *GPDLCOMP::GetCodeAddress(void)
{
  return code.codeAddress();
}

CString GPDLCOMP::constantValue(int n)
{
  return m_globals->getValue(n);
}

unsigned int GPDLCOMP::constantsSize(void)
{
  return m_globals->m_used;
}

int GPDLCOMP::indexValue(int n)
{
  return m_root->GetPublicValue(n);
}

CString GPDLCOMP::indexName(int n)
{
  return m_root->GetPublicName(CString(""), n);
}

unsigned int GPDLCOMP::indexSize(void)
{
  return m_root->CountPublic();
}

//**************************************************************************
//
//  LIST   LIST   LIST   LIST   LIST   LIST   LIST   LIST   LIST   LIST   
struct MNEUMONIC {                                            //LIST
  BINOPS  m_opcode;                                           //LIST
  char  m_mneumonic[20];                                      //LIST
};                                                            //LIST
struct SUBMNEUMONIC {                                         //LIST
  SUBOPS  m_subop;                                            //LIST
  char    m_mneumonic[20];                                    //LIST
  BYTE    m_UsageType;                                        //LIST
};                                                            //LIST
static MNEUMONIC mneumonics[] = {                             //LIST
  {BINOP_CALL,            "call"      },                      //LIST
  {BINOP_ReferenceGLOBAL, "FetchGlobal" },                    //LIST
  {BINOP_JUMP,            "Jump"      },                      //LIST
  {BINOP_JUMPFALSE,       "JumpFalse"   },                    //LIST
  {BINOP_STORE_FP,        "Store_FP"    },                    //LIST
  {BINOP_FETCH_FP,        "Fetch_FP"    },                    //LIST
  {BINOP_RETURN,          "$RETURN"   },                      //LIST
  {BINOP_LOCALS,          "AllocateLocals"},                  //LIST
  {BINOP_SUBOP,           "?????"     } // Must come last     //LIST
};                                                            //LIST
static SUBMNEUMONIC submneumonics[] = {                       //LIST
  {SUBOP_DUP,             "Dup"  , INTERNAL_USAGE   },        //LIST
  {SUBOP_ISEQUAL,         "=="   , ALL_USAGE        },        //LIST
  {SUBOP_LESS,            "<"    , ALL_USAGE        },        //LIST
  {SUBOP_LESSEQUAL,       "<="   , ALL_USAGE        },        //LIST
  {SUBOP_GREATER,         ">"    , ALL_USAGE        },        //LIST
  {SUBOP_GREATEREQUAL,    ">="   , ALL_USAGE        },        //LIST
  {SUBOP_nISEQUAL,        "==#"  , ALL_USAGE        },        //LIST
  {SUBOP_nNOTEQUAL,       "!=#"  , ALL_USAGE        },        //LIST
  {SUBOP_NOTEQUAL,        "!="   , ALL_USAGE        },        //LIST
  {SUBOP_nLESS,           "<#"   , ALL_USAGE        },        //LIST
  {SUBOP_nLESSEQUAL,      "<=#"  , ALL_USAGE        },        //LIST
  {SUBOP_nMINUS,          "-#"   , ALL_USAGE        },        //LIST
  {SUBOP_nGREATER,        ">#"   , ALL_USAGE        },        //LIST
  {SUBOP_nGREATEREQUAL,   ">=#"  , ALL_USAGE        },        //LIST
  {SUBOP_nTIMES,          "*#"   , ALL_USAGE        },        //LIST
  {SUBOP_nSLASH,          "/#"   , ALL_USAGE        },        //LIST
  {SUBOP_nPERCENT,        "%#"   , ALL_USAGE        },        //LIST
  {SUBOP_nPLUS,           "+#"   , ALL_USAGE        },        //LIST
  {SUBOP_nNEGATE,         "nNegate", INTERNAL_USAGE },        //LIST
  {SUBOP_nAND,            "&#"   , ALL_USAGE        },        //LIST
  {SUBOP_nOR,             "|#"   , ALL_USAGE        },        //LIST
  {SUBOP_nXOR,            "^#"   , ALL_USAGE        },        //LIST
  {SUBOP_NOOP,            "Noop" , INTERNAL_USAGE   },        //LIST
  {SUBOP_LAND,            "&&"   , ALL_USAGE        },        //LIST
  {SUBOP_LOR,             "||"   , ALL_USAGE        },        //LIST
  {SUBOP_POP,             "Pop"  , INTERNAL_USAGE   },        //LIST
  {SUBOP_PLUS,            "$PLUS", ALL_USAGE        },        //LIST
  {SUBOP_OVER,            "Over" , INTERNAL_USAGE   },        //LIST
  {SUBOP_SWAP,            "Swap" , INTERNAL_USAGE   },        //LIST
  {SUBOP_ILLEGAL,         "?????", INTERNAL_USAGE   },        //LIST
  {SUBOP_FORCENUMERIC,    "numeric",INTERNAL_USAGE  }         //LIST
};                                                            //LIST
static unsigned int numSubMneumonic                           //LIST
           = sizeof submneumonics/sizeof SUBMNEUMONIC;        //LIST
void GPDLCOMP::list(FILE *file) {                             //LIST
  unsigned int i, k;                                          //LIST
  unsigned int skip;                                          //LIST
  unsigned int bincode;                                       //LIST
  CString funcName;                                           //LIST
  CString operand;                                            //LIST
  CString dblquote="\"";                                      //LIST
  char *mneumonic;                                            //LIST
  int opcode, subop;                                          //LIST
  skip=0;                                                     //LIST
  for (k=0; k<code.here(); k++) {                             //LIST
    funcName=m_root->findUserFunc(k);                         //LIST
    if (funcName!="") {                                       //LIST
      fprintf(file,"%s\n",(LPCSTR)funcName);                          //LIST
    };                                                        //LIST
    bincode=code.peek(k);                                     //LIST
    fprintf(file,"        %06x %02x %06x ",                   //LIST
                 k, bincode>>24, bincode&0xffffff);           //LIST
    if (skip) {                                               //LIST
      fprintf(file,"\n");                                     //LIST
      skip--;                                                 //LIST
    }                                                         //LIST
    else {                                                    //LIST
      opcode=bincode>>24;                                     //LIST
      subop=bincode&0xffffff;                                 //LIST
      operand="";                                             //LIST
      for (i=0;                                               //LIST
           i<sizeof mneumonics/sizeof mneumonics[0]-1;        //LIST
           i++) {                                             //LIST
        if (opcode==mneumonics[i].m_opcode) break;            //LIST
      };                                                      //LIST
      mneumonic=mneumonics[i].m_mneumonic;                    //LIST
      switch (opcode) {                                       //LIST
      case BINOP_CALL:                                        //LIST
        operand=m_root->findUserFunc(subop);                  //LIST
        break;                                                //LIST
      case BINOP_ReferenceGLOBAL:                             //LIST
        if (bincode & 0x800000)                               //LIST
        {                                                     //LIST
          mneumonic = "StoreGlobal";                          //LIST
        };                                                    //LIST
        if (m_globals->getType(bincode & 0x7fffff) == 'C')    //LIST
        {                                                     //LIST
          operand=dblquote                                    //LIST
                   +m_globals->getValue(bincode&0x7fffff)     //LIST
                   +dblquote;                                 //LIST
        }                                                     //LIST
        else                                                  //LIST
        {                                                     //LIST
          operand = m_globals->getValue(bincode&0x7fffff);    //LIST
        };                                                    //LIST
        operand=operand.Left(30);                             //LIST
        break;                                                //LIST
      case BINOP_JUMP:                                        //LIST
      case BINOP_STORE_FP:                                    //LIST
      case BINOP_FETCH_FP:                                    //LIST
      case BINOP_RETURN:                                      //LIST
      case BINOP_LOCALS:                                      //LIST
      case BINOP_JUMPFALSE: operand=""; break;                //LIST
      case BINOP_SUBOP:                                       //LIST
        for (i=0; i<numSysFunc; i++) {                        //LIST
          if ( subop==systemfunctions[i].binaryCode) break;   //LIST
        };                                                    //LIST
        if (i==numSysFunc)                                    //LIST
        {                                                     //LIST
          for (i=0; i<numSubMneumonic-1; i++)                 //LIST
          {                                                   //LIST
            if (subop == submneumonics[i].m_subop) break;     //LIST
          };                                                  //LIST
          mneumonic = submneumonics[i].m_mneumonic;           //LIST
        }                                                     //LIST
        else mneumonic=systemfunctions[i].name;               //LIST
        break;                                                //LIST
      default:                                                //LIST
        operand="?????";                                      //LIST
      };                                                      //LIST
      fprintf(file,"%s %s\n",mneumonic, (LPCSTR)operand);     //LIST
    };                                                        //LIST
  };                                                          //LIST
}                                                             //LIST



                                                                        //RDR
struct COPERDEF                                                         //RDR
{                                                                       //RDR
  int priority;                                                         //RDR
  CTOKENTYPE tokenType;                                                 //RDR
  COPS opcode;                                                          //RDR
  char *name;                                                           //RDR
};                                                                      //RDR
                                                                        //RDR
static COPERDEF CoperDef[] =                                            //RDR
{                                                                       //RDR
    {2, CTKN_MIN,          COP_MIN,           "|<" },                   //RDR
    {2, CTKN_MAX,          COP_MAX,           ">|" },                   //RDR
    {5, CTKN_LOR,          COP_LOR,           "||" },                   //RDR
    {7, CTKN_LXOR,         COP_LXOR,          "^^" },                   //RDR
    {10,CTKN_LAND,         COP_LAND,          "&&" },                   //RDR
    {15,CTKN_BITOR,        COP_BITOR,         "|"  },                   //RDR
    {20,CTKN_BITXOR,       COP_BITXOR,        "^"  },                   //RDR
    {25,CTKN_BITAND,       COP_BITAND,        "&"  },                   //RDR
    {30,CTKN_ISEQUAL,      COP_ISEQUAL,       "==" },                   //RDR
    {30,CTKN_NOTEQUAL,     COP_ISNOTEQUAL,    "!=" },                   //RDR
    {35,CTKN_LESS,         COP_LESS,          "<"  },                   //RDR
    {35,CTKN_LESSEQUAL,    COP_LESSEQUAL,     "<=" },                   //RDR
    {35,CTKN_GREATER,      COP_GREATER,       ">"  },                   //RDR
    {35,CTKN_GREATEREQUAL, COP_GREATEREQUAL,  ">=" },                   //RDR
    {40,CTKN_PLUS,         COP_PLUS,          "+"  },                   //RDR
    {40,CTKN_MINUS,        COP_MINUS,         "-"  },                   //RDR
    {45,CTKN_GEAR,         COP_TIMES,         "*"  },                   //RDR
    {45,CTKN_SLASH,        COP_SLASH,         "/"  },                   //RDR
    {45,CTKN_PERCENT,      COP_PERCENT,       "%"  },                   //RDR
    { 0,CTKN_OPENPAREN,    COP_ILLEGAL,       "("  },                   //RDR
    { 0,CTKN_CLOSEPAREN,   COP_ILLEGAL,       ")"  }                    //RDR
};                                                                      //RDR
                                                                        //RDR
bool RDRCOMP::m_whitespace(char c) {                                    //RDR
  if (c==' ')  return true;                                             //RDR
  if (c=='\t') return true;                                             //RDR
  return false;                                                         //RDR
}                                                                       //RDR
                                                                        //RDR
RDRCOMP::RDRCOMP(void)                                                  //RDR
{                                                                       //RDR
  m_code = NULL;                                                        //RDR
  m_Initialize();                                                       //RDR
}                                                                       //RDR
                                                                        //RDR
RDRCOMP::~RDRCOMP(void)                                                 //RDR
{                                                                       //RDR
  if (m_code != NULL) My_free(m_code);                                  //RDR
  m_code = NULL;                                                        //RDR
}                                                                       //RDR
                                                                        //RDR
void RDRCOMP::m_Initialize(void)                                        //RDR
{                                                                       //RDR
  m_inputIndex = 0;                                                     //RDR
  m_codeIndex = 0;                                                      //RDR
  if (m_code != NULL) My_free(m_code);                                  //RDR
  m_code = NULL;                                                        //RDR
  m_backspaceTkn = CTKN_NONE;                                           //RDR
  m_errorMsg = "";                                                      //RDR
}                                                                       //RDR
                                                                        //RDR
CString RDRCOMP::m_Result(void)                                         //RDR
{                                                                       //RDR
  CString temp = "";                                                    //RDR
  if (m_code == NULL) return CString("");                               //RDR
  return CString(m_code,m_codeIndex);                                   //RDR
}                                                                       //RDR
                                                                        //RDR
                                                                        //RDR
void RDRCOMP::m_backspace(void) {                                       //RDR
  m_inputIndex--;                                                       //RDR
}                                                                       //RDR
                                                                        //RDR
                                                                        //RDR
void RDRCOMP::m_backspaceToken(void) {                                  //RDR
  if (m_backspaceTkn!=CTKN_NONE) {                                      //RDR
    MessageBox(NULL,                                                    //RDR
             "Internal error in backspaceToken",                        //RDR
           NULL,                                                        //RDR
           MB_OK|MB_TOPMOST);                                           //RDR
    exit(1);                                                            //RDR
  };                                                                    //RDR
  m_backspaceTkn=m_latestTkn;                                           //RDR
};                                                                      //RDR
                                                                        //RDR
                                                                        //RDR
bool RDRCOMP::m_initialChar(char c) {                                   //RDR
  if (c=='$') return true;                                              //RDR
  if (c=='_') return true;                                              //RDR
  if (c<'A') return false;                                              //RDR
  if (c<='Z') return true;                                              //RDR
  if (c<'a') return false;                                              //RDR
  if (c<='z') return true;                                              //RDR
  return false;                                                         //RDR
}                                                                       //RDR
                                                                        //RDR
bool RDRCOMP::m_moreChar(char c) {                                      //RDR
  if ((c>='0') && (c<='9')) return true;                                //RDR
  if (c=='@') return true;                                              //RDR
  return m_initialChar(c);                                              //RDR
}                                                                       //RDR
                                                                        //RDR
bool RDRCOMP::m_numeric(char c)                                         //RDR
{                                                                       //RDR
  return ((c>='0') && (c<='9'));                                        //RDR                
}                                                                       //RDR
                                                                        //RDR
CTOKENTYPE RDRCOMP::m_getRawToken(void)                                 //RDR
{                                                                       //RDR  
  char c;                                                               //RDR
  m_token="";                                                           //RDR
  while (m_whitespace(c=m_nextChar())) ;                                //RDR
  if (c == 0) return CTKN_NONE;                                         //RDR
  if (m_initialChar(c))                                                 //RDR
  { // We appear to have an RDR;                                        //RDR
    m_token += c;                                                       //RDR
    while (m_moreChar(c=m_nextChar())) m_token += c;                    //RDR
    m_backspace();                                                      //RDR
    return CTKN_NAME;                                                   //RDR
  };                                                                    //RDR
  if (c== '"')                                                          //RDR
  { // We appear to have a quoted RDR;                                  //RDR
    m_token += c;                                                       //RDR
    while (   ((c=m_nextChar()) != '"')                                 //RDR
           && (c != 0)  ) m_token += c;                                 //RDR
    if (c != 0) m_token += '"';                                         //RDR
    else m_backspace();                                                 //RDR
    return CTKN_STRING;                                                 //RDR
  };                                                                    //RDR
  if (m_numeric(c))                                                     //RDR
  { // We have found either a number or a diceroll                      //RDR
    m_integer = 0;                                                      //RDR
    do                                                                  //RDR
    {                                                                   //RDR
      m_token += c;                                                     //RDR
      m_integer = 10 * m_integer + c - '0';                             //RDR
    } while (m_numeric(c=m_nextChar()));                                //RDR
    if ( (c=='D') || (c== 'd') )                                        //RDR
    { //Must be a diceroll                                              //RDR
      m_token += c;                                                     //RDR
      while (m_numeric(c=m_nextChar())) m_token += c;                   //RDR
      m_backspace();                                                    //RDR
      return CTKN_DICE;                                                 //RDR
    };                                                                  //RDR
    m_backspace();                                                      //RDR
    return CTKN_INTEGER;                                                //RDR
  };                                                                    //RDR
  m_token += c;                                                         //RDR
  c = m_nextChar();                                                     //RDR
  if (c != 0)                                                           //RDR
  {                                                                     //RDR
    m_token += c; // Assume a two character operator                    //RDR
    for (int i=0; i<sizeof (CoperDef)/sizeof (COPERDEF); i++)           //RDR
    {                                                                   //RDR
      if (m_token == CoperDef[i].name)                                  //RDR
      {                                                                 //RDR
        return CoperDef[i].tokenType;                                   //RDR
      };                                                                //RDR
    };                                                                  //RDR
    // It was not a two-character operator                              //RDR
    m_token = m_token.Left(1);                                          //RDR
  };                                                                    //RDR
  m_backspace();                                                        //RDR
  for (int i=0; i<sizeof (CoperDef)/sizeof (COPERDEF); i++)             //RDR
  {                                                                     //RDR
    if (m_token == CoperDef[i].name)                                    //RDR
    {                                                                   //RDR
      return CoperDef[i].tokenType;                                     //RDR
    };                                                                  //RDR
  };                                                                    //RDR
  return CTKN_NONE;                                                     //RDR
}                                                                       //RDR
                                                                        //RDR
CTOKENTYPE RDRCOMP::m_NextToken(void)                                   //RDR
{                                                                       //RDR
  if (m_backspaceTkn!=CTKN_NONE)                                        //RDR
  {                                                                     //RDR
    m_latestTkn=m_backspaceTkn;                                         //RDR
    m_backspaceTkn=CTKN_NONE;                                           //RDR
    return m_latestTkn;                                                 //RDR
  };                                                                    //RDR
  m_latestTkn = m_getRawToken();                                        //RDR
  return m_latestTkn;                                                   //RDR
}                                                                       //RDR
                                                                        //RDR
char RDRCOMP::m_nextChar(void)                                          //RDR
{                                                                       //RDR
  if (m_inputIndex >= m_expr.GetLength())                               //RDR
  {                                                                     //RDR
    m_inputIndex++;                                                     //RDR
    return 0;                                                           //RDR
  };                                                                    //RDR
  return m_expr[m_inputIndex++];                                        //RDR
}                                                                       //RDR
                                                                        //RDR
void RDRCOMP::m_error(char *msg, bool print)                            //RDR
{                                                                       //RDR
  if (!print) m_errorMsg += '+';                                        //RDR
  m_errorMsg += msg;                                                    //RDR
  m_errorMsg += '\n';                                                   //RDR
}                                                                       //RDR
                                                                        //RDR
void RDRCOMP::m_commaBYTE(char c)                                       //RDR
{                                                                       //RDR
  if (m_code == NULL)                                                   //RDR
  {                                                                     //RDR
    m_code = (char *)My_malloc(4);                                      //RDR
    m_codeSize = 4;                                                     //RDR
    m_codeIndex = 0;                                                    //RDR
  };                                                                    //RDR
  if (m_codeIndex >= m_codeSize)                                        //RDR
  {                                                                     //RDR
    m_codeSize = m_codeSize * 3 / 2;                                    //RDR
    m_code = (char *)My_realloc(m_code,m_codeSize);                     //RDR
  };                                                                    //RDR
  m_code[m_codeIndex++] = c;                                            //RDR
}                                                                       //RDR
                                                                        //RDR
void RDRCOMP::m_commaINT(unsigned int integer)                          //RDR
{                                                                       //RDR
  unsigned int temp;                                                    //RDR
  unsigned int mask[3] = {0xffffff80,0xffff8000,0xff800000};            //RDR
  COPS ops[4] = {COP_INTEGER1,COP_INTEGER2,COP_INTEGER3,COP_INTEGER4};  //RDR
  int i;                                                                //RDR
  for (i=0; i<3; i++)                                                   //RDR
  {                                                                     //RDR
    temp = integer & mask[i];                                           //RDR
    if ( (temp == 0) || (temp == mask[i]) ) break;                      //RDR
  };                                                                    //RDR
  m_commaOP(ops[i]);                                                    //RDR
  while (i>=0) {m_commaBYTE((char)(integer & 0xff)); integer>>=8; i--;};//RDR
}                                                                       //RDR
                                                                        //RDR
void RDRCOMP::m_commaOP(COPS binCode)                                   //RDR
{                                                                       //RDR
  m_commaBYTE((char)binCode)  ;                                         //RDR
}                                                                       //RDR
                                                                        //RDR
CTOKENTYPE RDRCOMP::m_GetToken(void)                                    //RDR
{                                                                       //RDR
  CTOKENTYPE result;                                                    //RDR
  result = m_NextToken();                                               //RDR
  return result;                                                        //RDR
}                                                                       //RDR
                                                                        //RDR
int RDRCOMP::m_EvaluateAtomicElement(void) {                            //RDR
  // One element of an expression.                                      //RDR
  CTOKENTYPE tokenType;                                                 //RDR
  int result;                                                           //RDR
  CString token;                                                        //RDR
  COPS unaryOperator = COP_ILLEGAL;                                     //RDR
  tokenType = m_GetToken();                                             //RDR
  // We allow only one unary operator.  Do you want more?               //RDR
  if (tokenType == CTKN_MINUS)                                          //RDR
  {                                                                     //RDR
    unaryOperator = COP_NEGATE;                                         //RDR
    tokenType = m_GetToken();                                           //RDR
  };                                                                    //RDR
  if (tokenType == CTKN_PLUS)                                           //RDR
  {                                                                     //RDR
    unaryOperator = COP_NONE;                                           //RDR
    tokenType = m_GetToken();                                           //RDR
  };                                                                    //RDR
  if (tokenType == CTKN_OPENPAREN)                                      //RDR
  {                                                                     //RDR
    result = m_EvaluateExpression();                                    //RDR
    if (result != 0)                                                    //RDR
    {                                                                   //RDR
      m_error("Called from compileExpression",false);                   //RDR
      return result;                                                    //RDR
    };                                                                  //RDR
    tokenType = m_GetToken();                                           //RDR
    if (tokenType != CTKN_CLOSEPAREN)                                   //RDR
    {                                                                   //RDR
      m_error("Expected close parenthesis",true);                       //RDR
      return 1;                                                         //RDR
    };                                                                  //RDR
  }                                                                     //RDR
  else if (tokenType==CTKN_INTEGER)                                     //RDR
  {                                                                     //RDR
    m_commaINT(m_integer);                                              //RDR
  }                                                                     //RDR
  else if (   (tokenType==CTKN_NAME)                                    //RDR
           || (tokenType==CTKN_DICE)                                    //RDR
           || (tokenType==CTKN_STRING)  )                               //RDR
  { // Must be an RDR                                                   //RDR
    //int array[20];                                                    //RDR
    char array[80];                                                     //RDR
    int status;                                                         //RDR
    int i;                                                              //RDR
    status = m_compileRDR(m_token, array, 80);                          //RDR
    switch (status)                                                     //RDR
    {                                                                   //RDR
    case 0:                                                             //RDR
      m_error("Unrecognized Runtime Variable reference",true);          //RDR
      return 1;                                                         //RDR
    case 1:                                                             //RDR
      m_commaOP(COP_RDR);                                               //RDR
      m_commaINT(array[0]);                                             //RDR
      for (i=1; i<=array[0]; i++)                                       //RDR
      {                                                                 //RDR
        m_commaINT(array[i]);                                           //RDR
      };                                                                //RDR
      break;                                                            //RDR
    case 2:                                                             //RDR
      m_commaINT(array[0]);  // a literal integer                       //RDR
      break;                                                            //RDR
    case 3:                                                             //RDR
      m_commaOP(COP_RDR1);                                              //RDR
      m_commaBYTE(array[0]);                                            //RDR
      for (i=1; i<=array[0]; i++)                                       //RDR
      {                                                                 //RDR
        m_commaBYTE(array[i]);                                          //RDR
      };                                                                //RDR
      break;                                                            //RDR
    default:                                                            //RDR
      m_error("Internal error-unknown RDR type",true);                  //RDR
      return 1;                                                         //RDR
    };                                                                  //RDR
  }                                                                     //RDR
  else                                                                  //RDR
  {                                                                     //RDR
    m_error("Unrecognized syntax for a term in an expression", true);   //RDR
    return 1;                                                           //RDR
  };                                                                    //RDR
  if (unaryOperator != COP_ILLEGAL)                                     //RDR
  {                                                                     //RDR
    if (unaryOperator != COP_NONE)                                      //RDR
    {                                                                   //RDR
      m_commaOP (unaryOperator);                                        //RDR
    };                                                                  //RDR
  };                                                                    //RDR
  return 0;                                                             //RDR
}                                                                       //RDR
                                                                        //RDR
int RDRCOMP::m_EvaluateExpression(void)                                 //RDR
{                                                                       //RDR
  COPS opcodes[MAXSTACK];                                               //RDR
  int priority[MAXSTACK];                                               //RDR
  CTOKENTYPE tokenTypes[MAXSTACK];                                      //RDR
  int stackLen=0; // No entries yet.                                    //RDR
  CTOKENTYPE tokenType;                                                 //RDR
  int j, result;                                                        //RDR
  // Our strategy is:                                                   //RDR
  // 1) evaluate an atomic element  (might be parenthesized expression) //RDR
  // 2) If next operator has higher priority than the top               //RDR
  //    operator on the stack then put the new operator on the          //RDR
  //    stack and go to 1)                                              //RDR
  // 3) Perform the operation on the top of the stack.  Go to 2)        //RDR
  //                                                                    //RDR
  // First we must check for leading, unary operators.                  //RDR
  tokenType = m_GetToken();                                             //RDR
  if (tokenType == CTKN_NOT)                                            //RDR
  {                                                                     //RDR
    priority[stackLen] = 999;                                           //RDR
    tokenTypes[stackLen] = tokenType;                                   //RDR
    opcodes[stackLen] = COP_NOT;                                        //RDR
    stackLen++;                                                         //RDR
  }                                                                     //RDR
  else if (tokenType == CTKN_MINUS)                                     //RDR
  {                                                                     //RDR
    priority[stackLen] = 999;                                           //RDR
    tokenTypes[stackLen] = tokenType;                                   //RDR
    opcodes[stackLen] = COP_NEGATE;                                     //RDR
    stackLen++;                                                         //RDR
  }                                                                     //RDR
  else                                                                  //RDR
  {                                                                     //RDR
    m_backspaceToken();                                                 //RDR
  };                                                                    //RDR
  for (;;)                                                              //RDR
  {                                                                     //RDR
    result = m_EvaluateAtomicElement();                                 //RDR
    if (result != 0) break; // Prints error message.                    //RDR
    tokenType = m_GetToken();                                           //RDR
    if (tokenType == CTKN_CLOSEPAREN) {m_backspaceToken(); break;};     //RDR
    if (tokenType == CTKN_NONE) {break;};                               //RDR
    // See if this is a legal operator.                                 //RDR
    for (j=0; j<sizeof (CoperDef)/sizeof (CoperDef[0]); j++)            //RDR
    {                                                                   //RDR
      if (tokenType == CoperDef[j].tokenType) break;                    //RDR
    };                                                                  //RDR
    if (j == sizeof(CoperDef)/sizeof(CoperDef[0]))                      //RDR
    {                                                                   //RDR
      m_error("Unknown operator symbol", true);                         //RDR
      return 1;                                                         //RDR
    };                                                                  //RDR
    if (stackLen >= MAXSTACK-1)                                         //RDR
    {                                                                   //RDR
      m_error("RDRCOMP:: stack overflow",true);                         //RDR
      return 1;                                                         //RDR
    };                                                                  //RDR
    priority[stackLen] = CoperDef[j].priority;                          //RDR
    tokenTypes[stackLen] = CoperDef[j].tokenType;                       //RDR
    opcodes[stackLen] = CoperDef[j].opcode;                             //RDR
    stackLen++;                                                         //RDR
    // Now....while the penultimate operation on the stack              //RDR
    // is greater or equal in priority to the top entry, perform        //RDR
    // the penultimate operation and drop it from the  stack.           //RDR
    while (   (stackLen > 1)                                            //RDR
           && (priority[stackLen-2] >= priority[stackLen-1]) )          //RDR
    {                                                                   //RDR
      m_commaOP(opcodes[stackLen-2]);                                   //RDR
      stackLen--;                                                       //RDR
      priority[stackLen-1] = priority[stackLen];                        //RDR
      opcodes[stackLen-1] = opcodes[stackLen];                          //RDR
      tokenTypes[stackLen-1] = tokenTypes[stackLen];                    //RDR
    };                                                                  //RDR
  };                                                                    //RDR
  // tokenType is the type of token that terminated the expression.     //RDR
  if (result == 0)                                                      //RDR
  { //Empty the stack of any old pending operations                     //RDR
    while (stackLen > 0)                                                //RDR
    {                                                                   //RDR
      stackLen--;                                                       //RDR
      m_commaOP(opcodes[stackLen]);                                     //RDR
    };                                                                  //RDR
  }                                                                     //RDR
  else                                                                  //RDR
  {                                                                     //RDR
    m_error("Called from RDRCOMP::Evaluate",false);                     //RDR
  };                                                                    //RDR
  return result;                                                        //RDR
}                                                                       //RDR
                                                                        //RDR
CString RDRCOMP::CompileExpression(                                     //RDR
                      const CString& expr,                              //RDR
                      int (*RDR)(CString&, char *, int size))           //RDR
{                                                                       //RDR
  int status;                                                           //RDR
  m_Initialize();                                                       //RDR
  m_expr = expr;                                                        //RDR
  m_compileRDR = RDR;                                                   //RDR
  status = m_EvaluateExpression();                                      //RDR
  if (status != 0) return CString("");                                  //RDR
  return m_Result();                                                    //RDR
}                                                                       //RDR


// Provide a list of available public system function names and operators 
// that the user can call in scripts.
//
// System functions are formatted as:
//
//   ReturnType FuncName( Param1Type, ... ,ParamNType )
//
void GPDLCOMP::GetLogicBlockFunctionNames(TEXTLIST &list)                     //CODE
{
  list.RemoveAll();
  unsigned int i;
  // include system functions and operators
  for (i=0;i<numSysFunc;i++)
  {
    if (systemfunctions[i].UsageType & LOGICBLK_USAGE)
    {
      
      CString fname;
      switch (systemfunctions[i].types[0])
      {
      case STRING:
        fname = "String ";
        break;
      case ACTOR:
        fname = "Actor ";
        break;
      }
      
      fname += systemfunctions[i].name;
      
      fname += "( ";
      
      for (int p=0;p<systemfunctions[i].numParam;p++)
      {
        if (p > 0)
          fname += ", ";
        
        switch (systemfunctions[i].types[p+1])
        {
        case STRING:
          fname += "String";
          break;
        case ACTOR:
          fname += "Actor";
          break;
        }
      }
      
      fname += " )";
      
      list.AddTail(fname);
    }
  }
  
  for (i=0;i<numSubMneumonic-1;i++)
  {
    if (submneumonics[i].m_UsageType & LOGICBLK_USAGE)
      list.AddTail(submneumonics[i].m_mneumonic);  
  }
}
void GPDLCOMP::GetEventFunctionNames(TEXTLIST &list)                     //CODE
{
  list.RemoveAll();
  unsigned int i;
  // include system functions and operators
  for (i=0;i<numSysFunc;i++)
  {
    if (systemfunctions[i].UsageType & EVENT_USAGE)
    {
      
      CString fname;
      switch (systemfunctions[i].types[0])
      {
      case STRING:
        fname = "String ";
        break;
      case ACTOR:
        fname = "Actor ";
        break;
      default:
        fname = "????? ";
        break;
       }
      
      fname += systemfunctions[i].name;
      
      fname += "( ";
      
      for (int p=0;p<systemfunctions[i].numParam;p++)
      {
        if (p > 0)
          fname += ", ";
        
        switch (systemfunctions[i].types[p+1])
        {
        case STRING:
          fname += "String";
          break;
        case ACTOR:
          fname += "Actor";
          break;
        default:
          fname += "?????";
          break;
         }
      }
      
      fname += " )";
      
      list.AddTail(fname);
    }
  }
  
  for (i=0;i<numSubMneumonic-1;i++)
  {
    if (submneumonics[i].m_UsageType & EVENT_USAGE)
      list.AddTail(submneumonics[i].m_mneumonic);  
  }
}


void GPDLCOMP::GetSpecialAbilityFunctionNames(TEXTLIST &list)                     //CODE
{
  list.RemoveAll();
  unsigned int i;
  // include system functions and operators
  for (i=0;i<numSysFunc;i++)
  {
    if (systemfunctions[i].UsageType & (SPECIALABILITIES_USAGE | COMBAT_USAGE))
    {
      
      CString fname;
      switch (systemfunctions[i].types[0])
      {
      case STRING:
        fname = "String ";
        break;
      case ACTOR:
        fname = "Actor ";
        break;
      default:
        fname = "????? ";
        break;
      }

      fname += systemfunctions[i].name;

      fname += "( ";

      for (int p=0;p<systemfunctions[i].numParam;p++)
      {
        if (p > 0)
          fname += ", ";

        switch (systemfunctions[i].types[p+1])
        {
        case STRING:
          fname += "String";
          break;
        case ACTOR:
          fname += "Actor";
          break;
        default:
          fname += "?????";
          break;
        }
      }

      fname += " )";

      list.AddTail(fname);
    }
  }
  
  for (i=0;i<numSubMneumonic-1;i++)
  {
    if (submneumonics[i].m_UsageType & SPECIALABILITIES_USAGE)
      list.AddTail(submneumonics[i].m_mneumonic);  
  }
}

void GPDLCOMP::GetSpellFunctionNames(TEXTLIST &list)                     //CODE
{
  list.RemoveAll();
  unsigned int i;
  // include system functions and operators
  for (i=0;i<numSysFunc;i++)
  {
    if (systemfunctions[i].UsageType & SPELL_USAGE)
    {
      
      CString fname;
      switch (systemfunctions[i].types[0])
      {
      case STRING:
        fname = "String ";
        break;
      case ACTOR:
        fname = "Actor ";
        break;
      default:
        fname += "????? ";
        break;
       }

      fname += systemfunctions[i].name;

      fname += "( ";

      for (int p=0;p<systemfunctions[i].numParam;p++)
      {
        if (p > 0)
          fname += ", ";

        switch (systemfunctions[i].types[p+1])
        {
        case STRING:
          fname += "String";
          break;
        case ACTOR:
          fname += "Actor";
          break;
        default:
          fname += "?????";
          break;
         }
      }

      fname += " )";

      list.AddTail(fname);
    }
  }
  
  for (i=0;i<numSubMneumonic-1;i++)
  {
    if (submneumonics[i].m_UsageType & SPELL_USAGE)
      list.AddTail(submneumonics[i].m_mneumonic);  
  }
}
