#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class NULL_EVENT_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
};




void Event_Null(JReader& event, JReader& details, JWriter& jw)
{
  NULL_EVENT_DATA null_event_data;
  null_event_data.Read(event, details);
  null_event_data.Export(jw);
}

void NULL_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  NotImplemented(0xcd940, false);
  jw.EndList();
}

void NULL_EVENT_DATA::Read(JReader& event, JReader& details)
{
  NotImplemented(0xcd941, false);
  GameEvent::Read(event, NoEvent);
  return;
}
