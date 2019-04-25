#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

extern const char *SurfaceTypeTexts[];
extern int currentLevelIndex; // level index starts at zero
extern CArray<ARTMAP, ARTMAP&> artMap;

void EVENT_CONTROL::Read(JReader& event)
{
  int i[2] = {0,0};
  CString chainCondition, triggerCondition;
  event.NameAndBool       ("do_only_once",     onceOnly);
  {
    chainTrigger = AlwaysChain;
    event.Optional(); event.NameAndValue ("chained_event_follow", chainCondition);
    event.NameAndValue("chained_event_follow", chainCondition);
    if (chainCondition == "always") chainTrigger = AlwaysChain;
    else if (chainCondition == "if happens") chainTrigger = IfEventHappen;
    else if (chainCondition == "if does not happen") chainTrigger = IfEventNotHappen;
  };

  eventTrigger = AlwaysTrigger;
  event.Optional(); event.NameAndValue ("trigger", triggerCondition);
  if      (triggerCondition == "night time") eventTrigger = Nighttime;
  else if (triggerCondition == "day time")   eventTrigger = Daytime; 
  else if (triggerCondition == "always") eventTrigger = AlwaysTrigger;
  else if (triggerCondition == "party has special item") eventTrigger = PartyHaveSpecialItem;
  else if (triggerCondition == "party does not have special item") eventTrigger = PartyNotHaveSpecialItem;
  else if (triggerCondition == "random percentage") eventTrigger = RandomChance;
  else if (triggerCondition == "party is searching") eventTrigger = PartySearching;
  else if (triggerCondition == "party is not searching") eventTrigger = PartyNotSearching;
  else if (triggerCondition == "facing direction") eventTrigger = FacingDirection;
  else if (triggerCondition == "special quest complete") eventTrigger = QuestComplete;
  else if (triggerCondition == "special quest failed") eventTrigger = QuestFailed;
  else if (triggerCondition == "special quest in progress") eventTrigger = QuestInProgress;
  else if (triggerCondition == "party is detecting traps") eventTrigger = PartyDetectingTraps;
  else if (triggerCondition == "party is not detecting traps") eventTrigger = PartyNotDetectingTraps;
  else if (triggerCondition == "party can see invisible") eventTrigger = PartySeeInvisible;
  else if (triggerCondition == "party cannot see invisible") eventTrigger = PartyNotSeeInvisible;
  else if (triggerCondition == "party has class") eventTrigger = ClassInParty;
  else if (triggerCondition == "party has race") eventTrigger = RaceInParty;
  else event.PrintError("Unknown event trigger type");

  chance = 100;

  switch (eventTrigger)
  {
  case FacingDirectionAnyTime:
  case FacingDirection:
    {
      CString f;
      event.NameAndValue("trigger_data", f);
      if      (f == "East")  facing = East;
      else if (f == "West")  facing = West;
      else if (f == "North") facing = North;
      else if (f == "South") facing = South;
      else if (f == "East, South, West") facing = W_S_E;
      else if (f == "North, East") facing = N_E;
      else if (f == "North, South") facing = N_S;
      else if (f == "East, South") facing = S_E;
      else if (f == "North, East, South") facing = N_S_E;
      else if (f == "North, West") facing = N_W;
      else if (f == "East, West") facing = E_W;
      else if (f == "North, East, West") facing = N_W_E;
      else if (f == "South, West") facing = S_W;
      else if (f == "North, South, West") facing = N_S_W;
      else if (f == "North, East, South, West") facing = Any;
      else event.PrintError("Unrecognized trigger facing direction");
      break;
/*
           Any, North, South, East, West,
   N_S, N_E, N_W, S_E, S_W, E_W,
   N_S_E, N_S_W, N_W_E, W_S_E, InFront
   */
    };
  case RaceInParty:
  case RaceNotInParty:
    {
      event.NameAndQuotedNonBlank("trigger_data",     raceID);
    };
    break;
  case ClassInParty:
  case ClassNotInParty:
    {
      event.NameAndQuotedNonBlank("trigger_data",    classBaseclassID.classID);
    };
    break;
  case BaseclassInParty:
  case BaseclassNotInParty:
    {
      event.NameAndQuotedNonBlank("trigger_data",    classBaseclassID.baseclassID);
    };
    break;
  case PartyHaveItem:
  case PartyNotHaveItem:
    {
      event.NameAndQuotedNonBlank("trigger_data",    itemID);
    }
    break;
  case NPCInParty:
  case NPCNotInParty:
    {
      event.NameAndQuotedNonBlank("trigger_data",    characterID);
    };
  case SpellMemorized:
    {
      event.NameAndQuotedNonBlank("trigger_data",        memorizedSpellID);
    };
  case GenderInParty:
  case GenderNotInParty:
    {
      event.Optional(); event.NameAndEnum("trigger_data",  gender);
    };
    break;
  case QuestComplete:
  case QuestFailed:
  case QuestInProgress:
    {
      CString temp;
      event.NameAndValue   ("trigger_data",        temp);
      if (temp.Left(14) == "special quest ")
      {
        sscanf(temp.GetBuffer(17)+14, "%d", &quest);
      }
      else
      {
        event.PrintError("Cannot determine special quest number");
      };
    };
    break;
  case RandomChance:
    {
      event.Optional(); event.NameAndValue("trigger_data", chance);
    }
    break;
  case AlwaysTrigger:
  case Daytime:
  case Nighttime:
  case PartySearching:
  case PartyNotSearching:
  case PartyDetectingTraps:
  case PartyNotDetectingTraps:
  case PartySeeInvisible:
  case PartyNotSeeInvisible:
    break;
  case PartyHaveSpecialItem:
    {
      CString temp, type;
      int col, index;
      event.NameAndValue("trigger_data", temp);
      if (temp.Left(11) == "special key")
      {
        type = "key";
        col = 12;
        sscanf(temp.GetBuffer(col+3) + col, "%d", &index);
        specialKey = index;
      }
      else if (temp.Left(12) == "special item")
      {
        type = "item";
        col = 13;
        sscanf(temp.GetBuffer(col+3) + col, "%d", &index);
        specialItem = index;
      }
      else if (temp.Left(13) == "special quest")
      {
        type = "quest";
        col = 14;
        sscanf(temp.GetBuffer(col+3) + col, "%d", &index);
        quest = index;
      }
      else
      {
        type = "key";
        col = 0;
      };
    };
    break;
  default:
    {
      event.PrintError("Cannot determine event trigger type");
    };
  };
  event.Optional(); memorizedSpellClass = 0;
  event.NameAndValue         (JKEY_SPELLCLASS,   memorizedSpellClass);
  event.Optional(); memorizedSpellLevel = 0;
  event.NameAndValue         (JKEY_SPELLLEVEL,   memorizedSpellLevel);
  gpdlIsBinary = false;
  event.Optional(); event.NameAndBool(JKEY_GPDLBINARY,   gpdlIsBinary);
  gpdlData.Empty();
  event.Optional();
  if (gpdlIsBinary)
  {
    event.NameAndBinary      (JKEY_GPDLDATA,     gpdlData);
  }
  else
  {
    event.NameAndValue       (JKEY_GPDLDATA,     gpdlData);
  };
  //event.NameAndNonNegative   (JKEY_SPECIALITEM,  specialItem);
  //event.NameAndNonNegative   (JKEY_SPECIALKEY,   specialKey);
  if (eventTrigger == PartyAtXY)
  {
    event.NameAndValue(JKEY_PARTYXY, i, 2);
  };
  partyX = i[0];
  partyY = i[1];
  //eventcontrol_asl.Import(event);
  //event.EndList();
  return;
}

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
      jw.NameAndQuotedNonBlank(JKEY_GPDLDATA,   (LPCSTR)gpdlData);
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
  //eventcontrol_asl.Export(jw);
  jw.EndList();
}


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

void TRANSFER_DATA::Read(JReader& jr, const char *exKey, const char *xKey, const char *yKey, const char *fKey)
{
  int i[2];
  //jr.NameAndBool("dest_event_execute", execEvent);
  jr.NameAndBool(exKey, execEvent);
  destEP = -1;
  jr.Optional(); jr.NameAndNonNegative("dest_entry_point", destEP);
  jr.Optional(); jr.NameAndNonNegative("dest_entry", destEP);
  destLevel = 0;
  jr.NameAndValue("dest_level", destLevel);
  destLevel--; // Internal representation.
  if (destLevel < 0)
  {
    destLevel = currentLevelIndex;
  };
  jr.NameAndValue(xKey, i[0]);
  jr.NameAndValue(yKey, i[1]);
  destX = i[0];
  destY = i[1];
  {
    CString facing;
    m_facing = FaceUnchanged;
    jr.NameAndValue(fKey, facing);
	  if (facing == "north") m_facing = FaceNorth;
	  if (facing == "east")  m_facing = FaceEast;
	  if (facing == "south") m_facing = FaceSouth;
	  if (facing == "west")  m_facing = FaceWest;
  };
  //jr.EndList(DBGID_transfer);
}



void WallSetSlotMemType::Clear()
{ 
  SlotType = DEFAULT_FORMAT_TYPE;
  wallHash=0;
  doorHash=0;
  overlayHash=0;
  AVArtHash=0;
  wallFile="";doorFile="";overlayFile="";soundFile="";
  used=FALSE;
  doorFirst=FALSE;
  drawAV=TRUE;
  //UnlockSpell.Clear();UnlockSpellClass=0;UnlockSpellLevel=0;
  UnlockSpellName.Empty();
  BlendOverlay=FALSE;
  BlendAmount=0;
  AVArtTried=false;
  AVZoneStyle=FALSE;
  AVArtFile="";
};

void WallSetSlotMemType::Export(JWriter& jw)
{
  jw.StartList();
  jw.NameAndValue("wallFile"     , wallFile);
  jw.NameAndValue("wallFileName",  wallFileName);
  jw.NameAndValue("doorFile"     , doorFile);
  if (overlayFile != "")
  {
    int kkk=1;
  };
  jw.NameAndValue("overlayFile"  , overlayFile);
  jw.NameAndValue("soundFile"    , soundFile);
  jw.NameAndValue("avArtFile"    , AVArtFile);
  jw.NameAndBool ("used"         , true);
  jw.NameAndBool ("doorFirst"    , false);
  jw.NameAndBool ("drawAV"       , true);
  jw.NameAndValue("unlockSpellID", "");
  jw.NameAndValue("blendOverlay" , 0);
  jw.NameAndValue("blendAmount"  , 0);
  jw.EndList();
}

void BackgroundSlotMemType::Export(JWriter& jw)
{
  jw.StartList();
  jw.NameAndValue("backgroundFile"   , backgroundFile);
  jw.NameAndValue("backgroundFileAlt", "");
  jw.NameAndValue("soundFile"        , "");
  jw.NameAndBool ("suppressStepSound", false);
  jw.NameAndBool ("used"             , false);
  jw.NameAndValue("startTime"        , 0);
  jw.NameAndValue("endTime"          , 0);
  jw.NameAndBool ("useAltBackground" , false);
  jw.NameAndBool ("useAlphaBlend"    , false);
  jw.NameAndValue("alphaBlendPercent", 0);
  jw.NameAndBool( "useTransparency"  , false);
  jw.EndList();
}

void BackgroundSlotMemType::Clear(void)
{
  SlotType = DEFAULT_FORMAT_TYPE;
  suppressStepSound=FALSE;
  useAltBackground=FALSE; 
  backgroundSurface=backgroundSurfaceAlt=hsound-1; 
  backgroundFileAlt="";backgroundFile="";soundFile="";
  used=FALSE; 
  AlphaBlendPcnt=0;
  useAlphaBlend=FALSE;
  useTransparency = FALSE;
  backgroundFileHash=0;
  backgroundFileAltHash=0;
  memset(&surfRect, 0, sizeof(surfRect));
}

void PIC_DATA::Read(JReader& jr, const char *name)
{
  if (!jr.StartList(name, DBGID_picData)) return;
  jr.NameAndEnum(JKEY_PICTYPE, picType);
  jr.NameAndValue(JKEY_FILENAME, filename);
  jr.NameAndValue(JKEY_TIMEDELAY, timeDelay);
  jr.NameAndValue(JKEY_NUMFRAME, NumFrames);
  jr.NameAndValue(JKEY_WIDTH, FrameWidth);
  jr.NameAndValue(JKEY_HEIGHT, FrameHeight);   
  jr.NameAndValue(JKEY_FLAGS, flags);
  jr.NameAndValue(JKEY_MAXLOOPS, MaxLoops);
  jr.NameAndValue(JKEY_STYLE, style);
  jr.NameAndBool(JKEY_ALLOWCENTERING, AllowCentering);
    // no need to save LoopCounter
  jr.NameAndBool(JKEY_USEALPHA, UseAlpha);
  jr.NameAndValue(JKEY_ALPHA, AlphaValue);    
  jr.EndList(DBGID_picData);
};

void PIC_DATA::ReadSmallPicFile(JReader& jr, const char *name)
{
  jr.Optional();
  filename = jr.Fetch_String(name);
  if (filename.IsEmpty()) picType = BogusDib;
  else picType = SmallPicDib;
}

void PIC_DATA::Export(JWriter& jw, const char *name)
{
  if (picType == BogusDib) return;
  jw.StartList(name);
  jw.NameAndEnum(JKEY_PICTYPE, picType);
  if ( filename == "none" ) 
  {
    picType = BogusDib;
    filename = "";
  }
  else
  {
    int i;
    for (i=0; i<artMap.GetSize(); i++)
    {
      if (artMap[i].strings[0] == filename)
      {
        if (artMap[i].strings.GetSize() < 2)
        {
          printf("Cannot map picture for '%s'\n", filename);
        }
        else
        {
          filename = artMap[i].strings[1];
        };
        break;
      };
    };
    if (i == artMap.GetSize())
    {
      printf("Unable to map graphic for picture '%s'\n",filename);
    };
  };
  jw.NameAndValue(JKEY_FILENAME, filename);
  jw.NameAndValue(JKEY_TIMEDELAY, timeDelay);
  jw.NameAndValue(JKEY_NUMFRAME, NumFrames);
  jw.NameAndValue(JKEY_WIDTH, FrameWidth);
  jw.NameAndValue(JKEY_HEIGHT, FrameHeight);   
  jw.NameAndValue(JKEY_FLAGS, flags);
  jw.NameAndValue(JKEY_MAXLOOPS, MaxLoops);
  jw.NameAndValue(JKEY_STYLE, style);
  jw.NameAndBool(JKEY_ALLOWCENTERING, AllowCentering);
    // no need to save LoopCounter
  jw.NameAndBool(JKEY_USEALPHA, UseAlpha);
  jw.NameAndValue(JKEY_ALPHA, AlphaValue);    
  jw.EndList();
};

void LEVELDATA::operator =(const LEVELDATA& levelData)
{
  levelNum = levelData.levelNum;
  entryPoints.RemoveAll();
  entryPoints.Append(levelData.entryPoints);
}


void MONSTER_EVENT_DATA::Export(JWriter& jw)
{
  int count, i;
  monsters.FreeExtra();
  count = monsters.GetSize();
  if (count == 0) return;
  jw.StartArray(JKEY_MONSTERS);
  for (i=0;i<count;i++)
  {
    if (monsters[i].monsterID == "NONE") continue;
    jw.NextEntry();
    monsters[i].Export(jw);
  };
  jw.EndArray();
}

void MONSTER_EVENT_DATA::Read(JReader& jr)
{
  if (jr.Optional(), !jr.StartArray(JKEY_MONSTERS))return;
  MONSTER_EVENT monsterEvent;
  while (monsterEvent.Read(jr))
  {
    monsters.Add(monsterEvent);
  };
  jr.EndArray();
  return;
}



void MONEY_SACK::Export(JWriter& jw)
{
  int i, j, k;
  jw.StartList(JKEY_MONEY);
  {
    //for (i=0;i<NumCoinTypes();i++)
    for (i=0;i<MONEY_DATA_TYPE::MAX_COIN_TYPES;i++)
    {
      if (Coins[i] != 0) break;
    };
    if (i != MONEY_DATA_TYPE::MAX_COIN_TYPES)
    {
      jw.NameAndValue(JKEY_COINS, Coins, MONEY_DATA_TYPE::MAX_COIN_TYPES);
    };
    {
      //pos = Gems.GetHeadPosition();
      jw.StartArray(JKEY_GEMS);
      k = 0;
      for (i=0; i<Gems.GetSize(); i++)
      //while (pos != NULL)
      {
        GEM_TYPE *pGem;
        pGem = &Gems[i];
        for (j=0; j<pGem->value; j++)
        {
          jw.NextEntry();
          jw.Linefeed(false);
          jw.StartList();
          jw.NameAndValue("id", k);
          jw.NameAndValue("value", 1);
          k++;
          jw.EndList();
          jw.Linefeed(true);
        };
      };
      jw.EndArray();
    };
    {
      jw.StartArray(JKEY_JEWELS);
      //pos = Jewelry.GetHeadPosition();
      //while (pos != NULL)
      k = 0;
      for (i=0; i<Jewelry.GetSize(); i++)
      {
        GEM_TYPE *pGem;
        pGem = &Jewelry[i];
        for (j=0; j<pGem->value; j++)
        {
          jw.NextEntry();
          jw.Linefeed(false);
          jw.StartList();
          jw.NameAndValue("id", k);
          jw.NameAndValue("value", 1);
          k++;
          jw.EndList();
          jw.Linefeed(true);
        };
      };
      jw.EndArray();
    };
  };
  jw.EndList();
}

void MONEY_SACK::Read(JReader& jr)
{
  jr.StartList(JKEY_MONEY, DBGID_moneySack);
  memset(Coins,0, sizeof(Coins));
  jr.Optional(); jr.NameAndValue(JKEY_COINS, Coins, MONEY_DATA_TYPE::MAX_COIN_TYPES);

  {
    GEM_TYPE gem;
    jr.StartArray(JKEY_GEMS);
    while (gem.Read(jr))
    {
      Gems.Add(gem);
    };
    jr.EndArray();
  };
  {
    GEM_TYPE jewel;
    jr.StartArray(JKEY_JEWELS);
    while (jewel.Read(jr))
    {
      Jewelry.Add(jewel);
    };
    jr.EndArray();
  };
  jr.EndList(DBGID_moneySack);
  return;
}

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
/*
int TOUR_STEP::Read(JReader& jr, int stepNum)
{
  CString key;
  CString value;
  key.Format("step_%02d_action", stepNum);
  jr.NameAndValue(key, value);
  if      (value == "Right")   step = TStep_Right;
  else if (value == "Left")    step = TStep_Left;
  else if (value == "Forward") step = TStep_Forward;
  else if (value == "none")    step = TStep_NoMove;
  else 
  {
    jr.PrintError("Unknown Tour step type");
    step = TStep_NoMove;
  };
  return stepNum;
}
*/

void ITEM_LIST::Export(JWriter& jw, const char *name)
{
  //CObject::Serialize(ar);
  ITEM data;
  jw.StartArray(name);
  {
    int i;
    for (i=0; i<m_items.GetSize(); i++)
    {
      ITEM *pItem;
      pItem = &m_items[i];
      jw.NextEntry();
      pItem->Export(jw);
    };
  };
  jw.EndArray();
  //No longer needed!   rdyItems.Export(jw);
}
void ITEM_LIST::Read(JReader& jr, const char *name)
{
  //CObject::Serialize(ar);
  ITEM data;
  int key;
  key = 1;
  if (jr.Optional(), !jr.StartArray(name))return;
  while (jr.Optional(), jr.NextEntry())
  {
    data.Read(jr);
    data.key = key++;
    m_items.Add(data);
  };
  jr.EndArray();
  //No longer needed!   rdyItems.Export(jw);
  return;
}


void BACKGROUND_SOUND_DATA::Export(JWriter& jw)
{
  jw.StartList(JKEY_SOUNDS);
  backgroundSounds.Export(jw, JKEY_BACKGROUNDSOUNDS);
  nightSounds.Export(jw, JKEY_NIGHTSOUNDS);
  jw.NameAndBool(JKEY_USENIGHTMUSIC, UseNightMusic);
  jw.NameAndValue(JKEY_STARTTIME, StartTime);
  jw.NameAndValue(JKEY_ENDTIME, EndTime);
  jw.EndList();
}
void BACKGROUND_SOUND_DATA::Read(JReader& jr)
{
  if (!jr.StartList(JKEY_SOUNDS, DBGID_backgroundSound)) return;
  backgroundSounds.Read(jr, JKEY_BACKGROUNDSOUNDS);
  nightSounds.Read(jr, JKEY_NIGHTSOUNDS);
  jr.NameAndBool(JKEY_USENIGHTMUSIC, UseNightMusic);
  jr.NameAndValue(JKEY_STARTTIME, StartTime);
  jr.NameAndValue(JKEY_ENDTIME, EndTime);
  jr.EndList(DBGID_backgroundSound);
  return;
}

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

bool QBUTTON_DATA::Read(JReader& jr)
{
  int i;
  if (jr.Optional(), !jr.StartArray(JKEY_BUTTONLIST))return false;
  i = 0;
  while (jr.Optional(), jr.NextEntry())
  {
    QBUTTON_OPTION but;
    but.Read(jr);
    if (i < MAX_BUTTONS)
    {
      buttons[i++] = but;
    };
    numListButtons = i;
  };
  jr.EndArray();
  return true;
}

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

void QLIST_DATA::Read(JReader& jr)
{
  //jr.NameAndValue(JKEY_TITLE, title);
  jr.NameAndValue(JKEY_TEXT, title);
  //jr.NameAndValue(JKEY_NUMBUTTON, numListButtons);
  numListButtons = MAX_BUTTONS;
  //jr.StartArray(JKEY_OPTIONS);
  for (int i=0;i<MAX_BUTTONS;i++)
  {
    CString tag;
    buttons[i].Read(jr, i);
  };
  //jr.EndArray();
  return;
}


void spellBookType::Clear()
{
  spellLimits.Clear();
  list.Clear();
  spellPrepTimeNeeded=0;
  spellPrepTimeUsed=0;
}


void PIC_DATA::Clear()
{ 
   filename="";
   timeDelay=0;
   key=-1;
   picType=BogusDib; 
   NumFrames=0;
   FrameWidth=0;
   FrameHeight=0;
   style=AS_None;
   lastTime=0;
   sx = 0;
   sy = 0;
   flags=AF_Loop;
   MaxLoops=1;
   LoopCounter=0;
   AllowCentering=TRUE;
   UseAlpha=FALSE;
   AlphaValue=0;   
}

void EVENT_CONTROL::Clear()
{
  onceOnly = FALSE;
  chainTrigger = AlwaysChain;
  eventTrigger = AlwaysTrigger;
  facing = Any;
  //race = Human;
  raceID.Empty();
  //charClass = Cleric;
  classBaseclassID.classID.Empty();
  classBaseclassID.baseclassID.Empty();
  //m_giID.Clear();
  itemID.Empty();
  quest = -1;
  chance = 100;
  gender=Male;
  specialItem = -1;
  specialKey = -1;
  //npc=-1;
  characterID.Empty();
  gpdlData="";
  gpdlIsBinary=false;
  partyX=partyY=-1;
  //memorizedSpellDbKey.Clear();
  memorizedSpellID.Empty();
  memorizedSpellClass = 0;
  memorizedSpellLevel = 0;
}



void GameEvent::Clear()
{   
  evType = NoEvent;
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
  timer = 0;
}


void spellBookType::Export(JWriter& jw)
{
  jw.StartList(JKEY_SPELLBOOK);
  spellLimits.Export(jw);
  list.Export(jw);
  jw.EndList();
}

bool spellBookType::Read(JReader& jr)
{
  if (jr.Optional(), !jr.StartList(JKEY_SPELLBOOK, DBGID_spellbook)) return false;
  spellLimits.Read(jr);
  list.Read(jr);
  jr.EndList(DBGID_spellbook);
  return true;
}


/*
               "specialObjects":[
                  "specialObject":{
                     "type":"key",
                     "operation":"give",
                     "index":"1",
                     "itemID":"0"
                  },
                  "specialObject":{
                     "type":"key",
                     "operation":"take",
                     "index":"2",
                     "itemID":"1"
                  }
               ]
*/


void SPECIAL_OBJECT_EVENT_LIST::Export(JWriter& jw)
{
  int i;
  jw.StartArray("specialItems");
  for (i=0;i<list.GetSize(); i++)
  {
    jw.NextEntry();
    list[i].Export(jw);
  };
  jw.EndArray();
}

const char *JKEY_SPECIALITEMS = "specialItems";

bool SPECIAL_OBJECT_EVENT_LIST::Read(JReader& jr)
{
  SPECIAL_OBJECT_EVENT_DATA data;
  //jr.Optional();
  //if(!jr.StartArray(JKEY_SPECIALITEMS)) return false;
  //while (data.Read(jr))
  //{
    data.Read(jr);
    list.Add(data);
  //};
  //jr.EndArray();
  return true;
}

void GEM_TYPE::Export(JWriter& jw)
{
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_ID, id);
  jw.NameAndValue(JKEY_VALUE, value);
  jw.EndList();
  jw.Linefeed(true);
}

bool GEM_TYPE::Read(JReader& jr)
{
  jr.Optional();
  if (!jr.NextEntry())
  {
    return false;
  };
  jr.StartList(DBGID_gemType);
  jr.Linefeed(false);
  jr.NameAndValue(JKEY_ID, id);
  jr.NameAndValue(JKEY_VALUE, value);
  jr.EndList(DBGID_gemType);
  jr.Linefeed(true);
  return true;
}

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

bool FindItem(const CString& name);

bool ITEM::Read(JReader& jr)
{
  CString FRUA_id;
  if (jr.Optional(), !jr.StartList(DBGID_item)) return false;
  jr.NameAndValue ("name"     ,itemID);
  jr.NameAndValue("id", FRUA_id);
  if (!FindItem(itemID))
  {
    itemID += '|' + FRUA_id;
  };
  ready = 0;      //jr.NameAndValue (JKEY_READY      ,ready);
  qty = 1;        //jr.NameAndValue ("quantity"   ,qty);
  key = 0;        //jr.NameAndValue (JKEY_KEY        ,key);
  identified = 0; //jr.NameAndValue (JKEY_IDENTIFIED ,identified);
  charges = 1;    //jr.NameAndValue (JKEY_CHARGES    ,charges);
  cursed =0;      //jr.NameAndValue (JKEY_CURSED     ,cursed);
  paid = 0;       //jr.NameAndValue (JKEY_PAID       ,paid);
  jr.EndList(DBGID_item);
  return true;
}


void BACKGROUND_SOUNDS::Export(JWriter& jw, const char *name)
{
  POSITION pos;
  jw.StartList(name);
  pos = sounds.GetHeadPosition();
  while (pos != NULL)
  {
    CString *pSound;
    pSound = &sounds.GetNext(pos);
    jw.NameAndValue(JKEY_SOUND, *pSound);
  };
  jw.EndList();
}

bool BACKGROUND_SOUNDS::Read(JReader& jr, const char *name)
{
  CString sound;
  if(jr.Optional(), !jr.StartList(name, DBGID_sound)) return false;
  while (jr.Optional(), jr.NameAndValue(JKEY_SOUND, sound))
  {
    sounds.AddTail(sound);
  };
  jr.EndList(DBGID_sound);
  return true;
}

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

bool QBUTTON_OPTION::Read(JReader& jr)
{
  jr.StartList(NULL, DBGID_qbutton);
  jr.Linefeed(false);
  jr.NameAndBool(JKEY_PRESENT, present);
  jr.NameAndValue(JKEY_LABEL, label);
  jr.NameAndEnum(JKEY_POSTCHAINACTION, postChainAction);
  jr.NameAndValue(JKEY_CHAIN, chain);
  jr.EndList(DBGID_qbutton);
  jr.Linefeed(true);
  return true;
}

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

bool QLIST_OPTION::Read(JReader& jr, int i)
{
  CString tag;
  bool val;
  i++;

  tag.Format("answer_%d", i);
  jr.NameAndValue(tag, label);

  //jr.NameAndBool(JKEY_PRESENT, present);
  //jr.NameAndEnum(JKEY_POSTCHAINACTION, postChainAction);

  postChainAction = LabelDoNothing;

  tag.Format("answer_%d_chain_event", i);
  jr.NameAndValue(tag, chain);

  tag.Format("answer_%d_force_backup_one_step", i);
  jr.NameAndBool(tag, val);
  if (val) postChainAction = LabelBackupOneStep;

  tag.Format("answer_%d_return_to_question", i);
  jr.NameAndBool(tag, val);
  if (val) postChainAction = LabelReturnToQuestion;

  present = !label.IsEmpty();

  return true;
}

void SPELL_LIST::Clear() 
{ 
  spells.RemoveAll();
}

void SPELL_LIST::Export(JWriter& jw)
{
  int i,n;
  jw.StartArray(JKEY_SPELLLIST);
  n = spells.GetSize();
  for (i=0; i<n; i++)
  {
    jw.NextEntry();
    spells[i].Export(jw);
  };
  jw.EndArray();
}

bool SPELL_LIST::Read(JReader& jr)
{
  if (jr.Optional(), !jr.StartArray(JKEY_SPELLLIST)) return false;
  CHARACTER_SPELL spell;
  while (jr.Optional(), jr.NextEntry())
  {
    spell.Read(jr);
    spells.Add(spell);
  };
  jr.EndArray();
  return true;
}

void spellLimitsType::Clear()
{
  UseLimits=TRUE;
}

void spellLimitsType::Export(JWriter& jw)
{
  jw.NameAndBool(JKEY_USELIMITS, UseLimits);
}

bool spellLimitsType::Read(JReader& jr)
{
  jr.NameAndBool(JKEY_USELIMITS, UseLimits);
  return true;
}

/*
               "specialObjects":[
                  "specialObject":{
                     "type":"key",
                     "operation":"give",
                     "index":"1",
                     "itemID":"0"
                  },
                  "specialObject":{
                     "type":"key",
                     "operation":"take",
                     "index":"2",
                     "itemID":"1"
                  }
               ]
*/


void SPECIAL_OBJECT_EVENT_DATA::Export(JWriter& jw)
{
  CString temp;
  //jw.NameAndValue(JKEY_ITEMTYPE , ItemType);
  jw.StartList();
  jw.NameAndValue("index", index);
  if      (ItemType & ITEM_FLAG)  temp = "item";
  else if (ItemType & KEY_FLAG)   temp = "key";
  else if (ItemType & QUEST_FLAG) temp = "quest";
  else temp = "";
  jw.NameAndValue("type", temp);

  if      (operation & SPECIAL_OBJECT_GIVE) temp = "give";
  else if (operation & SPECIAL_OBJECT_TAKE) temp = "take";
  else temp = "";
  
  jw.NameAndValue(JKEY_OPERATION, temp );
  jw.NameAndValue("itemID"      , id);
  jw.EndList();
}

int SPECIAL_OBJECT_EVENT_DATA::Read(JReader& jr)
{
  //if (!jr.StartList(JKEY_SPECIALITEM, DBGID_specialItem)) return -1;
  CString temp;

  ItemType = 0;
  index = -1;
  jr.NameAndValue("special_item_key_quest", temp);
  if (temp.Left(12) == "special item") 
  {
    ItemType |= ITEM_FLAG;
    sscanf(temp.Right(temp.GetLength()-12), "%d", &index);
  }
  else if (temp.Left(13) == "special quest")
  {
    ItemType |= QUEST_FLAG;
    sscanf(temp.Right(temp.GetLength()-13), "%d", &index);
  }
  else if (temp.Left(11) == "special key")
  {
    ItemType |= KEY_FLAG;
    sscanf(temp.Right(temp.GetLength()-11), "%d", &index);
  }
  else
  {
    jr.PrintError("Unable to determine special object type");
  };

  operation = 0;
  jr.NameAndValue("action", temp);
  if      (temp == "give to party")   operation = SPECIAL_OBJECT_GIVE;
  else if (temp == "take from party") operation = SPECIAL_OBJECT_TAKE;
  else
  {
    jr.PrintError("Cannot determine special object operation");
  };

  id = 0;
  //.NameAndValue(JKEY_ITEMID, id);
  //jr.EndList(DBGID_specialItem);
  return index;
}

void CHARACTER_SPELL::Export(JWriter& jw)
{
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_NAME,spellID);
  jw.NameAndValue(JKEY_MEMORIZED,memorized);
  jw.NameAndValue(JKEY_LEVEL,level);
  jw.NameAndValue(JKEY_SELECTED,selected);
  jw.EndList();
  jw.Linefeed(true);
}

bool CHARACTER_SPELL::Read(JReader& jr)
{
  if (!jr.StartList(DBGID_characterSpell)) return false;
  jr.Linefeed(false);
  jr.NameAndValue(JKEY_NAME,spellID);
  jr.NameAndValue(JKEY_MEMORIZED,memorized);
  jr.NameAndValue(JKEY_LEVEL,level);
  jr.NameAndValue(JKEY_SELECTED,selected);
  jr.EndList(DBGID_characterSpell);
  jr.Linefeed(true);
  return true;
}

void MONSTER_EVENT::operator =(const MONSTER_EVENT& src)
{
  if (&src == this)
    return;
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
  return;
}

void ITEM_LIST::operator=(const ITEM_LIST& src)
{
  if (&src == this) return;
  m_items.RemoveAll();
  m_items.Append(src.m_items);
  //rdyItems = src.rdyItems;
  return;
}

void MONEY_SACK::Clear() 
{ 
  memset(Coins,0,sizeof(Coins));
  Jewelry.RemoveAll();
  Gems.RemoveAll(); 
}

void MONEY_SACK::operator=(const MONEY_SACK &rhs)
{
  if (&rhs==this) return;
  Clear();
  Gems.RemoveAll();
  Jewelry.RemoveAll();
  Gems.Append(rhs.Gems);
  Jewelry.Append(rhs.Jewelry);
  for (int i=0;i<MONEY_DATA_TYPE::MAX_COIN_TYPES;i++)
    Coins[i] = rhs.Coins[i];
}


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

bool ENCOUNTER_BUTTON_DATA::Read(JReader& jr)
{
   //CObject::Serialize(ar);
  int i;
  for (i=1; i<=5; i++)
  {
    {
      buttons[i-1].Import(jr, i);
    };
  };
  return true;
}

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

void ENCOUNTER_BUTTON_OPTION::Import(JReader& jr, int i)
{
  char name[200];
  CString value;
  sprintf(name, "button_%d_action", i);
  jr.NameAndValue(name, value);
  if      (value == "decrease range")                            optionResult = DecreaseRange;
  else if (value == "combat - party surprised if slow party")    optionResult = CombatSlowPartySurprised;
  else if (value == "combat - monsters surprised if fast party") optionResult = CombatSlowMonsterSurprised;
  else if (value == "combat - no surprise")                      optionResult = CombatNoSurprise;
  else if (value == "talk chain")                                optionResult = Talk;
  else if (value == "escape if fast party else combat")          optionResult = EscapeIfFastPartyElseCombat;
  else if (value == "no effect")                                 optionResult = NoResult;
  else                                                           optionResult = NoResult;
/*
   ,  ,
   , ,
   , , 
*/
  sprintf(name,"button_%d_present", i);
  jr.NameAndBool(name, present);

  sprintf(name,"button_%d_allow_close", i);
  jr.NameAndBool(name, allowedUpClose);
  
  //sprintf(name,"button_%d_present", i);
  //jr.NameAndValue(JKEY_CHAIN, chain);
  
  sprintf(name,"button_%d_allow_only_close", i);
  jr.NameAndBool(name, onlyUpClose);

  sprintf(name,"button_%d_label", i);
  jr.NameAndValue(name, label);

  chain = 0;
  
  return;
};


LINE_DATA *LINE_DATABASE::Add(const LINE_DATA& newData)
{
  int n;
  LINE_DATA *pData;
  n = database.GetSize();
  database.SetSize(n+1);
  pData = &database[n];
  pData->lineData.Append(newData.lineData);
  return pData;
}

LINE_DATA *LINE_DATABASE::Find(const char *key, const char *value)
{
  int i, n;
  n = database.GetSize();
  for (i=0; i<n; i++)
  {
    LINE_DATA *pData;
    LINE_ENTRY *pLine;
    pData = &database[i];
    pLine = (*pData)[key];
    if (pLine != NULL)
    {
      if (pLine->value == value)
      {
        return pData;
      };
    };
  };
  return NULL;
}

LINE_DATA *LINE_DATABASE::operator [](int i)
{
  return &database[i];
}

void LINE_DATA::Add(const CString& key, const CString& value)
{
  int n;
  n = lineData.GetSize();
  lineData.SetSize(n+1);
  lineData[n].key = key;
  lineData[n].value = value;
  ASSERT(true);
}

void LINE_DATA::Add(const CString& key, int value)
{
  CString v;
  v.Format("%d", value);
  Add(key, v);
  ASSERT(true);
}

LINE_ENTRY *LINE_DATA::operator [](int i)
{
  return &lineData[i];
}

LINE_ENTRY *LINE_DATA::operator [](const char *key)
{
  int i, n;
  n = lineData.GetSize();
  for (i=0; i<n; i++)
  {
    if (lineData[i].key == key) return &lineData[i];
  };
  return NULL;
}
