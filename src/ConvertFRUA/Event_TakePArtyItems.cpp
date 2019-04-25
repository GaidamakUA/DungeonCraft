#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

extern const char *TakeWhatText[];


class TAKE_PARTY_ITEMS_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL StoreItems; // put taken items in vault
  BYTE WhichVault;
  BOOL mustHitReturn;
  BYTE takeItems; // flag that indicates item types to take
  takeItemsAffectsType takeAffects; // who gets taken from
  takeItemQtyType itemSelectFlags; // random, percent, specified, all
  takeItemQtyType platinumSelectFlags;
  takeItemQtyType gemsSelectFlags;
  takeItemQtyType jewelrySelectFlags;
  int platinum;
  int gems;
  int jewelry;
  int itemPcnt;
  itemClassType moneyType; // which money type to take (plat,gold,etc)
  ITEM_LIST items;
};



void Event_TakePartyItems(JReader& event, JReader& details, JWriter& jw)
{
  TAKE_PARTY_ITEMS_DATA take_party_items_data;
  take_party_items_data.Read(event, details);
  take_party_items_data.Export(jw);
}





void TAKE_PARTY_ITEMS_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_STOREITEMS, StoreItems);
  if (StoreItems)
  {
    jw.NameAndValue(JKEY_WHICHVAULT, WhichVault);
  };
  jw.NameAndBool(JKEY_MUSTHITRETURN, mustHitReturn);
  jw.NameAndFlags(JKEY_TAKEITEMS, takeItems, 4, TakeWhatText);
  jw.NameAndEnum(JKEY_TAKEEFFECTS, takeAffects);
  jw.NameAndEnum(JKEY_MONEYTYPE, moneyType);
  jw.NameAndValue(JKEY_MONEYNUMBER, platinum);
  jw.NameAndEnum(JKEY_MONEYQUANTITY, platinumSelectFlags);
  jw.NameAndValue(JKEY_JEWELRY, jewelry);
  jw.NameAndEnum(JKEY_JEWELRYQUANTITY, jewelrySelectFlags);
  jw.NameAndValue(JKEY_GEMS, gems);
  jw.NameAndEnum(JKEY_GEMSQUANTITY, gemsSelectFlags);
  jw.NameAndValue(JKEY_ITEMS, itemPcnt);
  jw.NameAndEnum(JKEY_ITEMSQUANTITY, itemSelectFlags);
  items.Export(jw, JKEY_ITEMLIST);
  jw.EndList();
}

void TAKE_PARTY_ITEMS_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, TakePartyItems);
  details.NameAndBool(JKEY_STOREITEMS, StoreItems);
  if (StoreItems)
  {
    details.NameAndValue(JKEY_WHICHVAULT, WhichVault);
  };
  details.NameAndBool(JKEY_MUSTHITRETURN, mustHitReturn);
  details.NameAndFlags(JKEY_TAKEITEMS, takeItems, 4, TakeWhatText);
  details.NameAndEnum(JKEY_TAKEEFFECTS, takeAffects);
  details.NameAndEnum(JKEY_MONEYTYPE, moneyType);
  details.NameAndValue(JKEY_MONEYNUMBER, platinum);
  details.NameAndEnum(JKEY_MONEYQUANTITY, platinumSelectFlags);
  details.NameAndValue(JKEY_JEWELRY, jewelry);
  details.NameAndEnum(JKEY_JEWELRYQUANTITY, jewelrySelectFlags);
  details.NameAndValue(JKEY_GEMS, gems);
  details.NameAndEnum(JKEY_GEMSQUANTITY, gemsSelectFlags);
  details.NameAndValue(JKEY_ITEMS, itemPcnt);
  details.NameAndEnum(JKEY_ITEMSQUANTITY, itemSelectFlags);
  items.Read(details, JKEY_ITEMLIST);
  return;
}
