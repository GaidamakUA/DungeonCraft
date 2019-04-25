#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class CHAIN_EVENT : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);

  DWORD Chain;
};



void Event_Chain(JReader& event, JReader& details, JWriter& jw)
{
  CHAIN_EVENT chain_event;
  chain_event.Read(event, details);
  chain_event.Export(jw);
}


void CHAIN_EVENT::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndValue(JKEY_CHAIN, Chain);
  jw.EndList();
}

void CHAIN_EVENT::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, ChainEventType);
  CString temp, type;
  int col;
  Chain = 0; // Assume no chain
  details.Optional();
  details.NameAndValue("chained_event_id", Chain);
  event.NameAndValue("trigger_data", temp);
  if (temp.Left(11) == "special key")
  {
    control.eventTrigger = PartyHaveSpecialKey;
    col = 12;
    sscanf(temp.GetBuffer(0)+col, "%d", &control.specialKey);
  }
  else if (temp.Left(12) == "special item")
  {
    control.eventTrigger = PartyHaveSpecialItem;
    col = 13;
    sscanf(temp.GetBuffer(0)+col, "%d", &control.specialItem);
  }
  else if (temp.Left(13) == "special quest")
  {
    control.eventTrigger = QuestInProgress;
    col = 14;
    sscanf(temp.GetBuffer(0)+col, "%d", &control.quest);
  }
  else
  {
    type = "key";
    col = 0;
  };
  //sscanf(&temp[col], "%d", &index);

  return;
}
