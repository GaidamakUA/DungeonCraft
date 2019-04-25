#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int




class COMBAT_EVENT_DATA : public GameEvent
{
public:
   void Export(JWriter& jw);
   void Read(JReader& event, JReader& jr, bool pickOneCombat);

   BOOL m_UseOutdoorMap; // set at run-time as event activates
   eventDistType distance;
   eventDirType direction;
   eventSurpriseType m_eSurprise;
   eventSurpriseType m_eTEMPsurprise;
   BOOL autoApproach;
   BOOL outdoors;
   BOOL noMonsterTreasure;
   BOOL partyNeverDies;
   BOOL partyNoExperience;
   BOOL noMagic;
   BOOL randomMonster;
   CString DeathSound;
   CString MoveSound;
   CString TurnUndeadSound;
   int monsterMorale;
   eventTurnUndeadModType turningMod;
   MONSTER_EVENT_DATA monsters;
   BACKGROUND_SOUND_DATA bgSounds;
   BOOL needDeathSprite;
   BOOL needDyingSprite;
};




void Event_Combat(JReader& event, JReader& details, JWriter& jw, bool pickOneCombat)
{
  COMBAT_EVENT_DATA combat_event_data;
  combat_event_data.Read(event, details, pickOneCombat);
  combat_event_data.Export(jw);
}


void COMBAT_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndQuotedNonBlank(JKEY_DEATHSOUND,DeathSound);
  jw.NameAndQuotedNonBlank(JKEY_MOVESOUND,MoveSound);
  jw.NameAndQuotedNonBlank(JKEY_TURNUNDEADSOUND,TurnUndeadSound);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndEnum(JKEY_DIRECTION, direction);
  jw.NameAndEnum(JKEY_SURPRISE, m_eSurprise);
  jw.NameAndBool(JKEY_AUTOAPPROACH, autoApproach);
  jw.NameAndBool(JKEY_OUTDOORS, outdoors);
  jw.NameAndBool(JKEY_MONSTERTREASURE, noMonsterTreasure);
  jw.NameAndBool(JKEY_PARTYNEVERDIES, partyNeverDies);
  jw.NameAndBool(JKEY_NOMAGIC, noMagic);
  jw.NameAndValue(JKEY_MONSTERMORALE, monsterMorale);
  jw.NameAndEnum(JKEY_TURNMODIFIER, turningMod);
  jw.NameAndBool(JKEY_RANDOMMONSTER, randomMonster);
  jw.NameAndBool(JKEY_PARTYGAINSEXP, !partyNoExperience);
  bgSounds.Export(jw);
  monsters.Export(jw);
  jw.EndList();
}

void COMBAT_EVENT_DATA::Read(JReader& event, JReader& details, bool pickOneCombat)
{
  CString value;
  GameEvent::Read(event, Combat);
  details.NameAndQuotedNonBlank(JKEY_DEATHSOUND,DeathSound);
  details.NameAndQuotedNonBlank(JKEY_MOVESOUND,MoveSound);
  details.NameAndQuotedNonBlank(JKEY_TURNUNDEADSOUND,TurnUndeadSound);
  details.Optional(); details.NameAndQuotedNonBlank(JKEY_TEXT, text);

  {
    details.NameAndValue("start_distance", value);
    distance = UpClose;
    if (value == "nearby") distance = Nearby;
    if (value == "far away") distance = FarAway;
  };

  //details.NameAndEnum(JKEY_DIRECTION, direction);
  direction = InFront;

  {
    CString value;
    //details.NameAndEnum(JKEY_SURPRISE, m_eSurprise);
    details.NameAndValue("surprise", value);
    m_eSurprise = Neither;
    if (value == "neither") m_eSurprise = Neither;
    if (value == "party")   m_eSurprise = PartySurprised;
    if (value == "monster") m_eSurprise = MonsterSurprised;
  };

  details.NameAndBool("auto_approach", autoApproach);

  {
    CString value;
    //details.NameAndBool(JKEY_OUTDOORS, outdoors);
    details.NameAndValue("location", value);
    outdoors = false;
    if (value == "outdoors") outdoors = true;
    if (value == "indoors")  outdoors = false;
  };

  {
    BOOL monsterTreasure;
    details.NameAndBool("no_treasure_from_monsters", monsterTreasure);
    noMonsterTreasure = !monsterTreasure;
  }

  details.NameAndBool("party_never_dies", partyNeverDies);
  
  if (details.NameAndBool("magic_allowed", noMagic))
  {
    noMagic = !noMagic;
  }
  else if (details.NameAndBool("magic_disabled", noMagic))
  {
  }
  else noMagic = TRUE;

  if (!details.NameAndValue("monster_morale_modifier", monsterMorale))
  {
    details.NameAndValue("monster_morale_modifier_pcnt", monsterMorale);
  };

  {
    //details.NameAndEnum(JKEY_TURNMODIFIER, turningMod);
    CString value;
    details.NameAndValue("turn_modifier", value);
    if      (value == "hard")       turningMod = Hard;
    else if (value == "difficult")  turningMod = Difficult;
    else if (value == "impossible") turningMod = Impossible;
    else                            turningMod = None;
  };
 
  //details.NameAndBool(JKEY_RANDOMMONSTER, randomMonster);
  randomMonster = pickOneCombat;

  {
    //BOOL partyExperience;
    //details.NameAndBool(JKEY_PARTYGAINSEXP, partyExperience);
    //partyNoExperience = !partyExperience;
    partyNoExperience = false;
  };

  //bgSounds.Read(details);
  {
    //monsters.Read(details);
    int i;
    for (i=0; i<6; i++)
    {
      CString tag;
      MONSTER_EVENT monster;
      //pMonster = &monsters.monsters[i];
      tag.Format("monster_%d_type", i+1);
      details.NameAndValue(tag, monster.characterID);
      monster.characterID.MakeUpper();
      monster.monsterID = monster.characterID;
      monster.m_type = MONSTER_TYPE;
      tag.Format("monster_%d_friendly", i+1);
      details.NameAndValue(tag, monster.friendly);
      if (pickOneCombat)
      {
        tag.Format("monster_%d_quantity", i+1);
      }
      else
      {
        tag.Format("monster_%d_count", i+1);
      };
      details.NameAndValue(tag, monster.qty);
      monster.UseQty = true;
      monster.moraleAdj = 0;
      monster.qtyDiceSides = 0;
      monster.qtyDiceQty = 0;
      monster.qtyBonus = 0;
      monsters.monsters.Add(monster);
    };
  };
  pic.ReadSmallPicFile(details, "picture");
  return;
}

