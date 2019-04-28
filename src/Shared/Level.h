/******************************************************************************
* Filename: Level.h
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
#ifndef __LEVEL_H__
#define __LEVEL_H__

//#include "externs.h"
#include "Items.h"
#include "PicData.h"
#include "PicSlot.h"
#include "GameEvent.h"
#include "SoundMgr.h"

enum BlockageType { OpenBlk, 
                    OpenSecretBlk,
                    BlockedBlk,
                    FalseDoorBlk, //( secret + blocked )
                    LockedBlk, 
                    LockedSecretBlk, // ( secret + locked )
                    LockedWizardBlk, 
                    LockedWizardSecretBlk, // ( secret + wizard )
                    LockedKey1, LockedKey2,
                    LockedKey3, LockedKey4,
                    LockedKey5, LockedKey6,
                    LockedKey7, LockedKey8 };
const int NUM_BLOCKAGE_TYPES = 16;

// This struct is used when loading Pre-0.573 designs.
// Don't change it!
//
struct PRE_VERSION_0573_AREA_MAP_DATA 
{
  BYTE bkgrnd; // indicates blockage for overland maps
  BYTE zone;   // what zone is this spot in?
  BOOL eventExists;  // is there a root event linked to this spot?
  BYTE northWall; // index into wall slots
  BYTE southWall;
  BYTE eastWall;
  BYTE westWall;
  BlockageType northBlockage; // blockages for all 4 sides
  BlockageType southBlockage; 
  BlockageType eastBlockage;
  BlockageType westBlockage;
};

// Used for viewport map
struct AREA_MAP_DATA 
{
  AREA_MAP_DATA() { Clear(); }
  ~AREA_MAP_DATA() { /*Clear();*/ }
  void Clear();

  // TRUE/FALSE, indicates blockage present for overland maps
  // Using high bit of this var for dungeon maps as flag
  // to indicate showdistant on/off
  BYTE bkgrnd; 

  // this flag is not serialized, instead it is stored using
  // bkgrnd above.
  BYTE ShowDistantBG; // render backgrounds in front of player
                      // or just the one where player is standing
  BYTE DistantBGInBands;

  BYTE northBG; // index into background slots
  BYTE eastBG;
  BYTE southBG;
  BYTE westBG;

  BYTE zone;         // what zone is this spot in?
  BOOL eventExists;  // is there a root event linked to this spot?

  BYTE northWall; // index into wall slots
  BYTE southWall; // Obsolete...see below.
  BYTE eastWall;
  BYTE westWall;

  BlockageType northBlockage; // blockages for all 4 sides
  BlockageType southBlockage; // Obsolete.  See below.
  BlockageType eastBlockage;
  BlockageType westBlockage;

  BlockageType& blockages(int dir);
  BYTE& backgrounds(int dir);
  BYTE& walls(int dir);
  void SetAllBG(int slot) { northBG=eastBG=southBG=westBG=slot; }
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void operator=(const PRE_VERSION_0573_AREA_MAP_DATA &rhs);
  bool operator ==(const AREA_MAP_DATA &rhs);
  AREA_MAP_DATA &operator=(const AREA_MAP_DATA &rhs);
};


enum MappingType { AreaMapping, AutoMapping, NoMapping };
const int NUM_MAPPING_TYPES = 3;

class ZONE //: public CObject
{
public:
//   DECLARE_SERIAL( ZONE )
   ZONE();
   ~ZONE();

   void Serialize(CArchive &ar, double version);
   void Serialize(CAR &ar, double version);

   void GetEventText(char *text); 
   ZONE& operator =(const ZONE& src);
#ifdef UAFEDITOR
   bool operator ==(const ZONE& src);
   void Export(JWriter& jw);
   void Import(JReader& jr);
#endif
   void Clear(BOOL ctor=FALSE);

   MappingType allowMap;
   CString zoneMsg;
   CString zoneName;
   CString indoorCombatArt;
   CString outdoorCombatArt;
   PIC_DATA campArt;
   PIC_DATA treasurePicture;
   REST_EVENT restEvent;
   //int summonedMonster;
   MONSTER_ID summonedMonster;
   int addedTurningDifficulty;
   BOOL AllowMagic;
   BOOL AllowAutoDarken;
   BACKGROUND_SOUND_DATA bgSounds;
   A_ASLENTRY_L zone_asl;
};

class ZONE_DATA //: public CObject
{
public:
//  DECLARE_SERIAL( ZONE_DATA )
  ZONE_DATA() : AVArtSurf(-1) { }
  ~ZONE_DATA() {  Clear(TRUE); }
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void CrossReference(CR_LIST *pCRList, CR_REFERENCE *pCRReference);
  void SaveArt(int level);

  ZONE_DATA& operator =(const ZONE_DATA& src);
#ifdef UAFEDITOR
  void Export(JWriter& jw);
  void Import(JReader& jr);
  bool operator ==(ZONE_DATA& src);
#endif
  void Clear(BOOL ctor=FALSE); 
  void ReleaseSurfaces();

  long GetAreaViewArt();
  CString AVArt;
  long AVArtSurf;
  BOOL AVArtTried;

  ZONE zones[MAX_ZONES];
};

// configures which special keys will be used for
// blockage types LockedKey1 through LockedKey8
//
class BLOCKAGE_KEYS //: public CObject
{
public:
//  DECLARE_SERIAL( BLOCKAGE_KEYS )
  BLOCKAGE_KEYS();
  ~BLOCKAGE_KEYS();

  void Clear();
  BLOCKAGE_KEYS& operator=(const BLOCKAGE_KEYS& src);
#ifdef UAFEDITOR
  void Export(JWriter& jw);
  void Import(JReader& jr);
  bool operator==(const BLOCKAGE_KEYS& src);
#endif
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void CrossReference(CR_LIST *pCRList, CR_REFERENCE *pCRReference);

  int SpecialKeys[MAX_SPECIAL_KEYS];
};


class LEVEL //: public CObject
{
public:
//  DECLARE_SERIAL( LEVEL )
  LEVEL();
  ~LEVEL();
  void Clear(BOOL ctor=FALSE);
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
#ifdef UAFEDITOR
  double GetVersion(FILE *f);
#endif
#ifdef UAFEDITOR
  CONFIG_ITEM_STATUS Export(JWriter& jw);
  CONFIG_ITEM_STATUS Import(JReader& jw);
  CONFIG_ITEM_STATUS ExportCell(JWriter& jw, int x, int y);
  CONFIG_ITEM_STATUS ImportCell(JReader& jr);
  CONFIG_ITEM_STATUS ImportCells(JReader& jr);
  bool               ExportCellDir(JWriter& jw, 
                                   const char *dir, 
                                   const BYTE& wall, 
                                   const BlockageType& blockage, 
                                   const BYTE& background,
                                   bool testDefault);
  void               ImportCellDir(JReader& jw, 
                                   const char *dir, 
                                   BYTE& wall, 
                                   BlockageType& blockage, 
                                   BYTE& background);
  bool operator ==(LEVEL& src);
  void operator =(LEVEL& src);
#endif
  void SaveArt(int level);
  
  int  GetCurrZone(int x, int y);
  int  GetPrevZone() { return PrevZone; }
  void SetPrevZone(int z) { PrevZone = z; }

  void IncRestEventTime(int zone, int mins);
  //void IncTimeEventTime(); // by one minute each time
  BOOL haveZoneMsg(int x, int y);
  BOOL haveEvent(int x, int y);
  BOOL haveSecretDoor(int x, int y); // in any of the 4 sides of this map square
  BOOL haveSecretDoor(int x, int y, int facing); // on particular side of map square
  BOOL haveLockedDoor(int x, int y, int facing);
  BOOL haveSpelledDoor(int x, int y, int facing);
  //BOOL haveTimeEvent(int x, int y, int day, int hour, int minute, GameEvent **stepEvent);
  BOOL haveStepEvent(int x, int y, int stepCount, GameEvent **stepEvent);
  BOOL haveRestEvent(int x, int y, GameEvent **restEvent);
  // event descriptive text for editor
  //void GetTimeEventText(int num, char *text); 
  void GetStepEventText(int num, char *text); 
  void GetRestEventText(int zone, char *text); 
  BOOL ValidAreaMapCoords(int x, int y);
  //GLOBAL_SPELL_ID GetUnlockSpell(int x, int y, int facing);
  CString GetUnlockSpell(int x, int y, int facing);
    
  BYTE area_height;
  BYTE area_width;
  double version;
  AREA_MAP_DATA area[MAX_AREA_HEIGHT][MAX_AREA_WIDTH]; // viewport map
  GameEventList eventData;
  ZONE_DATA zoneData;
  STEP_EVENT_DATA stepEvents[MAX_STEP_EVENTS];
  //STEP_EVENT stepEvents[MAX_STEP_EVENTS];
  mCArray <TIME_EVENT_DATA, TIME_EVENT_DATA&> timeEvents;
  BLOCKAGE_KEYS blockageKeys;
  A_ASLENTRY_L level_asl;
  int PrevZone;

  // These are used just to load/save the images used
  // for this level. There are global equivalents
  // that this data is copied to after loading, and
  // they are used for viewport rendering
  void GetSlots();
  void SetSlots();
  BackgroundSlotMemType m_BackgroundSets[MAX_BACKGROUNDS];
  WallSetSlotMemType m_WallSets[MAX_WALLSETS];
};

//extern LEVEL levelData;

BOOL loadDesign(LPCSTR name);
BOOL loadDesign(CArchive *ar, CAR *car);
BOOL saveDesign();
BOOL saveDesign(CArchive& ar);
BOOL CheckSaveChangedDesign(char *msgIfFailure);
void clearDesign(BOOL FillDefaults=TRUE);
BOOL CopyDesignFiles(CString newFolder);

BOOL LoadLevel(LEVEL &data, int LevelIndex);
BOOL LoadLevel(int levelNum, BOOL loadArt=TRUE);
BOOL saveLevel(LEVEL &data, int LevelIndex);
BOOL saveLevel(int LevelIndex);
BOOL deleteLevel(int num);

BOOL levelExists(int i);
BOOL CheckLevelForWallSlot(int slot);
BOOL CheckLevelForBackgroundSlot(int slot);

#endif 
