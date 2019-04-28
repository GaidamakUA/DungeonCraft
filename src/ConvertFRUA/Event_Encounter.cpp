
#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class ENCOUNTER_DATA : public GameEvent
{
public:

  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BYTE currDist;
  eventDistType distance;
  int monsterSpeed;
  encounterButtonResultType zeroRangeResult;
  DWORD combatChain;
  DWORD talkChain;
  DWORD escapeChain;
  long Unused;   
  ENCOUNTER_BUTTON_DATA buttons;
};


void ENCOUNTER_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndValue(JKEY_MONSTERSPEED, monsterSpeed); 
/*
	"No Result",
	"Decrease Range",
	"Combat, No Surprise",
	"Combat, Slow Party Surprised",
	"Combat, Slow Monsters Surprised",
	"Talk",
	"Escape if Fast Party, Else Combat",
	"Chain to Event"
  
  NoResult, DecreaseRange, CombatNoSurprise, 
   CombatSlowPartySurprised, CombatSlowMonsterSurprised,
   Talk, EscapeIfFastPartyElseCombat, ChainToEvent 
*/
  jw.NameAndEnum(JKEY_ZERORANGERESULT, zeroRangeResult); 
  jw.NameAndValue(JKEY_COMBATCHAIN, combatChain);
  jw.NameAndValue(JKEY_TALKCHAIN, talkChain);
  jw.NameAndValue(JKEY_ESCAPECHAIN, escapeChain);
  buttons.Export(jw);
  jw.EndList();
}
void ENCOUNTER_DATA::Read(JReader& event, JReader& details)
{
  CString temp;
  GameEvent::Read(event, EncounterEvent);
  details.NameAndValue("picture_distance", temp);
  if (temp == "close") distance = UpClose;
  if (temp == "nearby") distance =  Nearby;
  if (temp == "far away") distance = FarAway;
  details.NameAndValue("monster_speed", monsterSpeed);
  {
    details.NameAndValue("action_if_range_zero", temp);
    if (temp =="decrease range") zeroRangeResult = DecreaseRange;
    if (temp =="combat - party surprised if slow party") zeroRangeResult = CombatSlowPartySurprised;
    if (temp =="combat - monsters surprised if fast party") zeroRangeResult = CombatSlowMonsterSurprised;
    if (temp =="combat - no surprise") zeroRangeResult = CombatNoSurprise;
    if (temp =="talk chain") zeroRangeResult = Talk;
    if (temp =="escape if fast party else combat") zeroRangeResult = EscapeIfFastPartyElseCombat;
    if (temp =="no effect") zeroRangeResult = NoResult;
  };    
  details.NameAndValue("combat_event", combatChain);
  details.NameAndValue("talk_event", talkChain);
  details.NameAndValue("escape_event", escapeChain);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
  details.NameAndValue("text", text);
  buttons.Read(details);
  return;
}



void Event_Encounter(JReader& event, JReader& details, JWriter& jw)
{
  ENCOUNTER_DATA encounter_data;
  encounter_data.Read(event, details);
  encounter_data.Export(jw);
}
