/******************************************************************************
* Filename: FRUAMonster.cpp
* Copyright (c) 2013, UAF Development Team (email CocoaSpud@hotmail.com)
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
******************************************************************************/
#include "stdafx.h"
#include <string>

#include "globals.h"
#include "FRUAMonster.h"
#include "FRUAStructs.h"
#include "FRUAItems.h"
#include "Utilities.h"
#include "FRUAStringTables.h"

bool ImportFRUAMonsterDat(const char *path)
{
   bool success = TRUE;
   
   FILE *fp = fopen(path, "rb");

   if (fp == NULL)
      return false;

   ImportFRUAMonster cch;
   success = (fread(&cch, sizeof(cch), 1, fp) == 1);

   fclose(fp);    

   if (!success)
      return false;   

   ASSERT(cch.monsterIndex >= 0);
   ASSERT(cch.monsterIndex < MAX_FRUA_MONSTERS);

   MonstersAndNPCs[cch.monsterIndex] = cch;

   //printf("Imported FRUA npc/monster %s from %s\n", MonstersAndNPCs[cch.monsterIndex].name, path);
   return success;
}


void FormatMonsterJson(ImportFRUAMonster monster, Json::Value &currMonster)
{
   std::string monsterName(monster.name);
   currMonster["name"] = monsterName;

   currMonster["experience_current"] = IntToString(monster.exp);
   currMonster["experience_predrain"] = IntToString(monster.expPreDrain);
   currMonster["experience_granted_for_killing"] = IntToString(monster.expWorth);
   currMonster["wealth_num_platinum"] = IntToString(monster.platinum);
   currMonster["wealth_num_gems"] = IntToString(monster.gems);
   currMonster["wealth_num_jewelry"] = IntToString(monster.jewelry);
   currMonster["age_years"] = IntToString(monster.age);   
   currMonster["encumbrance_in_coins"] = IntToString(monster.encumb);

   currMonster["race"] = FRUAGetRaceText(monster.race);
   currMonster["class"] = FRUAGetClassText(monster.charClass);
   
   switch (monster.undead)
   {
   case 0: currMonster["undead_type"] = "none"; break;
   case 1: currMonster["undead_type"] = "skeleton"; break;
   case 2: currMonster["undead_type"] = "zombie"; break;
   case 3: currMonster["undead_type"] = "ghoul"; break;
   case 4: currMonster["undead_type"] = "shadow"; break;
   case 5: currMonster["undead_type"] = "wight"; break;
   case 6: currMonster["undead_type"] = "ghast"; break;
   case 7: currMonster["undead_type"] = "wraith"; break;
   case 8: currMonster["undead_type"] = "mummy"; break;
   case 9: currMonster["undead_type"] = "spectre"; break;
   case 10: currMonster["undead_type"] = "vampire"; break;
   case 11: currMonster["undead_type"] = "ghost"; break;
   case 12: currMonster["undead_type"] = "lich"; break;
   case 13: currMonster["undead_type"] = "special"; break;
   default: currMonster["undead_type"] = UnknownString; break;
   }

   if (monster.gender == 0)
      currMonster["gender"] = "male";
   else if (monster.gender == 1)
      currMonster["gender"] = "female";
   else
      currMonster["gender"] = UnknownString;

   switch (monster.alignment)
   {
   case 0: currMonster["alignment"] = "lawful good"; break;
   case 1: currMonster["alignment"] = "lawful neutral"; break;
   case 2: currMonster["alignment"] = "lawful evil"; break;
   case 3: currMonster["alignment"] = "neutral good"; break;
   case 4: currMonster["alignment"] = "neutral"; break;
   case 5: currMonster["alignment"] = "neutral evil"; break;
   case 6: currMonster["alignment"] = "chaotic good"; break;
   case 7: currMonster["alignment"] = "chaotic neutral"; break;
   case 8: currMonster["alignment"] = "chaotic evil"; break;
   default: currMonster["alignment"] = UnknownString; break;
   }

   switch (monster.status)
   {
   case 0: currMonster["current_status"] = "okay"; break;
   case 1: currMonster["current_status"] = "animated"; break;
   case 2: currMonster["current_status"] = "temp gone"; break;
   case 3: currMonster["current_status"] = "running"; break;
   case 4: currMonster["current_status"] = "unconscious"; break;
   case 5: currMonster["current_status"] = "dying"; break;
   case 6: currMonster["current_status"] = "dead"; break;
   case 7: currMonster["current_status"] = "petrified"; break;
   case 8: currMonster["current_status"] = "gone"; break;
   case 9: currMonster["current_status"] = "awaiting summons"; break;
   default: currMonster["current_status"] = UnknownString; break;
   }  

   switch (monster.combatMode)
   {
   case 0: currMonster["combat_mode"] = "npc"; break;
   case 1: currMonster["combat_mode"] = "monster"; break;
   default: currMonster["combat_mode"] = UnknownString; break;
   }

   switch (monster.active)
   {
   case 0: currMonster["combat_status"] = "down"; break;
   case 1: currMonster["combat_status"] = "active"; break;
   default: currMonster["combat_status"] = UnknownString; break;
   }

   currMonster["ability_strength_base"] = IntToString(monster.str);
   currMonster["ability_strength_adjusted"] = IntToString(monster.strMod);
   currMonster["ability_strength_extraordinary_percentage"] = IntToString(monster.strExt);
   currMonster["ability_strength_extraordinary_adjusted"] = IntToString(monster.strExtMod);
   currMonster["ability_intelligence_base"] = IntToString(monster.intel);
   currMonster["ability_intelligence_adjusted"] = IntToString(monster.intMod);
   currMonster["ability_wisdom_base"] = IntToString(monster.wis);
   currMonster["ability_wisdom_adjusted"] = IntToString(monster.wisMod);
   currMonster["ability_dexterity_base"] = IntToString(monster.dex);
   currMonster["ability_dexterity_adjusted"] = IntToString(monster.dexMod);
   currMonster["ability_constitution_base"] = IntToString(monster.con);
   currMonster["ability_constitution_adjusted"] = IntToString(monster.conMod);
   currMonster["ability_charisma_base"] = IntToString(monster.cha);
   currMonster["ability_charisma_adjusted"] = IntToString(monster.chaMod);

   currMonster["thaco_base"] = IntToString( 60 - monster.thac0 );
   if (monster.adjThac0 != 0)
      currMonster["thaco_adjusted"] = IntToString( 60 - monster.adjThac0 );
   else
      currMonster["thaco_adjusted"] = IntToString( monster.adjThac0 );

   if (monster.cureDisease == 0)
      currMonster["cure_disease_allowed"] = "0";
   else
      currMonster["cure_disease_allowed"] = "1";
   currMonster["cure_disease_count"] = IntToString( monster.cureDisease );
   
   currMonster["hitpoints_maximum"] = IntToString( monster.maxHp );
   currMonster["hitpoints_predrain"] = IntToString( monster.hpPreDrain );
   currMonster["hitpoints_base"] = IntToString( monster.hp );
   currMonster["hitpoints_adjusted"] = IntToString( monster.adjHp );

   BYTE largeEvenIf1x1 = (0x80 & monster.size) ? 1 : 0;
   if (largeEvenIf1x1)
      currMonster["size_large_even_if_icon_1x1"] = "1";
   else
      currMonster["size_large_even_if_icon_1x1"] = "0";

   switch ( monster.size & 0x0F)
   {
   case 0: currMonster["size_icon"] = "small"; break; // zero not legit, but the default zombie uses it
   case 1: currMonster["size_icon"] = "small"; break;
   case 2: currMonster["size_icon"] = "tall"; break;
   case 3: currMonster["size_icon"] = "wide"; break;
   case 4: currMonster["size_icon"] = "big"; break;
   case 5: currMonster["size_icone"] = "huge"; break; 
   default: currMonster["size_icon"] = UnknownString; break;
   }
   
   currMonster["save_vs_paralysis_poison_deathmagic"] = IntToString(monster.svPaPoDm);
   currMonster["save_vs_petrification_polymorph"] = IntToString(monster.svPePo);
   currMonster["save_vs_rod_staff_wand"] = IntToString(monster.svRoStWa);
   currMonster["save_vs_breathweapon"] = IntToString(monster.svBw);
   currMonster["save_vs_spell"] = IntToString(monster.svSp);   

   currMonster["movement_base"]  = IntToString(monster.move);
   currMonster["movement_adjusted"]  = IntToString(monster.adjMove);

   currMonster["level_current"]  = IntToString(monster.level);
   currMonster["level_former_class"]  = IntToString(monster.levelFormerClass);

   if (monster.rdyToTrain)
      currMonster["level_ready_to_train"] = "1";
   else
      currMonster["level_ready_to_train"] = "0";

   currMonster["level_fighter_sweep"] = IntToString(monster.fighterSweepLevel);

   currMonster["level_predrain_cleric"] = IntToString(monster.clericLvlPreDrain);
   currMonster["level_predrain_knight"] = IntToString(monster.knightLvlPreDrain);
   currMonster["level_predrain_fighter"] = IntToString(monster.fighterLvlPreDrain);
   currMonster["level_predrain_paladin"] = IntToString(monster.paladinLvlPreDrain);
   currMonster["level_predrain_ranger"] = IntToString(monster.rangerLvlPreDrain);
   currMonster["level_predrain_mage"] = IntToString(monster.mageLvlPreDrain);
   currMonster["level_predrain_thief"] = IntToString(monster.thiefLvlPreDrain);

   currMonster["level_current_cleric"] = IntToString(monster.clericLvl);
   currMonster["level_current_knight"] = IntToString(monster.knightLvl);
   currMonster["level_current_fighter"] = IntToString(monster.fighterLvl);
   currMonster["level_current_paladin"] = IntToString(monster.paladinLvl);
   currMonster["level_current_ranger"] = IntToString(monster.rangerLvl);
   currMonster["level_current_mage"] = IntToString(monster.mageLvl);
   currMonster["level_current_thief"] = IntToString(monster.thiefLvl);

   currMonster["level_preclasschange_cleric"] = IntToString(monster.clericLvlPreClassChng);
   currMonster["level_preclasschange_knight"] = IntToString(monster.knightLvlPreClassChng);
   currMonster["level_preclasschange_fighter"] = IntToString(monster.fighterLvlPreClassChng);
   currMonster["level_preclasschange_paladin"] = IntToString(monster.paladinLvlPreClassChng);
   currMonster["level_preclasschange_ranger"] = IntToString(monster.rangerLvlPreClassChng);
   currMonster["level_preclasschange_mage"] = IntToString(monster.mageLvlPreClassChng);
   currMonster["level_preclasschange_thief"] = IntToString(monster.thiefLvlPreClassChng);

   // Thieving skills are adjusted for level
   // and race but not for dexterity or armor.
   currMonster["thieving_skill_pickpocket"] = IntToString(monster.pickPockets);
   currMonster["thieving_skill_openlock"] = IntToString(monster.openLocks);
   currMonster["thieving_skill_find_remove_trap"] = IntToString(monster.findRemTraps);
   currMonster["thieving_skill_move_silent"] = IntToString(monster.moveSilent);
   currMonster["thieving_skill_hide_in_shadows"] = IntToString(monster.hideShadows);
   currMonster["thieving_skill_hear_noise"] = IntToString(monster.hearNoise);
   currMonster["thieving_skill_climb_walls"] = IntToString(monster.climbWalls);
   currMonster["thieving_skill_read_languages"] = IntToString(monster.readLang);

   int tmpMorale = monster.morale;
   if (tmpMorale != 0)
      currMonster["morale_base"] = IntToString(tmpMorale - 128);
   else
      currMonster["morale_base"] = IntToString(tmpMorale);

   currMonster["attack_1_unarmed_nbr_attacks_per_2_rounds"] = IntToString(monster.attacksPer2Rnds);
   currMonster["attack_1_unarmed_num_damage_dice"] = IntToString(monster.dmgDiceNbr);
   currMonster["attack_1_unarmed_damage_dice_sides"] = IntToString(monster.dmgDiceSides);
   currMonster["attack_1_unarmed_damage_bonus"] = IntToString(monster.dmgBonus);

   currMonster["attack_2_unarmed_nbr_attacks_per_2_rounds"] = IntToString(monster.attacksPer2Rnds2nd);
   currMonster["attack_2_unarmed_num_damage_dice"] = IntToString(monster.dmgDiceNbr2nd);
   currMonster["attack_2_unarmed_damage_dice_sides"] = IntToString(monster.dmgDiceSides2nd);
   currMonster["attack_2_unarmed_damage_bonus"] = IntToString(monster.dmgBonus2nd);

   currMonster["attack_1_nbr_attacks_per_2_rounds"] = IntToString(monster.attacksPer2RndsEquipped);
   currMonster["attack_1_num_damage_dice"] = IntToString(monster.dmgDiceNbrEquipped);
   currMonster["attack_1_damage_dice_sides"] = IntToString(monster.dmgDiceSidesEquipped);
   currMonster["attack_1_damage_bonus"] = IntToString(monster.dmgBonusEquipped);

   currMonster["attack_2_nbr_attacks_per_2_rounds"] = IntToString(monster.attacksPer2Rnds2ndEquipped);
   currMonster["attack_2_num_damage_dice"] = IntToString(monster.dmgDiceNbr2ndEquipped);
   currMonster["attack_2_damage_dice_sides"] = IntToString(monster.dmgDiceSides2ndEquipped);
   currMonster["attack_2_damage_bonus"] = IntToString(monster.dmgBonus2ndEquipped);

   if (monster.ac > 0)
      currMonster["armor_class_base"] = IntToString(60 - monster.ac);
   else
      currMonster["armor_class_base"] = IntToString(monster.ac); // not sure that this case can happen

   if (monster.adjAC > 0)
      currMonster["armor_class_adjusted"] = IntToString(60 - monster.adjAC);
   else
      currMonster["armor_class_adjusted"] = IntToString(monster.adjAC); 

   if (monster.itemMask & 0x01)
      currMonster["item_mask_can_equip_magic_user"] = "1";
   else
      currMonster["item_mask_can_equip_magic_user"] = "0";

   if (monster.itemMask & 0x02)
      currMonster["item_mask_can_equip_cleric"] = "1";
   else
      currMonster["item_mask_can_equip_cleric"] = "0";

   if (monster.itemMask & 0x04)
      currMonster["item_mask_can_equip_thief"] = "1";
   else
      currMonster["item_mask_can_equip_thief"] = "0";

   if (monster.itemMask & 0x08)
      currMonster["item_mask_can_equip_fighter"] = "1";
   else
      currMonster["item_mask_can_equip_fighter"] = "0";

   if (monster.itemMask & 0x10)
      currMonster["item_mask_can_equip_knight"] = "1";
   else
      currMonster["item_mask_can_equip_knight"] = "0";

   if (monster.itemMask & 0x40)
      currMonster["item_mask_can_equip_paladin_ranger"] = "1";
   else
      currMonster["item_mask_can_equip_paladin_ranger"] = "0";


   // RBT: do we have a table of equivalent strings?
   currMonster["icon_index"] = IntToString(monster.iconId);

   currMonster["party_index"] = IntToString(monster.uniquePartyID);

   if (monster.specAbFlags & 0x01)
      currMonster["special_flag_affected_by_dispel_evil"] = "1";
   else
      currMonster["special_flag_affected_by_dispel_evil"] = "0";

   if (monster.specAbFlags & 0x02)
      currMonster["special_flag_is_a_mammal"] = "1";
   else
      currMonster["special_flag_is_a_mammal"] = "0";

   if (monster.specAbFlags & 0x04)
      currMonster["special_flag_penalty_to_hit_dwarf"] = "1";
   else
      currMonster["special_flag_penalty_to_hit_dwarf"] = "0";

   if (monster.specAbFlags & 0x08)
      currMonster["special_flag_extra_dmg_from_ranger"] = "1";
   else
      currMonster["special_flag_extra_dmg_from_ranger"] = "0";

   if (monster.specAbFlags & 0x10)
      currMonster["special_flag_is_a_snake"] = "1";
   else
      currMonster["special_flag_is_a_snake"] = "0";

   if (monster.specAbFlags & 0x20)
      currMonster["special_flag_penalty_to_hit_gnome"] = "1";
   else
      currMonster["special_flag_penalty_to_hit_gnome"] = "0";

   if (monster.specAbFlags & 0x40)
      currMonster["special_flag_is_a_animal"] = "1";
   else
      currMonster["special_flag_is_a_animal"] = "0";

   if (monster.specAbFlags & 0x80)
      currMonster["special_flag_dwarf_has_hit_bonus"] = "1";
   else
      currMonster["special_flag_dwarf_has_hit_bonus"] = "0";


   if (monster.specAbFlags2 & 0x01)
      currMonster["special_flag_is_a_true_giant"] = "1";
   else
      currMonster["special_flag_is_a_true_giant"] = "0";

   if (monster.specAbFlags2 & 0x02)
      currMonster["special_flag_can_be_held_or_charmed"] = "1";
   else
      currMonster["special_flag_can_be_held_or_charmed"] = "0";

   if (monster.specAbFlags2 & 0x04)
      currMonster["special_flag_gnome_has_hit_bonus"] = "1";
   else
      currMonster["special_flag_gnome_has_hit_bonus"] = "0";

   if (monster.specAbFlags2 & 0x08)
      currMonster["special_flag_immune_to_death_magic"] = "1";
   else
      currMonster["special_flag_immune_to_death_magic"] = "0";

   if (monster.specAbFlags2 & 0x10)
      currMonster["special_flag_immune_to_poison"] = "1";
   else
      currMonster["special_flag_immune_to_poison"] = "0";

   if (monster.specAbFlags2 & 0x20)
      currMonster["special_flag_immune_to_decapitation_from_vorpal_sword"] = "1";
   else
      currMonster["special_flag_immune_to_decapitation_from_vorpal_sword"] = "0";

   if (monster.specAbFlags2 & 0x40)
      currMonster["special_flag_immune_to_confusion"] = "1";
   else
      currMonster["special_flag_immune_to_confusion"] = "0";

   currMonster["item_bundles_carried"] = IntToString(monster.itemBundlesCarried);
   currMonster["equip_num_hands_full"] = IntToString(monster.nbrHandsFull);

   currMonster["magic_resist_base_percent"] = IntToString(monster.magicResistance);
   currMonster["magic_resist_bonus"] = IntToString(monster.magicProtBonus);

   currMonster["spells_num_cleric_level_1"] = IntToString(monster.numClericSpellsByLvl[0]);
   currMonster["spells_num_cleric_level_2"] = IntToString(monster.numClericSpellsByLvl[1]);
   currMonster["spells_num_cleric_level_3"] = IntToString(monster.numClericSpellsByLvl[2]);
   currMonster["spells_num_cleric_level_4"] = IntToString(monster.numClericSpellsByLvl[3]);
   currMonster["spells_num_cleric_level_5"] = IntToString(monster.numClericSpellsByLvl[4]);
   currMonster["spells_num_cleric_level_6"] = IntToString(monster.numClericSpellsByLvl[5]);
   currMonster["spells_num_cleric_level_7"] = IntToString(monster.numClericSpellsByLvl[6]);

   currMonster["spells_num_druid_level_1"] = IntToString(monster.numDruidSpellsByLvl[0]);
   currMonster["spells_num_druid_level_2"] = IntToString(monster.numDruidSpellsByLvl[1]);
   currMonster["spells_num_druid_level_3"] = IntToString(monster.numDruidSpellsByLvl[2]);

   currMonster["spells_num_magicuser_level_1"] = IntToString(monster.numMageSpellsByLvl[0]);
   currMonster["spells_num_magicuser_level_2"] = IntToString(monster.numMageSpellsByLvl[1]);
   currMonster["spells_num_magicuser_level_3"] = IntToString(monster.numMageSpellsByLvl[2]);
   currMonster["spells_num_magicuser_level_4"] = IntToString(monster.numMageSpellsByLvl[3]);
   currMonster["spells_num_magicuser_level_5"] = IntToString(monster.numMageSpellsByLvl[4]);
   currMonster["spells_num_magicuser_level_6"] = IntToString(monster.numMageSpellsByLvl[5]);
   currMonster["spells_num_magicuser_level_7"] = IntToString(monster.numMageSpellsByLvl[6]);
   currMonster["spells_num_magicuser_level_8"] = IntToString(monster.numMageSpellsByLvl[7]);
   currMonster["spells_num_magicuser_level_9"] = IntToString(monster.numMageSpellsByLvl[8]);

   //198-338 Memorized Spells (one byte per spell; will decode later)
   //BYTE spellsMemorized[141];
   int spellMemorizedCount = 0;
   for (int i=0;i<141;i++)
   {
      if (monster.spellsMemorized[i] > 0)
      {
         spellMemorizedCount++;
         CString strLabel;
         strLabel.Format("spells_memorized_%03u", spellMemorizedCount);

         CString spellName = FRUAGetSpellText( monster.spellsMemorized[i] );
         //if (spellName == "none")
         //   spellName.Format("unknown spell code %u", monster.spellsMemorized[i]);

         currMonster[strLabel] = CStringOrEmptyString(spellName);
      }
   }

   int itemsCarried = 0;
   for (int i=0;i<16;i++)
   {
      if (monster.itemCarried[i] > 0)
      {
         itemsCarried++;
         CString strLabel;
         strLabel.Format("equip_item_%02u", itemsCarried);

         CString itemName = LookupItemNameByItemCode( monster.itemCarried[i] );
         unsigned int itemIdx = LookupItemIndexByName(itemName);

         currMonster[strLabel] = CStringOrEmptyString(itemName);

         strLabel.Format("equip_item_%02u_count", itemsCarried);
         currMonster[strLabel] = IntToString(monster.itemQty[i]);

         strLabel.Format("equip_item_%02u_id", itemsCarried);
         currMonster[strLabel] = IntToString(itemIdx);
      }
   }

   int specAbCount = 0;
   for (int i=0;i<20;i++)
   {
      if (monster.specAbCodes[i] > 0)
      {
         specAbCount++;
         CString strLabel;
         strLabel.Format("special_ability_%02u", specAbCount);

         CString specAbName = FRUAGetSpecAbText( monster.specAbCodes[i] );
         //if (specAbName == "none")
         //   specAbName.Format("unknown specAb code %u", monster.specAbCodes[i]);

         currMonster[strLabel] = CStringOrEmptyString(specAbName);
      }
   }

   //currMonster[""] = IntToString();

   /*  
   //339-354 Spell Book (one bit per spell; will decode later.  Spells
   //   are designated by integer value from 1 to 126 if memorized,
   //   or 129 to 254 if not memorized.  For Cleric, Paladin, or Ranger,
   //   spell book is automatically updated to include all due spells.)
   BYTE spellBook[16];

   //397     Monster slot (Set to zero for player character)
   BYTE monsterIndex; // 0 for player
   */

   Json::ValueIterator itCurr = currMonster.begin();
   while (itCurr != currMonster.end())
   {
      TRACE("MONSTER: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

CString LookupMonsterNameByMonsterCode(int code)
{
   ASSERT( code >= 0 );
   ASSERT( code < MAX_FRUA_MONSTERS );

   return (MonstersAndNPCs[code].name);
}

bool ReadFRUAMonsterFile(const char *rootFolder)
{
   CFileFind finder;
   CString searchPath = rootFolder;
   searchPath += "\\Monst*.dat";

   bool atLeastOneFileRead = false;

   BOOL found = finder.FindFile(searchPath);

   while (found)
   {
      found = finder.FindNextFile();

      if (finder.IsDots())
         continue;
      if (finder.IsDirectory())
         continue;

      // can be many of these files, keep searching and read them all
      // but return whether we found at least one matching file
      atLeastOneFileRead = true;
      ImportFRUAMonsterDat(finder.GetFilePath());
   }

   return atLeastOneFileRead;
}

bool ReadMonsterDefaultsFile(const char *rootFolder)
{
   CFileFind finder;
   CString searchPath = rootFolder;
   searchPath += "\\default_monsters.dat";

   bool fileFound = false;

   BOOL found = finder.FindFile(searchPath);

   while ((found) && (!fileFound))
   {
      found = finder.FindNextFile();

      if (finder.IsDots())
         continue;
      if (finder.IsDirectory())
         continue;

      FILE *pFile = fopen(finder.GetFilePath(), "rb");
      if (pFile != NULL)
      {
         if (fread(MonstersAndNPCs, sizeof(MonstersAndNPCs), 1, pFile) == 1)
            fileFound = true;
         fclose(pFile);
      }
      else
      {
         printf("failed to open monster defaults file\n");
      }
   }

   return fileFound;
}


void ReadMonstersDat(Json::Value &root)
{
   printf("Exporting monsters\n");   

   // The FRUA design can have one or more
   // individual monster files present.
   //
   // We will first load a file that defines ALL monster defaults
   // and then look for individual monster files which will
   // write over the default settings for only that monster.

   bool fruaFileFound = false;

   // this file should always exist somewhere nearby
   fruaFileFound = ReadMonsterDefaultsFile("."); // local folder

   if (!fruaFileFound)
   {
      if (strlen(DesignFolder) > 0)
      {
         if (!ReadMonsterDefaultsFile(DesignFolder)) // design folder
            printf("Did not find or read the default_monsters.dat file!\n");
      }
      else
         printf("Did not find or read the default_monsters.dat file!\n");
   }

   // these monst*.dat files might exist, and will over ride the defaults
   fruaFileFound = false;
   if (strlen(DesignFolder) > 0)
      fruaFileFound = ReadFRUAMonsterFile(DesignFolder); // user folder

   // if file not read from user design folder, try the local folder
   if (!fruaFileFound)
   {
      if (!ReadFRUAMonsterFile(".")) // local folder
         printf("Did not find any FRUA monster files (monst*.dat), using defaults\n");
   }

   int monsterCount = 0;
   Json::Value monsterArray(Json::arrayValue);

   for (int i=1; i<MAX_FRUA_MONSTERS; i++)
   {
      Json::Value monster;
      monster["id"] = IntToString(i);

      TRACE("MONSTER %u BEGIN\n", i);

      FormatMonsterJson(MonstersAndNPCs[i], monster);

      TRACE("MONSTER %u END\n", i);

      monsterArray.append(monster);
      monsterCount++;
   }

   root["monsters"] = monsterArray;
   root["monsters_count"] = IntToString(monsterCount);
}

void WriteMonsterDefaultsFile()
{
   printf("Write monster defaults file\n");

   FILE *pFile = fopen("default_monsters.dat", "w+b");
   if (pFile != NULL)
   {
      fwrite( MonstersAndNPCs, sizeof(MonstersAndNPCs), 1, pFile);
      fclose(pFile);
   }
}
