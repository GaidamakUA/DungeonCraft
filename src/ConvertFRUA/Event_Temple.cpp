#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


void TEMPLE::Clear()
{ 
  GameEvent::Clear();
  
  forceExit=TRUE;
  allowDonations=TRUE;
  costFactor=Normal;
  maxLevel=7;
  donationTrigger=0;
  donationChain=0;
  templeSpells.Clear();
  totalDonation=0; 
  m_state = -1;
}


TEMPLE::TEMPLE(void) :  
   GameEvent(), 
   donationChain(0),
   templeSpells(*(new spellBookType)),
   //tcKey(-1)
   characterID()
{
  Clear(); 
  evType=TempleEvent;

}

TEMPLE::~TEMPLE(void)
{
  delete &templeSpells;
}



void Event_Temple(JReader& event, JReader& details, JWriter& jw)
{
  TEMPLE temple;
  temple.Read(event, details);
  temple.Export(jw);
}



void TEMPLE::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndBool(JKEY_ALLOWDONATIONS, allowDonations);
  jw.NameAndEnum(JKEY_COSTFACTOR, costFactor);
  jw.NameAndValue(JKEY_MAXLEVEL, maxLevel);
  jw.NameAndValue(JKEY_DONATIONTRIGGER, donationTrigger);
  jw.NameAndValue(JKEY_DONATIONCHAIN, donationChain);
  jw.NameAndValue(JKEY_TOTALDONATION, totalDonation);

  templeSpells.Export(jw);
  jw.EndList();
}

void TEMPLE::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, TempleEvent);
  details.NameAndBool("force_backup_one_step"                 , forceExit);
  details.NameAndBool("allow_donations"                       , allowDonations);
  details.NameAndEnum("cost_modifier"                         , costFactor);
  details.NameAndValue("highest_spell_level_offered"          , maxLevel);
  details.NameAndValue("chain_event_donation_trigger_platinum", donationTrigger);
  details.NameAndValue("chain_event_donations"                , donationChain);
  details.NameAndValue("chain_event_donation_trigger_platinum", totalDonation);
  details.NameAndValue("text_1"                               , text);
  details.NameAndValue("text_2"                               , text2);
  details.NameAndValue("picture"                              , pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };

  if (maxLevel >= 1)
  {
    templeSpells.Add("Cure Light Wounds|cleric|1|3", 1);
  };
  if (maxLevel >= 3)
  {
    templeSpells.Add("Cure Blindness|cleric|3|37", 3);
    templeSpells.Add("Cure Disease|cleric|3|39", 3);
  };
  if (maxLevel >= 4)
  {
    templeSpells.Add("Neutralize Poison|cleric|4|67", 4);
    templeSpells.Add("Cure Serious Wounds|cleric|4|58", 4);
  };
  if (maxLevel >= 5)
  {
    templeSpells.Add("Cure Critical Wounds|cleric|5|71", 5);
    templeSpells.Add("Raise Dead|cleric|5|75", 5);
  };
  if (maxLevel >= 6)
  {
    templeSpells.Add("Stone To Flesh|magic-user|6|113", 6);
    templeSpells.Add("Heal|cleric|6|36", 6);
  };
  if (maxLevel >= 7)
  {
    templeSpells.Add("Restoration|cleric|7|102", 7);
    templeSpells.Add("Resurrection|cleric|7|105", 7);
  };
  return;
 }
