#ifndef __FRUA_STRUCTS_H__
#define __FRUA_STRUCTS_H__
/******************************************************************************
* Filename: FRUAStructs.h
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

//---------------------------------------------------------------------------
//
// NOTE: some structure definitions were copied and/or modified from the following program
//
//---------------------------------------------------------------------------
//  idump - UAWin utiliy program
//  Copyright (C) 1999 Bil Simser (bsimser@home.com)
//  Modified for use with UAF by CocoaSpud@hotmail.com
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//---------------------------------------------------------------------------

// force 1 byte packing alignment for this file
#include <pshpack1.h>

#define MAX_FRUA_SOUND_VOCAB_STRINGS 40
#define MAX_FRUA_SOUNDS 100
#define MAX_FRUA_WALLS 16
#define MAX_FRUA_BACKDROPS 20
#define MAX_FRUA_PICS 240
#define MAX_FRUA_MONSTERS 128
#define MAX_FRUA_CHARACTERS 255
#define MAX_FRUA_ITEM_CLASSES 128
#define MAX_FRUA_ITEMS 254
#define MAX_FRUA_EVENTS 100
#define MAX_FRUA_EVENT_TYPES 39
#define MAX_FRUA_LEVEL_GRIDS 576
#define MAX_FRUA_LEVEL_STRINGS 400
#define MAX_FRUA_LEVEL_STRING_LEN 228
#define MAX_FRUA_ITEM_VOCAB_STRINGS 154
//#define MAX_FRUA_SPELL_VOCAB_STRINGS 127
#define MAX_FRUA_SPEC_AB_VOCAB_STRINGS 255
#define MAX_FRUA_SPELLS 137
#define MAX_FRUA_CLASSES 17
#define MAX_FRUA_RACES 7
// north, south, etc
#define MAX_FRUA_DIRECTIONS 15
#define MAX_FRUA_COST_MODIFIERS 20

//#define MAX_FRUA_ART_FILES 255
//#define MAX_FRUA_SOUND_FILES 255

// RBT not sure what actual value is yet
#define MAX_FRUA_LEVELS 50 


// where the spell records start inside ckit.exe
#define SPELL_TABLE_BYTE_OFFSET 554688


/*
http://frua.rosedragon.org/pc/uanews/uanl27/tc-merr1.htm

From bytes 554136 to 554683 are the pointers to the strings. There are 137 of them, some pointing to Null strings. (a single 0- terminator value). 
They're in groups of 2 words. The first word (low) is the interesting one, the second word (high) is probably something like a segment/memory code. 
I'm not sure of its exact usage.

Anyway, just looking at the first word (2 bytes) of each group of four, you get values like 12860, 12866, etc. 
These are the offsets of the Spell Name strings. The base address you add them to is 544864.

So 544864+12860=557724, which is the address of the first spell-name string: "Bless". All strings are terminated with a null(0) character.

As I said, there are 137 strings here, taking up 1684 bytes of space. Here they are, in order of how the pointers view them, for anyone who's interested.

If anyone knows what actually references the Potions & Wands strings, could you let me know? (I guess it _might_ be the spell effect code, but I could be dead wrong there).

0   Bless
1   Curse
2   Cure Light Wounds
3   Cause Light Wounds
4   Detect Magic
5   Protection From Evil
6   Protection from Good
7   Resist Cold
8   Burning Hands
9   Charm Person
10  Detect Magic
11  Enlarge
12  Reduce
13  Friends
14  Magic Missile
15  Protection From Evil
16  Protection From Good
17  Read Magic
18  Shield
19  Shocking Grasp
20  Sleep
21  Find Traps
22  Hold Person
23  Resist Fire
24  Silence, 15' Radius
25  Slow Poison
26  Snake Charm
27  Spiritual Hammer
28  Detect Invisibility
29  Invisibility
30  Knock
31  Mirror Image
32  Ray of Enfeeblement
33  Stinking Cloud
34  Strength
35  Heal
36  Cure Blindness
37  Cause Blindness
38  Cure Disease
39  Cause Disease
40  Dispel Magic
41  Prayer
42  Remove Curse
43  Bestow Curse
44  Blink
45  Dispel Magic
46  Fireball
47  Haste
48  Hold Person
49  Invisibility, 10' Radius
50  Lightning Bolt
51  Prot. From Evil, 10' Radius
52  Prot. From Good, 10' Radius
53  Prot. From Normal Missiles
54  Slow
55  Harm
56  Prot. From Evil, 10' Radius
57  Cure Serious Wounds
58  Silence 15' radius
59  Detect Magic
60  Remove Curse
61  Bless
62  Charm Person
63  Burning Hands
64
65  Cause Serious Wounds
66  Neutralize Poison
67  Poison
68  Prot. From Evil, 10' Radius
69  Sticks to Snakes
70  Cure Critical Wounds
71  Cause Critical Wounds
72  Dispel Evil
73  Flame Strike
74  Raise Dead
75  Slay Living
76  Detect Magic
77  Entangle
78  Faerie Fire
79  Invisibility to Animals
80  Charm Monsters
81  Confusion
82  Dimension Door
83  Fear
84  Fire Shield
85  Fumble
86  Ice Storm
87  Minor Globe Of Invulnerability
88  Remove Curse
89  Barkskin
90  Cloud Kill
91  Cone of Cold
92  Feeblemind
93  Hold Monsters
94  Scroll of Protection
95  Charm Person Or Mammal
96
97  Cure Light Wounds
98
99  Bestow Curse
100 Blade Barrier
101 Restoration
102 Energy Drain
103 Destruction
104 Resurrection
105 Cure Disease
106 Neutralize Poison
107 Hold Animal
108 Prot. from Fire
109 Death Spell
110 Disintegrate
111 Globe of Invulnerability
112 Stone to Flesh
113 Flesh to Stone
114 Delayed Blast Fireball
115 Mass Invisibility
116 Power Word Stun
117 Fire Touch
118 Iron Skin
119 Mass Charm
120 Otto's Irr. Dance
121 Mind Blank
122 Power Word Blind
123 Meteor Swarm
124 Power Word Kill
125 Monster Summoning
126 Potion Of Speed
127 Potion Of Frost
128 Javelin Of Lightning
129 Wand Of Paralyzation
130 Potion Of Healing
131 Elixir Of Youth
132 Necklace Of Missiles
133 Wand Of Magic Missiles
134 Scroll of Protection
135 Potion Of Invisibility
136 Potion Of Extra Healing

(I started from 0, remember? That's 137 strings! Counting the Null ones, of course! ;-)
*/
// where the spell names start inside ckit.exe
#define SPELL_NAMES_BYTE_OFFSET 554136
#define SPELL_NAMES_LIST_BYTE_OFFSET 557724


// where the level's map cells are located inside the geo*.dat files
#define LEVEL_MAP_CELLS_OFFSET 322
// where the level event records are located inside the geo*.dat files
#define LEVEL_EVENTS_OFFSET 3786
// where the level compressed strings are located inside the geo*.dat files
#define LEVEL_COMPRESSED_STRINGS_OFFSET 5798

#define FRUAWallType_None 0
#define FRUAWallType_Wall 1
#define FRUAWallType_Door 2


/*
Offset 554688:	The beginning of the spell effect records.  There appear
   Hex (0x876c0)	to be 137 records, each of which is 22 bytes long.

   Each spell is defined by a record of 22 bytes in length,
   with the data in each record allocated as follows:

Offset	Definition

   0	Spell Class: 
      0 = Cleric
      1 = Druid
      2 = Mage
      3 = Device
         (Examples:
            57/Potion of Speed,
            59/Potion of Giant Strength,
            60/Javelin of Lightning,
            61/Wand of Paralyzation,
            62/Potion of Healing,
            63/Elixir of Youth,
            64/Necklace of Missiles)
      4 = Device
         (Examples:
            65/Wand of Magic Missiles,
            95/Scroll of Prot. Dragons,
            97/Potion of Invisibility,
            99/Potion of Extra Healing)

   1	Set to zero.

   2	Spell level.  Spell level for devices is based on spell class as follows:
      Class 3 is spell level 0.
      Class 4 is spell level 6.

   3	Base range of spell in squares. 255 = Touch spell.

   4	Variable range of spell (in units of 1 square per level).
      Special cases:
         12 = Dimension Door
         16 = Minor Globe of Invulnerability
         Note that the ranges of those spells whose
         ranges are given in units of 0.25 or 0.5
         squares per level in the manual are actually
         too high.

   5	Base duration of spell in rounds.

   6	Variable duration of spell in rounds per level.

   7  The known field values can be defined as follows:
      0 = No target in combat (either spell affects caster, or spell is camp only)
      1 = 1-2 targets in combat
      2 = 1-3 targets in combat
      3 = 1-4 targets in combat
      4 = Single target in combat
      5 = 1-5 targets in combat:
         Faerie Fire and Charm Monsters. There may be a factor involving range from first target.
      6 = 1-3 targets in combat: Hold Person (Cleric spell)
      7 = 1-4 targets in combat: All other Hold spells
      8 = Line/Cone Area Effect: 
         Lightning Bolt, Dimension Door,
         Fear, Cone of Cold,
         and Meteor Swarm.
         The result is a single target for spells not originally designed for this option.
      9 = Small Area Effect:
         Sleep, Stinking Cloud,
         Dispel Magic,
         Invisibility 10' Radius,
         Cloudkill, Blade Barrier,
         Death Spell, Mass Charm,
         Power Word Kill
      10 = Medium Area Effect:
         Bless, Curse, Haste, Slow,
         Ice Storm, Mass Invisibility,
         Power Word Blind
      11 = Large Area Effect:
         Fireball, Haste, Confusion,
         Delayed Blast Fireball
      31 = Special (Silence 15' Radius)
      240 = Special (Snake Charm / Barkskin)

   8	Spell class:
      0 = Combat only spell
      1 = Spell affects self only
      2 = Spell affects selected individual(s)
      3 = Spell affects entire party

   9	Set to zero.

   10	Save Allowed. 
      0 = No Save Allowed
      1 = Save to Negate Spell
      2 = Save for Half Damage
      3 = Special Save

   11	Set to zero.

   12	Poison Effect or Save Type:
      0 = Poison Immunity protects (save vs. poison)
      4 = Non-Poison spell (save vs. spell)
      Note:  Values of 1-3 may correspond to other save types, but this has not yet been confirmed.

   13	Set to zero.

   14	Special effect/ability code generated by spell.

   15	Camp/Combat Casting.
       0 = Camp only spell
       1 = Combat only spell
       2 = Camp and Combat spell

   16	Set to zero.

   17	Casting time in segments.

   18	Priority:  In "Quick" mode, the computer searches through the caster's memorized spells
      and will (if possible) cast the spell with the highest priority.

   19	Attack Spell Flag.
       0 = Spell Cast on Allies
       1 = Spell Cast on Foes

   20	Set to zero.

   21	Some examples for study:
      1 = Examples (	21/Sleep,
              25/C: Silence 15 Radius,
              34/Stinking Cloud,
              41/C: Dispel Magic,
              46/Dispel Magic,
              59 (Potion of Giant Strength))
      3 = Examples (	47/Fireball)
      0 = All the other spells

   Offset 557724:	The beginning of the list of spell names.
   Hex (0x8829c)
*/

// 137 spells of 22 bytes each, starting at offset 554688 in ckit.exe
struct ImportUASpell
{
   //0	Spell Class: 
   BYTE type; // 0=cl,1=dr,2=mu,3=dev,4=dev
   //1	Set to zero.
   BYTE foo1;
   //2	Spell level.
   BYTE castLevel; // if type=3->level=0, type=4->level=6
   // 3	Base range of spell in squares. 255 = Touch spell.
   BYTE range; // in squares, 255=touch
   //4	Variable range of spell (in units of 1 square per level).
   BYTE rangePerLevel; // 12=DimensionDoor, 16=MinorGlobeOfInvulnerability
   //5	Base duration of spell in rounds.
   BYTE duration; // in rounds
   //6	Variable duration of spell in rounds per level.
   BYTE durationPerLevel;
   //
   BYTE targetingQty;
   //  8	Spell class:
   BYTE spellClass;
   //9	Set to zero
   BYTE foo2;
   //10	Save Allowed. 
   BYTE saveResult;
   //11	Set to zero.
   BYTE foo3;
   //12	Poison Effect or Save Type:
   BYTE saveVs;
   //13	Set to zero.
   BYTE foo4;
   // 14	Special effect/ability code generated by spell.
   BYTE specialAbility;
   //15	Camp/Combat Casting.
   BYTE castInCampCombat; // 0=camp,1=combat,2=both
   //16	Set to zero
   BYTE foo5;
   //17	Casting time in segments.
   BYTE castTime; // in segments
   //18	Priority
   BYTE priority;
   //19	Attack Spell Flag.
   BYTE friendlyFire; // 0=allies only, 1=foes only
   //20	Set to zero.
   BYTE foo6;
   //21	Some examples for study:
   BYTE unsure;
};

struct ImportFRUAMonster
{
   //0-3   Pointer to next character.  Always set to zero for new characters.
   BYTE foo1;
   BYTE foo2;
   BYTE foo3;
   BYTE foo3a;

   //4-7   Special Ability: 
   //   0/0/0/0 if no special abilities
   //   0/0/254/104 (or any other non-zero value) if one or more special abilities
   WORD saCount; // not needed
   WORD specAbs; // 0/0 if no special abilities, 254/104 if one or more special abilities

   //8-11  Inventory:
   //  0/0/0/0 if no items
   //  0/0/76/95 (or any other non-zero value) if one or more items carried
   WORD carryCount; // not needed
   WORD isCarry; // 0/0 if no items, 76/95 (0x1E0F) if one or more items carried - 74/57
   
   //12-15  As 8-11 but for item in weapon hand
   WORD whCount;
   WORD isWpnHand;

   //16-19  As 8-11 but for item in shield hand
   WORD shCount;
   WORD isShHand;

   //20-23  As 8-11 but for item worn as armor
   WORD arCount;
   WORD isArmor;

   //24-27  As 8-11 but for item worn as gauntlets
   WORD gCount;
   WORD isGauntlet;

   //28-31  As 8-11 but for item worn as helmet
   WORD hCount;
   WORD isHelmet;

   //32-35  As 8-11 but for item worn as belt
   WORD bCount;
   WORD isBelt;

   //36-39  As 8-11 but for item worn as robe
   WORD rCount;
   WORD isRobe;

   //40-43  As 8-11 but for item worn as cloak
   WORD cCount;
   WORD isCloak;

   //44-47  As 8-11 but for item worn as boots
   WORD btCount;
   WORD isBoots;

   //48-51  As 8-11 but for item worn as ring
   WORD rgCount;
   WORD isRing;

   //52-55  As 8-11 but for item worn as 2nd ring
   WORD rg2Count;
   WORD isRing2;

   //56-59  As 8-11 but for item carried as arrows
   WORD amCount;
   WORD isAmmo;

   //60-63  As 8-11 but for item carried as bolts
   WORD boltCount;
   WORD isBolts;

   //64-67  ???     Apparently reserved for additional items, but not used.
   WORD unknownItemCount;
   WORD unknownItems;

   //68-71  XP Total (Current)
   DWORD exp;

   //72-75  XP Total (Before Draining)
   DWORD expPreDrain;

   //76-77  Platinum Pieces
   WORD platinum;

   //78-79  Gems
   WORD gems;

   //80-81  Jewels
   WORD jewelry;

   //82-83  Age in Years
   WORD age;

   //84-85  XP for Killing          Set to zero for player character.
   WORD expWorth; // for killing, 0 for characters

   //86-87  Current Encumbrance in Coins (Since this is another field that is reset by UA, we need not worry about setting it correctly)
   WORD encumb; // in gold pcs

   //   88     Race            
   //     0 = Elf
   //     1 = Half-Elf
   //     2 = Dwarf
   //     3 = Gnome
   //     4 = Halfling
   //     5 = Human
   //     6 = Monster
   BYTE race; // 0=elf,1=halfelf,2=dwarf,3=gnome,4=halfling,5=human,6=monster

   //89     Current Class    
   //   0 = Cleric
   //   1 = Knight (Not Used)
   //   2 = Fighter
   //   3 = Paladin
   //   4 = Ranger
   //   5 = Magic-User
   //   6 = Thief
   //   7 = Monk (Not Used)
   //   8 = C/F
   //   9 = C/F/MU
   //   10 = C/R
   //   11 = C/MU
   //   12 = C/T
   //   13 = F/MU
   //   14 = F/T
   //   15 = F/MU/T
   //   16 = MU/T
   BYTE charClass; 

   //90     Undead Type      
   //   0 = Not Undead
   //   1 = Skeleton
   //   2 = Zombie
   //   3 = Ghoul
   //   4 = Shadow
   //   5 = Wight
   //   6 = Ghast
   //   7 = Wraith
   //   8 = Mummy
   //   9 = Spectre
   //   10 = Vampire
   //   11 = Ghost
   //   12 = Lich
   //   13 = Special
   BYTE undead;

   //91     ???             Not needed for new characters.
   BYTE foo6;

   //92     Gender          
   //   0 = Male
   //   1 = Female
   BYTE gender; // 0=male,1=female

   //93     Alignment       
   //   0 = LG   3 = NG   6 = CG
   //   1 = LN   4 = TN   7 = CN
   //   2 = LE   5 = NE   8 = CE
   BYTE alignment; 

   //94     Status          
   //   0 = Okay
   //   1 = Animated
   //   2 = Temp Gone
   //   3 = Running
   //   4 = Unconscious
   //   5 = Dying
   //   6 = Dead
   //   7 = Petrified
   //   8 = Gone
   //   9 = Awaiting Summons
   BYTE status; 
   
   //95     Combat Mode     
   //   0 = PC/NPC
   //   1 = Monster
   //   Other values yield undesirable results.
   BYTE combatMode; // 0=pc/npc, 1=monster

   //96-110 Name  15 ASCII Characters
   //111  Set to zero (To terminate Name string)
   char name[16]; // 15 plus zero terminator

   //   112-113 Str (raw/modified)
   //   114-115 Int 
   //   116-117 Wis
   //   118-119 Dex
   //   120-121 Con
   //   122-123 Cha
   BYTE str;
   BYTE strMod;
   BYTE intel;
   BYTE intMod;
   BYTE wis;
   BYTE wisMod;
   BYTE dex;
   BYTE dexMod;
   BYTE con;
   BYTE conMod;
   BYTE cha;
   BYTE chaMod;
   //124-125 Extraordinary (percentile) Str (modified/raw)
   BYTE strExt; // 0.100
   BYTE strExtMod;

   //126     ???             Set to zero for new character.
   BYTE foo7;

   //127     60 - Base THAC0     Reset by UA program.
   BYTE thac0; //60 - Base THAC0

   //128  Cure Disease Flag:
   //   0  = Cannot cure disease
   //   1+ = Number of times that character can cure disease
   BYTE cureDisease; // 0=cannot, else nbr times

   //129     Maximum HP
   BYTE maxHp;

   //130     Size Indicator
   //   1 = small combat icon
   //   2 = tall combat icon
   //   3 = wide combat icon
   //   4 = big combat icon
   //   5 = huge combat icon (2 tall x 3 wide)
   //   Add 128 to this value if button for "Large even if 1x1" has been selected.
   //   Presumably, UA regards as "large" all combatants for whom this byte is set to a value greater than 1.
   BYTE size; 

   //131     Save vs. paralysis/poison/death magic
   BYTE svPaPoDm;

   //132     Save vs. petrification/polymorph
   BYTE svPePo;

   //133     Save vs. rod/staff/wand
   BYTE svRoStWa;

   //134     Save vs. breath weapon
   BYTE svBw;

   //135     Save vs. spell
   BYTE svSp;

   //136     Movement Rate (Base)
   BYTE move;

   //137     Current Level (in highest active class)
   BYTE level; // in highest active class;

   //138     Level in Former Class
   BYTE levelFormerClass;

   // Thieving skills are adjusted for level
   // and race but not for dexterity or armor.

   //139     Pick Pocket
   BYTE pickPockets;
   //140     Open Lock
   BYTE openLocks;
   //141     Find/Remove Traps
   BYTE findRemTraps;
   //142     Move Silently
   BYTE moveSilent;
   //143     Hide in Shadows
   BYTE hideShadows;
   //144     Hear Noise
   BYTE hearNoise;
   //145     Climb Walls
   BYTE climbWalls;
   //146     Read Languages
   BYTE readLang;

   //reserved for 128 + Morale
   BYTE morale; // 128 + morale if non-zero

   //Warrior Level   Equivalent fighter level for "sweep"
   BYTE fighterSweepLevel;

   //149     ???             Set to zero for new character.
   BYTE foo8;

   //   Pre-Drain levels
   //   150     Cleric Level 
   //   151     Knight Level
   //   152     Fighter Level
   //   153     Paladin Level
   //   154     Ranger Level 
   //   155     Mage Level 
   //   156     Thief Level
   BYTE clericLvlPreDrain;
   BYTE knightLvlPreDrain;
   BYTE fighterLvlPreDrain;
   BYTE paladinLvlPreDrain;
   BYTE rangerLvlPreDrain;
   BYTE mageLvlPreDrain;
   BYTE thiefLvlPreDrain;

   // current levels
   //   157     Cleric Level (Current)
   //   158     Knight Level
   //   159     Fighter Level
   //   160     Paladin Level
   //   161     Ranger Level
   //   162     Mage Level
   //   163     Thief Level
   BYTE clericLvl;
   BYTE knightLvl;
   BYTE fighterLvl;
   BYTE paladinLvl;
   BYTE rangerLvl;
   BYTE mageLvl;
   BYTE thiefLvl;

   // levels before changing class; Human only
   //   164     Cleric Level 
   //   165     Knight Level
   //   166     Fighter Level
   //   167     Paladin Level
   //   168     Ranger Level
   //   169     Mage Level
   //   170     Thief Level
   BYTE clericLvlPreClassChng;
   BYTE knightLvlPreClassChng;
   BYTE fighterLvlPreClassChng;
   BYTE paladinLvlPreClassChng;
   BYTE rangerLvlPreClassChng;
   BYTE mageLvlPreClassChng;
   BYTE thiefLvlPreClassChng;

   //171-172 Number of attacks per two rounds (1st/2d attacks, unarmed)
   BYTE attacksPer2Rnds;
   BYTE attacksPer2Rnds2nd;

   //   173-174 Dice of Damage
   //   175-176 Type of Damage Die Rolled
   //   177-178 Damage Bonus
   BYTE dmgDiceNbr;
   BYTE dmgDiceNbr2nd;
   BYTE dmgDiceSides;
   BYTE dmgDiceSides2nd;
   BYTE dmgBonus;
   BYTE dmgBonus2nd;

   //179     60 - Base AC    This field is not reset by UA.
   BYTE ac; // 60 - base AC;

   //180-181 ???             Set to zero for new character.
   WORD foo9;

   //182     Pre-Drain HP Total
   BYTE hpPreDrain;

   //183     Item Use Indicator
   //   1 = Magic-User
   //   2 = Cleric
   //   4 = Thief
   //   8 = Fighter
   //   16 = Knight
   //   32 = ???
   //   64 = Paladin or Ranger
   //   128 = ???
   //   Add numbers together for multi-class combinations.  This field determines usable equipment.  Unfortunately for us hackers,
   //   the UA program resets this field whenever a character is played.  The available  equipment for a Knight in the item files is weird.
   BYTE itemMask;

   //184     HP (As rolled; ignores Con bonus and draining)
   BYTE hp; // as rolled, no con bonus
   
   //185-187 ???             Set to zero for new character.
   BYTE foo10;
   WORD foo11;

   //188     Icon ID         0-48
   BYTE iconId; // 0-48

   //189     March Order     
   //   8 = Has not begun adventuring
   //   0-7 = Position in marching order when removed from party
   BYTE uniquePartyID; // 0-7, 8=not in party yet

   //190     ???             Set to zero for new character.
   BYTE foo12;

   //191     Special Ability Flags:
   //   1 = Affected by Dispel Evil
   //   2 = Mammal
   //   4 = Penalty to Hit Dwarf
   //   8 = Extra Damage from Ranger
   //   16 = Snake
   //   32 = Penalty to Hit Gnome
   //   64 = Animal
   //   128 = Dwarf has bonus to hit you
   BYTE specAbFlags; 

   //192     More Special Ability Flags:
   //   1 = True Giant
   //   2 = May be Held or Charmed
   //   4 = Gnome has bonus to hit you
   //   8 = Immune to Death Magic
   //   16 = Immune to Poison
   //   32 = Immune to Decapitation by Vorpal Sword
   //   64 = Immune to Confusion
   BYTE specAbFlags2;

   //193     Number of Item Bundles Carried
   BYTE itemBundlesCarried;

   //194     Number of Hands Full
   BYTE nbrHandsFull;

   //195     Total (sum of maxima) Magic Protective Bonus
   BYTE magicProtBonus;

   //196     Magic Resistance (Percentage)
   BYTE magicResistance; // percentage

   //197     Train for Level?
   //   0 = No
   //   1 = Yes
   //   Automatically reset by UA.
   BYTE rdyToTrain; // 0=no,1=yes

   //198-338 Memorized Spells (one byte per spell; will decode later)
   BYTE spellsMemorized[141];

   //339-354 Spell Book (one bit per spell; will decode later.  Spells
   //   are designated by integer value from 1 to 126 if memorized,
   //   or 129 to 254 if not memorized.  For Cleric, Paladin, or Ranger,
   //   spell book is automatically updated to include all due spells.)
   BYTE spellBook[16];

   //355-361 Number of Cleric Spells (Levels 1-7)
   BYTE numClericSpellsByLvl[7];

   //362-363 Not Used
   WORD foo14;

   //364-366 Number of Druid Spells (Levels 1-3)
   BYTE numDruidSpellsByLvl[3];

   //367-372 Not Used
   BYTE foo15[6];

   //373-381 Number of Mage Spells (Levels 1-9)
   //    Note:  Offsets 355-381 are automatically reset by UA whenever the character
   //    attempts to memorize spells.  There is no point in editing these bytes.
   BYTE numMageSpellsByLvl[9];

   //382     Active Flag     
   //   0 = Down
   //   1 = Active
   BYTE active; // 0=down, 1=active

   //383     ???             Set to zero for new character.
   BYTE foo16;

   //384     60 - Adjusted THAC0     Reset by UA program.
   BYTE adjThac0; // 60 - adj thac0

   //385     60 - AC
   BYTE adjAC; // 60 - adj ac

   //386     ???             Set to 48 for new character.
   BYTE foo17;

   //387-394 Repeats 171-178, but for armed character.
   //BYTE dmgUsingCurrWpn[8]; // don't care
   BYTE attacksPer2RndsEquipped;
   BYTE attacksPer2Rnds2ndEquipped;
   BYTE dmgDiceNbrEquipped;
   BYTE dmgDiceNbr2ndEquipped;
   BYTE dmgDiceSidesEquipped;
   BYTE dmgDiceSides2ndEquipped;
   BYTE dmgBonusEquipped;
   BYTE dmgBonus2ndEquipped;

   //395     Current HP
   BYTE adjHp; // including con bonus;

   //396     Movement Rate (Adjusted)
   BYTE adjMove;

   //397     Monster slot (Set to zero for player character)
   BYTE monsterIndex; // 0 for player

   //MONST*.DAT files only:  These files always end with the following bytes:
   //398-413    Item codes for items carried by monster or NPC
   //414-429    Quantity code for each item carried
   //430-449    Special ability codes for special abilities of monster or NPC   
   BYTE itemCarried[16];
   BYTE itemQty[16];
   BYTE specAbCodes[20];
};

struct ImportCCHSpecAb
{
   BYTE code;
   BYTE constant; // 0 if constant;
   WORD duration; // in rounds, 0 if perm
   BYTE level; // 255 = NA
   BYTE constant2; // 0 if constant
   WORD counter; // inc by 10 from bytes 4 and 5
   BYTE next; // 254 if another spec ab follows, else 0
   BYTE next2;  // 104 if another follows, else 0
};

struct ImportFRUACharacter
{
   //0-3   Pointer to next character.  Always set to zero for new characters.
   BYTE foo1;
   BYTE foo2;
   BYTE foo3;
   BYTE foo3a;

   //4-7   Special Ability: 
   //   0/0/0/0 if no special abilities
   //   0/0/254/104 (or any other non-zero value) if one or more special abilities
   WORD saCount; // not needed
   WORD specAbs; // 0/0 if no special abilities, 254/104 if one or more special abilities

   //8-11  Inventory:
   //  0/0/0/0 if no items
   //  0/0/76/95 (or any other non-zero value) if one or more items carried
   WORD carryCount; // not needed
   WORD isCarry; // 0/0 if no items, 76/95 (0x1E0F) if one or more items carried - 74/57
   
   //12-15  As 8-11 but for item in weapon hand
   WORD whCount;
   WORD isWpnHand;

   //16-19  As 8-11 but for item in shield hand
   WORD shCount;
   WORD isShHand;

   //20-23  As 8-11 but for item worn as armor
   WORD arCount;
   WORD isArmor;

   //24-27  As 8-11 but for item worn as gauntlets
   WORD gCount;
   WORD isGauntlet;

   //28-31  As 8-11 but for item worn as helmet
   WORD hCount;
   WORD isHelmet;

   //32-35  As 8-11 but for item worn as belt
   WORD bCount;
   WORD isBelt;

   //36-39  As 8-11 but for item worn as robe
   WORD rCount;
   WORD isRobe;

   //40-43  As 8-11 but for item worn as cloak
   WORD cCount;
   WORD isCloak;

   //44-47  As 8-11 but for item worn as boots
   WORD btCount;
   WORD isBoots;

   //48-51  As 8-11 but for item worn as ring
   WORD rgCount;
   WORD isRing;

   //52-55  As 8-11 but for item worn as 2nd ring
   WORD rg2Count;
   WORD isRing2;

   //56-59  As 8-11 but for item carried as arrows
   WORD amCount;
   WORD isAmmo;

   //60-63  As 8-11 but for item carried as bolts
   WORD boltCount;
   WORD isBolts;

   //64-67  ???     Apparently reserved for additional items, but not used.
   WORD unknownItemCount;
   WORD unknownItems;

   //68-71  XP Total (Current)
   DWORD exp;

   //72-75  XP Total (Before Draining)
   DWORD expPreDrain;

   //76-77  Platinum Pieces
   WORD platinum;

   //78-79  Gems
   WORD gems;

   //80-81  Jewels
   WORD jewelry;

   //82-83  Age in Years
   WORD age;

   //84-85  XP for Killing          Set to zero for player character.
   WORD expWorth; // for killing, 0 for characters

   //86-87  Current Encumbrance in Coins (Since this is another field that is reset by UA, we need not worry about setting it correctly)
   WORD encumb; // in gold pcs

   //   88     Race            
   //     0 = Elf
   //     1 = Half-Elf
   //     2 = Dwarf
   //     3 = Gnome
   //     4 = Halfling
   //     5 = Human
   //     6 = Monster
   BYTE race; // 0=elf,1=halfelf,2=dwarf,3=gnome,4=halfling,5=human,6=monster

   //89     Current Class    
   //   0 = Cleric
   //   1 = Knight (Not Used)
   //   2 = Fighter
   //   3 = Paladin
   //   4 = Ranger
   //   5 = Magic-User
   //   6 = Thief
   //   7 = Monk (Not Used)
   //   8 = C/F
   //   9 = C/F/MU
   //   10 = C/R
   //   11 = C/MU
   //   12 = C/T
   //   13 = F/MU
   //   14 = F/T
   //   15 = F/MU/T
   //   16 = MU/T
   BYTE charClass; 

   //90     Undead Type      
   //   0 = Not Undead
   //   1 = Skeleton
   //   2 = Zombie
   //   3 = Ghoul
   //   4 = Shadow
   //   5 = Wight
   //   6 = Ghast
   //   7 = Wraith
   //   8 = Mummy
   //   9 = Spectre
   //   10 = Vampire
   //   11 = Ghost
   //   12 = Lich
   //   13 = Special
   BYTE undead;

   //91     ???             Not needed for new characters.
   BYTE foo6;

   //92     Gender          
   //   0 = Male
   //   1 = Female
   BYTE gender; // 0=male,1=female

   //93     Alignment       
   //   0 = LG   3 = NG   6 = CG
   //   1 = LN   4 = TN   7 = CN
   //   2 = LE   5 = NE   8 = CE
   BYTE alignment; 

   //94     Status          
   //   0 = Okay
   //   1 = Animated
   //   2 = Temp Gone
   //   3 = Running
   //   4 = Unconscious
   //   5 = Dying
   //   6 = Dead
   //   7 = Petrified
   //   8 = Gone
   //   9 = Awaiting Summons
   BYTE status; 
   
   //95     Combat Mode     
   //   0 = PC/NPC
   //   1 = Monster
   //   Other values yield undesirable results.
   BYTE combatMode; // 0=pc/npc, 1=monster

   //96-110 Name  15 ASCII Characters
   //111  Set to zero (To terminate Name string)
   BYTE name[16]; // 15 plus zero terminator

   //   112-113 Str (raw/modified)
   //   114-115 Int 
   //   116-117 Wis
   //   118-119 Dex
   //   120-121 Con
   //   122-123 Cha
   BYTE str;
   BYTE strMod;
   BYTE intel;
   BYTE intMod;
   BYTE wis;
   BYTE wisMod;
   BYTE dex;
   BYTE dexMod;
   BYTE con;
   BYTE conMod;
   BYTE cha;
   BYTE chaMod;
   //124-125 Extraordinary (percentile) Str (modified/raw)
   BYTE strExt; // 0.100
   BYTE strExtMod;

   //126     ???             Set to zero for new character.
   BYTE foo7;

   //127     60 - Base THAC0     Reset by UA program.
   BYTE thac0; //60 - Base THAC0

   //128  Cure Disease Flag:
   //   0  = Cannot cure disease
   //   1+ = Number of times that character can cure disease
   BYTE cureDisease; // 0=cannot, else nbr times

   //129     Maximum HP
   BYTE maxHp;

   //130     Size Indicator
   //   1 = small combat icon
   //   2 = tall combat icon
   //   3 = wide combat icon
   //   4 = big combat icon
   //   5 = huge combat icon (2 tall x 3 wide)
   //   Add 128 to this value if button for "Large even if 1x1" has been selected.
   //   Presumably, UA regards as "large" all combatants for whom this byte is set to a value greater than 1.
   BYTE size; 

   //131     Save vs. paralysis/poison/death magic
   BYTE svPaPoDm;

   //132     Save vs. petrification/polymorph
   BYTE svPePo;

   //133     Save vs. rod/staff/wand
   BYTE svRoStWa;

   //134     Save vs. breath weapon
   BYTE svBw;

   //135     Save vs. spell
   BYTE svSp;

   //136     Movement Rate (Base)
   BYTE move;

   //137     Current Level (in highest active class)
   BYTE level; // in highest active class;

   //138     Level in Former Class
   BYTE levelFormerClass;

   // Thieving skills are adjusted for level
   // and race but not for dexterity or armor.

   //139     Pick Pocket
   BYTE pickPockets;
   //140     Open Lock
   BYTE openLocks;
   //141     Find/Remove Traps
   BYTE findRemTraps;
   //142     Move Silently
   BYTE moveSilent;
   //143     Hide in Shadows
   BYTE hideShadows;
   //144     Hear Noise
   BYTE hearNoise;
   //145     Climb Walls
   BYTE climbWalls;
   //146     Read Languages
   BYTE readLang;

   //reserved for 128 + Morale
   BYTE morale; // 128 + morale if non-zero

   //Warrior Level   Equivalent fighter level for "sweep"
   BYTE fighterSweepLevel;

   //149     ???             Set to zero for new character.
   BYTE foo8;

   //   Pre-Drain levels
   //   150     Cleric Level 
   //   151     Knight Level
   //   152     Fighter Level
   //   153     Paladin Level
   //   154     Ranger Level 
   //   155     Mage Level 
   //   156     Thief Level
   BYTE clericLvlPreDrain;
   BYTE knightLvlPreDrain;
   BYTE fighterLvlPreDrain;
   BYTE paladinLvlPreDrain;
   BYTE rangerLvlPreDrain;
   BYTE mageLvlPreDrain;
   BYTE thiefLvlPreDrain;

   // current levels
   //   157     Cleric Level (Current)
   //   158     Knight Level
   //   159     Fighter Level
   //   160     Paladin Level
   //   161     Ranger Level
   //   162     Mage Level
   //   163     Thief Level
   BYTE clericLvl;
   BYTE knightLvl;
   BYTE fighterLvl;
   BYTE paladinLvl;
   BYTE rangerLvl;
   BYTE mageLvl;
   BYTE thiefLvl;

   // levels before changing class; Human only
   //   164     Cleric Level 
   //   165     Knight Level
   //   166     Fighter Level
   //   167     Paladin Level
   //   168     Ranger Level
   //   169     Mage Level
   //   170     Thief Level
   BYTE clericLvlPreClassChng;
   BYTE knightLvlPreClassChng;
   BYTE fighterLvlPreClassChng;
   BYTE paladinLvlPreClassChng;
   BYTE rangerLvlPreClassChng;
   BYTE mageLvlPreClassChng;
   BYTE thiefLvlPreClassChng;

   //171-172 Number of attacks per two rounds (1st/2d attacks, unarmed)
   BYTE attacksPer2Rnds;
   BYTE attacksPer2Rnds2nd;

   //   173-174 Dice of Damage
   //   175-176 Type of Damage Die Rolled
   //   177-178 Damage Bonus
   BYTE dmgDiceNbr;
   BYTE dmgDiceNbr2nd;
   BYTE dmgDiceSides;
   BYTE dmgDiceSides2nd;
   BYTE dmgBonus;
   BYTE dmgBonus2nd;

   //179     60 - Base AC    This field is not reset by UA.
   BYTE ac; // 60 - base AC;

   //180-181 ???             Set to zero for new character.
   WORD foo9;

   //182     Pre-Drain HP Total
   BYTE hpPreDrain;

   //183     Item Use Indicator
   //   1 = Magic-User
   //   2 = Cleric
   //   4 = Thief
   //   8 = Fighter
   //   16 = Knight
   //   32 = ???
   //   64 = Paladin or Ranger
   //   128 = ???
   //   Add numbers together for multi-class combinations.  This field determines usable equipment.  Unfortunately for us hackers,
   //   the UA program resets this field whenever a character is played.  The available  equipment for a Knight in the item files is weird.
   BYTE itemMask;

   //184     HP (As rolled; ignores Con bonus and draining)
   BYTE hp; // as rolled, no con bonus
   
   //185-187 ???             Set to zero for new character.
   BYTE foo10;
   WORD foo11;

   //188     Icon ID         0-48
   BYTE iconId; // 0-48

   //189     March Order     
   //   8 = Has not begun adventuring
   //   0-7 = Position in marching order when removed from party
   BYTE uniquePartyID; // 0-7, 8=not in party yet

   //190     ???             Set to zero for new character.
   BYTE foo12;

   //191     Special Ability Flags:
   //   1 = Affected by Dispel Evil
   //   2 = Mammal
   //   4 = Penalty to Hit Dwarf
   //   8 = Extra Damage from Ranger
   //   16 = Snake
   //   32 = Penalty to Hit Gnome
   //   64 = Animal
   //   128 = Dwarf has bonus to hit you
   BYTE specAbFlags; 

   //192     More Special Ability Flags:
   //   1 = True Giant
   //   2 = May be Held or Charmed
   //   4 = Gnome has bonus to hit you
   //   8 = Immune to Death Magic
   //   16 = Immune to Poison
   //   32 = Immune to Decapitation by Vorpal Sword
   //   64 = Immune to Confusion
   BYTE specAbFlags2;

   //193     Number of Item Bundles Carried
   BYTE itemBundlesCarried;

   //194     Number of Hands Full
   BYTE nbrHandsFull;

   //195     Total (sum of maxima) Magic Protective Bonus
   BYTE magicProtBonus;

   //196     Magic Resistance (Percentage)
   BYTE magicResistance; // percentage

   //197     Train for Level?
   //   0 = No
   //   1 = Yes
   //   Automatically reset by UA.
   BYTE rdyToTrain; // 0=no,1=yes

   //198-338 Memorized Spells (one byte per spell; will decode later)
   BYTE spellsMemorized[141];

   //339-354 Spell Book (one bit per spell; will decode later.  Spells
   //   are designated by integer value from 1 to 126 if memorized,
   //   or 129 to 254 if not memorized.  For Cleric, Paladin, or Ranger,
   //   spell book is automatically updated to include all due spells.)
   BYTE spellBook[16];

   //355-361 Number of Cleric Spells (Levels 1-7)
   BYTE numClericSpellsByLvl[7];

   //362-363 Not Used
   WORD foo14;

   //364-366 Number of Druid Spells (Levels 1-3)
   BYTE numDruidSpellsByLvl[3];

   //367-372 Not Used
   BYTE foo15[6];

   //373-381 Number of Mage Spells (Levels 1-9)
   //    Note:  Offsets 355-381 are automatically reset by UA whenever the character
   //    attempts to memorize spells.  There is no point in editing these bytes.
   BYTE numMageSpellsByLvl[9];

   //382     Active Flag     
   //   0 = Down
   //   1 = Active
   BYTE active; // 0=down, 1=active

   //383     ???             Set to zero for new character.
   BYTE foo16;

   //384     60 - Adjusted THAC0     Reset by UA program.
   BYTE adjThac0; // 60 - adj thac0

   //385     60 - AC
   BYTE adjAC; // 60 - adj ac

   //386     ???             Set to 48 for new character.
   BYTE foo17;

   //387-394 Repeats 171-178, but for armed character.
   //BYTE dmgUsingCurrWpn[8]; // don't care
   // Number of attacks per two rounds (1st/2d attacks, unarmed)
   BYTE attacksPer2RndsEquipped;
   BYTE attacksPer2Rnds2ndEquipped;
   BYTE dmgDiceNbrEquipped;
   BYTE dmgDiceNbr2ndEquipped;
   BYTE dmgDiceSidesEquipped;
   BYTE dmgDiceSides2ndEquipped;
   BYTE dmgBonusEquipped;
   BYTE dmgBonus2ndEquipped;


   //395     Current HP
   BYTE adjHp; // including con bonus;

   //396     Movement Rate (Adjusted)
   BYTE adjMove;

   //397     Monster slot (Set to zero for player character)
   BYTE monsterIndex; // 0 for player
   /*
   CCH files only:

      File size for an unequipped character who is unaffected by spells and has
      no special abilities is 398.  Add 18 bytes per item of equipment.  Add ten
      bytes per special ability or spell effect as follows:

      Dwarf or Gnome:  +3 abilities
      Elf, Half-Elf, or Halfling:  +1 ability
      Paladin or Ranger:  +1 ability

      Each special ability record comprises ten bytes and is formatted as follows:

      0     Code for first special ability
      1     Set to zero for constant effect
      2-3   Duration of effect in rounds (0 = permanent)
      4     Level of effect (255 = N/A)
      5     Set to zero for constant effect
      6-7   Counter (incremented by 10 from bytes 4 and 5)
      8     Set to 254 if another ability follows (0 otherwise)
      9     Set to 104 if another ability follows (0 otherwise)

      struct ImportCCHSpecAb { }
      */
};

struct ImportUADamage
{
   //Number of Damage Dice
   BYTE dice;

   //Type of Damage Dice
   BYTE sides;

   //Damage Bonus
   BYTE bonus;
};

//Data about items available in the UA game is kept in two files, both
//   of which are located in the directory "\frua\disk1" (assuming that
//   you installed your UA game with the default options).  These files 
//   are called "item.dat" and "items.dat".  The module files (geoxxx.dat)
//   and monster/NPC files (monstxxx.dat) have pointers to the "item.dat"
//   file, while that file and the ".cch" files contain pointers to the
//   "items.dat" file.  It is thus possible to change the contents of the
//   "item.dat" file for the duration of a single adventure and transfer
//   characters from that adventure to another adventure without loss; 
//   on the other hand, changes to the items.dat file are of necessity 
//   permanent.  Each of these files is described separately in the files
//   "item.txt" and "items.txt".

//
// items.dat structure - general item classes
//
//The file "items.dat" contains 128 records numbered from 0 to 127. There are many records that are not 
//referenced from the "item.dat" file; it appears (although I cannot be certain of this) that these records 
//are not used. If so, they may be usable for expansions to possible items. The known records are allocated as follows: 
struct ImportUAItems
{
   //Byte 0 	Location where item is to be carried:
   //   0 = Weapon Hand
   //   1 = Shield Hand
   //   2 = Body (Armor)
   //   3 = Hands (Gauntlets)
   //   4 = Head (Helm)
   //   5 = Waist (Belt)
   //   6 = Body (Robe)
   //   7 = Back (Cloak)
   //   8 = Feet (Boots)
   //   9 = Fingers (Ring)
   //   10 = Ammo Quiver
   //   For scrolls, the value is interpreted as follows:
   //   10 = Protection
   //   11 = Mage
   //   12 = Cleric 
   BYTE slot;  // location carried

   //Byte 1 	Hands required to use
   BYTE twohanded; // num hands needed


   ImportUADamage vs_large;

   //Byte 5 	Rate of Fire per two rounds (if ROF = 1, value is 2 for one-handed weapon and 1 for two-handed weapon)
   BYTE rate; // per 2 rounds; if 1, it = 2 for 1-handed and 1 for 2-handed

   //Byte 6 	Protection Value Value = 188 - Base AC for Armor and Bracers
   //          Value = AC Bonus - 128 for other protective gear
   //          Value = 0 for non-protective items 
   BYTE protection;

   //Byte 7 	0 = Not a Weapon
   //          1 = Cutting Weapon
   //          128 = Blunt Weapon 
   BYTE cutting_or_blunt; // 0 not wpn, 1 = cut wpn, 128 = blunt wpn

   //Byte 8 	Melee Class
   //          0 = Item usable in melee
   //          128 = Item not usable in melee
   //          This byte appears to be meaningless for protective items. 
   BYTE melee; // usable in melee


   ImportUADamage vs_small; // size small/medium

   //Byte 12: 	Range of item
   BYTE range;

   //Byte 13 	As offset 183 in .cch file, with one bit set per class that can use this item.
   BYTE classes; // 1=MU,2=Cl,4=Th,8=Fi,64=Pal,Rgr

   //Byte 14: 	Missile Type:
   //0 = Not a Weapon
   //   4 = Hand Held Weapon, Cannot be Thrown
   //   10 = Sling Weapon (requires no ammo)
   //   11 = Bow (requires Arrow)
   //   15 = Fine Bow
   //   18 = Ammo (Arrow or Bolt)
   //   20 = Hand Held Weapon, Can be Thrown
   //   26 = Thrown Weapon
   //   138 = Crossbow (requires Bolt) 
   BYTE weapon_type; // 0=NoWpn,4=HandHeld,10=Sling,11,15=Bow,18=ammo,20=HandHeldOrThrow,26=Thrown,138=Crossbow

   //Byte 15: 	Set to zero
   BYTE foo;
};

//
// item.dat structure - specific items
//
//The file "item.dat" contains 254 records (numbered from 1 to 254), with each record comprising 18 bytes. 
//This file thus permits editing of all possible items except for item 0 (no item) and item 255 (the random Mage spell scroll). 
//Numbering each byte in each record from 0 to 17, we have the following format: 
struct ImportUAItem
{
   //Byte 0: Pointer to record in "items.dat" file. Item 73 is a bundle of scrolls. 
   BYTE ptr; // ptr to record in items.dat file

   //Bytes 1 to 3: Name of item, read from byte 3 to byte 1. The vocabulary is given in the file "vocab.txt". If byte 3 is 77 (for "bundle of"), byte 2 gives the (original) number of scrolls in the bundle and byte 1 is 39 (for "scroll").
   BYTE name3;  // index into vocab list
   BYTE name2;
   BYTE name1;

   //Bytes 4 and 5: Encumbrance of item in coins (where 10 coins = one pound) 
   WORD encumbrance; // in coins, 10 coins = 1 pound

   //Bytes 6 and 7: Price of item in PP 
   WORD price; // in platinum

   //Byte 8: Magic Bonus of Item 
   BYTE magic_bonus;

   //Byte 9: Secondary Code:
   //Equals byte 8 for protective items other than armor and bracers.
   //   Equals 1 for magic rings other than rings of protection.
   //   Equals 1 for scroll of prot. from dragons. 
   BYTE secondary_code;

   //Byte 10: Indicates whether or not item is ready 
   BYTE ready;

   //Byte 11: Indicates whether or not item has been identified:
   //   0 = Item is fully identified
   //   1 = First name code (byte 3) concealed
   //   2 = Second name code (byte 2) concealed
   //   4 = Third name code (byte 1) concealed
   //   Add values for multiple parts of name concealed. It is not recommended that you conceal all three bytes or that you leave only zero (null) codes available. 
   BYTE identified;

   //Byte 12: Set to 1 if item is cursed (cannot be unreadied once readied) 
   BYTE cursed;

   //Byte 13: Number of items in "regular" bundle (0 = cannot be joined). For a bundle of scrolls, this byte gives the actual number of scrolls that are currently in the bundle. One item entry per scroll follows this item record in a character file entry. 
   BYTE numitems; // bundle qty

   //Byte 14: Number of charges (0 = permanent item) 
   BYTE numcharges; // 0 = permanent

   //Byte 15: Code for magical effect: See byte 16 for interpretation
   BYTE magicalcode;

   // Byte 16: Special Code
   //    0 = Byte 15 is spell code
   //    1-127 = Item is Scroll (bytes 14-16 give codes for spells on scroll)
   //    128 = Byte 15 is special ability code
   //    129 = Item is Ring of Wizardry
   //    (byte 15 = 0) 
   //    131 = Item is Gauntlets of Ogre Power
   //    (byte 15 = 0) 
   //    133 = Item is Girdle of Giant Strength
   //    (byte 15 = Str - 18) 
   BYTE special_code;

   //Byte 17: Always set to zero 
   BYTE foo;
};

struct UALevelEntryPoint
{
   // RBT: after testing, I think the hack docs are backwards
   // Entry Row
   BYTE entryRow;
   // Entry Col
   BYTE entryCol;
   
   /*
   // Entry Col
   BYTE entryCol;
   // Entry Row
   BYTE entryRow;
   */

   // Entry Facing  (0=N, 2=E, 4=S, 6=W)
   BYTE entryFacing;
   // unknown
   BYTE unknown;
};

struct UALevelRestEvents
{
   // Minutes
   BYTE minutes;
   // Not used?
   BYTE unknown;
   // Event
   BYTE eventCode;
   // % Chance / No Rest (+128)
   BYTE chancePcnt_NoRestFlag;
};

struct UALevelStepEvents
{
   // # of Steps
   BYTE numSteps;
   // Not used?
   BYTE unused;
   // Step Event
   BYTE eventCode;
   //  Zone flags:
   //      0 = All zones 
   //
   //      Add the following values to exclude zones:
   //         Zone1 (+1) Zone3 (+4)  Zone5 (+16)  Zone7 (+64)
   //         Zone2 (+2) Zone4 (+8)  Zone6 (+32)  Zone8 (+128)
   BYTE zoneFlags;   
};

// Grids start at offset 322 of the geo###.dat
//  Each grid in a geo map uses 6 bytes
//  there are 576 grinds
struct UAMapCell
{
   // Wall Slots:
   //    0    = Blank Wall
   //    1-15 = Slots #1-15 
   //
   // Wall Types:
   //      0   = Open
   //      16  = Secret
   //      32  = Locked
   //      48  = Locked Secret (16+32)
   //      64  = Wizard Locked
   //      80  = Wizard Locked Secret (16+64)
   //      96  = Locked: Key #1
   //      112 = Locked: Key #2
   //      128 = Locked: Key #3
   //      144 = Locked: Key #4
   //      160 = Locked: Key #5 
   //      176 = Locked: Key #6
   //      192 = Locked: Key #7
   //      208 = Locked: Key #8
   //      224 = Blocked
   //      240 = False Door (16+224)


    // 1- North Wall
   BYTE northWall;
   //2- East Wall
   BYTE eastWall;
   //3- South Wall
   BYTE southWall;
   //4- West Wall
   BYTE westWall;


   //5- Event # Stored (Actual event is elsewhere)
   BYTE eventCode;

   //Backdrop #
   //   0 = Backdrop #1
   //   1 = Backdrop #2
   //   2 = Backdrop #3
   //   3 = Backdrop #4
   //
   //   Zone #
   //   0  = Zone #1
   //  4  = Zone #2
   //   8  = Zone #3
   //   12 = Zone #4
   //   16 = Zone #5
   //   20 = Zone #6
   //   24 = Zone #7
   //   28 = Zone #8

   //6- Backdrop + Zone
   BYTE backdropAndZone;
};

struct UALevelEvent
{
   //01  Event Type#
   BYTE eventType;

   // 02  Do Only Once Flag:  
   //       0  No 
   //       1  Yes
   // Chain: 
   //       0  Always 
   //       2  If Happens
   //       4  If Doesn't Happen
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
   BYTE eventTrigger;

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

   // event condition to check
   BYTE eventTriggerData;

   // chain to event
   BYTE chainEvent;

   // event data, specific to each event type
   BYTE eventData[16];
};

struct UALevelStrings
{
   // 5798-5799      Number of individual text boxes
   WORD CompressedLengthForAllStrings; 

   // 5800 A series of 400 bytes, giving the lengths of the  strings in each individual text box
   BYTE StringLength[MAX_FRUA_LEVEL_STRINGS];

   // 6200		A series of 400 variable-length records giving
   //   the text strings in compressed (6 bits per character)
   //   format.  Note that by default the first text string
   //   is simply the character "\".
   //
   //   The longest a string can be is 228 characters (172 compressed). A single
   //   dungeon can have just over 39 completely filled-in text boxes. The text
   //   in a dungeon is 100% full when you have typed 8960 characters, because
   //   UA puts a 0-byte at the end of every string after compression.
   //
   //   Incidentally, the ASCII code for @ = "64"; however in the compression
   //   algorithm that corresponds to the code of 0, as UA only stores values
   //   from 0-63. So, it thinks the '@' character is an end of string, which
   //   is why it can badly screw up designs if you use it in text boxes.
   //
   //  32-63 are as normal; 1-31 are added 64 to for the range 65-95, but 0=0.
   //
   // The compressed name format uses six bits per character, with three bytes being sufficient for
   // four characters.  Since six bits are insufficient to store all eight bytes of an ASCII character code,
   // only a subset of the ASCII character set is available.  ASCII values
   // of zero and 32-63 are used as is.  Values in the range 1-31 have an
   // implicit "1" bit appended to them, shifting the range to 65-95.
   BYTE Strings[6761];
};

// read from offset 0
struct UALevelHeader
{
   // RBT: i cant find any explanation for these bytes
   BYTE Header[26];

   //26           Module Height
   //27           Module Width
   BYTE height;
   BYTE width;

   //28-30        Wall Slots #1-3 (Set to 255 in overland modules)
   BYTE wall1;
   BYTE wall2;
   BYTE wall3;

   //31           Area View flag
   BYTE areaViewFlag;

   //32-35        Dungeon:  Backdrop Slots #1-4
   //            Overland:  Offset 32 = Big picture slot for map
   //                       Offsets 33-35 = 255
   BYTE backdropSlot1;
   BYTE backdropSlot2;
   BYTE backdropSlot3;
   BYTE backdropSlot4;

   //36           Dungeon Combat Art Slot
   BYTE dungeonCombatArtSlot;

   //37           Wilderness Combat Art Slot
   BYTE wildernessCombatArtSlot;

   //38           Entry Col #1
   //39           Entry Row #1
   //40           Entry Facing #1
   //41           Not used?
   //Preceding four bytes are repeated for entry points 2-8.
   UALevelEntryPoint entryPoints[8];

   //70           Zone 1 Minutes
   //71           Not used?
   //72           Zone 1 Event
   //73           #1 % Chance/ No Rest (+128)
   //Preceding four bytes are repeated for rest events 2-8.
   UALevelRestEvents restEvents[8];

   //102           Step 1 # of Steps
   //103           Not used?
   //104           Step Event #1
   //105           Step 1 Zone flags:
   //0 = All zones (add the following values to exclude zones):
   //Zone1 (+1) Zone3 (+4)  Zone5 (+16)  Zone7 (+64)
   //Zone2 (+2) Zone4 (+8)  Zone6 (+32)  Zone8 (+128)
   //Preceding four bytes are repeated for step events 2-8.
   UALevelStepEvents stepEvents[8];

   //134		"blank" (in dungeon; zeroed out in overland)
   BYTE unusedBytes[8];

   //142           Module Name
   BYTE moduleName[16];

   //158-285       Zone Names (15 characters + 0 terminator each)
   BYTE zoneNames[8][16];

   //286           Monster Summoned
   BYTE whichMonsterSummoned;

   //287           Undead Turn Modifier
   BYTE undeadTurnModifier;

   //288-295       Overland- Wilderness Slots 1-8 (zeroed out in dungeon modules)
   BYTE overlandWildernessArtSlots[8];

   //296-311       Overland - Zone 1-8 Text String Slots. Two bytes are allocated per zone. These bytes are zeroed out in dungeon modules.
   WORD zoneMsgStringIds[8];

   //312           Camp Picture
   BYTE campPic;

   //313           Treasure Picture
   BYTE treasurePic;
};

// this thing is pretty big.
// there will be one for each level
struct UALevelData
{
   // This flag indicates whether we actually populated
   // this data from a geo*.dat file.
   //
   // I'm not sure if these files are sequential or have gaps.
   bool isPopulated;
   UALevelHeader header;
   UAMapCell map[MAX_FRUA_LEVEL_GRIDS];
   UALevelEvent events[MAX_FRUA_EVENTS];
   UALevelStrings strings;
};

#pragma region FRUA Docs

//*********************************************************************
//
// What follows is a bit of description for the default spells in 
// in FRUA. 
//
//*********************************************************************
// this is the spell table from the FRUA manual
/*
   Spell Parameters List

   This is a listing of spells available to player characters as they gain in
   level. The following are abbreviations used in the list:

   Cmbt    = Combat only spell
   Camp    = Camp only spell
   Both    = Camp or Combat spell
   T       = Touch Range
   dia     = diameter
   s       = squares
   c       = cone
   All     = All characters in combat
   r       = combat rounds
   t       = turns
   /|v|    = per level of caster
   targets = aim at each target.


   First-Level Clerical Spells

   SPELL NAME              WHEN    BARGE   AREA    DURATION
   -------------------------------------------------------
   Bless                   Both      6     5 dia      6r
   Cure Light Wounds       Both      T       1
   Cause Light Wounds      Cmbt      T       1
   DetectMagic             Both      0       1        1t
   Protection from Evil    Both      T       1        3r/|v|
   Protection from Good    Both      T       1        3r/|v|
   Resist Cold             Both      T       1        1t/|v|


   Second-Level Clerical Spells

   SPELL NAME              WHEN      RANGE   AREA    DURATION
   -------------------------------------------------------
   Find Traps              Camp      3       1       3t
   Hold Person             Cmbt      6       1-3     4r+1/|v|
   Resist Fire             Both      T       1       1t/|v|
   Silence 15' Radius      Cmbt      12      3 dia   2r/|v|
   Slow Poison             Both      T       1       1 hour/|v|
   Snake Charm             Cmbt      3       All     5-8r
   Spiritual Hammer        Cmbt      3       1       1r/|v|


   Third-Level Clerical Spells

   SPELL NAME             WHEN    RANGE   AREA    DURATION
   -------------------------------------------------------
   Cure Blindness         Both       T      1
   Cause Blindness        Cmbt       T      1
   Cure Disease           Camp       T      1
   Cause Disease          Cmbt       T      1
   Dispel Magic           Both       6      3x3 s
   Prayer                 Both       0      All     1r/|v|
   Remove Curse           Both       T      1
   Bestow Curse           Both       T      1       1t/|v|


   Fourth-Level Clerical Spells

   SPELL NAME              WHEN      RANGE   AREA     DURATION
   -------------------------------------------------------
   Cure Serious Wounds     Both      T       1
   Cause Serious Wounds    Cmht      T       i
   Neutralize Poison       Both      T       1
   Poison                  Cmbt      T       1
   Protection from
   Evil 10' Radius      Both      T       2 dia    1t/|v|
   Sticks to Snakes        Cmbt      3       1        2r/|v|


   Fifth-Level Clerical Spells

   SPELL NAME              WHEN     RANGE    AREA    DURATION
   -------------------------------------------------------
   Cure Critical Wounds    Both       T        1
   Cause Critical Wounds   Cmbt       T        1
   Dispel Evil             Cmbt       T        1      1r/|v|
   Flame Strike            Cmbt       6        1
   Raise Dead              Camp       3        1
   Slay Living             Cmbt       3        11


   Sixth-Level Clerical Spells

   SPELL NAME              WHEN     RANGE     AREA    DURATION
   -------------------------------------------------------
   Blade Barrier           Cmbt       3     Special    3r/|v|
   Harm                    Cmbt       T     1
   Heal                    Both       T     1


   Seventh-Level Clerical Spells

   SPELL NAME              WHEN     RANGE    AREA    DURATION
   -------------------------------------------------------
   Destruction             Cmbt       T      1
   Energy Drain            Cmbt       T      1
   Resurrection            Camp       T      1
   Restoration             Damp       T      1


   First-Level Druid Spells
   (For High-Level Rangers)

   SPELL NAME              WHEN    RANGE     AREA    DURATION
   -------------------------------------------------------
   Detect Magic            Both      0        1       12r
   Entangle                Cmbt      8       4 dia    1t
   Faerie Fire             Cmbt      8       8 dia    4r/|v|
   Invisibility to Animals Both      T        1      lt+lr/|v|


   Second-Level Druid Spells
   (For High-Level Rangers)

   SPELL NAME              WHEN     RANGE    AREA    DURATION
   -------------------------------------------------------
   Barkskin                Both      T        1       4r+lr/|v|
   Charm Person/Mammal     Cmbt      8        1       Special
   Cure Light Wounds       Both      T        1


   Third-Level Druid Spells
   (For High-Level Rangers)

   SPELL NAME              WHEN     RANGE    AREA    DURATION
   -------------------------------------------------------
   Cure Disease            Camp       T        1
   Hold Animal             Cmbt       8       1-4      2r/|v|
   Neutralize Poison       Both       T        1
   Protection from Fire    Both       T        1       Special


   First-Level Magic-User Spells

   SPELL NAME              WHEN     RANGE    AREA    DURATION
   -------------------------------------------------------
   Burning Hands           Cmbt       T       3 s
   Charm Person            Cmbt       12      1
   Detect Magic            Both       6       1        2r/|v|
   Enlarge                 Both    .5/|v|     1        ll/|v|
   Reduce                  Both    .5/|v|     1
   Friends                 Cmbt       0      All       lr/|v|
   Magic Missile           Cmbt     6+/|v|    1
   Protection from Evil    Both       T       1        2r/|v|
   Read Magic              Camp       0       1        2r/|v|
   Shield                  Cmbt       0       1        5r/|v|
   Shocking Grasp          Cmbt       T       1
   Sleep                   Cmbt      3+/|v|  1-16      5r/|v|


   Second-Level Magic-User Spells

   SPELL NAME              WHEN     RANGE     AREA    DURATION
   -------------------------------------------------------
   Detect Invisibilily     Both     1/|v|      1       5r/|v|
   Invisibility            Both       T        1       Special
   Knock                   Camp       6      1s/|v|
   Mirror Image            Both       0        1       2r/|v|
   Ray of Enfeeblement     Cmbt    1+.25/|v|   1       1r/|v|
   Stinking Cloud          Cmbt       3      2x2 s     1r/|v|
   Strength                Both       T        1       6t/|v|


   Third-Level Magic-User Spells

   SPELL NAME              WHEN     RANGE    AREA        DURATION
   -------------------------------------------------------
   Blink                   Both     0           1         lr/|v|
   Dispel Magic            Both     12        3x3 s
   Fireball                Cmbt     10+|v|   5/7 dia
   Haste                   Both     6         4x4 s      3r+1/|v|
   Hold Person             Cmbt     12         1-4        2r/|v|
   Invisibility 10' Radius Both     T 2 dia  Special
   Lightning Bolt          Cmbt     4+|v|      4,8
   Protection from
   Evil 10' Radius       Both      T        2 dia       2r/|v|
   Protection from
   Normal Missiles       Both      T          1         1t/|v|
   Slow                    Cmbt      9+M      4x4 s      3r+1/|v|

   Fourth-Level Magic-User Spells

   SPELL NAME              WHEN     RANGE    AREA    DURATION
   -------------------------------------------------------
   Bestow Curse            Cmbt      T       1        1t/|v|
   Charm Monster           Cmbt      6       1        Special
   Confusion               Cmbt      12     2-16     2r+1/|v|
   Dimension Door          Cmbt      0       1
   Fear                    Cmbt      0     6x3 c      1r/|v|
   Fire Shield (2 Types)   Both      0       1       2r+1/|v|
   Fumble                  Cmbt     1/|v|    1        1r/|v|
   Ice Storm (Drag only)   Cmbt     1/|v|   4 dia
   Minor Globe of
   Invulnerability       Both      0       1        1r/|v|
   Remove Curse            Both      T       1


   Fifth-Level Magic-User Spells

   SPELL NAME             WHEN    RANGE     AREA      DURATION
   -------------------------------------------------------
   Cloud Kill              Cmbt     1       3x3 s      1r/|v|
   Cone of Cold            Cmbt     0      .5/|v| c
   Feeblemind              Cmbt    1/|v|      1
   Fire Touch              Both     T      Special     1r/|v|
   Hold Monster            Cmbt    5/|v|     1-4       1r/|v|
   Iron Skin               Both     0      Special     1r/|v|


   Sixth-Level Magic-User Spells

   SPELL NAME                  WHEN    RANGE     AREA    DURATION
   -------------------------------------------------------
   Death Spell                 Cmbt     1      .5/|v|
   Disintegrate                Cmbt   .5/|v|   Special
   Flesh to Stone              Cmbt    1/|v|      1
   Globe of Invulnerability    Both     0         1      1r/|v|
   Stone to Flesh              Both    1/|v|      1


   Seventh-Level Magic-User Spells

   SPELL NAME                WHEN     RANGE    AREA    DURATION
   -------------------------------------------------------
   Delayed Blast Fire Ball   Cmbt    10+1/|v|  5/7dia    Special
   Mass Invisibility         Both     1/|v|    Special   Special
   Power Word, Stun          Cmbt    .5/|v|      1       Special


   Eighth-Level Magic-User Spells

   SPELL NAME                  WHEN     RANGE    AREA      DURATION
   -------------------------------------------------------
   Mass Charm                  Cmbt    .5/|v|   Special   Special
   Mind Blank                  Both      3        1        1 day
   Otto's Irresistible Dance   Cmbt      T        1        2-5r
   Power Word, Blind           Cmbt    .5/|v|   3 dia     Special


   Ninth-Level Magic-User Spells

   SPELL NAME              WHEN     RANGE     AREA     DURATION
   -------------------------------------------------------
   Meteor Swarm            Cmbt    4+1/|v|   Special
   Monster Summoning       Cmbt       0      Special    Special
   Power Word, Kill        Cmbt    .25/|v|   Special

/*
---------------------------
COST MODIFIER #S:
---------------------------
0  Free            7  Div 3          14 Mult 4
1  Div 100         8  Div 2          15 Mult 5
2  Div 50          9  Div 1.5        16 Mult 10
3  Div 20          10 Normal         17 Mult 20
4  Div 10          11 Mult 1.5       18 Mult 50
5  Div 5           12 Mult 2         19 Mult 100
6  Div 4           13 Mult 3

 
---------------------------
ITEM #S:
---------------------------    
   1   Arrows (20)	          86  Darts +2 (10)	  171 Battle Axe +5
   2   Battle Axe	          87  Flail +2	          172 Composite Short Bow +5
   3   Hand Axe	          88  Hammer +2	          173 Composite Long Bow +5
   4   Composite Short Bow	  89  Javelins +2 (2)	  174 Light Crossbow +5
   5   Composite Long Bow	  90  Mace +2	          175 Dagger +5
   6   Long Bow	          91  Bolts +2 (10)	  176 Darts +5 (10)
   7   Short Bow	          92  Scimitar +2	  177 Flail +5
   8   Club	          93  Sling +2	          178 Hammer +5
   9   Light Crossbow	  94  Quarter Staff +2	  179 Javelins +5 (2)
   10  Dagger	          95  Staff Sling +2	  180 Mace +5
   11  Darts (10)	          96  Long Sword +2	  181 Bolts +5 (10)
   12  Flail	          97  Short Sword +2	  182 Scimitar +5
   13  Halberd	          98  Two Handed Sword +2 183 Sling +5
   14  Hammer	          99  Trident +2	  184 Quarter Staff +5
   15  Javelins (5)	  100 Cloak of Prot. +2	  185 Staff Sling +5
   16  Mace	          101 Banded Mail +2	  186 Long Sword +5
   17  Morning Star	  102 Chain Mail +2	  187 Short Sword +5
   18  Military Pick	  103 Helm +2	          188 Two Handed Sword +5
   19  Awl Pike	          104 Leather Armor +2	  189 Trident +5
   20  Bolts (20)	          105 Plate Mail +2	  190 Cloak of Prot. +5
   21  Scimitar	          106 Ring Mail +2	  191 Banded Mail +5
   22  Sling	          107 Scale Mail +2	  192 Chain Mail +5
   23  Spear	          108 Shield +2	          193 Helm +5
   24  Quarter Staff	  109 Elfin Chain +2	  194 Leather Armor +5
   25  Staff Sling	          110 Arrows +3 (10)	  195 Plate Mail +5
   26  Bastard Sword	  111 Battle Axe +3	  196 Ring Mail +5
   27  Broad Sword	          112 Comp. Short Bow +3  197 Scale Mail +5
   28  Long Sword	          113 Comp. Long Bow +3	  198 Shield +5
   29  Short Sword	          114 Light Crossbow +3	  199 Elfin Chain +5
   30  Two Handed Sword	  115 Dagger +3	          200 Silver Shield +3
   31  Trident	          116 Darts +3 (10)	  201 Silver Shield +4
   32  Belt	          117 Flail +3	          202 Belt of Protection +3
   33  Boots	          118 Hammer +3	          203 Boots of Speed
   34  Cloak	          119 Javelins +3 (2)	  204 Girdle of Giant Strength
   35  Robe	          120 Mace +3	          205 Periapt of Health
   36  Banded Mail	          121 Bolts +3 (10)	  206 Javelin of Lightning
   37  Chain Mail	          122 Scimitar +3	  207 Stone of Good Luck
   38  Helm	          123 Sling +3	          208 Necklace of Missiles
   39  Leather Armor	  124 Quarter Staff +3	  209 Eyes of Charming
   40  Plate Mail	          125 Staff Sling +3	  210 Bracers of Defense AC 6
   41  Ring Mail	          126 Long Sword +3	  211 Bracers of Defense AC 4
   42  Scale Mail	          127 Short Sword +3	  212 Bracers of Defense AC 2
   43  Shield	          128 Two Handed Sword +3 213 Long Sword vs. Giants
   44  Elfin Chain	          129 Trident +3	  214 Vorpal Long Sword
   45  Fine Long Bow	  130 Cloak of Prot. +3	  215 Drow Long Sword +1
   46  Blessed Bolts (2)	  131 Banded Mail +3	  216 Drow Long Sword +3
   47  Dart of Hornet's Nest 132 Chain Mail +3	  217 Drow Long Sword +4
   48  Cloak of Displacement 133 Helm +3	          218 Drow Chain +1
   49  Mirror	          134 Leather Armor +3	  219 Drow Chain +3
   50  Arrows +1 (10)	  135 Plate Mail +3	  220 Drow Chain +4
   51  Battle Axe +1	  136 Ring Mail +3	  221 Fine Long Bow +2
   52  Comp. Short Bow +1	  137 Scale Mail +3	  222 Cursed Light Crossbow
   53  Comp. Long Bow +1	  138 Shield +3	          223 Ring (non-magical)
   54  Light Crossbow +1	  139 Elfin Chain +3	  224 Ring of Vulnerability
   55  Dagger +1	          140 Arrows +4 (10)	  225 Ring of Protection +1
   56  Darts +1 (10)	  141 Battle Axe +4	  226 Ring of Protection +2
   57  Flail +1	          142 Comp. Short Bow +4  227 Ring of Protection +3
   58  Hammer +1	          143 Comp. Long Bow +4	  228 Ring of Protection +4
   59  Javelins +1 (2)	  144 Light Crossbow +4	  229 Ring of Protection +5
   60  Mace +1	          145 Dagger +4	          230 Ring of Cold Resistance
   61  Bolts +1 (10)	  146 Darts +4 (10)	  231 Ring of Electrical Immunity
   62  Scimitar +1	          147 Flail +4	          232 Ring of Fire Resistance
   63  Sling +1	          148 Hammer +4	          233 Gauntlets of Ogre Power
   64  Quarter Staff +1	  149 Javelins +4 (2)	  234 Ring of Invisibility
   65  Staff Sling +1	  150 Mace +4	          235 Ring of Blinking
   66  Long Sword +1	  151 Bolts +4 (10)	  236 Ring of Wizardry
   67  Short Sword +1	  152 Scimitar +4	  237 Ring of Protection vs. Evil
   68  Two Handed Sword +1	  153 Sling +4	          238 Wand of Ice Storm
   69  Trident +1	          154 Quarter Staff +4	  239 Wand of Fireballs
   70  Cloak of Prot. +1	  155 Staff Sling +4	  240 Wand of Lightning
   71  Banded Mail +1	  156 Long Sword +4	  241 Wand of Magic Missiles
   72  Chain Mail +1	  157 Short Sword +4	  242 Wand of Paralyzation
   73  Helm +1	          158 Two Handed Sword +4 243 Potion of Speed
   74  Leather Armor +1	  159 Trident +4	  244 Potion of Healing
   75  Plate Mail +1	  160 Cloak of Prot. +4	  245 Potion of Extra Healing
   76  Ring Mail +1	  161 Banded Mail +4	  246 Potion of Invisibility
   77  Scale Mail +1	  162 Chain Mail +4	  247 Potion of Giant Strength
   78  Shield +1	          163 Helm +4	          248 Elixir of Youth
   79  Elfin Chain +1	  164 Leather Armor +4	  249 Scroll of Protection from Dragons
   80  Arrows +2 (10)	  165 Plate Mail +4	  250 Scroll of Cleric Spells
   81  Battle Axe +2	  166 Ring Mail +4	  251 Scrolls of Mage Spells
   82  Comp. Short Bow +2	  167 Scale Mail +4	  252 Scrolls of Mage Spells
   83  Comp. Long Bow +2	  168 Shield +4	          253 Scrolls of Mage Spells
   84  Light Crossbow +2	  169 Elfin Chain +4	  254 Scrolls of Mage Spells
   85  Dagger +2	          170 Arrows +5 (10)	  255 Scrolls of Mage Spells
      
*/
/*
---------------------------
SHOP #S: 
---------------------------
                  
   PART#1: SHOP TYPES (base#)
   ---------------------------------
   +0 Weapons A  (Base: Byte#1=0   Byte#2=0   Byte#3=0)
   +0 Weapons B  (Base: Byte#1=31  Byte#2=248 Byte#3=0)
   +0 Armor      (Base: Byte#1=47  Byte#2=193 Byte#3=0)
   +1 Weapons    (Base: Byte#1=48  Byte#2=0   Byte#3=0)
   +2 Weapons    (Base: Byte#1=64  Byte#2=0   Byte#3=0)
   +3 Weapons    (Base: Byte#1=80  Byte#2=0   Byte#3=0)
   +4 Weapons    (Base: Byte#1=96  Byte#2=0   Byte#3=0)
   +5 Weapons    (Base: Byte#1=112 Byte#2=0   Byte#3=0)
   +1,+2 Armor   (Base: Byte#1=128 Byte#2=0   Byte#3=0)
   +3,+4 Armor   (Base: Byte#1=144 Byte#2=0   Byte#3=0)
   Special A     (Base: Byte#1=160 Byte#2=0   Byte#3=0)
   Special B     (Base: Byte#1=188 Byte#2=0   Byte#3=0)
   Rings         (Base: Byte#1=207 Byte#2=192 Byte#3=0)
   Wands         (Base: Byte#1=223 Byte#2=255 Byte#3=224)
   Potions       (Base: Byte#1=239 Byte#2=255 Byte#3=192)
   Scrolls       (Base: Byte#1=255 Byte#2=255 Byte#3=128)


PART#2: ITEM SLOTS (add to base#)
---------------------------------
       **Byte#1**       **Byte#2**         **Byte#3**
    (+1) Item #17    (+1)   Item #09    (+1)   Item #01
    (+2) Item #18    (+2)   Item #10    (+2)   Item #02
    (+4) Item #19    (+4)   Item #11    (+4)   Item #03
    (+8) Item #20    (+8)   Item #12    (+8)   Item #04
                     (+16)  Item #13    (+16)  Item #05
                     (+32)  Item #14    (+32)  Item #06
                     (+64)  Item #15    (+64)  Item #07
                     (+128) Item #16    (+128) Item #08


*/

/*
   APPENDIX A - Item List for Starting Characters;

   Under Game Settings, equipment for new characters can be defined as
   "None" up through +4.  If None is selected, no equipment is received
   at all for new characters.  The following lists show exactly what
   equipment is received by each character class at all other levels.

   Clerics - This list includes full Clerics and Cleric/Magic-Users.
   Poor - Helm, Club
   Modest - Shield, Ring Mail, Mace
   Average - Hammer, Shield, Chain Mail, Mace
   Prosperous - Shield, Plate Mail, Staff Sling, Flail
   +1 - Staff Sling +1, Shield +1, Banded Mail +1, Mace +1
   +2 - Helm +2, Shield +2, Hammer +2, Banded Mail +2, Mace +2
   +3 - Helm +3, Shield +3, Staff Sling +3, Banded Mail +3, Mace +3
   +4 - Helm +4, Staff Sling +4, Shield +4, Banded Mail +4, Flail +4

   Fighters - This list includes full Fighters, Paladins, Rangers,
   Cleric/Fighters, Cleric/Fighter/Magic-Users, Cleric/Rangers,
   Fighter/Magic-Users, Fighter/Thieves, and Fighter/Magic-User/Thieves.
   Poor - Ring Mail, Spear
   Modest - Shield, Scale Mail, Broad Sword
   Average - 20 Arrows, Long Bow, Shield, Banded Mail, Long Sword
   Prosperous - 20 Arrows, Composite Long Bow, Plate Mail, Two-Handed
   Sword
   +1 - Helm +1, Shield +1, 10 Arrows +1, Composite Long Bow +1, Plate
   Mail +1, Long Sword +1
   +2 - Helm +2, Composite Long Bow +2, 40 Arrows +2, Shield +2, Plate
   Mail +2, Long Sword +2
   +3 - Helm +3, Shield +3, Composite Long Bow +3, 10 Arrows +3, Plate
   Mail +3, Long Sword +3
   +4 - Composite Long Bow +4, 10 Arrows +4, Helm +4, Shield +4, Plate
   Mail +4, Long Sword +4

   Magic-Users - This list includes only full Magic-Users (no
   multi-classed characters).
   Poor - 10 Darts
   Modest - Dagger, 10 Darts
   Average - Robe, Quarterstaff, 10 Darts
   Prosperous - Robe, Cloak, Quarterstaff, 10 Darts
   +1 - Cloak of Prot. +1, Bracers of AC 6, 10 Darts +1, Dagger +1
   +2 - Cloak of Prot. +2, Bracers of AC 4, 40 Darts +2, Quarterstaff +2
   +3 - Cloak of Prot. +3, Bracers of AC 2, 10 Darts +3, Dagger +3
   +4 - Cloak of Prot. +4, Bracers of AC 2, Dart of Hornet's Nest, Dart
   of Hornet's Nest, Quarterstaff +4

   Thieves - This list includes full Thieves and Magic-User/Thieves.
   Poor - Club
   Modest - Leather Armor, Short Sword
   Average - Sling, Leather Armor, Broad Sword
   Prosperous - Elfin Chain, 20 Arrows, Composite Short Bow, Long Sword
   +1 - 10 Arrows +1, Composite Short Bow +1, Elfin Chain +1, Long Sword
   +1
   +2 - Elfin Chain +2, Sling +2, Long Sword +2
   +3 - Elfin Chain +3, Sling +3, Long Sword +3
   +4 - Cloak of Prot. +4, Bracers of AC 2, Sling +4, Long Sword +4

   **********************************************************************

   Fighter: Includes Fighters, Paladins, Rangers, and all multi-class combinations that include at least one of these classes. Starting equipment is as follows: 

   Poor: Items 23 and 41 
   Modest: Items 27, 42, and 43 
   Average: Items 1, 6, 28, 36, and 43 
   Prosperous: Items 1, 5, 30, and 40 
   +1: Items 50, 53, 66, 73, 75, and 78 
   +2: Items 83, 96, 103, 105, and 108 and a bundle of forty of item 80 
   +3: Items 110, 113, 126, 133, 135, and 138 
   +4: Items 140, 143, 156, 163, 165, and 168 

   Cleric: Includes Clerics, Cleric/Thieves, and Cleric/Magic-Users. Starting equipment is as follows: 

   Poor: Items 8 and 38 
   Modest: Items 16, 41, and 43 
   Average: Items 14, 16, 37, and 43 
   Prosperous: Items 12, 25, 40, and 43 
   +1: Items 60, 65, 71, and 78 
   +2: Items 88, 90, 101, 103, and 108 
   +3: Items 120, 125, 131, 133, and 138 
   +4: Items 147, 155, 161, 163, and 168 

   Thief: Includes Thieves and Magic-User/Thieves. Starting equipment is as follows: 

   Poor: Item 8 
   Modest: Items 29 and 39 
   Average: Items 22, 27, and 39 
   Prosperous: Items 1, 4, 28, and 44 
   +1: Items 50, 52, 66, and 79 
   +2: Items 93, 96, and 109 
   +3: Items 123, 126, and 139 
   +4: Items 153, 156, 160, and 212 

   Magic-User: Includes single-classed Magic-Users only. Starting equipment is as follows: 

   Poor: Item 11 
   Modest: Items 10 and 11 
   Average: Items 11, 24, and 35 
   seProsperous: Items 11, 24, 34, and 35 
   +1: Items 55, 56, 70, and 210 
   +2: Items 94, 100, and 211 and a bundle of forty of item 86 
   +3: Items 115, 116, 130, and 212 
   +4: Items 154, 160, 212, and two of item 47 
   */

   //---------------------------
   //QUEST #S:
   //---------------------------            
   //0-7   Key 1-8 
   //8-19  Item 1-12
   //20-63 Quest 1-44


/*
WALLS:
   1==Adobe
   2==Bricks
   3==Cave
   4==Coral A
   5==Coral B
   6==Fire Bricks
   7==Lava
   8==Marble
   9==Rock A
   10=Rock B
   11=Trees A
   12=Trees B
   13=Wasteland
   14=Webs
   15=Wood A
   16=Wood B
*/

/*
BACK.TLB        01 = 16         BACK1001.TLB         *STONE FLOOR
                02 = 9          BACK1002.TLB         *MARBLE
                03 = 3          BACK1003.TLB         *CAVE
                04 = 7          BACK1004.TLB         *FOREST
                05 = 4          BACK1005.TLB         *CORAL
                06 = 8          BACK1006.TLB         *LAVA
                07 = 1          BACK1007.TLB         *ADOBE
                08 = 19         BACK1008.TLB         *WOOD
                09 = 2          BACK1009.TLB         *BRICKS
                10 = 6          BACK1010.TLB         *FIRE BRICKS
                11 = 17         BACK1011.TLB         *SWAMP
                12 = 5          BACK1012.TLB         *DARK CORAL
                13 = 18         BACK1013.TLB         *WASTELAND
                14 = 10         BACK1032.TLB         *OUTDOOR GRASS (DAY)
                15 = 11         BACK1033.TLB         *OUTDOOR GRASS (NIGHT)
                16 = 12         BACK1034.TLB         *OUTDOOR MUD (DAY)
                17 = 13         BACK1035.TLB         *OUTDOOR MUD (NIGHT)
                18 = 14         BACK1036.TLB         *OUTDOOR STONE (DAY)
                19 = 15         BACK1037.TLB         *OUTDOOR STONE (NIGHT)

BACKGROUNDS (UA editor ordering)
   1==Adobe
   2==Bricks
   3==Cave
   4==Coral
   5==Dark Coral
   6==Fire Bricks
   7==Forest
   8==Lava
   9==Marble
   10=Outdoor Grass (Day--A)
   11=Outdoor Grass (Night--A)
   12=Outdoor Mud (Day--B)
   13=Outdoor Mud (Night--B)
   14=Outdoor Stone (Day--B)
   15=Outdoor Stone (Night--B)
   16=Stone Floor
   17=Swamp
   18=Wasteland
   19=Wood
*/

#pragma endregion

// restore default packing alignment
#include <poppack.h>

#endif // __FRUA_STRUCTS_H__
