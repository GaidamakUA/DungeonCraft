#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

extern const char *LiteralOrPercentText[];
const int NUM_LITERALORPERCENT_TYPES = 3;


class HEAL_PARTY_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL HealHP;
  BOOL HealDrain;
  BOOL HealCurse;
  BYTE chance;
  int  HowMuchHP;
  BYTE LiteralOrPercent; // 0=literal,1=percent in HowMuchHP
  eventPartyAffectType who;
};




void Event_HealParty(JReader& event, JReader& details, JWriter& jw)
{
  HEAL_PARTY_DATA heal_party_data;
  heal_party_data.Read(event, details);
  heal_party_data.Export(jw);
};

void HEAL_PARTY_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_HEALHP, HealHP);
  jw.NameAndBool(JKEY_HEALDRAIN, HealDrain);
  jw.NameAndBool(JKEY_HEALCURSE, HealCurse);
  jw.StartList(JKEY_AFFECTS);
    jw.Linefeed(false);
    jw.NameAndEnum(JKEY_WHO, who);
    jw.NameAndValue(JKEY_CHANCE, chance);
  jw.EndList();
  jw.Linefeed(true);
  jw.StartList(JKEY_ADJUSTMENT);
    jw.Linefeed(false);
    jw.NameAndValue(JKEY_HP, HowMuchHP);
    jw.NameAndEnum(JKEY_ADJUSTHP, LiteralOrPercent, NUM_LITERALORPERCENT_TYPES, LiteralOrPercentText);
  jw.EndList();
  jw.Linefeed(true);
  jw.EndList();
}

void HEAL_PARTY_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, HealParty);
  details.NameAndBool(JKEY_HEALHP, HealHP);
  details.NameAndBool(JKEY_HEALDRAIN, HealDrain);
  details.NameAndBool(JKEY_HEALCURSE, HealCurse);
  if (details.Optional(), details.StartList(JKEY_AFFECTS, DBGID_healEffects))
  {
    details.Linefeed(false);
    details.NameAndEnum(JKEY_WHO, who);
    details.NameAndValue(JKEY_CHANCE, chance);
    details.EndList(DBGID_healEffects);
    details.Linefeed(true);
  };
  if (details.Optional(), details.StartList(JKEY_ADJUSTMENT, DBGID_healAdjustements))
  {
    details.Linefeed(false);
    details.NameAndValue(JKEY_HP, HowMuchHP);
    details.NameAndEnum(JKEY_ADJUSTHP, LiteralOrPercent, NUM_LITERALORPERCENT_TYPES, LiteralOrPercentText);
    details.EndList(DBGID_healAdjustements);
    details.Linefeed(true);
  };
  return;
}


