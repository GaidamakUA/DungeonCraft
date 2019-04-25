/******************************************************************************
* Filename: Items.cpp
* Copyright (c) 2000, UAF Development Team (email CocoaSpud@hotmail.com)
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
#include "..\Shared\stdafx.h"
//#include "..\Shared\ProjectVersion.h"
#ifdef UAFEDITOR
#include "..\UAFwinEd\resource.h"
#endif
#include <math.h>

#ifdef UAFEDITOR
#include "..\UAFWinEd\UAFWinEd.h"
#else
#include "externs.h"
#include "..\UAFWin\Dungeon.h"
#endif

#include "class.h"
#ifdef UAFEDITOR
#include "..\UAFWinEd\CrossReference.h"
#endif
#include "SurfaceMgr.h"
#include "PicSlot.h"
#include "SoundMgr.h"
#include "items.h"
#include "GlobalData.h"
//#include "level.h"
#include "Graphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "..\Shared\Items.h"


// store/retrieve data from asl maps
//void StorePicDataAsASL(A_ASLENTRY_L &asl, PIC_DATA& data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrievePicDataFromASL(A_ASLENTRY_L &asl, PIC_DATA& data, const char *prefix);
void StoreIntAsASL(A_ASLENTRY_L &asl, int data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrieveIntFromASL(A_ASLENTRY_L &asl, int &data, const char *prefix);
void StoreStringAsASL(A_ASLENTRY_L &asl, CString &data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrieveStringFromASL(A_ASLENTRY_L &asl, CString &data, const char *prefix);
void die(CString& msg);
void die(const char *msg);

extern CString ActivationScriptName;
extern CString DeActivationScriptName;
extern CString emptyString;
extern CString errorText;

extern const CString Baseclass_magicUser;
extern const CString Baseclass_thief;
extern const CString Baseclass_paladin;
extern const CString Baseclass_cleric;
extern const CString Baseclass_fighter;
extern const CString Baseclass_ranger;
extern const CString Baseclass_druid;

extern const double VersionSpellNames;
extern const double VersionSpellIDs;
extern const double VersionSaveIDs;
extern const double PRODUCT_VER;


//IMPLEMENT_SERIAL( ITEM_LIST, CObject, 1)
//IMPLEMENT_SERIAL( ITEM_DATA, CObject, 1)
//IMPLEMENT_SERIAL( ITEM_DATA_TYPE, CObject, 1)
//IMPLEMENT_SERIAL( READY_ITEMS, CObject, 1)

ITEM_DATA_TYPE itemData;

ITEM_TEXT_LIST itemListText;
ITEM_TEXT_LIST charItemListText;
ITEM_TEXT_LIST treasureItemListText;
ITEM_TEXT_LIST scriptChoiceListText;



//*****************************************************************************
//    NAME: isWeapon
//
// PURPOSE: 
//
//*****************************************************************************
BOOL isWeapon(weaponClassType type)
{
  switch (type) 
  {
  case NotWeapon:
  case Ammo:
    return FALSE;
  case HandBlunt:
  case HandCutting:
  case HandThrow:
  case SlingNoAmmo:
  case Bow:
  case Crossbow:
  case Throw:
  case SpellCaster:
  case SpellLikeAbility:
    return TRUE;
  default:
    WriteDebugString("Bogus item type in IsWeapon()\n");
    break;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: WpnUsesAmmo
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnUsesAmmo(weaponClassType type)
{
  switch (type) 
  {
  case NotWeapon:
  case Ammo:
  case HandBlunt:
  case HandCutting:
  case HandThrow:
  case SlingNoAmmo:
  case Throw:
  case SpellCaster:
  case SpellLikeAbility:
    return FALSE;
  case Bow:
  case Crossbow:
    return TRUE;
  default:
    WriteDebugString("Bogus item type in WpnUsesAmmo()\n");
    break;
  }
  return FALSE;
}


//*****************************************************************************
//    NAME: WpnConsumesSelfAsAmmo
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnConsumesSelfAsAmmo(weaponClassType type)
{
  switch (type)
  {
   case HandBlunt:
   case HandCutting:
   case SlingNoAmmo:
   case Bow:
   case Crossbow:
     return FALSE;

   case HandThrow: // dec item qty by 1 when item is thrown
   case Throw:
   case SpellCaster:
   case SpellLikeAbility:
     return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: isWeapon
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL isWeapon(GLOBAL_ITEM_ID giID)
BOOL isWeapon(const ITEM_ID& itemID)
{
  //if (ITEM_LIST::IsMoneyItem(giID))
  if (ITEM_LIST::IsMoneyItem(itemID))
    return FALSE;
  //return (isWeapon(itemData.GetWpnType(giID)));
  return (isWeapon(itemData.GetWpnType(itemID)));
}

//*****************************************************************************
//    NAME: WpnUsesAmmo
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL WpnUsesAmmo(GLOBAL_ITEM_ID giID)
BOOL WpnUsesAmmo(const ITEM_ID& itemID)
{
  //if (ITEM_LIST::IsMoneyItem(giID))
  if (ITEM_LIST::IsMoneyItem(itemID))
    return FALSE;
  //return (WpnUsesAmmo(itemData.GetWpnType(giID))); 
  return (WpnUsesAmmo(itemData.GetWpnType(itemID))); 
}

//*****************************************************************************
//    NAME: WpnCanAttackAtRange
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL WpnCanAttackAtRange(GLOBAL_ITEM_ID Wpn_giID, int Range)
BOOL WpnCanAttackAtRange(const ITEM_ID& weaponID, int Range)
{
  //if (ITEM_LIST::IsMoneyItem(Wpn_giID))
  if (ITEM_LIST::IsMoneyItem(weaponID))
    return FALSE;

  //switch (itemData.GetWpnType(Wpn_giID)) 
  switch (itemData.GetWpnType(weaponID)) 
  {
  case NotWeapon:
  case Ammo:   
    return FALSE;

  case SlingNoAmmo:
  case Bow:
  case Crossbow:
  case Throw:
    return (   (Range >= 2)
            //&& (Range <= itemData.GetItemRange(Wpn_giID)));
            && (Range <= itemData.GetItemRange(weaponID)));

  case HandBlunt:
  case HandCutting:
  case HandThrow:
    //return (Range <= itemData.GetItemRange(Wpn_giID));
    return (Range <= itemData.GetItemRange(weaponID));
  case SpellCaster:
  case SpellLikeAbility:
    {
      const SPELL_DATA *pSpell;
      const ITEM_DATA *pItem;
      pItem = itemData.PeekItem(weaponID);
      if (pItem->SpellID().IsEmpty())
      {
        return (Range <= itemData.GetItemRange(weaponID));
      };
      pSpell = spellData.PeekSpell(pItem->SpellID());
      if ((pSpell == NULL) || (pSpell->Targeting != Self))
      {
        return (Range <= itemData.GetItemRange(weaponID));
      };
      return Range == 0;
    };
    //NotImplemented(0x55109, false);
    //break;
  default:
    WriteDebugString("Bogus item type in IsWeapon()\n");
    break;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: WpnConsumesAmmoAtRange
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL WpnConsumesAmmoAtRange(GLOBAL_ITEM_ID Wpn_giID, int Range)
BOOL WpnConsumesAmmoAtRange(const ITEM_ID& weaponID, int Range)
{
  //if (ITEM_LIST::IsMoneyItem(Wpn_giID))
  if (ITEM_LIST::IsMoneyItem(weaponID))
    return FALSE;

  //switch (itemData.GetWpnType(Wpn_giID))
  switch (itemData.GetWpnType(weaponID))
  {
   case HandBlunt:
   case HandCutting:
   case SlingNoAmmo:
   case SpellCaster:
   case SpellLikeAbility:
     return FALSE;

   case HandThrow: // dec item qty by 1 when item is thrown
   case Throw:
     return (Range > 1);

   case Bow:
   case Crossbow:  // dec ammo qty by 1
     return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: WpnConsumesSelfAsAmmo
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL WpnConsumesSelfAsAmmo(GLOBAL_ITEM_ID Wpn_giID)
BOOL WpnConsumesSelfAsAmmo(const ITEM_ID& weaponID)
{
  //if (ITEM_LIST::IsMoneyItem(Wpn_giID))
  if (ITEM_LIST::IsMoneyItem(weaponID))
    return FALSE;
  //return (WpnConsumesSelfAsAmmo(itemData.GetWpnType(Wpn_giID)));
  return (WpnConsumesSelfAsAmmo(itemData.GetWpnType(weaponID)));
}

//*****************************************************************************
// NAME: isMagical
//
// PURPOSE:  
//*****************************************************************************
//BOOL isMagical(GLOBAL_ITEM_ID giID)
BOOL isMagical(const ITEM_ID& itemID)
{
  //if (ITEM_LIST::IsMoneyItem(giID))
  if (ITEM_LIST::IsMoneyItem(itemID))
    return FALSE;

  //ITEM_DATA *data = itemData.GetItemData(giID);
  ITEM_DATA *data = itemData.GetItem(itemID);
  if (data == NULL)
    return FALSE;

  if (data->IsNotMagical())
    return FALSE;

  if (data->Cursed)
    return TRUE;

  if (data->Protection_Bonus)
    return TRUE;

  if (data->Attack_Bonus)
    return TRUE;

  if ((data->Dmg_Bonus_Lg)||(data->Dmg_Bonus_Sm))
    return TRUE;

  //if ((data->IsUsable()) && (data->m_gsID.IsValidSpell()) )
  if ((data->IsUsable()) && (data->spellID.IsValidSpell()) )
    return TRUE;

  //if (data->specAbs.HaveAtLeastOneSpecAb())
    //return TRUE;
  if (data->specAbs.GetCount() != 0) 
  {
    //NotImplemented(0x4c9b9b,FALSE);
    return TRUE;
  };

  return FALSE;
}

//*****************************************************************************
//    NAME: itemUsesRdySlot
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL itemUsesRdySlot(GLOBAL_ITEM_ID giID)
BOOL itemUsesRdySlot(const ITEM_ID& itemID)
{
  //if (ITEM_LIST::IsMoneyItem(giID)) return FALSE;
  if (ITEM_LIST::IsMoneyItem(itemID)) return FALSE;
  //return (itemUsesRdySlot(itemData.GetItemData(giID)));
  return (itemUsesRdySlot(itemData.GetItem(itemID)));
}

//*****************************************************************************
//    NAME: itemUsesRdySlot
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemUsesRdySlot(const ITEM_DATA *pItem)
{
  if (pItem == NULL) return FALSE;
  return (pItem->Location_Readied != Cannot);
}


//*****************************************************************************
// NAME: itemCanBeTraded
//
// PURPOSE:  
//*****************************************************************************
//BOOL itemCanBeTraded(GLOBAL_ITEM_ID giID)
BOOL itemCanBeTraded(const ITEM_ID& itemID)
{
  //ITEM_DATA *pItem = itemData.GetItemData(giID);
  ITEM_DATA *pItem = itemData.GetItem(itemID);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);  
}

//*****************************************************************************
//    NAME: itemCanBeDropped
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL itemCanBeDropped(GLOBAL_ITEM_ID giID)
BOOL itemCanBeDropped(const ITEM_ID& itemID)
{
  //ITEM_DATA *pItem = itemData.GetItemData(giID);
  ITEM_DATA *pItem = itemData.GetItem(itemID);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);    
}

//*****************************************************************************
//    NAME: itemCanBeDeposited
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL itemCanBeDeposited(GLOBAL_ITEM_ID giID)
BOOL itemCanBeDeposited(const ITEM_ID& itemID)
{
  //ITEM_DATA *pItem = itemData.GetItemData(giID);
  ITEM_DATA *pItem = itemData.GetItem(itemID);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);    
}

//*****************************************************************************
//    NAME: itemCanBeSold
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL itemCanBeSold(GLOBAL_ITEM_ID giID)
BOOL itemCanBeSold(const ITEM_ID& itemID)
{
  //ITEM_DATA *pItem = itemData.GetItemData(giID);
  ITEM_DATA *pItem = itemData.GetItem(itemID);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);    
}

//*****************************************************************************
// NAME: itemCanBeJoined
//
// PURPOSE:  
//*****************************************************************************
//BOOL itemCanBeJoined(GLOBAL_ITEM_ID giID)
BOOL itemCanBeJoined(const ITEM_ID& itemID)
{
  //if (ITEM_LIST::IsMoneyItem(giID)) return FALSE;
  if (ITEM_LIST::IsMoneyItem(itemID)) return FALSE;
  //ITEM_DATA *pItem = itemData.GetItemData(giID);
  ITEM_DATA *pItem = itemData.GetItem(itemID);
  if (pItem==NULL) return FALSE;
  if (pItem->Bundle_Qty <= 1) return FALSE;
  if (!pItem->CanBeHalvedJoined) return FALSE;
  return TRUE;
}


//*****************************************************************************
// NAME: itemCanBeHalved
//
// PURPOSE:  
//*****************************************************************************
//BOOL itemCanBeHalved(GLOBAL_ITEM_ID giID)
BOOL itemCanBeHalved(const ITEM_ID& itemID)
{
  //if (ITEM_LIST::IsMoneyItem(giID)) return FALSE;
  if (ITEM_LIST::IsMoneyItem(itemID)) return FALSE;
  //ITEM_DATA *pItem = itemData.GetItemData(giID);
  ITEM_DATA *pItem = itemData.GetItem(itemID);
  if (pItem==NULL) return FALSE;
  if (pItem->Bundle_Qty <= 1) return FALSE;
  if (!pItem->CanBeHalvedJoined) return FALSE;
  return TRUE;
}


//*****************************************************************************
// NAME: itemCanBeReadied
//
// PURPOSE:  
//*****************************************************************************
//BOOL itemCanBeReadied(GLOBAL_ITEM_ID giID)
BOOL itemCanBeReadied(const ITEM_ID& itemID)
{
  //if (ITEM_LIST::IsMoneyItem(giID))
  if (ITEM_LIST::IsMoneyItem(itemID))
    return FALSE;
  return TRUE;
}

// checks by item class type
//*****************************************************************************
//    NAME: itemCanBeTraded
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeTraded(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeDropped
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeDropped(itemClassType type) 
{ 
  return ( /*(type!=SpecialItemType)&&(type!=SpecialKeyType)&&*/ (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeDeposited
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeDeposited(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeSold
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeSold(itemClassType type)
{
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}

//*****************************************************************************
//    NAME: itemCanBeJoined
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeJoined(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeHalved
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeHalved(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeReadied
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeReadied(itemClassType type) 
{ 
  return (type == itemType); 
}
//*****************************************************************************
//    NAME: itemIsMoney
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemIsMoney(itemClassType type) 
{
  return (   (type == PlatinumType) || (type == ElectrumType)
          || (type == GoldType)     || (type == SilverType)
          || (type == CopperType)   || (type == GemType)
          || (type == JewelryType)  || (type == CoinType6)
          || (type == CoinType7)    || (type == CoinType8)
          || (type == CoinType9)    || (type == CoinType10));
}

//*****************************************************************************
// NAME: getItemEncumbrance
//
// PURPOSE:  
//*****************************************************************************
//int getItemEncumbrance(GLOBAL_ITEM_ID giID, int qty)
int getItemEncumbrance(const ITEM_ID& itemID, int qty)
{
  if (qty<=0) return 0;

  //if (ITEM_LIST::IsMoneyItem(giID))
  if (ITEM_LIST::IsMoneyItem(itemID))
  {    
    if (globalData.moneyData.GetWeight()>0)
    {
      // GetWeight() returns number of coins
      // per unit of encumbrance
      int result = qty/globalData.moneyData.GetWeight();
      if (result <= 0) result = 1;
      return result;
    }
    else
      return qty;
  }

  //ITEM_DATA *data = itemData.GetItemData(giID);
  ITEM_DATA *data = itemData.GetItem(itemID);
  if (data == NULL)
    return 0;
  double bundle = max(data->Bundle_Qty, 1);

  // if bundle qty > 1 then stated encumbrance is for
  // the whole group of items. Figure out the encumbrance
  // for just one item.
	double encumbrance = (data->Encumbrance/bundle);
  /*
  // item encumbrance is stated in terms of number of gold peices.
  // the encumbrance rating given from strength table is in terms
  // of pounds. Divide item encumbrance by number of GP per pound
  int w = globalData.moneyData.GetWeight();
  encumbrance /= w;
  */

  // multiply encumbrance by qty of this item
  encumbrance *= (double)qty;
  return max((int)encumbrance, 0);
}


//*****************************************************************************
//    NAME: ITEM::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM::Serialize(CArchive &ar, double ver)
{
  if (ar.IsStoring())
  {
    ar << key;
          //m_giID.Serialize(ar);
    ar << itemID;
    ar << ready;
    ar << qty;
    ar << identified;
    ar << charges;
    ar << cursed;
    ar << paid;
  }
  else
  {
    ar >> key;
          //m_giID.Serialize(ar);
#ifdef UAFEDITOR
    if (ver < VersionSpellNames)
    {
      int preVersionSpellNames_giID;
      ar >> preVersionSpellNames_giID;
      itemID = itemData.FindPreVersionSpellNamesItemID(preVersionSpellNames_giID);
    }
    else
#endif
    {
      ar >> itemID;
    };
    ar >> ready;
    ar >> qty;
    ar >> identified;
    ar >> charges;
    ar >> cursed;
    ar >> paid;
  };
};

//*****************************************************************************
//    NAME: ITEM::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM::Serialize(CAR &ar, double ver)
{
  if (ar.IsStoring())
  {
    ar << key;
          //m_giID.Serialize(ar);
    ar << itemID;
    ar << ready;
    ar << qty;
    ar << identified;
    ar << charges;
    ar << cursed;
    ar << paid;
  }
  else
  {

    ar >> key;
          //m_giID.Serialize(ar);
#ifdef UAFEDITOR
    if (ver < VersionSpellNames)
    {
      int preVersionSpellNames_giID;
      ar >> preVersionSpellNames_giID;
      itemID = itemData.FindPreVersionSpellNamesItemID(preVersionSpellNames_giID);
    }
    else
#endif
    {
      ar >> itemID;
    };
    ar >> ready;
    ar >> qty;
    ar >> identified;
    ar >> charges;
    ar >> cursed;
    ar >> paid;
  };
};

#ifdef UAFEDITOR
const char *JKEY_ITEMID     = "itemID";
const char *JKEY_READY      = "ready";
const char *JKEY_QUANTITY   = "quantity";
const char *JKEY_IDENTIFIED = "identified";
const char *JKEY_CHARGES    = "charges";
const char *JKEY_CURSED     = "cursed";
const char *JKEY_PAID       = "paid";
extern const char *JKEY_ITEM;

void ITEM::Export(JWriter& jw)
{
  jw.StartList();
  jw.NameAndValue (JKEY_KEY        ,key);
  jw.NameAndValue (JKEY_ITEMID     ,itemID);
  jw.NameAndValue (JKEY_READY      ,ready);
  jw.NameAndValue (JKEY_QUANTITY   ,qty);
  jw.NameAndValue (JKEY_IDENTIFIED ,identified);
  jw.NameAndValue (JKEY_CHARGES    ,charges);
  jw.NameAndValue (JKEY_CURSED     ,cursed);
  jw.NameAndValue (JKEY_PAID       ,paid);
  jw.EndList();
}
bool ITEM::Import(JReader& jr)
{
  if (jr.Optional(), !jr.StartList()) return false;
  jr.NameAndValue (JKEY_KEY        ,key);
  jr.NameAndValue (JKEY_ITEMID     ,itemID);
  jr.NameAndValue (JKEY_READY      ,ready);
  jr.NameAndValue (JKEY_QUANTITY   ,qty);
  jr.NameAndValue (JKEY_IDENTIFIED ,identified);
  jr.NameAndValue (JKEY_CHARGES    ,charges);
  jr.NameAndValue (JKEY_CURSED     ,cursed);
  jr.NameAndValue (JKEY_PAID       ,paid);
  jr.EndList();
  return true;
}
#endif

//*****************************************************************************
// NAME: READY_ITEMS::GetReady
//
// PURPOSE:  
//*****************************************************************************
/*
int READY_ITEMS::GetReady(itemLocationType type) const
{
	switch (type) 
  {
		case WeaponHand:
			return readyWeaponHand;
		case ShieldHand:
			return readyShieldHand;
		case BodyArmor:
			return readyArmor;
		case Hands:
			return readyGauntlets;
		case Head:
			return readyHelmet;
		case Waist:
			return readyBelt;
		case BodyRobe:
			return readyRobe;
		case Back:
			return readyCloak;
		case Feet:
			return readyBoots;

		case Fingers:
      // This is an ambiguous function call for fingers since
      // what you get back as a readied item depends on how
      // many finger slots are currently being used.
      //
      // This function is mostly used for combat purposes to get the
      // current sword/sheild/ammo items being used. The ASSERT is here
      // as a warning that there probably isn't a good reason to call
      // this function for finger items.
      ASS ERT(FALSE);

			if (readyRing1 != NO_READY_ITEM) 
        return readyRing1;
      else
        return readyRing2;

		case AmmoQuiver:
			return readyMissileAmmo;
		default:
			WriteDebugString("BOGUS body location %i in READY_ITEMS::Ready\n", 
                       type);
			return NO_READY_ITEM;
  }
}
*/
//*****************************************************************************
//    NAME: READY_ITEMS::CanReady
//
// PURPOSE: 
//
//*****************************************************************************
/*
int READY_ITEMS::CanReady(itemLocationType type)
{
  switch (type) 
  {
		case WeaponHand:
      return (readyWeaponHand == NO_READY_ITEM);
    case ShieldHand:
      return (readyShieldHand == NO_READY_ITEM);
    case BodyArmor:
      return (readyArmor == NO_READY_ITEM);
    case Hands:
      return (readyGauntlets == NO_READY_ITEM);
    case Head:
      return (readyHelmet == NO_READY_ITEM);
    case Waist:
      return (readyBelt == NO_READY_ITEM);
    case BodyRobe:
      return (readyRobe == NO_READY_ITEM);
    case Back:
      return (readyCloak == NO_READY_ITEM);
    case Feet:
      return (readyBoots == NO_READY_ITEM);
    case Fingers:
      return ((readyRing1 == NO_READY_ITEM) || (readyRing2 == NO_READY_ITEM));    
    case AmmoQuiver:
      return (readyMissileAmmo == NO_READY_ITEM);

    default:
      WriteDebugString("BOGUS body location %i in READY_ITEMS::Ready\n", type);
      return NO_READY_ITEM;
  }
}
*/
//*****************************************************************************
// NAME: READY_ITEMS::ClearReady
//
// PURPOSE:  
//*****************************************************************************
/*BOOL READY_ITEMS::ClearReadyByType(itemLocationType type, BYTE hands)
{
	switch (type) 
  {
		case WeaponHand:
			readyWeaponHand = NO_READY_ITEM;
			if (hands > 1)
				readyShieldHand = NO_READY_ITEM;
			break;
		case ShieldHand:
			readyShieldHand = NO_READY_ITEM;
			if (hands > 1)
				readyWeaponHand = NO_READY_ITEM;
			break;
		case BodyArmor:
			readyArmor = NO_READY_ITEM;
			break;
		case Hands:
			readyGauntlets = NO_READY_ITEM;
			break;
		case Head:
			readyHelmet = NO_READY_ITEM;
			break;
		case Waist:
			readyBelt = NO_READY_ITEM;
			break;
		case BodyRobe:
			readyRobe = NO_READY_ITEM;
			break;
		case Back:
			readyCloak = NO_READY_ITEM;
			break;
		case Feet:
			readyBoots = NO_READY_ITEM;
			break;
		case Fingers:
			if (readyRing1 == item) 
        readyRing1 = NO_READY_ITEM;
      else if (readyRing2 == item)
        readyRing2 = NO_READY_ITEM;
			break;
		case AmmoQuiver:
			readyMissileAmmo = NO_READY_ITEM;
			break;
		default:
			WriteDebugString("BOGUS body location %i in READY_ITEMS::ClearReady()\n", 
                       type);
			return FALSE;
  }
  return TRUE;
}
*/

//*****************************************************************************
// NAME: READY_ITEMS::Ready
//
// PURPOSE:  
//*****************************************************************************
/*
BOOL READY_ITEMS::Ready(int item, itemLocationType type, BYTE hands)
{
	switch (type) 
  {
		case WeaponHand:
			readyWeaponHand = item;
			if (hands > 1)
				readyShieldHand = item;
			break;

		case ShieldHand:
			readyShieldHand = item;
			if (hands > 1)
				readyWeaponHand = item;
			break;

		case BodyArmor:
			readyArmor = item;
			break;

		case Hands:
			readyGauntlets = item;
			break;

		case Head:
			readyHelmet = item;
			break;

		case Waist:
			readyBelt = item;
			break;

		case BodyRobe:
			readyRobe = item;
			break;

		case Back:
			readyCloak = item;
			break;

		case Feet:
			readyBoots = item;
			break;

		case Fingers:
			if (readyRing1 == NO_READY_ITEM) 
        readyRing1 = item;
      else if (readyRing2 == NO_READY_ITEM)
        readyRing2 = item;
			break;

		case AmmoQuiver:
			readyMissileAmmo = item;
			break;

		default:
			WriteDebugString("BOGUS body location %i in READY_ITEMS::Ready\n", 
                       type);
			return FALSE;
	}

  return TRUE;
}
*/
//*****************************************************************************
// NAME: READY_ITEMS::UnReady
//
// PURPOSE:  
//*****************************************************************************
/*
BOOL READY_ITEMS::UnReady(int item)
{

  // items can be assigned to one, possibly two, slots
	if (readyWeaponHand == item)
		readyWeaponHand = NO_READY_ITEM;

	if (readyShieldHand == item)
		readyShieldHand = NO_READY_ITEM;

	if (readyMissileAmmo == item)
		readyMissileAmmo = NO_READY_ITEM;

	if (readyArmor == item)
		readyArmor = NO_READY_ITEM;

	if (readyGauntlets == item)
		readyGauntlets = NO_READY_ITEM;

	if (readyHelmet == item)
		readyHelmet = NO_READY_ITEM;

	if (readyBelt == item)
		readyBelt = NO_READY_ITEM;

	if (readyRobe == item)
		readyRobe = NO_READY_ITEM;

	if (readyCloak == item)
		readyCloak = NO_READY_ITEM;

	if (readyBoots == item)
		readyBoots = NO_READY_ITEM;

	if (readyRing1 == item)
		readyRing1 = NO_READY_ITEM;

	if (readyRing2 == item)
		readyRing2 = NO_READY_ITEM;

  return TRUE;
}
*/

//*****************************************************************************
// NAME: READY_ITEMS::operator=
//
// PURPOSE:  
//*****************************************************************************
READY_ITEMS& READY_ITEMS::operator=(const READY_ITEMS& src)
{
  if (&src == this) return *this;
	m_readyWeaponHand = src.m_readyWeaponHand;
	m_readyShieldHand = src.m_readyShieldHand;
	m_readyMissileAmmo = src.m_readyMissileAmmo;
	m_readyArmor = src.m_readyArmor;
	m_readyGauntlets = src.m_readyGauntlets;
	m_readyHelmet = src.m_readyHelmet;
	m_readyBelt = src.m_readyBelt;
	m_readyRobe = src.m_readyRobe;
	m_readyCloak = src.m_readyCloak;
	m_readyBoots = src.m_readyBoots;
	m_readyRing1 = src.m_readyRing1;
	m_readyRing2 = src.m_readyRing2;  
  return *this;
}

//*****************************************************************************
//    NAME: READY_ITEMS::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL READY_ITEMS::operator==(const READY_ITEMS& src)
{
  if (&src == this) return TRUE;
	if (m_readyWeaponHand != src.m_readyWeaponHand) return FALSE;
	if (m_readyShieldHand != src.m_readyShieldHand) return FALSE;
	if (m_readyMissileAmmo != src.m_readyMissileAmmo) return FALSE;
	if (m_readyArmor != src.m_readyArmor) return FALSE;
	if (m_readyGauntlets != src.m_readyGauntlets) return FALSE;
	if (m_readyHelmet != src.m_readyHelmet) return FALSE;
	if (m_readyBelt != src.m_readyBelt) return FALSE;
	if (m_readyRobe != src.m_readyRobe) return FALSE;
	if (m_readyCloak != src.m_readyCloak) return FALSE;
	if (m_readyBoots != src.m_readyBoots) return FALSE;
	if (m_readyRing1 != src.m_readyRing1) return FALSE;
	if (m_readyRing2 != src.m_readyRing2) return FALSE;
  return TRUE;
}

//*****************************************************************************
// NAME: READY_ITEMS::Clear
//
// PURPOSE:  
//*****************************************************************************
void READY_ITEMS::Clear()
{
	m_readyWeaponHand = NO_READY_ITEM;
	m_readyShieldHand = NO_READY_ITEM;
	m_readyMissileAmmo = NO_READY_ITEM;
	m_readyArmor = NO_READY_ITEM;
	m_readyGauntlets = NO_READY_ITEM;
	m_readyHelmet = NO_READY_ITEM;
	m_readyBelt = NO_READY_ITEM;
	m_readyRobe = NO_READY_ITEM;
	m_readyCloak = NO_READY_ITEM;
	m_readyBoots = NO_READY_ITEM;
	m_readyRing1 = NO_READY_ITEM;
	m_readyRing2 = NO_READY_ITEM;
}

//*****************************************************************************
// NAME: READY_ITEMS::Serialize
//
// PURPOSE:  
//*****************************************************************************
void READY_ITEMS::Serialize(CArchive &ar)
{
//  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
	  ar << m_readyWeaponHand;
	  ar << m_readyShieldHand;
	  ar << m_readyMissileAmmo;
	  ar << m_readyArmor;
	  ar << m_readyGauntlets;
	  ar << m_readyHelmet;
	  ar << m_readyBelt;
	  ar << m_readyRobe;
	  ar << m_readyCloak;
	  ar << m_readyBoots;
	  ar << m_readyRing1;
	  ar << m_readyRing2;
  }
  else
  {
	  ar >> m_readyWeaponHand;
	  ar >> m_readyShieldHand;
	  ar >> m_readyMissileAmmo;
	  ar >> m_readyArmor;
	  ar >> m_readyGauntlets;
	  ar >> m_readyHelmet;
	  ar >> m_readyBelt;
	  ar >> m_readyRobe;
	  ar >> m_readyCloak;
	  ar >> m_readyBoots;
	  ar >> m_readyRing1;
	  ar >> m_readyRing2;
  }
}
//*****************************************************************************
//    NAME: READY_ITEMS::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void READY_ITEMS::Serialize(CAR &ar)
{
  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
	  ar << m_readyWeaponHand;
	  ar << m_readyShieldHand;
	  ar << m_readyMissileAmmo;
	  ar << m_readyArmor;
	  ar << m_readyGauntlets;
	  ar << m_readyHelmet;
	  ar << m_readyBelt;
	  ar << m_readyRobe;
	  ar << m_readyCloak;
	  ar << m_readyBoots;
	  ar << m_readyRing1;
	  ar << m_readyRing2;
  }
  else
  {
	  ar >> m_readyWeaponHand;
	  ar >> m_readyShieldHand;
	  ar >> m_readyMissileAmmo;
	  ar >> m_readyArmor;
	  ar >> m_readyGauntlets;
	  ar >> m_readyHelmet;
	  ar >> m_readyBelt;
	  ar >> m_readyRobe;
	  ar >> m_readyCloak;
	  ar >> m_readyBoots;
	  ar >> m_readyRing1;
	  ar >> m_readyRing2;
  }
}

BOOL ITEM_DATA::IsUsableByClass(const CLASS_ID& classID) const
{
  {
    int i,n;
    const CLASS_DATA *pClass;
    pClass = classData.PeekClass(classID);
    if (pClass == NULL) return FALSE;
    n = GetBaseclassCount();
    for (i=0; i<n; i++)
    {
      const BASECLASS_ID *pBaseclassID;
      pBaseclassID = PeekBaseclass(i);
      {
        if (pClass->LocateBaseclass(*pBaseclassID) >= 0)
        {
          return TRUE;
        };
      };
    };
  };
  return FALSE;
}



//*****************************************************************************
// NAME: ITEM_LIST::CanReady
//
// PURPOSE:  
//*****************************************************************************
//BOOL ITEM_LIST::CanReady(int index, BYTE ItemMask, CHARACTER *pChar)
miscErrorType ITEM_LIST::CanReady(int index, CHARACTER *pChar)
{
  POSITION pos;
  const ITEM *pCharItem;
  if ((pos = m_items.FindKeyPos(index)) == NULL)
    return UnknownError;

  pCharItem = &m_items.PeekAtPos(pos);

  //if (IsMoneyItem(pItem->m_giID))
  if (IsMoneyItem(pCharItem->itemID))
    return UnknownError;

  if (pCharItem->ready)
    return NoError;

  if (pCharItem->qty <= 0)
    return UnknownError;

  //ITEM_DATA *data = itemData.GetItemData(pItem->m_giID);
  const ITEM_DATA *pItem = itemData.GetItem(pCharItem->itemID);
  if (pItem == NULL)
    return UnknownError;

  // dont handle items using more than 2 hands yet
  if (pItem->Hands_to_Use > 2)
    return UnknownError;


/*
	if ((pItem->Usable_by_Class & ItemMask) == 0)
*/ 
  if (!pItem->IsUsableByClass(pChar->GetClass()))
	{
		return WrongClass;
	}

  if (itemUsesRdySlot(pItem))
  {
	  if (   (pItem->Hands_to_Use == 2) 
        && ((pItem->Location_Readied == WeaponHand) || (pItem->Location_Readied == ShieldHand)))
	  {
		  //if (   (rdyItems.GetReady(WeaponHand) != NO_READY_ITEM)
			//    || (rdyItems.GetReady(ShieldHand) != NO_READY_ITEM))
		  if (   (GetReadiedItem(WeaponHand, 0) != NO_READY_ITEM)
			    || (GetReadiedItem(ShieldHand, 0) != NO_READY_ITEM))
      {
			  return TakesTwoHands;
      }
	  }
    else if (pItem->Hands_to_Use > 0)
    {
      int readiedItem, hand;
      itemReadiedType location;
      //if (   (rdyItems.GetReady(WeaponHand) != NO_READY_ITEM)
      //    && (rdyItems.GetReady(ShieldHand) != NO_READY_ITEM))
      for (hand=0,location=WeaponHand; hand<2; hand++,location=ShieldHand)
      {
        readiedItem = GetReadiedItem(location, 0);
        if (readiedItem != NO_READY_ITEM)
        {
          POSITION readiedPos;
          readiedPos = m_items.FindKeyPos(readiedItem);
          if (readiedPos!=NULL)
          {
            const ITEM *pCharReadiedItem;
            ITEM_DATA *pReadiedItem;
            pCharReadiedItem = &m_items.PeekAtPos(readiedPos);
            //pReadiedData = itemData.GetItemData(pReadiedItem->m_giID);
            pReadiedItem = itemData.GetItem(pCharReadiedItem->itemID);
            if (pReadiedItem != NULL)
            {
              if (pReadiedItem->Hands_to_Use > 1)
              {
                return NoFreeHands;
              };
            };
          };
        };
      };
    }

    // something in this slot already
    //if (!rdyItems.CanReady(data->Location_Carried))
    if (!CanReady(pItem->Location_Readied, pChar, pCharItem))
    {
      {
        return ItemAlreadyReadied;
      };
    }
  }

  return NoError;
}


//*****************************************************************************
// NAME: ITEM_LIST::Ready
//
// PURPOSE:  
//*****************************************************************************
//BOOL ITEM_LIST::Ready(const ITEM_ID& itemID, itemReadiedType ilt, BYTE itemMask, CHARACTER *pChar)
BOOL ITEM_LIST::Ready(const ITEM_ID& itemID, itemReadiedType ilt, CHARACTER *pChar)
{
  POSITION pos;
  ITEM *pItem;
  ITEM_DATA *pItemData;
  //if (name.IsEmpty())
  if (itemID.IsNoItem())
  {
    int index;
    //index = rdyItems.GetReady(ilt);
    index = GetReadiedItem(ilt, 0);
    if (ilt < 0) return FALSE;
    UnReady(index);
    return TRUE;
  };
  pos = GetHeadPosition();
  while (pos != NULL)
  {
    pItem = &GetNext(pos);
    //pItemData = itemData.GetItemData(pItem->m_giID);
    pItemData = itemData.GetItem(pItem->itemID);
    if (pItem == NULL) continue;
    if (pItemData->UniqueName() == itemID)
    {
      //return Ready(pItem->key, itemMask, pChar);
      return Ready(pItem->key, pChar);
    }
  };
  return FALSE;
}


//*****************************************************************************
// NAME: ITEM_LIST::Ready
//
// PURPOSE:  
//*****************************************************************************
//BOOL ITEM_LIST::Ready(int index, BYTE ItemMask, CHARACTER *pChar)
BOOL ITEM_LIST::Ready(int index, CHARACTER *pChar)
{
  ITEM_DATA *data = itemData.GetItem(GetItem(index));
  if (data == NULL) return FALSE;

  //if (!CanReady(index, ItemMask, pChar)) return FALSE;
  if (CanReady(index, pChar) != NoError) return FALSE;

  BOOL result = TRUE;
  //if (itemUsesRdySlot(data))
  //  result = rdyItems.Ready(index, data->Location_Carried, data->Hands_to_Use);
  if (result)
    SetReady(index, TRUE);
  return (IsReady(index));
}

//*****************************************************************************
//    NAME: ITEM_LIST::ClearReady
//
// PURPOSE: 
//
//*****************************************************************************
/*
BOOL ITEM_LIST::ClearReadyByType(itemLocationType type);
{  
  rdyItems.ClearReady(data->Location_Carried,);
  SetReady(item, FALSE);
  return TRUE;
}*/



//*****************************************************************************
// NAME: ITEM_LIST::UnReady
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::UnReady(int item)
{
  if (!IsReady(item)) return TRUE;
  if (!CanUnReady(item)) return FALSE;
//  if (itemUsesRdySlot(GetItem(item)))
//    rdyItems.UnReady(item);
  SetReady(item, FALSE);
  return TRUE;
}

//*****************************************************************************
// NAME: CanUnReady
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::CanUnReady(int item)
{
  if (!ValidItemListIndex(item))
    return FALSE;
  if (!IsReady(item)) 
    return FALSE;
  return (!IsCursed(item));
}

//*****************************************************************************
// NAME: ITEM_LIST::Serialize
//
// PURPOSE:  
//*****************************************************************************
void ITEM_LIST::Serialize(CArchive &ar, double ver)
{
//  CObject::Serialize(ar);
  int count;
  POSITION pos;
  ITEM data;

  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = GetHeadPosition();
    while (pos != NULL)
      GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Serialize(ar, ver);
      AddItemWithCurrKey(data);
    }
  }

  rdyItems.Serialize(ar);
}

//*****************************************************************************
//    NAME: ITEM_LIST::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::Serialize(CAR &ar, double ver)
{
  //CObject::Serialize(ar);
  int count;
  ITEM data;
  POSITION pos;

  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = GetHeadPosition();
    while (pos != NULL)
      GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    ar >> count;
    for (int i=0;i<count;i++)
    {
      ITEM_DATA *pItem;
      data.Serialize(ar, ver);
      //pItem = itemData.GetItemData(data.m_giID);
      pItem = itemData.GetItem(data.itemID);
      if (pItem != NULL)
      {
        data.charges = pItem->Num_Charges;
        AddItemWithCurrKey(data);
      }
      else
      {
        debugSeverity = 7;
        WriteDebugString("Undefined item named %s\n", data.itemID);
      };
    }
  }

  rdyItems.Serialize(ar);
}

#ifdef UAFEDITOR
extern const char *JKEY_POSSESSIONS;

void ITEM_LIST::Export(JWriter& jw, const char *name)
{
  //CObject::Serialize(ar);
  ITEM data;
  POSITION pos;
  jw.StartArray(name);
  {
    pos = GetHeadPosition();
    while (pos != NULL)
    {
      ITEM *pItem;
      pItem = &GetNext(pos);
      jw.NextEntry();
      pItem->Export(jw);
    };
  };
  jw.EndArray();
  //No longer needed!   rdyItems.Export(jw);
}
bool ITEM_LIST::Import(JReader& jr, const char *name)
{
  //CObject::Serialize(ar);
  ITEM data;
  if (jr.Optional(), !jr.StartArray(name))return false;
  while (jr.Optional(), jr.NextEntry())
  {
    data.Import(jr);
    m_items.Insert(data, 0);
  };
  jr.EndArray();
  //No longer needed!   rdyItems.Export(jw);
  return true;
}
#endif



#ifdef UAFEDITOR

void ITEM_LIST::CrossReference(CR_LIST *pCRList, const CR_REFERENCE *pCRReference) const
{
  /* ************************************************
   * For now, we will not check that 'readied' items exist in the
   * item list
   *************************************************
  pCRList->CR_AddItemReference(this->rdyItems.readyWeaponHand,  pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyShieldHand,  pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyMissileAmmo, pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyArmor,       pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyGauntlets,   pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyHelmut,      pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyBelt,        pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyRobe,        pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyCloak,       pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyBoots,       pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyRing1,       pCRReference);
  pCRList->CR_AddItemReference(this->rdyItems.readyRing2,       pCRReference);
  */
  {
    POSITION pos;
    for (pos=m_items.GetHeadPosition(); pos!=NULL; m_items.PeekNext(pos))
    {
      const ITEM *pItem;
      pItem = &m_items.PeekAtPos(pos);
      //pCRList->CR_AddItemReference(pItem->m_giID, pCRReference);
      pCRList->CR_AddItemReference(pItem->itemID, pCRReference);
    };
  };

}

#endif

//*****************************************************************************
// NAME: ITEM_LIST::GetProtectModForRdyItems
//
// PURPOSE:  
//*****************************************************************************
int ITEM_LIST::GetProtectModForRdyItems() const
{
  int acMod = 0; 
  POSITION pos;
  pos = m_items.GetHeadPosition();
  while (pos != NULL)
  {
    if (PeekAtPos(pos).ready)
    {
      //ITEM_DATA *data = itemData.GetItemData(PeekAtPos(pos).m_giID);
      ITEM_DATA *data = itemData.GetItem(PeekAtPos(pos).itemID);
      if (data != NULL)
		    acMod += (data->Protection_Base + data->Protection_Bonus);
    }
    PeekNext(pos);
  }
  return acMod;
}


//*****************************************************************************
// NAME: ITEM_LIST::GetAttackModForRdyItems
//
// PURPOSE:  
//*****************************************************************************
int ITEM_LIST::GetAttackModForRdyItems() const
{
  int mod = 0; 
  POSITION pos;
  pos = m_items.GetHeadPosition();
  while (pos != NULL)
  {
    if (PeekAtPos(pos).ready)
    {
      //ITEM_DATA *data = itemData.GetItemData(PeekAtPos(pos).m_giID);
      ITEM_DATA *data = itemData.GetItem(PeekAtPos(pos).itemID);
      if (data != NULL)
		    mod += data->Attack_Bonus;
    }
    PeekNext(pos);
  }

  return mod;
}

#ifdef UAFEngine

//*****************************************************************************
//    NAME: ITEM_TEXT_LIST::UpdateDetectMagic
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_TEXT_LIST::UpdateDetectMagic(BOOL detecting)
{
  DetectingMagic = detecting;
}

//*****************************************************************************
//    NAME: ITEM_TEXT_LIST::UpdateIsReady
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_TEXT_LIST::UpdateIsReady(int index, BOOL ready) 
{ 
  items[index].ready = ready;
  DetectingMagic = party.PartyIsDetectingMagic();
}

//*****************************************************************************
// NAME: ITEM_TEXT_LIST::FillItemListText
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::FillItemListText(ITEM_LIST &list, UINT flags, costFactorType type,
                                      BOOL (*pFilter)(ITEM *pItem, void *pFilterPkt), void *pFilterPkt)
{
  CString itemName;
	Clear();

  UseQty  = flags & QTY;
  UseRdy  = flags & RDY;
  UseCost = flags & COST;
  UseMoney = flags & MONEY;

  DetectingMagic = flags & DETECTING_MAGIC;
  CostFactor = type;

  POSITION pos = list.GetHeadPosition();
	while (pos != NULL)
	{
    //GLOBAL_ITEM_ID giID;
    ITEM_ID itemID;
    ITEM_DATA *pItem;
    //giID = list.GetAtPos(pos).m_giID;
    itemID = list.GetAtPos(pos).itemID;
    //if (giID.IsValidItem())
    {
      //pItem = itemData.GetItemData(giID);
      pItem = itemData.GetItem(itemID);
      if (pItem != NULL)
      {
        if ( (pFilter==NULL) || pFilter(&list.GetAtPos(pos), pFilterPkt) )
        {
          Add(list.PeekAtPos(pos).qty,
              itemType, 
              NULL, 
              list.PeekAtPos(pos).key,
              list.PeekAtPos(pos).identified,
              list.PeekAtPos(pos).charges,
              //giID,
              itemID,
              list.PeekAtPos(pos).ready);
        };
      };
    };
    list.GetNext(pos);
  }

  FormatItemListTitle();
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::AddItemListText
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::AddItemListText(ITEM_LIST &list, UINT flags, costFactorType type, int data)
{
  UseQty  = flags & QTY;
  UseRdy  = flags & RDY;
  UseCost = flags & COST;
  UseMoney = flags & MONEY;

  DetectingMagic = flags & DETECTING_MAGIC;
  CostFactor = type;

  POSITION pos = list.GetHeadPosition();
	while (pos != NULL)
	{
    Add(list.PeekAtPos(pos).qty,
        itemType, 
        NULL, 
        list.PeekAtPos(pos).key,
        list.PeekAtPos(pos).identified,
        list.PeekAtPos(pos).charges,
        //list.PeekAtPos(pos).m_giID,
        list.PeekAtPos(pos).itemID,
        list.PeekAtPos(pos).ready,
        data);

    list.GetNext(pos);
  }

  FormatItemListTitle();
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::AddItemText
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::AddItemText(ITEM &addItem, UINT flags, costFactorType type)
{
  UseQty  = flags & QTY;
  UseRdy  = flags & RDY;
  UseCost = flags & COST;
  UseMoney = flags & MONEY;

  DetectingMagic = flags & DETECTING_MAGIC;
  CostFactor = type;

	//if (addItem.m_giID.IsValidItem())
	if (addItem.itemID.IsValidItem())
	{
    Add(addItem.qty, 
        itemType, 
        NULL, 
        0,
        addItem.identified,
        addItem.charges,
        //addItem.m_giID,
        addItem.itemID,
        addItem.ready);
	}

  FormatItemListTitle();
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::Add
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::Add(int qty, itemClassType type, const char *label,
                         int origIndex, BOOL id, int charges, 
                         //GLOBAL_ITEM_ID giID, BOOL ready, int data)
                         const ITEM_ID& itemID, BOOL ready, int data)
{
  if (qty <= 0)
    return;

  ITEM_TEXT newItem;
  newItem.charges = charges;    
  newItem.itemClass = type;
  newItem.qty = qty;
  newItem.origIndex = origIndex;
  //newItem.m_giID = giID;  
  newItem.itemID = itemID;  
  newItem.identified = id;
  newItem.ready = ready;
  newItem.data = data; // data supplied for later identification of item context
  if (label != NULL)
    newItem.label = label;
  else
    newItem.label = "";
  //if (
  //ASSERT(giID.IsValidItem());
  //if (giID.IsValidItem())
  //{
      items.Add(newItem);
  //};
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::ItemCanBeHalved
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_TEXT_LIST::ItemCanBeHalved(int index)
{
  itemClassType type = GetItemClass(index);
  //GLOBAL_ITEM_ID giID = GetItemIndex(index);
  ITEM_ID itemID = GetItemID(index);

  if (   (itemCanBeHalved(type))
      //&& (itemCanBeHalved(giID)))
      && (itemCanBeHalved(itemID)))
    return TRUE;
  else
    return FALSE;
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::ItemCanBeJoined
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_TEXT_LIST::ItemCanBeJoined(int index)
{
  return (   (itemCanBeJoined(GetItemClass(index)))
          && (itemCanBeJoined(GetItemID(index))));
}

//*****************************************************************************
// NAME: ITEM_TEXT_LIST::FormatItemListTitle
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::FormatItemListTitle(const char *EmptyListTitle)
{
  //const int width = 6;
  
  RdyOffset=0;
  QtyOffset=0;
  CostOffset=0;
  NameOffset=0;

  int RdyColWidth=0;
  int QtyColWidth=0;
  int CostColWidth=0;

  if (GetCount() <= 0)
  {
    if (EmptyListTitle != NULL)
      title = EmptyListTitle;
    else
      title = "No Items";
    return;
  }

	char tmp[300];
	memset(tmp, ' ', sizeof(tmp));

	if (UseRdy)
    RdyColWidth =  GraphicsMgr.GetTextWidth("Rdy   ", -1);

	if (UseQty)
    QtyColWidth =  GraphicsMgr.GetTextWidth("Qty   ",-1);

	if (UseCost)
    CostColWidth = GraphicsMgr.GetTextWidth("Cost  ",-1);

  int PixelWidth = GraphicsMgr.GetTextWidth(" ", 1) + 1;

  RdyOffset  = 0; // will always start in col 0 if used
  QtyOffset  = RdyOffset  + (RdyColWidth  / PixelWidth);
  CostOffset = QtyOffset  + (QtyColWidth  / PixelWidth);
  NameOffset = CostOffset + (CostColWidth / PixelWidth);

  if (UseRdy)
    memmove(&tmp[RdyOffset], "Rdy", 3);
  if (UseQty)
    memmove(&tmp[QtyOffset], "Qty", 3);
  if (UseCost)
    memmove(&tmp[CostOffset], "Cost", 4);

	strcpy(&tmp[NameOffset], "Item");

	title = tmp;
}

char *ITEM_TEXT_LIST::GetItemRdyText(int index)
{
  if (!UseRdy)
    return NULL;  
  if (items[index].itemClass != itemType)
    return NULL;

  //if (ITEM_LIST::IsMoneyItem(items[index].m_giID))
  if (ITEM_LIST::IsMoneyItem(items[index].itemID))
    return NULL;

	if (items[index].ready)
		strcpy(temp, " Yes");
	else
		strcpy(temp, " No ");
  return temp;
}

//*****************************************************************************
//    NAME: *ITEM_TEXT_LIST::GetItemQtyText
//
// PURPOSE: 
//
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemQtyText(int index)
{
  if (!UseQty)
    return NULL;

  if (GetRunAsVersion(SpecialItemKeyQtyVersion) >= _VERSION_0820_)
  {
    if (   (items[index].itemClass == SpecialItemType)
        || (items[index].itemClass == SpecialKeyType)
        || (items[index].itemClass == QuestType))
      return NULL;
  }

	sprintf(temp,"%9i", items[index].qty);
  return temp;
}

//*****************************************************************************
//    NAME: *ITEM_TEXT_LIST::GetItemCostText
//
// PURPOSE: 
//
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemCostText(int index)
{
  if (!UseCost)
    return NULL;

  if (items[index].itemClass != itemType)
    return NULL;

  DWORD cost;

  //if (ITEM_LIST::IsMoneyItem(items[index].m_giID))
  if (ITEM_LIST::IsMoneyItem(items[index].itemID))
  {
    cost = items[index].charges;
  }
  else
  {
    //long iprice = itemData.GetItemCost(items[index].m_giID);
    long iprice = itemData.GetItemCost(items[index].itemID);
    cost = ApplyCostFactor(CostFactor, iprice);
  }
	sprintf(temp,"%9i", cost);
  return temp;
}

//*****************************************************************************
//    NAME: *ITEM_TEXT_LIST::GetItemNameText
//
// PURPOSE: 
//
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemNameText(int index)
{
  if (items[index].itemClass == itemType)
  {
    //if (ITEM_LIST::IsMoneyItem(items[index].m_giID))
    if (ITEM_LIST::IsMoneyItem(items[index].itemID))
    {
      //if (items[index].m_giID.IsGemType())
      if (items[index].itemID.IsGemType())
  		  sprintf(temp, "%s (%u %s)", globalData.moneyData.GetName(GemType), 
                items[index].charges,
                globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
      else
        sprintf(temp, "%s (%u %s)", globalData.moneyData.GetName(JewelryType),
                items[index].charges,
                globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
    }
    else
    {
      //if ((DetectingMagic) && (isMagical(items[index].m_giID)))
      if ((DetectingMagic) && (isMagical(items[index].itemID)))
      {
        //if ((items[index].identified) && (!itemData.GetItemIdName(items[index].m_giID).IsEmpty()))
        if ((items[index].identified) && (!itemData.GetItemIdName(items[index].itemID).IsEmpty()))
        {
          strcpy(temp, "*");
          //strncat(temp,(LPCSTR)itemData.GetItemIdName(items[index].m_giID) , MAX_ITEM_TEXT_LEN);
          strncat(temp,(LPCSTR)itemData.GetItemIdName(items[index].itemID) , MAX_ITEM_TEXT_LEN);
          temp[MAX_ITEM_TEXT_LEN]='\0';
  		    //sprintf(temp, "*%s",
	  			//        (LPCSTR)itemData.GetItemIdName(items[index].item));
        }
        else
        {
          strcpy(temp, "*");
          //strncat(temp,(LPCSTR)itemData.GetItemCommonName(items[index].m_giID) , MAX_ITEM_TEXT_LEN);
          strncat(temp,(LPCSTR)itemData.GetItemCommonName(items[index].itemID) , MAX_ITEM_TEXT_LEN);
          temp[MAX_ITEM_TEXT_LEN]='\0';
  		    //sprintf(temp, "*%s",
	  			//        (LPCSTR)itemData.GetItemName(items[index].item));
        }
      }
      else 
      {
        {
          ITEM_DATA *pItem;
          //pItem = itemData.GetItemData(items[index].m_giID);
          pItem = itemData.GetItem(items[index].itemID);
          if (pItem != NULL)
          {
            temp[0]=0;
            if (pItem->IsUsable() || pItem->IsScribable())
            {
              if (items[index].charges == 0)
              {
                strcat(temp, "#");
              };
            };
          };
        };
        //if ((items[index].identified) && (!itemData.GetItemIdName(items[index].m_giID).IsEmpty()))
        if ((items[index].identified) && (!itemData.GetItemIdName(items[index].itemID).IsEmpty()))
        {
			    //strncat(temp, itemData.GetItemIdName(items[index].m_giID), MAX_ITEM_TEXT_LEN-1);
			    strncat(temp, itemData.GetItemIdName(items[index].itemID), MAX_ITEM_TEXT_LEN-1);
          temp[MAX_ITEM_TEXT_LEN]='\0';
        }
        else
        {
			    //strncat(temp, itemData.GetItemCommonName(items[index].m_giID), MAX_ITEM_TEXT_LEN-1);
			    strncat(temp, itemData.GetItemCommonName(items[index].itemID), MAX_ITEM_TEXT_LEN-1);
          temp[MAX_ITEM_TEXT_LEN]='\0';
        }
      }
    }
  }
  else
  {
    strncpy(temp, items[index].label, MAX_ITEM_TEXT_LEN);
    temp[MAX_ITEM_TEXT_LEN]='\0';
  }
  return temp;
}

//*****************************************************************************
// NAME: *ITEM_TEXT_LIST::GetItemText
//
// PURPOSE:  
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemText(int index)
{
  memset(text,' ',sizeof(text));
  char *tmp;

  if (UseRdy)
  {
    tmp = GetItemRdyText(index);
    if (tmp)
		  memmove(&text[RdyOffset], tmp, strlen(tmp));
  }

	if (UseQty)
  {
    tmp = GetItemQtyText(index);
    if (tmp)
		  memmove(&text[QtyOffset], tmp, strlen(tmp));
  }

  if (UseCost)
  {
    tmp = GetItemCostText(index);
    if (tmp)
      memmove(&text[CostOffset], tmp, strlen(tmp));
  }

  int buffRemaining = MAX_ITEM_TEXT_LEN-NameOffset;
  strncpy(&text[NameOffset], GetItemNameText(index), buffRemaining);

  return text;
}

#endif

void ITEM_DATA::SetIdName(const CString& name)
{
  m_idName = name;
}

void ITEM_DATA::SetUniqueName(const CString& name)
{
  m_uniqueName = name;
}

//*****************************************************************************
// NAME: ITEM_DATA::Serialize
//
// PURPOSE:  
//*****************************************************************************
void ITEM_DATA::Serialize(CArchive &ar, double ver)
{
//  CObject::Serialize(ar);
  long temp;

  if (ar.IsStoring())
  {
    die("We should not be serializing itemdata with CArchive");
    DWORD uTemp;
    if (ver <= _VERSION_0830_)  m_idName.Replace('/','|');
    //m_giID.Serialize(ar); //ar << key;
    AS(ar,m_uniqueName);
    AS(ar,m_idName);

    StripFilenamePath(HitSound);
    StripFilenamePath(MissSound);
    StripFilenamePath(LaunchSound);

    AS(ar, HitSound);
    AS(ar,MissSound);
    AS(ar,LaunchSound);



    AS(ar,AmmoType);
    ar << Experience;
    ar << Cost;
    ar << Encumbrance;
    ar << Attack_Bonus;
    ar << Cursed;
    ar << Bundle_Qty;
    ar << Num_Charges;
    uTemp = Location_Readied;
    ar << uTemp;
    ar << Hands_to_Use;
    ar << Dmg_Dice_Sm;
    ar << Nbr_Dice_Sm;
    ar << Dmg_Bonus_Sm;
    ar << Dmg_Dice_Lg;
    ar << Nbr_Dice_Lg;
    ar << Dmg_Bonus_Lg;
    ar << ROF_Per_Round;
    ar << Protection_Base;
    ar << Protection_Bonus;

    temp = Wpn_Type;
    ar << temp;
    ar << m_usageFlags;
    //ar << Usable_by_Class;
    {
      int i, n;
      n = usableByBaseclass.GetSize();
      ar << n;
      for (i=0; i<n; i++)
      {
        ar << *GetBaseclass(i);
      }
    }
    ar << RangeMax;
    //m_gsID.Serialize(ar, ver); //ar << Spell;
    //ar << Spell_Level;
    //ar << Spell_Class;
    ar << m_useEvent;
    ar << ExamineEvent;
    AS(ar, ExamineLabel);      
    AS(ar, attackMsg);
    ar << (int)Recharge_Rate;
    ar << IsNonLethal;
#ifdef SIMPLE_STRUCTURE
    HitArt.Serialize(ar,ver, rte.CombatArtDir());
#else
    HitArt.Serialize(ar,ver);
#endif
    ar << CanBeHalvedJoined;
    ar << CanBeTradeDropSoldDep;
    // MissileArt is serialized in attribute map
  }
  else
  {
    DWORD uTemp;
    //m_giID.Serialize(ar); //ar >> key;
    die("We should look for a key in very old itemdata");
    DAS(ar,m_uniqueName);

#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Battle Axe|Dull" will be
    // read as simply "Battle Axe".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    //
    //  PRS 20110107
    // Sorry about that.  We shall be using the unique names
    // in scripts.  Scripts need to uniquely fetch and compare names.
    // What we are going to do here is to make the name a private member,
    // provide a function to get the unique name and a function
    // to get the ID name and a function to get the common name.
    //if (m_uniqueName.Find('|') >= 0) m_uniqueName = m_uniqueName.Left(m_uniqueName.Find('|')); // Remove qualifier.
    m_commonName = m_uniqueName;
    if (m_commonName.Find('|') >= 0) m_commonName = m_commonName.Left(m_commonName.Find('|')); // Remove qualifier.
#endif

    DAS(ar,m_idName);


    DAS(ar,HitSound);
    DAS(ar,MissSound);

    //AddFolderToPath(HitSound,    rte.SoundDir());
    //AddFolderToPath(MissSound,   rte.SoundDir());


    if (ver >= _VERSION_05691_)
    {
      DAS(ar,LaunchSound);
      //AddFolderToPath(LaunchSound, rte.SoundDir());
    }

    if (globalData.version < 0.930279)
    {
      StripFilenamePath(HitSound);
      StripFilenamePath(MissSound);
      StripFilenamePath(LaunchSound);
    };

    if (ver >= _VERSION_0690_)
      DAS(ar,AmmoType);
    if (AmmoType.CompareNoCase("None") == 0)
    {
      AmmoType = "";
    };
    ar >> Experience;
    ar >> Cost;
    ar >> Encumbrance;
    ar >> Attack_Bonus;
    ar >> Cursed;
    ar >> Bundle_Qty;
    ar >> Num_Charges;
  
    ar >> uTemp;
    switch(uTemp)
    {
      case 0:  uTemp = WeaponHand;  break;  // Sword								
      case 1:  uTemp = ShieldHand;  break;  // Shield
	    case 2:  uTemp = BodyArmor;   break;  // Armor
	    case 3:  uTemp = Hands;       break;  // Guantlets
	    case 4:  uTemp = Head;        break;  // Helm
	    case 5:  uTemp = Waist;       break;  // Belt
	    case 6:  uTemp = BodyRobe;    break;  // Robe
	    case 7:  uTemp = Back;        break;  // Cloak
	    case 8:  uTemp = Feet;        break;  // Boots
	    case 9:  uTemp = Fingers;     break;  // Rings
	    case 10: uTemp = AmmoQuiver;  break;  // Arrows, Bolts
    };
    Location_Readied = (itemReadiedType)uTemp;

    ar >> Hands_to_Use;
    ar >> Dmg_Dice_Sm;
    ar >> Nbr_Dice_Sm;
    ar >> Dmg_Bonus_Sm;
    ar >> Dmg_Dice_Lg;
    ar >> Nbr_Dice_Lg;
    ar >> Dmg_Bonus_Lg;
    ar >> ROF_Per_Round;
    ar >> Protection_Base;
    ar >> Protection_Bonus;
  
    ar >> temp;
    Wpn_Type = (weaponClassType)temp;

    ar >> m_usageFlags;
    //ar >> Usable_by_Class;
    {
      BASECLASS_ID baseclassID;
      int i, n;
      usableByBaseclass.RemoveAll();
      ar >> n;
      for (i=0; i<n; i++)
      {
        ar >> baseclassID;
        usableByBaseclass.Add(baseclassID);
      }
    }

    ar >> RangeMax;
  
    //m_gsID.Serialize(ar, ver); //ar >> Spell;
    //ar >> Spell_Level;
    //ar >> Spell_Class;

    if (ver >= _VERSION_0662_)
      ar >> m_useEvent;

    if (ver >= _VERSION_0800_)
    {
      ar >> ExamineEvent;
      DAS(ar, ExamineLabel);
    }

    if (ver >= _VERSION_0860_)
    {
      DAS(ar, attackMsg);
    }
    else
      attackMsg="attacks";

    if (ver >= _VERSION_0690_)
    {
      ar >> temp;
      Recharge_Rate = (itemRechargeRate)temp;
      ar >> IsNonLethal;
      HitArt.Serialize(ar,ver, rte.CombatArtDir());
    }
    if (ver >= _VERSION_0881_)
      ar >> CanBeHalvedJoined;
    else
      CanBeHalvedJoined = TRUE;

    if (ver >= _VERSION_0904_)
      ar >> CanBeTradeDropSoldDep;
    else
      CanBeTradeDropSoldDep = TRUE;

    // MissileArt is serialized in attribute map
//    if (ver <
  }

  specAbs.Serialize(ar, ver, this->m_idName, "item");
  item_asl.Serialize(ar, "ITEM_DATA_ATTRIBUTES");
}

#ifdef UAFEDITOR
void ITEM_DATA::AddUsableBaseclass(int usableFlags, int baseclassFlag, const CString& name)
{
  if ((usableFlags & baseclassFlag) == 0) return;
  {
    BASECLASS_ID baseclassID;
    baseclassID = name;
    if (LocateBaseclass(baseclassID) >= 0) return;
    usableByBaseclass.Add(baseclassID);
  };
}
#endif

//*****************************************************************************
//    NAME: ITEM_DATA::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::Serialize(CAR &ar, double ver)
{
  long temp;

  if (ar.IsStoring())
  {
    DWORD uTemp;
    if (ver <= _VERSION_0830_)
      m_idName.Replace('/','|');
    //m_giID.Serialize(ar); //ar << key;
    ar << preSpellNameKey;
    ar << spellID;
    AS(ar,m_uniqueName);
    AS(ar,m_idName);

    StripFilenamePath(HitSound);
    StripFilenamePath(MissSound);
    StripFilenamePath(LaunchSound);
    AS(ar,HitSound);
    AS(ar,MissSound);
    AS(ar,LaunchSound);

    HitArt.Serialize(ar, ver, "");
    MissileArt.Serialize(ar, ver, "");

    AS(ar,AmmoType);
    ar << Experience;
    ar << Cost;
    ar << Encumbrance;
    ar << Attack_Bonus;
    ar << Cursed;
    ar << Bundle_Qty;
    ar << Num_Charges;
    uTemp = Location_Readied;
    ar << uTemp;
    ar << Hands_to_Use;
    ar << Dmg_Dice_Sm;
    ar << Nbr_Dice_Sm;
    ar << Dmg_Bonus_Sm;
    ar << Dmg_Dice_Lg;
    ar << Nbr_Dice_Lg;
    ar << Dmg_Bonus_Lg;
    ar << ROF_Per_Round;
    ar << Protection_Base;
    ar << Protection_Bonus;

    temp = Wpn_Type;
    ar << temp;
    ar << m_usageFlags;
    //ar << Usable_by_Class;
    {
      int i, n;
      n = usableByBaseclass.GetSize();
      ar << n;
      for (i=0; i<n; i++)
      {
        ar << *GetBaseclass(i);
      }
    }
    ar << RangeMax;
    //m_gsID.Serialize(ar, ver); //ar << Spell;
    //ar << Spell_Level;
    //ar << Spell_Class;
    ar << m_useEvent;
    ar << ExamineEvent;
    AS(ar, ExamineLabel);
    AS(ar, attackMsg);
    ar << Recharge_Rate;
    ar << IsNonLethal;
    HitArt.Serialize(ar,ver, rte.CombatArtDir());
    ar << CanBeHalvedJoined;
    ar << CanBeTradeDropSoldDep;
    // MissileArt is serialized in attribute map
  }
  else
  {
    DWORD uTemp;
    //m_giID.Serialize(ar); //ar >> key;
    if ( (ver < VersionSpellNames) || (ver >= VersionSaveIDs) )
    {
      ar >> preSpellNameKey;  // For conversion only.
    }
    else
    {
      preSpellNameKey = -1;
    };
    if (ver >= 0.999647)
    {
      ar >> spellID;
    };
    DAS(ar,m_uniqueName);
#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Battle Axe|Dull" will be
    // read as simply "Battle Axe".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    //if (m_uniqueName.Find('|') >= 0) m_uniqueName = m_uniqueName.Left(m_uniqueName.Find('|')); // Remove qualifier.
    m_commonName = m_uniqueName;
    if (m_commonName.Find('|') >= 0) m_commonName = m_commonName.Left(m_commonName.Find('|')); // Remove qualifier.
#endif
    DAS(ar,m_idName);


    DAS(ar,HitSound);
    DAS(ar,MissSound);
    if (ver >= _VERSION_05691_)
      DAS(ar,LaunchSound);
#ifdef UAFEDITOR
    if (ver > VersionSpellIDs)
#endif
    {
      HitArt.Serialize(ar, ver, "");
      MissileArt.Serialize(ar, ver, "");
    };

    if (globalData.version < 0.930279)
    {
      StripFilenamePath(HitSound);
      StripFilenamePath(MissSound);
      StripFilenamePath(LaunchSound);
    };



#ifdef SIMPLE_STRUCTURE
    //if (HitSound != "")   HitSound     = rte.SoundDir() + HitSound;
    //if (MissSound != "")   MissSound   = rte.SoundDir() + MissSound;
    //if (LaunchSound != "") LaunchSound = rte.SoundDir() + LaunchSound;
#endif

    if (ver >= _VERSION_0690_)
      DAS(ar,AmmoType);
    if (AmmoType.CompareNoCase("None") == 0)
    {
      AmmoType = "";
    };
    ar >> Experience;
    ar >> Cost;
    ar >> Encumbrance;
    ar >> Attack_Bonus;
    ar >> Cursed;
    ar >> Bundle_Qty;
    ar >> Num_Charges;
  
    ar >> uTemp;
    switch(uTemp)  // Switch old numbers to base-38 names.
    {
      case 0:  uTemp = WeaponHand;  break;  // Sword								
      case 1:  uTemp = ShieldHand;  break;  // Shield
	    case 2:  uTemp = BodyArmor;   break;  // Armor
	    case 3:  uTemp = Hands;       break;  // Guantlets
	    case 4:  uTemp = Head;        break;  // Helm
	    case 5:  uTemp = Waist;       break;  // Belt
	    case 6:  uTemp = BodyRobe;    break;  // Robe
	    case 7:  uTemp = Back;        break;  // Cloak
	    case 8:  uTemp = Feet;        break;  // Boots
	    case 9:  uTemp = Fingers;     break;  // Rings
	    case 10: uTemp = AmmoQuiver;  break;  // Arrows, Bolts
    };
    Location_Readied = (itemReadiedType)uTemp;

    ar >> Hands_to_Use;
    ar >> Dmg_Dice_Sm;
    ar >> Nbr_Dice_Sm;
    ar >> Dmg_Bonus_Sm;
    ar >> Dmg_Dice_Lg;
    ar >> Nbr_Dice_Lg;
    ar >> Dmg_Bonus_Lg;
    ar >> ROF_Per_Round;
    ar >> Protection_Base;
    ar >> Protection_Bonus;
  
    ar >> temp;
    Wpn_Type = (weaponClassType)temp;

    ar >> m_usageFlags;
#ifdef UAFEDITOR
    if (ver < VersionSpellNames)
    {
      int Usable_by_Class;
      ar >> Usable_by_Class;
      AddUsableBaseclass(Usable_by_Class, FighterFlag,Baseclass_fighter);
      AddUsableBaseclass(Usable_by_Class, MagicUserFlag,Baseclass_magicUser);
      AddUsableBaseclass(Usable_by_Class, ClericFlag,Baseclass_cleric);
      AddUsableBaseclass(Usable_by_Class, ThiefFlag,Baseclass_thief);
      AddUsableBaseclass(Usable_by_Class, PaladinFlag,Baseclass_paladin);
      AddUsableBaseclass(Usable_by_Class, RangerFlag,Baseclass_ranger);
      AddUsableBaseclass(Usable_by_Class, DruidFlag,Baseclass_druid);
    }
    else
#endif
    {
      int i, n;
      BASECLASS_ID baseclassID;
      ar >> n;
      usableByBaseclass.RemoveAll();
      for (i=0; i<n; i++)
      {
        ar >> baseclassID;
        usableByBaseclass.Add(baseclassID);
      }
    }
    ar >> RangeMax;
#ifdef UAFEngine
    RangeShort = (RangeMax+2)/3; // These may be overridden by $AI_ShortRange
    RangeMedium = 2*RangeMax/3;  // and $AI_MediumRange Special Abilities. 
#endif
#ifdef UAFEDITOR
    if (ver < VersionSpellNames)
    {
      long junk;
      ar >> preVersionSpellNames_gsID;  //For conversion only
      ar >> junk;
      ar >> junk;
    }
    else
#endif
    {
    };
    //ar >> Spell_Level;
    //ar >> Spell_Class;

    if (ver >= _VERSION_0662_)
      ar >> m_useEvent;
    
    if (ver >= _VERSION_0800_)
    {
      ar >> ExamineEvent;
      DAS(ar, ExamineLabel);
    }
    
    if (ver >= _VERSION_0860_)
    {
      DAS(ar, attackMsg);
    }
    else
      attackMsg="attacks";

    if (ver >= _VERSION_0690_)
    {
      ar >> temp;
      Recharge_Rate = (itemRechargeRate)temp;
      ar >> IsNonLethal;
#ifdef SIMPLE_STRUCTURE
      HitArt.Serialize(ar,ver, rte.CombatArtDir());
#else
      HitArt.Serialize(ar,ver);
#endif
    }
    if (ver >= _VERSION_0881_)
      ar >> CanBeHalvedJoined;
    else
      CanBeHalvedJoined = TRUE;

    if (ver >= _VERSION_0904_)
      ar >> CanBeTradeDropSoldDep;
    else
      CanBeTradeDropSoldDep = TRUE;

    // MissileArt is serialized in attribute map
  }

  specAbs.Serialize(ar, ver, this->m_idName, "item");
  if (ar.IsStoring())
  item_asl.Serialize(ar, "ITEM_DATA_ATTRIBUTES");
  else
  item_asl.Serialize(ar, "ITEM_DATA_ATTRIBUTES");
}

#ifdef UAFEDITOR
// event descriptive text for editor
//*****************************************************************************
//    NAME: ITEM_DATA::GetItemEventText
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_DATA::GetItemEventText() 
{ 
  CString tmp;
  tmp.Format("Item Event: %s", UniqueName());
  return tmp;
}
#endif


BOOL ITEM_ID::IsValidItem(void) const
{
  //return itemData.FindItemID(*this) != NULL;
  return itemData.LocateItem(*this) >= 0;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::Clear() 
{ 
  //CSingleLock sLock(&m_CS, TRUE); 
  version = PRODUCT_VER;
  ItemData.RemoveAll(); 
  m_AmmoTypes.RemoveAll(); 
  m_AmmoTypes.Add("None");
}

#ifndef UAFEngine
//*****************************************************************************
// NAME: ITEM_DATA_TYPE::operator =
//
// PURPOSE: Make a copy of an ITEM_DATA list
//*****************************************************************************
ITEM_DATA_TYPE& ITEM_DATA_TYPE:: operator=(ITEM_DATA_TYPE& src) 
{
  if (&src==this) return *this;
  version=src.version;
  ItemData.RemoveAll();
  //POSITION p=src.ItemData.GetHeadPosition();
  //while (p!=NULL)
  {
    int i, n;
    n = src.GetCount();
    for (i=0; i<n; i++)
    {
      //AddItemWithCurrKey(src.ItemData.GetNext(p));
      //AddItem(src.ItemData.GetNext(p));
      AddItem(*src.PeekItem(i));
    };
  };

  m_AmmoTypes.RemoveAll();
  m_AmmoTypes.SetSize(src.m_AmmoTypes.GetSize());
  for (int i=0;i<src.m_AmmoTypes.GetSize();i++)
    m_AmmoTypes[i]=src.m_AmmoTypes[i];
  return *this;
}
#endif

//*****************************************************************************
// NAME: ITEM_DATA_TYPE::operator ==
//
// PURPOSE: Test that two ITEM_DATA lists are identical...even the ordering.
//          One might like to exclude order but then it is a bit tricky
//          because of the possiblility of two identical entries in a list.
//*****************************************************************************
bool ITEM_DATA_TYPE:: operator ==(ITEM_DATA_TYPE& src) const
{
  int i, n;
  if (&src == this) return true;
  if (version!=src.version) return false;
  //POSITION p1=ItemData.GetHeadPosition();
  //POSITION p2=src.ItemData.GetHeadPosition();
  //while (p1!=NULL)
  if (GetCount() != src.GetCount()) return false;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //if (p2==NULL) return false;
    //if (!(ItemData.GetNext(p1)==src.ItemData.GetNext(p2))) return false;
    if (!(*PeekItem(i) == *src.PeekItem(i))) 
    {
      return false;
    };
  };
  //if (p2!=NULL) return false;
  if (m_AmmoTypes.GetSize() != src.m_AmmoTypes.GetSize()) return false;
  for (i=0;i<m_AmmoTypes.GetSize();i++)
    if (m_AmmoTypes[i] != src.m_AmmoTypes[i]) return false;
  return true;
}

//*****************************************************************************
// NAME: ITEM_DATA_TYPE::Serialize
//
// PURPOSE:  
//*****************************************************************************
void ITEM_DATA_TYPE::Serialize(CArchive &ar)
{
//  CObject::Serialize(ar);  
  //int i;
  //POSITION pos;
  if (ar.IsStoring())
  {
    ar << GetCount();
    //pos = ItemData.GetHeadPosition();
    //while (pos != NULL)
    //  ItemData.GetNext(pos).Serialize(ar, version);
    int i,n;
    n = GetCount();
    for (i=0; i<n; i++)
    {
      //GetItem(i).Serialize(ar);
      GetItem(i)->Serialize(ar, version);
    };
    ar << m_AmmoTypes.GetSize();
    for (i=0;i<m_AmmoTypes.GetSize();i++)
    {
      AS(ar,m_AmmoTypes[i]);
    }
  }
  else
  {
    //Clear();
    ITEM_DATA data;
    int temp, i;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar,version);
      //AddItemWithCurrKey(data);
      AddItem(data);
    }
    if (version >= _VERSION_0690_)
    {
      ar >> temp;
      ASSERT((temp>=0)&&(temp<=MAX_ITEMS));
      m_AmmoTypes.SetSize(temp);
      for (i=0;i<temp;i++)
      {
        DAS(ar,m_AmmoTypes[i]);
      }
    }
  }
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::Serialize(CAR &ar)
{
  //int i;
  //POSITION pos;
  if (ar.IsStoring())
  {
    ar << GetCount();
    //pos = ItemData.GetHeadPosition();
    //while (pos != NULL)
    int i, n;
    n = GetCount();
    for (i=0; i<n; i++)
    {
      //ItemData.GetNext(pos).Serialize(ar, version);
      GetItem(i)->Serialize(ar, version);
    };
    ar << m_AmmoTypes.GetSize();
    for (i=0;i<m_AmmoTypes.GetSize();i++)
    {
      AS(ar,m_AmmoTypes[i]);
    }
  }
  else
  {
    //Clear();
    ITEM_DATA data;
    int temp, i;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar, version);
      //AddItemWithCurrKey(data);
      AddItem(data);
    }
    if (version >= _VERSION_0690_)
    {
      ar >> temp;
      ASSERT((temp>=0)&&(temp<=MAX_ITEMS));
      m_AmmoTypes.SetSize(temp);
      for (i=0;i<temp;i++)
        DAS(ar,m_AmmoTypes[i]);
    };
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA::ITEM_DATA
//
// PURPOSE: 
//
//*****************************************************************************
ITEM_DATA::ITEM_DATA() : 
  // ASLs named "ITEM_DATA_ATTRIBUTES"
	item_asl(),
	temp_asl(),
	hHitSound(-1), 
  hMissSound(-1), 
  hLaunchSound(-1), 
  m_useEvent(0),
  ExamineEvent(0),
  specAbs(true) 
{  
  Clear(TRUE);
}


int ITEM_DATA_TYPE::GetDataSize(void) 
{
  //POSITION pos;
  int size;
  const ITEM_DATA *pEntry;
  //pos = GetHeadPosition();
  size = sizeof (*this);
  //while (pos != NULL)
  int i,n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //pEntry = &GetNext(pos);
    pEntry = PeekItem(i);
    size += pEntry->GetDataSize();
  };
  return size;
}

//*****************************************************************************
//    NAME: ITEM_DATA::GetContext
//
// PURPOSE: 
//
//*****************************************************************************
ActorType ITEM_DATA::GetContext() const
{
  ActorType data;
  data.Clear();
  data.SetItemSrc();
  //data.Instance = this->m_giID.UniqueIdentifier();
  data.m_itemID = this->m_uniqueName;
  return data;
}
/*
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetContext
//
// PURPOSE: 
//
//*****************************************************************************
ActorType ITEM_DATA_TYPE::GetContext(int index)
{
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    return ItemData.PeekAtPos(pos).GetContext();
  }
  else 
  {
    ActorType data;
    data.Clear();
    ASS ERT(FALSE);
    return data;
  }
}
*/
int ITEM_DATA::GetDataSize(void) const
{
  return sizeof (*this) + sizeof (specAbs);
}

int ITEM_DATA::LocateBaseclass(const BASECLASS_ID& baseclassID) const
{
  int i, n;
  n = usableByBaseclass.GetSize();
  for (i=0; i<n; i++)
  {
    if (*PeekBaseclass(i) == baseclassID) return i;
  };
  return -1;
}

//*****************************************************************************
// NAME: ITEM_DATA::Clear
//
// PURPOSE:  
//*****************************************************************************
void ITEM_DATA::Clear(BOOL ctor)
{
  //if (event > 0)
  //  globalData.eventData.DeleteEvents(event);  
  //if (ExamineEvent > 0)
  //  globalData.eventData.DeleteEvents(ExamineEvent);
  //key=0;
  //m_giID.Clear();
  m_priorityAI = 500;
  m_useEvent=0;
  ExamineEvent=0;
  ExamineLabel="EXAMINE";
  attackMsg="attacks";
  m_uniqueName="";
  m_commonName = "";
  m_idName="";
  HitSound = "";
  MissSound = "";
  LaunchSound = "";
  AmmoType="";
  ClearSounds();
  ClearArt();
  Experience = 0;
  Cost = 0;
  Encumbrance = 0;
  Attack_Bonus = 0;
  Cursed = FALSE;
  Bundle_Qty = 1;
  Num_Charges = 0;
  Location_Readied = WeaponHand;
  Hands_to_Use = 1;  
  Dmg_Dice_Sm = 6;
  Nbr_Dice_Sm = 1;
  Dmg_Bonus_Sm = 0;
  Dmg_Dice_Lg = 6;
  Nbr_Dice_Lg = 1;
  Dmg_Bonus_Lg = 0;
  ROF_Per_Round = 1.0;
  Protection_Base = 0;
  Protection_Bonus = 0;
  Wpn_Type = NotWeapon;
  m_usageFlags = 0;
  //Usable_by_Class = FighterFlag;
  usableByBaseclass.RemoveAll();
  Recharge_Rate = irrNever;
  IsNonLethal=FALSE;
  CanBeHalvedJoined=TRUE;
  CanBeTradeDropSoldDep=TRUE;
  RangeMax = 0;
#ifdef UAFEngine
  RangeShort = 0;
  RangeMedium = 0;
#endif
  //m_gsID.Clear();
#ifdef UAFEDITOR
//  preVersionSpellNames_giID = -1;
  preVersionSpellNames_gsID = -1;
#endif
  preSpellNameKey = -1;
  spellID.Clear();
  //Spell_Level = 1; 

  //Spell_Class = MagicUserFlag;


  item_asl.Clear();
  specAbs.Clear();
}


//*****************************************************************************
// NAME: saveData
//
// PURPOSE:  
//*****************************************************************************
BOOL saveData(ITEM_DATA_TYPE& data)
{
  EditorStatusMsg("Saving item data...");

#ifdef SIMPLE_STRUCTURE
  CString fullPath;
  fullPath = rte.DataDir() + ITEM_DB_NAME;
#else
  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, ITEM_DB_NAME);
#endif
  return saveData(data, fullPath);
}


//*****************************************************************************
// NAME: saveData
//
// PURPOSE:  
//*****************************************************************************
BOOL saveData(ITEM_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success = TRUE;
    
  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open item db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  // put signal value and version at start of file
  __int64 hdr=0xFABCDEFABCDEFABF;
  double ver = PRODUCT_VER;
  data.version = ver;
  myFile.Write(&hdr, sizeof(hdr));
  myFile.Write(&ver, sizeof(double));

  CAR ar(&myFile, CArchive::store);
  ar.Compress(true);

  try
  {
    data.PreSerialize(ar.IsStoring());
    data.Serialize(ar);
    data.PostSerialize(ar.IsStoring(), globalData.version);
  }
  catch (...)
  {
    WriteDebugString("Failed to read item dat file\n");
    data.Clear();
    success = FALSE;
  }

  ar.Close();
  return success;
}


//*****************************************************************************
// NAME: loadData
//
// PURPOSE:  
//*****************************************************************************
int loadData(ITEM_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success=TRUE;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open item db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  clearData(data);

  // check for version
  __int64 hdr=0;
  double ver;
  myFile.Read(&hdr, sizeof(hdr));
  if (hdr == 0xFABCDEFABCDEFABF)
  {
    myFile.Read(&ver, sizeof(double));
  }
  else
  {
    // special hdr not present, so reset archive to beginning
    myFile.SeekToBegin();
    // set version to last build that didn't save version
    ver = min(globalData.version, _VERSION_0696_);
  }

  data.version = ver;

  // if loading older level data, don't use compression archive
  if (ver < _VERSION_0697_)
  {
    WriteDebugString("Loading pre-0697 version item DB - %4.7f\n", ver);

    CArchive ar(&myFile, CArchive::load);
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar);
       data.PostSerialize(ar.IsStoring(), globalData.version);
    }
    catch (...)
    {
       WriteDebugString("Unable to load item data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }
  else
  {
    WriteDebugString("Loading item DB version: %4.7f\n", ver);
    CAR ar(&myFile, CArchive::load);
    if (ver >= _SPECIAL_ABILITIES_VERSION_)
    {
      ar.Compress(true); // qqqqq
    };
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar);
       data.PostSerialize(ar.IsStoring(), globalData.version);
    }
    catch (...)
    {
       WriteDebugString("Unable to load item data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }  
  
  if (data.GetCount() == 0)
     WriteDebugString("No items in item db file\n");

  return success; //data.GetCount();
}


//*****************************************************************************
// NAME: clearData
//
// PURPOSE:  
//*****************************************************************************
void clearData(ITEM_DATA_TYPE& data)
{
  data.Clear();
  data.version = PRODUCT_VER;
}


//*****************************************************************************
// NAME: halveItem
//
// PURPOSE:  
//*****************************************************************************
void ITEM_LIST::halveItem(int index)
{
  ITEM theItem;
  if (!GetItem(index, theItem))
    return;

  //if (!itemCanBeHalved(theItem.m_giID))
  if (!itemCanBeHalved(theItem.itemID))
    return;  
 
  if (theItem.qty <= 1)
    return;

  int newQty = theItem.qty/2;
  
  // update original qty
  theItem.qty -= newQty;
  SetItem(index, theItem);

  // insert new item
  ITEM newItem = theItem; 
  newItem.qty = newQty;
  newItem.ready = FALSE;
  
  AddItem(newItem,FALSE); // don't auto join them back together!
}


//*****************************************************************************
// NAME: joinItems
//
// PURPOSE:  
//*****************************************************************************
void ITEM_LIST::joinItems(int index)
{
  ITEM origItem;
  if (!GetItem(index, origItem))
    return;

  //if (!itemCanBeJoined(origItem.m_giID))
  if (!itemCanBeJoined(origItem.itemID))
    return;

  // look for another instance of item
  int joinIndex = -1;
  int joinQty=0;
  POSITION pos;
  pos = m_items.GetHeadPosition();

  CList<int, int> delList;
  while ((pos != NULL) && (joinIndex < 0))
  {
    //if (   (m_items.PeekAtPos(pos).m_giID == origItem.m_giID) 
    if (   (m_items.PeekAtPos(pos).itemID == origItem.itemID) 
        && (m_items.PeekAtPos(pos).key != origItem.key))
    {
      //joinIndex = items.GetAt(pos).key;
      joinQty += m_items.PeekAtPos(pos).qty;
      delList.AddTail(m_items.PeekAtPos(pos).key);
    }
    m_items.GetNext(pos);
  }

  if (joinQty > 0)
  {
    ITEM joinItem;
    if (GetItem(index, origItem))
    {
      origItem.qty += joinQty;
      SetItem(index, origItem);
      pos=delList.GetHeadPosition();
      while (pos!=NULL)
        DeleteItem(delList.GetNext(pos));
    }
  }
}


//*****************************************************************************
// NAME: findItem
//
// PURPOSE:  
//*****************************************************************************
//int ITEM_LIST::findItem(GLOBAL_ITEM_ID giID, int index) const
int ITEM_LIST::findItem(const ITEM_ID& itemID, int index) const
{
  BOOL found = FALSE;
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) == NULL)
  {
    WriteDebugString("bogus index %i in findItem()\n", index);
    return -1;
  }

  //if (giID.IsValidItem())
  if (itemID.IsValidItem())
  {
    //WriteDebugString("bogus item index %i in findItem()\n", giID);
    WriteDebugString("bogus item index %s in findItem()\n", itemID.UniqueName());
    return -1;
  }

	while ((pos != NULL) && (!found)) 
  {
    //if (m_items.PeekAtPos(pos).m_giID == giID)
    if (m_items.PeekAtPos(pos).itemID == itemID)
      found = TRUE;
    else
		  m_items.PeekNext(pos);
  }

	if (found)
		return m_items.PeekAtPos(pos).key;

	return -1;
}

//*****************************************************************************
// NAME: addItem
//
// PURPOSE:  Add a single item to the inventory (or single bundle qty)
//*****************************************************************************
//BOOL ITEM_LIST::addItem(GLOBAL_ITEM_ID giID, int qty, int numCharges, BOOL id, int cost)
BOOL ITEM_LIST::addItem(const ITEM_ID& itemID, int qty, int numCharges, BOOL id, int cost)
{
  if (qty < 1)
    return FALSE;

  ITEM newItem;

  //if (ITEM_LIST::IsMoneyItem(giID))
  if (ITEM_LIST::IsMoneyItem(itemID))
  {
    if (numCharges < 0)
      numCharges = 0;
    
	  //newItem.m_giID = giID;
	  newItem.itemID = itemID;
	  newItem.qty = qty;
	  newItem.ready = FALSE;
 	  newItem.charges = numCharges;
	  newItem.identified = id;
    newItem.cursed = FALSE;
  }
  else
  {
    //ITEM_DATA *theItem = itemData.GetItemData(giID);
    ITEM_DATA *theItem = itemData.GetItem(itemID);
    if (theItem == NULL)
      return FALSE;

    if ((theItem->Bundle_Qty <= 1) && (qty > 1))
    {
      WriteDebugString("qty > 1 && BundleQty <= 1 in addItem()\n");
      qty = 1;
    }

    if (numCharges < 0)
      numCharges = theItem->Num_Charges;
    
	  //newItem.m_giID = giID;
	  newItem.itemID = itemID;
	  newItem.qty = qty;
	  newItem.ready = FALSE;
 	  newItem.charges = numCharges;
	  newItem.identified = id;
    newItem.cursed = theItem->Cursed;
    newItem.paid = cost;
  }

  return AddItem(newItem,FALSE); // no auto-join
}

//*****************************************************************************
// NAME: deleteItem
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::deleteItem(int index, int qty)
{
  ITEM theItem;

  if (!GetItem(index, theItem))
  {
    WriteDebugString("bogus item index in deleteItem()\n");
    return FALSE;
  }

  if (qty > theItem.qty)
  {
    WriteDebugString("bogus qty in deleteItem()\n");
    qty = theItem.qty;
  }

	theItem.qty -= qty;
	SetItem(index, theItem);

	if (theItem.qty < 1)
    DeleteItem(index);

  return TRUE;
}

void DROPPED_ITEM::Copy(const ITEM& item)
{
  const ITEM_DATA *pItem;
  pItem = itemData.PeekItem(item.itemID);
  itemtype = 1;
  data.commonItem.charges = item.charges;
  data.commonItem.cursed = item.cursed;
  data.commonItem.FNV_hash = pItem->FNV_Hash;
  data.commonItem.identified = item.identified!=0;
  data.commonItem.paid = item.paid;
  data.commonItem.qty = item.qty;
}

//*****************************************************************************
// NAME: ITEM_DATA::operator=
//
// PURPOSE:  
//*****************************************************************************
ITEM_DATA& ITEM_DATA::operator=(const ITEM_DATA& src)
{
  if (&src == this) return *this;
  Clear();
  //m_giID = src.m_giID; //key = src.key;
  m_useEvent = src.m_useEvent;
  ExamineEvent = src.ExamineEvent;
  ExamineLabel = src.ExamineLabel;
  attackMsg=src.attackMsg;
  m_uniqueName = src.m_uniqueName;
  m_commonName = src.m_commonName;
  m_idName = src.m_idName;
  HitSound = src.HitSound;
  MissSound = src.MissSound;
  LaunchSound = src.LaunchSound;
  AmmoType=src.AmmoType;
  Experience = src.Experience;
  Cost = src.Cost;
  Encumbrance = src.Encumbrance;
  Attack_Bonus = src.Attack_Bonus;
  Cursed = src.Cursed;
  Bundle_Qty = src.Bundle_Qty;
  Num_Charges = src.Num_Charges;
  Location_Readied = src.Location_Readied;
  Hands_to_Use = src.Hands_to_Use;
  Recharge_Rate = src.Recharge_Rate;
  IsNonLethal=src.IsNonLethal;
  CanBeHalvedJoined=src.CanBeHalvedJoined;
  CanBeTradeDropSoldDep=src.CanBeTradeDropSoldDep;
  Dmg_Dice_Sm = src.Dmg_Dice_Sm;
  Nbr_Dice_Sm = src.Nbr_Dice_Sm;
  Dmg_Bonus_Sm = src.Dmg_Bonus_Sm;
  Dmg_Dice_Lg = src.Dmg_Dice_Lg;
  Nbr_Dice_Lg = src.Nbr_Dice_Lg;
  Dmg_Bonus_Lg = src.Dmg_Bonus_Lg;
  ROF_Per_Round = src.ROF_Per_Round;
  Protection_Base = src.Protection_Base;
  Protection_Bonus = src.Protection_Bonus;
  Wpn_Type = src.Wpn_Type;
  m_usageFlags = src.m_usageFlags;
  //Usable_by_Class = src.Usable_by_Class;
  {
    int i, n;
    n = src.usableByBaseclass.GetSize();
    usableByBaseclass.RemoveAll();
    for (i=0; i<n; i++)
    {
      usableByBaseclass.Add(*const_cast<BASECLASS_ID*>(src.PeekBaseclass(i)));
    }
  };
  RangeMax = src.RangeMax;
#ifdef UAFEngine
  RangeShort = src.RangeShort;
  RangeMedium = src.RangeMedium;
#endif
  //m_gsID = src.m_gsID;
  spellID = src.spellID;
  preSpellNameKey = src.preSpellNameKey;
#ifdef UAFEDITOR
//  preVersionSpellNames_giID = src.preVersionSpellNames_giID;
  preVersionSpellNames_gsID = src.preVersionSpellNames_gsID;
#endif
  //Spell_Level = src.Spell_Level;
  //Spell_Class = src.Spell_Class;
  MissileArt = src.MissileArt;
  HitArt = src.HitArt;
  item_asl.Copy(src.item_asl);
  temp_asl.Copy(src.temp_asl);// Equal means equal
  hHitSound = src.hHitSound;
  hMissSound = src.hMissSound;
  hLaunchSound = src.hLaunchSound;
  specAbs.Copy(src.specAbs);
  return *this;
}

//*****************************************************************************
// NAME: ITEM_DATA::operator==
//
// PURPOSE:  
//*****************************************************************************
bool ITEM_DATA::operator==(const ITEM_DATA& src) const
{
  if (&src == this) return true;
  //if (m_giID           != src.m_giID           ) return false;
  if (spellID          != src.spellID          ) return false;
  if (m_uniqueName     != src.m_uniqueName     ) return false;
  if (m_idName         != src.m_idName         ) return false;
  if (HitSound         != src.HitSound         ) return false;
  if (MissSound        != src.MissSound        ) return false;
  if (LaunchSound      != src.LaunchSound      ) return false;
  if (AmmoType         != src.AmmoType         ) return false;
  if (Experience       != src.Experience       ) return false;
  if (Cost             != src.Cost             ) return false;
  if (Encumbrance      != src.Encumbrance      ) return false;
  if (Attack_Bonus     != src.Attack_Bonus     ) return false;
  if (Cursed           != src.Cursed           ) return false;
  if (Bundle_Qty       != src.Bundle_Qty       ) return false;
  if (Num_Charges      != src.Num_Charges      ) return false;
  if (Location_Readied != src.Location_Readied ) return false;
  if (Hands_to_Use     != src.Hands_to_Use     ) return false;
  if (Recharge_Rate    != src.Recharge_Rate    ) return false;
  if (IsNonLethal      != src.IsNonLethal      ) return false;
  if (CanBeHalvedJoined!= src.CanBeHalvedJoined)return false;
  if (CanBeTradeDropSoldDep!=src.CanBeTradeDropSoldDep)return false;
  if (Dmg_Dice_Sm      != src.Dmg_Dice_Sm      ) return false;
  if (Nbr_Dice_Sm      != src.Nbr_Dice_Sm      ) return false;
  if (Dmg_Bonus_Sm     != src.Dmg_Bonus_Sm     ) return false;
  if (Dmg_Dice_Lg      != src.Dmg_Dice_Lg      ) return false;
  if (Nbr_Dice_Lg      != src.Nbr_Dice_Lg      ) return false;
  if (Dmg_Bonus_Lg     != src.Dmg_Bonus_Lg     ) return false;
  if (ROF_Per_Round    != src.ROF_Per_Round    ) return false;
  if (Protection_Base  != src.Protection_Base  ) return false;
  if (Protection_Bonus != src.Protection_Bonus ) return false;
  if (Wpn_Type         != src.Wpn_Type         ) return false;
  if (m_usageFlags     != src.m_usageFlags     ) return false;
  //if (Usable_by_Class  != src.Usable_by_Class  ) return false;
  {
    int i, n;
    n = usableByBaseclass.GetSize();
    if (n != src.usableByBaseclass.GetSize()) return false;
    for (i=0; i<n; i++)
    {
      if (*PeekBaseclass(i) != *src.PeekBaseclass(i)) return false;
    };
  }
  if (RangeMax           != src.RangeMax          ) return false;
#ifdef UAFEngine
  if (RangeShort         != src.RangeShort       ) return false;
  if (RangeMedium        != src.RangeMedium      ) return false;
#endif
  //if (m_gsID           != src.m_gsID           ) return false;
  //if (Spell_Level      != src.Spell_Level      ) return false;
  //if (Spell_Class      != src.Spell_Class      ) return false;
  if (!(MissileArt     == src.MissileArt)      ) return false;
  if (!(HitArt         == src.HitArt)          ) return false;
  if (!(item_asl==src.item_asl) ) return false;
  if (!(temp_asl==src.temp_asl) ) return false;
  if (hHitSound        != src.hHitSound        ) return false;
  if (hMissSound       != src.hMissSound       ) return false;
  if (hLaunchSound     != src.hLaunchSound     ) return false;
  if (!(specAbs        == src.specAbs)         ) return false;
  if (m_useEvent       != src.m_useEvent       ) return false;        
  if (ExamineEvent     != src.ExamineEvent     ) return false;        
  if (ExamineLabel     != src.ExamineLabel     ) return false;        
  if (attackMsg        != src.attackMsg        ) return false;
  return true;
}


//*****************************************************************************
// NAME: ITEM_DATA_TYPE::SaveItemSounds
//
// PURPOSE:  
//*****************************************************************************
void saveSounds(ITEM_DATA_TYPE& data)
{
  data.SaveSounds();
}

void ITEM_DATA_TYPE::SaveSounds() 
{
  //POSITION p = ItemData.GetHeadPosition();
  //while (p != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //::SaveSound(ItemData.GetAtPos(p).HitSound,    GLOBAL_SOUND, rte.SoundDir());
    //::SaveSound(ItemData.GetAtPos(p).MissSound,   GLOBAL_SOUND, rte.SoundDir());
    //::SaveSound(ItemData.GetAtPos(p).LaunchSound, GLOBAL_SOUND, rte.SoundDir());
    ::SaveSound(PeekItem(i)->HitSound,    GLOBAL_SOUND, rte.SoundDir());
    ::SaveSound(PeekItem(i)->MissSound,   GLOBAL_SOUND, rte.SoundDir());
    ::SaveSound(PeekItem(i)->LaunchSound, GLOBAL_SOUND, rte.SoundDir());
    //ItemData.PeekNext(p);
    //ItemData.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: saveArt
//
// PURPOSE: 
//
//*****************************************************************************
void saveArt(ITEM_DATA_TYPE& data)
{
  data.SaveArt();
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SaveArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::SaveArt() 
{
  //POSITION p = ItemData.GetHeadPosition();
  //while (p != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //ItemData.GetAtPos(p).MissileArt.SavePicData(GLOBAL_ART);
    //ItemData.GetAtPos(p).HitArt.SavePicData(GLOBAL_ART);
    //ItemData.PeekNext(p);
    GetItem(i)->MissileArt.SavePicData(GLOBAL_ART);
    GetItem(i)->HitArt.SavePicData(GLOBAL_ART);
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PreSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  // before saving, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (IsStoring) ExpandDefaultFilenames();

  //POSITION p = ItemData.GetHeadPosition();
  //while (p!=NULL) 
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //ItemData.GetNext(p).PreSerialize(IsStoring);
    GetItem(i)->PreSerialize(IsStoring);
  };
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PostSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::PostSerialize(BOOL IsStoring, double Version)
{
  // after loading, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (!IsStoring) ExpandDefaultFilenames();

  //POSITION p = ItemData.GetHeadPosition();
  //while (p!=NULL) 
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //ItemData.GetAtPos(p).PostSerialize(IsStoring, Version);
    //ItemData.GetAt(p).PostSerialize(IsStoring, Version);
    GetItem(i)->PostSerialize(IsStoring, Version);
    
    if ((!IsStoring)&&(Version<=_VERSION_0840_))
    {
      //ItemData.GetAtPos(p).MissileArt.SetPre0840NonLoopDefaults();
      //ItemData.GetAtPos(p).HitArt.SetPre0840NonLoopDefaults();
      //ItemData.GetAt(p).MissileArt.SetPre0840NonLoopDefaults();
      //ItemData.GetAt(p).HitArt.SetPre0840NonLoopDefaults();
      GetItem(i)->MissileArt.SetPre0840NonLoopDefaults();
      GetItem(i)->HitArt.SetPre0840NonLoopDefaults();
    }

    //ItemData.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Save
//
// PURPOSE: 
//
//*****************************************************************************

void ITEM_DATA_TYPE::Save(CAR& car) const
{
  int count;
	//POSITION p;

	//count=ItemData.GetCount();
  count = GetCount();
	car << count;
	//p=ItemData.GetHeadPosition();
	//while (p!=NULL) 
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
	{
		//ar << ItemData.PeekAtPos(p).UniqueName();
		//ItemData.PeekAtPos(p).Save(ar);
		//ItemData.PeekNext(p);
		//ar << PeekAtPos(p).UniqueName();
		//ar << PeekItemData(p)->UniqueName();
		car << PeekItem(i)->UniqueName();
		//PeekAtPos(p).Save(ar);
		//PeekItemData(p)->Save(ar);
		PeekItem(i)->Save(car);
		//PeekNext(p);
	};
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Restore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::Restore(CArchive& ar) 
{
	int count;
	CString name;
	//POSITION p;
	if (version < _ASL_LEVEL_) return;
	//p=ItemData.GetHeadPosition();
	// Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ATTRIBUTES in case no restore operation clears it.
  //while (p!=NULL) ItemData.GetNext(p).PrepareForRestore();
  int i, n;
  ITEM_ID itemID;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    GetItem(i)->PrepareForRestore();
  };
	ar >> count;
	for (i=0; i<count; i++) 
  {
		ar >> name;
		//p=ItemData.GetHeadPosition();
		//while (p!=NULL) {
			//if (name=ItemData.PeekAtPos(p).UniqueName())
			//if (name=ItemData.PeekAt(p).UniqueName())
			//if (name=PeekAtPos(p).UniqueName())
			//if (name=PeekItemData(p)->UniqueName())
    itemID = name;
    i = LocateItem(itemID);
    if (i >= 0)
		{
				//ItemData.GetAtPos(p).Restore(ar);
				//ItemData.GetAt(p).Restore(ar);
		  GetItem(i)->Restore(ar);
				//break;
    };
			//PeekNext(p);
		//};
		// (p==NULL) 
		if (i < 0) 
		{	
    	ITEM_DATA junk;
			CString msg;
			msg.Format("SAVE-FILE ITEM_DATA \"%s\" does not exist in design\n",
				name);
			WriteDebugString(msg);
			junk.Restore(ar); // Discard saved data.  Any better ideas?
		};
	};
}

void ITEM_DATA_TYPE::Restore(CAR& car) 
{
	int count;
	CString name;
	//POSITION p;
	//p=ItemData.GetHeadPosition();
	// Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ATTRIBUTES in case no restore operation clears it.
  //while (p!=NULL) ItemData.GetNext(p).PrepareForRestore();
  int i, n;
  ITEM_ID itemID;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    GetItem(i)->PrepareForRestore();
  };
	car >> count;
	for (i=0; i<count; i++) 
  {
		car >> name;
		//p=ItemData.GetHeadPosition();
		//while (p!=NULL) {
			//if (name=ItemData.PeekAtPos(p).UniqueName())
			//if (name=ItemData.PeekAt(p).UniqueName())
			//if (name=PeekAtPos(p).UniqueName())
			//if (name=PeekItemData(p)->UniqueName())
    itemID = name;
    i = LocateItem(itemID);
    if (i >= 0)
		{
				//ItemData.GetAtPos(p).Restore(ar);
				//ItemData.GetAt(p).Restore(ar);
		  GetItem(i)->Restore(car);
				//break;
    };
			//PeekNext(p);
		//};
		// (p==NULL) 
		if (i < 0) 
		{	
    	ITEM_DATA junk;
			CString msg;
			msg.Format("SAVE-FILE ITEM_DATA \"%s\" does not exist in design\n",
				name);
			WriteDebugString(msg);
			junk.Restore(car); // Discard saved data.  Any better ideas?
		};
	};
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::CommitRestore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::CommitRestore(void) {
	//POSITION p;
	//p=ItemData.GetHeadPosition();
	//while (p!=NULL) ItemData.GetNext(p).CommitRestore();
  int i, n;
  n = GetCount();
	for (i=0; i<n; i++)
  {
    GetItem(i)->CommitRestore();
  };
}

//*****************************************************************************
//    NAME: ITEM_DATA::Save
//
// PURPOSE: 
//
//*****************************************************************************

void ITEM_DATA::Save(CAR& car) const
{
  item_asl.Save(car, "ITEM_DATA_ATTRIBUTES");
}

//*****************************************************************************
//    NAME: ITEM_DATA::PrepareForRestore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PrepareForRestore(void) 
{
	// In case no Restore comes along then we will simply use what we have.
	temp_asl.Clear();
}

//*****************************************************************************
//    NAME: ITEM_DATA::PreSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PreSerialize(BOOL IsStoring) 
{
  if (IsStoring)
  {
    // move ITEM_DATA fields that need to be serialized
    // as attributes into the attribute map  

    //StripFilenamePath(HitSound);
    //StripFilenamePath(MissSound);
    //StripFilenamePath(LaunchSound);
    MissileArt.PreSerialize(IsStoring);
    HitArt.PreSerialize(IsStoring);
    // MissileArt
    //StorePicDataAsASL(item_asl, MissileArt, "MissArt");
    
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA::PostSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PostSerialize(BOOL IsStoring, double version) 
{
  const CStringPAIR *pAbility;
  if (IsStoring)
  {
    item_asl.Delete("MissArt");
  }
  else
  {
    // retrieve ITEM_DATA fields that are serialized as
    // attributes from the attribute map

    // MissileArt
#ifdef UAFEDITOR
    if (version < VersionSpellNames)
    {
      RetrievePicDataFromASL(item_asl, MissileArt, "MissArt");
      item_asl.Delete("MissArt");
    };
#endif
  }

  //AddFolderToPath(HitSound,    rte.SoundDir());
  //AddFolderToPath(MissSound,   rte.SoundDir());
  //AddFolderToPath(LaunchSound, rte.SoundDir());
  MissileArt.PostSerialize(IsStoring, version);
  HitArt.PostSerialize(IsStoring, version);
  pAbility = specAbs.FindAbility("$AI_Priority");
  if (pAbility != NULL)
  {
    sscanf(pAbility->Value(), "%d", &m_priorityAI);
  }
  else
  {
    m_priorityAI = 500;
  };
#ifdef UAFEngine
  pAbility = specAbs.FindAbility("$AI_ShortRange");
  if (pAbility != NULL)
  {
    sscanf(pAbility->Value(), "%d", &RangeShort);
  }
  pAbility = specAbs.FindAbility("$AI_MediumRange");
  if (pAbility != NULL)
  {
    sscanf(pAbility->Value(), "%d", &RangeMedium);
  }
#endif
}

//*****************************************************************************
//    NAME: ITEM_DATA::Restore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::Restore(CArchive& ar) 
{
  temp_asl.Serialize(ar, "ITEM_DATA_ATTRIBUTES");
}

void ITEM_DATA::Restore(CAR& car) 
{
  temp_asl.Serialize(car, "ITEM_DATA_ATTRIBUTES");
}

//*****************************************************************************
//    NAME: ITEM_DATA::CommitRestore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::CommitRestore(void) 
{
	item_asl.CommitRestore(&temp_asl);
	temp_asl.Clear();
}

#ifdef UAFEDITOR

void ITEM_DATA::CrossReference(CR_LIST *pCRList) const
{
  CR_ENTRY *pCREntry;
  CR_REFERENCE CRReference;
  //pCREntry = pCRList->CR_AddResource(CR_TYPE_item, this->UniqueName(), m_giID.UniqueIdentifier());
  pCREntry = pCRList->CR_AddResource(CR_TYPE_item, this->UniqueName(), 0);
  CRReference.m_referenceType = CR_TYPE_item;
  CRReference.m_referenceName = this->UniqueName();
  pCRList->CR_AddSoundReference(HitSound, &CRReference);
  pCRList->CR_AddSoundReference(MissSound, &CRReference);
  pCRList->CR_AddSoundReference(LaunchSound, &CRReference);
  //pCRList->CR_AddSpellReference(m_gsID, &CRReference);
  pCRList->CR_AddSpellReference(spellID, &CRReference);
  specAbs.CrossReference(pCRList);
  MissileArt.CrossReference(pCRList, &CRReference);
  HitArt.CrossReference(pCRList, &CRReference);
}

#endif

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::ExpandDefaultFilenames
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::ExpandDefaultFilenames()
{  
  //POSITION p = GetHeadPosition();
  //GLOBAL_ITEM_ID giID;
  ITEM_ID itemID;
  int i, n;
  n = GetCount();
  //while (p != NULL)
  for (i=0; i<n; i++)
  {
    //ITEM_DATA &data = GetAtPos(p);
    ITEM_DATA *pItem = GetItem(i);
    
    //if (data.HitSound.CompareNoCase("DEFAULT") == 0)
    if (pItem->HitSound.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      //data.HitSound.Format("%s%s",ede.TemplateSoundDir(), DEF_HIT_SOUND);
      pItem->HitSound.Format("%s%s",ede.TemplateSoundDir(), DEF_HIT_SOUND);
#else
      //data.HitSound = DEF_HIT_SOUND;
      pItem->HitSound = DEF_HIT_SOUND;
#endif
    }
    //if (data.MissSound.CompareNoCase("DEFAULT") == 0)
    if (pItem->MissSound.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      //data.MissSound.Format("%s%s",ede.TemplateSoundDir(), DEF_MISS_SOUND);
      pItem->MissSound.Format("%s%s",ede.TemplateSoundDir(), DEF_MISS_SOUND);
#else
      //data.MissSound = DEF_MISS_SOUND;
      pItem->MissSound = DEF_MISS_SOUND;
#endif
    }
    //if (data.LaunchSound.CompareNoCase("DEFAULT") == 0)
    if (pItem->LaunchSound.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      //data.LaunchSound.Format("%s%s",ede.TemplateSoundDir(), DEF_LAUNCH_SOUND);
      pItem->LaunchSound.Format("%s%s",ede.TemplateSoundDir(), DEF_LAUNCH_SOUND);
#else
      //data.LaunchSound = DEF_LAUNCH_SOUND;
      pItem->LaunchSound = DEF_LAUNCH_SOUND;
#endif
    }

    //if (data.MissileArt.filename.CompareNoCase("DEFAULT")==0)
    if (pItem->MissileArt.filename.CompareNoCase("DEFAULT")==0)
    {
      //switch (data.Wpn_Type)
      switch (pItem->Wpn_Type)
      {
      case HandThrow:
      case Throw:
      case SpellCaster:
      case SpellLikeAbility:
#ifdef UAFEDITOR
		    //data.MissileArt.filename.Format("%s%s",ede.TemplateMissileArtDir(), DEFAULT_IIRT);
		    pItem->MissileArt.filename.Format("%s%s",ede.TemplateMissileArtDir(), DEFAULT_IIRT);
#else
        //data.MissileArt.filename = DEFAULT_IIRT;
        pItem->MissileArt.filename = DEFAULT_IIRT;
#endif
        //data.MissileArt.style = PIC_DATA::AS_None;
        //data.MissileArt.NumFrames = 2;
        pItem->MissileArt.style = PIC_DATA::AS_None;
        pItem->MissileArt.NumFrames = 2;
        break;
      case SlingNoAmmo:
#ifdef UAFEDITOR
		    //data.MissileArt.filename.Format("%s%s",ede.TemplateMissileArtDir(), DEFAULT_IIRS);
		    pItem->MissileArt.filename.Format("%s%s",ede.TemplateMissileArtDir(), DEFAULT_IIRS);
#else
        //data.MissileArt.filename = DEFAULT_IIRS;
        pItem->MissileArt.filename = DEFAULT_IIRS;
#endif
        //data.MissileArt.style = PIC_DATA::AS_None;
        //data.MissileArt.NumFrames = 2;
        pItem->MissileArt.style = PIC_DATA::AS_None;
        pItem->MissileArt.NumFrames = 2;
        break;
      case Ammo:
#ifdef UAFEDITOR
		    //data.MissileArt.filename.Format("%s%s",ede.TemplateMissileArtDir(), DEFAULT_IIRA);
		    pItem->MissileArt.filename.Format("%s%s",ede.TemplateMissileArtDir(), DEFAULT_IIRA);
#else
        //data.MissileArt.filename = DEFAULT_IIRA;
        pItem->MissileArt.filename = DEFAULT_IIRA;
#endif
        //data.MissileArt.style = PIC_DATA::AS_Directional;
        //data.MissileArt.NumFrames = 8;
        pItem->MissileArt.style = PIC_DATA::AS_Directional;
        pItem->MissileArt.NumFrames = 8;
        break;
      default:
        //data.MissileArt.Clear();
        pItem->MissileArt.Clear();
        break;
      }

      //data.MissileArt.picType = SpriteDib;
      //data.MissileArt.FrameHeight = COMBAT_TILE_HEIGHT;
      //data.MissileArt.timeDelay = 0;
      //data.MissileArt.FrameWidth = COMBAT_TILE_WIDTH;      
      //data.MissileArt.MaxLoops=1;
      //data.MissileArt.flags=6;
      pItem->MissileArt.picType = SpriteDib;
      pItem->MissileArt.FrameHeight = COMBAT_TILE_HEIGHT;
      pItem->MissileArt.timeDelay = 0;
      pItem->MissileArt.FrameWidth = COMBAT_TILE_WIDTH;      
      pItem->MissileArt.MaxLoops=1;
      pItem->MissileArt.flags=6;
    }

    //switch (data.Wpn_Type)
    switch (pItem->Wpn_Type)
    {
    case HandThrow:
    case Throw:
    case SlingNoAmmo:
    case SpellLikeAbility:
    case SpellCaster:
      break;
    case Ammo:
      //if (data.AmmoType.IsEmpty())
      if (pItem->AmmoType.IsEmpty())
      {
        //if (strstr(data.CommonName(), "Bolt") != NULL)
        if (strstr(pItem->CommonName(), "Bolt") != NULL)
          //data.AmmoType="CrossBow";
          pItem->AmmoType="CrossBow";
        else
          //data.AmmoType="Bow";
          pItem->AmmoType="Bow";
      }
      //AddAmmoType(data.AmmoType);
      AddAmmoType(pItem->AmmoType);
      break;
    case Bow:
      //if (data.AmmoType.IsEmpty())
      if (pItem->AmmoType.IsEmpty())
      {
        //data.AmmoType="Bow";
        pItem->AmmoType="Bow";
        //AddAmmoType(data.AmmoType);
        AddAmmoType(pItem->AmmoType);
      }
      break;
    case Crossbow:
      //if (data.AmmoType.IsEmpty())
      if (pItem->AmmoType.IsEmpty())
      {
        //data.AmmoType="CrossBow";
        pItem->AmmoType="CrossBow";
        //AddAmmoType(data.AmmoType);
        AddAmmoType(pItem->AmmoType);
      }
      break;

    default:
      break;
    }
    //giID = GetKeyAt(p);
    itemID = PeekItem(i)->ItemID();
    //SetItem(giID, data);
    SetItem(itemID, *pItem);
    //tNext(p);
  }
}

#ifdef UAFEDITOR
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::CollapseDefaultFilenames
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::CollapseDefaultFilenames()
{  
  //GLOBAL_ITEM_ID giID;
  ITEM_ID itemID;
  int i, n;
  //POSITION p = GetHeadPosition();
  CString DefFilename;

  //while (p != NULL)
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //ITEM_DATA &data = GetAtPos(p);
    ITEM_DATA &data = *GetItem(i);

    DefFilename.Format("%s%s",ede.TemplateSoundDir(), DEF_HIT_SOUND);
    if (data.HitSound.CompareNoCase(DefFilename)==0)
      data.HitSound = "DEFAULT";

    DefFilename.Format("%s%s", ede.TemplateSoundDir(), DEF_MISS_SOUND);
    if (data.MissSound.CompareNoCase(DefFilename)==0)
      data.MissSound = "DEFAULT";

    DefFilename.Format("%s%s", ede.TemplateSoundDir(), DEF_LAUNCH_SOUND);
    if (data.LaunchSound.CompareNoCase(DefFilename)==0)
      data.LaunchSound = "DEFAULT";


    switch (data.Wpn_Type)
    {
    case HandThrow:
    case Throw:
    case SpellLikeAbility:
    case SpellCaster:
		  DefFilename.Format("%s%s", ede.TemplateMissileArtDir(), DEFAULT_IIRT);
      break;
    case SlingNoAmmo:
		  DefFilename.Format("%s%s", ede.TemplateMissileArtDir(), DEFAULT_IIRS);
      break;
    case Ammo:
		  DefFilename.Format("%s%s", ede.TemplateMissileArtDir(), DEFAULT_IIRA);
      break;
    default: break;
    }

    if (data.MissileArt.filename.CompareNoCase(DefFilename)==0)
      data.MissileArt.filename = "DEFAULT";

    //giID = GetKeyAt(p);
    //itemID = ItemData.GetAt(p).ItemID();
    itemID = PeekItem(i)->ItemID();
    //SetItem(giID, data);
    SetItem(itemID, data);

    //GetNext(p);
  }
}

#endif // UAFEDITOR

//*****************************************************************************
//    NAME: ITEM_LIST::GetNextKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetNextKey()
{
  int key=0;
  POSITION pos;
  if (GetCount() > 0)
  {
    key = m_items.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_ITEMS.
      // Gaps occur when deletions are made to item list
      pos = m_items.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_items.GetNext(pos).key;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key++;
  }
  else
    key = 1;
  return key;
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetReady
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetReady(int index, BOOL ready)
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    m_items.GetAtPos(pos).ready = ready;
}


//*****************************************************************************
//    NAME: ITEM_LIST::ValidItemListIndex
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::ValidItemListIndex(int index)
{ 
  return (m_items.FindKeyPos(index) != NULL); 
}

// called when loading from scripts
//*****************************************************************************
//    NAME: ITEM_LIST::AddItem
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::AddItem(const ITEM &data, BOOL AutoJoin)
{
  int newkey = 0;
  BOOL joined = FALSE;
  if (AutoJoin)
  {
    //if (itemCanBeJoined(data.m_giID))
    if (itemCanBeJoined(data.itemID))
    {
      // look for another instance of item
      int joinIndex = -1;
      POSITION pos = GetHeadPosition();
      while ((pos != NULL) && (joinIndex < 0))
      {
        //if (   (PeekAtPos(pos).m_giID == data.m_giID)
        if (   (PeekAtPos(pos).itemID == data.itemID)
            /*&& (GetAt(pos).key != origItem.key)*/ ) // not needed here
          joinIndex = PeekAtPos(pos).key;
        GetNext(pos);
      }

      // if item instance in list
      if (joinIndex >= 0)
      {
        ITEM joinItem;
        if (GetItem(joinIndex, joinItem)) 
        {
          joinItem.qty += data.qty;
          SetItem(joinIndex, joinItem);
          joined=TRUE;
          newkey=joinIndex;
        }
      }
    }
  }

  if (!joined)
  {
    if (m_items.GetCount() < MAX_ITEMS)
    {
      //int key;
      const_cast<ITEM *>(&data)->key = GetNextKey();
      m_items.Insert(data, data.key);
      //m_items.Insert(data, GetNextKey());
      //itemsModified = true;
      newkey = data.key;
    }
  }
  return newkey;
}

// called when loading binary data that
// was saved with key info
//*****************************************************************************
//    NAME: ITEM_LIST::AddItemWithCurrKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::AddItemWithCurrKey(const ITEM &data)
{
  if (m_items.GetCount() < MAX_ITEMS)
  {
    m_items.Insert(data, data.key);
    itemsModified = true;
    return data.key;
  }
  else
    return 0;
}

void ITEM_LIST::Append(ITEM_LIST& newItems)
{
  POSITION pos;
  pos = newItems.GetHeadPosition();
  while (pos != NULL)
  {
    ITEM& item = newItems.GetNext(pos);
    AddItem(item);
  };
}


//*****************************************************************************
//    NAME: ITEM_LIST::DeleteItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::DeleteItem(int index)
{
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
  {
    if (UnReady(index))
    {
      m_items.RemoveAt(pos);
      itemsModified = true;
      return TRUE;
    }
  }
  return FALSE;
}  

//*****************************************************************************
//    NAME: ITEM_LIST::GetItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::GetItem(int index, ITEM &data)
{
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
  {
    data = m_items.PeekAtPos(pos);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::SetItem(int index, ITEM &data)
{
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
  {
    m_items.GetAtPos(pos) = data;
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::operator=
//
// PURPOSE: 
//
//*****************************************************************************
ITEM_LIST& ITEM_LIST::operator=(const ITEM_LIST& src)
{
  if (&src == this) return *this;
  Clear();
  POSITION pos;
  pos = const_cast<ITEM_LIST&>(src).GetHeadPosition();
  while (pos != NULL)
    AddItemWithCurrKey(const_cast<ITEM_LIST&>(src).GetNext(pos));
  rdyItems = src.rdyItems;
  return *this;
}

//*****************************************************************************
//    NAME: ITEM_LIST::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::operator==(const ITEM_LIST& src) const
{
  if (&src == this) return TRUE;
  POSITION p1=GetHeadPosition();
  POSITION p2=const_cast<ITEM_LIST&>(src).GetHeadPosition();
  while (p1!=NULL)
  {
    if (p2==NULL) return FALSE;
    ITEM item1 = const_cast<ITEM_LIST&>(*this).GetNext(p1);
    ITEM item2 = const_cast<ITEM_LIST&>(src).GetNext(p2);
    if (!(item1 == item2)) return FALSE;
  }
  if (p2!=NULL) return FALSE;
  return TRUE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetReady
//
// PURPOSE: 
//
//*****************************************************************************
// Deprecated
/*
int ITEM_LIST::GetReady(itemLocationType type) const
{ 
  return rdyItems.GetReady(type); 
}
*/
//*****************************************************************************


//*****************************************************************************
//    NAME: ITEM_LIST::GetNextReady
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetReadiedItem(itemReadiedType type, int position) const
{ 
  //return rdyItems.GetReady(type); 
  POSITION pos=NULL;
  const ITEM* pItem;
  const ITEM_DATA *pItemData;
//  WriteDebugString("DEBUG - Item Count = %d\n",m_items.GetCount());
  pos = GetHeadPosition();
  for (pItem=PeekNext(pos); pItem!=NULL; pItem=PeekNext(pos))
  {
//    WriteDebugString("DEBUG - weapon = %s; ready = %d\n",(LPCSTR)pItem->itemID, pItem->ready);
    if (pItem->ready)
    {
      //pItemData = itemData.GetItemData(pItem->m_giID);
      pItemData = itemData.GetItem(pItem->itemID);
//      WriteDebugString("DEBUG - Location Readied = %d\n", pItemData->Location_Readied);
      if (pItemData->Location_Readied == type)
      {
        if (position == 0)
        {
          return pItem->key;
        }
        else 
        {
          position--;
        };
      };
    };
  };
  return NO_READY_ITEM;
}


int ITEM_LIST::GetReadiedCount(itemReadiedType type) const
{
  POSITION pos;
  int result;
  const ITEM *pItem = NULL;
  ITEM_DATA *pItemData;
  pos = m_items.GetHeadPosition();
  if (pos != NULL) pItem = &PeekAtPos(pos);
  result = 0;
  while (pItem != NULL)
  {
    if (pItem->ready)
    {
      //pItemData = itemData.GetItemData(pItem->m_giID);
      pItemData = itemData.GetItem(pItem->itemID);
      if (pItemData->Location_Readied == type) result++;
    };
    pItem = m_items.PeekNext(pos);
  };
  return result;
}

//*****************************************************************************
//    NAME: ITEM_LIST::CanReady
//
// PURPOSE: Check to see if the 'Readied Location' is available.
// As of 20120613, we do this by calling a designer script.
// 
// If there is no script, or it returns an empty string, then
// the item can be readied if no item already exists at the
// current location.
//
// The script can override this by answering with a one character
// string "Y" for 'Yes' and anything else means 'No'.
// The hook receives the number of items already readied at
// this location in hook parameter 5.
// The script can add a message to be displayed in hook parameter 6.
//
//
//*****************************************************************************
BOOL ITEM_LIST::CanReady(itemReadiedType type, CHARACTER *pChar, const ITEM *pItem) const
{ 
  {
    int count;
    ITEM_DATA *pItemData;
    ActorType actor;
    CString result;
    BOOL answer;
    HOOK_PARAMETERS hookParameters;
    SCRIPT_CONTEXT scriptContext;
    pChar->GetContext(&actor);
    //pItemData = itemData.GetItemData(pItem->m_giID);
    pItemData = itemData.GetItem(pItem->itemID);
    scriptContext.SetCharacterContext(pChar);
    scriptContext.SetItemContext(pItemData);
    scriptContext.SetItemContextKey(pItem->key);
    count = GetReadiedCount(type);
    hookParameters[5].Format("%d", count);
    result = pItemData->RunItemScripts(CAN_READY, ScriptCallback_RunAllScripts, NULL);
    answer = TRUE;
    if (result.IsEmpty())
    {
      answer = count == 0;
    }
    else
    {
      if (result[0] != 'Y') answer = FALSE;
    };
    if (!hookParameters[6].IsEmpty())
    {
#ifdef UAFEngine
      errorText = hookParameters[6];
#endif
      SetMiscError(ErrorText);
    }
    return answer;
    //specAbs.RunScripts(scriptName, ScriptCallback_RunAllScripts, NULL, name, "");
  };
 
  /*
  //return rdyItems.GetReady(type); 
  switch (type) 
  {
		case WeaponHand:
    case ShieldHand:
    case BodyArmor:
    case Hands:
    case Head:
    case Waist:
    case BodyRobe:
    case Back:
    case Feet:
    case AmmoQuiver:
      return (GetNextReady(type) == NO_READY_ITEM);
    case Fingers:
      {
        int itemKey;
        itemKey = GetNextReady(type);
        if (itemKey == NO_READY_ITEM) return TRUE;
        itemKey = GetNextReady(type, itemKey);
        return (itemKey == NO_READY_ITEM);
      };
    default:
      WriteDebugString("BOGUS body location %i in READY_ITEMS::Ready\n", type);
      return NO_READY_ITEM;
  };
  */
}


//    NAME: ITEM_LIST::IsReady
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::IsReady(int index) const
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    return m_items.PeekAtPos(pos).ready; 
  else 
    return FALSE; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::IsCursed
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::IsCursed(int index) const
{
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    return m_items.PeekAtPos(pos).cursed;
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::RemoveCurse
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::RemoveCurse(int index)
{
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    m_items.GetAtPos(pos).cursed = FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::HaveItem
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_LIST::HaveItem(GLOBAL_ITEM_ID giID)
BOOL ITEM_LIST::HaveItem(const ITEM_ID& itemID)
{
  POSITION pos;
  pos = GetHeadPosition();
  while (pos != NULL)
  {
    //if (m_items.GetNext(pos).m_giID == giID)
    if (m_items.GetNext(pos).itemID == itemID)
      return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetListKeyByItemKey
//
// PURPOSE: 
//
//*****************************************************************************
//int ITEM_LIST::GetListKeyByItemKey(GLOBAL_ITEM_ID giID) const
int ITEM_LIST::GetListKeyByItemName(const ITEM_ID& itemID) const
{
  POSITION pos;
  pos = GetHeadPosition();
  while (pos != NULL)
  {
    //if (m_items.PeekAtPos(pos).m_giID == giID)
    if (m_items.PeekAtPos(pos).itemID == itemID)
      return m_items.PeekAtPos(pos).key;
    m_items.PeekNext(pos);
  }
  return -1;
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetItem
//
// PURPOSE: 
//
//*****************************************************************************
//GLOBAL_ITEM_ID ITEM_LIST::GetItem(int index) const
ITEM_ID ITEM_LIST::GetItem(int index) const
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    //return m_items.PeekAtPos(pos).m_giID; 
    return m_items.PeekAtPos(pos).itemID; 
  else 
    return ITEM_ID(); 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetITEM
//
// PURPOSE: 
//
//*****************************************************************************
ITEM *ITEM_LIST::GetITEM(int index)
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    return &m_items.GetAtPos(pos); 
  else 
    return NULL; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetId
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::GetId(int index) const
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    return m_items.PeekAtPos(pos).identified; 
  else 
    return FALSE; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetQty
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetQty(int index) const
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    return m_items.PeekAtPos(pos).qty; 
  else 
    return 0; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetPurchasePrice
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetPurchasePrice(int index) const
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    return m_items.PeekAtPos(pos).paid; 
  else 
    return -1; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetPurchasePrice
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetPurchasePrice(int index, int price)
{
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    m_items.GetAtPos(pos).paid = price; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetCharges
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetCharges(int index) const
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    return m_items.PeekAtPos(pos).charges; 
  else 
    return 0; 
}
//*****************************************************************************
//    NAME: ITEM_LIST::GetAmmoClass
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_LIST::GetAmmoClass(int index) const
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
  {
    //CString tmp(itemData.GetAmmoClass(m_items.PeekAtPos(pos).m_giID));
    CString tmp(itemData.GetAmmoClass(m_items.PeekAtPos(pos).itemID));
    return tmp;
  }
  else 
    return ""; 
}
//*****************************************************************************
//    NAME: ITEM_LIST::SetId
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetId(int index, BOOL id)
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    m_items.GetAtPos(pos).identified = id;
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetQty
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetQty(int index, int qty)
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
  {
    m_items.GetAtPos(pos).qty = qty;
    if (m_items.PeekAtPos(pos).qty <= 0)
    {
      if (UnReady(index))
        DeleteItem(index);
    }
  }
}

//*****************************************************************************
//    NAME: ITEM_LIST::AdjustQty
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::AdjustQty(int index, int qty)
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
  {
    m_items.GetAtPos(pos).qty += qty;
    qty = m_items.PeekAtPos(pos).qty;
    if (qty < 0) qty = 0;
    if (qty == 0)
    {
      if (UnReady(index))
        DeleteItem(index);
    };
    return qty;
  };
  return -1;
}

//*****************************************************************************
//    NAME: ITEM_LIST::ResetItemCharges
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::ResetItemCharges(BOOL IsNewDay)
{
  ITEM_DATA *pIdata;
  POSITION ipos = GetHeadPosition();
  while (ipos != NULL)
  {
    //pIdata = itemData.GetItemData(PeekAtPos(ipos).m_giID);
    pIdata = itemData.GetItem(PeekAtPos(ipos).itemID);
    if (pIdata != NULL)
    {
      switch (pIdata->Recharge_Rate)
      {
      case irrNever:
        // no adjustments needed
        break;
      case irrDaily:
        // reset available charges
        if (IsNewDay)
          GetAtPos(ipos).charges = pIdata->Num_Charges;
        break;
      }
    }
    GetNext(ipos);
  }
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetCharges
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetCharges(int index, int chrgs)
{ 
  POSITION pos;
  if ((pos = m_items.FindKeyPos(index)) != NULL)
    m_items.GetAtPos(pos).charges = chrgs; 
}

//*****************************************************************************
//    NAME: ITEM_DATA::GetItemName
//
// PURPOSE: 
//
//*****************************************************************************
// PRS 20110107 Replaced with QualifiedName() and UnqualifiedName()
//CString ITEM_DATA::GetItemName() const
//{ 
//  if (!Id_Name.IsEmpty()) 
//    return Id_Name; 
//  else 
//    return Name; 
//}

//*****************************************************************************
//    NAME: ITEM_DATA::PlayHit
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PlayHit()
{
  if (!PlaySoundEffects) return;
  if (HitSound == "") return;
  if (!ValidSound(hHitSound))
    hHitSound = LoadSound(HitSound);
  if (!PlaySound(hHitSound,FALSE))
    hHitSound=-1;
}
//*****************************************************************************
//    NAME: ITEM_DATA::PlayMiss
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PlayMiss()
{
  if (!PlaySoundEffects) return;
  if (MissSound == "") return;
  if (!ValidSound(hMissSound))
    hMissSound = LoadSound(MissSound);
  if (!PlaySound(hMissSound,FALSE))
    hMissSound=-1;
}
//*****************************************************************************
//    NAME: ITEM_DATA::PlayLaunch
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PlayLaunch()
{
  if (!PlaySoundEffects) return;
  if (LaunchSound == "") return;
  if (!ValidSound(hLaunchSound))
    hLaunchSound = LoadSound(LaunchSound);
  if (!PlaySound(hLaunchSound,FALSE))
    hLaunchSound=-1;
}

//*****************************************************************************
//    NAME: ITEM_DATA::ClearSounds
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::ClearSounds()
{
  if (hHitSound >= 0)
    ClearSound(&hHitSound);
  if (hMissSound >= 0)
    ClearSound(&hMissSound);
  if (hLaunchSound >= 0)
    ClearSound(&hLaunchSound);
  hHitSound = -1;
  hMissSound = -1;
  hLaunchSound = -1;
}
//*****************************************************************************
//    NAME: ITEM_DATA::LoadArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::LoadArt()
{
  //MissileArt.LoadPicSurfaces("it_");
  MissileArt.LoadPicSurfaces("");
  //HitArt.LoadPicSurfaces("it_");
  HitArt.LoadPicSurfaces("");
}
//*****************************************************************************
//    NAME: ITEM_DATA::ClearArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::ClearArt() // release surface and clear pic data
{
  MissileArt.Clear();
  HitArt.Clear();
}
//*****************************************************************************
//    NAME: ITEM_DATA::FreeArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::FreeArt() // release surface
{
  MissileArt.FreePic();
  HitArt.FreePic();
}
//*****************************************************************************
//    NAME: ITEM_DATA::SetAttribute
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::SetASL(CString akey, CString value, BYTE flags)
{
  item_asl.Insert(akey, value, flags);
}

//*****************************************************************************
//    NAME: ITEM_DATA::GetAttribute
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA::GetASL(CString akey, CString &value) const
{
  const ASLENTRY *pEntry;
  pEntry = item_asl.Find(akey);
  if (pEntry != NULL)
  {
    value = *pEntry->pValue();
    return TRUE;
  }
  return FALSE;
}
/*
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetNextKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::GetNextKey()
{
  int key;
  POSITION pos;
  if (GetCount() > 0)
  {
    key = ItemData.GetTail().m_giID.m_key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_ITEMS.
      // Gaps occur when deletions are made to item list
      pos = ItemData.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = ItemData.GetNext(pos).m_giID.m_key;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key++;
  }
  else
    key = 1;
  return key;
}

*/

#ifdef UAFEDITOR
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::Validate(const ITEM_DATA &data, FILE *pFile) const
{
  BOOL success = TRUE;
  if (data.Wpn_Type != NotWeapon)
  {
    if (data.HitSound != "")
    {
      if (!FileExists(data.HitSound))
      {
        WriteValidateLog(pFile, "Item '%s' missing Hit Sound file '%s'\n",
                         data.UniqueName(), 
                         data.HitSound);
        success = FALSE;
      }
    }

    if (data.MissSound != "")
    {
      if (!FileExists(data.MissSound))
      {
        WriteValidateLog(pFile, "Item '%s' missing Miss Sound file '%s'\n",
                         data.UniqueName(), 
                         data.MissSound);
        success = FALSE;
      }
    }
    if (   (data.Wpn_Type == SlingNoAmmo)
        || (data.Wpn_Type == Bow)
        || (data.Wpn_Type == Crossbow)
        || (data.Wpn_Type == SpellCaster)
        || (data.Wpn_Type == SpellLikeAbility)
        || (data.Wpn_Type == Throw))
    {
      if (data.LaunchSound != "")
      {
        if (!FileExists(data.LaunchSound))
        {
          WriteValidateLog(pFile, "Item '%s' missing Launch Sound file '%s'\n",
                           data.UniqueName(), 
                           data.LaunchSound);
          success = FALSE;
        }
      }
    }
  }

  POSITION pos = data.specAbs.GetHeadPosition();
  while (pos!=NULL)
  {
    const CStringPAIR *spab = data.specAbs.GetNextData(pos);
    if (spab!=NULL)
    {
      if (!specialAbilitiesData.FindString(spab->Key(),ActivationScriptName).IsEmpty())
      {
        if (specialAbilitiesData.FindString(spab->Key(),"ActivationScriptBin").GetLength() == 0)
        {
          success = FALSE;
          WriteValidateLog(pFile, "Item '%s' special ability script '%s' did not compile\n",
                           data.UniqueName(), specialAbilitiesData.FindString(spab->Key(),ActivationScriptName));
        }
      }
    }
  }
  return success;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::Validate(FILE *pFile)
{
  BOOL success = TRUE;
  //POSITION pos = ItemData.GetHeadPosition();
  int i, n;
  n = GetCount();
  //while (pos != NULL)
  for (i=0; i<n; i++)
  {
    //if (!Validate(ItemData.GetNext(pos),pFile))
    if (!Validate(*PeekItem(i),pFile))
      success = FALSE;
  }
  return success;
}


void ITEM_DATA_TYPE::CrossReference(CR_LIST *pCRList)
{
  //POSITION pos = ItemData.GetHeadPosition();
  //while (pos != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //ItemData.GetNext(pos).CrossReference(pCRList);
    PeekItem(i)->CrossReference(pCRList);
  };
}


//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::Validate(GLOBAL_ITEM_ID giID, FILE *pFile) const
BOOL ITEM_DATA_TYPE::Validate(const ITEM_ID& itemID, FILE *pFile) const
{
  BOOL success = TRUE;
  //POSITION pos;
  int indx;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //if ((pos = LocateItemData(itemID)) != NULL)
  indx = LocateItem(itemID);
  if (indx >= 0)
  {
    //if (!Validate(ItemData.PeekAtPos(pos),pFile))
    //if (!Validate(*PeekItemData(pos),pFile))
    if (!Validate(*PeekItem(indx),pFile))
      success = FALSE;
  }
  else
  {
    //WriteValidateLog(pFile, "Bogus item key %i\n", giID);
    WriteValidateLog(pFile, "Bogus item key %s\n", itemID);
    success = FALSE;
  }
  return success;
}
#endif
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PlayHit
//
// PURPOSE: 
//
//*****************************************************************************
//void ITEM_DATA_TYPE::PlayHit(GLOBAL_ITEM_ID giID)
void ITEM_DATA_TYPE::PlayHit(const ITEM_ID& itemID)
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  ItemData.GetAtPos(pos).PlayHit();
  ITEM_DATA *pItem;
  pItem = GetItem(itemID);
  if (pItem != NULL)
  {
    pItem->PlayHit();
  };
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PlayMiss
//
// PURPOSE: 
//
//*****************************************************************************
//void ITEM_DATA_TYPE::PlayMiss(GLOBAL_ITEM_ID giID)
void ITEM_DATA_TYPE::PlayMiss(const ITEM_ID& itemID)
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  ItemData.GetAtPos(pos).PlayMiss();
  ITEM_DATA *pItem;
  pItem = GetItem(itemID);
  if (pItem != NULL)
  {
    pItem->PlayMiss();
  };
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PlayLaunch
//
// PURPOSE: 
//
//*****************************************************************************
//void ITEM_DATA_TYPE::PlayLaunch(GLOBAL_ITEM_ID giID)
void ITEM_DATA_TYPE::PlayLaunch(const ITEM_ID& itemID)
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  ItemData.GetAtPos(pos).PlayLaunch();
  ITEM_DATA *pItem;
  pItem = GetItem(itemID);
  if (pItem != NULL)
  {
    pItem->PlayLaunch();
  };
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::ClearSounds
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::ClearSounds()
{
  //CSingleLock sLock(&m_CS,TRUE);
  //POSITION pos;
  //pos = ItemData.GetHeadPosition();
  //while (pos != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //ItemData.GetNext(pos).ClearSounds();
    GetItem(i)->ClearSounds();
  };
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::FreeArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::FreeArt()
{
  //CSingleLock sLock(&m_CS,TRUE);
  //POSITION pos;
  //pos = ItemData.GetHeadPosition();
  //while (pos != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //ItemData.GetNext(pos).FreeArt();
    GetItem(i)->FreeArt();
  };
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemCommonName
//
// PURPOSE: 
//
//*****************************************************************************
//CString ITEM_DATA_TYPE::GetItemCommonName(GLOBAL_ITEM_ID giID) const
CString ITEM_DATA_TYPE::GetItemCommonName(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //{
  //  return ItemData.PeekAtPos(pos).CommonName();
  //}
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->CommonName();
  }
  else
    return CString("No Item");
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemUniqueName
//
// PURPOSE: 
//
//*****************************************************************************
//CString ITEM_DATA_TYPE::GetItemUniqueName(GLOBAL_ITEM_ID giID) const
CString ITEM_DATA_TYPE::GetItemUniqueName(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //{
  //  return ItemData.PeekAtPos(pos).UniqueName();
  //}
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->UniqueName();
  }
  else
    return CString("No Item");
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemIdName
//
// PURPOSE: 
//
//*****************************************************************************
//CString ITEM_DATA_TYPE::GetItemIdName(GLOBAL_ITEM_ID giID) const
CString ITEM_DATA_TYPE::GetItemIdName(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).IdName();
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->IdName();
  }
  else
    return CString("No Item");
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemCost
//
// PURPOSE: 
//
//*****************************************************************************
//long ITEM_DATA_TYPE::GetItemCost(GLOBAL_ITEM_ID giID) const
long ITEM_DATA_TYPE::GetItemCost(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).Cost;
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->Cost;
  }
  else
    return 0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemExpWorth
//
// PURPOSE: 
//
//*****************************************************************************
//long ITEM_DATA_TYPE::GetItemExpWorth(GLOBAL_ITEM_ID giID) const
long ITEM_DATA_TYPE::GetItemExpWorth(const ITEM_ID& itemID) const
{
  //POSITION pos;
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  if (pItem != NULL)
  {
    //long val = ItemData.PeekAtPos(pos).Experience;
    long val = pItem->Experience;
    double mod = GetAllExpMod();
    if (mod != 0.0) val += ((mod/100.0)*val);
    if (val < 0) val=0;
    return val;
  }
  else
    return 0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemROF
//
// PURPOSE: 
//
//*****************************************************************************
//double ITEM_DATA_TYPE::GetItemROF(GLOBAL_ITEM_ID giID) const
double ITEM_DATA_TYPE::GetItemROF(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).ROF_Per_Round;
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->ROF_Per_Round;
  }
  else
    return 0.0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemRange
//
// PURPOSE: 
//
//*****************************************************************************
//long ITEM_DATA_TYPE::GetItemRange(GLOBAL_ITEM_ID giID) const
long ITEM_DATA_TYPE::GetItemRange(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).Range;
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->RangeMax;
  }
  else
    return 0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemBundleQty
//
// PURPOSE: 
//
//*****************************************************************************
//long ITEM_DATA_TYPE::GetItemBundleQty(GLOBAL_ITEM_ID giID) const
long ITEM_DATA_TYPE::GetItemBundleQty(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).Bundle_Qty;
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->Bundle_Qty;
  }
  else
    return 0;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemUsable
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::GetItemUsable(GLOBAL_ITEM_ID giID) const
BOOL ITEM_DATA_TYPE::GetItemUsable(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).IsUsable();
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->IsUsable();
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetAmmoClass
//
// PURPOSE: 
//
//*****************************************************************************
//CString ITEM_DATA_TYPE::GetAmmoClass(GLOBAL_ITEM_ID giID) const
CString ITEM_DATA_TYPE::GetAmmoClass(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).AmmoType;
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->AmmoType;
  }
  else
    return "";
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetWpnType
//
// PURPOSE: 
//
//*****************************************************************************
//weaponClassType ITEM_DATA_TYPE::GetWpnType(GLOBAL_ITEM_ID giID) const
weaponClassType ITEM_DATA_TYPE::GetWpnType(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //  return ItemData.PeekAtPos(pos).Wpn_Type;
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem != NULL)
  {
    return pItem->Wpn_Type;
  }
  else
    return NotWeapon;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::HaveItem
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::HaveItem(GLOBAL_ITEM_ID giID)  const
BOOL ITEM_DATA_TYPE::HaveItem(const ITEM_ID& itemID)  const
{
  //return (ItemData.FindKeyPos(giID.m_key) != NULL);
  //return PeekItemData(itemID) != NULL;
  return LocateItem(itemID) >= 0;
}


//#ifndef UAFEngine
// called when loading from scripts, or
// when adding from editor GUI
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::AddItem
//
// PURPOSE: 
//
//*****************************************************************************
//GLOBAL_ITEM_ID ITEM_DATA_TYPE::AddItem(ITEM_DATA &data)
int ITEM_DATA_TYPE::AddItem(const ITEM_DATA& data)
{
  // Returns the CArray index of the new item
  //CSingleLock sLock(&m_CS,TRUE);
  ITEM_ID itemID;
  int result;
  // (strlen(data.UniqueName()) == 0) return GLOBAL_ITEM_ID();
  if (strlen(data.UniqueName()) == 0) return -1;
  // Do not allow multiple items with same full name.
  // (m_FindName(data.UniqueName()).IsValidItem()) return GLOBAL_ITEM_ID();
  itemID = data.ItemID();
  if (LocateItem(itemID) >= 0) return -1;
  //if (ItemData.GetCount() < MAX_ITEMS)
  if (GetCount() < MAX_ITEMS)
  {
    //data.m_giID = GLOBAL_ITEM_ID(GetNextKey());
    //ItemData.Insert(data, data.m_giID.m_key);
    //ItemData.Insert(data);
    //ItemData.AddTail(const_cast<ITEM_DATA&>(data));
    result = ItemData.Add(const_cast<ITEM_DATA&>(data));
    AddAmmoType(data.AmmoType);
    //return data.m_giID;
    return result;
  }
  else
    //return GLOBAL_ITEM_ID();
    return -1;
}

//#endif

/*
// called when loading binary data that
// was saved with key info
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::AddItemWithCurrKey
//
// PURPOSE: 
//
//*****************************************************************************
//GLOBAL_ITEM_ID ITEM_DATA_TYPE::AddItemWithCurrKey(ITEM_DATA &data)
CString ITEM_DATA_TYPE::AddItemWithCurrKey(ITEM_DATA &data)
{
  CSingleLock sLock(&m_CS,TRUE);
  if (ItemData.GetCount() < MAX_ITEMS)
  {
    //ItemData.Insert(data, data.m_giID.m_key);
    ItemData.Insert(data);
    AddAmmoType(data.AmmoType);
    return data.m_giID;
  }
  else
    return 0;
}
*/

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SetItem
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::SetItem(GLOBAL_ITEM_ID giID, ITEM_DATA &data)
BOOL ITEM_DATA_TYPE::SetItem(const ITEM_ID& itemID, ITEM_DATA &data)
{
  //CSingleLock sLock(&m_CS,TRUE);
  //POSITION pos;
  ITEM_DATA *pItem;
  pItem = GetItem(itemID);
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  if (pItem != NULL)
  {
    //CString oldAmmoType(ItemData.PeekAtPos(pos).AmmoType);
    CString oldAmmoType(pItem->AmmoType);
    //ItemData.GetAtPos(pos) = data;
    *pItem = data;

    if (data.AmmoType != oldAmmoType)
    {
      RemoveAmmoType(oldAmmoType);
      AddAmmoType(data.AmmoType);
    }
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::DeleteItemEvents
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::DeleteItemEvents(GLOBAL_ITEM_ID giID)
BOOL ITEM_DATA_TYPE::DeleteItemEvents(const ITEM_ID& itemID)
{
  //CSingleLock sLock(&m_CS,TRUE);
  //POSITION pos;
  ITEM_DATA *pItem;
  pItem = GetItem(itemID);
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  if (pItem != NULL)
  {
    //globalData.eventData.DeleteEvents(ItemData.PeekAtPos(pos).m_useEvent);
    //globalData.eventData.DeleteEvents(ItemData.PeekAtPos(pos).ExamineEvent);
    //ItemData.GetAtPos(pos).ExamineLabel="EXAMINE";
    globalData.eventData.DeleteEvents(pItem->m_useEvent);
    globalData.eventData.DeleteEvents(pItem->ExamineEvent);
    pItem->ExamineLabel="EXAMINE";
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::RemoveItem
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::RemoveItem(GLOBAL_ITEM_ID giID)
BOOL ITEM_DATA_TYPE::RemoveItem(const ITEM_ID& itemID)
{
  //CSingleLock sLock(&m_CS,TRUE);
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //if ((pos = LocateItem(itemID)) != NULL)
  int i;
  if ((i = LocateItem(itemID)) >= 0)
  {
    //globalData.eventData.DeleteEvents(ItemData.GetAt(pos).event);
    //globalData.eventData.DeleteEvents(ItemData.GetAt(pos).ExamineEvent);
    //ItemData.GetAt(pos).ExamineLabel="EXAMINE";
    //CString oldAmmoType(ItemData.PeekAtPos(pos).AmmoType);
    //CString oldAmmoType(PeekItem(pos)->AmmoType);
    CString oldAmmoType(PeekItem(i)->AmmoType);
    //ItemData.RemoveAt(pos);
    ItemData.RemoveAt(i);
    RemoveAmmoType(oldAmmoType);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::HaveItemThatUsesAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::HaveItemThatUsesAmmoType(CString data) const
{
  data.TrimLeft();
  data.TrimRight();
  if (data.IsEmpty()) return FALSE;
  //POSITION ipos = ItemData.GetHeadPosition();
  //while (ipos!=NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //if (ItemData.PeekAtPos(ipos).AmmoType == data)
    //if (PeekItemData(ipos)->AmmoType == data)
    if (PeekItem(i)->AmmoType == data)
      return TRUE;
    //ItemData.PeekNext(ipos);
    //PeekNext(ipos);
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::AddAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::AddAmmoType(CString data)
{
  data.TrimLeft();
  data.TrimRight();
  if (data.IsEmpty()) return;
  if (HaveAmmoType(data)==-1)
    m_AmmoTypes.SetAtGrow(m_AmmoTypes.GetSize(), data);
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::RemoveAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::RemoveAmmoType(CString data)
{
  int index = HaveAmmoType(data);
  if (index >= 0)
  {
    if (!HaveItemThatUsesAmmoType(data))
      m_AmmoTypes.RemoveAt(index);
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::HaveAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::HaveAmmoType(CString data)
{
  if (data.IsEmpty()) return -1;
  for (int i=0;i<m_AmmoTypes.GetSize();i++)
  {
    if (m_AmmoTypes[i]==data)
      return i;
  }
  return -1;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemAttribute
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::GetItemAttribute(GLOBAL_ITEM_ID giID, CString akey, CString value) const
BOOL ITEM_DATA_TYPE::GetItemAttribute(const ITEM_ID& itemID, CString akey, CString value) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  //if ((pos = LocateItemData(itemID)) != NULL)
  int i;
  if ((i = LocateItem(itemID)) >= 0)
    //return (ItemData.PeekAtPos(pos).GetASL(akey, value));
    //return (PeekItemData(pos)->GetASL(akey, value));
    return (PeekItem(i)->GetASL(akey, value));
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SetItemAttribute
//
// PURPOSE: 
//
//*****************************************************************************
//BOOL ITEM_DATA_TYPE::SetItemAttribute(GLOBAL_ITEM_ID giID, CString akey, CString value)
BOOL ITEM_DATA_TYPE::SetItemAttribute(const ITEM_ID& itemID, CString akey, CString value)
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
  int i;
  //if ((pos = LocateItemData(itemID)) != NULL)
  if ((i = LocateItem(itemID)) >= 0)
  {
    //ItemData.GetAtPos(pos).SetASL(akey, value, ASLF_READONLY);
    //ItemData.GetAt(pos).SetASL(akey, value, ASLF_READONLY);
    GetItem(i)->SetASL(akey, value, ASLF_READONLY);
    return TRUE;
  }
  else
    return FALSE;
}

/*
//*****************************************************************************
//    NAME: *ITEM_DATA_TYPE::GetItemData
//
// PURPOSE: 
//
//*****************************************************************************
ITEM_DATA *ITEM_DATA_TYPE::GetItemData(GLOBAL_ITEM_ID giID)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(giID.m_key)) != NULL)
    return &(ItemData.GetAtPos(pos));
  else
    return NULL;
}

*/
/*
//*****************************************************************************
//    NAME: *ITEM_DATA_TYPE::GetItemData
//
// PURPOSE: 
//
//*****************************************************************************
ITEM_DATA *ITEM_DATA_TYPE::GetItem(const ITEM_ID& itemID)
{
  POSITION pos;
  pos=ItemData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    //if (ItemData.PeekAtPos(pos).UniqueName()==itemID) 
    if (PeekItemData(pos)->UniqueName()==itemID) 
          //return &ItemData.GetAtPos(pos);
          return &ItemData.GetAt(pos);
    ItemData.GetNext(pos);
  };
  return NULL;
}
*/
/*
// No locking... for internal use
//*****************************************************************************
//    NAME: 
//
// PURPOSE: 
//
//*****************************************************************************
GLOBAL_ITEM_ID ITEM_DATA_TYPE::m_FindName (const CString& name) const
// Return GLOBAL_ITEM_ID associated with the named object, or NO_ITEM
{
  POSITION pos;
  pos=ItemData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (ItemData.PeekAtPos(pos).UniqueName()==name) 
          return GLOBAL_ITEM_ID(ItemData.PeekAtPos(pos).m_giID.m_key);
    ItemData.PeekNext(pos);
  };
  return GLOBAL_ITEM_ID(NO_ITEM);
}

*/

/*
// With locking...for public use
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::FindName
//
// PURPOSE: 
//
//*****************************************************************************
//GLOBAL_ITEM_ID ITEM_DATA_TYPE::FindName(const CString& name)
CString ITEM_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}
*/

//POSITION ITEM_DATA_TYPE::FindIDPos(const ITEM_ID& itemID) const
int ITEM_DATA_TYPE::LocateItem(const ITEM_ID& itemID) const
{
  //POSITION pos;
  //pos=ItemData.GetHeadPosition(); 
  //while (pos!=NULL)
  int i, n;
  n = GetCount();
  if (    (itemID.hint >= 0)
       && (itemID.hint < n) 
       && (PeekItem(itemID.hint)->m_uniqueName == itemID)) return itemID.hint;
  for (i=0; i<n; i++)
  {
    //if (PeekItemData(pos)->UniqueName()==itemID) 
    if (PeekItem(i)->m_uniqueName==itemID) 
    {
          //return pos;
      itemID.hint = i;
      return i;
    //PeekNext(pos);
    };
  };
  //return pos;
  return -1;
}

/*
//ITEM_DATA *ITEM_DATA_TYPE::FindItemID(const ITEM_ID& itemID)
ITEM_DATA *ITEM_DATA_TYPE::GetItemData(const ITEM_ID& itemID)
{
  POSITION pos;
  pos = FindIDPos(itemID);
  if (pos == NULL) return NULL;
  //return &ItemData.GetAtPos(pos);
  return &ItemData.GetAt(pos);
}

*/
#ifdef UAFEngine

//*****************************************************************************
//    NAME: ITEM_DATA::CastSpell
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::CastSpell(int spell) const
{
  // nothing yet
  die(0xab523);
}
//*****************************************************************************
//    NAME: ITEM_DATA::SpellActivate
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::SpellActivate(const PENDING_SPELL &data) const
{
  // nothing yet
  die(0xab524);
}


//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::CastSpell
//
// PURPOSE: 
//
//*****************************************************************************
//void ITEM_DATA_TYPE::CastSpell(int index, int spell) const
void ITEM_DATA_TYPE::CastSpell(const ITEM_ID& itemID, int spell) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(index)) != NULL)
  //if ((pos = ItemData.FindIDPos(itemID)) != NULL)
  //if ((pos = LocateItemData(itemID)) != NULL)
  const ITEM_DATA *pItem;
  pItem = PeekItem(itemID);
  if (pItem!= NULL)
  {
    //ItemData.PeekAtPos(pos).CastSpell(spell);
    //PeekAtPos(pos).CastSpell(spell);
    pItem->CastSpell(spell);
  };
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SpellActivate
//
// PURPOSE: 
//
//*****************************************************************************
//void ITEM_DATA_TYPE::SpellActivate(int index, const PENDING_SPELL &data) const
void ITEM_DATA_TYPE::SpellActivate(const ITEM_ID& itemID, const PENDING_SPELL &data) const
{
  //POSITION pos;
  //if ((pos = ItemData.FindKeyPos(index)) != NULL)
  //if ((pos = LocateItemData(itemID)) != NULL)
  int i;
  i = LocateItem(itemID);
  if (i >= 0)
  {
    //PeekItemData(pos)->SpellActivate(data);
    PeekItem(i)->SpellActivate(data);
  };
}



//void SetItemContext(GLOBAL_ITEM_ID giID)
//{
//  itemContext.m_giID = giID;
//  itemContext.m_pItemData = itemData.GetItemData(giID);
//}

//void ClearItemContext(void)
//{
//  itemContext.Clear();
//}

CString ITEM_CONTEXT::Name(void)
{
  if (m_pItemData == NULL) return emptyString;
  return m_pItemData->UniqueName();
}

#endif // UAFEngine spell code

#ifdef UAFEDITOR
ITEM_ID ITEM_DATA_TYPE::FindPreVersionSpellNamesItemID(int preSpellNameKey) const
{
  //POSITION pos;
  //pos = GetHeadPosition();
  //while (pos != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    const ITEM_DATA *pItem;
    //pItem = PeekItemData(pos);
    pItem = PeekItem(i);
    if (pItem->preSpellNameKey == preSpellNameKey)
    {
      return pItem->ItemID();
    };
    //GetNext(pos);
  };
  MsgBoxInfo("Searching for an Item that was defined by its 'key'.  We lost that information "
             "at about version 0.998101 of the editor.  If this is a problem for you, please "
             "contact the DC development team for help.");
  return ITEM_ID();
}
#endif

//ITEM_CONTEXT itemContext;
