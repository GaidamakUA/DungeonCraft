
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class ADD_NPC_DATA:GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  eventDistType distance;
  CString characterID;
  int hitPointMod;
  BOOL useOriginal;
};



void Event_AddNPC(JReader& event, JReader& details, JWriter& jw)
{
  ADD_NPC_DATA add_NPC_data;
  add_NPC_data.Read(event, details);
  add_NPC_data.Export(jw);
}

void ADD_NPC_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, AddNpc);
  details.NameAndEnum("picture_distance", distance);
  details.NameAndValue("npc", characterID);
  details.NameAndValue("npc_added_with_pcnt_of_max_hitpoints", hitPointMod);
  useOriginal = FALSE; //details.NameAndBool(JKEY_USEORIGINAL, useOriginal);
  details.NameAndValue("text", text);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
}


void ADD_NPC_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndValue(JKEY_CHARACTER, characterID);
  jw.NameAndValue(JKEY_HITPOINTMOD, hitPointMod);
  jw.NameAndBool(JKEY_USEORIGINAL, useOriginal);
  jw.EndList();
}

