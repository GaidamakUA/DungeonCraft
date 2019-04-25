/******************************************************************************
* Filename: GameRules.cpp
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

#include "..\shared\stdafx.h"
//#include "..\Shared\Version.h"

#ifdef UAFEDITOR
#include "..\UAFWinEd\UAFWinEd.h"
#else
//#include "externs.h"
#include "..\UAFWin\Dungeon.h"
#endif

#include "GlobalData.h"
#include "GameRules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern const CString Ability_Dexterity;
extern const CString Ability_Constitution;
extern const CString Ability_Strength;
extern const CString Ability_Intelligence;
extern const CString Ability_Charisma;
extern const CString Ability_Wisdom;

extern const double VersionSpellNames;
extern const double VersionSpellIDs;

void die(CString msg);

int START_AGE = 17;
int AGE_RANGE = 5;


/*
struct DICEDATA
{
  DICEDATA(n,s,b)
  int nbr;
  int sides;
  int bonus;
};
*/

#ifdef UAFEDITOR

// For hit dice, the bonus represents a fixed hp addition
// and if non-zero the dice are not rolled. This fixed
// value is cumulative. Actual hit dice bonuses are
// determined elsewhere. Do not put a non-zero bonus
// with non-zero dice sides/nbr. It's one or the other.
//
#define DEFFIGHTERHD(n)   DICEDATA(n,FIGHTER_HIT_DIE,0)
#define DEFFIGHTERHD2()   DICEDATA(0,0,3)

#define DEFPALADINHD(n)   DICEDATA(n,PALADIN_HIT_DIE,0)
#define DEFPALADINHD2()   DICEDATA(0,0,3)

#define DEFRANGERHD(n)    DICEDATA(n,RANGER_HIT_DIE,0)
#define DEFRANGERHD2()    DICEDATA(0,0,2)

#define DEFMAGICUSERHD(n) DICEDATA(n,MAGIC_USER_HIT_DIE,0)
#define DEFMAGICUSERHD2() DICEDATA(0,0,1)

#define DEFTHIEFHD(n)     DICEDATA(n,THIEF_HIT_DIE,0)
#define DEFTHIEFHD2()     DICEDATA(0,0,2)

#define DEFCLERICHD(n)    DICEDATA(n,CLERIC_HIT_DIE,0)
#define DEFCLERICHD2()    DICEDATA(0,0,2)

#define DEFDRUIDHD(n)     DICEDATA(n,DRUID_HIT_DIE,0)
#define DEFDRUIDHD2()     DICEDATA(0,0,1)

DICEDATA FighterHitDiceTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(), // manikus 7-16-09
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2(), //
  DEFFIGHTERHD2()
};
DICEDATA PaladinHitDiceTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(), // manikus 7-16-09
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2(), //
  DEFPALADINHD2()
};

DICEDATA RangerHitDiceTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFRANGERHD(2),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(), // manikus 7-16-09
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2(), //
  DEFRANGERHD2()
};

DICEDATA MagicUserHitDiceTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(), // manikus 7-16-09
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2(), //
  DEFMAGICUSERHD2()
};

DICEDATA ThiefHitDiceTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(), // manikus 7-16-09
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2(), //
  DEFTHIEFHD2()
};

DICEDATA ClericHitDiceTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(), // manikus 7-16-09
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2(), //
  DEFCLERICHD2()
};

DICEDATA DruidHitDiceTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2() // manikus 7-16-09
//  DEFDRUIDHD2(),
//  DEFDRUIDHD2(),
//  DEFDRUIDHD2(),
//  DEFDRUIDHD2(),
//  DEFDRUIDHD2(),
//  DEFDRUIDHD2(),
//  DEFDRUIDHD2()
};

#endif

CString GetGenderName(genderType type)
{
  CString temp("??");
  switch (type)
  {
  case Male:   temp = "Male";   break;
  case Female: temp = "Female"; break;
  }
  return temp;
}

CString GetAlignmentName(alignmentType align)
{
  CString temp("??");
  switch (align) 
  {
  case LawfulGood:     temp = "Lawful Good";     break;
  case NeutralGood:    temp = "Neutral Good";    break;
  case ChaoticGood:    temp = "Chaotic Good";    break;
  case LawfulNeutral:  temp = "Lawful Neutral";  break;
  case TrueNeutral:    temp = "True Neutral";    break;
  case ChaoticNeutral: temp = "Chaotic Neutral"; break;
  case LawfulEvil:     temp = "Lawful Evil";     break;
  case NeutralEvil:    temp = "Neutral Evil";    break;
  case ChaoticEvil:    temp = "Chaotic Evil";    break;
  }
  return temp;
}

// Temporary defines for table initialization only.
// 
// from file char.h
//#define UNDEAD_NOAFFECT (0)
//#define UNDEAD_TURNED   (-1)
//#define UNDEAD_DESTROY  (-2)

#define NONE  UNDEAD_NOEFFECT
#define TURN  UNDEAD_TURNED
#define DEST  UNDEAD_DESTROY
#define DE_X  UNDEAD_DESTROY

// TODO: High-level Clerics can turn with a D* result,
//       I've put DE_X in the Table to act as placeholders,
//       but for now it just evaluates to UNDEAD_DESTROY.


#ifdef _DEBUG
// these next two tables are for debugging only
// only define one at a time, and don't define any
// for a release build
//
//#define DESTROY_ALL_UNDEAD
//#define TURN_ALL_UNDEAD

#ifdef DESTROY_ALL_UNDEAD
static const int UndeadTurnTable[13][14] =
{
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST }
};
#endif

#ifdef TURN_ALL_UNDEAD
static const int UndeadTurnTable[13][14] =
{
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN }
};
#endif

#endif // ifdef _DEBUG

#if (!defined TURN_ALL_UNDEAD) && (!defined DESTROY_ALL_UNDEAD)

// Table for Clerics Turning Undead - used as a lookup
//
// Obs. The order of Undead Types on the rows of this array
//      must match the order in which they are declared within
//      the UndeadType enumeration (NotUndead is accounted for
//      by special code.)
//
static const int UndeadTurnTable[13][14] =
{
// Cleric Lvl       1     2     3     4     5     6     7     8     9    10    11    12    13    14+

/* Skeletn */  {   10,    7,    4, TURN, TURN, DEST, DEST, DE_X, DE_X, DE_X, DE_X, DE_X, DE_X, DE_X },
/* Zombie  */  {   13,   10,    7,    4, TURN, TURN, DEST, DEST, DE_X, DE_X, DE_X, DE_X, DE_X, DE_X },
/* Ghoul   */  {   16,   13,   10,    7,    4, TURN, TURN, DEST, DEST, DE_X, DE_X, DE_X, DE_X, DE_X },
/* Shadow  */  {   19,   16,   13,   10,    7,    4, TURN, TURN, DEST, DEST, DEST, DE_X, DE_X, DE_X },
/* Wight   */  {   20,   19,   16,   13,   10,    7,    4, TURN, TURN, DEST, DEST, DEST, DEST, DE_X },
/* Ghast   */  { NONE,   20,   19,   16,   13,   10,    7,    4, TURN, TURN, TURN, DEST, DEST, DEST },
/* Wraith  */  { NONE, NONE,   20,   19,   16,   13,   10,    7,    4, TURN, TURN, TURN, TURN, DEST },
/* Mummy   */  { NONE, NONE, NONE,   20,   19,   16,   13,   10,    7,    4,    4, TURN, TURN, TURN },
/* Spectre */  { NONE, NONE, NONE, NONE,   20,   19,   16,   13,   10,    7,    7,    4,    4, TURN },
/* Vampire */  { NONE, NONE, NONE, NONE, NONE,   20,   19,   16,   13,   10,   10,    7,    7,    4 },
/* Ghost   */  { NONE, NONE, NONE, NONE, NONE, NONE,   20,   19,   16,   13,   13,   10,   10,    7 },
/* Lich    */  { NONE, NONE, NONE, NONE, NONE, NONE, NONE,   20,   19,   16,   16,   13,   13,   10 },
/* Special */  { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,   20,   19,   19,   16,   16,   13 }
};

#endif


// Undefine temporary defines for Undead Turning table.
// 
#undef NONE
#undef TURN
#undef DEST
#undef DE_X

// GetUndeadTurnValueByHD is used internally by the
// exported GetUndeadTurnValue method.
//
// modifier is one of [ None, Hard, Difficult, Impossible ].
//
int GetUndeadTurnValueByHD(int level, int undeadHD, eventTurnUndeadModType modifier)
{
  if ( ( undeadHD <= 0 ) ||       // this condition should be taken care of by caller
       ( modifier == Impossible ) ||
       ( level <= 0 ) )
  {
    return UNDEAD_NOEFFECT;
  }

  // Cap cleric level, UndeadTurnTable can only handle
  // Cleric levels of 1 to 15 (the column for 14 can be
  // used for 14-15th level Clerics).
  //
  if ( level > 14 )
  {
    level = 14;
  }

  // Assertion: UndeadTurnTable cannot handle more
  // than 12 hit dice of Undead.
  //
  ASSERT( undeadHD <= 12 );
  // just in case, so we don't crash
  if ( undeadHD > 12 )
  {
    undeadHD = 12;
  }

  // Array is zero-based indexing; so subtract one from
  // one-based level and hit dice to perform look-up.
  //
  // result will be 1-20=roll dice, 0=no affect, -1=turned, -2=destroyed
  int result = UndeadTurnTable[ undeadHD - 1][ level - 1];

  // OBS. We only adjust d20 Undead turn results for
  // Difficulty -- why not T and D results?
  //
  if ( ( result >= 1 ) && ( result <= 20 ) )
  {
    // Adjust for Difficulty
    //
    switch (modifier)
    {
      case Hard:      result += 2; break; // I only guessed for these modifier values,
      case Difficult: result += 4; break; // not sure what UA actually uses.
    }
  }

  // may have to cap result if the above condition was false,
  // or if +modifier pushed it above 20
  if ( result > 20 )
  {
    result = 20;
  }
  return result;
}


// GetUndeadTurnValue returns the number necessary on a
// d20 turning roll for Undead to be turned.
//
// GetUndeadTurnValue may also return three special values:
//
//   UNDEAD_NOAFFECT - cleric has no effect
//   UNDEAD_TURNED - 2d6 HD of undead should be turned
//   UNDEAD_DESTROYED - 2d6 HD of undead should be destroyed
//
// modifier parameter specifies game difficulty.
//
// See Char.h for the definitions of these constants.
//
//int GetUndeadTurnValue(int level, undeadType type, eventTurnUndeadModType modifier)
int GetUndeadTurnValue(int level, int undeadLevel, eventTurnUndeadModType modifier)
{
  //int hd;  // not 'really' hit dice, but row of table.

  // Assertion: Bad undeadType, perhaps undeadType was extended
  // w/o a corresponding change made to UndeadTurnTable.
  //
//  ASSERT( (int)type <= Special );

  //hd = (int)type;

  // This is equivalent to testing type == NotUndead
  //
  //if ( !hd )
  if (undeadLevel == 999)
  {
    return UNDEAD_NOEFFECT;
  }

  //return GetUndeadTurnValueByHD( level, hd, modifier);
  NotImplemented(0x145ab, false);
  return 0;
}

#ifdef UAFEDITOR
struct EXPERIENCE_POINT_DATA
{
  EXPERIENCE_POINT_DATA() {min=max=0;}
  EXPERIENCE_POINT_DATA(int mn,int mx) {min=mn;max=mx;}
  int min;
  int max;
};

#define DEFEXPPTS(a,b) EXPERIENCE_POINT_DATA(a,b)

EXPERIENCE_POINT_DATA FighterExpPointTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFEXPPTS(      0,   2000), // changed to 1e by manikus 6-22-09
  DEFEXPPTS(   2001,   4000), //
  DEFEXPPTS(   4001,   8000), //
  DEFEXPPTS(   8001,  18000), //
  DEFEXPPTS(  16001,  35000), //
  DEFEXPPTS(  32001,  70000), //
  DEFEXPPTS(  64001, 125000), //
  DEFEXPPTS( 125001, 250000), //
  DEFEXPPTS( 250001, 500000), //
  DEFEXPPTS( 500001, 750000), //
  DEFEXPPTS( 750001,1000000), //
  DEFEXPPTS(1000001,1250000), //
  DEFEXPPTS(1250001,1500000), //
  DEFEXPPTS(1500001,1750000), //
  DEFEXPPTS(1750001,2000000), //
  DEFEXPPTS(2000001,2250000), //
  DEFEXPPTS(2250001,2500000), //
  DEFEXPPTS(2500001,2750000), //
  DEFEXPPTS(2750001,3000000), //
  DEFEXPPTS(3000001,3250000), //
  DEFEXPPTS(3250001,3500000), //
  DEFEXPPTS(3500001,3750000), //
  DEFEXPPTS(3750001,4000000), //
  DEFEXPPTS(4000001,4250000), //
  DEFEXPPTS(4250001,4500000), //
  DEFEXPPTS(4500001,4750000), //
  DEFEXPPTS(4750001,5000000), //
  DEFEXPPTS(5000001,5250000), //
  DEFEXPPTS(5250001,5500000), //
  DEFEXPPTS(5500001,5750000), //
  DEFEXPPTS(5750001,6000000),  // manikus 7-16-09
  DEFEXPPTS(6000001,6250000),  //
  DEFEXPPTS(6250001,6500000),  //
  DEFEXPPTS(6500001,6750000),  //
  DEFEXPPTS(6750001,7000000),  //
  DEFEXPPTS(7000001,7250000),  //
  DEFEXPPTS(7250001,7500000),  //
  DEFEXPPTS(7500001,7750000),  //
  DEFEXPPTS(7750001,8000000),  //
  DEFEXPPTS(8000001,8250000)  //
};


EXPERIENCE_POINT_DATA RangerExpPointTable[HIGHEST_CHARACTER_LEVEL] =
//fixed by manikus
{
  DEFEXPPTS(      0,   2250),
  DEFEXPPTS(   2251,   4500),
  DEFEXPPTS(   4501,  10000), //corrected by manikus 10-14-08
  DEFEXPPTS(  10001,  20000),
  DEFEXPPTS(  20001,  40000),
  DEFEXPPTS(  40001,  90000),
  DEFEXPPTS(  90001, 150000),
  DEFEXPPTS( 150001, 225000),
  DEFEXPPTS( 225001, 325000),
  DEFEXPPTS( 325001, 650000),
  DEFEXPPTS( 650001, 975000),
  DEFEXPPTS( 975001,1300000),
  DEFEXPPTS(1300001,1625000),
  DEFEXPPTS(1625001,1950000),
  DEFEXPPTS(1950001,2275000),
  DEFEXPPTS(2275001,2600000),
  DEFEXPPTS(2600001,2925000),
  DEFEXPPTS(2925001,3250000),
  DEFEXPPTS(3250001,3575000),
  DEFEXPPTS(3575001,3900000),
  DEFEXPPTS(3900001,4225000),
  DEFEXPPTS(4225001,4550000),
  DEFEXPPTS(4550001,4875000),
  DEFEXPPTS(4875001,5200000),
  DEFEXPPTS(5200001,5525000),
  DEFEXPPTS(5525001,5850000),
  DEFEXPPTS(5850001,6175000),
  DEFEXPPTS(6175001,6500000),
  DEFEXPPTS(6500001,6825000),
  DEFEXPPTS( 6825001, 7150000), //
  DEFEXPPTS( 7150001, 7475000),  //
  DEFEXPPTS( 7475001, 7800000),  //
  DEFEXPPTS( 7800001, 8125000),  //
  DEFEXPPTS( 8125001, 8450000),  //
  DEFEXPPTS( 8450001, 8775000),  //
  DEFEXPPTS( 8775001, 9100000),  //
  DEFEXPPTS( 9100001, 9425000),  //
  DEFEXPPTS( 9425001, 9750000),  //
  DEFEXPPTS( 9750001,10075000),  //
  DEFEXPPTS(10075001,10400000)  //
};
EXPERIENCE_POINT_DATA PaladinExpPointTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFEXPPTS(      0,   2250),
  DEFEXPPTS(   2251,   4500),
  DEFEXPPTS(   4501,   9000),
  DEFEXPPTS(   9001,  18000),
  DEFEXPPTS(  18001,  36000),
  DEFEXPPTS(  36001,  75000),
  DEFEXPPTS(  75001, 150000),
  DEFEXPPTS( 150001, 300000),
  DEFEXPPTS( 300001, 600000),
  DEFEXPPTS( 600001, 900000),
  DEFEXPPTS( 900001,1200000),
  DEFEXPPTS(1200001,1500000),
  DEFEXPPTS(1500001,1800000),
  DEFEXPPTS(1800001,2100000),
  DEFEXPPTS(2100001,2400000),
  DEFEXPPTS(2400001,2700000),
  DEFEXPPTS(2700001,3000000),
  DEFEXPPTS(3000001,3300000),
  DEFEXPPTS(3300001,3600000),
  DEFEXPPTS(3600001,3900000),
  DEFEXPPTS(3900001,4200000),
  DEFEXPPTS(4200001,4500000),
  DEFEXPPTS(4500001,4800000),
  DEFEXPPTS(4800001,5100000),
  DEFEXPPTS(5100001,5400000),
  DEFEXPPTS(5400001,5700000),
  DEFEXPPTS(5700001,6000000),
  DEFEXPPTS(6000001,6300000),
  DEFEXPPTS(6300001,6600000),
  DEFEXPPTS(6600001,6900000), // manikus 7-16-09
  DEFEXPPTS(6900001,7200000),  //
  DEFEXPPTS(7200001,7500000),  //
  DEFEXPPTS(7500001,7800000),  //
  DEFEXPPTS(7800001,8100000),  //
  DEFEXPPTS(8100001,8400000),  //
  DEFEXPPTS(8400001,8700000),  //
  DEFEXPPTS(8700001,9000000),  //
  DEFEXPPTS(9000001,9300000),  //
  DEFEXPPTS(9300001,9600000),  //
  DEFEXPPTS(9600001,9900000)  //
};
EXPERIENCE_POINT_DATA MagicUserExpPointTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFEXPPTS(       0,    2500), // changed to 1e by manikus 6-22-09
  DEFEXPPTS(    2501,    5000), //
  DEFEXPPTS(    5001,   10000), //
  DEFEXPPTS(   10001,   22500), //
  DEFEXPPTS(   20001,   40000), //
  DEFEXPPTS(   40001,   60000), //
  DEFEXPPTS(   60001,   90000), //
  DEFEXPPTS(   90001,  135000), //
  DEFEXPPTS(  135001,  250000), //
  DEFEXPPTS(  250001,  375000), //
  DEFEXPPTS(  375001,  750000), //
  DEFEXPPTS(  750001, 1125000), //
  DEFEXPPTS( 1125001, 1500000), //
  DEFEXPPTS( 1500001, 1875000), //
  DEFEXPPTS( 1875001, 2250000), //
  DEFEXPPTS( 2250001, 2625000), //
  DEFEXPPTS( 2625001, 3000000), //
  DEFEXPPTS( 3000001, 3375000), //
  DEFEXPPTS( 3375001, 3750000), //
  DEFEXPPTS( 3750001, 4125000), //
  DEFEXPPTS( 4125001, 4500000), //
  DEFEXPPTS( 4500001, 4875000), //
  DEFEXPPTS( 4875001, 5250000), //
  DEFEXPPTS( 5250001, 5625000), //
  DEFEXPPTS( 5625001, 6000000), //
  DEFEXPPTS( 6000001, 6375000), //
  DEFEXPPTS( 6375001, 6750000), //
  DEFEXPPTS( 6750001, 7125000), //
  DEFEXPPTS( 7125001, 7500000), //
  DEFEXPPTS( 7500001, 7875000),  // manikus 7-16-09
  DEFEXPPTS( 7875001, 8250000),  //
  DEFEXPPTS( 8250001, 8625000),  //
  DEFEXPPTS( 8625001, 9000000),  //
  DEFEXPPTS( 9000001, 9375000),  //
  DEFEXPPTS( 9375001, 9750000),  //
  DEFEXPPTS( 9750001,10125000),  //
  DEFEXPPTS(10125001,10500000),  //
  DEFEXPPTS(10500001,10875000),  //
  DEFEXPPTS(10875001,11250000),  //
  DEFEXPPTS(11250001,11625000)  //
};
EXPERIENCE_POINT_DATA ThiefExpPointTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFEXPPTS(      0,   1250), // changed to 1e by manikus 6-22-09
  DEFEXPPTS(   1251,   2500), //
  DEFEXPPTS(   2501,   5000), //
  DEFEXPPTS(   5001,  10000), //
  DEFEXPPTS(  10001,  20000), //
  DEFEXPPTS(  20001,  42500), //
  DEFEXPPTS(  40001,  70000), //
  DEFEXPPTS(  70001, 110000), //
  DEFEXPPTS( 110001, 150000), //
  DEFEXPPTS( 160001, 220000), //
  DEFEXPPTS( 220001, 440000), //
  DEFEXPPTS( 440001, 660000), //
  DEFEXPPTS( 660001, 880000), //
  DEFEXPPTS( 880001,1100000), //
  DEFEXPPTS(1100001,1320000), //
  DEFEXPPTS(1320001,1540000), //
  DEFEXPPTS(1540001,1756000), //
  DEFEXPPTS(1760001,1980000), //
  DEFEXPPTS(1980001,2200000), //
  DEFEXPPTS(2200001,2420000), //
  DEFEXPPTS(2420001,2640000), //
  DEFEXPPTS(2640001,2860000), //
  DEFEXPPTS(2860001,3080000), //
  DEFEXPPTS(3080001,3300000), //
  DEFEXPPTS(3300001,3520000), //
  DEFEXPPTS(3520001,3740000), //
  DEFEXPPTS(3740001,3960000), //
  DEFEXPPTS(3960001,4180000), //
  DEFEXPPTS(4180001,4400000), //
  DEFEXPPTS(4400001,4620000),  // manikus 7-16-09
  DEFEXPPTS(4620001,4840000),  //
  DEFEXPPTS(4840001,5060000),  //
  DEFEXPPTS(5060001,5280000),  //
  DEFEXPPTS(5280001,5500000),  //
  DEFEXPPTS(5500001,5720000),  //
  DEFEXPPTS(5720001,5940000),  //
  DEFEXPPTS(5940001,6160000),  //
  DEFEXPPTS(6160001,6380000),  //
  DEFEXPPTS(6380001,6600000),  //
  DEFEXPPTS(6600001,6820000),  //
};
EXPERIENCE_POINT_DATA DruidExpPointTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFEXPPTS(       0,    2000),
  DEFEXPPTS(    2001,    4000),
  DEFEXPPTS(    4001,    7500),
  DEFEXPPTS(    7501,   12500),
  DEFEXPPTS(   12501,   20000),
  DEFEXPPTS(   20001,   35000),
  DEFEXPPTS(   35001,   60000),
  DEFEXPPTS(   60001,   90000),
  DEFEXPPTS(   90001,  125000),
  DEFEXPPTS(  125001,  200000),
  DEFEXPPTS(  200001,  300000),
  DEFEXPPTS(  300001,  750000),
  DEFEXPPTS(  750001, 1500000),
  DEFEXPPTS( 1500001, 3000000),
  DEFEXPPTS( 3000001, 3500000),
  DEFEXPPTS( 3500001, 4000000),
  DEFEXPPTS( 4000001, 4500000),
  DEFEXPPTS( 4500001, 5000000),
  DEFEXPPTS( 5000001, 5500000),
  DEFEXPPTS( 5500001, 6000000),
  DEFEXPPTS( 6000001, 6500000),
  DEFEXPPTS( 6500001, 7000000),
  DEFEXPPTS( 7000001, 7500000) // manikus 7-16-09
  // DEFEXPPTS( 7500001, 8000000), //
  // DEFEXPPTS( 8000001, 8500000), //
  // DEFEXPPTS( 8500001, 9000000), //
  // DEFEXPPTS( 9000001, 9500000), //
  // DEFEXPPTS( 9500001,10000000), //
  // DEFEXPPTS(10000001,10500000), //
  // DEFEXPPTS(10500001,11000000) //
};
EXPERIENCE_POINT_DATA ClericExpPointTable[HIGHEST_CHARACTER_LEVEL] =
{
  DEFEXPPTS(      0,   1500), // changed to 1e by manikus 6-22-09
  DEFEXPPTS(   1501,   3000), //
  DEFEXPPTS(   3001,   6000), //
  DEFEXPPTS(   6001,  13000), //
  DEFEXPPTS(  13001,  27500), //
  DEFEXPPTS(  27501,  55000), //
  DEFEXPPTS(  55001, 109999), //
  DEFEXPPTS( 110001, 225000), //
  DEFEXPPTS( 225001, 450000), //
  DEFEXPPTS( 450001, 675000), //
  DEFEXPPTS( 675001, 900000), //
  DEFEXPPTS( 900001,1125000), //
  DEFEXPPTS(1125001,1350000), //
  DEFEXPPTS(1350001,1575000), //
  DEFEXPPTS(1575001,1800000), //
  DEFEXPPTS(1800001,2025000), //
  DEFEXPPTS(2025001,2250000), //
  DEFEXPPTS(2250001,2475000), //
  DEFEXPPTS(2475001,2700000), //
  DEFEXPPTS(2700001,2925000), //
  DEFEXPPTS(2925001,3150000), //
  DEFEXPPTS(3150001,3375000), //
  DEFEXPPTS(3375001,3600000), //
  DEFEXPPTS(3600001,3825000), //
  DEFEXPPTS(3825001,4050000), //
  DEFEXPPTS(4050001,4275000), //
  DEFEXPPTS(4275001,4500000), //
  DEFEXPPTS(4500001,4725000), //
  DEFEXPPTS(4725001,4950000), //
  DEFEXPPTS(4950001,5175000),  // manikus 7-16-09
  DEFEXPPTS(5175001,5400000),  //
  DEFEXPPTS(5400001,5625000),  //
  DEFEXPPTS(5625001,5850000),  //
  DEFEXPPTS(5850001,6075000),  //
  DEFEXPPTS(6075001,6300000),  //
  DEFEXPPTS(6300001,6525000),  //
  DEFEXPPTS(6525001,6750000),  //
  DEFEXPPTS(6750001,6975000),  //
  DEFEXPPTS(6975001,7200000),  //
  DEFEXPPTS(7200001,7425000)  //
};

#endif



#ifdef UAFEDITOR
int GetMaxFighterExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up

  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return FighterExpPointTable[level].max;
}

int GetMinFighterExpForLevel(int level)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return FighterExpPointTable[level].min;
}
#endif


#ifdef UAFEDITOR
int GetMaxPaladinExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return PaladinExpPointTable[level].max;
}
#endif

/*
int GetMinPaladinExpForLevel(int level)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return PaladinExpPointTable[level].min;
}
*/

#ifdef UAFEDITOR
int GetMaxRangerExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return RangerExpPointTable[level].max;
}
#endif
/*
int GetMinRangerExpForLevel(int level)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return RangerExpPointTable[level].min;
}
*/
#ifdef UAFEDITOR
int GetMaxMagicUserExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return MagicUserExpPointTable[level].max;
}
#endif
/*
int GetMinMagicUserExpForLevel(int level)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return MagicUserExpPointTable[level].min;
}
*/
#ifdef UAFEDITOR
int GetMaxThiefExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return ThiefExpPointTable[level].max;
}
#endif
/*
int GetMinThiefExpForLevel(int level)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return ThiefExpPointTable[level].min;
}
*/

#ifdef UAFEDITOR
int GetMaxDruidExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return DruidExpPointTable[level].max;
}
#endif
/*
int GetMinDruidExpForLevel(int level)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return DruidExpPointTable[level].min;
}
*/
#ifdef UAFEDITOR
int GetMaxClericExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return ClericExpPointTable[level].max;
}
#endif
/*
int GetMinClericExpForLevel(int level)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;
  return ClericExpPointTable[level].min;
}
*/

int GetMaxExpForBaseclassLevel(const BASE_CLASS_DATA *pBaseClass, int lvl)
{
  NotImplemented(0x1b4329, false);
  return 0;
}
int GetMinExpForBaseclassLevel(const BASE_CLASS_DATA *pBaseClass, int lvl)
{
  NotImplemented(0x1b432a, false);
  return 0;
}

/*
void getFighterHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;

  dice = FighterHitDiceTable[level].sides;
  nbr = FighterHitDiceTable[level].nbr;
  constant = FighterHitDiceTable[level].bonus;    
}
void getPaladinHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;

  dice = PaladinHitDiceTable[level].sides;
  nbr = PaladinHitDiceTable[level].nbr;
  constant = PaladinHitDiceTable[level].bonus;   
}
void getRangerHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;

  dice = RangerHitDiceTable[level].sides;
  nbr = RangerHitDiceTable[level].nbr;
  constant = RangerHitDiceTable[level].bonus;   
}
void getMagicUserHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;

  dice = MagicUserHitDiceTable[level].sides;
  nbr = MagicUserHitDiceTable[level].nbr;
  constant = MagicUserHitDiceTable[level].bonus;   
}
void getThiefHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;

  dice = ThiefHitDiceTable[level].sides;
  nbr = ThiefHitDiceTable[level].nbr;
  constant = ThiefHitDiceTable[level].bonus;   
}
void getClericHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;

  dice = ClericHitDiceTable[level].sides;
  nbr = ClericHitDiceTable[level].nbr;
  constant = ClericHitDiceTable[level].bonus;    
}
void getDruidHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  if (level < 1) level = 1;
  level--;

  dice = DruidHitDiceTable[level].sides;
  nbr = DruidHitDiceTable[level].nbr;
  constant = DruidHitDiceTable[level].bonus;    
}
*/

/*
void getCharClassHitDice(WORD ClassFlag, int level, int& dice, int& nbr, int& constant)
{
  //ASSERT( NumClassBits(ClassFlag) == 1 );

  switch (ClassFlag)
  {
    case MagicUserFlag: getMagicUserHitDice(level,dice,nbr,constant); break;
    case ClericFlag:    getClericHitDice(level,dice,nbr,constant); break;
    case ThiefFlag:     getThiefHitDice(level,dice,nbr,constant); break;
    case FighterFlag:   getFighterHitDice(level,dice,nbr,constant); break;
    case PaladinFlag:   getPaladinHitDice(level,dice,nbr,constant); break;
    case RangerFlag:    getRangerHitDice(level,dice,nbr,constant); break;
    case DruidFlag:     getDruidHitDice(level,dice,nbr,constant); break;
    default: ASS ERT(FALSE); return;
  }
 NotImplemented(0xed56a0, false);



}
*/
void GetBaseclassHitDice(const BASECLASS_ID& baseclassID,
                         int level,
                         int& sides,
                         int& nbrDice,
                         int& constant)
{
  const BASE_CLASS_DATA *pBaseclass;
  const DICEDATA *pDiceData;
  pBaseclass = baseclassData.PeekBaseclass(baseclassID);
  if (pBaseclass == NULL)
  {
    sides = nbrDice = constant = 0;
    return;
  };
  if (level < 1) level = 1;
  if (level > HIGHEST_CHARACTER_LEVEL) level = HIGHEST_CHARACTER_LEVEL;
  pDiceData = &pBaseclass->hitDice[level-1];
  nbrDice  = pDiceData->nbr;
  sides    = pDiceData->sides;
  constant = pDiceData->bonus;
}


/*
int getFighterLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_CHARACTER_LEVEL)
  {
    if (   (exp >= FighterExpPointTable[count].min)
        && (exp <= FighterExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_CHARACTER_LEVEL;
}

int getPaladinLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_CHARACTER_LEVEL)
  {
    if (   (exp >= PaladinExpPointTable[count].min)
        && (exp <= PaladinExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_CHARACTER_LEVEL;
}

int getRangerLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_CHARACTER_LEVEL)
  {
    if (   (exp >= RangerExpPointTable[count].min)
        && (exp <= RangerExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_CHARACTER_LEVEL;
}

int getClericLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_CHARACTER_LEVEL)
  {
    if (   (exp >= ClericExpPointTable[count].min)
        && (exp <= ClericExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_CHARACTER_LEVEL;
}

int getDruidLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_CHARACTER_LEVEL)
  {
    if (   (exp >= DruidExpPointTable[count].min)
        && (exp <= DruidExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_CHARACTER_LEVEL;
}

int getMagicUserLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_CHARACTER_LEVEL)
  {
    if (   (exp >= MagicUserExpPointTable[count].min)
        && (exp <= MagicUserExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_CHARACTER_LEVEL;
}

int getThiefLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_CHARACTER_LEVEL)
  {
    if (   (exp >= ThiefExpPointTable[count].min)
        && (exp <= ThiefExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_CHARACTER_LEVEL;
}
*/
/*
WORD SetClassBitsFromCharClass(classType ctype)
{
  WORD mask=0;
  if (isFighter(ctype)) mask |= FighterFlag;
  if (isCleric(ctype)) mask |= ClericFlag;
  if (isMagicUser(ctype)) mask |= MagicUserFlag;
  if (isThief(ctype)) mask |= ThiefFlag;
  if (isRanger(ctype)) mask |= RangerFlag;
  if (isPaladin(ctype)) mask |= PaladinFlag;
  if (isDruid(ctype)) mask |= DruidFlag;
  return mask;
}
// don't pass a multi-class type
WORD CharClassToClassFlag(classType ctype)
{
  ASSERT(!isMultiClass(ctype));
  switch (ctype)
  {
  case Fighter: return FighterFlag;
  case Cleric: return ClericFlag;
  case Ranger: return RangerFlag;
  case Paladin: return PaladinFlag;
  case MagicUser: return MagicUserFlag;
  case Thief: return ThiefFlag;
  case Druid: return DruidFlag;
  default:
      ASS ERT(FALSE);
      return 0;
  }
  return 0;
}
// don't pass a multi-class flag
classType ClassFlagToCharClass(WORD cflag)
{
  ASSERT( NumClassBits(cflag) == 1 );

  switch (cflag)
  {
  case FighterFlag: return Fighter;
  case ClericFlag: return Cleric;
  case RangerFlag: return Ranger;
  case PaladinFlag: return Paladin;
  case MagicUserFlag: return MagicUser;
  case ThiefFlag: return Thief;
  case DruidFlag: return Druid;
  default:ASS ERT(FALSE);return BogusClass;
  }
  return BogusClass;
}
*/
//int numClasses(classType type)
//{
//  return (NumClassBits(SetClassBitsFromCharClass(type)));/
//}

/*
int NumClasses(const CLASS_ID& classID)
{
  return classData.NumClasses(classID);
}
*/


//BOOL isMultiClass(classType type)
BOOL isMultiClass(const CLASS_ID& classID)
{
  //return (numClasses(type)>1);
  int i;
  i = classData.LocateClass(classID);
  if (i < 0) return FALSE;
  return classData.PeekClass(i)->GetCount() > 1;
}

//BOOL doClassesOverlap(classType type1, classType type2)
BOOL doClassesOverlap(const CLASS_ID& classID1, const CLASS_ID& classID2)
{
  //DWORD bits1 = SetClassBitsFromCharClass(type1);
  //DWORD bits2 = SetClassBitsFromCharClass(type2);
  //return ((bits1 & bits2) ? TRUE : FALSE);
  NotImplemented(0x2b654c, false);
  return FALSE;
}
//BOOL doClassesOverlap(classType type1, classType type2)
BOOL doClassesOverlap(const BASECLASS_ID& classID1, const CLASS_ID& classID2)
{
  //DWORD bits1 = SetClassBitsFromCharClass(type1);
  //DWORD bits2 = SetClassBitsFromCharClass(type2);
  //return ((bits1 & bits2) ? TRUE : FALSE);
  NotImplemented(0x2b654d, false);
  return FALSE;
}

/* These are defined in GameRules.h
enum classType { Fighter, Cleric, Ranger, Paladin, MagicUser, Thief, Druid,
						C_F, C_F_MU, C_R, C_MU, C_T, F_MU, F_T, 
            F_MU_T, MU_T, F_D, D_MU, F_MU_D };
*/


#ifdef UAFEDITOR

//BOOL isDruid(classType type)
BOOL isDruid(classType_Editor type)
{
  switch (type)
  {
  case Druid:
  case F_D:
  case D_MU:
  case F_MU_D:
    return TRUE;
  }
  return FALSE;
}

//BOOL isPaladin(classType type)
BOOL isPaladin(classType_Editor type)
{
  switch (type)
  {
  case Paladin:
    return TRUE;
  }
  return FALSE;
}

//BOOL isRanger(classType type)
BOOL isRanger(classType_Editor type)
{
  switch (type)
  {
  case Ranger:
  case C_R:
    return TRUE;
  }
  return FALSE;
}

//BOOL isThief(classType type)
BOOL isThief(classType_Editor type)
{
  switch (type)
  {
  case Thief:
  case C_T:
  case F_T:
  case F_MU_T:
  case MU_T:
    return TRUE;
  }
  return FALSE;
}

//BOOL isMagicUser(classType type)
BOOL isMagicUser(classType_Editor type)
{
  switch (type)
  {
  case MagicUser:
  case C_F_MU:
  case C_MU:
  case F_MU:
  case F_MU_T:
  case MU_T:
  case D_MU:
  case F_MU_D:
    return TRUE;
  }
  return FALSE;
}
#endif UAFEDITOR


/*
//*****************************************************************************
//    NAME: CanCastMagicUserSpells
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CanCastMagicUserSpells(classType CharClass)
{
  WORD ClassBits = SetClassBitsFromCharClass(CharClass);
  for (int i=0;i<NumBaseClass;i++)
  {
    WORD flag = (ClassBits & (0x0001 << i));
    if (flag)
    {
      if (GetSpellCastingSchoolForCharBaseclass(flag) == MagicUserFlag)
        return TRUE;
    }
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: CanCastClericSpells
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CanCastClericSpells(classType CharClass)
{
  WORD ClassBits = SetClassBitsFromCharClass(CharClass);
  for (int i=0;i<NumBaseClass;i++)
  {
    WORD flag = (ClassBits & (0x0001 << i));
    if (flag)
    {
      if (GetSpellCastingSchoolForCharBaseclass(flag) == ClericFlag)
        return TRUE;
    }
  }
  return FALSE;
}
*/

#ifdef UAFEDITOR
//BOOL isCleric(classType type)
BOOL isCleric(classType_Editor type)
{
  switch (type)
  {
  case Cleric:
  case C_F:
  case C_F_MU:
  case C_R:
  case C_MU:
  case C_T:
    return TRUE;
  }
  return FALSE;
}
//BOOL isFighter(classType type)
BOOL isFighter(classType_Editor type)
{
  switch (type)
  {
  case Fighter:
  case C_F:
  case C_F_MU:
  case F_MU:
  case F_T:
  case F_MU_T:
  case F_D:
  case F_MU_D:
    return TRUE;
  }
  return FALSE;
}
#endif

void determineStrengthProperties(int strength, int strengthMod,
                                 int &hitBonus, int &dmgBon, BYTE &openDoor, 
                                 BYTE &openMagicDoor, BYTE &BB_LG)
{
  switch (strength) 
  {
  case 1:
    hitBonus = -5;
    dmgBon = -4;
    openDoor = 1;      // locked door
    openMagicDoor = 0; // locked needing spell to open
    BB_LG = 0;
    break;
  case 2:
    hitBonus = -3;
    dmgBon = -2;
    openDoor = 1;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 3:
    hitBonus = -3;
    dmgBon = -1;
    openDoor = 2;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 4:
  case 5:
    hitBonus = -2;
    dmgBon = -1;
    openDoor = 3;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 6:
  case 7:
    hitBonus = -1;
    dmgBon = 0;
    openDoor = 4;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 8:
  case 9:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 5;
    openMagicDoor = 0;
    BB_LG = 1;
    break;
  case 10:
  case 11:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 6;
    openMagicDoor = 0;
    BB_LG = 2;
    break;
  case 12:
  case 13:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 7;
    openMagicDoor = 0;
    BB_LG = 4;
    break;
  case 14:
  case 15:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 8;
    openMagicDoor = 0;
    BB_LG = 7;
    break;
  case 16:
    hitBonus = 0;
    dmgBon = 1;
    openDoor = 9;
    openMagicDoor = 0;
    BB_LG = 10;
    break;
  case 17:
    hitBonus = 1;
    dmgBon = 1;
    openDoor = 10;
    openMagicDoor = 0;
    BB_LG = 13;
    break;
  case 18:
    if (strengthMod == 0)
    {
      hitBonus = 1;
      dmgBon = 2;
      openDoor = 11;
      openMagicDoor = 0;
      BB_LG = 16;
    }
    else if (strengthMod < 51)
    {
      hitBonus = 1;
      dmgBon = 3;
      openDoor = 12;
      openMagicDoor = 0;
      BB_LG = 20;
    }
    else if (strengthMod < 76)
    {
      hitBonus = 2;
      dmgBon = 3;
      openDoor = 13;
      openMagicDoor = 0;
      BB_LG = 25;
    }
    else if (strengthMod < 91)
    {
      hitBonus = 2;
      dmgBon = 4;
      openDoor = 14;
      openMagicDoor = 0;
      BB_LG = 30;
    }
    else if (strengthMod < 100)
    {
      hitBonus = 2;
      dmgBon = 5;
      openDoor = 15;
      openMagicDoor = 3;
      BB_LG = 35;
    }
    else {
      hitBonus = 3;
      dmgBon = 6;
      openDoor = 16;
      openMagicDoor = 6;
      BB_LG = 40;
    }
    break;
  case 19:
    hitBonus = 3;
    dmgBon = 7;
    openDoor = 16;
    openMagicDoor = 8;
    BB_LG = 50;
    break;
  case 20:
    hitBonus = 3;
    dmgBon = 8;
    openDoor = 17;
    openMagicDoor = 10;
    BB_LG = 60;
    break;
  case 21:
    hitBonus = 4;
    dmgBon = 9;
    openDoor = 17;
    openMagicDoor = 12;
    BB_LG = 70;
    break;
  case 22:
    hitBonus = 4;
    dmgBon = 10;
    openDoor = 18;
    openMagicDoor = 14;
    BB_LG = 80;
    break;
  case 23:
    hitBonus = 5;
    dmgBon = 11;
    openDoor = 18;
    openMagicDoor = 16;
    BB_LG = 90;
    break;
  case 24:
    hitBonus = 6;
    dmgBon = 12;
    openDoor = 19;
    openMagicDoor = 17;
    BB_LG = 95;
    break;
  case 25:
    hitBonus = 7;
    dmgBon = 16;
    openDoor = 19;
    openMagicDoor = 18;
    BB_LG = 99;
    break;
  default:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 0;
    openMagicDoor = 0;
    BB_LG = 0;
    WriteDebugString("Unhandled strength %i in determineStrengthProperties()\n", strength);
    break;
  }
}

#ifdef UAFEDITOR
int determineHitDiceBonus(BYTE Score, BOOL IsFighter)
{
   int bonus = 0;
   switch (Score)
   {
   case 3:
      bonus = -2;
      break;
   case 4:
   case 5:
   case 6:
      bonus = -1;
      break;
   case 15:
      bonus = 1;
      break;
   case 16:
      bonus = 2;
      break;
   case 17:
      if (IsFighter)
         bonus = 3;
      else
         bonus = 2;
      break;
   case 18:
      if (IsFighter)
         bonus = 4;
      else
         bonus = 2;
      break;
   default:
      bonus = 0;
      break;
   }
   return bonus;
}

#endif

//BYTE rollSkillDie(abilityType type)
BYTE rollSkillDie(const CString& abilityName)
{
  if (globalData.version < _VERSION_0870_)
  {
    BYTE tmp1 = (BYTE)RollDice(SKILL_DIE,3);
    BYTE tmp2 = (BYTE)RollDice(SKILL_DIE,3);
    BYTE tmp3 = (BYTE)RollDice(SKILL_DIE,3);
    return ( max(tmp1, max(tmp2,tmp3)) );
  }
  else
  {
    double result;  
    const ABILITY_DATA *pAbilityData;
    BYTE tmp1=0,tmp2=0,tmp3=0;  
/*
    if (abilityData.RollAbility(type, result))
      tmp1=(BYTE)result;
    if (abilityData.RollAbility(type, result))
      tmp2=(BYTE)result;
    if (abilityData.RollAbility(type, result))
      tmp3=(BYTE)result;
*/
    pAbilityData = abilityData.PeekAbilityData(abilityData.LocateAbilityData(abilityName));
    if (pAbilityData != NULL)
    {
      if (pAbilityData->RollAbility(result))
        tmp1=(BYTE)result;
      if (pAbilityData->RollAbility(result))
        tmp2=(BYTE)result;
      if (pAbilityData->RollAbility(result))
        tmp3=(BYTE)result;
    };

    return ( max(tmp1, max(tmp2,tmp3)) );
  }
}


//void CheckNewCharClassScores(classType charClass, CHARSKILLDATA &scores)
void CheckNewCharClassScores(const CLASS_ID& classID, CHARSKILLDATA &scores)
{
/*
  if (globalData.version < _VERSION_0870_)
  {
    // old method
    if (isFighter(charClass))
    {
       if (scores.str < 9) scores.str = 9;
       if (scores.con < 7) scores.con = 7;
    }
    if (isCleric(charClass))
    {         
       if (scores.wis < 9) scores.wis = 9;
    }
    if (isRanger(charClass))
    {
       if (scores.str < 13) scores.str = 13;
       if (scores.intel < 13) scores.intel = 13;
       if (scores.wis < 14) scores.wis = 14;
       if (scores.con < 14) scores.con = 14;
    }
    if (isPaladin(charClass))
    {         
       if (scores.str < 12) scores.str = 12;
       if (scores.con < 9) scores.con = 9;
       if (scores.wis < 13) scores.wis = 13;
       if (scores.cha < 17) scores.cha = 17;
    }
    if (isMagicUser(charClass))
    {
       if (scores.intel < 9) scores.intel = 9;
       if (scores.dex < 6) scores.dex = 6;
    }
    if (isThief(charClass))
    {
       if (scores.dex < 9) scores.dex = 9;
    }
    if (isDruid(charClass))
    {
       if (scores.wis < 12) scores.wis = 12;
       if (scores.cha < 15) scores.cha = 15;
    }
  }
  else
*/
  {
    // new method, based on config files
    /*
    int minstr = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.strKey));
    int minint = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.intelKey));
    int minwis = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.wisKey));
    int mindex = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.dexKey));
    int mincon = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.conKey));
    int mincha = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.chaKey));

    int maxstr = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.strKey));
    int maxint = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.intelKey));
    int maxwis = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.wisKey));
    int maxdex = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.dexKey));
    int maxcon = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.conKey));
    int maxcha = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.chaKey));
    */
    const CLASS_DATA *pClass;
    pClass = classData.PeekClass(classID);
    //int minstr = ABILITYMINBASE(pClass->GetAbilityLimits(scores.strKey));
    //int minint = ABILITYMINBASE(pClass->GetAbilityLimits(scores.intelKey));
    //int minwis = ABILITYMINBASE(pClass->GetAbilityLimits(scores.wisKey));
    //int mindex = ABILITYMINBASE(pClass->GetAbilityLimits(scores.dexKey));
    //int mincon = ABILITYMINBASE(pClass->GetAbilityLimits(scores.conKey));
    //int mincha = ABILITYMINBASE(pClass->GetAbilityLimits(scores.chaKey));

    
    //int maxstr = ABILITYMAXBASE(pClass->GetAbilityLimits(scores.strKey));
    //int maxint = ABILITYMAXBASE(pClass->GetAbilityLimits(scores.intelKey));
    //int maxwis = ABILITYMAXBASE(pClass->GetAbilityLimits(scores.wisKey));
    //int maxdex = ABILITYMAXBASE(pClass->GetAbilityLimits(scores.dexKey));
    //int maxcon = ABILITYMAXBASE(pClass->GetAbilityLimits(scores.conKey));
    //int maxcha = ABILITYMAXBASE(pClass->GetAbilityLimits(scores.chaKey));

    ABILITY_ID abilityID;
    ABILITYLIMITS abilityLimits;
    
    if (pClass != NULL)
    {
      abilityID = Ability_Strength;
      abilityLimits = pClass->GetAbilityLimits(abilityID);
      int maxstr = ABILITYMAXBASE(abilityLimits);
      int minstr = ABILITYMINBASE(abilityLimits);

      abilityID = Ability_Intelligence;
      abilityLimits = pClass->GetAbilityLimits(abilityID);
      int maxint = ABILITYMAXBASE(abilityLimits);
      int minint = ABILITYMINBASE(abilityLimits);

      abilityID = Ability_Wisdom;
      abilityLimits = pClass->GetAbilityLimits(abilityID);
      int maxwis = ABILITYMAXBASE(abilityLimits);
      int minwis = ABILITYMINBASE(abilityLimits);

      abilityID = Ability_Dexterity;
      abilityLimits = pClass->GetAbilityLimits(abilityID);
      int maxdex = ABILITYMAXBASE(abilityLimits);
      int mindex = ABILITYMINBASE(abilityLimits);

      abilityID = Ability_Constitution;
      abilityLimits = pClass->GetAbilityLimits(abilityID);
      int maxcon = ABILITYMAXBASE(abilityLimits);
      int mincon = ABILITYMINBASE(abilityLimits);

      abilityID = Ability_Charisma;
      abilityLimits = pClass->GetAbilityLimits(abilityID);
      int maxcha = ABILITYMAXBASE(abilityLimits);
      int mincha = ABILITYMINBASE(abilityLimits);
    

      if (scores.m_str < minstr) scores.m_str = minstr;
      if (scores.m_int < minint) scores.m_int = minint;
      if (scores.m_wis < minwis) scores.m_wis = minwis;
      if (scores.m_dex < mindex) scores.m_dex = mindex;
      if (scores.m_con < mincon) scores.m_con = mincon;
      if (scores.m_cha < mincha) scores.m_cha = mincha;

      if (scores.m_str > maxstr) scores.m_str = maxstr;
      if (scores.m_int > maxint) scores.m_int = maxint;
      if (scores.m_wis > maxwis) scores.m_wis = maxwis;
      if (scores.m_dex > maxdex) scores.m_dex = maxdex;
      if (scores.m_con > maxcon) scores.m_con = maxcon;
      if (scores.m_cha > maxcha) scores.m_cha = maxcha;
    };
  }
}

//void CheckNewCharRaceScores(raceType race, CHARSKILLDATA &scores)
void CheckNewCharRaceScores(const RACE_ID& raceID, CHARSKILLDATA &scores)
{
  if (globalData.version < _VERSION_0870_)
  {
/*
    // old method
	  switch (race) 
    {
	  case Elf:
		  scores.dex++;
		  scores.con--;
		  break;
	  case Dwarf:
		  scores.con++;
		  scores.cha--;
		  break;
	  case Gnome:
		  scores.intel++;
		  scores.wis--;
		  break;
	  case Halfling:
		  scores.dex++;
		  scores.str--;
		  scores.strMod = 0;
		  break;
	  default:
      // no changes
		  break;
	  }

	  switch (race) 
    {
	  case Elf:
		  if (scores.dex < 6) scores.dex = 6;
		  if (scores.con < 7) scores.con = 7;
		  if (scores.intel < 8)	scores.intel = 8;
		  if (scores.cha < 8) scores.cha = 8;
		  break;
	  case Dwarf:
		  if (scores.str < 8)	scores.str = 8;
		  if (scores.con < 11) scores.con = 11;
		  if (scores.dex > 17)	scores.dex = 17;
		  if (scores.cha > 17) scores.cha = 17;
		  break;
	  case Gnome:
		  if (scores.str < 6) scores.str = 6;
		  if (scores.con < 8)	scores.con = 8;
		  if (scores.intel < 6)	scores.intel = 6;
		  break;
	  case HalfElf:
		  if (scores.dex < 6) scores.dex = 6;
		  if (scores.con < 6)	scores.con = 6;
		  if (scores.intel < 4)	scores.intel = 4;
		  break;
	  case Halfling:
		  if (scores.str < 7)	scores.str = 7;
      if (scores.dex < 7) scores.dex = 7;
		  if (scores.con < 10) scores.con = 10;
		  if (scores.intel < 6) scores.intel = 6;
		  if (scores.wis > 17) scores.wis = 17;
		  break;
	  default: // human
      // no changes
		  break;
	  }
*/
  }

  else
  {
    // new method, based on config files
    const RACE_DATA *pRace;
    pRace = raceData.GetRace(raceID);
/*
    int minstr = raceData.GetMinAbility(race, scores.strKey);
    int minint = raceData.GetMinAbility(race, scores.intelKey);
    int minwis = raceData.GetMinAbility(race, scores.wisKey);
    int mindex = raceData.GetMinAbility(race, scores.dexKey);
    int mincon = raceData.GetMinAbility(race, scores.conKey);
    int mincha = raceData.GetMinAbility(race, scores.chaKey);

    int maxstr = raceData.GetMaxAbility(race, scores.strKey);
    int maxint = raceData.GetMaxAbility(race, scores.intelKey);
    int maxwis = raceData.GetMaxAbility(race, scores.wisKey);
    int maxdex = raceData.GetMaxAbility(race, scores.dexKey);
    int maxcon = raceData.GetMaxAbility(race, scores.conKey);
    int maxcha = raceData.GetMaxAbility(race, scores.chaKey);
*/
    int minstr = pRace->GetMinAbility(Ability_Strength);
    int minint = pRace->GetMinAbility(Ability_Intelligence);
    int minwis = pRace->GetMinAbility(Ability_Wisdom);
    int mindex = pRace->GetMinAbility(Ability_Dexterity);
    int mincon = pRace->GetMinAbility(Ability_Constitution);
    int mincha = pRace->GetMinAbility(Ability_Charisma);

    int maxstr = pRace->GetMaxAbility(Ability_Strength);
    int maxint = pRace->GetMaxAbility(Ability_Intelligence);
    int maxwis = pRace->GetMaxAbility(Ability_Wisdom);
    int maxdex = pRace->GetMaxAbility(Ability_Dexterity);
    int maxcon = pRace->GetMaxAbility(Ability_Constitution);
    int maxcha = pRace->GetMaxAbility(Ability_Charisma);

    
    
    if (scores.m_str < minstr) scores.m_str = minstr;
    if (scores.m_int < minint) scores.m_int = minint;
    if (scores.m_wis < minwis) scores.m_wis = minwis;
    if (scores.m_dex < mindex) scores.m_dex = mindex;
    if (scores.m_con < mincon) scores.m_con = mincon;
    if (scores.m_cha < mincha) scores.m_cha = mincha;

    if (scores.m_str > maxstr) scores.m_str = maxstr;
    if (scores.m_int > maxint) scores.m_int = maxint;
    if (scores.m_wis > maxwis) scores.m_wis = maxwis;
    if (scores.m_dex > maxdex) scores.m_dex = maxdex;
    if (scores.m_con > maxcon) scores.m_con = maxcon;
    if (scores.m_cha > maxcha) scores.m_cha = maxcha;
  }
}

//int determineCharStartAge(raceType race)
int determineCharStartAge(const RACE_ID& race)
{
  int age = raceData.GetStartAge(race);
  if ((START_AGE > 0)&&(age < START_AGE)) 
    age = START_AGE;
  return age;
}

//int determineCharMaxAge(raceType race)
int determineCharMaxAge(const RACE_ID& raceID)
{
  if (globalData.version < _VERSION_0870_)
  {
/*
	  switch (race) 
    {
	  case Elf:   return 1200;
	  case Dwarf: return 350;
	  case Gnome:	return 350;
	  case HalfElf:	return 160;
	  case Halfling: return 150;
	  default: return 100;
    }
*/
    return 100;
  }
  else
  {
    //return (raceData.GetMaxAge(race));
    return (raceData.GetMaxAge(raceID));
  }
}

int DetermineNormalEncumbrance(int str, int strMod)
{
  int result=0;
  int BaseAllowance = 350;
  // normal is based on str 8-11 able to carry 350gp
	switch (str)
	{
  case 1:
	case 2:
	case 3: // -350
		result = BaseAllowance-350;
		break;
	case 4:
	case 5: // -250
		result = BaseAllowance-250;
		break;
	case 6:
	case 7: // -150
		result = BaseAllowance-150;
		break;
	case 8:
	case 9: // normal
	case 10:
	case 11:
		result = BaseAllowance;
		break;
	case 12:
	case 13:
		result = BaseAllowance+100;
		break;
	case 14:
	case 15:
		result = BaseAllowance+200;
		break;
	case 16:
		result = BaseAllowance+350;
		break;
	case 17:
		result = BaseAllowance+500;
		break;
	case 18:
    {
    BYTE mod = strMod;
    if (mod == 0)
      result = BaseAllowance+750;
		else if (mod < 51)
			result = BaseAllowance+1000;
		else if (mod < 76)
			result = BaseAllowance+1250;
		else if (mod < 91)
			result = BaseAllowance+1500;
		else if (mod < 100)
			result = BaseAllowance+2000;
    else
		  result = BaseAllowance+3000;
    }
		break;
	case 19:  // cases 19-25 by manikus 4-5-11
		result = BaseAllowance+4500;
		break;
	case 20:
		result = BaseAllowance+5000;
		break;
	case 21:
		result = BaseAllowance+6000;
		break;
	case 22:
		result = BaseAllowance+7500;
		break;
	case 23:
		result = BaseAllowance+9000;
		break;
	case 24:
		result = BaseAllowance+12000;
		break;
	case 25:
		result = BaseAllowance+15000;
		break;
	default:
		result = BaseAllowance;
	}
  if (result <= 0) result = 1;
  return result;
}


struct SPLVLENTRY
{
  SPLVLENTRY() { ClLvl=MuLvl=0; }
  SPLVLENTRY(BYTE cl, BYTE mu) { ClLvl=cl;MuLvl=mu; }
  BYTE ClLvl;
  BYTE MuLvl;
};

struct SPBONUSENTRY
{
  SPBONUSENTRY() { Lvl=Qty=0; }
  SPBONUSENTRY(BYTE qty, BYTE lvl) { Qty=qty;Lvl=lvl; }
  BYTE Lvl;
  BYTE Qty;
};


/*
enum classType { Fighter, Cleric, Ranger, Paladin, MagicUser, Thief, Druid,
						     C_F, C_F_MU, C_R, C_MU, C_T, F_MU, F_T, 
                 F_MU_T, MU_T, F_D, D_MU, F_MU_D };
*/

/*
// class flags
const int MagicUserFlag = 1;
const int ClericFlag    = 2;
const int ThiefFlag     = 4;
const int FighterFlag   = 8;
const int PaladinFlag   = 16;
const int RangerFlag    = 32;
const int DruidFlag     = 64;
const int AllClasses    = 255;
const int NumClassFlags = 7;
#endif

int SpellClassToCharClass(WORD SpellClassFlag);

inline BYTE IndexToFlag(BYTE index)
{
  switch (index)
  {
  case 0: return MagicUserFlag;
  case 1: return ClericFlag;
  case 2: return ThiefFlag;
  case 3: return FighterFlag;
  case 4: return PaladinFlag;
  case 5: return RangerFlag;
  case 6: return DruidFlag;
  case 7: return AllClasses;
  }
  ASS ERT(FALSE);
  return 0;
}

inline BYTE FlagToIndex(BYTE flag)
{ 
  switch (flag)
  {
  case MagicUserFlag: return 0;
  case ClericFlag:    return 1;
  case ThiefFlag:     return 2;
  case FighterFlag:   return 3;
  case PaladinFlag:   return 4;
  case RangerFlag:    return 5;
  case DruidFlag:     return 6;
  case AllClasses:    return 7;
  }
  ASS ERT(FALSE);
  return 0;
}
extern char *ClassFlagText[8];
*/

// max spells by level tables

#ifdef UAFEDITOR

#define DEFCLLVL(a)   SPLVLENTRY(a,0)
#define DEFMULVL(a)   SPLVLENTRY(0,a)
#define DEFSPLVL(a,b) SPLVLENTRY(a,b)

#define DEFCLERICROW(a,b,c,d,e,f,g) { DEFCLLVL(a),DEFCLLVL(b),DEFCLLVL(c),DEFCLLVL(d),DEFCLLVL(e),DEFCLLVL(f),DEFCLLVL(g),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFDRUIDROW(a,b,c,d,e,f,g) { DEFCLLVL(a),DEFCLLVL(b),DEFCLLVL(c),DEFCLLVL(d),DEFCLLVL(e),DEFCLLVL(f),DEFCLLVL(g),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFPALADINROW(a,b,c,d) { DEFCLLVL(a),DEFCLLVL(b),DEFCLLVL(c),DEFCLLVL(d),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFRANGERROW(a,b,c, d,e) { DEFSPLVL(a,d),DEFSPLVL(b,e),DEFSPLVL(c,0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFMUROW(a,b,c,d,e,f,g,h,i) { DEFMULVL(a),DEFMULVL(b),DEFMULVL(c),DEFMULVL(d),DEFMULVL(e),DEFMULVL(f),DEFMULVL(g),DEFMULVL(h),DEFMULVL(i) }
#define DEFFIGHTERROW { DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0) }
#define DEFTHIEFROW { DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0) }

void CopySpellLevels(SPLVLENTRY *splvlEntry, BYTE *dst, const char *school)
{
  int i;
  if (school[0] == 'M')  // Magic User
  {
    for (i=0; i<HIGHEST_CHARACTER_LEVEL*MAX_SPELL_LEVEL; i++)
    {
      dst[i] = splvlEntry[i].MuLvl;
    };
  }
  else // assume cleric
  {
    for (i=0; i<HIGHEST_CHARACTER_LEVEL*MAX_SPELL_LEVEL; i++)
    {
      dst[i] = splvlEntry[i].ClLvl;
    };
  };
}


// before modifications, the maximum spells per spell lvl 
// knowable for each character level.
// 
SPLVLENTRY BaseFighterSpellLvls[HIGHEST_CHARACTER_LEVEL][MAX_SPELL_LEVEL] = 
{
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW, // manikus 7-16-09
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW, //
  DEFFIGHTERROW
};
SPLVLENTRY BaseThiefSpellLvls[HIGHEST_CHARACTER_LEVEL][MAX_SPELL_LEVEL] = 
{
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW, // manikus 7-16-09
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW, //
  DEFTHIEFROW
};

SPLVLENTRY BaseClericSpellLvls[HIGHEST_CHARACTER_LEVEL][MAX_SPELL_LEVEL] = 
{
  DEFCLERICROW(1,0,0,0,0,0,0),  // CharLevel 1
  DEFCLERICROW(2,0,0,0,0,0,0),
  DEFCLERICROW(2,1,0,0,0,0,0),

  DEFCLERICROW(3,2,0,0,0,0,0),  // CharLevel 4
  DEFCLERICROW(3,3,1,0,0,0,0),
  DEFCLERICROW(3,3,2,0,0,0,0),

  DEFCLERICROW(3,3,2,1,0,0,0),  // CharLevel 7
  DEFCLERICROW(3,3,3,2,0,0,0),
  DEFCLERICROW(4,4,3,2,1,0,0),
  
  DEFCLERICROW(4,4,3,3,2,0,0),  // CharLevel 10
  DEFCLERICROW(5,4,4,3,2,1,0),
  DEFCLERICROW(6,5,5,3,2,2,0),
  
  DEFCLERICROW(6,6,6,4,2,2,0),
  DEFCLERICROW(6,6,6,5,3,2,0),
  DEFCLERICROW(7,7,7,5,4,2,0),
  
  DEFCLERICROW(7,7,7,6,5,3,1),
  DEFCLERICROW(7,7,7,7,5,3,2),
  DEFCLERICROW(8,8,8,7,6,4,1),
  
  DEFCLERICROW(9,9,8,7,6,4,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,9,8,6,2),
  
  DEFCLERICROW(9,9,9,9,9,6,3),
  DEFCLERICROW(9,9,9,9,9,7,3),
  DEFCLERICROW(9,9,9,9,9,8,3),
  
  DEFCLERICROW(9,9,9,9,9,8,4),
  DEFCLERICROW(9,9,9,9,9,9,4),
  DEFCLERICROW(9,9,9,9,9,9,5),
  
  DEFCLERICROW(9,9,9,9,9,9,6),
  DEFCLERICROW(9,9,9,9,9,9,7),
  DEFCLERICROW(9,9,9,9,9,9,8), // manikus 7-16-09

  DEFCLERICROW(9,9,9,9,9,9,9), //
  DEFCLERICROW(10,10,10,9,9,9,9), //
  DEFCLERICROW(10,10,10,10,10,9,9), //

  DEFCLERICROW(10,10,10,10,10,10,9), //
  DEFCLERICROW(10,10,10,10,10,10,10), //
  DEFCLERICROW(11,11,11,10,10,10,10), //

  DEFCLERICROW(11,11,11,11,11,10,10), //
  DEFCLERICROW(11,11,11,11,11,11,10), //
  DEFCLERICROW(11,11,11,11,11,11,11), //

  DEFCLERICROW(12,12,12,11,11,11,11) //
};

void CopyClericSpellLevels(BYTE *dst, const char* school)
{
  CopySpellLevels(BaseClericSpellLvls[0], dst, school);
}

SPLVLENTRY BaseDruidSpellLvls[HIGHEST_CHARACTER_LEVEL][MAX_SPELL_LEVEL] = 
{
  DEFDRUIDROW(2,0,0,0,0,0,0),  // CharLevel 1
  DEFDRUIDROW(2,1,0,0,0,0,0),
  DEFDRUIDROW(3,2,1,0,0,0,0),

  DEFDRUIDROW(4,2,2,0,0,0,0),  // CharLevel 4
  DEFDRUIDROW(4,3,2,0,0,0,0),
  DEFDRUIDROW(4,3,2,1,0,0,0),

  DEFDRUIDROW(4,4,3,1,0,0,0),  // CharLevel 7
  DEFDRUIDROW(4,4,3,2,0,0,0),
  DEFDRUIDROW(5,4,3,2,1,0,0),
  
  DEFDRUIDROW(5,4,3,3,2,0,0),  // CharLevel 10
  DEFDRUIDROW(5,5,4,3,2,1,0),
  DEFDRUIDROW(5,5,4,4,3,2,1),
  
  DEFDRUIDROW(6,5,5,5,4,3,2),  // CharLevel 13
  DEFDRUIDROW(6,6,6,6,5,4,3),
  DEFDRUIDROW(6,6,6,6,6,6,6),
  
  DEFDRUIDROW(6,6,6,6,6,6,6),  // CharLevel 16
  DEFDRUIDROW(6,6,6,6,6,6,6),
  DEFDRUIDROW(6,6,6,6,6,6,6),
  
  DEFDRUIDROW(6,6,6,6,6,6,6),  // CharLevel 19
  DEFDRUIDROW(6,6,6,6,6,6,6),
  DEFDRUIDROW(6,6,6,6,6,6,6),
  
  DEFDRUIDROW(6,6,6,6,6,6,6),  // CharLevel 22
  DEFDRUIDROW(6,6,6,6,6,6,6) // manikus 7-16-09
//  DEFDRUIDROW(6,6,6,6,6,6,6),
  
  // DEFDRUIDROW(6,6,6,6,6,6,6),
  // DEFDRUIDROW(6,6,6,6,6,6,6),
  // DEFDRUIDROW(6,6,6,6,6,6,6),
  
  // DEFDRUIDROW(6,6,6,6,6,6,6),
  // DEFDRUIDROW(6,6,6,6,6,6,6),
  // DEFDRUIDROW(6,6,6,6,6,6,6),
};

void CopyDruidSpellLevels(BYTE *dst, const char* school)
{
  CopySpellLevels(BaseDruidSpellLvls[0], dst, school);
}



SPLVLENTRY BasePaladinSpellLvls[HIGHEST_CHARACTER_LEVEL][MAX_SPELL_LEVEL] = 
{
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(1,0,0,0),
  
  DEFPALADINROW(2,0,0,0),
  DEFPALADINROW(2,1,0,0),
  DEFPALADINROW(2,2,0,0),
  
  DEFPALADINROW(2,2,1,0),
  DEFPALADINROW(3,2,1,0),
  DEFPALADINROW(3,2,1,1),
  
  DEFPALADINROW(3,3,1,1),
  DEFPALADINROW(3,3,2,1),
  DEFPALADINROW(3,3,3,1),
  
  DEFPALADINROW(3,3,3,2),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3), // manikus 7-16-09

  DEFPALADINROW(3,3,3,3), //
  DEFPALADINROW(3,3,3,3), //
  DEFPALADINROW(3,3,3,3), //
  
  DEFPALADINROW(3,3,3,3), //
  DEFPALADINROW(3,3,3,3), //
  DEFPALADINROW(3,3,3,3), //
  
  DEFPALADINROW(3,3,3,3), //
  DEFPALADINROW(3,3,3,3), //
  DEFPALADINROW(3,3,3,3), //
  
  DEFPALADINROW(3,3,3,3) //
};

void CopyPaladinSpellLevels(BYTE *dst, const char* school)
{
  CopySpellLevels(BasePaladinSpellLvls[0], dst, school);
}



SPLVLENTRY BaseRangerSpellLvls[HIGHEST_CHARACTER_LEVEL][MAX_SPELL_LEVEL] = 
//fixed by manikus
{
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(1,0,0, 0,0),
  DEFRANGERROW(1,0,0, 1,0),
  
  DEFRANGERROW(2,0,0, 1,0),
  DEFRANGERROW(2,0,0, 2,0),
  DEFRANGERROW(2,1,0, 2,0),
  
  DEFRANGERROW(2,1,0, 2,1),
  DEFRANGERROW(2,2,0, 2,1),
  DEFRANGERROW(2,2,0, 2,2),
  
  DEFRANGERROW(2,2,1, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),
  DEFRANGERROW(2,2,2, 2,2),

  DEFRANGERROW(2,2,2, 2,2), // manikus 7-16-09
  DEFRANGERROW(2,2,2, 2,2), //
  DEFRANGERROW(2,2,2, 2,2), //

  DEFRANGERROW(2,2,2, 2,2), //
  DEFRANGERROW(2,2,2, 2,2), //
  DEFRANGERROW(2,2,2, 2,2), //

  DEFRANGERROW(2,2,2, 2,2), //
  DEFRANGERROW(2,2,2, 2,2), //
  DEFRANGERROW(2,2,2, 2,2), //

  DEFRANGERROW(2,2,2, 2,2) //

};

void CopyRangerSpellLevels(BYTE *dst, const char* school)
{
  CopySpellLevels(BaseRangerSpellLvls[0], dst, school);
}




SPLVLENTRY BaseMuSpellLvls[HIGHEST_CHARACTER_LEVEL][MAX_SPELL_LEVEL] = 
{
  DEFMUROW(1,0,0,0,0,0,0,0,0),
  DEFMUROW(2,0,0,0,0,0,0,0,0),
  DEFMUROW(2,1,0,0,0,0,0,0,0),
  
  DEFMUROW(3,2,0,0,0,0,0,0,0),
  DEFMUROW(4,2,1,0,0,0,0,0,0),
  DEFMUROW(4,2,2,0,0,0,0,0,0),
  
  DEFMUROW(4,3,2,1,0,0,0,0,0),
  DEFMUROW(4,3,3,2,0,0,0,0,0),
  DEFMUROW(4,3,3,2,1,0,0,0,0),
  
  DEFMUROW(4,4,3,2,2,0,0,0,0),
  DEFMUROW(4,4,4,3,3,0,0,0,0),
  DEFMUROW(4,4,4,4,4,1,0,0,0),
  
  DEFMUROW(5,5,5,4,4,2,0,0,0),
  DEFMUROW(5,5,5,4,4,2,1,0,0),
  DEFMUROW(5,5,5,5,5,2,1,0,0),
  
  DEFMUROW(5,5,5,5,5,3,2,1,0),
  DEFMUROW(5,5,5,5,5,3,3,2,0),
  DEFMUROW(5,5,5,5,5,3,3,2,1),
  
  DEFMUROW(5,5,5,5,5,3,3,3,1),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,4,4,2),
  
  DEFMUROW(5,5,5,5,5,5,4,4,3),
  DEFMUROW(5,5,5,5,5,5,5,5,3),
  DEFMUROW(5,5,5,5,5,5,5,5,4),
  
  DEFMUROW(5,5,5,5,5,5,5,5,5),
  DEFMUROW(6,6,6,6,5,5,5,5,5),
  DEFMUROW(6,6,6,6,6,6,6,5,5),
  
  DEFMUROW(6,6,6,6,6,6,6,6,6),
  DEFMUROW(7,7,7,7,6,6,6,6,6),
  DEFMUROW(7,7,7,7,7,7,7,6,6), // manikus 7-16-09

  DEFMUROW(7,7,7,7,7,7,7,7,7), //
  DEFMUROW(8,8,8,8,7,7,7,7,7), //
  DEFMUROW(8,8,8,8,8,8,8,7,7), //

  DEFMUROW(8,8,8,8,8,8,8,8,8), //
  DEFMUROW(9,9,9,9,8,8,8,8,8), //
  DEFMUROW(9,9,9,9,9,9,9,8,8), //

  DEFMUROW(9,9,9,9,9,9,9,9,9), //
  DEFMUROW(10,10,10,10,9,9,9,9,9), //
  DEFMUROW(10,10,10,10,10,10,10,9,9), //

  DEFMUROW(10,10,10,10,10,10,10,10,10), //
};

void CopyMuSpellLevels(BYTE *dst, const char* school)
{
  CopySpellLevels(BaseMuSpellLvls[0], dst, school);
}



#endif  UAFEDITOR



#ifdef UAFEDITOR
// based on char intelligence, the maximum
// spells of each level that can be known.
// 0 = none, 255 = unlimited
//
// for example, a mu with intel 9 can only
// know a max of 6 first lvl spells, 6 2nd lvl, etc
// and MaxMuSpellsPerSpellLvl[9-1] = 6
//
BYTE MaxFighterSpellsForEachSpellLvlByPrime[19] =
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


BYTE MaxThiefSpellsForEachSpellLvlByPrime[19] =
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};



BYTE MaxClericSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};



BYTE MaxDruidSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};



BYTE MaxPaladinSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};



BYTE MaxRangerSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};




BYTE MaxMuSpellsForEachSpellLvlByPrime[19] =
{
  0,0,0,0,0,0,0,0,6,7,7,7,9,9,11,11,14,18,255
};

#endif

#ifdef UAFEDITOR
BYTE maxSpells[MAX_BASE_CLASSES][MAX_SPELL_SCHOOL][19] =
{
  // Fighter
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  // Cleric
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  // Ranger
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  // Paladin
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  // Magic-user
  0,0,0,0,0,0,0,0,6,7,7,7,9,9,11,11,14,18,255,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  // Thief
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  // Druid
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};
#endif

/*
// the maximum spell level allowed by the
// character's prime requisite score
// 
BYTE MaxFighterSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
BYTE MaxThiefSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
BYTE MaxClericSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7
};
BYTE MaxDruidSpellLvlByPrime[19] = 
//fixed by manikus
{
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7
};
BYTE MaxPaladinSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7
};
BYTE MaxRangerSpellLvlByPrime[19] = 
//fixed by manikus
{
  0,0,0,0,0,0,0,0,2,2,2,2,2,3,2,2,2,2,3
};
BYTE MaxMuSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,4,5,5,6,6,7,7,8,8,9,9
};
*/


#ifdef UAFEDITOR
BYTE MaxSpellLevel[MAX_BASE_CLASSES][MAX_SPELL_SCHOOL][19] = 
{
  // Fighter
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  // Cleric
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7,
  // Ranger
  0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,
  0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3, //Cleric school wisdom 1-19
  // Paladin
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4, //Cleric school wisdom 1-19
  // Magic-user
  0,0,0,0,0,0,0,0,4,5,5,6,6,7,7,8,8,9,9,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  // Thief
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  // Druid
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7
};
#endif
// the bonus spells allowed per spell lvl by the
// character's prime requisite score
// 

#ifdef UAFEDITOR

#define DEFSPELLBONUS(q,l)   SPBONUSENTRY(q,l)
#define DEFSPELLBONUSNONE    SPBONUSENTRY(0,0)
#endif

/*

SPBONUSENTRY BonusFighterSpellsByPrime[19] =
{
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE
};
SPBONUSENTRY BonusThiefSpellsByPrime[19] =
{
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE
};
*/

#ifdef UAFEDITOR
SPBONUSENTRY BonusClericSpellsByPrime[19] =
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,3),
  DEFSPELLBONUS(1,4),
  DEFSPELLBONUS(1,4)
};

BYTE GetClericBonusLevel(int prime) {return BonusClericSpellsByPrime[prime-1].Lvl;};
BYTE GetClericBonusNum(int prime) {return BonusClericSpellsByPrime[prime-1].Qty;};

#endif
/*
SPBONUSENTRY BonusDruidSpellsByPrime[19] =
//fixed by manikus
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0)
};
SPBONUSENTRY BonusPaladinSpellsByPrime[19] =
//fixed by manikus
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
};
SPBONUSENTRY BonusRangerSpellsByPrime[19] =
//fixed by manikus
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0)
};
SPBONUSENTRY BonusMuSpellsByPrime[19] =
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0)
};

SPBONUSENTRY *BonusSpellsByPrime[MAX_BASE_CLASSES] =
{ // Someday we will remove the tables above and put them in here.
  BonusFighterSpellsByPrime,
  BonusClericSpellsByPrime,
  BonusRangerSpellsByPrime, 
  BonusPaladinSpellsByPrime, 
  BonusMuSpellsByPrime, 
  BonusThiefSpellsByPrime, 
  BonusDruidSpellsByPrime
};
*/
///////////////////////////////////////////////////////////////////////
//
// Global Spell Functions
//
///////////////////////////////////////////////////////////////////////
/*
BOOL NumClassBits(WORD ClassFlag)
{
  int count=0;
  for (int i=0;i<NumBaseClass;i++)
  {
    if (ClassFlag & (0x0001 << i)) 
      count++;
  }
  return count;
}
*/
/*
BYTE GetMaxSpellsForSpellLvlPrime(WORD CharClassFlag, BYTE prime)
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );

  if (prime > 19) prime = 19;
  if (prime < 1) prime = 1;
  prime--;

  switch (CharClassFlag)
  {
  case ClericFlag:    return MaxClericSpellsForEachSpellLvlByPrime[prime];
  case MagicUserFlag: return MaxMuSpellsForEachSpellLvlByPrime[prime];
  case ThiefFlag:     return MaxThiefSpellsForEachSpellLvlByPrime[prime];
  case FighterFlag:   return MaxFighterSpellsForEachSpellLvlByPrime[prime];
  case PaladinFlag:   return MaxPaladinSpellsForEachSpellLvlByPrime[prime];
  case RangerFlag:    return MaxRangerSpellsForEachSpellLvlByPrime[prime];
  case DruidFlag:     return MaxDruidSpellsForEachSpellLvlByPrime[prime];
  default: ASS ERT(FALSE);
  }
  return 0;
}
*/

/*
// Added Dec 2010 - SHould eventually replace GetMaxSpellsForSpellLvlPrime
//BYTE MaxSpellsForSpellLvlPrime(unsigned int charBaseClass, unsigned int prime, unsigned int spellSchool)
BYTE MaxSpellCount_BaseclassSchoolPrime(const BASE_CLASS_DATA *pBaseclass,
                                        unsigned int prime,
                                        const SCHOOL_ID& schoolID) //yes
{

  //if (prime > 19) prime = 19;
  //if (prime < 1) prime = 1;
  //prime--;

  if (prime > HIGHEST_CHARACTER_PRIME) prime = HIGHEST_CHARACTER_PRIME;
  if (prime < 1) prime = 1;
  //if (spellSchool >= MAX_SPELL_SCHOOL) spellSchool = 0;

  //return maxSpells[charBaseClass][spellSchool][prime];
  const CASTING_INFO *pCastingInfo;
  if (pBaseclass == NULL) return 0;
  pCastingInfo = pBaseclass->PeekCastingInfo(schoolID);
  if (pCastingInfo == NULL) return 0;
  return pCastingInfo->m_maxSpells[prime-1];
  NotImplemented(0xd321a8, false); // Need bonus spells.

};
 */ 
/*
BYTE GetMaxSpellLvlByPrime(WORD CharClassFlag, BYTE prime)
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );

  if (prime > 19) prime = 19;
  if (prime < 1) prime = 1;
  prime--;
  switch (CharClassFlag)
  {
  case ClericFlag:    return MaxClericSpellLvlByPrime[prime];
  case MagicUserFlag: return MaxMuSpellLvlByPrime[prime];
  case ThiefFlag:     return MaxThiefSpellLvlByPrime[prime];
  case FighterFlag:   return MaxFighterSpellLvlByPrime[prime];
  case PaladinFlag:   return MaxPaladinSpellLvlByPrime[prime];
  case RangerFlag:    return MaxRangerSpellLvlByPrime[prime];
  case DruidFlag:     return MaxDruidSpellLvlByPrime[prime];
  default: ASS ERT(FALSE);
  }
  return 0;
}
*/

// Added Dec 2010 - Should eventually replace GetMaxSpellLvlByPrime
//BYTE MaxSpellLvlByPrime(unsigned int charBaseClass, int prime, unsigned int spellSchool)
BYTE MaxSpellLevel_BaseclassSchoolPrime(const BASE_CLASS_DATA *pBaseclass, //Yes
                                        int   prime, 
                                        const SCHOOL_ID& schoolID) //Yes
{
  //if (prime > 19) prime = 19;
  //if (prime < 1) prime = 1;
  //if (charBaseClass >= MAX_BASE_CLASSES) charBaseClass = 0;
  //if (spellSchool >= MAX_SPELL_SCHOOL) spellSchool = 0;
  //prime--;
  //return MaxSpellLevel[charBaseClass][spellSchool][prime];
  if (prime > HIGHEST_CHARACTER_PRIME) prime = HIGHEST_CHARACTER_PRIME;
  if (prime < 1) prime = 1;
  const CASTING_INFO *pCastingInfo;
  if (pBaseclass == NULL) return 0;
  pCastingInfo = pBaseclass->PeekCastingInfo(schoolID);
  if (pCastingInfo == NULL) return 0;
  return pCastingInfo->m_maxSpellLevelsByPrime[prime-1];
}





/*
BYTE GetBaseSpellLvlBySpellClass( SPLVLENTRY data, WORD SpellClassFlag )
{
  ASSERT( NumClassBits(SpellClassFlag) == 1 );

  switch (SpellClassFlag)
  {
  case ClericFlag: return data.ClLvl;
  case MagicUserFlag: return data.MuLvl;
  default: ASS ERT(FALSE);
  }
  return 0;
}
*/
/*
// Added Dec 2010 - Should eventually replace GetBase SpellLvlBySpellClass
int BaseSpellLvlBySpellClass( SPLVLENTRY data, int spellSchool )
{

  switch (spellSchool)
  {
  case ClericSchool: return data.ClLvl;
  case MagicUserSchool: return data.MuLvl;
  default: ASS ERT(FALSE);
  }
  return 0;
}
*/







/*
BYTE GetBonusSpellQtyBySpellLevel( int prime, int SpellLevel, WORD CharClassFlag )
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );

  if (prime > 19) prime = 19;
  if (prime < 1) prime = 1;
  prime--;
  switch (CharClassFlag)
  {
  case ClericFlag:    { if (BonusClericSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case MagicUserFlag: { if (BonusMuSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case ThiefFlag:     { if (BonusThiefSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case FighterFlag:   { if (BonusFighterSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case PaladinFlag:   { if (BonusPaladinSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case RangerFlag:    { if (BonusRangerSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case DruidFlag:     { if (BonusDruidSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  default: ASS ERT(FALSE);
  }
  return 0;
}
*/
// Added Dec 2010- eventually should replace GetBonusSpellQtyBySpellLevel
//BYTE BonusSpellQtyBySpellLevel( int prime, int spellLevel, int charBaseClass )
BYTE BonusSpellQtyBySpellLevel( int prime, int spellLevel, const BASE_CLASS_DATA *pBaseclass )
{  // Fetch the cumulative bonus for a prime and spellLevel.
  int i, n, result;
  //if (prime > 19) prime = 19;
  //if (prime < 1) prime = 1;
  //prime--;
  if (prime > HIGHEST_CHARACTER_PRIME) prime = HIGHEST_CHARACTER_PRIME;
  if (prime < 1) prime = 1;

  //if (BonusSpellsByPrime[charBaseClass][prime].Lvl==spellLevel)
  result = 0;
  n = pBaseclass->m_bonusSpells.GetSize();
  for (i=0; i<n; i+=3)
  {
    //if ((pBaseclass->m_bonusSpells[i] <= prime) && (pBaseclass->m_bonusSpells[i+1] == spellLevel))
    if ((pBaseclass->m_bonusSpells[i] <= prime) && (pBaseclass->m_bonusSpells[i+1] == spellLevel))
    {
      //result += pBaseclass->m_bonusSpells[i+2];
      result += pBaseclass->m_bonusSpells[i+2];
    };
  };
  return result;
}

//BYTE GetBaseSpellLvl(WORD CharClassFlag, WORD CastingClassFlag, int CharLevel, int SpellLevel)
BYTE BaseSpellCount(const BASE_CLASS_DATA *pBaseclass, 
                    const SCHOOL_ID& schoolID,   // Yes
                    int CharLevel,
                    int SpellLevel)
{
  //ASSERT( NumClassBits(CharClassFlag) == 1 );
  //ASSERT( NumClassBits(CastingClassFlag) == 1 );

  if (CharLevel > HIGHEST_CHARACTER_LEVEL) CharLevel = HIGHEST_CHARACTER_LEVEL;
  if (CharLevel < 1) CharLevel = 1;
  //CharLevel--;

  if (SpellLevel > MAX_SPELL_LEVEL) SpellLevel = MAX_SPELL_LEVEL;
  if (SpellLevel < 1) SpellLevel = 1;
  //SpellLevel--;
/*
  switch (CharClassFlag)
  {
  case MagicUserFlag: return GetBaseSpellLvlBySpellClass( BaseMuSpellLvls[CharLevel][SpellLevel],      CastingClassFlag);
  case ClericFlag:    return GetBaseSpellLvlBySpellClass( BaseClericSpellLvls[CharLevel][SpellLevel],  CastingClassFlag);
  case ThiefFlag:     return GetBaseSpellLvlBySpellClass( BaseThiefSpellLvls[CharLevel][SpellLevel],   CastingClassFlag);
  case FighterFlag:   return GetBaseSpellLvlBySpellClass( BaseFighterSpellLvls[CharLevel][SpellLevel], CastingClassFlag);
  case PaladinFlag:   return GetBaseSpellLvlBySpellClass( BasePaladinSpellLvls[CharLevel][SpellLevel], CastingClassFlag);
  case RangerFlag:    return GetBaseSpellLvlBySpellClass( BaseRangerSpellLvls[CharLevel][SpellLevel],  CastingClassFlag);
  case DruidFlag:     return GetBaseSpellLvlBySpellClass( BaseDruidSpellLvls[CharLevel][SpellLevel],   CastingClassFlag);
  default: ASS ERT(FALSE);
  }
*/
  // Find the BASE_CLASS_DATA for this charBaseclass;
  {
    const CASTING_INFO *pCastingInfo;
    int baseBaseclassSpellLvl;
    if (pBaseclass == NULL) return 0;
    pCastingInfo = pBaseclass->PeekCastingInfo(schoolID);
    if (pCastingInfo == NULL) return 0;
    baseBaseclassSpellLvl = pCastingInfo->m_spellLimits[CharLevel-1][SpellLevel-1];
    return baseBaseclassSpellLvl;
  }
}

/*
// Added Dec 2010 - should eventually replace GetBaseSpellLvl
BYTE BaseSpellLvl(int charBaseClass, int spellSchool, int charLevel, int spellLevel)
{

  if (charLevel > HIGHEST_CHARACTER_LEVEL) charLevel = HIGHEST_CHARACTER_LEVEL;
  if (charLevel < 1) charLevel = 1;
  charLevel--;

  if (spellLevel > MAX_SPELL_LEVEL) spellLevel = MAX_SPELL_LEVEL;
  if (spellLevel < 1) spellLevel = 1;
  spellLevel--;

  switch (charBaseClass)
  {
  case MagicUser: return BaseSpellLvlBySpellClass( BaseMuSpellLvls[charLevel][spellLevel],      spellSchool);
  case Cleric:    return BaseSpellLvlBySpellClass( BaseClericSpellLvls[charLevel][spellLevel],  spellSchool);
  case Thief:     return BaseSpellLvlBySpellClass( BaseThiefSpellLvls[charLevel][spellLevel],   spellSchool);
  case Fighter:   return BaseSpellLvlBySpellClass( BaseFighterSpellLvls[charLevel][spellLevel], spellSchool);
  case Paladin:   return BaseSpellLvlBySpellClass( BasePaladinSpellLvls[charLevel][spellLevel], spellSchool);
  case Ranger:    return BaseSpellLvlBySpellClass( BaseRangerSpellLvls[charLevel][spellLevel],  spellSchool);
  case Druid:     return BaseSpellLvlBySpellClass( BaseDruidSpellLvls[charLevel][spellLevel],   spellSchool);
  default: ASS ERT(FALSE);
  }
  return 0;
}
*/


/*
BYTE GetBaseSpells(int CharLevel, int prime, WORD CharClassMask, WORD CastingClass, int SpellLevel )
{  
  int maxlvl = GetMaxSpellLvlByPrime(CharClassMask, prime);
  if (SpellLevel > maxlvl) return 0;
  BYTE result = 0;
  //for (int i=1;i<=prime;i++)
  //  result += GetBonusSpellQtyBySpellLevel(i, SpellLevel, CharClass);
  result += GetBaseSpellLvl(CharClassMask, CastingClass, CharLevel, SpellLevel);  
  int MaxPerSpellLvl = GetMaxSpellsForSpellLvlPrime(CharClassMask, prime);
  if (result > MaxPerSpellLvl) result = MaxPerSpellLvl;
  return result;
}
*/
//Added Dec 2010 - Should evenutally replace GetBaseSpells
//BYTE GetNumBaseSpells(int charLevel, int prime, int charBaseClass, int spellSchool, int spellLevel )
//
// Given a character with baseclass
//                        baseclass Level
//                        prime attribute score
// How many spells can he know in a particular spellschool and spell level?
// 
// It will be the smaller of:
//  1 -  The number allowed by his prime casting attribute (intelligence)
//       which limits the total number and the highest spell level.
//  2 -  The number allowed by baseclass Level
/*
BYTE GetNumBaseSpells(int baseclassLevel, 
                      int prime, 
                      const BASE_CLASS_DATA *pBaseclass, 
                      const SCHOOL_ID& schoolID,  //Yes
                      int   spellLevel)
{  
  //int maxlvl = MaxSpellLvlByPrime(charBaseClass, prime, spellSchool);
  int maxSpellLevel = MaxSpellLevel_BaseclassSchoolPrime(pBaseclass, prime, schoolID);
  if (spellLevel > maxSpellLevel) return 0;
  BYTE result;
  //result = BaseSpellCount(charBaseClass, spellSchool, charLevel, spellLevel);  
  result = BaseSpellCount(pBaseclass, schoolID, baseclassLevel, spellLevel);  
  int maxPerSpellLvl = MaxSpellCount_BaseclassSchoolPrime(pBaseclass, prime, schoolID);
  if (result > maxPerSpellLvl) result = maxPerSpellLvl;
  return result;
}

*/



// The maximum spell level that a baseclass can cast.in a particular school.
// Consider the Max Level limited my the 'Prime Attribute Score'.
// Consider the limitation of the 'Baseclass Level'.
// Do not consider availability of spells.
//
//int MaxSpellLevelForClassAndSchool(baseclassKey charClass, int charLevel, BYTE prime, int spellSchool)
/*
int MaxSpellLevelForBaseclassAndSchool(const BASE_CLASS_DATA *pBaseclass, int baseclassLevel, BYTE prime, const SCHOOL_ID& schoolID)
{
  // type is character class mask
  // level is character level

   for (int i=MAX_SPELL_LEVEL;i>0;i--)
   {
     if (GetNumBaseSpells(baseclassLevel, prime, pBaseclass, schoolID, i) > 0) 
       return i;
   }
   return 0;
}
*/
/*
// max spell level that a character can cast at the current character level
// ie: a second level cleric can cast only first level cleric spells
//int CurrSpellLevel(WORD CharClassMask, int CharLevel, BYTE Prime, WORD spellSchoolMask)
int CurrSpellLevel(const BASECLASS_ID& baseclassID, int CharLevel, BYTE Prime, const SCHOOL_ID& schoolID)
{
  // type is character class mask
  // level is character level

   for (int i=MAX_SPELL_LEVEL;i>0;i--)
   {
     if (GetBaseSpells(CharLevel, Prime, CharClassMask, spellSchoolMask, i) > 0) 
       return i;
   }
   return 0;
}
*/
// Spells fall into two schools, MU and Cleric.
// Convert spell class into a school
//

WORD GetSpellCastingSchoolForCharBaseclass(WORD CharBaseClassFlag)
{
  //ASSERT( NumClassBits(CharBaseClassFlag) == 1 );

  // I have double-checked this - all classes except MU use
  // cleric spells.
//maybe we can have multiple schools such as druid, illusionist, etc.


/*
  switch (CharBaseClassFlag)
  {
  case ClericFlag:    return ClericFlag;
  case MagicUserFlag: return MagicUserFlag;
  case ThiefFlag:     return ClericFlag;
  case FighterFlag:   return ClericFlag;
  case PaladinFlag:   return ClericFlag;
  case RangerFlag:    return ClericFlag;//they should be able to use MU spells as well
  case DruidFlag:     return ClericFlag;//return DruidFlag;
  default: ASS ERT(FALSE);
  }
  return ClericFlag;
*/ NotImplemented(0xfd5ab7, false); return 0;


}
/*
// Split character class into two possible spell classes
// Multi-Class chars never have more than three spell classes available to them
//
void GetSpellClasses(classType CharClass, WORD &c1, WORD &c2, WORD &c3)
{
  c1=c2=c3=0;
  WORD mask = CharClassToClassFlag(CharClass);

  for (int i=0;i<NumBaseClass;i++)
  {
    WORD flag = (mask & (0x0001 << i));
    if (flag)
    {
      if (c1==0) 
        c1=flag;
      else if (c2==0)
        c2=flag;
      else if (c3==0)
        c3=flag;
      else ASS ERT(FALSE); // shouldn't be more than 3 classes in CharClass!
    }
  }
  // at least one must be valid
  ASSERT( c1!=0 );
}
*/
///////////////////////////////////////////////////////////////////////
//
// Spell Limits Functions
//
///////////////////////////////////////////////////////////////////////
/*
void SPELL_LIMITS::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    car << PrimeScore;
    //car << baseclassID;
    car << schoolID;
    car.Serialize((char *)numSpellsKnown, sizeof(numSpellsKnown));
    car.Serialize((char *)maxMemorizeCount, sizeof(maxMemorizeCount));
  }
  else
  {
    car >> PrimeScore;
    //car >> baseclassID;
    car >> schoolID;
    car.Serialize((char *)numSpellsKnown, sizeof(numSpellsKnown));
    car.Serialize((char *)maxMemorizeCount, sizeof(maxMemorizeCount));
  };
}

void SPELL_LIMITS::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << PrimeScore;
    //ar << baseclassID;
    ar << schoolID;
    ar.Write((char *)numSpellsKnown, sizeof(numSpellsKnown));
    ar.Write((char *)maxMemorizeCount, sizeof(maxMemorizeCount));
  }
  else
  {
    ar >> PrimeScore;
    //ar >> baseclassID;
    ar >> schoolID;
    ar.Read((char *)numSpellsKnown, sizeof(numSpellsKnown));
    ar.Write((char *)maxMemorizeCount, sizeof(maxMemorizeCount));
  };
}
*/

void spellLimitsType::Serialize(CArchive &ar, double version)
{
  //int i, n;
  if (ar.IsStoring())
  {
    die("We should not be using CArchive to store SPELL_LIMITS");
    //ar << m_spellClasses;    
    //n = m_spellLimits.GetSize();
    //ar << n;
    //for (i=0; i<n; i++)
    //{
    //  m_spellLimits[i].Serialize(ar);
    //};
    //ar << UseLimits;
  }
  else
  {   
#ifdef UAFEDITOR
    //ar >> m_spellClasses;
    {
      WORD spellClasses;
      ar >> spellClasses;
    };

#endif
    if (version < _VERSION_0780_)
    {
/*
  	  BYTE  maxSpellLevel[NumBaseClass];
      BYTE  maxSpells[NumBaseClass];
	    BYTE  maxSpellsPerLevel[NumBaseClass][MAX_SPELL_LEVEL];
      for (i=0;i<NumBaseClass;i++)
      {
        ar >> maxSpellLevel[i];
        ar >> maxSpells[i];
        for (int j=0;j<MAX_SPELL_LEVEL;j++)
          ar >> maxSpellsPerLevel[i][j];
      }
*/
      NotImplemented(0xac4cdb, false);
    }
    else
    {
      if (version >= _VERSION_0780_)
      {
        ar >> UseLimits;
      }
      else
      {
        UseLimits=TRUE;
      }

#ifdef UAFEDITOR
      if (version < VersionSpellNames)
      {
        int i;
        BYTE PrimeScore[NumBaseClass];
        for (i=0;i<NumBaseClass;i++)
          ar >> PrimeScore[i];
      }
      else
#endif
      {
        //int i, n;
        //ar >> n;
        //for (i=0; i<n; i++)
        //{
        //  SPELL_LIMITS spellLimits;
        //  spellLimits.Serialize(ar);
        //  m_spellLimits.Add(spellLimits);
        //};
      };
    };
  }
}
/*
BOOL SPELL_LIMITS::operator ==(const SPELL_LIMITS *pSpellLimits) const
{
  //if (baseclassID != pSpellLimits->baseclassID) return FALSE;
  if (schoolID != pSpellLimits->schoolID) return FALSE;
  if (PrimeScore != pSpellLimits->PrimeScore) return FALSE;
  int i;
  for (i=0; i<MAX_SPELL_LEVEL; i++)
  {
    if (numSpellsKnown[i] != pSpellLimits->numSpellsKnown[i]) return FALSE;
    if (maxMemorizeCount[i] != pSpellLimits->maxMemorizeCount[i]) return FALSE;
  };
  return TRUE;
}
*/

void spellLimitsType::Serialize(CAR &car, double version)
{
  if (car.IsStoring())
  {
    //ar << m_spellClasses;    
    car << UseLimits;
    //for (i=0;i<NumBaseClass;i++)
    //  ar << PrimeScore[i];
    //n = m_spellLimits.GetSize();
    //car << n;
    //for (i=0; i<n; i++)
    //{
    //  m_spellLimits[i].Serialize(car);
    //};
  }
  else
  {
    //car >> m_spellClasses;
   
    if (version < _VERSION_0780_)
    {
/*
  	  BYTE  maxSpellLevel[NumBaseClass];
      BYTE  maxSpells[NumBaseClass];
	    BYTE  maxSpellsPerLevel[NumBaseClass][MAX_SPELL_LEVEL];
      for (i=0;i<NumBaseClass;i++)
      {
        ar >> maxSpellLevel[i];
        ar >> maxSpells[i];
        for (int j=0;j<MAX_SPELL_LEVEL;j++)
          ar >> maxSpellsPerLevel[i][j];
      }
*/
      NotImplemented(0xbcbffa, false);
    }
    else
    {
      if (version >= _VERSION_0780_)
      {
#ifdef UAFEDITOR
        if (version < VersionSpellNames)
        {
          WORD spellClasses;
          BYTE primeScore;
          int i;
          car >> spellClasses;
          car >> UseLimits;
          for (i=0; i<NumBaseClass; i++)
          {
            car >> primeScore;
          };
        }
        else
#endif
        {
          car >> UseLimits;
        };
      }
      else
      {
        UseLimits=TRUE;
      };
      //for (i=0;i<NumBaseClass;i++)
      //  ar >> PrimeScore[i];
      //SPELL_LIMITS spellLimits;
      //car >> n;
      //for (i=0; i<n; i++)
      //{
      //  spellLimits.Serialize(car);
      //  m_spellLimits.Add(spellLimits);
      //};
    };
  }
}

#ifdef UAFEDITOR

const char *JKEY_USELIMITS = "useLimits";

void spellLimitsType::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_USELIMITS, UseLimits);
}
bool spellLimitsType::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_USELIMITS, UseLimits);
  return true;
}
#endif

spellLimitsType& spellLimitsType::operator=(const spellLimitsType &src)
{
  if (&src == this) return *this;
  //m_spellClasses = src.m_spellClasses;
  UseLimits=src.UseLimits;
/*
  m_spellLimits.RemoveAll();
  int i, n;
  n = src.m_spellLimits.GetSize();
  for (i=0; i<n; i++)
  {
    m_spellLimits.Add(*const_cast<SPELL_LIMITS*>(src.PeekSpellLimits(i)));
  };
  //for (int i=0;i<NumBaseClass;i++)
  //{
  //  PrimeScore[i] = src.PrimeScore[i];
  //  for (int j=0;j<MAX_SPELL_LEVEL;j++)
  //    numSpellsKnown[i][j] = src.numSpellsKnown[i][j];
  //}    
  */
  return *this;
}



BOOL spellLimitsType::operator==(const spellLimitsType &src) const
{
  if (&src == this) return TRUE;
  //if (m_spellClasses != src.m_spellClasses) return FALSE;
  if (UseLimits != src.UseLimits) return FALSE;
  //for (int i=0;i<NumBaseClass;i++)
  //{
  //  if (PrimeScore[i] != src.PrimeScore[i]) return FALSE;
  //  for (int j=0;j<MAX_SPELL_LEVEL;j++)
  //  {
  //    if (numSpellsKnown[i][j] != src.numSpellsKnown[i][j]) return FALSE;
  //  }
  //}    
  //int i, n;
  //n = m_spellLimits.GetSize();
  //if (n != src.m_spellLimits.GetSize()) return FALSE;
  //for (i=0; i<n; i++)
  //{
  //  if (!(*PeekSpellLimits(i) == src.PeekSpellLimits(i))) return FALSE;
  //};
  return TRUE;
}

void spellLimitsType::Clear()
{
  //m_spellClasses = 0;
  UseLimits=TRUE;
  //for (int i=0;i<NumBaseClass;i++)
  //{
    //PrimeScore[i]=0;
    //for (int j=0;j<MAX_SPELL_LEVEL;j++)
    //  numSpellsKnown[i][j] = 0;
  //}    
  //m_spellLimits.RemoveAll();
}

void spellLimitsType::SetToMaxLimits()
{
  //m_spellClasses = AllClasses;
  UseLimits=FALSE;  // This should suffice.  If not, we will add a m_maxLimits.

  //for (int i=0;i<NumBaseClass;i++)
  //{
  //  PrimeScore[i]=18;
  //  for (int j=0;j<MAX_SPELL_LEVEL;j++)
  //    numSpellsKnown[i][j] = 0;
  //}    
}

/*
// Do the class bits overlap with the spell book's class bits?
//BOOL spellLimitsType::ValidSpellClass(WORD CastMask) 
BOOL spellLimitsType::ValidSpellClass(const SCHOOL_ID& schoolID) 
{ 
  if (!UseLimits) return TRUE; 
  //return ((CastMask & m_spellClasses) ? TRUE : FALSE); 
  return LocateSpellLimits(schoolID) >= 0;
}
*/
//BOOL spellLimitsType::AddSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel)
/*
BOOL spellLimitsType::KnowSpell(const BASECLASS_ID& baseclassID, const SPELL_DATA *pSpell , 
                               const BASECLASS_LIST&  spellAllowedBaseclasses, //WORD CastMask,    // from spell - allowed baseclasses
                               int                    SpellLevel )
{
  //ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  //ASSERT( NumClassBits(CasterClassFlag) == 1 );
  
  //BOOL OkToAdd = CanKnowSpell(CasterClassFlag, SpellClassFlag, CastMask, SpellLevel);
  //BASECLASS_ID baseclassID;
  //baseclassID = pCasterBaseclass->BaseclassID();
  BOOL OkToAdd = CanKnowSpell(pSpell->schoolID, pSpell->Level);

  if (!UseLimits) 
    OkToAdd = TRUE;

  if (OkToAdd)
  {
    int i;
    i = LocateSpellLimits(pSpell->schoolID);
    if (i < 0) return FALSE;
    //numSpellsKnown[FlagToIndex(CasterClassFlag)][SpellLevel-1]++;
    GetSpellLimits(i)->numSpellsKnown[pSpell->Level-1]++;
    return TRUE;
  }
  else
    return FALSE;
}
*/
// remove one spell of this level, for this class
//void spellLimitsType::DecrementSpellCount(WORD CasterClassFlag, int SpellLevel)

/*
void spellLimitsType::DecrementSpellCount(const SPELL_DATA *pSpell)
{ 
  //ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  int i;
  //BASECLASS_ID baseclassID;
  SPELL_LIMITS *pSpellLimits;
  i = LocateSpellLimits(pSpell->schoolID);
  if (i < 0) return;
  pSpellLimits = GetSpellLimits(i);
  //if (numSpellsKnown[FlagToIndex(CasterClassFlag)][SpellLevel-1] > 0)
  //  numSpellsKnown[FlagToIndex(CasterClassFlag)][SpellLevel-1]--;    
  if (pSpellLimits->numSpellsKnown[pSpell->Level-1] > 0)
  {
    pSpellLimits->numSpellsKnown[pSpell->Level-1]--;
  };
}

// remove all spells of this level, for all classes
void spellLimitsType::RemoveSpellLevel(int SpellLevel)
{
  //ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  //for (int i=0;i<NumBaseClass;i++)
  //  numSpellsKnown[i][SpellLevel-1] = 0;
  int i, n;
  n = m_spellLimits.GetSize();
  for (i=0; i<n; i++)
  {
    m_spellLimits[i].numSpellsKnown[SpellLevel-1] = 0;
  };
}
*/
/*
int spellLimitsType::LocateSpellLimits(const BASECLASS_ID& baseclassID, const SCHOOL_ID& schoolID) const
{
  int i, n;
  n = m_spellLimits.GetSize();
  for (i=0; i<n; i++)
  {
    if (m_spellLimits[i].baseclassID == baseclassID)
    {
      if (m_spellLimits[i].schoolID == schoolID)
      {
        return i;
      };
    };
  };
  return -1;
}
*/

/*
int spellLimitsType::LocateSpellLimits(const SCHOOL_ID& schoolID) const
{
  int i, n;
  n = m_spellLimits.GetSize();
  for (i=0; i<n; i++)
  {
    if (m_spellLimits[i].schoolID == schoolID)
    {
      return i;
    };
  };
  return -1;
}

// ie: how many baseclass spells of a particualr level of any school?
//BYTE spellLimitsType::numKnown(WORD ClassFlag, int SpellLevel)
BYTE spellLimitsType::NumKnown(const SCHOOL_ID& schoolID, int spellLevel)
{ 
  //ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  int i;
  i = LocateSpellLimits(schoolID);
  if (i < 0) return 0;
  //    for (j=0; j<MAX_SPELL_LEVEL; j++)
  //    {
  return m_spellLimits[i].numSpellsKnown[spellLevel-1];
  //    };
};

  */
  /*
  BYTE count=0;
  for (int i=0;i<MAX_SPELL_LEVEL;i++)
    count += numSpellsKnown[FlagToIndex(ClassFlag)][i];
  return count; 
  */
//}

// max spells of SpellLevel that can be memorized by a particular baseclass at current CharLevel
//BYTE spellLimitsType::MaxSpellsForLevel(WORD CharClass, int CharLevel, int SpellLevel, BYTE prime)
// Includes bonus for high Prime Attribute Score

/*
BYTE spellLimitsType::MaxSpellsForLevel(const BASE_CLASS_DATA *pBaseclass, 
                                        int CharLevel, 
                                        int SpellLevel, 
                                        BYTE prime)
{ 
  int max;
  ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  if (!UseLimits) return 255;


  // Is this one of the baseclasses we know about?
  //if (!ValidSpellClass(CharClass))
  //  return 0;

  int i;
  //int max = GetBaseSpells(CharLevel, prime, CharClass, GetSpellCastingSchoolForCharBaseclass(CharClass), SpellLevel);




  max = GetNumBaseSpells(CharLevel, 
                          prime, 
                          CharClass, 
                          GetSpellCastingSchoolForCharBaseclass(CharClass), 
                          SpellLevel);
 NotImplemented(0x5dfdae, false); max=0;




  if (max > 0)
  {
    BYTE bonus = 0;
    for ( i=1;i<=prime;i++)
      //bonus += GetBonusSpellQtyBySpellLevel(i, SpellLevel, CharClass);
      bonus += BonusSpellQtyBySpellLevel(i, SpellLevel, pBaseclass);
    max += bonus;




    int MaxPerSpellLvl = GetMaxSpellsForSpellLvlPrime(CharClass, prime);
    if (max > MaxPerSpellLvl)
      max = MaxPerSpellLvl;
 NotImplemented(0x6d73e9, false);



  }

  return max;
}
*/

// Added Dec 2010.  Should eventually replace MaxSpellsForLevel
// Max spells of a particular school and SpellLevel that can be memorized by baseclass at current baseclassLevel.
// After limiting for Prime Attribute score we
// includes the bonus spells for High Prime Attribute score.
//BYTE spellLimitsType::MaxSpellsForLevelAndSchool(int charBaseClass, int charLevel, int spellLevel, int prime, int spellSchool)
/*
BYTE spellLimitsType::MaxSpellsForLevelAndSchool(const BASE_CLASS_DATA *pBaseclass,
                                                 int baseclassLevel, 
                                                 int spellLevel, 
                                                 int prime, 
                                                 const SCHOOL_ID& schoolID)
{ 
  ASSERT( (spellLevel>=1)&&(spellLevel<=MAX_SPELL_LEVEL) );
  if (!UseLimits) return 255;


//  if (!ValidSpellClass(charBaseClass))
//    return 0;

  int i;
  //int max = GetNumBaseSpells(charLevel, prime, charBaseClass, spellSchool, spellLevel);
  int max = GetNumBaseSpells(baseclassLevel, prime, pBaseclass, schoolID, spellLevel);

  if (max > 0)
  {
    BYTE bonus = 0;
    for ( i=1;i<=prime;i++)
      //bonus += BonusSpellQtyBySpellLevel(i, spellLevel, charBaseClass);
      bonus += BonusSpellQtyBySpellLevel(i, spellLevel, pBaseclass);
    max += bonus;

    //int MaxPerSpellLvl = MaxSpellsForSpellLvlPrime(charBaseClass, prime, spellSchool);
    int MaxPerSpellLvl = MaxSpellCount_BaseclassSchoolPrime(pBaseclass, prime, schoolID);
    if (max > MaxPerSpellLvl)
      max = MaxPerSpellLvl;
  }

  return max;
}
*/
// Max spells of a particular school and SpellLevel that can be memorized by baseclass at current baseclassLevel.
// After limiting for Prime Attribute score we
// includes the bonus spells for High Prime Attribute score.

/*
void spellLimitsType::GetMaxMemorizePerSpellLevel(const BASE_CLASS_DATA *pBaseclass,
                                                        int baseclassLevel, 
                                                        int *maxMemorizeArray, //[MAX_SPELL_LEVEL] 
                                                        int prime, 
                                                        const SCHOOL_ID& schoolID) const
{ 
  int spellLevel;
  if (!UseLimits)
  {
    for (spellLevel=0; spellLevel<MAX_SPELL_LEVEL; spellLevel++)
    {
      maxMemorizeArray[spellLevel-1] =  255;
    };
    return;
  };

  int i;

  for (spellLevel=1; spellLevel<=MAX_SPELL_LEVEL; spellLevel++)
  {
    if (maxMemorizeArray[spellLevel-1] > 0)
    {
      BYTE bonus = 0;
      for (i=1;i<=prime;i++)
      {
        //bonus += BonusSpellQtyBySpellLevel(i, spellLevel, charBaseClass);
        bonus += BonusSpellQtyBySpellLevel(i, spellLevel, pBaseclass);
      };
      maxMemorizeArray[spellLevel-1] += bonus;
    };
  };
  return;
}
*/
// this one checks the spell limits without regard to current char level,
// since chars can add spells to the book without actually being at a
// char level that can cast the spell
//BOOL spellLimitsType::CanKnowSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel)

/*
BOOL spellLimitsType::CanKnowSpell(const SCHOOL_ID& schoolID, // Yes
                                   int   spellLevel)
{
  //ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  //ASSERT( NumClassBits(CasterClassFlag) == 1 );
  //BASECLASS_ID baseclassID;
  if (!UseLimits) return TRUE;


  if (!ValidSpellClass(CastMask))
    return FALSE;
   // Is this 
 
  //baseclassID = pBaseclass->BaseclassID();


  const SPELL_LIMITS *pSpellLimits;
  pSpellLimits = PeekSpellLimits(schoolID);
  if (pSpellLimits == NULL) return FALSE;
  //BYTE prime = PrimeScore[FlagToIndex(CasterClassFlag)];  
  
  // what is the highest spell level usable by this char class
  //BYTE MaxSpellCastingLevel = GetMaxSpellLvlByPrime(CasterClassFlag, prime);
  //BYTE MaxSpellCastingLevel = MaxSpellLevel_BaseclassSchoolPrime(baseclassID, prime, schoolID);
  if (spellLevel > pSpellLimits->maxSpellLevel) return FALSE;

  // no matter what the char level is, what is the maximum number of spells
  // that can be known at this spell level
  //int MaxPerSpellLvl = MaxSpellCount_BaseclassSchoolPrime(CasterClassFlag, prime);

  //if (MaxPerSpellLvl < 255)
  //{
    // accumulate spell bonuses based upon prime score
  //  BYTE bonus = 0;
  //  for (int i=1;i<=prime;i++)
      //bonus += GetBonusSpellQtyBySpellLevel(i, SpellLevel, CasterClassFlag);
  //    bonus += pSpellLimits->bonusSpells[i];
  //  MaxPerSpellLvl += bonus;
  //}

  //if (MaxPerSpellLvl > 255) MaxPerSpellLvl = 255;

  //return ( numKnown(CasterClassFlag, SpellLevel) < MaxPerSpellLvl );
  return pSpellLimits->totalSpellsKnown < pSpellLimits->maxSpellCount;
}
*/
/*
void spellLimitsType::SetSpellLimits(const CHARACTER *pChar,
                                     int prime,
                                     const SCHOOL_ID& schoolID)
                    
{ 
  BASECLASS_ID baseclassID;
  int i;
  i = LocateSpellLimits(schoolID);
  if (i < 0)
  {
    SPELL_LIMITS spellLimits;
    spellLimits.schoolID = schoolID;
    i = m_spellLimits.GetSize();
    m_spellLimits.Add(spellLimits);
  };
  SPELL_LIMITS *pSpellLimits;
  pSpellLimits = GetSpellLimits(i);
  pSpellLimits->PrimeScore = prime;
  // Search the character's baseclasses
  for (statsIndex=0; statsIndex<GetBaseclassStatsCount(); statsIndex++)
  {
  
  if (maxSpellCount > pSpellLimits->maxSpellCount)
  {
    pSpellLimits->maxSpellCount = maxSpellCount;
  };
  if (maxSpellLevel > pSpellLimits->maxSpellLevel)
  {
    pSpellLimits->maxSpellLevel = maxSpellLevel;
  };
}
*/


/*
Memorizing a spell takes 15 minutes of games time per spell level, plus a
preparation period of four hours plus an additional two hours for 3rd level
spells, etc.  For example, 1st and 2nd level spells take a minimum preparation of
four hours, while 3rd level spells take six hours.

Example: To memorize two 1st level spells, one 2nd level spell, and one 3rd
level spell would take: (6 hours preparation) + (2 * 15 min) + (1 * 30 min) +
(1 * 45 min) = 7 hours 45 min
*/
// in minutes
int GetSpellMemorizeTime(int level)
{
  return (level*15);
}

// in minutes
int GetSpellPrepTime(int level)
{
  switch (level)
  {
  case -1:
  case 0:  return 0;

  case 1:
  case 2:  return 4*60; // 4 hours

  case 3:
  case 4:  return 6*60; // 6 hours

  case 5:
  case 6:  return 8*60; // 8 hours

  case 7:
  case 8:  return 10*60; // 10 hours

  default: return 12*60; // 12 hours
  }
}