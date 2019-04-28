
#include "StdAfx.h"
#include "Translate.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class REST_EVENT //: public CObject
{
public:
   void Export(JWriter& jw);
   void Read(JReader& event, JReader& jr);
   BOOL allowResting;
   DWORD restEvent;
   int chance;
   int everyMin;
   int prevMinChecked;
};


void Event_Rest(JReader& event, JReader& details, JWriter& jw)
{
  REST_EVENT rest_event;
  rest_event.Read(event, details);
  rest_event.Export(jw);
}

void REST_EVENT::Export(JWriter& jw)
{
  jw.StartList(JKEY_RESTEVENT);
  jw.NameAndValue(JKEY_ALLOWRESTING, allowResting);
  jw.NameAndValue(JKEY_RESTEVENT, restEvent);
  jw.NameAndValue(JKEY_CHANCE, chance);
  jw.NameAndValue(JKEY_EVERYMIN, everyMin);
  jw.NameAndValue(JKEY_PREVMINCHECKED, prevMinChecked);
  jw.EndList();
}

void REST_EVENT::Read(JReader& event, JReader& details)
{
  details.StartList(JKEY_RESTEVENT, DBGID_restEvent);
  details.NameAndValue(JKEY_ALLOWRESTING, allowResting);
  details.NameAndValue(JKEY_RESTEVENT, restEvent);
  details.NameAndValue(JKEY_CHANCE, chance);
  details.NameAndValue(JKEY_EVERYMIN, everyMin);
  details.NameAndValue(JKEY_PREVMINCHECKED, prevMinChecked);
  details.EndList(DBGID_restEvent);
}
