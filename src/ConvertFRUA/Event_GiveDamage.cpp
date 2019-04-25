
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class GIVE_DAMAGE_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  int nbrAttacks;
  int chancePerAttack;
  int dmgDice;
  int dmgDiceQty;
  int dmgBonus;
  int saveBonus;
  int attackTHAC0;
  spellSaveEffectType eventSave;
  spellSaveVsType spellSave;
  eventPartyAffectType who;
  eventDistType distance;
};



void Event_GiveDamage(JReader& event, JReader& details, JWriter& jw)
{
  GIVE_DAMAGE_DATA give_damage_data;
  give_damage_data.Read(event, details);
  give_damage_data.Export(jw);
}


void GIVE_DAMAGE_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndValue(JKEY_NUMBEROFATTACKS, nbrAttacks);
  jw.NameAndValue(JKEY_CHANCEPERATTACK, chancePerAttack);
  jw.NameAndDice(JKEY_DAMAGEDICE, dmgDiceQty, dmgDice, dmgBonus);
  jw.NameAndValue(JKEY_SAVEBONUS, saveBonus);
  jw.NameAndValue(JKEY_ATTACKTHAC0, attackTHAC0);
  jw.NameAndEnum(JKEY_EVENTSAVE, eventSave);
  jw.NameAndEnum(JKEY_SPELLSAVE, spellSave);
  jw.NameAndEnum(JKEY_AFFECTS, who);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.EndList();
}

void GIVE_DAMAGE_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, Damage);
  details.NameAndValue("num_attacks", nbrAttacks);
  details.NameAndValue("percent_chance_per_party_member", chancePerAttack);
  details.NameAndValue("num_dice", dmgDiceQty);
  details.NameAndValue("num_dice_sides", dmgDice);
  details.NameAndValue("dmg_bonus_per_attack", dmgBonus);
  details.NameAndValue("saving_throw_bonus", saveBonus);
  details.NameAndValue("attack_THAC0", attackTHAC0);
  {
    CString saveType;
    details.NameAndValue("saving_throw", saveType);
    if (saveType == "no save allowed")      eventSave = NoSave;
    if (saveType == "save for no damage")   eventSave = SaveNegates;
    if (saveType == "save for half damage") eventSave = SaveForHalf;
    if (saveType == "use THAC0")            eventSave = UseTHAC0;
  };
  {
    CString saveVs;
    details.NameAndValue("save_vs", saveVs);
    if (saveVs == "poison")        spellSave = ParPoiDM;
    if (saveVs == "petrification") spellSave = PetPoly;
    if (saveVs == "wand")          spellSave = RodStaffWand;
    if (saveVs == "breath")        spellSave = BreathWeapon;
    if (saveVs == "spell")         spellSave = Sp;
  };
  {
    CString affects;
    details.NameAndValue("target", affects);
    if (affects == "percent chance per party member") who = ChanceOnEach;
    else if (affects == "entire party")               who = EntireParty;
    else if (affects == "active party member")        who = ActiveChar;
    else if (affects == "random party member")        who = OneAtRandom;
    else {who = ActiveChar; details.PrintError("Cannont determine damage target");};

    
    
    //NoPartyMember, EntireParty, ActiveChar,
      //OneAtRandom, 
  };
  details.NameAndValue("text", text);
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
  return;
}
