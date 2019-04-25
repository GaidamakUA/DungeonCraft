
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

extern int nxtAvailEventId;

#define Str details.Fetch_String
#define Int details.Fetch_Int



class SMALL_TOWN_DATA : public GameEvent
{
public:
  SMALL_TOWN_DATA(void)
  {
    hasTavern=hasTemple=hasVault=hasShop=hasInn=hasTrainingHall=false;
    TempleChain=TrainingHallChain=ShopChain=InnChain=TavernChain=VaultChain=0;
    highestSpellLevel = 0;
    canClericTrain=canFighterTrain=canMageTrain=canPaladinTrain=canRangerTrain=canThiefTrain=false;
    tavernTale.Empty();
    shopCostModifier = trainingCostModifier = 0;
    items.RemoveAll();
  };
  void Export(JWriter& jw);
  bool Read(JReader& event, JReader& jr);
  long Unused;
  DWORD TempleChain;
  DWORD TrainingHallChain;
  DWORD ShopChain;
  DWORD InnChain;
  DWORD TavernChain;
  DWORD VaultChain;

  CArray<CString, CString&> items;
  CString tavernTale;
  int  highestSpellLevel;
  int  trainingCostModifier;
  int  shopCostModifier;
  bool hasTavern;
  bool hasTemple;
  bool hasVault;
  bool hasShop;
  bool hasInn;
  bool hasTrainingHall;
  bool canClericTrain; 
  bool canFighterTrain; 
  bool canMageTrain; 
  bool canPaladinTrain; 
  bool canRangerTrain; 
  bool canThiefTrain; 

  CAMP_EVENT_DATA inn;
  SHOP shop;
  TEMPLE temple;
  TRAININGHALL trainingHall;
  VAULT_EVENT_DATA vault;

  void ReadInn(JReader& details);
  void ReadShop(JReader& details);
  void ReadTemple(JReader& details);
  void ReadTrainingHall(JReader& details);
  void ReadVault(JReader& details);
 
  void CreateTavern(JWriter& jw, int lineNum);
  void CreateTemple(JWriter& jw, int lineNum);
  void CreateVault(JWriter& jw, int lineNum);
  void CreateInn(JWriter& jw, int lineNum);
  void CreateShop(JWriter& jw, int lineNum);
  void CreateTrainingHall(JWriter& jw, int lineNum);


};


void SMALL_TOWN_DATA::CreateTavern(JWriter& jw, int lineNum)
{
  if (hasTavern)
  {
    TAVERN tavern;
    tavern.jsonLineNum = lineNum;
    tavern.id = nxtAvailEventId++;
    tavern.control.eventTrigger = AlwaysTrigger;
    tavern.tales[0].tale = tavernTale;
    tavern.tales[0].count= 999;
    TavernChain = tavern.id;
    tavern.pic.filename = "Bartender";
    tavern.pic.picType = SmallPicDib;
    tavern.Export(jw);
    jw.EndList();
    jw.NextEntry();
    jw.StartList();
  };
}

void SMALL_TOWN_DATA::CreateTemple(JWriter& jw, int lineNum)
{
  if (hasTemple)
  {
    temple.jsonLineNum = lineNum;
    temple.id = nxtAvailEventId++;
    TempleChain = temple.id;
    temple.Export(jw);
    jw.EndList();
    jw.NextEntry();
    jw.StartList();
  };
}

void SMALL_TOWN_DATA::CreateVault(JWriter& jw, int lineNum)
{
  if (hasVault)
  {
    vault.jsonLineNum = lineNum;
    vault.id = nxtAvailEventId++;
    VaultChain = vault.id;
    vault.Export(jw);
    jw.EndList();
    jw.NextEntry();
    jw.StartList();
  };
}

void SMALL_TOWN_DATA::CreateInn(JWriter& jw, int lineNum)
{
  if (hasInn)
  {
    inn.jsonLineNum = lineNum;
    inn.id = nxtAvailEventId++;
    InnChain = inn.id;
    inn.Export(jw);
    jw.EndList();
    jw.NextEntry();
    jw.StartList();
  };
}

void SMALL_TOWN_DATA::CreateShop(JWriter& jw, int lineNum)
{
  if (hasShop)
  {
    shop.jsonLineNum = lineNum;
    shop.id = nxtAvailEventId++;
    ShopChain = shop.id;
    shop.canId = TRUE;
    shop.canApprGems = TRUE;
    shop.canApprJewels = TRUE;
    shop.costToId = 20;
    shop.Export(jw);
    jw.EndList();
    jw.NextEntry();
    jw.StartList();
  };
}

void SMALL_TOWN_DATA::CreateTrainingHall(JWriter& jw, int lineNum)
{
  if (hasTrainingHall)
  {
    trainingHall.jsonLineNum = lineNum;
    trainingHall.id = nxtAvailEventId++;
    TrainingHallChain = trainingHall.id;
    trainingHall.Export(jw);
    jw.EndList();
    jw.NextEntry();
    jw.StartList();
  };
}


void Event_SmallTown(JReader& event, JReader& details, JWriter& jw)
{
  SMALL_TOWN_DATA small_town_data;
  small_town_data.Read(event, details);

  small_town_data.CreateTavern(jw, small_town_data.jsonLineNum);
  small_town_data.CreateTemple(jw, small_town_data.jsonLineNum);
  small_town_data.CreateTrainingHall(jw, small_town_data.jsonLineNum);
  small_town_data.CreateShop(jw, small_town_data.jsonLineNum);
  small_town_data.CreateInn(jw, small_town_data.jsonLineNum);
  small_town_data.CreateVault(jw, small_town_data.jsonLineNum);

  small_town_data.Export(jw);
}



void SMALL_TOWN_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.StartList(JKEY_CHAINS);
  jw.Linefeed(false);

  jw.NameAndNonZero(JKEY_TEMPLE, TempleChain);
  jw.NameAndNonZero(JKEY_TRAIN, TrainingHallChain);
  jw.NameAndNonZero(JKEY_SHOP, ShopChain);
  jw.NameAndNonZero(JKEY_INN, InnChain);
  jw.NameAndNonZero(JKEY_TAVERN, TavernChain);
  jw.NameAndNonZero(JKEY_VAULT, VaultChain);
  jw.EndList();
  jw.Linefeed(true);
  jw.EndList();
}

void SMALL_TOWN_DATA::ReadVault(JReader& details)
{
  if (!hasVault) return;
  vault.pic.filename = "Sasha";
  vault.pic.picType = SmallPicDib;
}


void SMALL_TOWN_DATA::ReadTrainingHall(JReader& details)
{
  CString temp;
  if (!hasTrainingHall) return;
  if (Int("can_cleric_train") != 0)  trainingHall.AddBaseclass("cleric");
  if (Int("can_fighter_train") != 0) trainingHall.AddBaseclass("fighter");
  if (Int("can_mage_train") != 0)    trainingHall.AddBaseclass("magic-user");
  if (Int("can_paladin_train") != 0) trainingHall.AddBaseclass("paladin");
  if (Int("can_ranger_train") != 0)  trainingHall.AddBaseclass("ranger");
  if (Int("can_thief_train") != 0)   trainingHall.AddBaseclass("thief");
  trainingHall.Cost = 1000;
  temp = Str("traininghall_temple_cost_modifier");
  if(temp == "Free")    trainingHall.Cost = trainingHall.Cost * 0;
  if(temp == "Div 100") trainingHall.Cost = trainingHall.Cost /100;
  if(temp == "Div 50")  trainingHall.Cost = trainingHall.Cost /50;
  if(temp == "Div 20")  trainingHall.Cost = trainingHall.Cost /20;
  if(temp == "Div 10")  trainingHall.Cost = trainingHall.Cost /10;
  if(temp == "Div 5")   trainingHall.Cost = trainingHall.Cost /5;
  if(temp == "Div 4")   trainingHall.Cost = trainingHall.Cost /4;
  if(temp == "Div 3")   trainingHall.Cost = trainingHall.Cost /3;
  if(temp == "Div 2")   trainingHall.Cost = trainingHall.Cost /2;
  if(temp == "Div 1.5") trainingHall.Cost = trainingHall.Cost *2/3; 
  if(temp == "Normal")  trainingHall.Cost = trainingHall.Cost *1;
  if(temp == "Mult 1.5")trainingHall.Cost = trainingHall.Cost *3/2;
  if(temp == "Mult 2")  trainingHall.Cost = trainingHall.Cost *2;
  if(temp == "Mult 3")  trainingHall.Cost = trainingHall.Cost *3;
  if(temp == "Mult 4")  trainingHall.Cost = trainingHall.Cost *4;
  if(temp == "Mult 5")  trainingHall.Cost = trainingHall.Cost *5;
  if(temp == "Mult 10") trainingHall.Cost = trainingHall.Cost *10;
  if(temp == "Mult 20") trainingHall.Cost = trainingHall.Cost *20;
  if(temp == "Mult 50") trainingHall.Cost = trainingHall.Cost *50;
  if(temp == "Mult 100")trainingHall.Cost = trainingHall.Cost *100;

  trainingHall.pic.filename = "Arms Master";
  trainingHall.pic.picType = SmallPicDib;
}


void SMALL_TOWN_DATA::ReadInn(JReader& details)
{
  if (!hasInn) return;
  inn.pic.filename = "Camp 3";
  inn.pic.picType = SmallPicDib;
}

void SMALL_TOWN_DATA::ReadShop(JReader& details)
{
  if (!hasShop) return;
  shop.pic.filename = "Shopkeeper";
  shop.pic.picType = SmallPicDib;
  {
    CString temp;
    temp = Str("shop_cost_modifier");
    if(temp == "Free")    shop.costFactor = Free;
    if(temp == "Div 100") shop.costFactor = Div100;
    if(temp == "Div 50")  shop.costFactor = Div50;
    if(temp == "Div 20")  shop.costFactor = Div20;
    if(temp == "Div 10")  shop.costFactor = Div10;
    if(temp == "Div 5")   shop.costFactor = Div5;
    if(temp == "Div 4")   shop.costFactor = Div4;
    if(temp == "Div 3")   shop.costFactor = Div3;
    if(temp == "Div 2")   shop.costFactor = Div2;
    if(temp == "Div 1.5") shop.costFactor = Div1_5; 
    if(temp == "Normal")  shop.costFactor = Normal;
    if(temp == "Mult 1.5")shop.costFactor = Mult1_5;
    if(temp == "Mult 2")  shop.costFactor = Mult2;
    if(temp == "Mult 3")  shop.costFactor = Mult3;
    if(temp == "Mult 4")  shop.costFactor = Mult4;
    if(temp == "Mult 5")  shop.costFactor = Mult5;
    if(temp == "Mult 10") shop.costFactor = Mult10;
    if(temp == "Mult 20") shop.costFactor = Mult20;
    if(temp == "Mult 50") shop.costFactor = Mult50;
    if(temp == "Mult 100")shop.costFactor = Mult100;
  };
  details.StartArray("shop_items");
  shop.itemsAvail.Clear();
  while (details.Optional(), details.NextEntry())
  {
    ITEM item;
    details.StartList(DBGID_TownShopItems);
    item.key = Int("id");
	  item.itemID = Str("name");
	  item.qty = Int("quantity");;
    item.ready = 0;
    item.identified = 0;
    item.charges = 0;
    item.cursed = 0;
    item.paid = 0;
    shop.itemsAvail.m_items.Add(item);
    details.EndList(DBGID_TownShopItems);
  }
  details.EndArray();
}

void SMALL_TOWN_DATA::ReadTemple(JReader& details)
{
  if (!hasTemple) return;
  {
    CString temp;
    temp = Str("traininghall_temple_cost_modifier");
    if(temp == "Free")    temple.costFactor = Free;
    if(temp == "Div 100") temple.costFactor = Div100;
    if(temp == "Div 50")  temple.costFactor = Div50;
    if(temp == "Div 20")  temple.costFactor = Div20;
    if(temp == "Div 10")  temple.costFactor = Div10;
    if(temp == "Div 5")   temple.costFactor = Div5;
    if(temp == "Div 4")   temple.costFactor = Div4;
    if(temp == "Div 3")   temple.costFactor = Div3;
    if(temp == "Div 2")   temple.costFactor = Div2;
    if(temp == "Div 1.5") temple.costFactor = Div1_5; 
    if(temp == "Normal")  temple.costFactor = Normal;
    if(temp == "Mult 1.5")temple.costFactor = Mult1_5;
    if(temp == "Mult 2")  temple.costFactor = Mult2;
    if(temp == "Mult 3")  temple.costFactor = Mult3;
    if(temp == "Mult 4")  temple.costFactor = Mult4;
    if(temp == "Mult 5")  temple.costFactor = Mult5;
    if(temp == "Mult 10") temple.costFactor = Mult10;
    if(temp == "Mult 20") temple.costFactor = Mult20;
    if(temp == "Mult 50") temple.costFactor = Mult50;
    if(temp == "Mult 100")temple.costFactor = Mult100;
  };
  temple.maxLevel = Int("highest_spell_level_offered");
  temple.pic.filename = "Temple Priest";
  temple.pic.picType = SmallPicDib;
  if (temple.maxLevel >= 1)
  {
    temple.templeSpells.Add("Cure Light Wounds|cleric|1|3", 1);
  };
  if (temple.maxLevel >= 3)
  {
    temple.templeSpells.Add("Cure Blindness|cleric|3|37", 3);
    temple.templeSpells.Add("Cure Disease|cleric|3|39", 3);
  };
  if (temple.maxLevel >= 4)
  {
    temple.templeSpells.Add("Neutralize Poison|cleric|4|67", 4);
    temple.templeSpells.Add("Cure Serious Wounds|cleric|4|58", 4);
  };
  if (temple.maxLevel >= 5)
  {
    temple.templeSpells.Add("Cure Critical Wounds|cleric|5|71", 5);
    temple.templeSpells.Add("Raise Dead|cleric|5|75", 5);
  };
  if (temple.maxLevel >= 6)
  {
    temple.templeSpells.Add("Stone To Flesh|magic-user|6|113", 6);
    temple.templeSpells.Add("Heal|cleric|6|36", 6);
  };
  if (temple.maxLevel >= 7)
  {
    temple.templeSpells.Add("Restoration|cleric|7|102", 7);
    temple.templeSpells.Add("Resurrection|cleric|7|105", 7);
  };

}

bool SMALL_TOWN_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, SmallTown);

  details.NameAndBool("has_inn",hasInn);
  details.NameAndBool("has_shop",hasShop);
  details.NameAndBool("has_tavern",hasTavern);
  details.NameAndBool("has_temple" ,hasTemple);
  details.NameAndBool("has_traininghall",hasTrainingHall);
  details.NameAndBool("has_vault",hasVault);
 

  details.NameAndValue("text", text);
  details.NameAndValue("tavern_tale", tavernTale);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
  details.NameAndValue("traininghall_temple_cost_modifier", trainingCostModifier);
  details.NameAndValue("shop_cost_modifier", shopCostModifier);
  details.NameAndBool("can_cleric_train", canClericTrain);
  details.NameAndBool("can_fighter_train", canFighterTrain);
  details.NameAndBool("can_mage_train", canMageTrain);
  details.NameAndBool("can_paladin_train", canPaladinTrain);
  details.NameAndBool("can_ranger_train", canRangerTrain);
  details.NameAndBool("can_thief_train", canThiefTrain);

  ReadInn(details);
  ReadShop(details);
  ReadTemple(details);
  ReadTrainingHall(details);
  ReadVault(details);

  details.EndList(DBGID_smallTown);
  return true;
}
