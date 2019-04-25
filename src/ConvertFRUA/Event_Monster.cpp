#include "stdafx.h"
#include "Translate.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool


extern CString defaultMonsters[];
extern CArray<CString, CString&> monsterList;

void Event_Monster(JReader& event, JReader& details, JWriter& jw)
{
  MONSTER_EVENT monster_event;
  monster_event.Read(details);
  monster_event.Export(jw);
}

/*
 "number":"2",
 "type":"MONSTER",
 "name":"GOBLIN",
 "friendly":"false",
 "moraleAdjustment":"0",
 "fixedNumberOfMonsters":"true",
 "money":{
    "gems":[],
    "jewels":[]
  },
 "possessions":[]
*/

void MONSTER_EVENT::Export(JWriter& jw)
{
  CString monsterName;
  int i;
  jw.StartList();
  jw.NameAndNonZero(JKEY_NUMBEROFMONSTERS, qty);
  jw.NameAndValue(JKEY_TYPE, MonsterType(m_type));
  {
    monsterName = monsterID;
    for (i=0; !defaultMonsters[i].IsEmpty(); i++)
    {
      if (monsterID == defaultMonsters[i])
      {
        if (i < monsterList.GetSize())
        {
          monsterName = monsterList[i];;
          break;
        };
      };
    };
  };
  jw.NameAndValue(JKEY_MONSTERNAME, monsterName);
  jw.NameAndBool(JKEY_FRIENDLY, friendly);
  jw.NameAndValue(JKEY_MORALEADJUST, moraleAdj);
  jw.NameAndNonZero(JKEY_DICESIDES, qtyDiceSides);
  jw.NameAndNonZero(JKEY_NUMBEROFDICE, qtyDiceQty);
  jw.NameAndNonZero(JKEY_DICEBONUS, qtyBonus);
  jw.NameAndBool(JKEY_USENUMBER, UseQty);
  money.Export(jw);
  items.Export(jw, JKEY_POSSESSIONS);
  jw.EndList();
}

bool MONSTER_EVENT::Read(JReader& details)
{
  CString stype;
  if (details.Optional(), !details.NextEntry()) return false;
  details.StartList(DBGID_monsterEventEntry);
  details.NameAndNonZero(JKEY_NUMBEROFMONSTERS, qty);
  details.NameAndValue(JKEY_TYPE, stype);
  m_type = MonsterType(stype);
  details.NameAndValue(JKEY_MONSTERNAME, monsterID);
  details.NameAndBool(JKEY_FRIENDLY, friendly);
  details.NameAndValue(JKEY_MORALEADJUST, moraleAdj);
  details.NameAndNonZero(JKEY_DICESIDES, qtyDiceSides);
  details.NameAndNonZero(JKEY_NUMBEROFDICE, qtyDiceQty);
  details.NameAndNonZero(JKEY_DICEBONUS, qtyBonus);
  BOOL useNumber;
  details.NameAndBool(JKEY_USENUMBER, useNumber);
  UseQty = !useNumber;
  money.Read(details);
  items.Read(details, JKEY_POSSESSIONS);
  details.EndList(DBGID_monsterEventEntry);
  return true;
}
/*
void MONSTER_EVENT_DATA::Export(JWriter& jw)
{
  int count, i;
  monsters.FreeExtra();
  count = GetCount();
  if (count == 0) return;
  jw.StartArray(JKEY_MONSTERS);
  for (i=0;i<count;i++)
  {
    jw.NextEntry();
    monsters[i].Export(jw);
  };
  jw.EndArray();
}
*/

