/******************************************************************************
* Filename: FRUAStringTables.cpp
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
#include "FRUAStringTables.h"
#include "FRUAStructs.h" // to get definitions for limits

#define PICTYPE_NONE       0
#define PICTYPE_SPRITE     1
#define PICTYPE_BIGPIC     2
#define PICTYPE_SMALLPIC_A 3
#define PICTYPE_SMALLPIC_B 4
#define PICTYPE_SMALLPIC_C 5
#define PICTYPE_SMALLPIC_D 6
#define PICTYPE_SMALLPIC_E 7
#define PICTYPE_SMALLPIC_F 8

struct SPicInfo
{
   const char *pString;
   int picNum;
   int picType;
};

const SPicInfo BigSmallSpritePics[] =
{
   { "none", 0, PICTYPE_NONE },
   { "Kobold", 1, PICTYPE_SPRITE },
   { "Goblin", 2, PICTYPE_SMALLPIC_A },
   { "Orc", 3, PICTYPE_SMALLPIC_A },
   { "Hobgoblin", 4, PICTYPE_SMALLPIC_A },
   { "Gnoll", 7, PICTYPE_SMALLPIC_A },
   { "Ogre", 8, PICTYPE_SMALLPIC_A },
   { "Troll", 9, PICTYPE_SMALLPIC_A },
   { "Hill Giant", 10, PICTYPE_SPRITE },
   { "Fire Giant", 11, PICTYPE_SPRITE },
   { "Cloud Giant", 13, PICTYPE_SPRITE },
   { "Carrion Crawler", 16, PICTYPE_SMALLPIC_A },
   { "Enormous Spider", 17, PICTYPE_SMALLPIC_A },
   { "Zombie", 19, PICTYPE_SPRITE },
   { "Ghoul", 20, PICTYPE_SMALLPIC_A },
   { "Wight", 22, PICTYPE_SPRITE },
   { "Spectre", 25, PICTYPE_SPRITE },
   { "Vampire", 26, PICTYPE_SMALLPIC_A },
   { "Lich", 27, PICTYPE_SMALLPIC_A },
   { "Lizard Man", 28, PICTYPE_SMALLPIC_A },
   { "Minotaur", 30, PICTYPE_SMALLPIC_A },
   { "Boring Beetle", 32, PICTYPE_SPRITE },
   { "Griffon", 33, PICTYPE_SMALLPIC_A },
   { "Hydra", 34, PICTYPE_SMALLPIC_A },
   { "Black Dragon", 36, PICTYPE_SPRITE },
   { "Blue Dragon", 37, PICTYPE_SPRITE },
   { "Green Dragon", 38, PICTYPE_SPRITE },
   { "Red Dragon", 39, PICTYPE_SMALLPIC_A },
   { "White Dragon", 40, PICTYPE_SPRITE },
   { "Dracolich", 41, PICTYPE_SMALLPIC_A },
   { "Basilisk", 42, PICTYPE_SPRITE },
   { "Gorgon", 43, PICTYPE_SPRITE },
   { "Beholder", 45, PICTYPE_SPRITE },
   { "Ogre Mage", 46, PICTYPE_SMALLPIC_A },
   { "Bulette", 47, PICTYPE_SMALLPIC_A },
   { "Shambling Mound", 48, PICTYPE_SPRITE },
   { "Margoyle", 49, PICTYPE_SMALLPIC_A },
   { "Mobat", 51, PICTYPE_SPRITE },
   { "Black Pudding", 52, PICTYPE_SPRITE },
   { "Otyugh", 53, PICTYPE_SPRITE },
   { "Salamander", 55, PICTYPE_SPRITE },
   { "Earth Elemental", 57, PICTYPE_SMALLPIC_A },
   { "Fire Elemental", 58, PICTYPE_SMALLPIC_A },
   { "Umber Hulk", 59, PICTYPE_SPRITE },
   { "Ettin", 60, PICTYPE_SPRITE },
   { "Medusa", 63, PICTYPE_SPRITE },
   { "Giant Spider", 64, PICTYPE_SPRITE },
   { "Poisonous Snake", 66, PICTYPE_SPRITE },
   { "Hell Hound", 67, PICTYPE_SMALLPIC_A },
   { "Drider", 69, PICTYPE_SPRITE },
   { "Iron Golem", 70, PICTYPE_SMALLPIC_A },
   { "Rakshasa", 71, PICTYPE_SMALLPIC_A },
   { "Purple Worm", 72, PICTYPE_SPRITE },
   { "Drow Champion", 73, PICTYPE_SMALLPIC_A },
   { "Drow Sorceress", 75, PICTYPE_SMALLPIC_A },
   { "Warrior", 77, PICTYPE_SPRITE },
   { "Goon", 80, PICTYPE_SMALLPIC_B },
   { "Archer", 81, PICTYPE_SMALLPIC_B },
   { "Theurgist", 82, PICTYPE_SPRITE },
   { "Evil Champion", 85, PICTYPE_SPRITE },
   { "Magician", 86, PICTYPE_SPRITE },
   { "Dark Cleric", 87, PICTYPE_SMALLPIC_B },
   { "Rogue", 88, PICTYPE_SMALLPIC_B },
   { "Dark Knight", 89, PICTYPE_SPRITE },
   { "Necromancer", 90, PICTYPE_SPRITE },
   { "High Priest", 91, PICTYPE_SPRITE },
   { "Thief", 92, PICTYPE_SMALLPIC_B },
   { "Dark Warlord", 93, PICTYPE_SPRITE },
   { "Wizard", 94, PICTYPE_SMALLPIC_B },
   { "Master Thief", 96, PICTYPE_SPRITE },
   { "Master Wizard", 98, PICTYPE_SMALLPIC_B },
   { "High Thief", 100, PICTYPE_SPRITE },
   { "Vampire Lord", 101, PICTYPE_SMALLPIC_B },
   { "Vampiress", 102, PICTYPE_SMALLPIC_B },
   { "Dazmilar", 103, PICTYPE_SMALLPIC_B },
   { "Sir Dutiocs", 105, PICTYPE_SMALLPIC_B },
   { "Vidruand", 106, PICTYPE_SMALLPIC_B },
   { "Drow Amazon", 109, PICTYPE_SMALLPIC_B },
   { "Kallithrea", 112, PICTYPE_SMALLPIC_B },
   { "Yemandra", 113, PICTYPE_SMALLPIC_B },
   { "Alias", 117, PICTYPE_SMALLPIC_B },
   { "Dragonbait", 118, PICTYPE_SMALLPIC_B },
   { "Nacacia", 119, PICTYPE_SMALLPIC_B },
   { "Priam", 120, PICTYPE_SMALLPIC_B },
   { "Vala", 121, PICTYPE_SMALLPIC_B },
   { "Silk", 122, PICTYPE_SMALLPIC_B },
   { "Captain Daenor", 123, PICTYPE_SMALLPIC_B },
   { "Grunschka", 124, PICTYPE_SMALLPIC_B },
   { "Shal", 126, PICTYPE_SMALLPIC_B },
   { "Camp 2", 128, PICTYPE_SMALLPIC_B },
   { "Treasure 1", 129, PICTYPE_SMALLPIC_B },
   { "Camp 1", 130, PICTYPE_SMALLPIC_B },
   { "Treasure 2", 131, PICTYPE_SMALLPIC_B },
   { "Tower 4", 132, PICTYPE_SMALLPIC_B },
   { "Baldric", 133, PICTYPE_SMALLPIC_B },
   { "Bartender", 134, PICTYPE_SMALLPIC_B },
   { "Bit O' Moander", 135, PICTYPE_SMALLPIC_B },
   { "Bjorn", 136, PICTYPE_SMALLPIC_B },
   { "Gatekeeper", 137, PICTYPE_SMALLPIC_B },
   { "Town 3", 138, PICTYPE_SMALLPIC_C },
   { "Town 2", 139, PICTYPE_SMALLPIC_C },
   { "Town 1", 140, PICTYPE_SMALLPIC_C },
   { "Crysia", 141, PICTYPE_SMALLPIC_C },
   { "Ring", 142, PICTYPE_SMALLPIC_C },
   { "Lich and Bride", 143, PICTYPE_SMALLPIC_C },
   { "Ship 3", 144, PICTYPE_SMALLPIC_C },
   { "Morudel", 145, PICTYPE_SMALLPIC_C },
   { "Dragon Duel", 146, PICTYPE_SMALLPIC_C },
   { "Lich Noble", 147, PICTYPE_SMALLPIC_C },
   { "Sea Dragon", 148, PICTYPE_SMALLPIC_C },
   { "Sir Ftucis", 149, PICTYPE_SMALLPIC_C },
   { "Lunit Bdufe", 150, PICTYPE_SMALLPIC_C },
   { "Drow to Drider", 151, PICTYPE_SMALLPIC_C },
   { "Dwarf Cleric", 152, PICTYPE_SMALLPIC_C },
   { "Elastul", 153, PICTYPE_SMALLPIC_C },
   { "Elea", 154, PICTYPE_SMALLPIC_C },
   { "Elminster", 155, PICTYPE_SMALLPIC_C },
   { "Eshalla the Hag", 156, PICTYPE_SMALLPIC_C },
   { "Eshalla", 157, PICTYPE_SMALLPIC_C },
   { "Ezra", 158, PICTYPE_SMALLPIC_C },
   { "Fastillion", 159, PICTYPE_SMALLPIC_C },
   { "Fatman", 160, PICTYPE_SMALLPIC_C },
   { "Sad Dragon", 161, PICTYPE_SMALLPIC_C },
   { "Gateway 1", 162, PICTYPE_SMALLPIC_C },
   { "Gauhok", 163, PICTYPE_SMALLPIC_C },
   { "Maiden", 164, PICTYPE_SMALLPIC_D },
   { "Mage", 165, PICTYPE_SMALLPIC_D },
   { "Gheildar", 166, PICTYPE_SMALLPIC_D },
   { "Dwarf King", 167, PICTYPE_SMALLPIC_D },
   { "Tanar'ri, Balor", 168, PICTYPE_SMALLPIC_D },
   { "Barbarian", 169, PICTYPE_SMALLPIC_D },
   { "Horn", 170, PICTYPE_SMALLPIC_D },
   { "Icehunt", 171, PICTYPE_SMALLPIC_D },
   { "Innkeeper", 172, PICTYPE_SMALLPIC_D },
   { "Iron Wall 2", 173, PICTYPE_SMALLPIC_D },
   { "Jabarkus", 174, PICTYPE_SMALLPIC_D },
   { "Jagaerda", 175, PICTYPE_SMALLPIC_D },
   { "Tanar'ri, Mari", 176, PICTYPE_SMALLPIC_D },
   { "Moonshae Royals", 177, PICTYPE_SMALLPIC_D },
   { "Limbo", 178, PICTYPE_SMALLPIC_D },
   { "Tower 3", 179, PICTYPE_SMALLPIC_D },
   { "Lyzian", 180, PICTYPE_SMALLPIC_D },
   { "Malchor", 181, PICTYPE_SMALLPIC_D },
   { "Marcus", 182, PICTYPE_SMALLPIC_D },
   { "Evil Spirit", 183, PICTYPE_SMALLPIC_D },
   { "Amulet", 184, PICTYPE_SMALLPIC_D },
   { "Northman", 185, PICTYPE_SMALLPIC_D },
   { "Figurine", 186, PICTYPE_SMALLPIC_D },
   { "Town 4", 187, PICTYPE_SMALLPIC_D },
   { "Wand", 188, PICTYPE_SMALLPIC_D },
   { "Book", 189, PICTYPE_SMALLPIC_D },
   { "Orb", 190, PICTYPE_SMALLPIC_D },
   { "Rolf", 191, PICTYPE_SMALLPIC_D },
   { "Ship 2", 192, PICTYPE_SMALLPIC_D },
   { "Sasha", 193, PICTYPE_SMALLPIC_E },
   { "Maison Thorvald", 194, PICTYPE_SMALLPIC_E },
   { "Sapphire", 195, PICTYPE_SMALLPIC_E },
   { "Goblin King", 196, PICTYPE_SMALLPIC_E },
   { "Hammer", 197, PICTYPE_SMALLPIC_E },
   { "Dark Swordsman", 198, PICTYPE_SMALLPIC_E },
   { "Watchwoman", 199, PICTYPE_SMALLPIC_E },
   { "Jovena", 200, PICTYPE_SMALLPIC_E },
   { "Skeleton Horde", 201, PICTYPE_SMALLPIC_E },
   { "Fejyelsae", 202, PICTYPE_SMALLPIC_E },
   { "The Searchers", 203, PICTYPE_SMALLPIC_E },
   { "Mordenkainen", 204, PICTYPE_SMALLPIC_E },
   { "Mardus", 205, PICTYPE_SMALLPIC_E },
   { "Tanar'ri, Nalf", 206, PICTYPE_SMALLPIC_E },
   { "Tower 2", 207, PICTYPE_SMALLPIC_E },
   { "Selias", 208, PICTYPE_SMALLPIC_E },
   { "Ship 1", 209, PICTYPE_SMALLPIC_E },
   { "Shopkeeper", 210, PICTYPE_SMALLPIC_E },
   { "Crone", 211, PICTYPE_SMALLPIC_E },
   { "Blade", 212, PICTYPE_SMALLPIC_E },
   { "Scroll", 213, PICTYPE_SMALLPIC_E },
   { "Steelfist", 214, PICTYPE_SMALLPIC_E },
   { "Curio Dealer", 215, PICTYPE_SMALLPIC_E },
   { "Sulajia", 216, PICTYPE_SMALLPIC_E },
   { "Talisman", 217, PICTYPE_SMALLPIC_E },
   { "Barmaid", 218, PICTYPE_SMALLPIC_E },
   { "Gateway 2", 219, PICTYPE_SMALLPIC_E },
   { "Temple Priest", 220, PICTYPE_SMALLPIC_E },
   { "Gateway Doom", 221, PICTYPE_SMALLPIC_E },
   { "Spectral Minion", 222, PICTYPE_SMALLPIC_E },
   { "Chaotic Warrior", 223, PICTYPE_SMALLPIC_E},
   { "Ancient Red Dragon", 224, PICTYPE_SMALLPIC_E },
   { "Tower 1", 225, PICTYPE_SMALLPIC_E },
   { "Arms Master", 226, PICTYPE_SMALLPIC_E },
   { "Iron Wall", 227, PICTYPE_SMALLPIC_F },
   { "Tanar'ri, Glab", 228, PICTYPE_SMALLPIC_F },
   { "Sea Elf", 229, PICTYPE_SMALLPIC_F },
   { "Dolphin", 230, PICTYPE_SMALLPIC_F },
   { "Camp 3", 231, PICTYPE_SMALLPIC_F },
   { "Western Cormyr", 240, PICTYPE_BIGPIC },
   { "Moonsea Realms", 241, PICTYPE_BIGPIC },
   { "Web Realm", 242, PICTYPE_BIGPIC },
   { "Southern Ksoh", 243, PICTYPE_BIGPIC },
   { "White Worm", 244, PICTYPE_BIGPIC },
   { "Walking Forest", 245, PICTYPE_BIGPIC },
   { "Phlan Deluge", 246, PICTYPE_BIGPIC },
   { "Dread Gate", 247, PICTYPE_BIGPIC },
   { "Sorcery Duel", 248, PICTYPE_BIGPIC },
   { "Sword vs. Staff", 249, PICTYPE_BIGPIC },
   { "Tower 5", 250, PICTYPE_BIGPIC },
   { "Mayday Gold", 251, PICTYPE_BIGPIC },
   { "Dalelands", 252, PICTYPE_BIGPIC },
   { "none", 255, PICTYPE_NONE }
};

#define NUM_BIGSMALLSPRITE_PICS  (sizeof(BigSmallSpritePics) / sizeof(SPicInfo))
int GetNumBigSmallSpritePics() { return NUM_BIGSMALLSPRITE_PICS; }

//
// vocabulary table to generate item descriptions
//

char *ImportFRUAItemVocab[MAX_FRUA_ITEM_VOCAB_STRINGS] = 
{
   "", 
   "Battle Axe", // 1
   "Hand Axe", 
   "Club", 
   "Dagger", 
   "Dart", 
   "Hammer", 
   "Javelin", 
   "Mace", 
   "Morning Star", 
   "Military Pick", // 10 
   "Awl Pike", 
   "Bolt", 
   "Scimitar", 
   "Spear",
   "Quarter Staff", 
   "Bastard Sword", 
   "Broad Sword", 
   "Long Sword", 
   "Short Sword", 
   "Two-Handed Sword", // 20
   "Trident", 
   "Composite Long Bow", 
   "Composite Short Bow",
   "Long Bow", 
   "Short Bow", 
   "Fine", 
   "Light Crossbow", 
   "Sling", 
   "Staff", 
   "Arrow", // 30
   "Leather", 
   "Ring", 
   "Scale", 
   "Chain", 
   "Banded", 
   "Plate", 
   "Shield", 
   "Cleric",
   "Scroll", 
   "Mage", // 40
   "Helm", 
   "Belt", 
   "Robe", 
   "Cloak", 
   "Boots", 
   "Ring", 
   "Mail",
   "Armor", 
   "of Protection", 
   "Bracers", // 50
   "Wand", 
   "Elixir", 
   "Potion", 
   "Youth", 
   "Ruby",
   "Boulder", 
   "Dragon Breath", 
   "Displacement", 
   "Eyes", 
   "Drow", // 60
   "Elfin Chain",
   "Ice Storm", 
   "Sapphire", 
   "Emerald", 
   "Wizardry", 
   "Hornet's Nest", 
   "Fire Resistance",
   "Stone", 
   "Good Luck", 
   "Flail", // 70
   "Halberd", 
   "Gauntlets", 
   "Periapt", 
   "Health",
   "Cursed", 
   "Blessed", 
   "Bundle of", 
   "Ogre Power", 
   "Girdle", 
   "Giant Strength", // 80
   "Mirror", 
   "Necklace", 
   "Dragon", 
   "vs Giants", 
   "vorpal", 
   "cold resistance",
   "Diamond", 
   "Lightning", 
   "Fireballs", 
   "of", // 90
   "Vulnerability", 
   "Speed",
   "Silver", 
   "Extra", 
   "Healing", 
   "Charming", 
   "Fear", 
   "Magic Missiles", 
   "Missiles",
   "1 Spell", // 100
   "2 Spells", 
   "3 Spells", 
   "Paralyzation", 
   "Invisibility",
   "Cute Yellow Canary", 
   "AC 10", 
   "AC 6", 
   "AC 4", 
   "AC 3", 
   "AC 2", // 110
   "+1", 
   "+2", 
   "+3", 
   "+4", 
   "+5", 
   "-1", 
   "-2", 
   "-3",
   "Electric Immunity", 
   "Gaze Resistance", // 120 
   "Spiritual", 
   "Gem", 
   "Jewelry", 
   "blinking", 
   "from evil",
   "Hoopak", 
   "Dragonlance", 
   "Striking", 
   "Disruption", 
   "Dragon Slayer", // 130
   "Foethumper",
   "Solamnic", 
   "Petrification", 
   "Free Action", 
   "Footman's", 
   "Red Mage", 
   "White Mage",
   "Quarrel", 
   "Cleric", // cleric? 
   "Magic-User Scroll", // 140
   "Amulet", 
   "Bag", 
   "Solamnic Plate", 
   "Proof vs Poision",
   "Elixir of Youth", 
   "Eyes of Charming", 
   "vs Reptiles", 
   "Frost Brand", 
   "Berserker", 
   "Olin's", // 150
   "White", 
   "Red", 
   "Blank"
};
/*
const char *ImportFRUASpellVocab[MAX_FRUA_SPELLS] =
{
   "Bless",
   "Curse",
   "Cure Light Wounds",
   "Cause Light Wounds",
   "Detect Magic",
   "Protection From Evil",
   "Protection from Good",
   "Resist Cold",
   "Burning Hands",
   "Charm Person",
   "Detect Magic",
   "Enlarge",
   "Reduce",
   "Friends",
   "Magic Missile",
   "Protection From Evil",
   "Protection From Good",
   "Read Magic",
   "Shield",
   "Shocking Grasp",
   "Sleep",
   "Find Traps",
   "Hold Person",
   "Resist Fire",
   "Silence, 15' Radius",
   "Slow Poison",
   "Snake Charm",
   "Spiritual Hammer",
   "Detect Invisibility",
   "Invisibility",
   "Knock",
   "Mirror Image",
   "Ray of Enfeeblement",
   "Stinking Cloud",
   "Strength",
   "Heal",
   "Cure Blindness",
   "Cause Blindness",
   "Cure Disease",
   "Cause Disease",
   "Dispel Magic",
   "Prayer",
   "Remove Curse",
   "Bestow Curse",
   "Blink",
   "Dispel Magic",
   "Fireball",
   "Haste",
   "Hold Person",
   "Invisibility, 10' Radius",
   "Lightning Bolt",
   "Prot.From Evil, 10' Radius",
   "Prot.From Good, 10' Radius",
   "Prot.From Normal Missiles",
   "Slow",
   "Harm",
   "Prot.From Evil, 10' Radius",
   "Cure Serious Wounds",
   "Silence 15' radius",
   "Detect Magic",
   "Remove Curse",
   "Bless",
   "Charm Person",
   "Burning Hands",
   "",
   "Cause Serious Wounds",
   "Neutralize Poison",
   "Poison",
   "Prot.From Evil, 10' Radius",
   "Sticks to Snakes",
   "Cure Critical Wounds",
   "Cause Critical Wounds",
   "Dispel Evil",
   "Flame Strike",
   "Raise Dead",
   "Slay Living",
   "Detect Magic",
   "Entangle",
   "Faerie Fire",
   "Invisibility to Animals",
   "Charm Monsters",
   "Confusion",
   "Dimension Door",
   "Fear",
   "Fire Shield",
   "Fumble",
   "Ice Storm",
   "Minor Globe Of Invulnerability",
   "Remove Curse",
   "Barkskin",
   "Cloud Kill",
   "Cone of Cold",
   "Feeblemind",
   "Hold Monsters",
   "Scroll of Protection",
   "Charm Person Or Mammal",
   "",
   "Cure Light Wounds",
   "",
   "Bestow Curse",
   "Blade Barrier",
   "Restoration",
   "Energy Drain",
   "Destruction",
   "Resurrection",
   "Cure Disease",
   "Neutralize Poison",
   "Hold Animal",
   "Prot.from Fire",
   "Death Spell",
   "Disintegrate",
   "Globe of Invulnerability",
   "Stone to Flesh",
   "Flesh to Stone",
   "Delayed Blast Fireball",
   "Mass Invisibility",
   "Power Word Stun",
   "Fire Touch",
   "Iron Skin",
   "Mass Charm",
   "Otto's Irr. Dance",
   "Mind Blank",
   "Power Word Blind",
   "Meteor Swarm",
   "Power Word Kill",
   "Monster Summoning",
   "Potion Of Speed",
   "Potion Of Frost",
   "Javelin Of Lightning",
   "Wand Of Paralyzation",
   "Potion Of Healing",
   "Elixir Of Youth",
   "Necklace Of Missiles",
   "Wand Of Magic Missiles",
   "Scroll of Protection",
   "Potion Of Invisibility",
   "Potion Of Extra Healing"
};
*/

const char *ImportFRUASpellVocab[MAX_FRUA_SPELLS] =
{
   "none",
   "Bless", // 1
   "Curse",
   "Cure Light Wounds",
   "Cause Light Wounds",
   "Detect Magic",
   "Protection from Evil",
   "Protection from Good",
   "Resist Cold",
   "Burning Hands",
   "Charm Person", // 10
   "Detect Magic",
   "Enlarge",
   "Reduce",
   "Friends",
   "Magic Missile",
   "Protection from Evil",
   "Protection from Good",
   "Read Magic",
   "Shield",
   "Shocking Grasp", // 20
   "Sleep",
   "Find Traps",
   "Hold Person",
   "Resist Fire",
   "Silence 15' Radius",
   "Slow Poison",
   "Snake Charm",
   "Spiritual Hammer",
   "Detect Invisibility",
   "Invisibility", // 30
   "Knock",
   "Mirror Image",
   "Ray of Enfeeblement",
   "Stinking Cloud",
   "Strength",
   "Heal",
   "Cure Blindness",
   "Cause Blindness",
   "Cure Disease",
   "Cause Disease", // 40
   "Dispel Magic",
   "Prayer",
   "Remove Curse",
   "Bestow Curse",
   "Blink",
   "Dispel Magic",
   "Fireball",
   "Haste",
   "Hold Person",
   "Invisibility 10' Radius", // 50
   "Lightning Bolt",
   "Protection from Evil",
   "Protection from Good",
   "Protection from Normal Missiles",
   "Slow",
   "Harm",
   "Potion of Speed",
   "Cure Serious Wounds",
   "Potion of Giant Strength",
   "Javelin of Lightning", // 60
   "Wand of Paralyzation",
   "Potion of Healing",
   "Elixir of Youth",
   "Necklace of Missiles",
   "Wand of Magic Missiles",
   "Cause Serious Wounds",
   "Neutralize Poison",
   "Poison",
   "Protection from Evil 10' Radius",
   "Sticks to Snakes", // 70
   "Cure Critical Wounds",
   "Cause Critical Wounds",
   "Dispel Evil",
   "Flame Strike",
   "Raise Dead",
   "Slay Living",
   "Detect Magic",
   "Entangle",
   "Faerie Fire",
   "Invisibility to Animals", // 80
   "Charm Monsters",
   "Confusion",
   "Dimension Door",
   "Fear",
   "Fire Shield",
   "Fumble",
   "Ice Storm",
   "Min Globe of Invulnerability",
   "Remove Curse",
   "Barkskin", // 90
   "Cloudkill",
   "Cone of Cold",
   "Feeblemind",
   "Hold Monsters",
   "Scroll Protection from Dragon Breath",
   "Charm Person or Mammal",
   "Potion of Invisibility",
   "Cure Light Wounds",
   "Potion Extra Healing",
   "Bestow Curse", // 100
   "Blade Barrier",
   "Restoration",
   "Energy Drain",
   "Destruction",
   "Resurrection",
   "Cure Disease",
   "Neutralize Poison",
   "Hold Animal",
   "Pro from Fire",
   "Death", // 110
   "Disintegrate",
   "Globe of Invulnerability",
   "Stone to Flesh",
   "Flesh to Stone",
   "Delayed Blast Fireball",
   "Mass Invisibility",
   "Power Word Stun",
   "Fire Touch",
   "Iron Skin",
   "Mass Charm", // 120
   "Otto's Irresistible Dance",
   "Mind Blank",
   "Power Word Blind",
   "Meteor Swarm",
   "Power Word Kill",
   "Monster Summoning", // 126
   "Potion Of Speed",
   "Potion Of Frost",
   "Javelin Of Lightning", 
   "Wand Of Paralyzation", // 130
   "Potion Of Healing",
   "Elixir Of Youth",
   "Necklace Of Missiles",
   "Wand Of Magic Missiles",
   "Potion Of Invisibility", // 135
   "Potion Of Extra Healing" // 136
};


// Text strings for special abilities. Note that this text is also
// used on screen to indicate active buffs/debuffs, or to indicate
// activity from an inherent monster ability.
const char *ImportFRUASpecAbText[MAX_FRUA_SPEC_AB_VOCAB_STRINGS] =
{
   //0 -- No effect
   "none",

   //1 -- Blessed.  Effect of spell "Bless", also works as ability for PC, NPC, monster, or item.
   "Bless",

   //2 -- Cursed.  Effect of spell "Curse", also works as ability for PC, NPC, monster, or item.
   "Curse",

   //3 -- Fighting with Snakes.  Effect of spell "Sticks to Snakes", also
   //works as ability for PC, NPC, monster, or item.  This effect is totally
   //debilitating in combat but can be detected by readying and unreadying
   //the item -- the "fighting with snakes" message is printed upon unreadying.
   "Sticks to Snakes",

   //4 -- Dispelling Evil.  One of effects of spell "Dispel Evil" (effect 32 is
   //the other).  This effect apparently grants the +7 AC bonus vs. creatures
   //affected by Dispel Evil and seems to work for PC, NPC, monster, or item.
   //This effect is the one that causes the "Dispelling Evil" message to be
   //listed for magic display.
   "Dispel Evil",

   //5 -- Detecting Magic.  Effect of spell "Detect Magic", also works as
   //ability for PC, NPC, or item.  There is no point in assigning this
   //ability to a monster, as it would have no visible effect.
   "Detect Magic",

   //6 -- Immune to Electricity.  Standard monster ability as well as ability
   //for "Ring of Electric Immunity", also works as ability for PC, NPC, or
   //spell.
   "Electric Immunity",

   //7 -- Gnome THAC0.  Standard monster ability and default racial ability
   //for Gnome, also works as ability for PC, NPC, item, or spell.  Note that
   //this ability is rather subtle, so it is not easy to determine whether it
   //is in effect or not.
   "Gnome THAC0",

   //8 -- Protected from Evil.  Standard monster ability, default class ability
   //for Paladin, and effect of spell "Protection from Evil"; also works as
   //ability for PC, NPC, or item.
   "Protection from Evil",

   //9 -- Protected from Good.  Standard monster ability and effect of spell
   //"Protection from Good", also works as ability for PC, NPC, or item.
   "Protection from Good",

   //10 -- Resistant to Cold.  Effect of spell "Resist Cold" as well as
   //ability for "Ring of Cold Resistance", also works as ability for PC, NPC,
   //or monster.
   "Resist Cold",

   //11 -- Charmed.  Effect of various "Charm" spells.  This effect does not
   //seem to work as special ability for PC, NPC, monster, or item.
   "Charm",

   //12 -- Enlarged or Reduced.  Effect of "Enlarge" and "Reduce" spells,
   //although in the latter case it appears that the spell removes rather
   //than adds the effect.  Other than causing the "Enlarged" text to be
   //listed for magic display, this effect does nothing on its own.
   "Enlarge",

   //13 -- Barkskin.  Effect of "Barkskin" spell.  I have not been able to
   //determine whether this effect actually makes any difference as a spell
   //effect, let alone as a hacked ability for a PC, NPC, monster, or item.
   "Barkskin",

   //14 -- Friendly.  Effect of "Friends" spell.  A PC or NPC hacked with
   //this effect will have the "Friendly" text listed for magic display
   //but will otherwise be unaffected.  Readying an item or casting a
   //spell with this effect may alter the target's charisma.  There is,
   //of course, no point in granting this ability to a monster.
   "Friends",

   //15 -- Slowed Poison.  One of the effects of "Slow Poison" spell -- 22
   //is the other.  Contrary to the spell description, this spell does not
   //seem to have an effective duration -- the only difference is that a
   //character revived with this spell comes back with one hit point instead
   //of full hit points.  This effect seems to be completely ineffective as
   //a special ability for PCs, NPCs, monsters, or items.
   "Slow Poison",

   //16 -- Reading Magic.  Effect of "Read Magic" spell, also works as
   //ability for PC, NPC, or item.  There is, of course, no point in
   //granting this ability to a monster.
   "Read Magic",

   //17 -- Shielded.  Effect of "Shield" spell, also appears to work as
   //special ability for PC, NPC, monster, or item.
   "Shield",

   //18 -- Half-Elf MR.  Standard monster ability and default racial ability
   //for Half-Elf, also appears to work as special ability for PC, NPC, or
   //item.
   "Half-Elf Magic Resist",

   //19 -- Finding Traps.  Effect of "Find Traps" spell, also works as
   //ability for PC, NPC, or item.  There is, of course, no point in
   //granting this ability to a monster.
   "Find Traps",

   //20 -- Resistant to Fire.  Effect of "Resist Fire" spell, also appears
   //to work as special ability for PC, NPC, monster, or item.
   "Resist Fire (Cleric)",

   //21 -- Silenced.  Effect of "Silence 15' Radius" spell, also works as
   //special ability for PC, NPC, monster, or item.  While this effect
   //prevents the affected character from casting spells, all combatants
   //adjacent to the silenced character, whether friend or foe, are
   //affected in the same way.
   "Silence 15' Radius",

   //22 -- Slowed Poison (secondary effect) -- see also 15.
   "Slow Poison",

   //23 -- Has Spiritual Hammer.  Effect of spell "Spiritual Hammer".  This
   //effect appears to work in a rather strange manner except as the effect
   //of this spell.
   "Spiritual Hammer",

   //24 -- Sees Invisible.  Standard monster ability and effect of spell
   //"Detect Invisible", also appears to work as special ability of PC,
   //NPC, or item.
   "Detect Invisible",

   //25 -- Invisible.  Standard monster ability and effect of various
   //"Invisibility" spells as well as "Potion of Invisibility", also
   //works as special ability of PC, NPC, or item.
   "Invisibility",

   //26 -- Dwarf THAC0.  Standard monster ability and default racial ability
   //for Dwarf, also appears to work as special ability for PC, NPC, or item.
   "Dwarf THAC0",

   //27 -- Fumbling.  Effect of spell "Fumble" (if victim fails to save),
   //also appears to work as special vulnerability for monster (although
   //it does render the monster virtually helpless).  This effect does not
   //seem to work for player characters or NPCs and actually causes problems
   //as an item effect.
   "Fumble",

   //28 -- Duplicated.  Effect of "Mirror Image" spell, also appears to work
   //as special ability for PC, NPC, monster, or item.
   "Mirror Image",

   //29 -- Weakened.  Effect of "Ray of Enfeeblement" spell.  I have not been
   //able to determine whether this effect actually makes any difference as a
   //spell effect, let alone as a hacked ability for a PC, NPC, monster, or
   //item.
   "Ray of Enfeeblement",

   //30 -- Coughing.  Effect of "Stinking Cloud" spell (if victim saves vs.
   //poison).  This effect also appears to work as a special (dis)ability
   //for PCs, NPCs, monsters, and items.
   "Stinking Cloud",

   //31 -- Helpless.  Apparent effect of "Stinking Cloud" spell (if save vs.
   //poison is failed) and "final" effect of "Cause Disease" spell.  This
   //effect accumulates indefinitely for "diseased" characters.  This effect
   //appears to work as special (dis)ability for PCs, NPCs, monsters, items,
   //and spells.
   "Stinking Cloud",

   //32 -- Dispelling Evil.  One of effects of spell "Dispel Evil" (effect 4 is
   //the other).  This effect apparently is the one that causes creatures
   //vulnerable to this effect to be dispelled when struck.  It seems to work
   //for PC, NPC, or item but causes no message to be listed for magic display.
   //It is unknown whether this effect works for monsters.
   "Dispel Evil",

   //33 -- Blind.  Effect of "Cause Blindness" and "Power Word Blind" spells.
   //This effect also appears to work as special (dis)ability for PC, NPC,
   //monster, or item.
   "Cause Blindness",

   //34 -- Diseased.  Effect of "Cause Disease" spell, but appears to do
   //nothing on its own except cause "Diseased" message to be listed for
   //magic display.  Effects 43 and 44 are also set into motion by this
   //spell.
   "Cause Disease",

   //35 -- Confused.  Effect of "Confusion" spell.  This effect works well
   //as a special (dis)ability for PC, NPC, monster, or item.
   "Confusion",

   //36 -- Accursed.  Effect of "Bestow Curse".  I have not been able to
   //determine whether this effect actually makes any difference as a
   //spell effect, let alone as a hacked ability for a PC, NPC, monster,
   //or item.
   "Bestow Curse",

   //37 -- Blinking.  Standard monster ability and effect of "Blink" spell,
   //also appears to work as special ability for PC, NPC, monster, or item.
   "Blink",

   //38 -- Strengthened.  Effect of "Strength" spell.  Other than causing the
   //"Strengthened" text to be listed for magic display, this effect does
   //nothing on its own.
   "Strength",

   //39 -- Hasted.  Effect of spell "Haste" and "Potion of Speed" item,
   //also appears to work as special ability for PC, NPC, or monster.
   "Haste",

   //40 -- Rakshasa MR.  Standard monster ability, also appears to work as
   //special ability for PC, NPC, item, or spell effect.
   "Rakshasa Magic Resist",

   //41 -- Protected from Normal Missiles.  Effect of "Protection from
   //Normal Missiles" spell, also appears to work as special ability for
   //PC, NPC, monster, or item.
   "Protection From Normal Missiles",

   //42 -- Slowed.  Effect of "Slow" spell, also appears to work as special
   //ability for PC, NPC, monster, or item.
   "Slow",

   //43 -- Weakened.  This effect represents the loss of strength from the
   //"Cause Disease" spell.  Because of the time factor, it works only as
   //a spell effect or PC (dis)ability.  An item with this effect will
   //cause the loss of one point of strength whenever the item is unreadied.
   //After strength reaches 3, effect 31 (Helpless) is added.
   "Cause Disease",

   //44 -- Loss of hit point.  This effect represents the loss of hit points
   //from the "Cause Disease" spell.  Because of the time factor, it works
   //only as a spell effect or PC (dis)ability.  An item with this effect
   //will cause the loss of one hit point whenever the item is unreadied
   //(in addition to some minor display problems).  After hit points reach
   //10% of maximum, effect 31 (Helpless) is added.
   "Cause Disease",

   //45 -- Protection vs. Evil 10' Radius.  Standard monster ability and
   //effect of spell "Protection from Evil 10' Radius", also works as
   //ability for PC, NPC, or item.
   "Protection from Evil 10' Radius",

   //46 -- Protection vs. Good 10' Radius.  Standard monster ability and
   //effect of spell "Protection from Good 10' Radius", also works as
   //ability for PC, NPC, or item.
   "Protection from Good 10' Radius",

   //47 -- Dwarf AC Bonus.  Standard monster ability and default racial ability
   //for Dwarf, also appears to work as special ability for PC, NPC, or item.
   "Dwarf AC Bonus",

   //48 -- Gnome AC Bonus.  Standard monster ability and default racial ability
   //for Gnome, also appears to work as special ability for PC, NPC, or item.
   "Gnome AC Bonus",

   //49 -- Affected by Prayer.  Effect of spell "Prayer", also works as ability
   //for PC, NPC, monster, or item.
   "Prayer",

   //50 -- Fire Shield, Hot Flames.  Effect of spell "Fire Shield", also
   //appears to work as special ability for PC, NPC, monster, or item.
   //This effect is the one that controls vulnerability to fire and cold
   //(as does its cold flame counterpart, effect 54).  Effect 112 is the
   //effect that gives the "zap".
   "Fire Shield Hot",

   //51 -- Charmed Snake.  Effect of spell "Snake Charm", also appears to
   //work as special (dis)ability for PC, NPC, monster, or item.  The
   //victim of this effect is rendered helpless.
   "Snake Charm",

   //52 -- Held.  Effect of various "Hold" spells, also appears to work as
   //special (dis)ability for PC, NPC, monster, or item.  The victim of this
   //effect is rendered helpless.
   "Hold",

   //53 -- Asleep.  Effect of "Sleep" spell, also appears to work as special
   //(dis)ability for PC, NPC, monster, or item.  The victim of this effect
   //is rendered helpless.
   "Sleep",

   //54 -- Fire Shield, Cold Flames.  Effect of spell "Fire Shield", also
   //appears to work as special ability for PC, NPC, monster, or item.
   //This effect is the one that controls vulnerability to fire and cold
   //(as does its hot flame counterpart, effect 50).  Effect 112 is the
   //effect that gives the "zap".
   "Fire Shield Cold",

   //55 -- Poisoned.  This effect appears to do nothing on its own except
   //cause the "Poisoned" text to be listed for magic display.
   "Poison",

   //56 -- Invisible (Ring).  Effect of "Ring of Invisibility", also
   //appears to work as special ability of PC, NPC, monster, or spell
   //effect.
   "Invisibility",

   //57 -- Globe of Invulnerability.  Effect of "Globe of Invulnerability"
   //spell, also appears to work as special ability for PC, NPC, monster,
   //or item.
   "Globe of Invulnerability",

   //58 -- Petrifying Gaze.  Standard monster effect, also appears to work
   //as special ability for PC, NPC, item, or spell effect.  This ability
   //will work only for combatants in "quick" mode.
   "Petrifying Gaze",

   //59 -- Reflectable Gaze.  Standard monster (dis)ability, also appears
   //to work as special vulnerability for PC, NPC, item, or spell effect.
   //Naturally, it has no effect except on creatures with gaze attack
   //special abilities.
   "Reflectable Gaze",

   //60 -- +3 or better weapon to hit.  Standard monster ability, also appears
   //to work as special ability for PC, NPC, item, or spell effect.
   "+3 weapon to hit",

   //61 -- Resistant to Fire (Ring).  Effect of "Ring of Fire Resistance",
   //also appears to work as special ability of PC, NPC, monster, or spell
   //effect.
   "Resist Fire (Ring)",

   //62 -- Rapid Healing.  Standard monster ability, but does not work too
   //well in that capacity.  Because of the time factor, it works only as
   //a spell effect or PC (dis)ability.  As an item effect, it grants the
   //rapid healing effect to a character when wielded and also causes the
   //character to heal one hit point whenever the item is readied -- however,
   //this healing is accompanied by minor display problems.
   "Rapid Healing",

   //63 -- Minor Globe of Invulnerability.  Effect of "Minor Globe of
   //Invulnerability" spell, also appears to work as special ability for
   //PC, NPC, monster, or item.
   "Minor Globe of Invulnerability",

   //64 -- Lethal Poison.  Standard monster ability, also appears to work as
   //special ability for PC, NPC, item, or spell effect.
   "Lethal Poison",

   //65 -- Petrify Attack.  Standard monster ability, also appears to work
   //as special ability for PC, NPC, item, or spell effect.
   "Petrify Attack",

   //66 -- Half Damage from Cutting Weapons.  Standard monster ability, also
   //appears to work as special ability for PC, NPC, item, or spell effect.
   "Half damage from cutting weapons",

   //67 -- Half Damage from Blunt Weapons.  Standard monster ability, also
   //appears to work as special ability for PC, NPC, item, or spell effect.
   "Half damage from blunt weapons",

   //68 -- Mentally Enfeebled.  Effect of spell "Feeblemind", also works
   //as special ability for PC, NPC, monster, or item.  As an item effect,
   //it is inflicted when the item is readied and removed when the item
   //is unreadied.  As a personal effect, it is inflicted whenever the
   //character first enters combat.  A mentally enfeebled character is
   //unable to cast spells in combat and also suffers from the adjustements
   //set for the spell -- by default, intelligence and wisdom are reduced
   //to 3 each.
   "Feeblemind",

   //69 -- Invisible to Animals.  Effect of "Invisibility to Animals" spell.
   //I have not been able to determine whether this effect actually makes any
   //difference as a spell effect, let alone as a hacked ability for a PC, NPC,
   //monster, or item.
   "Invisibility to Animals",

   //70 -- Umber Hulk Gaze.  Standard monster effect, also appears to work
   //as special ability for PC, NPC, item, or spell effect.  This ability
   //will work only for combatants in "quick" mode.
   "Umber Hulk Gaze",

   //71 -- Magically Highlighted.  Effect of "Faerie Fire" spell.  I have not
   //been able to determine whether this effect actually makes any difference
   //as a spell effect, let alone as a hacked ability for a PC, NPC, monster,
   //or item.
   "Faerie Fire",

   //72 -- Mirror Readied.  Standard monster ability as well as effect of
   //"mirror" and "silver shield" items, also appears to work as special
   //ability for PC, NPC, or spell effect.
   "Mirror Readied",

   //73 -- Immune to Dragon Breath.  Standard monster ability, also works as
   //ability for PC, NPC, item, or spell.
   "Immune to Dragon Breath",

   //74 -- Wearing Boots of Speed.  Special ability for "Boots of Speed" item,
   //also appears to work as special ability for PC, NPC, monster, or spell
   //effect.
   "Boots of Speed",

   //75 -- Giant Slaying Sword.  Special ability for "Long Sword vs. Giants"
   //item, also appears to work as special ability for PC, NPC, monster, or
   //spell effect.
   "Long Sword vs. Giants",

   //76 -- Immune to Disease.  Standard monster ability as well as ability
   //for "Periapt of Health", also works as ability for PC, NPC, or spell.
   //Note that this ability seems to grant immunity only to the spell
   //"Cause Disease", not to the "Mummy Disease" special effect.
   "Immune to Disease",

   //77 -- Unknown effect
   "Unknown 77 effect",

   //78 -- Wearing Stone of Good Luck.  Special ability of "Stone of
   //Good Luck" item, also appears to work as special ability for PC,
   //NPC, monster, or spell effect.
   "Stone of Good Luck",

   //79 -- Iron Golem MR.  Standard monster ability, also appears to
   //work as special ability for PC, NPC, item, or spell effect.
   "Iron Golem Magic Resist",

   //80 -- Half Damage from Weapon of +2 or less.  Standard monster ability,
   //also appears to work as special ability for PC, NPC, item, or spell
   //effect.
   "Half damage from weapon +2 or less",

   //81 -- Half Damage from Weapon of +3 or less.  Standard monster ability,
   //also appears to work as special ability for PC, NPC, item, or spell
   //effect.
   "Half damage from weapon +3 or less",

   //82 -- Slain by Blessed Bolts.  Standard monster (dis)ability, also
   //appears to work as special ability for PC, NPC, item, or spell
   //effect.
   "Slain by Blessed Bolts",

   //83 -- Super Fire Breath.  This effect appears to work well only for
   //monsters.  It causes damage as the "Dragon Fire Breath" ability but
   //the damage is generally greater than the attacker's hit points and
   //(apparently) is concentrated in a smaller area.  It does not seem
   //to work for PCs, NPCs, or spell effects -- if it does, then it is
   //strictly under the control of UA's combat AI.  As an item effect,
   //this ability causes major display problems and may even crash your
   //computer.
   "Super Fire Breath",

   //84 -- Dragon Fire Breath.  Standard monster ability.  This effect
   //appears to work well only for monsters.  As a special ability for
   //PCs, NPCs, or spell effects, it works only when the combatant is
   //in "Quick" mode and then only if the UA combat AI determines that
   //it should.  As an item effect, this ability causes major display
   //problems and may even crash your computer.
   "Dragon Fire Breath",

   //85 -- Dragon Cold Breath.  Standard monster ability.  This effect
   //appears to work well only for monsters.  As a special ability for
   //PCs, NPCs, or spell effects, it works only when the combatant is
   //in "Quick" mode and then only if the UA combat AI determines that
   //it should.  As an item effect, this ability causes major display
   //problems and may even crash your computer.
   "Dragon Cold Breath",

   //86 -- Phase Spider Poison.  Standard monster ability, also appears
   //to work as a special ability for PC, NPC, item, or spell effect.
   //There have been some unconfirmed reports that it may cause problems
   //and/or work too well as an item effect.
   "Phase Spider Poison",

   //87 -- Unknown effect
   "Unknown 87 effect",

   //88 -- Paralyzing Attack.  Standard monster ability, also appears to
   //work as special ability for PC, NPC, item, or spell effect.  Note
   //that it is effective only for combatants who have an attack #2.
   "Paralyzing Attack",

   //89 -- Wearing Cloak of Displacement.  Standard monster ability and
   //effect of "Cloak of Displacement" item, also appears to work as special
   //ability for PC, NPC, or spell effect.
   "Cloak of Displacement",

   //90 -- Dragon Poison Breath.  Standard monster ability.  This effect
   //appears to work well only for monsters.  As a special ability for PCs,
   //NPCs, or spell effects, it works only when the combatant is in "Quick"
   //mode and then only if the UA combat AI determines that it should.  As
   //an item effect, this ability causes major display problems and may even
   //crash your computer.
   "Dragon Poison Breath",

   //91 -- Dragon Lightning Breath.  Standard monster ability This effect
   //appears to work well only for monsters.  As a special ability for PCs,
   //NPCs, or spell effects, it works only when the combatant is in "Quick"
   //mode and then only if the UA combat AI determines that it should.  As
   //an item effect, this ability causes major display problems and may even
   //crash your computer.
   "Dragon Lightning Breath",

   //92 -- Immune to Fear.  Standard monster ability, also works as ability
   //for PC, NPC, item, or spell.
   "Immune to Fear",

   //93 -- Immune to Fire.  Standard monster ability, also works as ability
   //for PC, NPC, item, or spell.
   "Immune to Fire",

   //94 -- Unknown effect
   "Unknown 94 effect",

   //95 -- Elf MR.  Standard monster ability and default racial ability for
   //Elf, also appears to work as special ability for PC, NPC, or item.
   "Elf Magic Resistance",

   //96 -- Dragon Acid Breath.  Standard monster ability.  This effect appears
   //to work well only for monsters.  As a special ability for PCs, NPCs, or
   //spell effects, it works only when the combatant is in "Quick" mode and
   //then only if the UA combat AI determines that it should.  As an item
   //effect, this ability causes major display problems and may even crash
   //your computer.
   "Dragon Acid Breath",

   //97 -- Short Guy MR.  Standard monster ability and default racial ability
   //for several races, also appears to work as special ability for PC, NPC,
   //or item.
   "Dwarf Magic Resistance"

   //98 -- Immune to Cold.  Standard monster ability, also works as special
   //ability for PC, NPC, item, or spell.
   "Immune to Cold",

   //99 -- Random Bonus Attacks.  Standard monster ability, also works as
   //special ability for PC, NPC, item, or spell effect.  However, note that
   //this effect applies only to attack #2 and thus is less effective for
   //combatants who lack that attack ability.
   "Random Bonus Attacks",

   //100 -- Dragon Fear Attack.  Standard monster ability, also works as
   //special ability for PC, NPC, or monster.  As an item effect, it seems
   //to work properly but causes major display problems.  As a spell effect,
   //this special ability has not yet been evaluated.
   "Dragon Fear Attack",

   //101 -- Drain 1 Level.  Standard monster ability, also appears to work as
   //special ability for PC, NPC, item, or spell effect.
   "Drain one level",

   //102 -- Drain 2 Levels.  Standard monster ability, also appears to work as
   //special ability for PC, NPC, item, or spell effect.
   "Drain two levels",

   //103 -- +1 or better weapon to hit.  Standard monster ability, also appears
   //to work as special ability for PC, NPC, item, or spell effect.
   "+1 or better weapon to hit",

   //104 -- +4d6 fire damage.  Standard monster ability, also works as
   //special ability for PC, NPC, item, or spell effect.  However, note that
   //this effect applies only to attack #2 and thus has no effect for
   //combatants who lack that attack ability.
   "+4D6 Fire Damage",

   //105 -- Ranger Damage Bonus.  Standard monster ability and default class
   //ability for Ranger; also works as special ability for PC, NPC, item, or
   //spell effect.  Note that a character must have levels as a Ranger (class
   //4) in order for this ability to have any effect.
   "Ranger Damage Bonus",

   //106 -- Stunned.  Effect of spell "Power Word Stun", also works as special
   //(dis)ability for item, monster, or spell effect.  For some reason, it has
   //no effect on PCs or NPCs.
   "Power Word Stun",

   //107 -- Aura of Hostility.  This effect deserves further investigation.
   //So far, it has been established as working only as an item effect.
   //When an item with this effect is readied, any attack that the player
   //attempts to initiate with this character (whether on friend or foe)
   //is treated as an attack on an ally.  If the character is set to
   //"Quick" mode, he will attack allies rather than enemies.  Finally,
   //if no other allies survive the battle, the game will end with the
   //"The monsters rejoice...." message.
   "Aura of Hostility",

   //108 -- +2d6 fire damage.  Standard monster ability, also works as
   //special ability for PC, NPC, item, or spell effect.  However, note that
   //this effect applies only to attack #2 and thus has no effect for
   //combatants who lack that attack ability.
   "+2D6 Fire Damage",

   //109 -- Unknown effect
   "Unknown 109 Effect",

   //110 -- Paladin Cure used up.  This effect is added to a Paladin after
   //he uses his "Cure Disease" ability and indicates the time delay until
   //this ability is fully restored.  Because of the time factor, it does
   //not appear to work for NPCs, monsters, or items; its applicability as
   //a spell effect is unknown.
   "Paladin Cure On Cooldown",

   //111 -- Afraid.  Effect of spell "Fear".  As an effect on PCs, NPCs,
   //monsters, or items, this effect has not yet been researched.
   "Fear",

   //112 -- Protected by Fire Shield.  Effect of spell "Fire Shield", also
   //appears to work as special ability for PC, NPC, monster, or item.
   //This effect is the one that gives the "zap" effect.  Note that it
   //causes no text to be shown for magic display.
   "Fire Shield",

   //113 -- Strengthened by Potion of Giant Strength.  Effect of "Potion
   //of Giant Strength" item.  Other than causing the "Strengthened" text
   //to be listed for magic display, this effect does nothing on its own.
   "Potion of Giant Strength",

   //114 -- Paralyzing Gaze.  Standard monster effect, also appears to work
   //as special ability for PC, NPC, item, or spell effect.  This ability
   //will work only for combatants in "quick" mode.
   "Paralyzing Gaze",

   //115 -- Vorpal Weapon.  Effect of "Vorpal long sword", also appears
   //to work as special ability for PC, NPC, monster, or spell effect.
   "Vorpal Weapon",

   //116 -- Marilith Attack.  Standard monster effect, also appears to work
   //as special ability for PC, NPC, item, or spell effect.  However, note
   //that it requires a large number of attack #1's to work properly.
   "Marilith Attack",

   //117 -- Beholder Attack.  Standard monster effect, also appears to work
   //as special ability for PC, NPC, or spell effect.  As an item effect, it
   //works when an item item is unreadied and causes some minor graphics
   //problems.  What makes this power especially nasty as an item ability is
   //the fact that the character can launch a beholder attack each and every
   //time he readies the item.  Thus, if the character is under player control
   //and repeatedly readies and unreadies the item, he can really wreak havoc.
   "Beholder Attack",

   //118 -- Dancing.  Effect of spell "Otto's Irresistible Dance".  As an effect
   //on PCs, NPCs, monsters, or items, this effect has not yet been researched.
   "Otto's Irresistable Dance",

   //119 -- Resist Fire (Druid).  Effect of spell "Resist Fire".  As an effect
   //on PCs, NPCs, monsters, or items, this effect has not yet been researched.
   "Resist Fire (Druid)",

   //120 -- No more attacks/movement.  As an item effect, this (dis)ability
   //prevents any further movements or attack.  Its effects as a special
   //(dis)ability for PCs, NPCs, monsters, or spell effects have not yet
   //been researched.
   "No more attacks or movement",

   //121 -- Fire Touch.  Effect of "Fire Touch" spell, also appears to work
   //as special ability for PC, NPC, monster, or item.
   "Fire Touch",

   //122 -- Iron Skin.  Effect of "Iron Skin" spell, also appears to work
   //(assuming that it works at all -- I am uncertain about this point) as
   //special ability for PC, NPC, monster, or item.
   "Iron Skin"

   //123 -- Shielded by Mind Blank.  Effect of "Mind Blank" spell, also
   //appears to work as special ability for PC, NPC, monster, or item.
   "Mind Blank",

   //124 -- Charm Gaze.  Standard monster effect, also appears to work
   //as special ability for PC, NPC, item, or spell effect.  This ability
   //will work only for combatants in "quick" mode.
   "Charm Gaze",

   //125 -- +2 or better weapon to hit.  Standard monster ability, also appears
   //to work as special ability for PC, NPC, monster, or spell effect.
   "+2 weapon to hit",

   //126 -- Protected vs. Gaze Attack.  Standard monster ability, also appears
   //to work as special ability for PC, NPC, monster, or spell effect.
   "Protected vs Gaze Attack",

   //127-131 -- Unknown effects
   "Unknown 127 effect",
   "Unknown 128 effect",
   "Unknown 129 effect",
   "Unknown 130 effect",
   "Unknown 131 effect",

   //132 -- Paralyze on Hit.  Standard monster ability, also appears to work
   //as special ability for PC or NPC.  As an item effect, it appears to
   //cause a variety of problems -- the paralysis effect may occur too easily.
   //As a spell effect, this special ability has yet to be researched.
   "Paralyze on Hit",

   //133-136 -- Unknown effects
   "Unknown 133 effect",
   "Unknown 134 effect",
   "Unknown 135 effect",
   "Unknown 136 effect",

   //137 -- Carrion Stench.  Standard monster ability, also appears to work
   //as special ability for PC, NPC, monster, item, or spell effect.
   "Carrion Stench",

   //138-139 -- Unknown effects
   "Unknown 138 effect",
   "Unknown 139 effect",

   //140 -- Paladin Lay Hands used up.  This effect is added to a Paladin
   //after he uses his "Lay Hands" ability and indicates the time delay
   //until this ability is restored.  Because of the time factor, it does
   //not appear to work for NPCs, monsters, or items; its applicability as
   //a spell effect is unknown.
   "Paladin Lay Hands On Cooldown",

   //141 -- Immune to Sleep/Charm.  Standard monster ability, also works as
   //ability for PC, NPC, item, or spell.
   "Immune to Sleep/Charm",

   //142 -- Unknown effect
   "Unknown 142 effect",

   //143 -- Immune to Poison/Paralysis.  Standard monster ability, also
   //works as ability for PC, NPC, item, or spell.  Note that this effect
   //seems to work only for spells that allow a save vs. poison or that
   //cause a paralysis effect -- it appears to be totally ineffective
   //against monster abilities that cause poison or paralysis.
   "Immune to Poison/Paralysis",

   //144-157 -- Unknown effects
   "Unknown 144 effect",
   "Unknown 145 effect",
   "Unknown 146 effect",
   "Unknown 147 effect",
   "Unknown 148 effect",
   "Unknown 149 effect",
   "Unknown 150 effect",
   "Unknown 151 effect",
   "Unknown 152 effect",
   "Unknown 153 effect",
   "Unknown 154 effect",
   "Unknown 155 effect",
   "Unknown 156 effect",
   "Unknown 157 effect",

   //158 -- Immune to Paralysis.  Standard monster ability, also works as
   //ability for PC, NPC, item, or spell.  Note that this effect seems to
   //work only for spells that cause a paralysis effect -- it appears to be
   //totally ineffective against monster abilities that cause paralysis.
   "Immune to Paralysis",

   //159 -- Damaged by Holy Water.  The effectiveness of this vulnerability
   //cannot be determined until we identify the special ability corresponding
   //to holy water.
   "Damaged by Holy Water",

   //160-167 -- Unknown effects
   "Unknown 160 effect",
   "Unknown 161 effect",
   "Unknown 162 effect",
   "Unknown 163 effect",
   "Unknown 164 effect",
   "Unknown 165 effect",
   "Unknown 166 effect",
   "Unknown 167 effect",

   //168 -- Vulnerable to Fire.  Standard monster (dis)ability, also
   //appears to work as special ability for PC, NPC, item, or spell
   //effect.
   "Vulnerable to Fire",

   //169 -- Half Damage from Magic Weapons.  Standard monster ability, also
   //appears to work as special ability for PC, NPC, monster, or spell effect.
   "Half Damage from Magic Weapons",

   //170 -- Mummy Disease.  Standard monster ability, also appears to work
   //as special ability for PC, NPC, monster, or spell effect.  As an item
   //effect, it causes major display problems and may even crash your computer.
   "Mummy Disease",

   //171 -- Mummy Fear Aura.  Standard monster ability, also appears to work
   //(although there are unconfirmed reports of problems) as special ability
   //for PC, NPC, or spell effect.  As an item effect, it is one of the surest
   //ways to crash your computer.
   "Mummy Fear Aura",

   //172 -- Afflicted with Mummy Rot.  This effect is the one that causes the
   //"Diseased" message to be listed for magic display.  This is one of the
   //effects caused by a hit from a Mummy -- effect 215 is the other.  This
   //ability is the one that causes a character to lose charisma over time and
   //eventually die and rot away.  It appears to work as a PC ability or spell
   //effect.  Because of the required time factor, it does not work as an NPC
   //or monster effect.  As an item special ability, it causes a character to
   //"rot" and lose charisma whenever the item is unreadied -- however, the
   //item effect is apparently non-fatal.
   "Mummy Rot diseased",

   //173 -- Grows from Electricity.  Standard monster ability, also works as
   //special ability for PC, NPC, item, or spell effect.
   "Grows from Electricity",

   //174 -- Resistant to Fire/Cold.  Standard monster ability, also appears
   //to work as special ability for PC, NPC, item, or spell effect.
   "Resist Fire/Cold",

   //175 -- Half Damage from All Weapons.  Standard monster ability, also
   //appears to work as special ability for PC, NPC, item, or spell effect.
   "Half damage from all weapons",

   //176 -- Shambling Mound Engulf.  Standard monster ability, also appears to
   //work as special ability for PC, NPC, item, or spell effect.
   "Shambling Mound Engulf",

   //177 -- Unknown effect
   "Unknown 177 effect",

   //178 -- Unknown effect that, when applied to items, prevents movement
   //when readied.  Nothing else is known about this effect at present.
   "Prevent Movement when readied",

   //179-185 -- Unknown effects.  Effect 179 causes problems with items.
   "Unknown 179 effect",
   "Unknown 180 effect",
   "Unknown 181 effect",
   "Unknown 182 effect",
   "Unknown 183 effect",
   "Unknown 184 effect",
   "Unknown 185 effect",

   //186 -- Black Pudding Attack.  Standard monster ability, also appears
   //to work as special ability for PCs, NPCs, and monsters.  It is not
   //known whether this ability works properly for items or spell effects.
   "Black Pudding Attack",

   //187-192 -- Unknown effects.
   "Unknown 187 effect",
   "Unknown 188 effect",
   "Unknown 189 effect",
   "Unknown 190 effect",
   "Unknown 191 effect",
   "Unknown 192 effect",

   //193 -- Immune to All Weapons.  Standard monster ability, also appears
   //to work as special ability for PC, NPC, monster, or spell effect.
   "Immune to all weapons",

   //194 -- Unknown effect.
   "Unknown 194 effect",

   //195 -- Gorgon Attack.  Standard monster ability.  This effect appears
   //to work well only for monsters -- if it does work as a special ability
   //for PCs, NPCs, items, or spell effects, it is activated only when the
   //character is in "quick" mode and then only if the UA combat AI determines
   //that it should be used.
   "Gorgon Attack",

   //196-214 -- Unknown effects.  Effect 201 causes problems with items.
   "Unknown 196 effect",
   "Unknown 197 effect",
   "Unknown 198 effect",
   "Unknown 199 effect",
   "Unknown 200 effect",
   "Unknown 201 effect",
   "Unknown 202 effect",
   "Unknown 203 effect",
   "Unknown 204 effect",
   "Unknown 205 effect",
   "Unknown 206 effect",
   "Unknown 207 effect",
   "Unknown 208 effect",
   "Unknown 209 effect",
   "Unknown 210 effect",
   "Unknown 211 effect",
   "Unknown 212 effect",
   "Unknown 213 effect",
   "Unknown 214 effect",

   //215 -- Afflicted with Mummy Rot.  This effect does not cause the
   //"Diseased" message to be listed for magic display, although a character
   //with this effect is still effectively diseased.  This is one of the
   //effects caused by a hit from a Mummy -- effect 172 is the other.  This
   //ability is the one that prevents a character from being healed.  It
   //appears to work as a spell effect or as a special (dis)ability for a
   //PC, NPC, or monster.  As an item effect, it causes a character to heal
   //one hit point whenever the item is unreadied; this unreadying also
   //causes minor display problems.
   "Mummy Rot prevents healing",

   //216 -- Troll Get Up.  Standard monster ability, also appears to work
   //as special ability for PC or NPC despite some minor graphics problems.
   //It is uncertain how well it will work as an item or spell effect.  See
   //effect 218 for more information.
   "Troll Get Up",

   //217 -- Troll Regeneration.  Standard monster ability, also works as
   //special ability for PC, NPC, or monster.  The effectiveness of this
   //ability as an item or spell effect is unknown.  See effects 225 and
   //226 for more information.
   "Troll Regeneration",

   //218 -- Waiting to Get Up.  When an item with this effect is unreadied,
   //the character holding that item is fully healed.  This effect is also
   //granted to a character with the "Troll Get Up" special ability (effect
   //216) when that character is killed or rendered unconscious -- when its
   //duration expires, the character is brought back to life with full hit
   //points.
   "Waiting to Get Up",

   //219 -- Efreet Fire Resistance.  Standard monster ability, also appears
   //to work as special ability for PC, NPC, item, or spell effect.
   "Efreet Fire Resistance",

   //220 -- Owl Bear Hug.  Standard monster ability, also appears to work as
   //special ability for PC, NPC, item, or spell effect.
   "Owl Bear Hug",

   //221 -- Guarding.  This effect forces a character or monster into a
   //"Guarding" mode.  The character cannot move for the remainder of the
   //turn (if readying an item with this effect).  A character or monster
   //under this effect (whether as a personal ability or as an item effect)
   //will not move at all if turned over to computer control.  It is unknown
   //how this effect will work as a spell effect.
   "Guarding",

   //222 -- Unknown effect.
   "Unknown 222 effect",

   //223 -- Hell Hound Breath.  Standard monster effect, also appears to
   //work as special ability for PC, NPC, item, or spell effect.  This
   //ability will work only for combatants in "quick" mode.
   "Hell Hound Breath",

   //224 -- Ghoul Paralysis Attack.  Standard monster ability, also appears
   //to work as special ability for PC or NPC.  As an item effect, it appears
   //to cause a variety of problems -- the paralysis effect may occur too
   //easily.  As a spell effect, this special ability has yet to be researched.
   "Ghoul Paralysis Attack",

   //225 -- Waiting for regeneration to start.  This effect is added to
   //a character with the Troll Regeneration ability (code 217) when that
   //character is wounded to give the delay of three rounds until the
   //regeneration sets in.  Once regeneration sets in, effect 226 is
   //added in place of effect 225.  Because of the required time factor,
   //this effect will not work as a special ability for an NPC, monster,
   //or item.  It is unknown whether it will work as a spell effect.
   "Waiting for regeneration to start",

   //226 -- Regenerating.  This effect is added to a character with the
   //Troll Regeneration ability (code 217) once the duration of effect
   //225 expires.  Any character with this ability will heal 3 hit points
   //per round until fully healed.  This effect appears to work as a
   //special ability for PC, NPCs, and items.  It is unknown whether
   //this effect will work properly for monsters or spell effects.
   "Regenerating",

   //227-254 -- Unknown effects.  Effects 230-254 will crash UA or even your
   //computer if used in any way.
   "Unknown 227 effect",
   "Unknown 228 effect",
   "Unknown 229 effect",
   "Unknown 230 effect",
   "Unknown 231 effect",
   "Unknown 232 effect",
   "Unknown 233 effect",
   "Unknown 234 effect",
   "Unknown 235 effect",
   "Unknown 236 effect",
   "Unknown 237 effect",
   "Unknown 238 effect",
   "Unknown 239 effect",
   "Unknown 240 effect",
   "Unknown 241 effect",
   "Unknown 242 effect",
   "Unknown 243 effect",
   "Unknown 244 effect",
   "Unknown 245 effect",
   "Unknown 246 effect",
   "Unknown 247 effect",
   "Unknown 248 effect",
   "Unknown 249 effect",
   "Unknown 250 effect",
   "Unknown 251 effect",
   "Unknown 252 effect",
   "Unknown 253 effect",
   "Unknown 254 effect",

   //255 -- Race/class special ability placeholder.
   "Race/Class special ability placeholder"
};

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

These monster codes are for NPC's

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

*/
char *ImportFRUAMonsterNames[MAX_FRUA_MONSTERS] = {
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
  "Raizel"
};

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
char *ImportFRUASoundNames[MAX_FRUA_SOUND_VOCAB_STRINGS] = 
{
   "none",               
   "Unknown",
   "Unknown",
   "Cast",
   "Blast",                 
   "Sorcery",               
   "Death",                 
   "Missile", 
   "Hit",
   "Lightning",
   "Swish",
   "Footstep",
   "Fireball",
   "Bowstring",
   "Sploosh",
   "Crackle",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Unknown",
   "Overture",
   "Treasure",
   "Foes",
   "Battle",
   "Mystery",
   "Uh Oh",
   "Evil March",
   "Victory"
};

/*
 00  None                       20  Enter Password
 01  Combat                     21  Question - List
 02  Text Statement             22  Small Town
 03  Give Treasure              23  Chain
 04  Damage                     24  Vault
 05  Stairs                     25  Combat Treasure
 06  Training Hall              26  Gain Experience
 07  Tavern                     27  Pass Time
 08  Shop                       28 
 09  Temple                     29  Camp
 10  Question - Button          30
 11  Transfer Module            31
 12  Guided Tour                32  Remove NPC
 13  Add NPC                    33  Pick One Combat
 14  NPC Says                   34  Teleporter
 15  Encounter                  35  Quest Stage
 16  Utilities                  36  Question - Yes/No
 17  Sounds                     37  Tavern Tales
 18  Who Tries                  38  Special Items
 19  Who Pays
*/
char *ImportFRUAEventNames[MAX_FRUA_EVENT_TYPES] = 
{
   "",
   "Combat", 
   "Text Statement",
   "Give Treasure",
   "Damage",
   "Stairs",
   "Training Hall",
   "Tavern",
   "Shop",
   "Temple",
   "Question Button",
   "Transfer Module",
   "Guided Tour",
   "Add NPC",
   "NPC Says",
   "Encounter",
   "Utilities",
   "Sounds", 
   "Who Tries",
   "Who Pays",
   "Enter Password",
   "Question List",
   "Small Town",
   "Chain", 
   "Vault",
   "Combat Treasure",
   "Gain Experience",
   "Pass Time",
   "", // not used
   "Camp",
   "", // not used
   "", // not used
   "Remove NPC",
   "Pick One Combat",
   "Teleporter",
   "Quest Stage",
   "Question Yes No",
   "Tavern Tales",
   "Special Items"
};
/*
28         Wall Slot #1
                 0 = No Art    5 = Brick      10 = Marble   15 = Lava
                 1 = Rock A    6 = Cave       11 = Tree A   16 = Fire Brick
                 2 = Rock B    7 = Wood A     12 = Tree B
                 3 = Adobe     8 = Wood B     13 = Coral A
                 4 = Webs      9 = Wasteland  14 = Coral B
*/

const char *WallSlotArtStrings[] =
{
   "none",
   "Rock A",
   "Rock B",
   "Adobe",
   "Webs",
   "Brick",
   "Cave",
   "Wood A",
   "Wood B",
   "Wasteland",
   "Marble",
   "Tree A",
   "Tree B",
   "Coral A",
   "Coral B",
   "Lava",
   "Fire Brick"
};

const char *DungeonCombatArtStrings[] =
{
   "none",
   "Rock",
   "Cave",
   "Coral",
   "Adobe",
   "Lava"
};

const char *WildernessCombatArtStrings[] =
{
   "none",
   "River",
   "Forest",
   "Swamp",
   "Wasteland",
   "UserCreated"
};

const char *OverlandWildernessArtStrings[] =
{
   "none",
   "River",
   "Forest",
   "Swamp",
   "Wasteland"
};

const char *RaceStrings[MAX_FRUA_RACES] =
{
   "Elf",
   "Half-Elf",
   "Dwarf",
   "Gnome",
   "Halfling",
   "Human",
   "Monster"
};

const char *ClassStrings[MAX_FRUA_CLASSES] =
{
   "Cleric",
   "Knight",
   "Fighter",
   "Paladin",
   "Ranger",
   "Magic-User",
   "Thief",
   "Monk",
   "Cleric/Fighter",
   "Cleric/Fighter/Magic-User",
   "Cleric/Ranger",
   "Cleric/Magic-User",
   "Cleric/Thief",
   "Fighter/Magic-User",
   "Fighter/Thief",
   "Fighter/Magic-User/Thief",
   "Magic-User/Thief"
};

//  64  Facing Correct Direction
//       * 1 North    * 9  N,W
//       * 2 East     * 10 E,W
//       * 3 N,E      * 11 N,E,W
//       * 4 South    * 12 S,W
//       * 5 N,S      * 13 N,S,W
//       * 6 E,S      * 14 E,S,W
//       * 7 N,E,S    * 15 N,E,S,W
//       * 8 West
const char *DirectionStrings[MAX_FRUA_DIRECTIONS] = 
{
   "North",
   "East",
   "North, East",
   "South",
   "North, South",
   "East, South",
   "North, East, South",
   "West",
   "North, West",
   "East, West",
   "North, East, West",
   "South, West",
   "North, South, West",
   "East, South, West",
   "North, East, South, West"
};

const char *CostModifierStrings[MAX_FRUA_COST_MODIFIERS] =
{
   "Free",     // 0 
   "Div 100",  // 1
   "Div 50",   // 2
   "Div 20",   // 3
   "Div 10",   // 4
   "Div 5",    // 5
   "Div 4",    // 6
   "Div 3",    // 7
   "Div 2",    // 8
   "Div 1.5",  // 9
   "Normal",   // 10
   "Mult 1.5", // 11
   "Mult 2",   // 12
   "Mult 3",   // 13
   "Mult 4",   // 14
   "Mult 5",   // 15
   "Mult 10",  // 16
   "Mult 20",  // 17
   "Mult 50",  // 18
   "Mult 100"  // 19
};


const char *FRUAGetCostModifierText(int value)
{
   if (value < 0) return UnknownString;
   if (value >= MAX_FRUA_COST_MODIFIERS) return UnknownString;
   return CostModifierStrings[value];
}

const char *FRUAGetWallSlotArtName(int slot)
{
   // wall slot == 255 is normal for overland maps
   if (slot == 255)
      return NoneString;
   if ((slot < 0) || (slot > 16) )
      return UnknownString;
   return WallSlotArtStrings[slot];
}

const char *FRUAGetDungeonCombatSlotArtName(int slot)
{
   if (slot == 255)
      return NoneString;

   if ((slot < 0) || (slot > 5) )
      return UnknownString;
   return DungeonCombatArtStrings[slot];
}

const char *FRUAGetWildernessCombatSlotArtName(int slot)
{
   if (slot == 255)
      return NoneString;

   if ((slot < 0) || (slot > 5) )
      return UnknownString;
   return WildernessCombatArtStrings[slot];
}

const char *FRUAGetOverlandWildernessSlotArtName(int slot)
{
   if ((slot < 0) || (slot > 5) )
      return UnknownString;
   return WildernessCombatArtStrings[slot];
}

const char *FRUAGetBackgroundSlotArtName(int slot)
{
   // non-sequential slot values...
   switch (slot)
   {
   case 0:  return "none";
   case 1:  return "Stone Floor";
   case 2:  return "Marble";
   case 3:  return "Cave";
   case 4:  return "Forest";
   case 5:  return "Coral";
   case 6:  return "Lava";
   case 7:  return "Adobe";
   case 8:  return "Wood";
   case 9:  return "Bricks";
   case 10: return "Fire Bricks";
   case 11: return "Swamp";
   case 12: return "Dark Coral";
   case 13: return "Wasteland";
   case 32: return "Outdoor-Grass (Day)";
   case 33: return "Outdoor-Grass (Night)";
   case 34: return "Outdoor-Mud (Day)";
   case 35: return "Outdoor-Mud (Night)";
   case 36: return "Outdoor-Stone (Day)";
   case 37: return "Outdoor-Stone (Night)";
   default: return "none";
   }
}

const char *FRUAGetSoundNameText(int soundCode)
{
   if (soundCode < 0) return UnknownString;
   if (soundCode >= MAX_FRUA_SOUND_VOCAB_STRINGS) return UnknownString;
   return (ImportFRUASoundNames[soundCode]);
}

const char *FRUAGetMonsterNameText(int monsterCode)
{
   if (monsterCode < 0) return UnknownString;
   if (monsterCode >= MAX_FRUA_MONSTERS) return UnknownString;
   return (ImportFRUAMonsterNames[monsterCode]);
}

const char *FRUAGetItemNameText(int nameCode)
{
   if (nameCode < 0) return UnknownString;
   if (nameCode >= MAX_FRUA_ITEM_VOCAB_STRINGS) return UnknownString;
   return (ImportFRUAItemVocab[nameCode]);
}

const char *FRUAGetSpellText(int spellCode)
{
   if (spellCode < 0) return UnknownString;
   if (spellCode >= MAX_FRUA_SPELLS) return UnknownString;
   return (ImportFRUASpellVocab[spellCode]);
}

const char *FRUAGetSpecAbText(int specAbCode)
{
   if (specAbCode < 0) return UnknownString;
   if (specAbCode >= MAX_FRUA_SPEC_AB_VOCAB_STRINGS) return UnknownString;
   return (ImportFRUASpecAbText[specAbCode]);
}

const char *FRUAGetEventTypeText(int eventType)
{
   if (eventType < 0) return UnknownString;
   if (eventType >= MAX_FRUA_EVENT_TYPES) return UnknownString;
   return (ImportFRUAEventNames[eventType]);
}

const char *FRUAGetClassText(int classCode)
{
   if (classCode < 0) return UnknownString;
   if (classCode >= MAX_FRUA_CLASSES) return UnknownString;
   return (ClassStrings[classCode]);
}

const char *FRUAGetDirectionText(int dirCode)
{
   if (dirCode < 0) return UnknownString;
   if (dirCode >= MAX_FRUA_DIRECTIONS) return UnknownString;
   return (DirectionStrings[dirCode]);
}

const char *FRUAGetRaceText(int raceCode)
{
   if (raceCode < 0) return UnknownString;
   if (raceCode >= MAX_FRUA_RACES) return UnknownString;
   return (RaceStrings[raceCode]);
}

const char *FRUAGetBigSmallSpritePicText(int picNum)
{
   for (int i=0; i<NUM_BIGSMALLSPRITE_PICS; i++)
   {
      if (BigSmallSpritePics[i].picNum == picNum)
         return BigSmallSpritePics[i].pString;
   }

   // "none"
   return BigSmallSpritePics[0].pString;
}

const int FRUATranslateBigSmallSpritePicIndexToPicNum(int index)
{
  if ((index >= 0) && (index < NUM_BIGSMALLSPRITE_PICS))
  {
    return (BigSmallSpritePics[index].picNum);
  }  
  return -1;
}

const char *FRUAGetBigSmallSpritePicOriginalFilenameText(int picNum)
{
  static char UAFilename[_MAX_PATH+1];
  strcpy(UAFilename, UnknownString );

  for (int i=0; i<NUM_BIGSMALLSPRITE_PICS; i++)
  {
    if (BigSmallSpritePics[i].picNum == picNum)
    {
       switch (BigSmallSpritePics[i].picType)
       {
       case PICTYPE_SPRITE:
         sprintf(UAFilename, "SPRI%04u.TLB", BigSmallSpritePics[i].picNum);
         break;

       case PICTYPE_BIGPIC:
         sprintf(UAFilename, "BIGP%04u.TLB", BigSmallSpritePics[i].picNum);
         break;

       case PICTYPE_SMALLPIC_A:
         sprintf(UAFilename, "PICA1%03u.TLB", BigSmallSpritePics[i].picNum);
         break;
       case PICTYPE_SMALLPIC_B:
         sprintf(UAFilename, "PICB1%03u.TLB", BigSmallSpritePics[i].picNum);
         break;
       case PICTYPE_SMALLPIC_C:
         sprintf(UAFilename, "PICC1%03u.TLB", BigSmallSpritePics[i].picNum);
         break;
       case PICTYPE_SMALLPIC_D:
         sprintf(UAFilename, "PICD1%03u.TLB", BigSmallSpritePics[i].picNum);
         break;
       case PICTYPE_SMALLPIC_E:
         sprintf(UAFilename, "PICE1%03u.TLB", BigSmallSpritePics[i].picNum);
         break;
       case PICTYPE_SMALLPIC_F:
         sprintf(UAFilename, "PICF1%03u.TLB", BigSmallSpritePics[i].picNum);
         break;
       }
    }
  }

  return UAFilename;
}

const char *FRUAGetWallBlockageText(int blockage)
{
   switch (blockage)
   {
   case 0: return "Open"; 
   case 16: return "Secret"; 
   case 32: return "Locked"; 
   case 48: return "Locked Secret"; 
   case 64: return "Wizard Locked"; 
   case 80: return "Wizard Locked Secret"; 
   case 96: return "Locked Key 1"; 
   case 112: return "Locked Key 2"; 
   case 128: return "Locked Key 3"; 
   case 144: return "Locked Key 4"; 
   case 160: return "Locked Key 5"; 
   case 176: return "Locked Key 6"; 
   case 192: return "Locked Key 7"; 
   case 208: return "Locked Key 8"; 
   case 224: return "Blocked"; 
   //      240 = False Door (16+224)
   case 240: return "Blocked Secret"; 
   default: return UnknownString; 
   }
}
