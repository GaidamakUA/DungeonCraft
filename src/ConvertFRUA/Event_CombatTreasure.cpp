#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool


class COMBAT_TREASURE : public GameEvent
{
public:
  COMBAT_TREASURE& operator =(const COMBAT_TREASURE& src);
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& details);
  ITEM_LIST items;
  MONEY_SACK money;
};


void Event_CombatTreasure(JReader& event, JReader& details, JWriter& jw)
{
  COMBAT_TREASURE combatTreasure;
  combatTreasure.Read(event,details);
  combatTreasure.Export(jw);
}


void COMBAT_TREASURE::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  money.Export(jw);
  items.Export(jw, "items");
  jw.EndList();
}

void COMBAT_TREASURE::Read(JReader& event, JReader& details)
{
  int i;
  GameEvent::Read(event, CombatTreasure);
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

  for (i=1; i<=8; i++)
  {
    ITEM item;
    CString key;
    key.Format("item_%d", i);
    if (!details.NameAndValue(key, item.itemID)) continue;
    key.Format("item_%d_quantity", i);
    details.NameAndValue(key, item.qty);
    items.m_items.Add(item);
  };
  return;
}
