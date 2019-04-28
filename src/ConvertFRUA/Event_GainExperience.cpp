
#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class GAIN_EXP_DATA : public GameEvent
{
public:
   void Export(JWriter& jw);
   void Read(JReader& event, JReader& jr);
  CString sound;
  int hSound;
  int experience; 
  int tempExp;
  int chance;
  eventPartyAffectType who;
};



void Event_GainExperience(JReader& event, JReader& details, JWriter& jw)
{
  GAIN_EXP_DATA gain_exp_data;
  gain_exp_data.Read(event, details);
  gain_exp_data.Export(jw);
}


void GAIN_EXP_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndValue(JKEY_EXPERIENCE, experience);
  jw.NameAndNonBlank(JKEY_SOUND, sound);
  jw.NameAndValue(JKEY_CHANCE, chance);
  jw.NameAndEnum(JKEY_WHO, who);
  jw.EndList();
}

void GAIN_EXP_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, GainExperience);
  details.NameAndValue("experience_given", experience);
  details.NameAndNonBlank("sound", sound);
  chance = 100; //details.NameAndValue(JKEY_CHANCE, chance);
  {
    CString whoString;
    details.NameAndValue("experience_given_to", whoString);
    who = NoPartyMember;
    if (whoString == "entire party")                    who = EntireParty;
    if (whoString == "active party member")             who = ActiveChar;
    if (whoString == "random party member")             who = OneAtRandom;
    if (whoString == "percent chance per party member") who = ChanceOnEach;
  };
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
  return;
}
