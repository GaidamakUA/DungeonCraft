
#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class JOURNAL_EVENT : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  int journal_entry;
};



void Event_Journal(JReader& event, JReader& details, JWriter& jw)
{
  JOURNAL_EVENT journal_event;
  journal_event.Read(event, details);
  journal_event.Export(jw);
}


void JOURNAL_EVENT::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndValue(JKEY_JOURNALENTRY, journal_entry);
  jw.EndList();
}

void JOURNAL_EVENT::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, JournalEvent);
  details.NameAndValue(JKEY_JOURNALENTRY, journal_entry);
  return;
}



/*
void JOURNAL_EVENT::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_JOURNALENTRY, journal_entry);
}
*/

