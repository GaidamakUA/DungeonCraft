#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool

class TRANSFER_EVENT_DATA : public GameEvent
{
public:
  TRANSFER_EVENT_DATA(void){Clear();};
  void Clear(void);
  void Export(JWriter& jw);
  void ReadTeleporter(JReader& event, JReader& jr);
  void ReadStairs(JReader& event, JReader& jr);
  void ReadTransferModule(JReader& event, JReader& jr);
  BOOL askYesNo;
  BOOL transferOnYes;
  BOOL destroyDrow;
  BOOL activateBeforeEntry;
  TRANSFER_DATA transferData;
};



void Event_Teleporter(JReader& event, JReader& details, JWriter& jw)
{
  TRANSFER_EVENT_DATA transfer_event_data;
  transfer_event_data.ReadTeleporter(event, details);
  transfer_event_data.Export(jw);
}

void Event_Stairs(JReader& event, JReader& details, JWriter& jw)
{
  TRANSFER_EVENT_DATA transfer_event_data;
  transfer_event_data.ReadStairs(event, details);
  transfer_event_data.Export(jw);
}


void Event_TransferModule(JReader& event, JReader& details, JWriter& jw)
{
  TRANSFER_EVENT_DATA transfer_event_data;
  transfer_event_data.ReadTransferModule(event, details);
  transfer_event_data.Export(jw);
}

