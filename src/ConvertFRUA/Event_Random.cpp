
#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class RANDOM_EVENT_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  enum { MAX_RANDOM_EVENTS=14 };
  DWORD EventChains[MAX_RANDOM_EVENTS];
  BYTE EventChance[MAX_RANDOM_EVENTS];
};




void Event_Random(JReader& event, JReader& details, JWriter& jw)
{
  RANDOM_EVENT_DATA random_event_data;
  random_event_data.Read(event, details);
  random_event_data.Export(jw);
}


void RANDOM_EVENT_DATA::Export(JWriter& jw)
{
  int i;
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  for (i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChance[i-1] == 0) continue;
    if (EventChains[i-1] == 0) continue;
    jw.StartList(JKEY_POSSIBLECHAIN);
    jw.Linefeed(false);
    jw.NameAndValue(JKEY_CHAIN, EventChains[i-1]);
    jw.NameAndValue(JKEY_CHANCE, EventChance[i-1]);
    jw.EndList();
    jw.Linefeed(true);
  }
  jw.EndList();
}

void RANDOM_EVENT_DATA::Read(JReader& event, JReader& details)
{
  int index=0;
  GameEvent::Read(event, RandomEvent);
  {
    while (details.Optional(), details.StartList(JKEY_POSSIBLECHAIN, DBGID_randomChain))
    {
      details.Linefeed(false);
      details.NameAndValue(JKEY_CHAIN, EventChains[index]);
      details.NameAndValue(JKEY_CHANCE, EventChance[index]);
      details.EndList(DBGID_randomChain);
      details.Linefeed(true);
      index++;
      if (index >= MAX_RANDOM_EVENTS-1) break;
    }
  };
  return;
}
