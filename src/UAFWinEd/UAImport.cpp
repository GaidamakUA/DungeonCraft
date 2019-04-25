/******************************************************************************
* Filename: UAImport.cpp
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
//#include "..\Shared\Version.h"

//#include "UAFWinEd.h"
#include "GlobalData.h"
#include "Level.h"
#include "GameEvent.h"
#include "PicSlot.h"
#include "SoundMgr.h"
#include "UAImport.h"

// force 1 byte packing alignment for this file
#include <pshpack1.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_IMPORT_SOUNDS 100
#define MAX_IMPORT_WALLS 16
#define MAX_IMPORT_BACKDROPS 20
#define MAX_IMPORT_PICS 240
#define MAX_IMPORT_MONSTERS 128
#define MAX_IMPORT_CHARACTERS 255
#define MAX_IMPORT_ITEMS 255
#define MAX_IMPORT_EVENTS 100

#define UAWallType_None 0
#define UAWallType_Wall 1
#define UAWallType_Door 2

TAVERN_TALES tavernTales;
bool talesPresent = false;
bool UseDefaultArtForImport=true;

struct UAWallDefine
{
  BYTE w1;
  BYTE w2;
  BYTE w3;
  BYTE w4;
  BYTE w5;
};

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

#define DEFUAWALL(a,b,c,d,e)  { UAWallType_##a,UAWallType_##b,UAWallType_##c,UAWallType_##d,UAWallType_##e }

UAWallDefine UAWallDefinitions[17] =
{
  DEFUAWALL(None,None,None,None,None), // 0
  DEFUAWALL(Wall,Door,Door,Door,Door), // 9   Rock A
  DEFUAWALL(Wall,Door,Door,Door,Wall), // 10  Rock B
  DEFUAWALL(Wall,Wall,Door,Door,Wall), // 1   Adobe
  DEFUAWALL(Wall,Wall,Wall,Wall,Door), // 14  Webs
  DEFUAWALL(Wall,Door,Door,Door,Door), // 2   Bricks
  DEFUAWALL(Wall,Door,Wall,Wall,Wall), // 3   Cave
  DEFUAWALL(Wall,Wall,Door,Door,Door), // 15  Wood A
  DEFUAWALL(Wall,Wall,Door,Door,Wall), // 16  Wood B
  DEFUAWALL(None,Door,Wall,Wall,None), // 13  Wasteland
  DEFUAWALL(Wall,Wall,Door,Wall,Wall), // 8   Marble
  DEFUAWALL(Wall,Door,Wall,Wall,Wall), // 11  Trees A
  DEFUAWALL(None,Wall,Wall,Wall,Wall), // 12  Trees B
  DEFUAWALL(Wall,Door,Door,Door,Wall), // 4   Coral A
  DEFUAWALL(Wall,Wall,Door,Wall,Wall), // 5   Coral B
  DEFUAWALL(Wall,Door,Door,Wall,Wall), // 7   Lava
  DEFUAWALL(Wall,Door,Door,Door,Door)  // 6   Fire Bricks
};

BYTE UAWallGroupToArtSlotBase(BYTE group)
{
  switch (group)
  {
  case 1: return (9-1)*5+1;
  case 2: return (10-1)*5+1;
  case 3: return (1-1)*5+1;
  case 4: return (14-1)*5+1;
  case 5: return (2-1)*5+1;
  case 6: return (3-1)*5+1;
  case 7: return (15-1)*5+1;
  case 8: return (16-1)*5+1;
  case 9: return (13-1)*5+1;
  case 10: return (8-1)*5+1;
  case 11: return (11-1)*5+1;
  case 12: return (12-1)*5+1;
  case 13: return (4-1)*5+1;
  case 14: return (5-1)*5+1;
  case 15: return (7-1)*5+1;
  case 16: return (6-1)*5+1;
  }
  return 1;
}

BOOL HasMask(BYTE val, BYTE mask)
{
  return ((val & mask)==mask);
}

BYTE GetWallType(int s1, int s2, int s3, int w)
{
  if ((w > 0) && (w <= 5))
  {
    if ((s1 <= 0) || (s1 >= 16)) return UAWallType_None;
  }
  else if ((w > 5) && (w <= 10))
  {
    if ((s2 <= 0) || (s2 >= 16)) return UAWallType_None;
  }
  else if ((w > 10) && (w <= 15))
  {
    if ((s3 <= 0) || (s3 >= 16)) return UAWallType_None;
  }
  else
    return UAWallType_None;

  switch (w)
  {
  case 0: return UAWallType_None;
  case 1: return UAWallDefinitions[s1].w1;
  case 2: return UAWallDefinitions[s1].w2;
  case 3: return UAWallDefinitions[s1].w3;
  case 4: return UAWallDefinitions[s1].w4;
  case 5: return UAWallDefinitions[s1].w5;
  case 6: return UAWallDefinitions[s2].w1;
  case 7: return UAWallDefinitions[s2].w2;
  case 8: return UAWallDefinitions[s2].w3;
  case 9: return UAWallDefinitions[s2].w4;
  case 10: return UAWallDefinitions[s2].w5;
  case 11: return UAWallDefinitions[s3].w1;
  case 12: return UAWallDefinitions[s3].w2;
  case 13: return UAWallDefinitions[s3].w3;
  case 14: return UAWallDefinitions[s3].w4;
  case 15: return UAWallDefinitions[s3].w5;
  default: return UAWallType_None;
  }
}

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

BYTE GetNightBackdropSlot(BYTE b)
{
  switch (b)
  {
  case 10: return 11;
  case 12: return 13;
  case 14: return 15;
  }
  return 0;
}

BYTE UABackdropToDCSlot(BYTE b)
{
  switch (b)
  {
  case 1: return 16;
  case 2: return 9;
  case 3: return 3;
  case 4: return 7;
  case 5: return 4;
  case 6: return 8;
  case 7: return 1;
  case 8: return 19;
  case 9: return 2;
  case 10: return 6;
  case 11: return 17;
  case 12: return 5;
  case 13: return 18;
  case 32: return 10;
  case 33: return 11;
  case 34: return 12;
  case 35: return 13;
  case 36: return 14;
  case 37: return 15;
  default: return 1;
  }
}

BYTE GetBackdropType(int b1, int b2, int b3, int b4, int b)
{
  switch (b)
  {
  case 1: return UABackdropToDCSlot(b1);
  case 2: return UABackdropToDCSlot(b2);
  case 3: return UABackdropToDCSlot(b3);
  case 4: return UABackdropToDCSlot(b4);
  }
  return 1;
}

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
#ifdef UAFEDITOR
char *MonsterLabels[MAX_IMPORT_MONSTERS] = {
  "none",
  "Kobold",
  "Goblin",
  "Orc",
  "Hobgoblin",
  "Orc Chieftain",
  "Hobgoblin Ldr",
  "Gnoll",
  "Ogre",
  "Troll",
  "Hill Giant",
  "Fire Giant",
  "Frost Giant",
  "Cloud Giant",
  "Storm Giant",
  "Giant Rat",
  "Carrion Crawler",
  "Enormous Spider",
  "Skeleton",
  "Zombie",
  "Ghoul",
  "Ghast",
  "Wight",
  "Wraith",
  "Mummy",
  "Spectre",
  "Vampire",
  "Lich",
  "Lizard Man",
  "Lizard Man King",
  "Minotaur",
  "Displacer Beast",
  "Boring Beetle",
  "Griffon",
  "Hydra",
  "Wyvern",
  "Black Dragon",
  "Blue Dragon",
  "Green Dragon",
  "Red Dragon",
  "White Dragon",
  "Dracolich",
  "Basilisk",
  "Gorgon",
  "Cockatrice",
  "Beholder",
  "Ogre Mage",
  "Bulette",
  "Shambling Mound",
  "Margoyle",
  "Dracolisk",
  "Mobat",
  "Black Pudding",
  "Otyugh",
  "Neo Otyugh",
  "Salamander",
  "Efreeti",
  "Earth Elemental",
  "Fire Elemental",
  "Umber Hulk",
  "Ettin",
  "Owlbear",
  "Bugbear",
  "Medusa",
  "Giant Spider",
  "Phase Spider",
  "Poisonous Snake",
  "Hell Hound",
  "Giant Crocodile",
  "Drider",
  "Iron Golem",
  "Rakshasa",
  "Purple Worm",
  "Drow Champion",
  "Drow Priest",
  "Drow Sorceress",
  "Drow Priestess",
  "Warrior",
  "Conjurer",
  "Acolyte",
  "Goon",
  "Archer",
  "Theurgist",
  "Priest",
  "Thug",
  "Evil Champion",
  "Magician",
  "Dark Cleric",
  "Rogue",
  "Dark Knight",
  "Necromancer",
  "High Priest",
  "Thief",
  "Dark Warlord",
  "Wizard",
  "Archpriest",
  "Master Thief",
  "Dark Overlord",
  "Master Wizard",
  "Dark Disciple",
  "High Thief",
  "Vampire Lord",
  "Vampiress",
  "Dazmilar",
  "Ogre Shaman",
  "Sir Dutiocs",
  "Vidruand",
  "Hill Giant Shaman",
  "Vampire Priest",
  "Drow Amazon",
  "Rakshasa Rukh",
  "Road Guard",
  "Kallithrea",
  "Yemandra",
  "Krondasz",
  "Arderiel",
  "Tornilee",
  "Alias",
  "Dragonbait",
  "Nacacia",
  "Priam",
  "Vala",
  "Silk",
  "Captain Daenor",
  "Grunschka",
  "Storm",
  "Shal",
  "Raizel",
};
#endif
/*
---------------------------
MONSTER #S:
---------------------------
001 Kobold	        044 Cockatrice	        087 Dark Cleric
002 Goblin	        045 Beholder	        088 Rogue
003 Orc	                046 Ogre Mage	        089 Dark Knight
004 Hobgoblin	        047 Bulette	        090 Necromancer
005 Orc Chieftain	048 Shambling Mound	091 High Priest
006 Hobgoblin Ldr	049 Margoyle	        092 Thief
007 Gnoll	        050 Dracolisk	        093 Dark Warlord
008 Ogre	        051 Mobat	        094 Wizard
009 Troll	        052 Black Pudding	095 Archpriest
010 Hill Giant	        053 Otyugh	        096 Master Thief
011 Fire Giant	        054 Neo Otyugh	        097 Dark Overlord
012 Frost Giant	        055 Salamander	        098 Master Wizard
013 Cloud Giant	        056 Efreeti	        099 Dark Disciple
014 Storm Giant	        057 Earth Elemental	100 High Thief
015 Giant Rat	        058 Fire Elemental	101 Vampire Lord
016 Carrion Crawler	059 Umber Hulk	        102 Vampiress
017 Enormous Spider	060 Ettin	        103 Dazmilar
018 Skeleton	        061 Owlbear	        104 Ogre Shaman
019 Zombie	        062 Bugbear	        105 Sir Dutiocs
020 Ghoul	        063 Medusa	        106 Vidruand
021 Ghast	        064 Giant Spider	107 Hill Giant Shaman
022 Wight	        065 Phase Spider	108 Vampire Priest
023 Wraith	        066 Poisonous Snake	109 Drow Amazon
024 Mummy	        067 Hell Hound	        110 Rakshasa Rukh
025 Spectre	        068 Giant Crocodile	111 Road Guard
026 Vampire	        069 Drider	        112 Kallithrea
027 Lich	        070 Iron Golem	        113 Yemandra
028 Lizard Man	        071 Rakshasa	        114 Krondasz
029 Lizard Man King	072 Purple Worm	        115 Arderiel
030 Minotaur	        073 Drow Champion	116 Tornilee
031 Displacer Beast	074 Drow Priest	        117 Alias
032 Boring Beetle	075 Drow Sorceress	118 Dragonbait
033 Griffon	        076 Drow Priestess	119 Nacacia
034 Hydra	        077 Warrior	        120 Priam
035 Wyvern	        078 Conjurer	        121 Vala
036 Black Dragon	079 Acolyte	        122 Silk
037 Blue Dragon	        080 Goon	        123 Captain Daenor
038 Green Dragon	081 Archer	        124 Grunschka
039 Red Dragon	        082 Theurgist	        125 Storm
040 White Dragon	083 Priest	        126 Shal
041 Dracolich	        084 Thug	        127 Raizel
042 Basilisk	        085 Evil Champion	
043 Gorgon	        086 Magician	


---------------------------
NPC #S: 
---------------------------
112 Kallithrea          120 Priam
113 Yemandra            121 Vala
114 Krondasz            122 Silk
115 Arderiel            123 Captain Daenor
116 Tornilee            124 Grunschka
117 Alias               125 Storm  
118 Dragonbait          126 Shal
119 Nacacia             127 Raizel

**May also input monster#


---------------------------
QUEST #S:
---------------------------            
0-7   Key 1-8 
8-19  Item 1-12
20-63 Quest 1-44


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

static void PumpWinMsgs(void);
BOOL ImportAllGeoDatFiles(const char *path);
BOOL ImportGeoDatFile(int level, const char *fullpath);

int CurrentLevel = 0;

BYTE EventByte(BYTE FileOffset, BYTE *pData)
{
  return pData[FileOffset-5];
}

static WORD GetWord(BYTE offset, BYTE* buf)
{
  WORD temp;
  temp = (EventByte(offset,buf) & 0xff);
  offset++;
  temp += ((EventByte(offset,buf) & 0xff) << 8);
  return temp;
}

static DWORD GetDWord(BYTE offset, BYTE* buf)
{
  WORD temp1,temp2;
  
  temp1 = (EventByte(offset,buf) & 0xff);
  offset++;
  temp1 += ((EventByte(offset,buf) & 0xff) << 8);
  offset++;

  temp2 = (EventByte(offset,buf) & 0xff);
  offset++;
  temp2 += ((EventByte(offset,buf) & 0xff) << 8);

  return ((temp2<<16)|temp1);
}

CString ImportWallSlotNames[MAX_IMPORT_WALLS+1];
CString ImportDoorSlotNames[MAX_IMPORT_WALLS+1];

CString ImportBackdropNames[MAX_IMPORT_BACKDROPS+1];
CString ImportPicSlotNames[MAX_IMPORT_PICS+1];
CString ImportSoundSlotNames[MAX_IMPORT_SOUNDS+1];
/*
const char *ImportDungeonCombatName = "UADungeonCombat.pcx";
const char *ImportWildernessCombatName = "UAWildCombat.pcx";
const char *ImportZoneCampName = "UAZoneCamp.pcx";
const char *ImportZoneTreasureName = "UAZoneTreasure.pcx";
*/
BYTE StartEP = 0;

//CMap<BYTE, BYTE, BYTE, BYTE> WallsetsUsed;
//CMap<BYTE, BYTE, BYTE, BYTE> BackdropsUsed;

int SpecialKey_Keys[9];
int SpecialItem_Keys[13];
int Quest_Keys[45];
//int Monster_Keys[MAX_IMPORT_MONSTERS];
MONSTER_ID Monster_Keys[MAX_IMPORT_MONSTERS];
int Monster_Type[MAX_IMPORT_MONSTERS];
int Sound_Keys[MAX_IMPORT_SOUNDS+1];
//GLOBAL_ITEM_ID giID_Keys[MAX_IMPORT_ITEMS+1];
ITEM_ID itemID_Keys[MAX_IMPORT_ITEMS+1];
DWORD Event_Keys[MAX_IMPORT_EVENTS+1];
int ImportMonsterCount=0;
int ImportNPCCount=0;
/*
DWORD GetEventKey(BYTE index)
{
  DWORD key=0;
  if ((index-1 >= 0) && (index-1 <= MAX_IMPORT_EVENTS))
    key = Event_Keys[index-1];
  //key &= 0x7FFFFFFF;
  return key;
}
*/
/*
int GetMonsterKey(BYTE index)
{
  int key = 0;
  if ((index > 0) && (index < MAX_IMPORT_MONSTERS))
  {
    key = Monster_Keys[index];
    if (key <= 0)
    {
      key = monsterData.FindNamei(MonsterLabels[index]);
      if (key <= 0)
        key = globalData.charData.FindNamei(MonsterLabels[index]);
    }
  }
  return key;
}
*/

int GetMonsterType(BYTE index)
{
  int key = MONSTER_TYPE;
  if ((index > 0) && (index < MAX_IMPORT_MONSTERS))
    key = Monster_Type[index];
  return key;
}
costFactorType ConvertCostModifier(BYTE data)
{
  costFactorType type = Free;
  switch (data)
  {
  case 0: type = Free; break;
  case 1: type = Div100; break;
  case 2: type = Div50; break;
  case 3: type = Div20; break;
  case 4: type = Div10; break;
  case 5: type = Div5; break;
  case 6: type = Div4; break;
  case 7: type = Div3; break;
  case 8: type = Div2; break;
  case 9: type = Div1_5; break;
  case 10: type = Normal; break;
  case 11: type = Mult1_5; break;
  case 12: type = Mult2; break;
  case 13: type = Mult3; break;
  case 14: type = Mult4; break;
  case 15: type = Mult5; break;
  case 16: type = Mult10; break;
  case 17: type = Mult20; break;
  case 18: type = Mult50; break;
  case 19: type = Mult100; break;
  }
  return type;
}

void AssignPic(BYTE slot, PIC_DATA &data, BOOL HiBit)
{
  data.Clear();

  if (!HiBit) // indicates pic, big pic, or no art
    return;

  CString temp;
  if ((slot > 0) && (slot <= MAX_IMPORT_PICS))
  {
    temp = ImportPicSlotNames[slot];
    if (!FileExists(temp))
      return;
    data.filename = temp;
    data.picType = SmallPicDib;
    data.SetDefaults();
  }
}

/*
---------------------------
SOUND #S:
---------------------------
0  No Sound        10 Swish          34 Foes
3  Cast            11 Footstep       35 Battle
4  Blast           12 Fireball       36 Mystery
5  Sorcery         13 Bowstring      37 Uh Oh
6  Death           14 Sploosh        38 Evil March
7  Missile         15 Crackle        39 Victory
8  Hit             32 Overture   
9  Lightning       33 Treasure  
*/

void AssignSound(BYTE slot, CString &data)
{
  data="";
  return;
  /*
  if ((slot > 0) && (slot <= MAX_IMPORT_SOUNDS))
  {
    CString temp;
    temp = ImportSoundSlotNames[slot];
    if (!FileExists(temp))
      return;
    data = temp;
  }
  */
}

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

enum startEquipType { seNone, sePoor, seModest, seAverage, seProsperous, seP1, seP2, seP3, seP4 };

*/

BOOL AssignItem(BYTE slot, ITEM &data, BOOL id)
{
  if ((slot < 1)  || (slot > MAX_IMPORT_ITEMS))
    return FALSE;


  //ITEM_DATA *idata = itemData.GetItemData(giID_Keys[slot-1]);
  //ITEM_DATA *pItem = itemData.GetItemData(itemID_Keys[slot-1]);
  ITEM_DATA *pItem = itemData.GetItem(itemID_Keys[slot-1]);
  if (pItem == NULL)
    return FALSE;

  data.Clear();
  //data.charges = idata->Num_Charges;
  data.charges = pItem->Num_Charges;
  //data.qty = idata->Bundle_Qty;
  data.qty = pItem->Bundle_Qty;
  data.ready = FALSE;
  data.identified = id;
  //data.m_giID = giID_Keys[slot-1];
  
  return TRUE;
}


//void AssignStartEquipItems(classType chtype, startEquipType seType, ITEM_LIST &data)
void AssignStartEquipItems(classType_Editor chtype, startEquipType seType, ITEM_LIST &data)
{
  if (seType==seNone) return;

  ITEM item;
  if ((isFighter(chtype))||(isRanger(chtype))||(isPaladin(chtype)))
  {
    switch (seType)
    {
    case sePoor:
      AssignItem(23, item, TRUE);
      data.AddItem(item);
      AssignItem(41, item, TRUE);
      data.AddItem(item);
      break;
    case seModest:
      AssignItem(27, item, TRUE);
      data.AddItem(item);
      AssignItem(42, item, TRUE);
      data.AddItem(item);
      AssignItem(43, item, TRUE);
      data.AddItem(item);
      break;
    case seAverage:
      AssignItem(1, item, TRUE);
      data.AddItem(item);
      AssignItem(6, item, TRUE);
      data.AddItem(item);
      AssignItem(28, item, TRUE);
      data.AddItem(item);
      AssignItem(36, item, TRUE);
      data.AddItem(item);
      AssignItem(43, item, TRUE);
      data.AddItem(item);
      break;
    case seProsperous:
      AssignItem(1, item, TRUE);
      data.AddItem(item);
      AssignItem(5, item, TRUE);
      data.AddItem(item);
      AssignItem(30, item, TRUE);
      data.AddItem(item);
      AssignItem(40, item, TRUE);
      data.AddItem(item);
      break;
    case seP1:
      AssignItem(50, item, TRUE);
      data.AddItem(item);
      AssignItem(53, item, TRUE);
      data.AddItem(item);
      AssignItem(66, item, TRUE);
      data.AddItem(item);
      AssignItem(73, item, TRUE);
      data.AddItem(item);
      AssignItem(75, item, TRUE);
      data.AddItem(item);
      AssignItem(78, item, TRUE);
      data.AddItem(item);
      break;
    case seP2:
      AssignItem(83, item, TRUE);
      data.AddItem(item);
      AssignItem(96, item, TRUE);
      data.AddItem(item);
      AssignItem(103, item, TRUE);
      data.AddItem(item);
      AssignItem(105, item, TRUE);
      data.AddItem(item);
      AssignItem(108, item, TRUE);
      data.AddItem(item);
      AssignItem(80, item, TRUE);
      item.qty=40;
      data.AddItem(item);
      break;
    case seP3:
      AssignItem(110, item, TRUE);
      data.AddItem(item);
      AssignItem(113, item, TRUE);
      data.AddItem(item);
      AssignItem(126, item, TRUE);
      data.AddItem(item);
      AssignItem(133, item, TRUE);
      data.AddItem(item);
      AssignItem(135, item, TRUE);
      data.AddItem(item);
      AssignItem(138, item, TRUE);
      data.AddItem(item);
      break;
    case seP4:
      AssignItem(140, item, TRUE);
      data.AddItem(item);
      AssignItem(143, item, TRUE);
      data.AddItem(item);
      AssignItem(156, item, TRUE);
      data.AddItem(item);
      AssignItem(163, item, TRUE);
      data.AddItem(item);
      AssignItem(165, item, TRUE);
      data.AddItem(item);
      AssignItem(168, item, TRUE);
      data.AddItem(item);
      break;
    }
  }

  if ((isCleric(chtype))||(isDruid(chtype)))
  {
    switch (seType)
    {
    case sePoor:
      AssignItem(8, item, TRUE);
      data.AddItem(item);
      AssignItem(38, item, TRUE);
      data.AddItem(item);
      break;
    case seModest:
      AssignItem(16, item, TRUE);
      data.AddItem(item);
      AssignItem(41, item, TRUE);
      data.AddItem(item);
      AssignItem(43, item, TRUE);
      data.AddItem(item);
      break;
    case seAverage:
      AssignItem(14, item, TRUE);
      data.AddItem(item);
      AssignItem(16, item, TRUE);
      data.AddItem(item);
      AssignItem(37, item, TRUE);
      data.AddItem(item);
      AssignItem(43, item, TRUE);
      data.AddItem(item);
      break;
    case seProsperous:
      AssignItem(12, item, TRUE);
      data.AddItem(item);
      AssignItem(25, item, TRUE);
      data.AddItem(item);
      AssignItem(40, item, TRUE);
      data.AddItem(item);
      AssignItem(43, item, TRUE);
      data.AddItem(item);
      break;
    case seP1:
      AssignItem(60, item, TRUE);
      data.AddItem(item);
      AssignItem(65, item, TRUE);
      data.AddItem(item);
      AssignItem(71, item, TRUE);
      data.AddItem(item);
      AssignItem(78, item, TRUE);
      data.AddItem(item);
      break;
    case seP2:
      AssignItem(88, item, TRUE);
      data.AddItem(item);
      AssignItem(90, item, TRUE);
      data.AddItem(item);
      AssignItem(101, item, TRUE);
      data.AddItem(item);
      AssignItem(103, item, TRUE);
      data.AddItem(item);
      AssignItem(108, item, TRUE);
      data.AddItem(item);
      break;
    case seP3:
      AssignItem(120, item, TRUE);
      data.AddItem(item);
      AssignItem(125, item, TRUE);
      data.AddItem(item);
      AssignItem(131, item, TRUE);
      data.AddItem(item);
      AssignItem(133, item, TRUE);
      data.AddItem(item);
      AssignItem(138, item, TRUE);
      data.AddItem(item);
      break;
    case seP4:
      AssignItem(147, item, TRUE);
      data.AddItem(item);
      AssignItem(155, item, TRUE);
      data.AddItem(item);
      AssignItem(161, item, TRUE);
      data.AddItem(item);
      AssignItem(163, item, TRUE);
      data.AddItem(item);
      AssignItem(168, item, TRUE);
      data.AddItem(item);
      break;
    }
  }

  if (isThief(chtype))
  {
    switch (seType)
    {
    case sePoor:
      AssignItem(8, item, TRUE);
      data.AddItem(item);
      break;
    case seModest:
      AssignItem(29, item, TRUE);
      data.AddItem(item);
      AssignItem(39, item, TRUE);
      data.AddItem(item);
      break;
    case seAverage:
      AssignItem(22, item, TRUE);
      data.AddItem(item);
      AssignItem(27, item, TRUE);
      data.AddItem(item);
      AssignItem(39, item, TRUE);
      data.AddItem(item);
      break;
    case seProsperous:
      AssignItem(1, item, TRUE);
      data.AddItem(item);
      AssignItem(4, item, TRUE);
      data.AddItem(item);
      AssignItem(28, item, TRUE);
      data.AddItem(item);
      AssignItem(44, item, TRUE);
      data.AddItem(item);
      break;
    case seP1:
      AssignItem(50, item, TRUE);
      data.AddItem(item);
      AssignItem(52, item, TRUE);
      data.AddItem(item);
      AssignItem(66, item, TRUE);
      data.AddItem(item);
      AssignItem(79, item, TRUE);
      data.AddItem(item);
      break;
    case seP2:
      AssignItem(93, item, TRUE);
      data.AddItem(item);
      AssignItem(96, item, TRUE);
      data.AddItem(item);
      AssignItem(109, item, TRUE);
      data.AddItem(item);
      break;
    case seP3:
      AssignItem(123, item, TRUE);
      data.AddItem(item);
      AssignItem(126, item, TRUE);
      data.AddItem(item);
      AssignItem(139, item, TRUE);
      data.AddItem(item);
      break;
    case seP4:
      AssignItem(153, item, TRUE);
      data.AddItem(item);
      AssignItem(156, item, TRUE);
      data.AddItem(item);
      AssignItem(160, item, TRUE);
      data.AddItem(item);
      AssignItem(212, item, TRUE);
      data.AddItem(item);
      break;
    }
  }

  if (isMagicUser(chtype))
  {
    switch (seType)
    {
    case sePoor:
      AssignItem(11, item, TRUE);
      data.AddItem(item);
      break;
    case seModest:
      AssignItem(10, item, TRUE);
      data.AddItem(item);
      AssignItem(11, item, TRUE);
      data.AddItem(item);
      break;
    case seAverage:
      AssignItem(11, item, TRUE);
      data.AddItem(item);
      AssignItem(24, item, TRUE);
      data.AddItem(item);
      AssignItem(35, item, TRUE);
      data.AddItem(item);
      break;
    case seProsperous:
      AssignItem(11, item, TRUE);
      data.AddItem(item);
      AssignItem(24, item, TRUE);
      data.AddItem(item);
      AssignItem(34, item, TRUE);
      data.AddItem(item);
      AssignItem(35, item, TRUE);
      data.AddItem(item);
      break;
    case seP1:
      AssignItem(55, item, TRUE);
      data.AddItem(item);
      AssignItem(56, item, TRUE);
      data.AddItem(item);
      AssignItem(70, item, TRUE);
      data.AddItem(item);
      AssignItem(210, item, TRUE);
      data.AddItem(item);
      break;
    case seP2:
      AssignItem(94, item, TRUE);
      data.AddItem(item);
      AssignItem(100, item, TRUE);
      data.AddItem(item);
      AssignItem(211, item, TRUE);
      data.AddItem(item);
      AssignItem(86, item, TRUE);
      item.qty = 40;
      data.AddItem(item);
      break;
    case seP3:
      AssignItem(115, item, TRUE);
      data.AddItem(item);
      AssignItem(116, item, TRUE);
      data.AddItem(item);
      AssignItem(130, item, TRUE);
      data.AddItem(item);
      AssignItem(212, item, TRUE);
      data.AddItem(item);
      break;
    case seP4:
      AssignItem(154, item, TRUE);
      data.AddItem(item);
      AssignItem(160, item, TRUE);
      data.AddItem(item);
      AssignItem(212, item, TRUE);
      data.AddItem(item);
      AssignItem(47, item, TRUE);
      item.qty = 2;
      data.AddItem(item);
      break;
    }
  }
}

void AssignShopItems(SHOP *data, BYTE mask, BYTE i1, BYTE i2, BYTE i3, BYTE i4, BYTE i5, BYTE i6, BYTE i7, BYTE i8)
{
   ITEM idata;
   if ((mask & 1) && (i1 > 0))
   {
     if (AssignItem(i1,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
   if ((mask & 2) && (i2 > 0))
   {
     if (AssignItem(i2,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
   if ((mask & 4) && (i3 > 0))
   {
     if (AssignItem(i3,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
   if ((mask & 8) && (i4 > 0))
   {
     if (AssignItem(i4,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
   if ((mask & 16) && (i5 > 0))
   {
     if (AssignItem(i5,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
   if ((mask & 32) && (i6 > 0))
   {
     if (AssignItem(i6,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
   if ((mask & 64) && (i7 > 0))
   {
     if (AssignItem(i7,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
   if ((mask & 128) && (i8 > 0))
   {
     if (AssignItem(i8,idata, FALSE))
      data->itemsAvail.AddItem(idata);
   }
}

void AssignShopItemBytes(SHOP *data, BYTE b1, BYTE b2, BYTE b3)
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
                               (+64)  5 Javalins        (+64)  Short Bow
                               (+128) Mace              (+128) Club
     */
     AssignShopItems(data, b1, 17,18,19,20,0,0,0,0);
     AssignShopItems(data, b2, 9,10,11,12,13,14,15,16);
     AssignShopItems(data, b3, 1,2,3,4,5,6,7,8);
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
     AssignShopItems(data, b1, 29,30,31,0,0,0,0,0);
     AssignShopItems(data, b2, 21,22,23,24,25,26,27,28);
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
     AssignShopItems(data, b2, 40,41,42,43,44,49,0,0);
     AssignShopItems(data, b3, 32,33,34,35,36,37,38,39);
   }
   else if (b1 < 64)
   {
     /*
    +1 Weapons    (Base: Byte#1=48  Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +1   (+1)   Hammer +1         (+1)   10 Arrows +1
        (+2) Short Sword +1  (+2)   2 Javalins +1     (+2)   Battle Axe +1
        (+4) 2-Hand Sword +1 (+4)   Mace +1           (+4)   Cmp Short Bow +1
        (+8) Trident +1      (+8)   10 Bolts +1       (+8)   Cmp Long Bow +1
                             (+16)  Scimitar +1       (+16)  Lt. Crossbow +1
                             (+32)  Sling +1          (+32)  Dagger +1 
                             (+64)  Quarter Staff +1  (+64)  10 Darts +1
                             (+128) Staff Sling +1    (+128) Flail +1
     */
     AssignShopItems(data, b1, 66,67,68,69,0,0,0,0);
     AssignShopItems(data, b2, 58,59,60,61,62,63,64,65);
     AssignShopItems(data, b3, 50,51,52,53,54,55,56,57);
   }
   else if (b1 < 80)
   {
     /*
    +2 Weapons    (Base: Byte#1=64  Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +2   (+1)   Hammer +2         (+1)   10 Arrows +2
        (+2) Short Sword +2  (+2)   2 Javalins +2     (+2)   Battle Axe +2
        (+4) 2-Hand Sword +2 (+4)   Mace +2           (+4)   Cmp Short Bow +2
        (+8) Trident +2      (+8)   10 Bolts +2       (+8)   Cmp Long Bow +2
                             (+16)  Scimitar +2       (+16)  Lt. Crossbow +2
                             (+32)  Sling +2          (+32)  Dagger +2 
                             (+64)  Quarter Staff +2  (+64)  10 Darts +2
                             (+128) Staff Sling +2    (+128) Flail +2
      */
     AssignShopItems(data, b1, 96,97,98,99,0,0,0,0);
     AssignShopItems(data, b2, 88,89,90,91,92,93,94,95);
     AssignShopItems(data, b3, 80,81,82,83,84,85,86,87);
   }
   else if (b1 < 96)
   {
     /*
    +3 Weapons    (Base: Byte#1=80  Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +3   (+1)   Hammer +3         (+1)   10 Arrows +3
        (+2) Short Sword +3  (+2)   2 Javalins +3     (+2)   Battle Axe +3
        (+4) 2-Hand Sword +3 (+4)   Mace +3           (+4)   Cmp Short Bow +3
        (+8) Trident +3      (+8)   10 Bolts +3       (+8)   Cmp Long Bow +3
                             (+16)  Scimitar +3       (+16)  Lt. Crossbow +3
                             (+32)  Sling +3          (+32)  Dagger +3 
                             (+64)  Quarter Staff +3  (+64)  10 Darts +3
                             (+128) Staff Sling +3    (+128) Flail +3
      */
     AssignShopItems(data, b1, 126,127,128,129,0,0,0,0);
     AssignShopItems(data, b2, 118,119,120,121,122,123,124,125);
     AssignShopItems(data, b3, 110,111,112,113,114,115,116,117);
   }
   else if (b1 < 112)
   {
     /*
      +4 Weapons    (Base: Byte#1=96  Byte#2=0   Byte#3=0)
              **Byte#1**            **Byte#2**               **Byte#3**
          (+1) Long Sword +4   (+1)   Hammer +4         (+1)   10 Arrows +4
          (+2) Short Sword +4  (+2)   2 Javalins +4     (+2)   Battle Axe +4
          (+4) 2-Hand Sword +4 (+4)   Mace +4           (+4)   Cmp Short Bow +4
          (+8) Trident +4      (+8)   10 Bolts +4       (+8)   Cmp Long Bow +4
                               (+16)  Scimitar +4       (+16)  Lt. Crossbow +4
                               (+32)  Sling +4          (+32)  Dagger +4 
                               (+64)  Quarter Staff +4  (+64)  10 Darts +4
                               (+128) Staff Sling +4    (+128) Flail +4
      */
     AssignShopItems(data, b1, 156,157,158,159,0,0,0,0);
     AssignShopItems(data, b2, 148,149,150,151,152,153,154,155);
     AssignShopItems(data, b3, 140,141,142,143,144,145,146,147);
   }
   else if (b1 < 128)
   {
     /*
    +5 Weapons    (Base: Byte#1=112 Byte#2=0   Byte#3=0)
            **Byte#1**            **Byte#2**               **Byte#3**
        (+1) Long Sword +5   (+1)   Hammer +5         (+1)   10 Arrows +5
        (+2) Short Sword +5  (+2)   2 Javalins +5     (+2)   Battle Axe +5
        (+4) 2-Hand Sword +5 (+4)   Mace +5           (+4)   Cmp Short Bow +5
        (+8) Trident +5      (+8)   10 Bolts +5       (+8)   Cmp Long Bow +5
                             (+16)  Scimitar +5       (+16)  Lt. Crossbow +5
                             (+32)  Sling +5          (+32)  Dagger +5 
                             (+64)  Quarter Staff +5  (+64)  10 Darts +5
                             (+128) Staff Sling +5    (+128) Flail +5
      */
     AssignShopItems(data, b1, 186,187,188,189,0,0,0,0);
     AssignShopItems(data, b2, 178,179,180,181,182,183,184,185);
     AssignShopItems(data, b3, 170,171,172,173,174,175,176,177);
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
     AssignShopItems(data, b1, 106,107,108,109,0,0,0,0);
     AssignShopItems(data, b2, 78,79,100,101,102,103,104,105);
     AssignShopItems(data, b3, 70,71,72,73,74,75,76,77);
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
     AssignShopItems(data, b1, 166,167,168,169,0,0,0,0);
     AssignShopItems(data, b2, 138,139,160,161,162,163,164,165);
     AssignShopItems(data, b3, 130,131,132,133,134,135,136,137);
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
     AssignShopItems(data, b1, 202,203,204,205,0,0,0,0);
     AssignShopItems(data, b2, 198,199,45,46,47,48,200,201);
     AssignShopItems(data, b3, 190,191,192,193,194,195,196,197);
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
     AssignShopItems(data, b1, 233,222,0,0,0,0,0,0);
     AssignShopItems(data, b2, 214,215,216,217,218,219,220,221);
     AssignShopItems(data, b3, 206,207,208,209,210,211,212,213);
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
     AssignShopItems(data, b2, 232,234,235,236,237,0,0,0);
     AssignShopItems(data, b3, 223,225,226,227,228,229,230,231);
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
     AssignShopItems(data, b3, 238,239,240,241,242,0,0,0);
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
          (+16) Elixer of Youth
      */
     AssignShopItems(data, b3, 243,244,245,247,248,0,0,0);
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
     AssignShopItems(data, b3, 249,250,251,252,253,254,255,0);
   }
}

struct MapEventData
{
  BYTE x;
  BYTE y;
  BYTE event;
  BOOL root;
};

struct ImportLevelData;
ImportLevelData *uadata = NULL;

struct UAImportMapCell
{
  BYTE NWall;
  BYTE EWall;
  BYTE SWall;
  BYTE WWall;
  BYTE Event;
  BYTE BackdropZone;

  BYTE ConvertUAWallSlot(int facing) 
  {
    BYTE value = 0;    

    switch (facing)
    {
    case FACE_NORTH:
      value = (NWall & 0x0F);
      break;
    case FACE_EAST:
      value = (EWall & 0x0F);
      break;
    case FACE_SOUTH:
      value = (SWall & 0x0F);
      break;
    case FACE_WEST:
      value = (WWall & 0x0F);
      break;           
    }

    //if (value > 0)
    //  WallsetsUsed.SetAt( value, value );

    return value;
  }

  BYTE ConvertUABackdropSlot();

  BYTE ConvertUAZone()
  {
    BYTE zone = 0;
    switch (BackdropZone & 0xFC)
    {    
    case 4: zone = 1;  break;
    case 8: zone = 2;  break;
    case 12: zone = 3; break;
    case 16: zone = 4; break;
    case 20: zone = 5; break;
    case 24: zone = 6; break;
    case 28: zone = 7; break;
    default: zone = 0; break;
    }
    return zone;
  }

  BlockageType ConvertUABlockage(int facing)
  {
    BlockageType block = OpenBlk;
    BYTE uablock = 0;

    switch (facing)
    {
    case FACE_NORTH:
      uablock = (NWall & 0xF0) >> 4;
      break;
    case FACE_EAST:
      uablock = (EWall & 0xF0) >> 4;
      break;
    case FACE_SOUTH:
      uablock = (SWall & 0xF0) >> 4;
      break;
    case FACE_WEST:
      uablock = (WWall & 0xF0) >> 4;
      break;           
    }

    switch (uablock)
    {
    case 0: block = OpenBlk;    break;
    case 1: block = OpenSecretBlk; break;
    case 2: block = LockedBlk;     break;
    case 3: block = LockedSecretBlk; break;
    case 4: block = LockedWizardBlk; break;
    case 5: block = LockedWizardSecretBlk; break;
    case 6: block = LockedKey1; break;
    case 7: block = LockedKey2; break;
    case 8: block = LockedKey3; break;
    case 9: block = LockedKey4; break;
    case 10: block = LockedKey5; break;
    case 11: block = LockedKey6; break;
    case 12: block = LockedKey7; break;
    case 13: block = LockedKey8; break;
    case 14: block = BlockedBlk; break;
    case 15: block = FalseDoorBlk; break;
    default: WriteDebugString("Unhandled obstruction %u in ConvertUABlockage()\n"); break;
    }
    return block;
  }

  BOOL ConvertUAOverlandBlockage(int facing)
  {
    BYTE uablock = 0;

    switch (facing)
    {
    case FACE_NORTH:
      uablock = (NWall & 0xF0) >> 4;
      break;
    case FACE_EAST:
      uablock = (EWall & 0xF0) >> 4;
      break;
    case FACE_SOUTH:
      uablock = (SWall & 0xF0) >> 4;
      break;
    case FACE_WEST:
      uablock = (WWall & 0xF0) >> 4;
      break;           
    }

    switch (uablock)
    {
    case 14: // blocked
    case 15: // secret, blocked
      return TRUE;
    }
    return FALSE;
  }
};

//---------------------------
//QUEST #S:
//---------------------------            
//0-7   Key 1-8 
//8-19  Item 1-12
//20-63 Quest 1-44

int GetObjectKeyType(BYTE data)
{
  if (data < 8) return KEY_FLAG;
  if (data < 20) return ITEM_FLAG;
  return QUEST_FLAG;
}

int GetObjectKey(BYTE data)
{
  switch (GetObjectKeyType(data))
  {
  case KEY_FLAG:
    if ((data>=0)&&(data<=7))
      return SpecialKey_Keys[data];
    break;

  case ITEM_FLAG:
    data -= 8;
    if ((data>=0)&&(data<=11))
      return SpecialItem_Keys[data];
    break;

  case QUEST_FLAG:
    data -= 20;
    if ((data>=0)&&(data<=43))
      return Quest_Keys[data];
    break;
  }
  return -1;
}

struct UAImportEvent
{
  GameEvent *AddEvent(BYTE index);
  void ConvertEventControl(EVENT_CONTROL &data);
  eventType ConvertEventType();

  BYTE EventType;
  BYTE EventTrigger;
  BYTE EventTriggerData;
  BYTE ChainEvent;
  BYTE EventData[16];
};

struct UAImportStrings
{
  BOOL GetStringAt(WORD offset, CString &result);
  WORD GetStrPtr(WORD offset);
  int GetMaxStrLen(int index);

  BYTE StringCount; // total compressed length of all strings
  BYTE StringLength[400];
  BYTE Strings[6761];
};

WORD UAImportStrings::GetStrPtr(WORD offset)
{
  WORD index = 0; // base offset for strings
  //offset--;

  ASSERT(offset < 399);
  // add lengths of all strings that preceed offset
  for (int i=0;i<offset;i++)
  {
    //ASSERT(StringLength[i] > 0);
    index += StringLength[i]; // nbr compressed bytes
  }

  ASSERT((index + 6200) < 12961);
  return index;
  //return offset + 6201;
}

int UAImportStrings::GetMaxStrLen(int index)
{
  /*
  static int MaxLen = 229;
  return MaxLen;
  */
  return StringLength[index];
}

/*
  The compressed name format
  uses six bits per character, with three bytes being sufficient for
  four characters.  Since six bits
  are insufficient to store all eight bytes of an ASCII character code,
  only a subset of the ASCII character set is available.  ASCII values
  of zero and 32-63 are used as is.  Values in the range 1-31 have an
  implicit "1" bit appended to them, shifting the range to 65-95.
*/

BOOL UAImportStrings::GetStringAt(WORD offset, CString &result)
{
  result = "";

  if ((offset == 0) || (offset > 400))
    return FALSE;

  //if (offset <= 1)
  //  return FALSE;

  long int index = GetStrPtr(offset-1);  
  
  int LongestString = 228;//min(GetMaxStrLen(), 229);
  BYTE buffer[512];
  memset(buffer, 0, sizeof(buffer));

  int j = 0;
  BYTE bitPattern = 0;
  BYTE bitNum   = 7;
  BYTE bitCount = 0;
  BYTE done = 0;

  /* Loop until end of string found */
  while ((!done) && (j < LongestString))
  {
    /* Left shift bit pattern to add new bit */
    bitPattern <<= 1;

    /* If the current bit is a 1, add 1 to the pattern */
    if (Strings[index] & (1 << bitNum))
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
        done = TRUE;      
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
  WriteDebugString("GetStringAt( %u ) returns %s, len %u\n", 
                   offset, result, result.GetLength());

  return (!result.IsEmpty());
}
 

// nowhere to put this stuff directly
struct ImportLevelData
{
  ImportLevelData() { Clear(); }
  void Clear()
  {
    Wall1 = 0;
    Wall2 = 0;
    Wall3 = 0;
    Backdrop1 = 0;
    Backdrop2 = 0;
    Backdrop3 = 0;
    Backdrop4 = 0;
    DungeonCombatArt = 0;
    WildernessCombatArt = 0;
    ZoneMonster = 0;
    ZoneCamp = 0;
    ZoneTreasure = 0;
    memset(RestEvents, 0, sizeof(RestEvents));
    memset(StepEvents, 0, sizeof(StepEvents));
    memset(MapData, 0, sizeof(MapData));
    memset(EventData, 0, sizeof(EventData));
    memset(&StringData, 0, sizeof(StringData));
    memset(MapEvents, 0, sizeof(MapEvents));
  }

  UAImportMapCell GetMapCell(int level, int x, int y)
  {
    int index = (y * globalData.levelInfo.stats[level].area_width) + x;
    return MapData[index];
  }

  UAImportEvent GetEvent(int index) { return EventData[index]; }

  BYTE Wall1;
  BYTE Wall2;
  BYTE Wall3;
  BYTE Backdrop1;
  BYTE Backdrop2;
  BYTE Backdrop3;
  BYTE Backdrop4;
  BYTE DungeonCombatArt;
  BYTE WildernessCombatArt;
  BYTE RestEvents[8];
  BYTE StepEvents[8];
  BYTE ZoneMonster;
  BYTE ZoneCamp;
  BYTE ZoneTreasure;
  UAImportMapCell MapData[576];
  MapEventData MapEvents[MAX_IMPORT_EVENTS+1];
  UAImportEvent EventData[MAX_IMPORT_EVENTS+1];
  UAImportStrings StringData;  
};

BYTE OffsetWallSlotIndex(BYTE w)
{
  BYTE offset = 0;
  if ((w >= 1) && (w <= 5))
  {
    offset=w-1;
    return (offset+UAWallGroupToArtSlotBase(uadata->Wall1));
  }
  else if ((w >= 6) && (w <= 10))
  {
    offset=w-6;
    return (offset+UAWallGroupToArtSlotBase(uadata->Wall2));
  }
  else if ((w >= 11) && (w <= 15))
  {
    offset=w-11;
    return (offset+UAWallGroupToArtSlotBase(uadata->Wall3));
  }
  else
    return w;
}


BYTE UAImportMapCell::ConvertUABackdropSlot() 
{ 
  BYTE value = (BackdropZone & 0x03) + 1; 
  return (GetBackdropType(uadata->Backdrop1,uadata->Backdrop2,uadata->Backdrop3,uadata->Backdrop4, value));
}

void addSoundEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(SOUND_EVENT)));
   SOUND_EVENT *data = (SOUND_EVENT*)(event);

   CString temp;
   AssignSound(EventByte(5, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(6, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(7, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(8, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(9, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(10, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(11, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(12, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(13, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
   AssignSound(EventByte(14, pData), temp);
   if (temp != "")
     data->sounds.AddTail(temp);
}

void addUtilitiesEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(UTILITIES_EVENT_DATA)));
   UTILITIES_EVENT_DATA *data = (UTILITIES_EVENT_DATA*)(event);

   BYTE op = EventByte(5, pData);
   BYTE math = op & 3;
   switch (math)
   {
   case 1: // stored in
     data->operation = MO_StoredIn;
     break;
   case 2: // added to
     data->operation = MO_AddedTo;
     break;
   case 3: // subtracted from
     data->operation = MO_SubtractFrom;
     break;
   }

   if (HasMask(op, 4))
     data->itemCheck = MIC_AllItems;
   else if (HasMask(op, 8))
     data->itemCheck = MIC_AtLeastOneItem;

   if (HasMask(op, 16))
     data->endPlay = TRUE;

   data->mathItemType = GetObjectKeyType(EventByte(6, pData));
   data->mathItemIndex = GetObjectKey(EventByte(6, pData));
   data->mathAmount = EventByte(7, pData);

   SPECIAL_OBJECT_EVENT_DATA sitem;   
   sitem.operation = SPECIAL_OBJECT_CHECK;   

   sitem.index = GetObjectKey(EventByte(8, pData));
   sitem.ItemType = GetObjectKeyType(EventByte(8, pData));
   data->items.Add(sitem);

   sitem.index = GetObjectKey(EventByte(9, pData));
   sitem.ItemType = GetObjectKeyType(EventByte(9, pData));
   data->items.Add(sitem);
   
   sitem.index = GetObjectKey(EventByte(10, pData));
   sitem.ItemType = GetObjectKeyType(EventByte(10, pData));
   data->items.Add(sitem);
   
   sitem.index = GetObjectKey(EventByte(11, pData));
   sitem.ItemType = GetObjectKeyType(EventByte(11, pData));
   data->items.Add(sitem);
   
   sitem.index = GetObjectKey(EventByte(12, pData));
   sitem.ItemType = GetObjectKeyType(EventByte(12, pData));
   data->items.Add(sitem);
   
   sitem.index = GetObjectKey(EventByte(13, pData));
   sitem.ItemType = GetObjectKeyType(EventByte(13, pData));
   data->items.Add(sitem);

   data->resultItemType = GetObjectKeyType(EventByte(14, pData));
   data->resultItemIndex = GetObjectKey(EventByte(14, pData));
}

void addQuestEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUEST_EVENT_DATA)));
   QUEST_EVENT_DATA *data = (QUEST_EVENT_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   AssignPic(EventByte(7,pData), data->pic, (EventByte(8,pData) & 128));
   
   BYTE temp = EventByte(8,pData);
   temp &= 127;
   if (temp == 0)
     data->accept = QA_Impossible;
   else if (HasMask(temp,40))
     data->accept = QA_AutoAccept;
   else if (HasMask(temp,32))
     data->accept = QA_ImpossibleAuto;
   else if (HasMask(temp,24))
     data->accept = QA_OnYesOrNo;
   else if (HasMask(temp, 16))
     data->accept = QA_OnNo;
   else if (HasMask(temp,8))
     data->accept = QA_OnYes;

   data->completeOnAccept = HasMask(temp,64);
   data->failOnRejection  = HasMask(temp,4);

   data->SetQuestTypeAndID(QUEST_FLAG, GetObjectKey(EventByte(9, pData)));
   data->stage = EventByte(10, pData) + 1;
}

void addSmallTownEvent(GameEvent *event, BYTE *pData)
{
   SMALL_TOWN_DATA *data = (SMALL_TOWN_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   AssignPic(EventByte(7,pData), data->pic, (EventByte(8,pData) & 128));
   
   BYTE temp = EventByte(8, pData);
   if (HasMask(temp, 1))
   {
     TEMPLE *pEvent = new TEMPLE;
     pEvent->text = "WELCOME TO THE TEMPLE";
     pEvent->text2 = "HOW MAY WE AID YOU?";
     pEvent->forceExit = FALSE;
     pEvent->costFactor = ConvertCostModifier(EventByte(13,pData));
     BYTE level = EventByte(10,pData);
     if (HasMask(level, 224))
       pEvent->maxLevel = 7;
     else if (HasMask(level, 192))
       pEvent->maxLevel = 6;
     else if (HasMask(level, 160))
       pEvent->maxLevel = 5;
     else if (HasMask(level, 128))
       pEvent->maxLevel = 4;
     else if (HasMask(level, 96))
       pEvent->maxLevel = 3;
     else if (HasMask(level, 64))
       pEvent->maxLevel = 2;
     else if (HasMask(level, 32))
       pEvent->maxLevel = 1;

     // need to add some spells here

     data->TempleChain = levelData.eventData.AddEvent(pEvent);     
   }
   if (HasMask(temp, 2))
   {
     TRAININGHALL *pEvent = new TRAININGHALL;
     pEvent->x = -1;
     pEvent->y = -1;
     pEvent->text = "WELCOME TO THE TRAINING HALL";
     pEvent->forceExit = FALSE;
     //BYTE train = EventByte(9, pData);
     //pEvent->TrainMagicUser = HasMask(train, 1);
     //pEvent->TrainCleric = HasMask(train, 2);
     //pEvent->TrainThief = HasMask(train, 4);
     //pEvent->TrainFighter = HasMask(train, 8);
     //pEvent->TrainPaladin = HasMask(train, 16);
     //pEvent->TrainRanger = HasMask(train, 32);
     NotImplemented(0x3cde3, false);
     costFactorType mod = ConvertCostModifier(EventByte(13,pData));
     pEvent->Cost = ApplyCostFactor(mod, 1000);
     data->TrainingHallChain = levelData.eventData.AddEvent(pEvent);     
   }
   if (HasMask(temp, 4))
   {
     SHOP *pEvent = new SHOP;
     pEvent->x = -1;
     pEvent->y = -1;
     pEvent->text = "WELCOME TO THE SHOP";
     pEvent->forceExit = FALSE;
     BYTE cost = EventByte(10, pData);
     pEvent->costFactor = ConvertCostModifier( cost&31 );
     BYTE b1 = EventByte(15,pData);
     BYTE b2 = EventByte(16,pData);
     BYTE b3 = EventByte(17,pData);
     AssignShopItemBytes(pEvent, b1,b2,b3);
     b1 = EventByte(18,pData);
     b2 = EventByte(19,pData);
     b3 = EventByte(20,pData);
     AssignShopItemBytes(pEvent, b1,b2,b3);
     data->ShopChain = levelData.eventData.AddEvent(pEvent);     
   }
   if (HasMask(temp, 8))
   {
     CAMP_EVENT_DATA *pEvent = new CAMP_EVENT_DATA;
     pEvent->x = -1;
     pEvent->y = -1;
     pEvent->text = "WELCOME TO THE INN";
     pEvent->ForceExit = FALSE;
     data->InnChain = levelData.eventData.AddEvent(pEvent);     
   }
   if (HasMask(temp, 16))
   {
     TAVERN *pEvent = new TAVERN;
     pEvent->forceExit = FALSE;
     pEvent->allowDrinks = FALSE;
     pEvent->allowFights = FALSE;
     CString tale;
     uadata->StringData.GetStringAt(GetWord(11, pData), tale);
     pEvent->tales[0].tale = tale;

/* not implemeted
      if (talesPresent)
      {
        int FirstEmptyTale = 0;
        if (((TAVERN*)pEvent)->tales[0].tale.IsEmpty())
          FirstEmptyTale = 0;
        else
          FirstEmptyTale = 1;
        for (int i=0;i<6;i++)
        {
          if (!tavernTales.tales[i].tale.IsEmpty())
          {
            ((TAVERN*)pEvent)->tales[FirstEmptyTale].tale = tavernTales.tales[i].tale;
            FirstEmptyTale++;
          }
        }
        // preceeding tavern tale event might be a root event
        data->x = tavernTales.x;
        data->y = tavernTales.y;
        talesPresent=false;
      }      
*/
     data->TavernChain = levelData.eventData.AddEvent(pEvent);     
   }
   if (HasMask(temp, 32))
   {
     VAULT_EVENT_DATA *pEvent = new VAULT_EVENT_DATA;
     pEvent->x = -1;
     pEvent->y = -1;
     pEvent->text = "WELCOME TO THE VAULT";
     data->VaultChain = levelData.eventData.AddEvent(pEvent);     
   }
}

void addNPCEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(ADD_NPC_DATA)));
   ADD_NPC_DATA *data = (ADD_NPC_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;
   if (HasMask(temp,1))
     data->distance = Nearby;
   else if (HasMask(temp, 2))
     data->distance = FarAway;
   else
     data->distance = UpClose;
   
/*
   data->charAdded = GetMonsterKey(EventByte(9,pData));
   data->charAdded = GetMonsterKey(EventByte(9,pData));
*/ NotImplemented(0x7105c7, false);

   data->hitPointMod = EventByte(10,pData);
}

void addVaultEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(VAULT_EVENT_DATA)));
   VAULT_EVENT_DATA *data = (VAULT_EVENT_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));
   temp &= 127;
   data->ForceBackup = HasMask(temp, 4);
}

void addCampEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(CAMP_EVENT_DATA)));
   CAMP_EVENT_DATA *data = (CAMP_EVENT_DATA*)(event);
   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));
   temp &= 127;
   data->ForceExit = HasMask(temp, 4);
}

void addCombatEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(COMBAT_EVENT_DATA)));
   COMBAT_EVENT_DATA *data = (COMBAT_EVENT_DATA*)(event);

   data->randomMonster = FALSE;
   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);

   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));
   temp &= 127;

   data->monsterMorale = temp & 0x7F;

   temp = EventByte(9, pData);
   data->outdoors = HasMask(temp,32);

   if (HasMask(temp, 64))
     data->m_eSurprise = PartySurprised;
   else if (HasMask(temp, 128))
     data->m_eSurprise = MonsterSurprised;
   else
     data->m_eSurprise = Neither;

   MONSTER_EVENT monster;
   monster.m_type = MONSTER_TYPE;
   monster.qty = temp & 0x1F;


/*
   monster.monster = GetMonsterKey(EventByte(10,pData));
   if (monster.monster > 0) monster.type = GetMonsterType(EventByte(10,pData));

   if ((monster.qty > 0) && (monster.monster > 0))
    data->monsters.Add(monster);
*/ NotImplemented(0x31b90,false);  
    

   monster.Clear();
   temp = EventByte(11,pData);
   monster.qty = temp & 0x1F;

   BOOL f4 = HasMask(temp, 32);
   //BOOL f5 = HasMask(temp, 64);
   //BOOL f6 = HasMask(temp, 128);


/*
   monster.monster = GetMonsterKey(EventByte(12,pData));
   if ((monster.qty > 0) && (monster.monster > 0))
   {
     if (monster.monster > 0) monster.type = GetMonsterType(EventByte(12,pData));
     data->monsters.Add(monster);
   }
*/ NotImplemented(0x419ca0, false);



   temp = EventByte(13, pData);
   monster.Clear();
   monster.qty = temp & 0x1F;
   
   data->autoApproach = HasMask(temp, 32); 
   data->partyNeverDies = HasMask(temp, 64);
   data->noMonsterTreasure = HasMask(temp, 128);


/*
   monster.monster = GetMonsterKey(EventByte(14,pData));
   if ((monster.qty > 0) && (monster.monster > 0))
   {
     if (monster.monster > 0) monster.type = GetMonsterType(EventByte(14,pData));
     data->monsters.Add(monster);
   }
*/ NotImplemented(0x5190a6, false);



   temp = EventByte(15, pData);
   monster.Clear();
   monster.qty = temp & 0x1F;

   data->noMagic = HasMask(temp, 128);
   if (HasMask(temp, 32))
     data->distance = Nearby;
   else if (HasMask(temp, 64))
     data->distance = FarAway;
   else
     data->distance = UpClose;

/*
   monster.monster = GetMonsterKey(EventByte(16,pData));
   if ((monster.qty > 0) && (monster.monster > 0))
   {
     if (monster.monster > 0) monster.type = GetMonsterType(EventByte(16,pData));
     data->monsters.Add(monster);
   }
*/ NotImplemented(0x61b9a, false);


   temp = EventByte(17, pData);
   monster.Clear();
   monster.friendly = f4;
   monster.qty = temp & 0x1F;

   if (HasMask(temp, 32))
     data->direction = North;
   else if (HasMask(temp, 64))
     data->direction = East;
   else if (HasMask(temp, 128))
     data->direction = South;
   else if (HasMask(EventByte(19,pData), 32))
     data->direction = West;
   else
     data->direction = InFront;


/*
   monster.monster = GetMonsterKey(EventByte(18,pData));
   monster.friendly = f5;
   if ((monster.qty > 0) && (monster.monster > 0))
   {
     if (monster.monster > 0) monster.type = GetMonsterType(EventByte(18,pData));
     data->monsters.Add(monster);
   }
*/ NotImplemented(0x71c84, false);


   temp = EventByte(19, pData);
   monster.Clear();
   monster.qty = EventByte(19,pData) & 0x1F;

   if (HasMask(temp, 192))
     data->turningMod = Impossible;
   else if (HasMask(temp, 64))
     data->turningMod = Hard;
   else if (HasMask(temp, 128))
     data->turningMod = Difficult;
   else 
     data->turningMod = None;


/*
   monster.monster = GetMonsterKey(EventByte(20,pData));
   monster.friendly = f6;
   if ((monster.qty > 0) && (monster.monster > 0))
   {
     if (monster.monster > 0) monster.type = GetMonsterType(EventByte(20,pData));
     data->monsters.Add(monster);
   }
*/ NotImplemented(0x8145a0, false);


}

void addTreasureEvent(GameEvent *event, BYTE *pData)
{
   GIVE_TREASURE_DATA *data = (GIVE_TREASURE_DATA*)(event);
   
   int platinum = GetWord(5, pData);
   int gems = GetWord(9, pData);
   int jewelry = GetWord(11, pData);

   data->money.Add(PlatinumType, platinum);
   for (int g=0;g<gems;g++) data->money.AddGem();
   for (int j=0;j<jewelry;j++) data->money.AddJewelry();

   BOOL id = HasMask(EventByte(8, pData), 128);

   ITEM idata;
   if (AssignItem(EventByte(13, pData), idata, id))
      data->items.AddItem(idata);
   if (AssignItem(EventByte(14, pData), idata, id))   
      data->items.AddItem(idata);
   if (AssignItem(EventByte(15, pData), idata, id))   
      data->items.AddItem(idata);
   if (AssignItem(EventByte(16, pData), idata, id))   
      data->items.AddItem(idata);
   if (AssignItem(EventByte(17, pData), idata, id))
      data->items.AddItem(idata);
   if (AssignItem(EventByte(18, pData), idata, id))   
      data->items.AddItem(idata);
   if (AssignItem(EventByte(19, pData), idata, id))   
      data->items.AddItem(idata);
   if (AssignItem(EventByte(20, pData), idata, id))   
      data->items.AddItem(idata);
}

void addGiveDamageEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GIVE_DAMAGE_DATA)));
   GIVE_DAMAGE_DATA *data = (GIVE_DAMAGE_DATA*)(event);
   
   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

   if (HasMask(temp,12))
     data->who = ChanceOnEach;
   else if (HasMask(temp, 4))
     data->who = ActiveChar;
   else if (HasMask(temp, 8))
     data->who = OneAtRandom;
   else
     data->who = EntireParty;

   if (HasMask(temp, 48))
     data->eventSave = UseTHAC0;
   else if (HasMask(temp,16))
     data->eventSave = SaveForHalf;
   else if (HasMask(temp, 32))
     data->eventSave = SaveNegates;
   else
     data->eventSave = NoSave;

   data->nbrAttacks = EventByte(9, pData);
   data->dmgDiceQty = EventByte(10, pData);
   data->dmgDice = EventByte(11, pData);
   data->dmgBonus = EventByte(12, pData);
   data->attackTHAC0 = 60-EventByte(13, pData);

   temp = EventByte(14, pData);
   data->saveBonus = temp & 0x0F;

   if (HasMask(temp, 48))
     data->spellSave = BreathWeapon;   
   else if (HasMask(temp, 16))
     data->spellSave = PetPoly;
   else if (HasMask(temp, 32))
     data->spellSave = RodStaffWand;
   else if (HasMask(temp, 64))
     data->spellSave = Sp;
   else
     data->spellSave = ParPoiDM;

   if (HasMask(EventByte(15,pData), 32))
     data->distance = Nearby;
   else if (HasMask(EventByte(15,pData), 64))
     data->distance = FarAway;
   else
     data->distance = UpClose;

   data->chancePerAttack = EventByte(17,pData); 
}

void addEncounterEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(ENCOUNTER_DATA)));
   ENCOUNTER_DATA *data = (ENCOUNTER_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

   if (HasMask(temp, 1))
     data->distance = Nearby;
   else if (HasMask(temp, 2))
     data->distance = FarAway;
   else
     data->distance = UpClose;

   data->monsterSpeed = EventByte(9, pData);

   data->buttons.numButtons = 0;
   data->buttons.buttons[0].label = "Approach";
   data->buttons.buttons[0].present = FALSE;
   data->buttons.buttons[1].label = "Retreat";
   data->buttons.buttons[1].present = FALSE;
   data->buttons.buttons[2].label = "Fight";
   data->buttons.buttons[2].present = FALSE;
   data->buttons.buttons[3].label = "Wait";
   data->buttons.buttons[3].present = FALSE;
   data->buttons.buttons[4].label = "Talk";
   data->buttons.buttons[4].present = FALSE;

   temp = EventByte(10, pData);
   if (HasMask(temp, 8))
   {
     data->buttons.numButtons++;
     data->buttons.buttons[0].present=TRUE;
     if (HasMask(temp, 16)) 
     {
       data->buttons.buttons[0].allowedUpClose = FALSE;
       data->buttons.buttons[0].onlyUpClose = FALSE;
     }
     else
     {
       data->buttons.buttons[0].allowedUpClose = TRUE;
       data->buttons.buttons[0].onlyUpClose = FALSE;
     }
     switch (temp & 0x07)
     {
     case 0: data->buttons.buttons[0].optionResult = DecreaseRange; break;
     case 1: data->buttons.buttons[0].optionResult = CombatSlowPartySurprised; break;
     case 2: data->buttons.buttons[0].optionResult = CombatSlowMonsterSurprised; break;
     case 3: data->buttons.buttons[0].optionResult = CombatNoSurprise; break;
     case 4: data->buttons.buttons[0].optionResult = Talk; break;
     case 5: data->buttons.buttons[0].optionResult = EscapeIfFastPartyElseCombat; break;
     case 6: data->buttons.buttons[0].optionResult = NoResult; break;
     }
   }

   temp = EventByte(11, pData);
   if (HasMask(temp, 8))
   {
     data->buttons.numButtons++;
     data->buttons.buttons[1].present=TRUE;
     //if (temp & 16) data->buttons.buttons[1].allowedUpClose = FALSE;
     switch (temp & 0x07)
     {
     case 0: data->buttons.buttons[1].optionResult = DecreaseRange; break;
     case 1: data->buttons.buttons[1].optionResult = CombatSlowPartySurprised; break;
     case 2: data->buttons.buttons[1].optionResult = CombatSlowMonsterSurprised; break;
     case 3: data->buttons.buttons[1].optionResult = CombatNoSurprise; break;
     case 4: data->buttons.buttons[1].optionResult = Talk; break;
     case 5: data->buttons.buttons[1].optionResult = EscapeIfFastPartyElseCombat; break;
     case 6: data->buttons.buttons[1].optionResult = NoResult; break;
     }
   }

   temp = EventByte(12, pData);
   if (HasMask(temp, 8))
   {
     data->buttons.numButtons++;
     data->buttons.buttons[2].present=TRUE;
     if (HasMask(temp, 16)) 
     {
       data->buttons.buttons[2].allowedUpClose = TRUE;
       data->buttons.buttons[0].onlyUpClose = TRUE;
     }
     else
     {
       data->buttons.buttons[2].allowedUpClose = TRUE;
       data->buttons.buttons[0].onlyUpClose = FALSE;
     }
     switch (temp & 0x07)
     {
     case 0: data->buttons.buttons[2].optionResult = DecreaseRange; break;
     case 1: data->buttons.buttons[2].optionResult = CombatSlowPartySurprised; break;
     case 2: data->buttons.buttons[2].optionResult = CombatSlowMonsterSurprised; break;
     case 3: data->buttons.buttons[2].optionResult = CombatNoSurprise; break;
     case 4: data->buttons.buttons[2].optionResult = Talk; break;
     case 5: data->buttons.buttons[2].optionResult = EscapeIfFastPartyElseCombat; break;
     case 6: data->buttons.buttons[2].optionResult = NoResult; break;
     }
   }

   temp = EventByte(13, pData);
   if (HasMask(temp, 8))
   {
     data->buttons.numButtons++;
     data->buttons.buttons[3].present=TRUE;
     //if (temp & 16) data->buttons.buttons[3].allowedUpClose = FALSE;
     switch (temp & 0x07)
     {
     case 0: data->buttons.buttons[3].optionResult = DecreaseRange; break;
     case 1: data->buttons.buttons[3].optionResult = CombatSlowPartySurprised; break;
     case 2: data->buttons.buttons[3].optionResult = CombatSlowMonsterSurprised; break;
     case 3: data->buttons.buttons[3].optionResult = CombatNoSurprise; break;
     case 4: data->buttons.buttons[3].optionResult = Talk; break;
     case 5: data->buttons.buttons[3].optionResult = EscapeIfFastPartyElseCombat; break;
     case 6: data->buttons.buttons[3].optionResult = NoResult; break;
     }
   }

   temp = EventByte(14, pData);
   if (HasMask(temp, 8))
   {
     data->buttons.numButtons++;
     data->buttons.buttons[4].present=TRUE;
     if (HasMask(temp, 16)) 
     {
       data->buttons.buttons[2].allowedUpClose = TRUE;
       data->buttons.buttons[0].onlyUpClose = TRUE;
     }
     else
     {
       data->buttons.buttons[2].allowedUpClose = TRUE;
       data->buttons.buttons[0].onlyUpClose = FALSE;
     }
     switch (temp & 0x07)
     {
     case 0: data->buttons.buttons[4].optionResult = DecreaseRange; break;
     case 1: data->buttons.buttons[4].optionResult = CombatSlowPartySurprised; break;
     case 2: data->buttons.buttons[4].optionResult = CombatSlowMonsterSurprised; break;
     case 3: data->buttons.buttons[4].optionResult = CombatNoSurprise; break;
     case 4: data->buttons.buttons[4].optionResult = Talk; break;
     case 5: data->buttons.buttons[4].optionResult = EscapeIfFastPartyElseCombat; break;
     case 6: data->buttons.buttons[4].optionResult = NoResult; break;
     }
   }

   temp = EventByte(15, pData);
   switch (temp)
   {
   case 1: data->zeroRangeResult = CombatSlowPartySurprised; break;
   case 2: data->zeroRangeResult = CombatSlowMonsterSurprised; break;
   case 3: data->zeroRangeResult = CombatNoSurprise; break;
   case 4: data->zeroRangeResult = Talk; break;
   case 5: data->zeroRangeResult = EscapeIfFastPartyElseCombat; break;
   }
}

void addPasswordEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(PASSWORD_DATA)));
   PASSWORD_DATA *data = (PASSWORD_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

   uadata->StringData.GetStringAt(GetWord(9, pData), data->password);
   uadata->StringData.GetStringAt(GetWord(12, pData), data->text3);
   uadata->StringData.GetStringAt(GetWord(14, pData), data->text2);

   data->matchCriteria = ExactMatch;
   data->matchCase = FALSE; // UA is all upper case, DC isn't

   temp = EventByte(11, pData);
   if (HasMask(temp, 12))
     data->successAction = BackupOneStep;
   else if (HasMask(temp, 8))
     data->successAction = Teleport;
   else if (HasMask(temp, 4))
     data->successAction = ChainEvent;

   if (HasMask(temp, 3))
     data->failAction = BackupOneStep;
   else if (HasMask(temp, 2))
     data->failAction = Teleport;
   else if (HasMask(temp, 1))
     data->failAction = ChainEvent;

   data->successTransferData.execEvent = HasMask(temp,64);
   data->successTransferData.destEP = -1;
   data->failTransferData.execEvent = HasMask(temp, 64);
   data->failTransferData.destEP = -1;

   if (HasMask(temp, 48))
     data->successTransferData.m_facing = FaceWest;
   else if (HasMask(temp, 32))
     data->successTransferData.m_facing = FaceSouth;
   else if (HasMask(temp, 16))
     data->successTransferData.m_facing = FaceEast;
   else
    data->successTransferData.m_facing = FaceNorth;

   data->successTransferData.destX = EventByte(18, pData);
   data->successTransferData.destY = EventByte(19, pData);
   data->successTransferData.destLevel = CurrentLevel;
   data->failTransferData.destY = data->successTransferData.destY;
   data->failTransferData.destX = data->successTransferData.destX;
   data->failTransferData.destLevel = CurrentLevel;
   data->nbrTries = EventByte(20, pData);
}

void addGiveExpEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GAIN_EXP_DATA)));
   GAIN_EXP_DATA *data = (GAIN_EXP_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;
   if (HasMask(temp, 4))
     data->who = ActiveChar;
   else
     data->who = EntireParty;
   data->experience = GetDWord(9, pData);
   AssignSound(EventByte(13, pData), data->sound);
   data->chance = 100;
}

void addTourEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GUIDED_TOUR)));
   GUIDED_TOUR *data = (GUIDED_TOUR*)(event);
   
   for (int s=0;s<MAX_TOUR_STEPS;s++)
     data->steps[s].text="";

   data->tour_y = EventByte(5,pData);
   data->tour_x = EventByte(6,pData);
   int MaxSteps = EventByte(7,pData);
   BYTE temp = EventByte(8,pData);
   data->useStartLoc = HasMask(temp, 16);
   data->executeEvent = HasMask(temp,32); 

   if (HasMask(temp,12))
     data->facing = FaceWest;
   else if (HasMask(temp,4))
     data->facing = FaceEast;
   else if (HasMask(temp,8))
     data->facing = FaceSouth;
   else
     data->facing = FaceNorth;

   TOUR_STEP step;
   BYTE count=0;
   int i=9;

   while ((i<=14) && (count < MaxSteps))
   {
     temp = EventByte(i,pData);

     step.Clear();
     if (HasMask(temp,3))
       step.step = TStep_Forward;
     else if (HasMask(temp,2))
       step.step = TStep_Right;
     else if (HasMask(temp, 1))
       step.step = TStep_Left;
     else
       step.step = TStep_Pause;

     data->steps[count++] = step;
     if (count >= MaxSteps)
       break;

     step.Clear();
     if (HasMask(temp,12))
       step.step = TStep_Forward;
     else if (HasMask(temp,8))
       step.step = TStep_Right;
     else if (HasMask(temp,4))
       step.step = TStep_Left;
     else
       step.step = TStep_Pause;

     data->steps[count++] = step;
     if (count >= MaxSteps)
       break;

     step.Clear();
     if (HasMask(temp, 48))
       step.step = TStep_Forward;
     else if (HasMask(temp, 32))
       step.step = TStep_Right;
     else if (HasMask(temp, 16))
       step.step = TStep_Left;
     else
       step.step = TStep_Pause;

     data->steps[count++] = step;
     if (count >= MaxSteps)
       break;

     step.Clear();
     if (HasMask(temp, 192))
       step.step = TStep_Forward;
     else if (HasMask(temp,128))
       step.step = TStep_Right;
     else if (HasMask(temp,64))
       step.step = TStep_Left;
     else
       step.step = TStep_Pause;

     data->steps[count++] = step;
     if (count >= MaxSteps)
       break;
   }

   BYTE textindex = EventByte(15,pData);
   if (textindex < MAX_TOUR_STEPS)
    uadata->StringData.GetStringAt(GetWord(17, pData), data->steps[textindex].text);
   
   if (MaxSteps > 1)
   {
     textindex = EventByte(16,pData);
     if (textindex < MAX_TOUR_STEPS)
      uadata->StringData.GetStringAt(GetWord(19, pData), data->steps[textindex].text);
   }
}

void addNPCSaysEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(NPC_SAYS_DATA)));
   NPC_SAYS_DATA *data = (NPC_SAYS_DATA*)(event);

   BYTE temp = EventByte(5,pData);

   data->mustHitReturn = 
     HasMask(temp,1)||HasMask(temp,2)||HasMask(temp,4)||HasMask(temp,8)||HasMask(temp,16);

   temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

/*   
   data->partySeesChar = GetMonsterKey(EventByte(6,pData));
*/ NotImplemented(0x813a09, false);



   if (HasMask(temp,1))
     data->distance = Nearby;
   else if (HasMask(temp, 2))
     data->distance = FarAway;
   else
     data->distance = UpClose;

   data->highlight = FALSE;

   CString buffer;
   CString result="";

   if (HasMask(temp, 4))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(9, pData), buffer);
   result += buffer;
   if (HasMask(temp, 4))
     result += "/h";

   data->text = result;   
   result = "";

   if (HasMask(temp, 8))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(11, pData), buffer);
   result += buffer;
   if (HasMask(temp, 8))
     result += "/h";

   data->text += result;   
   result = "";

   if (HasMask(temp, 16))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(13, pData), buffer);
   result += buffer;
   if (HasMask(temp, 16))
     result += "/h";

   data->text += result;   
   result = "";

   if (HasMask(temp, 32))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(15, pData), buffer);
   result += buffer;
   if (HasMask(temp, 32))
     result += "/h";

   data->text += result;   
   result = "";

   if (HasMask(temp, 64))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(17, pData), buffer);
   result += buffer;
   if (HasMask(temp, 64))
     result += "/h";

   data->text += result;
   result = "";

   AssignSound(EventByte(19, pData), data->sound);
}

void addQListEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_LIST_DATA)));
  QUESTION_LIST_DATA *data = (QUESTION_LIST_DATA*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->buttons.title);

  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

  data->buttons.numListButtons = 5;
  data->buttons.buttons[0].present = TRUE;
  data->buttons.buttons[1].present = TRUE;
  data->buttons.buttons[2].present = TRUE;
  data->buttons.buttons[3].present = TRUE;
  data->buttons.buttons[4].present = TRUE;

  data->buttons.buttons[0].postChainAction = 
      (HasMask(temp, 4)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[1].postChainAction = 
      (HasMask(temp, 8)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[2].postChainAction = 
      (HasMask(temp, 16)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[3].postChainAction = 
      (HasMask(temp, 32)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[4].postChainAction = 
      (HasMask(temp, 64)) ? LabelReturnToQuestion : LabelDoNothing;

  CString buffer;
  uadata->StringData.GetStringAt(GetWord(9, pData), buffer);
  
  if (!buffer.IsEmpty())
  {
    // parse button labels
    // each label is preceeded by a ^, as in
    // ^button1 text^button2 text^^^
    // empty buttons have no text after ^

    int L;
    for (L=0;L<5;L++)
      data->buttons.buttons[L].label="";

    char lab[50];
    char *start = (char*)strchr(buffer, '^');
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
        data->buttons.buttons[0].label = lab;
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
        data->buttons.buttons[1].label = lab;
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
        data->buttons.buttons[2].label = lab;
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
        data->buttons.buttons[3].label = lab;
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
        data->buttons.buttons[4].label = lab;
      }
    }
    else end = start;

    for (L=0;L<5;L++)
      if (data->buttons.buttons[L].label.IsEmpty())
        data->buttons.buttons[L].present=FALSE;
  }

  temp = EventByte(16,pData);

  // is 'backup' combinable with 'return to question'?
  // answer: yes, both options can be enabled in UA, DC doesn't support it
  if ((HasMask(temp, 1)) && (data->buttons.buttons[0].postChainAction == LabelDoNothing))
    data->buttons.buttons[0].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 2)) && (data->buttons.buttons[1].postChainAction == LabelDoNothing))
    data->buttons.buttons[1].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 4)) && (data->buttons.buttons[2].postChainAction == LabelDoNothing))
    data->buttons.buttons[2].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 8)) && (data->buttons.buttons[3].postChainAction == LabelDoNothing))
    data->buttons.buttons[3].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 16)) && (data->buttons.buttons[4].postChainAction == LabelDoNothing))
    data->buttons.buttons[4].postChainAction = LabelBackupOneStep;
}

void addQButtonEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_BUTTON_DATA)));
  QUESTION_BUTTON_DATA *data = (QUESTION_BUTTON_DATA*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
  
  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

  data->buttons.numListButtons = 5;
  data->buttons.buttons[0].present = TRUE;
  data->buttons.buttons[1].present = TRUE;
  data->buttons.buttons[2].present = TRUE;
  data->buttons.buttons[3].present = TRUE;
  data->buttons.buttons[4].present = TRUE;

  data->buttons.buttons[0].postChainAction = 
      (HasMask(temp, 4)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[1].postChainAction = 
      (HasMask(temp, 8)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[2].postChainAction = 
      (HasMask(temp, 16)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[3].postChainAction = 
      (HasMask(temp, 32)) ? LabelReturnToQuestion : LabelDoNothing;
  data->buttons.buttons[4].postChainAction = 
      (HasMask(temp, 64)) ? LabelReturnToQuestion : LabelDoNothing;

  int L;
  for (L=0;L<5;L++)
    data->buttons.buttons[L].label="";

  CString buffer;
  uadata->StringData.GetStringAt(GetWord(9, pData), buffer);

  if (!buffer.IsEmpty())
  {
    // parse button labels
    // each lable is preceeded by a ^, as in
    // ^button1 text^button2 text^^^
    // empty buttons have no text after ^

    char lab[50];
    char *start = (char*)strchr(buffer, '^');
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
        data->buttons.buttons[0].label = lab;
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
        data->buttons.buttons[1].label = lab;
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
        data->buttons.buttons[2].label = lab;
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
        data->buttons.buttons[3].label = lab;
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
        data->buttons.buttons[4].label = lab;
      }
    }
    else end = start;
  }

  temp = EventByte(16,pData);

  // is 'backup' combinable with 'return to question'?
  // answer: yes, both options can be enabled in UA, DC doesn't support it
  if ((HasMask(temp, 1)) && (data->buttons.buttons[0].postChainAction == LabelDoNothing))
    data->buttons.buttons[0].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 2)) && (data->buttons.buttons[1].postChainAction == LabelDoNothing))
    data->buttons.buttons[1].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 4)) && (data->buttons.buttons[2].postChainAction == LabelDoNothing))
    data->buttons.buttons[2].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 8)) && (data->buttons.buttons[3].postChainAction == LabelDoNothing))
    data->buttons.buttons[3].postChainAction = LabelBackupOneStep;
  if ((HasMask(temp, 16)) && (data->buttons.buttons[4].postChainAction == LabelDoNothing))
    data->buttons.buttons[4].postChainAction = LabelBackupOneStep;

  for (L=0;L<5;L++)
    if (data->buttons.buttons[L].label.IsEmpty())
      data->buttons.buttons[L].present=FALSE;
}

void addQYesNoEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_YES_NO)));
  QUESTION_YES_NO *data = (QUESTION_YES_NO*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);

  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));
  temp &= 127;

  if (temp == 0)
  {
    data->YesChainAction = LabelDoNothing;
    data->NoChainAction = LabelDoNothing;
  }
  else
  {
    if (HasMask(temp, 4))
      data->YesChainAction = LabelReturnToQuestion;
    else if (HasMask(temp, 32))
      data->YesChainAction = LabelBackupOneStep;

    if (HasMask(temp, 8))
      data->NoChainAction = LabelReturnToQuestion;
    else if (HasMask(temp, 16))
      data->NoChainAction = LabelBackupOneStep;
  }

  uadata->StringData.GetStringAt(GetWord(11, pData), data->text2);
  uadata->StringData.GetStringAt(GetWord(13, pData), data->text3);
}

void addRemoveNPCEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(REMOVE_NPC_DATA)));
   REMOVE_NPC_DATA *data = (REMOVE_NPC_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;
   if (HasMask(temp, 1))
     data->distance = Nearby;
   else if (HasMask(temp, 2))
     data->distance = FarAway;
   else
     data->distance = UpClose;

/*   
   data->charRemoved = GetMonsterKey(EventByte(9,pData));
*/ NotImplemented(0x91408, false);


}

void addShopEvent(GameEvent *event, BYTE *pData)
{
   SHOP *data = (SHOP*)(event);
   
   data->costFactor = ConvertCostModifier(EventByte(6,pData));
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

   data->forceExit = HasMask(temp, 4);
   
   BYTE b1 = EventByte(9,pData);
   BYTE b2 = EventByte(10,pData);
   BYTE b3 = EventByte(11,pData);
   AssignShopItemBytes(data, b1,b2,b3);

   b1 = EventByte(12,pData);
   b2 = EventByte(13,pData);
   b3 = EventByte(14,pData);
   AssignShopItemBytes(data, b1,b2,b3);

   b1 = EventByte(15,pData);
   b2 = EventByte(16,pData);
   b3 = EventByte(17,pData);
   AssignShopItemBytes(data, b1,b2,b3);

   b1 = EventByte(18,pData);
   b2 = EventByte(19,pData);
   b3 = EventByte(20,pData);
   AssignShopItemBytes(data, b1,b2,b3);

#ifdef _DEBUG
  POSITION pos = data->itemsAvail.GetHeadPosition();
  while (pos != NULL)
  {    
    TRACE("name %s, qty %i\n", 
      //itemData.GetItemUniqueName(data->itemsAvail.PeekAtPos(pos).m_giID), 
      itemData.GetItemUniqueName(data->itemsAvail.PeekAtPos(pos).itemID), 
      data->itemsAvail.PeekAtPos(pos).qty);
    data->itemsAvail.GetNext(pos);
  }
#endif

}


void addTavernTalesEvent(GameEvent *event, BYTE *pData)
{
  TAVERN_TALES *data = (TAVERN_TALES*)(event);
  //uadata->StringData.GetStringAt(GetWord( 9, pData), data->tales[0].tale);
  //uadata->StringData.GetStringAt(GetWord(11, pData), data->tales[1].tale);
  //uadata->StringData.GetStringAt(GetWord(13, pData), data->tales[2].tale);
  //uadata->StringData.GetStringAt(GetWord(15, pData), data->tales[3].tale);
  //uadata->StringData.GetStringAt(GetWord(17, pData), data->tales[4].tale);
  //uadata->StringData.GetStringAt(GetWord(19, pData), data->tales[5].tale);
  tavernTales = *data;
  talesPresent = true;
}

void addTavernEvent(GameEvent *event, BYTE *pData)
{
   TAVERN *data = (TAVERN*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);

  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

  data->allowFights = HasMask(temp, 16);
  data->allowDrinks = HasMask(temp, 32);
  data->taleOrder = HasMask(temp, 8) ? RandomTales : InOrderTales;
    
  uadata->StringData.GetStringAt(GetWord(9, pData), data->tales[0].tale);
  uadata->StringData.GetStringAt(GetWord(11, pData), data->tales[1].tale);
  uadata->StringData.GetStringAt(GetWord(13, pData), data->tales[2].tale);
  uadata->StringData.GetStringAt(GetWord(15, pData), data->tales[3].tale);

  for (int i=0;i<MAX_DRINKS;i++)
  {
    data->drinks[i].points = 10;
    data->drinks[i].name = "";
  }

  CString buffer;
  uadata->StringData.GetStringAt(GetWord(17, pData), buffer);

  if (!buffer.IsEmpty())
  {
    // parse drink names
    // each drink is preceeded by a ^, as in
    // ^drink1 text^drink2 text^^^
    // empty drinks have no text after ^

    char lab[50];
    char *start = (char*)strchr(buffer, '^');
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
        data->drinks[0].name = lab;
        data->drinks[0].points = 0;
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
        data->drinks[1].name = lab;
        data->drinks[1].points = 6;
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
        data->drinks[2].name = lab;
        data->drinks[2].points = 10;
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
        data->drinks[3].name = lab;
        data->drinks[3].points = 15;
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
        data->drinks[4].name = lab;
        data->drinks[4].points = 30;
      }
    }
    else end = start;
  }

  data->drinkPointTrigger = 60;

/* Not implemeted
  if (talesPresent)
  {
    int FirstEmptyTale = 0;
    if (((TAVERN*)data)->tales[0].tale.IsEmpty())
      FirstEmptyTale = 0;
    else if (((TAVERN*)data)->tales[1].tale.IsEmpty())
      FirstEmptyTale = 1;
    else if (((TAVERN*)data)->tales[2].tale.IsEmpty())
      FirstEmptyTale = 2;
    else if (((TAVERN*)data)->tales[3].tale.IsEmpty())
      FirstEmptyTale = 3;
    else
      FirstEmptyTale = 4;
    for (int i=0;i<6;i++)
    {
      if (!tavernTales.tales[i].tale.IsEmpty())
      {
        ((TAVERN*)data)->tales[FirstEmptyTale].tale = tavernTales.tales[i].tale;
        FirstEmptyTale++;
      }
    }
    data->x = tavernTales.x;
    data->y = tavernTales.y;
    talesPresent=false;
  }      
  */
}

void addTempleEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TEMPLE)));
   TEMPLE *data = (TEMPLE*)(event);
   //data->templeSpells.spellLimits.maxSpellLevel = EventByte(5,pData);
   data->costFactor = ConvertCostModifier(EventByte(6,pData));
   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

   data->forceExit = HasMask(temp, 4);
   data->allowDonations = HasMask(temp, 8);
   
   data->donationTrigger = GetDWord(9,pData);

   uadata->StringData.GetStringAt(GetWord(14, pData), data->text);
   uadata->StringData.GetStringAt(GetWord(16, pData), data->text2);
}

void addTextEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TEXT_EVENT_DATA)));
   TEXT_EVENT_DATA *data = (TEXT_EVENT_DATA*)(event);

   BYTE temp = EventByte(5, pData);
   data->ForceBackup =  HasMask(temp, 32);
   data->WaitForReturn = 
     HasMask(temp,1)||HasMask(temp,2)||HasMask(temp,4)||HasMask(temp,8)||HasMask(temp,16);

   temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

   data->HighlightText = FALSE;

   // no place to store pic distance

   CString buffer;
   CString result;
   buffer="";
   result = "";

   if (HasMask(temp, 4))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(9, pData), buffer);
   result += buffer;
   if (HasMask(temp, 4))
     result += "/h";

   data->text = result;   
   result = "";

   if (HasMask(temp, 8))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(11, pData), buffer);
   result += buffer;
   if (HasMask(temp, 8))
     result += "/h";

   data->text += result;   
   result = "";

   if (HasMask(temp, 16))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(13, pData), buffer);
   result += buffer;
   if (HasMask(temp, 16))
     result += "/h";

   data->text += result;   
   result = "";

   if (HasMask(temp, 32))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(15, pData), buffer);
   result += buffer;
   if (HasMask(temp, 32))
     result += "/h";

   data->text += result;   
   result = "";

   if (HasMask(temp, 64))
     result += "/h";
   uadata->StringData.GetStringAt(GetWord(17, pData), buffer);
   result += buffer;
   if (HasMask(temp, 64))
     result += "/h";

   data->text += result;
   
   AssignSound(EventByte(19, pData), data->sound);
}

void addTransferEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
   TRANSFER_EVENT_DATA *data = (TRANSFER_EVENT_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);

  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

   data->activateBeforeEntry = ((temp & 16) == 0);

   data->askYesNo = HasMask(temp, 32);
   data->transferOnYes = ((temp & 64) == 0);

   if (HasMask(temp, 12))
     data->transferData.m_facing = FaceWest;
   else if (HasMask(temp, 4))
     data->transferData.m_facing = FaceEast;
   else if (HasMask(temp, 8))
     data->transferData.m_facing = FaceSouth;
   else
     data->transferData.m_facing = FaceNorth;

   data->transferData.destY = EventByte(9,pData);
   data->transferData.destX = EventByte(10,pData);
   
   uadata->StringData.GetStringAt(GetWord(11, pData), data->text2);

   data->transferData.execEvent = HasMask(EventByte(13,pData), 4);
   data->destroyDrow = HasMask(EventByte(13,pData), 8);

   data->transferData.destLevel = (EventByte(15,pData)-1);
   ASSERT(data->transferData.destLevel >= 0);
   if (data->transferData.destLevel < 0) data->transferData.destLevel = 0;

   if (HasMask(EventByte(13,pData), 1))
   {
     // use entry point
     data->transferData.destEP = EventByte(14,pData)-1;
     if (data->transferData.destEP == 0)
       data->transferData.destEP = -1;
   }
   else // use x,y
     data->transferData.destEP = -1;
}

void addTeleporterEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
  TRANSFER_EVENT_DATA *data = (TRANSFER_EVENT_DATA*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);

  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

   data->askYesNo = HasMask(temp, 32);
   data->transferOnYes = ((temp & 64) == 0);

   if (HasMask(temp, 12))
     data->transferData.m_facing = FaceWest;
   else if (HasMask(temp, 4))
     data->transferData.m_facing = FaceEast;
   else if (HasMask(temp, 8))
     data->transferData.m_facing = FaceSouth;
   else
     data->transferData.m_facing = FaceNorth;

   data->transferData.destY = EventByte(9,pData);
   data->transferData.destX = EventByte(10,pData);
   
   uadata->StringData.GetStringAt(GetWord(11, pData), data->text2);

   data->transferData.execEvent = HasMask(EventByte(13,pData), 4);
   data->transferData.destLevel = CurrentLevel;

   if (HasMask(EventByte(13,pData), 1))
   {
     // use entry point
     data->transferData.destEP = EventByte(14,pData)-1;
     if (data->transferData.destEP == 0)
       data->transferData.destEP = -1;
   }
   else // use x,y
     data->transferData.destEP = -1;
}

void addStairsEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
   TRANSFER_EVENT_DATA *data = (TRANSFER_EVENT_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);

  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

   data->askYesNo = HasMask(temp, 32);
   data->transferOnYes = ((temp & 64) == 0);

   if (HasMask(temp, 12))
     data->transferData.m_facing = FaceWest;
   else if (HasMask(temp, 4))
     data->transferData.m_facing = FaceEast;
   else if (HasMask(temp, 8))
     data->transferData.m_facing = FaceSouth;
   else
     data->transferData.m_facing = FaceNorth;

   data->transferData.destY = EventByte(9,pData);
   data->transferData.destX = EventByte(10,pData);
   
   uadata->StringData.GetStringAt(GetWord(11, pData), data->text2);

   data->transferData.execEvent = HasMask(EventByte(13,pData), 4);
   data->transferData.destLevel = CurrentLevel;
   if (HasMask(EventByte(13,pData), 1))
   {
     // use entry point
     data->transferData.destEP = EventByte(14,pData)-1;
     if (data->transferData.destEP == 0)
       data->transferData.destEP = -1;
   }
   else // use x,y
     data->transferData.destEP = -1;
}

void addWhoPaysEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_PAYS_EVENT_DATA)));
  WHO_PAYS_EVENT_DATA *data = (WHO_PAYS_EVENT_DATA*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

  if (temp == 0)
    data->impossible = TRUE;
  else if (HasMask(temp, 12))
    data->platinum = GetWord(9,pData);
  else if (HasMask(temp, 8))
    data->jewels = GetWord(9,pData);
  else if (HasMask(temp, 4))
    data->gems = GetWord(9,pData);

   temp = EventByte(11, pData);
   if (HasMask(temp, 12))
     data->successAction = BackupOneStep;
   else if (HasMask(temp, 8))
     data->successAction = Teleport;
   else if (HasMask(temp, 4))
     data->successAction = ChainEvent;

   if (HasMask(temp, 3))
     data->failAction = BackupOneStep;
   else if (HasMask(temp,2))
     data->failAction = Teleport;
   else if (HasMask(temp, 1))
     data->failAction = ChainEvent;

   data->successTransferData.execEvent = HasMask(temp, 64);
   data->successTransferData.destEP = -1;
   data->failTransferData.execEvent = HasMask(temp, 64);
   data->failTransferData.destEP = -1;

   if (HasMask(temp, 48))
     data->successTransferData.m_facing = FaceWest;
   else if (HasMask(temp, 32))
     data->successTransferData.m_facing = FaceSouth;
   else if (HasMask(temp, 16))
     data->successTransferData.m_facing = FaceEast;
   else
     data->successTransferData.m_facing = FaceNorth;

   uadata->StringData.GetStringAt(GetWord(12, pData), data->text3);
   uadata->StringData.GetStringAt(GetWord(14, pData), data->text2);

   data->successTransferData.destX = EventByte(18, pData);
   data->successTransferData.destY = EventByte(19, pData);
   data->successTransferData.destLevel = CurrentLevel;
   data->failTransferData.destY = data->successTransferData.destY;
   data->failTransferData.destX = data->successTransferData.destX;
   data->failTransferData.destLevel = CurrentLevel;
}

void addWhoTriesEvent(GameEvent *event, BYTE *pData)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_TRIES_EVENT_DATA)));
   WHO_TRIES_EVENT_DATA *data = (WHO_TRIES_EVENT_DATA*)(event);

   uadata->StringData.GetStringAt(GetWord(5, pData), data->text);

   BYTE temp = EventByte(8,pData);
   AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
   temp &= 127;

   if (temp == 0)
     data->alwaysSucceeds = TRUE;
   else if (HasMask(temp, 60))
     data->checkRL = TRUE;
   else if (HasMask(temp, 56))
     data->checkCW = TRUE;
   else if (HasMask(temp, 52))
     data->checkHN = TRUE;
   else if (HasMask(temp, 48))
     data->checkHS = TRUE;
   else if (HasMask(temp, 44))
     data->checkMS = TRUE;
   else if (HasMask(temp, 40))
     data->checkFT = TRUE;
   else if (HasMask(temp, 36))
     data->checkOL = TRUE;
   else if (HasMask(temp, 32))
     data->checkPP = TRUE;
   else if (HasMask(temp, 28))
     data->checkCHA = TRUE;
   else if (HasMask(temp, 24))
     data->checkCON = TRUE;
   else if (HasMask(temp, 20))
     data->checkDEX = TRUE;
   else if (HasMask(temp, 16))
     data->checkWIS = TRUE;
   else if (HasMask(temp, 12))
     data->checkINT = TRUE;
   else if (HasMask(temp, 8))
     data->checkSTR = TRUE;
   else if (HasMask(temp, 4))
     data->alwaysFails = TRUE;

   data->compareToDie = !((temp & 64) == 64);
   data->compareDie = EventByte(9,pData);
   data->NbrTries = EventByte(10,pData);

   temp = EventByte(11, pData);
   if (HasMask(temp, 12))
     data->successAction = BackupOneStep;
   else if (HasMask(temp, 8))
     data->successAction = Teleport;
   else if (HasMask(temp, 4))
     data->successAction = ChainEvent;

   if (HasMask(temp, 3))
     data->failAction = BackupOneStep;
   else if (HasMask(temp, 2))
     data->failAction = Teleport;
   else if (HasMask(temp, 1))
     data->failAction = ChainEvent;

   data->successTransferData.execEvent = HasMask(temp, 64);
   data->successTransferData.destEP = -1;
   data->failTransferData.execEvent = HasMask(temp, 64);
   data->failTransferData.destEP = -1;

   if (HasMask(temp, 48))
     data->successTransferData.m_facing = FaceWest;
   else if (HasMask(temp,32))
     data->successTransferData.m_facing = FaceSouth;
   else if (HasMask(temp, 16))
     data->successTransferData.m_facing = FaceEast;
   else
    data->successTransferData.m_facing = FaceNorth;

   uadata->StringData.GetStringAt(GetWord(12, pData), data->text3);
   uadata->StringData.GetStringAt(GetWord(14, pData), data->text2);

   data->successTransferData.destX = EventByte(18, pData);
   data->successTransferData.destY = EventByte(19, pData);
   data->successTransferData.destLevel = CurrentLevel;
   data->failTransferData.destY = data->successTransferData.destY;
   data->failTransferData.destX = data->successTransferData.destX;
   data->failTransferData.destLevel = CurrentLevel;
   data->strBonus = EventByte(20, pData);
}

void addSpecialItemEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(SPECIAL_ITEM_KEY_EVENT_DATA)));
  SPECIAL_ITEM_KEY_EVENT_DATA *data = (SPECIAL_ITEM_KEY_EVENT_DATA*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

   SPECIAL_OBJECT_EVENT_DATA sitem; 
   if (temp == 0)
    sitem.operation = SPECIAL_OBJECT_GIVE;
   else
     sitem.operation = SPECIAL_OBJECT_TAKE;

   sitem.ItemType = GetObjectKeyType(EventByte(9,pData));
   sitem.index = GetObjectKey(EventByte(9,pData));
   data->items.Add(sitem);
}

void addPassTimeEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(PASS_TIME_EVENT_DATA)));
  PASS_TIME_EVENT_DATA *data = (PASS_TIME_EVENT_DATA*)(event);

  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
  data->Days = EventByte(9,pData);
  data->Hours = EventByte(10,pData);
  data->Minutes = EventByte(11,pData);
  data->AllowStop=FALSE;
  data->PassSilent=FALSE;
  data->SetTime=FALSE;
}

void addTrainingHallEvent(GameEvent *event, BYTE *pData)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRAININGHALL)));
  TRAININGHALL *data = (TRAININGHALL*)(event);
  
  uadata->StringData.GetStringAt(GetWord(5, pData), data->text);
  BYTE temp = EventByte(8,pData);
  AssignPic(EventByte(7,pData), data->pic, (temp & 128));   
  temp &= 127;

  data->forceExit = HasMask(temp, 4);

  temp = EventByte(9,pData);

  NotImplemented(0xea31b, false);
  //data->TrainMagicUser = HasMask(temp, 1);
  //data->TrainCleric = HasMask(temp, 2);
  //data->TrainThief = HasMask(temp,4);
  //data->TrainFighter = HasMask(temp, 8);
  //data->TrainPaladin = HasMask(temp, 16);
  //data->TrainRanger = HasMask(temp, 32);

  // FRUA gives cost modifier, not base cost
  costFactorType mod = ConvertCostModifier(EventByte(10,pData));
  data->Cost = ApplyCostFactor(mod, 1000);
}

GameEvent *UAImportEvent::AddEvent(BYTE index)
{
  eventType type = ConvertEventType();
  GameEvent *pEvent = levelData.eventData.CreateNewEvent(type);
  if (pEvent == NULL)
    return NULL;

  if (pEvent->event == NoEvent)
  {
    delete pEvent;
    return NULL;
  }

  if (uadata->MapEvents[index].root)
  {
    pEvent->x = uadata->MapEvents[index].x;
    pEvent->y = uadata->MapEvents[index].y;
  }
  else
  {
    // code not capable of chaining non-root events yet, they
    // get added to the event list but no event calls them and they
    // become orphans with no way of seeing/removing them.
    //delete pEvent;
    //return 0;

    pEvent->x = -1;
    pEvent->y = -1;
  }

  ConvertEventControl(pEvent->control);

  WriteDebugString("Importing \'%s\' event for level %u at %i,%i, chained to %i\n",
                   GetEventDescription(pEvent->event), 
                   CurrentLevel+1, 
                   pEvent->x, pEvent->y,
                   uadata->EventData[index-1].ChainEvent);

  switch (type)
  {
  case AddNpc:
    addNPCEvent(pEvent, EventData);
    break;    
  case Camp:
    addCampEvent(pEvent, EventData);
    break;    
  case Combat:
    addCombatEvent(pEvent, EventData);
    break;
  case PickOneCombat:
    {
      pEvent->event = Combat;
      addCombatEvent(pEvent, EventData);
      COMBAT_EVENT_DATA *data = (COMBAT_EVENT_DATA*)(pEvent);
      data->randomMonster = TRUE;
    }
    break;
  case CombatTreasure:
    addTreasureEvent(pEvent, EventData);
    break;    
  case Damage:
    addGiveDamageEvent(pEvent, EventData);
    break;    
  case EncounterEvent:
    addEncounterEvent(pEvent, EventData);
    break;    
  case EnterPassword:
    addPasswordEvent(pEvent, EventData);
    break;    
  case GainExperience:
    addGiveExpEvent(pEvent, EventData);
    break;    
  case GiveTreasure:
    addTreasureEvent(pEvent, EventData);
    break;    
  case GuidedTour:
    addTourEvent(pEvent, EventData);
    break;    
  case NPCSays:
    addNPCSaysEvent(pEvent, EventData);
    break;    
  case QuestionList:
    addQListEvent(pEvent, EventData);
    break;    
  case QuestionButton:
    addQButtonEvent(pEvent, EventData);
    break;    
  case PassTime:
    addPassTimeEvent(pEvent, EventData);
    break;    
  case QuestionYesNo:
    addQYesNoEvent(pEvent, EventData);
    break;    
  case RemoveNPCEvent:
    addRemoveNPCEvent(pEvent, EventData);
    break;    
  case ShopEvent:
    addShopEvent(pEvent, EventData);
    break;    
  case TempleEvent:
    addTempleEvent(pEvent, EventData);
    break;    
  case TavernEvent:
    addTavernEvent(pEvent, EventData);
    break;    
  case TavernTales:
    addTavernTalesEvent(pEvent, EventData);    
    //delete pEvent; // auto chained after next Tavern event
    //pEvent=NULL; 
    break;
  case Stairs:
    addStairsEvent(pEvent, EventData);
    break;    
  case Teleporter:
    addTeleporterEvent(pEvent, EventData);
    break;    
  case TextStatement:
    addTextEvent(pEvent, EventData);
    break;    
  case TransferModule:
    addTransferEvent(pEvent, EventData);
    break;    
  case WhoPays:
    addWhoPaysEvent(pEvent, EventData);
    break;    
  case WhoTries:
    addWhoTriesEvent(pEvent, EventData);
    break;    
  case SpecialItem:
    addSpecialItemEvent(pEvent, EventData);
    break;    
  case Vault:
    addVaultEvent(pEvent, EventData);
    break;  
  case TrainingHallEvent:
    addTrainingHallEvent(pEvent, EventData);
    break;
  case SmallTown:
    addSmallTownEvent(pEvent, EventData);
    break;
  case ChainEventType:
    {
      // no data other than event chain and triggers
      // event chain at offset 9 + normal chain
    }
    break;
  case QuestStage:
    addQuestEvent(pEvent, EventData);
    break;
  case Sounds:
    addSoundEvent(pEvent, EventData);
    break;
  case Utilities:
    addUtilitiesEvent(pEvent, EventData);
    break; 
  
  //case InnEvent:    
  default:
    break;
  }

  if (pEvent != NULL)
  {   
    // all three merged into one for DC
    if ((pEvent->event == Stairs)||(pEvent->event==Teleporter))
      pEvent->event = TransferModule;

    /*
    levelData.eventData.AddEvent(pEvent);

    if (pEvent->event == TavernEvent)
    {

    }
    return pEvent->id;
    */
  }
  //else
  //  return 0;

  return pEvent;
}

void UAImportEvent::ConvertEventControl(EVENT_CONTROL &data)
{
  data.onceOnly = EventTrigger & 0x01;
  switch (EventTrigger & 0x06)
  {
  case 2: data.chainTrigger = IfEventHappen; break;
  case 4: data.chainTrigger = IfEventNotHappen; break;
  default: data.chainTrigger = AlwaysChain; break;
  }
  
  switch (EventTrigger & 0xF8)
  {
  case 8:
    data.eventTrigger = PartyHaveItem;  
    
    switch (GetObjectKeyType(EventTriggerData))
    {
    case KEY_FLAG:
      data.specialKey = GetObjectKey(EventTriggerData);
      break;
    case ITEM_FLAG:
      data.specialItem = GetObjectKey(EventTriggerData);
      break;
    case QUEST_FLAG:
      data.quest = GetObjectKey(EventTriggerData);
      break;
    }
    break;
  case 16:
    data.eventTrigger = PartyNotHaveItem;

    switch (GetObjectKeyType(EventTriggerData))
    {
    case KEY_FLAG:
      data.specialKey = GetObjectKey(EventTriggerData);
      break;
    case ITEM_FLAG:
      data.specialItem = GetObjectKey(EventTriggerData);
      break;
    case QUEST_FLAG:
      data.quest = GetObjectKey(EventTriggerData);
      break;
    }
    break;
  case 24:
    data.eventTrigger = Daytime;
    break;
  case 32:
    data.eventTrigger = Nighttime;
    break;
  case 40:
    data.eventTrigger = RandomChance;
    data.chance = EventTriggerData;
    break;
  case 48:
    data.eventTrigger = PartySearching;
    break;
  case 56:
    data.eventTrigger = PartyNotSearching;
    break;
  case 64:
    data.eventTrigger = FacingDirection;
    // N=1, E=2, S=4, W=8
    switch (EventTriggerData)
    {
    case 1: data.facing = North; break;
    case 2: data.facing = East;  break;
    case 3: data.facing = N_E;   break;
    case 4: data.facing = South; break;
    case 5: data.facing = N_S;   break;
    case 6: data.facing = S_E;   break;
    case 7: data.facing = N_S_E; break;
    case 8: data.facing = West;  break;    
    case 9: data.facing = N_W;   break;    
    case 10: data.facing = E_W;   break;
    case 11: data.facing = N_W_E; break;
    case 12: data.facing = S_W;  break;    
    case 13: data.facing = N_S_W; break;
    case 14: data.facing = W_S_E; break;
    case 15: data.facing = Any;   break;        
    }
    break;
  case 72:
    data.eventTrigger = QuestComplete;
    switch (GetObjectKeyType(EventTriggerData))
    {
    case KEY_FLAG:
      data.specialKey = GetObjectKey(EventTriggerData);
      break;
    case ITEM_FLAG:
      data.specialItem = GetObjectKey(EventTriggerData);
      break;
    case QUEST_FLAG:
      data.quest = GetObjectKey(EventTriggerData);
      break;
    }
    break;
  case 80:
    data.eventTrigger = QuestFailed;
    switch (GetObjectKeyType(EventTriggerData))
    {
    case KEY_FLAG:
      data.specialKey = GetObjectKey(EventTriggerData);
      ASSERT(FALSE);
      break;
    case ITEM_FLAG:
      data.specialItem = GetObjectKey(EventTriggerData);
      ASSERT(FALSE);
      break;
    case QUEST_FLAG:
      data.quest = GetObjectKey(EventTriggerData);
      break;
    }
    break;
  case 88:
    data.eventTrigger = QuestInProgress;
    switch (GetObjectKeyType(EventTriggerData))
    {
    case KEY_FLAG:
      data.specialKey = GetObjectKey(EventTriggerData);
      ASSERT(FALSE);
      break;
    case ITEM_FLAG:
      data.specialItem = GetObjectKey(EventTriggerData);
      ASSERT(FALSE);
      break;
    case QUEST_FLAG:
      data.quest = GetObjectKey(EventTriggerData);
      break;
    }
    break;
  case 96:
    data.eventTrigger = PartyDetectingTraps;
    break;
  case 104:
    data.eventTrigger = PartyNotDetectingTraps;
    break;
  case 112:
    data.eventTrigger = PartySeeInvisible;
    break;
  case 120:
    data.eventTrigger = PartyNotSeeInvisible;
    break;
  case 128:
    data.eventTrigger = ClassInParty;
    {
      CLASS_ID classID;
      switch (EventTriggerData)
      {
        //case 0: data.charClass=Cleric; break;
        //case 2: data.charClass=Fighter; break;
        //case 3: data.charClass=Paladin; break;
        //case 4: data.charClass=Ranger;  break;
        //case 5: data.charClass=MagicUser; break;
        //case 6: data.charClass=Thief; break;
        case 0: classID = "Cleric"; break;
        case 2: classID = "Fighter"; break;
        case 3: classID = "Paladin"; break;
        case 4: classID = "Ranger";  break;
        case 5: classID = "Magic User"; break;
        case 6: classID = "Thief"; break;
      }
      data.classBaseclassID.classID = classID;
    };
    break;
  case 136:
    data.eventTrigger = RaceInParty;
    {
      RACE_ID raceID;
      switch (EventTriggerData)
      {
      case 0: raceID = "Elf";   break;
      case 1: raceID = "HalfElf"; break;
      case 2: raceID = "Dwarf";   break;
      case 3: raceID = "Gnome";   break;
      case 4: raceID = "Halfling"; break;
      case 5: raceID = "Human";    break;
      };
      data.raceID = raceID;
    };
    break;
  }
}

eventType UAImportEvent::ConvertEventType()
{
  eventType type = NoEvent;
  switch (EventType)
  {
  case 1: type = Combat;    break;
  case 2: type = TextStatement;  break;
  case 3: type = GiveTreasure;   break;
  case 4: type = Damage;      break;
  case 5: type = Stairs;      break;
  case 6: type = TrainingHallEvent;  break;
  case 7: type = TavernEvent;      break;
  case 8: type = ShopEvent;      break;
  case 9: type = TempleEvent;      break;
  case 10: type = QuestionButton;      break;
  case 11: type = TransferModule;      break;
  case 12: type = GuidedTour;      break;
  case 13: type = AddNpc;      break;
  case 14: type = NPCSays;      break;
  case 15: type = EncounterEvent;      break;
  case 16: type = Utilities;      break;
  case 17: type = Sounds;      break;
  case 18: type = WhoTries;      break;
  case 19: type = WhoPays;      break;
  case 20: type = EnterPassword;      break;
  case 21: type = QuestionList;      break;
  case 22: type = SmallTown;      break;
  case 23: type = ChainEventType;     break;
  case 24: type = Vault;      break;
  case 25: type = CombatTreasure;     break;
  case 26: type = GainExperience;     break;
  case 27: type = PassTime;      break;
  case 29: type = Camp;     break;
  case 32: type = RemoveNPCEvent;      break;
  case 33: type = PickOneCombat;      break;
  case 34: type = Teleporter;      break;
  case 35: type = QuestStage;      break;
  case 36: type = QuestionYesNo;      break;
  case 37: type = TavernTales;      break;
  case 38: type = SpecialItem;      break;
  default: type = NoEvent;     break;
  }
  return type;
}

void ClearItemBuffers(void)
{
  WriteDebugString("Clearing item buffers\n");
  //memset(giID_Keys, 0, sizeof(giID_Keys));
  //memset(itemID_Keys, 0, sizeof(itemID_Keys));
  for (int i=0; i<MAX_IMPORT_ITEMS; i++)
    itemID_Keys[i] = ITEM_ID();
}

void ClearMonsterBuffers(void)
{
  ImportMonsterCount=0;
  ImportNPCCount=0;
  WriteDebugString("Clearing monster buffers\n");
  //memset(Monster_Keys, 0, sizeof(Monster_Keys));
  for (int i=0; i<MAX_IMPORT_MONSTERS; i++)
    Monster_Keys[i] = MONSTER_ID();
  memset(Monster_Type, 0, sizeof(Monster_Type));
}

void ClearImportBuffers(void)
{
  WriteDebugString("Clearing import buffers\n");
  CurrentLevel = 0;
  StartEP = 0;  
  //WallsetsUsed.RemoveAll();
  //BackdropsUsed.RemoveAll();
  memset(SpecialKey_Keys, 0, sizeof(SpecialKey_Keys));
  memset(SpecialItem_Keys, 0, sizeof(SpecialItem_Keys));
  memset(Quest_Keys, 0, sizeof(Quest_Keys));
  memset(Sound_Keys, 0, sizeof(Sound_Keys));
  memset(Event_Keys, 0, sizeof(Event_Keys));
}

#pragma warning( disable : 4706 ) // assignment within conditional expression

BOOL ImportGameDat(const char *path)
{
  BOOL success = TRUE;
  CString fullpath;
  fullpath.Format("%s%s", path, "game001.dat");

  StartEP = 0; 

  int i;
  char TextBuffer[33];
  DWORD FourBytes;
  BYTE  OneByte;
  SPECIAL_OBJECT_DATA soData;

  PumpWinMsgs();

  CString deffile;

#ifdef SIMPLE_STRUCTURE
  deffile = ede.TemplateWallArtDir() + "wa_Wall1.png"; // fallback image
#else
  deffile.Format("%swa_Wall1.png", EditorArt); // fallback image
#endif

  for (i=1;i<=MAX_IMPORT_WALLS;i++)  
  {
    //ImportWallSlotNames[i].Format("%sWall%i.pcx", EditorArt, i);
    //if (!FileExists(ImportWallSlotNames[i]))
      ImportWallSlotNames[i] = deffile;
  }

#ifdef SIMPLE_STRUCTURE
  deffile = ede.TemplateDoorArtDir() + "dr_Door5.png"; // fallback image
#else
  deffile.Format("%sdr_Door5.png", EditorArt); // fallback image
#endif

  for (i=1;i<=MAX_IMPORT_WALLS;i++)   
  {
    //ImportDoorSlotNames[i].Format("%sDoor%i.pcx", EditorArt, i);
    //if (!FileExists(ImportDoorSlotNames[i]))
      ImportDoorSlotNames[i] = deffile;
  }

#ifdef SIMPLE_STRUCTURE
  deffile = ede.TemplateBackgroundArtDir() + "bd_Background1.png"; // fallback image
#else
  deffile.Format("%sbd_Background1.png", EditorArt); // fallback image
#endif

  for (i=1;i<=MAX_IMPORT_BACKDROPS;i++)   
  {
#ifdef SIMPLE_STRUCTURE
    ImportBackdropNames[i].Format("%sbd_Background%i.png", ede.TemplateBackgroundArtDir(), i);
#else
    ImportBackdropNames[i].Format("%sbd_Background%i.png", EditorArt, i);
#endif
    if (!FileExists(ImportBackdropNames[i]))
      ImportBackdropNames[i] = deffile;
  }

  PumpWinMsgs();

  for (i=0;i<=MAX_IMPORT_PICS;i++)   
#ifdef SIMPLE_STRUCTURE
    ImportPicSlotNames[i].Format("%sprt_SPic1.png", ede.TemplateSmallPicDir());
#else
    ImportPicSlotNames[i].Format("%sprt_SPic1.png", EditorArt);
#endif

  for (i=0;i<=MAX_IMPORT_SOUNDS;i++)   
    ImportSoundSlotNames[i] = ""; //.Format("%sUAImportSound%i.wav", EditorArt,i);

  PumpWinMsgs();

  //WallsetsUsed.RemoveAll();
  //BackdropsUsed.RemoveAll();

  EditorStatusMsg("Importing game001.dat...");
  CFile file;

  PumpWinMsgs();

  try
  {
    if (file.Open(fullpath, CFile::modeRead))
    {
      if ((success) && (success = (file.Read(TextBuffer, 32) == 32)))
      {
        TextBuffer[32] = '\0';
        if (strlen(TextBuffer) == 0)
          globalData.designName = "NoName FRUA Design";
        else
          globalData.designName = TextBuffer;
        globalData.designName.TrimLeft();
        globalData.designName.TrimRight();
        WriteDebugString("Importing global data for design \'%s\'\n", globalData.designName);
      }
      if ((success) && (success = (file.Read(&FourBytes, 4) == 4)))
      {
        globalData.startExp = FourBytes;
        globalData.startExpType = START_EXP_VALUE;
      }
      if ((success) && (success = (file.Read(&FourBytes, 4) == 4)))
        globalData.startPlatinum = FourBytes;
      if ((success) && (success = (file.Read(&FourBytes, 4) == 4)))
        globalData.startGem = FourBytes;
      if ((success) && (success = (file.Read(&FourBytes, 4) == 4)))
        globalData.startJewelry = FourBytes;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        globalData.startLevel = OneByte-1;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        StartEP = OneByte-1; // make zero based

      globalData.startEquip.Clear();
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
      {
        startEquipType startEquip = (startEquipType)OneByte;
        AssignStartEquipItems(Fighter,   startEquip, globalData.FighterEquip);
        AssignStartEquipItems(Ranger,    startEquip, globalData.RangerEquip);
        AssignStartEquipItems(Paladin,   startEquip, globalData.PaladinEquip);
        AssignStartEquipItems(Cleric,    startEquip, globalData.ClericEquip);
        AssignStartEquipItems(Druid,     startEquip, globalData.DruidEquip);
        AssignStartEquipItems(Thief,     startEquip, globalData.ThiefEquip);
        AssignStartEquipItems(MagicUser, startEquip, globalData.MagicUserEquip);        
      }

      PumpWinMsgs();

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        success = TRUE; // unused byte, assignment quiets compiler

      // special key names 1-8
      for (i=0; (i<8) && success;i++)
      {
        if ((success) && (success = (file.Read(TextBuffer, 16) == 16)))
        {
          if (strlen(TextBuffer) == 0) sprintf(TextBuffer, "Key %i", i+1);
          soData.name = TextBuffer;          
          SpecialKey_Keys[i] = globalData.keyData.AddItem(soData);
          TRACE("Special Key %i \'%s\', %i\n", i+1, soData.name, SpecialKey_Keys[i]);
        }
      }

      PumpWinMsgs();

      // special item names 1-12
      for (i=0; (i<12) && success;i++)
      {
        if ((success) && (success = (file.Read(TextBuffer, 16) == 16)))
        {
          if (strlen(TextBuffer) == 0) sprintf(TextBuffer, "Item %i", i+1);
          soData.name = TextBuffer;
          SpecialItem_Keys[i] = globalData.specialItemData.AddItem(soData);
          TRACE("Special Item %i \'%s\', %i\n", i+1, soData.name, SpecialItem_Keys[i]);
        }
      }

      PumpWinMsgs();

      // ignore design password

      // quests aren't stored in file, add all 44 manually
      QUEST_DATA qData;
      for (i=0;i<44;i++)
      {
        qData.name.Format("Quest %i", i+1);          
        Quest_Keys[i] = globalData.questData.AddQuest(qData);
      }

      PumpWinMsgs();
    }
    else
    {
      WriteDebugString("Failed to open %s\n", fullpath);
      success = FALSE;
    }
  }
  catch (...)
  {
    WriteDebugString("Failed to import %s\n", fullpath);
    success = FALSE;
  }

  if (file.m_hFile != CFile::hFileNull)
    file.Close();

  PumpWinMsgs();

#ifdef SIMPLE_STRUCTURE
  rte.DesignDir(rte.RootDir() +  globalData.designName + ".dsn\\");
#else
  sprintf(m_designFolder, "%s%s.dsn\\", m_installDir, globalData.designName);
#endif
  CreateDesignDirectory();

  if (success)
  {
    EditorStatusMsg("Importing FRUA geoxxx.dat files...");
    success = ImportAllGeoDatFiles(path);
  }

  PumpWinMsgs();

  /*
  if (success)
  {
    if (globalData.levelInfo.stats[globalData.startLevel].overland)
    {
      int sLevel = -1;
      i=0;
      while ((i < MAX_LEVELS) && (sLevel < 0))
      {
        if ((!globalData.levelInfo.stats[i].overland)&&(globalData.levelInfo.stats[i].used))
          sLevel = i;
        i++;
      }

      globalData.startLevel = sLevel;
      globalData.startX = globalData.levelInfo.stats[sLevel].entryPoints[StartEP].x;
      globalData.startY = globalData.levelInfo.stats[sLevel].entryPoints[StartEP].y;
      globalData.startFacing = globalData.levelInfo.stats[sLevel].entryPoints[StartEP].facing;
    }
  }
  */

  PumpWinMsgs();

#ifdef SIMPLE_STRUCTURE
  rte.DesignDir(rte.RootDir() + globalData.designName + ".dsn\\");
#else
  sprintf(m_designFolder, "%s%s.dsn\\", m_installDir, globalData.designName);
#endif

  if (success)
  {
    globalData.currLevel = CurrentLevel;
    success = saveDesign();
  }

  return success;
}

BOOL ImportAllGeoDatFiles(const char *path)
{
  BOOL success = TRUE;
  CString fullpath;
  int avail=0;
  int import=0;

  uadata = new ImportLevelData;

  for (int i=1; (i<41) && success; i++)
  {    
    fullpath.Format("%sgeo%03i.dat", path, i);
    if (FileExists(fullpath))
    {
      avail++;
      success = ImportGeoDatFile(i-1, fullpath);
      PumpWinMsgs();
      if (success)
        import++;
    }
  }

  if (uadata != NULL)
    delete uadata;

  WriteDebugString("Imported %u of %u geoxxx.dat files\n",
                   import, avail);

  return success;
}

CMap<DWORD, DWORD, DWORD, DWORD> UsedEvents;

DWORD AddEventChain(DWORD index)
{
  if (index > MAX_IMPORT_EVENTS) return 0;
  
  DWORD tempkey;
  if (UsedEvents.Lookup(index, tempkey)) return 0;

  TRACE("SETTING USED EVENT %u\n", index);
  UsedEvents.SetAt(index, index);

  eventType type = uadata->EventData[index].ConvertEventType();
  if (type == NoEvent) return 0;

  GameEvent *pEvent = uadata->EventData[index].AddEvent(index+1);  
  if (pEvent==NULL) return 0;

  if (pEvent->event == TavernTales)
  {
    delete pEvent;
    return (AddEventChain(uadata->EventData[index].ChainEvent-1));
  }

  levelData.eventData.AddEvent(pEvent);

  // normal chain    
  DWORD chainkey = AddEventChain(uadata->EventData[index].ChainEvent-1);
  if (chainkey > 0)
    pEvent->ChainToEventId(0, chainkey);

  // additional chains for each event type
  switch (pEvent->GetEventType())
  {
  case TavernEvent:
    {
      TAVERN *data = (TAVERN*)(pEvent);
      chainkey = AddEventChain(EventByte(19, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(1, chainkey); // fight
      chainkey = AddEventChain(EventByte(20, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(2, chainkey); // drunk
    }
    break;
  case TempleEvent:
    {
      TEMPLE *data = (TEMPLE*)(pEvent);        
      chainkey = AddEventChain(EventByte(13, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(1, chainkey); // donate
    }
    break;
  case QuestStage:
    {
      QUEST_EVENT_DATA *data = (QUEST_EVENT_DATA*)(pEvent);        
      chainkey = AddEventChain(EventByte(11, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(1, chainkey); // accept
      chainkey = AddEventChain(EventByte(12, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(2, chainkey); // reject
    }
    break;
  case ChainEventType:
    {
      CHAIN_EVENT *data = (CHAIN_EVENT*)(pEvent);        
      chainkey = AddEventChain(EventByte(9, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(1, chainkey); // chain
    }
    break;
 case EncounterEvent:
   {
     ENCOUNTER_DATA *data = (ENCOUNTER_DATA*)(pEvent);        
     // UA only has three chains, combat, talk, escape
     /*
     for (int b=0;b<5;b++)
     {
        chainkey = AddEventChain(EventByte(10+b, uadata->EventData[index].EventData)-1);
        if (chainkey > 0) data->ChainToEventId(1+b, chainkey); // buttons
     }
     */
     chainkey = AddEventChain(EventByte(18, uadata->EventData[index].EventData)-1);
     if (chainkey > 0) data->ChainToEventId(MAX_BUTTONS+1, chainkey); // combat
     chainkey = AddEventChain(EventByte(19, uadata->EventData[index].EventData)-1);
     if (chainkey > 0) data->ChainToEventId(MAX_BUTTONS+2, chainkey); // talk
     chainkey = AddEventChain(EventByte(20, uadata->EventData[index].EventData)-1);
     if (chainkey > 0) data->ChainToEventId(MAX_BUTTONS+3, chainkey); // escape
   }
   break;
  case QuestionList:
    {
       QUESTION_LIST_DATA *data = (QUESTION_LIST_DATA*)(pEvent);
       for (int b=0;b<5;b++)
       {
          chainkey = AddEventChain(EventByte(11+b, uadata->EventData[index].EventData)-1);
          if (chainkey > 0) data->ChainToEventId(1+b, chainkey); // buttons
       }
    }
    break;   
  case QuestionButton:
    {
       QUESTION_BUTTON_DATA *data = (QUESTION_BUTTON_DATA*)(pEvent);        
       for (int b=0;b<5;b++)
       {
          chainkey = AddEventChain(EventByte(11+b, uadata->EventData[index].EventData)-1);
          if (chainkey > 0) data->ChainToEventId(1+b, chainkey); // buttons
       }
    }
    break;
  case QuestionYesNo:
    {
      QUESTION_YES_NO *data = (QUESTION_YES_NO*)(pEvent);        
      chainkey = AddEventChain(EventByte(9, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(1, chainkey); // yes
      chainkey = AddEventChain(EventByte(10, uadata->EventData[index].EventData)-1);
      if (chainkey > 0) data->ChainToEventId(2, chainkey); // no
    }
    break;
  case EnterPassword:
    {
       PASSWORD_DATA *data = (PASSWORD_DATA*)(pEvent);        
       chainkey = AddEventChain(EventByte(17, uadata->EventData[index].EventData)-1);
       if (chainkey > 0) data->ChainToEventId(1, chainkey); // success
       chainkey = AddEventChain(EventByte(16, uadata->EventData[index].EventData)-1);
       if (chainkey > 0) data->ChainToEventId(2, chainkey); // fail
    }
    break;
  case WhoPays:
    {
       WHO_PAYS_EVENT_DATA *data = (WHO_PAYS_EVENT_DATA*)(pEvent);        
       chainkey = AddEventChain(EventByte(17, uadata->EventData[index].EventData)-1);
       if (chainkey > 0) data->ChainToEventId(1, chainkey); // success
       chainkey = AddEventChain(EventByte(16, uadata->EventData[index].EventData)-1);
       if (chainkey > 0) data->ChainToEventId(2, chainkey); // fail
    }
    break;

  case WhoTries:
    {
       WHO_TRIES_EVENT_DATA *data = (WHO_TRIES_EVENT_DATA*)(pEvent);        
       chainkey = AddEventChain(EventByte(17, uadata->EventData[index].EventData)-1);
       if (chainkey > 0) data->ChainToEventId(1, chainkey); // success
       chainkey = AddEventChain(EventByte(16, uadata->EventData[index].EventData)-1);
       if (chainkey > 0) data->ChainToEventId(2, chainkey); // fail
    }
    break;

  case SmallTown:
    // already chained for DC
    break;

  default:
    // do nothing
    break;        
  }

  return pEvent->GetEventId();
}

BOOL ImportGeoDatFile(int level, const char *fullpath)
{
  CString StatusMsg;
  BOOL success = TRUE;
  char text[50];
  //DWORD FourBytes;
  WORD  TwoBytes;
  BYTE  OneByte;
  int i;

  if (uadata == NULL)
    return FALSE;

  CurrentLevel = level;
  uadata->Clear();
  levelData.Clear();

  CFile file;

  try
  {
    if (file.Open(fullpath, CFile::modeRead))
    {
      WriteDebugString("Importing level file %s\n", fullpath);
      file.Seek(26, CFile::begin);

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
      {
        globalData.levelInfo.stats[level].area_height = OneByte;
        levelData.area_height = OneByte;
      }
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
      {
        globalData.levelInfo.stats[level].area_width = OneByte;
        levelData.area_width = OneByte;
      }

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->Wall1 = OneByte;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->Wall2 = OneByte;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->Wall3 = OneByte;

      if ((uadata->Wall1 == 255)&&(uadata->Wall2 == 255)&&(uadata->Wall3 == 255))
      {
        globalData.levelInfo.stats[level].overland = TRUE;
        globalData.levelInfo.stats[level].AVStyle=OnlyAreaView;
      }

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
      {
        // set area view in all zones for this level
        for (i=0;i<MAX_ZONES;i++)
          levelData.zoneData.zones[i].allowMap = (OneByte?AreaMapping:NoMapping);
      }

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->Backdrop1 = OneByte;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->Backdrop2 = OneByte;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->Backdrop3 = OneByte;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->Backdrop4 = OneByte;

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->DungeonCombatArt = OneByte;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->WildernessCombatArt = OneByte;

      // 8 entry points
      for (i=0; (i<8) && success ;i++)
      {
        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          globalData.levelInfo.stats[level].entryPoints[i].y = OneByte;
        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          globalData.levelInfo.stats[level].entryPoints[i].x = OneByte;
        if ((success) && (success = (file.Read(&OneByte, 1) == 1))) //(0=N, 2=E, 4=S, 6=W)
        {
          switch (OneByte & 0x7)
          {
          case 0:
            globalData.levelInfo.stats[level].entryPoints[i].facing = FACE_NORTH;
            break;
          case 2:
            globalData.levelInfo.stats[level].entryPoints[i].facing = FACE_EAST;
            break;
          case 4:
            globalData.levelInfo.stats[level].entryPoints[i].facing = FACE_SOUTH;
            break;
          case 6:
            globalData.levelInfo.stats[level].entryPoints[i].facing = FACE_WEST;
            break;
          }          
        }
        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          success = TRUE; // unused bytes, assignment quiets compiler
      }

      if (level == globalData.startLevel)
      {
        globalData.startX = globalData.levelInfo.stats[level].entryPoints[StartEP].x;
        globalData.startY = globalData.levelInfo.stats[level].entryPoints[StartEP].y;
        globalData.startFacing = globalData.levelInfo.stats[level].entryPoints[StartEP].facing;
      }

      // 8 rest events
      for (i=0; (i<8) && success ;i++)
      {
        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          levelData.zoneData.zones[i].restEvent.everyMin = OneByte;

        // unknown byte
        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          success = TRUE; // unused bytes, assignment quiets compiler

        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          uadata->RestEvents[i] = OneByte;

        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        {
          levelData.zoneData.zones[i].restEvent.chance = OneByte & 0x7F;
          levelData.zoneData.zones[i].restEvent.allowResting = !((OneByte & 0x80) == 0x80);
        }
      }

      int count=0;
      int z;
      // 8 step events
      for (i=0; (i<8) && success ;i++)
      {
        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          count = OneByte;

        // unknown byte
        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          success = TRUE; // unused bytes, assignment quiets compiler

        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
          uadata->StepEvents[i] = OneByte;
        
        // set all zones by default
        levelData.stepEvents[i].stepCount = count;
        //for (z=0;z<8;z++)
          //levelData.stepEvents[i].stepTrigger[z] = TRUE;
        levelData.stepEvents[i].zoneMask = (1 << 8) - 1;

        if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        {
          // if OneByte=0, all zones are used
          if (OneByte != 0)
          {
            // exclude indicated zones
            for (z=0;z<8;z++)
            {
              if (OneByte & (1 << z))
                //levelData.stepEvents[i].stepTrigger[z] = FALSE;
                levelData.stepEvents[i].zoneMask &= ~(1 << z);
            }
          }
        }
      }

      if ((success) && (success = (file.Read(text, 8) == 8)))
        success = TRUE;

      // level name
      if ((success) && (success = (file.Read(text, 16) == 16)))
      {
        text[15] = '\0';
        if (strlen(text) == 0)
          sprintf(text, "Level %i", level+1);
        globalData.levelInfo.stats[level].level_name = text;
        WriteDebugString("Level name is \'%s\'\n", text);
      }

      // 8 zone names
      for (i=0; (i<8) && success ;i++)
      {
        if ((success) && (success = (file.Read(text, 16) == 16)))
        {
          text[15] = '\0';
          if (strlen(text) == 0)
            sprintf(text, "Zone %i", i+1);
          levelData.zoneData.zones[i].zoneName = text;
        }
      }

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
         uadata->ZoneMonster = OneByte;

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
      {
        for (i=0; i<8;i++)
         levelData.zoneData.zones[i].addedTurningDifficulty = OneByte;
      }
        
      // overland art slots 1-8
      if ((success) && (success = (file.Read(text, 8) == 8)))
        success = TRUE; // unused bytes, assignment quiets compiler

      WORD ZoneMsgStrings[8];
      // overland zone messages, 1-8
      for (i=0;i<8 && success;i++)
      {
        if ((success) && (success = (file.Read(&TwoBytes, 2) == 2)))
          ZoneMsgStrings[i]= ( (LOBYTE(TwoBytes) << 8) | (HIBYTE(TwoBytes)) );
      }

      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
         uadata->ZoneCamp = OneByte;
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
         uadata->ZoneTreasure = OneByte;

      // sanity check
      if ((success) && (success = (file.Read(text, 8) == 8)))
        success = (strncmp(text, "MAP", 3) == 0);

      // read the map data
      if ((success) && (success = (file.Read(uadata->MapData, (576*sizeof(UAImportMapCell))) == (576*sizeof(UAImportMapCell)) )))
        success = TRUE; // unused bytes, assignment quiets compiler

      // sanity check
      if ((success) && (success = (file.Read(text, 8) == 8)))
        success = (strncmp(text, "ENCR", 4) == 0);

      // read the event data
      if ((success) && (success = (file.Read(uadata->EventData, (100*sizeof(UAImportEvent))) == (100*sizeof(UAImportEvent)) )))
        success = TRUE; // unused bytes, assignment quiets compiler

      // sanity check
      if ((success) && (success = (file.Read(text, 13) == 13)))
        success = (strncmp(text, "STRG", 4) == 0);

      // read the string count data
      if ((success) && (success = (file.Read(&OneByte, 1) == 1)))
        uadata->StringData.StringCount = OneByte;

      // read the string lengths
      if ((success) && (success = (file.Read(&(uadata->StringData.StringLength), 400) == 400)))
        success = TRUE;

      // read the strings
      if ((success) && (success = (file.Read(uadata->StringData.Strings, 6760) == 6760)))
        success = TRUE;

      if (globalData.levelInfo.stats[level].overland)
      {
        for (i=0;i<8 && success;i++)
          uadata->StringData.GetStringAt(ZoneMsgStrings[i], levelData.zoneData.zones[i].zoneMsg);
      }

#pragma warning (default : 4706) // assignment within conditional expression
    }
    else
    {
      WriteDebugString("Failed to open level file %s\n", fullpath);
      success = FALSE;
    }
  }
  catch (...)
  {
    WriteDebugString("Failed to import level file %s\n", fullpath);
    success = FALSE;
  }

  if (file.m_hFile != CFile::hFileNull)
    file.Close();

  PumpWinMsgs();

  if (success)
  {
    // convert map data
    for (int y=0;y<globalData.levelInfo.stats[level].area_height;y++)
    {
      for (int x=0; x<globalData.levelInfo.stats[level].area_width;x++)      
      {
        UAImportMapCell data = uadata->GetMapCell(level, x,y);

        if (globalData.levelInfo.stats[level].overland)
        {
          // ignore borders
          if (   (x>0) && (y>0)
              && (x!=globalData.levelInfo.stats[level].area_width-1)
              && (y!=globalData.levelInfo.stats[level].area_height-1))
          {
            if (data.ConvertUAOverlandBlockage(FACE_NORTH))
            {
              if (levelData.ValidAreaMapCoords(x, y-1))
                levelData.area[y-1][x].bkgrnd=TRUE;
            }
            if (data.ConvertUAOverlandBlockage(FACE_SOUTH))
            {
              if (levelData.ValidAreaMapCoords(x, y+1))
                levelData.area[y+1][x].bkgrnd=TRUE;
            }
            if (data.ConvertUAOverlandBlockage(FACE_WEST))
            {
              if (levelData.ValidAreaMapCoords(x-1, y))
                levelData.area[y][x-1].bkgrnd=TRUE;
            }
            if (data.ConvertUAOverlandBlockage(FACE_EAST))
            {
              if (levelData.ValidAreaMapCoords(x+1, y))
                levelData.area[y][x+1].bkgrnd=TRUE;
            }
          }
        }
        else
        {
          levelData.area[y][x].northWall = data.ConvertUAWallSlot(FACE_NORTH);
          levelData.area[y][x].northBlockage = data.ConvertUABlockage(FACE_NORTH);
        
          levelData.area[y][x].eastWall = data.ConvertUAWallSlot(FACE_EAST);
          levelData.area[y][x].eastBlockage = data.ConvertUABlockage(FACE_EAST);
        
          levelData.area[y][x].southWall = data.ConvertUAWallSlot(FACE_SOUTH);
          levelData.area[y][x].southBlockage = data.ConvertUABlockage(FACE_SOUTH);

          levelData.area[y][x].westWall = data.ConvertUAWallSlot(FACE_WEST);
          levelData.area[y][x].westBlockage = data.ConvertUABlockage(FACE_WEST);

          levelData.area[y][x].SetAllBG(data.ConvertUABackdropSlot());
        }

        levelData.area[y][x].zone = data.ConvertUAZone();
        levelData.area[y][x].eventExists = (data.Event > 0);

        if (data.Event > 0)
        {
          uadata->MapEvents[data.Event].x = x;
          uadata->MapEvents[data.Event].y = y;
          uadata->MapEvents[data.Event].event = data.Event;
          uadata->MapEvents[data.Event].root = TRUE;
        }
      }
    }

    PumpWinMsgs();


    UsedEvents.RemoveAll();

    // first add root events
    for (i=0; i<MAX_IMPORT_EVENTS; i++)
    {
      if (uadata->MapEvents[i+1].root)
        AddEventChain(i);
    }

    PumpWinMsgs();

    for (i=0;i<8;i++)
    {
      if (uadata->StepEvents[i] > 0)
        levelData.stepEvents[i].stepEvent = AddEventChain(uadata->StepEvents[i]-1);//GetEventKey(uadata->StepEvents[i]);
    }

    PumpWinMsgs();

    for (i=0;i<8;i++)
    {
      if (uadata->RestEvents[i] > 0)
        levelData.zoneData.zones[i].restEvent.restEvent =  AddEventChain(uadata->RestEvents[i]-1);//GetEventKey(uadata->RestEvents[i]);
    }    

    // now add non-root events that haven't been chained to anything
    for (i=0; i<MAX_IMPORT_EVENTS; i++)
    {
      if (uadata->EventData[i].ConvertEventType()!=NoEvent)
      {
#ifdef _DEBUG
        DWORD tempkey;
        ASSERT(UsedEvents.Lookup(i, tempkey)); // I think all should be added by now?
#endif
        AddEventChain(i);
      }
    }

    PumpWinMsgs();
  }

  PumpWinMsgs();

  StatusMsg.Format("Imported %d x %d level called %s", 
                   globalData.levelInfo.stats[level].area_width,
                   globalData.levelInfo.stats[level].area_height,
                   globalData.levelInfo.stats[level].level_name);

  EditorStatusMsg(StatusMsg);

  WriteDebugString("Imported %d x %d level called %s from file %s\n", 
                   globalData.levelInfo.stats[level].area_width,
                   globalData.levelInfo.stats[level].area_height,
                   globalData.levelInfo.stats[level].level_name,
                   fullpath);

  PumpWinMsgs();

  if (success)
  {
    if (UseDefaultArtForImport)
    {
      for (i=0;i<MAX_WALLSETS;i++)
        WallSets[i].Clear();

      for (i=0;i<MAX_BACKGROUNDS;i++)
        BackgroundSets[i].Clear();

      if (globalData.levelInfo.stats[level].overland)
      {
#ifdef SIMPLE_STRUCTURE
        BackgroundSets[1].backgroundFile.Format("%s%s", ede.TemplateOverlandArtDir(), DEFAULT_OVERLAND);
#else
        BackgroundSets[1].backgroundFile.Format("%s%s", EditorArt, DEFAULT_OVERLAND);
#endif
      }
      else
      {
        for (int w=1;w<=16;w++)
        {
          switch (GetWallType(uadata->Wall1, uadata->Wall2, uadata->Wall3, w))
          {
          case UAWallType_None:
            break;

          case UAWallType_Wall:
            WallSets[w].wallFile = ImportWallSlotNames[w];
            break;

          case UAWallType_Door:
            WallSets[w].wallFile = ImportWallSlotNames[w];
            WallSets[w].doorFile = ImportDoorSlotNames[w];
            break;
          }
        }

        for (i=1;i<=4;i++)
        {
          BYTE value = GetBackdropType(uadata->Backdrop1,uadata->Backdrop2,uadata->Backdrop3,uadata->Backdrop4, i);
          if ((value >= 1) && (value <= MAX_IMPORT_BACKDROPS))
            BackgroundSets[i].backgroundFile = ImportBackdropNames[value];
          else
            BackgroundSets[i].backgroundFile = ImportBackdropNames[1];
        }
      }
    }
    else
    {
      BYTE offset;
      for (int y=0;y<globalData.levelInfo.stats[level].area_height;y++)
      {
        for (int x=0; x<globalData.levelInfo.stats[level].area_width;x++)
        {
          offset = OffsetWallSlotIndex(levelData.area[y][x].northWall);
          if (   (!WallSets[offset].wallFile.IsEmpty())
              || (!WallSets[offset].doorFile.IsEmpty())
              || (!WallSets[offset].overlayFile.IsEmpty()))
            levelData.area[y][x].northWall=offset;

          offset = OffsetWallSlotIndex(levelData.area[y][x].southWall);
          if (   (!WallSets[offset].wallFile.IsEmpty())
              || (!WallSets[offset].doorFile.IsEmpty())
              || (!WallSets[offset].overlayFile.IsEmpty()))
            levelData.area[y][x].southWall=offset;

          offset = OffsetWallSlotIndex(levelData.area[y][x].eastWall);
          if (   (!WallSets[offset].wallFile.IsEmpty())
              || (!WallSets[offset].doorFile.IsEmpty())
              || (!WallSets[offset].overlayFile.IsEmpty()))
            levelData.area[y][x].eastWall=offset;

          offset = OffsetWallSlotIndex(levelData.area[y][x].westWall);
          if (   (!WallSets[offset].wallFile.IsEmpty())
              || (!WallSets[offset].doorFile.IsEmpty())
              || (!WallSets[offset].overlayFile.IsEmpty()))
            levelData.area[y][x].westWall=offset;

          /*
          offset = OffsetBackdropSlotIndex(levelData.area[y][x].northBG);
          if (!BackgroundSets[offset].backgroundFile.IsEmpty())
            levelData.area[y][x].northBG=offset;

          offset = OffsetBackdropSlotIndex(levelData.area[y][x].southBG);
          if (!BackgroundSets[offset].backgroundFile.IsEmpty())
            levelData.area[y][x].southBG=offset;
          
          offset = OffsetBackdropSlotIndex(levelData.area[y][x].eastBG);
          if (!BackgroundSets[offset].backgroundFile.IsEmpty())
            levelData.area[y][x].eastBG=offset;
          
          offset = OffsetBackdropSlotIndex(levelData.area[y][x].westBG);
          if (!BackgroundSets[offset].backgroundFile.IsEmpty())
            levelData.area[y][x].westBG=offset;
          */
        }
      }

      
    }
  }

  PumpWinMsgs();

  if (success)
  {
    levelData.GetSlots();
    levelData.SaveArt(level);
    saveLevel(level);
  }

  PumpWinMsgs();

  talesPresent = false;

  if (success)
    globalData.levelInfo.stats[level].used = TRUE;

  return success;
}

#pragma warning( default : 4706 ) // assignment within conditional expression

//////////////////////////////////////////////////////////////
//
//  item importing definitions
//
//////////////////////////////////////////////////////////////

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

//
// damage vs. creature structures
//
struct ImportUADamage
{
    BYTE dice;
    BYTE sides;
    BYTE bonus;
};

//
// items.dat structure - general item classes
//
struct ImportUAItems
{
    BYTE slot;  // location carried
    BYTE twohanded; // num hands needed
    ImportUADamage vs_large;
    BYTE rate; // per 2 rounds; if 1, it = 2 for 1-handed and 1 for 2-handed
    BYTE protection;
    BYTE cutting_or_blunt; // 0 not wpn, 1 = cut wpn, 128 = blunt wpn
    BYTE melee; // usable in melee
    ImportUADamage vs_small; // size small/medium
    BYTE range;
    BYTE classes; // 1=MU,2=Cl,4=Th,8=Fi,64=Pal,Rgr
    BYTE weapon_type; // 0=NoWpn,4=HandHeld,10=Sling,11,15=Bow,18=ammo,20=HandHeldOrThrow,26=Thrown,138=Crossbow
    BYTE foo;
};

//
// item.dat structure - specific items
//
struct ImportUAItem
{
    BYTE ptr; // ptr to record in items.dat file
    BYTE name3;  // index into vocab list
    BYTE name2;
    BYTE name1;
    WORD encumbrance; // in coins, 10 coins = 1 pound
    WORD price; // in platinum
    BYTE magic_bonus;
    BYTE secondary_code;
    BYTE ready;
    BYTE identified;
    BYTE cursed;
    BYTE numitems; // bundle qty
    BYTE numcharges; // 0 = permanent
    BYTE magicalcode;
    BYTE special_code;
    BYTE foo;
};

//
// vocabulary table to generate description
//
char *ImportUAItemVocab[] = {
    "",
    "Battle Axe",
    "Hand Axe",
    "Club",
    "Dagger",
    "Dart",
    "Hammer",
    "Javelin",
    "Mace",
    "Morning Star",
    "Military Pick",
    "Awl Pike",
    "Bolt",
    "Scimitar",
    "Spear",
    "Quarter Staff",
    "Bastard Sword",
    "Broad Sword",
    "Long Sword",
    "Short Sword",
    "Two-Handed Sword",
    "Trident",
    "Composite Long Bow",
    "Composite Short Bow",
    "Long Bow",
    "Short Bow",
    "Fine",
    "Light Crossbow",
    "Sling",
    "Staff",
    "Arrow",
    "Leather",
    "Ring",
    "Scale",
    "Chain",
    "Banded",
    "Plate",
    "Shield",
    "Cleric",
    "Scroll",
    "Mage",
    "Helm",
    "Belt",
    "Robe",
    "Cloak",
    "Boots",
    "Ring",
    "Mail",
    "Armor",
    "Of Prot",
    "Bracers",
    "Wand",
    "Elixir",
    "Potion",
    "Youth",
    "Ruby",
    "Boulder",
    "Dragon Breath",
    "Displacement",
    "Eyes",
    "Drow",
    "Elfin Chain",
    "Ice Storm",
    "Sapphire",
    "Emerald",
    "Wizardry",
    "Hornet's Nest",
    "Fire Resistance",
    "Stone",
    "Good Luck",
    "Flail",
    "Halberd",
    "Gauntlets",
    "Periapt",
    "Health",
    "Cursed",
    "Blessed",
    "Bundle of",
    "Ogre Power",
    "Girdle",
    "Giant Strength",
    "Mirror",
    "Necklace",
    "Dragon",
    "vs Giants",
    "vorpal",
    "cold resistance",
    "Diamond",
    "Lightning",
    "Fireballs",
    "of",
    "Vulnerability",
    "Speed",
    "Silver",
    "Extra",
    "Healing",
    "Charming",
    "Fear",
    "Magic Missiles",
    "Missiles",
    "1 Spell",
    "2 Spells",
    "3 Spells",
    "Paralyzation",
    "Invisibility",
    "Cute Yellow Canary",
    "AC 10",
    "AC 6",
    "AC 4",
    "AC 3",
    "AC 2",
    "+1",
    "+2",
    "+3",
    "+4",
    "+5",
    "-1",
    "-2",
    "-3",
    "Electric Immunity",
    "Gaze Resistance",
    "Spiritual",
    "Gem",
    "Jewelry",
    "blinking",
    "from evil"
};

BOOL ImportUAItemsToUAF(const char *path)
{
  FILE *fp;
  ImportUAItem Items[300];
  ImportUAItems ItemClasses[300];
  memset(Items, 0, sizeof(Items));
  memset(ItemClasses, 0, sizeof(ItemClasses));
    
  int i;

  WriteDebugString("idump v1.0\n");
  WriteDebugString("Copyright (c) 1999 by Bil Simser\n");
  WriteDebugString("All rights reserved\n\n");

  WriteDebugString("Items.dat record size: %u (=16)\n", sizeof(ImportUAItems));
  WriteDebugString("Item.dat record size: %u (=18)\n", sizeof(ImportUAItem));
  CString fullPath;

  //
  // read generic item class information
  //
  fullPath.Format("%sitems.dat", path);
  WriteDebugString("Opening %s\n", fullPath);
  fp = fopen(fullPath, "rb");
  if(fp == NULL)
  {
      WriteDebugString("Error opening %s!\n", fullPath);
      return FALSE;
  }
  WriteDebugString("Reading item classes\n");

  int ClassCount=0;
  BOOL success = TRUE;
  while ((ClassCount<300) && (success))//(!feof(fp)))
  {
    success = fread(&ItemClasses[ClassCount], sizeof(ImportUAItems), 1, fp);
    ClassCount++;
  }
  fclose(fp);
  WriteDebugString("Read %i item classes\n", ClassCount);

  //
  // read specific items
  //
  fullPath.Format("%sitem.dat", path);
  WriteDebugString("Opening %s\n", fullPath);
  fp = fopen(fullPath, "rb");
  if(fp == NULL)
  {
      WriteDebugString("Error opening %s!\n", fullPath);
      return FALSE;
  }
  WriteDebugString("Reading item information\n");

  int minclass=301;
  int maxclass=-1;

  int ItemCount=0;
  success=TRUE;
  while ((ItemCount<300) && (success))//(!feof(fp)))
  {
    success = fread(&Items[ItemCount], sizeof(ImportUAItem), 1, fp);
    if (Items[ItemCount].ptr < minclass) minclass = Items[ItemCount].ptr;
    if (Items[ItemCount].ptr > maxclass) maxclass = Items[ItemCount].ptr;
    ItemCount++;
  }
  fclose(fp);
  WriteDebugString("Read %i item information records\n", ItemCount);

  WriteDebugString("Importing %d items\n", ItemCount);


  ITEM_DATA idata;
  CString tempName;
  CString tempIdName;

  for(i=0; i<ItemCount; i++)
  {
    idata.Clear();

    ImportUAItem Item = Items[i];
    ASSERT( (Item.ptr >= 0) && (Item.ptr <= 255) );
    ImportUAItems Class = ItemClasses[Item.ptr];

    tempIdName="";
    tempName="";

    // known and unknown names
    if ((Item.name1 > 0) && (Item.name1 < 126))
    {
      tempIdName += ImportUAItemVocab[Item.name1];
      tempIdName += " ";

      if ((Item.identified & 1) == 0)
      {
        tempName += ImportUAItemVocab[Item.name1];
        tempName += " ";
      }
    }

    if ((Item.name2 > 0) && (Item.name2 < 126))
    {
      if (Item.name3 == 77)
      {
        CString qty;
        qty.Format("%u ", Item.name2);
        tempIdName += qty;

        if ((Item.identified & 2) == 0)
          tempName += qty;
      }
      else
      {
        tempIdName += ImportUAItemVocab[Item.name2];
        tempIdName += " ";

        if ((Item.identified & 2) == 0)
        {
          tempName += ImportUAItemVocab[Item.name2];
          tempName += " ";
        }
      }
    }

    if ((Item.name3 > 0) && (Item.name3 < 126))
    {
      tempIdName += ImportUAItemVocab[Item.name3];      

      if ((Item.identified & 4) == 0)
        tempName += ImportUAItemVocab[Item.name3];
    }

    idata.SetUniqueName(tempName);
    idata.SetIdName(tempIdName);

    switch (Class.slot)
    {
    case 0: idata.Location_Readied = WeaponHand; break;
    case 1: idata.Location_Readied = ShieldHand; break;
    case 2: idata.Location_Readied = BodyArmor; break;
    case 3: idata.Location_Readied = Hands; break;
    case 4: idata.Location_Readied = Head; break;
    case 5: idata.Location_Readied = Waist; break;
    case 6: idata.Location_Readied = BodyRobe; break;
    case 7: idata.Location_Readied = Back; break;
    case 8: idata.Location_Readied = Feet; break;
    case 9: idata.Location_Readied = Fingers; break;
    case 10: idata.Location_Readied = AmmoQuiver; break;
    }

    if (   (idata.Location_Readied == WeaponHand) 
        || (idata.Location_Readied == ShieldHand)
        || (idata.Location_Readied == AmmoQuiver))
      idata.Hands_to_Use = Class.twohanded; // num hands needed to hold
    else
      idata.Hands_to_Use = 0; // doesn't occupy hand slots when readied

    idata.Nbr_Dice_Sm = Class.vs_small.dice;
    idata.Dmg_Dice_Sm = Class.vs_small.sides;
    idata.Dmg_Bonus_Sm = Class.vs_small.bonus;
    
    idata.Nbr_Dice_Lg = Class.vs_large.dice;
    idata.Dmg_Dice_Lg = Class.vs_large.sides;
    idata.Dmg_Bonus_Lg = Class.vs_large.bonus;

    if (Class.rate <= 2)
      idata.ROF_Per_Round = Class.rate;
    else
      idata.ROF_Per_Round = ((double)Class.rate)/2.0;    

    if (Class.cutting_or_blunt != 0)
    {
      if (Class.weapon_type == 138)
        idata.Wpn_Type = Crossbow;
      else if (Class.weapon_type == 26)
        idata.Wpn_Type = Throw;
      else if (Class.weapon_type == 20)
        idata.Wpn_Type = HandThrow;
      else if (Class.weapon_type == 18)
        idata.Wpn_Type = Ammo;
      else if (Class.weapon_type == 15)
        idata.Wpn_Type = Bow;
      else if (Class.weapon_type == 11)
        idata.Wpn_Type = Bow;
      else if (Class.weapon_type == 10)
        idata.Wpn_Type = SlingNoAmmo;
      else if (Class.weapon_type == 4)
      {
        if (Class.cutting_or_blunt & 1)
          idata.Wpn_Type = HandCutting;
        else if (Class.cutting_or_blunt & 128)
          idata.Wpn_Type = HandBlunt;
        else
          idata.Wpn_Type = NotWeapon; // shouldn't hit this!
      }
      else
        idata.Wpn_Type = NotWeapon;
    }
    else
      idata.Wpn_Type = NotWeapon;
 

    BYTE prot;
    if (Class.protection > 170)
    {
      prot = 188 - Class.protection;
      if (prot > 0)
        idata.Protection_Base = -(10 - prot);
      else
        idata.Protection_Base = 0;
    }
    else if (Class.protection > 117) // < 128 would be shield -2 for example
    {
      prot = Class.protection - 128;
      idata.Protection_Base = -(prot);
    }
    else
      idata.Protection_Base = 0;

    idata.m_usageFlags = 0;
    idata.RangeMax = Class.range;

    // perform some conversion checks, not all
    // FRUA data matches exactly with UAF interpretation

    // min range is 1 for weapon
    if (idata.Wpn_Type != NotWeapon)
      idata.RangeMax = max(1, idata.RangeMax);

    // Bows use weapon hand, ammo uses shield hand, so
    // cannot leave Bows as two-handed weapon
    if ((idata.Wpn_Type == Bow) || (idata.Wpn_Type == Crossbow))
      idata.Hands_to_Use = 1;

    // ammo uses ROF and range of firing weapon
    if (idata.Location_Readied == AmmoQuiver)
    {      
      idata.RangeMax = 0;
      idata.ROF_Per_Round = 0.0;
    }


    {
      BASECLASS_ID baseclassID;
      if (Class.classes & 1)
      {
        baseclassID= "Magic User";
        //idata.Usable_by_Class |= MagicUserFlag;
        idata.usableByBaseclass.Add(baseclassID);
      };
      if (Class.classes & 2)
      {
        //idata.Usable_by_Class |= ClericFlag;
        //idata.Usable_by_Class |= DruidFlag;
        baseclassID = "Cleric";
        idata.usableByBaseclass.Add(baseclassID);
        baseclassID = "Druid";
        idata.usableByBaseclass.Add(baseclassID);
      };
      if (Class.classes & 4)
      {
        //idata.Usable_by_Class |= ThiefFlag;
        baseclassID = "Thief";
        idata.usableByBaseclass.Add(baseclassID);
      };
      if (Class.classes & 8)
      {
        //idata.Usable_by_Class |= FighterFlag;
        baseclassID = "Fighter";
        idata.usableByBaseclass.Add(baseclassID);
      };
      if (Class.classes & 64)
      {
        //idata.Usable_by_Class |= (RangerFlag | PaladinFlag);
        baseclassID = "Ranger";
        idata.usableByBaseclass.Add(baseclassID);
        baseclassID = "Paladin";
        idata.usableByBaseclass.Add(baseclassID);
      };
    };


    idata.Encumbrance = Item.encumbrance;
    idata.Cost = Item.price;

    if (idata.Wpn_Type != NotWeapon)
    {
      idata.Attack_Bonus = Item.magic_bonus;
      if (idata.Attack_Bonus > 127) idata.Attack_Bonus = -(256-Item.magic_bonus);
      idata.Dmg_Bonus_Sm = idata.Attack_Bonus;
      idata.Dmg_Bonus_Lg = idata.Attack_Bonus;
    } 
    else
      idata.Protection_Bonus = -(Item.magic_bonus);

    idata.Cursed = Item.cursed;
    idata.Bundle_Qty = max(Item.numitems, 1);
    idata.Num_Charges = Item.numcharges;
        
    // not using magic yet
    //idata.m_gsID.Clear();
    //idata.Spell_Level = 1;
    //idata.Spell_Class = MagicUserFlag;
    //fprintf(fp, "%d\n", Item.magicalcode);
    //fprintf(fp, "%d\n", Item.special_code);

#ifdef SIMPLE_STRUCTURE
    idata.HitSound = ede.TemplateSoundDir() + DEF_HIT_SOUND;
    idata.MissSound = ede.TemplateSoundDir() + DEF_MISS_SOUND;

    if ((idata.Wpn_Type == Throw) || (idata.Wpn_Type == HandThrow))
    {
      idata.MissileArt.style = PIC_DATA::AS_None;
      idata.MissileArt.filename = ede.TemplateMissileArtDir() + DEFAULT_IIRT;
    }
    else if (idata.Wpn_Type == Ammo)
    {
      idata.MissileArt.style = PIC_DATA::AS_Directional;
      idata.MissileArt.filename = ede.TemplateMissileArtDir() + DEFAULT_IIRA;
    }
    else if (idata.Wpn_Type == SlingNoAmmo)
    {
      idata.MissileArt.style = PIC_DATA::AS_None;
      idata.MissileArt.filename = ede.TemplateMissileArtDir() + DEFAULT_IIRS;
    }   
#else
    idata.HitSound.Format("%s%s",EditorArt, DEF_HIT_SOUND);
    idata.MissSound.Format("%s%s",EditorArt, DEF_MISS_SOUND);

    if ((idata.Wpn_Type == Throw) || (idata.Wpn_Type == HandThrow))
    {
      idata.MissileArt.style = PIC_DATA::AS_None;
      idata.MissileArt.filename.Format("%s%s",EditorArt, DEFAULT_IIRT);
    }
    else if (idata.Wpn_Type == Ammo)
    {
      idata.MissileArt.style = PIC_DATA::AS_Directional;
      idata.MissileArt.filename.Format("%s%s",EditorArt, DEFAULT_IIRA);
    }
    else if (idata.Wpn_Type == SlingNoAmmo)
    {
      idata.MissileArt.style = PIC_DATA::AS_None;
      idata.MissileArt.filename.Format("%s%s",EditorArt, DEFAULT_IIRS);
    }   
#endif
    switch (idata.Wpn_Type)
    {
    case HandThrow:
    case Throw:
    case SlingNoAmmo:
      break;
    case Ammo:
      if (idata.AmmoType.IsEmpty())
      {
        if (strstr(idata.IdName(), "Bolt") != NULL)
          idata.AmmoType="CrossBow";
        else
          idata.AmmoType="Bow";
      }
      itemData.AddAmmoType(idata.AmmoType);
      break;
    case Bow:
      if (idata.AmmoType.IsEmpty())
      {
        idata.AmmoType="Bow";
        itemData.AddAmmoType(idata.AmmoType);
      }
      break;
    case Crossbow:
      if (idata.AmmoType.IsEmpty())
      {
        idata.AmmoType="CrossBow";
        itemData.AddAmmoType(idata.AmmoType);
      }
      break;

    default:
      break;
    }
    {
      CString temp;
      temp = idata.UniqueName();
      temp.TrimLeft();
      temp.TrimRight();
      idata.SetUniqueName(temp);
      temp = idata.IdName();
      temp.TrimLeft();
      temp.TrimRight();
      idata.SetIdName(temp);
    };

    TRACE("Adding item %i: \'%s\' (%s)\n", i+1, idata.UniqueName(), idata.IdName());
    if (!idata.UniqueName().IsEmpty())
    {
      //GLOBAL_ITEM_ID giID = itemData.AddItem(idata);
      ITEM_ID itemID;
      int itemIndex;
      itemIndex = itemData.AddItem(idata);
      //if (giID.IsValidItem())
      if (itemIndex >= 0)
      {
        DWORD index=1;
        BOOL validname=false;
        CString name;
        itemID = itemData.PeekItem(itemIndex)->ItemID();
        while (!validname)
        {
          ITEM_ID itemId;
          name.Format("%s/%u", idata.IdName(), index);
          itemId = name;
          //validname = !itemData.FindName(name).IsValidItem();
          validname = !itemId.IsValidItem();
          if (index >= MAX_ITEMS)
            break;
          index++;
        }
        if (validname) idata.SetIdName(name);
        //giID = itemData.AddItem(idata);
        itemIndex = itemData.AddItem(idata);
        //ASSERT( giID.IsValidItem() );
        ASSERT( itemIndex >= 0);
      }
      //giID_Keys[i] = giID;
      itemID_Keys[i] = itemID;
    }
  }

  return TRUE;
}

//////////////////////////////////////////////////////////////
//
//  spell importing definitions
//
//////////////////////////////////////////////////////////////

#define MAX_IMPORT_SPELLS 137

// 137 spell names start at offset 557724. Each is variable length
// and null terminated.
//
CString ImportUASpellNames[MAX_IMPORT_SPELLS];

// 137 spells of 22 bytes each, starting at offset 554688 in ckit.exe
struct ImportUASpell
{
  BYTE type; // 0=cl,1=dr,2=mu,3=dev,4=dev
  BYTE foo1;
  BYTE level; // type=3->level=0, type=4->level=6
  BYTE range; // 255=touch
  BYTE rangePerLevel; // 12=DimensionDoor, 16=MinorGlobeOfInvulnerability
  BYTE duration; // in rounds
  BYTE durationPerLevel;
  BYTE targeting;
  BYTE moreTargeting;
  BYTE foo2;
  BYTE saveResult;
  BYTE foo3;
  BYTE saveVersus;
  BYTE foo4;
  BYTE specialAbility;
  BYTE castInCampCombat; // 0=camp,1=combat,2=both
  BYTE foo5;
  BYTE castTime; // in segments
  BYTE priority;
  BYTE friendlyFire; // 0=allies only, 1=foes only
  BYTE foo6;
  BYTE foo7;
};

BOOL ImportUASpellsToUAF(const char *path)
{

  return TRUE;
}

struct ImportUACCH
{
  BYTE foo1;
  BYTE foo2;
  BYTE foo3;
  BYTE foo3a;
  WORD saCount; // not needed
  WORD specAbs; // 0/0 if no special abilities, 254/104 if one or more special abilities
  WORD carryCount; // not needed
  WORD isCarry; // 0/0 if no items, 76/95 (0x1E0F) if one or more items carried - 74/57
  WORD whCount;
  WORD isWpnHand;
  WORD shCount;
  WORD isShHand;
  WORD arCount;
  WORD isArmor;
  WORD gCount;
  WORD isGauntlet;
  WORD hCount;
  WORD isHelmet;
  WORD bCount;
  WORD isBelt;
  WORD rCount;
  WORD isRobe;
  WORD cCount;
  WORD isCloak;
  WORD btCount;
  WORD isBoots;
  WORD rgCount;
  WORD isRing;
  WORD rg2Count;
  WORD isRing2;
  WORD amCount;
  WORD isAmmo;
  DWORD foo4;
  DWORD foo5;
  DWORD exp;
  DWORD expPreDrain;
  WORD platinum;
  WORD gems;
  WORD jewelry;
  WORD age;
  WORD expWorth; // for killing, 0 for characters
  WORD encumb; // in gold pcs
  BYTE race; // 0=elf,1=halfelf,2=dwarf,3=gnome,4=halfling,5=human,6=monster
  BYTE charClass; //0 = Cleric,1 = Knight (Not Used),2 = Fighter,3 = Paladin
                  //4 = Ranger,5 = Magic-User,6 = Thief,7 = Monk (Not Used)
                  //8 = C/F,9 = C/F/MU,10 = C/R,11 = C/MU,12 = C/T,13 = F/MU
                  //14 = F/T,15 = F/MU/T,16 = MU/T
  BYTE undead; // 0 = Not Undead,1 = Skeleton,2 = Zombie,3 = Ghoul,4 = Shadow
               //5 = Wight,6 = Ghast,7 = Wraith,8 = Mummy,9 = Spectre
               //10 = Vampire,11 = Ghost,12 = Lich,13 = Special
  BYTE foo6;
  BYTE gender; // 0=male,1=female
  BYTE alignment; //0 = LG   3 = NG   6 = CG
                  //1 = LN   4 = TN   7 = CN
                  //2 = LE   5 = NE   8 = CE
  BYTE status; //0 = Okay,1 = Animated,2 = Temp Gone,3 = Running,4 = Unconscious
               //5 = Dying,6 = Dead,7 = Petrified,8 = Gone
  BYTE combatMode; // 0=pc/npc, 1=monster
  BYTE name[16]; // 15 plus zero terminator
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
  BYTE strExt; // 0.100
  BYTE strExtMod;
  BYTE foo7;
  BYTE thac0; //60 - Base THAC0
  BYTE cureDisease; // 0=cannot, else nbr times
  BYTE maxHp;
  BYTE size; // 1 = small combat icon
             // 2 = tall combat icon
             // 3 = wide combat icon
             // 4 = big combat icon
             // 5 = huge combat icon (2 tall x 3 wide)
             // Add 128 to this value if button for "Large even if 1x1" has been selected.
  BYTE svPaPoDm;
  BYTE svPePo;
  BYTE svRoStWa;
  BYTE svBw;
  BYTE svSp;
  BYTE move;
  BYTE level; // in highest active class;
  BYTE levelFormerClass;
  BYTE pickPockets; //Note:  Thieving skills are adjusted for level
  BYTE openLocks;   // and race but not for dexterity or armor.
  BYTE findRemTraps;
  BYTE moveSilent;
  BYTE hideShadows;
  BYTE hearNoise;
  BYTE climbWalls;
  BYTE readLang;
  BYTE morale; // 128 + morale if non-zero
  BYTE fighterSweepLevel;
  BYTE foo8;

  BYTE clericLvlPreDrain;
  BYTE knightLvlPreDrain;
  BYTE fighterLvlPreDrain;
  BYTE paladinLvlPreDrain;
  BYTE rangerLvlPreDrain;
  BYTE mageLvlPreDrain;
  BYTE thiefLvlPreDrain;

  BYTE clericLvl;
  BYTE knightLvl;
  BYTE fighterLvl;
  BYTE paladinLvl;
  BYTE rangerLvl;
  BYTE mageLvl;
  BYTE thiefLvl;

  BYTE clericLvlPreClassChng;
  BYTE knightLvlPreClassChng;
  BYTE fighterLvlPreClassChng;
  BYTE paladinLvlPreClassChng;
  BYTE rangerLvlPreClassChng;
  BYTE mageLvlPreClassChng;
  BYTE thiefLvlPreClassChng;

  BYTE attacksPer2Rnds;
  BYTE attacksPer2Rnds2nd;
  BYTE dmgDiceNbr;
  BYTE dmgDiceNbr2nd;
  BYTE dmgDiceSides;
  BYTE dmgDiceSides2nd;
  BYTE dmgBonus;
  BYTE dmgBonus2nd;
  BYTE ac; // 60 - base AC;
  WORD foo9;
  BYTE hpPreDrain;
  BYTE itemMask; // 1 = Magic-User,2 = Cleric,4 = Thief,8 = Fighter
                 // 16 = Knight,32 = ???,64 = Paladin or Ranger,128 = ???
  BYTE hp; // as rolled, no con bonus
  BYTE foo10;
  WORD foo11;
  BYTE iconId; // 0-48
  BYTE uniquePartyID; // 0-7, 8=not in party yet
  BYTE foo12;
  BYTE specAbFlags;   // 1 = Affected by Dispel Evil
                      // 2 = Mammal
                      // 4 = Penalty to Hit Dwarf
                      // 8 = Extra Damage from Ranger
                      // 16 = Snake
                      // 32 = Penalty to Hit Gnome
                      // 64 = Animal
                      // 128 = Dwarf has bonus to hit you
  BYTE specAbFlags2;  // 1 = True Giant
                      // 2 = May be Held or Charmed
                      // 4 = Gnome has bonus to hit you
                      // 8 = Immune to Death Magic
                      // 16 = Immune to Poison
                      // 32 = Immune to Decapitation by Vorpal Sword
                      // 64 = Immune to Confusion
  BYTE itemBundlesCarried;
  BYTE nbrHandsFull;
  BYTE foo13;
  BYTE magicResistance; // percentage
  BYTE rdyToTrain; // 0=no,1=yes
  BYTE spellsMemorized[141];
  BYTE spellBook[16];
  BYTE numClericSpellsByLvl[7];
  WORD foo14;
  BYTE numDruidSpellsByLvl[3];
  BYTE foo15[6];
  BYTE numMageSpellsByLvl[9];
  BYTE active; // 0=down, 1=active
  BYTE foo16;
  BYTE adjThac0; // 60 - adj thac0
  BYTE adjAC; // 60 - adj ac
  BYTE foo17;
  BYTE dmgUsingCurrWpn[8]; // don't care
  BYTE adjHp; // including con bonus;
  BYTE adjMove;
  BYTE monsterIndex; // 0 for player
  BYTE itemCarried[16];
  BYTE itemQty[16];
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

struct ImportMonstFooter
{
  BYTE itemsCarried[16];
  BYTE itemQty[16];
  BYTE specAbs[20];
};

WORD SwapTwoBytes(WORD data)
{
  //return ( (LOBYTE(data) << 8) | (HIBYTE(data)) );  
  return data;
}

DWORD SwapFourBytes(DWORD data)
{
  //return ( (SwapTwoBytes(LOWORD(data)) << 16) | (SwapTwoBytes(HIWORD(data))) );
  return data;
}

void SetClass(char *className, CHARACTER& dude)
{
  CLASS_ID classID;
  classID = className;
  dude.SetClass(classID);
}

void SetLevels(CHARACTER& dude, CString baseclass, int curr, int prev, int predrain)
{
  int i;
  BASECLASS_ID baseclassID;
  baseclassID = baseclass;
  i = dude.LocateBaseclassStats(baseclassID);
  if (i >= 0)
  {
    BASECLASS_STATS *pbs;
    pbs = dude.GetBaseclassStats(i);
    if (pbs->currentLevel != curr)
    {
      dude.temp__canUsePrevClass = -1;
    };
    pbs->currentLevel = curr;
    pbs->previousLevel = prev;
    pbs->preDrainLevel = predrain;
  };
}


void ProcessNpcCchData(ImportUACCH &cch, CHARACTER &dude)
{
  WriteDebugString("Importing NPC \'%s\'\n", cch.name);
  dude.SetType(NPC_TYPE);
  dude.SetAC(60 - cch.ac);
  dude.SetAge(SwapTwoBytes(cch.age));
  switch (cch.alignment)
  {
  case 0:
    dude.SetAlignment(LawfulGood);
    break;
  case 1:
    dude.SetAlignment(LawfulNeutral);
    break;
  case 2:
    dude.SetAlignment(LawfulEvil);
    break;
  case 3:
    dude.SetAlignment(NeutralGood);
    break;
  case 4:
    dude.SetAlignment(TrueNeutral);
    break;
  case 5:
    dude.SetAlignment(NeutralEvil);
    break;
  case 6:
    dude.SetAlignment(ChaoticGood);
    break;
  case 7:
    dude.SetAlignment(ChaoticNeutral);
    break;
  case 8:
    dude.SetAlignment(ChaoticEvil);
    break;
  }  


  //dude.ItemMask = 0;
  
  int min1=0; // exp totals
  int min2=0;
  int min3=0;

  switch (cch.charClass)
  {
  case 0:
    //dude.SetClass(Cleric);
    SetClass("Cleric", dude);
    //dude.ItemMask |= ClericFlag;
    min1 = GetMaxClericExpForLevel(cch.clericLvl);
    break;
  case 1:
    //dude.SetClass(Druid); // knight
    SetClass("Druid", dude); // knight
    //dude.ItemMask |= DruidFlag;
    min1 = GetMaxDruidExpForLevel(cch.knightLvl);
    break;
  case 2:
    //dude.SetClass(Fighter);
    SetClass("Fighter", dude);
    //dude.ItemMask |= FighterFlag;
    min1 = GetMaxFighterExpForLevel(cch.fighterLvl);
    break;
  case 3:
    //dude.SetClass(Paladin);
    SetClass("Paladin", dude);
    //dude.ItemMask |= PaladinFlag;
    min1 = GetMaxPaladinExpForLevel(cch.paladinLvl);
    break;
  case 4:
    //dude.SetClass(Ranger);
    SetClass("Ranger", dude);
    //dude.ItemMask |= RangerFlag;
    min1 = GetMaxRangerExpForLevel(cch.rangerLvl);
    break;
  case 5:
    //dude.SetClass(MagicUser);
    SetClass("Magic User", dude);
    //dude.ItemMask |= MagicUserFlag;
    min1 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    break;
  case 6:
    //dude.SetClass(Thief);
    SetClass("Thief", dude);
    //dude.ItemMask |= ThiefFlag;
    min1 = GetMaxThiefExpForLevel(cch.thiefLvl);
    break;
  case 7:
    //dude.SetClass(Fighter); // monk
    SetClass("Fighter", dude); // monk
    //dude.ItemMask |= FighterFlag;
    min1 = GetMaxFighterExpForLevel(cch.fighterLvl);
    break;
  case 8:
    //dude.SetClass(C_F);
    SetClass("C_F", dude);
    //dude.ItemMask |= FighterFlag;
    //dude.ItemMask |= ClericFlag;
    min1 = GetMaxClericExpForLevel(cch.clericLvl);
    min2 = GetMaxFighterExpForLevel(cch.fighterLvl);
    break;
  case 9:
    //dude.SetClass(C_F_MU);
    SetClass("C_F_MU", dude);
    //dude.ItemMask |= FighterFlag;
    //dude.ItemMask |= MagicUserFlag;
    //dude.ItemMask |= ClericFlag;
    min1 = GetMaxClericExpForLevel(cch.clericLvl);
    min2 = GetMaxFighterExpForLevel(cch.fighterLvl);
    min3 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    break;
  case 10:
    //dude.SetClass(C_R);
    SetClass("C_R", dude);
    //dude.ItemMask |= ClericFlag;
    //dude.ItemMask |= RangerFlag;
    min1 = GetMaxClericExpForLevel(cch.clericLvl);
    min2 = GetMaxRangerExpForLevel(cch.rangerLvl);
    break;
  case 11:
    //dude.SetClass(C_MU);
    SetClass("C_MU", dude);
    //dude.ItemMask |= ClericFlag;
    //dude.ItemMask |= MagicUserFlag;
    min1 = GetMaxClericExpForLevel(cch.clericLvl);
    min2 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    break;
  case 12:
    //dude.SetClass(C_T);
    SetClass("C_T", dude);
    //dude.ItemMask |= ClericFlag;
    //dude.ItemMask |= ThiefFlag;
    min1 = GetMaxClericExpForLevel(cch.clericLvl);
    min2 = GetMaxThiefExpForLevel(cch.thiefLvl);
    break;
  case 13:
    //dude.SetClass(F_MU);
    SetClass("F_MU", dude);
    //dude.ItemMask |= FighterFlag;
    //dude.ItemMask |= MagicUserFlag;
    min1 = GetMaxFighterExpForLevel(cch.fighterLvl);
    min2 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    break;
  case 14:
    //dude.SetClass(F_T);
    SetClass("F_T", dude);
    //dude.ItemMask |= FighterFlag;
    //dude.ItemMask |= ThiefFlag;
    min1 = GetMaxFighterExpForLevel(cch.fighterLvl);
    min2 = GetMaxThiefExpForLevel(cch.thiefLvl);
    break;
  case 15:
    //dude.SetClass(F_MU_T);
    SetClass("F_MU_T", dude);
    //dude.ItemMask |= FighterFlag;
    //dude.ItemMask |= ThiefFlag;
    //dude.ItemMask |= MagicUserFlag;
    min1 = GetMaxFighterExpForLevel(cch.fighterLvl);
    min2 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    min3 = GetMaxThiefExpForLevel(cch.thiefLvl);
    break;
  case 16:
    //dude.SetClass(MU_T);
    SetClass("MU_T", dude);
    //dude.ItemMask |= ThiefFlag;
    //dude.ItemMask |= MagicUserFlag;
    min1 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    min2 = GetMaxThiefExpForLevel(cch.thiefLvl);
    break;
  case 17:
    //dude.SetClass(F_D); // last three not really there
    SetClass("F_D", dude); // last three not really there
    //dude.ItemMask |= FighterFlag;
    min1 = GetMaxDruidExpForLevel(cch.knightLvl);
    min2 = GetMaxFighterExpForLevel(cch.fighterLvl);
    break;
  case 18:
    //dude.SetClass(D_MU);
    SetClass("D_MU", dude);
    //dude.ItemMask |= MagicUserFlag;
    //dude.ItemMask |= DruidFlag;
    min1 = GetMaxDruidExpForLevel(cch.knightLvl);
    min2 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    break;
  case 19:
    //dude.SetClass(F_MU_D);
    SetClass("F_MU_D", dude);
    //dude.ItemMask |= DruidFlag;
    //dude.ItemMask |= FighterFlag;
    //dude.ItemMask |= MagicUserFlag;
    min1 = GetMaxDruidExpForLevel(cch.knightLvl);
    min2 = GetMaxFighterExpForLevel(cch.fighterLvl);
    min3 = GetMaxMagicUserExpForLevel(cch.mageLvl);
    break;
  }
  
  dude.SetPermStr(cch.str);
  dude.SetPermStrMod(cch.strExt);
  dude.SetPermInt(cch.intel);
  dude.SetPermWis(cch.wis);
  dude.SetPermDex(cch.dex);
  dude.SetPermCon(cch.con);
  dude.SetPermCha(cch.cha);

  BYTE size = cch.size & 0x7F;
  BYTE forceLarge = cch.size & 0x80;

  if (!forceLarge)
  {
    switch (size)
    {
    case 1:
    case 2:
    case 3:
      dude.SetSize(Medium);
      break;
    default:
      dude.SetSize(Large);
      break;
    }
  }
  else
    dude.SetSize(Large);

  dude.SetEncumbrance(SwapTwoBytes(cch.encumb));
  dude.SetGender((genderType)cch.gender);
  dude.SetHitPoints(cch.adjHp);
  dude.IsPreGen = FALSE;  
  //if (cch.itemMask & 1)
  //  dude.ItemMask |= MagicUserFlag;
  //if (cch.itemMask & 2)
  //  dude.ItemMask |= ClericFlag;
  //if (cch.itemMask & 4)
  //  dude.ItemMask |= ThiefFlag;
  //if (cch.itemMask & 8)
  //  dude.ItemMask |= FighterFlag;
  //if (cch.itemMask & 16)
  //  dude.ItemMask |= FighterFlag;
  //if (cch.itemMask & 32)
  //  dude.ItemMask |= FighterFlag;
  //if (cch.itemMask & 64)
  //  dude.ItemMask |= (PaladinFlag | RangerFlag);
  //if (cch.itemMask & 128)
  //  dude.ItemMask |= FighterFlag;

  dude.SetMagicResistance(cch.magicResistance);
  dude.uniquePartyID = cch.uniquePartyID;
  dude.DetermineCharMaxAge();
  dude.SetMaxCureDisease(cch.cureDisease);  
  dude.SetMaxHitPoints(cch.maxHp);
  dude.SetMaxMovement(cch.move);
  dude.SetMorale((cch.morale & 0x7F));
  if (dude.GetMorale() == 0)
    dude.SetMorale(50);
  dude.SetName((LPCSTR)cch.name);
  {
    RACE_ID raceID;
    switch (cch.race)
    {
    case 0:
      raceID = "Elf";
      //dude.race(Elf);
      break;
    case 1:
      raceID = "HalfElf";
      //dude.race(HalfElf);
      break;
    case 2:
      raceID = "Dwarf";
      //dude.race(Dwarf);
      break;
    case 3:
      raceID = "Gnome";
      //dude.race(Gnome);
      break;
    case 4:
      raceID = "Halfling";
      //dude.race(Halfling);
      break;
    case 5:
      raceID = "Human";
      //dude.race(Human);
      break;
    default:
      raceID = "Unknown";
      //dude.race(raceUnknown); // monster
      break;
    }
    dude.RaceID(raceID);
  };
  
  dude.SetReadyToTrain(cch.rdyToTrain);

  switch (cch.status)
  {
  case 0:
    dude.SetStatus(Okay);
    break;
  case 1:
    dude.SetStatus(Animated);
    break;
  case 2:
    dude.SetStatus(TempGone);
    break;
  case 3:
    dude.SetStatus(Running);
    break;
  case 4:
    dude.SetStatus(Unconscious);
    break;
  case 5:
    dude.SetStatus(Dying);
    break;
  case 6:
    dude.SetStatus(Dead);
    break;
  case 7:
    dude.SetStatus(Petrified);
    break;
  case 8:
    dude.SetStatus(Gone);
    break;
  }

  dude.SetTHAC0(60 - cch.thac0);  
  switch (cch.undead)
  {
  case 0:
    dude.SetUndead(UndeadTypeText[NotUndead]);
    break;
  case 1:
    dude.SetUndead(UndeadTypeText[Skeleton]);
    break;
  case 2:
    dude.SetUndead(UndeadTypeText[Zombie]);
    break;
  case 3:
    dude.SetUndead(UndeadTypeText[Ghoul]);
    break;
  case 4:
    dude.SetUndead(UndeadTypeText[Shadow]);
    break;
  case 5:
    dude.SetUndead(UndeadTypeText[Wight]);
    break;
  case 6:
    dude.SetUndead(UndeadTypeText[Ghast]);
    break;
  case 7:
    dude.SetUndead(UndeadTypeText[Wraith]);
    break;
  case 8:
    dude.SetUndead(UndeadTypeText[Mummy]);
    break;
  case 9:
    dude.SetUndead(UndeadTypeText[Spectre]);
    break;
  case 10:
    dude.SetUndead(UndeadTypeText[Vampire]);
    break;
  case 11:
    dude.SetUndead(UndeadTypeText[Ghost]);
    break;
  case 12:
    dude.SetUndead(UndeadTypeText[Lich]);
    break;
  case 13:
    dude.SetUndead(UndeadTypeText[Special]);
    break;
  }

  dude.unarmedBonus = SwapTwoBytes(cch.dmgBonus);
  dude.unarmedDieL = SwapTwoBytes(cch.dmgDiceSides);
  dude.unarmedNbrDieL = SwapTwoBytes(cch.dmgDiceNbr);
  dude.unarmedDieS = SwapTwoBytes(cch.dmgDiceSides);
  dude.unarmedNbrDieS = SwapTwoBytes(cch.dmgDiceNbr);
  
  NotImplemented(0x54ac1,false);

  NotImplemented(0x64a9a, false); // Put skills in baseclass???

  dude.SetNbrAttacks( ((double)SwapTwoBytes(cch.attacksPer2Rnds)) / 2.0);
  dude.SetNbrHD((double)(cch.level <= 0 ? 0.5 : cch.level));

	SetLevels(dude, "Fighter",   cch.fighterLvl, cch.fighterLvlPreClassChng, cch.fighterLvlPreDrain);
  SetLevels(dude, "Cleric",    cch.clericLvl,  cch.clericLvlPreClassChng,  cch.clericLvlPreDrain);
  SetLevels(dude, "Ranger",    cch.rangerLvl,  cch.rangerLvlPreClassChng,  cch.rangerLvlPreDrain);
  SetLevels(dude, "Paladin",   cch.paladinLvl, cch.paladinLvlPreClassChng, cch.paladinLvlPreDrain);
  SetLevels(dude, "MagicUser", cch.mageLvl,    cch.mageLvlPreClassChng,    cch.mageLvlPreDrain);
  SetLevels(dude, "Thief",     cch.thiefLvl,   cch.thiefLvlPreClassChng,   cch.thiefLvlPreDrain);
  SetLevels(dude, "Druid",     0,              0,                          0);




  dude.money.Add(dude.money.GetDefaultType(), SwapTwoBytes(cch.platinum));
  for (int g=0;g<SwapTwoBytes(cch.gems);g++)
    dude.money.AddGem();
  for (int j=0;j<SwapTwoBytes(cch.jewelry);j++)
    dude.money.AddJewelry();

  if (min1 > 500)
    min1 -= 500;
  if (min2 > 500)
    min2 -= 500;
  if (min3 > 500)
    min3 -= 500;

  int exp = SwapFourBytes(cch.exp);
  int exp2 = SwapFourBytes(cch.expPreDrain);
  exp = max(exp, exp2);
  if (exp == 0)
    exp = min1+min2+min3;
  dude.giveCharacterExperience(exp);

  int hb,db;
  BYTE od,omd,bl;
  determineStrengthProperties(dude.GetPermStr(), dude.GetPermStrMod(),
                              hb, db, 
                              od, omd, bl); 
  dude.SetHitBonus(hb);
  dude.SetDmgBonus(db);
  NotImplemented(0x74e09, false); // Put in baseclass??
  dude.SetMaxEncumbrance(dude.determineMaxEncumbrance()); //cch.encumb;
  dude.SetEncumbrance(dude.determineEffectiveEncumbrance());

#ifdef SIMPLE_STRUCTURE
  dude.icon.filename = ede.TemplateIconArtDir() + DEFAULT_NPC_ICON;
#else
  dude.icon.filename.Format("%s%s",EditorArt, DEFAULT_NPC_ICON);
#endif
  dude.icon.picType = IconDib;
  dude.icon.SetDefaults();
#ifdef SIMPLE_STRUCTURE
  dude.smallPic.filename = ede.TemplateSmallPicDir() + DEFAULT_NPC_PIC;
#else
  dude.smallPic.filename.Format("%s%s",EditorArt, DEFAULT_NPC_PIC);
#endif
  dude.smallPic.picType = SmallPicDib;
  dude.smallPic.SetDefaults();

  ITEM idata;
  for (int i=0;i<16;i++)
  {
    if (AssignItem(cch.itemCarried[i], idata, FALSE))
    {
      if (cch.itemQty[i] > 0)
        //idata.qty = cch.itemQty[i]*itemData.GetItemBundleQty(idata.m_giID);
        idata.qty = cch.itemQty[i]*itemData.GetItemBundleQty(idata.itemID);
      if (idata.qty > 0)
        dude.myItems.AddItem(idata);
    }
  }
}

void ProcessMonsterCchData(ImportUACCH &cch, MONSTER_DATA &dude)
{
  WriteDebugString("Importing monster \'%s\'\n", cch.name);
  dude.Armor_Class = 60 - cch.ac;
  dude.Name = cch.name;

#ifdef SIMPLE_STRUCTURE
  dude.Icon.filename = ede.TemplateIconArtDir() + DEFAULT_MONSTER_ICON;
  dude.Icon.picType = IconDib;
  dude.Icon.SetDefaults();
  dude.HitSound = ede.TemplateSoundDir() + DEF_HIT_SOUND;
  dude.MissSound = ede.TemplateSoundDir() + DEF_MISS_SOUND;
#else
  dude.Icon.filename.Format("%s%s",EditorArt, DEFAULT_MONSTER_ICON);
  dude.Icon.picType = IconDib;
  dude.Icon.SetDefaults();
  dude.HitSound.Format("%s%s",EditorArt, DEF_HIT_SOUND);
  dude.MissSound.Format("%s%s",EditorArt, DEF_MISS_SOUND);
#endif

  dude.Intelligence = cch.intel;
  dude.Movement = cch.move;
  dude.Hit_Dice = (float) (cch.level <= 0 ? 0.5 : cch.level);	
  dude.UseHitDice = TRUE;
	//long Hit_Dice_Bonus;
  dude.THAC0 = 60 - cch.thac0;

  int Dmg_Dice_Bonus = SwapTwoBytes(cch.dmgBonus);
  int Dmg_Dice_for_Attack = SwapTwoBytes(cch.dmgDiceSides);
  int Nbr_Dmg_Dice = SwapTwoBytes(cch.dmgDiceNbr);	
	int Nbr_Attacks = SwapTwoBytes(cch.attacksPer2Rnds) / 2;
  if (Nbr_Attacks <= 0) Nbr_Attacks=1;

  dude.attackData.Clear();
  MONSTER_ATTACK_DETAILS tmp;
  tmp.sides = Dmg_Dice_for_Attack;
  tmp.nbr = Nbr_Dmg_Dice;
  tmp.bonus = Dmg_Dice_Bonus;
  tmp.attackMsg = "attacks";
  for (int cnt=0;cnt<Nbr_Attacks;cnt++)
    dude.attackData.Add(tmp);

  dude.Magic_Resistance = cch.magicResistance;

  dude.Morale = (cch.morale & 0x7F);
  if (dude.Morale == 0)
    dude.Morale = 50;

  dude.XP_Value = SwapTwoBytes(cch.expWorth);
	
  BYTE size = cch.size & 0x7F;
  BYTE forceLarge = cch.size & 0x80;

  switch (size)
  {
  case 1:
  case 2:
  case 3:
    dude.Size = Medium;
    break;
  default:
    dude.Size = Large;
    break;
  }

/*
  dude.ItemMask = 0;

  switch (cch.charClass)
  {
  case 0:
    dude.ItemMask |= ClericFlag;
    break;
  case 1:
    dude.ItemMask |= DruidFlag;
    break;
  case 2:
    dude.ItemMask |= FighterFlag;
    break;
  case 3:
    dude.ItemMask |= PaladinFlag;
    break;
  case 4:
    dude.ItemMask |= RangerFlag;
    break;
  case 5:
    dude.ItemMask |= MagicUserFlag;
    break;
  case 6:
    dude.ItemMask |= ThiefFlag;
    break;
  case 7:
    dude.ItemMask |= FighterFlag;
    break;
  case 8:
    dude.ItemMask |= FighterFlag;
    dude.ItemMask |= ClericFlag;
    break;
  case 9:
    dude.ItemMask |= FighterFlag;
    dude.ItemMask |= MagicUserFlag;
    dude.ItemMask |= ClericFlag;
    break;
  case 10:
    dude.ItemMask |= ClericFlag;
    dude.ItemMask |= RangerFlag;
    break;
  case 11:
    dude.ItemMask |= ClericFlag;
    dude.ItemMask |= MagicUserFlag;
    break;
  case 12:
    dude.ItemMask |= ClericFlag;
    dude.ItemMask |= ThiefFlag;
    break;
  case 13:
    dude.ItemMask |= FighterFlag;
    dude.ItemMask |= MagicUserFlag;
    break;
  case 14:
    dude.ItemMask |= FighterFlag;
    dude.ItemMask |= ThiefFlag;
    break;
  case 15:
    dude.ItemMask |= FighterFlag;
    dude.ItemMask |= ThiefFlag;
    dude.ItemMask |= MagicUserFlag;
    break;
  case 16:
    dude.ItemMask |= ThiefFlag;
    dude.ItemMask |= MagicUserFlag;
    break;
  case 17:
    dude.ItemMask |= FighterFlag;
    break;
  case 18:
    dude.ItemMask |= MagicUserFlag;
    dude.ItemMask |= DruidFlag;
    break;
  case 19:
    dude.ItemMask |= DruidFlag;
    dude.ItemMask |= FighterFlag;
    dude.ItemMask |= MagicUserFlag;
    break;
  }
*/
  /*
  if (cch.itemMask & 1)
    dude.ItemMask |= MagicUserFlag;
  if (cch.itemMask & 2)
    dude.ItemMask |= ClericFlag;
  if (cch.itemMask & 4)
    dude.ItemMask |= ThiefFlag;
  if (cch.itemMask & 8)
    dude.ItemMask |= FighterFlag;
  if (cch.itemMask & 16)
    dude.ItemMask |= FighterFlag;
  if (cch.itemMask & 32)
    dude.ItemMask |= FighterFlag;
  if (cch.itemMask & 64)
    dude.ItemMask |= (PaladinFlag | RangerFlag);
  if (cch.itemMask & 128)
    dude.ItemMask |= FighterFlag;
  if (dude.ItemMask==0) dude.ItemMask=FighterFlag;
*/

  //SPECIAL_ABILITIES specAbs;

  if (cch.specAbFlags & 1)
    dude.MiscOptions_Type |= OptionAffectedByDispelEvil;
  if (cch.specAbFlags & 2)
    dude.Form_Type |= FormMammal;
  if (cch.specAbFlags & 4)
    dude.Penalty_Type |= PenaltyDwarfAC;
  if (cch.specAbFlags & 8)
    dude.Penalty_Type |= PenaltyRangerDmg;
  if (cch.specAbFlags & 16)
    dude.Form_Type |= FormSnake;
  if (cch.specAbFlags & 32)
    dude.Penalty_Type |= PenaltyGnomeAC;
  if (cch.specAbFlags & 64)
    dude.Form_Type |= FormAnimal;
  if (cch.specAbFlags & 128)
    dude.Penalty_Type |= PenaltyDwarfTHAC0;

  if (cch.specAbFlags2 & 1)
    dude.Form_Type |= FormGiant;
  if (cch.specAbFlags2 & 2)
    dude.MiscOptions_Type |= OptionCanBeHeldCharmed;
  if (cch.specAbFlags2 & 4)
    dude.Penalty_Type |= PenaltyGnomeTHAC0;
  if (cch.specAbFlags2 & 8)
    dude.Immunity_Type |= ImmuneDeath;
  if (cch.specAbFlags2 & 16)
    dude.Immunity_Type |= ImmunePoison;
  if (cch.specAbFlags2 & 32)
    dude.Immunity_Type |= ImmuneVorpal;
  if (cch.specAbFlags2 & 64)
    dude.Immunity_Type |= ImmuneConfusion;

  if (forceLarge)
    dude.Form_Type |= FormLarge;
  //if (cch.undead > 0)
  //  dude.Form_Type |= FormUndead;

  ITEM idata;
  for (int i=0;i<16;i++)
  {
    if (AssignItem(cch.itemCarried[i], idata, FALSE))
    {
      if (cch.itemQty[i] > 0)
        //idata.qty = cch.itemQty[i]*itemData.GetItemBundleQty(idata.m_giID);
        idata.qty = cch.itemQty[i]*itemData.GetItemBundleQty(idata.itemID);
      if (idata.qty > 0)
        dude.myItems.AddItem(idata);
    }
  }

  dude.GuessUndeadStatus();
}

BOOL ImportMonsterToUAF(const char *path)
{
  BOOL success = TRUE;
  FILE *fp;
  ImportUACCH cch;
  //CHARACTER dude;

  //WriteDebugString("sizeof CCH: %i\n", sizeof(cch));
  //
  // read cch information
  //
  WriteDebugString("Opening %s\n", path);
  fp = fopen(path, "rb");
  if(fp == NULL)
  {
      WriteDebugString("Error opening %s!\n", path);
      return FALSE;
  }
  if (fread(&cch, sizeof(cch), 1, fp) != 1)
  {
    fclose(fp);    
    WriteDebugString("Failed to read character data\n");
    return FALSE;
  }

  fclose(fp);

  if ((cch.race == 6) && (cch.combatMode == 1))
  {
    MONSTER_DATA mdude;
    mdude.Clear();
    ProcessMonsterCchData(cch, mdude);
    Monster_Keys[cch.monsterIndex] = monsterData.AddMonster(&mdude);
    Monster_Type[cch.monsterIndex] = MONSTER_TYPE;
    WriteDebugString("Monster \'%s\' added to key index %i\n", mdude.Name, cch.monsterIndex);
    ImportMonsterCount++;
  }
  else
  {
    CHARACTER dude;
    dude.Clear();
    ProcessNpcCchData(cch, dude);
    // if not an NPC, monsterIndex = 0.
    //Monster_Keys[cch.monsterIndex] = globalData.charData.AddCharacter(&dude);
    globalData.charData.AddCharacter(&dude);
    Monster_Keys[cch.monsterIndex] = dude.CharacterID();
    Monster_Type[cch.monsterIndex] = NPC_TYPE;
    WriteDebugString("NPC \'%s\' added to key index %i\n", dude.GetName(), cch.monsterIndex);
    ImportNPCCount++;
  }

  return success;
}

static void PumpWinMsgs()
{
  const int MAX_WIN_MSGS = 30;
  int count=0;
  MSG msg;
  while ((::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) && (count<MAX_WIN_MSGS))
  {
    if (GetMessage(&msg, NULL, 0, 0) > 0)
    {
       ::TranslateMessage(&msg);
       ::DispatchMessage(&msg);
    }
    count++;
  }
  AfxGetApp()->DoWaitCursor(1);
}

// restore default packing alignment
#include <poppack.h>