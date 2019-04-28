#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int




void Event_Camp(JReader& event, JReader& details, JWriter& jw)
{
  CAMP_EVENT_DATA camp_event_data;
  camp_event_data.Read(event, details);
  camp_event_data.Export(jw);
}


void CAMP_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_FORCEEXIT, ForceExit);
  jw.EndList();
}

void CAMP_EVENT_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, Camp);
  details.NameAndBool(JKEY_FORCEBACKUPONESTEP, ForceExit);
  details.NameAndQuotedValue("text", text);
  pic.ReadSmallPicFile(details, "picture");
  return;
}
