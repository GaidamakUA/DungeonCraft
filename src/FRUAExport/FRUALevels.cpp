/******************************************************************************
* Filename: FRUALevels.cpp
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
#include "FRUALevels.h"
#include "FRUAStructs.h"
#include "FRUAItems.h"
#include "Utilities.h"
#include "FRUAStringTables.h"
#include "FRUAMonster.h"

const char *GetWallSlotText(const UALevelData &level, int wallCode)
{
   static char strWall[128];

   // the level global data indicates the three wall art styles 
   // that can be used for the current level.
   //
   // each geo grid has a code for n,e,s,w that ranges 1..15, or 0 for blank wall
   //
   // 1..5 is slot 1-5 for wall style 1
   // 6..10 is slot 1-5 for wall style 2
   // 11-15 is slot 1-5 for wall style 3

   if (wallCode == 0)
      return NoneString;

   if (wallCode <= 5)
      sprintf(strWall, "%s - slot %i", FRUAGetWallSlotArtName(level.header.wall1), wallCode);
   else if (wallCode <= 10)
      sprintf(strWall, "%s - slot %i", FRUAGetWallSlotArtName(level.header.wall2), wallCode-5);
   else if (wallCode <= 15)
      sprintf(strWall, "%s - slot %i", FRUAGetWallSlotArtName(level.header.wall3), wallCode-10);
   else
      sprintf(strWall, "unknown - slot %i", wallCode);

   return strWall;
}

double ApplyCostModifier(double baseAmount, int costMod)
{
  switch (costMod)
  { 
  case 0: return 0;
  case 1: return baseAmount / 100.0;
  case 2: return baseAmount / 50.0;
  case 3: return baseAmount / 20.0;
  case 4: return baseAmount / 10.0;
  case 5: return baseAmount / 5.0;
  case 6: return baseAmount / 4.0;
  case 7: return baseAmount / 3.0;
  case 8: return baseAmount / 2.0;
  case 9: return baseAmount / 1.5;
  case 10: return baseAmount;
  case 11: return baseAmount * 1.5;
  case 12: return baseAmount * 2.0;
  case 13: return baseAmount * 3.0;
  case 14: return baseAmount * 4.0;
  case 15: return baseAmount * 5.0;
  case 16: return baseAmount * 10.0;
  case 17: return baseAmount * 20.0;
  case 18: return baseAmount * 50.0;
  case 19: return baseAmount * 100.0;
  default: return baseAmount;
  }
}

const char *GetGuidedTourStepActionText(BYTE action)
{
   const char *ActionText[] =
   {
      "Pause",
      "Left",
      "Right",
      "Forward"
   };

   if (action == 0)
      return (ActionText[0]);

   if (action & 3)
   {
      switch (action & 3)
      {
      case 1: return ActionText[1];
      case 2: return ActionText[2];
      case 3: return ActionText[3];      
      }
   }
   else if (action & 12)
   {
      switch (action & 12)
      {
      case 4: return ActionText[1];
      case 8: return ActionText[2];
      case 12: return ActionText[3];      
      }
   }
   else if (action & 48)
   {
      switch (action & 48)
      {
      case 16: return ActionText[1];
      case 32: return ActionText[2];
      case 48: return ActionText[3];      
      }
   }
   else if (action & 192)
   {
      switch (action & 192)
      {      
      case 64: return ActionText[1];
      case 128: return ActionText[2];
      case 192: return ActionText[3];      
      }
   }

   return UnknownString;
}

struct SShopItemInfo
{
   CString name;
   unsigned int quantity;
   unsigned int itemIdx;
};

typedef CList<SShopItemInfo, SShopItemInfo> ShopItemListType;

void AssignShopItems(ShopItemListType &list, BYTE mask, BYTE i1, BYTE i2, BYTE i3, BYTE i4, BYTE i5, BYTE i6, BYTE i7, BYTE i8)
{
   CString strTemp;

   if ((mask & 1) && (i1 > 0))
   {
      strTemp = LookupItemNameByItemCode(i1);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i1);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
   if ((mask & 2) && (i2 > 0))
   {
      strTemp = LookupItemNameByItemCode(i2);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i2);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
   if ((mask & 4) && (i3 > 0))
   {
      strTemp = LookupItemNameByItemCode(i3);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i3);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
   if ((mask & 8) && (i4 > 0))
   {
      strTemp = LookupItemNameByItemCode(i4);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i4);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
   if ((mask & 16) && (i5 > 0))
   {
      strTemp = LookupItemNameByItemCode(i5);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i5);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
   if ((mask & 32) && (i6 > 0))
   {
      strTemp = LookupItemNameByItemCode(i6);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i6);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
   if ((mask & 64) && (i7 > 0))
   {
      strTemp = LookupItemNameByItemCode(i7);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i7);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
   if ((mask & 128) && (i8 > 0))
   {
      strTemp = LookupItemNameByItemCode(i8);
      if (strTemp.GetLength() > 0)
      {
         SShopItemInfo info;
         info.name = strTemp;
         info.quantity = LookupItemBundleQuantityByItemCode(i8);
         info.itemIdx = LookupItemIndexByName(info.name);
         list.AddTail(info);
      }
   }
}

// RBT: copied from DC, not sure how correct it is
void AssignShopItemBytes(ShopItemListType &list, BYTE b1, BYTE b2, BYTE b3)
{
   if (b1 == 0) return;

   if ((b1 > 0) && (b1 < 31))
   {
     /*
      +0 Weapons A  (Base: Byte#1=0   Byte#2=0   Byte#3=0)
              **Byte#1**            **Byte#2**               **Byte#3**
          (+1) Morning Star    (+1)   Lt. Crossbow      (+1)   20 Arrows
          (+2) Military Pick   (+2)   Dagger            (+2)   Battle Axe
          (+4) Awl Pike        (+4)   10 Darts          (+4)   Hand Axe
          (+8) 20 Bolts        (+8)   Flail             (+8)   Cmp Short Bow
                               (+16)  Halberd           (+16)  Cmp Long Bow
                               (+32)  Hammer            (+32)  Long Bow
                               (+64)  5 Javelins        (+64)  Short Bow
                               (+128) Mace              (+128) Club
     */
     AssignShopItems(list, b1, 17,18,19,20,0,0,0,0);
     AssignShopItems(list, b2, 9,10,11,12,13,14,15,16);
     AssignShopItems(list, b3, 1,2,3,4,5,6,7,8);
   }
   else if ((b1 < 47) && (b2 >= 248))
   {
     /*
      +0 Weapons B  (Base: Byte#1=31  Byte#2=248 Byte#3=0)
              **Byte#2**            **Byte#3**
          (+1) Short Sword     (+1)   Scimitar
          (+2) 22-Hand Sword   (+2)   Sling
          (+4) Trident         (+4)   Spear
                               (+8)   Quarter Staff
                               (+16)  Staff Sling
                               (+32)  Bastard Sword 
                               (+64)  Broad Sword
                               (+128) Long Sword
      */
     AssignShopItems(list, b1, 29,30,31,0,0,0,0,0);
     AssignShopItems(list, b2, 21,22,23,24,25,26,27,28);
   }
   else if ((b1 < 48) && (b2 >= 192))
   {
     /*
      +0 Armor      (Base: Byte#1=47  Byte#2=192 Byte#3=0)
              **Byte#2**            **Byte#3**
          (+1)  Plate Mail     (+1)   Belt
          (+2)  Ring Mail      (+2)   Boots
          (+4)  Scale Mail     (+4)   Cloak
          (+8)  Shield         (+8)   Robe
          (+16) Elfin Chain    (+16)  Banded Mail
          (+32) Mirror         (+32)  Chain Mail
                               (+64)  Helm
                               (+128) Leather Armor 
      */
     AssignShopItems(list, b2, 40,41,42,43,44,49,0,0);
     AssignShopItems(list, b3, 32,33,34,35,36,37,38,39);
   }
   else if (b1 < 64)
   {
     /*
    +1 Weapons    (Base: Byte#1=48  Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +1   (+1)   Hammer +1         (+1)   10 Arrows +1
        (+2) Short Sword +1  (+2)   2 Javelins +1     (+2)   Battle Axe +1
        (+4) 2-Hand Sword +1 (+4)   Mace +1           (+4)   Cmp Short Bow +1
        (+8) Trident +1      (+8)   10 Bolts +1       (+8)   Cmp Long Bow +1
                             (+16)  Scimitar +1       (+16)  Lt. Crossbow +1
                             (+32)  Sling +1          (+32)  Dagger +1 
                             (+64)  Quarter Staff +1  (+64)  10 Darts +1
                             (+128) Staff Sling +1    (+128) Flail +1
     */
     AssignShopItems(list, b1, 66,67,68,69,0,0,0,0);
     AssignShopItems(list, b2, 58,59,60,61,62,63,64,65);
     AssignShopItems(list, b3, 50,51,52,53,54,55,56,57);
   }
   else if (b1 < 80)
   {
     /*
    +2 Weapons    (Base: Byte#1=64  Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +2   (+1)   Hammer +2         (+1)   10 Arrows +2
        (+2) Short Sword +2  (+2)   2 Javelins +2     (+2)   Battle Axe +2
        (+4) 2-Hand Sword +2 (+4)   Mace +2           (+4)   Cmp Short Bow +2
        (+8) Trident +2      (+8)   10 Bolts +2       (+8)   Cmp Long Bow +2
                             (+16)  Scimitar +2       (+16)  Lt. Crossbow +2
                             (+32)  Sling +2          (+32)  Dagger +2 
                             (+64)  Quarter Staff +2  (+64)  10 Darts +2
                             (+128) Staff Sling +2    (+128) Flail +2
      */
     AssignShopItems(list, b1, 96,97,98,99,0,0,0,0);
     AssignShopItems(list, b2, 88,89,90,91,92,93,94,95);
     AssignShopItems(list, b3, 80,81,82,83,84,85,86,87);
   }
   else if (b1 < 96)
   {
     /*
    +3 Weapons    (Base: Byte#1=80  Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +3   (+1)   Hammer +3         (+1)   10 Arrows +3
        (+2) Short Sword +3  (+2)   2 Javelins +3     (+2)   Battle Axe +3
        (+4) 2-Hand Sword +3 (+4)   Mace +3           (+4)   Cmp Short Bow +3
        (+8) Trident +3      (+8)   10 Bolts +3       (+8)   Cmp Long Bow +3
                             (+16)  Scimitar +3       (+16)  Lt. Crossbow +3
                             (+32)  Sling +3          (+32)  Dagger +3 
                             (+64)  Quarter Staff +3  (+64)  10 Darts +3
                             (+128) Staff Sling +3    (+128) Flail +3
      */
     AssignShopItems(list, b1, 126,127,128,129,0,0,0,0);
     AssignShopItems(list, b2, 118,119,120,121,122,123,124,125);
     AssignShopItems(list, b3, 110,111,112,113,114,115,116,117);
   }
   else if (b1 < 112)
   {
     /*
      +4 Weapons    (Base: Byte#1=96  Byte#2=0   Byte#3=0)
              **Byte#1**            **Byte#2**               **Byte#3**
          (+1) Long Sword +4   (+1)   Hammer +4         (+1)   10 Arrows +4
          (+2) Short Sword +4  (+2)   2 Javelins +4     (+2)   Battle Axe +4
          (+4) 2-Hand Sword +4 (+4)   Mace +4           (+4)   Cmp Short Bow +4
          (+8) Trident +4      (+8)   10 Bolts +4       (+8)   Cmp Long Bow +4
                               (+16)  Scimitar +4       (+16)  Lt. Crossbow +4
                               (+32)  Sling +4          (+32)  Dagger +4 
                               (+64)  Quarter Staff +4  (+64)  10 Darts +4
                               (+128) Staff Sling +4    (+128) Flail +4
      */
     AssignShopItems(list, b1, 156,157,158,159,0,0,0,0);
     AssignShopItems(list, b2, 148,149,150,151,152,153,154,155);
     AssignShopItems(list, b3, 140,141,142,143,144,145,146,147);
   }
   else if (b1 < 128)
   {
     /*
    +5 Weapons    (Base: Byte#1=112 Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +5   (+1)   Hammer +5         (+1)   10 Arrows +5
        (+2) Short Sword +5  (+2)   2 Javelins +5     (+2)   Battle Axe +5
        (+4) 2-Hand Sword +5 (+4)   Mace +5           (+4)   Cmp Short Bow +5
        (+8) Trident +5      (+8)   10 Bolts +5       (+8)   Cmp Long Bow +5
                             (+16)  Scimitar +5       (+16)  Lt. Crossbow +5
                             (+32)  Sling +5          (+32)  Dagger +5 
                             (+64)  Quarter Staff +5  (+64)  10 Darts +5
                             (+128) Staff Sling +5    (+128) Flail +5
      */
     AssignShopItems(list, b1, 186,187,188,189,0,0,0,0);
     AssignShopItems(list, b2, 178,179,180,181,182,183,184,185);
     AssignShopItems(list, b3, 170,171,172,173,174,175,176,177);
   }
   else if (b1 < 144)
   {
     /*
      +1,+2 Armor   (Base: Byte#1=128 Byte#2=0   Byte#3=0)
              **Byte#1**            **Byte#2**               **Byte#3**
          (+1) Ring Mail +2    (+1)   Shield +1         (+1)   Cloak of Pro +1
          (+2) Scale Mail +2   (+2)   Elfin Chain +1    (+2)   Banded Mail +1
          (+4) Shield +2       (+4)   Cloak of Pro +2   (+4)   Chain Mail +1
          (+8) Elfin Chain +2  (+8)   Banded Mail +2    (+8)   Helm +1
                               (+16)  Chain Mail +2     (+16)  Leather Armor +1
                               (+32)  Helm +2           (+32)  Plate Mail +1 
                               (+64)  Leather Armor +2  (+64)  Ring Mail +1
                               (+128) Plate Mail +2     (+128) Scale Mail +1
      */
     AssignShopItems(list, b1, 106,107,108,109,0,0,0,0);
     AssignShopItems(list, b2, 78,79,100,101,102,103,104,105);
     AssignShopItems(list, b3, 70,71,72,73,74,75,76,77);
   }
   else if (b1 < 160)
   {
     /*
    +3,+4 Armor   (Base: Byte#1=144 Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Ring Mail +4    (+1)   Shield +3         (+1)   Cloak of Pro +3
        (+2) Scale Mail +4   (+2)   Elfin Chain +3    (+2)   Banded Mail +3
        (+4) Shield +4       (+4)   Cloak of Pro +4   (+4)   Chain Mail +3
        (+8) Elfin Chain +4  (+8)   Banded Mail +4    (+8)   Helm +3
                             (+16)  Chain Mail +4     (+16)  Leather Armor +3
                             (+32)  Helm +4           (+32)  Plate Mail +3 
                             (+64)  Leather Armor +2  (+64)  Ring Mail +1
                             (+128) Plate Mail +2     (+128) Scale Mail +1
      */
     AssignShopItems(list, b1, 166,167,168,169,0,0,0,0);
     AssignShopItems(list, b2, 138,139,160,161,162,163,164,165);
     AssignShopItems(list, b3, 130,131,132,133,134,135,136,137);
   }
   else if (b1 < 188)
   {
     /*
    Special A     (Base: Byte#1=160 Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Belt of Prot +3 (+1)   Shield +5         (+1)   Cloak of Pro +5
        (+2) Boots: Speed    (+2)   Elfin Chain +5    (+2)   Banded Mail +5
        (+4) Girdle: Gnt STR (+4)   Fine Long Bow     (+4)   Chain Mail +5
        (+8) Periapt: Health (+8)   2 Blessed Bolts   (+8)   Helm +5
                             (+16)  Dart: Hornet's    (+16)  Leather Armor +5
                             (+32)  Cloak: Displace.  (+32)  Plate Mail +5
                             (+64)  Silver Shield +3  (+64)  Ring Mail +5
                             (+128) Silver Shield +4  (+128) Scale Mail +5
      */
     AssignShopItems(list, b1, 202,203,204,205,0,0,0,0);
     AssignShopItems(list, b2, 198,199,45,46,47,48,200,201);
     AssignShopItems(list, b3, 190,191,192,193,194,195,196,197);
   }
   else if (b1 < 207)
   {
     /*
    Special B     (Base: Byte#1=188 Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Gaunt: Ogre Pow (+1)   Vorpal Sword      (+1)   Javelin: Light.
        (+2) Lt. Crossbow -3 (+2)   Drow Lng Sword +1 (+2)   Stone: Good Luck
                             (+4)   Drow Lng Sword +3 (+4)   Neck: Missiles
                             (+8)   Drow Lng Sword +4 (+8)   Eyes: Charming
                             (+16)  Drow Chain +1     (+16)  Bracers: AC 6
                             (+32)  Drow Chain +3     (+32)  Bracers: AC 4
                             (+64)  Drow Chain +4     (+64)  Bracers: AC 2
                             (+128) Fine Long Bow +2  (+128) L Sword vs Giant
      */
     AssignShopItems(list, b1, 233,222,0,0,0,0,0,0);
     AssignShopItems(list, b2, 214,215,216,217,218,219,220,221);
     AssignShopItems(list, b3, 206,207,208,209,210,211,212,213);
   }
   else if (b1 < 223)
   {
     /*
    Rings         (Base: Byte#1=207 Byte#2=192 Byte#3=0)
            **Byte#2**            **Byte#3**
        (+1)  Fire Resist    (+1)   Ring
        (+2)  Invisibility   (+2)   Protection +1
        (+4)  Blinking       (+4)   Protection +2
        (+8)  Wizardry       (+8)   Protection +3
        (+16) Pro vs Evil    (+16)  Protection +4
                             (+32)  Protection +5
                             (+64)  Cold Resist
                             (+128) Electric Immunity
      */
     AssignShopItems(list, b2, 232,234,235,236,237,0,0,0);
     AssignShopItems(list, b3, 223,225,226,227,228,229,230,231);
   }
   else if (b1 < 239)
   {
     /*
      Wands         (Base: Byte#1=223 Byte#2=255 Byte#3=224)
              **Byte#3**
          (+1)  Ice Storm
          (+2)  Fireballs
          (+4)  Lightning
          (+8)  Magic Missiles
          (+16) Paralyzation
      */
     AssignShopItems(list, b3, 238,239,240,241,242,0,0,0);
   }
   else if ((b1 < 255) && (b2 == 255))
   {
     /*
      Potions       (Base: Byte#1=239 Byte#2=255 Byte#3=192)
              **Byte#3**
          (+1)  Speed
          (+2)  Healing
          (+4)  Extra Healing
          (+8)  Giant Strength
          (+16) Elixir of Youth
          // RBT - FRUA also allows Potion of Invisibility
      */
     AssignShopItems(list, b3, 243,244,245,246,247,248,0,0);
   }
   else if ((b1 == 255) && (b2 == 255))
   {
     /*
    Scrolls       (Base: Byte#1=255 Byte#2=255 Byte#3=128)
            **Byte#3**
        (+1)  Prot vs Dragon Breath
        (+2)  Cleric: 3 Spells
        (+4)  Cleric: 3 Spells
        (+8)  Mage: 3 Spells
        (+16) Mage: 3 Spells
        (+32) Mage: 3 Spells
        (+64) Mage: 3 Spells
      */
     //AssignShopItems(list, b3, 249,250,251,252,253,254,255,0);
     AssignShopItems(list, b3, 249,250,251,252,253,254,254,0); // item id 255 doesnt exist?
   }
}

UAMapCell GetMapCell(const UALevelData &level, int x, int y)
{  
   int index = (y * level.header.width) + x;

   ASSERT( index < MAX_FRUA_LEVEL_GRIDS );

   return level.map[index];
}

int GetStrPtr(const UALevelStrings *pStrings, int offset)
{
  int index = 0; // base offset for strings

  ASSERT(offset < MAX_FRUA_LEVEL_STRINGS);

  // add lengths of all strings that preceed offset
  for (int i=0; i < offset; i++)
     index += pStrings->StringLength[i]; // nbr compressed bytes

  ASSERT( index < 6761);
  return index;
}

bool GetStringAt(const UALevelStrings *pStrings, int offset, CString &result)
{
  result = "";
  
  if (offset == 0)
     return false; // offset zero indicates an emptry string

  if (offset > MAX_FRUA_LEVEL_STRINGS)
    return false; // probably a bug

  offset--;

  if (pStrings->StringLength[offset] == 0)
     return false; // this string has no length

  int compressedLength = pStrings->StringLength[offset];

  int index = GetStrPtr(pStrings, offset); 

  int maxCompressedStringIdx = (index + compressedLength) - 1;
  
  BYTE buffer[512];
  memset(buffer, 0, sizeof(buffer));

  int j = 0;
  BYTE bitPattern = 0;
  BYTE bitNum   = 7;
  BYTE bitCount = 0;
  bool done = false;

  /* Loop until end of string found */
  while ((!done) && (j < MAX_FRUA_LEVEL_STRING_LEN) && (index <= maxCompressedStringIdx))
  {
    /* Left shift bit pattern to add new bit */
    bitPattern <<= 1;

    /* If the current bit is a 1, add 1 to the pattern */
    if (pStrings->Strings[index] & (1 << bitNum))
      bitPattern++;

    bitCount++;

    if (bitCount == 6)
    {
      if (bitPattern != 0)
      {
        if (bitPattern <= 31)
          bitPattern |= 0x40;
        
        buffer[j++] = bitPattern;
        bitPattern = 0;
        bitCount = 0;
      }
      else
        done = true;      
    }

    /* If reached the end of current byte, move to the next one */
    if (bitNum == 0)
    {
      bitNum = 7;
      index++;
    }
    else
      bitNum--;
  }

  buffer[j] = '\0';
  result = buffer;

  return (!result.IsEmpty());
}

CString GetSpecialItemKeyQuestText(int code)
{
   CString strTemp;
   //       * 0-7   Key 1-8 
   //       * 8-19  Item 1-12
   //       * 20-63 Quest 1-44
   if (code < 8)
   {
      strTemp.Format("special key %i", code+1);      
   }
   else if (code < 20)
   {
      strTemp.Format("special item %i", (code-8)+1);      
   }
   else if (code < 64)
   {
      strTemp.Format("special quest %i", (code-20)+1);      
   }
   else
   {
      strTemp.Format("unknown special item %i", code);
   }
   return strTemp;
}

bool ImportFRUAGeoDat(int levelIdx, const char *path)
{
   bool success = TRUE;
   
   ASSERT( levelIdx >= 0 );
   ASSERT( levelIdx < MAX_FRUA_LEVELS );

   FILE *fp = fopen(path, "rb");

   if (fp == NULL)
      return false;

   // read the header
   success = (fread(&Levels[levelIdx].header, sizeof(Levels[levelIdx].header), 1, fp) == 1);

   if (success)
   {
      fseek(fp, LEVEL_MAP_CELLS_OFFSET, SEEK_SET);
      success = (fread(&Levels[levelIdx].map, sizeof(Levels[levelIdx].map), 1, fp) == 1);
   }

   if (success)
   {
      fseek(fp, LEVEL_EVENTS_OFFSET, SEEK_SET);
      success = (fread(&Levels[levelIdx].events, sizeof(Levels[levelIdx].events), 1, fp) == 1);
   }

   if (success)
   {
      fseek(fp, LEVEL_COMPRESSED_STRINGS_OFFSET, SEEK_SET);
      success = (fread(&Levels[levelIdx].strings, sizeof(Levels[levelIdx].strings), 1, fp) == 1);
   }

   fclose(fp);    

   Levels[levelIdx].isPopulated = success;

   return success;
}

void FormatLevelHeaderJson(const UALevelData &level, Json::Value &currLevel)
{  
   currLevel["name"] = CStringOrEmptyString(level.header.moduleName);
   currLevel["area_view_allowed"] = IntToString(level.header.areaViewFlag);
   currLevel["map_height"] = IntToString(level.header.height);
   currLevel["map_width"] = IntToString(level.header.width);
   currLevel["monster_summoned"] = CStringOrEmptyString(LookupMonsterNameByMonsterCode( level.header.whichMonsterSummoned ));
   currLevel["turn_undead_modifier"] = IntToString(level.header.undeadTurnModifier);

   bool isOverlandMap = ((level.header.wall1 == 255) && (level.header.wall2 == 255) && (level.header.wall3 == 255));

   if (isOverlandMap)
      currLevel["is_overland"] = "1";
   else
      currLevel["is_overland"] = "0";

   Json::Value &artSlots = currLevel["art_slots"];
   artSlots["Wall_1"] = FRUAGetWallSlotArtName(level.header.wall1);// IntToString(level.header.wall1);
   artSlots["Wall_2"] = FRUAGetWallSlotArtName(level.header.wall2);// IntToString(level.header.wall2);
   artSlots["Wall_3"] = FRUAGetWallSlotArtName(level.header.wall3);// IntToString(level.header.wall3);

   artSlots["camp"] =  FRUAGetBigSmallSpritePicText(level.header.campPic); //IntToString(level.header.campPic);
   artSlots["dungeon_combat"] = FRUAGetDungeonCombatSlotArtName(level.header.dungeonCombatArtSlot); //IntToString(level.header.dungeonCombatArtSlot);
   artSlots["treasure"] = FRUAGetBigSmallSpritePicText(level.header.treasurePic); //IntToString(level.header.treasurePic);
   artSlots["wilderness_combat"] = FRUAGetWildernessCombatSlotArtName(level.header.wildernessCombatArtSlot); //IntToString(level.header.wildernessCombatArtSlot);

   // RBT array?
   artSlots["overland_wilderness_1"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[0]); //IntToString(level.header.overlandWildernessArtSlots[0]);
   artSlots["overland_wilderness_2"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[1]); //IntToString(level.header.overlandWildernessArtSlots[1]);
   artSlots["overland_wilderness_3"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[2]); //IntToString(level.header.overlandWildernessArtSlots[2]);
   artSlots["overland_wilderness_4"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[3]); //IntToString(level.header.overlandWildernessArtSlots[3]);
   artSlots["overland_wilderness_5"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[4]); //IntToString(level.header.overlandWildernessArtSlots[4]);
   artSlots["overland_wilderness_6"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[5]); //IntToString(level.header.overlandWildernessArtSlots[5]);
   artSlots["overland_wilderness_7"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[6]); //IntToString(level.header.overlandWildernessArtSlots[6]);
   artSlots["overland_wilderness_8"] = FRUAGetOverlandWildernessSlotArtName(level.header.overlandWildernessArtSlots[7]); //IntToString(level.header.overlandWildernessArtSlots[7]);

   if (isOverlandMap)
   {
      artSlots["overland_big_pic"] = IntToString(level.header.backdropSlot1);

      artSlots["backdrop_1"] = NoneString;
      artSlots["backdrop_2"] = NoneString;
      artSlots["backdrop_3"] = NoneString;
      artSlots["backdrop_4"] = NoneString;
   }
   else
   {
      artSlots["overland_big_pic"] = NoneString;

      artSlots["backdrop_1"] = FRUAGetBackgroundSlotArtName(level.header.backdropSlot1); //IntToString(level.header.backdropSlot1);
      artSlots["backdrop_2"] = FRUAGetBackgroundSlotArtName(level.header.backdropSlot2); //IntToString(level.header.backdropSlot2);
      artSlots["backdrop_3"] = FRUAGetBackgroundSlotArtName(level.header.backdropSlot3); //IntToString(level.header.backdropSlot3);
      artSlots["backdrop_4"] = FRUAGetBackgroundSlotArtName(level.header.backdropSlot4); //IntToString(level.header.backdropSlot4);      
   }

   Json::Value entryPointsArray(Json::arrayValue);
   for (int i=0; i<8; i++)
   {
      Json::Value entryPoint;

      entryPoint["id"] = IntToString(i+1);
      entryPoint["x"] = IntToString(level.header.entryPoints[i].entryCol);
      entryPoint["y"] = IntToString(level.header.entryPoints[i].entryRow);
      
      if (isOverlandMap)
      {
         // facing has no meaning for overland maps
         entryPoint["facing"] = NoneString;
      }
      else
      {
         switch (level.header.entryPoints[i].entryFacing)
         {
         // RBT: North = 8, not 0
         case 8: entryPoint["facing"] = "north"; break;
         case 2: entryPoint["facing"] = "east"; break;
         case 4: entryPoint["facing"] = "south"; break;
         case 6: entryPoint["facing"] = "west"; break;
         default: entryPoint["facing"] = UnknownString; break;
         }         
      }

      entryPointsArray.append(entryPoint);
   }
   currLevel["entry_points_count"] = IntToString(8);
   currLevel["entry_points"] = entryPointsArray;

   Json::Value zoneArray(Json::arrayValue);
   for (int i=0; i<8; i++)
   {
      CString strZone("");
      Json::Value zone;

      zone["id"] = IntToString(i+1);
      zone["name"] = CStringOrEmptyString(level.header.zoneNames[i]);

      unsigned short temp;
      temp = (level.header.zoneMsgStringIds[i] & 0xff00) >> 8;
      temp |= (level.header.zoneMsgStringIds[i] & 0x00ff) << 8;

      GetStringAt(&level.strings, temp, strZone);
      zone["message"] = CStringOrEmptyString(strZone);

      zoneArray.append(zone);
   }
   currLevel["zones_count"] = IntToString(8);
   currLevel["zones"] = zoneArray;

   Json::Value restEventArray(Json::arrayValue);
   for (int i=0; i<8; i++)
   {
      Json::Value restEvent;

      restEvent["id"] = IntToString(i+1);
      restEvent["event_id"] = IntToString(level.header.restEvents[i].eventCode);
      restEvent["minute_interval"] = IntToString(level.header.restEvents[i].minutes);

      BYTE pcntChance = level.header.restEvents[i].chancePcnt_NoRestFlag & 0x7F;
      BYTE restAllowed = (level.header.restEvents[i].chancePcnt_NoRestFlag & 0x80) ? 0 : 1;

      restEvent["encounter_percent_chance"] = IntToString(pcntChance);
      restEvent["resting_allowed"] = IntToString(restAllowed);

      restEventArray.append(restEvent);
   }
   currLevel["rest_events_count"] = IntToString(8);
   currLevel["rest_events"] = restEventArray;

   Json::Value stepEventArray(Json::arrayValue);
   for (int i=0; i<8; i++)
   {
      Json::Value stepEvent;

      stepEvent["id"] = IntToString(i+1);
      stepEvent["num_steps"] = IntToString(level.header.stepEvents[i].numSteps);
      stepEvent["event_id"] = IntToString(level.header.stepEvents[i].eventCode);

      // all enabled by default, check bits to see if a zone excluded
      stepEvent["enabled_zone_1"] = ((level.header.stepEvents[i].zoneFlags & 0x01) ? "0" : "1");
      stepEvent["enabled_zone_2"] = ((level.header.stepEvents[i].zoneFlags & 0x02) ? "0" : "1");
      stepEvent["enabled_zone_3"] = ((level.header.stepEvents[i].zoneFlags & 0x04) ? "0" : "1");
      stepEvent["enabled_zone_4"] = ((level.header.stepEvents[i].zoneFlags & 0x08) ? "0" : "1");
      stepEvent["enabled_zone_5"] = ((level.header.stepEvents[i].zoneFlags & 0x10) ? "0" : "1");
      stepEvent["enabled_zone_6"] = ((level.header.stepEvents[i].zoneFlags & 0x20) ? "0" : "1");
      stepEvent["enabled_zone_7"] = ((level.header.stepEvents[i].zoneFlags & 0x40) ? "0" : "1");
      stepEvent["enabled_zone_8"] = ((level.header.stepEvents[i].zoneFlags & 0x80) ? "0" : "1");

      stepEventArray.append(stepEvent);
   }
   currLevel["step_events_count"] = IntToString(8);
   currLevel["step_events"] = stepEventArray;
}

void FormatLevelMapJson(const UALevelData &level, Json::Value &currLevel)
{
   if ((level.header.height == 0) || (level.header.width == 0))
   {
      currLevel["map_grid_count"] = "0";
      return;
   }

   int count = 0;
   Json::Value gridArray(Json::arrayValue);

   for (int y=0; y < level.header.height; y++)
   {
      for (int x=0; x < level.header.width; x++)      
      { 
         UAMapCell gridCell = GetMapCell(level, x, y);
         Json::Value grid;
                  
         grid["x"] = IntToString(x);
         grid["y"] = IntToString(y);

         BYTE backdrop = gridCell.backdropAndZone & 0x03;
         switch (backdrop)
         {
         case 0: grid["backdrop"] = FRUAGetBackgroundSlotArtName( level.header.backdropSlot1 ); break;
         case 1: grid["backdrop"] = FRUAGetBackgroundSlotArtName( level.header.backdropSlot2 ); break;
         case 2: grid["backdrop"] = FRUAGetBackgroundSlotArtName( level.header.backdropSlot3 ); break;
         case 3: grid["backdrop"] = FRUAGetBackgroundSlotArtName( level.header.backdropSlot4 ); break;
         default: grid["backdrop"] = NoneString; break;
         }

         switch (gridCell.backdropAndZone & 0xFC)
         {
         case 0:  grid["zone"] = "1"; break;
         case 4:  grid["zone"] = "2"; break;
         case 8:  grid["zone"] = "3"; break;
         case 12: grid["zone"] = "4"; break;
         case 16: grid["zone"] = "5"; break;
         case 20: grid["zone"] = "6"; break;
         case 24: grid["zone"] = "7"; break;
         case 28: grid["zone"] = "8"; break;
         default: grid["zone"] = UnknownString; break;
         }

         grid["event_id"] = IntToString(gridCell.eventCode);

         grid["wall_north"] = GetWallSlotText(level, gridCell.northWall & 0xF);
         grid["wall_east"] = GetWallSlotText(level, gridCell.eastWall & 0xF);
         grid["wall_south"] = GetWallSlotText(level, gridCell.southWall & 0xF);
         grid["wall_west"] = GetWallSlotText(level, gridCell.westWall & 0xF);

         grid["wall_north_blockage"] = FRUAGetWallBlockageText(gridCell.northWall & 0xF0);
         grid["wall_east_blockage"] = FRUAGetWallBlockageText(gridCell.eastWall & 0xF0);
         grid["wall_south_blockage"] = FRUAGetWallBlockageText(gridCell.southWall & 0xF0);
         grid["wall_west_blockage"] = FRUAGetWallBlockageText(gridCell.westWall & 0xF0);

         count++;
         gridArray.append(grid);
      }
   }

   currLevel["map_grid"] = gridArray;
   currLevel["map_grid_count"] = IntToString(count);
}

// using this only because it's easier to visually spot check 
// byte offsets in the hack docs and compare directly to byte
// offsets in our local event details buffer
#define EVENT_DETAILS_OFFSET( byteNum ) (event.eventData[byteNum-5])

// copied from DC
unsigned short GetWord(BYTE offset, const UALevelEvent &event)
{
   unsigned short temp;
   temp = (EVENT_DETAILS_OFFSET(offset) & 0xff);
   offset++;
   temp += ((EVENT_DETAILS_OFFSET(offset) & 0xff) << 8);
   return temp;
}

unsigned int GetThreeBytes(BYTE offset, const UALevelEvent &event)
{
   unsigned short temp1 = GetWord(offset, event);
   offset += 2;
   unsigned short temp2 = (EVENT_DETAILS_OFFSET(offset) & 0xff);

   return ((temp2<<16)|temp1);
}

// copied from DCqu
unsigned int GetDWord(BYTE offset, const UALevelEvent &event)
{
   unsigned short temp1 = GetWord(offset, event);
   unsigned short temp2 = GetWord(offset+2, event);
   return ((temp2<<16)|temp1);
}

void FormatLevelEventJson_Combat(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{  
   WORD text =                               GetWord(5, event);
   BYTE picture =                            EVENT_DETAILS_OFFSET(7);
   BYTE monsterMoraleModifierPcnt =          EVENT_DETAILS_OFFSET(8) & 0x7F;
   BYTE surprise =                           EVENT_DETAILS_OFFSET(9) & 0xC0;
   BYTE location =                           EVENT_DETAILS_OFFSET(9) & 0x20;   
   BYTE monster1count =                      EVENT_DETAILS_OFFSET(9) & 0x1F;
   BYTE monster1Type =                       EVENT_DETAILS_OFFSET(10);
   BYTE monster2count =                      EVENT_DETAILS_OFFSET(11) & 0x1F;
   BYTE monster4Friendly =                   EVENT_DETAILS_OFFSET(11) & 32;
   BYTE monster5Friendly =                   EVENT_DETAILS_OFFSET(11) & 64;
   BYTE monster6Friendly =                   EVENT_DETAILS_OFFSET(11) & 128;
   BYTE monster2Type =                       EVENT_DETAILS_OFFSET(12);
   BYTE monster3count =                      EVENT_DETAILS_OFFSET(13) & 0x1F;
   BYTE autoApproach =                       EVENT_DETAILS_OFFSET(13) & 32;
   BYTE partyNeverDies =                     EVENT_DETAILS_OFFSET(13) & 64;
   BYTE noTreasureFromMonsters =             EVENT_DETAILS_OFFSET(13) & 128;
   BYTE monster3type =                       EVENT_DETAILS_OFFSET(14);
   BYTE monster4count =                      EVENT_DETAILS_OFFSET(15) & 0x1F;
   BYTE magicPrevented =                     EVENT_DETAILS_OFFSET(15) & 128;   
   BYTE monsterDistance =                    EVENT_DETAILS_OFFSET(15) & 0x60;   
   BYTE monster4type =                       EVENT_DETAILS_OFFSET(16);
   BYTE monster5count =                      EVENT_DETAILS_OFFSET(17) & 0x1F;
   BYTE monsterAttackFrontNorthEastSouth =   EVENT_DETAILS_OFFSET(17) & 0xE0;   
   BYTE monster5type =                       EVENT_DETAILS_OFFSET(18);
   BYTE monster6count =                      EVENT_DETAILS_OFFSET(19) & 0x1F;
   BYTE monsterAttackWest =                  EVENT_DETAILS_OFFSET(19) & 32;
   BYTE monsterTurnModifier =                EVENT_DETAILS_OFFSET(19) & 0xC0;
   BYTE monster6type =                       EVENT_DETAILS_OFFSET(20);

   CString strTemp;
   GetStringAt(pStrings, text, strTemp);

   details["text"] = CStringOrEmptyString(strTemp);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);   
  
   details["monster_1_type"] = FRUAGetMonsterNameText(monster1Type); 
   details["monster_1_count"] = IntToString(monster1count);
   details["monster_1_friendly"] = "0";
   details["monster_2_type"] = FRUAGetMonsterNameText(monster2Type); 
   details["monster_2_count"] = IntToString(monster2count);
   details["monster_2_friendly"] = "0";
   details["monster_3_type"] = FRUAGetMonsterNameText(monster3type); 
   details["monster_3_count"] = IntToString(monster3count);
   details["monster_3_friendly"] = "0";
   details["monster_4_type"] = FRUAGetMonsterNameText(monster4type); 
   details["monster_4_count"] = IntToString(monster4count);
   details["monster_4_friendly"] = monster4Friendly?"1":"0";
   details["monster_5_type"] = FRUAGetMonsterNameText(monster5type); 
   details["monster_5_count"] = IntToString(monster5count);
   details["monster_5_friendly"] = monster5Friendly?"1":"0";
   details["monster_6_type"] = FRUAGetMonsterNameText(monster6type); 
   details["monster_6_count"] = IntToString(monster6count);
   details["monster_6_friendly"] = monster6Friendly?"1":"0";

   details["monster_morale_modifier_pcnt"] = IntToString(monsterMoraleModifierPcnt);
   details["auto_approach"] = autoApproach?"1":"0";
   details["party_never_dies"] = partyNeverDies?"1":"0";
   details["no_treasure_from_monsters"] = noTreasureFromMonsters?"1":"0";
   details["magic_disabled"] = magicPrevented?"1":"0";

   switch (monsterDistance)
   {
   case  0: details["start_distance"] = "close"; break;
   case 32: details["start_distance"] = "nearby"; break;
   case 64: details["start_distance"] = "far away"; break;
   default: details["start_distance"] = UnknownString; break;
   }

   switch (surprise)
   {
   case 0:   details["surprise"] = "neither"; break;
   case 64:  details["surprise"] = "party"; break;
   case 128: details["surprise"] = "monster"; break;
   default:  details["surprise"] = UnknownString; break;
   }
   switch (location)
   {
   case 0:  details["location"] = "indoors"; break;
   case 32: details["location"] = "outdoors"; break;
   default: details["location"] = UnknownString; break;
   }

   // The direction flag is split across two bytes.
   // The gist seems to be that if none of the direction
   // flags are set, then the attack direction is North,
   // else combine direction flags

   CString directions("");
   if (monsterAttackFrontNorthEastSouth & 32)
      directions += "north";
   if (monsterAttackFrontNorthEastSouth & 64)
   {
      if (directions.GetLength() > 0)
         directions += ", ";
      directions += "east";
   }

   if (monsterAttackFrontNorthEastSouth & 128)
   {
      if (directions.GetLength() > 0)
         directions += ", ";
      directions += "south";
   }

   if (monsterAttackWest & 32)
   {
      if (directions.GetLength() > 0)
         directions += ", ";
      directions += "west";
   }

   if (directions.GetLength() == 0)
      directions += "front";

   details["attack_direction"] = CStringOrEmptyString(directions);
   /*
   if (monsterAttackFrontNorthEastSouth == 0)
      details["attack_direction"] = "front";
   else if (monsterAttackFrontNorthEastSouth == 32)
      details["attack_direction"] = "north";
   else if (monsterAttackFrontNorthEastSouth == 64)
      details["attack_direction"] = "east";
   else if (monsterAttackFrontNorthEastSouth == 128)
      details["attack_direction"] = "south";
   else if (monsterAttackWest == 32)
      details["attack_direction"] = "west";
      */

   switch (monsterTurnModifier)
   {
   case 0: details["turn_modifier"] = NoneString; break;
   case 64: details["turn_modifier"] = "hard"; break;
   case 128: details["turn_modifier"] = "difficult"; break;
   case 192: details["turn_modifier"] = "impossible"; break;
   default: details["turn_modifier"] = UnknownString; break;
   }

   /*
   TRACE("COMBAT: Text %s\n", details["text"].asCString());
   TRACE("COMBAT: Picture %s\n", details["picture"].asCString());
   TRACE("COMBAT:     Monst 1 Type: %s\n", details["monster_1_type"].asCString());
   TRACE("COMBAT:    Monst 1 Count: %s\n", details["monster_1_count"].asCString());
   TRACE("COMBAT: Monst 1 Friendly: %s\n", details["monster_1_friendly"].asCString());
   TRACE("COMBAT:     Monst 2 Type: %s\n", details["monster_2_type"].asCString());
   TRACE("COMBAT:    Monst 2 Count: %s\n", details["monster_2_count"].asCString());
   TRACE("COMBAT: Monst 2 Friendly: %s\n", details["monster_2_friendly"].asCString());
   TRACE("COMBAT:     Monst 3 Type: %s\n", details["monster_3_type"].asCString());
   TRACE("COMBAT:    Monst 3 Count: %s\n", details["monster_3_count"].asCString());
   TRACE("COMBAT: Monst 3 Friendly: %s\n", details["monster_3_friendly"].asCString());
   TRACE("COMBAT:     Monst 4 Type: %s\n", details["monster_4_type"].asCString());
   TRACE("COMBAT:    Monst 4 Count: %s\n", details["monster_4_count"].asCString());
   TRACE("COMBAT: Monst 4 Friendly: %s\n", details["monster_4_friendly"].asCString());
   TRACE("COMBAT:     Monst 5 Type: %s\n", details["monster_5_type"].asCString());
   TRACE("COMBAT:    Monst 5 Count: %s\n", details["monster_5_count"].asCString());
   TRACE("COMBAT: Monst 5 Friendly: %s\n", details["monster_5_friendly"].asCString());
   TRACE("COMBAT:     Monst 6 Type: %s\n", details["monster_6_type"].asCString());
   TRACE("COMBAT:    Monst 6 Count: %s\n", details["monster_6_count"].asCString());
   TRACE("COMBAT: Monst 6 Friendly: %s\n", details["monster_6_friendly"].asCString());
   TRACE("COMBAT: MonstMoralModPcnt %s\n", details["monster_morale_modifier_pcnt"].asCString());
   TRACE("COMBAT: AutoApproach %s\n", details["auto_approach"].asCString());
   TRACE("COMBAT: PartyNeverDies %s\n", details["party_never_dies"].asCString());
   TRACE("COMBAT: NoTreasFromMonster %s\n", details["no_treasure_from_monsters"].asCString());
   TRACE("COMBAT: MagicDisabled %s\n", details["magic_disabled"].asCString());
   TRACE("COMBAT: StartDistance %s\n", details["start_distance"].asCString());
   TRACE("COMBAT: Surprise %s\n", details["surprise"].asCString());
   TRACE("COMBAT: Location %s\n", details["location"].asCString());
   TRACE("COMBAT: MonstersAttackFrom %s\n", details["attack_direction"].asCString());
   TRACE("COMBAT: TurnUndeadMod %s\n", details["turn_modifier"].asCString());
   */

   // RBT: just noticed this method for iterating. I cant order
   // the output but it is much easier to paste this in every event
   // instead of doing the above TRACE formatting...
   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("COMBAT: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_TextEvent(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   BYTE backupOneStep = EVENT_DETAILS_OFFSET(5) & 32;
   BYTE message1hitreturn = EVENT_DETAILS_OFFSET(5) & 0x01;
   BYTE message2hitreturn = EVENT_DETAILS_OFFSET(5) & 0x02;
   BYTE message3hitreturn = EVENT_DETAILS_OFFSET(5) & 0x04;
   BYTE message4hitreturn = EVENT_DETAILS_OFFSET(5) & 0x08;
   BYTE message5hitreturn = EVENT_DETAILS_OFFSET(5) & 0x10;

   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE picDistance = EVENT_DETAILS_OFFSET(8) & 0x03;
   BYTE message1highlight = EVENT_DETAILS_OFFSET(8) & 0x04;
   BYTE message2highlight = EVENT_DETAILS_OFFSET(8) & 0x08;
   BYTE message3highlight = EVENT_DETAILS_OFFSET(8) & 0x10;
   BYTE message4highlight = EVENT_DETAILS_OFFSET(8) & 0x20;
   BYTE message5highlight = EVENT_DETAILS_OFFSET(8) & 0x40;

   WORD message1 = GetWord(9, event);
   WORD message2 = GetWord(11, event);
   WORD message3 = GetWord(13, event);
   WORD message4 = GetWord(15, event);
   WORD message5 = GetWord(17, event);

   BYTE sound = EVENT_DETAILS_OFFSET(19);

   CString strMsg1;
   GetStringAt(pStrings, message1, strMsg1);
   details["message_1_text"] = CStringOrEmptyString(strMsg1);
   details["message_1_hit_return"] = message1hitreturn?"1":"0";
   details["message_1_highlight"] = message1highlight?"1":"0";

   CString strMsg2;
   GetStringAt(pStrings, message2, strMsg2);
   details["message_2_text"] = CStringOrEmptyString(strMsg2);
   details["message_2_hit_return"] = message2hitreturn?"1":"0";
   details["message_2_highlight"] = message2highlight?"1":"0";
   
   CString strMsg3;
   GetStringAt(pStrings, message3, strMsg3);
   details["message_3_text"] = CStringOrEmptyString(strMsg3);
   details["message_3_hit_return"] = message3hitreturn?"1":"0";
   details["message_3_highlight"] = message3highlight?"1":"0";

   CString strMsg4;
   GetStringAt(pStrings, message4, strMsg4);
   details["message_4_text"] = CStringOrEmptyString(strMsg4);
   details["message_4_hit_return"] = message4hitreturn?"1":"0";
   details["message_4_highlight"] = message4highlight?"1":"0";

   CString strMsg5;
   GetStringAt(pStrings, message5, strMsg5);
   details["message_5_text"] = CStringOrEmptyString(strMsg5);
   details["message_5_hit_return"] = message5hitreturn?"1":"0";
   details["message_5_highlight"] = message5highlight?"1":"0";

   details["force_backup_one_step"] = backupOneStep?"1":"0";
   details["sound"] = FRUAGetSoundNameText(sound);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   switch (picDistance)
   {
   case 0: details["picture_distance"] = "close"; break;
   case 1: details["picture_distance"] = "nearby"; break;
   case 2: details["picture_distance"] = "far away"; break;
   default: details["picture_distance"] = UnknownString; break;
   }   

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("TEXT: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_GiveTreasure(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   DWORD platinum = GetThreeBytes(5, event);
   BYTE itemsIdentified = EVENT_DETAILS_OFFSET(8) & 128;
   WORD gems = GetWord(9, event);
   WORD jewelry = GetWord(11, event);
   BYTE item1 = EVENT_DETAILS_OFFSET(13);
   BYTE item2 = EVENT_DETAILS_OFFSET(14);
   BYTE item3 = EVENT_DETAILS_OFFSET(15);
   BYTE item4 = EVENT_DETAILS_OFFSET(16);
   BYTE item5 = EVENT_DETAILS_OFFSET(17);
   BYTE item6 = EVENT_DETAILS_OFFSET(18);
   BYTE item7 = EVENT_DETAILS_OFFSET(19);
   BYTE item8 = EVENT_DETAILS_OFFSET(20);

   CString strItemName;
   unsigned int itemIdx;

   details["platinum"] = IntToString(platinum);
   details["gems"] = IntToString(gems);
   details["jewelry"] = IntToString(jewelry);
   details["items_are_identified"] = itemsIdentified?"1":"0";

   strItemName = LookupItemNameByItemCode(item1);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_1"] = CStringOrEmptyString(strItemName);
   details["item_1_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item1));
   details["item_1_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item2);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_2"] = CStringOrEmptyString(strItemName);
   details["item_2_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item2));
   details["item_2_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item3);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_3"] = CStringOrEmptyString(strItemName);
   details["item_3_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item3));
   details["item_3_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item4);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_4"] = CStringOrEmptyString(strItemName);
   details["item_4_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item4));
   details["item_4_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item5);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_5"] = CStringOrEmptyString(strItemName);
   details["item_5_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item5));
   details["item_5_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item6);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_6"] = CStringOrEmptyString(strItemName);
   details["item_6_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item6));
   details["item_6_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item7);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_7"] = CStringOrEmptyString(strItemName);
   details["item_7_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item7));
   details["item_7_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item8);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_8"] = CStringOrEmptyString(strItemName);
   details["item_8_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item8));
   details["item_8_id"] = IntToString(itemIdx);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("GIVE TREASURE: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Damage(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE target = EVENT_DETAILS_OFFSET(8) & 0x0C;
   BYTE savingThrow = EVENT_DETAILS_OFFSET(8) & 0x30;
   BYTE numAttacks = EVENT_DETAILS_OFFSET(9);
   BYTE numDice = EVENT_DETAILS_OFFSET(10);
   BYTE diceSides = EVENT_DETAILS_OFFSET(11);
   BYTE dmgBonus = EVENT_DETAILS_OFFSET(12);
   BYTE thaco = 60 - EVENT_DETAILS_OFFSET(13);
   BYTE savingThrowBonus = EVENT_DETAILS_OFFSET(14) & 0x0F;
   BYTE saveType = EVENT_DETAILS_OFFSET(14) & 0x70;
   BYTE picDistance = EVENT_DETAILS_OFFSET(15);
   BYTE pcntChanceToTargetEachPartyMember = EVENT_DETAILS_OFFSET(17);

   CString strTemp;
   GetStringAt(pStrings, text, strTemp);
   details["text"] = CStringOrEmptyString(strTemp);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   switch (picDistance)
   {
   case 0: details["picture_distance"] = "close"; break;
   case 32: details["picture_distance"] = "nearby"; break;
   case 64: details["picture_distance"] = "far away"; break;
   default: details["picture_distance"] = UnknownString; break;
   }   

   switch (target)
   {
   case  0: details["target"] = "entire party"; break;
   case  4: details["target"] = "active party member"; break;
   case  8: details["target"] = "random party member"; break;
   case 12: details["target"] = "percent chance per party member"; break;
   default: details["target"] = UnknownString; break;
   }
   switch (savingThrow)
   {
   case  0: details["saving_throw"] = "no save allowed"; break;
   case 16: details["saving_throw"] = "save for half damage"; break;
   case 32: details["saving_throw"] = "save for no damage"; break;
   case 48: details["saving_throw"] = "use THAC0"; break;
   default: details["saving_throw"] = UnknownString; break;
   }

   details["num_attacks"] = IntToString(numAttacks);
   details["num_dice"] = IntToString(numDice);
   details["num_dice_sides"] = IntToString(diceSides);
   details["dmg_bonus_per_attack"] = IntToString(dmgBonus);
   details["attack_THAC0"] = IntToString(thaco);
   details["saving_throw_bonus"] = IntToString(savingThrowBonus);
   details["percent_chance_per_party_member"] = IntToString(pcntChanceToTargetEachPartyMember);

   switch (saveType)
   {
   case  0: details["save_vs"] = "poison"; break;
   case 16: details["save_vs"] = "petrification"; break;
   case 32: details["save_vs"] = "wand"; break;
   case 48: details["save_vs"] = "breath"; break;
   case 64: details["save_vs"] = "spell"; break;
   default: details["save_vs"] = UnknownString; break;
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("GIVE DMG: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Stairs(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD asktext = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE askYesNo = EVENT_DETAILS_OFFSET(8) & 32;
   BYTE transferOn = EVENT_DETAILS_OFFSET(8) & 64;
   BYTE destFace = EVENT_DETAILS_OFFSET(8) & 12;
   BYTE destY = EVENT_DETAILS_OFFSET(9);
   BYTE destX = EVENT_DETAILS_OFFSET(10);
   WORD enterText = GetWord(11, event);
   BYTE execDestEvent = EVENT_DETAILS_OFFSET(13) & 4;
   BYTE destPlacement = EVENT_DETAILS_OFFSET(13) & 1;
   BYTE entryPoint = EVENT_DETAILS_OFFSET(14);

   CString strTemp1;
   GetStringAt(pStrings, asktext, strTemp1);
   details["text_ask"] = CStringOrEmptyString(strTemp1);
   CString strTemp2;
   GetStringAt(pStrings, enterText, strTemp2);
   details["text_enter"] = CStringOrEmptyString(strTemp2);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["ask_yes_no"] = askYesNo?"1":"0";
   details["transfer_on_yes_or_no"] = (transferOn==64)?"no":"yes";
   details["dest_x"] = IntToString(destX);
   details["dest_y"] = IntToString(destY);
   details["dest_entry_point"] = IntToString(entryPoint);
   details["dest_type"] = (destPlacement==1)?"entry point":"xy";
   details["dest_event_execute"] = execDestEvent?"1":"0";   

   switch (destFace)
   {
   case 0: details["dest_facing"] = "north"; break;
   case 4: details["dest_facing"] = "east"; break;
   case 8: details["dest_facing"] = "south"; break;
   case 12: details["dest_facing"] = "west"; break;
   default: details["dest_facing"] = UnknownString; break;
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("STAIRS: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_TrainingHall(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE backupOneStep = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE allowMage = EVENT_DETAILS_OFFSET(9) & 1;
   BYTE allowCleric = EVENT_DETAILS_OFFSET(9) & 2;
   BYTE allowThief = EVENT_DETAILS_OFFSET(9) & 4;
   BYTE allowFighter = EVENT_DETAILS_OFFSET(9) & 8;
   BYTE allowPaladin = EVENT_DETAILS_OFFSET(9) & 16;
   BYTE allowRanger = EVENT_DETAILS_OFFSET(9) & 32;
   BYTE costModifier = EVENT_DETAILS_OFFSET(10);

   CString strTemp;
   GetStringAt(pStrings, text, strTemp);
   details["text"] = CStringOrEmptyString(strTemp);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["force_backup_one_step"] = backupOneStep?"1":"0";

   // RBT: hack docs dont mention that this event displays
   //      training cost in platinum instead of cost modifier
   details["cost"] = DoubleToString(ApplyCostModifier(1000.0, costModifier)); // 1000 pp is the 'normal' cost

   details["train_mage"] = allowMage?"1":"0";
   details["train_cleric"] = allowCleric?"1":"0";
   details["train_thief"] = allowThief?"1":"0";
   details["train_fighter"] = allowFighter?"1":"0";
   details["train_paladin"] = allowPaladin?"1":"0";
   details["train_ranger"] = allowRanger?"1":"0";

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("TRAIN HALL: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Tavern(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE forceBackup = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE taleOrder = EVENT_DETAILS_OFFSET(8) & 8;
   BYTE allowFights = EVENT_DETAILS_OFFSET(8) & 16;
   BYTE allowDrinks = EVENT_DETAILS_OFFSET(8) & 32;
   WORD tale1 = GetWord(9, event);
   WORD tale2 = GetWord(11, event);
   WORD tale3 = GetWord(13, event);
   WORD tale4 = GetWord(15, event);
   WORD drinkNames = GetWord(17, event);
   BYTE fightChainEvent = EVENT_DETAILS_OFFSET(19);
   BYTE drunkChainEvent = EVENT_DETAILS_OFFSET(20);

   CString strTemp;
   GetStringAt(pStrings, text, strTemp);
   details["text"] = CStringOrEmptyString(strTemp);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["force_backup_one_step"] = forceBackup?"1":"0";
   details["allow_fights"] = allowFights?"1":"0";
   details["allow_drinks"] = allowDrinks?"1":"0";
   details["chain_event_fight"] = IntToString(fightChainEvent);
   details["chain_event_drunk"] = IntToString(drunkChainEvent);
   details["chain_event_drunk_trigger_points"] = "60";
   details["tale_order"] = (taleOrder==8)?"random":"in order";

   CString strDrinkNames;
   GetStringAt(pStrings, drinkNames, strDrinkNames);

   struct SDrinkInfo
   {
      char name[50];
      int points;
   } drinks[5];

   memset(drinks, 0, sizeof(drinks));

   if (!strDrinkNames.IsEmpty())
   {
      // parse drink names
      // each drink is preceded by a ^, as in
      // ^drink1 text^drink2 text^^^
      // empty drinks have no text after ^

      char lab[50];
      char *start = (char*)strchr(strDrinkNames, '^');
      if (start!=NULL)
         start++;
      char *end = strchr(start, '^');
      int len;

      if (*start != '^')
      {
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(drinks[0].name, lab);
            drinks[0].points = 0;
         }
      }
      else end = start;

      start = end;
      start++;
      if (*start != '^')
      {
         end = strchr(start, '^');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(drinks[1].name, lab);
            drinks[1].points = 6;
         }
      }
      else end = start;

      start = end;
      start++;
      if (*start != '^')
      {
         end = strchr(start, '^');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(drinks[2].name, lab);
            drinks[2].points = 10;
         }
      }
      else end = start;

      start = end;
      start++;
      if (*start != '^')
      {
         end = strchr(start, '^');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(drinks[3].name, lab);
            drinks[3].points = 15;
         }
      }
      else end = start;

      start = end;
      start++;
      if ((*start != '^') && (*start != '\0'))
      {
         end = strchr(start, '\0');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(drinks[4].name, lab);
            drinks[4].points = 30;
         }
      }
      else end = start;
   }

   if (strlen(drinks[0].name) > 0)
   {
      details["drink_1_name"] = drinks[0].name;
      details["drink_1_points"] = IntToString(drinks[0].points);
   }
   else
   {
      details["drink_1_name"] = "";
      details["drink_1_points"] = "0";
   }

   if (strlen(drinks[1].name) > 0)
   {
      details["drink_2_name"] = drinks[1].name;
      details["drink_2_points"] = IntToString(drinks[1].points);
   }
   else
   {
      details["drink_2_name"] = "";
      details["drink_2_points"] = "0";
   }

   if (strlen(drinks[2].name) > 0)
   {
      details["drink_3_name"] = drinks[2].name;
      details["drink_3_points"] = IntToString(drinks[2].points);
   }
   else
   {
      details["drink_3_name"] = "";
      details["drink_3_points"] = "0";
   }

   if (strlen(drinks[3].name) > 0)
   {
      details["drink_4_name"] = drinks[3].name;
      details["drink_4_points"] = IntToString(drinks[3].points);
   }
   else
   {
      details["drink_4_name"] = "";
      details["drink_4_points"] = "0";
   }

   if (strlen(drinks[4].name) > 0)
   {
      details["drink_5_name"] = drinks[4].name;
      details["drink_5_points"] = IntToString(drinks[4].points);
   }
   else
   {
      details["drink_5_name"] = "";
      details["drink_5_points"] = "0";
   }

   CString strTale1;
   GetStringAt(pStrings, tale1, strTale1);
   details["tale_1"] = CStringOrEmptyString(strTale1);

   CString strTale2;
   GetStringAt(pStrings, tale2, strTale2);
   details["tale_2"] =  CStringOrEmptyString(strTale2);

   CString strTale3;
   GetStringAt(pStrings, tale3, strTale3);
   details["tale_3"] = CStringOrEmptyString(strTale3);

   CString strTale4;
   GetStringAt(pStrings, tale4, strTale4);
   details["tale_4"] =  CStringOrEmptyString(strTale4);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("TAVERN: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Shop(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   BYTE costModifier = EVENT_DETAILS_OFFSET(6);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE forceBackup = EVENT_DETAILS_OFFSET(8) & 4;
   
   BYTE item1_byte1 = EVENT_DETAILS_OFFSET(9);
   BYTE item1_byte2 = EVENT_DETAILS_OFFSET(10);
   BYTE item1_byte3 = EVENT_DETAILS_OFFSET(11);

   BYTE item2_byte1 = EVENT_DETAILS_OFFSET(12);
   BYTE item2_byte2 = EVENT_DETAILS_OFFSET(13);
   BYTE item2_byte3 = EVENT_DETAILS_OFFSET(14);

   BYTE item3_byte1 = EVENT_DETAILS_OFFSET(15);
   BYTE item3_byte2 = EVENT_DETAILS_OFFSET(16);
   BYTE item3_byte3 = EVENT_DETAILS_OFFSET(17);

   BYTE item4_byte1 = EVENT_DETAILS_OFFSET(18);
   BYTE item4_byte2 = EVENT_DETAILS_OFFSET(19);
   BYTE item4_byte3 = EVENT_DETAILS_OFFSET(20);
   
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["force_backup_one_step"] = forceBackup?"1":"0";
   details["cost_modifier"] = FRUAGetCostModifierText(costModifier);

   ShopItemListType itemList;
   AssignShopItemBytes(itemList, item1_byte1, item1_byte2, item1_byte3);
   AssignShopItemBytes(itemList, item2_byte1, item2_byte2, item2_byte3);
   AssignShopItemBytes(itemList, item3_byte1, item3_byte2, item3_byte3);
   AssignShopItemBytes(itemList, item4_byte1, item4_byte2, item4_byte3);

   int count = 0;
   Json::Value itemArray(Json::arrayValue);
   POSITION pos = itemList.GetHeadPosition();
   while (pos != NULL)
   {
      SShopItemInfo itemInfo = itemList.GetNext(pos);
      Json::Value item;

      item["name"] = CStringOrEmptyString(itemInfo.name);
      item["quantity"] = IntToString(itemInfo.quantity);
      item["id"] = IntToString(itemInfo.itemIdx);
      
      count++;
      itemArray.append(item);
   }

   details["items"] = itemArray;
   details["items_count"] = IntToString(count);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      if ( (*itCurr).isArray() )
      {

         Json::ValueIterator itItem = (*itCurr).begin();
         // if array not empty
         while (itItem != (*itCurr).end())
         {
            Json::Value::Members shopItems = (*itItem).getMemberNames();
            std::vector<std::string>::iterator itCurrArray = shopItems.begin();
            while (itCurrArray != shopItems.end())
            {
               TRACE("SHOP ITEM: %s = %s\n", (*itCurrArray).c_str(), (*itItem)[(*itCurrArray).c_str()].asCString());
               ++itCurrArray;
            }

            ++itItem;
         }
      }
      else
      {
         TRACE("SHOP: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      }
      ++itCurr;
   }
}

void FormatLevelEventJson_Temple(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   BYTE highestSpellLevel = EVENT_DETAILS_OFFSET(5);
   BYTE costModifier = EVENT_DETAILS_OFFSET(6);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE forceBackup = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE allowDonations = EVENT_DETAILS_OFFSET(8) & 8;
   DWORD donationAmountEventTrigger = GetDWord(9, event);
   BYTE donationChainEvent = EVENT_DETAILS_OFFSET(13);
   WORD text1 = GetWord(14, event);
   WORD text2 = GetWord(16, event);

   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["force_backup_one_step"] = forceBackup?"1":"0";
   details["cost_modifier"] = FRUAGetCostModifierText(costModifier);
   details["highest_spell_level_offered"] = IntToString(highestSpellLevel);
   details["allow_donations"] = allowDonations?"1":"0";
   details["chain_event_donations"] = IntToString(donationChainEvent);
   details["chain_event_donation_trigger_platinum"] = IntToString(donationAmountEventTrigger);

   CString strTemp1;
   GetStringAt(pStrings, text1, strTemp1);
   details["text_1"] = CStringOrEmptyString(strTemp1);

   CString strTemp2;
   GetStringAt(pStrings, text2, strTemp2);
   details["text_2"] = CStringOrEmptyString(strTemp2);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("TEMPLE: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_QuestionButton(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
    WORD text = GetWord(5, event);
    BYTE picture = EVENT_DETAILS_OFFSET(7);
    BYTE btn1_return = EVENT_DETAILS_OFFSET(8) & 4;
    BYTE btn2_return = EVENT_DETAILS_OFFSET(8) & 8;
    BYTE btn3_return = EVENT_DETAILS_OFFSET(8) & 16;
    BYTE btn4_return = EVENT_DETAILS_OFFSET(8) & 32;
    BYTE btn5_return = EVENT_DETAILS_OFFSET(8) & 64;
    WORD btn_labels = GetWord(9, event);
    BYTE btn1_event = EVENT_DETAILS_OFFSET(11);
    BYTE btn2_event = EVENT_DETAILS_OFFSET(12);
    BYTE btn3_event = EVENT_DETAILS_OFFSET(13);
    BYTE btn4_event = EVENT_DETAILS_OFFSET(14);
    BYTE btn5_event = EVENT_DETAILS_OFFSET(15);
    BYTE btn1_backup = EVENT_DETAILS_OFFSET(16) & 1;
    BYTE btn2_backup = EVENT_DETAILS_OFFSET(16) & 2;
    BYTE btn3_backup = EVENT_DETAILS_OFFSET(16) & 4;
    BYTE btn4_backup = EVENT_DETAILS_OFFSET(16) & 8;
    BYTE btn5_backup = EVENT_DETAILS_OFFSET(16) & 16;

    CString strText;
    GetStringAt(pStrings, text, strText);
    details["text"] = CStringOrEmptyString(strText);

    CString strButtons;
    GetStringAt(pStrings, btn_labels, strButtons);

    char buttons[5][50];
    memset(buttons, 0, sizeof(buttons));

    if (!strButtons.IsEmpty())
    {
       // parse button labels
       // each label is preceded by a ^, as in
       // ^button1 text^button2 text^^^
       // empty buttons have no text after ^

       char lab[50];
       char *start = (char*)strchr(strButtons, '^');
       if (start!=NULL)
          start++;
       char *end = strchr(start, '^');
       int len;

       if (*start != '^')
       {
          len = end-start;
          lab[0]='\0';
          if (len > 0)
          {
             strncpy(lab, start, end-start);
             lab[len]='\0';
             strcpy(buttons[0], lab);
          }
       }
       else end = start;

       start = end;
       start++;
       if (*start != '^')
       {
          end = strchr(start, '^');
          len = end-start;
          lab[0]='\0';
          if (len > 0)
          {
             strncpy(lab, start, end-start);
             lab[len]='\0';
             strcpy(buttons[1], lab);
          }
       }
       else end = start;

       start = end;
       start++;
       if (*start != '^')
       {
          end = strchr(start, '^');
          len = end-start;
          lab[0]='\0';
          if (len > 0)
          {
             strncpy(lab, start, end-start);
             lab[len]='\0';
             strcpy(buttons[2], lab);
          }
       }
       else end = start;

       start = end;
       start++;
       if (*start != '^')
       {
          end = strchr(start, '^');
          len = end-start;
          lab[0]='\0';
          if (len > 0)
          {
             strncpy(lab, start, end-start);
             lab[len]='\0';
             strcpy(buttons[3], lab);
          }
       }
       else end = start;

       start = end;
       start++;
       if ((*start != '^') && (*start != '\0'))
       {
          end = strchr(start, '\0');
          len = end-start;
          lab[0]='\0';
          if (len > 0)
          {
             strncpy(lab, start, end-start);
             lab[len]='\0';
             strcpy(buttons[4], lab);
          }
       }
       else end = start;
    }

    if (strlen(buttons[0]) > 0)
    {
       details["button_1_label"] = CStringOrEmptyString(buttons[0]);
       details["button_1_present"] = "1";
    }
    else
    {
       details["button_1_label"] = "";
       details["button_1_present"] = "0";
    }

    if (strlen(buttons[1]) > 0)
    {
       details["button_2_label"] = CStringOrEmptyString(buttons[1]);
       details["button_2_present"] = "1";
    }
    else
    {
       details["button_2_label"] = "";
       details["button_2_present"] = "0";
    }

    if (strlen(buttons[2]) > 0)
    {
       details["button_3_label"] = CStringOrEmptyString(buttons[2]);
       details["button_3_present"] = "1";
    }
    else
    {
       details["button_3_label"] = "";
       details["button_3_present"] = "0";
    }

    if (strlen(buttons[3]) > 0)
    {
       details["button_4_label"] = CStringOrEmptyString(buttons[3]);
       details["button_4_present"] = "1";
    }
    else
    {
       details["button_4_label"] = "";
       details["button_4_present"] = "0";
    }

    if (strlen(buttons[4]) > 0)
    {
       details["button_5_label"] = CStringOrEmptyString(buttons[4]);
       details["button_5_present"] = "1";
    }
    else
    {
       details["button_5_label"] = "";
       details["button_5_present"] = "0";
    }

    details["picture"] = FRUAGetBigSmallSpritePicText(picture);
    details["button_1_force_backup_one_step"] = btn1_backup?"1":"0";
    details["button_2_force_backup_one_step"] = btn2_backup?"1":"0";
    details["button_3_force_backup_one_step"] = btn3_backup?"1":"0";
    details["button_4_force_backup_one_step"] = btn4_backup?"1":"0";
    details["button_5_force_backup_one_step"] = btn5_backup?"1":"0";

    details["button_1_return_to_questions"] = btn1_return?"1":"0";
    details["button_2_return_to_questions"] = btn2_return?"1":"0";
    details["button_3_return_to_questions"] = btn3_return?"1":"0";
    details["button_4_return_to_questions"] = btn4_return?"1":"0";
    details["button_5_return_to_questions"] = btn5_return?"1":"0";

    details["button_1_chain_event"] = IntToString(btn1_event);
    details["button_2_chain_event"] = IntToString(btn2_event);
    details["button_3_chain_event"] = IntToString(btn3_event);
    details["button_4_chain_event"] = IntToString(btn4_event);
    details["button_5_chain_event"] = IntToString(btn5_event);

    Json::ValueIterator itCurr = details.begin();
    while (itCurr != details.end())
    {
       TRACE("QUESTION BTN: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
       ++itCurr;
    }
}

void FormatLevelEventJson_TransferModule(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text1 = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE transferWhen = EVENT_DETAILS_OFFSET(8) & 16;
   BYTE askYesNo = EVENT_DETAILS_OFFSET(8) & 32;
   BYTE transferOn = EVENT_DETAILS_OFFSET(8) & 64;
   BYTE destFace = EVENT_DETAILS_OFFSET(8) & 12;
   BYTE destY = EVENT_DETAILS_OFFSET(9);
   BYTE destX = EVENT_DETAILS_OFFSET(10);
   WORD text2 = GetWord(11, event);
   BYTE destPlacement = EVENT_DETAILS_OFFSET(13) & 1;
   BYTE execDestEvent = EVENT_DETAILS_OFFSET(13) & 4;
   BYTE destroyDrow = EVENT_DETAILS_OFFSET(13) & 8;
   BYTE entryPoint = EVENT_DETAILS_OFFSET(14);
   BYTE destLevel = EVENT_DETAILS_OFFSET(15);

   CString strText1;
   GetStringAt(pStrings, text1, strText1);
   details["text_1"] = CStringOrEmptyString(strText1);

   CString strText2;
   GetStringAt(pStrings, text2, strText2);
   details["text_2"] = CStringOrEmptyString(strText2);

   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["transfer_when"] = (transferWhen==16)?"party in cell":"party attempts to enter cell";
   details["destroy_drow_equipment_after_transfer"] = destroyDrow?"1":"0";
   details["ask_yes_no"] = askYesNo?"1":"0";
   details["transfer_on_yes_no"] = (transferOn==64)?"no":"yes";
   details["dest_x"] = IntToString(destX);
   details["dest_y"] = IntToString(destY);
   details["dest_entry_point"] = IntToString(entryPoint);
   details["dest_type"] = (destPlacement==1)?"entry point":"xy";
   details["dest_event_execute"] = execDestEvent?"1":"0";   
   details["dest_level"] = IntToString(destLevel);

   switch (destFace)
   {
   case 0: details["dest_facing"] = "north"; break;
   case 4: details["dest_facing"] = "east"; break;
   case 8: details["dest_facing"] = "south"; break;
   case 12: details["dest_facing"] = "west"; break;
   default: details["dest_facing"] = UnknownString; break;
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("TRANSFER MODULE: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_GuidedTour(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   const int NUM_STEPS = 24;
   BYTE stepActions[NUM_STEPS];

   BYTE startY = EVENT_DETAILS_OFFSET(5);
   BYTE startX = EVENT_DETAILS_OFFSET(6);
   BYTE numSteps = EVENT_DETAILS_OFFSET(7);
   BYTE useStartLoc = EVENT_DETAILS_OFFSET(8) & 16;
   BYTE execDestEvent = EVENT_DETAILS_OFFSET(8) & 32;
   BYTE startFace = EVENT_DETAILS_OFFSET(8) & 12;

   stepActions[0] = EVENT_DETAILS_OFFSET(9) & 3;
   stepActions[1] = EVENT_DETAILS_OFFSET(9) & 12;
   stepActions[2] = EVENT_DETAILS_OFFSET(9) & 48;
   stepActions[3] = EVENT_DETAILS_OFFSET(9) & 192;

   stepActions[4] = EVENT_DETAILS_OFFSET(10) & 3;
   stepActions[5] = EVENT_DETAILS_OFFSET(10) & 12;
   stepActions[6] = EVENT_DETAILS_OFFSET(10) & 48;
   stepActions[7] = EVENT_DETAILS_OFFSET(10) & 192;

   stepActions[8] = EVENT_DETAILS_OFFSET(11) & 3;
   stepActions[9] = EVENT_DETAILS_OFFSET(11) & 12;
   stepActions[10] = EVENT_DETAILS_OFFSET(11) & 48;
   stepActions[11] = EVENT_DETAILS_OFFSET(11) & 192;

   stepActions[12] = EVENT_DETAILS_OFFSET(12) & 3;
   stepActions[13] = EVENT_DETAILS_OFFSET(12) & 12;
   stepActions[14] = EVENT_DETAILS_OFFSET(12) & 48;
   stepActions[15] = EVENT_DETAILS_OFFSET(12) & 192;

   stepActions[16] = EVENT_DETAILS_OFFSET(13) & 3;
   stepActions[17] = EVENT_DETAILS_OFFSET(13) & 12;
   stepActions[18] = EVENT_DETAILS_OFFSET(13) & 48;
   stepActions[19] = EVENT_DETAILS_OFFSET(13) & 192;

   stepActions[20] = EVENT_DETAILS_OFFSET(14) & 3;
   stepActions[21] = EVENT_DETAILS_OFFSET(14) & 12;
   stepActions[22] = EVENT_DETAILS_OFFSET(14) & 48;
   stepActions[23] = EVENT_DETAILS_OFFSET(14) & 192;

   BYTE displayText1BeforeStep = EVENT_DETAILS_OFFSET(15)+1;
   BYTE displayText2BeforeStep = EVENT_DETAILS_OFFSET(16)+1;
   WORD text1 = GetWord(17, event);
   WORD text2 = GetWord(19, event);

   details["start_loc_use"] = useStartLoc?"1":"0";
   details["start_loc_x"] = IntToString(startX);
   details["start_loc_y"] = IntToString(startY);
   switch (startFace)
   {
   case  0: details["start_facing"] = "north"; break;
   case  4: details["start_facing"] = "east"; break;
   case  8: details["start_facing"] = "south"; break;
   case 12: details["start_facing"] = "west"; break;
   default: details["start_facing"] = UnknownString; break;
   }

   details["dest_event_execute"] = execDestEvent?"1":"0";   
   
   CString strText1;
   GetStringAt(pStrings, text1, strText1);
   details["text_1"] = CStringOrEmptyString(strText1);
   details["text_1_displayed_before_step"] = IntToString(displayText1BeforeStep);

   CString strText2;
   GetStringAt(pStrings, text2, strText2);
   details["text_2"] = CStringOrEmptyString(strText2);
   details["text_2_displayed_before_step"] = IntToString(displayText2BeforeStep);  
   
   ASSERT( numSteps <= NUM_STEPS );
   details["num_steps"] = IntToString(numSteps);

   for (int i=0; i<NUM_STEPS; i++)
   {
      CString strKey;
      strKey.Format("step_%02i_action", i+1);
      
      if (i < numSteps)
      {
         details[ (const char*)strKey ] = GetGuidedTourStepActionText( stepActions[i] );
      }
      else
      {
         details[ (const char*)strKey ] = NoneString;
      }
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("GUIDED TOUR: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_AddNPC(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE picDistance = EVENT_DETAILS_OFFSET(8) & 3;
   BYTE npc = EVENT_DETAILS_OFFSET(9);
   BYTE pcntOfMaxHp = EVENT_DETAILS_OFFSET(10);

   CString strText;
   GetStringAt(pStrings, text, strText);

   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText( picture );
   details["npc"] = FRUAGetMonsterNameText( npc );
   details["npc_added_with_pcnt_of_max_hitpoints"] = IntToString(pcntOfMaxHp);

   switch (picDistance)
   {
   case 0: details["picture_distance"] = "close"; break;
   case 1: details["picture_distance"] = "nearby"; break;
   case 2: details["picture_distance"] = "far away"; break;
   default: details["picture_distance"] = UnknownString; break;
   }   

   /*
   TRACE("ADD NPC: Text %s\n", details["text"].asCString());
   TRACE("ADD NPC: Picture %s\n", details["picture"].asCString());
   TRACE("ADD NPC: PicDistance %s\n", details["picture_distance"].asCString());
   TRACE("ADD NPC: NPC %s\n", details["npc"].asCString());
   TRACE("ADD NPC: NPC PCNT HP %s\n", details["npc_added_with_pcnt_of_max_hitpoints"].asCString());
   */
   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("ADD NPC: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_NPCSays(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   BYTE text1_pressReturn = EVENT_DETAILS_OFFSET(5) & 1;
   BYTE text2_pressReturn = EVENT_DETAILS_OFFSET(5) & 2;
   BYTE text3_pressReturn = EVENT_DETAILS_OFFSET(5) & 4;
   BYTE text4_pressReturn = EVENT_DETAILS_OFFSET(5) & 8;
   BYTE text5_pressReturn = EVENT_DETAILS_OFFSET(5) & 16;
   BYTE npc = EVENT_DETAILS_OFFSET(6);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE picDistance = EVENT_DETAILS_OFFSET(8) & 3;
   BYTE text1_highlight = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE text2_highlight = EVENT_DETAILS_OFFSET(8) & 8;
   BYTE text3_highlight = EVENT_DETAILS_OFFSET(8) & 16;
   BYTE text4_highlight = EVENT_DETAILS_OFFSET(8) & 32;
   BYTE text5_highlight = EVENT_DETAILS_OFFSET(8) & 64;
   WORD text1 = GetWord(9, event);
   WORD text2 = GetWord(11, event);
   WORD text3 = GetWord(13, event);
   WORD text4 = GetWord(15, event);
   WORD text5 = GetWord(17, event);
   BYTE sound = EVENT_DETAILS_OFFSET(19);

   CString strText1;
   GetStringAt(pStrings, text1, strText1);
   details["text_1"] = CStringOrEmptyString(strText1);

   CString strText2;
   GetStringAt(pStrings, text2, strText2);
   details["text_2"] = CStringOrEmptyString(strText2);

   CString strText3;
   GetStringAt(pStrings, text3, strText3);
   details["text_3"] = CStringOrEmptyString(strText3);

   CString strText4;
   GetStringAt(pStrings, text4, strText4);
   details["text_4"] = CStringOrEmptyString(strText4);

   CString strText5;
   GetStringAt(pStrings, text5, strText5);
   details["text_5"] = CStringOrEmptyString(strText5);

   details["text_1_highlight"] = text1_highlight?"1":"0";
   details["text_2_highlight"] = text2_highlight?"1":"0";
   details["text_3_highlight"] = text3_highlight?"1":"0";
   details["text_4_highlight"] = text4_highlight?"1":"0";
   details["text_5_highlight"] = text5_highlight?"1":"0";

   details["text_1_press_return"] = text1_pressReturn?"1":"0";
   details["text_2_press_return"] = text2_pressReturn?"1":"0";
   details["text_3_press_return"] = text3_pressReturn?"1":"0";
   details["text_4_press_return"] = text4_pressReturn?"1":"0";
   details["text_5_press_return"] = text5_pressReturn?"1":"0";

   details["picture"] = FRUAGetBigSmallSpritePicText( picture );
   details["if_npc_in_party"] = FRUAGetMonsterNameText( npc );

   switch (picDistance)
   {
   case 0: details["picture_distance"] = "close"; break;
   case 1: details["picture_distance"] = "nearby"; break;
   case 2: details["picture_distance"] = "far away"; break;
   default: details["picture_distance"] = UnknownString; break;
   }   

   details["sound"] = FRUAGetSoundNameText( sound );

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("NPC SAYS: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Encounter(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   const char *BtnActionText[] = 
   {
      "decrease range",
      "combat - party surprised if slow party",
      "combat - monsters surprised if fast party",
      "combat - no surprise",
      "talk chain",
      "escape if fast party else combat",
      "no effect"
   };

   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE picDistance = EVENT_DETAILS_OFFSET(8) & 3;
   BYTE monsterSpeed = EVENT_DETAILS_OFFSET(9);

   BYTE appBtn_present = EVENT_DETAILS_OFFSET(10) & 8;
   BYTE appBtn_notAllowedClose = EVENT_DETAILS_OFFSET(10) & 16;
   BYTE appBtn_action = EVENT_DETAILS_OFFSET(10) & 7;

   BYTE retreatBtn_present = EVENT_DETAILS_OFFSET(11) & 8;
   BYTE retreatBtn_action = EVENT_DETAILS_OFFSET(11) & 7;

   BYTE fightBtn_present = EVENT_DETAILS_OFFSET(12) & 8;
   BYTE fightBtn_allowOnlyClose = EVENT_DETAILS_OFFSET(12) & 16;
   BYTE fightBtn_action = EVENT_DETAILS_OFFSET(12) & 7;

   BYTE waitBtn_present = EVENT_DETAILS_OFFSET(13) & 8;
   BYTE waitBtn_action = EVENT_DETAILS_OFFSET(13) & 7;

   BYTE talkBtn_present = EVENT_DETAILS_OFFSET(14) & 8;
   BYTE talkBtn_allowOnlyClose = EVENT_DETAILS_OFFSET(14) & 16;
   BYTE talkBtn_action = EVENT_DETAILS_OFFSET(14) & 7;

   BYTE zeroRangeAction = EVENT_DETAILS_OFFSET(15);
   BYTE combatEvent = EVENT_DETAILS_OFFSET(18);
   BYTE talkEvent = EVENT_DETAILS_OFFSET(19);
   BYTE escapeEvent = EVENT_DETAILS_OFFSET(20);

   details["button_1_label"] = "APPROACH";
   details["button_1_present"] = appBtn_present?"1":"0";
   details["button_1_allow_close"] = appBtn_notAllowedClose?"0":"1";
   details["button_1_allow_only_close"] = "0";
   details["button_1_action"] = BtnActionText[appBtn_action];

   details["button_2_label"] = "RETREAT";
   details["button_2_present"] = retreatBtn_present?"1":"0";
   details["button_2_allow_close"] = "1";
   details["button_2_allow_only_close"] = "0";
   details["button_2_action"] = BtnActionText[retreatBtn_action];

   details["button_3_label"] = "FIGHT";
   details["button_3_present"] = fightBtn_present?"1":"0";
   details["button_3_allow_close"] = "1";
   details["button_3_allow_only_close"] = fightBtn_allowOnlyClose?"1":"0";
   details["button_3_action"] = BtnActionText[fightBtn_action];

   details["button_4_label"] = "WAIT";
   details["button_4_present"] = waitBtn_present?"1":"0";
   details["button_4_allow_close"] = "1";
   details["button_4_allow_only_close"] = "0";
   details["button_4_action"] = BtnActionText[waitBtn_action];

   details["button_5_label"] = "TALK";
   details["button_5_present"] = talkBtn_present?"1":"0";
   details["button_5_allow_close"] = "1";
   details["button_5_allow_only_close"] = talkBtn_allowOnlyClose?"1":"0";
   details["button_5_action"] = BtnActionText[talkBtn_action];

   CString strText1;
   GetStringAt(pStrings, text, strText1);
   details["text"] = CStringOrEmptyString(strText1);

   details["picture"] = FRUAGetBigSmallSpritePicText( picture );
   switch (picDistance)
   {
   case 0: details["picture_distance"] = "close"; break;
   case 1: details["picture_distance"] = "nearby"; break;
   case 2: details["picture_distance"] = "far away"; break;
   default: details["picture_distance"] = UnknownString; break;
   }   

   details["monster_speed"] = IntToString(monsterSpeed);
   details["action_if_range_zero"] = BtnActionText[zeroRangeAction];

   details["combat_event"] = IntToString(combatEvent);
   details["talk_event"] = IntToString(talkEvent);
   details["escape_event"] = IntToString(escapeEvent);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("ENCOUNTER: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Utilities(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   BYTE mathOp = EVENT_DETAILS_OFFSET(5) & 3;
   BYTE multiItemCheck = EVENT_DETAILS_OFFSET(5) & 12;
   BYTE exitGame = EVENT_DETAILS_OFFSET(5) & 16;
   BYTE mathTarget = EVENT_DETAILS_OFFSET(6);
   BYTE mathValue = EVENT_DETAILS_OFFSET(7);
   BYTE multiItem1 = EVENT_DETAILS_OFFSET(8);
   BYTE multiItem2 = EVENT_DETAILS_OFFSET(9);
   BYTE multiItem3 = EVENT_DETAILS_OFFSET(10);
   BYTE multiItem4 = EVENT_DETAILS_OFFSET(11);
   BYTE multiItem5 = EVENT_DETAILS_OFFSET(12);
   BYTE multiItem6 = EVENT_DETAILS_OFFSET(13);
   BYTE multiItemCheckTarget = EVENT_DETAILS_OFFSET(14);

   // single operation details
   switch (mathOp)
   {
   case 0: details["single_item_operation"] = NoneString; break;
   case 1: details["single_item_operation"] = "store in"; break;
   case 2: details["single_item_operation"] = "add to"; break;
   case 3: details["single_item_operation"] = "subtract from"; break;
   default: details["single_item_operation"] = UnknownString; break;
   }
   details["single_item_target"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(mathTarget));
   details["single_item_operand"] = IntToString(mathValue);

   // multi item operation details
   switch (multiItemCheck)
   {
   case 0: details["multi_item_check"] = NoneString; break;
   case 4: details["multi_item_check"] = "all present"; break;
   case 8: details["multi_item_check"] = "one or more present"; break;
   default: details["multi_item_check"] = UnknownString; break;
   }
   details["multi_item_target"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(multiItemCheckTarget));
   details["multi_item_operand"] = "1"; // not specified in the event, implied 1?
   details["multi_item_1"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(multiItem1));
   details["multi_item_2"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(multiItem2));
   details["multi_item_3"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(multiItem3));
   details["multi_item_4"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(multiItem4));
   details["multi_item_5"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(multiItem5));
   details["multi_item_6"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(multiItem6));

   details["force_game_exit"] = exitGame?"1":"0";

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("UTILITIES: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Sounds(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   BYTE sound1 = EVENT_DETAILS_OFFSET(5);
   BYTE sound2 = EVENT_DETAILS_OFFSET(6);
   BYTE sound3 = EVENT_DETAILS_OFFSET(7);
   BYTE sound4 = EVENT_DETAILS_OFFSET(8);
   BYTE sound5 = EVENT_DETAILS_OFFSET(9);
   BYTE sound6 = EVENT_DETAILS_OFFSET(10);
   BYTE sound7 = EVENT_DETAILS_OFFSET(11);
   BYTE sound8 = EVENT_DETAILS_OFFSET(12);
   BYTE sound9 = EVENT_DETAILS_OFFSET(13);
   BYTE sound10 = EVENT_DETAILS_OFFSET(14);

   details["sound_01"] = FRUAGetSoundNameText(sound1);
   details["sound_02"] = FRUAGetSoundNameText(sound2);
   details["sound_03"] = FRUAGetSoundNameText(sound3);
   details["sound_04"] = FRUAGetSoundNameText(sound4);
   details["sound_05"] = FRUAGetSoundNameText(sound5);
   details["sound_06"] = FRUAGetSoundNameText(sound6);
   details["sound_07"] = FRUAGetSoundNameText(sound7);
   details["sound_08"] = FRUAGetSoundNameText(sound8);
   details["sound_09"] = FRUAGetSoundNameText(sound9);
   details["sound_10"] = FRUAGetSoundNameText(sound10);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("SOUNDS: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_WhoTries(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE checkOperation = EVENT_DETAILS_OFFSET(8) & 0x3F;
   BYTE checkRelation = EVENT_DETAILS_OFFSET(8) & 64;
   BYTE checkValue = EVENT_DETAILS_OFFSET(9);
   BYTE numTries = EVENT_DETAILS_OFFSET(10);
   BYTE onFailAction = EVENT_DETAILS_OFFSET(11) & 3;
   BYTE onSuccessAction = EVENT_DETAILS_OFFSET(11) & 12;   
   BYTE destFace = EVENT_DETAILS_OFFSET(11) & 48;
   BYTE execDestEvent = EVENT_DETAILS_OFFSET(11) & 64;
   WORD failText = GetWord(12, event);
   WORD successText = GetWord(14, event);
   BYTE failEvent = EVENT_DETAILS_OFFSET(16);
   BYTE successEvent = EVENT_DETAILS_OFFSET(17);
   BYTE teleportX = EVENT_DETAILS_OFFSET(18);
   BYTE teleportY = EVENT_DETAILS_OFFSET(19);
   BYTE minCharacterStrengthBonus = EVENT_DETAILS_OFFSET(20);

   CString strText;
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   switch (checkOperation)
   {
   case 0: details["check_operation"] = "always succeed"; break;
   case 4: details["check_operation"] = "always fail"; break;
   case 8: details["check_operation"] = "compare strength"; break;
   case 12: details["check_operation"] = "compare intelligence"; break;
   case 16: details["check_operation"] = "compare wisdom"; break;
   case 20: details["check_operation"] = "compare dexterity"; break;
   case 24: details["check_operation"] = "compare constitution"; break;
   case 28: details["check_operation"] = "compare charisma"; break;
   case 32: details["check_operation"] = "compare thief pick pocket"; break;
   case 36: details["check_operation"] = "compare thief open lock"; break;
   case 40: details["check_operation"] = "compare thief find remove trap"; break;
   case 44: details["check_operation"] = "compare thief move silently"; break;
   case 48: details["check_operation"] = "compare thief hide in shadow"; break;
   case 52: details["check_operation"] = "compare thief hear noise"; break;
   case 56: details["check_operation"] = "compare thief climb wall"; break;
   case 60: details["check_operation"] = "compare thief read language"; break;
   default: details["check_operation"] = UnknownString; break;
   }

   switch (checkRelation)
   {
   case 0: details["check_relation"] = "compare to die roll"; break;
   case 64: details["check_relation"] = "compare to value"; break;
   default: details["check_relation"] = UnknownString; break;
   }

   details["check_value"] = IntToString(checkValue);
   // if check operation should check strength, then also compare the strength bonus
   details["check_value_strength_bonus_pcnt"] = IntToString(minCharacterStrengthBonus);

   details["num_attempts_allowed"] = IntToString(numTries);

   switch (onSuccessAction)
   {
   case 0: details["success_action"] = NoneString; break;
   case 4: details["success_action"] = "chain event"; break;
   case 8: details["success_action"] = "teleport"; break;
   case 12: details["success_action"] = "backup one step"; break;
   default: details["success_action"] = UnknownString; break;
   }

   switch (onFailAction)
   {
   case 0: details["fail_action"] = NoneString; break;
   case 1: details["fail_action"] = "chain event"; break;
   case 2: details["fail_action"] = "teleport"; break;
   case 3: details["fail_action"] = "backup one step"; break;
   default: details["fail_action"] = UnknownString; break;
   }

   switch (destFace)
   {
   case 0: details["teleport_dest_facing"] = "north"; break;
   case 16: details["teleport_dest_facing"] = "east"; break;
   case 32: details["teleport_dest_facing"] = "south"; break;
   case 48: details["teleport_dest_facing"] = "west"; break;
   default: details["teleport_dest_facing"] = UnknownString; break;
   }

   details["teleport_dest_event_execute"] = execDestEvent?"1":"0";   
   details["teleport_x"] = IntToString(teleportX);
   details["teleport_y"] = IntToString(teleportY);

   details["success_event"] = IntToString(successEvent);
   details["fail_event"] = IntToString(failEvent);

   CString strSuccess;
   GetStringAt(pStrings, successText, strSuccess);
   details["success_text"] = CStringOrEmptyString(strSuccess);

   CString strFail;
   GetStringAt(pStrings, failText, strFail);
   details["fail_text"] = CStringOrEmptyString(strFail);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("WHO TRIES: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_WhoPays(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE successType = EVENT_DETAILS_OFFSET(8) & 12;
   WORD successAmount = GetWord(9, event);
   BYTE failAction = EVENT_DETAILS_OFFSET(11) & 3;
   BYTE successAction = EVENT_DETAILS_OFFSET(11) & 12;   
   BYTE destFace = EVENT_DETAILS_OFFSET(11) & 48;
   BYTE execDestEvent = EVENT_DETAILS_OFFSET(11) & 64;
   WORD failText = GetWord(12, event);
   WORD successText = GetWord(14, event);
   BYTE failEvent = EVENT_DETAILS_OFFSET(16);
   BYTE successEvent = EVENT_DETAILS_OFFSET(17);
   BYTE teleportX = EVENT_DETAILS_OFFSET(18);
   BYTE teleportY = EVENT_DETAILS_OFFSET(19);

   CString strText;
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   switch (successType)
   {
   case 0: details["success_check"] = "impossible"; break;
   case 4: details["success_check"] = "sufficient gems"; break;
   case 8: details["success_check"] = "sufficient jewelry"; break;
   case 12: details["success_check"] = "sufficient platinum"; break;
   default: details["success_check"] = UnknownString; break;
   }
   details["success_amount"] = IntToString(successAmount);

   switch (successAction)
   {
   case  0: details["success_action"] = NoneString; break;
   case  4: details["success_action"] = "chain event"; break;
   case  8: details["success_action"] = "teleport"; break;
   case 12: details["success_action"] = "backup one step"; break;
   default: details["success_action"] = UnknownString; break;
   }

   switch (failAction)
   {
   case 0: details["fail_action"] = NoneString; break;
   case 1: details["fail_action"] = "chain event"; break;
   case 2: details["fail_action"] = "teleport"; break;
   case 3: details["fail_action"] = "backup one step"; break;
   default: details["fail_action"] = UnknownString; break;
   }

   switch (destFace)
   {
   case 0: details["teleport_dest_facing"] = "north"; break;
   case 16: details["teleport_dest_facing"] = "east"; break;
   case 32: details["teleport_dest_facing"] = "south"; break;
   case 48: details["teleport_dest_facing"] = "west"; break;
   default: details["teleport_dest_facing"] = UnknownString; break;
   }

   details["teleport_dest_event_execute"] = execDestEvent?"1":"0";   
   details["teleport_x"] = IntToString(teleportX);
   details["teleport_y"] = IntToString(teleportY);

   details["success_event"] = IntToString(successEvent);
   details["fail_event"] = IntToString(failEvent);

   CString strSuccess;
   GetStringAt(pStrings, successText, strSuccess);
   details["success_text"] = CStringOrEmptyString(strSuccess);

   CString strFail;
   GetStringAt(pStrings, failText, strFail);
   details["fail_text"] = CStringOrEmptyString(strFail);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("WHO PAYS: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Password(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   WORD password = GetWord(9, event);
   BYTE successAction = EVENT_DETAILS_OFFSET(11) & 12;
   BYTE failAction = EVENT_DETAILS_OFFSET(11) & 3;
   BYTE execDestEvent = EVENT_DETAILS_OFFSET(11) & 64;
   BYTE destFace = EVENT_DETAILS_OFFSET(11) & 48;
   WORD failText = GetWord(12, event);
   WORD successText = GetWord(14, event);
   BYTE failEvent = EVENT_DETAILS_OFFSET(16);
   BYTE successEvent = EVENT_DETAILS_OFFSET(17);
   BYTE teleportX = EVENT_DETAILS_OFFSET(18);
   BYTE teleportY = EVENT_DETAILS_OFFSET(19);
   BYTE numTries = EVENT_DETAILS_OFFSET(20);

   CString strText;
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   
   CString strPswd;
   GetStringAt(pStrings, password, strPswd);
   details["password"] = CStringOrEmptyString(strPswd);

   switch (successAction)
   {
   case  0: details["success_action"] = NoneString; break;
   case  4: details["success_action"] = "chain event"; break;
   case  8: details["success_action"] = "teleport"; break;
   case 12: details["success_action"] = "backup one step"; break;
   default: details["success_action"] = UnknownString; break;
   }

   switch (failAction)
   {
   case 0: details["fail_action"] = NoneString; break;
   case 1: details["fail_action"] = "chain event"; break;
   case 2: details["fail_action"] = "teleport"; break;
   case 3: details["fail_action"] = "backup one step"; break;
   default: details["fail_action"] = UnknownString; break;
   }

   switch (destFace)
   {
   case 0: details["teleport_dest_facing"] = "north"; break;
   case 16: details["teleport_dest_facing"] = "east"; break;
   case 32: details["teleport_dest_facing"] = "south"; break;
   case 48: details["teleport_dest_facing"] = "west"; break;
   default: details["teleport_dest_facing"] = UnknownString; break;
   }

   details["teleport_dest_event_execute"] = execDestEvent?"1":"0";   
   details["teleport_x"] = IntToString(teleportX);
   details["teleport_y"] = IntToString(teleportY);

   details["success_event"] = IntToString(successEvent);
   details["fail_event"] = IntToString(failEvent);

   CString strSuccess;
   GetStringAt(pStrings, successText, strSuccess);
   details["success_text"] = CStringOrEmptyString(strSuccess);

   CString strFail;
   GetStringAt(pStrings, failText, strFail);
   details["fail_text"] = CStringOrEmptyString(strFail);

   details["num_attempts_allowed"] = IntToString(numTries);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("PASSWORD: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_QuestionList(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);   
   BYTE q1_return = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE q2_return = EVENT_DETAILS_OFFSET(8) & 8;
   BYTE q3_return = EVENT_DETAILS_OFFSET(8) & 16;
   BYTE q4_return = EVENT_DETAILS_OFFSET(8) & 32;
   BYTE q5_return = EVENT_DETAILS_OFFSET(8) & 64;
   WORD answers = GetWord(9, event);
   BYTE q1_event = EVENT_DETAILS_OFFSET(11);
   BYTE q2_event = EVENT_DETAILS_OFFSET(12);
   BYTE q3_event = EVENT_DETAILS_OFFSET(13);
   BYTE q4_event = EVENT_DETAILS_OFFSET(14);
   BYTE q5_event = EVENT_DETAILS_OFFSET(15);
   BYTE q1_backup = EVENT_DETAILS_OFFSET(16) & 1;
   BYTE q2_backup = EVENT_DETAILS_OFFSET(16) & 2;
   BYTE q3_backup = EVENT_DETAILS_OFFSET(16) & 4;
   BYTE q4_backup = EVENT_DETAILS_OFFSET(16) & 8;
   BYTE q5_backup = EVENT_DETAILS_OFFSET(16) & 16;

   CString strText; // this is the question asked
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   details["answer_1_force_backup_one_step"] = q1_backup?"1":"0";
   details["answer_2_force_backup_one_step"] = q2_backup?"1":"0";
   details["answer_3_force_backup_one_step"] = q3_backup?"1":"0";
   details["answer_4_force_backup_one_step"] = q4_backup?"1":"0";
   details["answer_5_force_backup_one_step"] = q5_backup?"1":"0";

   details["answer_1_return_to_question"] = q1_return?"1":"0";
   details["answer_2_return_to_question"] = q2_return?"1":"0";
   details["answer_3_return_to_question"] = q3_return?"1":"0";
   details["answer_4_return_to_question"] = q4_return?"1":"0";
   details["answer_5_return_to_question"] = q5_return?"1":"0";

   details["answer_1_chain_event"] = IntToString(q1_event);
   details["answer_2_chain_event"] = IntToString(q2_event);
   details["answer_3_chain_event"] = IntToString(q3_event);
   details["answer_4_chain_event"] = IntToString(q4_event);
   details["answer_5_chain_event"] = IntToString(q5_event);

   CString strAnswers;
   GetStringAt(pStrings, answers, strAnswers); // list of answers presented to player

   char buttons[5][50];
   memset(buttons, 0, sizeof(buttons));

   if (!strAnswers.IsEmpty())
   {
      // parse button labels
      // each label is preceded by a ^, as in
      // ^button1 text^button2 text^^^
      // empty buttons have no text after ^

      char lab[50];
      char *start = (char*)strchr(strAnswers, '^');
      if (start!=NULL)
         start++;
      char *end = strchr(start, '^');
      int len;

      if (*start != '^')
      {
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(buttons[0], lab);
         }
      }
      else end = start;

      start = end;
      start++;
      if (*start != '^')
      {
         end = strchr(start, '^');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(buttons[1], lab);
         }
      }
      else end = start;

      start = end;
      start++;
      if (*start != '^')
      {
         end = strchr(start, '^');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(buttons[2], lab);
         }
      }
      else end = start;

      start = end;
      start++;
      if (*start != '^')
      {
         end = strchr(start, '^');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(buttons[3], lab);
         }
      }
      else end = start;

      start = end;
      start++;
      if ((*start != '^') && (*start != '\0'))
      {
         end = strchr(start, '\0');
         len = end-start;
         lab[0]='\0';
         if (len > 0)
         {
            strncpy(lab, start, end-start);
            lab[len]='\0';
            strcpy(buttons[4], lab);
         }
      }
      else end = start;
   }

   if (strlen(buttons[0]) > 0)
      details["answer_1"] = CStringOrEmptyString(buttons[0]);
   else
      details["answer_1"] = "";

   if (strlen(buttons[1]) > 0)
      details["answer_2"] = CStringOrEmptyString(buttons[1]);
   else
      details["answer_2"] = "";

   if (strlen(buttons[2]) > 0)
      details["answer_3"] = CStringOrEmptyString(buttons[2]);
   else
      details["answer_3"] = "";

   if (strlen(buttons[3]) > 0)
      details["answer_4"] = CStringOrEmptyString(buttons[3]);
   else
      details["answer_4"] = "";

   if (strlen(buttons[4]) > 0)
      details["answer_5"] = CStringOrEmptyString(buttons[4]);
   else
      details["answer_5"] = "";

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("QUESTION LIST: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_SmallTown(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE hasTemple = EVENT_DETAILS_OFFSET(8) & 1;
   BYTE hasTrainHall = EVENT_DETAILS_OFFSET(8) & 2;
   BYTE hasShop = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE hasInn = EVENT_DETAILS_OFFSET(8) & 8;
   BYTE hasTavern = EVENT_DETAILS_OFFSET(8) & 16;
   BYTE hasVault = EVENT_DETAILS_OFFSET(8) & 32;
   BYTE mageTrain = EVENT_DETAILS_OFFSET(9) & 1;
   BYTE clericTrain = EVENT_DETAILS_OFFSET(9) & 2;
   BYTE thiefTrain = EVENT_DETAILS_OFFSET(9) & 4;
   BYTE fighterTrain = EVENT_DETAILS_OFFSET(9) & 8;
   BYTE paladinTrain = EVENT_DETAILS_OFFSET(9) & 16;
   BYTE rangerTrain = EVENT_DETAILS_OFFSET(9) & 32;
   BYTE shopCostModifier = EVENT_DETAILS_OFFSET(10) & 0x1F;
   BYTE highestLevelTempleSpell = EVENT_DETAILS_OFFSET(10) & 0xE0;
   WORD tavernTale = GetWord(11, event);
   BYTE trainTavernCostModifier = EVENT_DETAILS_OFFSET(13);
   BYTE item1_byte1 = EVENT_DETAILS_OFFSET(15);
   BYTE item1_byte2 = EVENT_DETAILS_OFFSET(16);
   BYTE item1_byte3 = EVENT_DETAILS_OFFSET(17);
   BYTE item2_byte1 = EVENT_DETAILS_OFFSET(18);
   BYTE item2_byte2 = EVENT_DETAILS_OFFSET(19);
   BYTE item2_byte3 = EVENT_DETAILS_OFFSET(20);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   details["has_temple"] = hasTemple?"1":"0";
   details["has_traininghall"] = hasTrainHall?"1":"0";
   details["has_shop"] = hasShop?"1":"0";
   details["has_inn"] = hasInn?"1":"0";
   details["has_tavern"] = hasTavern?"1":"0";
   details["has_vault"] = hasVault?"1":"0";

   details["can_mage_train"] = mageTrain?"1":"0";
   details["can_cleric_train"] = clericTrain?"1":"0";
   details["can_thief_train"] = thiefTrain?"1":"0";
   details["can_fighter_train"] = fighterTrain?"1":"0";
   details["can_paladin_train"] = paladinTrain?"1":"0";
   details["can_ranger_train"] = rangerTrain?"1":"0";

   switch (highestLevelTempleSpell)
   {
   case  32: details["highest_spell_level_offered"] = "1"; break;
   case  64: details["highest_spell_level_offered"] = "2"; break;
   case  96: details["highest_spell_level_offered"] = "3"; break;
   case 128: details["highest_spell_level_offered"] = "4"; break;
   case 160: details["highest_spell_level_offered"] = "5"; break;
   case 192: details["highest_spell_level_offered"] = "6"; break;
   case 224: details["highest_spell_level_offered"] = "7"; break;
   default: details["highest_spell_level_offered"] = UnknownString; break;
   }

   CString strTale;
   GetStringAt(pStrings, tavernTale, strTale);
   if (strTale.GetLength())
      details["tavern_tale"] = CStringOrEmptyString(strTale);
   else
      details["tavern_tale"] = "";

   details["shop_cost_modifier"] = FRUAGetCostModifierText( shopCostModifier );

   // FRUA BUG WORK AROUND
   //
   // FRUA will store a value of 5 for Div_10 (should be 4) here. 
   // Div_5 is stored as a value of 7 (Div_3) so a Div_5=5 will never show up.
   // If we see a value of 5 we interpret it as a Div_10 (index 4).
   if (trainTavernCostModifier == 5)
      details["traininghall_temple_cost_modifier"] = FRUAGetCostModifierText( 4 );
   else
      details["traininghall_temple_cost_modifier"] = FRUAGetCostModifierText(trainTavernCostModifier);

   ShopItemListType itemList;
   AssignShopItemBytes(itemList, item1_byte1, item1_byte2, item1_byte3);
   AssignShopItemBytes(itemList, item2_byte1, item2_byte2, item2_byte3);

   int count = 0;
   Json::Value itemArray(Json::arrayValue);
   POSITION pos = itemList.GetHeadPosition();
   while (pos != NULL)
   {
      SShopItemInfo itemInfo = itemList.GetNext(pos);
      Json::Value item;

      item["name"] = CStringOrEmptyString(itemInfo.name);
      item["quantity"] = IntToString(itemInfo.quantity);
      item["id"] = IntToString(itemInfo.itemIdx);

      count++;
      itemArray.append(item);
   }

   details["shop_items"] = itemArray;
   details["shop_items_count"] = IntToString(count);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      if ( (*itCurr).isArray() )
      {

         Json::ValueIterator itItem = (*itCurr).begin();
         // if array not empty
         while (itItem != (*itCurr).end())
         {
            Json::Value::Members shopItems = (*itItem).getMemberNames();
            std::vector<std::string>::iterator itCurrArray = shopItems.begin();
            while (itCurrArray != shopItems.end())
            {
               TRACE("SHOP ITEM: %s = %s\n", (*itCurrArray).c_str(), (*itItem)[(*itCurrArray).c_str()].asCString());
               ++itCurrArray;
            }

            ++itItem;
         }
      }
      else
      {
         TRACE("SMALL TOWN: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      }
      ++itCurr;
   }
}

void FormatLevelEventJson_ChainEvent(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   BYTE chainEvent = EVENT_DETAILS_OFFSET(9);
   details["chain_event"] = IntToString(chainEvent);

   //TRACE("CHAIN EVENT: Chained Event %s\n", details["chain_event"].asCString());
   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("CHAIN EVENT: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Vault(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE forceBackup = EVENT_DETAILS_OFFSET(8) & 4;

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["force_backup_one_step"] = forceBackup?"1":"0";

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("VAULT: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_CombatTreasure(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   DWORD platinum = GetThreeBytes(5, event);
   BYTE itemsIdentified = EVENT_DETAILS_OFFSET(8) & 128;
   WORD gems = GetWord(9, event);
   WORD jewelry = GetWord(11, event);
   BYTE item1 = EVENT_DETAILS_OFFSET(13);
   BYTE item2 = EVENT_DETAILS_OFFSET(14);
   BYTE item3 = EVENT_DETAILS_OFFSET(15);
   BYTE item4 = EVENT_DETAILS_OFFSET(16);
   BYTE item5 = EVENT_DETAILS_OFFSET(17);
   BYTE item6 = EVENT_DETAILS_OFFSET(18);
   BYTE item7 = EVENT_DETAILS_OFFSET(19);
   BYTE item8 = EVENT_DETAILS_OFFSET(20);

   details["platinum"] = IntToString(platinum);
   details["gems"] = IntToString(gems);
   details["jewelry"] = IntToString(jewelry);
   details["items_identified"] = itemsIdentified?"1":"0";
   
   CString strItemName;
   unsigned int itemIdx;

   strItemName = LookupItemNameByItemCode(item1);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_1"] = CStringOrEmptyString(strItemName);
   details["item_1_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item1));
   details["item_1_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item2);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_2"] = CStringOrEmptyString(LookupItemNameByItemCode(item2));
   details["item_2_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item2));
   details["item_2_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item3);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_3"] = CStringOrEmptyString(LookupItemNameByItemCode(item3));
   details["item_3_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item3));
   details["item_3_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item4);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_4"] = CStringOrEmptyString(LookupItemNameByItemCode(item4));
   details["item_4_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item4));
   details["item_4_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item5);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_5"] = CStringOrEmptyString(LookupItemNameByItemCode(item5));
   details["item_5_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item5));
   details["item_5_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item6);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_6"] = CStringOrEmptyString(LookupItemNameByItemCode(item6));
   details["item_6_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item6));
   details["item_6_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item7);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_7"] = CStringOrEmptyString(LookupItemNameByItemCode(item7));
   details["item_7_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item7));
   details["item_7_id"] = IntToString(itemIdx);

   strItemName = LookupItemNameByItemCode(item8);
   itemIdx = LookupItemIndexByName(strItemName);
   details["item_8"] = CStringOrEmptyString(LookupItemNameByItemCode(item8));
   details["item_8_quantity"] = IntToString(LookupItemBundleQuantityByItemCode(item8));
   details["item_8_id"] = IntToString(itemIdx);

   /*
   TRACE("COMBAT TREASURE: plat %s\n", details["platinum"].asCString());
   TRACE("COMBAT TREASURE: gems %s\n", details["gems"].asCString());
   TRACE("COMBAT TREASURE: jewelru %s\n", details["jewelry"].asCString());
   TRACE("COMBAT TREASURE: items id %s\n", details["items_identified"].asCString());
   TRACE("COMBAT TREASURE: item_1 %s\n", details["item_1"].asCString());
   TRACE("COMBAT TREASURE: item_1_quantity %s\n", details["item_1_quantity"].asCString());
   TRACE("COMBAT TREASURE: item_2 %s\n", details["item_2"].asCString());
   TRACE("COMBAT TREASURE: item_2_quantity %s\n", details["item_2_quantity"].asCString());
   TRACE("COMBAT TREASURE: item_3 %s\n", details["item_3"].asCString());
   TRACE("COMBAT TREASURE: item_3_quantity %s\n", details["item_3_quantity"].asCString());
   TRACE("COMBAT TREASURE: item_4 %s\n", details["item_4"].asCString());
   TRACE("COMBAT TREASURE: item_4_quantity %s\n", details["item_4_quantity"].asCString());
   TRACE("COMBAT TREASURE: item_5 %s\n", details["item_5"].asCString());
   TRACE("COMBAT TREASURE: item_5_quantity %s\n", details["item_5_quantity"].asCString());
   TRACE("COMBAT TREASURE: item_6 %s\n", details["item_6"].asCString());
   TRACE("COMBAT TREASURE: item_6_quantity %s\n", details["item_6_quantity"].asCString());
   TRACE("COMBAT TREASURE: item_7 %s\n", details["item_7"].asCString());
   TRACE("COMBAT TREASURE: item_7_quantity %s\n", details["item_7_quantity"].asCString());
   TRACE("COMBAT TREASURE: item_8 %s\n", details["item_8"].asCString());
   TRACE("COMBAT TREASURE: item_8_quantity %s\n", details["item_8_quantity"].asCString());
   */

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("COMBAT TREASURE: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_GainExperience(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE whoGets = EVENT_DETAILS_OFFSET(8) & 4;
   int exp = GetDWord(9, event);
   BYTE sound = EVENT_DETAILS_OFFSET(13);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   details["experience_given"] = IntToString(exp);
   details["sound"] = FRUAGetSoundNameText(sound);

   switch (whoGets)
   {
   case 0: details["experience_given_to"] = "entire party"; break;
   case 4: details["experience_given_to"] = "active party member"; break;
   default: details["experience_given_to"] = UnknownString; break;
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("GAIN EXP: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_PassTime(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE days = EVENT_DETAILS_OFFSET(9);
   BYTE hours = EVENT_DETAILS_OFFSET(10);
   BYTE minutes = EVENT_DETAILS_OFFSET(11);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);

   details["days"] = IntToString(days);
   details["hours"] = IntToString(hours);
   details["minutes"] = IntToString(minutes);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("PASS TIME: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Camp(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE forceBackup = EVENT_DETAILS_OFFSET(8) & 4;

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["force_backup_one_step"] = forceBackup?"1":"0";

   //TRACE("CAMP EVENT: Text %s\n", details["text"].asCString());
   //TRACE("CAMP EVENT: Picture %s\n", details["picture"].asCString());
   //TRACE("CAMP EVENT: Force Backup %s\n", details["force_backup_one_step"].asCString());
   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("CAMP: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_RemoveNPC(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE picDistance = EVENT_DETAILS_OFFSET(8) & 3;
   BYTE npc = EVENT_DETAILS_OFFSET(9);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   
   details["if_npc_in_party"] = FRUAGetMonsterNameText( npc );
   switch (picDistance)
   {
   case 0: details["picture_distance"] = "close"; break;
   case 1: details["picture_distance"] = "nearby"; break;
   case 2: details["picture_distance"] = "far away"; break;
   default: details["picture_distance"] = UnknownString; break;
   }   

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("REMOVE NPC: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_PickOneCombat(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE monsterMoraleMod = EVENT_DETAILS_OFFSET(8) & 0x7F;
   BYTE m1_num = EVENT_DETAILS_OFFSET(9) & 31;
   BYTE surprise = EVENT_DETAILS_OFFSET(9) & 0xC0;
   BYTE combatLoc = EVENT_DETAILS_OFFSET(9) & 32;
   BYTE m1_type = EVENT_DETAILS_OFFSET(10);
   BYTE m2_num = EVENT_DETAILS_OFFSET(11);
   BYTE m2_type = EVENT_DETAILS_OFFSET(12);
   BYTE m3_num = EVENT_DETAILS_OFFSET(13) & 31;
   BYTE autoApproach = EVENT_DETAILS_OFFSET(13) & 32;
   BYTE partyNeverDies = EVENT_DETAILS_OFFSET(13) & 64;
   BYTE noTreasure = EVENT_DETAILS_OFFSET(13) & 128;
   BYTE m3_type = EVENT_DETAILS_OFFSET(14);
   BYTE m4_num = EVENT_DETAILS_OFFSET(15) & 31;
   BYTE magicPrevented = EVENT_DETAILS_OFFSET(15) & 128;
   BYTE monsterDist = EVENT_DETAILS_OFFSET(15) & 0x60;
   BYTE m4_type = EVENT_DETAILS_OFFSET(16);
   BYTE m5_num = EVENT_DETAILS_OFFSET(17) & 31;
   BYTE monsterAttackFrontNorthEastSouth = EVENT_DETAILS_OFFSET(17) & 0xE0;
   BYTE m5_type = EVENT_DETAILS_OFFSET(18);
   BYTE m6_num = EVENT_DETAILS_OFFSET(19) & 31;
   BYTE monsterAttackWest = EVENT_DETAILS_OFFSET(19) & 32;
   BYTE turnUndeadMod = EVENT_DETAILS_OFFSET(19) & 0xC0;
   BYTE m6_type = EVENT_DETAILS_OFFSET(20);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText);
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   details["monster_morale_modifier"] = IntToString(monsterMoraleMod);
   details["location"] = (combatLoc==32)?"outdoors":"indoors";
   details["monsters_auto_approach"] = autoApproach?"1":"0";
   details["party_never_dies"] = partyNeverDies?"1":"0";
   details["no_treasure"] = noTreasure?"1":"0";
   details["magic_allowed"] = magicPrevented?"0":"1";

   switch (turnUndeadMod)
   {
   case 0: details["turn_undead_modifier"] = NoneString; break;
   case 64: details["turn_undead_modifier"] = "hard"; break;
   case 128: details["turn_undead_modifier"] = "difficult"; break;
   case 192: details["turn_undead_modifier"] = "impossible"; break;
   default: details["turn_undead_modifier"] = UnknownString; break;
   }

   switch (monsterDist)
   {
   case 0: details["monster_distance"] = "close"; break;
   case 32: details["monster_distance"] = "nearby"; break;
   case 64: details["monster_distance"] = "far away"; break;
   default: details["monster_distance"] = UnknownString; break;
   }

   switch (surprise)
   {
   case 0: details["surprise"] = "neither"; break;
   case 64: details["surprise"] = "party"; break;
   case 128: details["surprise"] = "monster"; break;
   default: details["surprise"] = UnknownString; break;
   }

   /*
   if (monsterAttackFrontNorthEastSouth == 0)
      details["attack_direction"] = "front";
   else if (monsterAttackFrontNorthEastSouth == 32)
      details["attack_direction"] = "north";
   else if (monsterAttackFrontNorthEastSouth == 64)
      details["attack_direction"] = "east";
   else if (monsterAttackFrontNorthEastSouth == 128)
      details["attack_direction"] = "south";
   else if (monsterAttackWest == 32)
      details["attack_direction"] = "west";
   */

   CString directions("");
   if (monsterAttackFrontNorthEastSouth & 32)
      directions += "north";
   if (monsterAttackFrontNorthEastSouth & 64)
   {
      if (directions.GetLength() > 0)
         directions += ", ";
      directions += "east";
   }

   if (monsterAttackFrontNorthEastSouth & 128)
   {
      if (directions.GetLength() > 0)
         directions += ", ";
      directions += "south";
   }

   if (monsterAttackWest & 32)
   {
      if (directions.GetLength() > 0)
         directions += ", ";
      directions += "west";
   }

   if (directions.GetLength() == 0)
      directions += "front";

   details["attack_direction"] = CStringOrEmptyString(directions);

   details["monster_1_quantity"] = IntToString(m1_num);
   details["monster_1_type"] = FRUAGetMonsterNameText(m1_type);

   details["monster_2_quantity"] = IntToString(m2_num);
   details["monster_2_type"] = FRUAGetMonsterNameText(m2_type);

   details["monster_3_quantity"] = IntToString(m3_num);
   details["monster_3_type"] = FRUAGetMonsterNameText(m3_type);

   details["monster_4_quantity"] = IntToString(m4_num);
   details["monster_4_type"] = FRUAGetMonsterNameText(m4_type);

   details["monster_5_quantity"] = IntToString(m5_num);
   details["monster_5_type"] = FRUAGetMonsterNameText(m5_type);

   details["monster_6_quantity"] = IntToString(m6_num);
   details["monster_6_type"] = FRUAGetMonsterNameText(m6_type);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("PICK ONE COMBAT: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_Teleporter(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE askYesNo = EVENT_DETAILS_OFFSET(8) & 32;
   BYTE transferOn = EVENT_DETAILS_OFFSET(8) & 64;
   BYTE destFace = EVENT_DETAILS_OFFSET(8) & 12;
   BYTE destY = EVENT_DETAILS_OFFSET(9);
   BYTE destX = EVENT_DETAILS_OFFSET(10);
   WORD text2 = GetWord(11, event);
   BYTE execDestEvent = EVENT_DETAILS_OFFSET(13) & 4;
   BYTE destType = EVENT_DETAILS_OFFSET(13) & 1;
   BYTE entryPoint = EVENT_DETAILS_OFFSET(14);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text_1"] = CStringOrEmptyString(strText); // ask

   CString strText2; 
   GetStringAt(pStrings, text2, strText2);
   details["text_2"] = CStringOrEmptyString(strText2);

   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   details["ask_yes_no"] = askYesNo?"1":"0";
   details["transfer_on_yes_no"] = (transferOn==64)?"no":"yes";
   details["dest_event_execute"] = execDestEvent?"1":"0";   
   details["dest_type"] = (destType==1)?"entry point":"xy";
   details["entry_point"] = IntToString(entryPoint);
   details["dest_x"] = IntToString(destX);
   details["dest_y"] = IntToString(destY);

   switch (destFace)
   {
   case 0: details["dest_facing"] = "north"; break;
   case 4: details["dest_facing"] = "east"; break;
   case 8: details["dest_facing"] = "south"; break;
   case 12: details["dest_facing"] = "west"; break;
   default: details["dest_facing"] = UnknownString; break;
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("TELEPORTER: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_QuestStage(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE acceptOn = EVENT_DETAILS_OFFSET(8) & 0x38;
   BYTE completeOnAccept = EVENT_DETAILS_OFFSET(8) & 64;
   BYTE failOnReject = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE quest = EVENT_DETAILS_OFFSET(9);
   BYTE stage = EVENT_DETAILS_OFFSET(10)+1;
   BYTE accentEvent = EVENT_DETAILS_OFFSET(11);
   BYTE rejectEvent = EVENT_DETAILS_OFFSET(12);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText); // ask
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);
   details["quest_completes_on_accept"] = completeOnAccept?"1":"0";
   details["quest_fails_on_reject"] = failOnReject?"1":"0";
   details["special_item_key_quest"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(quest));
   details["stage"] = IntToString(stage);
   details["accept_event"] = IntToString(accentEvent);
   details["reject_event"] = IntToString(rejectEvent);

   switch (acceptOn)
   {
   case 0: details["quest_accepted_on"] = "impossible"; break;
   case 8: details["quest_accepted_on"] = "yes"; break;
   case 16: details["quest_accepted_on"] = "no"; break;
   case 24: details["quest_accepted_on"] = "yes or no"; break;
   case 32: details["quest_accepted_on"] = "impossible no question"; break;
   case 40: details["quest_accepted_on"] = "automatic no question"; break;
   default: details["quest_accepted_on"] = UnknownString; break;
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("QUEST STAGE: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_QuestionYesNo(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE yesNoAction = EVENT_DETAILS_OFFSET(8) & 63;
   BYTE yesEvent = EVENT_DETAILS_OFFSET(9);
   BYTE noEvent = EVENT_DETAILS_OFFSET(10);
   WORD yesText = GetWord(11, event);
   WORD noText = GetWord(13, event);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText); 
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   CString strYesText; 
   GetStringAt(pStrings, yesText, strYesText);
   details["yes_text"] = CStringOrEmptyString(strYesText); 

   CString strNoText; 
   GetStringAt(pStrings, noText, strNoText);
   details["no_text"] = CStringOrEmptyString(strNoText); 

   details["yes_event"] = IntToString(yesEvent);
   details["no_event"] = IntToString(noEvent);

   /*
   After Chain On "Yes":
      0   Nothing
      4   Repeat Question
      32  Backup

   After Chain On "No":
      0   Nothing
      8   Repeat Question
      16  Backup
   */

   if (yesNoAction & 4)
   {
      details["yes_action"] = "repeat";
   }
   else if (yesNoAction & 32)
   {
      details["yes_action"] = "backup";
   }
   else
   {
      details["yes_action"] = NoneString;
   }

   if (yesNoAction & 8)
   {
      details["no_action"] = "repeat";
   }
   else if (yesNoAction & 16)
   {
      details["no_action"] = "backup";
   }
   else
   {
      details["no_action"] = NoneString;
   }

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("QUESTION YES NO: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_TavernTales(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD tale1 = GetWord(9, event);
   WORD tale2 = GetWord(11, event);
   WORD tale3 = GetWord(13, event);
   WORD tale4 = GetWord(15, event);
   WORD tale5 = GetWord(17, event);
   WORD tale6 = GetWord(19, event);

   CString strTale1;
   GetStringAt(pStrings, tale1, strTale1);
   details["tale_1"] = CStringOrEmptyString(strTale1);

   CString strTale2;
   GetStringAt(pStrings, tale2, strTale2);
   details["tale_2"] = CStringOrEmptyString(strTale2);

   CString strTale3;
   GetStringAt(pStrings, tale3, strTale3);
   details["tale_3"] = CStringOrEmptyString(strTale3);

   CString strTale4;
   GetStringAt(pStrings, tale4, strTale4);
   details["tale_4"] = CStringOrEmptyString(strTale4);

   CString strTale5;
   GetStringAt(pStrings, tale5, strTale5);
   details["tale_5"] = CStringOrEmptyString(strTale5);

   CString strTale6;
   GetStringAt(pStrings, tale6, strTale6);
   details["tale_6"] = CStringOrEmptyString(strTale6);

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("TAVERN TALES: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson_SpecialItems(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &details)
{
   WORD text = GetWord(5, event);
   BYTE picture = EVENT_DETAILS_OFFSET(7);
   BYTE action = EVENT_DETAILS_OFFSET(8) & 4;
   BYTE item = EVENT_DETAILS_OFFSET(9);

   CString strText; 
   GetStringAt(pStrings, text, strText);
   details["text"] = CStringOrEmptyString(strText); // ask
   details["picture"] = FRUAGetBigSmallSpritePicText(picture);

   details["special_item_key_quest"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(item));
   details["action"] = (action==4)?"take from party":"give to party";

   Json::ValueIterator itCurr = details.begin();
   while (itCurr != details.end())
   {
      TRACE("SPECIAL ITEM: %s = %s\n", itCurr.key().asCString(), (*itCurr).asCString());
      ++itCurr;
   }
}

void FormatLevelEventJson(const UALevelStrings *pStrings, const UALevelEvent &event, Json::Value &currEvent)
{
   currEvent["type"] = FRUAGetEventTypeText(event.eventType);
   currEvent["chained_event_id"] = IntToString(event.chainEvent);
   
   //  Do Only Once Flag:  
   //       0  No 
   //       1  Yes
   BYTE flag = event.eventTrigger & 0x01;
   if (flag == 0)
      currEvent["do_only_once"] = "0";
   else 
      currEvent["do_only_once"] = "1";

   // Chain: 
   //       0  Always 
   //       2  If Happens
   //       4  If Doesn't Happen
   flag = event.eventTrigger & 0x06;
   switch (flag)
   {
   case 0: currEvent["chained_event_follow"] = "always"; break;
   case 2: currEvent["chained_event_follow"] = "if happens"; break;
   case 4: currEvent["chained_event_follow"] = "if does not happen"; break;
   default: currEvent["chained_event_follow"] = UnknownString; break;
   }

   // Event Happens if:
   //       0  Always
   //       8  Party Has Special Item
   //      16  Party Doesn't Have Special Item
   //      24  Day Time
   //      32  Night Time
   //      40  Random Percentage
   //      48  Party Is Searching
   //      56  Party Is Not Searching
   //      64  Facing Correct Direction
   //      72  Quest Is Complete
   //      80  Quest Failed
   //      88  Quest In Progress
   //      96  Party Is Detecting Traps
   //     104  Party Is Not Detecting Traps
   //     112  Party Can See Invisible
   //     120  Party Cannot See Invisible
   //     128  Specified Class In Party
   //     136  Specified Race In Party
   flag = event.eventTrigger & 0xF8;
   switch (flag)
   {
   case 0:   currEvent["trigger"] = "always"; break;
   case 8:   currEvent["trigger"] = "party has special item"; break;
   case 16:  currEvent["trigger"] = "party does not have special item"; break;
   case 24:  currEvent["trigger"] = "day time"; break;
   case 32:  currEvent["trigger"] = "night time"; break;
   case 40:  currEvent["trigger"] = "random percentage"; break;
   case 48:  currEvent["trigger"] = "party is searching"; break;
   case 56:  currEvent["trigger"] = "party is not searching"; break;
   case 64:  currEvent["trigger"] = "facing direction"; break;
   case 72:  currEvent["trigger"] = "special quest complete"; break;
   case 80:  currEvent["trigger"] = "special quest failed"; break;
   case 88:  currEvent["trigger"] = "special quest in progress"; break;
   case 96:  currEvent["trigger"] = "party is detecting traps"; break;
   case 104: currEvent["trigger"] = "party is not detecting traps"; break;
   case 112: currEvent["trigger"] = "party can see invisible"; break;
   case 120: currEvent["trigger"] = "party cannot see invisible"; break;
   case 128: currEvent["trigger"] = "party has class"; break;
   case 136: currEvent["trigger"] = "party has race"; break;
   default:  currEvent["trigger"] = UnknownString; break;
   }

   // if the eventFlags match one of these codes
   // then the eventCondition should be interpreted 
   // as follows:
   //
   // Event Happens if:
   //   8   Party Has Special Item
   //       * 0-7   Key 1-8 
   //       * 8-19  Item 1-12
   //       * 20-63 Quest 1-44
   //  16  Party Doesn't Have Special Item
   //       * 0-7   Key 1-8 
   //       * 8-19  Item 1-12
   //       * 20-63 Quest 1-44
   //  40  Random Percentage
   //       * 1-99%
   //  64  Facing Correct Direction
   //       * 1 North    * 9  N,W
   //       * 2 East     * 10 E,W
   //       * 3 N,E      * 11 N,E,W
   //       * 4 South    * 12 S,W
   //       * 5 N,S      * 13 N,S,W
   //       * 6 E,S      * 14 E,S,W
   //       * 7 N,E,S    * 15 N,E,S,W
   //       * 8 West
   // 72  Quest Is Complete
   //       * 0-7   Key 1-8 
   //       * 8-19  Item 1-12
   //       * 20-63 Quest 1-44
   // 80  Quest Failed
   //       * 0-7   Key 1-8 
   //       * 8-19  Item 1-12
   //       * 20-63 Quest 1-44
   // 88  Quest In Progress
   //       * 0-7   Key 1-8 
   //       * 8-19  Item 1-12
   //       * 20-63 Quest 1-44
   // 128  Specified Class In Party
   //       * 0 Cleric
   //       * 2 Fighter
   //       * 3 Paladin
   //       * 4 Ranger
   //       * 5 Magic User
   //       * 6 Thief
   // 136 Specified Race In Party
   //       * 0 Elf
   //       * 1 Half-Elf
   //       * 2 Dwarf
   //       * 3 Gnome
   //       * 4 Halfling
   //       * 5 Human
   switch (flag)
   {
   case 8:   currEvent["trigger_data"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(event.eventTriggerData)); break;
   case 16:  currEvent["trigger_data"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(event.eventTriggerData)); break;
   case 40:  currEvent["trigger_data"] = IntToString(event.eventTriggerData); break;
   case 64:  currEvent["trigger_data"] = FRUAGetDirectionText(event.eventTriggerData-1); break; // code starts at 1 for north
   case 72:  currEvent["trigger_data"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(event.eventTriggerData)); break;
   case 80:  currEvent["trigger_data"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(event.eventTriggerData)); break;
   case 88:  currEvent["trigger_data"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(event.eventTriggerData)); break;
   case 128: currEvent["trigger_data"] = FRUAGetClassText(event.eventTriggerData); break;
   case 136: currEvent["trigger_data"] = FRUAGetRaceText(event.eventTriggerData); break;
   default:  currEvent["trigger_data"] = NoneString; break;
   }

   TRACE("EVENT         TYPE: %s\n", currEvent["type"].asCString());
   TRACE("EVENT      DO ONCE: %s\n", currEvent["do_only_once"].asCString());
   TRACE("EVENT     CHAIN IF: %s\n", currEvent["chained_event_follow"]);
   TRACE("EVENT  CHAIN EVENT: %s\n", currEvent["chained_event_id"].asCString());      
   TRACE("EVENT      TRIGGER: %s\n", currEvent["trigger"].asCString());
   TRACE("EVENT TRIGGER DATA: %s\n", currEvent["trigger_data"].asCString());

   Json::Value &eventDetails = currEvent["details"];

   switch (event.eventType)
   {
   case 1: FormatLevelEventJson_Combat(pStrings, event, eventDetails); break; //type = Combat;    break;
   case 2: FormatLevelEventJson_TextEvent(pStrings, event, eventDetails); break; //type = TextStatement;  break;
   case 3: FormatLevelEventJson_GiveTreasure(pStrings, event, eventDetails); break; //type = GiveTreasure;   break;
   case 4: FormatLevelEventJson_Damage(pStrings, event, eventDetails); break; //type = Damage;      break;
   case 5: FormatLevelEventJson_Stairs(pStrings, event, eventDetails); break; //type = Stairs;      break;
   case 6: FormatLevelEventJson_TrainingHall(pStrings, event, eventDetails); break; //type = TrainingHallEvent;  break;
   case 7: FormatLevelEventJson_Tavern(pStrings, event, eventDetails); break; //type = TavernEvent;      break;
   case 8: FormatLevelEventJson_Shop(pStrings, event, eventDetails); break; //type = ShopEvent;      break;
   case 9: FormatLevelEventJson_Temple(pStrings, event, eventDetails); break; //type = TempleEvent;      break;
   case 10: FormatLevelEventJson_QuestionButton(pStrings, event, eventDetails); break; //type = QuestionButton;      break;
   case 11: FormatLevelEventJson_TransferModule(pStrings, event, eventDetails); break; //type = TransferModule;      break;
   case 12: FormatLevelEventJson_GuidedTour(pStrings, event, eventDetails); break; //type = GuidedTour;      break;
   case 13: FormatLevelEventJson_AddNPC(pStrings, event, eventDetails); break; //type = AddNpc;      break;
   case 14: FormatLevelEventJson_NPCSays(pStrings, event, eventDetails); break; //type = NPCSays;      break;
   case 15: FormatLevelEventJson_Encounter(pStrings, event, eventDetails); break; //type = EncounterEvent;      break;
   case 16: FormatLevelEventJson_Utilities(pStrings, event, eventDetails); break; //type = Utilities;      break;
   case 17: FormatLevelEventJson_Sounds(pStrings, event, eventDetails); break; //type = Sounds;      break;
   case 18: FormatLevelEventJson_WhoTries(pStrings, event, eventDetails); break; //type = WhoTries;      break;
   case 19: FormatLevelEventJson_WhoPays(pStrings, event, eventDetails); break; //type = WhoPays;      break;
   case 20: FormatLevelEventJson_Password(pStrings, event, eventDetails); break; //type = EnterPassword;      break;
   case 21: FormatLevelEventJson_QuestionList(pStrings, event, eventDetails); break; //type = QuestionList;      break;
   case 22: FormatLevelEventJson_SmallTown(pStrings, event, eventDetails); break; //type = SmallTown;      break;
   case 23: FormatLevelEventJson_ChainEvent(pStrings, event, eventDetails); break; //type = ChainEventType;     break;
   case 24: FormatLevelEventJson_Vault(pStrings, event, eventDetails); break; //type = Vault;      break;
   case 25: FormatLevelEventJson_CombatTreasure(pStrings, event, eventDetails); break; //type = CombatTreasure;     break;
   case 26: FormatLevelEventJson_GainExperience(pStrings, event, eventDetails); break; //type = GainExperience;     break;
   case 27: FormatLevelEventJson_PassTime(pStrings, event, eventDetails); break; //type = PassTime;      break;
      // case 28 unused
   case 29: FormatLevelEventJson_Camp(pStrings, event, eventDetails); break; //type = Camp;     break;
      // case 30 unused
      // case 31 unused
   case 32: FormatLevelEventJson_RemoveNPC(pStrings, event, eventDetails); break; //type = RemoveNPCEvent;      break;
   case 33: FormatLevelEventJson_PickOneCombat(pStrings, event, eventDetails); break; //type = PickOneCombat;      break;
   case 34: FormatLevelEventJson_Teleporter(pStrings, event, eventDetails); break; //type = Teleporter;      break;
   case 35: FormatLevelEventJson_QuestStage(pStrings, event, eventDetails); break; //type = QuestStage;      break;
   case 36: FormatLevelEventJson_QuestionYesNo(pStrings, event, eventDetails); break; //type = QuestionYesNo;      break;
   case 37: FormatLevelEventJson_TavernTales(pStrings, event, eventDetails); break; //type = TavernTales;      break;
   case 38: FormatLevelEventJson_SpecialItems(pStrings, event, eventDetails); break; //type = SpecialItem;      break;
   default: eventDetails["unknown event type"] = "unknown event details"; break; //type = NoEvent;     break;
   }

}

void FormatLevelEventsJson(const UALevelData &level, Json::Value &currLevel)
{
   int count = 0;
   Json::Value eventArray(Json::arrayValue);

   for (int i=0; i<MAX_FRUA_EVENTS; i++)
   {
      if (level.events[i].eventType > 0)
      {
         Json::Value event;

         event["id"] = IntToString(i+1);

         TRACE("EVENT ID %u START\n", i+1);

         FormatLevelEventJson(&level.strings, level.events[i], event);

         TRACE("EVENT ID %u END\n", i+1);

         eventArray.append(event);
         count++;
      }
   }

   currLevel["events"] = eventArray;
   currLevel["events_count"] = IntToString(count);
}

void FormatLevelStringsJson(const UALevelData &level, Json::Value &currLevel)
{   
   int count = 0;

   Json::Value stringsArray(Json::arrayValue);
   for (int i=1; i<MAX_FRUA_LEVEL_STRINGS; i++)
   {
      CString strTemp("");
      GetStringAt(&level.strings, i, strTemp);

      if (strTemp.GetLength() > 0)
      {
         Json::Value stringInfo;
         
         stringInfo["id"] = IntToString(i);      
         stringInfo["string"] = CStringOrEmptyString(strTemp);

         count++;
         stringsArray.append(stringInfo);
      }
   }
   currLevel["strings_count"] = IntToString(count);
   currLevel["strings"] = stringsArray;   
}

void FormatLevelJson(const UALevelData &level, Json::Value &currLevel)
{
   Json::Value &header = currLevel["config"];
   FormatLevelHeaderJson(level, header);

   //Json::Value &map = currLevel["map"];
   FormatLevelMapJson(level, currLevel);

   //Json::Value &events = currLevel["events"];
   FormatLevelEventsJson(level, currLevel);

   //Json::Value &strings = currLevel["strings"];
   FormatLevelStringsJson(level, currLevel);
}

void ReadLevelsDat(Json::Value &root)
{
   printf("Exporting map\n");   

   CFileFind finder;
   CString searchPath = DesignFolder;
   searchPath += "\\geo*.dat";

   BOOL found = finder.FindFile(searchPath);

   while (found)
   {
      found = finder.FindNextFile();

      if (finder.IsDots())
         continue;
      if (finder.IsDirectory())
         continue;

      // convert 'geo005.dat' 
      // to '005'
      // to levelIdx = 5
      // 
      // RBT: is there a geo000.dat?
      //      or do the level numbers start at 1
      CString filename = finder.GetFileTitle();
      filename.Trim();
      filename = filename.Right(3);
      int levelIdx = atoi(filename);

      ASSERT( levelIdx > 0 ); // lets assume they start at 1
      levelIdx--;

      ImportFRUAGeoDat(levelIdx, finder.GetFilePath());
   }

   //Json::Value &levelsCollection = root["levels"];
   //CString strIdKey;

   int levelCount = 0;
   Json::Value levelArray(Json::arrayValue);
   for (int i=0; i<MAX_FRUA_LEVELS; i++)
   {
      if (Levels[i].isPopulated)
      {
         Json::Value level;

         //strIdKey.Format("level_%03u", i+1);
         //Json::Value &currLevel = levelsCollection[strIdKey];
         level["id"] = IntToString(i+1);

         FormatLevelJson(Levels[i], level);

         levelCount++;
         levelArray.append(level);
      }
   }

   root["levels"] = levelArray;
   root["levels_count"] = IntToString(levelCount);
}
