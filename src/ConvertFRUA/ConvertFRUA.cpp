// ConvertFRUA.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "ConvertFRUA.h"
#include "Translate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

void ConvertClasses(JReader& globals);

FILE *f;
CString jsonPath;
CString executablePath;
CString FRUAExportDate, FRUAExportTime;
CString ConvertFRUAVersion = "1.0203";
CArray <CString, CString&> artArray;
CArray <CString, CString&> soundArray;
CArray <LEVELDATA, LEVELDATA&> levelArray;
CArray <BIG_SMALL_SPRITE_PIC, BIG_SMALL_SPRITE_PIC&> bigSmallSpritePics;
int currentLevelIndex; // starts at zero
extern CString strVersion = CString("I am ConvertFRUA version ") +  ConvertFRUAVersion;

CArray <ARTMAP, ARTMAP&> artMap;

void ARTMAP::operator = (const ARTMAP& src)
{
  strings.RemoveAll();
  strings.Append(src.strings);
}


void die(const char *msg)
{
  printf("%s\n", strVersion);
  MessageBox(NULL, msg, "Error", MB_OK);
  throw(1);
}

void PrintError(JReader& jr)
{
  die("JR ERROR");
}

void WriteLine(FILE *f, const char *key, const char *value)
{
  fprintf(f, "%s = %s\n", key, value);
}

void Line(const char *key, const char *value)
{
  WriteLine(f, key, value);
}

void Line(const char *key, int value)
{
  CString v;
  v.Format("%d", value);
  WriteLine(f, key, v);
}


void NotImplemented(int msg, bool)
{
  CString m;
  m.Format("Not Implemetned %x", msg);
  die(m);
}

int GetString(char *in, int col, char *out)
{
  static bool error=false;
  int n;
  while (in[col] == ' ') {col++;};
  if (in[col] == 0) return -1;
  if (in[col] == '\n') return -1;
  if (in[col] != '"')
  {
    printf("Error reading string:\n%s\n", in);
    return -1;
  };
  col++;
  n = 0;
  while (in[col] != '"')
  {
    if (in[col] == 0)
    {
      printf("syntax error in string:\n%s\n", in);
      if (!error)
      {
        char line[80];
        printf("Press Enter -->");
        fgets(line,79,stdin);
        error = true;
      };
      return -1;
    };
    out[n++] = in[col];
    col++;
  };
  out[n] = 0;
  return col+1;
}


void ReadArtMap(FILE *f)
{
  ARTMAP am;
  char line[1000];
  char name[1000];
  artMap.RemoveAll();
  for (;;)
  {
    int col;
    am.strings.RemoveAll();
    if (fgets(line, 999, f) == NULL) break;
    col = 0;
    for (;;)
    {
      CString str;
      col = GetString(line, col, name);
      if (col < 0) break;
      str = name;
      am.strings.Add(str);
    };
    if (am.strings.GetSize() == 0) continue;
    artMap.Add(am);
  };
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
    CFile cf;
    printf("%s\n", strVersion);
    if (argc != 2)
    {
      die("No input file specified");
    };
    if (!cf.Open(argv[1], CFile::modeRead))
    {
      die("Cannot open input file");
    };
    {
      int n;
      jsonPath = argv[1];
      n = jsonPath.ReverseFind('\\');
      if (n < 0)
      {
        die("Cannot find path to the '.json' file");
      };
      jsonPath = jsonPath.Left(n+1);

      executablePath = argv[0];
      n = executablePath.ReverseFind('\\');
      if (n < 0)
      {
        die("Cannot find path to the ConvertFRUA executable file");
      };
      executablePath = executablePath.Left(n+1);
      {
        FILE *f;
        f = fopen(executablePath + "FRUAArtMap.txt","r");
        if (f == NULL)
        {
          die("Cannot open'FRUAArtMap.txt' in the same folder as the FRUAConvert.exe");
        };
        ReadArtMap(f);
        fclose(f);
      };
    };
    JReader jr(&cf);
    try
    {
      printf("Reading FRUA file.....");
      jr.Initialize();
      cf.Close();
      JReader root(jr);
      root.Rewind();
      {
        root.StartArray("FRUAExport");
        root.NextEntry();
        root.StartList(DBGID_FRUAExport);
        root.NameAndValue("Build Date", FRUAExportDate);
        root.NameAndValue("Build Time", FRUAExportTime);
        root.EndList(DBGID_FRUAExport);
        root.EndArray();
      };
      {
        JReader pics(root);
        pics.StartArray("Vocab_BigSmallSpritePics");
        while (pics.Optional(), pics.NextEntry())
        {
          CString file, name;
          int value;
          pics.StartList(DBGID_BigSmallSpritePics);
          name = pics.Fetch_String("name");
          file = pics.Fetch_String("filename");
          value = pics.Fetch_Int("index");
          pics.EndList(DBGID_BigSmallSpritePics);
          if (!name.IsEmpty())
          {
            BIG_SMALL_SPRITE_PIC bssp;
            bssp.file = file;
            bssp.name = name;
            bssp.value = value;
            bigSmallSpritePics.Add(bssp);
          };
        };
      };
      {
        JReader art(root);
        art.StartArray("art");
        while (art.Optional(), art.NextEntry())
        {
          CString name;
          art.StartList(DBGID_singleArtEntry);
          name = art.Fetch_String("name");
          artArray.Add(name);
          art.EndList(DBGID_singleArtEntry);
        };
      };
      {
        JReader s(jr);
        s.StartArray("sounds");
        while (s.Optional(), s.NextEntry())
        {
          CString str;
          s.StartList(DBGID_singleSoundEntry);
          str = s.Fetch_String("name");
          soundArray.Add(str);
          s.EndList(DBGID_singleSoundEntry);
        };
      };
      {
        JReader spells(root);
        spells.StartArray("spells");
        ConvertSpells(spells);
        spells.EndArray();
      };
      {
        JReader items(root);
        items.StartArray("items");
        ConvertItems(items);
        items.EndArray();
      };
      {
        JReader monsters(root);
        monsters.StartArray("monsters");
        ConvertMonsters(monsters);
        monsters.EndArray();
      };
      {
        JReader levels(root);
        JReader global(root);
        levels.StartArray("levels");
        global.StartList("global", DBGID_globalData);
        ConvertLevels(global, levels); // Add data to levelArray
                                       // as a side-effect.
        ConvertGlobals(global);
        global.EndList(DBGID_globalData);
        levels.EndArray();
      };
      {
        JReader global(root);
        global.StartList("global", DBGID_globalData);
        ConvertClasses(global);
        global.EndList(DBGID_globalData);
      };
    }
    catch (int )
    {
      jr.Clear();
      die("JReader error");
    };
    {
      char line[80];
      printf("%s\n", strVersion);
      printf("All Done.  Press Enter -->");
      gets(line);
    }
	}

	return nRetCode;
}


