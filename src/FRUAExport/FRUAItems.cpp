/******************************************************************************
* Filename: FRUAItems.cpp
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
#include "FRUAItems.h"
#include "FRUAStructs.h"
#include "Utilities.h"
#include "FRUAStringTables.h"

struct SItemNameToIndexType
{
  unsigned int index;
  CString name;
};

static SItemNameToIndexType ItemNameToIndexList[MAX_FRUA_ITEMS];

unsigned int LookupItemIndexByName(CString name)
{
  if (name.GetLength() == 0)
    return 0;

  for (int i=0;i<MAX_FRUA_ITEMS;i++)
  {
    if (name.CompareNoCase(ItemNameToIndexList[i].name) == 0)
      return ItemNameToIndexList[i].index;

  }
  return 0;
}

unsigned int LookupItemBundleQuantityByItemCode(int code)
{
   if (code <= 0)
      return 0;
   if (code > MAX_FRUA_ITEMS)
      return 0;

   // there are 255 default items 0..254
   // the FRUA files reference items 1..255
   // so we subtract one for proper indexing here
   code--;

   unsigned int count = Items[code].numitems;

   // RBT: FRUA uses a zero when the item has no bundly qty
   //      It looks better in the export file to have a quantity 1
   //      sword in a given record than quantity zero so I 
   //      interfere with the given value.
   if (count <= 0)
      count = 1;

   return (count);
}

CString LookupItemNameByItemCode(int code)
{
   // item code zero is 'no item'
   if (code <= 0)
      return NoneString;

   // Not certain, but I think item 255 is special
   // and should indicate a mage scroll with randomized
   // spells on it (according to hack docs). However there
   // are only 254 items defined in the FRUA files so 
   // that's what we are going with for now.
   if (code > MAX_FRUA_ITEMS)
      return UnknownString;

   // there are 254 default items 0..253
   // the FRUA files reference items 1..254
   // so we subtract one for proper indexing here
   code--;   
   
   CString name1 = FRUAGetItemNameText(Items[code].name1);
   CString name2 = FRUAGetItemNameText(Items[code].name2);
   CString name3 = FRUAGetItemNameText(Items[code].name3);

   CString tempIdName="";

   // known and unknown names
   if (Items[code].name1 > 0)
   {
      tempIdName += name1;
      tempIdName += " ";
   }

   if (Items[code].name2 > 0)
   {
      if (Items[code].name3 == 77) // bundle of
      {
         CString qty;
         qty.Format("%u ", Items[code].name2);
         tempIdName += qty;
      }
      else
      {
         tempIdName += name2;
         tempIdName += " ";
      }
   }

   if (Items[code].name3 > 0)
      tempIdName += name3;      

   tempIdName.Trim();
   return tempIdName;   
}

void FormatItemJson(ImportUAItem Item, Json::Value &currItem)
{
   CString tempName;
   CString tempIdName;   

   ASSERT( (Item.ptr >= 0) && (Item.ptr < MAX_FRUA_ITEM_CLASSES) );
   ImportUAItems Class = ItemClasses[Item.ptr];

   tempIdName="";
   tempName="";

   CString name1 = FRUAGetItemNameText(Item.name1);
   CString name2 = FRUAGetItemNameText(Item.name2);
   CString name3 = FRUAGetItemNameText(Item.name3);

   //TRACE("IMPORTING ITEM: %s %s %s\n", name1, name2, name3);

   // known and unknown names
   if (Item.name1 > 0)
   {
      tempIdName += name1;
      tempIdName += " ";

      if ((Item.identified & 1) == 0)
      {
         tempName += name1;
         tempName += " ";
      }
   }

   if (Item.name2 > 0)
   {
      if (Item.name3 == 77) // bundle of
      {
         CString qty;
         qty.Format("%u ", Item.name2);
         tempIdName += qty;

         if ((Item.identified & 2) == 0)
            tempName += qty;
      }
      else
      {
         tempIdName += name2;
         tempIdName += " ";

         if ((Item.identified & 2) == 0)
         {
            tempName += name2;
            tempName += " ";
         }
      }
   }

   if (Item.name3 > 0)
   {
      tempIdName += name3;      

      if ((Item.identified & 4) == 0)
         tempName += name3;
   }

   tempName.Trim();
   tempIdName.Trim();

   currItem["name"] = CStringOrEmptyString(tempName);
   currItem["identified_name"] = CStringOrEmptyString(tempIdName);

   switch (Class.slot)
   {
   case 0: currItem["ready_slot"] = "weapon_hand"; break;
   case 1: currItem["ready_slot"] = "shield_hand"; break;
   case 2: currItem["ready_slot"] = "body_armor"; break;
   case 3: currItem["ready_slot"] = "hands"; break;
   case 4: currItem["ready_slot"] = "head"; break;
   case 5: currItem["ready_slot"] = "waist"; break;
   case 6: currItem["ready_slot"] = "body_robe"; break;
   case 7: currItem["ready_slot"] = "back"; break;
   case 8: currItem["ready_slot"] = "feet"; break;
   case 9: currItem["ready_slot"] = "fingers"; break;
   case 10: currItem["ready_slot"] = "ammo_quiver"; break;
   default: currItem["ready_slot"] = "none"; break;
   }

   // ammo uses ROF and range of firing weapon
   if (Class.weapon_type == 18) // ammo
   {
      currItem["rate"] = DoubleToString(0.0);
   }
   else
   {
      if (Class.rate <= 2)
         currItem["rate"] = DoubleToString(Class.rate);
      else
         currItem["rate"] = DoubleToString(((double)Class.rate)/2.0);
   }

   if (Class.cutting_or_blunt != 0)
   {
      if (Class.weapon_type == 138)
         currItem["weapon_type"] = "crossbow"; //idata.Wpn_Type = Crossbow;
      else if (Class.weapon_type == 26)
         currItem["weapon_type"] = "thrown"; //idata.Wpn_Type = Throw;
      else if (Class.weapon_type == 20)
         currItem["weapon_type"] = "hand_thrown"; //idata.Wpn_Type = HandThrow;
      else if (Class.weapon_type == 18)
         currItem["weapon_type"] = "ammo"; //idata.Wpn_Type = Ammo;
      else if (Class.weapon_type == 15)
         currItem["weapon_type"] = "fine_bow"; //idata.Wpn_Type = Bow;
      else if (Class.weapon_type == 11)
         currItem["weapon_type"] = "bow"; //idata.Wpn_Type = Bow;
      else if (Class.weapon_type == 10)
         currItem["weapon_type"] = "sling"; //idata.Wpn_Type = SlingNoAmmo;
      else if (Class.weapon_type == 4)
      {
         if (Class.cutting_or_blunt & 1)
            currItem["weapon_type"] = "hand_cutting";  //idata.Wpn_Type = HandCutting;
         else if (Class.cutting_or_blunt & 128)
            currItem["weapon_type"] = "hand_blunt"; //idata.Wpn_Type = HandBlunt;
         else
            currItem["weapon_type"] = "none"; //idata.Wpn_Type = NotWeapon; // shouldn't hit this!
      }
      else
         currItem["weapon_type"] = "none"; //idata.Wpn_Type = NotWeapon;
   }
   else
      currItem["weapon_type"] = "none"; //idata.Wpn_Type = NotWeapon;

   currItem["hands_needed"] = IntToString(Class.twohanded);

   int prot;
   if (Class.protection > 170)
   {
      prot = 188 - Class.protection;
      if (prot > 0)
         currItem["protection_base"] = IntToString(-(10 - prot)); //idata.Protection_Base = -(10 - prot);
      else
         currItem["protection_base"] = IntToString(0); //idata.Protection_Base = 0;
   }
   else if (Class.protection > 117) // < 128 would be shield -2 for example
   {
      prot = Class.protection - 128;
      currItem["protection_base"] = IntToString(-(prot)); // idata.Protection_Base = -(prot);
   }
   else
      currItem["protection_base"] = IntToString(0); //idata.Protection_Base = 0;

   currItem["range"] = IntToString(Class.range); //idata.Range = Class.range;

   if (Class.classes & 1)
      currItem["useable_magic_user"] = "1";
   else
      currItem["useable_magic_user"] = "0";  

   if (Class.classes & 2)
      currItem["useable_cleric"] = "1";
   else
      currItem["useable_cleric"] = "0";  
      
   if (Class.classes & 4)
      currItem["useable_thief"] = "1";
   else
      currItem["useable_thief"] = "0";  

   if (Class.classes & 8)
      currItem["useable_fighter"] = "1";
   else
      currItem["useable_fighter"] = "0";  

   if (Class.classes & 16)
      currItem["useable_knight"] = "1";
   else
      currItem["useable_knight"] = "0";  

   //if (Class.classes & 32)
   //   currItem["useable_class_bit32"] = "1";
   //else
   //   currItem["useable_class_bit32"] = "0";  

   if (Class.classes & 64)
      currItem["useable_paladin_ranger"] = "1";
   else
      currItem["useable_paladin_ranger"] = "0";  

   currItem["encumbrance_coins"] = IntToString(Item.encumbrance); //idata.Encumbrance = Item.encumbrance;
   currItem["price_platinum"] = IntToString(Item.price); //idata.Cost = Item.price;

   int magicAttackBonus;
   if (Item.magic_bonus > 127) 
      magicAttackBonus = (-(256-Item.magic_bonus));
   else
      magicAttackBonus = (Item.magic_bonus);

   currItem["vs_small_numDice"] = IntToString(Class.vs_small.dice);
   currItem["vs_small_numSides"] = IntToString(Class.vs_small.sides);

   if (Class.cutting_or_blunt == 0)
      currItem["vs_small_bonus"] = IntToString(magicAttackBonus);
   else
      currItem["vs_small_bonus"] = IntToString(Class.vs_small.bonus);
      
   currItem["vs_large_numDice"] = IntToString(Class.vs_large.dice);
   currItem["vs_large_numSides"] = IntToString(Class.vs_large.sides);

   // cutting_or_blunt: 0 not wpn, 1 = cut wpn, 128 = blunt wpn
   if (Class.cutting_or_blunt == 0)
      currItem["vs_large_bonus"] = IntToString(magicAttackBonus);
   else      
      currItem["vs_large_bonus"] = IntToString(Class.vs_large.bonus);
      
   if (Class.cutting_or_blunt == 0)
      currItem["protection_bonus"] = IntToString(magicAttackBonus);//-(Item.magic_bonus);
   else
      currItem["protection_bonus"] = IntToString(0);

   currItem["cursed"] = IntToString(Item.cursed); //idata.Cursed = Item.cursed;

   int bundleQty = Item.numitems;      

   currItem["bundle_quantity"] = IntToString(bundleQty);     

   // Byte 16: Special Code
   //    0 = Byte 15 is spell code
   //    1-127 = Item is Scroll (bytes 14-16 give codes for spells on scroll)
   //    128 = Byte 15 is special ability code
   //    129 = Item is Ring of Wizardry  (byte 15 = 0) 
   //    131 = Item is Gauntlets of Ogre Power (byte 15 = 0) 
   //    133 = Item is Girdle of Giant Strength (byte 15 = Str - 18) 

   if (Item.special_code == 0)      
   {
      currItem["num_charges"] = IntToString(Item.numcharges); //idata.Num_Charges = Item.numcharges;
      currItem["spell_number_1"] = FRUAGetSpellText(Item.magicalcode);
      currItem["spell_number_2"] = FRUAGetSpellText(0);
      currItem["spell_number_3"] = FRUAGetSpellText(0);
      currItem["special_ability"] = "none";
   }
   else if ((Item.special_code >= 1) && (Item.special_code <= 127))
   {
      currItem["num_charges"] = IntToString(0);
      currItem["spell_number_1"] = FRUAGetSpellText(Item.numcharges);
      currItem["spell_number_2"] = FRUAGetSpellText(Item.magicalcode);
      currItem["spell_number_3"] = FRUAGetSpellText(Item.special_code);
      currItem["special_ability"] = "none";
   }
   else if (Item.special_code == 128)
   {
      currItem["num_charges"] = IntToString(Item.numcharges); //idata.Num_Charges = Item.numcharges;
      currItem["spell_number_1"] = FRUAGetSpellText(0);
      currItem["spell_number_2"] = FRUAGetSpellText(0);
      currItem["spell_number_3"] = FRUAGetSpellText(0);
      currItem["special_ability"] = FRUAGetSpecAbText(Item.magicalcode);
   }
   else if (Item.special_code == 129)
   {
      currItem["num_charges"] = IntToString(Item.numcharges); //idata.Num_Charges = Item.numcharges;
      currItem["spell_number_1"] = FRUAGetSpellText(0);
      currItem["spell_number_2"] = FRUAGetSpellText(0);
      currItem["spell_number_3"] = FRUAGetSpellText(0);
      currItem["special_ability"] = "Ring of Wizardry";
   }
   else if (Item.special_code == 131)
   {
      currItem["num_charges"] = IntToString(Item.numcharges); //idata.Num_Charges = Item.numcharges;
      currItem["spell_number_1"] = FRUAGetSpellText(0);
      currItem["spell_number_2"] = FRUAGetSpellText(0);
      currItem["spell_number_3"] = FRUAGetSpellText(0);
      currItem["special_ability"] = "Gauntlets of Ogre Power";
   }
   else if (Item.special_code == 133)
   {
      currItem["num_charges"] = IntToString(Item.numcharges); //idata.Num_Charges = Item.numcharges;
      currItem["spell_number_1"] = FRUAGetSpellText(0);
      currItem["spell_number_2"] = FRUAGetSpellText(0);
      currItem["spell_number_3"] = FRUAGetSpellText(0);

      unsigned char strengthValue = Item.magicalcode + 18;
      CString strTemp;
      strTemp.Format("Girdle of Giant Strength %u", strengthValue);         
      currItem["special_ability"] = CStringOrEmptyString(strTemp);
   }      

   Json::ValueIterator itCurr = currItem.begin();
   while (itCurr != currItem.end())
   {
      TRACE("ITEM: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

bool ReadFRUAItemFile(const char *rootFolder)
{
   try
   {
      FILE *fp;
      CString fullPath;
      BOOL success = TRUE;   

      // read generic item class information
      fullPath = rootFolder;
      fullPath += "\\items.dat";

      fp = fopen(fullPath, "rb");
      if (fp == NULL)
      {
         return false;
      }
      else
      {
         int ClassCount=0;
         success = TRUE;
         while ((ClassCount<MAX_FRUA_ITEM_CLASSES) && (success))//(!feof(fp)))
         {
            success = (fread(&ItemClasses[ClassCount], sizeof(ImportUAItems), 1, fp) == 1);
            if (success)
               ClassCount++;
         }
         fclose(fp);
      }

      // read specific items
      fullPath = rootFolder;
      fullPath += "\\item.dat";

      fp = fopen(fullPath, "rb");
      if (fp == NULL)
      {
         return false;
      }
      else
      {
         success=TRUE;
         int ItemCount=0;
         while ((ItemCount<MAX_FRUA_ITEMS) && (success))//(!feof(fp)))
         {
            success = (fread(&Items[ItemCount], sizeof(ImportUAItem), 1, fp) == 1);
            if (success)
               ItemCount++;
         }
         fclose(fp);
      }

      return true;
   }
   catch (...)
   {
      printf("Exception encountered while reading FRUA item files\n");
      return false;
   }
}

void ReadItemsDat(Json::Value &root)
{
   printf("Exporting items\n");   
   
   // The FRUA items are defined in two files
   //    items.dat
   //    item.dat
   //
   // If present, they will define all 255 items
   //
   // We will read the item files provided by the design
   // and if that fails then we will read the default
   // files provided with this program.

   // this file might exist
   CString path = FRUAFolder;
   path += "\\DISK1";

   bool fruaFileFound = ReadFRUAItemFile(path); // DISK1 folder

   if (!fruaFileFound)
   {
      if (strlen(DesignFolder) > 0)
         fruaFileFound = ReadFRUAItemFile(DesignFolder); // user folder
   }

   // if file not read from user design folder, try the local folder
   if (!fruaFileFound)
   {
      if (!ReadFRUAItemFile(".")) // local folder
         printf("Did not find any FRUA item files, using defaults\n");
   }

   if (!fruaFileFound)
   {
      // this file should always be present
      ReadItemDefaultsFile();   
   }
             
   int itemCount = 0;
   Json::Value itemArray(Json::arrayValue);
   CString strIdKey;

   for (int i=0; i<MAX_FRUA_ITEMS; i++)
   {
      Json::Value item;
      item["id"] = IntToString(i+1);

      // format item data as json
      FormatItemJson(Items[i], item);

      ItemNameToIndexList[i].index = i+1;
      ItemNameToIndexList[i].name = item.get("identified_name", "").asString().c_str();
         
      itemCount++;
      itemArray.append(item);
   }

   root["items"] = itemArray;
   root["items_count"] = IntToString(itemCount);
}

void WriteItemDefaultsFile()
{
   printf("Write item defaults file\n");

   BOOL success;
   FILE *pItemClassFile = fopen(".\\default_items.dat", "w+b");
   if (pItemClassFile != NULL)
   {
      //fwrite( &ItemClasses, sizeof(ItemClasses), 1, pFile);
      int ClassCount=0;
      success = TRUE;
      while ((ClassCount<MAX_FRUA_ITEM_CLASSES) && (success))//(!feof(fp)))
      {
         success = (fwrite(&ItemClasses[ClassCount], sizeof(ImportUAItems), 1, pItemClassFile) == 1);
         if (success)
            ClassCount++;
      }
      fclose(pItemClassFile);
   }
   else
   {
      printf("Could not open default item classes file [default_items.dat]\n");
   }

   FILE *pItemsFile = fopen(".\\default_item.dat", "w+b");
   if (pItemsFile != NULL)
   {
      //fwrite( &Items, sizeof(Items), 1, pFile);
      success=TRUE;
      int ItemCount=0;
      while ((ItemCount<MAX_FRUA_ITEMS) && (success))//(!feof(fp)))
      {
         success = (fwrite(&Items[ItemCount], sizeof(ImportUAItem), 1, pItemsFile) == 1);
         if (success)
            ItemCount++;
      }
      //printf("%u ITEMS WRITTEN\n", ItemCount);
      fclose(pItemsFile);
   }
   else
   {
      printf("Could not open default item classes file [default_item.dat]\n");
   }
}

void ReadItemDefaultsFile()
{
   BOOL success = TRUE;
   FILE *pItemClassFile = fopen(".\\default_items.dat", "rb");
   if (pItemClassFile != NULL)
   {
      //fread( &ItemClasses, sizeof(ItemClasses), 1, pFile);
      int ClassCount=0;
      success = TRUE;
      while ((ClassCount<MAX_FRUA_ITEM_CLASSES) && (success))//(!feof(fp)))
      {
         success = (fread(&ItemClasses[ClassCount], sizeof(ImportUAItems), 1, pItemClassFile) == 1);
         if (success)
            ClassCount++;
      }
      fclose(pItemClassFile);
   }
   else
   {
      printf("Could not open default item classes file [default_items.dat]\n");
   }

   FILE *pItemsFile = fopen(".\\default_item.dat", "rb");
   if (pItemsFile != NULL)
   {
      //fread( &Items, sizeof(Items), 1, pFile);
      success=TRUE;
      int ItemCount=0;
      while ((ItemCount<MAX_FRUA_ITEMS) && (success))//(!feof(fp)))
      {
         success = (fread(&Items[ItemCount], sizeof(ImportUAItem), 1, pItemsFile) == 1);
         if (success)
            ItemCount++;
      }
      fclose(pItemsFile);
   }
   else
   {
      printf("Could not open default item classes file [default_item.dat]\n");
   }
}
