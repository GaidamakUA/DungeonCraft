/******************************************************************************
* Filename: Monster.cpp
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

#ifdef UAFEDITOR
#include "..\UAFWinEd\UAFWinEd.h"
#include "class.h"
#include "..\UAFWinEd\CrossReference.h"
#else
#include "externs.h"
#include "..\UAFWin\Dungeon.h"
#endif

#include "items.h"
#include "globalData.h"
#include "SoundMgr.h"
#include "monster.h"
#include "PicSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void die(CString errorMsg);
void StoreStringAsASL(A_ASLENTRY_L &asl, CString &data, const char *prefix, unsigned char flags);
void RetrieveStringFromASL(A_ASLENTRY_L &asl, CString &data, const char *prefix);
extern CString ActivationScriptName;
extern CString DeActivationScriptName;

extern const double VersionSpellNames;
extern const double VersionSaveIDs;
extern const double VersionSpellIDs;
extern const double PRODUCT_VER;


// 20121212 PRS IMPLEMENT_SERIAL( MONSTER_DATA, CObject, 1 )
// 20121212 PRS IMPLEMENT_SERIAL( MONSTER_DATA_TYPE, CObject, 1 )


#ifdef UAFEDITOR
void MONSTER_ATTACK_DETAILS::CrossReference(CR_LIST *pCRList, CR_REFERENCE *pCRReference) const
{
  //pCRList->CR_AddSpellReference(spelldbkey, pCRReference);
  pCRList->CR_AddSpellReference(spellID, pCRReference);
};

#endif




void MONSTER_ATTACK_DETAILS::Serialize(CArchive &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << sides;
    ar << nbr;
    ar << bonus;
    AS(ar,attackMsg);
    //spelldbkey.Serialize(ar, version); //ar << spelldbkey;
    ar << spellID; 
    ar << spellclass;
    ar << spelllevel;
  }
  else
  {
    ar >> sides;
    ar >> nbr;
    ar >> bonus;
    DAS(ar,attackMsg);
    if (version >= _VERSION_0904_)
    {
      //spelldbkey.Serialize(ar, version); //ar >> spelldbkey;      
      ar >> spellID; //ar >> spelldbkey;      
      ar >> spellclass;
      ar >> spelllevel;
    }
  }
}
void MONSTER_ATTACK_DETAILS::Serialize(CAR &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << sides;
    ar << nbr;
    ar << bonus;
    AS(ar,attackMsg);
    //spelldbkey.Serialize(ar, version); //ar << spelldbkey;
    ar << spellID;
    ar << spellclass;
    ar << spelllevel;
  }
  else
  {
    ar >> sides;
    ar >> nbr;
    ar >> bonus;
    DAS(ar,attackMsg);
    if (version >= _VERSION_0904_)
    {
      //spelldbkey.Serialize(ar, version); //ar >> spelldbkey;
#ifdef UAFEDITOR
      if (version < VersionSpellNames)
      {
        ar >> preVersionSpellNames_gsID;
      }
      else
#endif
      {
        ar >> spellID;
      };
      ar >> spellclass;
      ar >> spelllevel;
    }
  }
}





MONSTER_DATA_TYPE monsterData;

MONSTER_DATA::MONSTER_DATA() :
  // ASL names are "MONSTER_DATA_ATTRIBUTES"
	mon_asl(),
	temp_asl(),
 //hHitSound(-1), hMissSound(-1), hMoveSound(-1), hDeathSound(-1)
 HitSound(), 
 MissSound(), 
 MoveSound(), 
 DeathSound(),
 specAbs(true)
{ 
  //key=0;
  Name="";
  Icon.filename="DEFAULT";
  Icon.picType=IconDib;
  Icon.SetDefaults();
  HitSound = "DEFAULT";
  MissSound = "DEFAULT";
  MoveSound="DEFAULT";
  DeathSound="DEFAULT";
  ClearSounds();
  Intelligence = 0;
  Armor_Class = 0;
  Movement = 0;
  Hit_Dice = 0.0F;
  UseHitDice=TRUE;
  Hit_Dice_Bonus = 0;
  THAC0 = 20;
  //Nbr_Attacks = 0;
  //Dmg_Dice_for_Attack = 0;
  //Dmg_Dice_Bonus = 0;
  //Nbr_Dmg_Dice = 0;
  Magic_Resistance = 0;
  Size = Medium;
  Morale = 0;
  XP_Value = 0;
 	specAbs.Clear();
  //ItemMask = AllClasses;
  Form_Type = 0;
  Penalty_Type = 0;
  Immunity_Type = 0;
  MiscOptions_Type = 0;
  myItems.Clear();
  money.Clear();
  //undeadStatus = NotUndead;
  undeadType.Empty();
  attackData.Clear();
}


int MONSTER_DATA::GetDataSize(void) const
{
  return sizeof(*this) + specAbs.GetDataSize();
}



void MONSTER_DATA::Save(CAR& car) const
{
  mon_asl.Save(car, "MONSTER_DATA_ATTRIBUTES");
}

void MONSTER_DATA::PrepareForRestore(void) {
	temp_asl.Clear();
}

void MONSTER_DATA::Restore(CArchive& ar) 
{
  temp_asl.Serialize(ar, "MONSTER_DATA_ATTRIBUTES");
}

void MONSTER_DATA::Restore(CAR& car) 
{
  temp_asl.Serialize(car, "MONSTER_DATA_ATTRIBUTES");
}

void MONSTER_DATA::CommitRestore(void) 
{
	mon_asl.CommitRestore(&temp_asl);
	temp_asl.Clear();
}


void MONSTER_DATA::Clear(BOOL ctor)
{
  //key=0;
  Name="";
  preSpellNameKey = -1;
  Icon.filename="DEFAULT";
  Icon.picType=IconDib;
  Icon.SetDefaults();
  HitSound = "DEFAULT";
  MissSound = "DEFAULT";
  MoveSound="DEFAULT";
  DeathSound="DEFAULT";
  ClearSounds();
  Intelligence = 0;
  Armor_Class = 0;
  Movement = 0;
  Hit_Dice = 0.0F;
  UseHitDice=TRUE;
  Hit_Dice_Bonus = 0;
  THAC0 = 20;
  //Nbr_Attacks = 0;
  //Dmg_Dice_for_Attack = 0;
  //Dmg_Dice_Bonus = 0;
  //Nbr_Dmg_Dice = 0;
  Magic_Resistance = 0;
  Size = Medium;
  Morale = 0;
  XP_Value = 0;
 	specAbs.Clear();
  //ItemMask = AllClasses;
  Form_Type = 0;
  Penalty_Type = 0;
  Immunity_Type = 0;
  MiscOptions_Type = 0;
  mon_asl.Clear();
  myItems.Clear();
  money.Clear();
  //undeadStatus=NotUndead;
  undeadType.Empty();
  attackData.Clear();
  raceID = "Human";
}

MONSTER_DATA& MONSTER_DATA::operator =(const MONSTER_DATA& src)
{
  if (&src == this)
    return *this;
  //hHitSound = src.hHitSound;
  //hMissSound = src.hMissSound;
  //hMoveSound = src.hMoveSound;
  //hDeathSound = src.hDeathSound;
  //key = src.key;
  Name=src.Name;
#ifdef UAFEngine
  monsterID = src.monsterID;
#endif
  Icon=src.Icon;
  HitSound = src.HitSound;
  MissSound = src.MissSound;
  MoveSound = src.MoveSound;
  DeathSound = src.DeathSound;
  Intelligence = src.Intelligence;
  Armor_Class = src.Armor_Class;
  Movement = src.Movement;
  Hit_Dice = src.Hit_Dice;
  UseHitDice=src.UseHitDice;
  Hit_Dice_Bonus = src.Hit_Dice_Bonus;
  THAC0 = src.THAC0;
  //Nbr_Attacks = src.Nbr_Attacks;
  //Dmg_Dice_for_Attack = src.Dmg_Dice_for_Attack;
  //Dmg_Dice_Bonus = src.Dmg_Dice_Bonus;
  //Nbr_Dmg_Dice = src.Nbr_Dmg_Dice;
  Magic_Resistance = src.Magic_Resistance;
  Size = src.Size;
  classID = src.classID;
  raceID = src.raceID;
  Morale = src.Morale;
  XP_Value = src.XP_Value;
	specAbs.Copy(src.specAbs);
  //ItemMask = src.ItemMask;
  Form_Type = src.Form_Type;
  Penalty_Type = src.Penalty_Type;
  Immunity_Type = src.Immunity_Type;
  MiscOptions_Type = src.MiscOptions_Type;
  mon_asl.Copy(src.mon_asl);
  temp_asl.Copy(src.temp_asl);
  myItems=src.myItems;
  money=src.money;
  //undeadStatus=src.undeadStatus;
  undeadType = src.undeadType;
  attackData=src.attackData;
  preSpellNameKey = src.preSpellNameKey;
  return *this;
}
#ifdef UAFEDITOR
bool MONSTER_DATA::operator ==(const MONSTER_DATA& src) const
{
  //if (hHitSound           != src.hHitSound          ) return false;
  //if (hMissSound          != src.hMissSound         ) return false;
  //if (hMoveSound          != src.hMoveSound         ) return false;
  //if (hDeathSound         != src.hDeathSound        ) return false;
  //if (key                 != src.key                ) return false;
  if (Name                  != src.Name               ) return false;
#ifdef UAFEngine
  if (monsterID             != src.monsterID          ) return false;
#endif
  if (!(Icon              == src.Icon              )) return false;
  if (HitSound            != src.HitSound           ) return false;
  if (MissSound           != src.MissSound          ) return false;
  if (MoveSound           != src.MoveSound          ) return false;
  if (DeathSound          != src.DeathSound         ) return false;
  if (Intelligence        != src.Intelligence       ) return false;
  if (Armor_Class         != src.Armor_Class        ) return false;
  if (Movement            != src.Movement           ) return false;
  if (Hit_Dice            != src.Hit_Dice           ) return false;
  if (UseHitDice          != src.UseHitDice         ) return false;
  if (Hit_Dice_Bonus      != src.Hit_Dice_Bonus     ) return false;
  if (THAC0               != src.THAC0              ) return false;
  //if (Nbr_Attacks         != src.Nbr_Attacks        ) return false;
  //if (Dmg_Dice_for_Attack != src.Dmg_Dice_for_Attack) return false;
  //if (Dmg_Dice_Bonus      != src.Dmg_Dice_Bonus     ) return false;
  //if (Nbr_Dmg_Dice        != src.Nbr_Dmg_Dice       ) return false;
  if (Magic_Resistance    != src.Magic_Resistance   ) return false;
  if (Size                != src.Size               ) return false;
  if (classID             != src.classID            ) return false;
  if (raceID              != src.raceID            ) return false;
  if (Morale              != src.Morale             ) return false;
  if (XP_Value            != src.XP_Value           ) return false;
	if (!(specAbs           == src.specAbs)           ) return false;
  //if (ItemMask            != src.ItemMask           ) return false;
  if (Form_Type           != src.Form_Type          ) return false;
  if (Penalty_Type        != src.Penalty_Type       ) return false;
  if (Immunity_Type       != src.Immunity_Type      ) return false;
  if (MiscOptions_Type    != src.MiscOptions_Type   ) return false;
  if (!(mon_asl    == src.mon_asl)    ) return false;
  if (!(temp_asl   == src.temp_asl)   ) return false;
  if (!(myItems           == src.myItems)           ) return false;      
  if (!(const_cast<MONEY_SACK&>(money) == src.money)) return false;
  //if (undeadStatus        != src.undeadStatus       ) return false;
  if (undeadType          != src.undeadType         ) return false;
  if (!(attackData        == src.attackData)        ) return false;  
  return true;
}
#endif
#ifdef UAFEDITOR

void MONSTER_DATA::GuessUndeadStatus()
{
  //enum undeadType { NotUndead, Skeleton, Zombie, Ghoul, Shadow, Wight, Ghast, Wraith, Mummy,
	//					Spectre, Vampire, Ghost, Lich, Special };

  //if (undeadStatus != NotUndead) return; // already have undead status selected
  if (!undeadType.IsEmpty()) return; // already have undead status selected

  CString tmp(Name);
  tmp.MakeLower();
  if (tmp.Find("skeleton",0)>=0)
    //undeadStatus = Skeleton;
    undeadType = "Skeleton";
  else if (tmp.Find("zombie",0)>=0)
    //undeadStatus = Zombie;
    undeadType = "Zombie";
  else if (tmp.Find("ghoul",0)>=0)
    //undeadStatus = Ghoul;
    undeadType = "Ghoul";
  else if (tmp.Find("shadow",0)>=0)
    //undeadStatus = Shadow;
    undeadType = "Shadow";
  else if (tmp.Find("wight",0)>=0)
    //undeadStatus = Wight;
    undeadType = "wight";
  else if (tmp.Find("ghast",0)>=0)
    //undeadStatus = Ghast;
    undeadType = "Ghast";
  else if (tmp.Find("wraith",0)>=0)
    //undeadStatus = Wraith;
    undeadType = "Wraith";
  else if (tmp.Find("mummy",0)>=0)
    //undeadStatus = Mummy;
    undeadType = "Mummy";
  else if (tmp.Find("spectre",0)>=0)
    //undeadStatus = Spectre;
    undeadType = "Spectre";
  else if (tmp.Find("vampire",0)>=0)
    //undeadStatus = Vampire;
    undeadType = "Vampire";
  else if (tmp.Find("ghost",0)>=0)
    //undeadStatus = Ghost;
    undeadType = "Ghost";
  else if (tmp.Find("lich",0)>=0)
    //undeadStatus = Lich;
    undeadType = "Lich";
  else
    //undeadStatus = NotUndead;
    undeadType.Empty();
}

#endif

// this version will only be called if the monster data
// file is older and does not contain a version header.
//
void MONSTER_DATA::Serialize(CArchive &ar, double ver)
{
//  CObject::Serialize(ar);
  long temp;

  if (ar.IsStoring())
  {
    die("We should not serialize monsters with CArchive");
    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');
    //ar << key;
    AS(ar,Name);
#ifdef SIMPLE_STRUCTURE
    Icon.Serialize(ar, ver, rte.IconArtDir());
#else
    Icon.Serialize(ar, ver);
#endif
    AS(ar, HitSound);
    AS(ar, MissSound);
    AS(ar, MoveSound);
    AS(ar, DeathSound);
    ar << Intelligence;
    ar << Armor_Class;
    ar << Movement;
    ar << Hit_Dice;
    ar << UseHitDice;
    ar << Hit_Dice_Bonus;
    ar << THAC0;
    attackData.Serialize(ar, ver);
    ar << Magic_Resistance;
    temp = Size;
    ar << temp;
    ar << Morale;
    ar << XP_Value;
    //ar << ItemMask;
    ar << Form_Type;
    ar << Penalty_Type;
    ar << Immunity_Type;
    ar << MiscOptions_Type;
    //ar << (long)undeadStatus;    
    ar << undeadType;
  }
  else
  {
    //ar >> key;
    DAS(ar,Name);

#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Kobald|Stupid" will be
    // read as simply "Kobald".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif

#ifdef UAFEDITOR
    {
      die("Serialize very old monster data.  We should look for a key.");
    };
#endif
    if (ver < _VERSION_0640_)
    {
      CString tmp;
      DAS(ar,tmp);
      Icon.filename=tmp;
      Icon.picType = IconDib;
      Icon.SetDefaults();
    }
    else
    {
      Icon.Serialize(ar, ver, rte.IconArtDir());
      if (ver <= _VERSION_0840_)
        Icon.SetPre0840NonLoopDefaults();
      StripFilenamePath(Icon.filename);
    }
    DAS(ar,HitSound);
    DAS(ar,MissSound);
    if (ver >= _VERSION_0575_)
    {
      DAS(ar,MoveSound);
      DAS(ar,DeathSound);
    }
    else
    {
      MoveSound="DEFAULT";
      DeathSound="DEFAULT";
    }
    //AddFolderToPath(Icon.filename, rte.IconArtDir());
    //AddFolderToPath(HitSound,      rte.SoundDir());
    //AddFolderToPath(MissSound,     rte.SoundDir());
    //AddFolderToPath(MoveSound,     rte.SoundDir());
    //AddFolderToPath(DeathSound,    rte.SoundDir());

    ar >> Intelligence;
    ar >> Armor_Class;
    ar >> Movement;
    ar >> Hit_Dice;
    if (ver >= _VERSION_0906_)
      ar >> UseHitDice;
    ar >> Hit_Dice_Bonus;
    ar >> THAC0;
    
    if (ver < _VERSION_0750_)
    {
      long Nbr_Attacks;
      long Dmg_Dice_for_Attack;
      long Dmg_Dice_Bonus;
      long Nbr_Dmg_Dice;

      ar >> Nbr_Attacks;
      ar >> Dmg_Dice_for_Attack;
      ar >> Dmg_Dice_Bonus;
      ar >> Nbr_Dmg_Dice;

      if (Nbr_Attacks<=0) Nbr_Attacks=1;
      if (Dmg_Dice_for_Attack <= 0) Dmg_Dice_for_Attack = 10;
      if (Nbr_Dmg_Dice <= 0) Nbr_Dmg_Dice = 1;

      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = Dmg_Dice_for_Attack;
      tmp.nbr = Nbr_Dmg_Dice;
      tmp.bonus = Dmg_Dice_Bonus;
      tmp.attackMsg = "attacks";
      for (int cnt=0;cnt<Nbr_Attacks;cnt++)
        attackData.Add(tmp);
    }
    else
    {
      attackData.Serialize(ar, ver);
    }

    if (attackData.GetMonsterAttackDetailsCount()==0)
    {
      WriteDebugString("Forcing monster %s to have at least one attack\n", Name);
      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = 6;
      tmp.nbr = 1;
      tmp.bonus = 0;
      tmp.attackMsg = "attacks";
      attackData.Add(tmp);
    }

    ar >> Magic_Resistance;

    ar >> temp;
    Size = (creatureSizeType)temp;

    ar >> Morale;
    ar >> XP_Value;
    //ar >> ItemMask;
    ar >> Form_Type;
    ar >> Penalty_Type;
    ar >> Immunity_Type;
    ar >> MiscOptions_Type;
#ifdef UAFEDITOR
    if (ver >= _VERSION_0750_)
#endif
    {
      //ar >> temp;
      //undeadStatus = (undeadType)temp;
      ar >> undeadType;
    }
#ifdef UADEDITOR
    else
      GuessUndeadStatus();
#endif
  }

  specAbs.Serialize(ar, ver, this->Name, "monster");
  mon_asl.Serialize(ar, "MONSTER_DATA_ATTRIBUTES");
}


void MONSTER_DATA::Serialize(CAR &ar, double ver)
{
  long temp;

  if (ar.IsStoring())
  {
    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');
    //ar << key;
    ar << preSpellNameKey;
    AS(ar,Name);
    Icon.Serialize(ar, ver, rte.IconArtDir());
    AS(ar,HitSound);
    AS(ar,MissSound);
    AS(ar,MoveSound);
    AS(ar,DeathSound);
    ar << Intelligence;
    ar << Armor_Class;
    ar << Movement;
    ar << Hit_Dice;
    ar << UseHitDice;
    ar << Hit_Dice_Bonus;
    ar << THAC0;
    attackData.Serialize(ar, ver);
    ar << Magic_Resistance;
    temp = Size;
    ar << temp;
    ar << classID;
    StoreStringAsASL(mon_asl, raceID, "$SYS$Race", ASLF_EDITOR);
    ar << Morale;
    ar << XP_Value;
    //ar << ItemMask;
    ar << Form_Type;
    ar << Penalty_Type;
    ar << Immunity_Type;
    ar << MiscOptions_Type;
    //ar << undeadStatus;
    ar << undeadType;
  }
  else
  {
    //ar >> key;
    if ( (ver < VersionSpellNames) || (ver >= VersionSaveIDs) )
    {
      ar >> preSpellNameKey; // Keep track for later linking.
    }
    else 
    {
      preSpellNameKey = -1;
    };
    DAS(ar,Name);
#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Kobald|Stupid" will be
    // read as simply "Kobald".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    monsterID = Name;  // Only for internal use as a monsterID
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif

    if (ver < _VERSION_0640_)
    {
      CString tmp;
      DAS(ar,tmp);
      Icon.filename=tmp;
      Icon.picType = IconDib;
      Icon.SetDefaults();
    }
    else
    {
      Icon.Serialize(ar, ver, rte.IconArtDir());
      if (ver <= _VERSION_0840_)
        Icon.SetPre0840NonLoopDefaults();
      StripFilenamePath(Icon.filename);
    };
    DAS(ar,HitSound);
    DAS(ar,MissSound);
    if (ver >= _VERSION_0575_)
    {
      DAS(ar,MoveSound);
      DAS(ar,DeathSound);
    }
    else
    {
      MoveSound="DEFAULT";
      DeathSound="DEFAULT";
    };
    if (ver < VersionSpellNames)
    {
      StripFilenamePath(HitSound);
      StripFilenamePath(MissSound);
      StripFilenamePath(MoveSound);
      StripFilenamePath(DeathSound);
    };
    ar >> Intelligence;
    ar >> Armor_Class;
    ar >> Movement;
    ar >> Hit_Dice;
    if (ver >= _VERSION_0906_)
      ar >> UseHitDice;
    ar >> Hit_Dice_Bonus;
    ar >> THAC0;

    if (ver < _VERSION_0750_)
    {
      long Nbr_Attacks;
      long Dmg_Dice_for_Attack;
      long Dmg_Dice_Bonus;
      long Nbr_Dmg_Dice;

      ar >> Nbr_Attacks;
      ar >> Dmg_Dice_for_Attack;
      ar >> Dmg_Dice_Bonus;
      ar >> Nbr_Dmg_Dice;

      if (Nbr_Attacks<=0) Nbr_Attacks=1;
      if (Dmg_Dice_for_Attack <= 0) Dmg_Dice_for_Attack = 10;
      if (Nbr_Dmg_Dice <= 0) Nbr_Dmg_Dice = 1;

      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = Dmg_Dice_for_Attack;
      tmp.nbr = Nbr_Dmg_Dice;
      tmp.bonus = Dmg_Dice_Bonus;
      tmp.attackMsg = "attacks";
      for (int cnt=0;cnt<Nbr_Attacks;cnt++)
        attackData.Add(tmp);
    }
    else
    {
      attackData.Serialize(ar, ver);
    }

    if (attackData.GetMonsterAttackDetailsCount()==0)
    {
      WriteDebugString("Forcing monster %s to have at least one attack\n", Name);
      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = 6;
      tmp.nbr = 1;
      tmp.bonus = 0;
      tmp.attackMsg = "attacks";
      attackData.Add(tmp);
    }

    ar >> Magic_Resistance;

    ar >> temp;
    Size = (creatureSizeType)temp;
#ifdef UAFEDITOR
    if (ver < VersionSpellNames)
    {
      classID = "Fighter";
    }
    else
#endif
    {
      ar >> classID;
    };
    ar >> Morale;
    ar >> XP_Value;
#ifdef UAFEDITOR
    if (ver < VersionSpellNames)
    {
      BYTE ItemMask;
      ar >> ItemMask;
    };
#endif

    //ar >> ItemMask;
    ar >> Form_Type;
    ar >> Penalty_Type;
    ar >> Immunity_Type;
    ar >> MiscOptions_Type;
#ifdef UAFEDITOR
    if (ver >= _VERSION_0750_)
#endif
    {
#ifdef UAFEDITOR
      if (ver <= 0.998115)
      {
        ar >> temp;
        undeadType.Empty();
        if ((temp >0) && (temp<14))
        {
          undeadType = UndeadTypeText[temp];
        };
      }
      else
#endif
      {
        ar >> undeadType;
      };

    }
#ifdef UAFEDITOR
    else
      GuessUndeadStatus();
#endif
  }

  specAbs.Serialize(ar, ver, this->Name, "monster");
  mon_asl.Serialize(ar, "MONSTER_DATA_ATTRIBUTES");

  if (ar.IsStoring())
  {
    myItems.Serialize(ar,ver);
    money.Serialize(ar, ver);
  }
  else
  {
    raceID = "Human";
    {
      CString tmp;
      RetrieveStringFromASL(mon_asl, tmp, "$SYS$Race");
      if (!tmp.IsEmpty())
      {
        raceID = tmp;
      };
    };
    if (ver > _VERSION_0693_)
      myItems.Serialize(ar, ver);
    if (ver >= _VERSION_0906_)
      money.Serialize(ar, ver);
  }
}

//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::operator =
//
// PURPOSE: Make a copy of a MONSTER_DATA list
//*****************************************************************************
MONSTER_DATA_TYPE& MONSTER_DATA_TYPE:: operator = (MONSTER_DATA_TYPE& src)
{
  if (&src==this) return *this;
  MonsterData.RemoveAll();
  //POSITION p=src.MonsterData.GetHeadPosition();
  //while (p!=NULL)
  //{
  //  AddMonsterWithCurrKey(src.MonsterData.GetNext(p));
  //};
  {
    int i, n;
    n = src.GetCount();
    for (i=0; i<n; i++)
    {
      AddMonster(src.GetMonster(i));
      //AddMonster(&src.MonsterData[i]);
    };
  };
  //pos=src.pos;
  return *this;
}

#ifdef UAFEDITOR
//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::operator ==
//
// PURPOSE: Test that two MONSTER_DATA lists are identical...even the ordering.
//          One might like to exclude order but then it is a bit tricky
//          because of the possiblility of two identical entries in a list.
//*****************************************************************************
bool MONSTER_DATA_TYPE:: operator == (MONSTER_DATA_TYPE& src) const
{
  //POSITION p1=MonsterData.GetHeadPosition();
  //POSITION p2=src.MonsterData.GetHeadPosition();
  //while (p1!=NULL)
  //{
  //  if (p2==NULL) return false;
  //  if (!(MonsterData.GetNext(p1)==src.MonsterData.GetNext(p2))) return false;
  //};
  //if (p2!=NULL) return false;
//  if (pos!=src.pos) return false;
  int i, n;
  n = GetCount();
  if (n != src.GetCount()) return false;
  for (i=0; i<n; i++)
  {
    if (*PeekMonster(i) == *src.PeekMonster(i)) continue;
    return false;
  };
  return true;
}
#endif
#ifdef UAFEDITOR
int MONSTER_DATA_TYPE::LocatePreSpellNamesKey(int key)
{
  int i, n;
  static int messageCount = 0;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    if (PeekMonster(i)->preSpellNameKey == key) return i;
  };
  if (messageCount == 0)
  {
    MsgBoxInfo("Searching for a Monster that was defined by its 'key'.  We lost that information "
               "at about version 0.998101 of the editor.  If this is a problem for you, please "
               "contact the DC development team for help.");
  }
  else
  {
    if (messageCount == 1)
    {
      MsgBoxInfo("Messages for additional monster key problems have been supressed. See log.");
    };
  };
  messageCount++;
  return -1;
}
#endif


int MONSTER_DATA_TYPE::LocateMonster(const MONSTER_ID& monsterID) const
{
  int i,n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
#ifdef UAFEDITOR
    if (PeekMonster(i)->Name == monsterID) return i;
#else
    if (PeekMonster(i)->monsterID == monsterID) return i;
#endif
  };
  return -1;
}

int MONSTER_DATA_TYPE::GetDataSize(void) const
{
  //POSITION pos;
  int size;
  //MONSTER_DATA *pEntry;
  //pos = GetHeadPosition();
  size = sizeof (*this);
  //while (pos != NULL)
  //{
  //  pEntry = &GetNext(pos);
  //  size += pEntry->GetDataSize();
  //};
  {
    int i,n;
    n = GetCount();
    for (i=0; i<n; i++)
    {
      size += PeekMonster(i)->GetDataSize();
      //size += MonsterData[i].GetDataSize();
    };
  };
  return size;
}

//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::Serialize
//
// PURPOSE:  
//*****************************************************************************
void MONSTER_DATA_TYPE::Serialize(CArchive &ar)
{
  //20121212 PRS CObject::Serialize(ar);
  int i, n;
  //POSITION pos;
  MONSTER_DATA data;
  if (ar.IsStoring())
  {
    ar << GetCount();
    //pos = MonsterData.GetHeadPosition();
    //while (pos != NULL)
    //  MonsterData.GetNext(pos).Serialize(ar, MONSTER_DATA_TYPE::version);
    n = GetCount();
    for (i=0; i<n; i++)
    {
      GetMonster(i)->Serialize(ar, MONSTER_DATA_TYPE::version);
      //MonsterData[i].Serialize(ar, MONSTER_DATA_TYPE::version);
    };
  }
  else
  {
    //Clear();
    //version = _VERSION_0662_;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar, MONSTER_DATA_TYPE::version);
      //AddMonsterWithCurrKey(data);
      AddMonster(&data);
    }
  }
}

void MONSTER_DATA_TYPE::Serialize(CAR &ar, double ver)
{
  int i, n;
  MONSTER_DATA data;
  //POSITION pos;
  if (ar.IsStoring())
  {
    ar << GetCount();
    //pos = MonsterData.GetHeadPosition();
    //while (pos != NULL)
    //{
    //  //if (ver <= _VERSION_0693_)
    //  //  MonsterData.GetNext(pos).Serialize(ar);
    //  //else
    //    MonsterData.GetNext(pos).Serialize(ar, ver);
    //}
    n = GetCount();
    for (i=0; i<n; i++)
    {
      GetMonster(i)->Serialize(ar, ver);
      //MonsterData[i].Serialize(ar, ver);
    };
  }
  else
  {
    //Clear();
    //version=ver;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      //if (ver <= _VERSION_0693_)
      //  data.Serialize(ar);
      //else
      data.Serialize(ar, ver);
      //AddMonsterWithCurrKey(data);
      AddMonster(&data);
    }
  }
}


void MONSTER_DATA_TYPE::Save(CAR& car) const
{
	//POSITION p;
	int i, count;
	//count=MonsterData.GetCount();
	count=GetCount();
	car << count;
	//p=MonsterData.GetHeadPosition();
	//while (p!=NULL) {
	//	ar << MonsterData.PeekAtPos(p).Name;
	//	MonsterData.PeekAtPos(p).Save(ar);
	//	MonsterData.PeekNext(p);
	//};
  for (i=0; i<count; i++)
  {
		car << PeekMonster(i)->Name;
		//car << MonsterData[i].Name;
		PeekMonster(i)->Save(car);
		//MonsterData[i].Save(car);
  };
}

void MONSTER_DATA_TYPE::Restore(CArchive& ar) 
{
	//POSITION p;
	int i, n, count;
	CString name;
	if (version < _ASL_LEVEL_) return;
	//p=MonsterData.GetHeadPosition();
	// Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ASL in case no restore operation clears it.
	//while (p!=NULL) MonsterData.GetNext(p).PrepareForRestore();
  n = GetCount();
	for (i=0; i<n; i++)
  {
    GetMonster(i)->PrepareForRestore();
    //MonsterData[i].PrepareForRestore();
  };
	ar >> count;
	for (i=0; i<count; i++) 
	{
		ar >> name;
		//p=MonsterData.GetHeadPosition();
		//while (p!=NULL)
    for (i=0; i<n; i++)
		{
			//if (name==MonsterData.PeekAtPos(p).Name) 
			if (name==PeekMonster(i)->Name) 
			//if (name==MonsterData[i].Name) 
			{
				//MonsterData.GetAtPos(p).Restore(ar);
				GetMonster(i)->Restore(ar);
				//MonsterData[i].Restore(ar);
				break;
			};
			//MonsterData.GetNext(p);
		};
		//if (p==NULL) 
		if (i==n) 
		{
			CString msg;
			MONSTER_DATA junk;
			msg.Format("SAVE_FILE MONSTER_DATA \"%s\" for monster not in design\n",
				name);
			WriteDebugString(msg);
			junk.Restore(ar); // discard data
		};
	};
}

void MONSTER_DATA_TYPE::Restore(CAR& car) 
{
	//POSITION p;
	int i, j, n, count;
	CString name;
	//p=MonsterData.GetHeadPosition();
	// Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ASL in case no restore operation clears it.
	//while (p!=NULL) MonsterData.GetNext(p).PrepareForRestore();
  n = GetCount();
	for (i=0; i<n; i++) 
  {
    GetMonster(i)->PrepareForRestore();
    //MonsterData[i].PrepareForRestore();
  };
	car >> count;
	for (i=0; i<count; i++) 
	{
    //if (i == 0xa2)
    //{
    //  int kkk=1;
    //};
		car >> name;
		//p=MonsterData.GetHeadPosition();
		//while (p!=NULL)
    for (j=0; j<n; j++)
		{
			//if (name==MonsterData.PeekAtPos(p).Name) 
			if (name==PeekMonster(j)->Name) 
			//if (name==MonsterData[i].Name) 
			{
				//MonsterData.GetAtPos(p).Restore(ar);
				GetMonster(j)->Restore(car);
				//MonsterData[i].Restore(car);
				break;
			};
			//MonsterData.GetNext(p);
		};
		//if (p==NULL) 
		if (j==n) 
		{
			CString msg;
			MONSTER_DATA junk;
			msg.Format("SAVE_FILE MONSTER_DATA \"%s\" for monster not in design\n",
				name);
			WriteDebugString(msg);
			junk.Restore(car); // discard data
		};
	};
}

void MONSTER_DATA_TYPE::CommitRestore(void) {
	//POSITION p;
	//p=MonsterData.GetHeadPosition();
	//while (p!=NULL) MonsterData.GetNext(p).CommitRestore();
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    GetMonster(i)->CommitRestore();
    //MonsterData[i].CommitRestore();
  };
}




//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::SaveSounds
//
// PURPOSE:  
//*****************************************************************************
void saveSounds(MONSTER_DATA_TYPE& data)
{
  data.SaveSounds();
}

void MONSTER_DATA_TYPE::SaveSounds()
{
  //POSITION p = MonsterData.GetHeadPosition();
  //while (p != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //SaveSound(MonsterData.GetAtPos(p).HitSound,   GLOBAL_SOUND, rte.SoundDir());
    //SaveSound(MonsterData.GetAtPos(p).MissSound,  GLOBAL_SOUND, rte.SoundDir());
    //SaveSound(MonsterData.GetAtPos(p).MoveSound,  GLOBAL_SOUND, rte.SoundDir());
    //SaveSound(MonsterData.GetAtPos(p).DeathSound, GLOBAL_SOUND, rte.SoundDir());
    //MonsterData.GetNext(p);
    //SaveSound(MonsterData[i].HitSound,   GLOBAL_SOUND, rte.SoundDir());
    //SaveSound(MonsterData[i].MissSound,  GLOBAL_SOUND, rte.SoundDir());
    //SaveSound(MonsterData[i].MoveSound,  GLOBAL_SOUND, rte.SoundDir());
    //SaveSound(MonsterData[i].DeathSound, GLOBAL_SOUND, rte.SoundDir());
    SaveSound(PeekMonster(i)->HitSound,   GLOBAL_SOUND, rte.SoundDir());
    SaveSound(PeekMonster(i)->MissSound,  GLOBAL_SOUND, rte.SoundDir());
    SaveSound(PeekMonster(i)->MoveSound,  GLOBAL_SOUND, rte.SoundDir());
    SaveSound(PeekMonster(i)->DeathSound, GLOBAL_SOUND, rte.SoundDir());
  }
}


//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::SaveArt
//
// PURPOSE:  
//*****************************************************************************
void saveArt(MONSTER_DATA_TYPE& data)
{
  data.SaveArt();
}

void MONSTER_DATA_TYPE::SaveArt() 
{
  //POSITION p = MonsterData.GetHeadPosition();
  //while (p != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //MonsterData.GetAtPos(p).Icon.SavePicData(GLOBAL_ART);
    //MonsterData.PeekNext(p);
    GetMonster(i)->Icon.SavePicData(GLOBAL_ART);
    //MonsterData[i].Icon.SavePicData(GLOBAL_ART);
  }
}

void clearData(MONSTER_DATA_TYPE& data)
{
  data.Clear();
  data.version = PRODUCT_VER;
}

BOOL saveData(MONSTER_DATA_TYPE& data)
{
  EditorStatusMsg("Saving monster data...");

#ifdef SIMPLE_STRUCTURE
  CString fullPath;
  fullPath = rte.DataDir() + MONSTER_DB_NAME;
#else
  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, MONSTER_DB_NAME);
#endif

  return saveData(data, fullPath);
}

BOOL saveData(MONSTER_DATA_TYPE& data, LPCSTR fullPath)
{
  BOOL success = TRUE;

  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open monster db file %s, error %i\n", fullPath, e.m_cause);
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
  //CArchive ar(&myFile, CArchive::store);

  try
  {
    data.PreSerialize(ar.IsStoring());
    data.Serialize(ar, ver);    
    data.PostSerialize(ar.IsStoring());
  }
  catch (...)
  {
    success = FALSE;
    WriteDebugString("Failed to read monster dat file\n");
  }

  ar.Close();
  return success;
}

int loadData(MONSTER_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success=TRUE;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open monster db file %s, error %i\n", fullPath, e.m_cause);
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
    ver = min(globalData.version, _VERSION_0662_);
    //ver = _VERSION_0662_;
  }

  data.version = ver;

  // if loading older level data, don't use compression archive
  if (ver <= _VERSION_0670_)
  {
    WriteDebugString("Loading pre-0670 version monster DB - %4.7f\n", ver);

    CArchive ar(&myFile, CArchive::load);
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar);
       data.PostSerialize(ar.IsStoring());
    }
    catch (...)
    {
       WriteDebugString("Unable to load monster data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }
  else
  {
    WriteDebugString("Loading monster DB version: %4.7f\n", ver);
    CAR ar(&myFile, CArchive::load);
    if (ver >= _SPECIAL_ABILITIES_VERSION_)
    {
      ar.Compress(true); // qqqqq
    };
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar, data.version);
       data.PostSerialize(ar.IsStoring());
    }
    catch (...)
    {
       WriteDebugString("Unable to load monster data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }

  if (data.GetCount() <= 0)
     WriteDebugString("No monsters in monster db file\n");
  
  return success; //data.GetCount();
}

void MONSTER_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  // before saving, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (IsStoring) ExpandDefaultFilenames();

  //POSITION p = GetHeadPosition();
  //while (p!=NULL) 
  //  GetNext(p).PreSerialize(IsStoring);
  {
    int i, n;
    n = GetCount();
    for (i=0; i<n; i++)
    {
      GetMonster(i)->PreSerialize(IsStoring);
      //MonsterData[i].PreSerialize(IsStoring);
    };
  };
}

void MONSTER_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
  // after loading, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (!IsStoring) ExpandDefaultFilenames();

  //POSITION p = GetHeadPosition();
  //while (p!=NULL) 
  //  GetNext(p).PostSerialize(IsStoring);
  {
    int i,n;
    n = GetCount();
    for (i=0; i<n; i++)
    {
      //MonsterData[i].PostSerialize(IsStoring);
      GetMonster(i)->PostSerialize(IsStoring);
    };
  };
}

void MONSTER_DATA::PreSerialize(BOOL IsStoring) 
{
  if (IsStoring)
  {    
    //StripFilenamePath(Icon.filename);
    //StripFilenamePath(HitSound);
    //StripFilenamePath(MissSound);
    //StripFilenamePath(MoveSound);
    //StripFilenamePath(DeathSound);
  }
}

void MONSTER_DATA::PostSerialize(BOOL IsStoring) 
{
  //AddFolderToPath(Icon.filename, rte.IconArtDir());
  //AddFolderToPath(HitSound,      rte.SoundDir());
  //AddFolderToPath(MissSound,     rte.SoundDir());
  //AddFolderToPath(MoveSound,     rte.SoundDir());
  //AddFolderToPath(DeathSound,    rte.SoundDir());
}

#ifdef UAFEDITOR


void MONSTER_DATA::CrossReference(CR_LIST *pCRList) const
{
  CR_REFERENCE crReference;
  crReference.m_referenceName = Name;
  crReference.m_referenceType = CR_TYPE_monster;
  pCRList->CR_AddSoundReference(HitSound, &crReference);
  pCRList->CR_AddSoundReference(MissSound, &crReference);
  pCRList->CR_AddSoundReference(MoveSound, &crReference);
  pCRList->CR_AddSoundReference(DeathSound, &crReference);
  Icon.CrossReference(pCRList, &crReference);
  myItems.CrossReference(pCRList, &crReference);
  attackData.CrossReference(pCRList, &crReference);
}

#endif

void MONSTER_DATA_TYPE::ExpandDefaultFilenames()
{  
  //POSITION p = GetHeadPosition();
  int i, n;
  //int key;
  n = GetCount();
  //while (p != NULL)
  for (i=0; i<n; i++)
  {
    //MONSTER_DATA data = PeekAtPos(p);
    MONSTER_DATA *pMonster;
    pMonster = GetMonster(i); //&MonsterData[i];
/*
    if (data.Icon.filename.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      data.Icon.filename = ede.TemplateIconArtDir() +  DEFAULT_MONSTER_ICON;
#else
      data.Icon.filename = DEFAULT_MONSTER_ICON;
#endif
      data.Icon.picType=IconDib;
      data.Icon.SetDefaults();
    }

    if (data.HitSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.HitSound = ede.TemplateSoundDir() + DEF_HIT_SOUND;
#else
      data.HitSound = DEF_HIT_SOUND;
#endif
    }
    if (data.MissSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.MissSound = ede.TemplateSoundDir() + DEF_MISS_SOUND;
#else
      data.MissSound = DEF_MISS_SOUND;
#endif
    }
    if (data.MoveSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.MoveSound = ede.TemplateSoundDir() + DEF_CHAR_MOVE_SOUND;
#else
      data.MoveSound = DEF_CHAR_MOVE_SOUND;
#endif
    }
    if (data.DeathSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.DeathSound = ede.TemplateSoundDir() + DEF_CHAR_DEATH_SOUND;
#else
      data.DeathSound = DEF_CHAR_DEATH_SOUND;
#endif
    }
    key = GetKeyAt(p);
    SetMonster(key, data);

    GetNext(p);
*/
    if (pMonster->Icon.filename.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      pMonster->Icon.filename = ede.TemplateIconArtDir() +  DEFAULT_MONSTER_ICON;
#else
      pMonster->Icon.filename = DEFAULT_MONSTER_ICON;
#endif
      pMonster->Icon.picType=IconDib;
      pMonster->Icon.SetDefaults();
    }

    if (pMonster->HitSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      pMonster->HitSound = ede.TemplateSoundDir() + DEF_HIT_SOUND;
#else
      pMonster->HitSound = DEF_HIT_SOUND;
#endif
    }
    if (pMonster->MissSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      pMonster->MissSound = ede.TemplateSoundDir() + DEF_MISS_SOUND;
#else
      pMonster->MissSound = DEF_MISS_SOUND;
#endif
    }
    if (pMonster->MoveSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      pMonster->MoveSound = ede.TemplateSoundDir() + DEF_CHAR_MOVE_SOUND;
#else
      pMonster->MoveSound = DEF_CHAR_MOVE_SOUND;
#endif
    }
    if (pMonster->DeathSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      pMonster->DeathSound = ede.TemplateSoundDir() + DEF_CHAR_DEATH_SOUND;
#else
      pMonster->DeathSound = DEF_CHAR_DEATH_SOUND;
#endif
    }
  }
}

#ifdef UAFEDITOR
void MONSTER_DATA_TYPE::CollapseDefaultFilenames()
{  
  //int key;
  //POSITION p = GetHeadPosition();
  CString DefFilename;

  //while (p != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //MONSTER_DATA& data = GetAtPos(p);
    MONSTER_DATA& data = *GetMonster(i);
#ifdef SIMPLE_STRUCTURE
    DefFilename = ede.TemplateIconArtDir() + DEFAULT_MONSTER_ICON;
    if (data.Icon.filename.CompareNoCase(DefFilename)==0)
    {
      data.Icon.filename = "DEFAULT";
      data.Icon.picType=IconDib;
      data.Icon.SetDefaults();
    }

    DefFilename  = ede.TemplateSoundDir() + DEF_HIT_SOUND;
    if (data.HitSound.CompareNoCase(DefFilename)==0)
      data.HitSound = "DEFAULT";

    DefFilename  = ede.TemplateSoundDir() + DEF_MISS_SOUND;
    if (data.MissSound.CompareNoCase(DefFilename)==0)
      data.MissSound = "DEFAULT";

    DefFilename  = ede.TemplateSoundDir() + DEF_CHAR_MOVE_SOUND;
    if (data.MoveSound.CompareNoCase(DefFilename)==0)
      data.MoveSound = "DEFAULT";

    DefFilename  = ede.TemplateSoundDir() + DEF_CHAR_DEATH_SOUND;
    if (data.DeathSound.CompareNoCase(DefFilename)==0)
      data.DeathSound = "DEFAULT";
#else
    DefFilename.Format("%s%s",EditorArt, DEFAULT_MONSTER_ICON);
    if (data.Icon.filename.CompareNoCase(DefFilename)==0)
    {
      data.Icon.filename = "DEFAULT";
      data.Icon.picType=IconDib;
      data.Icon.SetDefaults();
    }

    DefFilename.Format("%s%s",EditorArt, DEF_HIT_SOUND);
    if (data.HitSound.CompareNoCase(DefFilename)==0)
      data.HitSound = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEF_MISS_SOUND);
    if (data.MissSound.CompareNoCase(DefFilename)==0)
      data.MissSound = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEF_CHAR_MOVE_SOUND);
    if (data.MoveSound.CompareNoCase(DefFilename)==0)
      data.MoveSound = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEF_CHAR_DEATH_SOUND);
    if (data.DeathSound.CompareNoCase(DefFilename)==0)
      data.DeathSound = "DEFAULT";
#endif
    //key = GetKeyAt(p);
    //SetMonster(key, data);
    //GetNext(p);
  }
}

#endif // UAFEDITOR


void MONSTER_DATA::PlayHit()
{
  if (!PlaySoundEffects) return;
  //if (!ValidSound(hHitSound))
  //  hHitSound = LoadSound(HitSound);
  //if (!PlaySound(hHitSound,FALSE))
  //  hHitSound=-1;
  HitSound.Play();
}
void MONSTER_DATA::PlayMiss()
{
  if (!PlaySoundEffects) return;
  //if (!ValidSound(hMissSound))
  //  hMissSound = LoadSound(MissSound);
  //if (!PlaySound(hMissSound,FALSE))
  //  hMissSound=-1;
  MissSound.Play();
}
void MONSTER_DATA::PlayMove()
{
  if (!PlaySoundEffects) return;
  //if (!ValidSound(hMoveSound))
  //  hMoveSound = LoadSound(MoveSound);

  //if (!ValidSound(hMoveSound))
  //{
  //  // default didn't work, try the char move sound
  //  MoveSound = DEF_CHAR_MOVE_SOUND;
  //  hMoveSound = LoadSound(MoveSound);
  //}
  //if (!ValidSound(hMoveSound))
  //{
  //  // that didn't work either, try the party step sound
  //  MoveSound = globalData.sounds.PartyStep;
  //  hMoveSound = LoadSound(MoveSound);
  //}

  //if (!PlaySound(hMoveSound,FALSE))
  //  hMoveSound=-1;
  if (!MoveSound.Play())
  {
    MoveSound = DEF_CHAR_MOVE_SOUND;
    if (!MoveSound.Play())
    {
      MoveSound = globalData.sounds.PartyStep;
      if (!MoveSound.Play())
      {
        MoveSound.TotalDisable();
      };
    };
  };
}

void MONSTER_DATA::PlayDeath()
{
  if (!PlaySoundEffects) return;
  //if (!ValidSound(hDeathSound))
  //{
  //  //WriteDebugString("107 Calling LoadSound(%s)\n", DeathSound);
  //  hDeathSound = LoadSound(DeathSound);
  //};

  //if (!ValidSound(hDeathSound))
  //{
  //  // default didn't work, try the char death sound
  //  DeathSound = DEF_CHAR_DEATH_SOUND;
  //  //WriteDebugString("108 Calling LoadSound(%s)\n", DeathSound);
  //  hDeathSound = LoadSound(DeathSound);
  //}
  //if (!PlaySound(hDeathSound,FALSE))
  //  hDeathSound=-1;
  if (!DeathSound.Play())
  {
    DeathSound = DEF_CHAR_DEATH_SOUND;
    if (!DeathSound.Play())
    {
      DeathSound.TotalDisable();
    };
  };
}

void MONSTER_DATA::ClearSounds()
{
  //if (hHitSound >= 0)
  //  ClearSound(hHitSound);
  //if (hMissSound >= 0)
  //  ClearSound(hMissSound);
  //if (hMoveSound >= 0)
  //  ClearSound(hMoveSound);
  //if (hDeathSound >= 0)
  //  ClearSound(hDeathSound);

  //hHitSound = -1;
  //hMissSound = -1;
  //hMoveSound = -1;
  //hDeathSound = -1;
  HitSound.Clear();
  MissSound.Clear();
  MoveSound.Clear();
  DeathSound.Clear();
}
/*
int MONSTER_DATA_TYPE::GetNextKey()
{
  int key=0;
  POSITION pos;
  if (GetCount() > 0)
  {
    key = MonsterData.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_MONSTERS.
      // Gaps occur when deletions are made to the monster list
      pos = MonsterData.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = MonsterData.GetNext(pos).key;
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
//BOOL MONSTER_DATA_TYPE::Validate(MONSTER_DATA data, FILE *pFile) const
BOOL MONSTER_DATA_TYPE::Validate(const MONSTER_DATA *pMonster, FILE *pFile) const
{
  BOOL success = TRUE;
  // hit and miss sounds start with defaults, so if they are
  // now blank it is because of a designer decision
  if (pMonster->HitSound != "")
  {
    //if (!FileExists(data.HitSound))
    if (!FileExists(pMonster->HitSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Hit Sound file '%s'\n",
                       //data.Name, data.HitSound);
                       pMonster->Name, pMonster->HitSound);
      success = FALSE;
    }
  }
  //if (data.MissSound != "")
  if (pMonster->MissSound != "")
  {
    //if (!FileExists(data.MissSound))
    if (!FileExists(pMonster->MissSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Miss Sound file '%s'\n",
                       //data.Name, data.MissSound);
                       pMonster->Name, pMonster->MissSound);
      success = FALSE;
    }
  }
  //if (data.MoveSound != "")
  if (pMonster->MoveSound != "")
  {
    //if (!FileExists(data.MoveSound))
    if (!FileExists(pMonster->MoveSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Move Sound file '%s'\n",
                       //data.Name, data.MoveSound);
                       pMonster->Name, pMonster->MoveSound);
      success = FALSE;
    }
  }
  //if (data.DeathSound != "")
  if (pMonster->DeathSound != "")
  {
    //if (!FileExists(data.DeathSound))
    if (!FileExists(pMonster->DeathSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Death Sound file '%s'\n",
                       //data.Name, data.DeathSound);
                       pMonster->Name, pMonster->DeathSound);
      success = FALSE;
    }
  }
  // icons are required, else a default will be chosen by the
  // engine anyway.
  //if (!FileExists(data.Icon.filename))
  if (!FileExists(pMonster->Icon.filename))
  {
    WriteValidateLog(pFile, "Monster '%s' missing Icon file '%s'\n",
                     //data.Name, data.Icon.filename);
                     pMonster->Name, pMonster->Icon.filename);
    success = FALSE;
  }

  //POSITION pos = data.specAbs.GetHeadPosition();
  POSITION pos = pMonster->specAbs.GetHeadPosition();
  while (pos!=NULL)
  {
    const CStringPAIR *spab;
    //spab = data.specAbs.GetNextData(pos);
    spab = pMonster->specAbs.GetNextData(pos);
    if (spab!=NULL)
    {
      if (!specialAbilitiesData.FindString(spab->Key(),ActivationScriptName).IsEmpty())
      {
        if (specialAbilitiesData.FindString(spab->Key(),"ActivationScriptBin").GetLength() == 0)
        {
          success = FALSE;
          WriteValidateLog(pFile, "Monster '%s' special ability script '%s' did not compile\n",
                           //data.Name, specialAbilitiesData.FindString(spab->Key(),ActivationScriptName));
                           pMonster->Name, specialAbilitiesData.FindString(spab->Key(),ActivationScriptName));
        }
      }
    }
  }
  return success;
}

BOOL MONSTER_DATA_TYPE::Validate(FILE *pFile)
{
  BOOL success = TRUE;
  //POSITION pos = MonsterData.GetHeadPosition();
  //while (pos != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    // hit and miss sounds start with defaults, so if they are
    // now blank it is because of a designer decision
    //if (!Validate(MonsterData.GetNext(pos),pFile))
    if (!Validate(PeekMonster(i),pFile))
      success = FALSE;
  }
  return success;
}

//BOOL MONSTER_DATA_TYPE::Validate(int index, FILE *pFile) const
BOOL MONSTER_DATA_TYPE::Validate(const MONSTER_ID& monsterID, FILE *pFile) const
{
  BOOL success = TRUE;
  //POSITION pos;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  const MONSTER_DATA *pMonster;
  pMonster = monsterData.PeekMonster(monsterID);
  if (pMonster != NULL)
  {
    // hit and miss sounds start with defaults, so if they are
    // now blank it is because of a designer decision
    //if (!Validate(MonsterData.PeekAtPos(pos),pFile))
    if (!Validate(pMonster,pFile))
      success = FALSE;
  }
  else
  {
    //WriteValidateLog(pFile, "Bogus monster key %i\n", index);
    WriteValidateLog(pFile, "Bogus monster key %s\n", monsterID.UniqueName());
    success = FALSE;
  }
  return success;
}

void MONSTER_DATA_TYPE::CrossReference(CR_LIST *pCRList) const
{
  //POSITION pos = MonsterData.GetHeadPosition();
  //while (pos != NULL)
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    //MonsterData.GetNext(pos).CrossReference(pCRList);
    PeekMonster(i)->CrossReference(pCRList);
  };
}

#endif
//void MONSTER_DATA_TYPE::PlayHit(int index)
void MONSTER_DATA_TYPE::PlayHit(const MONSTER_ID& monsterID)
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  //  MonsterData.GetAtPos(pos).PlayHit();
  MONSTER_DATA *pMonster;
  pMonster = GetMonster(monsterID);
  if (pMonster != NULL) pMonster->PlayHit();
}
void MONSTER_DATA_TYPE::PlayMiss(const MONSTER_ID& monsterID)
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  //  MonsterData.GetAtPos(pos).PlayMiss();
  MONSTER_DATA *pMonster;
  pMonster = GetMonster(monsterID);
  if (pMonster != NULL) pMonster->PlayMiss();
}
void MONSTER_DATA_TYPE::PlayMove(const MONSTER_ID& monsterID)
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  //  MonsterData.GetAtPos(pos).PlayMove();
  MONSTER_DATA *pMonster;
  pMonster = GetMonster(monsterID);
  if (pMonster != NULL) pMonster->PlayMove();
}
void MONSTER_DATA_TYPE::PlayDeath(const MONSTER_ID& monsterID)
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  //  MonsterData.GetAtPos(pos).PlayDeath();
  MONSTER_DATA *pMonster;
  pMonster = GetMonster(monsterID);
  if (pMonster != NULL) pMonster->PlayDeath();
}
void MONSTER_DATA_TYPE::ClearSounds()
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  //pos = MonsterData.GetHeadPosition();
  //while (pos != NULL)
  //  MonsterData.GetNext(pos).ClearSounds();
  int i, n;
  n = GetCount();
  for (i=0; i<n; i++)
  {
    GetMonster(i)->ClearSounds();
    //MonsterData[i].ClearSounds();
  };
}  
/*
CString MONSTER_DATA_TYPE::GetMonsterName(int index) const
{
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.PeekAtPos(pos).Name;
  else
    return CString("No Monster");
}
*/
//BOOL MONSTER_DATA_TYPE::HaveMonster(int index)
BOOL MONSTER_DATA_TYPE::HaveMonster(const MONSTER_ID& monsterID) const
{
  //CSingleLock sLock(&m_CS, TRUE);
  //return (MonsterData.FindKeyPos(index) != NULL);
  return LocateMonster(monsterID) >= 0;
}

// called when loading from scripts, or
// when adding from editor GUI
MONSTER_ID MONSTER_DATA_TYPE::AddMonster(MONSTER_DATA *pMonster)
//int MONSTER_DATA_TYPE::AddMonster(MONSTER_DATA *pMonster)
{
  //CSingleLock sLock(&m_CS, TRUE);
  if (pMonster->Name.GetLength() == 0) return MONSTER_ID();
  // Do not allow multiple items with same full name.
  //if (m_FindName(data.Name)!=0) return 0;
  MONSTER_ID monsterID;
#ifdef UAFEngine
  monsterID = pMonster->monsterID;
#else
  monsterID = pMonster->Name;
#endif
  if (LocateMonster(monsterID) >= 0) return MONSTER_ID();
  if (GetCount() < MAX_MONSTERS)
  {
    //data.key = GetNextKey();
    //MonsterData.Insert(data, data.key);
    MonsterData.Add(*pMonster);
    return pMonster->MonsterID();
  }
  else
    return MONSTER_ID();
}
/*
// called when loading binary monster data that
// was saved with key info
int MONSTER_DATA_TYPE::AddMonsterWithCurrKey(MONSTER_DATA &data)
{
  //CSingleLock sLock(&m_CS, TRUE);
  if (MonsterData.GetCount() < MAX_MONSTERS)
  {
    MonsterData.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}
*/
//BOOL MONSTER_DATA_TYPE::SetMonster(int index, MONSTER_DATA *pMonster)
BOOL MONSTER_DATA_TYPE::SetMonster(const MONSTER_ID& monsterID, MONSTER_DATA *pMonster)
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
  {
    *GetMonster(index) = *pMonster;
    return TRUE;
  }
  else
    return FALSE;
}

//BOOL MONSTER_DATA_TYPE::RemoveMonster(int index)
BOOL MONSTER_DATA_TYPE::RemoveMonster(const MONSTER_ID& monsterID)
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
  {
    MonsterData.RemoveAt(index);
    return TRUE;
  }
  else
    return FALSE;
}

/*
//MONSTER_DATA *MONSTER_DATA_TYPE::GetMonsterData(int index)
MONSTER_DATA *MONSTER_DATA_TYPE::GetMonster(const MONSTER_ID& monsterID)
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonsterData(monsterID)) >= 0)
    return &MonsterData[index];
  else
    return NULL;
}
*/
/*
const MONSTER_DATA *MONSTER_DATA_TYPE::PeekMonsterData(const MONSTER_ID& monsterID) const
{
  //CSingleLock sLock(&m_CS, TRUE);
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonsterData(monsterID)) >= 0)
    return PeekMonsterData(index);
  else
    return NULL;
}
*/
//creatureSizeType MONSTER_DATA_TYPE::GetMonsterSize(int index) const
creatureSizeType MONSTER_DATA_TYPE::GetMonsterSize(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).Size;
    return PeekMonster(index)->Size;
    //return MonsterData[index].Size;
  else
    return Small;
}

//int MONSTER_DATA_TYPE::GetMonsterNbrAttacks(int index) const
int MONSTER_DATA_TYPE::GetMonsterNbrAttacks(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).attackData.GetCount();
    return PeekMonster(index)->attackData.GetMonsterAttackDetailsCount();
    //return MonsterData[index].attackData.GetMonsterAttackDetailsCount();
  else
    return 0;
}

//int MONSTER_DATA_TYPE::GetMonsterExpValue(int index) const
int MONSTER_DATA_TYPE::GetMonsterExpValue(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
  {
    //int val = MonsterData.PeekAtPos(pos).XP_Value;
    int val = PeekMonster(index)->XP_Value;
    double mod = GetMonsterExpMod();
    if (mod != 0.0)
    {
      val += ((mod/100.0)*val);
    }
    else
    {
      mod = GetAllExpMod();
      if (mod != 0.0) 
        val += ((mod/100.0)*val);
    }
    if (val < 0) val=0;
    return val;
  }
  else
    return 0;
}

//int MONSTER_DATA_TYPE::GetMonsterTHAC0(int index) const
int MONSTER_DATA_TYPE::GetMonsterTHAC0(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).THAC0;
    return PeekMonster(index)->THAC0;
    //return MonsterData[index].THAC0;
  else
    return 0;
}

//int MONSTER_DATA_TYPE::GetMonsterAC(int index) const
int MONSTER_DATA_TYPE::GetMonsterAC(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).Armor_Class;
    return PeekMonster(index)->Armor_Class;
    //return MonsterData[index].Armor_Class;
  else
    return 0;
}

//void MONSTER_DATA_TYPE::GetMonsterAttackMsg(int index, int attackNum, CString &msg) const
void MONSTER_DATA_TYPE::GetMonsterAttackMsg(const MONSTER_ID& monsterID, int attackNum, CString &msg) const
{
  //POSITION pos;
  int index;
  msg="";
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
  {
    //int attackMsgCnt = MonsterData.PeekAtPos(pos).attackData.GetCount();
    int attackMsgCnt = PeekMonster(index)->attackData.GetMonsterAttackDetailsCount();
    //int attackMsgCnt = MonsterData[index].attackData.GetMonsterAttackDetailsCount();
    if (attackMsgCnt==0) // shouldn't happen if monster configured properly
    {
      msg = "attacks";
      return;
    }

    // a monster can have more attacks than is natural due
    // to readying a weapon
    if (attackNum >= attackMsgCnt) attackNum = 0;    
    //msg = MonsterData.PeekAtPos(pos).attackData.PeekAtIndex(attackNum).attackMsg;
    msg = PeekMonster(index)->attackData.PeekMonsterAttackDetails(attackNum)->attackMsg;
    //msg = MonsterData[index].attackData.PeekMonsterAttackDetails(attackNum)->attackMsg;
  }
  if (msg.GetLength()==0) msg = "attacks";
}

//void MONSTER_DATA_TYPE::GetMonsterDamageDice(int index, int attackNum, int &num, int &sides, int &bonus) const
void MONSTER_DATA_TYPE::GetMonsterDamageDice(const MONSTER_ID& monsterID, int attackNum, int &num, int &sides, int &bonus) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) != NULL)
  {
    //int attackCnt = MonsterData.PeekAtPos(pos).attackData.GetCount();
    int attackCnt = PeekMonster(index)->attackData.GetMonsterAttackDetailsCount();
    //int attackCnt = MonsterData[index].attackData.GetMonsterAttackDetailsCount();
    ASSERT(attackCnt > 0);
    if (attackCnt == 0)
    {
      //WriteDebugString("Monster %s has zero attacks defined\n", MonsterData.PeekAtPos(pos).Name);
      WriteDebugString("Monster %s has zero attacks defined\n", PeekMonster(index)->Name);
      //WriteDebugString("Monster %s has zero attacks defined\n", MonsterData[index].Name);
    }
    if (attackNum < attackCnt )
    {
      //MONSTER_ATTACK_DETAILS tmp;
      //tmp = MonsterData.PeekAtPos(pos).attackData.PeekAtIndex(attackNum);
      //num = tmp.nbr;
      //sides = tmp.sides;
      //bonus = tmp.bonus;
      const MONSTER_ATTACK_DETAILS *pMAD;
      const MONSTER_DATA *pMonster;
      pMonster = PeekMonster(index);
      pMAD = pMonster->attackData.PeekMonsterAttackDetails(attackNum);
      //pMAD = MonsterData[index].attackData.PeekMonsterAttackDetails(attackNum);
      num = pMAD->nbr;
      sides = pMAD->sides;
      bonus = pMAD->bonus;
    }
    else
    {
      num=0;sides=0;bonus=0;
    }
  }
  else
  {
    num=0;sides=0;bonus=0;
  }
}

/*
//BYTE MONSTER_DATA_TYPE::GetMonsterItemMask(int index) const
BYTE MONSTER_DATA_TYPE::GetMonsterItemMask(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).ItemMask;
    return MonsterData[index].ItemMask;
  else
    return 0;
}
*/

//double MONSTER_DATA_TYPE::GetMonsterHitDice(int index) const
double MONSTER_DATA_TYPE::GetMonsterHitDice(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
  {
    //double val = MonsterData.PeekAtPos(pos).Hit_Dice;
    double val = PeekMonster(index)->Hit_Dice;
    //double val = MonsterData[index].Hit_Dice;
    //if (MonsterData.PeekAtPos(pos).UseHitDice)
    if (PeekMonster(index)->UseHitDice)
    {
      double mod = GetMonsterHDMod();
      if (mod != 0.0) val += ((mod/100.0)*val);
    }
    if (val < 0.0) val=0.5; // 1/2 hit dice is lowest possible
    return val;
  }
  else
    return 0.0;
}

//int MONSTER_DATA_TYPE::GetMonsterHitDiceBonus(int index) const
int MONSTER_DATA_TYPE::GetMonsterHitDiceBonus(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).Hit_Dice_Bonus;
    return PeekMonster(index)->Hit_Dice_Bonus;
    //return MonsterData[index].Hit_Dice_Bonus;
  else
    return 0;
}

//int MONSTER_DATA_TYPE::GetMonsterMovement(int index) const
int MONSTER_DATA_TYPE::GetMonsterMovement(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).Movement;
    return PeekMonster(index)->Movement;
    //return MonsterData[index].Movement;
  else
    return 0;
}

//DWORD MONSTER_DATA_TYPE::GetMonsterFormFlags(int index) const
DWORD MONSTER_DATA_TYPE::GetMonsterFormFlags(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).Form_Type;
    return PeekMonster(index)->Form_Type;
    //return MonsterData[index].Form_Type;
  else
    return 0;
}

//DWORD MONSTER_DATA_TYPE::GetMonsterOptionFlags(int index) const
DWORD MONSTER_DATA_TYPE::GetMonsterOptionFlags(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).MiscOptions_Type;
    return PeekMonster(index)->MiscOptions_Type;
    //return MonsterData[index].MiscOptions_Type;
  else
    return 0;
}

//DWORD MONSTER_DATA_TYPE::GetMonsterImmunityFlags(int index) const
DWORD MONSTER_DATA_TYPE::GetMonsterImmunityFlags(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).Immunity_Type;
    return PeekMonster(index)->Immunity_Type;
    //return MonsterData[index].Immunity_Type;
  else
    return 0;  
}

//DWORD MONSTER_DATA_TYPE::GetMonsterPenaltyFlags(int index) const
//DWORD MONSTER_DATA_TYPE::GetMonsterPenaltyFlags(int index) const
DWORD MONSTER_DATA_TYPE::GetMonsterPenaltyFlags(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
    //return MonsterData.PeekAtPos(pos).Penalty_Type;
    return PeekMonster(index)->Penalty_Type;
    //return MonsterData[index].Penalty_Type;
  else
    return 0;  
}
/*
// No locking... for internal use
int MONSTER_DATA_TYPE::m_FindName (const CString& name) const
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=MonsterData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (MonsterData.PeekAtPos(pos).Name==name) 
          return MonsterData.PeekAtPos(pos).key;
    MonsterData.PeekNext(pos);
  };
  return 0;
}
*/
/*
MONSTER_DATA *MONSTER_DATA_TYPE::GetMonsterData(const CString& name)
{
  POSITION pos;
  pos=MonsterData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (MonsterData.PeekAtPos(pos).Name==name) 
          return &MonsterData.GetAtPos(pos);
    MonsterData.GetNext(pos);
  };
  return NULL;
}
*/
/*
// With locking...for public use
int MONSTER_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  //CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}
*/
/*
// No locking... for internal use
int MONSTER_DATA_TYPE::m_FindNamei (const CString& name) const
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=MonsterData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (MonsterData.PeekAtPos(pos).Name.CompareNoCase(name)) 
          return MonsterData.PeekAtPos(pos).key;
    MonsterData.PeekNext(pos);
  };
  return 0;
}
*/
/*
// With locking...for public use
int MONSTER_DATA_TYPE::FindNamei(const CString& name)
// Return key associated with the named object, or zero
{
  //CSingleLock sLock(&m_CS,TRUE);
  return m_FindNamei(name);
}
*/
ActorType MONSTER_DATA::GetContext() const
{
  ActorType data;
  data.Clear();
  data.SetMonsterSrc();
  //data.Instance = key;
  data.m_monsterID = MonsterID();
  data.Level = Hit_Dice;
  return data;
}

//ActorType MONSTER_DATA_TYPE::GetContext(int index) const
ActorType MONSTER_DATA_TYPE::GetContext(const MONSTER_ID& monsterID) const
{
  //POSITION pos;
  int index;
  //if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  if ((index = LocateMonster(monsterID)) >= 0)
  {
    //return MonsterData.PeekAtPos(pos).GetContext();
    return PeekMonster(index)->GetContext();
    //return MonsterData[index].GetContext();
  }
  else 
  {
    ActorType data;
    data.Clear();
    die(0xab528);
    return data;
  }
}