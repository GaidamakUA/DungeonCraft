#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class TAVERN_TALES_TALE //: public CObject
{
public:
  void Export(JWriter& jw, int index);
  bool Read(JReader& jr, int index);
  CString tale;
  int count;
};


class TAVERN_TALES : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  void operator =(const TAVERN_TALES& src);
  void Clear(void);

  //A_ASLENTRY_L m_asl; // Just in case.
  DWORD m_flags; // TAVERN_TALES_FLAGS
  CArray<TAVERN_TALES_TALE, TAVERN_TALES_TALE&> m_tales;

};




void Event_TavernTales(JReader& event, JReader& details, JWriter& jw)
{
  TAVERN_TALES tavernTales;
  tavernTales.Read(event, details);
  tavernTales.Export(jw);
}




void TAVERN_TALES::Export(JWriter& jw)
{
  int i, n;
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);

  jw.StartArray(JKEY_TALES);
  {
    n = m_tales.GetSize();
    for (i=0;i<n;i++)
    {
      if (m_tales[i].tale.IsEmpty()) continue;
      jw.NextEntry();
      m_tales[i].Export(jw, i);
    };
  };
  jw.EndArray();
  jw.StartArray("flags"); jw.EndArray();
  jw.EndList();
}

void TAVERN_TALES::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, TavernTales);
  {
    TAVERN_TALES_TALE tale;
    int index;
    for (index=0; index<MAX_TALES; index++)
    {
      if (tale.Read(details, index+1))
      {
        m_tales.Add(tale);
      };
    };
  };
  return;
}

void TAVERN_TALES_TALE::Export(JWriter& jw, int i)
{
  if (tale.IsEmpty()) return;
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_INDEX, i);
  jw.NameAndValue(JKEY_COUNT, count);
  jw.NameAndValue("tale", tale);
  jw.StartArray("flags"); jw.EndArray();
  jw.EndList();
  jw.Linefeed(true);
}

bool TAVERN_TALES_TALE::Read(JReader& jr, int taleNum)
{
  CString key;
  key.Format("tale_%d", taleNum);
  jr.Optional(); if (!jr.NameAndValue(key, tale)) return false;
  count = 0;
  return true;
}

