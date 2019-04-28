#include "StdAfx.h"



const char *ChainTriggerText[] = {
	"Always",
	"If Event Happens",
	"If Event does not happen"
};

 //EventTriggerText
 //DistanceText
 //DirectionText
const char *MappingTypeText[] =
{
  "Area Mapping",
  "Auto Mapping",
  "No Mapping"
};
// TurnModText
// ButtonPostChainOptionText
// PasswordActionText
// AffectsWhoText
// SaveEffectText
// SaveVsText
// TakeItemsAffectsText
// TakeItemsQtyText
// TaleOrderTypeText
// costFactorText
const char *MathOperationText[] = {
  "No Operation",
  "Stored In",
  "Added To",
  "Subtracted From"
};
// MultiItemCheckText
// genderText
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
 //QuestStateTypeText
 //CreatureSizeText
 //alignmentText
 //FacingText
 //StepText
 //ButtonOptionText
 //GlobalAVStyleText
 //BlockageText
 //itemClassText
 //EventListText

const char *MultiItemCheckText[] = {
  "No Check",
  "All Items",
  "At Least 1 Item"
};

const char *QuestTypeText[] = {
  "None",
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

const char *SaveVsText[] = {
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

const char *PasswordActionText[] = {
	"No Action",
	"Chain Event",
	"Teleport",
	"Backup one step"
};


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




void JWriter::String(const char *str)
{
  if (strchr(str, '"') != NULL)
  {
    CString s(str);
    s.Replace("\"", "\\\"");
    m_f->Write(s, s.GetLength());
  }
  else
  {
    m_f->Write(str, strlen(str));
  };
}

void JWriter::QuotedString(const char *str)
{
  m_f->Write("\"",1);
  String(str);
  m_f->Write("\"",1);
}

void JWriter::Tab(void)
{
  if (linefeed)
  {
    m_f->Write(tabs, JWTAB*indent);
  };
}

void JWriter::MaybeComma(void)
{
  if (lineCount[indent] > 0)
  {
    m_f->Write(",", 1);
  };
  lineCount[indent]++;
  if (linefeed)
  {
    m_f->Write("\n",1);
  };
}

void JWriter::Name(const char *name)
{
  if (name != NULL)
  {
    if (name[0] == 0)
    {
      die("Bad item name");
    };
    MaybeComma();
    Tab();
    QuotedString(name);
    Colon();
  };
}

bool JReader::NameAndDice(const char *name, int& n, int& s, int& b)
{
  CString v;
  if (!NameAndValue(name, v)) return false;
  const char *p = (LPCSTR)v;
  sscanf(p,"%dd%d+%d", &n, &s, &b);
  return true;
}


void JWriter::NameAndValue(const char *name, const CString& value)
{
  NameAndQuotedValue(name, value);
}

void JWriter::NameAndBinary(const char *name, const CString& value)
{
  int i, len, col;
  char result[81];
  len = value.GetLength();
  StartArray(name);
  i = 0;
  while (i < len)
  {
    int n;
    n = len-i;
    if (n >= 80) n = 40;
    else if (n > 40) n = n/2;
    NextEntry();
    col = 0;
    while (n > 0)
    {
      unsigned int c;
      c = value[i] & 0xff;
      sprintf(result+col, "%02x", c);
      i++;
      col +=2;
      n--;
    };
    QuotedString(result);
  }
  EndArray();
};

void JWriter::NameAndQuotedValue(const char *name, const CString& value)
{
  Name(name);
  QuotedString((LPCSTR)value);
}

void JWriter::NameAndQuotedNonBlank(const char *name, const CString& value)
{
  if (value.GetLength() == 0) return;
  Name(name);
  QuotedString((LPCSTR)value);
}

void JWriter::NameAndNonBlank(const char *name, const CString& value)
{
  if (value.GetLength() == 0) return;
  NameAndValue(name, value);
}

void JWriter::NameAndValue(const char *name, const int& value)
{
  char v[50];
  sprintf(v,"%d", value);
  NameAndValue(name,v);
}

void JWriter::NameAndValue(const char *name, const long& value)
{
  char v[50];
  sprintf(v,"%d", value);
  NameAndValue(name,v);
}

void JWriter::NameAndValue(const char *name, const unsigned char& value)
{
  char v[50];
  sprintf(v,"%d", value);
  NameAndValue(name,v);
}

void JWriter::NameAndValue(const char *name, const WORD& value)
{
  char v[50];
  sprintf(v,"%d", value);
  NameAndValue(name,v);
}

void JWriter::NameAndValue(const char *name, const DWORD& value)
{
  char v[50];
  sprintf(v,"%u", value);
  NameAndValue(name,v);
}

void JWriter::NameAndValue(const char *name, const LONGLONG& value)
{
  char v[50];
  sprintf(v,"%I64d", value);
  NameAndValue(name,v);
}


void JWriter::NameAndValue(const char *name, const double& value)
{
  char v[50];
  sprintf(v,"%f", value);
  NameAndValue(name, v);
}

void JWriter::NameAndValue(const char *name, const float& value)
{
  char v[50];
  sprintf(v,"%f", value);
  NameAndValue(name, v);
}

void JWriter::NameAndBool(const char *name, const bool& value)
{
  NameAndValue(name, value?"true":"false");
}

void JWriter::NameAndEnum(const char *name, const DWORD& value, DWORD numValue, const char *text[])
{
  if (value >= numValue)
  {
    errorStatus = ES_illegalValue;
    return;
  };
  NameAndValue(name, text[value]);
}

void JWriter::NameAndEnum(const char *name, const BYTE& value, DWORD numValue, const char *text[])
{
  if (value >= numValue)
  {
    errorStatus = ES_illegalValue;
    return;
  };
  NameAndValue(name, text[value]);
}

void JWriter::NameAndEnum(const char *name, const int& value, DWORD numValue, const char *text[])
{
  if ((unsigned)value >= numValue)
  {
    errorStatus = ES_illegalValue;
    return;
  };
  NameAndValue(name, text[value]);
}

void JWriter::NameAndEnum(const char *name, const chainTriggerType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_CHAINTRIG_TYPES, ChainTriggerText);
}

void JWriter::NameAndEnum(const char *name, const eventTriggerType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_EVENTTRIG_TYPES, EventTriggerText);
}

void JWriter::NameAndEnum(const char *name, const eventDistType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_DIST_TYPES, DistanceText);
}

void JWriter::NameAndEnum(const char *name, const QuestAcceptType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, MAX_QUEST_ACCEPT_TYPES, questAcceptText);
}

void JWriter::NameAndEnum(const char *name, const eventDirType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_DIR_TYPES, DirectionText);
}

extern const char *MappingTypeText[];

void JWriter::NameAndEnum(const char *name, const MappingType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_MAPPING_TYPES, MappingTypeText);
}

void JWriter::NameAndEnum(const char *name, const eventSurpriseType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_SURPRISE_TYPES, SurpriseText);
}

void JWriter::NameAndEnum(const char *name, const eventTurnUndeadModType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_TURN_MOD_TYPES, TurnModText);
}

void JWriter::NameAndEnum(const char *name, const labelPostChainOptionsType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_LABEL_POST_CHAIN_OPTION_TYPES, ButtonPostChainOptionText);
}

void JWriter::NameAndEnum(const char *name, const passwordActionType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_PASSACTION_TYPES, PasswordActionText);
}

void JWriter::NameAndEnum(const char *name, const eventPartyAffectType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_AFFECT_TYPES, AffectsWhoText);
}

void JWriter::NameAndEnum(const char *name, const spellSaveEffectType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_SAVE_EFFECT_TYPES, SaveEffectText);
}

void JWriter::NameAndEnum(const char *name, const spellSaveVsType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_SAVE_VS_TYPES, SaveVsText);
}

void JWriter::NameAndEnum(const char *name, const takeItemsAffectsType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_TAKEAFFECTS_TYPES, TakeItemsAffectsText);
}

void JWriter::NameAndEnum(const char *name, const takeItemQtyType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_TAKEITEMQTY_TYPES, TakeItemsQtyText);
}

void JWriter::NameAndEnum(const char *name, const taleOrderType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_TALEORDER_TYPES, TaleOrderTypeText);
}

void JWriter::NameAndEnum(const char *name, const costFactorType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_COST_FACTOR_TYPES, costFactorText);
}

void JWriter::NameAndEnum(const char *name, const MathOperationType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, MAX_MATH_OPERATION_TYPES, MathOperationText);
}

void JWriter::NameAndEnum(const char *name, const MultiItemCheckType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, MAX_MULTI_ITEM_CHECK_TYPES, MultiItemCheckText);
}

void JWriter::NameAndEnum(const char *name, const genderType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_GENDER_TYPES, genderText);
}


extern const char *SurfaceTypeTexts[];

void JWriter::NameAndEnum(const char *name, const SurfaceType& value)
{
  DWORD v;
  v = value;
  NameAndFlags(name, v, NUM_SURFACE_TYPES, SurfaceTypeTexts);
}


extern const char *QuestStateTypeText[];

void JWriter::NameAndEnum(const char *name, const QuestStateType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_QUEST_STATE_TYPE, QuestStateTypeText);
}

extern const char *CreatureSizeText[];
void JWriter::NameAndEnum(const char *name, const creatureSizeType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_SIZE_TYPES, CreatureSizeText);
}

void JWriter::NameAndEnum(const char *name, const alignmentType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_ALIGNMENT_TYPES, alignmentText);
}

void JWriter::NameAndEnum(const char *name, const eventFacingType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_FACING_TYPES, FacingText);
}

void JWriter::NameAndEnum(const char *name, const eventStepType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_STEP_TYPES, StepText);
}

void JWriter::NameAndEnum(const char *name, const encounterButtonResultType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_ENCOUNTER_BUTTON_OPTION_TYPES, ButtonOptionText);
}

void JWriter::NameAndEnum(const char *name, const GlobalAreaViewStyle& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, MAX_GLOBAL_AVSTYLES, GlobalAVStyleText);
}

void JWriter::NameAndEnum(const char *name, const BlockageType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_BLOCKAGE_TYPES, BlockageText);
}

void JWriter::NameAndEnum(const char *name, const itemClassType& value)
{
  DWORD temp = value;
  NameAndEnum(name, temp, NUM_ITEMCLASS_TYPES, itemClassText);
}

//void JWriter::NameAndEnum(const char *name, const takeItemQtyType& value)
//{
//  DWORD temp = value;
//  NameAndEnum(name, temp, NUM_TAKEITEMQTY_TYPES, TakeItemsQtyText);
//}




void JWriter::NameAndFlags(const char *name, const DWORD& value, int num, const char *text[])
{
  int i;
  DWORD v;
  v = value;
  StartArray(name);
  Linefeed(false);
  for (i=0; i<num; i++, v>>=1)
  {
    if (v & 1)
    {
      NextEntry();
      String(text[i]);
    };
  }
  EndArray();
  Linefeed(true);
};

void JWriter::NameAndFlags(const char *name, const BYTE& value, int num, const char *text[])
{
  DWORD v;
  v = value;
  NameAndFlags(name, v, num, text);
};

void JWriter::NameAndDice(const char *name, const int& n, const int& s, const int& b)
{
  char v[80];
  sprintf(v,"%dd%d+%d",n,s,b);
  NameAndValue(name, v);
}

void JWriter::NameAndValue(const char *name, const int *value, int num)
{
  int i;
  Name(name);
  String("[");
  for (i=0; i<num; i++)
  {
    char s[20];
    if (i != 0) String(",");
    sprintf(s,"%d",value[i]);
    String(s);
  };
  String("]");
}

void JWriter::NameAndValue(const char *name, const char *value, int num)
{
  int i;
  Name(name);
  String("[");
  for (i=0; i<num; i++)
  {
    char s[20];
    if (i != 0) String(",");
    sprintf(s,"%d",value[i]);
    String(s);
  };
  String("]");
}

void JWriter::OpenBrace(void)
{
  m_f->Write("{",1);
  indent++;
  lineCount.SetAtGrow(indent, 0);
}

void JWriter::OpenBracket(void)
{
  m_f->Write("[",1);
  indent++;
  lineCount.SetAtGrow(indent, 0);
}


void JWriter::Colon(void)
{
  String(":");
}

void JWriter::NewLine(void)
{
  m_f->Write("\n",1);
  Tab();
}

void JWriter::StartList(const char *name)
{
  if (name[0] != 0)
  {
    Name(name);
  };
  OpenBrace();
}

void JWriter::StartList(void)
{
  OpenBrace();
}

void JWriter::StartArray(const char *name)
{
  Name(name);
  OpenBracket();
}

void JWriter::StartArray()
{
  OpenBracket();
}

void JWriter::NextEntry(void)
{
  if (lineCount[indent] != 0)
  {
    m_f->Write(",",1);
  };
  lineCount[indent]++;
  if (linefeed)
  {
    m_f->Write("\n",1);
    Tab();
  };
}

void JWriter::EndList(void)
{
  indent--;
  if (linefeed && (lineCount[indent+1]>0))
  {
    m_f->Write("\n",1);
    Tab();
  };
  m_f->Write("}", 1);
}



void JWriter::EndArray(void)
{
  indent--;
  if (linefeed && (lineCount[indent+1]>0))
  {
    m_f->Write("\n",1);
    Tab();
  };
  m_f->Write("]", 1);
}


void JReader::SetError(ES e)
{
  errStat = e;
  errLine = lineNum;
  if (lineNum != 0) return;
  if (pCurRoot != NULL) errLine = pCurRoot->lineNum;
}

ES JReader::GetError(void)
{
  return errStat;
}

CString JReader::GetErrorMessage(void)
{
  CString errorCode;
  CString result;
  switch (errStat)
  {
  case ES_ok: errorCode = "No Error"; break;
  case ES_EOF: errorCode = "Unexpected End of File"; break;
  case ES_syntax: errorCode = "Syntax Error"; break;
  case ES_missingColon: errorCode = "Missing Colon"; break;
  case ES_newlineInString: errorCode = "Newline in String"; break;
  case ES_missingList: errorCode = "Missing List"; break;
  case ES_missingArray: errorCode = "Missing an array"; break;
  case ES_illegalValue: errorCode = "Illegal Value"; break;
  case ES_missingData: errorCode = "Missing data"; break;
  default:
    errorCode = "Unknown Error Code";
  };
  result.Format("Error Code = %s\nError at line number %d\n%s", errorCode, errLine, errMessage);
  return result;
}

void JReader::PrintError(const char *message) const
{
  int line;
  printf("%s\n", message);
  fprintf(errors, "%s\n", message);
  JReader temp(*this);
  line = temp.pCurRoot->lineNum;
  if (temp.pCurRoot->listValue != NULL)
  {
    line = temp.pCurRoot->listValue->lineNum;
  }
  else if (temp.pCurRoot->arrayValue != NULL)
  {
    line = temp.pCurRoot->arrayValue->lineNum;
  };
  temp.SetError(ES_missingData);
  printf("   Error occurred at line number %d\n\n", line);
  fprintf(errors, "   Error occurred at line number %d\n\n", line);
}


JReader::JReader(const JReader& jr)
{
  *this = jr;
  m_thisIsACopy = true;
}

void JReader::operator =(const JReader& jr)
{
  m_f = jr.m_f;
  pCurRoot = jr.pCurRoot;
  pCurArrayEntry = jr.pCurArrayEntry;
  m_root = jr.m_root;
  m_optional = false;
  lineNum = jr.lineNum;
  errLine = jr.errLine;
  rootStackSize = jr.rootStackSize;
  rootStack.RemoveAll();
  rootStack.Append(jr.rootStack);
  errors = jr.errors;
}


void JReader::JRError(const char *msg)
{
  errMessage = msg;
  throw 2;
}

BOOL JReader::Whitespace(void)
{
  return (thisChar==' ')||(thisChar=='\t')||(thisChar=='\n');
}



void JReader::GetChar(void)
{
  if (nextCharValid)
  {
    thisChar = nextChar;
    nextCharValid = false;
  }
  else
  {
    if (inputCol >= inBufSize)
    {
      inBufSize = m_f->Read(inBuf,1000);
      inputCol = 0;
    };
    if (inputCol >= inBufSize) 
    {
      SetError(ES_EOF);
    };
  };
  thisChar = inBuf[inputCol++];
  if (thisChar == '\n')
  {
    lineNum++;
    if ((lineNum % 1000) == 0)
    {
      printf("%d lines\n", lineNum);
    };
  };
  return;
}

void JReader::EndList(DEBUG_ID dbid)
{
  rootStackSize--;
  if (rootStackSize < 0)
  {
    die("Import Internal Error");
  };
  pCurArrayEntry = pCurRoot;
  pCurRoot = rootStack[rootStackSize];
}

bool JReader::EndArray(void)
{
  rootStackSize--;
  if (rootStackSize < 0)
  {
    die("Import Internal Error");
  };
  pCurArrayEntry = pCurRoot;
  pCurRoot = rootStack[rootStackSize];
  return true;
}

void JReader::QuotedString(char term)
{
  tokenSize = 0;
  for (;;)
  {
    GetChar();
    if (GetError() != ES_ok) return;
    if (thisChar == '\n')
    {
      SetError(ES_newlineInString);
      return;
    };
    if (thisChar == '\\')
    {
      GetChar();
      if (GetError() != ES_ok) return;
    }
    else
    {
      if (thisChar == term)
      {
        token = CString(tokenArray.GetData(), tokenSize);
        tokenType = TT_String;
        return;
      };
    };
    tokenArray.SetAtGrow(tokenSize, thisChar);
    tokenSize++;
  };
}

void JReader::GetToken(void)
{
  do
  {
    GetChar();
    if (GetError() != ES_ok) return;
  } while (Whitespace());
  if (thisChar == '"')
  {
    QuotedString('"');
    return;
  }
  if (thisChar == '[')
  {
    tokenType = TT_OpenBracket;
    return;
  }
  else if (thisChar == ']')
  {
    tokenType = TT_CloseBracket;
    return;
  }
  else if (thisChar == '{')
  {
    tokenType = TT_OpenBrace;
    return;
  }
  else if (thisChar == ':')
  {
    tokenType = TT_Colon;
    return;
  }
  else if (thisChar == '}')
  {
    tokenType = TT_CloseBrace;
    return;
  }
  else if (thisChar == ',')
  {
    tokenType = TT_Comma;
    return;
  }
  else
  {
    tokenSize = 0;
    do
    {
      if (thisChar == '\\')
      {
        GetChar();
        if (GetError() != ES_ok) return;
      };
      tokenArray.SetAtGrow(tokenSize++, thisChar);
      GetChar();
      if (GetError() != ES_ok) return;
      if (   (thisChar == ']')
          || (thisChar == '}')
          || (thisChar == ':')
          || (thisChar == ',')
         )
      {
        nextChar = thisChar;
        nextCharValid = TRUE;
        break;
      }
    } while (!Whitespace());
  };
  token = CString(tokenArray.GetData(), tokenSize);
  tokenType = TT_String;
  return;
}

JR_LIST::~JR_LIST(void)
{
  static int count = 0;
  count++;
  if ((count%1000) == 0)
  {
    printf(".");
  };
  if ((count % 40000) == 0)
  {
    printf("\n");
  };
  JR_DATA *pData;
  while (first != NULL)
  {
    pData = first;
    first = pData->next;
    pData->next = NULL;
    delete pData;
  };
}

JR_DATA::~JR_DATA(void)
{
  if (listValue != NULL)
  {
    delete listValue;
    listValue = NULL;
  };
  if (arrayValue != NULL) 
  {
    delete arrayValue;
    arrayValue = NULL;
  };
  if (next != NULL) 
  {
    die("Internal Destructor error");
  };
}


JReader::~JReader(void)
{
  Clear();
  if (!m_thisIsACopy)
  {
    fclose(errors);
  };
}

void JReader::Clear(void)
{
  if (!m_thisIsACopy)
  {
    delete m_root;
    m_root = NULL;
  };
}


void JReader::GetParseList(JR_LIST **pList)
{ // We gobble up the closeBrace.
  if (*pList != NULL) die("Internal Import Level error");
  // Loop while we get a name:value pair.  Stop at a closeBrace.
  *pList = new JR_LIST;
  (*pList)->lineNum = lineNum;
  for (;;)
  {
    GetToken();
    if (GetError() != ES_ok) return;
    if (tokenType == TT_CloseBrace)
    {
      return;
    };
    // Get the name.
    if (tokenType != TT_String)
    {
      SetError(ES_syntax);
      return;
    };
    // Name must be followed by a colon.
    {
      JR_DATA data; // Accumulate it locally utile it is complete.
      data.name = token;
      GetToken();
      if (GetError() != ES_ok) return;
      if (tokenType != TT_Colon)
      {
        SetError(ES_missingColon);
        return;
      };
      GetToken();
      if (GetError() != ES_ok) return;
      if (tokenType == TT_String)
      { // A simple node.
        data.stringValue = token;
      }
      else if (tokenType == TT_OpenBrace)
      {
        GetParseList(&data.listValue);
        if (GetError() != ES_ok) return;
      }
      else if (tokenType == TT_OpenBracket)
      {
        GetParseArray(&data.arrayValue);
        if (GetError() != ES_ok) return;
      };
      {
        JR_DATA *pData;
        pData = new JR_DATA;
        pData->lineNum = lineNum;
        pData->name = data.name;
        pData->stringValue = data.stringValue;
        pData->listValue = data.listValue;
        data.listValue = NULL;
        pData->arrayValue = data.arrayValue;
        data.arrayValue = NULL;
        if ((*pList)->first == NULL)
        {
          (*pList)->first = pData;
        }
        else
        {
          (*pList)->last->next = pData;
        };
        (*pList)->last = pData;
      }
      GetToken();
      if (GetError() != ES_ok) return;
      if (tokenType == TT_Comma) continue;
      else if (tokenType == TT_CloseBrace)
      {
        return;
      }
      else
      {
        SetError(ES_syntax);
        JRError("List not properly terminated");
        return;
      };
    };
  }
}

void JReader::GetParseArray(JR_LIST **pList)
{ // We gobble up the closeBracket.
  if (*pList != NULL) die("Internal Import Level error");
  // Loop while we get a new value.  Stop at a closeBracket.
  *pList = new JR_LIST;
  (*pList)->lineNum = lineNum;
  for (;;)
  {
    GetToken();
    if (GetError() != ES_ok) return;
    if (tokenType == TT_CloseBracket)
    {
      return;
    }
    {
      JR_DATA data; // Accumulate it locally until it is complete.
      // Name is empty.
      if (GetError() != ES_ok) return;
      if (tokenType == TT_String)
      { // A simple node.
        data.stringValue = token;
      }
      else if (tokenType == TT_OpenBrace)
      {
        GetParseList(&data.listValue);
        if (GetError() != ES_ok) return;
      }
      else if (tokenType == TT_OpenBracket)
      {
        GetParseArray(&data.arrayValue);
        if (GetError() != ES_ok) return;
      };
      {
        JR_DATA *pData;
        pData = new JR_DATA;
        pData->lineNum = lineNum;
        pData->name = data.name;
        pData->stringValue = data.stringValue;
        pData->listValue = data.listValue;
        data.listValue = NULL;
        pData->arrayValue = data.arrayValue;
        data.arrayValue = NULL;
        if ((*pList)->first == NULL)
        {
          (*pList)->first = pData;
        }
        else
        {
          (*pList)->last->next = pData;
        };
        (*pList)->last = pData;
      }
      GetToken();
      if (GetError() != ES_ok) return;
      if (tokenType == TT_Comma) continue;
      else if (tokenType == TT_CloseBracket)
      {
        return;
      }
      else
      {
        SetError(ES_syntax);
        JRError("Array not properly terminated");
        return;
      };
    };
  }
}

int JReader::Initialize(void)
{
  // Parse the entire file into a tree.
  nextCharValid = FALSE;
  lineNum = 1;
  inputCol = 0;
  inBufSize = 0;
  SetError(ES_ok);
  m_root = new JR_DATA;
  m_root->lineNum = lineNum;
  pCurRoot = m_root;
  rootStackSize = 0;
  GetToken();
  if (tokenType != TT_OpenBrace)
  {
    SetError(ES_syntax);
    JRError("File missing open '{'");
    return GetError();
  };
  GetParseList(&m_root->listValue); if (GetError()!=ES_ok) return GetError();
  return GetError();
};

JR_DATA root;

void JReader::Rewind(void)
{
  pCurRoot = m_root;
  pCurArrayEntry = NULL;
  inBufSize = 0;
  inputCol = 0;
}


JR_DATA *JReader::Locate(const char *name) const
{
  JR_DATA *result;
  if (pCurRoot->listValue != NULL)
  {
    result = pCurRoot->listValue->first;
    while (result != NULL)
    {
      if (result->name == name)
      {
        return result;
      };
      result = result->next;
    };
  };
  if (!m_optional)
  {
    CString msg;
    msg.Format("Cannot find data named %s", name);
    PrintError(msg);
  };
  return NULL;
};


CString JReader::CurrentTag(void)
{
  if ( (pCurRoot == NULL) || (pCurRoot->listValue == NULL))
  {
    die("'CurrentTag' called when no list exists");
  };
  if (pCurRoot->listValue->current == NULL)
  {
    return "";
  }
  else
  {
    return pCurRoot->listValue->current->name;
  };
}

CString JReader::CurrentValue(void)
{
  if ( (pCurRoot == NULL) || (pCurRoot->listValue == NULL))
  {
    die("'CurrentTag' called when no list exists");
  };
  if (pCurRoot->listValue->current == NULL)
  {
    return "";
  }
  else
  {
    return pCurRoot->listValue->current->stringValue;
  };
}

void JReader::NextTag(void)
{
  if ( (pCurRoot == NULL) || (pCurRoot->listValue == NULL))
  {
    die("'NextTag' called when no list exists");
  };
  if (pCurRoot->listValue->current == NULL) return;
  pCurRoot->listValue->current = pCurRoot->listValue->current->next;
}

bool JReader::NextEntry(void)
{
  if (pCurArrayEntry == NULL)
  {
    if (pCurRoot->arrayValue != NULL)
    {
      pCurArrayEntry = pCurRoot->arrayValue->first;
    };

  }
  else
  {
    pCurArrayEntry = pCurArrayEntry->next;
  };
  if (pCurArrayEntry == NULL)
  {
    if (!this->m_optional)
    {
      throw(1);
    };
    return false;
  };
  return true;
};

bool JReader::Fetch_String(const char *name, CString *result) const
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    result->Empty();
    return false;
  };
  *result = pData->stringValue;
  return true;
}

bool JReader::Fetch_Int(const char *name, int *result) const
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    *result = 0;
    return false;
  };
  sscanf(pData->stringValue,"%d", result);
  return true;
}

int JReader::Fetch_Int(const char *name) const
{
  int result;
  Fetch_Int(name, &result);
  return result;
}

bool JReader::Fetch_Bool(const char *name, bool *result) const
{
  JR_DATA *pData;
  int tmp;
  pData = Locate(name);
  if (pData == NULL)
  {
    *result = false;
    return false;
  };
  if (pData->stringValue == "false")
  {
    *result = false;
  }
  else if (pData->stringValue == "true")
  {
    *result = true;
  }
  else
  {
    sscanf(pData->stringValue,"%d", &tmp);
    *result = tmp != 0;
  };
  return true;
}

bool JReader::Fetch_Bool(const char *name) const
{
  bool result;
  Fetch_Bool(name, &result);
  return result;
}

bool JReader::Fetch_Float(const char *name, double *result) const
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    *result = 0.0;
    return false;
  };
  sscanf(pData->stringValue,"%lf", result);
  return true;
}


double JReader::Fetch_Float(const char *name) const
{
  double result;
  Fetch_Float(name, &result);
  return result;
}

CString JReader::Fetch_String (const char *name) const
{
  CString result;
  Fetch_String(name, &result);
  return result;
}

CString JReader::Fetch_YesNo(const char *name) const
{
  CString result;
  Fetch_String(name, &result);
  if (result == "0") return "no";
  if (result == "1") return "yes";
  return "no";
}

bool JReader::StartList(const char *name, DEBUG_ID dbid)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  OpenBrace(pData, dbid);
  return true;
}

bool JReader::StartList(DEBUG_ID dbid)
{
  return OpenBrace(pCurArrayEntry, dbid);
}

int JReader::LineNum(void)
{
  return pCurArrayEntry->listValue->lineNum;
  NotImplemented(0x551, false);
}

bool JReader::StartArray(const char *name)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL) return false;
  OpenBracket(pData);
  return true;
}

bool JReader::StartArray()
{
  return OpenBracket(NULL);
}

/*
bool JReader::NextEntry()
{
  USEOPTIONAL
  if (pCurList->first == NULL)
  {
    if (!ISOPTIONAL)
    {
      SetError(ES_missingData);
      JRError("Too few elements in array");
      throw 1;
    };
    return false;
  }
  if (pCurData != NULL)
  {
    die("Internal Error - pCurData != NULL");
  };
  pCurData = pCurList->first;
  pCurList->first = pCurData->next;
  return true;
}
*/
bool JReader::OpenBrace(JR_DATA *pNewRoot, DEBUG_ID dbid)
{
  rootStack.SetAtGrow(rootStackSize, pCurRoot);
  rootStackSize++;
  pCurRoot = pNewRoot;
  if (pCurRoot->listValue == NULL)
  {
    SetError(ES_missingList);
    JRError("Expected an open Brace");
    return false;
  };
  pCurRoot->listValue->current = pCurRoot->listValue->first;
  return true;
}

bool JReader::OpenBracket(JR_DATA *pNewRoot)
{
  rootStack.SetAtGrow(rootStackSize, pCurRoot);
  rootStackSize++;
  pCurRoot = pNewRoot;
  if (pCurRoot->arrayValue == NULL)
  {
    SetError(ES_missingArray);
    JRError("Expected an open Bracket");
    return false;
  };
  pCurArrayEntry = NULL;
  return true;
}

bool JReader::NameAndValue(const char *name, CString& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    value = pData->stringValue;
    return true;
  };
}

bool JReader::NameAndBinary(const char *name, CString& value)
{
  NotImplemented(0x3abd, false);
  return false;
}

bool JReader::NameAndValue(const char *name, int& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    int temp;
    sscanf(pData->stringValue,"%d", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndValue(const char *name, WORD& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    int temp;
    sscanf(pData->stringValue,"%d", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndValue(const char *name, long& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    int temp;
    sscanf(pData->stringValue,"%d", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndValue(const char *name, DWORD& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    unsigned int temp;
    sscanf(pData->stringValue,"%d", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndValue(const char *name, char *value, int num)
{
  int i, *p;
  bool result;
  p = new int[num];
  result = NameAndValue(name, p, num);
  for (i=0; i<num; i++)
  {
    value[i] = p[i];
  };
  delete[] p;
  return result;
}


bool JReader::NameAndValue(const char *name, int *value, int num)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    int i;
    int sign;
    const char *c;
    const JR_LIST *pA;
    const JR_DATA *pD;
    pA = pData->arrayValue;
    pD = pA->first;
    for (i=0; i<num; i++)
    {
      sign = 1;
      if (pD == NULL)
      {
        SetError(ES_missingData);
        JRError("Too few emements in array");
        return false;
      };
      c = (LPCSTR)pD->stringValue;
      value[i] = 0;
      for (;;)
      {
        if (*c == 0) break;
        if (*c == ',')
        {
          c++;
          break;
        };
        if (*c == '-')
        {
          sign = -sign;
          c++;
          continue;
        };
        value[i] = 10*value[i] + *c - '0';
        c++;
      };
      value[i] = sign * value[i];
      pD = pD->next;
    };
    return true;
  };
}

bool JReader::NameAndValue(const char *name, double& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    double temp;
    sscanf(pData->stringValue,"%f", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndValue(const char *name, LONGLONG& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    LONGLONG temp;
    sscanf(pData->stringValue,"%I64d", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndValue(const char *name, float& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    float temp;
    sscanf(pData->stringValue,"%f", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndBool(const char *name, BOOL& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    value = FALSE;
    return false;
  }
  else
  {
    if (pData->stringValue == "true") value = TRUE;
    else if (pData->stringValue == "false") value = FALSE;
    else if (pData->stringValue == "0") value = FALSE;
    else if (pData->stringValue == "") value = FALSE;
    else value = TRUE;
    return true;
  };
}

bool JReader::NameAndBool(const char *name, bool& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    value = false;
    return false;
  }
  else
  {
    if (pData->stringValue == "true") value = true;
    else if(pData->stringValue == "1") value = true;
    else value = false;
    return true;
  };
}

bool JReader::NameAndBool(const char *name, BYTE& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    value = false;
    return false;
  }
  else
  {
    if (pData->stringValue == "true") value = true;
    else if(pData->stringValue == "1") value = true;
    else value = false;
    return true;
  };
}


bool JReader::NameAndFlags(const char *name, DWORD& value, int num, const char *text[])
{
  int i;
  value = 0;
  StartArray(name);
  while (Optional(), NextEntry())
  {
    for (i=0; i<num; i++)
    {
      if (pCurRoot->stringValue == text[i])
      {
        value |= (1 << i);
        break;
      };
    };
  };
  EndArray();
  return true;
};

bool JReader::NameAndFlags(const char *name, BYTE& value, int num, const char *text[])
{
  DWORD v;
  bool result;
  result = NameAndFlags(name, v, num, text);
  value = v;
  return result;
};






bool JReader::NameAndValue(const char *name, unsigned char& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    unsigned int temp;
    sscanf(pData->stringValue,"%d", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndValue(const char *name, char& value)
{
  JR_DATA *pData;
  pData = Locate(name);
  if (pData == NULL)
  {
    return false;
  }
  else
  {
    unsigned int temp;
    sscanf(pData->stringValue,"%d", &temp);
    value = temp;
    return true;
  };
}

bool JReader::NameAndEnum(const char *name, DWORD& value, DWORD numValue, const char *text[])
{
  JR_DATA *pData;
  DWORD i;
  pData = Locate(name);
  if (pData == NULL)
  {
    value = 0;
    return false;
  };
  for (i=0; i<numValue; i++)
  {
    if (pData->stringValue == text[i])
    {
      value = i;
      return true;
    };
  };
  value = 0;
  return false;
}

bool JReader::NameAndEnum(const char *name, eventDistType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_DIST_TYPES, DistanceText);
  value = (eventDistType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, QuestAcceptType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_DIST_TYPES, questAcceptText);
  value = (QuestAcceptType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, genderType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_GENDER_TYPES, genderText);
  value = (genderType)temp;
  return status;
}

extern const char *SurfaceTypeTexts[];

bool JReader::NameAndEnum(const char *name, SurfaceType& value)
{
  DWORD v;
  NameAndFlags(name, v, NUM_SURFACE_TYPES, SurfaceTypeTexts);
  value = (SurfaceType)v;
  return true;
}


bool JReader::NameAndEnum(const char *name, QuestStateType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_QUEST_STATE_TYPE, QuestStateTypeText);
  value = (QuestStateType)temp;
  return status;
}

extern const char *CreatureSizeText[];
bool JReader::NameAndEnum(const char *name, creatureSizeType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_SIZE_TYPES, CreatureSizeText);
  value = (creatureSizeType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, alignmentType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_SIZE_TYPES, alignmentText);
  value = (alignmentType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, itemClassType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_ITEMCLASS_TYPES, itemClassText);
  value = (itemClassType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, chainTriggerType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_CHAINTRIG_TYPES, ChainTriggerText);
  value = (chainTriggerType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, eventTriggerType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_EVENTTRIG_TYPES, EventTriggerText);
  value = (eventTriggerType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, MappingType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_MAPPING_TYPES, MappingTypeText);
  value = (MappingType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, eventType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_EVENT_TYPES, EventListText);
  value = (eventType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, eventFacingType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_FACING_TYPES, FacingText);
  value = (eventFacingType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, eventStepType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_STEP_TYPES, StepText);
  value = (eventStepType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, encounterButtonResultType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_ENCOUNTER_BUTTON_OPTION_TYPES, ButtonOptionText);
  value = (encounterButtonResultType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, labelPostChainOptionsType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_LABEL_POST_CHAIN_OPTION_TYPES, ButtonPostChainOptionText);
  value = (labelPostChainOptionsType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, eventTurnUndeadModType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_TURN_MOD_TYPES, TurnModText);
  value = (eventTurnUndeadModType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, eventSurpriseType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_SURPRISE_TYPES, DistanceText);
  value = (eventSurpriseType)temp;
  return status;
}
bool JReader::NameAndEnum(const char *name, eventDirType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_DIR_TYPES, DirectionText);
  value = (eventDirType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, passwordActionType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_PASSACTION_TYPES, PasswordActionText);
  value = (passwordActionType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, eventPartyAffectType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_AFFECT_TYPES, AffectsWhoText);
  value = (eventPartyAffectType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, spellSaveVsType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_SAVE_VS_TYPES, SaveVsText);
  value = (spellSaveVsType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, spellSaveEffectType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_SAVE_EFFECT_TYPES, SaveEffectText);
  value = (spellSaveEffectType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, takeItemQtyType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_TAKEITEMQTY_TYPES, TakeItemsQtyText);
  value = (takeItemQtyType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, taleOrderType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_TALEORDER_TYPES, TaleOrderTypeText);
  value = (taleOrderType)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, takeItemsAffectsType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_TAKEAFFECTS_TYPES, TakeItemsAffectsText);
  value = (takeItemsAffectsType)temp;
  return status;
}
bool JReader::NameAndEnum(const char *name, costFactorType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_COST_FACTOR_TYPES, costFactorText);
  value = (costFactorType)temp;
  return status;
}
bool JReader::NameAndEnum(const char *name, MultiItemCheckType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, MAX_MULTI_ITEM_CHECK_TYPES, MultiItemCheckText);
  value = (MultiItemCheckType)temp;
  return status;
}
bool JReader::NameAndEnum(const char *name, MathOperationType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, MAX_MATH_OPERATION_TYPES, MathOperationText);
  value = (MathOperationType)temp;
  return status;
}
bool JReader::NameAndEnum(const char *name, BlockageType& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, NUM_BLOCKAGE_TYPES, BlockageText);
  value = (BlockageType)temp;
  return status;
}
bool JReader::NameAndEnum(const char *name, GlobalAreaViewStyle& value)
{
  DWORD temp = value;
  bool status;
  status = NameAndEnum(name, temp, MAX_GLOBAL_AVSTYLES, GlobalAVStyleText);
  value = (GlobalAreaViewStyle)temp;
  return status;
}

bool JReader::NameAndEnum(const char *name, int& value, DWORD numValue, const char *text[])
{
  JR_DATA *pData;
  DWORD i;
  pData = Locate(name);
  if (pData == NULL)
  {
    value = 0;
    return false;
  };
  for (i=0; i<numValue; i++)
  {
    if (pData->stringValue == text[i])
    {
      value = i;
      return true;
    };
  };
  value = 0;
  SetError(ES_missingData);
  JRError("Illegal enumeration value");
  throw 1;
  return false;
}

bool JReader::NameAndEnum(const char *name, BYTE& value, DWORD numValue, const char *text[])
{
  JR_DATA *pData;
  DWORD i;
  pData = Locate(name);
  if (pData == NULL)
  {
    value = 0;
    return false;
  };
  for (i=0; i<numValue; i++)
  {
    if (pData->stringValue == text[i])
    {
      value = i;
      return true;
    };
  };
  value = 0;
  return false;
}

