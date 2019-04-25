#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int



void Event_Shop(JReader& event, JReader& details, JWriter& jw)
{
  SHOP shop;
  shop.Read(event, details);
  shop.Export(jw);
}


void SHOP::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndEnum(JKEY_COSTFACTOR, costFactor);
  jw.NameAndValue(JKEY_COSTTOID, costToId);
  jw.NameAndValue(JKEY_BUYBACKPERCENTAGE, buybackPercentage);
  jw.NameAndBool(JKEY_BUYITEMSOLDONLY, buyItemsSoldOnly);
  jw.NameAndValue(JKEY_SELLPRICE, sellprice);
  jw.NameAndBool(JKEY_CANID, canId);
  jw.NameAndBool(JKEY_CANAPPRAISEGEMS, canApprGems);
  jw.NameAndBool(JKEY_CANAPPRAISEJEWELS, canApprJewels);
  itemsAvail.Export(jw, JKEY_INVENTORY);
  jw.EndList();
}

void SHOP::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, ShopEvent);
  details.NameAndBool("force_backup_one_step", forceExit);
  details.NameAndEnum("cost_modifier", costFactor);
  if (details.NameAndValue("picture", pic.filename))
  {
    pic.picType = SmallPicDib;
  };
  //details.NameAndValue(JKEY_COSTTOID, costToId);
  costToId = 20;
  //details.NameAndValue(JKEY_BUYBACKPERCENTAGE, buybackPercentage);
  //details.NameAndBool(JKEY_BUYITEMSOLDONLY, buyItemsSoldOnly);
  //details.NameAndValue(JKEY_SELLPRICE, sellprice);
  //details.NameAndBool(JKEY_CANID, canId);
  canId = TRUE;
  //details.NameAndBool(JKEY_CANAPPRAISEGEMS, canApprGems);
  canApprGems = TRUE;
  //details.NameAndBool(JKEY_CANAPPRAISEJEWELS, canApprJewels);
  canApprJewels = TRUE;
  itemsAvail.Read(details, "items");

  return;
}
