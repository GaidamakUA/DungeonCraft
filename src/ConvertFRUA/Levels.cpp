#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

extern CArray<LEVELDATA, LEVELDATA&> levelArray;
extern CELL cells[100][100];
extern int currentLevelIndex; // level index starts at zero
int nxtAvailEventId;
extern CString jsonPath;
int mapWidth, mapHeight;
extern CString strVersion;
extern CString FRUAExportDate, FRUAExportTime;
extern CString ConvertFRUAVersion;
extern CArray<BIG_SMALL_SPRITE_PIC, BIG_SMALL_SPRITE_PIC&> bigSmallSpritePics;

#define CInt(a) (config.Fetch_Int(a))

const char *triggerNames[] =
{
  "day time"  , "Daytime",
  "night time", "Nighttime"
};

const int numTriggerNames = sizeof(triggerNames)/sizeof(triggerNames[0])/2;

extern CArray <ARTMAP, ARTMAP&> artMap;

struct REST_EVENT
{
  int encounter_percent_chance;
  int event_id;
  int id;
  int minute_interval;
  int resting_allowed;
};



void GetWallNames(JReader& artSlots, CString *wallNames)
{
  int i;
  wallNames[0] = "none";
  for (i=0; i<3; i++)
  {
    CString key;
    CString value;
    key.Format("Wall_%d", i+1);
    artSlots.Optional();
    if (artSlots.NameAndValue(key, value))
    {
      int j;
      for (j=0; j<5; j++)
      {
        if (value == "none")
        {
          wallNames[5*i+j+1].Empty();
        }
        else
        {
          wallNames[5*i+j+1].Format("%s - slot %d", value, j+1);
        };
      };
    }
  };
}

CString GetDefaultBackdrop(int overland, int levelNum, int overlandBigPic)
{
  if (overland)
  {
    if (overlandBigPic > 0)
    {
      int n, i;
      n = bigSmallSpritePics.GetSize();
      for (i=0; i<n; i++)
      {
        if (bigSmallSpritePics[i].value == overlandBigPic)
        {
          return bigSmallSpritePics[i].name;
        };
      };
    };
    switch (levelNum)
    {
    case 1: return "Western Cormyr";
    case 2: return "Moonsea Realms";
    case 3: return "Web Realm";
    case 4: return "Southern Ksoh";
    default: return "Moonsea Realms";
    };
  }
  else
  {
    return "";
  };
}

void GetBackdropNames(JReader& artSlots,
                      CString *backdropNames,
                      int overland,
                      int levelNum,
                      int overlandBigPic)
{
  int i;
  backdropNames[0] = "none";
  for (i=0; i<4; i++)
  {
    CString key;
    CString value;
    key.Format("backdrop_%d", i+1);
    artSlots.Optional();
    if (artSlots.NameAndValue(key, value))
    {
      if (value == "none")
      {
        backdropNames[i+1] = GetDefaultBackdrop(overland, levelNum, overlandBigPic);
      }
      else
      {
        backdropNames[i+1] = value;
      };
    }
    else
    {
        backdropNames[i+1] = GetDefaultBackdrop(overland, levelNum, overlandBigPic);
    };
  };
}

void ConvertOneLevel(JReader& global, JReader& level, JWriter& jw)
{
  int levelNum;
  int overland;
  int overlandBigPic;
  LEVELDATA *pLevelData;
  JReader config(level);
  CString wallNames[16]; // 0 through 15
  CString backdropNames[5];
  CString treasurePicName;
  CString campPicName;
  CString indoorCombatArt, outdoorCombatArt;
  CArray <CString, CString&> strings;
  CArray <REST_EVENT, REST_EVENT&> restEventArray;
  levelNum = level.Fetch_Int("id");
  printf("Converting level %d\n", levelNum);
  //jw.Linefeed(false);
  jw.NameAndValue("comment FRUA json line number = ", level.LineNum());
  //jw.NameAndValue("ConvertFRUA version", strVersion);
  jw.Linefeed(true);
  jw.NameAndValue("level", levelNum);
  {
    LEVELDATA levDat;
    int i;
    i = levelArray.Add(levDat);
    pLevelData = &levelArray[i];
    pLevelData->levelNum = levelNum;
  };
  currentLevelIndex = levelNum-1;
  config.StartList("config", DBGID_singleLevelConfig);
  {
    JReader s(level);
    s.StartArray("strings");
    while (s.Optional(), s.NextEntry())
    {
      int index;
      CString str;
      s.StartList(DBGID_singleLevelString);
      index = s.Fetch_Int("id");
      str = s.Fetch_String("string");
      strings.SetAtGrow(index, str);
      s.EndList(DBGID_singleLevelString);
    };
  };
  {
    REST_EVENT re;
    JReader restEvents(config);
    restEvents.StartArray("rest_events");
    while (restEvents.Optional(), restEvents.NextEntry())
    {
      restEvents.StartList(DBGID_restEvent);
      re.encounter_percent_chance = restEvents.Fetch_Int("encounter_percent_chance");
      re.event_id= restEvents.Fetch_Int("event_id");
      re.id= restEvents.Fetch_Int("id");
      re.minute_interval= restEvents.Fetch_Int("minute_interval");
      re.resting_allowed= restEvents.Fetch_Int("resting_allowed");
      restEventArray.Add(re);
      restEvents.EndList(DBGID_restEvent);
    };
  };
  jw.StartList("levelInfo");
  {
    JReader entries(config);
    entries.StartArray("entry_points");
    mapWidth = CInt("map_width");
    mapHeight = CInt("map_height");
    jw.NameAndValue("width", mapWidth);
    jw.NameAndValue("height", mapHeight);
    if ((mapWidth > 100) || (mapHeight > 100))
    {
      die("Map too large");
    };
    overland = CInt("is_overland");
    jw.NameAndValue("overland", overland);
    jw.NameAndValue("avStyle","AnyView");
    jw.NameAndValue("name",config.Fetch_String("name"));
    jw.NameAndValue("stepSound", "");
    jw.NameAndValue("bumpSound", "");
    jw.StartList("backgroundSounds");
    jw.EndList();
    jw.StartArray("entryPoints");
    while (entries.Optional(), entries.NextEntry())
    {
      CString face, first;
      ENTRYPOINT epData;
      int xy[2];
      jw.NextEntry();
      jw.StartList();
      jw.Linefeed(false);
      entries.StartList(DBGID_entryPoint);
      entries.NameAndValue("id", epData.id);
      jw.NameAndValue("index", epData.id);
      face = entries.Fetch_String("facing");
      first = face.Left(1);
      first.MakeUpper();
      face = first + face.Right(face.GetLength()-1);
      jw.NameAndValue("facing", face);
      epData.facing = face;
      xy[0] = entries.Fetch_Int("x");
      xy[1] = entries.Fetch_Int("y");
      epData.x = xy[0];
      epData.y = xy[1];
      jw.NameAndValue("xy", xy, 2);
      entries.EndList(DBGID_entryPoint);
      jw.EndList();
      jw.Linefeed(true);

      pLevelData->entryPoints.Add(epData);
    };
    jw.EndArray();
  };
  jw.EndList();
  jw.StartList("levelData");
  jw.NameAndValue("width", CInt("map_width"));
  jw.NameAndValue("height",CInt("map_height"));
  jw.NameAndValue("version","???");
  jw.NameAndValue("prevZone","???");
  {
    int x, y;
    JReader mapGrid(level);
    JReader artSlots(level);
    mapGrid.StartArray("map_grid");
    artSlots.StartList("config", DBGID_levelConfigArt);
    artSlots.StartList("art_slots", DBGID_artSlots3);
    overlandBigPic = artSlots.Fetch_Int("overland_big_pic");
    indoorCombatArt = artSlots.Fetch_String("dungeon_combat");
    if (indoorCombatArt == "none") indoorCombatArt = "Rock";
    outdoorCombatArt = artSlots.Fetch_String("wilderness_combat");
    if (outdoorCombatArt == "none") outdoorCombatArt = "River";
    indoorCombatArt = "dungeon_" + indoorCombatArt + ".png";
    outdoorCombatArt = "wilderness_" + outdoorCombatArt + ".png";
    treasurePicName = artSlots.Fetch_String("treasure");
    campPicName = artSlots.Fetch_String("camp");
    GetWallNames(artSlots, wallNames);
    GetBackdropNames(artSlots, backdropNames, CInt("is_overland"), levelNum, overlandBigPic);
    while (mapGrid.Optional(), mapGrid.NextEntry())
    {
      mapGrid.StartList(DBGID_mapGrid);
      MapGrid(mapGrid, wallNames, backdropNames, mapWidth, mapHeight);
      mapGrid.EndList(DBGID_mapGrid);
    };
    if (overland) for (y=0; y<mapHeight; y++) for (x=0; x<mapWidth; x++)
    {
      if (    ((y>0)           && (cells[x][y-1].wallSouthBlockage != "Open"))
           || ((x>0)           && (cells[x-1][y].wallEastBlockage  != "Open"))
           || ((y<mapHeight-1) && (cells[x][y+1].wallNorthBlockage != "Open"))
           || ((x<mapWidth-1)  && (cells[x+1][y].wallWestBlockage  != "Open"))
         )
      {
        cells[x][y].backdrop = "1";
      };
    };
    jw.StartArray("map_grid");
    for (y=0; y<mapHeight; y++) for (x=0; x<mapWidth; x++)
    {
      int i[2];
      jw.NextEntry();
      jw.StartList();
      jw.Linefeed(false);
      jw.NameAndValue("comment FRUA json line number = ", cells[x][y].jsonLine); 
      jw.Linefeed(true);
      i[0] = cells[x][y].x;
      i[1] = cells[x][y].y;
      jw.Linefeed(false);
        jw.NameAndValue("xy", i, 2); 
        int n = cells[x][y].backdrop.GetLength();
        if (!cells[x][y].backdrop.IsEmpty())
        {
          jw.NameAndValue("backdrop", cells[x][y].backdrop);
        };
      jw.Linefeed(true);
      jw.NameAndValue("zone", cells[x][y].zone);
      jw.Linefeed(false);
      jw.NameAndBool("eventExists", cells[x][y].eventID != 0);
      if (cells[x][y].eventID != 0)
      {
        jw.NameAndValue("comment EventID = ", cells[x][y].eventID);
      };
      jw.Linefeed(true);
      {
        jw.StartList("N");
        jw.Linefeed(false);
        jw.NameAndValue("wall", cells[x][y].wallNorth);
        jw.NameAndValue("block",cells[x][y].wallNorthBlockage);
        jw.NameAndValue("background", cells[x][y].backdrop);
        jw.EndList();
        jw.Linefeed(true);
      };
      {
        jw.StartList("E");
        jw.Linefeed(false);
        jw.NameAndValue("wall", cells[x][y].wallEast);
        jw.NameAndValue("block",cells[x][y].wallEastBlockage);
        jw.NameAndValue("background", cells[x][y].backdrop);
        jw.EndList();
        jw.Linefeed(true);
      };
      {
        jw.StartList("S");
        jw.Linefeed(false);
        jw.NameAndValue("wall", cells[x][y].wallSouth);
        jw.NameAndValue("block",cells[x][y].wallSouthBlockage);
        jw.NameAndValue("background", cells[x][y].backdrop);
        jw.EndList();
        jw.Linefeed(true);
      };
      {
        jw.StartList("W");
        jw.Linefeed(false);
        jw.NameAndValue("wall", cells[x][y].wallWest);
        jw.NameAndValue("block",cells[x][y].wallWestBlockage);
        jw.NameAndValue("background", cells[x][y].backdrop);
        jw.EndList();
        jw.Linefeed(true);
      };
      jw.EndList();
    };
    jw.EndArray();
    mapGrid.EndArray();
  };
  {
    JReader zones(config);
    zones.StartArray("zones");
    jw.StartArray("zones");
    while (zones.Optional(), zones.NextEntry())
    {
      CString zoneName;
      int id;
      jw.NextEntry();
      jw.StartList();
      zones.StartList(DBGID_zone);
      id = zones.Fetch_Int("id");
      jw.NameAndValue("id"                    , id);
      jw.NameAndValue("zoneMsg"               , zones.Fetch_String("message"));
      zoneName = zones.Fetch_String("name");
      if (zoneName.IsEmpty())
      {
        zoneName.Format("Zone %d", id);
      };
      jw.NameAndValue("zoneName"              , zoneName);
      jw.NameAndValue("summonedMonster"       , "");
      jw.NameAndValue("addedTurningDifficulty", 0);
      jw.NameAndValue("allowMap"              , "Area Mapping");
      jw.NameAndBool ("AllowMagic"            , true);
      jw.NameAndBool ("AllowAutoDarken"       , true);
      jw.NameAndValue("indoorCombatArt"       , indoorCombatArt);
      jw.NameAndValue("outdoorCombatArt"      , outdoorCombatArt);
      {
        jw.StartList("sounds");
        jw.NameAndBool("useNightMusic", false);
        jw.NameAndValue("startTime"   , 1800);
        jw.NameAndValue("endTime"     , 600);
        jw.EndList();
      };
      {
        PIC_DATA pic;
        pic.filename = campPicName;
        if (!pic.filename.IsEmpty())
        {
          pic.picType = SmallPicDib;
        };
        pic.Export(jw, "campArt");
      };
      {
        PIC_DATA pic;
        pic.filename = treasurePicName;
        if (!pic.filename.IsEmpty())
        {
          pic.picType = SmallPicDib;
        };
        pic.Export(jw, "treasurePicture");
      };
      {
        int i;
        jw.StartList("restEvent");
        for (i=0; i<restEventArray.GetSize(); i++)
        {
          if (restEventArray[i].id == id)
          {
            jw.NameAndValue("allowResting"   , restEventArray[i]. resting_allowed);
            jw.NameAndValue("restEvent"      , restEventArray[i]. event_id);
            jw.NameAndValue("chance"         , restEventArray[i]. encounter_percent_chance);
            jw.NameAndValue("everyMin"       , restEventArray[i]. minute_interval);
            jw.NameAndValue("prevMinChecked" , 9);
            break;
          };
        };
        if (i == restEventArray.GetSize())
        { // Default values
          jw.NameAndValue("allowResting"   , 1);
          jw.NameAndValue("restEvent"      , 0);
          jw.NameAndValue("chance"         , 0);
          jw.NameAndValue("everyMin"       , 0);
          jw.NameAndValue("prevMinChecked" , 0);
        };
        jw.EndList();
      };
      jw.EndList();
      zones.EndList(DBGID_zone);
    };
    {
      int i;
      for (i=9; i<=16; i++)
      {
        CString name;
        jw.NextEntry();
        jw.StartList();
          jw.NameAndValue("id" , i); 
          name.Format("Zone %d", i);
          jw.NameAndValue("zoneName", name);
          jw.NameAndValue("zoneMsg", "");
          jw.NameAndValue("summonedMonster", "");
          jw.NameAndValue("addedTurningDifficulty", "0");
          jw.NameAndValue("allowMap", "Area Mapping");
          jw.NameAndValue("AllowMagic", "true");
          jw.NameAndValue("AllowAutoDarken", "true");
          jw.NameAndValue("indoorCombatArt", indoorCombatArt);
          jw.NameAndValue("outdoorCombatArt", outdoorCombatArt);
          jw.StartList("sounds");
            jw.NameAndBool("useNightMusic", false);
            jw.NameAndValue("startTime"   , 1800);
            jw.NameAndValue("endTime"     , 600);
          jw.EndList();
          jw.StartList("restEvent");
            jw.NameAndValue("allowResting"   , 1);
            jw.NameAndValue("restEvent"      , 0);
            jw.NameAndValue("chance"         , 0);
            jw.NameAndValue("everyMin"       , 0);
            jw.NameAndValue("prevMinChecked" , 0);
          jw.EndList();
        jw.EndList();
      };
    };
    jw.EndArray();
  };
  {
    JReader events(level);
    {
      JReader evt(level);
      nxtAvailEventId = 1;
      evt.StartArray("events");
      // We need to run through the list of events to find whaich
      // ids are being used.  We will need unique ids to generate
      // new events (for the SMALL TOWN, for example).
      // What about Rest Events, Step Events
      while (evt.Optional(), evt.NextEntry())
      {
        int id;
        evt.StartList(DBGID_singleEvent);
        evt.NameAndValue("id", id);
        if (id >= nxtAvailEventId) nxtAvailEventId = id+1;
        evt.EndList(DBGID_singleEvent);
      };
      evt.EndArray();
    };

    {
      jw.StartArray("specialKeys");
      int i;
      for (i=0; i<8; i++)
      {
        jw.NextEntry();
        jw.QuotedString("0");
      };
      jw.EndArray();
    };
    events.StartArray("events");
    jw.StartArray("events");
    while (events.Optional(), events.NextEntry())
    {
      int lineNumber;
      events.StartList(DBGID_singleEvent);
      lineNumber = events.LineNum();
      JReader details(events);
      details.StartList("details", DBGID_details);
      jw.NextEntry();
      jw.StartList();
      if (lineNumber == 46936)
      {
        int kkk=1;
      };
      CString type;
      events.NameAndValue("type", type);
      if      (type == "Text Statement") 
      {
        //Event_General(events, details, jw, "Text Statement");
        Event_TextStatement(events, details, jw);
      }
      else if (type == "Guided Tour") 
      {
        //Event_General(events, details, jw, "Guided Tour");
        Event_GuidedTour(events, details, jw);
      }
      else if (type == "Question List") 
      {
        //Event_General(events, details, jw, "Question List");
        Event_QuestionList(events, details, jw);
      }
      else if (type == "Utilities") 
      {
        //Event_General(events, details, jw, "Utilities");
        Event_Utilities(events, details, jw);
      }
      else if (type == "Chain") 
      {
        //Event_General(events, details, jw, "Chain");
        Event_Chain(events, details, jw);
      }
      else if (type == "Question Button") 
      {
        //Event_General(events, details, jw, "Question Button");
        Event_QuestionButton(events, details, jw);
      }
      else if (type == "Shop") 
      {
        //Event_General(events, details, jw, "Shop");
        Event_Shop(events, details, jw);
      }
      else if (type == "Camp") 
      {
        //Event_General(events, details, jw, "Camp");
        Event_Camp(events, details, jw);
      }
      else if (type == "Combat") 
      {
        //Event_General(events, details, jw, "Combat");
        Event_Combat(events, details, jw, false);
      }
      else if (type == "Pick One Combat") 
      {
        //Event_General(events, details, jw, "Combat");
        Event_Combat(events, details, jw, true);
      }
      else if (type == "Teleporter") 
      {
        //Event_General(events, details, jw, "Transfer Module");
        Event_Teleporter(events, details, jw);
      }
      else if (type == "Transfer Module") 
      {
        //Event_General(events, details, jw, "Transfer Module");
        Event_TransferModule(events, details, jw);
      }
      else if (type == "Stairs") 
      {
        //Event_General(events, details, jw, "Transfer Module");
        Event_Stairs(events, details, jw);
      }
      else if (type == "Null") 
      {
        //Event_General(events, details, jw, "Null Event");
        Event_Null(events, details, jw);
      }
      else if (type == "Monster") 
      {
        //Event_General(events, details, jw, "Monster");
        Event_Monster(events, details, jw);
      }
      else if (type == "Tour") 
      {
        //Event_General(events, details, jw, "Tour");
        Event_Tour(events, details, jw);
      }
      else if (type == "Add NPC") 
      {
        //Event_General(events, details, jw, "AddNPC");
        Event_AddNPC(events, details, jw);
      }
      else if (type == "Remove NPC") 
      {
        //Event_General(events, details, jw, "RemoveNPC");
        Event_RemoveNPC(events, details, jw);
      }
      else if (type == "Question Yes No") 
      {
        //Event_General(events, details, jw, "QuestionYesNo");
        Event_QuestionYesNo(events, details, jw);
      }
      else if (type == "Encounter") 
      {
        //Event_General(events, details, jw, "Encounter");
        Event_Encounter(events, details, jw);
      }
      else if (type == "Enter Password") 
      {
        //Event_General(events, details, jw, "Password");
        Event_Password(events, details, jw);
      }
      else if (type == "Gain Experience") 
      {
        //Event_General(events, details, jw, "GainExperience");
        Event_GainExperience(events, details, jw);
      }
      else if (type == "Give Treasure") 
      {
        //Event_General(events, details, jw, "GiveTreasure");
        Event_GiveTreasure(events, details, jw);
      }
      else if (type == "Combat Treasure") 
      {
        //Event_General(events, details, jw, "GiveTreasure");
        Event_CombatTreasure(events, details, jw);
      }
      else if (type == "Damage") 
      {
        //Event_General(events, details, jw, "GiveDamage");
        Event_GiveDamage(events, details, jw);
      }
      else if (type == "Rest") 
      {
        //Event_General(events, details, jw, "Rest");
        Event_Rest(events, details, jw);
      }
      else if (type == "Quest Stage") 
      {
        //Event_General(events, details, jw, "Rest");
        Event_Quest(events, details, jw);
      }
      else if (type == "NPC Says") 
      {
        //Event_General(events, details, jw, "NPCSays");
        Event_NPCSays(events, details, jw);
      }
      else if (type == "TakePartyItems") 
      {
        //Event_General(events, details, jw, "TakePartyItems");
        Event_TakePartyItems(events, details, jw);
      }
      else if (type == "Who Pays") 
      {
        //Event_General(events, details, jw, "WhoPays");
        Event_WhoPays(events, details, jw);
      }
      else if (type == "Who Tries") 
      {
        //Event_General(events, details, jw, "WhoTries");
        Event_WhoTries(events, details, jw);
      }
      else if (type == "Pass Time") 
      {
        //Event_General(events, details, jw, "PassTime");
        Event_PassTime(events, details, jw);
      }
      else if (type == "Vault") 
      {
        //Event_General(events, details, jw, "Vault");
        Event_Vault(events, details, jw);
      }
      else if (type == "Tavern") 
      {
        //Event_General(events, details, jw, "Tavern");
        Event_Tavern(events, details, jw);
      }
      else if (type == "Tavern Tales") 
      {
        //Event_General(events, details, jw, "Tavern");
        Event_TavernTales(events, details, jw);
      }
      else if (type == "Temple") 
      {
        //Event_General(events, details, jw, "Temple");
        Event_Temple(events, details, jw);
      }
      else if (type == "Random") 
      {
        //Event_General(events, details, jw, "Random");
        Event_Random(events, details, jw);
      }
      else if (type == "Small Town") 
      {
        //Event_General(events, details, jw, "SmallTown");
        Event_SmallTown(events, details, jw);
      }
      else if (type == "Training Hall") 
      {
        //Event_General(events, details, jw, "TrainingHall");
        Event_TrainingHall(events, details, jw);
      }
      else if (type == "Sounds") 
      {
        //Event_General(events, details, jw, "Sound");
        Event_Sound(events, details, jw);
      }
      else if (type == "HealParty") 
      {
        //Event_General(events, details, jw, "HealParty");
        Event_HealParty(events, details, jw);
      }
      else if (type == "Special Items")
      {
        Event_SpecialItems(events, details, jw);
      }
      else if (type == "Journal") 
      {
        //Event_General(events, details, jw, "Journal");
        Event_Journal(events, details, jw);
      }
      else
      {
        CString msg;
        msg.Format("Unknown event type '%s'", type);
        events.PrintError(msg);
      };
      events.EndList(DBGID_singleEvent);
      jw.EndList();
    };
    jw.EndArray();
  };
  jw.NameAndValue("avArt","AreaViewZoneArt.png");
  jw.StartArray("asl");
  jw.EndArray();
  {
    JReader stepEvents(config);
    stepEvents.StartArray("step_events");
    StepEvents(stepEvents, jw);
  };
  {
    int i;
    //JReader artSlots(config);
    //artSlots.StartList("art_slots", DBGID_artSlots1);    
    jw.StartArray("walls");
    {

      for (i=0; i<16; i++)
      {
        int j;
        jw.NextEntry();

        WallSetSlotMemType wall;
        if (!wallNames[i].IsEmpty() && (wallNames[i] != "none"))
        {
          for (j=0; j<artMap.GetSize(); j++)
          {
            if (wallNames[i] == artMap[j].strings[0])
            {
              wall.wallFile = artMap[j].strings[1];
              wall.doorFile = artMap[j].strings[2];
              wall.overlayFile = artMap[j].strings[3];
              break;
            };
          };
          if (j == artMap.GetSize())
          {
            printf("Cannot determine Art Mapping for %s\n", wallNames[i], wallNames[i].GetLength());
          };
        };
        wall.Export(jw);
      };
    };
    /*
    for (i=0; i<16; i++)
    {
      WallSetSlotMemType wall;
      CString wallName, fileName;
      wallName.Format("Wall_%d", i);
      wall.Clear();
      artSlots.Optional();
      jw.NextEntry();
      if (artSlots.NameAndValue(wallName, fileName))
      {
        wall.wallFileName = fileName;
        wall.wallFile.Format("wa_Wall%d.png", i);
      };
      wall.Export(jw);
    };
    */
    jw.EndArray();

    jw.StartArray("backgroundSets");
    for (i=0; i<5; i++)
    {
      int j;
      BackgroundSlotMemType background;
      CString backgroundName, fileName;
      //artSlots.Optional();
      jw.NextEntry();
      if (!backdropNames[i].IsEmpty()  && (backdropNames[i] != "none"))
      {
        for (j=0; j<artMap.GetSize(); j++)
        {
          if (backdropNames[i] == artMap[j].strings[0])
          {
            background.backgroundFile = artMap[j].strings[1];
            if (artMap[j].strings.GetSize() > 2)
            {
              background.backgroundFileAlt = artMap[j].strings[2];
              background.useAltBackground = 1;
            };
            break;
          };
        };
        if (j == artMap.GetSize())
        {
          background.backgroundFile = backdropNames[i];
          if (background.backgroundFile.Right(4) != ".png")
          {
            printf("Cannot map background named '%s' to a graphic file\n", backdropNames[i]);
          };
        };
      };
      background.Export(jw);
    };
    jw.EndArray();
  };
  /*
  {
    int i;
    CString keyname, keyvalue;
    jw.StartArray("specialKeys");
    for (i=1; i<=8; i++)
    {
      keyname.Format("special_key_%03d", i);
      global.NameAndValue(keyname, keyvalue);
      jw.NextEntry();
      jw.QuotedString(keyvalue);
    };
    jw.EndArray();
  };
  */
  jw.EndList();
}



void GameEvent::Export(JWriter& jw)
{
  int i[2];
  {
    {
      jw.StartList(JKEY_GENERAL); 
      jw.Linefeed(false); 
      jw.NameAndValue("json Line Number", jsonLineNum);
      jw.Linefeed(true);
    };
    jw.NameAndQuotedValue(JKEY_EVENTTYPE, EventListText[evType]);
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
  };
  jw.Linefeed(true);
}

void GameEvent::Read(JReader& event, eventType type)
{
  int i[2] = {-1,-1};
  evType = type;
  jsonLineNum = event.LineNum();
  event.NameAndValue("id", id);
  for (x=0; x<mapWidth; x++) for (y=0; y<mapHeight; y++)
  {
    if (cells[x][y].eventID == id)
    {
      i[0] = x;
      i[1] = y;
      break;
    };
  };
  x = i[0];
  y = i[1];
  control.Read(event);
                    pic.ReadSmallPicFile(event, "picture");
  event.Optional(); pic2.Read(event, JKEY_PICTURE2);
  event.Optional(); event.NameAndQuotedNonBlank(JKEY_TEXT, text);
  event.Optional(); event.NameAndQuotedNonBlank(JKEY_TEXT2, text2);
  event.Optional(); event.NameAndQuotedNonBlank(JKEY_TEXT3, text3);
  {
    CString chainEventCondition;
    event.NameAndNonZero("chained_event_id",chainEventHappen);
    event.NameAndNonZero("chained_event_id",chainEventNotHappen);
    if (control.chainTrigger == IfEventNotHappen) chainEventHappen=0;
    if (control.chainTrigger == IfEventHappen) chainEventNotHappen=0;
    if (control.chainTrigger == AlwaysChain) chainEventNotHappen=0;
  };
  {
    timer = 0;
    event.Optional(); event.NameAndValue(JKEY_TIMER, timer);
  };
  return;
}


/*

void GameEvent::Read(JReader& jr)
{
  int xy[2];
  int eventID;
  jw.StartList("general");
  jw.NameAndValue("eventType", eventType);
  eventID = jr.Fetch_Int("id");
  jw.NameAndValue("eventID", eventID);
  xy[0] = 0;
  xy[1] = 0;
  {
    int i;
    for (i=0; i<cells.GetSize(); i++)
    {
      if ((cells[i].eventID == eventID))
      {
        xy[0] = cells[i].x;
        xy[1] = cells[i].y;
      };
    };        
  };
  jw.NameAndValue("xy", xy, 2);
  {
    int i[2];
    jw.StartList("control");
    jw.NameAndValue       ("onceOnly",     jr.Fetch_Int("do_only_once"));
    jw.NameAndValue       ("chainTrigger", jr.Fetch_String("chainedEventFollow"));
    {
      int i;
      CString trig = jr.Fetch_String("trigger");
      for (i=0; i<numTriggerNames; i++)
      {
        if (trig == triggerNames[2*i])
        {
          trig = triggerNames[2*i+1];
          break;
        };
      };
      jw.NameAndValue       ("eventTrigger", trig);
    };
    jw.NameAndValue       ("race",         jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("class",        jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("baseclass",    jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("item",         jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("character",    jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("spell",        jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("spellClass",   jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("spellLevel",   jr.Fetch_String("trigger_data"));
    jw.NameAndBool        ("gpdlIsBinary", false);
    jw.NameAndValue       ("gpdlData",      "");
    jw.NameAndValue       ("gender",       jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("quest",        0);
    jw.NameAndValue       ("chance",       100);
    jw.NameAndValue       ("specialItem",   jr.Fetch_String("trigger_data"));
    jw.NameAndValue       ("specialKey",    jr.Fetch_String("trigger_data"));
    i[0] = 0;
    i[1] = 0;
    jw.NameAndValue("partyXY", i, 2);
    jw.NameAndValue("eventcontrol_asl", "");
    jw.NameAndValue("facing", "Any/All Side(s)");
    jw.EndList();
  };
  {
    PIC_DATA pic;
    TEXT text1, text2, text3, text4, text5;
    int i = 0;
    CString Text[5];;
    {
      details.Optional(); pic.picture = details.Fetch_String("picture");
      details.Optional(); pic.distance = details.Fetch_String("picture_distance");
      pic.Export(jw, "picture1");
    };
    if (strcmp(eventType, "Transfer Module") == 0)
    {
      details.NameAndValue("text_ask", text1.text);
      details.NameAndValue("text_enter", text2.text);
      text3.text = "";
      text4.text = "";
      text5.text = "";
    }
    else
    {
      text1.text = details.Fetch_String("message_1_text");
      text2.text = details.Fetch_String("message_2_text");
      text3.text = details.Fetch_String("message_3_text");
      text4.text = details.Fetch_String("message_4_text");
      text5.text = details.Fetch_String("message_5_text");
      text1.needReturn = details.Fetch_Int("message_1_hit_return");
      text2.needReturn = details.Fetch_Int("message_2_hit_return");
      text3.needReturn = details.Fetch_Int("message_3_hit_return");
      text4.needReturn = details.Fetch_Int("message_4_hit_return");
      text5.needReturn = details.Fetch_Int("message_5_hit_return");
    };
    Text[i] = text1.text;
    if (text1.needReturn)
    {
      i++;
      Text[i] = text2.text;
    }
    else
    {
      Text[i] += "\\n";
      Text[i] += text2.text;
    };
    if (text2.needReturn)
    {
      i++;
      Text[i] = text3.text;
    }
    else
    {
      Text[i] += "\\n";
      Text[i] += text3.text;
    };
    if (text3.needReturn)
    {
      i++;
      Text[i] = text4.text;
    }
    else
    {
      Text[i] += "\\n";
      Text[i] += text4.text;
    };
    if (text4.needReturn)
    {
      i++;
      Text[i] = text5.text;
    }
    else
    {
      Text[i] += "\\n";
      Text[i] += text5.text;
    };
    while (i > 2)
    {
      Text[i-1] += Text[i];
      i--;
    };
    jw.NameAndValue("text",  Text[0]);
    jw.NameAndValue("text2", Text[1]);
    jw.NameAndValue("text3", Text[2]);
    pic.picture = "";
    pic.distance = "";
    pic.Export(jw, "picture2");
    // Here we could modify the text to insert highlight characters.
    jw.NameAndValue("chainEventHappen",jr.Fetch_Int("chain_event_id"));
    jw.NameAndValue("chainEventNotHappen", 0);
    jw.NameAndNonZero("timer", 0);
  };  
  jw.EndList();
  jw.StartList("particular");
}
*/

/*
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
*/

bool OpenOutputFile(CFile& cf, CString fname)
{
  CString path;
  path = jsonPath + fname;
  if (!cf.Open(path, CFile::modeCreate|CFile::modeWrite))
  {
    CString msg;
    msg.Format("Cannot open output file '%s'", fname);
    die(msg);
  };
  return true;
}


void ConvertLevels(JReader& global, JReader& jr)
{
  int levelNum;
  CFile output;
  CString fname;
  while (jr.Optional(), jr.NextEntry())
  {
    jr.StartList(DBGID_levelEntry);
    levelNum = jr.Fetch_Int("id");
    fname.Format("Level%03d.txt", levelNum);
    OpenOutputFile(output, fname); // Open file in same folder as input.
    JWriter jw(&output);
    jw.StartList();
    {
      CString ver;
      ver.Format("%s:%s", FRUAExportDate, FRUAExportTime);
      jw.NameAndValue("FRUAExport Version", ver);
      jw.NameAndValue("ConverFRUA Version", ConvertFRUAVersion);
    };
    ConvertOneLevel(global, jr, jw);
    jw.EndList();
    output.Close();
    jr.EndList(DBGID_levelEntry);
  };
}
