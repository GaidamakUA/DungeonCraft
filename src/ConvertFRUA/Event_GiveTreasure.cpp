#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool


class GIVE_TREASURE_DATA : public GameEvent
{
public:
  GIVE_TREASURE_DATA(void)
  {
    SilentGiveToActiveChar = FALSE;
  };
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL SilentGiveToActiveChar;
  CString m_detectSpellID;
  ITEM_LIST items;
  MONEY_SACK money;
  MONEY_SACK moneyRemain;
  ITEM_LIST itemsRemain;
};




void Event_GiveTreasure(JReader& event, JReader& details, JWriter& jw)
{
  GIVE_TREASURE_DATA give_treasure_data;
  give_treasure_data.Read(event,details);
  give_treasure_data.Export(jw);
}


void GIVE_TREASURE_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  money.Export(jw);
  jw.NameAndBool(JKEY_SILENTGIVETOACTIVECHAR, SilentGiveToActiveChar);
  items.Export(jw, "items");
  jw.EndList();
}

void GIVE_TREASURE_DATA::Read(JReader& event, JReader& details)
{
  int i;
  GameEvent::Read(event, GiveTreasure);
  pic.ReadSmallPicFile(details, "picture");
  //money.Read(details);
  //details.NameAndBool(JKEY_SILENTGIVETOACTIVECHAR, SilentGiveToActiveChar);
  SilentGiveToActiveChar = FALSE;

  if (details.NameAndValue("gems", i))
  {
    GEM_TYPE gem;
    gem.value = i;
    gem.id = 0;
    money.Gems.Add(gem);
  };
  
  if (details.NameAndValue("platinum", i))
  {
    money.Coins[0] = i;
  };

  if (details.NameAndValue("jewelry", i))
  {
    GEM_TYPE gem;
    gem.value = i;
    gem.id = 0;
    money.Jewelry.Add(gem);
  };

  {
    int identified;
    details.NameAndValue("items_are_identified", identified);
    for (i=1; i<=8; i++)
    {
      ITEM item;
      CString key;
      key.Format("item_%d", i);
      if (!details.NameAndValue(key, item.itemID)) continue;
      if (item.itemID.IsEmpty()) continue;
      if (item.itemID == "none") continue;
      key.Format("item_%d_quantity", i);
      details.NameAndValue(key, item.qty);
      item.identified = identified;
      item.charges = 0;
      item.ready = false;
      item.cursed = false;
      item.paid = 0;
      item.key = i;
      items.m_items.Add(item);
    };
  };
  return;
}
