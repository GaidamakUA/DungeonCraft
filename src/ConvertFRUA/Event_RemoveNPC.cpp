#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class REMOVE_NPC_DATA : public GameEvent
{
public:
   void Export(JWriter& jw);
   void Read(JReader& event, JReader& jr);
   eventDistType distance;
   CString characterID;
};



void Event_RemoveNPC(JReader& event, JReader& details, JWriter& jw)
{
  REMOVE_NPC_DATA remove_npc_data;
  remove_npc_data.Read(event, details);
  remove_npc_data.Export(jw);
}


void REMOVE_NPC_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndValue(JKEY_CHARACTER, characterID);
  jw.EndList();
}

void REMOVE_NPC_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, RemoveNPCEvent);
  details.NameAndEnum("picture_distance", distance);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
  details.NameAndValue("if_npc_in_party", characterID);
  details.NameAndValue("text", text);
  return;
}
