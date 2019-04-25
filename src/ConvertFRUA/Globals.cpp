#include "stdafx.h"
#include "Translate.h"

extern CArray<LEVELDATA, LEVELDATA&> levelArray;
extern CString jsonPath;
extern CString FRUAExportDate, FRUAExportTime;
extern CString ConvertFRUAVersion;

void Art(JWriter& jw, const char *name, const char *value)
{
  jw.StartList(name);
  jw.StartArray("picType");
  jw.EndArray();
  jw.NameAndValue("name", value);
  jw.EndList();
}

void Coins(JWriter& jw, const char *name, double rate, int isBase)
{
  jw.NextEntry();
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue("name", name);
  jw.NameAndValue("rate", rate);
  jw.NameAndValue("isBase", isBase);
  jw.EndList();
  jw.Linefeed(true);
}


class DIFFICULTY_DATA
{
public:
  void Export(JWriter& jw);
  CString m_Name;
  BOOL m_ModifyHD;
  BOOL m_ModifyQty;
  BOOL m_ModifyMonsterExp;
  BOOL m_ModifyAllExp;
  char m_HDAmount;
  char m_QtyAmount;
  char m_MExpAmount;
  char m_AExpAmount;
};


void DIFFICULTY_DATA::Export(JWriter& jw)
{
  jw.NextEntry();
  jw.StartList();
  jw.NameAndValue("name", m_Name),    
  jw.NameAndValue("modifyHD"         ,m_ModifyHD);
  jw.NameAndValue("modifyQty"        ,m_ModifyQty);
  jw.NameAndValue("modifyMonsterExp" ,m_ModifyMonsterExp);
  jw.NameAndValue("modifyAllExp"     ,m_ModifyAllExp);
  jw.NameAndValue("hdAmount"         ,m_HDAmount);
  jw.NameAndValue("qtyAmount"        ,m_QtyAmount);
  jw.NameAndValue("mExpAmount"       ,m_MExpAmount);
  jw.NameAndValue("aExpAmount"       ,m_AExpAmount);    
  jw.EndList();  
};


void ConvertGlobalFile(JReader& globals, JWriter& jw)
{
  jw.NameAndValue("version", "0.914");
  jw.NameAndValue("designName", globals.Fetch_String("design_name"));
  {
    int i, startLevel, startFacing=0, entryPoint;
    int startxy[2] = {0,0};
    globals.NameAndValue("start_level", startLevel);
    globals.NameAndValue("start_entrypoint", entryPoint);
    for (i=0; i<levelArray.GetSize(); i++)
    {
      if (levelArray[i].levelNum == startLevel)
      {
        int j;
        for (j=0; j<levelArray[i].entryPoints.GetSize(); j++)
        {
          if (levelArray[i].entryPoints[j].id == entryPoint)
          {
            CString facing;
            startxy[0] = levelArray[i].entryPoints[j].x;
            startxy[1] = levelArray[i].entryPoints[j].y;
            facing     = levelArray[i].entryPoints[j].facing;
            if (facing == "North") startFacing = 0;
            if (facing == "East")  startFacing = 1;
            if (facing == "South") startFacing = 2;
            if (facing == "West")  startFacing = 3;
          };
        };
      };
    };
    jw.NameAndValue("startLevel"     , startLevel);
    jw.NameAndValue("startXY"        , startxy, 2);
    jw.NameAndValue("startFacing"    , startFacing);
    jw.NameAndValue("startTime"      , "800");
    jw.NameAndValue("startExp"       , globals.Fetch_Int("start_experience"));
    jw.NameAndValue("startExpType"   , "0");
    jw.NameAndValue("startPlatinum"  , globals.Fetch_Int("start_platinum"));
    jw.NameAndValue("startGem"       , globals.Fetch_Int("start_gem"));
    jw.NameAndValue("startJewelry"   , globals.Fetch_Int("start_jewelry"));
    jw.NameAndBool ("deadAtZeroHP"   , false);

    {
      jw.StartArray("specialItems");
      for (i=1; i<=12; i++)
      {
        CString tag;
        CString name;
        tag.Format("special_item_%03d", i);
        globals.NameAndValue(tag, name);
        jw.NextEntry();
        jw.StartList();
        jw.NameAndValue("name", name);
        jw.NameAndValue("id", i);
        jw.NameAndValue("stage", 0);
        jw.NameAndValue("examineEvent", 0);
        jw.NameAndValue("examineLabel","EXAMINE");
        jw.NameAndValue("canBeDropped", 1);
        jw.EndList();
      };
      jw.EndArray();
    };

    {
      jw.StartArray("specialKeys");
      for (i=1; i<=8; i++)
      {
        CString tag;
        CString name;
        tag.Format("special_key_%03d", i);
        globals.NameAndValue(tag, name);
        jw.NextEntry();
        jw.StartList();
        jw.NameAndValue("name", name);
        jw.NameAndValue("id", i);
        jw.NameAndValue("stage", 0);
        jw.NameAndValue("examineEvent", 0);
        jw.NameAndValue("examineLabel","EXAMINE");
        jw.NameAndValue("canBeDropped", 1);
        jw.EndList();
      };
      jw.EndArray();
    };
      
    {
      jw.StartArray("quests");
      for (i=1; i<=44; i++)
      {
        CString name;
        name.Format("quest_%03d", i);
        jw.NextEntry();
        jw.StartList();
        jw.NameAndValue("name", name);
        jw.NameAndValue("id", i);
        jw.NameAndValue("state", "Not Started");
        jw.NameAndValue("stage", 0);
        jw.EndList();
      };
      jw.EndArray();
    };
      
  };
  jw.NameAndValue("dungeonTimeDelta", "1");

  jw.NameAndValue("dungeonSearchDelta", "10");
  jw.NameAndValue("wildernessTimeDelta", "720");
  jw.NameAndValue("wildernessSearchDelta", "1080");
  jw.NameAndValue("autoDaken", "0");
  jw.NameAndValue("autoAarkenAmount", "256");
  jw.NameAndValue("startDarken", "0");
  jw.NameAndValue("endDarken", "0");
  jw.NameAndValue("minPCs",       "1");
  jw.NameAndValue("maxPartySize", "6");
  jw.NameAndValue("maxPCs",       "12");
  jw.NameAndValue("allowCharCreate", "1");
  jw.NameAndValue("mapArt", "AreaViewArt.png");
  {
    int logfont[64] = {16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-68,2,0,0,0,0,0,0,0,0,3,2,83,89,83,84,69,77,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    jw.NameAndValue("logFont", logfont, 64);
  };
  jw.NameAndValue("IconBgArt", "iconBackground.png");
  jw.NameAndValue("BackgroundArt","");
  jw.NameAndValue("CreditsBgArt", "Credits.png");
  {
    jw.StartArray("smallPics");
    jw.EndArray();
  };

   /*
   "smallPics":[
      {
         "picType":[SmallPicDib],
         "filename":"portrait_PC_ClericFemale.png",
         "timeDelay":"0",
         "numFrame":"1",
         "width":"176",
         "height":"211",
         "flags":"0",
         "maxLoops":"1",
         "style":"0",
         "allowCentering":"true",
         "useAlpha":"false",
         "alpha":"0"
      }
   ],
*/
  {
    jw.StartArray("iconPics");
    jw.EndArray();
  };
  /*
  {
   "iconPics":[
      {
         "picType":[IconDib],
         "filename":"icon_PC_ClericFemale.png",
         "timeDelay":"0",
         "numFrame":"2",
         "width":"48",
         "height":"48",
         "flags":"0",
         "maxLoops":"1",
         "style":"0",
         "allowCentering":"true",
         "useAlpha":"false",
         "alpha":"0"
      }
   ],
*/
  {
    jw.StartList("titleScreenData");
    jw.NameAndValue("timeout", 0);
    {
      jw.StartArray("titles");
      jw.EndArray();
    };
    jw.EndList();
  };
  /*
   "titleScreenData":{
      "timeout":"0",
      "titles":[
         {
            "titleBgArt":"Title.png",
            "useTrans":"0",
            "useBlend":"0",
            "displayBy":"1"
         }
      ]
   },
   "asl":[
      {"key":"GuidedTourVersion","value":"0.9140","flags":"5"},
      {"key":"ItemUseEventVersion","value":"0.9140","flags":"5"},
      {"key":"RunAsVersion","value":"0.9140","flags":"5"},
      {"key":"SpecialItemKeyQtyVersion","value":"0.9140","flags":"5"},
      {"key":"_ZAPCMD_","value":"MOON,1","flags":"0"}
   ],
   */
    Art(jw, "hBarVPArt"         , "border_Horizontal.png");
    Art(jw, "vBarVPArt"         , "border_Vertical.png");
    Art(jw, "frameVPArt"        , "border_Viewport.png");
    Art(jw, "vBarCombArt"       , "border_Vertical.png");
    Art(jw, "hBarCombArt"       , "border_Horizontal.png");
    Art(jw, "combatWallArt"     , "cw_defcw.png");
    Art(jw, "combatCursorArt"   , "cursor_Combat.png");
    Art(jw, "combatDeathIcon"   , "cursor_CombatDying.png");
    Art(jw, "combatPetrifidIcon", "cursor_CombatPetrified.png");
    Art(jw, "combatDeathArt"    , "cursor_CombatDeath.png");


    {
      jw.StartList("cursorArt");
      jw.NameAndEnum("picType"        , TransBufferDib);
      jw.NameAndValue("filename"      , "cursor_Arrows.png");
      jw.NameAndValue("timeDelay"     , "0");
      jw.NameAndValue("numFrame"      , "15");
      jw.NameAndValue("width"         , "32");
      jw.NameAndValue("height"        , "32");
      jw.NameAndValue("flags"         , "4");
      jw.NameAndValue("maxLoops"      , "1");
      jw.NameAndValue("style"         , "0");
      jw.NameAndValue("allowCentering", "true");
      jw.NameAndValue("useAlpha"      , "false");
      jw.NameAndValue("alpha"         , "0");
      jw.EndList();
   };
 
  jw.NameAndValue("charHit"   , "");
  jw.NameAndValue("charMiss"  , "");
  jw.NameAndValue("partyBump" , "sound_Bump.wav");
  jw.NameAndValue("partyStep" , "sound_Step.wav");
  jw.NameAndValue("deathMusic", "");
  {
    jw.StartList("introMusic");
    jw.EndList();
  };
  {
    jw.StartList("campMusic");
    jw.EndList();
  };
  {
    jw.StartArray("specialObjects");
    jw.EndArray();
  };
  {
    jw.StartArray("specialObjects");
    jw.EndArray();
  };


  {
    jw.StartArray("quests");
    jw.EndArray();
  };
    /*
      {
         "name":"moon_New",
         "state":"In Progress",
         "stage":"2",
         "id":"1"
      }
    ],
*/
  {
    jw.StartArray("characterList");
    jw.EndArray();
  };
/*
      {
         "charVersion":"-2147483647",
         "type":"2",
         "race":"Human",
         "gender":"Male",
         "class":"Thief",
         "alignment":"Chaotic Neutral",
         "allowInCombat":"1",
         "undeadType":"",
         "size":"Medium",
         "name":"NPC_Thief",
         "characterID":"NPC_Thief",
         "thac0":"12",
         "morale":"50",
         "encumbrance":"1",
         "maxencumbrance":"2750",
         "ac":"9",
         "HP":"150",
         "maxHP":"150",
         "nbrHitDice":"40.000000",
         "age":"20",
         "maxAge":"321",
         "birthdat":"268",
         "maxCureDisease":"0",
         "unarmedDiceS":"2",
         "unarmedNbrDiceS":"1",
         "unarmedDiceBonus":"0",
         "unarmedDiceL":"2",
         "unarmedNbrDiceL":"1",
         "maxMovement":"12",
         "readyToTrain":"0",
         "canTraceItems":"1",
         "str":"15",
         "strMod":"0",
         "int":"15",
         "wis":"15",
         "dex":"15",
         "con":"15",
         "cha":"15",
         "openDoors":"0",
         "openMagicDoors":"0",
         "bblg":"0",
         "hitBonus":"0",
         "dmgBonus":"0",
         "magicResistance":"0",
         "baseclassStats":[
            {
               "baseclassID":"thief",
               "currentLevel":"20",
               "previousLevel":"0",
               "preDrainLevel":"0",
               "experience":"2200001"
            }
         ],
         "skillAdjustments":[],
         "spellAdjustments":[],
         "isPregen":"1",
         "canBeSaved":"1",
         "hasLayedOnHandsToday":"0",
         "money":{
            "gems":[],
            "jewels":[]
         },
         "nbrAttacks":"1.000000",
         "icon":{
            "picType":[IconDib],
            "filename":"icon_NPC_Guard.png",
            "timeDelay":"0",
            "numFrame":"2",
            "width":"48",
            "height":"48",
            "flags":"0",
            "maxLoops":"1",
            "style":"0",
            "allowCentering":"true",
            "useAlpha":"false",
            "alpha":"0"
         },
         "iconIndex":"1",
         "origIndex":"-1",
         "uniquePartyID":"255",
         "disableTalkIfDead":"1",
         "talkEvent":"0",
         "talkLabel":"TALK",
         "examineEvent":"0",
         "examieLabel":"EXAMINE",
         "spellbook":{
            "useLimits":"true",
            "spellList":[]
         },
         "detectingInvisible":"0",
         "detectingTraps":"0",
         "spellEffects":[],
         "blockageStatus":[],
         "smallPic":{
            "picType":[SmallPicDib],
            "filename":"portrait_NPC_Guard.png",
            "timeDelay":"0",
            "numFrame":"1",
            "width":"176",
            "height":"211",
            "flags":"4",
            "maxLoops":"1",
            "style":"0",
            "allowCentering":"true",
            "useAlpha":"false",
            "alpha":"0"
         },
         "possessions":[],
         "specialAbilities":[{"name":"class_Thief","value":"npc"}]
      }
   ],
*/
  jw.NameAndValue("weight"     , 10);
  jw.NameAndValue("highestRate", 1000);
  jw.NameAndValue("hrType"     , "ItemType04");
  jw.NameAndValue("defType"    , "ItemType01");
  {
    jw.StartList("gems");
    jw.NameAndValue("minValue", 1);
    jw.NameAndValue("maxValue", 100);
    jw.NameAndValue("names", "Gem");
    jw.EndList();
  };
  {
    jw.StartList("jewels");
    jw.NameAndValue("minValue", 10);
    jw.NameAndValue("maxValue", 1000);
    jw.NameAndValue("names", "Jewelry");
    jw.EndList();
  };
  {
    jw.StartArray("coins");
    Coins(jw, "Platinum", 1.000000   , 1);
    Coins(jw, "Electrum", 10.000000  , 1);
    Coins(jw, "Gold"    , 5.000000   , 1);
    Coins(jw, "Silver"  , 100.000000 , 1);
    Coins(jw, "Copper"  , 1000.000000, 1);
    Coins(jw, ""        , 0.000000   , 0);
    Coins(jw, ""        , 0.000000   , 0);
    Coins(jw, ""        , 0.000000   , 0);
    Coins(jw, ""        , 0.000000   , 0);
    Coins(jw, ""        , 0.000000   , 0);
    jw.EndArray();
  };
  {
    int startLevel;
    globals.NameAndValue("start_level", startLevel);
    jw.NameAndValue("defaultLevel", startLevel);
  };
  {
    DIFFICULTY_DATA data;
    jw.StartArray("data");
    {
      data.m_Name = "NOVICE";
      data.m_ModifyHD = 1;
      data.m_ModifyQty = 1;
      data.m_ModifyMonsterExp = 1;
      data.m_ModifyAllExp = 1;
      data.m_HDAmount = -20;
      data.m_QtyAmount = -50;
      data.m_MExpAmount = -30;
      data.m_AExpAmount = -10;
      data.Export(jw);
    };
    {
      data.m_Name = "SQUIRE";
      data.m_ModifyHD = 1;
      data.m_ModifyQty = 1;
      data.m_ModifyMonsterExp = 1;
      data.m_ModifyAllExp = 1;
      data.m_HDAmount = -10;
      data.m_QtyAmount = -25;
      data.m_MExpAmount = -15;
      data.m_AExpAmount = -5;
      data.Export(jw);
    };
    {
      data.m_Name = "VETERAN";
      data.m_ModifyHD = 0;
      data.m_ModifyQty = 0;
      data.m_ModifyMonsterExp = 0;
      data.m_ModifyAllExp = 0;
      data.m_HDAmount = 0;
      data.m_QtyAmount = 0;
      data.m_MExpAmount = 0;
      data.m_AExpAmount = 0;
      data.Export(jw);
    };
    {
      data.m_Name = "ADEPT";
      data.m_ModifyHD = 1;
      data.m_ModifyQty = 1;
      data.m_ModifyMonsterExp = 1;
      data.m_ModifyAllExp = 1;
      data.m_HDAmount = 10;
      data.m_QtyAmount = 25;
      data.m_MExpAmount = 15;
      data.m_AExpAmount = 5;
      data.Export(jw);
    };
    {
      data.m_Name = "CHAMPION";
      data.m_ModifyHD = 1;
      data.m_ModifyQty = 1;
      data.m_ModifyMonsterExp = 1;
      data.m_ModifyAllExp = 1;
      data.m_HDAmount = 20;
      data.m_QtyAmount = 50;
      data.m_MExpAmount = 30;
      data.m_AExpAmount = 10;
      data.Export(jw);
    };
    jw.EndArray();
  };
}



void ConvertGlobals(JReader& globals)
{
  CFile output;

  output.Open(jsonPath + "GlobalData.txt", CFile::modeCreate|CFile::modeWrite);
  JWriter jw(&output);
  jw.StartList();
  {
    CString ver;
    ver.Format("%s:%s", FRUAExportDate, FRUAExportTime);
    jw.NameAndValue("FRUAExport Version", ver);
    jw.NameAndValue("ConverFRUA Version", ConvertFRUAVersion);
  };
  ConvertGlobalFile(globals, jw);

  jw.EndList();
  output.Close();
}