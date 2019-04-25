/******************************************************************************
* Filename: Globtext.cpp
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

#ifdef UAFEDITOR
#endif
#include "globtext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// The text in this file is used by the editor to populate 
// dialog controls with text equivalent to various enums
//
// The engine does not use this text.
//

const char *SurfaceTypeTexts[] = {
 "CommonDib",
 "CombatDib", 
 "WallDib",
 "DoorDib", 
 "BackGndDib",
 "OverlayDib", 
 "IconDib",
 "OutdoorCombatDib",
 "BigPicDib",
 "MapDib",
 "SmallPicDib",
 "SpriteDib",
 "TitleDib",
 "BufferDib",
 "FontDib",
 "MouseDib",
 "TransBufferDib"
};

const char *MappingTypeText[] = 
{
  "Area Mapping",
  "Auto Mapping",
  "No Mapping"
};

const char *MathOperationText[] = {
  "No Operation",
  "Stored In",
  "Added To",
  "Subtracted From"
};

const char *MultiItemCheckText[] = {
  "No Check",
  "All Items",
  "At Least 1 Item"
};

const char *QuestTypeText[] = {
  "Item",
  "Key",
  "Quest"
};

const char *QuestStateTypeText[] = {
  "Not Started", 
  "In Progress",
  "Complete",
  "Failed"
};

const char *questAcceptText[] = {
  "Impossible",
  "On Yes",
  "On No",
  "On Yes or No",
  "Impossible (No Question)",
  "Automatic (No Question)"
};

const char *EquipStartText[] = {
  "None",
  "Poor",
  "Modest",
  "Average",
  "Prosperous",
  "+1",
  "+2",
  "+3",
  "+4"
};

const char *CreatureSizeText[] = {
  "Small",
  "Medium",
  "Large"
};

const char *EventListText[] = {
	"No Event",
	"Add NPC",
	"Camp",
	"Chain",
	"Combat",
	"Combat Treasure",
	"Damage",
	"Encounter",
	"Enter Password",
	"Gain Experience",
	"Give Treasure",
	"Guided Tour",
	"Inn (Obsolete)",
	"NPC Says",
	"Pass Time",
	"Pick One Combat",
	"Quest Stage",
	"Question Button",
	"Question List",
	"Question Yes/No",
	"Remove NPC",
	"Shop",
	"Small Town",
	"Sounds",
	"Special Item",
	"Stairs",
	"Tavern",
	"Tavern Tales",
	"Teleporter",
	"Temple",
	"Text Statement",
	"Training Hall",
	"Transfer Module",
	"Utilities",
	"Vault",
	"Who Pays",
	"Who Tries",
  "Take Party Items",
  "Heal Party",
  "Logic Block",
  "GPDL",
  "Random Event",
  "Play Movie",
  "Add To Journal"
};

const int numEventListText = sizeof(EventListText)/sizeof(EventListText[0]);

const char *TakeItemsQtyText[] = {
  "Specified",
  "Random",
  "Percent",
  "All"
};

const char *TakeWhatText[] = {
  "Inventory",
  "Money",
  "Gems",
  "Jewelry"
};

const char *itemClassText[] = {
  "ItemType00",
  "ItemType01",
  "ItemType00",
  "ItemType02",
  "ItemType03",
  "ItemType04",
  "ItemType05",
  "ItemType06",
  "ItemType07",
  "ItemType08",
  "ItemType09",
  "ItemType10",
  "ItemType11",
  "ItemType12",
  "ItemType13",
  "ItemType14",
  "ItemType15",
  "ItemType16"
};

const char *LiteralOrPercentText[] = {
  "Add to Current",
  "Add Percent of Max",
  "Set to Percent of Max"
};

const char *TaleOrderTypeText[] = {
  "In Order",
  "Random"
};


const char *TavernTalesFlagsText[] = {
  "Cumulative",
  "If drink",
  "If drunk",
  "Replace",
  "Highlight"
};
extern const int NumTavernTalesFlags = sizeof (TavernTalesFlagsText) / sizeof (TavernTalesFlagsText[0]);


const char *TakeItemsAffectsText[] = {
  "All Characters",
  "Random Character",
  "Active Character",
};

const char *DistanceText[] = {
  "Up Close",
  "Nearby",
	"Far Away"
};

const char *FacingText[] = {
	"North",
	"East",
	"South",
	"West",
  "Unchanged"
};

const char *RelativeDirectionText[] = {
    "Front - North",  // Facing north
    "Right - East",
    "Rear - South",
    "Left - West",

    "Front - East",   // Facing east
    "Right - South",
    "Rear - West",
    "Left - North",

    "Front - South",  // Facing south
    "Right - West",
    "Rear - North",
    "Left - East",

    "Front - West",   // Facing west
    "Right - North",
    "Rear - East",
    "Left - South"
};



const char *DirectionText[] = {
	"Any/All Side(s)",
	"North",
	"South",
	"East",
	"West",
	"North, South",
	"North, East",
	"North, West",
	"South, East",
	"South, West",
	"East,  West",
	"North, South, East",
	"North, South, West",
	"North, West,  East",
	"West,  South, East",
	"In Front Of"
};

const char *SurpriseText[] = {
	"Neither",
	"Party",
	"Monster"
};

const char *AffectsWhoText[] = {
	"None",
	"Entire Party",
	"Active Char",
	"One Char at Random",
	"Chance on each Char"
};

const char *TurnModText[] = {
	"None",
	"Hard",
	"Difficult",
	"Impossible"
};

const char *SaveVersusText[] = {
	"Paralysis/Poison/Death Magic",
	"Petrification/Polymorph",
	"Rod/Staff/Wand",
	"Spell",
	"Breath Weapon"
};

const char *SaveEffectText[] = {
	"No Save",
	"Save Negates",
	"Save for Half",
	"Use Player THAC0"
};

const char *spellClassText[] = {
   "Magic User",
   "Cleric"
};

const char *spellDurationText[] = {
   "in rounds",
   "by damage taken",
   "in hours",
   "in days",
   "permanent",//"instantaneous"
   "by nbr attacks"
};

const char *spellTargetingText[] = 
{
   "Self",
   "Selected by Count",
   "Whole Party",
   "Touched Targets",
   "Area: Circle",
   "Selected by Hit Dice",
   "Area: Line, Pick Start",
   "Area: Line, Pick End",
   "Area: Square",
   "Area: Cone"
};

const char *spellCastingTimeText[] = 
{
  "Immediate",
  "Initiative",
  "Rounds",
  "Turns"
};


const char *ButtonOptionText[] = {
	"No Result",
	"Decrease Range",
	"Combat, No Surprise",
	"Combat, Slow Party Surprised",
	"Combat, Slow Monsters Surprised",
	"Talk",
	"Escape if Fast Party, Else Combat",
	"Chain to Event"
};

const char *ButtonPostChainOptionText[] = {
  "Do Nothing",
  "Return To Question",
  "Backup One Step"
};

const char *ChainTriggerText[] = {
	"Always",
	"If Event Happens",
	"If Event does not happen"
};

#ifdef UAFEDITOR
const char *EventTriggerText[] = {
	"Always",
	"Party has item",
	"Party NOT have item",
	"Daytime",
	"Nighttime",
	"Random Chance",
	"Party is Searching",
	"Party is NOT Searching",
	"Facing dir, only when arrive",
	"Quest is complete",
	"Quest failed",
	"Quest in Progress",
	"Party detecting traps",
	"Party NOT detecting traps",
	"Party can see invisible",
	"Party cannot see invisible",
	"Specific Class in Party",
	"Specific Class NOT in Party",
	"Specific Race in Party",
	"Specific Race NOT in Party",
  "Quest present in Party",
  "Quest NOT present in Party",
  "Gender in Party",
  "Gender is NOT in Party",
  "Facing dir, even after arrive",
  "NPC in Party",
  "NPC is NOT in Party",
  "Execute GPDL function",
  "Party has spell memorized",
  "Party at x,y",
  "Specific Baseclass in Party",
  "Specific Baseclass NOT in Party",
  "Party has Special Item",
  "Party NOT have Special Item",
  "Party has Special Key",
  "Party NOT have Special Key"
};
#endif
#ifdef UAFEDITOR
const char *RaceText[] = {
	"Human",
	"Elf",
	"Dwarf",
	"Gnome",
	"Half Elf",
	"Halfling"
};

const char *ClassText[] = {
	"Fighter",
	"Cleric",
	"Ranger",
	"Paladin",
	"Magic User",
	"Thief",
	"Druid",
  "Cleric/Fighter",
  "Cleric/Fighter/Magic User",
  "Cleric/Ranger",
  "Cleric/Magic User",
  "Cleric/Thief",
  "Fighter/Magic User",
  "Fighter/Thief",
  "Fighter/Magic User/Thief",
  "Magic User/Thief",
  "Fighter/Druid",
  "Druid/Magic User",
  "Fighter/Magic User/Druid"
};

extern const int NumClassText = sizeof(ClassText)/sizeof(ClassText[0]);
#endif

const char *PasswordActionText[] = {
	"No Action",
	"Chain Event",
	"Teleport",
	"Backup one step"
};

const int NUM_PASSMATCH_TYPES = 3;

const char *PasswordMatchText[] = {
	"Exact Match",
	"Pswd is substring of typed",
	"Typed is substring of pswd"
};

const char *StepText[] = {
	"No Action",
  "Pause",
	"Forward",
	"Turn Left",
	"Turn Right"
};

const char *BlockageText[] = {
  "Open",
  "Open, Secret",
  "Blocked",
  "False Door",
  "Locked",
  "Locked, Secret",
  "Locked, Wizard",
  "Locked, Wizard, Secret",
  "Locked, Key 1",
  "Locked, Key 2",
  "Locked, Key 3",
  "Locked, Key 4",
  "Locked, Key 5",
  "Locked, Key 6",
  "Locked, Key 7",
  "Locked, Key 8"
};


/*
const char *LocationCarriedText[] = {
  "Weapon Hand",
  "Shield Hand",
  "Body (Armor)",
  "Hands",
  "Head",
  "Waist",
  "Body (Robe)",
  "Back (Cloak)",
  "Feet",
  "Fingers",
  "Ammo Quiver",
  "Not Readied",
  "Arms",
  "Legs",
  "Face",
  "Neck",
  "Pack",
};
*/

const char *spellAbilitiesText[] = 
{
   "None",
   "Bless",
   "Curse",
   "Fear, (Undead only)",
   "Enlarge",
   "Reduce",
   "Charm Person",
   "Detect Magic",
   "Reflect Gaze Attack",
   "Prot from Evil",
   "Prot from Good",
   "Shield",
   "Sleep",
   "Fog",
   "Entangle",
   "Invisible to Animals",
   "Invisible to Undead",
   "Fear, (except Undead)",
   "Sanctuary",
   "Shillelagh",
   "Displacement",
   "Wizardry",
   "Vorpal Attack",
   "Hold Person",
   "Silenced",
   "Poisoned",
   "Slow Poison",
   "Mirror Image",
   "Invisible",
   "Enfeebled",
   "Blinded",
   "Diseased"
};
/*
const char *itemAbilitiesText[] = {
   "None",
   "Displacement",
   "Drow item",
   "Wizardry",
   "Hornets Nest",
   "Vorpal Weapon"
};
*/

const char *alignmentText[] = {
   "Lawful Good",
   "Neutral Good",
   "Chaotic Good",
   "Lawful Neutral",
   "True Neutral",
   "Chaotic Neutral",
   "Lawful Evil",
   "Neutral Evil",
   "Chaotic Evil"
};

const char *CharStatusTypeText[] =
{
  "OKAY", 
  "UNCONSCIOUS", 
  "DEAD", 
  "FLED", 
  "PETRIFIED", 
  "GONE", 
  "ANIMATED", 
  "TEMP GONE",
  "RUNNING", 
  "DYING"  
};



const char *genderText[] = {
   "Male",
   "Female",
   "Bishop",
};

const char *costFactorText[] = {
  "Free",
  "Div 100",
  "Div 50",
  "Div 20",
  "Div 10",
  "Div 5",
  "Div 4",
  "Div 3",
  "Div 2",
  "Div 1.5",
  "Normal",
  "Mult 1.5",
  "Mult 2",
  "Mult 3",
  "Mult 4",
  "Mult 5",
  "Mult 10",
  "Mult 20",
  "Mult 50",
  "Mult 100"
};

const char *GlobalAVStyleText[] = 
{
  "Any View",
  "Area/Large View Only",
  "3D/Small View Only"
};

#ifdef UAFEDITOR
const char *UndeadTypeText[] =
{
  "Not Undead",
  "Skeleton",
  "Zombie",
  "Ghoul",
  "Shadow",
  "Wight",
  "Ghast",
  "Wraith",
  "Mummy",
  "Spectre",
  "Vampire",
  "Ghost",
  "Lich",
  "Special"
};
#endif
const char *SpecAbMsgText[] = 
{
  "None",
  "Begin Casting Spell",
  "Cast Spell",
  "Flee",
  "Turn Undead",
  "Bandage",
  "Guard",
  "Attack",
  "Move",
  "End Turn",
  "Delay Turn",
  "Ready Item"
};

