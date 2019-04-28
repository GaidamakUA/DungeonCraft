#include "StdAfx.h"
#include "Translate.h"

//
extern FILE *f;
extern CString FRUAExportDate, FRUAExportTime;
extern CString ConvertFRUAVersion;
extern CString executablePath;
void Line(const char *name, const char *value);
void Line(const char *name, int value);

#define Str(a) monster.Fetch_String(a)
#define Int(a) monster.Fetch_Int(a)

CString tag, leftTag, rightTag, tagValue;
int instance;
JReader *mon;

struct MONSTER_DATA
{
  CArray<CString, CString&> key;
  CArray<CString, CString&> value;
};

CArray<CString, CString&> monsterList;
CArray<MONSTER_DATA, MONSTER_DATA&> monsterData;

MONSTER_DATA *pMonster;

CString defaultMonsters[] = {
"KOBOLD",
"GOBLIN",
"ORC",
"HOBGOBLIN",
"ORC CHIEFTAIN",
"HOBGOBLIN LDR",
"GNOLL",
"OGRE",
"TROLL",
"HILL GIANT",
"FIRE GIANT",
"FROST GIANT",
"CLOUD GIANT",
"STORM GIANT",
"GIANT RAT",
"CARRION CRAWLER",
"ENORMOUS SPIDER",
"SKELETON",
"ZOMBIE",
"GHOUL",
"GHAST",
"WIGHT",
"WRAITH",
"MUMMY",
"SPECTRE",
"VAMPIRE",
"LICH",
"LIZARD MAN",
"LIZARD MAN KING",
"MINOTAUR",
"DISPLACER BEAST",
"BORING BEETLE",
"GRIFFON",
"HYDRA",
"WYVERN",
"BLACK DRAGON",
"BLUE DRAGON",
"GREEN DRAGON",
"RED DRAGON",
"WHITE DRAGON",
"DRACOLICH",
"BASILISK",
"GORGON",
"COCKATRICE",
"BEHOLDER",
"OGRE MAGE",
"BULETTE",
"SHAMBLING MOUND",
"MARGOYLE",
"DRACOLISK",
"MOBAT",
"BLACK PUDDING",
"OTYUGH",
"NEO OTYUGH",
"SALAMANDER",
"EFREETI",
"EARTH ELEMENTAL",
"FIRE ELEMENTAL",
"UMBER HULK",
"ETTIN",
"OWL BEAR",
"BUGBEAR",
"MEDUSA",
"GIANT SPIDER",
"PHASE SPIDER",
"POISONOUS SNAKE",
"HELL HOUND",
"GIANT CROCODILE",
"DRIDER",
"IRON GOLEM",
"RAKSHASA",
"PURPLE WORM",
"DROW CHAMPION",
"DROW PRIEST",
"DROW SORCERESS",
"DROW PRIESTESS",
"WARRIOR",
"CONJURER",
"ACOLYTE",
"GOON",
"ARCHER",
"THEURGIST",
"PRIEST",
"THUG",
"EVIL CHAMPION",
"MAGICIAN",
"DARK CLERIC",
"ROGUE",
"DARK KNIGHT",
"NECROMANCER",
"HIGH PRIEST",
"THIEF",
"DARK WARLORD",
"WIZARD",
"ARCHPRIEST",
"MASTER THIEF",
"DARK OVERLORD",
"MASTER WIZARD",
"DARK DISCIPLE",
"HIGH THIEF",
"VAMPIRE LORD",
"VAMPIRESS",
"DAZMILAR",
"OGRE SHAMAN",
"SIR DUTIOCS",
"VIDRUAND",
"HILL GNT SHAMAN",
"VAMPIRE PRIEST",
"DROW AMAZON",
"RAKSHASA RUKH",
"ROAD GUARD",
"KALLITHREA",
"YEMANDRA",
"KRONDASZ",
"ARDERIEL",
"TORNILEE",
"ALIAS",
"DRAGONBAIT",
"NACACIA",
"NPC ONE",
"VALA",
"SILK",
"CAPTAIN DAENOR",
"GRUNSCHKA",
"STORM",
"SHAL",
"RAIZEL",
""
};


/*
// The following table is from DungeonCraftEditor version 1.0107
//
//CONFIGID monsterIDs[] =
//{
//CONFIGID( "" ,CONFIG_DATA_object ,monsterObject),
//DEFID( "name"                    ,string ,Name),
//DEFID( "Key"                     ,long   ,Key),
//DEFID( "icon file"               ,spritedata   ,IconFile),
//DEFID( "miss sound"              ,file   ,MissSound),
//DEFID( "hit sound"               ,file   ,HitSound),
//DEFID( "move sound"              ,file   ,MoveSound),
//DEFID( "death sound"             ,file   ,DeathSound),
//DEFID( "intelligence"            ,long   ,Intelligence),
//DEFID( "armor class"             ,long   ,ArmorClass),
//DEFID( "movement rate"           ,long   ,MovementRate),
//DEFID( "hit dice"                ,float  ,HitDice),
//DEFID( "treat HD as hit points"  ,enum   ,UseHitDice),
//                                 "", "yes, "no"
//DEFID( "hit dice bonus"          ,long   ,HitDiceBonus),
//DEFID( "THAC0"                   ,long   ,THAC0),
//DEFIDz( "Number of Attacks"      ,long   ,NbrAttacks, true), // read-only
//DEFIDz( "Attack dice"            ,dice   ,AttackDice, true), // read-only
//DEFID( "magic resistance"        ,long   ,MagicResistance),
//DEFID( "size"                    ,enum   ,Size),
//                                 "", "small", "medium", "large"
//DEFID( "morale"                  ,long   ,Morale),
//DEFID( "experience point value"  ,long   ,XPValue),
//DEFID( "Special Ability"         ,string ,SpecialAbility),
//                                 name, value
////DEFID( "classes"                 ,flags  ,Classes),
//DEFID( "Class"                   ,string ,Class),
//DEFID( "Race"                    ,string ,Race),
//DEFID( "form"                    ,flags  ,Form),
//                                 "", "mammal", "animal", "snake", "giant", "large", "undead", "none"
//DEFID( "penalty"                 ,flags  ,Penalty),
//                                 "", "DwarfAC", "GnomeAC", "DwarfTHAC0", "GnomeTHAC0", "RangerDmg", "none"
//DEFID( "immunity"                ,flags  ,Immunity),
//                                 "", "poison", "death magic", "confusion", "vorpal weapon", "none" 
//DEFID( "Misc Options"            ,flags  ,MiscOptions),
//                                 "", "can be held/charmed", "affected by dispel evil", "none" 
//DEFID( "item"                    ,string ,Item),
//DEFID( "attack"                  ,string ,Attack),
//                                 sides, number, bonus, attackMessage, spellID
//DEFID( "Undead"                  ,string ,UndeadType),
//CONFIGID( "" ,CONFIG_DATA_end ,NULL)
//};
*/


/*
  Line("class",Str("class"));
  Line("name", Str("name"));
  //monsterList.Add(Str("name"));
  Line("Key"                     ,Str("id")); //"???long"); //   ,Key),
  for (i=1; i<10; i++)
  {
    CString tag, value;
    int        numDice,        sides,        bonus,        nbrAttacks;
    int unarmedNumDice, unarmedSides, unarmedBonus, unarmedNbrAttacks;
    tag.Format("attack_%d_damage_bonus", i);
    monster.Optional(); if (!monster.NameAndValue(tag, bonus)) continue;
    tag.Format("attack_%d_damage_dice_sides", i);
    monster.Optional(); if (!monster.NameAndValue(tag, sides)) continue;
    tag.Format("attack_%d_nbr_attacks_per_2_rounds", i);
    monster.Optional(); if (!monster.NameAndValue(tag, nbrAttacks)) continue;
    tag.Format("attack_%d_num_damage_dice", i);
    monster.Optional(); if (!monster.NameAndValue(tag, numDice)) continue;
    tag.Format("attack_%d_unarmed_damage_bonus", i);
    monster.Optional(); if (!monster.NameAndValue(tag, unarmedBonus)) continue;
    tag.Format("attack_%d_unarmed_damage_dice_sides", i);
    monster.Optional(); if (!monster.NameAndValue(tag, unarmedSides)) continue;
    tag.Format("attack_%d_unarmed_nbr_attacks_per_2_rounds", i);
    monster.Optional(); if (!monster.NameAndValue(tag, unarmedNbrAttacks)) continue;
    tag.Format("attack_%d_unarmed_num_damage_dice", i);
    monster.Optional(); if (!monster.NameAndValue(tag, unarmedNumDice)) continue;
    value.Format("%d,%d,%d,attacks", sides, numDice, bonus);
    if ((numDice == 0) && ( bonus == 0)) continue;
    Line("attack", value);
  };
  //Line("icon file"               ,"???spritedata"
  {
    CString iconFile;
    iconFile.Format("icon_%s.png", Str("name"));
    Line("icon file"               , iconFile); //   ,IconFile),
  };
  Line("miss sound"              ,"");
  Line("hit sound"               ,"");
  Line("move sound"              ,"");
  Line("death sound"             ,"");

  //Int("ability_intelligence_adjusted");
  //Int("ability_intelligence_base");
  Line("intelligence"            ,Str("ability_intelligence_base"));

  //Int("armor_class_adjusted");
  //Int("armor_class_base");
  Line("armor class"             ,Str("armor_class_base"));

  //Int("movement_adjusted");
  //Int("movement_base");
  Line("movement rate"           ,Str("movement_base"));

  Line("hit dice"                ,"0");
  Line("hit dice bonus"          ,"0"); //   ,HitDiceBonus),
  Line("treat HD as hit points"  ,"no");

  //Int("thaco_adjusted");
  //Int("thaco_base");
  Line("THAC0"                   ,Str("thaco_base"));

  // The following have been replaced with "attack" entries
  //Line("Number of Attacks"       ,"???long"); //   ,NbrAttacks, true), // read-only
  //Line("Attack dice"             ,"???dice"); //   ,AttackDice, true), // read-only

  //Int("magic_resist_base_percent");
  //Int("magic_resist_bonus");
  Line("magic resistance"        ,Str("magic_resist_base_percent"));

  //STR("size_icon");
  //Int("size_large_even_if_icon_1x1");
  {
    CString temp, v;
//   case 0: currMonster["size_icon"] = "small"; break; // zero not legit, but the default zombie uses it
//   case 1: currMonster["size_icon"] = "small"; break;
//   case 2: currMonster["size_icon"] = "tall"; break;
//   case 3: currMonster["size_icon"] = "wide"; break;
//   case 4: currMonster["size_icon"] = "big"; break;
//   case 5: currMonster["size_icone"] = "huge"; break; 
    temp = Str("size_icon");
    if (temp == "small") v = "small";
    if (temp == "tall") v =  "medium";
    if (temp == "wide") v = "medium";
    if (temp == "big") v = "large";
    if (temp == "huge") v = "large";
    Line("size"                    ,v);
  };

  //Int("morale_base");
  Line("morale"                  ,Str("morale_base"));

  //Int("experience_current");
  //Int("experience_granted_for_killing");
  //Int("experience_predrain");
  Line("experience point value"  ,Str("experience_current")); //   ,XPValue),

  //Line("Special Ability"         ,"???string"); // ,SpecialAbility),

  //Line("form"                    ,"???flags"); //  ,Form),

  //Line("penalty"                 ,"???flags"); //  ,Penalty),

  //Line("immunity"                ,"???flags"); //  ,Immunity),

  //Line("Misc Options"            ,"???flags"); //  ,MiscOptions),

  //Str("equip_item_01");
  //Int("equip_item_01_count");
  //Str("equip_item_02");
  //Int("equip_item_02_count");
  for(i=0; i<9; i++)
  {
    CString tag, itemName, value;
    int itemCount;
    tag.Format("equip_item_%02d", i);
    monster.Optional(); if (!monster.NameAndValue(tag, itemName)) continue;
    tag.Format("equip_item_%02d_count", i);
    monster.Optional(); if (!monster.NameAndValue(tag, itemCount)) continue;
    value.Format("%s,%d", itemName, itemCount);
    Line("item"                    ,value);
  };

  //STR("undead_type");
  Line("Undead"                  ,Str("undead_type"));


//Int("ability_charisma_adjusted");
//Int("ability_charisma_base");
//Int("ability_constitution_adjusted");
//Int("ability_constitution_base");
//Int("ability_dextericy_adjusted");
//Int("ability_dexterity_base");
//Int("ability_strength_adjusted");
//Int("ability_strength_base");
//Int("ability_strength_extraordinary_adjusted");
//Int("ability_strength_extraordinary_percentage");
//Int("ability_wisdom_adjusted");
//Int("ability_wisdom_base");
//Int("age_years");
//Str("alignment");
//Str("combat_mode");
//Str("combat_status");
//Int("cure_disease_allowed");
//Int("cure_disease_count");
//Str("current_status");
//Int("encumbrance_in_coins");
//Int("equip_num_hands_full");
//Str("gender");
//Int("hitpoints_adjusted");
//Int("hitpoints_base");
//Int("hitpoints_maximum");
//Int("hitpoints_predrain");
//Int("item_bundles_carried");
//Int("item_mask_can_equip_cleric");
//Int("item_mask_can_equip_fighter");
//Int("item_mask_can_equip_knight");
//Int("item_mask_can_equip_magic_user");
//Int("item_mask_can_equip_paladin_ranger");
//Int("item_mask_can_equip_thief");
//Int("level_current");
//Int("level_current_cleric");
//Int("level_current_fighter");
//Int("level_current_knight");
//Int("level_current_mage");
//Int("level_current_paladin");
//Int("level_current_ranger");
//Int("level_current_thief");
//Int("level_fighter_sweep");
//Int("level_former_class");
//Int("level_preclasschange_cleric");
//Int("level_preclasschange_fighter");
//Int("level_preclasschange_knight");
//Int("level_preclasschange_mage");
//Int("level_preclasschange_paladin");
//Int("level_preclasschange_ranger");
//Int("level_preclasschange_thief");
//Int("level_predrain_cleric");
//Int("level_predrain_fighter");
//Int("level_predrain_knight");
//Int("level_predrain_mage");
//Int("level_predrain_paladin");
//Int("level_predrain_ranger");
//Int("level_predrain_thief");
//Int("level_ready_to_train");
//Int("party_index");
//STR("race");
//Int("save_vs_breathweapon");
//Int("save_vs_paralysis_poison_deathmagic");
//Int("save_vs_petrification_polymorph");
//Int("save_vs_rod_staff_wand");
//Int("save_vs_spell");
//Int("special_flag_affected_by_dispel_evil");
//Int("special_flag_can_be_held_or_charmed");
//Int("special_flag_dwarf_has_hit_bonus");
//Int("special_flag_extra_dmg_from_ranger");
//Int("special_flag_gnome_has_hit_bonus");
//Int("special_flag_immune_to_confusion");
//Int("special_flag_immune_to_death_magic");
//Int("special_flag_immune_to_decapitation_from_vorpal_sword");
//Int("special_flag_immune_to_poison");
//Int("special_flag_is_a_animal");
//Int("special_flag_is_a_mammal");
//Int("special_flag_is_a_snake");
//Int("special_flag_is_a_true_giant");
//Int("special_flag_penalty_to_hit_dwarf");
//Int("special_flag_penalty_to_hit_gnome");
//Int("spells_num_cleric_level_1");
//Int("spells_num_cleric_level_2");
//Int("spells_num_cleric_level_3");
//Int("spells_num_cleric_level_4");
//Int("spells_num_cleric_level_5");
//Int("spells_num_cleric_level_6");
//Int("spells_num_cleric_level_7");
//Int("spells_num_druid_level_1");
//Int("spells_num_druid_level_2");
//Int("spells_num_druid_level_3");
//Int("spells_num_magicuser_level_1");
//Int("spells_num_magicuser_level_2");
//Int("spells_num_magicuser_level_3");
//Int("spells_num_magicuser_level_4");
//Int("spells_num_magicuser_level_5");
//Int("spells_num_magicuser_level_6");
//Int("spells_num_magicuser_level_7");
//Int("spells_num_magicuser_level_8");
//Int("spells_num_magicuser_level_9");
//Int("thieving_skill_climb_walls");
//Int("thieving_skill_find_remove_trap");
//Int("thieving_skill_hear_noise");
//Int("thieving_skill_hide_in_shadows");
//Int("thieving_skill_move_silent");
//Int("thieving_skill_openlock");
//Int("thieving_skill_pickpocket");
//Int("thieving_skill_read_languages");
//Int("wealth_num_gems");
//Int("wealth_num_jewelry");
//Int("wealth_num_platinum");

*/

int GetIntVal(const CString& tag)
{
  return mon->Fetch_Int(tag);
}

int FindLine(const CString& tag)
{
  int i, n;
  n = pMonster->key.GetSize();
  for (i=0; i<n; i++)
  {
    if (pMonster->key[i] == tag) return i;
  };
  return -1;
}

void InsertLine(const CString& tag, const CString& value)
{
  int n;
  n = pMonster->key.GetSize();
  pMonster->key.SetSize(n+1);
  pMonster->value.SetSize(n+1);
  pMonster->key[n] = tag;
  pMonster->value[n] = value;
}

void UpdateLine(const CString& tag, const CString& value)
{
  int i;
  i = FindLine(tag);
  if (i < 0)
  {
    InsertLine(tag, value);
  }
  else
  {
    pMonster->value[i] = value;
  };
}
void UpdateLine(const CString& tag, int value)
{
  CString text;
  text.Format("%d", value);
  UpdateLine(tag, text);
}

void RenameLine(const CString& tag, const CString& newTag)
{
  int i;
  while ((i = FindLine(tag)) >= 0)
  {
    pMonster->key[i] = newTag;
  };
}

void AddImmunity(const char *immunity)
{
  int i;
  i = FindLine("immunity");
  if (i < 0) InsertLine("immunity", immunity);
  else UpdateLine("immunity", pMonster->value[i] + "+" + immunity);
}

void UpdateSpecialAbility(const char *saName, const char *value)
{
  int i, n, comma;
  n = pMonster->key.GetSize();
  for (i=0; i<n; i++)
  {
    if (pMonster->key[i] == "Special Ability")
    {
      CString name;
      comma = pMonster->value[i].Find(',');
      if (comma < 0) comma = pMonster->value[i].GetLength();
      name = pMonster->value[i].Left(comma);
      if (name == saName)
      {
        pMonster->value[i] = name + "," + value;
        return;
      };
    };
  };
  InsertLine("Special Ability", CString("monster_Race,") + value);
}


void NOP(void)
{
}
void HandleAbilityCharisma(void)
{
  UpdateLine("//charisma", tagValue);
}
void HandleAbilityConstitution(void)
{
  UpdateLine("//constitution", tagValue);
}
void HandleAbilityIntelligence(void)
{
  UpdateLine("intelligence", tagValue);
}
void HandleAbilityStrength(void)
{
  UpdateLine("//strength", tagValue);
}
void HandleAbilityWisdom(void)
{
  UpdateLine("//wisdom", tagValue);
}
void HandleAbilityDexterity(void)
{
  UpdateLine("//dexterity", tagValue);
}
void HandleAge(void)
{
  UpdateLine("//age", tagValue);
}
void HandleAlignment(void)
{
  UpdateLine("//alignment", tagValue);
}
void HandleArmorClass(void)
{
  int value;
  value = GetIntVal("armor_class_adjusted") + GetIntVal("armor_class_base");
  UpdateLine("armor class", value);
}

int Attack_Int(const char *tag)
{
  CString temp;
  temp.Format("attack_%d_%s", instance, tag);
  return mon->Fetch_Int(temp);
};
void HandleAttack(void)
{
//DEFID( "attack"                  ,string ,Attack),
//                                 sides, number, bonus, attackMessage, spellID
  int damageBonus, damageDiceSides, nbrAttackPer2Rounds;
  int numDamageDice, unarmedDamageBonus, unarmedDamageDiceSides;
  int unarmedNbrAttacksPer2Rounds, unarmedNumDamageDice;
  CString attack;
  damageBonus                 = Attack_Int("damage_bonus");
  damageDiceSides             = Attack_Int("damage_dice_sides");
  nbrAttackPer2Rounds         = Attack_Int("nbr_attacks_per_2_rounds");
  numDamageDice               = Attack_Int("num_damage_dice");
  unarmedDamageBonus          = Attack_Int(  "unarmed_damage_bonus");
  unarmedDamageDiceSides      = Attack_Int(      "unarmed_damage_dice_sides");
  unarmedNbrAttacksPer2Rounds = Attack_Int("unarmed_nbr_attacks_per_2_rounds");
  unarmedNumDamageDice        = Attack_Int("unarmed_num_damage_dice");
  attack.Format("%d, %d, %d, Attacks, none",
                 unarmedDamageDiceSides,
                 unarmedNumDamageDice,
                 unarmedDamageBonus);
  InsertLine("attack", attack);
}
void HandleClass(void)
{
  UpdateLine("class", tagValue);
}
void HandleCombatMode(void)
{
  UpdateLine("//combat mode", tagValue);
}
void HandleCombatStatus(void)
{
  UpdateLine("//combat status", tagValue);
}
void HandleCureDiseaseAllowed(void)
{
  UpdateLine("//cure disease allowed", tagValue);
}
void HandleCurrentStatus(void)
{
  UpdateLine("//current status", tagValue);
}

CString Equip_Str(const char *tag)
{
  CString temp;
  temp.Format("equip_item_%02d_%s", instance, tag);
  return mon->Fetch_String(temp);
}
int Equip_Int(const char *tag)
{
  CString temp;
  temp.Format("equip_item_%02d_%s", instance, tag);
  return mon->Fetch_Int(temp);
}

void HandleEquipItem(void)
{
  CString item, id, equip;
  int count;
  item = tagValue;
  count = Equip_Int("count");
  id =    Equip_Str("id");
  {
    // Make sure the item exists.
    // Rename it if that makes sense.
    {
      LINE_DATA *pDefaultData;
      pDefaultData = defaultItemData.Find("name", item);
      if (pDefaultData == NULL)
      {
        // Perhaps we can rename this item.
        // Search the default list for the 'identified name"
        pDefaultData = defaultItemData.Find("id name", item);
        if (pDefaultData != NULL)
        {
          // Rename this monster's item to agree with the defaultItemDatabase.
          LINE_ENTRY *pDefaultLine;
          pDefaultLine = (*pDefaultData)["name"];
          if (pDefaultLine != NULL)
          {
            item = pDefaultLine->value;
          };
        };
      };
    };
  };
  equip.Format("%s, %d", item, count);
  InsertLine("item", equip);
}
void HandleExperiencePredrain(void)
{
  UpdateLine("//experience predrain", tagValue);
}
void HandleGender(void)
{
  UpdateLine("//gender", tagValue);
}
void HandleEncumbranceInCoins(void)
{
  UpdateLine("//encumbrance in coins", tagValue);
}
void HandleHitpoints(void)
{
  int hitpointsBase, hitpointsAdjusted, hitpointsMaximum, hitpointsPredrain;
  hitpointsBase = mon->Fetch_Int("hitpoints_base");
  hitpointsAdjusted = mon->Fetch_Int("hitpoints_adjusted");
  hitpointsMaximum = mon->Fetch_Int("hitpoints_maximum");
  hitpointsPredrain = mon->Fetch_Int("hitpoints_predrain");
  UpdateLine("hit dice", hitpointsMaximum);
  UpdateLine("treat HD as hit points", "yes");
  UpdateLine("//hitpoints adjusted", hitpointsAdjusted);
  UpdateLine("//hitpoints predrain", hitpointsPredrain);
  UpdateLine("//hitpoints base", hitpointsBase);
}
void HandleEquipNumHandsFull(void)
{
  UpdateLine("//equip num hands full", tagValue);
}
void HandleExperienceCurrent(void)
{
  UpdateLine("//experience current", tagValue);
}
void HandleExperienceGrantedForKilling(void)
{
  UpdateLine("experience point value", tagValue);
}
void HandleIconIndex(void)
{
  UpdateLine("//icon index", tagValue);
}
void HandleIconId(void)
{
  UpdateLine("//icon id", tagValue);
}
void HandleItemBundlesCarried(void)
{
  UpdateLine("//bundles carried", tagValue);
}
void HandleItemMaskCanEquipCleric(void)
{
  UpdateLine("//can equip cleric", tagValue);
}
void HandleItemMaskCanEquipFighter(void)
{
  UpdateLine("//can equip fighter", tagValue);
}
void HandleItemMaskCanEquipKnight(void)
{
  UpdateLine("//can equip knight", tagValue);
}
void HandleItemMaskCanEquipMagicUser(void)
{
  UpdateLine("//can equip magic user", tagValue);
}
void HandleItemMaskCanEquipPaladinRanger(void)
{
  UpdateLine("//can equip paladin ranger", tagValue);
}
void HandleItemMaskCanEquipThief(void)
{
  UpdateLine("//can equip thief", tagValue);
}
void HandleLevelCurrent(void)
{
  UpdateLine("//level current", tagValue);
}
void HandleLevelCurrentCleric(void)
{
  UpdateLine("//level current cleric", tagValue);
}
void HandleLevelCurrentFighter(void)
{
  UpdateLine("//level current fighter", tagValue);
}
void HandleLevelCurrentKnight(void)
{
  UpdateLine("//level current knight", tagValue);
}
void HandleLevelCurrentMage(void)
{
  UpdateLine("//level current mage", tagValue);
}
void HandleLevelCurrentPaladin(void)
{
  UpdateLine("//level current paladin", tagValue);
}
void HandleLevelCurrentRanger(void)
{
  UpdateLine("//level current ranger", tagValue);
}
void HandleLevelCurrentThief(void)
{
  UpdateLine("//level current thief", tagValue);
}
void HandleLevelFighterSweep(void)
{
  UpdateLine("//level fighter sweep", tagValue);
}
void HandleLevelFormerClass(void)
{
  UpdateLine("//level former class", tagValue);
}
void HandleLevelPreClassChangeCleric(void)
{
  UpdateLine("//level pre class change cleric", tagValue);
}
void HandleLevelPreClassChangeFighter(void)
{
  UpdateLine("//level pre class change fighter", tagValue);
}
void HandleLevelPreClassChangeKnight(void)
{
  UpdateLine("//level pre class change knight", tagValue);
}
void HandleLevelPreClassChangeMage(void)
{
  UpdateLine("//level pre class change mage", tagValue);
}
void HandleLevelPreClassChangePaladin(void)
{
  UpdateLine("//level pre class change paladin", tagValue);
}
void HandleLevelPreClassChangeRanger(void)
{
  UpdateLine("//level pre class change ranger", tagValue);
}
void HandleLevelPreClassChangeThief(void)
{
  UpdateLine("//level pre class change thief", tagValue);
}
void HandleLevelPreDrainCleric()
{
  UpdateLine("//level pre drain cleric", tagValue);
}
void HandleLevelPreDrainFighter()
{
  UpdateLine("//level pre drain fighter", tagValue);
}
void HandleLevelPreDrainKnight()
{
  UpdateLine("//level pre drain knight", tagValue);
}
void HandleLevelPreDrainMage()
{
  UpdateLine("//level pre drain mage", tagValue);
}
void HandleLevelPreDrainPaladin()
{
  UpdateLine("//level pre drain paladin", tagValue);
}
void HandleLevelPreDrainRanger()
{
  UpdateLine("//level pre drain ranger", tagValue);
}
void HandleLevelPreDrainThief()
{
  UpdateLine("//level pre drain thief", tagValue);
}
void HandleLevelReadyToTrain()
{
  UpdateLine("//level ready to train", tagValue);
}
void HandleMagicResist()
{
  int base, bonus;
  base = mon->Fetch_Int("magic_resist_base_percent");
  bonus = mon->Fetch_Int("magic_resist_bonus");
  UpdateLine("magic resistance", base);
}
void HandleMorale()
{
  UpdateLine("morale", tagValue);
}
void HandleMovement()
{
  UpdateLine("movement rate", tagValue);
}
void HandleRace()
{
  UpdateSpecialAbility("monster_Race", tagValue);
}
void HandleSaveVsBreathWeapon(void)
{
}
void HandleSaveVsParalysisPoisonDeathmagic(void)
{
}
void HandleSaveVsPetrificationPolymorph(void)
{
}
void HandleSaveVsRodStaffWand(void)
{
}
void HandleSaveVsSpell(void)
{
}
void HandleSizeIcon(void)
{
}
void HandleSizeLargeEvenIfIcon1x1(void)
{
}
void HandleSpecialFlagAffectedByDispelEvil(void)
{
}
void HandleSpecialFlagCanBeHeldOrCharmed(void)
{
}
void HandleSpecialFlagDwarfHasHitBonus(void)
{
}
void HandleSpecialFlagExtraDmgFromRanger(void)
{
}
void HandleSpecialFlagGnomeHasHitBonus(void)
{
}
void HandleSpecialFlagImmuneToConfusion(void)
{
}
void HandleSpecialFlagImmuneToDeathMagic(void)     
{
}
void HandleSpecialFlagImmuneToDecapitationFromVorpalSword(void)
{
}
void HandleSpecialFlagImmuneToPoison(void)
{
}
void HandleSpecialFlagIsAAnimal(void)
{
}
void HandleSpecialFlagIsAMammal(void)
{
}
void HandleSpecialFlagIsASnake(void)
{
}
void HandleSpecialFlagIsATrueGiant(void)
{
}
void HandleSpecialFlagPenaltyToHitDwarf(void)
{
}
void HandleSpecialFlagPenaltyToHitGnome(void)
{
}
void HandleNumSpellCleric(void)
{
}
void HandleNumSpellDruid(void)
{
}
void HandleNumSpellMagicUser(void)
{
}
void HandleTHAC0(void)
{
}
void HandleThievingSkillClimbWalls(void)
{
}
void HandleThievingSkillFindRemoveTrap(void)
{
}
void HandleThievingSkillHearNoise(void)
{
}
void HandleThievingSkillHideInShadows(void)
{
}
void HandleThievingSkillMoveSilent(void)
{
}
void HandleThievingSkillOpenLock(void)
{
}
void HandleThievingSkillPickpocket(void)
{
}
void HandleThievingSkillReadLanguages(void)
{
}
void HandleUndeadType(void)
{
}
void HandleWealthGems(void)
{
}
void HandleWealthJewelry(void)
{
}
void HandleWealthPlatinum(void)
{
}
void HandleSpecialAbility01(void)
{
}
void HandleSpecialAbility02(void)
{
}
void HandleSpecialAbility03(void)
{
}
void HandleSpecialAbility04(void)
{
}
void HandleSpecialAbility05(void)
{
}
void HandleSpecialAbility06(void)
{
}
void HandleSpecialAbility07(void)
{
}
void HandleSpecialAbility08(void)
{
}
void HandleSpecialAbility09(void)
{
}
void HandleSpecialAbility10(void)
{
}
void HandleSpellsMemorized(void)
{
}


struct TAG_ENTRY
{
  const char *tag;
  void (*function)(void);
};

#define PROCESS_TAG(tag,func) {tag, func},

TAG_ENTRY tagTable[] =
{
    PROCESS_TAG("ability_charisma_adjusted",                 ,HandleAbilityCharisma)
    PROCESS_TAG("ability_charisma_base"                      ,NOP)
    PROCESS_TAG("ability_constitution_adjusted",             ,HandleAbilityConstitution)
    PROCESS_TAG("ability_constitution_base"                  ,NOP)
    PROCESS_TAG("ability_intelligence_adjusted",             ,HandleAbilityIntelligence)
    PROCESS_TAG("ability_intelligence_base"                  ,NOP)
    PROCESS_TAG("ability_strength_adjusted",                 ,HandleAbilityStrength)
    PROCESS_TAG("ability_strength_base"                      ,NOP)
    PROCESS_TAG("ability_strength_extraordinary_adjusted"    ,NOP)
    PROCESS_TAG("ability_strength_extraordinary_percentage"  ,NOP)
    PROCESS_TAG("ability_wisdom_adjusted",                   ,HandleAbilityWisdom)
    PROCESS_TAG("ability_wisdom_base"                        ,NOP)
    PROCESS_TAG("ability_dexterity_adjusted",                ,HandleAbilityDexterity)
    PROCESS_TAG("ability_dexterity_base"                     ,NOP)
    PROCESS_TAG("age_years",                                 ,HandleAge)
    PROCESS_TAG("alignment",                                 ,HandleAlignment)
    PROCESS_TAG("armor_class_adjusted",                      ,HandleArmorClass)
    PROCESS_TAG("armor_class_base"                           ,NOP)
    PROCESS_TAG("attack_%i_damage_bonus"                     ,HandleAttack)
    PROCESS_TAG("attack_%i_damage_dice_sides"                ,NOP)
    PROCESS_TAG("attack_%i_nbr_attacks_per_2_rounds"         ,NOP)
    PROCESS_TAG("attack_%i_num_damage_dice"                  ,NOP)
    PROCESS_TAG("attack_%i_unarmed_damage_bonus"             ,NOP)
    PROCESS_TAG("attack_%i_unarmed_damage_dice_sides"        ,NOP)
    PROCESS_TAG("attack_%i_unarmed_nbr_attacks_per_2_rounds" ,NOP)
    PROCESS_TAG("attack_%i_unarmed_num_damage_dice"          ,NOP)
    PROCESS_TAG("class",                                     ,HandleClass)
    PROCESS_TAG("combat_mode",                               ,HandleCombatMode)
    PROCESS_TAG("combat_status",                             ,HandleCombatStatus)
    PROCESS_TAG("cure_disease_allowed",                      ,HandleCureDiseaseAllowed)
    PROCESS_TAG("cure_disease_count"                         ,NOP)
    PROCESS_TAG("current_status",                            ,HandleCurrentStatus)
    PROCESS_TAG("encumbrance_in_coins",                      ,HandleEncumbranceInCoins)
    PROCESS_TAG("equip_item_%i",                             ,HandleEquipItem)
    PROCESS_TAG("equip_item_%i_count"                        ,NOP)
    PROCESS_TAG("equip_item_%i_id"                           ,NOP)
    PROCESS_TAG("equip_num_hands_full",                      ,HandleEquipNumHandsFull)
    PROCESS_TAG("experience_current",                        ,HandleExperienceCurrent)
    PROCESS_TAG("experience_granted_for_killing",            ,HandleExperienceGrantedForKilling)
    PROCESS_TAG("experience_predrain",                       ,HandleExperiencePredrain)
    PROCESS_TAG("gender",                                    ,HandleGender)
    PROCESS_TAG("hitpoints_adjusted",                        ,HandleHitpoints)
    PROCESS_TAG("hitpoints_base"                             ,NOP)
    PROCESS_TAG("hitpoints_maximum"                          ,NOP)
    PROCESS_TAG("hitpoints_predrain"                         ,NOP)
    PROCESS_TAG("icon_index",                                ,HandleIconIndex)
    PROCESS_TAG("id",                                        ,HandleIconId)
    PROCESS_TAG("item_bundles_carried",                      ,HandleItemBundlesCarried)
    PROCESS_TAG("item_mask_can_equip_cleric",                ,HandleItemMaskCanEquipCleric)
    PROCESS_TAG("item_mask_can_equip_fighter",               ,HandleItemMaskCanEquipFighter)
    PROCESS_TAG("item_mask_can_equip_knight",                ,HandleItemMaskCanEquipKnight)
    PROCESS_TAG("item_mask_can_equip_magic_user",            ,HandleItemMaskCanEquipMagicUser)
    PROCESS_TAG("item_mask_can_equip_paladin_ranger",        ,HandleItemMaskCanEquipPaladinRanger)
    PROCESS_TAG("item_mask_can_equip_thief",                 ,HandleItemMaskCanEquipThief)
    PROCESS_TAG("level_current",                             ,HandleLevelCurrent)
    PROCESS_TAG("level_current_cleric",                      ,HandleLevelCurrentCleric)
    PROCESS_TAG("level_current_fighter",                     ,HandleLevelCurrentFighter)
    PROCESS_TAG("level_current_knight",                      ,HandleLevelCurrentKnight)
    PROCESS_TAG("level_current_mage",                        ,HandleLevelCurrentMage)
    PROCESS_TAG("level_current_paladin",                     ,HandleLevelCurrentPaladin)
    PROCESS_TAG("level_current_ranger",                      ,HandleLevelCurrentRanger)
    PROCESS_TAG("level_current_thief",                       ,HandleLevelCurrentThief)
    PROCESS_TAG("level_fighter_sweep",                       ,HandleLevelFighterSweep)
    PROCESS_TAG("level_former_class",                        ,HandleLevelFormerClass)
    PROCESS_TAG("level_preclasschange_cleric",               ,HandleLevelPreClassChangeCleric)
    PROCESS_TAG("level_preclasschange_fighter",              ,HandleLevelPreClassChangeFighter)
    PROCESS_TAG("level_preclasschange_knight",               ,HandleLevelPreClassChangeKnight)
    PROCESS_TAG("level_preclasschange_mage",                 ,HandleLevelPreClassChangeMage)
    PROCESS_TAG("level_preclasschange_paladin",              ,HandleLevelPreClassChangePaladin)
    PROCESS_TAG("level_preclasschange_ranger",               ,HandleLevelPreClassChangeRanger)
    PROCESS_TAG("level_preclasschange_thief",                ,HandleLevelPreClassChangeThief)
    PROCESS_TAG("level_predrain_cleric",                     ,HandleLevelPreDrainCleric)
    PROCESS_TAG("level_predrain_fighter",                    ,HandleLevelPreDrainFighter)
    PROCESS_TAG("level_predrain_knight",                     ,HandleLevelPreDrainKnight)
    PROCESS_TAG("level_predrain_mage",                       ,HandleLevelPreDrainMage)
    PROCESS_TAG("level_predrain_paladin",                    ,HandleLevelPreDrainPaladin)
    PROCESS_TAG("level_predrain_ranger",                     ,HandleLevelPreDrainRanger)
    PROCESS_TAG("level_predrain_thief",                      ,HandleLevelPreDrainThief)
    PROCESS_TAG("level_ready_to_train",                      ,HandleLevelReadyToTrain)
    PROCESS_TAG("magic_resist_base_percent",                 ,HandleMagicResist)
    PROCESS_TAG("magic_resist_bonus"                         ,NOP)
    PROCESS_TAG("morale_base",                               ,HandleMorale)
    PROCESS_TAG("movement_adjusted",                         ,HandleMovement)
    PROCESS_TAG("movement_base"                              ,NOP)
    PROCESS_TAG("name"                                       ,NOP)
    PROCESS_TAG("party_index"                                ,NOP)
    PROCESS_TAG("race",                                      ,HandleRace)
    PROCESS_TAG("save_vs_breathweapon",                      ,HandleSaveVsBreathWeapon)
    PROCESS_TAG("save_vs_paralysis_poison_deathmagic",       ,HandleSaveVsParalysisPoisonDeathmagic)
    PROCESS_TAG("save_vs_petrification_polymorph",           ,HandleSaveVsPetrificationPolymorph)
    PROCESS_TAG("save_vs_rod_staff_wand",                    ,HandleSaveVsRodStaffWand)
    PROCESS_TAG("save_vs_spell",                             ,HandleSaveVsSpell)
    PROCESS_TAG("size_icon",                                 ,HandleSizeIcon)
    PROCESS_TAG("size_large_even_if_icon_1x1",               ,HandleSizeLargeEvenIfIcon1x1)
    PROCESS_TAG("special_flag_affected_by_dispel_evil",      ,HandleSpecialFlagAffectedByDispelEvil)
    PROCESS_TAG("special_flag_can_be_held_or_charmed",       ,HandleSpecialFlagCanBeHeldOrCharmed)
    PROCESS_TAG("special_flag_dwarf_has_hit_bonus",          ,HandleSpecialFlagDwarfHasHitBonus)
    PROCESS_TAG("special_flag_extra_dmg_from_ranger",        ,HandleSpecialFlagExtraDmgFromRanger)
    PROCESS_TAG("special_flag_gnome_has_hit_bonus",          ,HandleSpecialFlagGnomeHasHitBonus)
    PROCESS_TAG("special_flag_immune_to_confusion",          ,HandleSpecialFlagImmuneToConfusion)
    PROCESS_TAG("special_flag_immune_to_death_magic",        ,HandleSpecialFlagImmuneToDeathMagic)
    PROCESS_TAG("special_flag_immune_to_decapitation_from_vorpal_sword",      
                                                             ,HandleSpecialFlagImmuneToDecapitationFromVorpalSword)
    PROCESS_TAG("special_flag_immune_to_poison",             ,HandleSpecialFlagImmuneToPoison)
    PROCESS_TAG("special_flag_is_a_animal",                  ,HandleSpecialFlagIsAAnimal)
    PROCESS_TAG("special_flag_is_a_mammal",                  ,HandleSpecialFlagIsAMammal)
    PROCESS_TAG("special_flag_is_a_snake",                   ,HandleSpecialFlagIsASnake)
    PROCESS_TAG("special_flag_is_a_true_giant",              ,HandleSpecialFlagIsATrueGiant)
    PROCESS_TAG("special_flag_penalty_to_hit_dwarf",         ,HandleSpecialFlagPenaltyToHitDwarf)
    PROCESS_TAG("special_flag_penalty_to_hit_gnome",         ,HandleSpecialFlagPenaltyToHitGnome)
    PROCESS_TAG("spells_num_cleric_level_1"                  ,HandleNumSpellCleric)
    PROCESS_TAG("spells_num_cleric_level_2"                  ,NOP)
    PROCESS_TAG("spells_num_cleric_level_3"                  ,NOP)
    PROCESS_TAG("spells_num_cleric_level_4"                  ,NOP)
    PROCESS_TAG("spells_num_cleric_level_5"                  ,NOP)
    PROCESS_TAG("spells_num_cleric_level_6"                  ,NOP)
    PROCESS_TAG("spells_num_cleric_level_7"                  ,NOP)
    PROCESS_TAG("spells_num_druid_level_1",                  ,HandleNumSpellDruid)
    PROCESS_TAG("spells_num_druid_level_2"                   ,NOP)
    PROCESS_TAG("spells_num_druid_level_3"                   ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_1",              ,HandleNumSpellMagicUser)
    PROCESS_TAG("spells_num_magicuser_level_2"               ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_3"               ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_4"               ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_5"               ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_6"               ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_7"               ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_8"               ,NOP)
    PROCESS_TAG("spells_num_magicuser_level_9"               ,NOP)
    PROCESS_TAG("thaco_adjusted"                             ,HandleTHAC0)
    PROCESS_TAG("thaco_base"                                 ,NOP)
    PROCESS_TAG("thieving_skill_climb_walls"                 ,HandleThievingSkillClimbWalls)
    PROCESS_TAG("thieving_skill_find_remove_trap"            ,HandleThievingSkillFindRemoveTrap)
    PROCESS_TAG("thieving_skill_hear_noise"                  ,HandleThievingSkillHearNoise)
    PROCESS_TAG("thieving_skill_hide_in_shadows"             ,HandleThievingSkillHideInShadows)
    PROCESS_TAG("thieving_skill_move_silent"                 ,HandleThievingSkillMoveSilent)
    PROCESS_TAG("thieving_skill_openlock"                    ,HandleThievingSkillOpenLock)
    PROCESS_TAG("thieving_skill_pickpocket"                  ,HandleThievingSkillPickpocket)
    PROCESS_TAG("thieving_skill_read_languages"              ,HandleThievingSkillReadLanguages)
    PROCESS_TAG("undead_type"                                ,HandleUndeadType)
    PROCESS_TAG("wealth_num_gems"                            ,HandleWealthGems)
    PROCESS_TAG("wealth_num_jewelry"                         ,HandleWealthJewelry)
    PROCESS_TAG("wealth_num_platinum"                        ,HandleWealthPlatinum)
    PROCESS_TAG("special_ability_01"                         ,HandleSpecialAbility01)
    PROCESS_TAG("special_ability_02"                         ,HandleSpecialAbility02)
    PROCESS_TAG("special_ability_03"                         ,HandleSpecialAbility03)
    PROCESS_TAG("special_ability_04"                         ,HandleSpecialAbility04)
    PROCESS_TAG("special_ability_05"                         ,HandleSpecialAbility05)
    PROCESS_TAG("special_ability_06"                         ,HandleSpecialAbility06)
    PROCESS_TAG("special_ability_07"                         ,HandleSpecialAbility07)
    PROCESS_TAG("special_ability_08"                         ,HandleSpecialAbility08)
    PROCESS_TAG("special_ability_09"                         ,HandleSpecialAbility09)
    PROCESS_TAG("special_ability_10"                         ,HandleSpecialAbility10)
    PROCESS_TAG("spells_memorized_%i"                        ,HandleSpellsMemorized)
    {"", NOP}
};

bool TagMatch(const char *pattern, const char *tag)
{
  // Pattern can have up to three parts.
  // [leftTag [+ %i]] + rightTag
  // The integer is placed in 'instance'
  int i, j, startCol;
  i = 0;
  j = 0;
  startCol = 0;
  leftTag.Empty();
  rightTag.Empty();
  for (;;)
  {
    if (tag[j] == pattern[i])
    {
      if (pattern[i] == 0)
      {
        rightTag = CString(tag+startCol, i-startCol);
        return true;
      };
      i++;
      j++;
    }
    else
    {
      if (pattern[i] == '%')
      {
        i++;
        if (pattern[i] == 'i')
        {
          instance = 0;
          while ((tag[j] >= '0') && (tag[j] <= '9'))
          {
            instance = 10*instance + tag[j] - '0';
            j++;
          };
          i++;
        }
        else
        {
          return false;
        };
      }
      else
      {
        return false;
      };
    };
  };
}


void ConvertOneMonster(JReader& monster)
{
  int i, n;
  CString monsterName;
  mon = &monster;
  monster.StartList(DBGID_oneMonster);

  monsterName = monster.Fetch_String("name");
  n = monsterData.GetSize();
  for (i=0; i<n; i++)
  {
    int j, m;
    pMonster = &monsterData[i];
    m = pMonster->key.GetSize();
    for (j=0; j<m; j++)
    {
      if (pMonster->key[j] == "name")
      {
        if (pMonster->value[j].CompareNoCase(monsterName) == 0)
        {
          break;
        };
      };
    };
    if (j != m) break;
  }
  if (i == n)
  {
    CString msg;
    msg.Format("JSON file references non-existent default monster \"%s\"", monsterName);
    monster.PrintError(msg);
    if (n > 1) // Skip template
    {
      int m, j;
      m = monsterData[1].key.GetSize();
      for (j=0; j<m; j++)
      {
        if (monsterData[1].key[j] == "name")
        {
          monsterData.SetSize(n+1);
          monsterData[n].key.Append(monsterData[1].key);
          monsterData[n].value.Append(monsterData[1].value);
          monsterData[n].value[j] = monsterName;
          pMonster = &monsterData[i];
          n++;
          break;
        };
      };
    };
  };
  if (i == n)
  {
    monster.EndList(DBGID_oneMonster);
    return;
  };
  {
    CString line;
    line.Format("%d", monster.LineNum());
    UpdateLine("//JSON Line", CString(line));
  };

  RenameLine("attack", "//attack");
  RenameLine("immunity", "//immunity");
  for (; 
       !monster.CurrentTag().IsEmpty();
       monster.NextTag())
  {
    tag   = monster.CurrentTag();
    tagValue = monster.CurrentValue();
    for (i=0; tagTable[i].tag[0]!=0; i++)
    {
      if (TagMatch(tagTable[i].tag, tag))
      {
        (*tagTable[i].function)();
        break;
      };
    };
    if (tagTable[i].tag[0] == 0)
    {
      CString msg;
      msg.Format("Unrecognized tag in JSON monster data = \"%s\"", tag);
      monster.PrintError(msg);
    };
  };
  monster.EndList(DBGID_oneMonster);
};


void ReadDCDefaultMonsters(FILE *out)
{
  FILE *f;
  char *line;
  int len;
  bool copyComments = true;
  f = fopen(executablePath + "DC_default_monsters.txt", "r");
  if (f == NULL)
  {
    char line[200];
    printf("Cannot open \"DC_default_monsters.txt\"\n");
    printf("Press Enter ---->");
    gets(line);
    printf("\n");
    return;
  };
  line = (char *)malloc(100000);
  while (fgets(line, 99999, f) != NULL)
  {
    len = strlen(line);
    while ( (len > 0) && (line[len-1] == '\n'))
    {
      len--;
      line[len] = 0;
    };
    if (len < 2) continue;
    if ((line[0] == '/') && (line[1] == '/'))
    {
      if (copyComments)
      {
        fprintf(out, "%s\n", line);
      };
      continue;
    }
    else
    {
      copyComments = false;
    };
    if (len < 8) continue;
    if (memcmp(line, "\\(BEGIN)", 8) == 0)
    {
      int i;
      i = monsterData.GetSize();
      monsterData.SetSize(i + 1);
      monsterData[i].key.SetSize(1);
      monsterData[i].value.SetSize(1);
      monsterData[i].key[0] = "//JSON Line";
      monsterData[i].value[0] = "????";
      while (fgets(line, 99999, f) != NULL)
      {
        int col, len, n;
        char *equal;
        len = strlen(line);
        while ((len > 0) && ((line[len-1] == '\n')||(line[len-1] == ' ')))
        {
          len--;
          line[len] = 0;
        };
        if ( (len == 6) && (memcmp(line, "\\(END)", 6) == 0)) 
        {
          break;
        };
        equal = strchr(line, '=');
        if (equal == NULL) continue;
        col = equal - line ;
        line[col] = 0;
        equal++;
        while ((col > 0) && (line[col-1] == ' ')) line[--col] = 0;
        if (col < 2) continue;
        n = monsterData[i].key.GetSize();
        monsterData[i].value.SetSize(n+1);
        monsterData[i].key.SetSize(n+1);
        while (*equal == ' ') equal++;
        while (*line == ' ') line++;
        monsterData[i].key[n] = CString(line);
        monsterData[i].value[n] = CString(equal);
      };
      pMonster = &monsterData[i];
      RenameLine("classes", "class");  
    };
  };
  free(line);
};

void ConvertMonsters(JReader& jr)
{
  if ( f != NULL)
  {
    die("Internal error in Monsters.cpp.  f not null");
  };
  f = fopen(jsonPath + "monsters.txt", "w");
  fprintf(f,"// Monster database file\n");
  fprintf(f,"// The line above **must** be the first line of the monsters file!!!\n\n");

  fprintf(f,"// FRUA Export Version %s:%s\n", FRUAExportDate, FRUAExportTime);
  fprintf(f,"// ConverFRUA version %s\n", ConvertFRUAVersion);
  monsterList.RemoveAll();
  ReadDCDefaultMonsters(f);
  while (jr.Optional(), jr.NextEntry())
  {
    JReader monster(jr);
    ConvertOneMonster(monster);
  };
  {
    int i, n;
    n = monsterData.GetSize();
    for (i=0; i<n; i++)
    {
      int j, m;
    fprintf(f,"\n\\(BEGIN)\n");
      m = monsterData[i].key.GetSize();
      for (j=0; j<m; j++)
      {
        Line(monsterData[i].key[j], monsterData[i].value[j]);
      };
    fprintf(f,"\\(END)\n");
    };
  };
  fclose(f);
  f = NULL;
}
