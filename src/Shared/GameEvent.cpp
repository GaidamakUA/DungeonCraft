/******************************************************************************
* Filename: GameEvent.cpp
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
#include "..\UAFwinEd\resource.h"
#include "externs.h"

//#include "externs.h"
#include "class.h"
#include "GameEvent.h"
#include "monster.h"
#include "Level.h"
#include "party.h"
#include "GlobalData.h"
#include "SoundMgr.h"
#include "..\Shared\GPDLopCodes.h"
#include "GPDLexec.h"
#include "GPDLComp.h"

#include "..\UAFwinEd\CrossReference.h"

#ifdef UAFEngine
#include "..\UAFWin\Combatants.h"
#endif

#ifdef UAFEDITOR
#include "ConfigFile.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// store/retrieve data from asl maps
//void StorePicDataAsASL(A_ASLENTRY_L &asl, PIC_DATA& data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrievePicDataFromASL(A_ASLENTRY_L &asl, PIC_DATA& data, const char *prefix);
void StoreIntAsASL(A_ASLENTRY_L &asl, int data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrieveIntFromASL(A_ASLENTRY_L &asl, int &data, const char *prefix);
void StoreStringAsASL(A_ASLENTRY_L &asl, CString &data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrieveStringFromASL(A_ASLENTRY_L &asl, CString &data, const char *prefix);

extern const double VersionSpellNames;
extern const double VersionSpellIDs;
extern const CString Baseclass_cleric;
extern const CString Baseclass_fighter;
extern const CString Baseclass_magicUser;
extern const CString Baseclass_thief;
extern const CString Baseclass_ranger;
extern const CString Baseclass_paladin;
extern const CString Baseclass_druid;

extern const CString Ability_Dexterity;
extern const CString Ability_Constitution;
extern const CString Ability_Strength;
extern const CString Ability_Intelligence;
extern const CString Ability_Charisma;
extern const CString Ability_Wisdom;



extern const CString Skill_PickPockets;
extern const CString Skill_OpenLocks;
extern const CString Skill_FindTraps;
extern const CString Skill_MoveSilent;
extern const CString Skill_HideInShadows;
extern const CString Skill_HearNoise;
extern const CString Skill_ClimbWalls;
extern const CString Skill_ReadLanguages;

extern BOOL logDebuggingInfo;

int setPartyXY_x = -1;
int setPartyXY_y = -1;

#ifdef UAFEDITOR
CLASS_ID ConvertClassTypeToClassID(int type);
#endif

// base event data structures
//IMPLEMENT_SERIAL( EVENT_CONTROL, CObject, 1)
//IMPLEMENT_SERIAL( GameEvent, CObject, 1)
//IMPLEMENT_SERIAL( GameEventList, CObject, 1)
//IMPLEMENT_SERIAL( NULL_EVENT_DATA, CObject, 1)

// event support data
//IMPLEMENT_SERIAL( TRANSFER_DATA, CObject, 1)
//IMPLEMENT_SERIAL( MONSTER_EVENT, CObject, 1)
//IMPLEMENT_SERIAL( MONSTER_EVENT_DATA, CObject, 1)
//IMPLEMENT_SERIAL( TOUR_STEP, CObject, 1)
//IMPLEMENT_SERIAL( ENCOUNTER_BUTTON_OPTION, CObject, 1)
//IMPLEMENT_SERIAL( ENCOUNTER_BUTTON_DATA, CObject, 1)
//IMPLEMENT_SERIAL( QLIST_OPTION, CObject, 1)
//IMPLEMENT_SERIAL( QLIST_DATA, CObject, 1)
//IMPLEMENT_SERIAL( QBUTTON_OPTION, CObject, 1)
//IMPLEMENT_SERIAL( QBUTTON_DATA, CObject, 1)
//IMPLEMENT_SERIAL( TALE, CObject, 1)
//IMPLEMENT_SERIAL( DRINK, CObject, 1)
//IMPLEMENT_SERIAL( REST_EVENT, CObject, 1)
//IMPLEMENT_SERIAL( TIME_EVENT, CObject, 1)
//IMPLEMENT_SERIAL( STEP_EVENT, CObject, 1)
//IMPLEMENT_SERIAL( SPECIAL_OBJECT_EVENT_DATA, CObject, 1)
//IMPLEMENT_SERIAL( SPECIAL_OBJECT_EVENT_LIST, CObject, 1)

// events
//IMPLEMENT_SERIAL( ADD_NPC_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( REMOVE_NPC_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( CAMP_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( COMBAT_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( GUIDED_TOUR, GameEvent, 1)
//IMPLEMENT_SERIAL( QUESTION_YES_NO, GameEvent, 1)
//IMPLEMENT_SERIAL( ENCOUNTER_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( PASSWORD_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( GAIN_EXP_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( GIVE_TREASURE_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( GIVE_DAMAGE_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( NPC_SAYS_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( QUESTION_LIST_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( QUESTION_BUTTON_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( TEXT_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( GPDL_EVENT, GameEvent, 1)
//IMPLEMENT_SERIAL( TRANSFER_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( WHO_PAYS_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( WHO_TRIES_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( SPECIAL_ITEM_KEY_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( PASS_TIME_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( VAULT_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( TAVERN, GameEvent, 1)
//IMPLEMENT_SERIAL( TEMPLE, GameEvent, 1)
//IMPLEMENT_SERIAL( SHOP, GameEvent, 1)
//IMPLEMENT_SERIAL( TRAININGHALL, GameEvent, 1)
//IMPLEMENT_SERIAL( SMALL_TOWN_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( CHAIN_EVENT, GameEvent, 1)
//IMPLEMENT_SERIAL( QUEST_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( UTILITIES_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( SOUND_EVENT, GameEvent, 1)
//IMPLEMENT_SERIAL( TAKE_PARTY_ITEMS_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( HEAL_PARTY_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( LOGIC_BLOCK_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( RANDOM_EVENT_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( PLAY_MOVIE_DATA, GameEvent, 1)
//IMPLEMENT_SERIAL( JOURNAL_EVENT, GameEvent, 1)

double LoadingVersion;

#ifdef UAFEDITOR
GameEventList CopyEventData; // temp storage for copy/paste of events
#endif

BOOL MyDeleteFile(LPCSTR name);


void WarnOfClearingWithChainsPresent(GameEvent *pEvent)
{
#ifdef UAFEDITOR
  if (pEvent->IsChained())
  {
    CString msg;
    msg.Format("An event is being cleared which contains chained events\n%s\n"
               "Please make a note of your current activity and send\n"
               "this information to CocoaSpud@hotmail.com",
               GetEventIdDescription(pEvent->id, pEvent->GetEventSource()));
    MsgBoxError(msg, "Invalid Editor State");
  }
#endif
}

BOOL CheckValidEvent(GameEvent *pEvent)
{
  if (pEvent==NULL) return FALSE;
  /*
  if (pEvent->IsGlobalEvent())
    return globalData.eventData.IsValidEvent(pEvent->id);
  else
    return levelData.eventData.IsValidEvent(pEvent->id);
  */

  switch (pEvent->GetEventSource())
  {
  case LevelEventSrc:
    return levelData.eventData.IsValidEvent(pEvent->id);
    break;
  case GlobalEventSrc:
    return globalData.eventData.IsValidEvent(pEvent->id);
    break;
#ifdef UAFEDITOR
  case CopyEventSrc:
    return CopyEventData.IsValidEvent(pEvent->id);
    break;
#endif
  default:
    die(0xab50b);
    return FALSE;
  }
}

BOOL CheckValidEvent(GameEvent *pEvent, DWORD id) // id might be a chain id
{
  if (pEvent==NULL) return FALSE;

  /*
  if (pEvent->IsGlobalEvent())
    return globalData.eventData.IsValidEvent(id);
  else
    return levelData.eventData.IsValidEvent(id);
  */

  switch (pEvent->GetEventSource())
  {
  case LevelEventSrc:
    return levelData.eventData.IsValidEvent(id);
    break;
  case GlobalEventSrc:
    return globalData.eventData.IsValidEvent(id);
    break;
#ifdef UAFEDITOR
  case CopyEventSrc:
    return CopyEventData.IsValidEvent(id);
    break;
#endif
  default:
    die(0xab50c);
    return FALSE;
  }
}
/*
  These events can push SAVEGAME_MENU and
  are not allowed as global events.

  QUESTION_BUTTON_DATA (QuestionButton)
  QUESTION_LIST_DATA   (QuestionList)
  SMALL_TOWN_DATA      (SmallTown)
  ENCAMP_MENU_DATA     N/A
  QUESTION_YES_NO      (QuestionYesNo)
  BEGIN_STEP_EVENT     N/A
  BEGIN_TIME_EVENT     N/A
  ADVENTURE_MENU_DATA  N/A
  MAIN_MENU_DATA       N/A
  TRAININGHALL         (TrainingHallEvent)
  CAMP_EVENT_DATA      (Camp)
  START_MENU_DATA      N/A
*/

BOOL AllowedAsGlobalEvent(eventType type)
{
  BOOL result = TRUE;

#ifdef UAFEngine
  if (IsCombatActive())
    return (AllowedAsGlobalCombatEvent(type));
#endif

  return result;
}

BOOL AllowedAsGlobalCombatEvent(eventType type)
{
  if (type >= /*(eventType)*/CONTROL_Splash) return FALSE;

  BOOL result = TRUE;
  switch (type)
  {
    // global events are triggered by NPC 'Talk' and Item 'Use'.
    // the 'Use' trigger can happen during combat, and needs further restrictions
   case NoEvent:
   case AddNpc:
   case Camp:
   case Combat:
   case EncounterEvent:
   case ShopEvent:
   case TempleEvent:
   case TavernTales:
   case TavernEvent:
   case Stairs:
   case Teleporter:
   case TransferModule:
   case Vault:
   case TrainingHallEvent:
   case SmallTown:
   case PickOneCombat:
   case InnEvent:
   case Damage:
   case EnterPassword:
   case GainExperience:
   case CombatTreasure:
   case GiveTreasure:
   case GuidedTour:
   case NPCSays:
   case QuestionList:
   case QuestionButton:
   case PassTime:
   case QuestionYesNo:
   case RemoveNPCEvent:
   case TextStatement:
   case WhoPays:
   case WhoTries:
   case SpecialItem:   
   case QuestStage:   
   case TakePartyItems:   
   case GPDLEvent:
   case RandomEvent:
   case PlayMovieEvent:
   case JournalEvent:
     result = FALSE;
     break;

   case Utilities:
   case Sounds:
   case LogicBlock:
   case HealParty:   
     result = TRUE;
     break;

   default:
     WriteDebugString("Unhandled event type %u in AllowedAsGlobalEvent()\n", type);
     result = FALSE;
     break;
  }  
  return result;
}

void SPECIAL_OBJECT_EVENT_DATA::Serialize(CArchive &ar)
{
// CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << ItemType;
   ar << operation;
   ar << index;
   ar << id;
 }
 else
 {
   ar >> ItemType;
   ar >> operation;
   ar >> index;
   ar >> id;
 }
}

void SPECIAL_OBJECT_EVENT_DATA::Serialize(CAR &ar)
{
 //CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << ItemType;
   ar << operation;
   ar << index;
   ar << id;
 }
 else
 {
   ar >> ItemType;
   ar >> operation;
   ar >> index;
   ar >> id;
 }
}

#ifdef UAFEDITOR
void SPECIAL_OBJECT_EVENT_DATA::CrossReference(CR_LIST *pCRList, CR_REFERENCE *pCRReference)
{
  pCRList->CR_AddSpecialItemReference(this, pCRReference);
}


extern const char *JKEY_SPECIALOBJECTS;
const char *JKEY_SPECIALOBJECT = "specialObject";
const char *JKEY_SPECIALITEM = "specialItem";
const char *JKEY_OPERATION = "operation";
const char *JKEY_INDEX = "index";
extern const char *JKEY_ITEMID;
const char *JKEY_ITEMTYPE = "itemtype";
const char *JKEY_TYPE = "type";

void SPECIAL_OBJECT_EVENT_DATA::Export(JWriter& jw)
{
  jw.StartList(JKEY_SPECIALOBJECT);
  CString temp;

  if      (ItemType & ITEM_FLAG)  temp = "item";
  else if (ItemType & KEY_FLAG)   temp = "key";
  else if (ItemType & QUEST_FLAG) temp = "quest";
  else temp = "";
  jw.NameAndValue(JKEY_TYPE, temp);

  if      (operation & SPECIAL_OBJECT_GIVE) temp = "give";
  else if (operation & SPECIAL_OBJECT_TAKE) temp = "take";
  else temp = "";
  jw.NameAndValue(JKEY_OPERATION, temp);

  jw.NameAndValue(JKEY_INDEX, index);
  jw.NameAndValue(JKEY_ITEMID, id);
  jw.EndList();
}


int SPECIAL_OBJECT_EVENT_DATA::Import(JReader& jr)
{
  if (!jr.StartList()) return -1;
  {
    CString temp;
    ItemType = 0;
    jr.NameAndValue(JKEY_TYPE, temp);
    if      (temp == "item") ItemType  |= ITEM_FLAG;
    else if (temp == "key" ) ItemType  |= KEY_FLAG;
    else if (temp == "quest") ItemType |= QUEST_FLAG;

    jr.NameAndValue(JKEY_OPERATION, temp);
    if      (temp == "give")operation |= SPECIAL_OBJECT_GIVE;
    else if (temp == "take")operation |= SPECIAL_OBJECT_TAKE;
    else temp = "";
  };

  jr.NameAndValue(JKEY_INDEX, index);
  if ( (index < 0) || (index > 99) ) 
  {
    jr.EndList();
    return -1;
  };
  jr.NameAndValue(JKEY_ITEMID, id);
  jr.EndList();
  return index;
}

#endif


SPECIAL_OBJECT_EVENT_DATA& SPECIAL_OBJECT_EVENT_DATA::operator =(const SPECIAL_OBJECT_EVENT_DATA& src)
{ 
 if (&src == this) 
   return *this;
 ItemType=src.ItemType;
 operation=src.operation;
 index=src.index; 
 id=src.id; 
 return *this; 
}
#ifdef UAFEDITOR
bool SPECIAL_OBJECT_EVENT_DATA::operator ==(const SPECIAL_OBJECT_EVENT_DATA& src) const
{ 
 if (&src == this) return true;
 if (ItemType != src.ItemType) return false;
 if (operation != src.operation) return false;
 if (index != src.index) return false; 
 if (id != src.id) return false; 
 return true; 
}
#endif
void SPECIAL_OBJECT_EVENT_LIST::Serialize(CArchive &ar)
{
 int i;
// CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << list.GetSize();
   for (i=0;i<list.GetSize();i++)
     list[i].Serialize(ar);
 }
 else
 {
   int count;
   ar >> count;
   list.SetSize(count);
   for (i=0;i<count;i++)
     list[i].Serialize(ar);
 }      
}

void SPECIAL_OBJECT_EVENT_LIST::Serialize(CAR &ar)
{
 int i;
 //CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << list.GetSize();
   for (i=0;i<list.GetSize();i++)
     list[i].Serialize(ar);
 }
 else
 {
   int count;
   ar >> count;
   list.SetSize(count);
   for (i=0;i<count;i++)
     list[i].Serialize(ar);
 }      
}


#ifdef UAFEDITOR

const char *JKEY_SPECIALITEMS = "specialItems";
void SPECIAL_OBJECT_EVENT_LIST::Export(JWriter& jw)
{
  int i;
  jw.StartArray(JKEY_SPECIALITEMS);
  for (i=0;i<list.GetSize();i++)
  {
    list[i].Export(jw);
  };
  jw.EndArray();
}


bool SPECIAL_OBJECT_EVENT_LIST::Import(JReader& jr)
{
  SPECIAL_OBJECT_EVENT_DATA data;
  jr.Optional();
  if(!jr.StartArray(JKEY_SPECIALITEMS)) return false;
  while (jr.Optional(), jr.NextEntry())
  {
    if (data.Import(jr) >= 0)
    {
      list.Add(data);
    };
  };
  jr.EndArray();
  return true;
}

void SPECIAL_OBJECT_EVENT_LIST::CrossReference(CR_LIST *pCRList, CR_REFERENCE *pCRReference)
{
  int i;
  for (i=0; i<list.GetSize(); i++)
  {
    list.GetAt(i).CrossReference(pCRList, pCRReference);
  };
}
#endif

SPECIAL_OBJECT_EVENT_LIST& SPECIAL_OBJECT_EVENT_LIST::operator =(const SPECIAL_OBJECT_EVENT_LIST& src)
{
 if (&src == this)
   return *this;
 list.RemoveAll();
 list.SetSize(src.list.GetSize());
 for (int i=0;i<src.list.GetSize(); i++)
   list[i] = src.list[i];
 return *this;
}
#ifdef UAFEDITOR
bool SPECIAL_OBJECT_EVENT_LIST::operator ==(const SPECIAL_OBJECT_EVENT_LIST& src) const
{
 if (&src == this) return true;
 if (list.GetSize() != src.list.GetSize()) return false;
 for (int i=0;i<src.list.GetSize(); i++)
 {
   if (!(list[i] == src.list[i])) return false;
 };
 return true;
}
#endif
BOOL SPECIAL_OBJECT_EVENT_LIST::Get(int id, SPECIAL_OBJECT_EVENT_DATA &data)
{
 if ((id >= 0) && (id < list.GetSize()))
 {
   data = list[id];
   return TRUE;
 }
 return FALSE;
}

int SPECIAL_OBJECT_EVENT_LIST::Add(SPECIAL_OBJECT_EVENT_DATA &data)
{
 int id = list.GetSize();
 data.id = id;
 list.SetAtGrow(id, data);
 return id;
}

BOOL SPECIAL_OBJECT_EVENT_LIST::Delete(int id)
{
 if ((id >= 0) && (id < list.GetSize()))
 {
   list.RemoveAt(id);
   return TRUE;
 }
 return FALSE;
}

#ifdef UAFEDITOR
void WriteValidateLog(FILE *pFile, const char *text, ... )
{
  // 1024 is limit on TRACE()
  static char errorText[512+1];

  if (pFile == NULL)
    return;

  va_list marker;	
  memset(errorText, 0, sizeof(errorText));

  va_start(marker, text);  
  int count = _vsnprintf(errorText, 512, text, marker);
  va_end(marker);

  if (count < 0)
  {
    strcpy(errorText, "WriteValidateLog() msg exceeded buffer capacity\n");
    fprintf(pFile, errorText);
    TRACE(errorText);
  }
  else if (count == 0)
  {
    strcpy(errorText, "WriteValidateLog() msg is empty\n");
    fprintf(pFile, errorText);
    TRACE(errorText);
  }
  else
  {
    errorText[512] = '\0'; // guarantee null terminator
    fprintf(pFile, errorText);
    TRACE(errorText);
  }
}
#endif


void EVENT_CONTROL::Clear()
{
  eventStatusUnused=0;
  eventResultUnused=0;
  onceOnly = FALSE;
  chainTrigger = AlwaysChain;
  eventTrigger = AlwaysTrigger;
  facing = Any;
  //race = Human;
  raceID.Clear();
  //charClass = Cleric;
  classBaseclassID.classID.Clear();
  classBaseclassID.baseclassID.Clear();
  //m_giID.Clear();
  itemID.Clear();
  quest = -1;
  chance = 100;
  gender=Male;
  specialItem = -1;
  specialKey = -1;
  //npc=-1;
  characterID.Clear();
  gpdlData="";
  gpdlIsBinary=false;
  partyX=partyY=-1;
  //memorizedSpellDbKey.Clear();
  memorizedSpellID.Clear();
  memorizedSpellClass = 0;
  memorizedSpellLevel = 0;
  eventcontrol_asl.Clear();
  temp_asl.Clear();
}

EVENT_CONTROL& EVENT_CONTROL::operator =(const EVENT_CONTROL& src)
{
  if (&src == this)
    return *this;
  onceOnly = src.onceOnly;
  chainTrigger = src.chainTrigger;
  eventTrigger = src.eventTrigger;
  facing = src.facing;
  //race = src.race;
  raceID = src.raceID;
  //charClass = src.charClass;
  classBaseclassID.classID = src.classBaseclassID.classID;
  classBaseclassID.baseclassID = src.classBaseclassID.baseclassID;
  //m_giID = src.m_giID;
  itemID = src.itemID;
  quest = src.quest;
  chance = src.chance;
  specialItem = src.specialItem;
  specialKey = src.specialKey;
  //npc=src.npc;
  characterID=src.characterID;
  gpdlData=src.gpdlData;
  gpdlIsBinary=src.gpdlIsBinary;
  eventcontrol_asl.Copy(src.eventcontrol_asl);
  gender=src.gender;
  partyX=src.partyX;
  partyY=src.partyY;
  //memorizedSpellDbKey=src.memorizedSpellDbKey;
  memorizedSpellID=src.memorizedSpellID;
  memorizedSpellClass=src.memorizedSpellClass;
  memorizedSpellLevel=src.memorizedSpellLevel;
  return *this;
}

#ifdef UAFEDITOR
bool EVENT_CONTROL::operator ==(const EVENT_CONTROL& src) const
{
  if (&src == this) return true;
  if (onceOnly != src.onceOnly) return false;
  if (chainTrigger != src.chainTrigger) return false;
  if (eventTrigger != src.eventTrigger) return false;
  if (facing != src.facing) return false;
  if ((eventTrigger==RaceInParty)||(eventTrigger==RaceNotInParty))
  {
    if (raceID != src.raceID) return false;
  };
  if (classBaseclassID.classID != src.classBaseclassID.classID) return false;
  if (classBaseclassID.baseclassID != src.classBaseclassID.baseclassID) return false;
  if (itemID != src.itemID) return false;
  if (quest != src.quest) return false;
  if (chance != src.chance) return false;
  if (specialItem != src.specialItem) return false;
  if (specialKey != src.specialKey) return false;
  if (characterID != src.characterID) return false;
  if (gpdlData != src.gpdlData) return false;
  if (gpdlIsBinary != src.gpdlIsBinary) return false;
  if (!(eventcontrol_asl == src.eventcontrol_asl)) return false;
  if (gender != src.gender) return false;
  if (eventTrigger == PartyAtXY)
  {
    if (partyX != src.partyX) return false;
    if (partyY != src.partyY) return false;
  };
  if (memorizedSpellID != src.memorizedSpellID) return false;
  if (memorizedSpellClass != src.memorizedSpellClass) return false;
  if (memorizedSpellLevel != src.memorizedSpellLevel) return false;
  return true;
}
#endif
EVENT_CONTROL::EVENT_CONTROL(const EVENT_CONTROL& src) :
    // ASLs are named "EVENTCONT_ATTR"
    eventcontrol_asl(),
    temp_asl()
{
  *this = src;
}

#ifdef UAFEngine
BOOL EVENT_CONTROL::EventShouldTrigger(DWORD id)
{
  BOOL shouldTrigger = FALSE;

  switch (eventTrigger)
  {
  case AlwaysTrigger:
    if (id != 0)
    {
      if (logDebuggingInfo)
      {
        WriteDebugString("Event %d triggered: always triggers\n", id);
      };
    };
    shouldTrigger = TRUE;
    break;

  case PartyHaveItem:
    //if (m_giID.IsValidItem())
    if (itemID.IsValidItem())
      //shouldTrigger = party.PartyHasItem(m_giID);
      shouldTrigger = party.PartyHasItem(itemID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party has item\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party does not have item\n", id);
    };
    break;

  case PartyNotHaveItem:
    //if (m_giID.IsValidItem())
    //shouldTrigger = !party.PartyHasItem(m_giID);
    if (itemID.IsValidItem())
      shouldTrigger = !party.PartyHasItem(itemID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party does not have item\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party has item\n", id);
    };
    break;

  case PartyHaveSpecialItem:
    //if (m_giID.IsValidItem())
    //else if (specialItem >= 0)
    shouldTrigger = party.hasSpecialItem(specialItem);

    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party has item\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party does not have item\n", id);
    };
    break;

  case PartyNotHaveSpecialItem:
    //if (m_giID.IsValidItem())
    //shouldTrigger = !party.PartyHasItem(m_giID);
    //if (specialItem >= 0)
    shouldTrigger = !party.hasSpecialItem(specialItem);

    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party does not have item\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party has item\n", id);
    };
    break;


  case PartyHaveSpecialKey:
    //else if (specialKey >= 0)
    shouldTrigger = party.hasSpecialKey(specialKey);


    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party has item\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party does not have item\n", id);
    };
    break;

  case PartyNotHaveSpecialKey:
    //else if (specialKey >= 0)
    shouldTrigger = !party.hasSpecialKey(specialKey);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party has item\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party does not have item\n", id);
    };
    break;



  case Daytime:
    shouldTrigger = party.PartyInDaytime();
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: daytime\n", id);
      else
        WriteDebugString("Event %d NOT triggered: not daytime\n", id);
    };
    break;

  case Nighttime:
    shouldTrigger = !party.PartyInDaytime();
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: night-time\n", id);
      else
        WriteDebugString("Event %d NOT triggered: not night-time\n", id);
    };
    break;

  case RandomChance:
    shouldTrigger = (RollDice(100, 1, 0) <= chance);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: rnd chance\n", id);
      else
        WriteDebugString("Event %d NOT triggered: rnd chance\n", id);
    };
    break;

  case PartySearching:
    shouldTrigger = party.PartyIsSearching();
    shouldTrigger |= party.looking;
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party is searching or looking\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party is not searching or looking\n", id);
    };
    break;

  case PartyNotSearching:
    shouldTrigger = !party.PartyIsSearching();
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d NOT triggered: party is searching\n", id);
      else
        WriteDebugString("Event %d triggered: party is not searching\n", id);
    };
    break;

  case FacingDirection:
  case FacingDirectionAnyTime:
    if ((facing == Any) || (facing == InFront)) // InFront shouldn't happen
    {
      shouldTrigger = TRUE;
    }
    else
    {
      switch (party.GetPartyFacing())
      {
      case FACE_NORTH:
        shouldTrigger = ((facing == North)
          || (facing == N_S)
          || (facing == N_E)
          || (facing == N_W)
          || (facing == N_S_E)
          || (facing == N_S_W)
          || (facing == N_W_E));
        break;
      case FACE_EAST:
        shouldTrigger = ((facing == East)
          || (facing == N_E)
          || (facing == S_E)
          || (facing == E_W)
          || (facing == N_S_E)
          || (facing == N_W_E)
          || (facing == W_S_E));
        break;
      case FACE_SOUTH:
        shouldTrigger = ((facing == South)
          || (facing == N_S)
          || (facing == S_E)
          || (facing == S_W)
          || (facing == N_S_E)
          || (facing == N_S_W)
          || (facing == W_S_E));
        break;
      case FACE_WEST:
        shouldTrigger = ((facing == West)
          || (facing == N_W)
          || (facing == S_W)
          || (facing == E_W)
          || (facing == N_S_W)
          || (facing == N_W_E)
          || (facing == W_S_E));
        break;
      }
    }
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party facing\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party facing\n", id);
    };
    break;

  case QuestPresent:
    shouldTrigger = globalData.questData.IsPresent(quest);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: quest present\n", id);
      else
        WriteDebugString("Event %d NOT triggered: quest not present\n", id);
    };
    break;

  case QuestNotPresent:
    shouldTrigger = !globalData.questData.IsPresent(quest);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: quest not present\n", id);
      else
        WriteDebugString("Event %d NOT triggered: quest present\n", id);
    };
    break;

  case QuestComplete:
    shouldTrigger = globalData.questData.IsComplete(quest);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: quest complete\n", id);
      else
        WriteDebugString("Event %d NOT triggered: quest not complete\n", id);
    };
    break;

  case QuestFailed:
    shouldTrigger = globalData.questData.IsFailed(quest);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: quest failed\n", id);
      else
        WriteDebugString("Event %d NOT triggered: quest not failed\n", id);
    };
    break;

  case QuestInProgress:
    shouldTrigger = globalData.questData.IsInProgress(quest);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: quest in progress\n", id);
      else
        WriteDebugString("Event %d NOT triggered: quest not in progress\n", id);
    };
    break;

  case PartyDetectingTraps:
    shouldTrigger = party.PartyIsDetectingTraps();
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party is detecting traps\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party is not detecting traps\n", id);
    };
    break;

  case PartyNotDetectingTraps:
    shouldTrigger = !party.PartyIsDetectingTraps();
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party is not detecting traps\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party is detecting traps\n", id);
    };
    break;

  case PartySeeInvisible:
    shouldTrigger = party.PartyIsDetectingInvisible();
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party can see invisible\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party can not see invisible\n", id);
    };
    break;

  case PartyNotSeeInvisible:
    shouldTrigger = !party.PartyIsDetectingInvisible();
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party can not see invisible\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party can see invisible\n", id);
    };
    break;

  case ClassInParty:
    //shouldTrigger = party.PartyHasClass(charClass);
    shouldTrigger = party.PartyHasClass(classBaseclassID.classID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: class in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: class not in party\n", id);
    };
    break;

  case ClassNotInParty:
    //shouldTrigger = !party.PartyHasClass(charClass);
    shouldTrigger = !party.PartyHasClass(classBaseclassID.classID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: class not in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: class in party\n", id);
    };
    break;

  case BaseclassInParty:
    //shouldTrigger = party.PartyHasClass(charClass);
    shouldTrigger = party.PartyHasBaseclass(classBaseclassID.baseclassID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: class in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: class not in party\n", id);
    };
    break;

  case BaseclassNotInParty:
    //shouldTrigger = !party.PartyHasClass(charClass);
    shouldTrigger = !party.PartyHasBaseclass(classBaseclassID.baseclassID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: class not in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: class in party\n", id);
    };
    break;

  case RaceInParty:
    //shouldTrigger = party.PartyHasRace(race);
    shouldTrigger = party.PartyHasRace(raceID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: race in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: race not in party\n", id);
    };
    break;
      
  case RaceNotInParty:
    //shouldTrigger = !party.PartyHasRace(race);
    shouldTrigger = !party.PartyHasRace(raceID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: race not in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: race in party\n", id);
    };
    break;

  case GenderInParty:
    shouldTrigger = party.PartyHasGender(gender);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: gender in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: gender not in party\n", id);
    };
    break;

  case GenderNotInParty:
    shouldTrigger = !party.PartyHasGender(gender);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: gender not in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: gender in party\n", id);
    };
    break;

  case NPCInParty:
    //shouldTrigger = party.PartyHasNPC(npc);
    shouldTrigger = party.PartyHasNPC(characterID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: npc in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: npc not in party\n", id);
    };
    break;

  case NPCNotInParty:
    //shouldTrigger = !party.PartyHasNPC(npc);
    shouldTrigger = !party.PartyHasNPC(characterID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: npc not in party\n", id);
      else
        WriteDebugString("Event %d NOT triggered: npc in party\n", id);
    };
    break;

  case ExecuteGPDL:
    {
      if (gpdlData.GetLength()==0) return FALSE;
      if (!gpdlIsBinary)
      {       
        GPDLCOMP gpdlComp;
        CString funcname;
        funcname.Format("GameEventTrigger_%u", GetTickCount());
        CString sourcePgm, binaryPgm;
        const char *entries[] = {NULL, NULL};
        entries[0] = (LPCSTR)funcname;
        CString begin;
        begin.Format("$PUBLIC $FUNC %s() {", funcname);
        CString end;
        end.Format("}%s;",funcname);
        sourcePgm =   begin
                    + gpdlData
                    + end;
        binaryPgm = gpdlComp.CompileScript(sourcePgm, entries);
        if (binaryPgm[0] != 0)
        { // error
          WriteDebugString("Event %u NOT triggered: GPDL compile error \'%s\'",id, gpdlData);
          gpdlData="";
          return FALSE;
        }
        gpdlData=binaryPgm;
        gpdlIsBinary=true;
      }      

      if (IsCombatActive())
      {
        ActorType actor;
        combatData.getActiveCombatCharacter().GetContext(&actor);
        SetCharContext(&actor);
        SetTargetContext(&actor); // combatData.getActiveCombatCharacter().GetContext() );
      }
      else
      {
        ActorType actor;
        party.getActiveChar().GetContext(&actor);
        SetCharContext(&actor);
        SetTargetContext(&actor);// party.getActiveChar().GetContext() );
      }
      setPartyXY_x = -1;
      gpdlStack.Push();
      shouldTrigger = gpdlStack.activeGPDL()->ExecuteScript(gpdlData,1)=="1";
      gpdlStack.Pop();

      ClearTargetContext();
      ClearCharContext();      

      if (setPartyXY_x >= 0)
      {
        shouldTrigger = false;
      };
      if (logDebuggingInfo)
      {
        if (shouldTrigger)
          WriteDebugString("Event %d triggered: GPDL script returns true\n", id);
        else
          WriteDebugString("Event %d NOT triggered: GPDL script returns false\n", id);
      };
    }  
    break;

  case SpellMemorized:    
    //shouldTrigger = party.PartyHasSpellMemorized(memorizedSpellDbKey);
    shouldTrigger = party.PartyHasSpellMemorized(memorizedSpellID);
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party has spell memorized\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party does not have spell memorized\n", id);
    };
    break;

  case PartyAtXY:
    shouldTrigger = ((party.Posx == partyX) && (party.Posy == partyY));
    if (logDebuggingInfo)
    {
      if (shouldTrigger)
        WriteDebugString("Event %d triggered: party at x,y\n", id);
      else
        WriteDebugString("Event %d NOT triggered: party not at x,y\n", id);
    };
    break;

  default:
    WriteDebugString("Event %d NOT triggered: unhandled eventTrigger type\n", id);
    return FALSE;
  }
      
  return shouldTrigger;
}

#endif

void EVENT_CONTROL::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    StoreIntAsASL(eventcontrol_asl, (int)gender, "Gen");
    StoreIntAsASL(eventcontrol_asl, specialItem, "SpIt");
    StoreIntAsASL(eventcontrol_asl, specialKey, "SpKy");
  }
}

void EVENT_CONTROL::PostSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    eventcontrol_asl.Delete("Gen");
    eventcontrol_asl.Delete("SpIt");
    eventcontrol_asl.Delete("SpKy");
  }
  else
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    int temp;
    RetrieveIntFromASL(eventcontrol_asl, temp, "Gen");
    eventcontrol_asl.Delete("Gen");
    gender = (genderType)temp;
    RetrieveIntFromASL(eventcontrol_asl, specialItem, "SpIt");
    eventcontrol_asl.Delete("SpIt");
    RetrieveIntFromASL(eventcontrol_asl, specialKey, "SpKy");
    eventcontrol_asl.Delete("SpKy");
  }
}

void EVENT_CONTROL::Serialize( CArchive &ar, double version )
{
  PreSerialize(ar.IsStoring());

//  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     ar << (int)eventStatusUnused;
     ar << (int)eventResultUnused;
     ar << (int)onceOnly;
     ar << (int)chainTrigger;
     ar << (int)eventTrigger;
           //m_giID.Serialize(ar);
     ar << itemID;
     ar << quest;
     ar << chance;
     ar << (int)facing;
     //ar << (int)race;
     ar << raceID;
     //ar << (int)charClass;
     //ar << classID;
     classBaseclassID.Serialize(ar, eventTrigger == BaseclassInParty);
     //ar << npc;
     ar << characterID;
     eventcontrol_asl.Serialize(ar, "EVENTCONT_ATTR");
     AS(ar, gpdlData);
     ar << gpdlIsBinary;
     ar << partyX;
     ar << partyY;
     //memorizedSpellDbKey.Serialize(ar, version);//ar << memorizedSpellDbKey;
     ar << memorizedSpellID;
     ar << memorizedSpellClass;
     ar << memorizedSpellLevel;
  }
  else
  {
     int temp;
   
     ar >> temp; 
     eventStatusUnused = temp;  
     ar >> temp;
     eventResultUnused = temp; 
     ar >> temp;
     onceOnly = temp;   
     ar >> temp;
     chainTrigger = (chainTriggerType)temp;   
     ar >> temp;
     eventTrigger = (eventTriggerType)temp;
#ifdef UAFEDITOR
     if (version < VersionSpellNames)
     {
       int id;
       ar >> id;
       if (id >= 0)
       {
         itemID = itemData.FindPreVersionSpellNamesItemID(id);
       }
       else
       {
         itemID.Empty();
       };
     }
     else
#endif
     {
        ar >> itemID;
     };
     ar >> quest;
     ar >> chance;   
     ar >> temp;
     facing = (eventDirType)temp;
     //ar >> temp;
     //race = (raceType)temp;
#ifdef UAFEDITOR
     if (version < VersionSpellNames)
     {
       DWORD id;
       ar >> id;
       raceID = raceData.FindPreVersionSpellNamesRaceID(id);
     }
     else
#endif
     {
       ar >> raceID;
     };
#ifdef UAFEDITOR
     if (version < VersionSpellNames)
     {
     //ar >> temp;
     //charClass = (classType)temp;
       ar >> temp;
       if (temp > 0)
       {
         classBaseclassID.classID = classData.FindPreVersionSpellNamesClassID(temp);
       }
       else
       {
         classBaseclassID.classID.Empty();
       };
     }
     else
#endif
     {
       classBaseclassID.Serialize(ar, (eventTrigger==BaseclassInParty)||(eventTrigger==BaseclassNotInParty));
     };

     if (LoadingVersion >= _VERSION_0820_)
     {
#ifdef UAFEDITOR
       if (version < VersionSpellNames)
       {
         int npc;
         int indx;
       //ar >> npc;
         ar >> npc;
         if (npc > 0)
         {
           indx = globalData.charData.LocatePreSpellNamesKey(npc);
         }
         else
         {
           indx = -1;
         }
         if (indx >= 0) 
         {
           characterID = globalData.charData.PeekCharacter(indx)->CharacterID();
         }
         else
         {
           characterID.Empty();
         };
       }
       else
#endif
       {
          ar >> characterID;
       };
     };
     if (LoadingVersion >= _VERSION_0566_)
      eventcontrol_asl.Serialize(ar, "EVENTCONT_ATTR");

     if (LoadingVersion >= _VERSION_0880_)
     {
       DAS(ar, gpdlData);
       ar >> gpdlIsBinary;
     }

     if (LoadingVersion >= _VERSION_0911_)
     {
       ar >> partyX;
       ar >> partyY;
#ifdef UAFEDITOR
       if (version < VersionSpellNames)
       {
         DWORD spellDbKey;
         ar >> spellDbKey;
         if (this->eventTrigger == SpellMemorized)
         {
           memorizedSpellID = spellData.FindPreVersionSpellNamesSpellID(spellDbKey);
         }
         else
         {
           memorizedSpellID.Empty();
         };
       //memorizedSpellDbKey.Serialize(ar, version);//ar >> memorizedSpellDbKey;
       }
       else
#endif
       {
         ar >> memorizedSpellID;
       };
       ar >> memorizedSpellClass;
       ar >> memorizedSpellLevel;
     }
  }   
  PostSerialize(ar.IsStoring());
}

void EVENT_CONTROL::Serialize( CAR &ar, double version )
{
  PreSerialize(ar.IsStoring());

  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     ar << (int)eventStatusUnused;
     ar << (int)eventResultUnused;
     ar << (int)onceOnly;
     ar << (int)chainTrigger;
     ar << (int)eventTrigger;
           //m_giID.Serialize(ar);
     ar << itemID;
     ar << quest;
     ar << chance;
     ar << (int)facing;
     //ar << (int)race;
     ar << raceID;
     //ar << (int)charClass;
     //ar << classID;
     classBaseclassID.Serialize(ar, (eventTrigger==BaseclassInParty)||(eventTrigger==BaseclassNotInParty));
     //ar << npc;
     ar << characterID;
     eventcontrol_asl.Serialize(ar,  "EVENTCONT_ATTR");
     AS(ar, gpdlData);
     ar << gpdlIsBinary;
     ar << partyX;
     ar << partyY;
     //memorizedSpellDbKey.Serialize(ar, version);//ar << memorizedSpellDbKey;
     ar << memorizedSpellID;
     ar << memorizedSpellClass;
     ar << memorizedSpellLevel;
  }
  else
  {
     int temp;   
     ar >> temp; 
     eventStatusUnused = temp;  
     ar >> temp;
     eventResultUnused = temp; 
     ar >> temp;
     onceOnly = temp;   
     ar >> temp;
     chainTrigger = (chainTriggerType)temp;   
     ar >> temp;
     eventTrigger = (eventTriggerType)temp;
           //m_giID.Serialize(ar);
#ifdef UAFEDITOR
     if (version < VersionSpellNames)
     {
       int preSpellNameKey;
       ar >> preSpellNameKey;
       if (preSpellNameKey <= 0)
       {
         itemID.Empty();
       }
       else
       {
         itemID = itemData.FindPreVersionSpellNamesItemID(preSpellNameKey);
       };
     }
     else
#endif
     {
      ar >> itemID;
     };
     ar >> quest;
     ar >> chance;   
     ar >> temp;
     facing = (eventDirType)temp;
#ifdef UAFEDITOR
     if (version < VersionSpellNames)
     {
       int race;
     //ar >> temp;
     //race = (raceType)temp;
       ar >> race;
       switch(race)
       {
       case Human:    raceID = "Human";    break;
       case Elf:      raceID = "ELf";      break; 
       case Dwarf:    raceID = "Dwarf";    break;
       case Gnome:    raceID = "Gnome";    break; 
       case HalfElf:  raceID = "HalfElf";  break; 
       case Halfling: raceID = "Halfling"; break;
       };
     }
     else
#endif
     {
       ar >> raceID;
     };
#ifdef UAFEDITOR
     if (version < VersionSpellNames)
     {
       int iclass;
       ar >> iclass;
       classBaseclassID.classID = ConvertClassTypeToClassID(iclass);
     // >> temp;
     //charClass = (classType)temp;
     }
     else
#endif
     {
       //ar >> classID;
       classBaseclassID.Serialize(ar,(eventTrigger==BaseclassInParty)||(eventTrigger==BaseclassNotInParty));
     };

     if (LoadingVersion >= _VERSION_0820_)
     {
#ifdef UAFEDITOR
       if (version < VersionSpellNames)
       {
         int npc, indx;
         ar >> npc;
         if (npc < 0)
         {
           characterID.Empty();
         }
         else
         {
           indx = globalData.charData.LocatePreSpellNamesKey(npc);
           if (indx >= 0) 
           {
             characterID = globalData.charData.PeekCharacter(indx)->CharacterID();
           }
           else
           {
             characterID.Empty();
           };
         };
       //ar >> npc;
       }
       else
#endif
       {
         ar >> characterID;
       };
     };
     if (LoadingVersion >= _VERSION_0566_)
      eventcontrol_asl.Serialize(ar, "EVENTCONT_ATTR");

     if (LoadingVersion >= _VERSION_0880_)
     {
       DAS(ar, gpdlData);
       ar >> gpdlIsBinary;
     }

     if (LoadingVersion >= _VERSION_0911_)
     {
       ar >> partyX;
       ar >> partyY;
       //memorizedSpellDbKey.Serialize(ar, version);//ar >> memorizedSpellDbKey;
#ifdef UAFEDITOR
       if (LoadingVersion < VersionSpellNames)
       {
         int preSpellNameKey;
         ar >> preSpellNameKey;
         if ( (preSpellNameKey < 0) || (eventTrigger != SpellMemorized))
         {
           memorizedSpellID.Empty();
         }
         else
         {
           memorizedSpellID = spellData.FindPreVersionSpellNamesSpellID(preSpellNameKey);
         };
       }
       else
#endif
       {
         ar >> memorizedSpellID;
       };
       ar >> memorizedSpellClass;
       ar >> memorizedSpellLevel;
     }
  }   

  PostSerialize(ar.IsStoring());
}

#ifdef UAFEDITOR

const char *JKEY_CONTROL = "control";
const char *JKEY_CHAINTRIGGER = "chainTrigger";
const char *JKEY_EVENTTRIGGER = "eventTrigger";
const char *JKEY_ONCEONLY = "onceOnly";
const char *JKEY_RACE = "race";
const char *JKEY_CLASS = "class";
const char *JKEY_BASECLASS = "baseclass";
const char *JKEY_ITEM = "item";
const char *JKEY_GENDER = "gender";
const char *JKEY_QUEST = "quest";
const char *JKEY_CHARACTER = "character";
const char *JKEY_SPELL = "spell";
const char *JKEY_CHANCE = "chance";
//const char *JKEY_SPECIALITEM = "specialItem";
const char *JKEY_SPECIALKEY = "specialKey";
const char *JKEY_PARTYXY = "partyXY";
const char *JKEY_FACING = "facing";
const char *JKEY_SPELLCLASS = "spellClass";
const char *JKEY_SPELLLEVEL = "spellLevel";
const char *JKEY_GPDLDATA = "gpdlData";
const char *JKEY_GPDLISBINARY = "gpdlIsBinary";
const char *JKEY_GPDLBINARY = "gpdlBinary";


void EVENT_CONTROL::Export(JWriter& jw)
{
  int i[2];
  jw.StartList(JKEY_CONTROL);
  jw.NameAndNonZero       (JKEY_ONCEONLY,     onceOnly);
  if (chainTrigger != AlwaysChain)
  {
    jw.NameAndEnum   (JKEY_CHAINTRIGGER, chainTrigger);
  };
  if (eventTrigger != AlwaysTrigger)
  {
    jw.NameAndEnum   (JKEY_EVENTTRIGGER, eventTrigger);
  };
  if ((eventTrigger==RaceInParty)||(eventTrigger==RaceNotInParty))
  {
    jw.NameAndQuotedNonBlank(JKEY_RACE,         (LPCSTR)raceID);
  };
  jw.NameAndQuotedNonBlank(JKEY_CLASS,        (LPCSTR)classBaseclassID.classID);
  jw.NameAndQuotedNonBlank(JKEY_BASECLASS,    (LPCSTR)classBaseclassID.baseclassID);
  jw.NameAndQuotedNonBlank(JKEY_ITEM,         (LPCSTR)itemID);
  jw.NameAndQuotedNonBlank(JKEY_CHARACTER,    (LPCSTR)characterID);
  jw.NameAndQuotedNonBlank(JKEY_SPELL,        (LPCSTR)memorizedSpellID);
  jw.NameAndNonZero       (JKEY_SPELLCLASS,   memorizedSpellClass);
  jw.NameAndNonZero       (JKEY_SPELLLEVEL,   memorizedSpellLevel);
  if (gpdlIsBinary)
  {
    jw.NameAndBool          (JKEY_GPDLBINARY,   gpdlIsBinary);
  };
  if (!gpdlData.IsEmpty())
  {
    if (gpdlIsBinary)
    {
      jw.NameAndBinary        (JKEY_GPDLBINARY, gpdlData);
    }
    else
    {
      jw.NameAndMultiLineString(JKEY_GPDLDATA,   (LPCSTR)gpdlData);
    };
  };
  if ((eventTrigger==GenderInParty)||(eventTrigger==GenderNotInParty))
  {
    jw.NameAndEnum         (JKEY_GENDER,      gender);
  };
  jw.NameAndNonNegative   (JKEY_QUEST,        quest);
  if (chance != 100)
  {
    jw.NameAndValue         (JKEY_CHANCE,       chance);
  };
  jw.NameAndEnum(JKEY_FACING, facing);
  jw.NameAndNonNegative   (JKEY_SPECIALITEM,  specialItem);
  jw.NameAndNonNegative   (JKEY_SPECIALKEY,   specialKey);
  i[0] = partyX;
  i[1] = partyY;
  if (eventTrigger == PartyAtXY)
  {
    jw.NameAndValue(JKEY_PARTYXY, i, 2);
  };
  eventcontrol_asl.Export(jw);
  jw.EndList();
}

bool EVENT_CONTROL::Import(JReader& jr)
{
  int i[2];
  if (!jr.StartList       (JKEY_CONTROL)) return false;
  jr.NameAndNonZero       (JKEY_ONCEONLY,     onceOnly);
  chainTrigger = AlwaysChain;
  jr.Optional(); jr.NameAndEnum (JKEY_CHAINTRIGGER, chainTrigger);
  eventTrigger = AlwaysTrigger;
  jr.Optional(); jr.NameAndEnum (JKEY_EVENTTRIGGER, eventTrigger);
  jr.NameAndQuotedNonBlank(JKEY_RACE,         raceID);
  jr.NameAndQuotedNonBlank(JKEY_CLASS,        classBaseclassID.classID);
  jr.NameAndQuotedNonBlank(JKEY_BASECLASS,    classBaseclassID.baseclassID);
  jr.NameAndQuotedNonBlank(JKEY_ITEM,         itemID);
  jr.NameAndQuotedNonBlank(JKEY_CHARACTER,    characterID);
  jr.NameAndQuotedNonBlank(JKEY_SPELL,        memorizedSpellID);
  jr.Optional(); memorizedSpellClass = 0;
  jr.NameAndValue         (JKEY_SPELLCLASS,   memorizedSpellClass);
  jr.Optional(); memorizedSpellLevel = 0;
  jr.NameAndValue         (JKEY_SPELLLEVEL,   memorizedSpellLevel);
  gpdlIsBinary = false;
  jr.Optional(); jr.NameAndBool(JKEY_GPDLBINARY,   gpdlIsBinary);
  gpdlData.Empty();
  jr.Optional();
  if (gpdlIsBinary)
  {
    jr.NameAndBinary      (JKEY_GPDLDATA,     gpdlData);
  }
  else
  {
    jr.NameAndMultiLineString(JKEY_GPDLDATA, gpdlData);
  };
  jr.Optional(); jr.NameAndEnum(JKEY_GENDER,  gender);
  jr.NameAndNonNegative   (JKEY_QUEST,        quest);
  chance = 100;
  jr.Optional(); jr.NameAndValue(JKEY_CHANCE, chance);
  jr.NameAndEnum          (JKEY_FACING,       facing);
  jr.NameAndNonNegative   (JKEY_SPECIALITEM,  specialItem);
  jr.NameAndNonNegative   (JKEY_SPECIALKEY,   specialKey);
  if (eventTrigger == PartyAtXY)
  {
    jr.NameAndValue(JKEY_PARTYXY, i, 2);
  };
  partyX = i[0];
  partyY = i[1];
  eventcontrol_asl.Import(jr);
  jr.EndList();
  return true;
}

#endif

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************


GameEvent::GameEvent() : 
  chainEventHappen(0),
  chainEventNotHappen(0),
  // ASLs are named "EVENT_DATA_ATTR"
  event_asl(),
  temp_asl()
{
  GameEvent::Clear();
}

#ifdef UAFEDITOR
CString GameEvent::Name(void)
{
  eventType type;
  type = GetEventType();
  if (type >= NUM_EVENT_TYPES) return CString("???");
  return CString(EventListText[type]);
};
#else
const char *GameEvent::Name(void)
{
  static char name[20];
  sprintf(name,"[%d]", GetEventType());
  return name;
}
#endif


GameEvent& GameEvent::operator =(const GameEvent& src)
{
  if (this == &src)
    return *this;

   id = src.id;
   //IsGlobal=src.IsGlobal;
   m_source=src.m_source;
   event = src.event;
   x = src.x;
   y = src.y;
   control = src.control;
   pic = src.pic; 
   pic2 = src.pic2; 
   if (!src.text.IsEmpty())
      text = src.text;
   else
      text="";
   if (!src.text2.IsEmpty())
      text2 = src.text2;
   else
      text2="";
   if (!src.text3.IsEmpty())
      text3 = src.text3;
   else
      text3="";

   chainEventHappen = src.chainEventHappen;
   chainEventNotHappen = src.chainEventNotHappen;
   event_asl.Copy(src.event_asl);
   temp_asl.Copy(src.temp_asl);

   return *this;
}
#ifdef UAFEDITOR
bool GameEvent::operator ==(const GameEvent& src) const
{
  die("Should never be used!");
  return false;
}

bool GameEvent::Equals(const GameEvent& src) const
{
  if (this == &src) return true;
  if (id != src.id) return false;
  if (m_source != src.m_source) return false;
  if (event != src.event) return false;
  if (x != src.x) return false;
  if (y != src.y) return false;
  if (!(control == src.control)) return false;
  if (!(pic == src.pic)) return false; 
  if (!(pic2 == src.pic2)) return false; 
  if (text != src.text) return false;
  if (text2 != src.text2) return false;
  if (text3 != src.text3) return false;
  if (chainEventHappen != src.chainEventHappen) return false;
  if (chainEventNotHappen != src.chainEventNotHappen) return false;
  if (!(event_asl == src.event_asl)) return false;
  if (!(temp_asl == src.temp_asl)) return false;
  return true;
}
#endif

GameEvent::GameEvent(const GameEvent& src) :
  chainEventHappen(0),
  chainEventNotHappen(0),
  //ASLs are named "EVENT_DATA_ATTR"
  event_asl(),
  temp_asl()
{
  *this = src;
}

GameEvent::GameEvent(const GameEvent *src) :
  chainEventHappen(0),
  chainEventNotHappen(0),
  // ASLs are named "EVENT_DATA_ATTR"
  event_asl(),
  temp_asl()
{
  *this = *src;
}

void GameEvent::Clear()
{   
  WarnOfClearingWithChainsPresent(this);

  event = NoEvent;
  id = 0;
  x = -1;
  y = -1;
  text="";
  text2="";
  text3="";
  pic.Clear();
  pic2.Clear();
  control.Clear();  
  chainEventHappen = 0;
  chainEventNotHappen = 0;
  //IsGlobal=FALSE;
  m_source=LevelEventSrc;
  event_asl.Clear();
  temp_asl.Clear();
  timer = 0;
}

#ifdef UAFEngine

BOOL GameEvent::EventShouldTrigger()
{
   BOOL shouldTrigger=FALSE;
   //eventLocationRestriction location;
   
   if (event == NoEvent)
   {
      WriteDebugString("Event NOT triggered: bogus event type\n");
      return FALSE;
   }
   
   //
   // location = GetEventLocationRestriction(event);
   //
   // if (location != AllowAnywhere)
   // {
   //    if ((InWilderness) && (location == AllowInCities))
   //      shouldTrigger = FALSE;
   //    if ((!InWilderness) && (location == AllowInWilderness))
   //      shouldTrigger = FALSE;
   // }
   //
   
   //if (!shouldTrigger)
   {
     // onceOnly should never be set in evets of type CONTROL_something
     // Otherwise, we could check for event >= CONTROL_Splash
     BOOL alreadyHappened;

     /*
     if (IsGlobalEvent())
       alreadyHappened = globalData.eventData.HasEventHappened(id);
     else
       alreadyHappened = levelData.eventData.HasEventHappened(id);
      */

    switch (GetEventSource())
    {
    case LevelEventSrc:
      alreadyHappened = levelData.eventData.HasEventHappened(id);
      break;
    case GlobalEventSrc:
      alreadyHappened = globalData.eventData.HasEventHappened(id);
      break;
  #ifdef UAFEDITOR
    case CopyEventSrc:
      alreadyHappened = CopyEventData.HasEventHappened(id);
      break;
  #endif
    default:
      die(0xab50d);
      return FALSE;
    }

     if (control.onceOnly && alreadyHappened)
       WriteDebugString("Event %d NOT triggered: Once Only\n", id);
     else
     {
       setPartyXY_x = -1;
       shouldTrigger = control.EventShouldTrigger(id);
     };
   }
   return shouldTrigger;
}

#endif

BOOL GameEvent::IsChained()
{
  if ((control.chainTrigger == AlwaysChain) && (chainEventHappen > 0))
    return TRUE;

  if ((control.chainTrigger == IfEventHappen) && (chainEventHappen > 0))
    return TRUE;

  if ((control.chainTrigger == IfEventNotHappen) && (chainEventNotHappen > 0))
    return TRUE;

  return FALSE;
}

#if defined UAFEDITOR
BOOL GameEvent::GetChainedEvent(DWORD &cid)
{
  BOOL found = FALSE;

  if ((control.chainTrigger == AlwaysChain) && (chainEventHappen > 0))
  {
    if (chainEventNotHappen > 0)
    {
      die(0xab50e);
      if (chainEventHappen!=chainEventNotHappen) die(0xab50f); // shouldn't happen
      chainEventNotHappen=0;
    }
    cid = chainEventHappen;
    found = TRUE;
  }
  else if ((control.chainTrigger == IfEventHappen) && (chainEventHappen > 0))
  {
    if (chainEventNotHappen > 0)
    {
      die(0xab510);
      if (chainEventHappen!=chainEventNotHappen) die(0xab511); // shouldn't happen
      chainEventNotHappen=0;        
    }
    cid = chainEventHappen;
    found = TRUE;
  }
  else if ((control.chainTrigger == IfEventNotHappen) && (chainEventNotHappen > 0))
  {
    if (chainEventHappen > 0)
    {
      die(0xab512);
      if (chainEventHappen!=chainEventNotHappen) die(0xab513); // shouldn't happen
      chainEventHappen=0;        
    }
    cid = chainEventNotHappen;
    found = TRUE;
  }

  return found;
}
#endif // UAFEDITOR


void GameEvent::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=1;
  if (chainEventHappen > 0)
    curr++;
  else if (chainEventNotHappen > 0)
    curr++;
}

void GameEvent::FreeEventResources()
{
  pic.FreePic();
  pic2.FreePic();
}

void GameEvent::SaveEventResources(int level)
{
  pic.SavePicData(level);
  pic2.SavePicData(level);  
}

void GameEvent::PostSerialize(BOOL IsStoring, double version)
{
  if (!IsStoring)
  {
    switch (GetEventType())
    {
   // case CombatTreasure: event=GiveTreasure; break;    

    case Stairs:
    case Teleporter: event=TransferModule; break;

    case InnEvent: die(0xab514); break;

    case TavernTales: 
    default:
      // do nothing
      break;
    };
    //
    // Adjust old triggering conditions
    // We now have explicit triggering for each of
    //  Item
    //  Special Item
    //  Special Key
    //  Quest
    // If the old trigger is "Party [Not] have item" and the itemID
    // is blank, then perhaps it needs to be changed to one of the
    // new triggering conditions.
    if (version < 0.999680)
    {
      if (    (control.eventTrigger == PartyHaveItem) 
           || (control.eventTrigger == PartyNotHaveItem))
      {
        if (!control.itemID.IsValidItem())
        {
          // Probably need to update trigger condition
          if (control.specialItem >= 0)
          {
            if (control.eventTrigger == PartyHaveItem) control.eventTrigger = PartyHaveSpecialItem;
            else control.eventTrigger = PartyNotHaveSpecialItem;
          }
          else if (control.specialKey >= 0)
          {
            if (control.eventTrigger == PartyHaveItem) control.eventTrigger = PartyHaveSpecialKey;
            else control.eventTrigger = PartyNotHaveSpecialKey;
          }
          else if (control.quest >= 0)
          {
            if (control.eventTrigger == PartyHaveItem) control.eventTrigger = QuestPresent;
            else control.eventTrigger = QuestNotPresent;
          }
          else
          {
            WriteDebugString("Triggering for eventID=%d is ambiguous\n", event);
          };
        };
      };
    };
  };
}

#ifdef UAFEDITOR

void GameEvent::Export(JWriter& jw)
{
  die("Export of GameEvent not implemented");
}


bool GameEvent::Import(JReader& jr)
{
  die("Import of GameEvent not implemented");
  return false;
}

const char *JKEY_EVENTTYPE = "eventType";
const char *JKEY_GENERAL   = "general";
extern const char *JKEY_XY;
const char *JKEY_EVENTID   = "eventID";
const char *JKEY_TEXT      ="text";
const char *JKEY_TEXT2     ="text2";
const char *JKEY_TEXT3     ="text3";
const char *JKEY_CHAINEVENTHAPPEN = "chainEventHappen";
const char *JKEY_CHAINEVENTNOTHAPPEN = "chainEventNotHappen";
const char *JKEY_TIMER     = "timer";
const char *JKEY_PICTURE1     = "picture1";
const char *JKEY_PICTURE2     = "picture2";
extern const char *JKEY_PARTICULAR;

void GameEvent::ExportEvent(JWriter& jw)
{
  int i[2];
  jw.ListEntry();
  {
    jw.StartList(JKEY_GENERAL);
    jw.NameAndQuotedValue(JKEY_EVENTTYPE, EventListText[event]);
    jw.NameAndValue(JKEY_EVENTID, id);
    i[0] = x;
    i[1] = y;
    jw.NameAndValue(JKEY_XY, i, 2);
    control.Export(jw);
    pic.Export(jw, JKEY_PICTURE1);
    pic2.Export(jw, JKEY_PICTURE2);
    jw.NameAndQuotedNonBlank(JKEY_TEXT, text);
    jw.NameAndQuotedNonBlank(JKEY_TEXT2, text2);
    jw.NameAndQuotedNonBlank(JKEY_TEXT3, text3);
    jw.NameAndNonZero(JKEY_CHAINEVENTHAPPEN,chainEventHappen);
    jw.NameAndNonZero(JKEY_CHAINEVENTNOTHAPPEN,chainEventNotHappen);
    jw.NameAndNonZero(JKEY_TIMER, timer);
    jw.EndList();
    jw.StartList(JKEY_PARTICULAR);
    Export(jw);
    jw.EndList();
  };
  jw.EndList();
  jw.Linefeed(true);
}

bool GameEvent::ImportEvent(JReader& jr)
{
  int i[2];
  jr.NameAndValue(JKEY_EVENTID, id);
  jr.NameAndValue(JKEY_XY, i, 2);
  x = i[0];
  y = i[1];
  control.Import(jr);
  jr.Optional(); pic.Import(jr, JKEY_PICTURE1);
  jr.Optional(); pic2.Import(jr, JKEY_PICTURE2);
  jr.Optional(); jr.NameAndQuotedNonBlank(JKEY_TEXT, text);
  jr.Optional(); jr.NameAndQuotedNonBlank(JKEY_TEXT2, text2);
  jr.Optional(); jr.NameAndQuotedNonBlank(JKEY_TEXT3, text3);
  jr.NameAndNonZero(JKEY_CHAINEVENTHAPPEN,chainEventHappen);
  jr.NameAndNonZero(JKEY_CHAINEVENTNOTHAPPEN,chainEventNotHappen);
  if (control.chainTrigger == AlwaysChain) chainEventNotHappen = 0;
  if (control.chainTrigger == IfEventHappen) chainEventNotHappen = 0;
  if (control.chainTrigger == IfEventNotHappen) chainEventHappen = 0;
  jr.Optional(); jr.NameAndValue(JKEY_TIMER, timer);
  jr.EndList();
  if (jr.Optional(), !jr.StartList(JKEY_PARTICULAR)) return false;
  Import(jr);
  jr.EndList();
  return true;
}

#endif

void GameEvent::Serialize( CArchive &ar, double version )
{
  PreSerialize(ar.IsStoring());

//  CObject::Serialize(ar);
  control.Serialize(ar, version);
#ifdef SIMPLE_STRUCTURE
  pic.Serialize(ar, LoadingVersion, rte.PicArtDir());
  pic2.Serialize(ar, LoadingVersion, rte.PicArtDir());
#else
  pic.Serialize(ar, LoadingVersion);
  pic2.Serialize(ar, LoadingVersion);
#endif

  if (ar.IsStoring())
  {
     ar << (int)event;
     ar << id;
     ar << x;
     ar << y;
     ar << chainEventHappen;
     ar << chainEventNotHappen;
  
     AS(ar,text)
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;

     AS(ar,text2)
//     if (text2.IsEmpty())
//     {
//        text2 = "*";
//        ar << text2;
//        text2="";
//     }
//     else
//        ar << text2;

     AS(ar,text3)
//     if (text3.IsEmpty())
//     {
//        text3 = "*";
//        ar << text3;
//        text3="";
//     }
//     else
//        ar << text3;

     //ar << IsGlobal;
     event_asl.Serialize(ar, "EVENT_DATA_ATTR");
  }
  else
  {
     int temp;
     DWORD tid;
     ar >> temp;
     event = (eventType)temp;
     ar >> tid;
     id = tid;
     ar >> x;
     ar >> y;
     ar >> chainEventHappen;
     ar >> chainEventNotHappen;

     DAS(ar,text)
     DAS(ar,text2)
     DAS(ar,text3)
//     ar >> text;
//     ar >> text2;
//     ar >> text3;
//
//     if (text == "*")
//       text = "";
//     if (text2 == "*")
//       text2 = "";
//     if (text3 == "*")
//       text3 = "";

     // load event attributes if version indicates
     // their presence
     if (LoadingVersion >= _VERSION_0564_)
       event_asl.Serialize(ar, "EVENT_DATA_ATTR");
  }

  PostSerialize(ar.IsStoring(), version);
}

void GameEvent::Serialize( CAR &ar, double version )
{
  PreSerialize(ar.IsStoring());

  //CObject::Serialize(ar);
  control.Serialize(ar, version);
  pic.Serialize(ar, LoadingVersion, rte.PicArtDir());
  pic2.Serialize(ar, LoadingVersion, rte.PicArtDir());

  if (ar.IsStoring())
  {
     ar << (int)event;
     ar << id;
     ar << x;
     ar << y;
     ar << chainEventHappen;
     ar << chainEventNotHappen;
  
     AS(ar,text);
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;

     AS(ar,text2);
//     if (text2.IsEmpty())
//     {
//        text2 = "*";
//        ar << text2;
//        text2="";
//     }
//     else
//        ar << text2;

     AS(ar,text3);
//     if (text3.IsEmpty())
//     {
//        text3 = "*";
//        ar << text3;
//        text3="";
//     }
//     else
//        ar << text3;

     //ar << IsGlobal;
     event_asl.Serialize(ar, "EVENT_DATA_ATTR");
  }
  else
  {
     int temp;
     DWORD tid;
     ar >> temp;
     event = (eventType)temp;
     ar >> tid;
     id = tid;
     ar >> x;
     ar >> y;
     ar >> chainEventHappen;
     ar >> chainEventNotHappen;

     DAS(ar,text);
     DAS(ar,text2);
     DAS(ar,text3);
//     ar >> text;
//     ar >> text2;
//     ar >> text3;
//
//     if (text == "*")
//       text = "";
//     if (text2 == "*")
//       text2 = "";
//     if (text3 == "*")
//       text3 = "";

     // load event attributes if version indicates
     // their presence
     if (LoadingVersion >= _VERSION_0564_)
       event_asl.Serialize(ar, "EVENT_DATA_ATTR");
  }

  PostSerialize(ar.IsStoring(), version);
}

void NULL_EVENT_DATA::Serialize(CArchive& ar, double version)
{
  GameEvent::Serialize(ar, version);
}

void NULL_EVENT_DATA::Serialize(CAR& car, double version)
{
  GameEvent::Serialize(car, version);
}

#ifdef UAFEDITOR
void NULL_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("NULL_EVENT_DATA::DetailedCrossReference");
}
void NULL_EVENT_DATA::Export(JWriter& jw)
{
  NotImplemented(0xcd940, false);
}

bool NULL_EVENT_DATA::Import(JReader& jr)
{
  NotImplemented(0xcd941, false);
  return true;
}


void GameEvent::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("Someone forgot to override GameEvent::DetailedCrossReference");
}

void GameEvent::CreateCRReference(CR_EVENT_INFO *pCREI)
{
  if ( (x >= 0) && (pCREI->m_anchorLocation == "") )
  {
    pCREI->m_anchorLocation.Format("%d,%d,%d",pCREI->m_level, x, y);
  };
  pCREI->m_CRReference.m_referenceName.Format("%s(%s)%d", Name(),pCREI->m_anchorLocation,id);
  pCREI->m_CRReference.m_referenceType = CR_TYPE_event;
}

void GameEvent::CrossReferenceEventItems(CR_EVENT_INFO *pCREI, ITEM_LIST *itemList)
{
  CreateCRReference(pCREI);
  itemList->CrossReference(pCREI->m_pCRList, &pCREI->m_CRReference);
}

void GameEvent::CrossReferenceEventItems(CR_EVENT_INFO *pCREI, SPECIAL_OBJECT_EVENT_LIST *itemList)
{
  CreateCRReference(pCREI);
  itemList->CrossReference(pCREI->m_pCRList, &pCREI->m_CRReference);
}



void GameEvent::CrossReferenceEventSound(CR_EVENT_INFO *pCREI, const CString& sound)
{
  pCREI->m_pCRList->CR_AddSoundReference(sound, &pCREI->m_CRReference);
}

void GameEvent::CrossReferenceEventSpells(CR_EVENT_INFO *pCREI, spellBookType *pSpellBook)
{
  MsgBoxInfo("GameEvent::CrossReferenceEventSpells not implemented");
}


/*
void GameEvent::CrossReferenceEventChain(CR_EVENT_INFO *pCREI, DWORD eventID)
{
  GameEvent *pEvent;
  if (!pCREI->m_pEventList->IsValidEvent(eventID)) return;
  pEvent = pCREI->m_pEventList->GetEvent(eventID);
  pEvent->CrossReference(pCREI);
}
*/
#endif

void GameEventList::PreSerialize(BOOL IsStoring)
{
  POSITION p = GetHeadPosition();
  while (p!=NULL) 
    GetNext(p)->PreSerialize(IsStoring);
}

void GameEventList::PostSerialize(BOOL IsStoring, double version)
{
  POSITION p = GetHeadPosition();
  while (p!=NULL) 
    GetNext(p)->PostSerialize(IsStoring, version);
}


void GameEventList::Restore(CArchive& ar) 
{
	int count;
	DWORD id;
	POSITION p;
	NULL_EVENT_DATA junk; //GameEvent junk;
	// Let events know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ATTRIBUTES in case no restore operation clears it.
	p=GetHeadPosition();
  while (p!=NULL) 
    GetNext(p)->PrepareForRestore();

	ar >> count;

	for (int i=0; i<count; i++) 
  {
		ar >> id;

		p=GetHeadPosition();
		while (p!=NULL) 
    {
			if (id == PeekAtPos(p)->id)
			{
				GetAtPos(p)->Restore(ar);
				break;
			}
			GetNext(p);
		}
		if (p==NULL) 
		{	
			CString msg;
			msg.Format("SAVE-FILE GameEvent \"%u\" does not exist in design\n",
				         id);
			WriteDebugString(msg);
			junk.Restore(ar); // Discard saved data.  Any better ideas?
		}
	}
}

void GameEventList::CommitRestore() 
{
	POSITION p;
	p=GetHeadPosition();
	while (p!=NULL) 
    GetNext(p)->CommitRestore();
}

#ifdef UAFEDITOR
char *EventSourceText[] = 
{
  "Level",
  "Global",
  "Copy"
};
#endif

void GameEventList::Clear() 
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos = GetHeadPosition();
  if (pos!=NULL)
    WriteDebugString("GameEventList::Clear() deleting all events\n");

  while (pos != NULL)
  {
#ifdef UAFEDITOR
    GameEvent *pevent;
    pevent = m_events.GetAtPos(pos);
    WriteDebugString("Deleting '%s' %s event\n",
                     GetEventIdDescription(m_events.PeekAtPos(pos)->id,GetEventSource()),
                     EventSourceText[(int)GetEventSource()]);
#endif
    delete m_events.GetAtPos(pos);
    m_events.GetNext(pos);
  }
  m_events.RemoveAll(); 
  m_nextKey=1;
  m_ListType=LevelEventSrc;
}

int GameEventList::AddEvent(GameEvent *data)
{
//  CSingleLock sLock(&m_CS, TRUE);
  if (GetCount() < MAX_EVENTS)
  {
    data->id = GetNextKey();
    //data->SetGlobalEvent( IsGlobalList() );
    data->SetEventSource( GetEventSource() );
    m_events.Insert(data, data->id);
#if ((defined UAFEDITOR) && (defined _DEBUG))
    DWORD count = GetCount();
    count = max(0,count);
    WriteDebugString("Added '%s' %s event: x %i, y %i - (%u total)\n",
                    GetEventIdDescription(data->id,GetEventSource()), 
                    //(IsGlobalList()?"global":"level"),
                    EventSourceText[(int)data->GetEventSource()],
                    data->x, data->y,
                    count);
#endif
    return data->id;
  }
  else
    return 0;
}

// called when loading binary monster data that
// was saved with key info
int GameEventList::AddEventWithCurrKey(GameEvent *data)
{
//  CSingleLock sLock(&m_CS, TRUE);
  if (GetCount() < MAX_EVENTS)
  {
    //data->SetGlobalEvent(IsGlobalList());
    data->SetEventSource( GetEventSource() );
    m_events.Insert(data, data->id);
    if (m_nextKey<=data->id) m_nextKey=data->id+1;
#if ((defined UAFEDITOR) && (defined _DEBUG))
    DWORD count = GetCount();
    count = max(0,count);

    WriteDebugString("Added '%s' %s event: x %i, y %i - (%u total)\n",
                    GetEventIdDescription(data->id,GetEventSource()), 
                    //(IsGlobalList()?"global":"level"),
                    EventSourceText[(int)data->GetEventSource()],
                    data->x, data->y,
                    count);
#endif
    return data->id;
  }
  else
    return 0;
}

GameEvent *GameEventList::GetEvent(DWORD id) 
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAtPos(pos);
  else
    return NULL;
}

BOOL GameEventList::IsChained(GameEvent *event)
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  if (IsValidEvent(event->id))
    return (event->IsChained()); 
  else
    return FALSE;
}

BOOL GameEventList::IsValidEvent(DWORD num) 
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(num)) != NULL)
    return (IsValidEvent(m_events.PeekAtPos(pos))); 
  else
    return FALSE;
}

BOOL GameEventList::IsValidEventChainWord(DWORD num) 
{ // Zero or a valid event
  // Many times the chain words of an event did not get cleared
  // when the chained event was deleted.  So I added ASSERTs
  // in EventViewer's PopulateTree to help discover those cases.  PRS
  // I also added code to use event IDs only once.  We used to reuse
  // an event ID if the last event was deleted and then a new event added.
  if (num==0) return TRUE;
//  CSingleLock sLock(&m_CS, TRUE);
  if (m_events.FindKeyPos(num) != NULL) return TRUE;
  return FALSE;
}


BOOL GameEventList::IsValidEvent(GameEvent *event) const 
{ 
  if (event != NULL) 
    return (event->IsValidEvent());
  else
    return FALSE;
}

void GameEventList::DeleteEvents(DWORD id)
{
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    DeleteEvents(m_events.GetAtPos(pos));
}

void GameEventList::DeleteEvents(GameEvent *event)
{
//  CSingleLock sLock(&m_CS, TRUE);
  DeleteEventAndAllChains(event);
}

// recursive, so make sure event chains don't loop back to
// themselves
void GameEventList::DeleteEventAndAllChains(GameEvent *event)
{
   if (!IsValidEvent(event))
      return;

   DeleteEvents(event->chainEventHappen);
   event->chainEventHappen = 0;

   DeleteEvents(event->chainEventNotHappen);
   event->chainEventNotHappen = 0;

   DeleteEvent(event);
}

void GameEventList::DeleteEvent(GameEvent *event)
{
   if (!IsValidEvent(event))
   {
     EventChainError("Delete Invalid Event", false);
     return;
   };

   if (event->IsChained())
   {
      int i;
      switch (event->GetEventType())
      {
      case TavernEvent:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(TAVERN)));
          TAVERN *data = (TAVERN*)(event);        

          DeleteEvents(data->drinkChain);
          data->drinkChain = 0;
          DeleteEvents(data->fightChain);
          data->fightChain = 0;          
        }
        break;

      case TempleEvent:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(TEMPLE)));
          TEMPLE *data = (TEMPLE*)(event);        

          DeleteEvents(data->donationChain);
          data->donationChain = 0;
        }
        break;

      case QuestStage:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUEST_EVENT_DATA)));
          QUEST_EVENT_DATA *data = (QUEST_EVENT_DATA*)(event);        

          DeleteEvents(data->acceptChain);
          data->acceptChain = 0;
          DeleteEvents(data->rejectChain);
          data->rejectChain = 0;
        }
        break;

      case ChainEventType:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(CHAIN_EVENT)));
          CHAIN_EVENT *data = (CHAIN_EVENT*)(event);        

          DeleteEvents(data->Chain);
          data->Chain = 0;
        }
        break;

      case RandomEvent:
        {
          RANDOM_EVENT_DATA *data = (RANDOM_EVENT_DATA*)(event);
          for (i=1;i<RANDOM_EVENT_DATA::MAX_RANDOM_EVENTS;i++)
          {
            DeleteEvents(data->EventChains[i-1]);
            data->EventChains[i-1]=0;
          }
        }
        break;

      case SmallTown:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(SMALL_TOWN_DATA)));
          SMALL_TOWN_DATA *data = (SMALL_TOWN_DATA*)(event);        

          DeleteEvents(data->TempleChain);
          data->TempleChain = 0;
          DeleteEvents(data->TrainingHallChain);
          data->TrainingHallChain = 0;
          DeleteEvents(data->ShopChain);
          data->ShopChain = 0;
          DeleteEvents(data->InnChain);
          data->InnChain = 0;
          DeleteEvents(data->TavernChain);
          data->TavernChain = 0;
          DeleteEvents(data->VaultChain);
          data->VaultChain = 0;
        }
        break;

	   case EncounterEvent:
       {
         //ASSERT(event->IsKindOf(RUNTIME_CLASS(ENCOUNTER_DATA)));
         ENCOUNTER_DATA *data = (ENCOUNTER_DATA*)(event);        

		     for (i=0;i<MAX_BUTTONS;i++)
		     {
           DeleteEvents(data->buttons.buttons[i].chain);
           data->buttons.buttons[i].chain = 0;
		     }
         DeleteEvents(data->combatChain);
         data->combatChain=0;
         DeleteEvents(data->talkChain);
         data->talkChain=0;
         DeleteEvents(data->escapeChain);
         data->escapeChain=0;
       }
       break;

      case QuestionList:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_LIST_DATA)));
           QUESTION_LIST_DATA *data = (QUESTION_LIST_DATA*)(event);        

           for (i=0;i<MAX_BUTTONS;i++)
           {
             DeleteEvents(data->buttons.buttons[i].chain);
             data->buttons.buttons[i].chain = 0;
           }
        }
        break;
   
      case QuestionButton:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_BUTTON_DATA)));
           QUESTION_BUTTON_DATA *data = (QUESTION_BUTTON_DATA*)(event);        

           for (i=0;i<MAX_BUTTONS;i++)
           {
             DeleteEvents(data->buttons.buttons[i].chain);
             data->buttons.buttons[i].chain = 0;
           }
        }
        break;

      case QuestionYesNo:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_YES_NO)));
          QUESTION_YES_NO *data = (QUESTION_YES_NO*)(event);        

          DeleteEvents(data->YesChain);
          data->YesChain = 0;
          DeleteEvents(data->NoChain);
          data->NoChain = 0;
        }
        break;

      case EnterPassword:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(PASSWORD_DATA)));
           PASSWORD_DATA *data = (PASSWORD_DATA*)(event);        

           DeleteEvents(data->successChain);
           data->successChain = 0;
           DeleteEvents(data->failChain);
           data->failChain = 0;
        }
        break;

      case WhoPays:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_PAYS_EVENT_DATA)));
           WHO_PAYS_EVENT_DATA *data = (WHO_PAYS_EVENT_DATA*)(event);        

           DeleteEvents(data->successChain);
           data->successChain = 0;
           DeleteEvents(data->failChain);
           data->failChain = 0;
        }
        break;

      case WhoTries:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_TRIES_EVENT_DATA)));
           WHO_TRIES_EVENT_DATA *data = (WHO_TRIES_EVENT_DATA*)(event);        
          
           DeleteEvents(data->successChain);
           data->successChain = 0;
           DeleteEvents(data->failChain);
           data->failChain = 0;
        }
        break;

      case LogicBlock:
        {
           LOGIC_BLOCK_DATA *data = (LOGIC_BLOCK_DATA*)(event);        
          
           DeleteEvents(data->falseChain);
           data->falseChain = 0;
           DeleteEvents(data->trueChain);
           data->trueChain = 0;
        }
        break;

      default:
        // do nothing
        break;
      }
   }

   DWORD OldId = event->id;
   POSITION pos = m_events.FindKeyPos(OldId);
   if (pos != NULL)
   {
#if ((defined UAFEDITOR) && (defined _DEBUG))
     DWORD count = GetCount()-1;
     count = max(0,count);
     WriteDebugString("Deleted '%s' %s event - %u remaining\n",
                      GetEventIdDescription(OldId,GetEventSource()),
                      //(IsGlobalList()?"global":"level"),
                      EventSourceText[(int)GetEventSource()],
                      count);
#endif
    //event->Clear();
    delete event;
    m_events.RemoveAt(pos);
   }
}

EVENT_CONTROL &GameEventList::GetEventControl(DWORD id) 
{ 
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventControl();
  else
    return NullEvent.GetEventControl();
}

eventType &GameEventList::GetEventType(DWORD id)
{ 
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventType();
  else
    return NullEvent.GetEventType();
}  

PIC_DATA &GameEventList::GetEventPic(DWORD id)  
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventPic();
  else
    return NullEvent.GetEventPic();
}

PIC_DATA &GameEventList::GetEventPic2(DWORD id)  
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventPic2();
  else
    return NullEvent.GetEventPic2();
}

CString &GameEventList::GetEventText(DWORD id)  
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventText();
  else
    return NullEvent.GetEventText();
}

CString &GameEventList::GetEventText2(DWORD id)  
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventText2();
  else
    return NullEvent.GetEventText2();
}

CString &GameEventList::GetEventText3(DWORD id)  
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventText3();
  else
    return NullEvent.GetEventText3();
}

int &GameEventList::GetEventX(DWORD id)  
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventX();
  else
    return NullEvent.GetEventX();
}

int &GameEventList::GetEventY(DWORD id)  
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.PeekAtPos(pos)->GetEventY();
  else
    return NullEvent.GetEventY();
}

DWORD GameEventList::GetNextKey()
{
  DWORD key=0;
  if (GetCount() > 0)
  {
    key = m_events.GetTail()->id;
    if (key < m_nextKey) key=m_nextKey;
    if (key >= MAX_EVENTS)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      POSITION pos = m_events.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_events.GetNext(pos)->id;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // 16 million events is too many events
    }
    else
    {
      if (key < MAX_EVENTS) m_nextKey=key+1;
    };
  }
  else
  {
    if (m_nextKey==0) key=1;
    else key=m_nextKey;
    m_nextKey=key+1;
  };
  return key;
}

BOOL GameEventList::ValidEventNum(DWORD num) const
{ 
  POSITION pos = m_events.FindKeyPos(num);
  if (pos == NULL) return FALSE;
  if (m_events.PeekAtPos(pos) == NULL) return FALSE;
  return TRUE;
}


void GameEvent::PrepareForRestore() 
{
	// In case no Restore comes along then we will simply use what we have.
	temp_asl.Clear();
}


void GameEvent::Restore(CArchive& ar) 
{
  temp_asl.Serialize(ar, "EVENT_DATA_ATTR");
}

void GameEvent::CommitRestore(void) 
{
	event_asl.CommitRestore(&temp_asl);
	temp_asl.Clear();
}


#if (defined UAFEDITOR)

void GameEvent::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }

  if (num == 0)
  {
    switch (control.chainTrigger)
    {
    case AlwaysChain:
    case IfEventHappen:
      if (chainEventNotHappen > 0)
      {
        // these two are mutually exclusive
        die(0xab515);
        if (chainEventHappen!=chainEventNotHappen) die(0xab516); // shouldn't happen
        chainEventNotHappen=0;
      }
      chainEventHappen = cid;
      break;

    case IfEventNotHappen:
      if (chainEventHappen > 0)
      {
        // these two are mutually exclusive
        die(0xab517);
        if (chainEventHappen!=chainEventNotHappen) die(0xab518); // shouldn't happen
        chainEventHappen=0;
      }
      chainEventNotHappen = cid;      
      break;
    }
  }
}

int GameEvent::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We should not point to it anymore.
  // Returns number of chain words cleared.
  ASSERT(num<1); // This ASSERTion fails if you forget to override
                 // this function in a derived class with additional
                 // chain words.  See ENCOUNTER_DATA::UnchainToEvent()
                 // for an example.
  // We ignore num.  We get called when the chained event is deleted.
  // Nothing should point to it.  So it is quicker, easier,
  // and safer (IMHO) to do it this way. PRS
  int result=0;
  if (chainEventNotHappen==eid) {result++;chainEventNotHappen=0;};
  if (chainEventHappen==eid) {result++;chainEventHappen=0;};
  return result;
}


BOOL GameEvent::GetEVChainText(int num, char *ctext)
{
  if (num == 0)
  {
    switch (control.chainTrigger)
    {
    case AlwaysChain:
      sprintf(ctext, 
              "%s: %s", 
              "Normal Chain",
              (LPCSTR)GetEventIdDescription(chainEventHappen,GetEventSource()));
      break;
    case IfEventHappen:
      sprintf(ctext, 
              "%s: %s", 
              "Normal Chain",
              (LPCSTR)GetEventIdDescription(chainEventHappen,GetEventSource()));
      break;
    case IfEventNotHappen:
      sprintf(ctext, 
              "%s: %s", 
              "Normal Chain",
              (LPCSTR)GetEventIdDescription(chainEventNotHappen,GetEventSource()));
      break;
    }
    return TRUE;
  }
  return FALSE;
}


#endif

void GameEventList::markEventHappened(GameEvent *event)
{
//  CSingleLock sLock(&m_CS, TRUE);
  if (!IsValidEvent(event))
    return;
  party.MarkEventHappened(m_level, event->id);
  if (logDebuggingInfo)
  {
    WriteDebugString("Marking event %u happened\n", event->id);
  };
}

BOOL GameEventList::HasEventHappened(DWORD eventID)
{
  return party.HasEventHappened(m_level, eventID);
}

GameEvent *GameEventList::GetFirstEvent(int x, int y)
{ 
//  CSingleLock sLock(&m_CS, TRUE);
  // find first event in chain at x,y
  POSITION pos = GetHeadPosition();

  while (pos != NULL)
  {
    if (   (PeekAtPos(pos)->x == x)
        && (PeekAtPos(pos)->y == y))
    {      
       return (GetAtPos(pos));
    }
    GetNext(pos);
  }

  return NULL;
}
void GameEventList::Serialize( CArchive &ar, double version )
{
//  CSingleLock sLock(&m_CS, TRUE);

  LoadingVersion = version;

//  CObject::Serialize(ar);
  
  if (ar.IsStoring())
  {
    ar << m_level;
    ar << GetCount();
    POSITION pos = GetHeadPosition();

    while (pos != NULL)
    {
      ar << (int)(GetAtPos(pos)->GetEventType());
      GetNext(pos)->Serialize(ar, version);
    }
  }
  else
  {
    GameEvent *data;
    int temp;
    int count;

    ar >> m_level;
    ar >> count;

    for (int i=0; i<count; i++)
    {
      ar >> temp;
      data = CreateNewEvent(temp);

      if (data != NULL)
      {
        data->Serialize(ar, version);
        AddEventWithCurrKey(data);
      }
    }
  }
}

void GameEventList::Serialize( CAR &ar, double version )
{
  //CSingleLock sLock(&m_CS, TRUE);
  //CObject::Serialize(ar);

  LoadingVersion = version;

  
  if (ar.IsStoring())
  {
    ar << m_level;
    ar << GetCount();
    POSITION pos = GetHeadPosition();

    while (pos != NULL)
    {
      ar << (int)(GetAtPos(pos)->GetEventType());
      GetNext(pos)->Serialize(ar, version);
    }
  }
  else
  {
    GameEvent *data;
    int temp;
    int count;

    ar >> m_level;
    ar >> count;

    for (int i=0; i<count; i++)
    {
      ar >> temp;
      data = CreateNewEvent(temp);

      if (data != NULL)
      {
        data->Serialize(ar, version);
        AddEventWithCurrKey(data);
      }
    }
  }
}

#ifdef UAFEDITOR
const char *JKEY_EVENTS = "events";
extern const char *JKEY_EVENT;
const char *JKEY_LEVEL  = "level";


CONFIG_ITEM_STATUS GameEventList::Export(JWriter& jw)
{
  jw.StartArray(JKEY_EVENTS);
  //jw.NameAndValue(JKEY_LEVEL, m_level);
  POSITION pos = GetHeadPosition();
  while (pos != NULL)
  {
    GameEvent *pEvent;
    pEvent = GetNext(pos);
    pEvent->ExportEvent(jw);
  };
  jw.EndArray();
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS GameEventList::Import(JReader& jr)
{
  if (jr.Optional(), !jr.StartArray(JKEY_EVENTS)) return CONFIG_STAT_ok;
  while (jr.Optional(), jr.NextEntry())
  {
    ImportEvent(jr);
  };
  jr.EndArray();
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS GameEventList::ImportEvent(JReader& jr)
{
  if (jr.Optional(), !jr.StartList())
  {
    return CONFIG_STAT_syntax;
  };
  jr.StartList(JKEY_GENERAL);
  {
    GameEvent *pNewEvent;
    eventType type;
    jr.NameAndEnum(JKEY_EVENTTYPE, type);
    pNewEvent = CreateNewEvent(type);
    if (pNewEvent != NULL)
    {
      pNewEvent->ImportEvent(jr);
      AddEventWithCurrKey(pNewEvent);
    }
    else
    {
      pNewEvent = CreateNewEvent(ChainEventType);
      pNewEvent->ImportEvent(jr);
      AddEventWithCurrKey(pNewEvent);      
    };
  };
  jr.EndList();
  return CONFIG_STAT_ok;
}

#endif

GameEventList& GameEventList::operator =(GameEventList& src)
{
  if (this == &src) return *this;
  WriteDebugString("GameEventList::operator= calling Clear()\n");
  Clear();

  m_level = src.m_level;
  m_ListType = src.m_ListType;

  POSITION pos = src.GetHeadPosition();
  while (pos != NULL)
  {
    ASSERT(src.PeekAtPos(pos) != NULL);
    GameEvent *pNewEvent = CreateNewEvent(src.GetAtPos(pos)->GetEventType());
    if (pNewEvent != NULL)
    {
      GameEvent *pSrcEvent = src.GetAtPos(pos);
      //*pDst = *pSrc;

      switch (pSrcEvent->GetEventType())
      {
       case AddNpc:            *((ADD_NPC_DATA*)pNewEvent)=*((ADD_NPC_DATA*)pSrcEvent);  break;    
       case Camp:              *((CAMP_EVENT_DATA*)pNewEvent)=*((CAMP_EVENT_DATA*)pSrcEvent);  break;
       case Combat:
       case PickOneCombat:     *((COMBAT_EVENT_DATA*)pNewEvent)=*((COMBAT_EVENT_DATA*)pSrcEvent);  break;
       case Damage:            *((GIVE_DAMAGE_DATA*)pNewEvent)=*((GIVE_DAMAGE_DATA*)pSrcEvent);  break;
       case EncounterEvent:    *((ENCOUNTER_DATA*)pNewEvent)=*((ENCOUNTER_DATA*)pSrcEvent);  break;
       case EnterPassword:     *((PASSWORD_DATA*)pNewEvent)=*((PASSWORD_DATA*)pSrcEvent);  break;
       case GainExperience:    *((GAIN_EXP_DATA*)pNewEvent)=*((GAIN_EXP_DATA*)pSrcEvent);  break;
       case CombatTreasure:
       case GiveTreasure:      *((GIVE_TREASURE_DATA*)pNewEvent)=*((GIVE_TREASURE_DATA*)pSrcEvent);  break;
       case GuidedTour:        *((GUIDED_TOUR*)pNewEvent)=*((GUIDED_TOUR*)pSrcEvent);  break;
       case NPCSays:           *((NPC_SAYS_DATA*)pNewEvent)=*((NPC_SAYS_DATA*)pSrcEvent);  break;
       case QuestionList:      *((QUESTION_LIST_DATA*)pNewEvent)=*((QUESTION_LIST_DATA*)pSrcEvent);  break;
       case QuestionButton:    *((QUESTION_BUTTON_DATA*)pNewEvent)=*((QUESTION_BUTTON_DATA*)pSrcEvent);  break;
       case PassTime:          *((PASS_TIME_EVENT_DATA*)pNewEvent)=*((PASS_TIME_EVENT_DATA*)pSrcEvent);  break;
       case QuestionYesNo:     *((QUESTION_YES_NO*)pNewEvent)=*((QUESTION_YES_NO*)pSrcEvent);  break;
       case RemoveNPCEvent:    *((REMOVE_NPC_DATA*)pNewEvent)=*((REMOVE_NPC_DATA*)pSrcEvent);  break;
       case ShopEvent:         *((SHOP*)pNewEvent)=*((SHOP*)pSrcEvent);  break;
       case TempleEvent:       *((TEMPLE*)pNewEvent)=*((TEMPLE*)pSrcEvent);  break;
       case TavernEvent:       *((TAVERN*)pNewEvent)=*((TAVERN*)pSrcEvent);  break;
       case TextStatement:     *((TEXT_EVENT_DATA*)pNewEvent)=*((TEXT_EVENT_DATA*)pSrcEvent);  break;
       case Stairs: 
       case Teleporter: 
       case TransferModule:    *((TRANSFER_EVENT_DATA*)pNewEvent)=*((TRANSFER_EVENT_DATA*)pSrcEvent);  break;
       case WhoPays:           *((WHO_PAYS_EVENT_DATA*)pNewEvent)=*((WHO_PAYS_EVENT_DATA*)pSrcEvent);  break;
       case WhoTries:          *((WHO_TRIES_EVENT_DATA*)pNewEvent)=*((WHO_TRIES_EVENT_DATA*)pSrcEvent);  break;
       case SpecialItem:       *((SPECIAL_ITEM_KEY_EVENT_DATA*)pNewEvent)=*((SPECIAL_ITEM_KEY_EVENT_DATA*)pSrcEvent);  break;
       case Vault:             *((VAULT_EVENT_DATA*)pNewEvent)=*((VAULT_EVENT_DATA*)pSrcEvent);  break;
       case TrainingHallEvent: *((TRAININGHALL*)pNewEvent)=*((TRAININGHALL*)pSrcEvent);  break;
       case SmallTown:         *((SMALL_TOWN_DATA*)pNewEvent)=*((SMALL_TOWN_DATA*)pSrcEvent);  break;
       case RandomEvent:       *((RANDOM_EVENT_DATA*)pNewEvent)=*((RANDOM_EVENT_DATA*)pSrcEvent);  break;
       case ChainEventType:    *((CHAIN_EVENT*)pNewEvent)=*((CHAIN_EVENT*)pSrcEvent);  break;
       case QuestStage:        *((QUEST_EVENT_DATA*)pNewEvent)=*((QUEST_EVENT_DATA*)pSrcEvent);  break;
       case Utilities:         *((UTILITIES_EVENT_DATA*)pNewEvent)=*((UTILITIES_EVENT_DATA*)pSrcEvent);  break;
       case Sounds:            *((SOUND_EVENT*)pNewEvent)=*((SOUND_EVENT*)pSrcEvent);  break;
       case TakePartyItems:    *((TAKE_PARTY_ITEMS_DATA*)pNewEvent)=*((TAKE_PARTY_ITEMS_DATA*)pSrcEvent);  break;
       case HealParty:         *((HEAL_PARTY_DATA*)pNewEvent)=*((HEAL_PARTY_DATA*)pSrcEvent);  break;
       case LogicBlock:        *((LOGIC_BLOCK_DATA*)pNewEvent)=*((LOGIC_BLOCK_DATA*)pSrcEvent);  break;
       case PlayMovieEvent:    *((PLAY_MOVIE_DATA*)pNewEvent)=*((PLAY_MOVIE_DATA*)pSrcEvent);  break;    
       case JournalEvent:      *((JOURNAL_EVENT*)pNewEvent)=*((JOURNAL_EVENT*)pSrcEvent);  break;
       default:
          die(0xab519);
          WriteDebugString("Unhandled event type %i in GameEventList::operator=()\n", pSrcEvent->GetEventType());
          break;
      }  

      AddEventWithCurrKey(pNewEvent);
    }
    src.GetNext(pos);
  }
  return *this;
}

#ifdef UAFEDITOR


bool GameEventList::operator ==(GameEventList& src)
{
  if (m_level != src.m_level) return false;
  if (m_ListType != src.m_ListType)return false;

  POSITION srcpos = src.GetHeadPosition();
  POSITION mypos = GetHeadPosition();
  while (srcpos != NULL)
  {
    //GameEvent *pNewEvent = CreateNewEvent(src.GetAtPos(pos)->GetEventType());
    //if (pNewEvent != NULL)
    if (mypos == NULL) return false;
    {
      GameEvent *pSrcEvent = src.GetAtPos(srcpos);
      GameEvent *pMyEvent = GetAtPos(mypos);
      //*pDst = *pSrc;
      if (!(*pMyEvent == *pSrcEvent)) 
      {
        return false;
      };
    };
    src.GetNext(srcpos);
    GetNext(mypos);
  }
  return true;
}
#endif


void GameEventList::saveUsedEventArt(int level)
{
  if (GetCount() <= 0)
    return;

  POSITION pos = GetHeadPosition();
  while (pos != NULL)
    GetNext(pos)->SaveEventResources(level);
}

void GameEventList::saveUsedEventSounds(int level)
{
  if (GetCount() <= 0)
    return;

  POSITION pos = GetHeadPosition();
  while (pos != NULL)
    GetNext(pos)->SaveEventSounds(level);
}

GameEvent *GameEventList::CreateNewEvent(int type)
{
  if (type == NoEvent) 
  {
    return NULL;
  };
  GameEvent *event = NULL;

  switch (type)
  {
   case AddNpc:            event = new ADD_NPC_DATA;                break;    
   case Camp:              event = new CAMP_EVENT_DATA;             break;  
   case Combat:            event = new COMBAT_EVENT_DATA;           break;    
   case PickOneCombat:     event = new COMBAT_EVENT_DATA; event->event=PickOneCombat; break;   /* what a hack! */
   case Damage:            event = new GIVE_DAMAGE_DATA;            break;    
   case EncounterEvent:    event = new ENCOUNTER_DATA;              break;    
   case EnterPassword:     event = new PASSWORD_DATA;               break;    
   case GainExperience:    event = new GAIN_EXP_DATA;               break;    
   case CombatTreasure:    event = new COMBAT_TREASURE;             break;
   case GiveTreasure:      event = new GIVE_TREASURE_DATA;          break;    
   case GuidedTour:        event = new GUIDED_TOUR;                 break;    
   case NPCSays:           event = new NPC_SAYS_DATA;               break;   
   case QuestionList:      event = new QUESTION_LIST_DATA;          break;
   case QuestionButton:    event = new QUESTION_BUTTON_DATA;        break;
   case PassTime:          event = new PASS_TIME_EVENT_DATA;        break;
   case QuestionYesNo:     event = new QUESTION_YES_NO;             break;    
   case RemoveNPCEvent:    event = new REMOVE_NPC_DATA;             break;   
   case ShopEvent:         event = new SHOP;                        break;
   case TempleEvent:       event = new TEMPLE;                      break;
 
//#ifdef UAFEDITOR
   case TavernTales:       event = new TAVERN_TALES;event->event=TavernTales;  break;
//#else
//   case TavernTales: // only used for UA importing, tales are part of TAVERN event
//#endif
   case TavernEvent:       event = new TAVERN;                      break;

   case TextStatement:     event = new TEXT_EVENT_DATA;             break;
   case Stairs: 
   case Teleporter: 
   case TransferModule:    event = new TRANSFER_EVENT_DATA;         break;
   case WhoPays:           event = new WHO_PAYS_EVENT_DATA;         break;
   case WhoTries:          event = new WHO_TRIES_EVENT_DATA;        break;          
   case SpecialItem:       event = new SPECIAL_ITEM_KEY_EVENT_DATA; break;
   case Vault:             event = new VAULT_EVENT_DATA;            break;  
   case TrainingHallEvent: event = new TRAININGHALL;                break;
   case SmallTown:         event = new SMALL_TOWN_DATA;             break;
   case RandomEvent:       event = new RANDOM_EVENT_DATA;           break;
   case ChainEventType:    event = new CHAIN_EVENT;                 break;
   case QuestStage:        event = new QUEST_EVENT_DATA;            break;    
   case Utilities:         event = new UTILITIES_EVENT_DATA;        break;
   case Sounds:            event = new SOUND_EVENT;                 break;
   case TakePartyItems:    event = new TAKE_PARTY_ITEMS_DATA;       break;
   case HealParty:         event = new HEAL_PARTY_DATA;             break;
   case LogicBlock:        event = new LOGIC_BLOCK_DATA;            break;
   case PlayMovieEvent:    event = new PLAY_MOVIE_DATA;             break;
   case JournalEvent:      event = new JOURNAL_EVENT;               break;
   
   case InnEvent: // never
   default:
      die(0xab51a);
      WriteDebugString("Unhandled event type %i in CreateNewEvent()\n", type);
      break;       
  }
  
  ASSERT( event != NULL );
  if (event != NULL)
    ASSERT( event->GetEventType() != NoEvent );
  return event;
}

#ifdef UAFEDITOR

DWORD GameEventList::GetLastEventInNormalChain(DWORD SrcId)
{  
  BOOL search = TRUE;
  DWORD lastId = SrcId;
  DWORD currId = SrcId;

  while (search)
  {
    GameEvent *pEvent = GetEvent(currId);
    if (pEvent!=NULL)
    {
      if (pEvent->GetChainedEvent(currId))
        lastId = currId;
      else
        search=FALSE;
    }
    else
      search=FALSE;
  }
  return lastId;
}

GameEvent* GameEventList::CreateEventCopy(GameEvent *pSrcEvent)
{
  if (pSrcEvent==NULL) return NULL;
  ASSERT (IsValidEvent(pSrcEvent));
  GameEvent *pNewEvent = CreateNewEvent(pSrcEvent->GetEventType());
  if (pNewEvent == NULL) return NULL;

  switch (pSrcEvent->GetEventType())
  {
   case AddNpc:            *((ADD_NPC_DATA*)pNewEvent)=*((ADD_NPC_DATA*)pSrcEvent);  break;    
   case Camp:              *((CAMP_EVENT_DATA*)pNewEvent)=*((CAMP_EVENT_DATA*)pSrcEvent);  break;
   case Combat:
   case PickOneCombat:     *((COMBAT_EVENT_DATA*)pNewEvent)=*((COMBAT_EVENT_DATA*)pSrcEvent);  break;
   case Damage:            *((GIVE_DAMAGE_DATA*)pNewEvent)=*((GIVE_DAMAGE_DATA*)pSrcEvent);  break;
   case EncounterEvent:    *((ENCOUNTER_DATA*)pNewEvent)=*((ENCOUNTER_DATA*)pSrcEvent);  break;
   case EnterPassword:     *((PASSWORD_DATA*)pNewEvent)=*((PASSWORD_DATA*)pSrcEvent);  break;
   case GainExperience:    *((GAIN_EXP_DATA*)pNewEvent)=*((GAIN_EXP_DATA*)pSrcEvent);  break;
   case CombatTreasure:    *((COMBAT_TREASURE*)pNewEvent)=*((COMBAT_TREASURE*)pSrcEvent);  break;
   case GiveTreasure:      *((GIVE_TREASURE_DATA*)pNewEvent)=*((GIVE_TREASURE_DATA*)pSrcEvent);  break;
   case GuidedTour:        *((GUIDED_TOUR*)pNewEvent)=*((GUIDED_TOUR*)pSrcEvent);  break;
   case NPCSays:           *((NPC_SAYS_DATA*)pNewEvent)=*((NPC_SAYS_DATA*)pSrcEvent);  break;
   case QuestionList:      *((QUESTION_LIST_DATA*)pNewEvent)=*((QUESTION_LIST_DATA*)pSrcEvent);  break;
   case QuestionButton:    *((QUESTION_BUTTON_DATA*)pNewEvent)=*((QUESTION_BUTTON_DATA*)pSrcEvent);  break;
   case PassTime:          *((PASS_TIME_EVENT_DATA*)pNewEvent)=*((PASS_TIME_EVENT_DATA*)pSrcEvent);  break;
   case QuestionYesNo:     *((QUESTION_YES_NO*)pNewEvent)=*((QUESTION_YES_NO*)pSrcEvent);  break;
   case RemoveNPCEvent:    *((REMOVE_NPC_DATA*)pNewEvent)=*((REMOVE_NPC_DATA*)pSrcEvent);  break;
   case ShopEvent:         *((SHOP*)pNewEvent)=*((SHOP*)pSrcEvent);  break;
   case TempleEvent:       *((TEMPLE*)pNewEvent)=*((TEMPLE*)pSrcEvent);  break;
   case TavernEvent:       *((TAVERN*)pNewEvent)=*((TAVERN*)pSrcEvent);  break;
   case TavernTales:       *((TAVERN_TALES*)pNewEvent)=*((TAVERN_TALES*)pSrcEvent);  break;
   case TextStatement:     *((TEXT_EVENT_DATA*)pNewEvent)=*((TEXT_EVENT_DATA*)pSrcEvent);  break;
   case Stairs: 
   case Teleporter: 
   case TransferModule:    *((TRANSFER_EVENT_DATA*)pNewEvent)=*((TRANSFER_EVENT_DATA*)pSrcEvent);  break;
   case WhoPays:           *((WHO_PAYS_EVENT_DATA*)pNewEvent)=*((WHO_PAYS_EVENT_DATA*)pSrcEvent);  break;
   case WhoTries:          *((WHO_TRIES_EVENT_DATA*)pNewEvent)=*((WHO_TRIES_EVENT_DATA*)pSrcEvent);  break;
   case SpecialItem:       *((SPECIAL_ITEM_KEY_EVENT_DATA*)pNewEvent)=*((SPECIAL_ITEM_KEY_EVENT_DATA*)pSrcEvent);  break;
   case Vault:             *((VAULT_EVENT_DATA*)pNewEvent)=*((VAULT_EVENT_DATA*)pSrcEvent);  break;
   case TrainingHallEvent: *((TRAININGHALL*)pNewEvent)=*((TRAININGHALL*)pSrcEvent);  break;
   case SmallTown:         *((SMALL_TOWN_DATA*)pNewEvent)=*((SMALL_TOWN_DATA*)pSrcEvent);  break;
   case RandomEvent:       *((RANDOM_EVENT_DATA*)pNewEvent)=*((RANDOM_EVENT_DATA*)pSrcEvent);  break;
   case ChainEventType:    *((CHAIN_EVENT*)pNewEvent)=*((CHAIN_EVENT*)pSrcEvent);  break;
   case QuestStage:        *((QUEST_EVENT_DATA*)pNewEvent)=*((QUEST_EVENT_DATA*)pSrcEvent);  break;
   case Utilities:         *((UTILITIES_EVENT_DATA*)pNewEvent)=*((UTILITIES_EVENT_DATA*)pSrcEvent);  break;
   case Sounds:            *((SOUND_EVENT*)pNewEvent)=*((SOUND_EVENT*)pSrcEvent);  break;
   case TakePartyItems:    *((TAKE_PARTY_ITEMS_DATA*)pNewEvent)=*((TAKE_PARTY_ITEMS_DATA*)pSrcEvent);  break;
   case HealParty:         *((HEAL_PARTY_DATA*)pNewEvent)=*((HEAL_PARTY_DATA*)pSrcEvent);  break;
   case LogicBlock:        *((LOGIC_BLOCK_DATA*)pNewEvent)=*((LOGIC_BLOCK_DATA*)pSrcEvent);  break;
   case PlayMovieEvent:    *((PLAY_MOVIE_DATA*)pNewEvent)=*((PLAY_MOVIE_DATA*)pSrcEvent);  break;    
   case JournalEvent:      *((JOURNAL_EVENT*)pNewEvent)=*((JOURNAL_EVENT*)pSrcEvent);  break;
   default:
      die(0xab51b);
      WriteDebugString("Unhandled event type %i in CreateNewEvent()\n", pSrcEvent->GetEventType());
      break;
  }  
  return pNewEvent;
}

void GameEventList::CopyEvent(DWORD &RootId, DWORD SrcId, GameEventList *pSrcList)
{
  if (pSrcList==NULL) return;
  if (!pSrcList->IsValidEvent(SrcId)) return;
  //if (!IsValidEvent(RootId)) return;

  GameEvent *pSrcEvent = pSrcList->GetEvent(SrcId);
  if (pSrcEvent==NULL) return;
  GameEvent *pNewEvent = pSrcList->CreateEventCopy(pSrcEvent);
  if (pNewEvent==NULL) return;

  AddEvent(pNewEvent);
  RootId = pNewEvent->GetEventId();
#if ((defined UAFEDITOR) && (defined _DEBUG))
  WriteDebugString("Copied '%s' %s event to %s list - %u events total\n",
                   GetEventIdDescription(SrcId,pSrcList->GetEventSource()),
                   EventSourceText[(int)pSrcList->GetEventSource()],
                   EventSourceText[(int)GetEventSource()],
                   GetCount());
#endif
  if (pSrcEvent->IsChained())
  {
    switch (pSrcEvent->control.chainTrigger)
    {
    case AlwaysChain:
    case IfEventHappen:
      CopyEvent(pNewEvent->chainEventHappen, pSrcEvent->chainEventHappen, pSrcList);
      break;
    case IfEventNotHappen:
      CopyEvent(pNewEvent->chainEventNotHappen, pSrcEvent->chainEventNotHappen, pSrcList);
      break;
    }

    int i;
    switch (pSrcEvent->GetEventType())
    {
    case TavernEvent:
      {
        TAVERN *sdata = (TAVERN*)(pSrcEvent);     
        TAVERN *ndata = (TAVERN*)(pNewEvent);     
        CopyEvent(ndata->drinkChain, sdata->drinkChain, pSrcList);
        CopyEvent(ndata->fightChain, sdata->fightChain, pSrcList);
      }
      break;

    case TempleEvent:
      {
        TEMPLE *sdata = (TEMPLE*)(pSrcEvent);
        TEMPLE *ndata = (TEMPLE*)(pNewEvent);
        CopyEvent(ndata->donationChain, sdata->donationChain, pSrcList);
      }
      break;

    case QuestStage:
      {
        QUEST_EVENT_DATA *sdata = (QUEST_EVENT_DATA*)(pSrcEvent);        
        QUEST_EVENT_DATA *ndata = (QUEST_EVENT_DATA*)(pNewEvent);
        CopyEvent(ndata->acceptChain, sdata->acceptChain, pSrcList);
        CopyEvent(ndata->rejectChain, sdata->rejectChain, pSrcList);
      }
      break;

    case ChainEventType:
      {
        CHAIN_EVENT *sdata = (CHAIN_EVENT*)(pSrcEvent);        
        CHAIN_EVENT *ndata = (CHAIN_EVENT*)(pNewEvent);
        CopyEvent(ndata->Chain, sdata->Chain, pSrcList);
      }
      break;

    case RandomEvent:
      {
        RANDOM_EVENT_DATA *sdata = (RANDOM_EVENT_DATA*)(pSrcEvent);
        RANDOM_EVENT_DATA *ndata = (RANDOM_EVENT_DATA*)(pNewEvent);
        for (i=1;i<RANDOM_EVENT_DATA::MAX_RANDOM_EVENTS;i++)
          CopyEvent(ndata->EventChains[i-1], sdata->EventChains[i-1], pSrcList);
      }
      break;

    case SmallTown:
      {
        SMALL_TOWN_DATA *sdata = (SMALL_TOWN_DATA*)(pSrcEvent);        
        SMALL_TOWN_DATA *ndata = (SMALL_TOWN_DATA*)(pNewEvent);

        CopyEvent(ndata->TempleChain, sdata->TempleChain, pSrcList);
        CopyEvent(ndata->TrainingHallChain, sdata->TrainingHallChain, pSrcList);
        CopyEvent(ndata->ShopChain, sdata->ShopChain, pSrcList);
        CopyEvent(ndata->InnChain, sdata->InnChain, pSrcList);
        CopyEvent(ndata->TavernChain, sdata->TavernChain, pSrcList);
        CopyEvent(ndata->VaultChain, sdata->VaultChain, pSrcList);
      }
      break;

   case EncounterEvent:
     {
       ENCOUNTER_DATA *sdata = (ENCOUNTER_DATA*)(pSrcEvent);    
       ENCOUNTER_DATA *ndata = (ENCOUNTER_DATA*)(pNewEvent);

		   for (i=0;i<MAX_BUTTONS;i++)
         CopyEvent(ndata->buttons.buttons[i].chain, sdata->buttons.buttons[i].chain, pSrcList);
       CopyEvent(ndata->combatChain, sdata->combatChain, pSrcList);
       CopyEvent(ndata->talkChain, sdata->talkChain, pSrcList);
       CopyEvent(ndata->escapeChain, sdata->escapeChain, pSrcList);
     }
     break;

    case QuestionList:
      {
         QUESTION_LIST_DATA *sdata = (QUESTION_LIST_DATA*)(pSrcEvent);        
         QUESTION_LIST_DATA *ndata = (QUESTION_LIST_DATA*)(pNewEvent);

         for (i=0;i<MAX_BUTTONS;i++)
           CopyEvent(ndata->buttons.buttons[i].chain, sdata->buttons.buttons[i].chain, pSrcList);
      }
      break;

    case QuestionButton:
      {
         QUESTION_BUTTON_DATA *sdata = (QUESTION_BUTTON_DATA*)(pSrcEvent);        
         QUESTION_BUTTON_DATA *ndata = (QUESTION_BUTTON_DATA*)(pNewEvent);

         for (i=0;i<MAX_BUTTONS;i++)
           CopyEvent(ndata->buttons.buttons[i].chain, sdata->buttons.buttons[i].chain, pSrcList);
      }
      break;

    case QuestionYesNo:
      {
        QUESTION_YES_NO *sdata = (QUESTION_YES_NO*)(pSrcEvent);        
        QUESTION_YES_NO *ndata = (QUESTION_YES_NO*)(pNewEvent);

        CopyEvent(ndata->YesChain, sdata->YesChain, pSrcList);
        CopyEvent(ndata->NoChain, sdata->NoChain, pSrcList);
      }
      break;

    case EnterPassword:
      {
         PASSWORD_DATA *sdata = (PASSWORD_DATA*)(pSrcEvent);    
         PASSWORD_DATA *ndata = (PASSWORD_DATA*)(pNewEvent);

         CopyEvent(ndata->successChain, sdata->successChain, pSrcList);
         CopyEvent(ndata->failChain, sdata->failChain, pSrcList);
      }
      break;

    case WhoPays:
      {
         WHO_PAYS_EVENT_DATA *sdata = (WHO_PAYS_EVENT_DATA*)(pSrcEvent);        
         WHO_PAYS_EVENT_DATA *ndata = (WHO_PAYS_EVENT_DATA*)(pNewEvent);

         CopyEvent(ndata->successChain, sdata->successChain, pSrcList);
         CopyEvent(ndata->failChain, sdata->failChain, pSrcList);
      }
      break;

    case WhoTries:
      {
         WHO_TRIES_EVENT_DATA *sdata = (WHO_TRIES_EVENT_DATA*)(pSrcEvent);        
         WHO_TRIES_EVENT_DATA *ndata = (WHO_TRIES_EVENT_DATA*)(pNewEvent);

         CopyEvent(ndata->successChain, sdata->successChain, pSrcList);
         CopyEvent(ndata->failChain, sdata->failChain, pSrcList);
      }
      break;

    case LogicBlock:
      {
         LOGIC_BLOCK_DATA *sdata = (LOGIC_BLOCK_DATA*)(pSrcEvent);        
         LOGIC_BLOCK_DATA *ndata = (LOGIC_BLOCK_DATA*)(pNewEvent);

         CopyEvent(ndata->falseChain, sdata->falseChain, pSrcList);
         CopyEvent(ndata->trueChain, sdata->trueChain, pSrcList);
      }
      break;

    default:
      // do nothing
      break;
    }
  } // if has chained events      
}

BOOL GameEventList::IsEventTypeUsed(eventType type)
{
  if (type >= CONTROL_Splash) return FALSE;
  BOOL used = TRUE;
  switch (type)
  {
    // these events cannot be created or used
  //case TavernTales:
  case InnEvent:
  case PickOneCombat:
  case GPDLEvent:
  case NoEvent:
    used = FALSE;
    break;

  default:
    used = TRUE;
    break;
  }
  return used;
}

BOOL GameEventList::Validate()
{
  BOOL success = TRUE;
  FILE *pFile = NULL;  
  
  CString filename;
  filename = rte.DataDir() + "Verify.txt";

  SetFileAttributes(filename, FILE_ATTRIBUTE_NORMAL);
  pFile = fopen(filename, "wt");
  if (pFile == NULL)
  {
    WriteDebugString("Unable to open validation logfile %s\n", filename);
    return FALSE;
  }

  if (!monsterData.Validate(pFile)) success = FALSE;
  if (!spellData.Validate(pFile)) success = FALSE;
  if (!itemData.Validate(pFile)) success = FALSE;
  if (!globalData.charData.Validate(pFile)) success = FALSE;

  POSITION gpos = m_events.GetHeadPosition();
  
  while (gpos != NULL)
  {
    if (!m_events.PeekAtPos(gpos)->Validate(pFile))
      success = FALSE;
    m_events.GetNext(gpos);
  }

  if (pFile != NULL)
    fclose(pFile);
#ifndef _DEBUG
  if (success) 
    MyDeleteFile(filename);
#endif
  return success;
}


void GameEventList::CrossReference(CR_EVENT_INFO *pCREI)
{
  POSITION gpos = m_events.GetHeadPosition();
  
  while (gpos != NULL)
  {
    m_events.PeekAtPos(gpos)->CrossReference(pCREI);
    m_events.GetNext(gpos);
  };
}


void GameEventList::LogEventText()
{
  FILE *pFile = NULL;  
  char filename[_MAX_PATH+1];  
  GetDesignPath(filename);
  CString file;

  bool IsGlobalSrc=false;
  if (GetEventSource()==LevelEventSrc)
  {
    file.Format("Level%03i_TextLog.txt", globalData.currLevel);
    strcat(filename, file);
  }
  else if (GetEventSource()==GlobalEventSrc)
  {
    file = "Global_TextLog.txt";
    strcat(filename, file);
    IsGlobalSrc=true;
  }
  else
    return;

  SetFileAttributes(filename, FILE_ATTRIBUTE_NORMAL);
  pFile = fopen(filename, "wt");
  if (pFile == NULL)
  {    
    CString temp;
    temp.Format("Unable to open event text log: %s\n", filename);
    MsgBoxError(temp);
    return;
  }

  if (GetEventSource()==LevelEventSrc)
  {
    fprintf(pFile, "Event text for level %i\n\n", globalData.currLevel);
    fprintf(pFile, "The 'at x,y' indicates event location on map\n");
    fprintf(pFile, "and 'at -1,-1' indicates a chained event\n\n");
  }
  else
  {
    fprintf(pFile, "Global event text (items, NPCs)\n\n");
    fprintf(pFile, "The 'at -1,-1' can be ignored for global events\n\n");
  }
  fprintf(pFile, "The number in parens is unique for each event\n");
  fprintf(pFile, "and matches the same number shown in the event dialog.\n\n");
  fprintf(pFile, "Text 1 is the main text that all events can display.\n");
  fprintf(pFile, "Text 2 and 3 are optional for events that have\n");
  fprintf(pFile, "multiple text boxes.\n");
  fprintf(pFile, " Ex: Password Event has: Text2=Success Text, Text3=Fail Text\n\n");

  POSITION gpos = m_events.GetHeadPosition();  
  bool hasText;
  while (gpos != NULL)
  {
    hasText=false;
    GameEvent *pEvent = m_events.PeekAtPos(gpos);
    if (pEvent==NULL) continue;    
    fprintf(pFile, "\nEvent '%s' at %i,%i:\n",
            (LPCSTR)GetEventIdDescription(pEvent->id,GetEventSource()),
            pEvent->x,pEvent->y);

    char label[512+1];
    for (int c=0;c<15;c++)
    {
      if (pEvent->GetEVChainText(c, label))
        fprintf(pFile, "\t%s\n", label);
    }

    if (pEvent->IsAChainEvent())
    {
      POSITION srchpos = m_events.GetHeadPosition();
      bool found = false;
      DWORD pid=0;
      while ((found==false)&&(srchpos!=NULL))
      {
        // scan chains for match
        for (int c=0;c<15;c++)
        {
          if (m_events.PeekAtPos(srchpos)->GetEVChain(c, pid))
          {
            if (pid == pEvent->GetEventId())
            {
              found=true;
              pid = m_events.PeekAtPos(srchpos)->GetEventId();
              break;
            }
          }
        }

        m_events.GetNext(srchpos);
      }

      if (found==false)
      {
        if (IsGlobalSrc)
        {
          fprintf(pFile, "\tParent Event Id: This chain event has no parent\n");
        }
        else
        {
          // no event claims to chain to this one
          fprintf(pFile, "\tParent Event Id: *** ERROR *** This chain event has no parent\n");
        }
      }
      else
      {
        fprintf(pFile, "\tParent Event Id: %u\n", pid);

      }
    }

    if (pEvent->GetEventText() != "")
    {
      fprintf(pFile, "Text 1: '%s'\n", (LPCSTR)pEvent->GetEventText());
      hasText=true;
    }
    if (pEvent->GetEventText2() != "")
    {
      fprintf(pFile, "Text 2: '%s'\n", (LPCSTR)pEvent->GetEventText2());
      hasText=true;
    }
    if (pEvent->GetEventText3() != "")
    {
      fprintf(pFile, "Text 3: '%s'\n", (LPCSTR)pEvent->GetEventText3());
      hasText=true;
    }
    if (!hasText)
      fprintf(pFile, "No Event Text\n");

    if (pEvent->GetEventType()==TavernEvent)
    {
      TAVERN *pTavern = (TAVERN*)pEvent;
      int i;
      for (i=0;i<MAX_TALES;i++)
        fprintf(pFile, "Tavern Tale %i: %s\n", i+1, (LPCSTR)pTavern->tales[i].tale);
      for (i=0;i<MAX_DRINKS;i++)
        fprintf(pFile, "Drink Name %i: %s\n", i+1, (LPCSTR)pTavern->drinks[i].name);
    }
    else if (   (pEvent->GetEventType()==GiveTreasure)
             || (pEvent->GetEventType()==CombatTreasure))
    {
      GIVE_TREASURE_DATA *pTreasure = (GIVE_TREASURE_DATA*)pEvent;
      POSITION ipos = pTreasure->items.GetHeadPosition();
      while (ipos!=NULL)
      {
        const ITEM& it = pTreasure->items.PeekAtPos(ipos);
        //CString name = itemData.GetItemIdName(it.m_giID);
        CString name = itemData.GetItemIdName(it.itemID);
        fprintf(pFile, "GiveTreasure Item: %s\n", (LPCSTR)name);
        pTreasure->items.GetNext(ipos);
      }
    }
    else if (pEvent->GetEventType()==ShopEvent)
    {
      SHOP *pData = (SHOP*)pEvent;
      POSITION ipos = pData->itemsAvail.GetHeadPosition();
      while (ipos!=NULL)
      {
        const ITEM& it = pData->itemsAvail.PeekAtPos(ipos);
        //CString name = itemData.GetItemIdName(it.m_giID);
        CString name = itemData.GetItemIdName(it.itemID);
        fprintf(pFile, "SHOP Item: %s\n", (LPCSTR)name);
        pData->itemsAvail.GetNext(ipos);
      }
    }

    m_events.GetNext(gpos);
  }

#ifdef _DEBUG
  fprintf(pFile, "\n\n");
  if (IsGlobalSrc)
  {
    CMap<DWORD, DWORD, DWORD, DWORD> eids;
    DWORD tmpid;

    //POSITION ipos = itemData.GetHeadPosition();
    //while (ipos!=NULL)
    int i, n;
    n = itemData.GetCount();
    for (i=0; i<n; i++)
    {
      //const ITEM_DATA &item = itemData.PeekAtPos(ipos);
      //const ITEM_DATA *pItem = itemData.PeekItemData(ipos);
      const ITEM_DATA *pItem = itemData.PeekItem(i);
      
      //if (item.ExamineEvent > 0)
      if (pItem->ExamineEvent > 0)
      {
        //if (eids.Lookup(item.ExamineEvent, tmpid))
        if (eids.Lookup(pItem->ExamineEvent, tmpid))
        {
          fprintf(pFile, "Item %s Examine Event '%s' *** uses duplicate id ***\n",
                  //item.UniqueName(),
                  (LPCSTR)pItem->UniqueName(),
                  //GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
                  (LPCSTR)GetEventIdDescription(pItem->ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Item %s Examine Event '%s'\n",
                  //item.UniqueName(),
                  (LPCSTR)pItem->UniqueName(),
                  //GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
                  (LPCSTR)GetEventIdDescription(pItem->ExamineEvent, GlobalEventSrc));
          //eids[item.ExamineEvent] = item.ExamineEvent;
          eids[pItem->ExamineEvent] = pItem->ExamineEvent;
        }
      }

      // 'Use' event
      //if (item.m_useEvent > 0)
      if (pItem->m_useEvent > 0)
      {
        //if (eids.Lookup(item.m_useEvent, tmpid))
        if (eids.Lookup(pItem->m_useEvent, tmpid))
        {
          fprintf(pFile, "Item %s Use Event '%s' *** uses duplicate id ***\n",
                  //item.UniqueName(),
                  (LPCSTR)pItem->UniqueName(),
                  //GetEventIdDescription(item.m_useEvent, GlobalEventSrc));
                  (LPCSTR)GetEventIdDescription(pItem->m_useEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Item %s Use Event '%s'\n",
                  //item.UniqueName(),
                  (LPCSTR)pItem->UniqueName(),
                  //GetEventIdDescription(item.m_useEvent, GlobalEventSrc));
                  (LPCSTR)GetEventIdDescription(pItem->m_useEvent, GlobalEventSrc));
          //eids[item.m_useEvent] = item.m_useEvent;
          eids[pItem->m_useEvent] = pItem->m_useEvent;
        }
      }

      //itemData.GetNext(ipos);
    }

    //ipos = globalData.charData.GetHeadPosition();
    //while (ipos!=NULL)
    n = globalData.charData.GetCount();
    for (i=0; i<n; i++)
    {
      //CHARACTER &item = globalData.charData.GetAt(ipos);
      CHARACTER &item = *globalData.charData.GetCharacter(i);
      
      if (item.ExamineEvent > 0)
      {
        if (eids.Lookup(item.ExamineEvent, tmpid))
        {
          fprintf(pFile, "Character %s Examine Event '%s' *** uses duplicate id ***\n",
                  (LPCSTR)item.GetName(),
                  (LPCSTR)GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Character %s Examine Event '%s'\n",
            (LPCSTR)item.GetName(),
            (LPCSTR)GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
          eids[item.ExamineEvent] = item.ExamineEvent;
        }
      }

      if (item.TalkEvent > 0)
      {
        if (eids.Lookup(item.TalkEvent, tmpid))
        {
          fprintf(pFile, "Character %s Talk Event '%s' *** uses duplicate id ***\n",
            (LPCSTR)item.GetName(),
            (LPCSTR)GetEventIdDescription(item.TalkEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Character %s Talk Event '%s'\n",
            (LPCSTR)item.GetName(),
            (LPCSTR)GetEventIdDescription(item.TalkEvent, GlobalEventSrc));
          eids[item.TalkEvent] = item.TalkEvent;
        }
      }
      //globalData.charData.GetNext(ipos);
    }

    POSITION ipos;
    ipos = globalData.specialItemData.GetHeadPosition();
    while (ipos!=NULL)
    {
      const SPECIAL_OBJECT_DATA &item = globalData.specialItemData.PeekAtPos(ipos);
      
      if (item.ExamineEvent > 0)
      {
        if (eids.Lookup(item.ExamineEvent, tmpid))
        {
          fprintf(pFile, "Special Item %s Examine Event '%s' *** uses duplicate id ***\n",
            (LPCSTR)item.name,
            (LPCSTR)GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Special Item %s Examine Event '%s'\n",
            (LPCSTR)item.name,
            (LPCSTR)GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
          eids[item.ExamineEvent] = item.ExamineEvent;
        }
      }

      globalData.specialItemData.GetNext(ipos);
    }

    ipos = globalData.keyData.GetHeadPosition();
    while (ipos!=NULL)
    {
      const SPECIAL_OBJECT_DATA &item = globalData.keyData.PeekAtPos(ipos);
      
      if (item.ExamineEvent > 0)
      {
        if (eids.Lookup(item.ExamineEvent, tmpid))
        {
          fprintf(pFile, "Special Key %s Examine Event '%s' *** uses duplicate id ***\n",
            (LPCSTR)item.name,
            (LPCSTR)GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Special Key %s Examine Event '%s'\n",
            (LPCSTR)item.name,
            (LPCSTR)GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
          eids[item.ExamineEvent] = item.ExamineEvent;
        }
      }

      globalData.keyData.GetNext(ipos);
    }
  }
#endif // _DEBUG

  if (pFile != NULL)
    fclose(pFile);
}
#endif


GPDL_EVENT::~GPDL_EVENT(void)
{
  delete m_talkGPDL;
  m_talkGPDL = NULL;
}



GPDL_EVENT::GPDL_EVENT(TEXT_EVENT_DATA *txtEvnt) : GameEvent()
{
  GameEvent::operator = (*txtEvnt);
  WaitForReturn=txtEvnt->WaitForReturn;
  HighlightText=txtEvnt->HighlightText;
  ForceBackup=txtEvnt->ForceBackup;
  KillSound=txtEvnt->KillSound;
  sound=txtEvnt->sound;
  hSound=-1;
}

#ifdef UAFEDITOR
void GPDL_EVENT::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("GPDL_EVENT::DetailedCrossReference");
}
//void GPDL_EVENT::Export(JWriter& jw)
//{
//  NotImplemented(0xcd952, false);
//}
//bool GPDL_EVENT::Import(JReader& jr)
//{
//  NotImplemented(0xcd953, false);
//  return true;
//}
#endif



void TRANSFER_DATA::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << execEvent;
      ar << destEP;
      ar << destLevel;
      ar << destX;
      ar << destY;
      ar << (int)m_facing;
   }
   else
   {
      int temp;
      ar >> execEvent;
      ar >> destEP;
      ar >> destLevel;
      ar >> destX;
      ar >> destY;
      ar >> temp;
      m_facing = (eventFacingType)temp;
   }
}

void TRANSFER_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   if (ar.IsStoring())
   {
      ar << execEvent;
      ar << destEP;
      ar << destLevel;
      ar << destX;
      ar << destY;
      ar << (int)m_facing;
   }
   else
   {
      int temp;
      ar >> execEvent;
      ar >> destEP;
      ar >> destLevel;
      ar >> destX;
      ar >> destY;
      ar >> temp;
      m_facing = (eventFacingType)temp;
   }
}

#ifdef UAFEDITOR

const char *JKEY_EXECUTEEVENTATDEST = "executeEventAtDestination";
const char *JKEY_ENTRYPOINT = "entryPoint";
//const char *JKEY_LEVEL = "level";
//const char *JKEY_XY = "xy";
//const char *JKEY_FACING = "facing";

void TRANSFER_DATA::Export(JWriter& jw, const char *name)
{
  int i[2];
  jw.StartList(name);
  jw.NameAndBool(JKEY_EXECUTEEVENTATDEST, execEvent);
  jw.NameAndNonNegative(JKEY_ENTRYPOINT, destEP);
  jw.NameAndValue(JKEY_LEVEL, destLevel+1); // External representation starts at one.
  i[0] = destX;
  i[1] = destY;
  jw.NameAndValue(JKEY_XY, i, 2);
  jw.NameAndEnum(JKEY_FACING, m_facing);
  jw.EndList();
}
bool TRANSFER_DATA::Import(JReader& jr, const char *name)
{
  int i[2];
  if (jr.Optional(),!jr.StartList(name)) return false;
  jr.NameAndBool(JKEY_EXECUTEEVENTATDEST, execEvent);
  jr.NameAndNonNegative(JKEY_ENTRYPOINT, destEP);
  jr.NameAndValue(JKEY_LEVEL, destLevel);
  destLevel--; // The internal representation starts at zero;
  jr.NameAndValue(JKEY_XY, i, 2);
  destX = i[0];
  destY = i[1];
  jr.NameAndEnum(JKEY_FACING, m_facing);
  jr.EndList();
  return true;
}
#endif

void MONSTER_EVENT::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << qty;
      ar << m_type;
      //ar << monster;
      ar << monsterID;
      ar << friendly;
      ar << moraleAdj;

      ar << qtyDiceSides;
      ar << qtyDiceQty;
      ar << qtyBonus;
      ar << UseQty;

      money.Serialize(ar, LoadingVersion);
   }
   else
   {
      ar >> qty;
      ar >> m_type;
      //ar >> monster;
      ar >> monsterID;
      ar >> friendly;
      if (LoadingVersion >= _VERSION_0690_)
        ar >> moraleAdj;  

      if (LoadingVersion >= _VERSION_0910_)
      {
        ar >> qtyDiceSides;
        ar >> qtyDiceQty;
        ar >> qtyBonus;
        ar >> UseQty;
      }

      if (LoadingVersion >= _VERSION_0740_)
        money.Serialize(ar, LoadingVersion);
   }

   items.Serialize(ar,LoadingVersion);
}

void MONSTER_EVENT::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   if (ar.IsStoring())
   {
      ar << qty;
      ar << m_type;
      //ar << monster;
      ar << monsterID;
      ar << characterID;
      ar << friendly;
      ar << moraleAdj;

      ar << qtyDiceSides;
      ar << qtyDiceQty;
      ar << qtyBonus;
      ar << UseQty;

      money.Serialize(ar, LoadingVersion);
   }
   else
   {
      ar >> qty;
      ar >> m_type;
#ifdef UAFEDITOR
      if (LoadingVersion < VersionSpellNames)
      {
        int monster;
        int indx;
        ar >> monster;
        indx = monsterData.LocatePreSpellNamesKey(monster);
        if (indx < 0)
        {
          monsterID.Clear();
        }
        else
        {
          monsterID = monsterData.PeekMonster(indx)->MonsterID();
        };
        characterID.Empty();
        //ar >> monster;
      }
      else
#endif
      {
        ar >> monsterID;
#ifdef UAFEDITOR
        if (LoadingVersion > 0.9984016)
#endif
        {
          ar >> characterID;
        };
      };
      ar >> friendly;
      if (LoadingVersion >= _VERSION_0690_)
        ar >> moraleAdj;        

      if (LoadingVersion >= _VERSION_0910_)
      {
        ar >> qtyDiceSides;
        ar >> qtyDiceQty;
        ar >> qtyBonus;
        ar >> UseQty;
      }

      if (LoadingVersion >= _VERSION_0740_)
        money.Serialize(ar, LoadingVersion);
   }

   items.Serialize(ar,LoadingVersion);
}

void MONSTER_EVENT_DATA::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);
   int count, i;

   if (ar.IsStoring())
   {
      monsters.FreeExtra();
      count = GetCount();
      ar << count;
   }
   else
   {
      ar >> count;
      monsters.SetSize(count);
   }

   for (i=0;i<count;i++)
     monsters[i].Serialize(ar);
}
void MONSTER_EVENT_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   int count, i;

   if (ar.IsStoring())
   {
      monsters.FreeExtra();
      count = GetCount();
      ar << count;
   }
   else
   {
      ar >> count;
      monsters.SetSize(count);
   }

   for (i=0;i<count;i++)
     monsters[i].Serialize(ar);
}

#ifdef UAFEDITOR
const char *JKEY_MONSTER = "monster";
const char *JKEY_NUMBEROFMONSTERS = "number";
extern const char *JKEY_TYPE;
const char *JKEY_MONSTERNAME = "name";
const char *JKEY_FRIENDLY = "friendly";
const char *JKEY_MORALEADJUST = "moraleAdjustment";
const char *JKEY_DICESIDES = "diceSides";
const char *JKEY_NUMBEROFDICE = "diceNumber";
const char *JKEY_DICEBONUS = "diceBonus";
const char *JKEY_USENUMBER = "fixedNumberOfMonsters";
const char *JKEY_POSSESSIONS = "possessions";

const char *MonsterTypeText[] = {
  "CHARACTER",
  "NPC",
  "MONSTER",
  "FAKE CHARACTER_TYPE",
  "IN_PARTY",
};

const char *MonsterType(int type)
{
  switch(type)
  {
  case CHAR_TYPE: return MonsterTypeText[0];
  case NPC_TYPE: return MonsterTypeText[1];
  case MONSTER_TYPE: return MonsterTypeText[2];
  case FAKE_CHARACTER_TYPE: return MonsterTypeText[3];
  case IN_PARTY_TYPE: return MonsterTypeText[4];
  default: return "UNKNOWN";
  }
}
int MonsterType(const CString& type)
{
  if (type == MonsterTypeText[0]) return CHAR_TYPE;
  if (type == MonsterTypeText[1]) return NPC_TYPE;
  if (type == MonsterTypeText[2]) return MONSTER_TYPE;  
  if (type == MonsterTypeText[3]) return FAKE_CHARACTER_TYPE;
  if (type == MonsterTypeText[4]) return IN_PARTY_TYPE;
  return 0;
}


void MONSTER_EVENT::Export(JWriter& jw)
{
  jw.StartList();
  jw.NameAndNonZero(JKEY_NUMBEROFMONSTERS, qty);
  jw.NameAndValue(JKEY_TYPE, MonsterType(m_type));
  jw.NameAndValue(JKEY_MONSTERNAME, monsterID);
  jw.NameAndBool(JKEY_FRIENDLY, friendly);
  jw.NameAndValue(JKEY_MORALEADJUST, moraleAdj);
  jw.NameAndNonZero(JKEY_DICESIDES, qtyDiceSides);
  jw.NameAndNonZero(JKEY_NUMBEROFDICE, qtyDiceQty);
  jw.NameAndNonZero(JKEY_DICEBONUS, qtyBonus);
  jw.NameAndBool(JKEY_USENUMBER, UseQty==0);
  money.Export(jw);
  items.Export(jw, JKEY_POSSESSIONS);
  jw.EndList();
}
bool MONSTER_EVENT::Import(JReader& jr)
{
  CString stype;
  if (jr.Optional(), !jr.NextEntry()) return false;
  jr.StartList();
  jr.NameAndNonZero(JKEY_NUMBEROFMONSTERS, qty);
  jr.NameAndValue(JKEY_TYPE, stype);
  m_type = MonsterType(stype);
  jr.NameAndValue(JKEY_MONSTERNAME, monsterID);
  jr.NameAndBool(JKEY_FRIENDLY, friendly);
  jr.NameAndValue(JKEY_MORALEADJUST, moraleAdj);
  jr.NameAndNonZero(JKEY_DICESIDES, qtyDiceSides);
  jr.NameAndNonZero(JKEY_NUMBEROFDICE, qtyDiceQty);
  jr.NameAndNonZero(JKEY_DICEBONUS, qtyBonus);
  BOOL useNumber;
  jr.NameAndBool(JKEY_USENUMBER, useNumber);
  UseQty = !useNumber;
  money.Import(jr);
  items.Import(jr, JKEY_POSSESSIONS);
  jr.EndList();
  return true;
}
#endif

#ifdef UAFEDITOR
const char *JKEY_MONSTERS = "monsters";

void MONSTER_EVENT_DATA::Export(JWriter& jw)
{
  int count, i;
  monsters.FreeExtra();
  count = GetCount();
  if (count == 0) return;
  jw.StartArray(JKEY_MONSTERS);
  for (i=0;i<count;i++)
  {
    jw.NextEntry();
    monsters[i].Export(jw);
  };
  jw.EndArray();
}
bool MONSTER_EVENT_DATA::Import(JReader& jr)
{
  if (jr.Optional(), !jr.StartArray(JKEY_MONSTERS))return false;
  MONSTER_EVENT monsterEvent;
  while (monsterEvent.Import(jr))
  {
    monsters.Add(monsterEvent);
  };
  jr.EndArray();
  return true;
}
#endif


void TOUR_STEP::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,text);
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;
     ar << (int)step;
   }
   else
   {
     int temp;
     DAS(ar,text);
//     ar >> text;
//     if (text == "*")
//       text = "";
     ar >> temp;
     step = (eventStepType)temp;
   }
}
void TOUR_STEP::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,text);
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;
     ar << (int)step;
   }
   else
   {
     int temp;
     DAS(ar,text);
//     ar >> text;
//     if (text == "*")
//       text = "";
     ar >> temp;
     step = (eventStepType)temp;
   }
}

#ifdef UAFEDITOR

const char *JKEY_STEP = "step";
const char *JKEY_STEPNUM = "stepNum";
const char *JKEY_STEPS = "steps";

void TOUR_STEP::Export(JWriter& jw, int stepNum)
{
  if (text.IsEmpty() && (step == TStep_NoMove)) return;
  jw.NextEntry();
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_STEPNUM, stepNum);
  jw.NameAndValue(JKEY_TEXT, text);
  jw.NameAndEnum(JKEY_STEP, step);
  jw.EndList();
  jw.Linefeed(true);
}
int TOUR_STEP::Import(JReader& jr)
{
  int stepNum;
  jr.StartList();
  jr.Linefeed(false);
  jr.NameAndValue(JKEY_STEPNUM, stepNum);
  jr.NameAndValue(JKEY_TEXT, text);
  jr.NameAndEnum(JKEY_STEP, step);
  jr.EndList();
  jr.Linefeed(true);
  return stepNum;
}
#endif

void ENCOUNTER_BUTTON_OPTION::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
//
     ar << present;
     ar << allowedUpClose;
     ar << (int)optionResult;
     ar << chain;
     ar << onlyUpClose;
   }
   else
   {
     int temp;

     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> allowedUpClose;
     ar >> temp;
     optionResult = (encounterButtonResultType)temp;
     ar >> chain;
     if (LoadingVersion >= _VERSION_0890_)
      ar >> onlyUpClose;
   }
}
void ENCOUNTER_BUTTON_OPTION::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
     ar << present;
     ar << allowedUpClose;
     ar << (int)optionResult;
     ar << chain;
     ar << onlyUpClose;
   }
   else
   {
     int temp;

     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> allowedUpClose;
     ar >> temp;
     optionResult = (encounterButtonResultType)temp;
     ar >> chain;
     if (LoadingVersion >= _VERSION_0890_)
       ar >> onlyUpClose;
   }
}

#ifdef UAFEDITOR

const char *JKEY_LABEL = "label";
const char *JKEY_ENCOUNTERBUTTON = "encounterButton";
const char *JKEY_PRESENT = "present";
const char *JKEY_ALLOWEDUPCLOSE = "allowedUpClose";
const char *JKEY_OPTIONRESULT = "optionResult";
const char *JKEY_CHAIN = "chain";
const char *JKEY_ONLYUPCLOSE = "onlyUpClose";

void ENCOUNTER_BUTTON_OPTION::Export(JWriter& jw)
{
  jw.StartList();
  jw.NameAndValue(JKEY_LABEL, label);
  jw.NameAndBool(JKEY_PRESENT, present);
  jw.NameAndBool(JKEY_ALLOWEDUPCLOSE, allowedUpClose);
  jw.NameAndEnum(JKEY_OPTIONRESULT, optionResult);
  jw.NameAndValue(JKEY_CHAIN, chain);
  jw.NameAndBool(JKEY_ONLYUPCLOSE, onlyUpClose);
  jw.EndList();
};
bool ENCOUNTER_BUTTON_OPTION::Import(JReader& jr)
{
  jr.StartList();
  jr.NameAndValue(JKEY_LABEL, label);
  jr.NameAndBool(JKEY_PRESENT, present);
  jr.NameAndBool(JKEY_ALLOWEDUPCLOSE, allowedUpClose);
  jr.NameAndEnum(JKEY_OPTIONRESULT, optionResult);
  jr.NameAndValue(JKEY_CHAIN, chain);
  jr.NameAndBool(JKEY_ONLYUPCLOSE, onlyUpClose);
  jr.EndList();
  return true;
};
#endif


void ENCOUNTER_BUTTON_DATA::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);
   int i;

   if (ar.IsStoring())
   {
     ar << numButtons;
   }
   else
   {
     ar >> numButtons;
   }

   for (i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}
void ENCOUNTER_BUTTON_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   int i;

   if (ar.IsStoring())
   {
     ar << numButtons;
   }
   else
   {
     ar >> numButtons;
   }

   for (i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}
#ifdef UAFEDITOR

const char *JKEY_ENCOUNTERBUTTONS = "encounterButtons";
void ENCOUNTER_BUTTON_DATA::Export(JWriter& jw)
{
   //CObject::Serialize(ar);
  int i;
  jw.StartArray(JKEY_ENCOUNTERBUTTONS);  
  for (i=0;i<MAX_BUTTONS;i++)
  {
    if (!buttons[i].present) continue;
    jw.NextEntry();
    buttons[i].Export(jw);
  };
  jw.EndArray();
}

bool ENCOUNTER_BUTTON_DATA::Import(JReader& jr)
{
   //CObject::Serialize(ar);
  int i;
  if (jr.Optional(), !jr.StartArray(JKEY_ENCOUNTERBUTTONS)) return false;  
  i = 0;
  while (jr.Optional(), jr.NextEntry())
  {
    if (i < MAX_BUTTONS)
    {
      buttons[i++].Import(jr);
    };
  };
  numButtons = i;
  jr.EndList();
  return true;
}
#endif

void QLIST_OPTION::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {

     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;

     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}
void QLIST_OPTION::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
//
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;
     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}

#ifdef UAFEDITOR

const char *JKEY_QLISTOPTION = "qListOption";
//const char *JKEY_LABEL = "label";
//const char *JKEY_PRESENT = "present";
const char *JKEY_POSTCHAINACTION = "postChainAction";
//const char *JKEY_CHAIN = "chain";


void QLIST_OPTION::Export(JWriter& jw)
{
  jw.ListEntry();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_LABEL, label);
  jw.NameAndBool(JKEY_PRESENT, present);
  jw.NameAndEnum(JKEY_POSTCHAINACTION, postChainAction);
  jw.NameAndValue(JKEY_CHAIN, chain);
  jw.EndList();
  jw.Linefeed(true);
}
bool QLIST_OPTION::Import(JReader& jr)
{
  jr.StartList();
  jr.Linefeed(false);
  jr.NameAndValue(JKEY_LABEL, label);
  jr.NameAndBool(JKEY_PRESENT, present);
  jr.NameAndEnum(JKEY_POSTCHAINACTION, postChainAction);
  jr.NameAndValue(JKEY_CHAIN, chain);
  jr.EndList();
  jr.Linefeed(true);
  return true;
}
#endif

void QLIST_DATA::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,title);
//     if (title.IsEmpty())
//     {
//        title = "*";
//        ar << title;
//        title="";
//     }
//     else
//        ar << title;
//
     ar << numListButtons;
   }
   else
   {
     DAS(ar,title);
//     ar >> title;
//     if (title == "*")
//       title = "";
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}
void QLIST_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,title);
//     if (title.IsEmpty())
//     {
//        title = "*";
//        ar << title;
//        title="";
//     }
//     else
//        ar << title;
//
     ar << numListButtons;
   }
   else
   {
     DAS(ar,title);
//     ar >> title;
//     if (title == "*")
//       title = "";
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}

#ifdef UAFEDITOR

const char *JKEY_TITLE = "title";
const char *JKEY_PARTICULAR = "particular";
const char *JKEY_OPTIONS = "options";
const char *JKEY_NUMBUTTON = "numButton";
void QLIST_DATA::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_TITLE, title);
  jw.NameAndValue(JKEY_NUMBUTTON, numListButtons);
  jw.StartArray(JKEY_OPTIONS);
  for (int i=0;i<MAX_BUTTONS;i++)
  {
    buttons[i].Export(jw);
  };
  jw.EndArray();
}
bool QLIST_DATA::Import(JReader& jr)
{
  jr.NameAndValue(JKEY_TITLE, title);
  jr.NameAndValue(JKEY_NUMBUTTON, numListButtons);
  jr.StartArray(JKEY_OPTIONS);
  for (int i=0;i<MAX_BUTTONS;i++)
  {
    jr.Optional();
    if (!jr.NextEntry()) break;
    buttons[i].Import(jr);
  };
  jr.EndArray();
  return true;
}

#endif


void QBUTTON_OPTION::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
//
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;
     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}
void QBUTTON_OPTION::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;
     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}

#ifdef UAFEDITOR

//const char *JKEY_LABEL = "label";
//const char *JKEY_PRESENT = "present";
//const char *JKEY_POSTCHAINACTION = "postChainAction";
//const char *JKEY_CHAIN = "chain";
const char *JKEY_BUTTONLIST = "buttonList";

void QBUTTON_OPTION::Export(JWriter& jw)
{
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndBool(JKEY_PRESENT, present);
  jw.NameAndValue(JKEY_LABEL, label);
  jw.NameAndEnum(JKEY_POSTCHAINACTION, postChainAction);
  jw.NameAndValue(JKEY_CHAIN, chain);
  jw.EndList();
  jw.Linefeed(true);
}

bool QBUTTON_OPTION::Import(JReader& jr)
{
  jr.StartList(NULL);
  jr.Linefeed(false);
  jr.NameAndBool(JKEY_PRESENT, present);
  jr.NameAndValue(JKEY_LABEL, label);
  jr.NameAndEnum(JKEY_POSTCHAINACTION, postChainAction);
  jr.NameAndValue(JKEY_CHAIN, chain);
  jr.EndList();
  jr.Linefeed(true);
  return true;
}

#endif


void QBUTTON_DATA::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     ar << numListButtons;
   }
   else
   {
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
      buttons[i].Serialize(ar);
}
void QBUTTON_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     ar << numListButtons;
   }
   else
   {
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
      buttons[i].Serialize(ar);
}

#ifdef UAFEDITOR
void QBUTTON_DATA::Export(JWriter& jw)
{
  jw.StartArray(JKEY_BUTTONLIST);
  for (int i=0;i<numListButtons; i++)
  {
    jw.NextEntry();
    buttons[i].Export(jw);
  };
  jw.EndArray();
}
bool QBUTTON_DATA::Import(JReader& jr)
{
  int i;
  if (jr.Optional(), !jr.StartArray(JKEY_BUTTONLIST))return false;
  i = 0;
  while (jr.Optional(), jr.NextEntry())
  {
    QBUTTON_OPTION but;
    but.Import(jr);
    if (i < MAX_BUTTONS)
    {
      buttons[i++] = but;
    };
    numListButtons = i;
  };
  jr.EndArray();
  return true;
}
#endif


void TALE::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,tale);
     ar << m_count;
   }
   else
   {
     DAS(ar,tale);
     if (LoadingVersion >= _VERSION_0910_)
       ar >> m_count;
   }
}
void TALE::Serialize(CAR &ar)
{
   if (ar.IsStoring())
   {
     AS(ar,tale);
     ar << m_count;
   }
   else
   {
     DAS(ar,tale);
     if (LoadingVersion >= _VERSION_0910_)
       ar >> m_count;
   }
}

#ifdef UAFEDITOR

const char *JKEY_TALES = "tales";
const char *JKEY_COUNT = "count";
//const char *JKEY_INDEX = "index";

void TALE::Export(JWriter& jw, int i)
{
  if (tale.IsEmpty()) return;
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_INDEX, i);
  jw.NameAndValue(JKEY_COUNT, m_count);
  jw.NameAndValue(JKEY_TEXT, tale);
  jw.EndList();
  jw.Linefeed(true);
}

int TALE::Import(JReader& jr)
{
  int i;
  jr.StartList();
  jr.Linefeed(false);
  jr.NameAndValue(JKEY_INDEX, i);
  jr.NameAndValue(JKEY_TEXT, tale);
  jr.NameAndValue(JKEY_COUNT, m_count);
  jr.EndList();
  jr.Linefeed(true);
  return i;
}

#endif

void DRINK::Serialize(CArchive &ar)
{
//   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,name);
//     if (name.IsEmpty())
//     {
//        name = "*";
//        ar << name;
//        name="";
//     }
//     else
//        ar << name;
     ar << points;
   }
   else
   {
     DAS(ar,name);
//     ar >> name;
//     if (name == "*")
//       name = "";
     ar >> points;
   }
}
void DRINK::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,name);
//     if (name.IsEmpty())
//     {
//        name = "*";
//        ar << name;
//        name="";
//     }
//     else
//        ar << name;
     ar << points;
   }
   else
   {
     DAS(ar,name);
//     ar >> name;
//     if (name == "*")
//       name = "";
     ar >> points;
   }
}

#ifdef UAFEDITOR

extern const char *JKEY_NAME;
const char *JKEY_POINTS = "points";

void DRINK::Export(JWriter& jw, int i)
{
  if (name.IsEmpty()) return;
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_INDEX, i);
  jw.NameAndValue(JKEY_POINTS, points);
  jw.NameAndValue(JKEY_NAME, name);
//     if (name.IsEmpty())
//     {
//        name = "*";
//        ar << name;
//        name="";
//     }
//     else
//        ar << name;
  jw.EndList();
  jw.Linefeed(true);
}

extern const char *JKEY_DRINKS;

int DRINK::Import(JReader& jr)
{
  int i;
  jr.StartList();
  jr.Linefeed(false);
  jr.NameAndValue(JKEY_INDEX, i);
  jr.NameAndValue(JKEY_NAME, name);
  jr.NameAndValue(JKEY_POINTS, points);
  jr.EndList();
  jr.Linefeed(true);
  return i;
}

#endif


void REST_EVENT::Serialize(CArchive &ar)
{
//  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << allowResting;
    ar << restEvent;
    ar << chance;
    ar << everyMin;
    ar << prevMinChecked;
  }
  else
  {
    ar >> allowResting;
    ar >> restEvent;
    ar >> chance;
    ar >> everyMin;
    ar >> prevMinChecked;
  }
}
void REST_EVENT::Serialize(CAR &ar)
{
  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << allowResting;
    ar << restEvent;
    ar << chance;
    ar << everyMin;
    ar << prevMinChecked;
  }
  else
  {
    ar >> allowResting;
    ar >> restEvent;
    ar >> chance;
    ar >> everyMin;
    ar >> prevMinChecked;
  }
}
#ifdef UAFEDITOR
const char *JKEY_ALLOWRESTING = "allowResting";
extern const char *JKEY_RESTEVENT;
//const char *JKEY_CHANCE = "chance";
const char *JKEY_EVERYMIN = "everyMin";
const char *JKEY_PREVMINCHECKED = "prevMinChecked";
void REST_EVENT::Export(JWriter& jw)
{
  jw.StartList(JKEY_RESTEVENT);
  jw.NameAndValue(JKEY_ALLOWRESTING, allowResting);
  jw.NameAndValue(JKEY_RESTEVENT, restEvent);
  jw.NameAndValue(JKEY_CHANCE, chance);
  jw.NameAndValue(JKEY_EVERYMIN, everyMin);
  jw.NameAndValue(JKEY_PREVMINCHECKED, prevMinChecked);
  jw.EndList();
}
void REST_EVENT::Import(JReader& jr)
{
  jr.StartList(JKEY_RESTEVENT);
  jr.NameAndValue(JKEY_ALLOWRESTING, allowResting);
  jr.NameAndValue(JKEY_RESTEVENT, restEvent);
  jr.NameAndValue(JKEY_CHANCE, chance);
  jr.NameAndValue(JKEY_EVERYMIN, everyMin);
  jr.NameAndValue(JKEY_PREVMINCHECKED, prevMinChecked);
  jr.EndList();
}
#endif

void TIME_EVENT_DATA::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
  }
}

void TIME_EVENT_DATA::PostSerialize(BOOL IsStoring)
{
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map    
  }
}

void TIME_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  PreSerialize(ar.IsStoring());
  
//  CObject::Serialize(ar);
  
  if (ar.IsStoring())
  {
    ar << timeEvent;
    ar << triggerType;
    ar << day;
    ar << hour;
    ar << minute;

    timeevent_asl.Serialize(ar, "TIME_EVENT_ATTR");
  }
  else
  {
    ar >> timeEvent;
    ar >> triggerType;
    ar >> day;
    ar >> hour;
    ar >> minute;

    timeevent_asl.Serialize(ar, "TIME_EVENT_ATTR");
  }
  
  PostSerialize(ar.IsStoring());
}

void TIME_EVENT_DATA::Serialize(CAR &ar, double version)
{
  PreSerialize(ar.IsStoring());
  //CObject::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << timeEvent;
    ar << triggerType;
    ar << day;
    ar << hour;
    ar << minute;

    timeevent_asl.Serialize(ar, "TIME_EVENT_ATTR");
  }
  else
  {
    ar >> timeEvent;
    ar >> triggerType;
    ar >> day;
    ar >> hour;
    ar >> minute;

    timeevent_asl.Serialize(ar, "TIME_EVENT_ATTR");
  }
  
  PostSerialize(ar.IsStoring());
}


void STEP_EVENT_DATA::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    //StoreIntAsASL(stepevent_asl, stepCount, "StpCnt");
  }
}

void STEP_EVENT_DATA::PostSerialize(BOOL IsStoring, double version)
{
  if (IsStoring)
  {
    //stepevent_asl.Delete("StpCnt");
  }
  else
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    if (version < 1.0210)
    {
    RetrieveIntFromASL(stepevent_asl, stepCount, "StpCnt");
    stepevent_asl.Delete("StpCnt");
    };
  }
}

void STEP_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  PreSerialize(ar.IsStoring());

//  CObject::Serialize(ar);
  int i;
  
  if (ar.IsStoring())
  {
    ar << stepCount;
    ar << stepEvent;
    //ar << MAX_ZONES;
    //for (i=0;i<MAX_ZONES;i++)
    //  ar << stepTrigger[i];
    ar << zoneMask;
    ar << name;
    stepevent_asl.Serialize(ar, "STEPEVENT_ATTR");
  }
  else
  {
    if (version < 1.0210)
    {
    ar >> stepEvent;
    if (version >= _VERSION_05661_)
    {
      int count;
        BOOL v;
      ar >> count;
      for (i=0;i<count;i++)
        //  ar >> stepTrigger[i];
        {
          ar >> v;
          if (v) zoneMask |= (1 << i);
        };
    }
    else
    {
        BOOL v;
      for (i=0;i<8;i++) // load old quantity of zones (= 8)
          //ar >> stepTrigger[i];
        {
          ar >> v;
          if (v) zoneMask |= (1 << i);
        };
      };
    }
    else
    {
      ar >> stepCount;
      ar >> stepEvent;
      ar >> zoneMask;
      ar >> name;
    };


    if (version >= _VERSION_0566_)
      stepevent_asl.Serialize(ar, "STEPEVENT_ATTR");
  }

  PostSerialize(ar.IsStoring(), version);
}

void STEP_EVENT_DATA::Serialize(CAR &ar, double version)
{
  PreSerialize(ar.IsStoring());
  //CObject::Serialize(ar);
  int i;  
  if (ar.IsStoring())
  {
    ar << stepCount;
    ar << stepEvent;
    //ar << MAX_ZONES;
    //for (i=0;i<MAX_ZONES;i++)
    //  ar << stepTrigger[i];
    ar << zoneMask;
    ar << name;
    stepevent_asl.Serialize(ar, "STEPEVENT_ATTR");
  }
  else
  {
    if (version < 1.0210)
    {
    ar >> stepEvent;
    if (version >= _VERSION_05661_)
    {
      int count;
        BOOL v;
      ar >> count;
      for (i=0;i<count;i++)
        //  ar >> stepTrigger[i];
        {
          ar >> v;
          if (v) zoneMask |= (1 << i);
        };
    }
    else
    {
        BOOL v;
      for (i=0;i<8;i++) // load old quantity of zones (= 8)
          //ar >> stepTrigger[i];
        {
          ar >> v;
          if (v) zoneMask |= (1 << i);
        };
      };
    }
    else
    {
      ar >> stepCount;
      ar >> stepEvent;
      ar >> zoneMask;
      ar >> name;
    };


    if (version >= _VERSION_0566_)
      stepevent_asl.Serialize(ar, "STEPEVENT_ATTR");
  }

  PostSerialize(ar.IsStoring(), version);
}

#ifdef UAFEDITOR
const char *JKEY_STEPEVENTS = "stepEvents";
const char *JKEY_ID = "id";
const char *JKEY_TRIGGERS = "triggers";
extern const char *JKEY_ASL;

void STEP_EVENT_DATA::Export(JWriter& jw)
{
  int i;
  jw.StartList();
  //jw.NameAndValue(JKEY_ID, stepEvent);
  jw.StartArray(JKEY_TRIGGERS);
  jw.Linefeed(false);
  for (i=0;i<MAX_ZONES;i++)
  {
    BOOL b;
    jw.NextEntry();
    b = ((1 << i) & zoneMask) != 0;
    //jw.NameAndValue(NULL, stepTrigger[i]);
    jw.NameAndValue(NULL, b);
  };
  jw.EndArray();
  jw.Linefeed(true);
  jw.NameAndValue(JKEY_EVENT, stepEvent);
  jw.NameAndValue(JKEY_STEPS, stepCount);
  jw.NameAndValue(JKEY_NAME,  name);
  stepevent_asl.Export(jw);
  jw.EndList();
}

void STEP_EVENT_DATA::Import(JReader& jr)
{
  int i;
  jr.StartList();
  jr.NameAndValue(JKEY_ID, stepEvent);
  jr.StartArray(JKEY_TRIGGERS);
  jr.Linefeed(false);
  //for (i=0;i<MAX_ZONES;i++)
  //{
  //  stepTrigger[i] = 0;
  //};
  zoneMask = 0;
  for (i=0;i<MAX_ZONES;i++)
  {
    BOOL b;
    jr.Optional(); 
    if (!jr.NextEntry()) break;;
    //jr.NameAndValue(NULL, stepTrigger[i]);
    jr.NameAndValue(NULL, b);
    if (b) zoneMask |= (1<<i);
  };
  jr.EndArray();
  jr.Linefeed(true);
  jr.NameAndValue(JKEY_EVENT, stepEvent);
  jr.NameAndValue(JKEY_STEPS, stepCount);
  stepevent_asl.Import(jr);
  jr.EndList();
}


void STEP_EVENT_DATA::CrossReference(CR_EVENT_INFO *pCREI)
{
  GameEvent *pEvent;
  if (pCREI->m_pEventList->IsValidEvent(stepEvent))
  {
    pEvent = pCREI->m_pEventList->GetEvent(stepEvent);
    if (pEvent != NULL)
    {
      pEvent->CrossReference(pCREI);
      return;
    };
  };
  // Something wrong.....event does not exist.  But the reference exists!
  pCREI->m_pCRList->CR_AddEventReference(stepEvent, &pCREI->m_CRReference);
}

#endif

TRANSFER_DATA& TRANSFER_DATA::operator =(const TRANSFER_DATA& src)
{
  if (&src == this)
    return *this;
  execEvent = src.execEvent;
  m_facing = src.m_facing;
  destEP = src.destEP;
  destLevel = src.destLevel;
  destX = src.destX;
  destY = src.destY;
  return *this;
}
#ifdef UAFEDITOR
bool TRANSFER_DATA::operator ==(const TRANSFER_DATA& src) const
{
  if (&src == this) return true;
  if (execEvent != src.execEvent) return false;
  if (m_facing != src.m_facing) return false;
  if (destEP != src.destEP) return false;
  if (destLevel != src.destLevel) return false;
  if (destX != src.destX) return false;
  if (destY != src.destY) return false;
  return true;
}
#endif

MONSTER_EVENT& MONSTER_EVENT::operator =(const MONSTER_EVENT& src)
{
  if (&src == this)
    return *this;
  qty = src.qty;
  m_type = src.m_type;
  //monster = src.monster;
  monsterID = src.monsterID;
  characterID = src.characterID;
  friendly = src.friendly;
  items = src.items;
  moraleAdj = src.moraleAdj;
  qtyDiceSides = src.qtyDiceSides;
  qtyDiceQty = src.qtyDiceQty;
  qtyBonus = src.qtyBonus;
  UseQty = src.UseQty;

  money=src.money;
  return *this;
}
#ifdef UAFEDITOR
bool MONSTER_EVENT::operator ==(const MONSTER_EVENT& src) const
{
  if (&src == this) return true;
  if (qty != src.qty) return false;
  if (m_type != src.m_type) return false;
  if (monsterID != src.monsterID) return false;
  if (characterID != src.characterID) return false;
  if (friendly != src.friendly) return false;
  if (!(items == src.items)) return false;
  if (moraleAdj != src.moraleAdj) return false;
  if (qtyDiceSides != src.qtyDiceSides) return false;
  if (qtyDiceQty != src.qtyDiceQty) return false;
  if (qtyBonus != src.qtyBonus) return false;
  if (UseQty != src.UseQty) return false;
  if (!(money == src.money)) return false;
  return true;
}
#endif
MONSTER_EVENT_DATA& MONSTER_EVENT_DATA::operator =(const MONSTER_EVENT_DATA& src)
{
  if (&src == this)
    return *this;

  Clear();
  for (int i=0;i<src.monsters.GetSize();i++)
    Add(src.monsters[i]);
  return *this;
}
#ifdef UAFEDITOR
bool MONSTER_EVENT_DATA::operator ==(const MONSTER_EVENT_DATA& src) const
{
  if (&src == this) return true;
  if (monsters.GetSize() != src.monsters.GetSize()) return false;
  for (int i=0;i<src.monsters.GetSize();i++)
  {
    if (!(monsters[i] == src.monsters[i])) return false;
  };
  return true;
}
#endif
TOUR_STEP& TOUR_STEP::operator =(const TOUR_STEP& src)
{
  if (&src == this)
    return *this;
  text = src.text;
  step = src.step;
  return *this;
}
#ifdef UAFEDITOR
bool TOUR_STEP::operator ==(const TOUR_STEP& src) const
{
  if (&src == this) return true;
  if (text != src.text) return false;
  if (step != src.step) return false;
  return true;
}
#endif

ENCOUNTER_BUTTON_OPTION& ENCOUNTER_BUTTON_OPTION::operator =(const ENCOUNTER_BUTTON_OPTION& src)
{
  if (&src == this)
    return *this;

  label = src.label;
  present = src.present;
  allowedUpClose = src.allowedUpClose;
  optionResult = src.optionResult;
  chain = src.chain;
  onlyUpClose=src.onlyUpClose;
  return *this;
}
#ifdef UAFEDITOR
bool ENCOUNTER_BUTTON_OPTION::operator ==(const ENCOUNTER_BUTTON_OPTION& src) const
{
  if (&src == this) return true;
  if (label != src.label) return false;
  if (present != src.present) return false;
  if (allowedUpClose != src.allowedUpClose) return false;
  if (optionResult != src.optionResult) return false;
  if (chain != src.chain) return false;
  if (onlyUpClose != src.onlyUpClose) return false;
  return true;
}
#endif
ENCOUNTER_BUTTON_DATA& ENCOUNTER_BUTTON_DATA::operator =(const ENCOUNTER_BUTTON_DATA& src)
{
  if (&src == this)
    return *this;

  numButtons = src.numButtons;
  for (int i=0;i<MAX_BUTTONS;i++)
    buttons[i] = src.buttons[i];
  return *this;
}
#ifdef UAFEDITOR
bool ENCOUNTER_BUTTON_DATA::operator ==(const ENCOUNTER_BUTTON_DATA& src) const
{
  if (&src == this) return true;
  if (numButtons != src.numButtons) return false;
  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (!(buttons[i] == src.buttons[i])) return false;
  };
  return true;
}
#endif
QLIST_OPTION& QLIST_OPTION::operator =(const QLIST_OPTION& src)
{
  if (&src == this)
    return *this;

  label = src.label;
  postChainAction = src.postChainAction;
  present = src.present;
  chain = src.chain;
  return *this;
}
#ifdef UAFEDITOR
bool QLIST_OPTION::operator ==(const QLIST_OPTION& src) const
{
  if (&src == this) return true;
  if (label != src.label) return false;
  if (postChainAction != src.postChainAction) return false;
  if (present != src.present) return false;
  if (chain != src.chain) return false;
  return true;
}
#endif
QLIST_DATA& QLIST_DATA::operator =(const QLIST_DATA& src)
{
  if (&src == this)
    return *this;

  title = src.title;
  numListButtons = src.numListButtons;
  for (int i=0;i<MAX_BUTTONS; i++)
    buttons[i] = src.buttons[i];
  return *this;
}
#ifdef UAFEDITOR
bool QLIST_DATA::operator ==(const QLIST_DATA& src) const
{
  if (&src == this) return true;
  if (title != src.title) return false;
  if (numListButtons != src.numListButtons) return false;
  for (int i=0;i<MAX_BUTTONS; i++)
    if (!(buttons[i] == src.buttons[i])) return false;
  return true;
}

#endif
QBUTTON_OPTION& QBUTTON_OPTION::operator =(const QBUTTON_OPTION& src)
{
  if (&src == this)
    return *this;

  label = src.label;
  postChainAction = src.postChainAction;
  present = src.present;
  chain = src.chain;
  return *this;
}
#ifdef UAFEDITOR
bool QBUTTON_OPTION::operator ==(const QBUTTON_OPTION& src) const
{
  if (&src == this) return true;
  if (label != src.label) return false;
  if (postChainAction != src.postChainAction) return false;
  if (present != src.present) return false;
  if (chain != src.chain) return false;
  return true;
}
#endif
QBUTTON_DATA& QBUTTON_DATA::operator =(const QBUTTON_DATA& src)
{
  if (&src == this)
    return *this;

  numListButtons = src.numListButtons;
  for (int i=0;i<MAX_BUTTONS; i++)
    buttons[i] = src.buttons[i];
  return *this;
}
#ifdef UAFEDITOR
bool QBUTTON_DATA::operator ==(const QBUTTON_DATA& src) const
{
  if (&src == this) return true;
  if (numListButtons != src.numListButtons) return false;
  for (int i=0;i<MAX_BUTTONS; i++)
  {
    if (!(buttons[i] == src.buttons[i])) return false;
  };
  return true;
}
#endif
TALE& TALE::operator =(const TALE& src)
{
  if (&src == this)
    return *this;

  tale = src.tale;
  m_count = src.m_count;
  return *this;
}
#ifdef UAFEDITOR
bool TALE::operator ==(const TALE& src) const
{
  if (&src == this) return true;
  if (tale != src.tale) return false;
  if (m_count != src.m_count) return false;
  return true;
}
#endif
DRINK& DRINK::operator =(const DRINK& src)
{
  if (&src == this)
    return *this;

  name = src.name;
  points = src.points;
  return *this;
}
#ifdef UAFEDITOR
bool DRINK::operator ==(const DRINK& src) const
{
  if (&src == this) return true;
  if (name != src.name) return false;
  if (points != src.points) return false;
  return true;
}
#endif
REST_EVENT& REST_EVENT::operator =(const REST_EVENT& src)
{
  if (&src == this)
    return *this;

  allowResting = src.allowResting;
  restEvent = src.restEvent;
  chance = src.chance;
  everyMin = src.everyMin;
  prevMinChecked = src.prevMinChecked;
  return *this;
}

#ifdef UAFEDITOR
bool REST_EVENT::operator ==(const REST_EVENT& src)
{
  if (&src == this) return true;

  if (allowResting != src.allowResting) return false;
  if (restEvent != src.restEvent) return false;
  if (chance != src.chance) return false;
  if (everyMin != src.everyMin) return false;
  if (prevMinChecked != src.prevMinChecked) return false;
  return true;
}
#endif
TIME_EVENT_DATA& TIME_EVENT_DATA::operator =(const TIME_EVENT_DATA& src)
{
  if (&src == this)
    return *this;
  
  timeEvent = src.timeEvent;
  triggerType = src.triggerType;
  day = src.day;
  hour = src.hour;
  minute = src.minute;

  dayCounter = src.dayCounter;
  hourCounter = src.hourCounter;
  minuteCounter = src.minuteCounter;

  timeevent_asl.Copy(src.timeevent_asl);
  temp_asl.Copy(src.temp_asl);
  return *this;
}

STEP_EVENT_DATA& STEP_EVENT_DATA::operator =(const STEP_EVENT_DATA& src)
{
  if (&src == this)
    return *this;

  stepEvent = src.stepEvent;
  stepCount = src.stepCount;
  //for (int i=0;i<MAX_ZONES;i++)
  //  stepTrigger[i] = src.stepTrigger[i];
  zoneMask = src.zoneMask;
  name = src.name;
  stepevent_asl.Copy(src.stepevent_asl);
  temp_asl.Copy(src.temp_asl);
  return *this;
}

#ifdef UAFEDITOR
bool STEP_EVENT_DATA::operator ==(const STEP_EVENT_DATA& src)const
{
  if (&src == this) return true;
  if (stepEvent != src.stepEvent) return false;
  if (stepCount != src.stepCount) return false;
  if (name != src.name) return false;
  //for (int i=0;i<MAX_ZONES;i++)
  //{
  //  if (stepTrigger[i] != src.stepTrigger[i]) return false;
  //};
  if (zoneMask != src.zoneMask) return false;
  if (!(stepevent_asl == src.stepevent_asl)) return false;
  //if (!(temp_asl ==src.temp_asl)) return false;
  return true;
}
#endif


void PLAY_MOVIE_DATA::SaveEventResources(int level)
{
  GameEvent::SaveEventResources(level);
  SaveArt(m_filename,BufferDib, level, TRUE, rte.MovieDir());
}

void PLAY_MOVIE_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    AS(ar, m_filename);
    ar << m_mode;
  }
  else
  {
    DAS(ar, m_filename);
    if (LoadingVersion >= _VERSION_0790_)
      ar >> m_mode;
  }
}

void PLAY_MOVIE_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    AS(ar, m_filename);
    ar << m_mode;
  }
  else
  {
    DAS(ar, m_filename);
    if (LoadingVersion >= _VERSION_0790_)
      ar >> m_mode;
  }
}

#ifdef UAFEDITOR
void PLAY_MOVIE_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  //m_filename.  But what is it?  
  MsgBoxInfo("PLAY_MOVIE_DATA::DetailedCrossReference not implemented");
}

extern const char *JKEY_FILENAME;
const char *JKEY_MODE = "mode";
void PLAY_MOVIE_DATA::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_FILENAME, m_filename);
  jw.NameAndValue(JKEY_MODE, m_mode);
}
bool PLAY_MOVIE_DATA::Import(JReader& jr)
{
  jr.NameAndValue(JKEY_FILENAME, m_filename);
  jr.NameAndValue(JKEY_MODE, m_mode);
  return true;
}
#endif



void ADD_NPC_DATA::Serialize(CArchive &ar, double version)
{
  int temp;
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    //ar << charAdded;
    ar << characterID;
    ar << hitPointMod;
    ar << useOriginal;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    //ar >> charAdded;
    ar >> characterID;
    ar >> hitPointMod;
    if (LoadingVersion >= _VERSION_0695_)
      ar >> useOriginal;
  }
}
void ADD_NPC_DATA::Serialize(CAR &ar, double version)
{
  int temp;
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    //ar << charAdded;
    ar << characterID;
    ar << hitPointMod;
    ar << useOriginal;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
#ifdef UAFEDITOR
    if (version < VersionSpellNames)
    {
      int charKey, indx;
      ar >> charKey;
    //ar >> charAdded;
      indx = globalData.charData.LocateOrigIndex(charKey);
      characterID = globalData.charData.PeekCharacter(indx)->GetName();
    }
    else
#endif
    {
      ar >> characterID;
    };
    ar >> hitPointMod;
    if (LoadingVersion >= _VERSION_0695_)
      ar >> useOriginal;
  }
}


#ifdef UAFEDITOR
void ADD_NPC_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

extern const char *JKEY_PARTICULAR;
extern const char *JKEY_DISTANCE;
const char *JKEY_HITPOINTMOD = "HitPointModification";
const char *JKEY_USEORIGINAL = "UseOriginal";


void ADD_NPC_DATA::Export(JWriter& jw)
{
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndValue(JKEY_CHARACTER, characterID);
  jw.NameAndValue(JKEY_HITPOINTMOD, hitPointMod);
  jw.NameAndBool(JKEY_USEORIGINAL, useOriginal);
}
bool ADD_NPC_DATA::Import(JReader& jr)
{
  jr.NameAndEnum(JKEY_DISTANCE, distance);
  jr.NameAndValue(JKEY_CHARACTER, characterID);
  jr.NameAndValue(JKEY_HITPOINTMOD, hitPointMod);
  jr.NameAndBool(JKEY_USEORIGINAL, useOriginal);
  return true;
}
#endif

void REMOVE_NPC_DATA::Serialize(CArchive &ar, double version)
{
  int temp;
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    //ar << charRemoved;
    ar << characterID;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    //ar >> charRemoved;
    {
      int chID, indx;
      ar >> chID;
      indx = globalData.charData.LocatePreSpellNamesKey(chID);
      if (indx >= 0)
      {
        characterID = globalData.charData.PeekCharacter(indx)->CharacterID();
      }
      else
      {
        characterID.Empty();
      };
    };
  }
}
void REMOVE_NPC_DATA::Serialize(CAR &ar, double version)
{
  int temp;
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    //ar << charRemoved;
    ar << characterID;
  }
  else
  {
    ar >> temp;  
    distance = (eventDistType)temp;
    //ar >> charRemoved;
#ifdef UAFEDITOR
    if (version < VersionSpellNames)
    {
      int chID, indx;
      ar >> chID;
      indx = globalData.charData.LocatePreSpellNamesKey(chID);
      if (indx >= 0)
      {
        characterID = globalData.charData.PeekCharacter(indx)->CharacterID();
      }
      else
      {
        characterID.Empty();
      }
    }
    else
#endif
    {
      ar >> characterID;
    };
  }
}

#ifdef UAFEDITOR
void REMOVE_NPC_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}
void REMOVE_NPC_DATA::Export(JWriter& jw)
{
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndValue(JKEY_CHARACTER, characterID);
}
bool REMOVE_NPC_DATA::Import(JReader& jr)
{
  jr.NameAndEnum(JKEY_DISTANCE, distance);
  jr.NameAndValue(JKEY_CHARACTER, characterID);
  return true;
}
#endif




void CAMP_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    ar << ForceExit;
  }
  else
  {
    ar >> ForceExit;
  }
}
void CAMP_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    ar << ForceExit;
  }
  else
  {
    ar >> ForceExit;
  }
}

#ifdef UAFEDITOR
void CAMP_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}


extern const char *JKEY_PARTICULAR;
extern const char *JKEY_FORCEEXIT;

void CAMP_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_FORCEEXIT, ForceExit);
}
bool CAMP_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_FORCEEXIT, ForceExit);
  return true;
}
#endif


COMBAT_EVENT_DATA::COMBAT_EVENT_DATA() : GameEvent()
{ 
  Clear(); event=Combat;
}

COMBAT_EVENT_DATA::~COMBAT_EVENT_DATA() 
{ 
  //Clear(); 
}

void COMBAT_EVENT_DATA::Clear() 
{
   GameEvent::Clear();

#ifdef UAFEngine
   m_stoppedSounds = false;
#endif

   m_UseOutdoorMap=FALSE;
   distance=FarAway;direction=Any;m_eSurprise=Neither;
   autoApproach=FALSE;outdoors=FALSE;noMonsterTreasure=FALSE;
   partyNeverDies=FALSE;noMagic=FALSE;monsterMorale=50;
   partyNoExperience=FALSE;
   randomMonster=FALSE;
   turningMod=None;
   monsters.Clear();
   DeathSound = "";
   MoveSound = "";
   TurnUndeadSound="";
   bgSounds.Clear(); m_eTEMPsurprise=Neither;
#if (defined UAFEDITOR)
   DeathSound      = ede.TemplateSoundDir() + DEF_CHAR_DEATH_SOUND;//Format("%s%s", global_editorResourceDir, DEF_CHAR_DEATH_SOUND);
   MoveSound       = ede.TemplateSoundDir() + DEF_CHAR_MOVE_SOUND; //Format("%s%s", global_editorResourceDir, DEF_CHAR_MOVE_SOUND);
   TurnUndeadSound = ede.TemplateSoundDir() + DEF_CAST_SOUND;      //.Format("%s%s", global_editorResourceDir, DEF_CAST_SOUND);
#endif
}

void COMBAT_EVENT_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  SaveSound(DeathSound,      level, rte.SoundDir()); 
  SaveSound(MoveSound,       level, rte.SoundDir()); 
  SaveSound(TurnUndeadSound, level, rte.SoundDir());
  bgSounds.backgroundSounds.SaveSounds(level);
  bgSounds.nightSounds.SaveSounds(level);
}

void COMBAT_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    StripFilenamePath(DeathSound);
    StripFilenamePath(MoveSound);
    StripFilenamePath(TurnUndeadSound);
    AS(ar,DeathSound);
    AS(ar,MoveSound);
    AS(ar,TurnUndeadSound);

    //AddFolderToPath(DeathSound, rte.SoundDir());
    //AddFolderToPath(MoveSound, rte.SoundDir());
    //AddFolderToPath(TurnUndeadSound, rte.SoundDir());

    ar << (int)distance;
    ar << (int)direction;
    ar << (int)m_eSurprise;
    ar << autoApproach;
    ar << outdoors;
    ar << noMonsterTreasure;
    ar << partyNeverDies;
    ar << noMagic;
    ar << monsterMorale;
    ar << (int)turningMod;
    ar << randomMonster;
    ar << partyNoExperience;

    bgSounds.Serialize(ar);
  }
  else
  {
    DAS(ar,DeathSound);
    DAS(ar,MoveSound);

    if (LoadingVersion >= _VERSION_0750_)
    {
      DAS(ar, TurnUndeadSound);
    }
    else
      TurnUndeadSound = DEF_CAST_SOUND;

    if (globalData.version < 0.930283)
    {
      StripFilenamePath(DeathSound);
      StripFilenamePath(MoveSound);
      StripFilenamePath(TurnUndeadSound);
    };

    //AddFolderToPath(DeathSound,      rte.SoundDir());
    //AddFolderToPath(MoveSound,       rte.SoundDir());
    //AddFolderToPath(TurnUndeadSound, rte.SoundDir());

    int temp;
    ar >> temp;
    distance = (eventDistType)temp;   
    ar >> temp;
    direction = (eventDirType)temp;   
    ar >> temp;
    m_eSurprise = (eventSurpriseType)temp;   
    ar >> autoApproach;
    ar >> outdoors;
    ar >> noMonsterTreasure;
    ar >> partyNeverDies;
    ar >> noMagic;
    ar >> monsterMorale;   
    ar >> temp;
    turningMod = (eventTurnUndeadModType)temp;
    if (LoadingVersion >= _VERSION_0690_)
      ar >> randomMonster;
    if (LoadingVersion >= _VERSION_0860_)
      ar >> partyNoExperience;
    if (LoadingVersion >= _VERSION_0790_)
      bgSounds.Serialize(ar);
  }
  monsters.Serialize(ar);
}
void COMBAT_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    StripFilenamePath(DeathSound);
    StripFilenamePath(MoveSound);
    StripFilenamePath(TurnUndeadSound);
    AS(ar, DeathSound);
    AS(ar, MoveSound);
    AS(ar, TurnUndeadSound);

    //AddFolderToPath(DeathSound,      rte.SoundDir());
    //AddFolderToPath(MoveSound,       rte.SoundDir());
    //AddFolderToPath(TurnUndeadSound, rte.SoundDir());

    ar << (int)distance;
    ar << (int)direction;
    ar << (int)m_eSurprise;
    ar << autoApproach;
    ar << outdoors;
    ar << noMonsterTreasure;
    ar << partyNeverDies;
    ar << noMagic;
    ar << monsterMorale;
    ar << (int)turningMod;
    ar << randomMonster;
    ar << partyNoExperience;
    bgSounds.Serialize(ar);
  }
  else
  {
    DAS(ar,DeathSound);
    DAS(ar,MoveSound);

    if (LoadingVersion >= _VERSION_0750_)
    {
      DAS(ar, TurnUndeadSound);
    }
    else
      TurnUndeadSound=DEF_CAST_SOUND;

    if (globalData.version < 0.930283)
    {
      StripFilenamePath(DeathSound);
      StripFilenamePath(MoveSound);
      StripFilenamePath(TurnUndeadSound);
    };

    //AddFolderToPath(DeathSound,      rte.SoundDir());
    //AddFolderToPath(MoveSound,       rte.SoundDir());
    //AddFolderToPath(TurnUndeadSound, rte.SoundDir());

    int temp;
    ar >> temp;
    distance = (eventDistType)temp;   
    ar >> temp;
    direction = (eventDirType)temp;   
    ar >> temp;
    m_eSurprise = (eventSurpriseType)temp;   
    ar >> autoApproach;
    ar >> outdoors;
    ar >> noMonsterTreasure;
    ar >> partyNeverDies;
    ar >> noMagic;
    ar >> monsterMorale;   
    ar >> temp;
    turningMod = (eventTurnUndeadModType)temp;
    if (LoadingVersion >= _VERSION_0690_)
      ar >> randomMonster;
    if (LoadingVersion >= _VERSION_0860_)
      ar >> partyNoExperience;
    if (LoadingVersion >= _VERSION_0790_)
      bgSounds.Serialize(ar);
  }
  monsters.Serialize(ar);
}

#ifdef UAFEDITOR
//const char *JKEY_PARTICULAR = "particular";
const char *JKEY_DEATHSOUND = "deathSound";
const char *JKEY_MOVESOUND = "moveSound";
const char *JKEY_TURNUNDEADSOUND = "turnUdeadSound";
const char *JKEY_DISTANCE = "distance";
const char *JKEY_DIRECTION = "direction";
const char *JKEY_SURPRISE = "surprise";
const char *JKEY_AUTOAPPROACH = "autoApproach";
const char *JKEY_OUTDOORS = "outdoors";
const char *JKEY_MONSTERTREASURE = "monsterTreasure";
const char *JKEY_PARTYNEVERDIES = "partyNeverDies";
const char *JKEY_TURNMODIFIER = "turnModifier";
const char *JKEY_NOMAGIC = "noMagic";
const char *JKEY_RANDOMMONSTER = "randomMonster";
const char *JKEY_PARTYGAINSEXP = "partyGainsExp";
const char *JKEY_MONSTERMORALE = "monsterMorale";


void COMBAT_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndQuotedNonBlank(JKEY_DEATHSOUND,DeathSound);
  jw.NameAndQuotedNonBlank(JKEY_MOVESOUND,MoveSound);
  jw.NameAndQuotedNonBlank(JKEY_TURNUNDEADSOUND,TurnUndeadSound);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndEnum(JKEY_DIRECTION, direction);
  jw.NameAndEnum(JKEY_SURPRISE, m_eSurprise);
  jw.NameAndBool(JKEY_AUTOAPPROACH, autoApproach);
  jw.NameAndBool(JKEY_OUTDOORS, outdoors);
  jw.NameAndBool(JKEY_MONSTERTREASURE, !noMonsterTreasure);
  jw.NameAndBool(JKEY_PARTYNEVERDIES, partyNeverDies);
  jw.NameAndBool(JKEY_NOMAGIC, noMagic);
  jw.NameAndValue(JKEY_MONSTERMORALE, monsterMorale);
  jw.NameAndEnum(JKEY_TURNMODIFIER, turningMod);
  jw.NameAndBool(JKEY_RANDOMMONSTER, randomMonster);
  jw.NameAndBool(JKEY_PARTYGAINSEXP, !partyNoExperience);
  bgSounds.Export(jw);
  monsters.Export(jw);
}
bool COMBAT_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndQuotedNonBlank(JKEY_DEATHSOUND,DeathSound);
  jr.NameAndQuotedNonBlank(JKEY_MOVESOUND,MoveSound);
  jr.NameAndQuotedNonBlank(JKEY_TURNUNDEADSOUND,TurnUndeadSound);
  jr.NameAndEnum(JKEY_DISTANCE, distance);
  jr.NameAndEnum(JKEY_DIRECTION, direction);
  jr.NameAndEnum(JKEY_SURPRISE, m_eSurprise);
  jr.NameAndBool(JKEY_AUTOAPPROACH, autoApproach);
  jr.NameAndBool(JKEY_OUTDOORS, outdoors);
  BOOL monsterTreasure;
  jr.NameAndBool(JKEY_MONSTERTREASURE, monsterTreasure);
  noMonsterTreasure = !monsterTreasure;
  jr.NameAndBool(JKEY_PARTYNEVERDIES, partyNeverDies);
  jr.NameAndBool(JKEY_NOMAGIC, noMagic);
  jr.NameAndValue(JKEY_MONSTERMORALE, monsterMorale);
  jr.NameAndEnum(JKEY_TURNMODIFIER, turningMod);
  jr.NameAndBool(JKEY_RANDOMMONSTER, randomMonster);
  BOOL partyExperience;
  jr.NameAndBool(JKEY_PARTYGAINSEXP, partyExperience);
  partyNoExperience = !partyExperience;
  bgSounds.Import(jr);
  monsters.Import(jr);
  return true;
}
#endif

#ifdef UAFEDITOR
void COMBAT_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  pCREI->m_pCRList->CR_AddSoundReference(DeathSound,      &pCREI->m_CRReference);
  pCREI->m_pCRList->CR_AddSoundReference(MoveSound,       &pCREI->m_CRReference);
  pCREI->m_pCRList->CR_AddSoundReference(TurnUndeadSound, &pCREI->m_CRReference);
  bgSounds.CrossReference(pCREI->m_pCRList, &pCREI->m_CRReference);
}
#endif

void GUIDED_TOUR::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << tour_x;
    ar << tour_y;
    ar << facing;
    ar << useStartLoc;   
    ar << executeEvent;
  }
  else
  {
    ar >> tour_x;
    ar >> tour_y;
    ar >> facing;
    ar >> useStartLoc;   
    ar >> executeEvent;
  }

  for (int i=0;i<MAX_TOUR_STEPS;i++)
     steps[i].Serialize(ar);
}
void GUIDED_TOUR::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << tour_x;
    ar << tour_y;
    ar << facing;
    ar << useStartLoc;   
    ar << executeEvent;
  }
  else
  {
    ar >> tour_x;
    ar >> tour_y;
    ar >> facing;
    ar >> useStartLoc;   
    ar >> executeEvent;
  }

  for (int i=0;i<MAX_TOUR_STEPS;i++)
     steps[i].Serialize(ar);
}


#ifdef UAFEDITOR
void GUIDED_TOUR::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("GUIDED_TOUR::DetailedCrossReference not implemented");
}

const char *JKEY_USESTARTLOCATION = "useStartLocation";
const char *JKEY_EXECUTEEVENT = "executeEvent";

void GUIDED_TOUR::Export(JWriter& jw)
{
  int i[2];
  i[0] = tour_x;
  i[1] = tour_y;
  jw.NameAndValue(JKEY_XY, i, 2);
  jw.NameAndValue(JKEY_FACING, facing);
  jw.NameAndBool(JKEY_USESTARTLOCATION, useStartLoc);   
  jw.NameAndBool(JKEY_EXECUTEEVENT, executeEvent);

  jw.StartArray(JKEY_STEPS);
  for (int j=0;j<MAX_TOUR_STEPS;j++)
  {
    steps[j].Export(jw, j);
  };
  jw.EndArray();
}
bool GUIDED_TOUR::Import(JReader& jr)
{
  int i[2];
  jr.NameAndValue(JKEY_XY, i, 2);
  tour_x = i[0];
  tour_y = i[1];
  jr.NameAndValue(JKEY_FACING, facing);
  jr.NameAndBool(JKEY_USESTARTLOCATION, useStartLoc);   
  jr.NameAndBool(JKEY_EXECUTEEVENT, executeEvent);
  jr.StartArray(JKEY_STEPS);
  {
    int k;
    TOUR_STEP step;
    while(jr.Optional(), jr.NextEntry())
    {
      k = step.Import(jr);
      if (k < MAX_TOUR_STEPS)
      steps[k] = step;
    };
  };
  jr.EndArray();
  return true;
}
#endif


void QUESTION_YES_NO::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {  
    temp = YesChainAction;
    ar << temp;
    temp = NoChainAction;
    ar << temp;
    ar << YesChain;
    ar << NoChain;
  }
  else
  {
    ar >> temp;
    YesChainAction = (labelPostChainOptionsType)temp;
    ar >> temp;
    NoChainAction = (labelPostChainOptionsType)temp;
    ar >> YesChain;
    ar >> NoChain;
  }
}
void QUESTION_YES_NO::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {  
    temp = YesChainAction;
    ar << temp;
    temp = NoChainAction;
    ar << temp;
    ar << YesChain;
    ar << NoChain;
  }
  else
  {
    ar >> temp;
    YesChainAction = (labelPostChainOptionsType)temp;
    ar >> temp;
    NoChainAction = (labelPostChainOptionsType)temp;
    ar >> YesChain;
    ar >> NoChain;
  }
}

#ifdef UAFEDITOR
void QUESTION_YES_NO::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
//  CrossReferenceEventChain(pCREI, YesChain);
//  CrossReferenceEventChain(pCREI, NoChain);
}

const char *JKEY_YESCHAINACTION = "yesChainAction";
const char *JKEY_NOCHAINACTION = "noChainAction";
const char *JKEY_YESCHAIN = "yesChain";
const char *JKEY_NOCHAIN = "noChain";
void QUESTION_YES_NO::Export(JWriter& jw)
{
    jw.NameAndEnum(JKEY_YESCHAINACTION, YesChainAction);
    jw.NameAndNonZero(JKEY_YESCHAIN, YesChain);
    jw.NameAndEnum(JKEY_NOCHAINACTION, NoChainAction);
    jw.NameAndNonZero(JKEY_NOCHAIN, NoChain);
}
bool QUESTION_YES_NO::Import(JReader& jr)
{
    jr.NameAndEnum(JKEY_YESCHAINACTION, YesChainAction);
    jr.NameAndNonZero(JKEY_YESCHAIN, YesChain);
    jr.NameAndEnum(JKEY_NOCHAINACTION, NoChainAction);
    jr.NameAndNonZero(JKEY_NOCHAIN, NoChain);
    return true;
}
#endif




void ENCOUNTER_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << monsterSpeed;
    temp = zeroRangeResult;
    ar << temp;
    ar << combatChain;
    ar << talkChain;
    ar << escapeChain;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> monsterSpeed;   
    ar >> temp;
    zeroRangeResult = (encounterButtonResultType)temp;
    ar >> combatChain;
    ar >> talkChain;
    ar >> escapeChain;
  }

  buttons.Serialize(ar);
}
void ENCOUNTER_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << monsterSpeed;
    temp = zeroRangeResult;
    ar << temp;
    ar << combatChain;
    ar << talkChain;
    ar << escapeChain;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> monsterSpeed;   
    ar >> temp;
    zeroRangeResult = (encounterButtonResultType)temp;
    ar >> combatChain;
    ar >> talkChain;
    ar >> escapeChain;
  }

  buttons.Serialize(ar);
}

#ifdef UAFEDITOR
void ENCOUNTER_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  //pCREI->m_pCRList->CR_AddEventReference(combatChain, &pCREI->m_CRReference);
  //pCREI->m_pCRList->CR_AddEventReference(talkChain, &pCREI->m_CRReference);
  //pCREI->m_pCRList->CR_AddEventReference(escapeChain, &pCREI->m_CRReference);
}

//const char *JKEY_DISTANCE = "distance";
const char *JKEY_MONSTERSPEED = "monsterSpeed";
const char *JKEY_ZERORANGERESULT = "zeroRangeResult";
const char *JKEY_COMBATCHAIN = "combatChain";
const char *JKEY_TALKCHAIN = "talkChain";
const char *JKEY_ESCAPECHAIN = "escapeChain";

void ENCOUNTER_DATA::Export(JWriter& jw)
{
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndValue(JKEY_MONSTERSPEED, monsterSpeed);
  jw.NameAndValue(JKEY_ZERORANGERESULT, zeroRangeResult);
  jw.NameAndValue(JKEY_COMBATCHAIN, combatChain);
  jw.NameAndValue(JKEY_TALKCHAIN, talkChain);
  jw.NameAndValue(JKEY_ESCAPECHAIN, escapeChain);
  buttons.Export(jw);
}
bool ENCOUNTER_DATA::Import(JReader& jr)
{
  jr.NameAndEnum(JKEY_DISTANCE, distance);
  jr.NameAndValue(JKEY_MONSTERSPEED, monsterSpeed);
  jr.NameAndEnum(JKEY_ZERORANGERESULT, zeroRangeResult);
  jr.NameAndValue(JKEY_COMBATCHAIN, combatChain);
  jr.NameAndValue(JKEY_TALKCHAIN, talkChain);
  jr.NameAndValue(JKEY_ESCAPECHAIN, escapeChain);
  buttons.Import(jr);
  return true;
}
#endif



BOOL PASSWORD_DATA::PasswordMatches(CString typed)
{
  BOOL success = FALSE;
  switch (matchCriteria)
  {
  case PswdInTyped:
    if (matchCase)
    {
      success = (strstr(typed, password) != NULL);
    }
    else
    {
      CString p = password;
      p.MakeLower();
      typed.MakeLower();
      success = (strstr(typed, p) != NULL);
    }
    break;
  case TypedInPswd:
    if (matchCase)
    {
      success = (strstr(password, typed) != NULL);
    }
    else
    {
      CString p = password;
      p.MakeLower();
      typed.MakeLower();
      success = (strstr(p, typed) != NULL);
    }
    break;
  default: // should be ExactMatch
    if (matchCase)
      success = (strcmp(typed, password) == 0);
    else
      success = (stricmp(typed, password) == 0);
    break;
  }
  return success;
}

void PASSWORD_DATA::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    StoreIntAsASL(event_asl, (int)matchCriteria, "MtchCri");
    StoreIntAsASL(event_asl, (int)matchCase, "MtchCse");
  }
}

void PASSWORD_DATA::PostSerialize(BOOL IsStoring, double version)
{
  GameEvent::PostSerialize(IsStoring, version);
  if (IsStoring)
  {
    event_asl.Delete("MtchCri");
    event_asl.Delete("MtchCse");
  }
  else
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    int temp;
    RetrieveIntFromASL(event_asl, temp, "MtchCri");
    event_asl.Delete("MtchCri");
    matchCriteria = (passwordMatchType)temp;
    RetrieveIntFromASL(event_asl, temp, "MtchCse");
    event_asl.Delete("MtchCse");
    matchCase = (BOOL)temp;
  }
}

void PASSWORD_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrTries;
    ar << successChain;
    ar << failChain;
    temp = successAction;
    ar << temp;
    temp = failAction;
    ar << temp;

    AS(ar,password);
//    if (password.IsEmpty())
//    {
//      password = "*";
//      ar << password;
//      password="";
//    }
//    else
//      ar << password;
  }
  else
  {
    ar >> nbrTries;
    ar >> successChain;
    ar >> failChain;
    ar >> temp;
    successAction = (passwordActionType)temp;   
    ar >> temp;
    failAction = (passwordActionType)temp;
    DAS(ar,password);
//    ar >> password;
//    if (password == "*")
//     password = "";
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}
void PASSWORD_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrTries;
    ar << successChain;
    ar << failChain;
    temp = successAction;
    ar << temp;
    temp = failAction;
    ar << temp;

    AS(ar,password);
//    if (password.IsEmpty())
//    {
//      password = "*";
//      ar << password;
//      password="";
//    }
//    else
//      ar << password;
  }
  else
  {
    ar >> nbrTries;
    ar >> successChain;
    ar >> failChain;
    ar >> temp;
    successAction = (passwordActionType)temp;   
    ar >> temp;
    failAction = (passwordActionType)temp;
    DAS(ar,password);
//    ar >> password;
//    if (password == "*")
//     password = "";
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}

#ifdef UAFEDITOR
void PASSWORD_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

const char *JKEY_NUMBEROFTRIES = "numberOfTries";
const char *JKEY_SUCCESSCHAIN = "successChain";
const char *JKEY_FAILCHAIN = "failChain";
const char *JKEY_SUCCESSACTION = "successAction";
const char *JKEY_FAILACTION = "failAction";
const char *JKEY_PASSWORD = "password";
const char *JKEY_SUCCESSDESTINATION = "successDestination";
const char *JKEY_FAILDESTINATION = "failDestination";
const char *JKEY_MATCHCASE = "matchCase";

void PASSWORD_DATA::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_PASSWORD, password);
  jw.NameAndValue(JKEY_NUMBEROFTRIES, nbrTries);
  jw.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jw.NameAndValue(JKEY_FAILCHAIN, failChain);
  jw.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  jw.NameAndEnum(JKEY_FAILACTION, failAction);
  jw.NameAndBool(JKEY_MATCHCASE, matchCase);
  successTransferData.Export(jw, JKEY_SUCCESSDESTINATION);
  failTransferData.Export(jw, JKEY_FAILDESTINATION);
}
bool PASSWORD_DATA::Import(JReader& jr)
{
  jr.NameAndValue(JKEY_PASSWORD, password);
  jr.NameAndValue(JKEY_NUMBEROFTRIES, nbrTries);
  jr.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jr.NameAndValue(JKEY_FAILCHAIN, failChain);
  jr.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  jr.NameAndEnum(JKEY_FAILACTION, failAction);
  jr.NameAndBool(JKEY_MATCHCASE, matchCase);
  successTransferData.Import(jr, JKEY_SUCCESSDESTINATION);
  failTransferData.Import(jr, JKEY_FAILDESTINATION);
  return true;
}
#endif


void GAIN_EXP_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  ::SaveSound(sound, level, rte.SoundDir()); 
}

void GAIN_EXP_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {   
    ar << experience;
    
    //StripFilenamePath(sound);
    AS(ar, sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    //AddFolderToPath(sound, rte.SoundDir());

    ar << chance;
    temp = who;
    ar << temp;
  }
  else
  {
    ar >> experience;

    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    //AddFolderToPath(sound, rte.SoundDir());
    
    ar >> chance;
    ar >> temp;
    who = (eventPartyAffectType)temp;
  }
}
void GAIN_EXP_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {   
    ar << experience;
    
    //StripFilenamePath(sound);
    AS(ar, sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    //AddFolderToPath(sound, rte.SoundDir());

    ar << chance;
    temp = who;
    ar << temp;
  }
  else
  {
    ar >> experience;

    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    //AddFolderToPath(sound, rte.SoundDir());
    
    ar >> chance;
    ar >> temp;
    who = (eventPartyAffectType)temp;
  }
}

#ifdef UAFEDITOR
void GAIN_EXP_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventSound(pCREI, sound);
}

extern const char *JKEY_SOUND;
const char *JKEY_WHO = "who";
const char *JKEY_EXPERIENCE = "experience";
void GAIN_EXP_DATA::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_EXPERIENCE, experience);
  jw.NameAndNonBlank(JKEY_SOUND, sound);
  jw.NameAndValue(JKEY_CHANCE, chance);
  jw.NameAndEnum(JKEY_WHO, who);
}
bool GAIN_EXP_DATA::Import(JReader& jr)
{
  jr.NameAndValue(JKEY_EXPERIENCE, experience);
  jr.NameAndNonBlank(JKEY_SOUND, sound);
  jr.NameAndValue(JKEY_CHANCE, chance);
  jr.NameAndEnum(JKEY_WHO, who);
  return true;
}
#endif


void COMBAT_TREASURE::Clear() 
{ 
  GameEvent::Clear();
  
  //m_detectSpellKey.Clear(); //m_detectSpellKey=0;;
#ifdef SpellDetectMagic
  m_detectSpellID.Clear();
#endif
  money.Clear();
  items.Clear(); 
}


void GIVE_TREASURE_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  SilentGiveToActiveChar=FALSE; 
  //m_detectSpellKey.Clear(); //m_detectSpellKey=0;;
#ifdef SpellDetectMagic
  m_detectSpellID.Clear();
#endif
  money.Clear();
  items.Clear(); 
  moneyRemain.Clear();
  itemsRemain.Clear();
}

void GIVE_TREASURE_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    money.Serialize(ar, LoadingVersion);
    ar << SilentGiveToActiveChar;
  }
  else
  {
    if (LoadingVersion < _VERSION_0740_)
    {
      itemClassType coinType=PlatinumType;
      if (LoadingVersion >= _VERSION_0670_)
      {
        int tmp;
        ar >> tmp; coinType=(itemClassType)tmp;
      }
      int plat,gems,jewels;
      ar >> plat;
      ar >> gems;
      ar >> jewels;
      money.Add(coinType, plat);
      for (int g=0;g<gems;g++) money.AddGem();
      for (int j=0;j<jewels;j++) money.AddGem();
    }
    else
    {
      money.Serialize(ar, LoadingVersion);
    }

    if (LoadingVersion >= _VERSION_0890_)
      ar >> SilentGiveToActiveChar;
  }

  items.Serialize(ar,LoadingVersion);
}
void GIVE_TREASURE_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    money.Serialize(ar, LoadingVersion);
    ar << SilentGiveToActiveChar;
  }
  else
  {
    if (LoadingVersion < _VERSION_0740_)
    {
      itemClassType coinType=PlatinumType;
      if (LoadingVersion >= _VERSION_0670_)
      {
        int tmp;
        ar >> tmp; coinType=(itemClassType)tmp;
      }
      int plat,gems,jewels;
      ar >> plat;
      ar >> gems;
      ar >> jewels;
      money.Add(coinType, plat);
      for (int g=0;g<gems;g++) money.AddGem();
      for (int j=0;j<jewels;j++) money.AddGem();
    }
    else
    {
      money.Serialize(ar, LoadingVersion);
    }

    if (LoadingVersion >= _VERSION_0890_)
      ar >> SilentGiveToActiveChar;
  }

  items.Serialize(ar,LoadingVersion);
}


void COMBAT_TREASURE_DATA::Serialize(CAR& car, double version)
{
  items.Serialize(car, version);
  money.Serialize(car, version);
}

void COMBAT_TREASURE::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  //if (ar.IsStoring())
  {
    money.Serialize(ar, version);
//    ar << SilentGiveToActiveChar;
  }
  //else
  //{
  //  money.Serialize(ar, LoadingVersion);
  //};
  items.Serialize(ar,version);
}

#ifdef UAFEDITOR
void GIVE_TREASURE_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventItems(pCREI, &items);
}

void COMBAT_TREASURE::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventItems(pCREI, &items);
}

const char *JKEY_SILENTGIVETOACTIVECHAR = "silentGiveToActiveChar";
void GIVE_TREASURE_DATA::Export(JWriter& jw)
{

  money.Export(jw);
  jw.NameAndBool(JKEY_SILENTGIVETOACTIVECHAR, SilentGiveToActiveChar);
  items.Export(jw, "items");
}
void COMBAT_TREASURE::Export(JWriter& jw)
{
  money.Export(jw);
  items.Export(jw, "items");
}
bool GIVE_TREASURE_DATA::Import(JReader& jr)
{

  money.Import(jr);
  jr.NameAndBool(JKEY_SILENTGIVETOACTIVECHAR, SilentGiveToActiveChar);
  items.Import(jr, "items");
  return true;
}

bool COMBAT_TREASURE::Import(JReader& jr)
{
  money.Import(jr);
  items.Import(jr, "items");
  return true;
}
#endif


void GIVE_DAMAGE_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrAttacks;
    ar << chancePerAttack;
    ar << dmgDice;
    ar << dmgDiceQty;
    ar << dmgBonus;
    ar << saveBonus;
    ar << attackTHAC0;
    temp = eventSave;
    ar << temp;
    temp = spellSave;
    ar << temp;
    temp = who;
    ar << temp;
    temp = distance;
    ar << temp;
  }
  else
  {
    ar >> nbrAttacks;
    ar >> chancePerAttack;
    ar >> dmgDice;
    ar >> dmgDiceQty;
    ar >> dmgBonus;
    ar >> saveBonus;
    ar >> attackTHAC0;   
    ar >> temp;
    eventSave = (spellSaveEffectType)temp;   
    ar >> temp;
    spellSave = (spellSaveVersusType)temp;   
    ar >> temp;
    who = (eventPartyAffectType)temp;   
    ar >> temp;
    distance = (eventDistType)temp;
  }
}
void GIVE_DAMAGE_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrAttacks;
    ar << chancePerAttack;
    ar << dmgDice;
    ar << dmgDiceQty;
    ar << dmgBonus;
    ar << saveBonus;
    ar << attackTHAC0;
    temp = eventSave;
    ar << temp;
    temp = spellSave;
    ar << temp;
    temp = who;
    ar << temp;
    temp = distance;
    ar << temp;
  }
  else
  {
    ar >> nbrAttacks;
    ar >> chancePerAttack;
    ar >> dmgDice;
    ar >> dmgDiceQty;
    ar >> dmgBonus;
    ar >> saveBonus;
    ar >> attackTHAC0;   
    ar >> temp;
    eventSave = (spellSaveEffectType)temp;   
    ar >> temp;
    spellSave = (spellSaveVersusType)temp;   
    ar >> temp;
    who = (eventPartyAffectType)temp;   
    ar >> temp;
    distance = (eventDistType)temp;
  }
}

#ifdef UAFEDITOR

extern const char *JKEY_PARTICULAR;
const char *JKEY_NUMBEROFATTACKS = "numberOfAttacks";
const char *JKEY_CHANCEPERATTACK = "chancePerAttack";
const char *JKEY_DAMAGEDICE = "damageDice";
const char *JKEY_SAVEBONUS = "saveBonus";
const char *JKEY_ATTACKTHAC0 = "attackTHAC0";
const char *JKEY_EVENTSAVE = "eventSave";
const char *JKEY_SPELLSAVE = "spellSave";
const char *JKEY_AFFECTS = "affects";
//const char *JKEY_DISTANCE = "distance";

void GIVE_DAMAGE_DATA::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_NUMBEROFATTACKS, nbrAttacks);
  jw.NameAndValue(JKEY_CHANCEPERATTACK, chancePerAttack);
  jw.NameAndDice(JKEY_DAMAGEDICE, dmgDiceQty, dmgDice, dmgBonus);
  jw.NameAndValue(JKEY_SAVEBONUS, saveBonus);
  jw.NameAndValue(JKEY_ATTACKTHAC0, attackTHAC0);
  jw.NameAndEnum(JKEY_EVENTSAVE, eventSave);
  jw.NameAndEnum(JKEY_SPELLSAVE, spellSave);
  jw.NameAndEnum(JKEY_AFFECTS, who);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
}

bool GIVE_DAMAGE_DATA::Import(JReader& jr)
{
  jr.NameAndValue(JKEY_NUMBEROFATTACKS, nbrAttacks);
  jr.NameAndValue(JKEY_CHANCEPERATTACK, chancePerAttack);
  jr.NameAndDice(JKEY_DAMAGEDICE, dmgDiceQty, dmgDice, dmgBonus);
  jr.NameAndValue(JKEY_SAVEBONUS, saveBonus);
  jr.NameAndValue(JKEY_ATTACKTHAC0, attackTHAC0);
  jr.NameAndEnum(JKEY_EVENTSAVE, eventSave);
  jr.NameAndEnum(JKEY_SPELLSAVE, spellSave);
  jr.NameAndEnum(JKEY_AFFECTS, who);
  jr.NameAndEnum(JKEY_DISTANCE, distance);
  return true;
}


void GIVE_DAMAGE_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}
#endif



void NPC_SAYS_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  ::SaveSound(sound, level, rte.SoundDir()); 
}

void NPC_SAYS_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;
  if (ar.IsStoring())
  {
    //ar << partySeesChar;
    ar << characterID;
    temp = distance;
    ar << temp;
    //StripFilenamePath(sound);
    AS(ar, sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    //AddFolderToPath(sound, rte.SoundDir());

    ar << mustHitReturn;
    ar << highlight;
  }
  else
  {
    //ar >> partySeesChar;
    ar >> characterID;
    ar >> temp;
    distance = (eventDistType)temp;
  
    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    //AddFolderToPath(sound, rte.SoundDir());

    ar >> mustHitReturn;
    ar >> highlight;
  }
}
void NPC_SAYS_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;
  if (ar.IsStoring())
  {
    //ar << partySeesChar;
    ar << characterID;
    temp = distance;
    ar << temp;
    //StripFilenamePath(sound);

    AS(ar, sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    //AddFolderToPath(sound, rte.SoundDir());

    ar << mustHitReturn;
    ar << highlight;
  }
  else
  {
#ifdef UAFEDITOR
    if (version < VersionSpellNames)
    {
      int npcid, indx;
      ar >> npcid;
      if (npcid >= 0)
      {
        indx = globalData.charData.LocatePreSpellNamesKey(npcid);
        if (indx >= 0)
          characterID = globalData.charData.PeekCharacter(indx)->CharacterID();
        else
          characterID.Empty();
      }
      else
      {
        characterID.Empty();
      };
    }
    else
#endif
    {
    //ar >> partySeesChar;
      ar >> characterID;
    };
    ar >> temp;
    distance = (eventDistType)temp;
  
    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    //AddFolderToPath(sound, rte.SoundDir());

    ar >> mustHitReturn;
    ar >> highlight;
  }
}


#ifdef UAFEDITOR
void NPC_SAYS_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventSound(pCREI, sound);
}

const char *JKEY_CHARACTERID = "characterID";
extern const char *JKEY_SOUND;
const char *JKEY_MUSTHITRETURN = "mustHitReturn";
const char *JKEY_HIGHLIGHT = "highlight";

void NPC_SAYS_DATA::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_CHARACTERID, characterID);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndNonBlank(JKEY_SOUND, sound);
  jw.NameAndBool(JKEY_MUSTHITRETURN, mustHitReturn);
  jw.NameAndBool(JKEY_HIGHLIGHT, highlight);
}
bool NPC_SAYS_DATA::Import(JReader& jr)
{
  jr.NameAndValue(JKEY_CHARACTERID, characterID);
  jr.NameAndEnum(JKEY_DISTANCE, distance);
  jr.NameAndNonBlank(JKEY_SOUND, sound);
  jr.NameAndBool(JKEY_MUSTHITRETURN, mustHitReturn);
  jr.NameAndBool(JKEY_HIGHLIGHT, highlight);
  return true;
}
#endif



void QUESTION_LIST_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  buttons.Serialize(ar);
}

void QUESTION_LIST_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  buttons.Serialize(ar);
}


#ifdef UAFEDITOR
void QUESTION_LIST_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}
void QUESTION_LIST_DATA::Export(JWriter& jw)
{
  buttons.Export(jw);
}
bool QUESTION_LIST_DATA::Import(JReader& jr)
{
  buttons.Import(jr);
  return true;
}
#endif


void QUESTION_BUTTON_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  buttons.Serialize(ar);
}

void QUESTION_BUTTON_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  buttons.Serialize(ar);
}

#ifdef UAFEDITOR
void QUESTION_BUTTON_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}
void QUESTION_BUTTON_DATA::Export(JWriter& jw)
{
  buttons.Export(jw);
}
bool QUESTION_BUTTON_DATA::Import(JReader& jr)
{
  buttons.Import(jr);
  return true;
}
#endif



void TAKE_PARTY_ITEMS_DATA::Clear()
{
  GameEvent::Clear();
  
  StoreItems = FALSE;
  mustHitReturn = TRUE;
  takeItems = TakeInventoryAction;
  takeAffects = TakeAffectsActiveChar;
  itemSelectFlags = TakeSpecifiedQty;
  platinumSelectFlags = TakeSpecifiedQty;
  gemsSelectFlags = TakeSpecifiedQty;
  jewelrySelectFlags = TakeSpecifiedQty;
  platinum = 0;
  gems = 0;
  jewelry = 0;
  itemPcnt = 0;
  moneyType = PlatinumType;
  items.Clear();
  WhichVault=0;
}

TAKE_PARTY_ITEMS_DATA& TAKE_PARTY_ITEMS_DATA::operator =(const TAKE_PARTY_ITEMS_DATA& src)
{
  if (&src == this)
    return *this;
  GameEvent::operator =(src);
  StoreItems = src.StoreItems;
  mustHitReturn = src.mustHitReturn;
  takeItems = src.takeItems;
  takeAffects = src.takeAffects;
  itemSelectFlags = src.itemSelectFlags;
  platinumSelectFlags = src.platinumSelectFlags;
  gemsSelectFlags = src.gemsSelectFlags;
  jewelrySelectFlags = src.jewelrySelectFlags;
  platinum = src.platinum;
  gems = src.gems;
  jewelry = src.jewelry;
  itemPcnt = src.itemPcnt;
  moneyType = src.moneyType;
  items = src.items;
  WhichVault=src.WhichVault;
  return *this;
}
#ifdef UAFEDITOR
bool TAKE_PARTY_ITEMS_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const TAKE_PARTY_ITEMS_DATA& src = (const TAKE_PARTY_ITEMS_DATA&)(s);
  if (StoreItems != src.StoreItems) return false;
  if (mustHitReturn != src.mustHitReturn) return false;
  if (takeItems != src.takeItems) return false;
  if (takeAffects != src.takeAffects) return false;
  if (itemSelectFlags != src.itemSelectFlags) return false;
  if (platinumSelectFlags != src.platinumSelectFlags) return false;
  if (gemsSelectFlags != src.gemsSelectFlags) return false;
  if (jewelrySelectFlags != src.jewelrySelectFlags) return false;
  if (platinum != src.platinum) return false;
  if (gems != src.gems) return false;
  if (jewelry != src.jewelry) return false;
  if (itemPcnt != src.itemPcnt) return false;
  if (moneyType != src.moneyType) return false;
  if (!(items == src.items)) return false;
  if (WhichVault != src.WhichVault) return false;
  return true;
}
#endif
void TAKE_PARTY_ITEMS_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << StoreItems;
    ar << mustHitReturn;
    ar << takeItems;
    ar << (int)takeAffects;
    ar << (int)itemSelectFlags;
    ar << (int)platinumSelectFlags;
    ar << (int)gemsSelectFlags;
    ar << (int)jewelrySelectFlags;
    ar << platinum;
    ar << gems;
    ar << jewelry;
    ar << itemPcnt;
    ar << (int)moneyType;
    ar << WhichVault;
  }
  else
  {
    int temp;
    ar >> StoreItems;
    ar >> mustHitReturn;
    ar >> takeItems;
    ar >> temp;
    takeAffects = (takeItemsAffectsType)temp;
    ar >> temp;
    itemSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    platinumSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    gemsSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    jewelrySelectFlags = (takeItemQtyType)temp;
    ar >> platinum;
    ar >> gems;
    ar >> jewelry; 
    ar >> itemPcnt;
    ar >> temp;
    moneyType = (itemClassType)temp;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }

  items.Serialize(ar,LoadingVersion);
}
void TAKE_PARTY_ITEMS_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << StoreItems;
    ar << mustHitReturn;
    ar << takeItems;
    ar << (int)takeAffects;
    ar << (int)itemSelectFlags;
    ar << (int)platinumSelectFlags;
    ar << (int)gemsSelectFlags;
    ar << (int)jewelrySelectFlags;
    ar << platinum;
    ar << gems;
    ar << jewelry;
    ar << itemPcnt;
    ar << (int)moneyType;
    ar << WhichVault;
  }
  else
  {
    int temp;
    ar >> StoreItems;
    ar >> mustHitReturn;
    ar >> takeItems;
    ar >> temp;
    takeAffects = (takeItemsAffectsType)temp;
    ar >> temp;
    itemSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    platinumSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    gemsSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    jewelrySelectFlags = (takeItemQtyType)temp;
    ar >> platinum;
    ar >> gems;
    ar >> jewelry; 
    ar >> itemPcnt;
    ar >> temp;
    moneyType = (itemClassType)temp;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }

  items.Serialize(ar,LoadingVersion);
}

#ifdef UAFEDITOR
void TAKE_PARTY_ITEMS_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventItems(pCREI, &items);
}

const char *JKEY_STOREITEMS = "putItemsInVault";
const char *JKEY_WHICHVAULT = "whichVault";
extern const char *JKEY_GEMS;
const char *JKEY_MONEYNUMBER = "money";
const char *JKEY_MONEYTYPE = "moneyType";
const char *JKEY_TAKEITEMS = "takeWhat";
const char *JKEY_JEWELRY = "jewelry";
const char *JKEY_ITEMS = "items";
const char *JKEY_TAKEEFFECTS = "takeFrom";
const char *JKEY_ITEMSQUANTITY = "itemsQuantity";
const char *JKEY_MONEYQUANTITY = "moneyQuantity";
const char *JKEY_GEMSQUANTITY = "gemsQuantity";
const char *JKEY_JEWELRYQUANTITY = "jewelryQuantity";
const char *JKEY_ITEMLIST = "itemList";
const char *JKEY_PLATINUM = "platinum";

void TAKE_PARTY_ITEMS_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_STOREITEMS, StoreItems);
  if (StoreItems)
  {
    jw.NameAndValue(JKEY_WHICHVAULT, WhichVault);
  };
  jw.NameAndBool(JKEY_MUSTHITRETURN, mustHitReturn);
  jw.NameAndFlags(JKEY_TAKEITEMS, takeItems, 4, TakeWhatText);
  jw.NameAndEnum(JKEY_TAKEEFFECTS, takeAffects);
  jw.NameAndEnum(JKEY_MONEYTYPE, moneyType);
  jw.NameAndValue(JKEY_MONEYNUMBER, platinum);
  jw.NameAndEnum(JKEY_MONEYQUANTITY, platinumSelectFlags);
  jw.NameAndValue(JKEY_JEWELRY, jewelry);
  jw.NameAndEnum(JKEY_JEWELRYQUANTITY, jewelrySelectFlags);
  jw.NameAndValue(JKEY_GEMS, gems);
  jw.NameAndEnum(JKEY_GEMSQUANTITY, gemsSelectFlags);
  jw.NameAndValue(JKEY_ITEMS, itemPcnt);
  jw.NameAndEnum(JKEY_ITEMSQUANTITY, itemSelectFlags);
  items.Export(jw, JKEY_ITEMLIST);
}
bool TAKE_PARTY_ITEMS_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_STOREITEMS, StoreItems);
  if (StoreItems)
  {
    jr.NameAndValue(JKEY_WHICHVAULT, WhichVault);
  };
  jr.NameAndBool(JKEY_MUSTHITRETURN, mustHitReturn);
  jr.NameAndFlags(JKEY_TAKEITEMS, takeItems, 4, TakeWhatText);
  jr.NameAndEnum(JKEY_TAKEEFFECTS, takeAffects);
  jr.NameAndEnum(JKEY_MONEYTYPE, moneyType);
  jr.NameAndValue(JKEY_MONEYNUMBER, platinum);
  jr.NameAndEnum(JKEY_MONEYQUANTITY, platinumSelectFlags);
  jr.NameAndValue(JKEY_JEWELRY, jewelry);
  jr.NameAndEnum(JKEY_JEWELRYQUANTITY, jewelrySelectFlags);
  jr.NameAndValue(JKEY_GEMS, gems);
  jr.NameAndEnum(JKEY_GEMSQUANTITY, gemsSelectFlags);
  jr.NameAndValue(JKEY_ITEMS, itemPcnt);
  jr.NameAndEnum(JKEY_ITEMSQUANTITY, itemSelectFlags);
  items.Import(jr, JKEY_ITEMLIST);
  return true;
}
#endif


void TEXT_EVENT_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  ::SaveSound(sound, level, rte.SoundDir()); 
}

void TEXT_EVENT_DATA::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    StoreIntAsASL(event_asl, (int)KillSound, "KSnd");
  }
}

void TEXT_EVENT_DATA::PostSerialize(BOOL IsStoring, double version)
{
  GameEvent::PostSerialize(IsStoring, version);
  if (IsStoring)
  {
    event_asl.Delete("KSnd");
  }
  else
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    int temp;
    RetrieveIntFromASL(event_asl, temp, "KSnd");
    event_asl.Delete("KSnd");
    KillSound = (BOOL)temp;
  }
}

void TEXT_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << WaitForReturn;
    ar << ForceBackup;
    ar << HighlightText;
    ar << distance;

    StripFilenamePath(sound);
    AS(ar, sound);
    //AddFolderToPath(sound, rte.SoundDir());
  }
  else
  {
    ar >> WaitForReturn;
    ar >> ForceBackup;
    ar >> HighlightText;

    if (LoadingVersion >= _VERSION_0908_)
    {
      int temp;
      ar >> temp;
      distance = (eventDistType)temp;
    }
    
    DAS(ar,sound);
    if (globalData.version < 0.930285)
    {
      StripFilenamePath(sound);
    };
    //AddFolderToPath(sound, rte.SoundDir());
  }
}
void TEXT_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << WaitForReturn;
    ar << ForceBackup;
    ar << HighlightText;
    ar << distance;

    StripFilenamePath(sound);
    AS(ar, sound);
    //AddFolderToPath(sound, rte.SoundDir());
  }
  else
  {
    ar >> WaitForReturn;
    ar >> ForceBackup;
    ar >> HighlightText;

    if (LoadingVersion >= _VERSION_0908_)
    {
      int temp;
      ar >> temp;
      distance = (eventDistType)temp;
    }
    
    DAS(ar,sound);
    if (globalData.version < 0.930285)
    {
      StripFilenamePath(sound);
    };
    //AddFolderToPath(sound, rte.SoundDir());
  }
}

#ifdef UAFEDITOR

const char *JKEY_WAITFORRETURN = "waitForReturn";
const char *JKEY_FORCEBACKUP = "forceBackup";
const char *JKEY_HIGHLIGHTTEXT = "highLightText";
extern const char *JKEY_SOUND;

void TEXT_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_WAITFORRETURN, WaitForReturn);
  jw.NameAndBool(JKEY_FORCEBACKUP, ForceBackup);
  jw.NameAndBool(JKEY_HIGHLIGHTTEXT, HighlightText);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndNonBlank(JKEY_SOUND, sound);
}

bool TEXT_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_WAITFORRETURN, WaitForReturn);
  jr.NameAndBool(JKEY_FORCEBACKUP, ForceBackup);
  jr.NameAndBool(JKEY_HIGHLIGHTTEXT, HighlightText);
  jr.NameAndEnum(JKEY_DISTANCE, distance);
  jr.NameAndNonBlank(JKEY_SOUND, sound);
  return true;
}

#endif

#ifdef UAFEDITOR
void TEXT_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventSound(pCREI, sound);
}
#endif


void TRANSFER_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << askYesNo;
    ar << transferOnYes;
    ar << destroyDrow;
    ar << activateBeforeEntry;
  }
  else
  {
    ar >> askYesNo;
    ar >> transferOnYes;
    ar >> destroyDrow;
    ar >> activateBeforeEntry;
  }

  transferData.Serialize(ar);
}
void TRANSFER_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << askYesNo;
    ar << transferOnYes;
    ar << destroyDrow;
    ar << activateBeforeEntry;
  }
  else
  {
    ar >> askYesNo;
    ar >> transferOnYes;
    ar >> destroyDrow;
    ar >> activateBeforeEntry;
  }

  transferData.Serialize(ar);
}

#ifdef UAFEDITOR
void TRANSFER_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

const char *JKEY_ASKYESNO = "askYesNo";
const char *JKEY_TRANSFER = "transfer";
const char *JKEY_DESTROYDROW = "destroyDrow";
const char *JKEY_ACTIVATEBEFOREENTRY = "activateBeforeEntry";
const char *JKEY_TRANSFERLOCATION = "transferLocation";
void TRANSFER_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_ASKYESNO, askYesNo);
  jw.NameAndBool(JKEY_TRANSFER, transferOnYes);
  jw.NameAndBool(JKEY_DESTROYDROW, destroyDrow);
  jw.NameAndBool(JKEY_ACTIVATEBEFOREENTRY, activateBeforeEntry);
  transferData.Export(jw, JKEY_TRANSFERLOCATION);
}
bool TRANSFER_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_ASKYESNO, askYesNo);
  jr.NameAndBool(JKEY_TRANSFER, transferOnYes);
  jr.NameAndBool(JKEY_DESTROYDROW, destroyDrow);
  jr.NameAndBool(JKEY_ACTIVATEBEFOREENTRY, activateBeforeEntry);
  transferData.Import(jr, JKEY_TRANSFERLOCATION);
  return true;
}
#endif



void WHO_PAYS_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << impossible;
    ar << gems;
    ar << jewels;
    ar << platinum;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
    ar << (int)moneyType;
  }
  else
  {
    int temp;
    ar >> impossible;
    ar >> gems;
    ar >> jewels;
    ar >> platinum;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;

    if (LoadingVersion >= _VERSION_0912_)
    {
      int tmp;
      ar >> tmp;
      moneyType = (itemClassType)tmp;
    }
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}
void WHO_PAYS_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << impossible;
    ar << gems;
    ar << jewels;
    ar << platinum;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
    ar << moneyType;
  }
  else
  {
    int temp;
    ar >> impossible;
    ar >> gems;
    ar >> jewels;
    ar >> platinum;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;

    if (LoadingVersion >= _VERSION_0912_)
    {
      int tmp;
      ar >> tmp;
      moneyType = (itemClassType)tmp;
    }
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}

#ifdef UAFEDITOR
void WHO_PAYS_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

const char *JKEY_POSSIBLE = "possible";
extern const char *JKEY_GEMS;
extern const char *JKEY_JEWELS;
extern const char *JKEY_PLATINUM;
extern const char *JKEY_SUCCESSCHAIN;
extern const char *JKEY_SUCCESSACTION;
extern const char *JKEY_FAILCHAIN;
extern const char *JKEY_FAILACTION;
extern const char *JKEY_MONEYTYPE;
const char *JKEY_SUCCESSTELEPORT = "successTeleport";
const char *JKEY_FAILTELEPORT = "failTeleport";

void WHO_PAYS_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_POSSIBLE, !impossible);
  jw.NameAndValue(JKEY_GEMS, gems);
  jw.NameAndValue(JKEY_JEWELS, jewels);
  jw.NameAndValue(JKEY_PLATINUM, platinum);
  jw.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jw.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  if (successAction == Teleport)
  {
    successTransferData.Export(jw, JKEY_SUCCESSTELEPORT);
  };
  jw.NameAndValue(JKEY_FAILCHAIN, failChain);
  jw.NameAndEnum(JKEY_FAILACTION, failAction);
  if (failAction == Teleport)
  {
    failTransferData.Export(jw, JKEY_FAILTELEPORT);
  };
  jw.NameAndEnum(JKEY_MONEYTYPE, moneyType);
}
bool WHO_PAYS_EVENT_DATA::Import(JReader& jr)
{
  bool possible;
  jr.NameAndBool(JKEY_POSSIBLE, possible);
  impossible = !possible;
  jr.NameAndValue(JKEY_GEMS, gems);
  jr.NameAndValue(JKEY_JEWELS, jewels);
  jr.NameAndValue(JKEY_PLATINUM, platinum);
  jr.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jr.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  successTransferData.Import(jr, JKEY_SUCCESSTELEPORT);
  jr.NameAndValue(JKEY_FAILCHAIN, failChain);
  jr.NameAndEnum(JKEY_FAILACTION, failAction);
  failTransferData.Import(jr, JKEY_FAILTELEPORT);
  jr.NameAndEnum(JKEY_MONEYTYPE, moneyType);
  return true;
}
#endif

#ifdef UAFEDITOR

void WHO_TRIES_EVENT_DATA::CheckSkill(const char *name, BOOL *check)
{
  if (!(*check)) return;
  ASLENTRY *pASL;
  pASL = event_asl.Find(name);
  if (pASL != NULL) return;
  {
    CString value;
    if (compareToDie)
    {
      value.Format("-%d", compareDie);
    }
    else
    {
      value.Format("%d", compareDie);
    };
    event_asl.Insert(name, value, ASLF_DESIGN|ASLF_READONLY);
  };
}

void WHO_TRIES_EVENT_DATA::CheckAbility(const char *name, BOOL *check)
{
  if (!(*check)) return;
  ASLENTRY *pASL;
  pASL = event_asl.Find(name);
  if (pASL != NULL) return;
  {
    CString value;
    if (compareToDie)
    {
      value.Format("-%d", compareDie);
    }
    else
    {
      value.Format("%d", compareDie);
    };
    event_asl.Insert(name, value, ASLF_DESIGN|ASLF_READONLY);
  };
}


void WHO_TRIES_EVENT_DATA::CheckOldSkills(void)
{
  CheckSkill(Skill_PickPockets,   &checkPP);
  CheckSkill(Skill_OpenLocks,     &checkOL);
  CheckSkill(Skill_FindTraps,     &checkFT);
  CheckSkill(Skill_MoveSilent,    &checkMS);
  CheckSkill(Skill_HideInShadows, &checkHS);
  CheckSkill(Skill_HearNoise,     &checkHN);
  CheckSkill(Skill_ClimbWalls,    &checkCW);
  CheckSkill(Skill_ReadLanguages, &checkRL);
}
void WHO_TRIES_EVENT_DATA::CheckOldAbilities(void)
{
  CheckAbility(Ability_Strength,    &checkSTR);
  CheckAbility(Ability_Intelligence,&checkINT);
  CheckAbility(Ability_Wisdom,      &checkWIS);
  CheckAbility(Ability_Dexterity,   &checkDEX);
  CheckAbility(Ability_Constitution,&checkCON);
  CheckAbility(Ability_Charisma,    &checkCHA);
}
#endif


void WHO_TRIES_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << alwaysSucceeds;
    ar << alwaysFails;
    ar << checkSTR;
    ar << checkINT;
    ar << checkWIS;
    ar << checkDEX;
    ar << checkCON;
    ar << checkCHA;
    ar << checkPP;
    ar << checkOL;
    ar << checkFT;
    ar << checkMS;
    ar << checkHS;
    ar << checkHN;
    ar << checkCW;
    ar << checkRL;
    ar << strBonus;
    ar << compareToDie;
    ar << compareDie;
    ar << NbrTries;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
  }
  else
  {
    int temp;
    ar >> alwaysSucceeds;
    ar >> alwaysFails;
    ar >> checkSTR;
    ar >> checkINT;
    ar >> checkWIS;
    ar >> checkDEX;
    ar >> checkCON;
    ar >> checkCHA;
    ar >> checkPP;
    ar >> checkOL;
    ar >> checkFT;
    ar >> checkMS;
    ar >> checkHS;
    ar >> checkHN;
    ar >> checkCW;
    ar >> checkRL;
    ar >> strBonus;
    ar >> compareToDie;
    ar >> compareDie;
    ar >> NbrTries;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;
#ifdef UAFEDITOR
    CheckOldSkills();
#endif
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}


void WHO_TRIES_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << alwaysSucceeds;
    ar << alwaysFails;
    ar << checkSTR;
    ar << checkINT;
    ar << checkWIS;
    ar << checkDEX;
    ar << checkCON;
    ar << checkCHA;
    ar << FALSE; //checkPP;
    ar << FALSE; //checkOL;
    ar << FALSE; //checkFT;
    ar << FALSE; //checkMS;
    ar << FALSE; //checkHS;
    ar << FALSE; //checkHN;
    ar << FALSE; //checkCW;
    ar << FALSE; //checkRL;
    ar << strBonus;
    ar << FALSE; //compareToDie;
    ar << 0;     //compareDie;
    ar << NbrTries;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
  }
  else
  {
    int temp;
    ar >> alwaysSucceeds;
    ar >> alwaysFails;
    ar >> checkSTR;
    ar >> checkINT;
    ar >> checkWIS;
    ar >> checkDEX;
    ar >> checkCON;
    ar >> checkCHA;
    ar >> checkPP;
    ar >> checkOL;
    ar >> checkFT;
    ar >> checkMS;
    ar >> checkHS;
    ar >> checkHN;
    ar >> checkCW;
    ar >> checkRL;
    ar >> strBonus;
    ar >> compareToDie;
    ar >> compareDie;
    ar >> NbrTries;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;
#ifdef UAFEDITOR
    CheckOldSkills();
#endif
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}

#ifdef UAFEDITOR
void WHO_TRIES_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

const char *JKEY_ALWAYSSUCCEEDS = "alwaysSucceeds";
const char *JKEY_ALWAYSFAILS = "alwaysFails";
const char *JKEY_CHECKSTR = "checkStr";
const char *JKEY_CHECKINT = "checkInt";
const char *JKEY_CHECKWIS = "checkWis";
const char *JKEY_CHECKDEX = "checkDex";
const char *JKEY_CHECKCON = "cedckCon";
const char *JKEY_CHECKCHA = "checkCha";
const char *JKEY_CHECKPP = "checkPP";
const char *JKEY_CHECKOL = "checkOL";
const char *JKEY_CHECKFT = "checkFT";
const char *JKEY_CHECKMS = "checkMS";
const char *JKEY_CHECKHS = "checkHS";
const char *JKEY_CHECKHN = "checkHN";
const char *JKEY_CHECKCW = "checkCW";
const char *JKEY_CHECKRL = "checkRL";
const char *JKEY_STRBONUS = "strBonus";
const char *JKEY_COMPARETODIE = "compareToDie";
const char *JKEY_DIESIDES = "dieSides";
//const char *JKEY_NUMBEROFTRIES = "numberofTries";
//const char *JKEY_SUCCESSACTION = "successAction";
//const char *JKEY_SUCCESSCHAIN = "successChain";
//const char *JKEY_FAILACTION = "failAction";
//const char *JKEY_FAILCHAIN = "failChain";
const char *JKEY_SUCCESSTRANSFER = "successTransfer";
const char *JKEY_FAILTRANSFER = "failTransfer";

void WHO_TRIES_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_ALWAYSSUCCEEDS, alwaysSucceeds);
  jw.NameAndBool(JKEY_ALWAYSFAILS, alwaysFails);
  jw.NameAndBool(JKEY_CHECKSTR, checkSTR);
  jw.NameAndBool(JKEY_CHECKINT, checkINT);
  jw.NameAndBool(JKEY_CHECKWIS, checkWIS);
  jw.NameAndBool(JKEY_CHECKDEX, checkDEX);
  jw.NameAndBool(JKEY_CHECKCON, checkCON);
  jw.NameAndBool(JKEY_CHECKCHA, checkCHA);
  jw.NameAndBool(JKEY_CHECKPP, checkPP);
  jw.NameAndBool(JKEY_CHECKOL, checkOL);
  jw.NameAndBool(JKEY_CHECKFT, checkFT);
  jw.NameAndBool(JKEY_CHECKMS, checkMS);
  jw.NameAndBool(JKEY_CHECKHS, checkHS);
  jw.NameAndBool(JKEY_CHECKHN, checkHN);
  jw.NameAndBool(JKEY_CHECKCW, checkCW);
  jw.NameAndBool(JKEY_CHECKRL, checkRL);
  jw.NameAndValue(JKEY_STRBONUS, strBonus);
  jw.NameAndBool(JKEY_COMPARETODIE, compareToDie);
  jw.NameAndValue(JKEY_DIESIDES, compareDie);
  jw.NameAndValue(JKEY_NUMBEROFTRIES, NbrTries);
  jw.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  jw.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jw.NameAndEnum(JKEY_FAILACTION, failAction);
  jw.NameAndValue(JKEY_FAILCHAIN, failChain);
  successTransferData.Export(jw, JKEY_SUCCESSTRANSFER);
  failTransferData.Export(jw, JKEY_FAILTRANSFER);
}
bool WHO_TRIES_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_ALWAYSSUCCEEDS, alwaysSucceeds);
  jr.NameAndBool(JKEY_ALWAYSFAILS, alwaysFails);
  jr.NameAndBool(JKEY_CHECKSTR, checkSTR);
  jr.NameAndBool(JKEY_CHECKINT, checkINT);
  jr.NameAndBool(JKEY_CHECKWIS, checkWIS);
  jr.NameAndBool(JKEY_CHECKDEX, checkDEX);
  jr.NameAndBool(JKEY_CHECKCON, checkCON);
  jr.NameAndBool(JKEY_CHECKCHA, checkCHA);
  jr.NameAndBool(JKEY_CHECKPP, checkPP);
  jr.NameAndBool(JKEY_CHECKOL, checkOL);
  jr.NameAndBool(JKEY_CHECKFT, checkFT);
  jr.NameAndBool(JKEY_CHECKMS, checkMS);
  jr.NameAndBool(JKEY_CHECKHS, checkHS);
  jr.NameAndBool(JKEY_CHECKHN, checkHN);
  jr.NameAndBool(JKEY_CHECKCW, checkCW);
  jr.NameAndBool(JKEY_CHECKRL, checkRL);
  jr.NameAndValue(JKEY_STRBONUS, strBonus);
  jr.NameAndBool(JKEY_COMPARETODIE, compareToDie);
  jr.NameAndValue(JKEY_DIESIDES, compareDie);
  jr.NameAndValue(JKEY_NUMBEROFTRIES, NbrTries);
  jr.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  jr.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jr.NameAndEnum(JKEY_FAILACTION, failAction);
  jr.NameAndValue(JKEY_FAILCHAIN, failChain);
  successTransferData.Import(jr, JKEY_SUCCESSTRANSFER);
  failTransferData.Import(jr, JKEY_FAILTRANSFER);
  return true;
}
#endif


void SPECIAL_ITEM_KEY_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  items.Serialize(ar);
  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << WaitForReturn;
  }
  else
  {
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> forceExit;
      ar >> WaitForReturn;
    }
  }
}
void SPECIAL_ITEM_KEY_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  items.Serialize(ar);
  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << WaitForReturn;
  }
  else
  {
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> forceExit;
      ar >> WaitForReturn;
    }
  }
}

#ifdef UAFEDITOR
void SPECIAL_ITEM_KEY_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventItems(pCREI, &items);
}
void SPECIAL_ITEM_KEY_EVENT_DATA::Export(JWriter& jw)
{ 
  jw.NameAndBool(JKEY_WAITFORRETURN, WaitForReturn);
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  items.Export(jw);
}
bool SPECIAL_ITEM_KEY_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_WAITFORRETURN, WaitForReturn);
  jr.NameAndBool(JKEY_FORCEEXIT, forceExit);
  items.Import(jr);
  return true;
}
#endif


void PASS_TIME_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << Days;
    ar << Hours;
    ar << Minutes;
    ar << AllowStop;
    ar << SetTime;
    ar << PassSilent;
  }
  else
  {
    ar >> Days;
    ar >> Hours;
    ar >> Minutes;
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> AllowStop;
      ar >> SetTime;
      ar >> PassSilent;
    }
  }
}
void PASS_TIME_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << Days;
    ar << Hours;
    ar << Minutes;
    ar << AllowStop;
    ar << SetTime;
    ar << PassSilent;
  }
  else
  {
    ar >> Days;
    ar >> Hours;
    ar >> Minutes;
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> AllowStop;
      ar >> SetTime;
      ar >> PassSilent;
    }
  }
}


#ifdef UAFEDITOR
void PASS_TIME_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("PASS_TIME_EVENT_DATA::DetailedCrossReference");
}

const char *JKEY_DAYS = "days";
const char *JKEY_HOURS = "hours";
const char *JKEY_MINUTES = "minutes";
const char *JKEY_ALLOWSTOP = "allowStop";
const char *JKEY_SETTIME = "setTime";
const char *JKEY_PASSSILENT = "passSilent";
void PASS_TIME_EVENT_DATA::Export(JWriter& jw)
{

  jw.NameAndValue(JKEY_DAYS, Days);
  jw.NameAndValue(JKEY_HOURS, Hours);
  jw.NameAndValue(JKEY_MINUTES, Minutes);
  jw.NameAndBool(JKEY_ALLOWSTOP, AllowStop);
  jw.NameAndBool(JKEY_SETTIME, SetTime);
  jw.NameAndBool(JKEY_PASSSILENT, PassSilent);
}
bool PASS_TIME_EVENT_DATA::Import(JReader& jr)
{

  jr.NameAndValue(JKEY_DAYS, Days);
  jr.NameAndValue(JKEY_HOURS, Hours);
  jr.NameAndValue(JKEY_MINUTES, Minutes);
  jr.NameAndBool(JKEY_ALLOWSTOP, AllowStop);
  jr.NameAndBool(JKEY_SETTIME, SetTime);
  jr.NameAndBool(JKEY_PASSSILENT, PassSilent);
  return true;
}

#endif

void VAULT_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << ForceBackup;
    ar << WhichVault;
  }
  else
  {
    ar >> ForceBackup;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }
}
void VAULT_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << ForceBackup;
    ar << WhichVault;
  }
  else
  {
    ar >> ForceBackup;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }
}


#ifdef UAFEDITOR
void VAULT_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("VAULT_EVENT_DATA::DetailedCrossReference");
}

const char *JKEY_VAULTID = "vaultID";

void VAULT_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_FORCEBACKUP, ForceBackup);
  jw.NameAndValue(JKEY_VAULTID, WhichVault);
}
bool VAULT_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_FORCEBACKUP, ForceBackup);
  jr.NameAndValue(JKEY_VAULTID, WhichVault);
  return true;
}
#endif


void QUEST_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {
    ar << (int)accept;
    ar << completeOnAccept;
    ar << failOnRejection;
    ar << m_quest;
    ar << stage;
    ar << acceptChain;
    ar << rejectChain;
  }
  else
  {
    ar >> temp;
    accept = (QuestAcceptType)temp;
    ar >> completeOnAccept;
    ar >> failOnRejection;
    ar >> m_quest;
    ar >> stage;
    ar >> acceptChain;
    ar >> rejectChain;
  }
}
void QUEST_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {
    ar << (int)accept;
    ar << completeOnAccept;
    ar << failOnRejection;
    ar << m_quest;
    ar << stage;
    ar << acceptChain;
    ar << rejectChain;
  }
  else
  {
    ar >> temp;
    accept = (QuestAcceptType)temp;
    ar >> completeOnAccept;
    ar >> failOnRejection;
    ar >> m_quest;
    ar >> stage;
    ar >> acceptChain;
    ar >> rejectChain;
  }
}

#ifdef UAFEDITOR

const char *JKEY_ACCEPTTYPE = "acceptType";
const char *JKEY_FAILONREJECT = "failOnReject";
const char *JKEY_QUESTNUM = "questNum";
const char *JKEY_QUESTSTAGE = "questStage";
const char *JKEY_ACCEPTCHAIN = "acceptChain";
const char *JKEY_REJECTCHAIN = "rejectChain";
const char *JKEY_COMPLETEONACCEPT = "completeOnAccept";


void QUEST_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}
void QUEST_EVENT_DATA::Export(JWriter& jw)
{

  jw.NameAndEnum(JKEY_ACCEPTTYPE, accept);
  jw.NameAndBool(JKEY_FAILONREJECT, failOnRejection);
  jw.NameAndBool(JKEY_COMPLETEONACCEPT, completeOnAccept);
  jw.NameAndValue(JKEY_QUESTNUM, m_quest);
  jw.NameAndValue(JKEY_QUESTSTAGE, stage);
  jw.NameAndValue(JKEY_ACCEPTCHAIN, acceptChain);
  jw.NameAndValue(JKEY_REJECTCHAIN, rejectChain);
    //ar << rejectChain;
}
bool QUEST_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndEnum(JKEY_ACCEPTTYPE, accept);
  jr.NameAndBool(JKEY_FAILONREJECT, failOnRejection);
  jr.NameAndBool(JKEY_COMPLETEONACCEPT, completeOnAccept);
  jr.NameAndValue(JKEY_QUESTNUM, m_quest);
  jr.NameAndValue(JKEY_QUESTSTAGE, stage);
  jr.NameAndValue(JKEY_ACCEPTCHAIN, acceptChain);
  jr.NameAndValue(JKEY_REJECTCHAIN, rejectChain);
  return true;

}
#endif

void TAVERN_TALES::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << m_flags;
    int i, n;
    n = m_tales.GetSize();
    ar << n;
    for (i=0;i<n;i++)
    {
      GetTale(i)->Serialize(ar, version);
    }
    m_asl.Serialize(ar, "TAVTALE");
  }
  else
  {
    ar >> m_flags;
    {
      int i, n;
      m_tales.RemoveAll();
      ar >> n;
      for (i=0; i<n; i++)
      {
        TAVERN_TALES_TALE tale;
        tale.Serialize(ar, version);
        m_tales.Add(tale);
      };;
    };
    m_asl.Serialize(ar, "TAVTALE");
  };
}


void TAVERN::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << inflation;
    ar << barkeep;
    ar << allowFights;
    ar << allowDrinks;
    ar << fightChain;
    ar << drinkChain;
    ar << drinkPointTrigger;
    temp = (int)taleOrder;
    ar << temp;
    ar << EachTaleOnceOnly;
  }
  else
  {
    ar >> forceExit;
    ar >> inflation;
    ar >> barkeep;
    ar >> allowFights;
    ar >> allowDrinks;
    ar >> fightChain;
    ar >> drinkChain;
    ar >> drinkPointTrigger;
    ar >> temp;
    taleOrder = (taleOrderType)temp;
    if (LoadingVersion >= _VERSION_0910_)
      ar >> EachTaleOnceOnly;
  }

  int i;

  if (ar.IsStoring())
  {
    ar << MAX_TALES;
    for (i=0;i<MAX_TALES;i++)
      tales[i].Serialize(ar);
  }
  else
  {
    if (LoadingVersion < _VERSION_0910_)
    {
      //int count = 10; // old number of tales

      for (i=0;i<10;i++)
        tales[i].Serialize(ar);
    }
    else
    {
      int tmp;
      ar >> tmp;
      
      ASSERT( tmp >= 10 );
      ASSERT( tmp <= MAX_TALES );
      
      if (tmp > MAX_TALES)
        tmp = MAX_TALES;

      for (i=0;i<tmp;i++)
        tales[i].Serialize(ar);
    }
  }

  for (i=0;i<MAX_DRINKS;i++)
    drinks[i].Serialize(ar);
}
void TAVERN::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int temp;

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << inflation;
    ar << barkeep;
    ar << allowFights;
    ar << allowDrinks;
    ar << fightChain;
    ar << drinkChain;
    ar << drinkPointTrigger;
    temp = (int)taleOrder;
    ar << temp;
    ar << EachTaleOnceOnly;
  }
  else
  {
    ar >> forceExit;
    ar >> inflation;
    ar >> barkeep;
    ar >> allowFights;
    ar >> allowDrinks;
    ar >> fightChain;
    ar >> drinkChain;
    ar >> drinkPointTrigger;
    ar >> temp;
    taleOrder = (taleOrderType)temp;
    if (LoadingVersion >= _VERSION_0910_)
      ar >> EachTaleOnceOnly;
  }

  int i;
  if (ar.IsStoring())
  {
    ar << MAX_TALES;
    for (i=0;i<MAX_TALES;i++)
      tales[i].Serialize(ar);
  }
  else
  {
    if (LoadingVersion < _VERSION_0910_)
    {
      //int count = 10; // old number of tales

      for (i=0;i<10;i++)
        tales[i].Serialize(ar);
    }
    else
    {
      int tmp;
      ar >> tmp;
      
      ASSERT( tmp >= 10 );
      ASSERT( tmp <= MAX_TALES );
      
      if (tmp > MAX_TALES)
        tmp = MAX_TALES;

      for (i=0;i<tmp;i++)
        tales[i].Serialize(ar);
    }
  }
  
  for (i=0;i<MAX_DRINKS;i++)
    drinks[i].Serialize(ar);
}

#ifdef UAFEDITOR
void TAVERN::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("TAVERN::DetailedCrossReference");
}

extern const char *JKEY_FORCEEXIT;
const char *JKEY_INFLATION = "inflation";
const char *JKEY_BARKEEP = "barkeep";
const char *JKEY_ALLOWFIGHTS = "allowFights";
const char *JKEY_FIGHTCHAIN = "fightChain";
const char *JKEY_ALLOWDRINKS = "allowDrinks";
const char *JKEY_DRINKCHAIN = "drinkChain";
const char *JKEY_DRINKPOINTTRIGGER = "drinkPointTrigger";
const char *JKEY_TALEORDER = "taleOrder";
const char *JKEY_EACHTALEONCEONLY = "eachTaleOnlyOnce";
const char *JKEY_DRINKS = "drinks";

void TAVERN::Export(JWriter& jw)
{
  int i;
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndValue(JKEY_INFLATION, inflation);
  jw.NameAndValue(JKEY_BARKEEP, barkeep);
  jw.NameAndBool(JKEY_ALLOWFIGHTS, allowFights);
  jw.NameAndBool(JKEY_ALLOWDRINKS, allowDrinks);
  jw.NameAndValue(JKEY_FIGHTCHAIN, fightChain);
  jw.NameAndValue(JKEY_DRINKCHAIN, drinkChain);
  jw.NameAndValue(JKEY_DRINKPOINTTRIGGER, drinkPointTrigger);
  jw.NameAndEnum(JKEY_TALEORDER, taleOrder);
  jw.NameAndBool(JKEY_EACHTALEONCEONLY, EachTaleOnceOnly);

  jw.StartArray(JKEY_TALES);
  {
    for (i=0;i<MAX_TALES;i++)
    {
      if (tales[i].tale.IsEmpty()) continue;
      jw.NextEntry();
      tales[i].Export(jw, i);
    };
  };
  jw.EndArray();
  jw.StartArray(JKEY_DRINKS);
  {
    for (i=0;i<MAX_DRINKS;i++)
    {
      if (drinks[i].name.IsEmpty()) continue;
      jw.NextEntry();
      drinks[i].Export(jw, i);
    };
  };
  jw.EndArray();
}
bool TAVERN::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jr.NameAndValue(JKEY_INFLATION, inflation);
  jr.NameAndValue(JKEY_BARKEEP, barkeep);
  jr.NameAndBool(JKEY_ALLOWFIGHTS, allowFights);
  jr.NameAndBool(JKEY_ALLOWDRINKS, allowDrinks);
  jr.NameAndValue(JKEY_FIGHTCHAIN, fightChain);
  jr.NameAndValue(JKEY_DRINKCHAIN, drinkChain);
  jr.NameAndValue(JKEY_DRINKPOINTTRIGGER, drinkPointTrigger);
  jr.NameAndEnum(JKEY_TALEORDER, taleOrder);
  jr.NameAndBool(JKEY_EACHTALEONCEONLY, EachTaleOnceOnly);
  
  {
    TALE tale;
    if (jr.Optional(), jr.StartArray(JKEY_TALES))
    {
      while (jr.Optional(), jr.NextEntry())
      {
        int index;
        index = tale.Import(jr);
        tales[index] = tale;
      };
      jr.EndArray();
    };
  };
  {
    DRINK drink;
    if (jr.Optional(), jr.StartArray(JKEY_DRINKS))
    {
      while (jr.Optional(), jr.NextEntry())
      {
        int index;
        index = drink.Import(jr);
        if (index < MAX_DRINKS)
        {
          drinks[index] = drink;
        };
      };
      jr.EndArray();
    };
  };
  return true;
}


extern const char *JKEY_FLAGS;

const char *JKEY_TALE = "tale";

extern const char *TavernTalesFlagsText[];
extern const int NumTavernTalesFlags;


void TAVERN_TALES_TALE::Export(JWriter& jw)
{
  jw.StartList();
  jw.NameAndFlags(JKEY_FLAGS, m_flags, NumTavernTalesFlags, TavernTalesFlagsText);
  jw.NameAndValue(JKEY_TALE, m_tale);
  m_asl.Export(jw);
  jw.EndList();
}

void TAVERN_TALES_TALE::Import(JReader& jr)
{
  jr.StartList();
  jr.NameAndFlags(JKEY_FLAGS, m_flags, NumTavernTalesFlags, TavernTalesFlagsText);
  jr.NameAndValue(JKEY_TALE, m_tale);
  m_asl.Import(jr);
  jr.EndList();
}

void TAVERN_TALES::Export(JWriter& jw)
{
  jw.NameAndFlags(JKEY_FLAGS, m_flags, NumTavernTalesFlags, TavernTalesFlagsText);

  jw.StartArray(JKEY_TALES);
  {
    int i, n;
    n = m_tales.GetSize();
    for (i=0; i<n; i++)
    {
      jw.NextEntry();
      GetTale(i)->Export(jw);
    };
  };
  jw.EndArray();
  m_asl.Export(jw);
}

bool TAVERN_TALES::Import(JReader& jr)
{
  jr.NameAndFlags(JKEY_FLAGS, m_flags, NumTavernTalesFlags, TavernTalesFlagsText);

  jr.StartArray(JKEY_TALES);
  {
    while (jr.Optional(),jr.NextEntry())
    {
      TAVERN_TALES_TALE tale;
      tale.Import(jr);
      m_tales.Add(tale);
    };
  };
  jr.EndArray();
  m_asl.Import(jr);
  return true;
}



#endif


TEMPLE::TEMPLE(void) :  
   GameEvent(), 
   donationChain(0),
   templeSpells(*(new spellBookType)),
   //tcKey(-1)
   characterID()
{
  Clear(); 
  event=TempleEvent;

  templeSpells = globalData.fixSpellBook;
}

TEMPLE::TEMPLE(const TEMPLE &src) : 
    GameEvent(src), 
    donationChain(0), 
    templeSpells(*(new spellBookType)),
    //tcKey(-1)
    characterID()
{ 
  *this=src; 
}

TEMPLE::~TEMPLE(void) 
{ 
  //Clear(); 
  delete (&templeSpells);
}

void TEMPLE::Clear()
{ 
  GameEvent::Clear();
  
  forceExit=TRUE;
  allowDonations=TRUE;
  costFactor=Normal;
  maxLevel=7;
  donationTrigger=0;
  donationChain=0;
  templeSpells.Clear();
  totalDonation=0; 
  m_state = -1;
}

void TEMPLE::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << allowDonations;
    ar << (int) costFactor;
    ar << maxLevel;
    ar << donationTrigger;
    ar << donationChain;

    //ar << templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar << totalDonation;
  }
  else
  {
    int temp;
    ar >> forceExit;
    ar >> allowDonations;
    ar >> temp;
    costFactor = (costFactorType)temp;
    ar >> maxLevel;
    ar >> donationTrigger;
    ar >> donationChain;

    //ar >> templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar >> totalDonation;
  }
}
void TEMPLE::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << allowDonations;
    ar << (int) costFactor;
    ar << maxLevel;
    ar << donationTrigger;
    ar << donationChain;

    //ar << templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar << totalDonation;
  }
  else
  {
    int temp;
    ar >> forceExit;
    ar >> allowDonations;
    ar >> temp;
    costFactor = (costFactorType)temp;
    ar >> maxLevel;
    ar >> donationTrigger;
    ar >> donationChain;

    //ar >> templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar >> totalDonation;
  }
}

#ifdef UAFEDITOR
void TEMPLE::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventSpells(pCREI, &(templeSpells));
}

const char *JKEY_ALLOWDONATIONS = "allowDonations";
const char *JKEY_COSTFACTOR = "costFactor";
extern const char *JKEY_MAXLEVEL;
const char *JKEY_DONATIONTRIGGER = "donationTrigger";
const char *JKEY_DONATIONCHAIN = "donationChain";
const char *JKEY_TOTALDONATION = "totalDonation";

void TEMPLE::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndBool(JKEY_ALLOWDONATIONS, allowDonations);
  jw.NameAndEnum(JKEY_COSTFACTOR, costFactor);
  jw.NameAndValue(JKEY_MAXLEVEL, maxLevel);
  jw.NameAndValue(JKEY_DONATIONTRIGGER, donationTrigger);
  jw.NameAndValue(JKEY_DONATIONCHAIN, donationChain);
  jw.NameAndValue(JKEY_TOTALDONATION, totalDonation);

  templeSpells.Export(jw);
}
bool TEMPLE::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jr.NameAndBool(JKEY_ALLOWDONATIONS, allowDonations);
  jr.NameAndEnum(JKEY_COSTFACTOR, costFactor);
  jr.NameAndValue(JKEY_MAXLEVEL, maxLevel);
  jr.NameAndValue(JKEY_DONATIONTRIGGER, donationTrigger);
  jr.NameAndValue(JKEY_DONATIONCHAIN, donationChain);
  jr.NameAndValue(JKEY_TOTALDONATION, totalDonation);

  templeSpells.Import(jr);
  return true;
 }
#endif



void RANDOM_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int i;
  if (ar.IsStoring())
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar << EventChains[i-1];
      ar << EventChance[i-1];
    }
  }
  else
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar >> EventChains[i-1];
      ar >> EventChance[i-1];
    }
  }
}

void RANDOM_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int i;
  if (ar.IsStoring())
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar << EventChains[i-1];
      ar << EventChance[i-1];
    }
  }
  else
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar >> EventChains[i-1];
      ar >> EventChance[i-1];
    }
  }
}

#ifdef UAFEDITOR
void RANDOM_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("RANDOM_EVENT_DATA::DetailedCrossReference");
}

const char *JKEY_POSSIBLECHAIN = "possibleChain";
void RANDOM_EVENT_DATA::Export(JWriter& jw)
{
  int i;
  for (i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChance[i-1] == 0) continue;
    if (EventChains[i-1] == 0) continue;
    jw.StartList(JKEY_POSSIBLECHAIN);
    jw.Linefeed(false);
    jw.NameAndValue(JKEY_CHAIN, EventChains[i-1]);
    jw.NameAndValue(JKEY_CHANCE, EventChance[i-1]);
    jw.EndList();
    jw.Linefeed(true);
  }
}
bool RANDOM_EVENT_DATA::Import(JReader& jr)
{
  int index=0;
  {
    while (jr.Optional(), jr.StartList(JKEY_POSSIBLECHAIN))
    {
      jr.Linefeed(false);
      jr.NameAndValue(JKEY_CHAIN, EventChains[index]);
      jr.NameAndValue(JKEY_CHANCE, EventChance[index]);
      jr.EndList();
      jr.Linefeed(true);
      index++;
      if (index >= MAX_RANDOM_EVENTS-1) break;
    }
  };
  return true;
}
#endif


void SMALL_TOWN_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << Unused;
    ar << TempleChain;
    ar << TrainingHallChain;
    ar << ShopChain;
    ar << InnChain;
    ar << TavernChain;
    ar << VaultChain;

  }
  else
  {
    ar >> Unused;
    ar >> TempleChain;
    ar >> TrainingHallChain;
    ar >> ShopChain;
    ar >> InnChain;
    ar >> TavernChain;
    ar >> VaultChain;
  }
}
void SMALL_TOWN_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
  {
    ar << Unused;
    ar << TempleChain;
    ar << TrainingHallChain;
    ar << ShopChain;
    ar << InnChain;
    ar << TavernChain;
    ar << VaultChain;

  }
  else
  {
    ar >> Unused;
    ar >> TempleChain;
    ar >> TrainingHallChain;
    ar >> ShopChain;
    ar >> InnChain;
    ar >> TavernChain;
    ar >> VaultChain;
  }
}

#ifdef UAFEDITOR
void SMALL_TOWN_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("SMALL_TOWN_DATA::DetailedCrossReference");
}

const char *JKEY_TEMPLE = "temple";
const char *JKEY_SHOP = "shop";
const char *JKEY_INN = "inn";
const char *JKEY_TRAIN = "train";
const char *JKEY_TAVERN = "tavern";
const char *JKEY_VAULT = "vault";
const char *JKEY_CHAINS = "chains";

void SMALL_TOWN_DATA::Export(JWriter& jw)
{
  jw.StartList(JKEY_CHAINS);
  jw.Linefeed(false);
  jw.NameAndNonZero(JKEY_TEMPLE, TempleChain);
  jw.NameAndNonZero(JKEY_TRAIN, TrainingHallChain);
  jw.NameAndNonZero(JKEY_SHOP, ShopChain);
  jw.NameAndNonZero(JKEY_INN, InnChain);
  jw.NameAndNonZero(JKEY_TAVERN, TavernChain);
  jw.NameAndNonZero(JKEY_VAULT, VaultChain);
  jw.EndList();
  jw.Linefeed(true);
}
bool SMALL_TOWN_DATA::Import(JReader& jr)
{
  if (jr.Optional(), !jr.StartList(JKEY_CHAINS)) return false;
  jr.Linefeed(false);
  jr.NameAndNonZero(JKEY_TEMPLE, TempleChain);
  jr.NameAndNonZero(JKEY_TRAIN, TrainingHallChain);
  jr.NameAndNonZero(JKEY_SHOP, ShopChain);
  jr.NameAndNonZero(JKEY_INN, InnChain);
  jr.NameAndNonZero(JKEY_TAVERN, TavernChain);
  jr.NameAndNonZero(JKEY_VAULT, VaultChain);
  jr.EndList();
  jr.Linefeed(true);
  return true;
}
#endif

#ifdef UAFEngine
BOOL JOURNAL_EVENT::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;
  return TRUE;
}
#endif
void JOURNAL_EVENT::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
    ar << journal_entry;
  else
    ar >> journal_entry;
}
void JOURNAL_EVENT::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
    ar << journal_entry;
  else
    ar >> journal_entry;
}

#ifdef UAFEDITOR
void JOURNAL_EVENT::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

const char *JKEY_JOURNALENTRY = "journalEntry";
void JOURNAL_EVENT::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_JOURNALENTRY, journal_entry);
}
bool JOURNAL_EVENT::Import(JReader& jr)
{
  jr.NameAndValue(JKEY_JOURNALENTRY, journal_entry);
  return true;
}
#endif

void CHAIN_EVENT::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
    ar << Chain;
  else
    ar >> Chain;
}
void CHAIN_EVENT::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  if (ar.IsStoring())
    ar << Chain;
  else
    ar >> Chain;
}

#ifdef UAFEDITOR
void CHAIN_EVENT::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

//const char *JKEY_CHAIN = "chain";
void CHAIN_EVENT::Export(JWriter& jw)
{
  jw.NameAndValue(JKEY_CHAIN, Chain);
}
bool CHAIN_EVENT::Import(JReader& jr)
{
  Chain = 0; // Assume no chain
  jr.Optional();
  jr.NameAndValue(JKEY_CHAIN, Chain);
  return true;
}
#endif


void TRAININGHALL::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);


  if (ar.IsStoring())
  {
    NotImplemented(0xce9ac, false);  // Should not be using CArchive
 	  ar  << forceExit
        //<< TrainCleric
        //<< TrainFighter
        //<< TrainMagicUser
        //<< TrainThief
        //<< TrainRanger
        //<< TrainPaladin
        //<< TrainDruid
	      << Cost;
  }
  else
  {
    NotImplemented(0xc4e5d, false);
 	  ar  >> forceExit;
    //ar  >> TrainCleric;
    //ar  >> TrainFighter;
    //ar  >> TrainMagicUser;
    //ar  >> TrainThief;
    //ar  >> TrainRanger;
    //ar  >> TrainPaladin;
    //ar  >> TrainDruid;
	  ar  >> Cost;
  }
}

CAR& TRAINABLE_BASECLASS::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    car << baseclassID;
    car << minLevel;
    car << maxLevel;
    car << whateverthosenutsmightwant;
  }
  else
  {
    car >> baseclassID;
    car >> minLevel;
    car >> maxLevel;
    car >> whateverthosenutsmightwant;
  };
  return car;
}

#ifdef UAFEDITOR
bool TRAINABLE_BASECLASS::operator ==(const TRAINABLE_BASECLASS& src) const
{
  if (baseclassID != src.baseclassID) return false;
  if (minLevel != src.minLevel) return false;
  if (maxLevel != src.maxLevel) return false;
  if (whateverthosenutsmightwant != src.whateverthosenutsmightwant) return false;
  return true;
}



const char *JKEY_MINLEVEL = "minLevel";
const char *JKEY_MAXLEVEL = "maxLevel";
const char *JKEY_WHATEVER = "whatever";
//const char *JKEY_ID       = "id";

void TRAINABLE_BASECLASS::Export(JWriter& jw) const
{
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_ID, baseclassID);
  jw.NameAndValue(JKEY_MINLEVEL, minLevel);
  jw.NameAndValue(JKEY_MAXLEVEL, maxLevel);
  jw.NameAndValue(JKEY_WHATEVER, whateverthosenutsmightwant);
  jw.EndList();
  jw.Linefeed(true);
}
bool TRAINABLE_BASECLASS::Import(JReader& jr)
{
  if (jr.Optional(), !jr.NextEntry()) return false;
  jr.StartList();
  jr.Linefeed(false);
  jr.NameAndValue(JKEY_ID, baseclassID);
  jr.NameAndValue(JKEY_MINLEVEL, minLevel);
  jr.NameAndValue(JKEY_MAXLEVEL, maxLevel);
  jr.NameAndValue(JKEY_WHATEVER, whateverthosenutsmightwant);
  jr.EndList();
  jr.Linefeed(true);
  return true;
}
#endif

void TRAININGHALL::Serialize(CAR &car, double version)
{
  GameEvent::Serialize(car, version);


  if (car.IsStoring())
  {
 	  car  << forceExit;
        //<< TrainCleric
        //<< TrainFighter
        //<< TrainMagicUser
        //<< TrainThief
        //<< TrainRanger
        //<< TrainPaladin
        //<< TrainDruid
    {
      int i, n;
      n = GetTrainableBaseclassCount();
      car << n;
      for (i=0; i<n; i++)
      {
        TRAINABLE_BASECLASS *pTBC;
        pTBC = GetTrainableBaseclass(i);
        pTBC->Serialize(car);
      };
    };
	  car  << Cost;
  }
  else
  {
    trainableBaseclasses.RemoveAll();
 	  car  >> forceExit;
        //>> TrainCleric
        //>> TrainFighter
        //>> TrainMagicUser
        //>> TrainThief
        //>> TrainRanger
        //>> TrainPaladin
        //>> TrainDruid
#ifdef UAFEDITOR
    if (version > 0.9984)
#endif
    {
      int i, n;
      car >> n;
      for (i=0; i<n; i++)
      {
        TRAINABLE_BASECLASS TBC;
        TBC.Serialize(car);
        Add(TBC);
      };
    }
#ifdef UAFEDITOR
    else
    {
      TRAINABLE_BASECLASS trainableBaseclass;
      trainableBaseclass.minLevel = 1;
      trainableBaseclass.maxLevel = 40;
        //>> TrainCleric
        //>> TrainFighter
        //>> TrainMagicUser
        //>> TrainThief
        //>> TrainRanger
        //>> TrainPaladin
        //>> TrainDruid
      BOOL train;
      car >> train;
      if (train)
      {
        trainableBaseclass.baseclassID = Baseclass_cleric;
        Add(trainableBaseclass);
      };
      car >> train;
      if (train)
      {
        trainableBaseclass.baseclassID = Baseclass_fighter;
        Add(trainableBaseclass);
      };
      car >> train;
      if (train)
      {
        trainableBaseclass.baseclassID = Baseclass_magicUser;
        Add(trainableBaseclass);
      };
      car >> train;
      if (train)
      {
        trainableBaseclass.baseclassID = Baseclass_thief;
        Add(trainableBaseclass);
      };
      car >> train;
      if (train)
      {
        trainableBaseclass.baseclassID = Baseclass_ranger;
        Add(trainableBaseclass);
      };
      car >> train;
      if (train)
      {
        trainableBaseclass.baseclassID = Baseclass_paladin;
        Add(trainableBaseclass);
      };
      car >> train;
      if (train)
      {
        trainableBaseclass.baseclassID = Baseclass_druid;
        Add(trainableBaseclass);
      };
    };
#endif
	  car  >> Cost;
  }
}

#ifdef UAFEDITOR
const char *JKEY_FORCEEXIT = "forceExit";
const char *JKEY_BASECLASSES = "baseclasses";
const char *JKEY_COST = "cost";

void TRAININGHALL::Export(JWriter& jw)
{
  int i;
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndValue(JKEY_COST, Cost);
  jw.StartArray(JKEY_BASECLASSES);
  for (i=0; i<trainableBaseclasses.GetSize(); i++)
  {
    const TRAINABLE_BASECLASS *pBaseclass;
    pBaseclass = PeekTrainableBaseclass(i);
    jw.NextEntry();
    pBaseclass->Export(jw);
  };
  jw.EndArray();
}
bool TRAININGHALL::Import(JReader& jr)
{
  trainableBaseclasses.RemoveAll();
  jr.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jr.NameAndValue(JKEY_COST, Cost);
  if (!jr.StartArray(JKEY_BASECLASSES)) return false;
  {
    TRAINABLE_BASECLASS tb;
    while (tb.Import(jr))
    {
      trainableBaseclasses.Add(tb);
    };
  };
  jr.EndArray();
  return true;
}
#endif
#ifdef UAFEDITOR
void TRAININGHALL::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("TRAININGHALL::DetailedCrossReference");
}
#endif


void SHOP::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);

  // no need to store sellprice
  if (ar.IsStoring())
  {
    ar << forceExit
       << (int)costFactor
       << costToId
       << buybackPercentage;
    ar << canId;
    ar << canApprGems;
    ar << canApprJewels;
    ar << buyItemsSoldOnly;
  }
  else
  {
    int temp;
    ar >> forceExit
       >> temp;
    costFactor = (costFactorType)temp;
    if (LoadingVersion >= _VERSION_0696_)
    {
      ar >> costToId;
      ar >> buybackPercentage;
    }
    if (LoadingVersion >= _VERSION_0740_)
    {
      ar >> canId;
      ar >> canApprGems;
      ar >> canApprJewels;
    }

    if (LoadingVersion >= _VERSION_0910_)
      ar >> buyItemsSoldOnly;
  }

  itemsAvail.Serialize(ar,LoadingVersion);
}

void SHOP::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);

  // no need to store sellprice
  if (ar.IsStoring())
  {
    ar << forceExit
       << (int)costFactor
       << costToId
       << buybackPercentage;
    ar << canId;
    ar << canApprGems;
    ar << canApprJewels;
    ar << buyItemsSoldOnly;
  }
  else
  {
    int temp;
    ar >> forceExit
       >> temp;
    costFactor = (costFactorType)temp;
    if (LoadingVersion >= _VERSION_0696_)
    {
      ar >> costToId;
      ar >> buybackPercentage;
    }
    if (LoadingVersion >= _VERSION_0740_)
    {
      ar >> canId;
      ar >> canApprGems;
      ar >> canApprJewels;
    }

    if (LoadingVersion >= _VERSION_0910_)
      ar >> buyItemsSoldOnly;
  }

  itemsAvail.Serialize(ar,LoadingVersion);
}
#ifdef UAFEDITOR
extern const char *JKEY_FORCEEXIT;
extern const char *JKEY_COSTFACTOR;
const char *JKEY_COSTTOID = "costToID";
const char *JKEY_BUYBACKPERCENTAGE = "buybackPercentage";
const char *JKEY_BUYITEMSOLDONLY = "buyItemsSoldOnly";
const char *JKEY_SELLPRICE = "sellPrice";
const char *JKEY_CANID = "canID";
const char *JKEY_CANAPPRAISEGEMS = "canAppraiseGems";
const char *JKEY_CANAPPRAISEJEWELS = "canAppraiseJewels";
const char *JKEY_INVENTORY = "inventory";

void SHOP::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndEnum(JKEY_COSTFACTOR, costFactor);
  jw.NameAndValue(JKEY_COSTTOID, costToId);
  jw.NameAndValue(JKEY_BUYBACKPERCENTAGE, buybackPercentage);
  jw.NameAndBool(JKEY_BUYITEMSOLDONLY, buyItemsSoldOnly);
  jw.NameAndValue(JKEY_SELLPRICE, sellprice);
  jw.NameAndBool(JKEY_CANID, canId);
  jw.NameAndBool(JKEY_CANAPPRAISEGEMS, canApprGems);
  jw.NameAndBool(JKEY_CANAPPRAISEJEWELS, canApprJewels);
  itemsAvail.Export(jw, JKEY_INVENTORY);
}
bool SHOP::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jr.NameAndEnum(JKEY_COSTFACTOR, costFactor);
  jr.NameAndValue(JKEY_COSTTOID, costToId);
  jr.NameAndValue(JKEY_BUYBACKPERCENTAGE, buybackPercentage);
  jr.NameAndBool(JKEY_BUYITEMSOLDONLY, buyItemsSoldOnly);
  jr.NameAndValue(JKEY_SELLPRICE, sellprice);
  jr.NameAndBool(JKEY_CANID, canId);
  jr.NameAndBool(JKEY_CANAPPRAISEGEMS, canApprGems);
  jr.NameAndBool(JKEY_CANAPPRAISEJEWELS, canApprJewels);
  itemsAvail.Import(jr, JKEY_INVENTORY);
  return true;
}
#endif

#ifdef UAFEDITOR
void SHOP::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  MsgBoxInfo("SHOP::DetailedCrossReference");
}
#endif

void UTILITIES_EVENT_DATA::Clear() 
{
  GameEvent::Clear();
  
  items.Clear();
  endPlay=FALSE;operation=MO_NoOperation;itemCheck=MIC_None;mathItemType=0;resultItemType=0;
  mathAmount=0;mathItemIndex=-1;resultItemIndex=-1;
}
void UTILITIES_EVENT_DATA::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    ar << endPlay;
    ar << (int)operation;
    ar << (int)itemCheck;
    ar << mathItemType;
    ar << resultItemType;
    ar << mathAmount;
    ar << mathItemIndex;
    ar << resultItemIndex;
  }
  else
  {
    int temp;
    ar >> endPlay;
    ar >> temp;
    operation = (MathOperationType)temp;
    ar >> temp;
    itemCheck = (MultiItemCheckType)temp;
    ar >> mathItemType;
    ar >> resultItemType;
    ar >> mathAmount;
    ar >> mathItemIndex;
    ar >> resultItemIndex;
  }
  items.Serialize(ar);
}
void UTILITIES_EVENT_DATA::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    ar << endPlay;
    ar << (int)operation;
    ar << (int)itemCheck;
    ar << mathItemType;
    ar << resultItemType;
    ar << mathAmount;
    ar << mathItemIndex;
    ar << resultItemIndex;
  }
  else
  {
    int temp;
    ar >> endPlay;
    ar >> temp;
    operation = (MathOperationType)temp;
    ar >> temp;
    itemCheck = (MultiItemCheckType)temp;
    ar >> mathItemType;
    ar >> resultItemType;
    ar >> mathAmount;
    ar >> mathItemIndex;
    ar >> resultItemIndex;
  }
  items.Serialize(ar);
}

#ifdef UAFEDITOR
void UTILITIES_EVENT_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  CrossReferenceEventItems(pCREI, &items);
}

const char *JKEY_ENDPLAY = "endPlay";
//const char *JKEY_OPERATION = "operation";
const char *JKEY_ITEMCHECK = "itemCheck";
const char *JKEY_MATHITEMTYPE = "mathItemType";
const char *JKEY_RESULTITEMTYPE = "resultItemType";
const char *JKEY_MATHAMOUNT = "mathAmount";
const char *JKEY_MATHITEMINDEX = "mathItemIndex";
const char *JKEY_RESULTITEMINDEX = "resultItemIndex";

void UTILITIES_EVENT_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_ENDPLAY, endPlay);
  jw.NameAndEnum(JKEY_OPERATION, operation);
  jw.NameAndEnum(JKEY_ITEMCHECK, itemCheck);
  jw.NameAndValue(JKEY_MATHAMOUNT, mathAmount);
  jw.NameAndFlags(JKEY_MATHITEMTYPE, mathItemType, 3, QuestTypeText);
  jw.NameAndValue(JKEY_MATHITEMINDEX, mathItemIndex);
  jw.NameAndFlags(JKEY_RESULTITEMTYPE, resultItemType, 3, QuestTypeText);
  jw.NameAndValue(JKEY_RESULTITEMINDEX, resultItemIndex);
  items.Export(jw);
}
bool UTILITIES_EVENT_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_ENDPLAY, endPlay);
  jr.NameAndEnum(JKEY_OPERATION, operation);
  jr.NameAndEnum(JKEY_ITEMCHECK, itemCheck);
  jr.NameAndValue(JKEY_MATHAMOUNT, mathAmount);
  jr.NameAndFlags(JKEY_MATHITEMTYPE, mathItemType, 3, QuestTypeText);
  jr.NameAndValue(JKEY_MATHITEMINDEX, mathItemIndex);
  jr.NameAndFlags(JKEY_RESULTITEMTYPE, resultItemType, 3, QuestTypeText);
  jr.NameAndValue(JKEY_RESULTITEMINDEX, resultItemIndex);
  items.Import(jr);
  return true;
}
#endif

#ifdef UAFEngine
BOOL NPC_SAYS_DATA::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;

  //BOOL shouldTrigger = party.isNPCinParty(partySeesChar);
  BOOL shouldTrigger = party.isNPCinParty(characterID);
  if (!shouldTrigger)
    WriteDebugString("NPC_SAYS event not triggerd, NPC not in party\n");
  return shouldTrigger;
}
#endif
#ifdef UAFEngine

BOOL ADD_NPC_DATA::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;

//BOOL shouldTrigger = ((!party.isNPCinParty(charAdded)) && (party.CanAddNPC()));
  BOOL shouldTrigger = ((!party.isNPCinParty(characterID)) && (party.CanAddNPC()));
  
  if (!shouldTrigger)
    WriteDebugString("ADD_NPC event not triggerd\n");
  return shouldTrigger;
}
#endif

#ifdef UAFEngine
BOOL REMOVE_NPC_DATA::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;

  //BOOL shouldTrigger = party.isNPCinParty(charRemoved);
  BOOL shouldTrigger = party.isNPCinParty(characterID);
  
  if (!shouldTrigger)
    WriteDebugString("REMOVE_NPC event not triggerd, NPC not in party\n");
  return shouldTrigger;
}
#endif

UTILITIES_EVENT_DATA& UTILITIES_EVENT_DATA::operator =(const UTILITIES_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(UTILITIES_EVENT_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const UTILITIES_EVENT_DATA &data = (const UTILITIES_EVENT_DATA&)(src);
  endPlay = data.endPlay;
  operation = data.operation;
  itemCheck = data.itemCheck;
  mathItemType = data.mathItemType;
  resultItemType = data.resultItemType;
  mathAmount = data.mathAmount;
  mathItemIndex = data.mathItemIndex;
  resultItemIndex = data.resultItemIndex;
  items = data.items;
  return *this;
}
#ifdef UAFEDITOR
bool UTILITIES_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const UTILITIES_EVENT_DATA &src = (const UTILITIES_EVENT_DATA&)(s);
  if (endPlay != src.endPlay) return false;
  if (operation != src.operation) return false;
  if (itemCheck != src.itemCheck) return false;
  if (mathItemType != src.mathItemType) return false;
  if (resultItemType != src.resultItemType) return false;
  if (mathAmount != src.mathAmount) return false;
  if (mathItemIndex != src.mathItemIndex) return false;
  if (resultItemIndex != src.resultItemIndex) return false;
  if (!(items == src.items)) return false;
  return true;
}
#endif

PLAY_MOVIE_DATA& PLAY_MOVIE_DATA::operator =(const PLAY_MOVIE_DATA& src)
{
  if (&src == this) return *this;

  GameEvent::operator =(src);
  const PLAY_MOVIE_DATA &data = (const PLAY_MOVIE_DATA&)(src);
  m_filename = data.m_filename;
  m_mode = data.m_mode;
  return *this;
}

#ifdef UAFEDITOR
bool PLAY_MOVIE_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const PLAY_MOVIE_DATA &src = (const PLAY_MOVIE_DATA&)(s);
  if (m_filename != src.m_filename) return false;
  if (m_mode != src.m_mode) return false;
  return true;
}

#endif

ADD_NPC_DATA& ADD_NPC_DATA::operator =(const ADD_NPC_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(ADD_NPC_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const ADD_NPC_DATA &data = (const ADD_NPC_DATA&)(src);

  distance = data.distance;
  //charAdded = data.charAdded;
  characterID = data.characterID;
  hitPointMod = data.hitPointMod;
  useOriginal=data.useOriginal;
  return *this;
}
#ifdef UAFEDITOR
bool ADD_NPC_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const ADD_NPC_DATA &src = (const ADD_NPC_DATA&)(s);
  if (distance != src.distance)return false;
  if (characterID != src.characterID)return false;
  if (hitPointMod != src.hitPointMod)return false;
  if (useOriginal != src.useOriginal)return false;
  return true;
}
#endif
REMOVE_NPC_DATA& REMOVE_NPC_DATA::operator =(const REMOVE_NPC_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(REMOVE_NPC_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const REMOVE_NPC_DATA &data = (const REMOVE_NPC_DATA&)(src);

  distance = data.distance;
  //charRemoved = data.charRemoved;
  characterID = data.characterID;
  return *this;
}
#ifdef UAFEDITOR
bool REMOVE_NPC_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const REMOVE_NPC_DATA &src = (const REMOVE_NPC_DATA&)(s);
  if (distance != src.distance) return false;
  if (characterID != src.characterID) return false;
  return true;
}

#endif

CAMP_EVENT_DATA& CAMP_EVENT_DATA::operator =(const CAMP_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(CAMP_EVENT_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const CAMP_EVENT_DATA &data = (const CAMP_EVENT_DATA&)(src);

  ForceExit = data.ForceExit;
  return *this;
}
#ifdef UAFEDITOR
bool CAMP_EVENT_DATA::operator ==(const GameEvent& s)const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const CAMP_EVENT_DATA &src = (const CAMP_EVENT_DATA&)(s);
  if (ForceExit != src.ForceExit) return false;
  return true;
}
#endif
COMBAT_EVENT_DATA& COMBAT_EVENT_DATA::operator =(const COMBAT_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(COMBAT_EVENT_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const COMBAT_EVENT_DATA &data = (const COMBAT_EVENT_DATA&)(src);

  m_UseOutdoorMap = data.m_UseOutdoorMap;
  distance = data.distance;
  direction = data.direction;
  m_eSurprise = data.m_eSurprise;
  autoApproach = data.autoApproach;
  outdoors = data.outdoors;
  noMonsterTreasure = data.noMonsterTreasure;
  partyNeverDies = data.partyNeverDies;
  partyNoExperience=data.partyNoExperience;
  noMagic = data.noMagic;
  monsterMorale = data.monsterMorale;
  turningMod = data.turningMod;
  monsters = data.monsters;
  DeathSound = data.DeathSound;
  MoveSound = data.MoveSound;
  TurnUndeadSound=data.TurnUndeadSound;
  randomMonster = data.randomMonster;
  m_eTEMPsurprise=data.m_eTEMPsurprise;
  bgSounds = data.bgSounds;
  return *this;
}
#ifdef UAFEDITOR
bool COMBAT_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const COMBAT_EVENT_DATA &src = (const COMBAT_EVENT_DATA&)(s);
  if (m_UseOutdoorMap != src.m_UseOutdoorMap) return false;
  if (distance != src.distance) return false;
  if (direction != src.direction) return false;
  if (m_eSurprise != src.m_eSurprise) return false;
  if (autoApproach != src.autoApproach) return false;
  if (outdoors != src.outdoors) return false;
  if (noMonsterTreasure != src.noMonsterTreasure) return false;
  if (partyNeverDies != src.partyNeverDies) return false;
  if (partyNoExperience != src.partyNoExperience) return false;
  if (noMagic != src.noMagic) return false;
  if (monsterMorale != src.monsterMorale) return false;
  if (turningMod != src.turningMod) return false;
  if (!(monsters == src.monsters)) return false;
  if (DeathSound != src.DeathSound) return false;
  if (MoveSound != src.MoveSound) return false;
  if (TurnUndeadSound != src.TurnUndeadSound) return false;
  if (randomMonster != src.randomMonster) return false;
  if (m_eTEMPsurprise != src.m_eTEMPsurprise) return false;
  if (!(bgSounds == src.bgSounds))return false;
  return true;
}
#endif
GUIDED_TOUR& GUIDED_TOUR::operator =(const GUIDED_TOUR& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(GUIDED_TOUR)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GUIDED_TOUR &data = (const GUIDED_TOUR&)(src);

  tour_x = data.tour_x;
  tour_y = data.tour_y;
  facing = data.facing;
  useStartLoc = data.useStartLoc;
  currStep = data.currStep;
  executeEvent = data.executeEvent;
  for (int i=0;i<MAX_TOUR_STEPS;i++)
    steps[i] = data.steps[i];
  return *this;
}

#ifdef UAFEDITOR
bool GUIDED_TOUR::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if(!Equals(s)) return false;
  const GUIDED_TOUR &src = (const GUIDED_TOUR&)(s);
  if (tour_x != src.tour_x) return false;
  if (tour_y != src.tour_y) return false;
  if (facing != src.facing) return false;
  if (useStartLoc != src.useStartLoc) return false;
  if (currStep != src.currStep) return false;
  if (executeEvent != src.executeEvent) return false;
  for (int i=0;i<MAX_TOUR_STEPS;i++)
  {
    if (!(steps[i] == src.steps[i])) return false;
  };
  return true;
}

#endif
QUESTION_YES_NO& QUESTION_YES_NO::operator =(const QUESTION_YES_NO& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUESTION_YES_NO)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUESTION_YES_NO &data = (const QUESTION_YES_NO&)(src);

  YesChainAction = data.YesChainAction;
  NoChainAction = data.NoChainAction;
  YesChain = data.YesChain;
  NoChain = data.NoChain;
  sx=data.sx;
  sy=data.sy;
  //Unused = data.Unused;
  return *this;
}
#ifdef UAFEDITOR
bool QUESTION_YES_NO::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const QUESTION_YES_NO &data = (const QUESTION_YES_NO&)(s);
  if (YesChainAction != data.YesChainAction) return false;
  if (NoChainAction != data.NoChainAction) return false;
  if (YesChain != data.YesChain) return false;
  if (NoChain != data.NoChain) return false;
  if (sx != data.sx) return false;
  if (sy != data.sy) return false;
  return true;
}
#endif
ENCOUNTER_DATA& ENCOUNTER_DATA::operator =(const ENCOUNTER_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(ENCOUNTER_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const ENCOUNTER_DATA &data = (const ENCOUNTER_DATA&)(src);

  currDist = data.currDist;
  distance = data.distance;
  monsterSpeed = data.monsterSpeed;
  zeroRangeResult = data.zeroRangeResult;
  combatChain = data.combatChain;
  talkChain = data.talkChain;
  escapeChain = data.escapeChain;
  //Unused = data.Unused;
  buttons = data.buttons;
  return *this;
}

#ifdef UAFEDITOR
bool ENCOUNTER_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const ENCOUNTER_DATA &src = (const ENCOUNTER_DATA&)(s);
  if (currDist != src.currDist) return false;
  if (distance != src.distance) return false;
  if (monsterSpeed != src.monsterSpeed) return false;
  if (zeroRangeResult != src.zeroRangeResult) return false;
  if (combatChain != src.combatChain) return false;
  if (talkChain != src.talkChain) return false;
  if (escapeChain != src.escapeChain) return false;
  if (!(buttons == src.buttons)) return false;
  return true;
}
#endif


PASSWORD_DATA& PASSWORD_DATA::operator =(const PASSWORD_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(PASSWORD_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const PASSWORD_DATA &data = (const PASSWORD_DATA&)(src);

  password = data.password;
  successAction = data.successAction;
  failAction = data.failAction;
  nbrTries = data.nbrTries;
  currTry = data.currTry;
  //Unused = data.Unused;
  successChain = data.successChain;
  failChain = data.failChain;
  successTransferData = data.successTransferData;
  failTransferData = data.failTransferData;
  matchCase = data.matchCase;
  matchCriteria = data.matchCriteria;
  return *this;
}
#ifdef UAFEDITOR
bool PASSWORD_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const PASSWORD_DATA &src = (const PASSWORD_DATA&)(s);
  if (password != src.password) return false;
  if (successAction != src.successAction) return false;
  if (failAction != src.failAction) return false;
  if (nbrTries != src.nbrTries) return false;
  if (currTry != src.currTry) return false;
  //Unused = data.Unused;
  if (successChain != src.successChain) return false;
  if (failChain != src.failChain) return false;
  if (!(successTransferData == src.successTransferData)) return false;
  if (!(failTransferData == src.failTransferData)) return false;
  if (matchCase != src.matchCase) return false;
  if (matchCriteria != src.matchCriteria) return false;
  return true;
}
#endif
GAIN_EXP_DATA& GAIN_EXP_DATA::operator =(const GAIN_EXP_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(GAIN_EXP_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GAIN_EXP_DATA &data = (const GAIN_EXP_DATA&)(src);

  experience = data.experience;
  sound = data.sound;
  chance = data.chance;
  who = data.who;
  hSound = data.hSound;
  tempExp=data.tempExp;
  return *this;
}
#ifdef UAFEDITOR
bool GAIN_EXP_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const GAIN_EXP_DATA &src = (const GAIN_EXP_DATA&)(s);
  if (experience != src.experience) return false;
  if (sound != src.sound) return false;
  if (chance != src.chance) return false;
  if (who != src.who) return false;
  if (hSound != src.hSound) return false;
  if (tempExp != src.tempExp) return false;
  return true;
}
#endif

GIVE_TREASURE_DATA& GIVE_TREASURE_DATA::operator =(const GIVE_TREASURE_DATA& src)
{
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GIVE_TREASURE_DATA &data = (const GIVE_TREASURE_DATA&)(src);

  SilentGiveToActiveChar=data.SilentGiveToActiveChar;
  money=data.money;
  items = data.items;
  moneyRemain=data.moneyRemain;
  itemsRemain=data.itemsRemain;
  //m_detectSpellKey=data.m_detectSpellKey;
#ifdef SpellDetectMagic
  m_detectSpellID=data.m_detectSpellID;
#endif
  return *this;
}

COMBAT_TREASURE& COMBAT_TREASURE::operator =(const COMBAT_TREASURE& src)
{
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const COMBAT_TREASURE &data = (const COMBAT_TREASURE&)(src);

  money=data.money;
  items = data.items;
  //m_detectSpellKey=data.m_detectSpellKey;
#ifdef SpellDetectMagic
  m_detectSpellID=data.m_detectSpellID;
#endif
  return *this;
}
#ifdef UAFEDITOR
bool GIVE_TREASURE_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const GIVE_TREASURE_DATA &src = (const GIVE_TREASURE_DATA&)(s);
  if (SilentGiveToActiveChar != src.SilentGiveToActiveChar) return false;
  if (!(money == src.money)) return false;
  if (!(items == src.items)) return false;
  if (!(moneyRemain == src.moneyRemain)) return false;
  if (!(itemsRemain == src.itemsRemain)) return false;
#ifdef SpellDetectMagic
  if (m_detectSpellID != src.m_detectSpellID) return false;
#endif
  return true;
}

bool COMBAT_TREASURE::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const COMBAT_TREASURE &src = (const COMBAT_TREASURE&)(s);
  if (!(money == src.money)) return false;
  if (!(items == src.items)) return false;
#ifdef SpellDetectMagic
  if (m_detectSpellID != src.m_detectSpellID) return false;
#endif
  return true;
}

#endif
void GIVE_DAMAGE_DATA::Clear()
{ 
  GameEvent::Clear();
  
  nbrAttacks=1;
  chancePerAttack=100;
  dmgDice=6;
  dmgDiceQty=1;
  dmgBonus=0;
  attackTHAC0=0;
  saveBonus=0; 
  eventSave=NoSave;
  spellSave=ParPoiDM;
  who=ActiveChar;
  distance=FarAway; 
}
 

GIVE_DAMAGE_DATA& GIVE_DAMAGE_DATA::operator =(const GIVE_DAMAGE_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(GIVE_DAMAGE_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GIVE_DAMAGE_DATA &data = (const GIVE_DAMAGE_DATA&)(src);

  nbrAttacks = data.nbrAttacks;
  chancePerAttack = data.chancePerAttack;
  dmgDice = data.dmgDice;
  dmgDiceQty = data.dmgDiceQty;
  dmgBonus = data.dmgBonus;
  saveBonus = data.saveBonus;
  attackTHAC0 = data.attackTHAC0;
  eventSave = data.eventSave;
  spellSave = data.spellSave;
  who = data.who;
  distance = data.distance;
  return *this;
}
#ifdef UAFEDITOR
bool GIVE_DAMAGE_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const GIVE_DAMAGE_DATA &src = (const GIVE_DAMAGE_DATA&)(s);
  if (nbrAttacks != src. nbrAttacks) return false;
  if (chancePerAttack != src. chancePerAttack) return false;
  if (dmgDice != src. dmgDice) return false;
  if (dmgDiceQty != src.dmgDiceQty) return false;
  if (dmgBonus != src.dmgBonus) return false;
  if (saveBonus != src.saveBonus) return false;
  if (attackTHAC0 != src.attackTHAC0) return false;
  if (eventSave != src.eventSave) return false;
  if (spellSave != src.spellSave) return false;
  if (who != src.who) return false;
  if (distance != src.distance) return false;
  return true;
}
#endif
NPC_SAYS_DATA& NPC_SAYS_DATA::operator =(const NPC_SAYS_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(NPC_SAYS_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const NPC_SAYS_DATA &data = (const NPC_SAYS_DATA&)(src);

  //partySeesChar = data.partySeesChar;
  characterID = data.characterID;
  distance = data.distance;
  sound = data.sound;
  mustHitReturn = data.mustHitReturn;
  highlight = data.highlight;
  hSound = data.hSound;
  return *this;
}
#ifdef UAFEDITOR
bool NPC_SAYS_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const NPC_SAYS_DATA &src = (const NPC_SAYS_DATA&)(s);
  if (characterID != src.characterID) return false;
  if (distance != src.distance) return false;
  if (sound != src.sound) return false;
  if (mustHitReturn != src.mustHitReturn) return false;
  if (highlight != src.highlight) return false;
  if (hSound != src.hSound) return false;
  return true;
}
#endif
QUESTION_LIST_DATA& QUESTION_LIST_DATA::operator =(const QUESTION_LIST_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUESTION_LIST_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUESTION_LIST_DATA &data = (const QUESTION_LIST_DATA&)(src);

  UserResult = data.UserResult;
  buttons = data.buttons;
  sx=data.sx;
  sy=data.sy;
  return *this;
}
#ifdef UAFEDITOR
bool QUESTION_LIST_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const QUESTION_LIST_DATA &src = (const QUESTION_LIST_DATA&)(s);

  if (UserResult != src.UserResult) return false;
  if (!(buttons == src.buttons)) return false;
  if (sx != src.sx) return false;
  if (sy != src.sy) return false;
  return true;
}
#endif
QUESTION_BUTTON_DATA& QUESTION_BUTTON_DATA::operator =(const QUESTION_BUTTON_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUESTION_BUTTON_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUESTION_BUTTON_DATA &data = (const QUESTION_BUTTON_DATA&)(src);

  UserResult = data.UserResult;
  buttons = data.buttons;
  sx=data.sx;
  sy=data.sy;
  return *this;
}
#ifdef UAFEDITOR
bool QUESTION_BUTTON_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const QUESTION_BUTTON_DATA &src = (const QUESTION_BUTTON_DATA&)(s);
  if (UserResult != src.UserResult) return false;
  if (!(buttons == src.buttons)) return false;
  if (sx != src.sx) return false;
  if (sy != src.sy) return false;
  return true;
}
#endif
TEXT_EVENT_DATA& TEXT_EVENT_DATA::operator =(const TEXT_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TEXT_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TEXT_EVENT_DATA &data = (const TEXT_EVENT_DATA&)(src);

  WaitForReturn = data.WaitForReturn;
  HighlightText = data.HighlightText;
  ForceBackup = data.ForceBackup;
  KillSound = data.KillSound;
  sound = data.sound;
  hSound = data.hSound;
  distance = data.distance;
  return *this;
}
#ifdef UAFEDITOR
bool TEXT_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == (GameEvent *)this) return true;
  if (!s.Equals(*this)) return false;
  const TEXT_EVENT_DATA& src = (const TEXT_EVENT_DATA&)(s);
  if (WaitForReturn != src.WaitForReturn) return false;
  if (HighlightText != src.HighlightText) return false;
  if (ForceBackup != src.ForceBackup) return false;
  if (KillSound != src.KillSound) return false;
  if (sound != src.sound) return false;
  if (hSound != src.hSound) return false;
  if (distance != src.distance) return false;
  return true;
}
#endif
TRANSFER_EVENT_DATA& TRANSFER_EVENT_DATA::operator =(const TRANSFER_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TRANSFER_EVENT_DATA &data = (const TRANSFER_EVENT_DATA&)(src);

  askYesNo = data.askYesNo;
  transferOnYes = data.transferOnYes;
  destroyDrow = data.destroyDrow;
  activateBeforeEntry = data.activateBeforeEntry;
  transferData = data.transferData;
  return *this;
}
#ifdef UAFEDITOR
bool TRANSFER_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const TRANSFER_EVENT_DATA &src = (const TRANSFER_EVENT_DATA&)(s);
  if (askYesNo != src.askYesNo) return false;
  if (transferOnYes != src.transferOnYes) return false;
  if (destroyDrow != src.destroyDrow) return false;
  if (activateBeforeEntry != src.activateBeforeEntry) return false;
  if (!(transferData == src.transferData)) return false;
  return true;
}
#endif
WHO_PAYS_EVENT_DATA& WHO_PAYS_EVENT_DATA::operator =(const WHO_PAYS_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(WHO_PAYS_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const WHO_PAYS_EVENT_DATA &data = (const WHO_PAYS_EVENT_DATA&)(src);

  impossible = data.impossible;
  successAction = data.successAction;
  failAction = data.failAction;
  gems = data.gems;
  jewels = data.jewels;
  platinum = data.platinum;
  //Unused = data.Unused;
  successChain = data.successChain;
  failChain = data.failChain;
  successTransferData = data.successTransferData;
  failTransferData = data.failTransferData;
  moneyType = data.moneyType;
  return *this;
}
#ifdef UAFEDITOR
bool WHO_PAYS_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  const WHO_PAYS_EVENT_DATA &src = (const WHO_PAYS_EVENT_DATA&)(s);
  if (impossible != src.impossible) return false;
  if (successAction != src.successAction) return false;
  if (failAction != src.failAction) return false;
  if (gems != src.gems) return false;
  if (jewels != src.jewels) return false;
  if (platinum != src.platinum) return false;
  if (successChain != src.successChain) return false;
  if (failChain != src.failChain) return false;
  if (!(successTransferData == src.successTransferData)) return false;
  if (!(failTransferData == src.failTransferData)) return false;
  if (moneyType != src.moneyType) return false;
  return true;
}
#endif

WHO_TRIES_EVENT_DATA& WHO_TRIES_EVENT_DATA::operator =(const WHO_TRIES_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(WHO_TRIES_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const WHO_TRIES_EVENT_DATA &data = (const WHO_TRIES_EVENT_DATA&)(src);

  alwaysSucceeds = data.alwaysSucceeds;
  alwaysFails = data.alwaysFails;
  checkSTR = data.checkSTR;
  checkINT = data.checkINT;
  checkWIS = data.checkWIS;
  checkDEX = data.checkDEX;
  checkCON = data.checkCON;
  checkCHA = data.checkCHA;
  checkPP = data.checkPP;
  checkOL = data.checkOL;
  checkFT = data.checkFT;
  checkMS = data.checkMS;
  checkHS = data.checkHS;
  checkHN = data.checkHN;
  checkCW = data.checkCW;
  checkRL = data.checkRL;
  strBonus = data.strBonus;
  compareToDie = data.compareToDie;
  successAction = data.successAction;
  failAction = data.failAction;
  compareDie = data.compareDie;
  NbrTries = data.NbrTries;
  currTry = data.currTry;
  //Unused = data.Unused;
  successChain = data.successChain;
  failChain = data.failChain;
  successTransferData = data.successTransferData;
  failTransferData = data.failTransferData;
  return *this;
}
#ifdef UAFEDITOR
bool WHO_TRIES_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  const WHO_TRIES_EVENT_DATA &src = (const WHO_TRIES_EVENT_DATA&)(s);
  if (alwaysSucceeds != src.alwaysSucceeds) return false;
  if (alwaysFails != src.alwaysFails) return false;
  if (checkSTR != src.checkSTR) return false;
  if (checkINT != src.checkINT) return false;
  if (checkWIS != src.checkWIS) return false;
  if (checkDEX != src.checkDEX) return false;
  if (checkCON != src.checkCON) return false;
  if (checkCHA != src.checkCHA) return false;
  if (checkPP != src.checkPP) return false;
  if (checkOL != src.checkOL) return false;
  if (checkFT != src.checkFT) return false;
  if (checkMS != src.checkMS) return false;
  if (checkHS != src.checkHS) return false;
  if (checkHN != src.checkHN) return false;
  if (checkCW != src.checkCW) return false;
  if (checkRL != src.checkRL) return false;
  if (strBonus != src.strBonus) return false;
  if (compareToDie != src.compareToDie) return false;
  if (successAction != src.successAction) return false;
  if (failAction != src.failAction) return false;
  if (compareDie != src.compareDie) return false;
  if (NbrTries != src.NbrTries) return false;
  if (currTry != src.currTry) return false;
  if (successChain != src.successChain) return false;
  if (failChain != src.failChain) return false;
  if (!(successTransferData == src.successTransferData)) return false;
  if (!(failTransferData == src.failTransferData)) return false;
  return true;
}
#endif
SPECIAL_ITEM_KEY_EVENT_DATA& SPECIAL_ITEM_KEY_EVENT_DATA::operator =(const SPECIAL_ITEM_KEY_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SPECIAL_ITEM_KEY_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const SPECIAL_ITEM_KEY_EVENT_DATA &data = (const SPECIAL_ITEM_KEY_EVENT_DATA&)(src);
  items = data.items;
  forceExit=data.forceExit;
  WaitForReturn=data.WaitForReturn;
  return *this;
}
#ifdef UAFEDITOR
bool SPECIAL_ITEM_KEY_EVENT_DATA::operator ==(const GameEvent& src) const
{
  /*
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SPECIAL_ITEM_KEY_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const SPECIAL_ITEM_KEY_EVENT_DATA &data = (const SPECIAL_ITEM_KEY_EVENT_DATA&)(src);
  items = data.items;
  forceExit=data.forceExit;
  WaitForReturn=data.WaitForReturn;
  */
  return false;
}
#endif
PASS_TIME_EVENT_DATA& PASS_TIME_EVENT_DATA::operator =(const PASS_TIME_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(PASS_TIME_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const PASS_TIME_EVENT_DATA &data = (const PASS_TIME_EVENT_DATA&)(src);

  Days = data.Days;
  Hours = data.Hours;
  Minutes = data.Minutes;
  AllowStop=data.AllowStop;
  SetTime=data.SetTime;
  PassSilent=data.PassSilent;
  //daysLeft = data.daysLeft;
  //hoursLeft = data.hoursLeft;
  //minutesLeft = data.minutesLeft;
  return *this;
}
#ifdef UAFEDITOR
bool PASS_TIME_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const PASS_TIME_EVENT_DATA &src = (const PASS_TIME_EVENT_DATA&)(s);
  if (Days != src.Days) return false;
  if (Hours != src.Hours) return false;
  if (Minutes != src.Minutes) return false;
  if (AllowStop != src.AllowStop) return false;
  if (SetTime != src.SetTime) return false;
  if (PassSilent != src.PassSilent) return false;
  return true;
}
#endif
VAULT_EVENT_DATA& VAULT_EVENT_DATA::operator =(const VAULT_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(VAULT_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const VAULT_EVENT_DATA &data = (const VAULT_EVENT_DATA&)(src);

  ForceBackup = data.ForceBackup;
  WhichVault = data.WhichVault;

  return *this;
}
#ifdef UAFEDITOR
bool VAULT_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const VAULT_EVENT_DATA &src = (const VAULT_EVENT_DATA&)(s);
  if (ForceBackup != src.ForceBackup) return false;
  if (WhichVault != src.WhichVault) return false;
  return true;
}
#endif
QUEST_EVENT_DATA& QUEST_EVENT_DATA::operator =(const QUEST_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUEST_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUEST_EVENT_DATA &data = (const QUEST_EVENT_DATA&)(src);
  
  accept = data.accept;
  completeOnAccept = data.completeOnAccept;
  failOnRejection = data.failOnRejection;
  m_quest = data.m_quest;
  stage = data.stage;
  acceptChain = data.acceptChain;
  rejectChain = data.rejectChain;
  //Unused = data.Unused;
  return *this;
}

#ifdef UAFEDITOR
bool QUEST_EVENT_DATA::operator ==(const GameEvent& src) const
{
  /*
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUEST_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUEST_EVENT_DATA &data = (const QUEST_EVENT_DATA&)(src);
  
  accept = data.accept;
  completeOnAccept = data.completeOnAccept;
  failOnRejection = data.failOnRejection;
  m_quest = data.m_quest;
  stage = data.stage;
  acceptChain = data.acceptChain;
  rejectChain = data.rejectChain;
  //Unused = data.Unused;
  */
  return false;
}

#endif

TAVERN& TAVERN::operator =(const TAVERN& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TAVERN)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TAVERN &data = (const TAVERN&)(src);

  forceExit = data.forceExit;
  allowFights = data.allowFights;
  allowDrinks = data.allowDrinks;
  inflation = data.inflation;
  barkeep = data.barkeep;
  fightChain = data.fightChain;
  drinkChain = data.drinkChain;
  drinkPointTrigger = data.drinkPointTrigger;
  //Unused = data.Unused;
  taleOrder = data.taleOrder;
  EachTaleOnceOnly=data.EachTaleOnceOnly;
  int i;
  for (i=0;i<MAX_TALES;i++)
    tales[i] = data.tales[i];
  for (i=0;i<MAX_DRINKS;i++)
    drinks[i] = data.drinks[i];
  return *this;
}


void TAVERN_TALES_TALE::operator =(const TAVERN_TALES_TALE& src)
{
  m_tale = src.m_tale;
  m_flags = src.m_flags;
  m_asl.Clear();
  m_asl.Copy(src.m_asl);
}

bool TAVERN_TALES_TALE::operator ==(const TAVERN_TALES_TALE& src) const
{
  if (m_tale != src.m_tale) return false;
  if (m_flags != src.m_flags) return false;
  if (!(m_asl == src.m_asl)) return false;
  return true;
}

void TAVERN_TALES_TALE::Serialize(CAR& car, double version)
{
  if (car.IsStoring())
  {
    car << m_tale;
    car << m_flags;
    m_asl.Serialize(car, "TALE");
  }
  else
  {
    car >> m_tale;
    car >> m_flags;
    m_asl.Serialize(car, "TALE");
  };
}

void TAVERN_TALES::operator =(const TAVERN_TALES& src)
{
  m_tales.RemoveAll();
  m_tales.Append(src.m_tales);
  m_flags = src.m_flags;
  m_asl.Copy(src.m_asl);
}

#ifdef UAFEDITOR
bool TAVERN_TALES::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const TAVERN_TALES &src = (const TAVERN_TALES&)(s);
  if (m_flags != src.m_flags) return false;
  if (!(m_asl == src.m_asl))   return false;
  {
    int i, n;
    n = m_tales.GetSize();
    if (n != src.m_tales.GetSize()) return false;
    for (i=0; i<n; i++)
    {
      if (!(*PeekTale(i) == *src.PeekTale(i))) return false;
    };
  };
  return true;
}
#endif
void TAVERN_TALES::Clear(void)
{
  GameEvent::Clear();
  m_tales.RemoveAll();
  m_flags = 0;
  m_asl.Clear();
}




#ifdef UAFEDITOR
bool TAVERN::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const TAVERN &src = (const TAVERN&)(s);

  if (forceExit != src.forceExit) return false;
  if (allowFights != src.allowFights) return false;
  if (allowDrinks != src.allowDrinks) return false;
  if (inflation != src.inflation) return false;
  if (barkeep != src.barkeep) return false;
  if (fightChain != src.fightChain) return false;
  if (drinkChain != src.drinkChain) return false;
  if (drinkPointTrigger != src.drinkPointTrigger) return false;
  if (taleOrder != src.taleOrder) return false;
  if (EachTaleOnceOnly != src.EachTaleOnceOnly) return false;
  int i;
  for (i=0;i<MAX_TALES;i++)
  {
    if (!(tales[i] == src.tales[i])) return false;
  };
  for (i=0;i<MAX_DRINKS;i++)
  {
    if (!(drinks[i] == src.drinks[i])) return false;
  };
  return true;
}


#endif

TEMPLE& TEMPLE::operator =(const TEMPLE& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TEMPLE)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TEMPLE &data = (const TEMPLE&)(src);

  forceExit = data.forceExit;
  allowDonations = data.allowDonations;
  totalDonation = data.totalDonation;
  costFactor = data.costFactor;
  maxLevel = data.maxLevel;
  donationTrigger = data.donationTrigger;
  donationChain = data.donationChain;
  templeSpells = data.templeSpells;
  //tcKey=data.tcKey;
  characterID=data.characterID;
  //Unused = data.Unused;
  return *this;
}
#ifdef UAFEDITOR
bool TEMPLE::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const TEMPLE &src = (const TEMPLE&)(s);
  if (forceExit != src.forceExit) return false;
  if (allowDonations != src.allowDonations) return false;
  if (totalDonation != src.totalDonation) return false;
  if (costFactor != src.costFactor) return false;
  if (maxLevel != src.maxLevel) return false;
  if (donationTrigger != src.donationTrigger) return false;
  if (donationChain != src.donationChain) return false;
  if (!(templeSpells == src.templeSpells)) return false;
  if (characterID != src.characterID) return false;
  return true;
}
#endif
RANDOM_EVENT_DATA& RANDOM_EVENT_DATA::operator =(const RANDOM_EVENT_DATA& src)
{
  if (&src==this) return *this;
  GameEvent::operator =(src);
  const RANDOM_EVENT_DATA &data = (const RANDOM_EVENT_DATA&)(src);
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    EventChains[i-1] = data.EventChains[i-1];
    EventChance[i-1] = data.EventChance[i-1];
  }
  return *this;
}
#ifdef UAFEDITOR
bool RANDOM_EVENT_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if(!Equals(s)) return false;
  const RANDOM_EVENT_DATA &src = (const RANDOM_EVENT_DATA&)(s);
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChains[i-1] != src.EventChains[i-1]) return false;
    if (EventChance[i-1] != src.EventChance[i-1]) return false;
  }
  return true;
}
#endif
SMALL_TOWN_DATA& SMALL_TOWN_DATA::operator =(const SMALL_TOWN_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SMALL_TOWN_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const SMALL_TOWN_DATA &data = (const SMALL_TOWN_DATA&)(src);

  //Unused = data.Unused;
  TempleChain = data.TempleChain;
  TrainingHallChain = data.TrainingHallChain;
  ShopChain = data.ShopChain;
  InnChain = data.InnChain;
  TavernChain = data.TavernChain;
  VaultChain = data.VaultChain;  
  return *this;
}
#ifdef UAFEDITOR
bool SMALL_TOWN_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const SMALL_TOWN_DATA &src = (const SMALL_TOWN_DATA&)(s);
  if (TempleChain != src.TempleChain) return false;
  if (TrainingHallChain != src.TrainingHallChain) return false;
  if (ShopChain != src.ShopChain) return false;
  if (InnChain != src.InnChain) return false;
  if (TavernChain != src.TavernChain) return false;
  if (VaultChain != src.VaultChain) return false;  
  return true;
}

#endif
JOURNAL_EVENT& JOURNAL_EVENT::operator =(const JOURNAL_EVENT& src)
{
  if (&src == this) return *this;
  GameEvent::operator =(src);
  const JOURNAL_EVENT &data = (const JOURNAL_EVENT&)(src);
  journal_entry = data.journal_entry;
  return *this;
}
#ifdef UAFEDITOR
bool JOURNAL_EVENT::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const JOURNAL_EVENT &src = (const JOURNAL_EVENT&)(s);
  if (journal_entry != src.journal_entry) return false;
  return true;
}
#endif
CHAIN_EVENT& CHAIN_EVENT::operator =(const CHAIN_EVENT& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(CHAIN_EVENT)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const CHAIN_EVENT &data = (const CHAIN_EVENT&)(src);

  Chain = data.Chain;
  return *this;
}
#ifdef UAFEDITOR
bool CHAIN_EVENT::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const CHAIN_EVENT &src = (const CHAIN_EVENT&)(s);
  if (Chain != src.Chain) return false;
  return true;
}
#endif
TRAININGHALL& TRAININGHALL::operator =(const TRAININGHALL& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TRAININGHALL)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TRAININGHALL &data = (const TRAININGHALL&)(src);

  forceExit = data.forceExit;
  //TrainCleric = data.TrainCleric;
  //TrainFighter = data.TrainFighter;
  //TrainMagicUser = data.TrainMagicUser;
  //TrainThief = data.TrainThief;
  //TrainRanger = data.TrainRanger;
  //TrainPaladin = data.TrainPaladin;
  //TrainDruid = data.TrainDruid;
  trainableBaseclasses.RemoveAll();
  trainableBaseclasses.Append(data.trainableBaseclasses);
  Cost = data.Cost;
  return *this;
}
#ifdef UAFEDITOR
bool TRAININGHALL::operator ==(const GameEvent& s) const
{
  int i;
  if (&s == this) return  true;
  if (!Equals(s)) return false;
  const TRAININGHALL &src = (const TRAININGHALL&)(s);
  if (forceExit != src.forceExit) return false;
  if (trainableBaseclasses.GetSize() != src.trainableBaseclasses.GetSize()) return false;
  for (i=0; i<trainableBaseclasses.GetSize(); i++)
  {
    if (!(*PeekTrainableBaseclass(i) == *src.PeekTrainableBaseclass(i))) return false;
    //if (!(trainableBaseclasses[i] == src.trainableBaseclasses[i])) return false;
  };
  if (Cost != src.Cost) return false;
  return true;
}
#endif
SHOP& SHOP::operator =(const SHOP& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SHOP)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const SHOP &data = (const SHOP&)(src);

  forceExit = data.forceExit;
  costFactor = data.costFactor;
  itemsAvail = data.itemsAvail;
  costToId=data.costToId;
  buybackPercentage = data.buybackPercentage;
  canId=data.canId;
  canApprGems=data.canApprGems;
  canApprJewels=data.canApprJewels;
  sellprice=data.sellprice;
  buyItemsSoldOnly=data.buyItemsSoldOnly;

  return *this;
}
#ifdef UAFEDITOR
bool SHOP::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const SHOP &src = (const SHOP&)(s);

  if (forceExit != src.forceExit) return false;
  if (costFactor != src.costFactor) return false;
  if (!(itemsAvail == src.itemsAvail)) return false;
  if (costToId != src.costToId) return false;
  if (buybackPercentage != src.buybackPercentage) return false;
  if (canId != src.canId) return false;
  if (canApprGems != src.canApprGems) return false;
  if (canApprJewels != src.canApprJewels) return false;
  if (sellprice != src.sellprice) return false;
  if (buyItemsSoldOnly != src.buyItemsSoldOnly) return false;

  return true;
}
#endif

void SOUND_EVENT::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  POSITION pos = sounds.GetHeadPosition();
  while (pos != NULL)
    ::SaveSound(sounds.GetNext(pos), level, rte.SoundDir());
};

void SOUND_EVENT::Serialize(CArchive &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int count;
  CString temp;

  if (ar.IsStoring())
  {      
    count = sounds.GetCount();      
    ar << count;
    
    POSITION pos = sounds.GetHeadPosition();
    while (pos != NULL)
    {
      temp = sounds.GetAt(pos);
      StripFilenamePath(temp);
      AS(ar,temp);
      //AddFolderToPath(temp, rte.SoundDir());
      //sounds.SetAt(pos, temp);
      sounds.GetNext(pos);

//      if (temp == "")
//      {
//        temp = "*";
//        ar << temp;
//      }
//      else
//        ar << temp;        
    }
  }
  else
  {
    ar >> count;
    for (int i=0;i<count;i++)
    {
      DAS(ar,temp);
      if (globalData.version < 0.930301)
      {
        StripFilenamePath(temp);
      };
//      ar >> temp;
//      if (temp == "*")
//        temp = "";
      //AddFolderToPath(temp, rte.SoundDir());
      sounds.AddTail(temp);
    }
  }
}
void SOUND_EVENT::Serialize(CAR &ar, double version)
{
  GameEvent::Serialize(ar, version);
  int count;
  CString temp;

  if (ar.IsStoring())
  {      
    count = sounds.GetCount();      
    ar << count;
    
    POSITION pos = sounds.GetHeadPosition();
    while (pos != NULL)
    {
      temp = sounds.GetAt(pos);
      StripFilenamePath(temp);
      AS(ar,temp);
      //AddFolderToPath(temp, rte.SoundDir());
      //sounds.SetAt(pos, temp);
      sounds.GetNext(pos);

//      if (temp == "")
//      {
//        temp = "*";
//        ar << temp;
//      }
//      else
//        ar << temp;        
    }
  }
  else
  {
    ar >> count;
    for (int i=0;i<count;i++)
    {
      DAS(ar,temp);
      if (globalData.version < 0.930301)
      {
        StripFilenamePath(temp);
      };
//      ar >> temp;
//      if (temp == "*")
//        temp = "";
      //AddFolderToPath(temp, rte.SoundDir());
      sounds.AddTail(temp);
    }
  }
}

#ifdef UAFEDITOR
void SOUND_EVENT::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
  POSITION pos;
  for (pos=sounds.GetHeadPosition(); pos!=NULL; sounds.GetNext(pos))
  {
    CrossReferenceEventSound(pCREI, sounds.GetAt(pos));
  };
  
}

extern const char *JKEY_SOUNDS;
void SOUND_EVENT::Export(JWriter& jw)
{
  CString temp;
  POSITION pos = sounds.GetHeadPosition();
  jw.StartArray(JKEY_SOUNDS);
  while (pos != NULL)
  {
    temp = sounds.GetAt(pos);
    jw.NextEntry();
    jw.NameAndValue(NULL, temp);
    sounds.GetNext(pos);
  }
  jw.EndArray();
}
bool SOUND_EVENT::Import(JReader& jr)
{
  CString temp;
  jr.StartArray(JKEY_SOUNDS);
  while (jr.Optional(), jr.NextEntry())
  {
    jr.NameAndValue(NULL, temp);
    sounds.AddTail(temp);
  };
  jr.EndArray();
  return true;
}
#endif

SOUND_EVENT& SOUND_EVENT::operator =(const SOUND_EVENT& src)
{
  if (&src == this)
    return *this;
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SOUND_EVENT)));

  GameEvent::operator =(src);
  const SOUND_EVENT &data = (const SOUND_EVENT&)(src);

  sounds.RemoveAll();
  POSITION pos = data.sounds.GetHeadPosition();

  CString temp;
  while (pos != NULL)
  {
    temp = data.sounds.GetNext(pos);
    sounds.AddTail(temp);
  }
  return *this;
}

#ifdef UAFEDITOR
bool SOUND_EVENT::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const SOUND_EVENT &src = (const SOUND_EVENT&)(s);
  POSITION pos1 = src.sounds.GetHeadPosition();
  POSITION pos2 = sounds.GetHeadPosition();
  CString temp1, temp2;
  while (pos1 != NULL)
  {
    if (pos2 == NULL) return false;
    temp1 = src.sounds.GetNext(pos1);
    temp2 = sounds.GetNext(pos2);
    if (temp1 != temp2) return false;;
  };
  return true;
}
#endif

#if (defined UAFEDITOR)



void QUEST_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    acceptChain = cid;
    break;
  case 2:
    rejectChain = cid;
    break;
  }
}

int QUEST_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (acceptChain == eid) {acceptChain=0;result++;};
  if (rejectChain == eid) {rejectChain=0;result++;};
  return result;
}



BOOL QUEST_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Accept Chain",
            (LPCSTR)GetEventIdDescription(acceptChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Reject Chain",
            (LPCSTR)GetEventIdDescription(rejectChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL QUEST_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=acceptChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=rejectChain;
    return TRUE;
  }

  return FALSE;
}

#endif


#if (defined UAFEDITOR)

void TAVERN::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }

  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    fightChain = cid;
    break;
  case 2:
    drinkChain = cid;
    break;
  }
}

int TAVERN::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (fightChain == eid) {fightChain=0;result++;};
  if (drinkChain == eid) {drinkChain=0;result++;};
  return result;
}



BOOL TAVERN::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Fight Chain",
            (LPCSTR)GetEventIdDescription(fightChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Drunk Chain",
            (LPCSTR)GetEventIdDescription(drinkChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL TAVERN::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=fightChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=drinkChain;
    return TRUE;
  }

  return FALSE;
}

#endif
void TEMPLE::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=1;  
  if (donationChain > 0)
    curr++;
}

BOOL TEMPLE::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (donationChain > 0)
    return TRUE;

  return FALSE;
}


#if (defined UAFEDITOR)

void TEMPLE::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    donationChain = cid;
    break;
  }
}

int TEMPLE::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (donationChain == eid) {donationChain=0;result++;};
  return result;
}



BOOL TEMPLE::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Donate Chain",
            (LPCSTR)GetEventIdDescription(donationChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL TEMPLE::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=donationChain;
    return TRUE;
  }

  return FALSE;
}
#endif

void RANDOM_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_RANDOM_EVENTS-1;
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChains[i-1] > 0)
      curr++;
  }
}
BOOL RANDOM_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained()) return TRUE;
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChains[i-1] > 0)
      return TRUE;
  }
  return FALSE;
}
#if (defined UAFEDITOR)

void RANDOM_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  default:
    EventChains[num-1]=cid;
    break;
  }
}

int RANDOM_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ 
  // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChains[i-1] == eid) 
    {
      EventChains[i-1]=0;  
      result++;
    };
  }
  return result;
}

BOOL RANDOM_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if ((num>0) && (num < MAX_RANDOM_EVENTS))
  {
    sprintf(ptext, 
            "%s %i: %s", 
            "Random Chain",
            num,
            (LPCSTR)GetEventIdDescription(EventChains[num-1],GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL RANDOM_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if ((num>0) && (num < MAX_RANDOM_EVENTS))
  {
    cid = EventChains[num-1];
    return TRUE;
  }
  return FALSE;
}
#endif

void SMALL_TOWN_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=6;  
  if (TempleChain > 0)
    curr++;
  if (TrainingHallChain > 0)
    curr++;
  if (ShopChain > 0)
    curr++;
  if (InnChain > 0)
    curr++;
  if (TavernChain > 0)
    curr++;
  if (VaultChain > 0)
    curr++;
}

BOOL SMALL_TOWN_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (TempleChain > 0)
    return TRUE;
  if (TrainingHallChain > 0)
    return TRUE;
  if (ShopChain > 0)
    return TRUE;
  if (InnChain > 0)
    return TRUE;
  if (TavernChain > 0)
    return TRUE;
  if (VaultChain > 0)
    return TRUE;

  return FALSE;
}


#if (defined UAFEDITOR)

void SMALL_TOWN_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    TempleChain = cid;
    break;
  case 2:
    TrainingHallChain = cid;
    break;
  case 3:
    ShopChain = cid;
    break;
  case 4:
    InnChain = cid;
    break;
  case 5:
    TavernChain = cid;
    break;
  case 6:
    VaultChain = cid;
    break;
  }
}

int SMALL_TOWN_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (TempleChain == eid)       {TempleChain=0;      result++;};
  if (TrainingHallChain == eid) {TrainingHallChain=0;result++;};
  if (ShopChain == eid)         {ShopChain=0;        result++;};
  if (InnChain == eid)          {InnChain=0;         result++;};
  if (TavernChain == eid)       {TavernChain=0;      result++;};
  if (VaultChain == eid)        {VaultChain=0;       result++;};
  return result;
}



BOOL SMALL_TOWN_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  switch (num)
  {
  case 1:
    sprintf(ptext, 
            "%s: %s", 
            "Temple Chain",
            (LPCSTR)GetEventIdDescription(TempleChain,GetEventSource()));
    return TRUE;
    break;
  case 2:
    sprintf(ptext, 
            "%s: %s", 
            "Training Hall Chain",
            (LPCSTR)GetEventIdDescription(TrainingHallChain,GetEventSource()));
    return TRUE;
    break;  
  case 3:
    sprintf(ptext, 
            "%s: %s", 
            "Shop Chain",
            (LPCSTR)GetEventIdDescription(ShopChain,GetEventSource()));
    return TRUE;
    break;  
  case 4:
    sprintf(ptext, 
            "%s: %s", 
            "Inn Chain",
            (LPCSTR)GetEventIdDescription(InnChain,GetEventSource()));
    return TRUE;
    break;  
  case 5:
    sprintf(ptext, 
            "%s: %s", 
            "Tavern Chain",
            (LPCSTR)GetEventIdDescription(TavernChain,GetEventSource()));
    return TRUE;
    break;  
  case 6:
    sprintf(ptext, 
            "%s: %s", 
            "Vault Chain",
            (LPCSTR)GetEventIdDescription(VaultChain,GetEventSource()));
    return TRUE;
    break;
  }
  return FALSE;
}

BOOL SMALL_TOWN_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  switch (num)
  {
  case 1:
    cid=TempleChain;
    return TRUE;
    break;
  case 2:
    cid=TrainingHallChain;
    return TRUE;
    break;
  case 3:
    cid=ShopChain;
    return TRUE;
    break;
  case 4:
    cid=InnChain;
    return TRUE;
    break;
  case 5:
    cid=TavernChain;
    return TRUE;
    break;
  case 6:
    cid=VaultChain;
    return TRUE;
    break;
  }
  return FALSE;
}
#endif

void CHAIN_EVENT::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=1;  
  if (Chain > 0)
    curr++;
}

BOOL CHAIN_EVENT::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (Chain > 0)
    return TRUE;

  return FALSE;
}

#if (defined UAFEDITOR)

void CHAIN_EVENT::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    Chain = cid;
    break;
  }
}

int CHAIN_EVENT::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (Chain == eid) {Chain=0;result++;};
  return result;
}



BOOL CHAIN_EVENT::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  switch (num)
  {
  case 1:
    sprintf(ptext, 
            "%s: %s", 
            "Chained Event",
            (LPCSTR)GetEventIdDescription(Chain,GetEventSource()));
    return TRUE;
    break;
  }

  return FALSE;
}

BOOL CHAIN_EVENT::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  switch (num)
  {
  case 1:
    cid=Chain;
    return TRUE;
    break;
  }
  return FALSE;
}
#endif

void QUEST_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;  
  if (acceptChain > 0)
    curr++;
  if (rejectChain > 0)
    curr++;
}
void TAVERN::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;  
  if (fightChain > 0)
    curr++;
  if (drinkChain > 0)
    curr++;
}


#if (defined UAFEDITOR)

void ENCOUNTER_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if ((num < 0) || (num > MAX_BUTTONS+3))
    return;

  if (num == 0)
  {
    GameEvent::ChainToEventId(num, cid);
    return;
  }

  if (num <= MAX_BUTTONS)
    buttons.buttons[num-1].chain = cid;

  if (num==MAX_BUTTONS+1)
    combatChain = cid;
  if (num==MAX_BUTTONS+2)
    talkChain = cid;
  if (num==MAX_BUTTONS+3)
    escapeChain = cid;
}

int ENCOUNTER_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=0; i<MAX_BUTTONS; i++)
    if (buttons.buttons[i].chain==eid) 
    {
      buttons.buttons[i].chain=0;
      result++;
    };
  if (combatChain==eid) {result++;combatChain=0;};
  if (talkChain==eid)   {result++;talkChain=0;};
  if (escapeChain==eid) {result++;escapeChain=0;};
  return result;
}


BOOL ENCOUNTER_DATA::GetEVChainText(int num, char *ptext)
{
  if ((num < 0) || (num > MAX_BUTTONS+3))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);
  if (num <= MAX_BUTTONS)
  {
    sprintf(ptext, 
            "Button %i Chain: %s", 
            num,
            (LPCSTR)GetEventIdDescription(buttons.buttons[num-1].chain,GetEventSource()));
    return TRUE;
  };
  if (num==MAX_BUTTONS+1)
  {
    sprintf(ptext,"Combat Chain: %s",
            (LPCSTR)GetEventIdDescription(combatChain,GetEventSource()));
    return TRUE;
  };
  if (num==MAX_BUTTONS+2)
  {
    sprintf(ptext,"Talk Chain: %s",
            (LPCSTR)GetEventIdDescription(talkChain,GetEventSource()));
    return TRUE;
  };
  if (num==MAX_BUTTONS+3)
  {
    sprintf(ptext,"Escape Chain: %s",
            (LPCSTR)GetEventIdDescription(escapeChain,GetEventSource()));
    return TRUE;
  };
  return FALSE;
}

BOOL ENCOUNTER_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > MAX_BUTTONS+3))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChain(num, cid);
  if (num<=MAX_BUTTONS)
  {
    cid = buttons.buttons[num-1].chain;
    return TRUE;
  };
  if (num==MAX_BUTTONS+1)
  {
    cid=combatChain;
    return TRUE;
  };
  if (num==MAX_BUTTONS+2)
  {
    cid=talkChain;
    return TRUE;
  };
  if (num==MAX_BUTTONS+3)
  {
    cid=escapeChain;
    return TRUE;
  };
  return FALSE;
}

#endif

void ENCOUNTER_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_BUTTONS+3;

  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].chain > 0)
      curr++;
  }
  if (combatChain > 0) curr++;
  if (talkChain > 0) curr++;
  if (escapeChain > 0) curr++;
}

#if (defined UAFEDITOR)

void PASSWORD_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    successChain = cid;
    break;
  case 2:
    failChain = cid;
    break;
  }
}


int PASSWORD_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (successChain == eid) {successChain=0;result++;};
  if (failChain == eid)    {failChain=0;   result++;};
  return result;
}


BOOL PASSWORD_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pswd Success Chain",
            (LPCSTR)GetEventIdDescription(successChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pswd Fail Chain",
            (LPCSTR)GetEventIdDescription(failChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL PASSWORD_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=successChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=failChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void PASSWORD_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (successChain > 0)
    curr++;
  if (failChain > 0)
    curr++;
}

#if (defined UAFEDITOR)

void WHO_PAYS_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    successChain = cid;
    break;
  case 2:
    failChain = cid;
    break;
  }
}

int WHO_PAYS_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (successChain == eid) {successChain=0;result++;};
  if (failChain == eid)    {failChain=0;   result++;};
  return result;
}



BOOL WHO_PAYS_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pay Success Chain",
            (LPCSTR)GetEventIdDescription(successChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pay Fail Chain",
            (LPCSTR)GetEventIdDescription(failChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL WHO_PAYS_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=successChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=failChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void WHO_PAYS_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (successChain > 0)
    curr++;
  if (failChain > 0)
    curr++;
}

#if (defined UAFEDITOR)

void WHO_TRIES_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    successChain = cid;
    break;
  case 2:
    failChain = cid;
    break;
  }
}

int WHO_TRIES_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (successChain == eid) {successChain=0;result++;};
  if (failChain == eid)    {failChain=0;   result++;};
  return result;
}



BOOL WHO_TRIES_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Try Success Chain",
            (LPCSTR)GetEventIdDescription(successChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Try Fail Chain",
            (LPCSTR)GetEventIdDescription(failChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}


BOOL WHO_TRIES_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=successChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=failChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void WHO_TRIES_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (successChain > 0)
    curr++;
  if (failChain > 0)
    curr++;
}

#if (defined UAFEDITOR)

void QUESTION_BUTTON_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if ((num < 0) || (num > MAX_BUTTONS))
    return;

  if (num == 0)
  {
    GameEvent::ChainToEventId(num, cid);
    return;
  }

  buttons.buttons[num-1].chain = cid;
}

int QUESTION_BUTTON_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=0; i<MAX_BUTTONS; i++)
    if (buttons.buttons[i].chain==eid)
    {
      buttons.buttons[i].chain=0;
      result++;
    };
  return result;
}



BOOL QUESTION_BUTTON_DATA::GetEVChainText(int num, char *ptext)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

    sprintf(ptext, 
            "Button %i Chain: %s", 
            num,
            (LPCSTR)GetEventIdDescription(buttons.buttons[num-1].chain,GetEventSource()));

  return TRUE;
}


BOOL QUESTION_BUTTON_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  cid = buttons.buttons[num-1].chain;  
  return TRUE;
}

#endif

void QUESTION_BUTTON_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_BUTTONS;

  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].chain > 0)
      curr++;
  }
}

#if (defined UAFEDITOR)



void QUESTION_LIST_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if ((num < 0) || (num > MAX_BUTTONS))
    return;

  if (num == 0)
  {
    GameEvent::ChainToEventId(num, cid);
    return;
  }

  buttons.buttons[num-1].chain = cid;
}

int QUESTION_LIST_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=0; i<MAX_BUTTONS; i++)
    if (buttons.buttons[i].chain==eid) 
    {
      buttons.buttons[i].chain=0;
      result++;
    };
  return result;
}


BOOL QUESTION_LIST_DATA::GetEVChainText(int num, char *ptext)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

    sprintf(ptext, 
            "Button %i Chain: %s", 
            num,
            (LPCSTR)GetEventIdDescription(buttons.buttons[num-1].chain,GetEventSource()));

  return TRUE;
}

BOOL QUESTION_LIST_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  cid = buttons.buttons[num-1].chain;  
  return TRUE;
}

#endif

void QUESTION_LIST_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_BUTTONS;

  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].chain > 0)
      curr++;
  }
}

#if (defined UAFEDITOR)

void QUESTION_YES_NO::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    YesChain = cid;
    break;
  case 2:
    NoChain = cid;
    break;
  }
}

int QUESTION_YES_NO::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (YesChain == eid) {YesChain=0;result++;};
  if (NoChain == eid)  {NoChain=0; result++;};
  return result;
}



BOOL QUESTION_YES_NO::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Yes Chain",
            (LPCSTR)GetEventIdDescription(YesChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "No Chain",
            (LPCSTR)GetEventIdDescription(NoChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL QUESTION_YES_NO::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=YesChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=NoChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void QUESTION_YES_NO::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (YesChain > 0)
    curr++;
  if (NoChain > 0)
    curr++;
}
BOOL QUEST_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (acceptChain > 0)
    return TRUE;
  if (rejectChain > 0)
    return TRUE;

  return FALSE;
}

BOOL TAVERN::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (drinkChain > 0)
    return TRUE;
  if (fightChain > 0)
    return TRUE;

  return FALSE;
}

BOOL ENCOUNTER_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

	for (int i=0;i<MAX_BUTTONS;i++)
	{
    if (buttons.buttons[i].chain > 0)
			return TRUE;
	}

  return FALSE;
}

BOOL QUESTION_LIST_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

	for (int i=0;i<MAX_BUTTONS;i++)
	{
    if (buttons.buttons[i].chain > 0)
			return TRUE;
	}

  return FALSE;
}

BOOL QUESTION_BUTTON_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

	for (int i=0;i<MAX_BUTTONS;i++)
	{
    if (buttons.buttons[i].chain > 0)
			return TRUE;
	}

  return FALSE;
}

BOOL QUESTION_YES_NO::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (YesChain > 0)
    return TRUE;
  if (NoChain > 0)
    return TRUE;

  return FALSE;
}

BOOL PASSWORD_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (successChain > 0)
    return TRUE;
  if (failChain > 0)
    return TRUE;

  return FALSE;
}

BOOL WHO_PAYS_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (successChain > 0)
    return TRUE;
  if (failChain > 0)
    return TRUE;

  return FALSE;
}

BOOL WHO_TRIES_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (successChain > 0)
    return TRUE;
  if (failChain > 0)
    return TRUE;

  return FALSE;
}

void HEAL_PARTY_DATA::Clear()
{
  GameEvent::Clear();
  
  HealHP=TRUE;
  HealDrain=FALSE;
  HealCurse=FALSE;
  chance = 100;
  who=EntireParty;
  HowMuchHP=100;
  LiteralOrPercent=1;
}

HEAL_PARTY_DATA& HEAL_PARTY_DATA::operator =(const HEAL_PARTY_DATA& src)
{
  if (&src == this)
    return *this;
  GameEvent::operator =(src);
  HealHP = src.HealHP;
  HealDrain = src.HealDrain;
  HealCurse = src.HealCurse;
  chance = src.chance;
  who = src.who;
  HowMuchHP=src.HowMuchHP;
  LiteralOrPercent=src.LiteralOrPercent;
  return *this;
}
#ifdef UAFEDITOR
bool HEAL_PARTY_DATA::operator ==(const GameEvent& s) const
{
  if (&s == this) return true;
  if (!Equals(s)) return false;
  const HEAL_PARTY_DATA& src = (const HEAL_PARTY_DATA&)s;
  if (HealHP != src.HealHP) return false;
  if (HealDrain != src.HealDrain) return false;
  if (HealCurse != src.HealCurse) return false;
  if (chance != src.chance) return false;
  if (who != src.who) return false;
  if (HowMuchHP != src.HowMuchHP) return false;
  if (LiteralOrPercent != src.LiteralOrPercent) return false;
  return true;
}
#endif
void HEAL_PARTY_DATA::Serialize(CArchive &ar, double version)
{
   GameEvent::Serialize(ar, version);
   if (ar.IsStoring())
   {
      ar << HealHP;
      ar << HealDrain;
      ar << HealCurse;
      ar << chance;
      ar << (int)who;
      ar << HowMuchHP;
      ar << LiteralOrPercent;
   }
   else
   {
      int temp;
      ar >> HealHP;
      ar >> HealDrain;
      ar >> HealCurse;
      ar >> chance;
      ar >> temp;
      who = (eventPartyAffectType)temp;
      if (LoadingVersion >= _VERSION_0882_)
      {
        ar >> HowMuchHP;
        ar >> LiteralOrPercent;
      }
   }  
}
void HEAL_PARTY_DATA::Serialize(CAR &ar, double version)
{
   GameEvent::Serialize(ar, version);
   if (ar.IsStoring())
   {
      ar << HealHP;
      ar << HealDrain;
      ar << HealCurse;
      ar << chance;
      ar << (int)who;
      ar << HowMuchHP;
      ar << LiteralOrPercent;
   }
   else
   {
      int temp;
      ar >> HealHP;
      ar >> HealDrain;
      ar >> HealCurse;
      ar >> chance;
      ar >> temp;
      who = (eventPartyAffectType)temp;
      if (LoadingVersion >= _VERSION_0882_)
      {
        ar >> HowMuchHP;
        ar >> LiteralOrPercent;
      }
   }  
}

#ifdef UAFEDITOR
void HEAL_PARTY_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREI)
{
}

const char *JKEY_HEALHP = "healHP";
const char *JKEY_HEALDRAIN = "healDrain";
const char *JKEY_HEALCURSE = "healCurse";
extern const char *JKEY_CHANCE;
//const char *JKEY_WHO = "who";
const char *JKEY_HP = "HP";
const char *JKEY_ADJUSTHP = "adjustHP";
const char *JKEY_ADJUSTMENT = "adjustment";

void HEAL_PARTY_DATA::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_HEALHP, HealHP);
  jw.NameAndBool(JKEY_HEALDRAIN, HealDrain);
  jw.NameAndBool(JKEY_HEALCURSE, HealCurse);
  jw.StartList(JKEY_AFFECTS);
    jw.Linefeed(false);
    jw.NameAndEnum(JKEY_WHO, who);
    jw.NameAndValue(JKEY_CHANCE, chance);
  jw.EndList();
  jw.Linefeed(true);
  jw.StartList(JKEY_ADJUSTMENT);
    jw.Linefeed(false);
    jw.NameAndValue(JKEY_HP, HowMuchHP);
    jw.NameAndEnum(JKEY_ADJUSTHP, LiteralOrPercent, NUM_LITERALORPERCENT_TYPES, LiteralOrPercentText);
  jw.EndList();
  jw.Linefeed(true);
}
bool HEAL_PARTY_DATA::Import(JReader& jr)
{
  jr.NameAndBool(JKEY_HEALHP, HealHP);
  jr.NameAndBool(JKEY_HEALDRAIN, HealDrain);
  jr.NameAndBool(JKEY_HEALCURSE, HealCurse);
  if (jr.Optional(), jr.StartList(JKEY_AFFECTS))
  {
    jr.Linefeed(false);
    jr.NameAndEnum(JKEY_WHO, who);
    jr.NameAndValue(JKEY_CHANCE, chance);
    jr.EndList();
    jr.Linefeed(true);
  };
  if (jr.Optional(), jr.StartList(JKEY_ADJUSTMENT))
  {
    jr.Linefeed(false);
    jr.NameAndValue(JKEY_HP, HowMuchHP);
    jr.NameAndEnum(JKEY_ADJUSTHP, LiteralOrPercent, NUM_LITERALORPERCENT_TYPES, LiteralOrPercentText);
    jr.EndList();
    jr.Linefeed(true);
  };
  return true;
}
#endif

LOGIC_BLOCK_DATA::LOGIC_BLOCK_DATA(void) : GameEvent(), falseChain(0),trueChain(0) 
{
  Clear();
  event=LogicBlock;
}

LOGIC_BLOCK_DATA::~LOGIC_BLOCK_DATA(void)
{
  //Clear();
}

void LOGIC_BLOCK_DATA::Clear(void)
{
  GameEvent::Clear();
  
  falseChain=0;
  trueChain=0;
	m_InputParamA="";
	m_InputParamB="";
	m_InputParamD="";
	m_InputParamF="";
	m_InputParamG="";
	m_ActionParam1="";
	m_ActionParam2="";
	m_GateTypeC=0;
	m_GateTypeE=0;
	m_GateTypeH=0;
	m_GateTypeI=0;
	m_GateTypeJ=0;
	m_GateTypeK=0;
	m_GateTypeL=0;
	m_InputTypeA=0;
	m_InputTypeB=0;
	m_InputTypeD=0;
	m_InputTypeF=0;
	m_InputTypeG=0;
	m_ActionType1=0;
	m_ActionType2=0;
	m_ChainIfFalse=0;
	m_ChainIfTrue=0;
	m_NoChain=1;
	m_NotC=0;
	m_NotE=0;
	m_NotH=0;
	m_NotI=0;
	m_NotJ=0;
	m_NotK=0;
  m_IfTrue1=0;
  m_IfTrue2=0;
  m_Flags=0;
  m_Misc="";Result=-1;
}

LOGIC_BLOCK_DATA& LOGIC_BLOCK_DATA::operator = (const LOGIC_BLOCK_DATA& src)
{
  if (&src == this) return *this;

  GameEvent::operator = (src);
  Result=src.Result;
  falseChain=src.falseChain;
  trueChain=src.trueChain;
	m_InputParamA=src.m_InputParamA;
	m_InputParamB=src.m_InputParamB;
	m_InputParamD=src.m_InputParamD;
	m_InputParamF=src.m_InputParamF;
	m_InputParamG=src.m_InputParamG;
	m_ActionParam1=src.m_ActionParam1;
	m_ActionParam2=src.m_ActionParam2;
	m_GateTypeC=src.m_GateTypeC;
	m_GateTypeE=src.m_GateTypeE;
	m_GateTypeH=src.m_GateTypeH;
	m_GateTypeI=src.m_GateTypeI;
	m_GateTypeJ=src.m_GateTypeJ;
	m_GateTypeK=src.m_GateTypeK;
	m_GateTypeL=src.m_GateTypeL;
	m_InputTypeA=src.m_InputTypeA;
	m_InputTypeB=src.m_InputTypeB;
	m_InputTypeD=src.m_InputTypeD;
	m_InputTypeF=src.m_InputTypeF;
	m_InputTypeG=src.m_InputTypeG;
	m_ActionType1=src.m_ActionType1;
	m_ActionType2=src.m_ActionType2;
	m_ChainIfFalse=src.m_ChainIfFalse;
	m_ChainIfTrue=src.m_ChainIfTrue;
	m_NoChain=src.m_NoChain;
	m_NotC=src.m_NotC;
	m_NotE=src.m_NotE;
	m_NotH=src.m_NotH;
	m_NotI=src.m_NotI;
	m_NotJ=src.m_NotJ;
	m_NotK=src.m_NotK;
  m_IfTrue1=src.m_IfTrue1;
  m_IfTrue2=src.m_IfTrue2;
  m_Flags=src.m_Flags;
  m_Misc=src.m_Misc;
  return *this;
}
#ifdef UAFEDITOR
bool LOGIC_BLOCK_DATA::operator == (const GameEvent& src) const
{
  /*
  if (&src == this) return *this;

  GameEvent::operator = (src);
  Result=src.Result;
  falseChain=src.falseChain;
  trueChain=src.trueChain;
	m_InputParamA=src.m_InputParamA;
	m_InputParamB=src.m_InputParamB;
	m_InputParamD=src.m_InputParamD;
	m_InputParamF=src.m_InputParamF;
	m_InputParamG=src.m_InputParamG;
	m_ActionParam1=src.m_ActionParam1;
	m_ActionParam2=src.m_ActionParam2;
	m_GateTypeC=src.m_GateTypeC;
	m_GateTypeE=src.m_GateTypeE;
	m_GateTypeH=src.m_GateTypeH;
	m_GateTypeI=src.m_GateTypeI;
	m_GateTypeJ=src.m_GateTypeJ;
	m_GateTypeK=src.m_GateTypeK;
	m_GateTypeL=src.m_GateTypeL;
	m_InputTypeA=src.m_InputTypeA;
	m_InputTypeB=src.m_InputTypeB;
	m_InputTypeD=src.m_InputTypeD;
	m_InputTypeF=src.m_InputTypeF;
	m_InputTypeG=src.m_InputTypeG;
	m_ActionType1=src.m_ActionType1;
	m_ActionType2=src.m_ActionType2;
	m_ChainIfFalse=src.m_ChainIfFalse;
	m_ChainIfTrue=src.m_ChainIfTrue;
	m_NoChain=src.m_NoChain;
	m_NotC=src.m_NotC;
	m_NotE=src.m_NotE;
	m_NotH=src.m_NotH;
	m_NotI=src.m_NotI;
	m_NotJ=src.m_NotJ;
	m_NotK=src.m_NotK;
  m_IfTrue1=src.m_IfTrue1;
  m_IfTrue2=src.m_IfTrue2;
  m_Flags=src.m_Flags;
  m_Misc=src.m_Misc;
  */
  return false;
}
#endif
void LOGIC_BLOCK_DATA::Serialize(CArchive& ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    ar << falseChain;
    ar << trueChain;
  	ar << m_InputParamA;
	  ar << m_InputParamB;
	  ar << m_InputParamD;
	  ar << m_InputParamF;
	  ar << m_InputParamG;
	  ar << m_ActionParam1;
	  ar << m_ActionParam2;
	  ar << m_GateTypeC;
	  ar << m_GateTypeE;
	  ar << m_GateTypeH;
	  ar << m_GateTypeI;
	  ar << m_GateTypeJ;
	  ar << m_GateTypeK;
	  ar << m_GateTypeL;
	  ar << m_InputTypeA;
	  ar << m_InputTypeB;
	  ar << m_InputTypeD;
	  ar << m_InputTypeF;
	  ar << m_InputTypeG;
	  ar << m_ActionType1;
	  ar << m_ActionType2;
	  ar << m_ChainIfFalse;
	  ar << m_ChainIfTrue;
	  ar << m_NoChain;
	  ar << m_NotC;
	  ar << m_NotE;
	  ar << m_NotH;
	  ar << m_NotI;
	  ar << m_NotJ;
	  ar << m_NotK;
    ar << m_IfTrue1;
    ar << m_IfTrue2;
    ar << m_Flags;
    ar << m_Misc;
  }
  else
  {
    ar >> falseChain;
    ar >> trueChain;
  	ar >> m_InputParamA;
  	ar >> m_InputParamB;
  	ar >> m_InputParamD;
  	ar >> m_InputParamF;
  	ar >> m_InputParamG;
  	ar >> m_ActionParam1;
  	ar >> m_ActionParam2;
  	ar >> m_GateTypeC;
  	ar >> m_GateTypeE;
  	ar >> m_GateTypeH;
  	ar >> m_GateTypeI;
  	ar >> m_GateTypeJ;
  	ar >> m_GateTypeK;
  	ar >> m_GateTypeL;
  	ar >> m_InputTypeA;
  	ar >> m_InputTypeB;
  	ar >> m_InputTypeD;
  	ar >> m_InputTypeF;
  	ar >> m_InputTypeG;
  	ar >> m_ActionType1;
  	ar >> m_ActionType2;
  	ar >> m_ChainIfFalse;
  	ar >> m_ChainIfTrue;
  	ar >> m_NoChain;
  	ar >> m_NotC;
  	ar >> m_NotE;
  	ar >> m_NotH;
  	ar >> m_NotI;
  	ar >> m_NotJ;
  	ar >> m_NotK;
    ar >> m_IfTrue1;
    ar >> m_IfTrue2;
    ar >> m_Flags;
    ar >> m_Misc;
  };
}
void LOGIC_BLOCK_DATA::Serialize(CAR& ar, double version)
{
  GameEvent::Serialize(ar, version);
  if (ar.IsStoring())
  {
    ar << falseChain;
    ar << trueChain;
  	ar << m_InputParamA;
	  ar << m_InputParamB;
	  ar << m_InputParamD;
	  ar << m_InputParamF;
	  ar << m_InputParamG;
	  ar << m_ActionParam1;
	  ar << m_ActionParam2;
	  ar << m_GateTypeC;
	  ar << m_GateTypeE;
	  ar << m_GateTypeH;
	  ar << m_GateTypeI;
	  ar << m_GateTypeJ;
	  ar << m_GateTypeK;
	  ar << m_GateTypeL;
	  ar << m_InputTypeA;
	  ar << m_InputTypeB;
	  ar << m_InputTypeD;
	  ar << m_InputTypeF;
	  ar << m_InputTypeG;
	  ar << m_ActionType1;
	  ar << m_ActionType2;
	  ar << m_ChainIfFalse;
	  ar << m_ChainIfTrue;
	  ar << m_NoChain;
	  ar << m_NotC;
	  ar << m_NotE;
	  ar << m_NotH;
	  ar << m_NotI;
	  ar << m_NotJ;
	  ar << m_NotK;
    ar << m_IfTrue1;
    ar << m_IfTrue2;
    ar << m_Flags;
    ar << m_Misc;
  }
  else
  {
    ar >> falseChain;
    ar >> trueChain;
  	ar >> m_InputParamA;
  	ar >> m_InputParamB;
  	ar >> m_InputParamD;
  	ar >> m_InputParamF;
  	ar >> m_InputParamG;
  	ar >> m_ActionParam1;
  	ar >> m_ActionParam2;
  	ar >> m_GateTypeC;
  	ar >> m_GateTypeE;
  	ar >> m_GateTypeH;
  	ar >> m_GateTypeI;
  	ar >> m_GateTypeJ;
  	ar >> m_GateTypeK;
  	ar >> m_GateTypeL;
  	ar >> m_InputTypeA;
  	ar >> m_InputTypeB;
  	ar >> m_InputTypeD;
  	ar >> m_InputTypeF;
  	ar >> m_InputTypeG;
  	ar >> m_ActionType1;
  	ar >> m_ActionType2;
  	ar >> m_ChainIfFalse;
  	ar >> m_ChainIfTrue;
  	ar >> m_NoChain;
  	ar >> m_NotC;
  	ar >> m_NotE;
  	ar >> m_NotH;
  	ar >> m_NotI;
  	ar >> m_NotJ;
  	ar >> m_NotK;
    ar >> m_IfTrue1;
    ar >> m_IfTrue2;
    ar >> m_Flags;
    ar >> m_Misc;
  };
}


#ifdef UAFEDITOR
void LOGIC_BLOCK_DATA::DetailedCrossReference(CR_EVENT_INFO *pCREIl)
{
}
void LOGIC_BLOCK_DATA::Export(JWriter& jw)
{
  NotImplemented(0xcd929, false);
}
bool LOGIC_BLOCK_DATA::Import(JReader& jr)
{
  NotImplemented(0xcd92a, false);
  return true;
}
#endif


BOOL LOGIC_BLOCK_DATA::IsChained()
{
  if (GameEvent::IsChained()) return TRUE;
  if (falseChain > 0) return TRUE;
  if (trueChain > 0) return TRUE;
  return FALSE;
}

void LOGIC_BLOCK_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;
  if (falseChain > 0) curr++;
  if (trueChain > 0)  curr++;
}

#if (defined UAFEDITOR)

void LOGIC_BLOCK_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if (num==0) GameEvent::ChainToEventId(num, cid);
  if (num==1) falseChain = cid;
  if (num==2) trueChain = cid;
}

int LOGIC_BLOCK_DATA::UnchainToEventId(int num, DWORD cid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, cid);
  if (num==1) if (falseChain==cid) {falseChain=0; result++;};
  if (num==2) if (trueChain ==cid) {trueChain =0; result++;};
  return result;
}



BOOL LOGIC_BLOCK_DATA::GetEVChainText(int num, char *ctext)
{
  if ((num < 0) || (num > 2))
    return FALSE;

  if (num==0) return GameEvent::GetEVChainText(num, ctext);
  if (num==1) sprintf(ctext,
                      "False Chain: %s",
                      (LPCSTR)GetEventIdDescription(falseChain,GetEventSource()));
  if (num==2) sprintf(ctext,
                      "True Chain: %s",
                      (LPCSTR)GetEventIdDescription(trueChain,GetEventSource()));
  return TRUE;
}


BOOL LOGIC_BLOCK_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > 2))
    return FALSE;
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);
  if (num==1) cid=falseChain;
  if (num==2) cid=trueChain;
  return TRUE;
}


BOOL GameEvent::Validate(FILE *pFile)
{
  BOOL success = TRUE;
  // default is to validate pic and pic1 if needed
  if (pic.filename != "")
  {
    if (!FileExists(pic.filename))
    {
      if (IsAChainEvent())
        WriteValidateLog(pFile, "%s chain event missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), pic.filename);
      else
        WriteValidateLog(pFile, "%s event at %i,%i missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y, pic.filename);
      success = FALSE;
    }
  }
  if (pic2.filename != "")
  {
    if (!FileExists(pic2.filename))
    {
      if (IsAChainEvent())
        WriteValidateLog(pFile, "%s chain event missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), pic2.filename);
      else
        WriteValidateLog(pFile, "%s event at %i,%i missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y, pic2.filename);
      success = FALSE;
    }
  }
  return success;
}


void GameEvent::CrossReference(CR_EVENT_INFO *pCREI)
{ // By default, we only cross-reference pic and pic2

  int i;
  DWORD cid;
  CR_EVENT_INFO crEI;
  GameEvent *pEvent;
  crEI.m_anchorLocation = pCREI->m_anchorLocation;
  crEI.m_pCRList = pCREI->m_pCRList;
  crEI.m_level = pCREI->m_level;
  crEI.m_pEventList = pCREI->m_pEventList;
  CreateCRReference(&crEI);


  pCREI->m_pCRList->CR_AddResource(
                      crEI.m_CRReference.m_referenceType, 
                      crEI.m_CRReference.m_referenceName,
                      id);
  pCREI->m_pCRList->CR_AddReference(
                      CR_TYPE_event, 
                      crEI.m_CRReference.m_referenceName, 
                      id, 
                      &pCREI->m_CRReference);
  pic.CrossReference  (pCREI->m_pCRList, &pCREI->m_CRReference);
  pic2.CrossReference (pCREI->m_pCRList, &pCREI->m_CRReference);
  DetailedCrossReference(&crEI);  // The particular event
  for (i=0; GetEVChain(i, cid); i++)
  {
    if (pCREI->m_pEventList->IsValidEvent(cid))
    {
      pEvent = pCREI->m_pEventList->GetEvent(cid);
      if (pEvent != NULL)
      {
        pEvent->CrossReference(&crEI);
      };
    };
  };
}


BOOL COMBAT_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);

  if (!FileExists(DeathSound))
  {
    WriteValidateLog(pFile, "%s event at %i,%i missing death sound: %s\n",
             GetEventIdDescription(this->id,GetEventSource()), x,y, DeathSound);
    success = FALSE;
  }
  if (!FileExists(MoveSound))
  {
    WriteValidateLog(pFile, "%s event at %i,%i missing move sound: %s\n",
             GetEventIdDescription(this->id,GetEventSource()), x,y, MoveSound);
    success = FALSE;
  }

  
  if (!FileExists(TurnUndeadSound))
  {
    WriteValidateLog(pFile, "%s event at %i,%i missing turn undead sound: %s\n",
             GetEventIdDescription(this->id,GetEventSource()), x,y, TurnUndeadSound);
    success = FALSE;
  }

  POSITION pos = bgSounds.backgroundSounds.sounds.GetHeadPosition();
  while (pos != NULL)
  {
    if (!FileExists(bgSounds.backgroundSounds.sounds.GetAt(pos)))
    {
      WriteValidateLog(pFile, "%s event at %i,%i missing combat background sound: %s\n",
                       GetEventIdDescription(this->id,GetEventSource()), x,y, bgSounds.backgroundSounds.sounds.GetAt(pos));
      success = FALSE;
    }
    bgSounds.backgroundSounds.sounds.GetNext(pos);
  }

  for (int i=0;i<monsters.GetCount();i++)
  {
    if (monsters.monsters[i].m_type == MONSTER_TYPE)
    {
      //if (!monsterData.Validate(monsters.monsters[i].monster, pFile))
      if (!monsterData.Validate(monsters.monsters[i].monsterID, pFile))
      {
        WriteValidateLog(pFile, "%s event at %i,%i failed monster validation\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y);
        success = FALSE;
      }
      pos = monsters.monsters[i].items.GetHeadPosition();
      while (pos != NULL)
      {
        //if (!itemData.Validate(monsters.monsters[i].items.PeekAtPos(pos).m_giID, pFile))
        if (!itemData.Validate(monsters.monsters[i].items.PeekAtPos(pos).itemID, pFile))
        {
          WriteValidateLog(pFile, "%s event at %i,%i failed item validation\n",
                   GetEventIdDescription(this->id,GetEventSource()), x,y);
          success = FALSE;
        }
        monsters.monsters[i].items.GetNext(pos);
      }
    }
    else
    {
      //if (!globalData.charData.Validate(monsters.monsters[i].monster, pFile, VAL_ICON))
      if (!globalData.charData.Validate(monsters.monsters[i].characterID, pFile, VAL_ICON))
      {
        WriteValidateLog(pFile, "%s event at %i,%i failed NPC validation\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y);
        success = FALSE;
      }
      
      pos = monsters.monsters[i].items.GetHeadPosition();
      while (pos != NULL)
      {
        //if (!itemData.Validate(monsters.monsters[i].items.PeekAtPos(pos).m_giID, pFile))
        if (!itemData.Validate(monsters.monsters[i].items.PeekAtPos(pos).itemID, pFile))
        {
          WriteValidateLog(pFile, "%s event at %i,%i failed item validation\n",
                   GetEventIdDescription(this->id,GetEventSource()), x,y);
          success = FALSE;
        }
        monsters.monsters[i].items.GetNext(pos);
      }
    }
  }

  return success;
}

BOOL TRANSFER_DATA::Validate(FILE *pFile)
{
  int dx,dy;
  BOOL success = TRUE;

  if ((destLevel < 0) || (destLevel >= MAX_LEVELS))
  {
    success = FALSE;
    WriteValidateLog(pFile, "Invalid dest level %i in TRANSFER DATA\n", 
      destLevel+1);
  }
  else
  {
    if (!globalData.levelInfo.stats[destLevel].used)
    {
      success = FALSE;
      WriteValidateLog(pFile, "Dest level %i in TRANSFER DATA does not exist\n", 
        destLevel+1);
    }
    else
    {
      if (destEP < 0) // use x,y
      {
        dx = destX;
        dy = destY;
      }
      else
      {
        if (destEP > MAX_ENTRY_POINTS)
        {
          dx = dy = 0;
          WriteValidateLog(pFile, "Dest entry point %d in TRANSFER data is invalid\n", destEP);
        }
        else
        {
          dx = globalData.levelInfo.stats[destLevel].entryPoints[destEP].x;
          dy = globalData.levelInfo.stats[destLevel].entryPoints[destEP].y;
        };
      }

      if ((dx < 0) || (dx >= globalData.levelInfo.stats[destLevel].area_width))
      {
        success = FALSE;
        WriteValidateLog(pFile, "Dest x=%i for level %i in TRANSFER DATA not valid\n", 
          dx, destLevel+1);
      }
      if ((dy < 0) || (dy >= globalData.levelInfo.stats[destLevel].area_height))
      {
        success = FALSE;
        WriteValidateLog(pFile, "Dest y=%i for level %i in TRANSFER DATA not valid\n", 
          dy, destLevel+1);
      }
    }
  }
  return success;
}

BOOL TRANSFER_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  if (!transferData.Validate(pFile))
  {
    WriteValidateLog(pFile, "TRANSFER DATA in %s at %i,%i not valid\n", GetEventIdDescription(this->id,GetEventSource()),x,y);
    success = FALSE;
  }
  return success;
}

BOOL PASSWORD_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  success &= successTransferData.Validate(pFile);
  success &= failTransferData.Validate(pFile);
  if ((ChainEvent == successAction) && (!CheckValidEvent(this, successChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "success chain in %s at %i,%i not valid\n", GetEventIdDescription(this->id,GetEventSource()),x,y);
  }
  if ((ChainEvent == failAction) && (!CheckValidEvent(this,failChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "fail chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  return success;
}

BOOL WHO_TRIES_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  success &= successTransferData.Validate(pFile);
  success &= failTransferData.Validate(pFile);
  if ((ChainEvent == successAction) && (!CheckValidEvent(this,successChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "success chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  if ((ChainEvent == failAction) && (!CheckValidEvent(this,failChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "fail chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  return success;
}

BOOL WHO_PAYS_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  success &= successTransferData.Validate(pFile);
  success &= failTransferData.Validate(pFile);
  if ((ChainEvent == successAction) && (!CheckValidEvent(this,successChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "success chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  if ((ChainEvent == failAction) && (!CheckValidEvent(this,failChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "fail chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  return success;
}

#endif

void PLAY_MOVIE_DATA::Clear() 
{ 
  GameEvent::Clear();   
  m_filename="";
  m_mode=movieFull; 
}
void ADD_NPC_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  
  distance=FarAway;
  //charAdded=0;
  characterID.Clear();
  hitPointMod=100;
  useOriginal=TRUE; 
}
void REMOVE_NPC_DATA::Clear() 
{
  GameEvent::Clear(); 
  
  distance=FarAway; 
  //charRemoved=0; 
  characterID.Clear(); 
}
void CAMP_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  ForceExit=FALSE; 
}
void GUIDED_TOUR::Clear() 
{ 
  GameEvent::Clear();
  
  tour_x=0;tour_y=0;
  facing=0;
  useStartLoc=FALSE;
  currStep=0;
  executeEvent=FALSE;
  for (int i=0;i<MAX_TOUR_STEPS;i++) 
    steps[i].Clear(); 
}
void ENCOUNTER_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  currDist=0;
  distance=FarAway;
  monsterSpeed=12;
  zeroRangeResult=NoResult;
  combatChain=0;talkChain=0;escapeChain=0;
  buttons.Clear(); 
}
void PASSWORD_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  password="";
  nbrTries=0;
  currTry=0;
  successChain=0;
  failChain=0;
  successAction=NoAction;failAction=NoAction;
  successTransferData.Clear(); failTransferData.Clear(); 
  matchCriteria = ExactMatch; matchCase = TRUE; 
}
void GAIN_EXP_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  tempExp=0;
  experience=0;
  hSound=-1;
  sound="";
  chance=100; 
  who=EntireParty; 
}
void NPC_SAYS_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  //partySeesChar=0;
  characterID.Clear();
  distance=FarAway;
  hSound=-1;
  sound="";
  mustHitReturn=TRUE;
  highlight=TRUE; 
}
void QUESTION_YES_NO::Clear() 
{ 
  GameEvent::Clear();
  
  YesChainAction=LabelDoNothing; 
  NoChainAction=LabelDoNothing;
  YesChain=0;
  NoChain=sx=sy=0; 
}
void QUESTION_LIST_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  UserResult=NoUserResult;
  buttons.Clear();
  sx=sy=0; 
}
void QUESTION_BUTTON_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  UserResult=NoUserResult;
  buttons.Clear();
  sx=sy=0; 
}
void TEXT_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  hSound=-1;
  sound="";
  WaitForReturn=TRUE;
  HighlightText=FALSE;
  ForceBackup=FALSE;
  KillSound=FALSE; 
  distance = UpClose;
}
void TRANSFER_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  askYesNo=TRUE;
  transferOnYes=TRUE;
  destroyDrow=FALSE;
  activateBeforeEntry=FALSE;
  transferData.Clear(); 
}
void WHO_PAYS_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  impossible=FALSE;
  gems=0;
  jewels=0;
  platinum=0;
  successChain=0;
  failChain=0;
  successAction=NoAction;
  failAction=NoAction;
  successTransferData.Clear(); 
  failTransferData.Clear(); 
  moneyType = PlatinumType;
}
void WHO_TRIES_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  alwaysSucceeds=FALSE;alwaysFails=FALSE;
  checkSTR=FALSE;checkINT=FALSE;checkWIS=FALSE;checkDEX=FALSE;checkCON=FALSE;checkCHA=FALSE;
  checkPP=FALSE;checkOL=FALSE;checkFT=FALSE;checkMS=FALSE;checkHS=FALSE;checkHN=FALSE;
  checkCW=FALSE;checkRL=FALSE;strBonus=0;compareToDie=FALSE;compareDie=0;NbrTries=0;currTry=0;
  successChain=0;failChain=0;successAction=NoAction;failAction=NoAction;
  successTransferData.Clear(); failTransferData.Clear(); 
}
void SPECIAL_ITEM_KEY_EVENT_DATA::Clear() 
{
  GameEvent::Clear();
  
  items.Clear();
  forceExit=FALSE;
  WaitForReturn=TRUE;
}
void QUEST_EVENT_DATA::Clear() 
{
  GameEvent::Clear();
  
  accept=QA_OnYes;
  completeOnAccept=FALSE;
  failOnRejection=FALSE;
  m_quest=-1;
  stage=1;
  acceptChain=0;
  rejectChain=0;
}
void PASS_TIME_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  PassSilent=FALSE;
  AllowStop=FALSE;
  SetTime=FALSE;
  Days=0;Hours=0;Minutes=0;
}
void PASSING_TIME_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  AllowStop=FALSE;
  daysLeft=0;hoursLeft=0;minutesLeft=0; 
}
void VAULT_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  WhichVault=0;
  ForceBackup=TRUE; 
}
void TAVERN::Clear() 
{ 
  GameEvent::Clear();
  
  forceExit=TRUE;
  allowFights=TRUE;
  allowDrinks=FALSE;
  inflation=0;
  barkeep=0;
  fightChain=0;
  drinkChain=0;
  drinkPointTrigger=0;
  taleOrder=InOrderTales;
  EachTaleOnceOnly=FALSE;
  for (int t=0;t<MAX_TALES;t++) 
    tales[t].Clear();
  for (int d=0;d<MAX_DRINKS;d++) 
    drinks[d].Clear(); 
}
void TRAININGHALL::Clear() 
{ 
  GameEvent::Clear();
  
  forceExit=FALSE;
  trainableBaseclasses.RemoveAll();
  //TrainCleric=TRUE;TrainFighter=TRUE;TrainMagicUser=TRUE;
  //TrainThief=TRUE; TrainRanger=TRUE; TrainPaladin=TRUE; TrainDruid=TRUE;
  {
    int i, n;
    n = baseclassData.GetBaseclassCount();
    for (i=0; i<n; i++)
    {
      const BASE_CLASS_DATA *pBaseclass;
      TRAINABLE_BASECLASS trainableBaseclass;
      pBaseclass = baseclassData.PeekBaseclass(i);
      trainableBaseclass.baseclassID = pBaseclass->m_name;
      trainableBaseclass.minLevel = 1;
      trainableBaseclass.maxLevel = 40;
      Add(trainableBaseclass);
    };
  };

  Cost=2000; 
}
void SHOP::Clear() 
{ 
  GameEvent::Clear();
  
  sellprice=0;
  buybackPercentage=50;
  costToId=20;
  forceExit=TRUE;
  itemsAvail.Clear();
  costFactor=Normal;
  canId=canApprGems=canApprJewels=TRUE;
  buyItemsSoldOnly=FALSE;
}
void SMALL_TOWN_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  
  TempleChain=0; 
  ShopChain=0;
  InnChain=0; 
  TavernChain=0; 
  VaultChain=0; 
  TrainingHallChain=0; 
}
void RANDOM_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  
  for (int i=0;i<MAX_RANDOM_EVENTS;i++) 
  {
    EventChains[i]=0; 
    EventChance[i]=0; 
  }
}
void CHAIN_EVENT::Clear() 
{ 
  GameEvent::Clear(); 
  
  Chain=0; 
}
void JOURNAL_EVENT::Clear() 
{ 
  GameEvent::Clear(); 
  
  journal_entry=-1; 
}
void SOUND_EVENT::Clear() 
{ 
  GameEvent::Clear(); 
  
  sounds.RemoveAll(); 
}