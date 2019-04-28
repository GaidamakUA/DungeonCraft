#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


void Event_Tavern(JReader& event, JReader& details, JWriter& jw)
{
  TAVERN tavern;
  tavern.Read(event, details);
  tavern.Export(jw);
}




void TAVERN::Export(JWriter& jw)
{
  int i;
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndValue(JKEY_INFLATION, inflation);
  jw.NameAndValue(JKEY_BARKEEP, barkeep);
  jw.NameAndBool(JKEY_ALLOWFIGHTS, allowFights);
  jw.NameAndBool(JKEY_ALLOWDRINKS, allowDrinks);
  jw.NameAndValue(JKEY_FIGHTCHAIN, fightChain);
  jw.NameAndValue(JKEY_DRINKCHAIN, drinkChain);
  jw.NameAndValue(JKEY_DRINKPOINTTRIGGER, drinkPointTrigger);
  jw.NameAndEnum(JKEY_TALEORDER, taleOrder);
  jw.NameAndBool(JKEY_EACHTALEONCEONLY, EachTaleOnceOnly);

  jw.StartArray(JKEY_TALES);
  {
    for (i=0;i<MAX_TALES;i++)
    {
      if (tales[i].tale.IsEmpty()) continue;
      jw.NextEntry();
      tales[i].Export(jw, i);
    };
  };
  jw.EndArray();
  jw.StartArray(JKEY_DRINKS);
  {
    for (i=0;i<MAX_DRINKS;i++)
    {
      if (drinks[i].name.IsEmpty()) continue;
      jw.NextEntry();
      drinks[i].Export(jw, i);
    };
  };
  jw.EndArray();
  jw.EndList();
}
void TAVERN::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, TavernEvent);
  details.NameAndBool("force_backup_one_step", forceExit);
  //details.NameAndValue(JKEY_INFLATION, inflation);
  inflation = 0;
  //details.NameAndValue(JKEY_BARKEEP, barkeep);
  barkeep = 0;
  details.NameAndBool("allow_fights", allowFights);
  details.NameAndBool("allow_drinks", allowDrinks);
  details.NameAndValue("chain_event_fight", fightChain);
  details.NameAndValue("chain_event_drunk", drinkChain);
  details.NameAndValue("chain_event_drunk_trigger_points", drinkPointTrigger);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
  {
    CString Order;
    details.NameAndEnum("tale_order", taleOrder);
    if (Order == "in order") taleOrder = InOrderTales;
    else taleOrder = RandomTales ;
  };
  //details.NameAndBool(JKEY_EACHTALEONCEONLY, EachTaleOnceOnly);
  EachTaleOnceOnly = FALSE;
  details.NameAndValue("picture", pic.filename);
  details.NameAndValue("text", text);
  
  {
    TALE tale;
    int index;
    for (index=0; index<MAX_TALES; index++)
    {
      if (tale.Read(details, index+1))
      {
        tales[index] = tale;
      };
    };
  };
  {
    DRINK drink;
    int index;
    for (index=0; index<MAX_DRINKS; index++)
    {
      if (drink.Read(details, index+1))
      {
        drinks[index] = drink;
      };
    };
  };
  return;
}

void TALE::Export(JWriter& jw, int i)
{
  if (tale.IsEmpty()) return;
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_INDEX, i);
  jw.NameAndValue(JKEY_COUNT, count);
  jw.NameAndValue(JKEY_TEXT, tale);
  jw.EndList();
  jw.Linefeed(true);
}

bool TALE::Read(JReader& jr, int taleNum)
{
  CString key;
  key.Format("tale_%d", taleNum);
  jr.Optional(); if (!jr.NameAndValue(key, tale)) return false;
  count = 0;
  return true;
}

void DRINK::Export(JWriter& jw, int i)
{
  if (name.IsEmpty()) return;
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_INDEX, i);
  jw.NameAndValue(JKEY_POINTS, points);
  jw.NameAndValue(JKEY_NAME, name);
//     if (name.IsEmpty())
//     {
//        name = "*";
//        ar << name;
//        name="";
//     }
//     else
//        ar << name;
  jw.EndList();
  jw.Linefeed(true);
}


bool DRINK::Read(JReader& jr, int drinkNum)
{
  CString key;
  key.Format("drink_%d_name", drinkNum);
  jr.Optional(); if (!jr.NameAndValue(key, name)) return false;
  key.Format("drink_%d_points", drinkNum);
  jr.NameAndValue(key, points);
  return true;
}
