
#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool


class PASS_TIME_EVENT_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL AllowStop;
  BOOL SetTime;
  BOOL PassSilent;
  BYTE Days;
  BYTE Hours;
  BYTE Minutes;
  BYTE daysLeftUnused;
  BYTE hoursLeftUnused;
  BYTE minutesLeftUnused;
};




void Event_PassTime(JReader& event, JReader& details, JWriter& jw)
{
  PASS_TIME_EVENT_DATA pass_time_event_data;
  pass_time_event_data.Read(event, details);
  pass_time_event_data.Export(jw);
}

void PASS_TIME_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndValue(JKEY_DAYS, Days);
  jw.NameAndValue(JKEY_HOURS, Hours);
  jw.NameAndValue(JKEY_MINUTES, Minutes);
  jw.NameAndBool(JKEY_ALLOWSTOP, AllowStop);
  jw.NameAndBool(JKEY_SETTIME, SetTime);
  jw.NameAndBool(JKEY_PASSSILENT, PassSilent);
  jw.EndList();
}

void PASS_TIME_EVENT_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, PassTime);
  details.NameAndValue("days", Days);
  details.NameAndValue("hours", Hours);
  details.NameAndValue("minutes", Minutes);
  AllowStop = FALSE;  //details.NameAndBool(JKEY_ALLOWSTOP, AllowStop);
  SetTime = FALSE;    //details.NameAndBool(JKEY_SETTIME, SetTime);
  PassSilent = FALSE; //details.NameAndBool(JKEY_PASSSILENT, PassSilent);
  details.NameAndValue("text", text);
  return;
}

