#include "stdafx.h"
#include "Translate.h"

#define Str stepEvents.Fetch_String
#define Int stepEvents.Fetch_Int

void StepEvents(JReader& stepEvents, JWriter& jw)
{
  jw.StartArray("stepEvents");
  {
    while (stepEvents.Optional(), stepEvents.NextEntry())
    {
      int i;
      int triggers[16];
      jw.NextEntry();
      stepEvents.StartList(DBGID_stepEvent);
      jw.StartList();
      jw.NameAndValue("id", Str("id"));
      jw.NameAndValue("event", Str("event_id"));
      jw.NameAndValue("steps", Str("num_steps"));
      for (i=0; i<16; i++)
      {
        CString tag;
        tag.Format("enabled_zone_%d", i+1);
        stepEvents.Optional();
        if (!stepEvents.NameAndValue(tag, triggers[i]))
        {
          triggers[i] = 0;
        };
      };
      jw.NameAndValue("triggers", triggers, 16);
      jw.EndList();
      stepEvents.EndList(DBGID_stepEvent);
    };
  };
  jw.EndArray();
}