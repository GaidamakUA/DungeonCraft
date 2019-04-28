#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool


class NPC_SAYS_DATA : public GameEvent
{
public:
   void Export(JWriter& jw);
   void Read(JReader& event, JReader& jr);
  CString sound;
  int hSound;
  BOOL mustHitReturn;
  BOOL highlight;
  eventDistType distance;  
  //int partySeesChar;
  CString characterID;
};



void Event_NPCSays(JReader& event, JReader& details, JWriter& jw)
{
  NPC_SAYS_DATA npc_says_data;
  npc_says_data.Read(event, details);
  npc_says_data.Export(jw);
}


void NPC_SAYS_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndValue(JKEY_CHARACTERID, characterID);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndNonBlank(JKEY_SOUND, sound);
  jw.NameAndBool(JKEY_MUSTHITRETURN, mustHitReturn);
  jw.NameAndBool(JKEY_HIGHLIGHT, highlight);
  jw.EndList();
}

void NPC_SAYS_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, NPCSays);
  details.NameAndValue("if_npc_in_party", characterID);
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
  details.NameAndNonBlank("sound", sound);
  details.NameAndBool("text_1_press_return", mustHitReturn);
  details.NameAndBool("text_1_highlight", highlight);
  details.NameAndValue("text_1", text);
  details.NameAndValue("text_2", text2);
  details.NameAndValue("text_3", text3);
  return;
}
