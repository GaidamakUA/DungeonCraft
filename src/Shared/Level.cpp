/******************************************************************************
* Filename: Level.cpp
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
#include "../Shared/stdafx.h"
//#include "..\Shared\ProjectVersion.h"


#ifdef UAFEDITOR
#include "../UAFWinEd/UAFWinEd.h"
#include "../UAFWinEd/shelllink.h"
#include <sys/stat.h>
#include "Specab.h"
#include "../UAFWinEd/ItemDB.h"
#include "class.h"
#include "../UAFWinEd/resource.h"
#include "../UAFWinEd/CrossReference.h"
#include "../UAFWinEd/ImportWarning.h"
#else
#include "Externs.h"
#include "../UAFWin/Dungeon.h"
#include "class.h"
#endif

#include "Char.h"
#include "Level.h"
#include "GameEvent.h"
#include "Spell.h"
#include "Graphics.h"
#include "Party.h"
#include "GlobalData.h"
#include "FileParse.h"
#include "Viewport.h"
#include "SoundMgr.h"
#include "Monster.h"
#include <io.h>




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PumpWinMsgs(void);
void die(CString msg);
BOOL MyDeleteFile(LPCSTR name);
BOOL MyCopyFile(LPCSTR src, LPCSTR dest, BOOL failfIfExists);
extern A_CStringInteger_L *fileExistsList;

extern const double VersionSpellNames;
extern const double VersionSpellIDs;
extern const double PRODUCT_VER;



//IMPLEMENT_SERIAL( ZONE, CObject, 1 )
//IMPLEMENT_SERIAL( ZONE_DATA, CObject, 1 )
//IMPLEMENT_SERIAL( BLOCKAGE_KEYS, CObject, 1 )
//IMPLEMENT_SERIAL( LEVEL, CObject, 1 )

// When saving a design the design's version is updated
// to be the most current version. Some decisions rely
// on knowing what the design version was before we updated it.
DWORD PreSaveDesignVer;

LEVEL levelData;

// ASL name is "ZONE_ATTRIBUTES"
ZONE::ZONE() :zone_asl()
{ 
  Clear(TRUE);
}

void ZONE::Serialize(CArchive &ar, double version)
{
//  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << summonedMonster;
    ar << addedTurningDifficulty;
    ar << (int)allowMap;
    ar << AllowMagic;
    ar << AllowAutoDarken;


    AS(ar,zoneMsg);
    AS(ar,zoneName);

    //StripFilenamePath(indoorCombatArt);
    AS(ar, indoorCombatArt);

    //StripFilenamePath(outdoorCombatArt);
    AS(ar, outdoorCombatArt);


    //AddFolderToPath(indoorCombatArt, rte.CombatArtDir());
    //AddFolderToPath(outdoorCombatArt, rte.CombatArtDir());

    bgSounds.Serialize(ar);
  
  }
  else
  {
    int temp;
    ar >> summonedMonster;
    ar >> addedTurningDifficulty;
    ar >> temp;
    allowMap = (MappingType)temp;
    if (version >= _VERSION_0660_)
      ar >> AllowMagic;
    if (version >= _VERSION_0730_)
      ar >> AllowAutoDarken;

    DAS(ar,zoneMsg);
    DAS(ar,zoneName);

    DAS(ar,indoorCombatArt);
    DAS(ar,outdoorCombatArt);
    //AddFolderToPath(indoorCombatArt,  rte.CombatArtDir());
    //AddFolderToPath(outdoorCombatArt, rte.CombatArtDir());

    if (version >= _VERSION_0720_)
      bgSounds.Serialize(ar);
  }
#ifdef SIMPLE_STRUCTURE
  campArt.Serialize(ar, version, rte.PicArtDir());
#else
  campArt.Serialize(ar, version);
#endif
#ifdef SIMPLE_STRUCTURE
  treasurePicture.Serialize(ar, version, rte.PicArtDir());
#else
  treasurePicture.Serialize(ar, version);
#endif
  restEvent.Serialize(ar);
  zone_asl.Serialize(ar, "ZONE_ATTRIBUTES");
}

void ZONE::Serialize(CAR &ar, double version)
{
  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << summonedMonster;
    ar << addedTurningDifficulty;
    ar << (int)allowMap;
    ar << AllowMagic;
    ar << AllowAutoDarken;

    AS(ar,zoneMsg);
    AS(ar,zoneName);
    
    CString tcs;
    //StripFilenamePath(indoorCombatArt);
    AS(ar, indoorCombatArt);

    //StripFilenamePath(outdoorCombatArt);
    AS(ar, outdoorCombatArt);    

    //AddFolderToPath(indoorCombatArt,  rte.CombatArtDir());
    //AddFolderToPath(outdoorCombatArt, rte.CombatArtDir());

    bgSounds.Serialize(ar);
  }
  else
  {
    int temp;
#ifdef UAFEDITOR
    if (version < VersionSpellNames)
    {
      int indx, mon;
      ar >> mon;
      indx = monsterData.LocatePreSpellNamesKey(mon);
      if (indx < 0)
      {
        WriteDebugString("Cannot determine which monster had key = %d\n", mon);
        summonedMonster.Empty();
      }
      else
      {
        summonedMonster = monsterData.PeekMonster(indx)->MonsterID();
      };
    }
    else
#endif
    {
      ar >> summonedMonster;
    };
    ar >> addedTurningDifficulty;
    ar >> temp;
    allowMap = (MappingType)temp;
    if (version >= _VERSION_0660_)
      ar >> AllowMagic;
    if (version >= _VERSION_0730_)
      ar >> AllowAutoDarken;

    DAS(ar,zoneMsg);
    DAS(ar,zoneName);
    DAS(ar,indoorCombatArt);
    StripFilenamePath(indoorCombatArt);
    DAS(ar,outdoorCombatArt);
    StripFilenamePath(outdoorCombatArt);

    if (version >= _VERSION_0720_)
      bgSounds.Serialize(ar);

    //AddFolderToPath(indoorCombatArt,  rte.CombatArtDir());
    //AddFolderToPath(outdoorCombatArt, rte.CombatArtDir());
  }
  campArt.Serialize(ar, version, rte.PicArtDir());
  treasurePicture.Serialize(ar, version, rte.PicArtDir());
  restEvent.Serialize(ar);
  if (ar.IsStoring())
  zone_asl.Serialize(ar, "ZONE_ATTRIBUTES");
  else
    zone_asl.Serialize(ar, "ZONE_ATTRIBUTES");
}

#ifdef UAFEDITOR
const char *JKEY_SUMMONEDMONSTER = "summonedMonster";
const char *JKEY_ADDEDTURNINGDIFFICULTY = "addedTurningDifficulty";
const char *JKEY_ALLOWMAP = "allowMap";
const char *JKEY_ALLOWMAGIC = "AllowMagic";
const char *JKEY_ALLOWAUTODARKEN = "AllowAutoDarken";
const char *JKEY_ZONEMSG = "zoneMsg";
const char *JKEY_ZONENAME = "zoneName";
const char *JKEY_INDOORCOMBATART = "indoorCombatArt";
const char *JKEY_OUTDOORCOMBATART = "outdoorCombatArt";  
const char *JKEY_CAMPART = "campArt";
const char *JKEY_TREASUREPICTURE = "treasurePicture";
const char *JKEY_RESTEVENT = "restEvent";  
void ZONE::Export(JWriter& jw)
{
  jw.StartList();
  jw.NameAndValue(JKEY_SUMMONEDMONSTER ,summonedMonster);
  jw.NameAndValue(JKEY_ADDEDTURNINGDIFFICULTY ,addedTurningDifficulty);
  jw.NameAndEnum(JKEY_ALLOWMAP ,allowMap);
  jw.NameAndBool(JKEY_ALLOWMAGIC ,AllowMagic);
  jw.NameAndBool(JKEY_ALLOWAUTODARKEN ,AllowAutoDarken);
  jw.NameAndValue(JKEY_ZONEMSG ,zoneMsg);
  jw.NameAndValue(JKEY_ZONENAME ,zoneName);
  jw.NameAndValue(JKEY_INDOORCOMBATART ,indoorCombatArt);
  jw.NameAndValue(JKEY_OUTDOORCOMBATART ,outdoorCombatArt);    
  bgSounds.Export(jw);
  campArt.Export(jw, JKEY_CAMPART);
  treasurePicture.Export(jw, JKEY_TREASUREPICTURE);
  restEvent.Export(jw);
  zone_asl.Export(jw);
  jw.EndList();
}
void ZONE::Import(JReader& jr)
{
  jr.StartList();
  jr.NameAndValue(JKEY_SUMMONEDMONSTER ,summonedMonster);
  jr.NameAndValue(JKEY_ADDEDTURNINGDIFFICULTY ,addedTurningDifficulty);
  jr.NameAndEnum(JKEY_ALLOWMAP ,allowMap);
  jr.NameAndBool(JKEY_ALLOWMAGIC ,AllowMagic);
  jr.NameAndBool(JKEY_ALLOWAUTODARKEN ,AllowAutoDarken);
  jr.NameAndValue(JKEY_ZONEMSG ,zoneMsg);
  jr.NameAndValue(JKEY_ZONENAME ,zoneName);
  jr.NameAndValue(JKEY_INDOORCOMBATART ,indoorCombatArt);
  jr.NameAndValue(JKEY_OUTDOORCOMBATART ,outdoorCombatArt);    
  bgSounds.Import(jr);
  campArt.Import(jr, JKEY_CAMPART);
  treasurePicture.Import(jr, JKEY_TREASUREPICTURE);
  restEvent.Import(jr);
  zone_asl.Import(jr);
  jr.EndList();
}
#endif

ZONE::~ZONE() 
{      
 campArt.Clear();
 treasurePicture.Clear();
 restEvent.Clear();
}
#ifdef UAFEDITOR
void ZONE::GetEventText(char *ptext) 
{ 
 sprintf(ptext, "%s: %s", 
         (LPCSTR)zoneName, 
         (LPCSTR)GetEventIdDescription(restEvent.restEvent, LevelEventSrc));
}
#endif
ZONE& ZONE::operator =(const ZONE& src)
{
 if (&src == this) return *this;
 Clear();
 summonedMonster=src.summonedMonster;
 addedTurningDifficulty=src.addedTurningDifficulty;
 allowMap=src.allowMap;
 AllowMagic=src.AllowMagic;
 AllowAutoDarken=src.AllowAutoDarken;
 zoneMsg=src.zoneMsg;
 zoneName=src.zoneName;
 indoorCombatArt=src.indoorCombatArt;
 outdoorCombatArt=src.outdoorCombatArt;
 campArt=src.campArt;
 treasurePicture=src.treasurePicture;
 restEvent=src.restEvent;
 zone_asl.Copy(src.zone_asl);
 bgSounds=src.bgSounds;
 return *this;
}
#ifdef UAFEDITOR
bool ZONE::operator ==(const ZONE& src)
{
 if (&src == this) return true;
 if (summonedMonster != src.summonedMonster) return false;
 if (addedTurningDifficulty != src.addedTurningDifficulty) return false;
 if (allowMap != src.allowMap) return false;
 if (AllowMagic != src.AllowMagic) return false;
 if (AllowAutoDarken != src.AllowAutoDarken) return false;
 if (zoneMsg != src.zoneMsg) return false;
 if (zoneName != src.zoneName) return false;
 if (indoorCombatArt != src.indoorCombatArt) return false;
 if (outdoorCombatArt != src.outdoorCombatArt) return false;
 if (!(campArt == src.campArt)) return false;
 if (!(treasurePicture == src.treasurePicture)) return false;
 if (!(restEvent == src.restEvent)) return false;
 if (!(zone_asl == src.zone_asl)) return false;
 if (!(bgSounds == src.bgSounds)) return false;
 return true;
}
#endif
void ZONE::Clear(BOOL ctor)
{
  summonedMonster=DEFAULT_SUMMONED_MONSTER;
  addedTurningDifficulty=0;
  allowMap=AreaMapping;
  AllowMagic=TRUE;
  AllowAutoDarken=TRUE;
  zoneMsg="";
  zoneName="";
  outdoorCombatArt="";
  indoorCombatArt="";
  campArt.Clear();
  treasurePicture.Clear();
  restEvent.Clear();
  zone_asl.Clear();
  bgSounds.Clear();

#if (defined UAFEDITOR)
 if (!ctor)
 {
#ifdef SIMPLE_STRUCTURE
   indoorCombatArt  = ede.TemplateCombatArtDir() + DEFAULT_CWD;
   outdoorCombatArt = ede.TemplateCombatArtDir() + DEFAULT_CWW;
   campArt.filename = ede.TemplateCampArtDir()   + DEFAULT_CA;
   campArt.picType  = SmallPicDib;
   campArt.SetDefaults();
   treasurePicture.filename =  ede.TemplateSmallPicDir() + DEFAULT_TR;
   treasurePicture.picType = SmallPicDib;
   treasurePicture.SetDefaults();
#else
   indoorCombatArt.Format("%s%s", EditorArt, DEFAULT_CWD);
   outdoorCombatArt.Format("%s%s", EditorArt, DEFAULT_CWW);
   campArt.filename.Format("%s%s", EditorArt, DEFAULT_CA);
   campArt.picType = SmallPicDib;
   campArt.SetDefaults();
   treasurePicture.filename.Format("%s%s", EditorArt, DEFAULT_TR);
   treasurePicture.picType = SmallPicDib;
   treasurePicture.SetDefaults();
#endif
 }
#endif
}

///////////////////////////////////////////////////////////////////////

ZONE_DATA& ZONE_DATA::operator =(const ZONE_DATA& src)
{
  if (&src == this)
    return *this;
  Clear();
  for (int i=0;i<MAX_ZONES;i++) 
    zones[i] = src.zones[i];
  AVArt=src.AVArt;
  AVArtSurf=src.AVArtSurf;
  AVArtTried=src.AVArtTried;
  if (!GraphicsMgr.ValidSurface(AVArtSurf))
    ReleaseSurfaces();
  return *this;
}
#ifdef UAFEDITOR
bool ZONE_DATA::operator ==(ZONE_DATA& src)
{
  if (&src == this) return true;
  for (int i=0;i<MAX_ZONES;i++) 
  {
    if (!(zones[i] == src.zones[i])) return false;
  };
  if (AVArt != src.AVArt) return false;
  if (AVArtSurf != src.AVArtSurf) return false;
  if (AVArtTried != src.AVArtTried) return false;
  return true;
}
#endif
void ZONE_DATA::Clear(BOOL ctor) 
{ 
  ReleaseSurfaces();
  AVArt="";  
  for (int i=0;i<MAX_ZONES;i++) 
  {
    zones[i].Clear(ctor);
    zones[i].zoneName.Format("Zone %i", i+1);
  }
#if (defined UAFEDITOR)
 if (!ctor)
#ifdef SIMPLE_STRUCTURE
    AVArt.Format("%s%s", ede.TemplateAreaViewArtDir(), DEFAULT_ZONEAVART);
#else
    AVArt.Format("%s%s", EditorArt, DEFAULT_ZONEAVART);
#endif
#else
 if (!ctor)
    AVArt = DEFAULT_ZONEAVART;
#endif

}

void ZONE_DATA::ReleaseSurfaces()
{
  if (AVArtSurf >= 0)
    GraphicsMgr.ReleaseSurface(AVArtSurf);
  AVArtSurf=-1;
  AVArtTried=FALSE;
}

long ZONE_DATA::GetAreaViewArt()
{
  if ((AVArtTried) || (AVArt.IsEmpty()))
    return -1;

  if ((AVArtSurf <= 0)||(!GraphicsMgr.ValidSurface(AVArtSurf)))
  {
#ifdef UAFEngine
    AVArtSurf = GraphicsMgr.AddFileSurface(AVArt, TransBufferDib, rte.AreaViewArtDir());
#else
    AVArtSurf = GraphicsMgr.AddFileSurface(AVArt, TransBufferDib, rte.AreaViewArtDir(), ede.TemplateAreaViewArtDir());
#endif

    if (!GraphicsMgr.ValidSurface(AVArtSurf))
    {
      AVArtTried=TRUE;
      WriteDebugString("Failed to load area view art \'%s\' in ZONE_DATA::GetAreaViewArt\n", AVArt);
    }
  }
  return AVArtSurf;
}

void ZONE_DATA::Serialize(CArchive &ar, double version)
{
//  CObject::Serialize(ar);
  int i;
  if (ar.IsStoring())
  {
    ar << MAX_ZONES;
    for (i=0;i<MAX_ZONES;i++)
      zones[i].Serialize(ar, version);
    AS(ar, AVArt);
  }
  else
  {
    Clear();
    if (version >= _VERSION_05661_)
    {
      int count;
      ar >> count;
      for (i=0;i<count;i++)
      {
        zones[i].Clear();
        zones[i].Serialize(ar, version);
      }
    }
    else
    {
      for (i=0;i<8;i++) // load old quantity of zones (= 8)
      {
        zones[i].Clear();
        zones[i].Serialize(ar, version);
      }
    }
    if (version >= _VERSION_0731_)
      DAS(ar, AVArt);
    StripFilenamePath(AVArt);
  }
}
void ZONE_DATA::Serialize(CAR &ar, double version)
{
  //CObject::Serialize((CArchive&)ar);
  int i;
  if (ar.IsStoring())
  {
    ar << MAX_ZONES;
    for (i=0;i<MAX_ZONES;i++)
      zones[i].Serialize(ar, version);

    //StripFilenamePath(AVArt);
    AS(ar, AVArt);
  }
  else
  {
    Clear();
    int count;
    ar >> count;
    for (i=0;i<count;i++)
    {
      zones[i].Clear();
      zones[i].Serialize(ar, version);
    }
    if (version >= _VERSION_0731_)
      DAS(ar, AVArt);
    StripFilenamePath(AVArt);
  }
}

#ifdef UAFEDITOR
const char *JKEY_ZONES = "zones";
const char *JKEY_AVART = "avArt";
void ZONE_DATA::Export(JWriter& jw)
{
  int i;
  jw.StartArray(JKEY_ZONES);
  for (i=0;i<MAX_ZONES;i++)
  {
    jw.NextEntry();
    zones[i].Export(jw);
  };
  jw.EndArray();
  jw.NameAndValue(JKEY_AVART, AVArt);
}
void ZONE_DATA::Import(JReader& jr)
{
  int i;
  jr.StartArray(JKEY_ZONES);
  for (i=0;i<MAX_ZONES;i++)
  {
    jr.Optional();
    if (jr.NextEntry())
    {
      zones[i].Import(jr);
    }
    else
    {
      zones[i].Clear();
    };
  };
  jr.EndArray();
  jr.NameAndValue(JKEY_AVART, AVArt);
}

void ZONE_DATA::CrossReference(CR_LIST *pCRList, CR_REFERENCE *pCRReference)
{
  CR_REFERENCE crRef;
  crRef.m_referenceName = pCRReference->m_referenceName + "[Zone]";
  crRef.m_referenceType = pCRReference->m_referenceType;
  pCRList->CR_AddPicReference(AVArt, &crRef);
}
#endif

void ZONE_DATA::SaveArt(int level)
{
  ::SaveArt(AVArt, CombatDib, level, TRUE, rte.AreaViewArtDir());

  for (int i=0;i<MAX_ZONES; i++)
  {    
    ::SaveArt(zones[i].indoorCombatArt, CombatDib, level, TRUE, rte.CombatArtDir());
    ::SaveArt(zones[i].outdoorCombatArt, CombatDib, level, TRUE, rte.CombatArtDir());
    zones[i].campArt.SavePicData(level);  
    zones[i].treasurePicture.SavePicData(level);
    zones[i].bgSounds.backgroundSounds.SaveSounds(level);
    zones[i].bgSounds.nightSounds.SaveSounds(level);
  }
}

///////////////////////////////////////////////////////////////////////
void AREA_MAP_DATA::Serialize(CAR &ar, double ver)
{
  if (ar.GetCompression()==0)
  {
    Serialize((CArchive&)ar, ver);
    return;
  }

  if (ar.IsStoring())
  {
    if (ShowDistantBG) bkgrnd |= 0x80;
    if (DistantBGInBands) bkgrnd |= 0x40;
    ar << bkgrnd;
    bkgrnd &= 0x3F;
    ar << northBG;
    ar << eastBG;
    ar << southBG;
    ar << westBG;
    ar << zone;
    if (eventExists) ar << (BYTE)1; else ar << (BYTE)0;
    ar << northWall;
    ar << southWall;
    ar << eastWall;
    ar << westWall;
    ar << (BYTE)northBlockage;
    ar << (BYTE)southBlockage;
    ar << (BYTE)eastBlockage;
    ar << (BYTE)westBlockage;
  }
  else
  {
    BYTE tmp;
    ar >> bkgrnd;
    if (bkgrnd & 0x80) ShowDistantBG = TRUE;
    if (bkgrnd & 0x40) DistantBGInBands = TRUE;
    bkgrnd &= 0x3F;

    if ((ver < _VERSION_0695_) && (ShowDistantBG))
      DistantBGInBands=TRUE;

    if (ver >= _VERSION_05771_)
    {
      ar >> northBG;
      ar >> eastBG;
      ar >> southBG;
      ar >> westBG;
    }
    else
    {
      northBG = bkgrnd;
      eastBG = bkgrnd;
      southBG = bkgrnd;
      westBG = bkgrnd;
    }

    ar >> zone;
    ar >> tmp;
    eventExists = tmp!=0;
    ar >> northWall;
    ar >> southWall;
    ar >> eastWall;
    ar >> westWall;
    ar >> tmp; 
    northBlockage = (BlockageType)tmp;
    ar >> tmp; 
    southBlockage = (BlockageType)tmp;
    ar >> tmp; 
    eastBlockage = (BlockageType)tmp;
    ar >> tmp; 
    westBlockage = (BlockageType)tmp;
  }
}

// the compression archive falls back to this if compression is turned off
void AREA_MAP_DATA::Serialize(CArchive &ar, double ver)
{
  if (ar.IsStoring())
  {
    if (ShowDistantBG) bkgrnd |= 0x80; // set high bit
    if (DistantBGInBands) bkgrnd |= 0x40;
    ar << bkgrnd;
    bkgrnd &= 0x3F;

    ar << northBG;
    ar << eastBG;
    ar << southBG;
    ar << westBG;
    ar << zone;
    if (eventExists) ar << (BYTE)1; else ar << (BYTE)0;
    ar << northWall;
    ar << southWall;
    ar << eastWall;
    ar << westWall;
    ar << (BYTE)northBlockage;
    ar << (BYTE)southBlockage;
    ar << (BYTE)eastBlockage;
    ar << (BYTE)westBlockage;
  }
  else
  {
    BYTE tmp;
    ar >> bkgrnd;
    if (bkgrnd & 0x80) ShowDistantBG = TRUE;
    if (bkgrnd & 0x40) DistantBGInBands = TRUE;
    bkgrnd &= 0x3F;
    if ((ver < _VERSION_0695_) && (ShowDistantBG))
      DistantBGInBands=TRUE;

    if (ver >= _VERSION_05771_)
    {
      ar >> northBG;
      ar >> eastBG;
      ar >> southBG;
      ar >> westBG;
    }
    else
    {
      northBG=bkgrnd;
      eastBG=bkgrnd;
      southBG=bkgrnd;
      westBG=bkgrnd;
    }

    ar >> zone;
    ar >> tmp;
    eventExists = tmp != 0;
    ar >> northWall;
    ar >> southWall;
    ar >> eastWall;
    ar >> westWall;
    ar >> tmp; 
    northBlockage = (BlockageType)tmp;
    ar >> tmp; 
    southBlockage = (BlockageType)tmp;
    ar >> tmp; 
    eastBlockage = (BlockageType)tmp;
    ar >> tmp; 
    westBlockage = (BlockageType)tmp; 
  }
}

void AREA_MAP_DATA::Clear()
{
  ShowDistantBG = FALSE;
  DistantBGInBands=FALSE;
  bkgrnd = 0; zone = 0; eventExists = FALSE;
  for (int i=0; i<4; i++)
  {
    backgrounds(i)=0;
    blockages(i)=OpenBlk;
    walls(i)=0;
  };
}

void AREA_MAP_DATA::operator=(const PRE_VERSION_0573_AREA_MAP_DATA &rhs)
{
  // convert old map data into new format
  bkgrnd = rhs.bkgrnd;  
  zone = rhs.zone;
  eventExists = rhs.eventExists;
  northWall = rhs.northWall;
  southWall = rhs.southWall;
  eastWall = rhs.eastWall;
  westWall = rhs.westWall;
  northBlockage = rhs.northBlockage;
  southBlockage = rhs.southBlockage;
  eastBlockage = rhs.eastBlockage;
  westBlockage = rhs.westBlockage;

  // this data is not present in pre-0573 AREA_MAP_DATA
  northBG = rhs.bkgrnd;
  eastBG = rhs.bkgrnd;
  southBG = rhs.bkgrnd;
  westBG = rhs.bkgrnd;
  ShowDistantBG = FALSE;
  DistantBGInBands=TRUE;
}

AREA_MAP_DATA &AREA_MAP_DATA::operator=(const AREA_MAP_DATA &rhs)
{
  if (&rhs==this) return *this;
  bkgrnd=rhs.bkgrnd; 
  ShowDistantBG=rhs.ShowDistantBG;
  DistantBGInBands=rhs.DistantBGInBands;
  northBG=rhs.northBG;
  eastBG=rhs.eastBG;
  southBG=rhs.southBG;
  westBG=rhs.westBG;
  zone=rhs.zone;
  eventExists=rhs.eventExists;
  northWall=rhs.northWall;
  southWall=rhs.southWall;
  eastWall=rhs.eastWall;
  westWall=rhs.westWall;
  northBlockage=rhs.northBlockage;
  southBlockage=rhs.southBlockage;
  eastBlockage=rhs.eastBlockage;
  westBlockage=rhs.westBlockage;
  return *this;
}

#ifdef UAFEDITOR
bool AREA_MAP_DATA::operator==(const AREA_MAP_DATA &rhs)
{
  if (&rhs==this) return true;
  if (bkgrnd != rhs.bkgrnd) return false; 
  if (ShowDistantBG != rhs.ShowDistantBG) return false;
  if (DistantBGInBands != rhs.DistantBGInBands) return false;
  if (northBG != rhs.northBG) return false;
  if (eastBG != rhs.eastBG) return false;
  if (southBG != rhs.southBG) return false;
  if (westBG != rhs.westBG) return false;
  if (zone != rhs.zone) return false;
  if (eventExists != rhs.eventExists) return false;
  if (northWall != rhs.northWall) return false;
  if (southWall != rhs.southWall) return false;
  if (eastWall != rhs.eastWall) return false;
  if (westWall != rhs.westWall) return false;
  if (northBlockage != rhs.northBlockage) return false;
  if (southBlockage != rhs.southBlockage) return false;
  if (eastBlockage != rhs.eastBlockage) return false;
  if (westBlockage != rhs.westBlockage) return false;
  return true;
}
#endif


BYTE& AREA_MAP_DATA::walls(int dir)
{
  static int room_rearranger[4]= 
  { 
    (&northWall)-(&northWall),
    (&eastWall) -(&northWall),
    (&southWall)-(&northWall),
    (&westWall) -(&northWall)
  };
  // Adding 0x10000000 would make it work on 1's-complement machines
  return *((&northWall)+room_rearranger[dir&3]);
}

BlockageType& AREA_MAP_DATA::blockages(int dir)
{
  static int room_rearranger[4]= 
  { 
    (&northBlockage)-(&northBlockage),
    (&eastBlockage) -(&northBlockage),
    (&southBlockage)-(&northBlockage),
    (&westBlockage) -(&northBlockage)
  };
  // Adding 0x10000000 would make it work on 1's-complement machines
  return *((&northBlockage)+room_rearranger[dir&3]);
}

BYTE& AREA_MAP_DATA::backgrounds(int dir)
{
  static int room_rearranger[4]= 
  { 
    (&northBG)-(&northBG),
    (&eastBG) -(&northBG),
    (&southBG)-(&northBG),
    (&westBG) -(&northBG)
  };
  // Adding 0x10000000 would make it work on 1's-complement machines
  return *((&northBG)+room_rearranger[dir&3]);
}

///////////////////////////////////////////////////////////////////////

BLOCKAGE_KEYS::BLOCKAGE_KEYS() 
{ 
  Clear(); 
}

BLOCKAGE_KEYS::~BLOCKAGE_KEYS() { }

void BLOCKAGE_KEYS::Clear() 
{ 
  memset(SpecialKeys, 0, sizeof(SpecialKeys)); 
}

BLOCKAGE_KEYS& BLOCKAGE_KEYS::operator=(const BLOCKAGE_KEYS& src)
{
  if (this==&src) return *this;
  Clear();
  for (int i=0;i<MAX_SPECIAL_KEYS;i++)
    SpecialKeys[i]=src.SpecialKeys[i];
  return *this;
}  

#ifdef UAFEDITOR
bool BLOCKAGE_KEYS::operator==(const BLOCKAGE_KEYS& src)
{
  if (this==&src) return true;
  for (int i=0;i<MAX_SPECIAL_KEYS;i++)
    if (SpecialKeys[i] != src.SpecialKeys[i]) return false;
  return true;
}  

#endif

void BLOCKAGE_KEYS::Serialize(CArchive &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << MAX_SPECIAL_KEYS;
    for (int i=0;i<MAX_SPECIAL_KEYS;i++)
      ar << SpecialKeys[i];
  }
  else
  {
    Clear();
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
      ar >> SpecialKeys[i];
  }
}

void BLOCKAGE_KEYS::Serialize(CAR &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << MAX_SPECIAL_KEYS;
    for (int i=0;i<MAX_SPECIAL_KEYS;i++)
      ar << SpecialKeys[i];
  }
  else
  {
    Clear();
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
      ar >> SpecialKeys[i];
  }
}
#ifdef UAFEDITOR
const char *JKEY_SPECIALKEYS = "specialKeys";
void BLOCKAGE_KEYS::Export(JWriter& jw)
{
  jw.StartArray(JKEY_SPECIALKEYS);
  for (int i=0;i<MAX_SPECIAL_KEYS;i++)
  {
    //CString v;
    //v.Format("%d", SpecialKeys[i]);
    jw.NextEntry();
    jw.NameAndValue(NULL, SpecialKeys[i]);
  };
  jw.EndArray();
}
void BLOCKAGE_KEYS::Import(JReader& jr)
{
  jr.StartArray(JKEY_SPECIALKEYS);
  for (int i=0;i<MAX_SPECIAL_KEYS;i++)
  {
    //CString v;
    //v.Format("%d", SpecialKeys[i]);
    jr.NextEntry();
    jr.NameAndValue(NULL, SpecialKeys[i]);
  };
  jr.EndArray();
}
#endif

void BLOCKAGE_KEYS::CrossReference(CR_LIST *pCRList, CR_REFERENCE *pCRReference)
{
  /*  It appears these keys are not objects at all!
  int i;
  CR_REFERENCE crRef;
  crRef.m_referenceName = pCRReference->m_referenceName + "[KEYS]";
  crRef.m_referenceType = pCRReference->m_referenceType;
  for (i=0; i<MAX_SPECIAL_KEYS; i++)
  {
    pCRList->CR_AddItemReference(SpecialKeys[i], &crRef);
  };
  */
}

#ifdef UAFEDITOR
bool LEVEL::operator ==(LEVEL& src)
{
  int x;
  {  
    if (area_width != src.area_width) return false;
    if (area_height != src.area_height) return false;
    for (int h=0;h<area_height;h++)
    {
      for (int w=0;w<area_width;w++)
      {
        //ar << area[h][w]; // operator<< defined in class.cpp
        if (!(area[h][w] == src.area[h][w])) 
        {
          return false;
        };
      }
    }
  }

  //eventData.m_level = globalData.currLevel;
  //eventData.SetEventSource(LevelEventSrc);
  //ASSERT( (globalData.currLevel >= 0) && (globalData.currLevel < MAX_LEVELS) );
  if (!(eventData == src.eventData)) return false;
  //eventData.SetEventSource(LevelEventSrc);
  if (!(zoneData == src.zoneData)) 
  {
    return false;
  };
  if (!(level_asl == src.level_asl)) return false;

  for (x=0;x<MAX_STEP_EVENTS;x++)
  {
    if (!(stepEvents[x] == src.stepEvents[x])) return false;
  };

  int wall_count;
  //  // always store all wall slots
  wall_count = MAX_WALLSETS;
  //  ar << wall_count;
  for (x=0; x<wall_count; x++)
  {
    if (!(m_WallSets[x] == src.m_WallSets[x])) return false;
  };

  wall_count = MAX_BACKGROUNDS;
    //ar << wall_count;
  for (x=0; x<wall_count; x++)
  {
    if (!(m_BackgroundSets[x] == src.m_BackgroundSets[x])) 
    {
      return false;
    };
  };

    // set to default special keys if index slot is empty
    //for (int i=0;i<MAX_SPECIAL_KEYS;i++)
    //{
    //  if (blockageKeys.SpecialKeys[i]<=0)
    //    blockageKeys.SpecialKeys[i] = globalData.keyData.GetItemKeyUsingSequence(i+1);
    //}

  if (!(blockageKeys == src.blockageKeys)) return false;
  return true;
}

void LEVEL::operator =(LEVEL& src)
{
  NotImplemented(0xd544, false);
}

#endif


///////////////////////////////////////////////////////////////////////

void LEVEL::Serialize(CArchive &ar, double ver)
{
//  CObject::Serialize(ar);
  int x;  

  if (ar.IsStoring())
  {  
    die(0xab525); // shouldn't get here, use CAR instead
  }
  else
  {
    PRE_VERSION_0573_AREA_MAP_DATA tempAREA[50][50]; // old size
    ar.Read(tempAREA, sizeof(tempAREA)); // old size
    // copy to new format
    for (int h=0;h<50;h++)
      for (int w=0;w<50;w++)
        area[h][w] = tempAREA[h][w]; // operator=() performs translation
  }

  eventData.m_level = globalData.currLevel;
  eventData.SetEventSource(LevelEventSrc);
  eventData.Serialize(ar, ver);
  eventData.SetEventSource(LevelEventSrc);
  zoneData.Serialize(ar, ver);
  level_asl.Serialize(ar, "LEVEL_ATTRIBUTES");

  for (x=0;x<MAX_STEP_EVENTS;x++)
     stepEvents[x].Serialize(ar, ver);

  // won't be here unless loading pre-0.5772 data
  // older level files always have 16 wall slots
  for (x=0; x<16; x++)
    m_WallSets[x].Serialize(ar, ver);

  for (x=0; x<16; x++)
     m_BackgroundSets[x].Serialize(ar, ver);
}

#ifdef UAFEDITOR
double LEVEL::GetVersion(FILE *f)
{
  __int64 hdr=0;
  double ver;
  if (fread(&hdr, 1, sizeof(hdr), f) == sizeof(hdr))
  {
    if (hdr == 0xFABCDEFABCDEFABF)
    {
      if (fread(&ver, 1, sizeof(double), f) == sizeof(double))
      {
        return ver;
      };
    }
    else
    {
      // special hdr not present, so reset archive to beginning
      fseek(f, 0, SEEK_SET);
      // set version to last build that didn't save version
      ver = _VERSION_0572_;
      return ver;
    }
  };
  return -1.0;
}
#endif


void LEVEL::Serialize(CAR &ar, double ver)
{
  //CObject::Serialize((CArchive&)ar);
  int x;

  if (ar.IsStoring())
  {  
    ar << area_width;
    ar << area_height;
    for (int h=0;h<area_height;h++)
    {
      for (int w=0;w<area_width;w++)
      {
        //ar << area[h][w]; // operator<< defined in class.cpp
        area[h][w].Serialize(ar, ver);
      }
    }
  }
  else
  {
    ar >> area_width;
    ar >> area_height;
    for (int h=0;h<area_height;h++)
    {
      for (int w=0;w<area_width;w++)
      {
        //ar >> area[h][w];
        area[h][w].Serialize(ar, ver);
      }
    }
  }

  eventData.m_level = globalData.currLevel;
  eventData.SetEventSource(LevelEventSrc);
  ASSERT( (globalData.currLevel >= 0) && (globalData.currLevel < MAX_LEVELS) );
  eventData.Serialize(ar, ver);
  eventData.SetEventSource(LevelEventSrc);
  zoneData.Serialize(ar, ver);
  if (ar.IsStoring())
      level_asl.Serialize(ar, "LEVEL_ATTRIBUTES");
  else
    level_asl.Serialize(ar, "LEVEL_ATTRIBUTES");
  {
    int numStep;
    if (ver < 1.0210) numStep = 8; else numStep = MAX_STEP_EVENTS;
    for (x=0;x<numStep;x++)
     stepEvents[x].Serialize(ar, ver);
  };

  int wall_count;
  if (ar.IsStoring())
  {
    // always store all wall slots
    wall_count = MAX_WALLSETS;
    ar << wall_count;
    for (x=0; x<wall_count; x++)
      m_WallSets[x].Serialize(ar, ver);

    wall_count = MAX_BACKGROUNDS;
    ar << wall_count;
    for (x=0; x<wall_count; x++)
      m_BackgroundSets[x].Serialize(ar, ver);

    // set to default special keys if index slot is empty
    for (int i=0;i<MAX_SPECIAL_KEYS;i++)
    {
      if (blockageKeys.SpecialKeys[i]<=0)
        blockageKeys.SpecialKeys[i] = globalData.keyData.GetItemKeyUsingSequence(i+1);
    }

    blockageKeys.Serialize(ar, ver);
  }
  else
  {
    // older levels just stored 16 wall slots
    if (ver < _VERSION_0600_)
    {
      for (x=0; x<16; x++)
        m_WallSets[x].Serialize(ar, ver);
    }
    else
    {
      ar >> wall_count;
      ASSERT( wall_count <= MAX_WALLSETS );
      wall_count = min(wall_count, MAX_WALLSETS);
      for (x=0; x<wall_count; x++)
        m_WallSets[x].Serialize(ar, ver);
    }

    // older levels just stored 16 background slots
    if (ver < _VERSION_0660_)
    {
      for (x=0; x<16; x++)
        m_BackgroundSets[x].Serialize(ar, ver);
    }
    else
    {
      ar >> wall_count;
      ASSERT( wall_count <= MAX_BACKGROUNDS );
      wall_count = min(wall_count, MAX_BACKGROUNDS);
      for (x=0; x<wall_count; x++)
        m_BackgroundSets[x].Serialize(ar, ver);
    }

    if (ver >= _VERSION_0842_)
    {
      blockageKeys.Serialize(ar, ver);
    }
    else
    {   
      // global data is loaded prior to level data
      for (int i=0;i<MAX_SPECIAL_KEYS;i++)
        blockageKeys.SpecialKeys[i] = globalData.keyData.GetItemKeyUsingSequence(i+1);
    }
  }
}

#ifdef UAFEDITOR

const char *JKEY_WIDTH      = "width";
const char *JKEY_HEIGHT     = "height";
const char *JKEY_PREVZONE   = "prevZone";
const char *JKEY_VERSION    = "version";
const char *JKEY_MAPGRID    = "map_grid";
const char *JKEY_LOCATION   = "l";
const char *JKEY_WALLS      = "walls";
const char *JKEY_BLOCKAGE   = "blockage";
const char *JKEY_CELL       = "cell";
const char *JKEY_BG         = "bg";
const char *JKEY_BKDROP     = "backdrop";
const char *JKEY_SHOWDISTBG = "showDistBg";
const char *JKEY_DSTBGBANDS = "DistantBgInBands";
const char *JKEY_ZONE       = "zone";
const char *JKEY_EVENT      = "event";
const char *JKEY_EVENTEXISTS= "eventExists";
const char *JKEY_DIR        = "dir";
const char *JKEY_WALL       = "wall";
const char *JKEY_BLOCK      = "block";
const char *JKEY_BACKGROUND = "background";
extern const char *JKEY_XY;

bool LEVEL::ExportCellDir(JWriter& jw, 
                          const char *dir, 
                          const BYTE& wall, 
                          const BlockageType& blockage,
                          const BYTE& background,
                          bool  testDefault)
{
  if ((wall | blockage | background) == 0) return true;
  if (testDefault) return false;
  jw.StartList(dir);
  jw.Linefeed(false);
  if (wall != 0)       jw.NameAndValue(JKEY_WALL, wall+1);
  if (blockage != 0)   jw.NameAndEnum(JKEY_BLOCK, blockage);
  if (background != 0) jw.NameAndValue(JKEY_BACKGROUND, background);
  jw.EndList();
  jw.Linefeed(true);
  return false;
}

void LEVEL::ImportCellDir(JReader& jr, 
                          const char *dir, 
                          BYTE& wall, 
                          BlockageType& blockage,
                          BYTE& background)
{
  wall = 0;
  blockage = OpenBlk;
  background = 0;
  if (jr.Optional(), !jr.StartList(dir)) 
  {
    return;
  };
  jr.Linefeed(false);
  jr.Optional(); jr.NameAndValue(JKEY_WALL, wall);
  wall--;
  jr.Optional(); jr.NameAndEnum(JKEY_BLOCK, blockage);
  jr.Optional(); jr.NameAndValue(JKEY_BACKGROUND, background);
  jr.EndList();
  jr.Linefeed(true);
}


CONFIG_ITEM_STATUS LEVEL::ExportCell(JWriter& jw, int x, int y)
{
  AREA_MAP_DATA *c;
  bool allDefault;
  int i[2];
  c = &area[y][x];
  allDefault =   ExportCellDir(jw, "N",c->northWall, c->northBlockage, c->northBG, true)
              && ExportCellDir(jw, "E",c->eastWall,  c->eastBlockage,  c->eastBG, true)
              && ExportCellDir(jw, "S",c->southWall, c->southBlockage, c->southBG, true)
              && ExportCellDir(jw, "W",c->westWall,  c->westBlockage,  c->westBG, true);
  if (c->eventExists)      allDefault = false;
  if (c->zone   != 0)      allDefault = false;
  if (c->bkgrnd != 0)      allDefault = false;
  if (c->ShowDistantBG)    allDefault = false;
  if (c->DistantBGInBands) allDefault = false;
  if (!allDefault)
  {
    jw.NextEntry();
    i[0] = x;
    i[1] = y;
    jw.StartList();
    jw.NameAndValue(JKEY_XY, i, 2);
    jw.Linefeed(false);
    if (c->eventExists)           jw.NameAndBool(JKEY_EVENTEXISTS, c->eventExists);
                                  jw.NameAndValue(JKEY_ZONE, c->zone+1);
    if (c->bkgrnd != 0)           jw.NameAndValue(JKEY_BKDROP, c->bkgrnd);
    if (c->ShowDistantBG != 0)    jw.NameAndBool(JKEY_SHOWDISTBG, c->ShowDistantBG);
    if (c->DistantBGInBands != 0) jw.NameAndBool(JKEY_DSTBGBANDS, c->DistantBGInBands);
    jw.Linefeed(true);
    ExportCellDir(jw, "N",c->northWall, c->northBlockage, c->northBG, false);
    ExportCellDir(jw, "E",c->eastWall,  c->eastBlockage,  c->eastBG, false);
    ExportCellDir(jw, "S",c->southWall, c->southBlockage, c->southBG, false);
    ExportCellDir(jw, "W",c->westWall,  c->westBlockage,  c->westBG, false);
    jw.EndList();
  };
  return CONFIG_STAT_ok;
}

CONFIG_ITEM_STATUS LEVEL::ImportCell(JReader& jr)
{
  AREA_MAP_DATA *c;
  int i[2];
  int x, y;
  jr.StartList();
  jr.NameAndValue(JKEY_XY, i, 2);
  x = i[0];
  y = i[1];
  c = &area[y][x];
  jr.Linefeed(false);
  jr.Optional(); jr.NameAndBool(JKEY_EVENTEXISTS, c->eventExists);
  c->zone = 1;
  jr.Optional(); jr.NameAndValue(JKEY_ZONE, c->zone);
  c->zone--;
  jr.Optional(); jr.NameAndValue(JKEY_BKDROP, c->bkgrnd);
  jr.Optional(); jr.NameAndBool(JKEY_SHOWDISTBG, c->ShowDistantBG);
  jr.Optional(); jr.NameAndBool(JKEY_DSTBGBANDS, c->DistantBGInBands);
  jr.Linefeed(true);
  ImportCellDir(jr, "N",c->northWall, c->northBlockage, c->northBG);
  ImportCellDir(jr, "E",c->eastWall,  c->eastBlockage,  c->eastBG);
  ImportCellDir(jr, "S",c->southWall, c->southBlockage, c->southBG);
  ImportCellDir(jr, "W",c->westWall,  c->westBlockage,  c->westBG);
  jr.EndList();
  return CONFIG_STAT_ok;
}


CONFIG_ITEM_STATUS LEVEL::ImportCells(JReader& jr)
{
  int x, y;
  for (x=0; x<area_width; x++)
  {
    for (y=0; y<area_height; y++)
    {
      area[x][y].Clear();
    };
  };
  /*
  while (jr.FindAndRemoveOpenBrace(JKEY_CELL))
  {
    AREA_MAP_DATA *c;
    int i[4];
    jr.NameAndValue(JKEY_LOCATION, i, 2);
    x = i[0];
    y = i[1];
    c = &area[x][y];
    jr.NameAndValue(JKEY_WALL, i, 4);
    jr.NameAndValue(JKEY_BLOCKAGE, i, 4);
    jr.NameAndValue(JKEY_BG, i, 4);
    jr.NameAndValue(JKEY_ZONE, c->zone);
    jr.NameAndValue(JKEY_EVID, c->event);
    jr.NameAndValue(JKEY_BKGRND, i, 3);
    jr.DeleteName();
    jr.CloseBrace();
  };
  */
  NotImplemented(0xce67, false);
  return CONFIG_STAT_ok;
}

const char *JKEY_LEVELDATA = "levelData";
const char *JKEY_BACKGROUNDSETS = "backgroundSets";
extern const char *JKEY_STEPEVENTS;

CONFIG_ITEM_STATUS LEVEL::Export(JWriter& jw)
{
  jw.StartList(JKEY_LEVELDATA);
  jw.NameAndValue(JKEY_WIDTH, area_width);
  jw.NameAndValue(JKEY_HEIGHT, area_height);
  jw.NameAndValue(JKEY_VERSION, version);
  jw.NameAndValue(JKEY_PREVZONE, PrevZone);
  {
    int x, y;
    jw.StartArray(JKEY_MAPGRID);
    for (y=0; y<area_height; y++)
    {
      for (x=0; x<area_width; x++)
      {
        // Handled by ExportCell if non-default cell.  jw.NextEntry();
        ExportCell(jw,x,y);
      };
    };
    jw.EndArray();
  };
  eventData.Export(jw);
  zoneData.Export(jw);
  level_asl.Export(jw);

  int wall_count;
  {
    int x;
    jw.StartArray(JKEY_STEPEVENTS);
    for (x=0;x<MAX_STEP_EVENTS;x++)
    {
      jw.NextEntry();
      stepEvents[x].Export(jw);
    };
    jw.EndArray();


    // always store all wall slots
    wall_count = MAX_WALLSETS;
    jw.StartArray(JKEY_WALLS);
    for (x=0; x<wall_count; x++)
    {
      jw.NextEntry();
      m_WallSets[x].Export(jw);
    };
    jw.EndArray();


    wall_count = MAX_BACKGROUNDS;
    jw.StartArray(JKEY_BACKGROUNDSETS);
    for (x=0; x<wall_count; x++)
    {
      jw.NextEntry();
      m_BackgroundSets[x].Export(jw);
    };
    jw.EndArray();
    blockageKeys.Export(jw);
  };
  jw.EndList();
  return CONFIG_STAT_ok;
}

extern const char *JKEY_LEVELINFO;

CONFIG_ITEM_STATUS LEVEL::Import(JReader& jr)
{
  jr.StartList(JKEY_LEVELDATA);
  jr.NameAndValue(JKEY_WIDTH, area_width);
  jr.NameAndValue(JKEY_HEIGHT, area_height);
  jr.NameAndValue(JKEY_VERSION, version);
  jr.NameAndValue(JKEY_PREVZONE, PrevZone);
  {
    jr.StartArray(JKEY_MAPGRID);
    while (jr.Optional(), jr.NextEntry())
    {
      ImportCell(jr);
    };
    jr.EndArray();
  };
  eventData.Import(jr);
  zoneData.Import(jr);
  level_asl.Import(jr);
  {
    int wall_count;
    int x;
    jr.StartArray(JKEY_STEPEVENTS);
    for (x=0;x<MAX_STEP_EVENTS;x++)
    {
      jr.Optional();
      if (jr.NextEntry())
      {
      stepEvents[x].Import(jr);
      }
      else
      {
        stepEvents[x].Clear();
      };
    };
    jr.EndArray();


    // always store all wall slots
    wall_count = MAX_WALLSETS;
    jr.StartArray(JKEY_WALLS);
    for (x=0; x<wall_count; x++)
    {
      jr.Optional();
      if (jr.NextEntry())
      {
        m_WallSets[x].Import(jr);
      }
      else
      {
        m_WallSets[x].Clear();
      };
    };
    jr.EndArray();


    wall_count = MAX_BACKGROUNDS;
    jr.StartArray(JKEY_BACKGROUNDSETS);
    for (x=0; x<wall_count; x++)
    {
      jr.Optional();
      if (jr.NextEntry())
      {
        m_BackgroundSets[x].Import(jr);
      }
      else
      {
        m_BackgroundSets[x].Clear();
      };
    };
    jr.EndArray();
    blockageKeys.Import(jr);
  };
  
  
  
  jr.EndList();
  return CONFIG_STAT_ok;
}
#endif

void LEVEL::Clear(BOOL ctor)
{
   int i;   
   area_height = 0;
   area_width = 0;
   version = 0.0;
   memset(area, 0, sizeof(area));
   eventData.Clear();
   eventData.SetEventSource(LevelEventSrc);
   for (i=0;i<MAX_ZONES;i++)
    eventData.DeleteEvents(zoneData.zones[i].restEvent.restEvent);
   zoneData.Clear(ctor);
   blockageKeys.Clear();
   for (i=0;i<MAX_STEP_EVENTS;i++)
   {
     eventData.DeleteEvents(stepEvents[i].stepEvent);
     stepEvents[i].Clear();
   }

   level_asl.Clear();
   SetPrevZone(-1);

   int x;
   for (x=0; x<MAX_WALLSETS; x++)
     m_WallSets[x].Clear();

   for (x=0; x<MAX_BACKGROUNDS; x++)
     m_BackgroundSets[x].Clear();
}

void LEVEL::SaveArt(int level)
{
  eventData.saveUsedEventArt(level);
  zoneData.SaveArt(level);
  eventData.saveUsedEventSounds(level);

  int x;
  for (x=0; x<MAX_WALLSETS; x++)
  {
    m_WallSets[x].SaveArt(level);
    m_WallSets[x].SaveSound(level);
  }

  for (x=0; x<MAX_BACKGROUNDS; x++)
  {
     m_BackgroundSets[x].SaveArt(level);
     m_BackgroundSets[x].SaveSound(level);
  }
}

// load global slots into level slots
void LEVEL::GetSlots()
{
  int x;
  for (x=0; x<MAX_WALLSETS; x++)
    m_WallSets[x]=WallSets[x];

  for (x=0; x<MAX_BACKGROUNDS; x++)
     m_BackgroundSets[x]=BackgroundSets[x];
}

// load level slots into global slots
void LEVEL::SetSlots()
{
  ASSERT( this == &levelData );
  int x;
  for (x=0; x<MAX_WALLSETS; x++)
    WallSets[x]=m_WallSets[x];

  for (x=0; x<MAX_BACKGROUNDS; x++)
    BackgroundSets[x]=m_BackgroundSets[x];
}

// ASL name is "LEVEL_ATTRIBUTES"
LEVEL::LEVEL() :level_asl()
{ 
 area_height = 0;
 area_width = 0;
 memset(area, 0, sizeof(area));
 Clear(TRUE);
}
LEVEL::~LEVEL() 
{ 
  Clear(TRUE); 
}
int  LEVEL::GetCurrZone(int x, int y) 
{ 
  ASSERT(ValidAreaMapCoords(x,y));
  ASSERT((area[y][x].zone >=0)&&(area[y][x].zone<MAX_ZONES));
  return area[y][x].zone; 
}

void LEVEL::IncRestEventTime(int zone, int mins)
{
  if (eventData.IsValidEvent(zoneData.zones[zone].restEvent.restEvent))
    zoneData.zones[zone].restEvent.prevMinChecked += mins;
}
/*
void LEVEL::IncTimeEventTime()
{
  for (int i=0; i<MAX_TIME_EVENTS; i++)
  {
    if (eventData.IsValidEvent(timeEvents[i].timeEvent))
    {
      int mn, hr, day;
      
      day = 0;
      hr = 0;
      mn = 1;
       
      timeEvents[i].minuteCounter += mn;
      
      if (timeEvents[i].minuteCounter >= 60)
      { 
        timeEvents[i].minuteCounter -= 60;
        hr++;
      }
      
      timeEvents[i].hourCounter += hr;
      
      if (timeEvents[i].hourCounter >= 24)
      {
        timeEvents[i].hourCounter -= 24;
        timeEvents[i].dayCounter++;
      }
    }
  }
}
*/

BOOL LEVEL::haveZoneMsg(int x, int y)
{ 
  return (strlen(zoneData.zones[GetCurrZone(x,y)].zoneMsg) > 0);
}

BOOL LEVEL::haveEvent(int x, int y)
{
  ASSERT(ValidAreaMapCoords(x,y));
  return (area[y][x].eventExists);
}

BOOL LEVEL::haveSecretDoor(int x, int y)
{
  //AREA_MAP_DATA data = area[y][x];
  for (int i=0;i<4;i++)
  {
    if (haveSecretDoor(x,y,i))
      return TRUE;
    //BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,i);
    //if ((data.blockages(i)==OpenSecretBlk) && (isSecret)) return TRUE;
    //if ((data.blockages(i)==LockedSecretBlk) && (isSecret)) return TRUE;
    //if ((data.blockages(i)==LockedWizardSecretBlk) && (isSecret)) return TRUE;
  }
  return FALSE;
}

BOOL LEVEL::haveSecretDoor(int x, int y, int facing)
{
  ASSERT(ValidAreaMapCoords(x,y));  
  AREA_MAP_DATA data = area[y][x];
  BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,facing);
  if (!isSecret) return FALSE;
  if (data.blockages(facing)==OpenSecretBlk) return TRUE;
  if (data.blockages(facing)==LockedSecretBlk) return TRUE;
  if (data.blockages(facing)==LockedWizardSecretBlk) return TRUE;
  return FALSE;
}

BOOL LEVEL::haveLockedDoor(int x, int y, int facing)
{
  // only care about locked doors that need (or can be) bashed, so
  // ignore LockedKey1, etc.
  //
  ASSERT(ValidAreaMapCoords(x,y));
  AREA_MAP_DATA data = area[y][x];
  if (!party.blockageData.IsLocked(GetCurrentLevel(),x,y,facing)) return FALSE;
  BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,facing);
  BOOL isSpelled=party.blockageData.IsSpelled(GetCurrentLevel(),x,y,facing);
  if (data.blockages(facing)==LockedBlk) return TRUE;
  if ((!isSecret) && (data.blockages(facing)==LockedSecretBlk)) return TRUE;
  if ((!isSpelled) && (data.blockages(facing)==LockedWizardBlk)) return TRUE;
  if ((!isSecret) && (!isSpelled) && (data.blockages(facing)==LockedWizardSecretBlk)) return TRUE;
  return FALSE;
}

BOOL LEVEL::haveSpelledDoor(int x, int y, int facing)
{
  ASSERT(ValidAreaMapCoords(x,y));
  AREA_MAP_DATA data = area[y][x];
  
  if (!party.blockageData.IsSpelled(GetCurrentLevel(),x,y,facing)) return FALSE;
  BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,facing);
  if (data.blockages(facing)==LockedWizardBlk) return TRUE;
  if ((!isSecret) && (data.blockages(facing)==LockedWizardSecretBlk)) return TRUE;
  return FALSE;
}

//GLOBAL_SPELL_ID LEVEL::GetUnlockSpell(int x, int y, int facing)
CString LEVEL::GetUnlockSpell(int x, int y, int facing)
{
  ASSERT(ValidAreaMapCoords(x,y));
  AREA_MAP_DATA data = area[y][x];
  //GLOBAL_SPELL_ID spell; // starts clear = -1;
  CString spell; // starts clear = -1;
  switch (facing)
  {
  case FACE_NORTH:
    spell = WallSets[data.northWall].UnlockSpellID;
    break;
  case FACE_EAST:
    spell = WallSets[data.eastWall].UnlockSpellID;
    break;
  case FACE_SOUTH:
    spell = WallSets[data.southWall].UnlockSpellID;
    break;
  case FACE_WEST:
    spell = WallSets[data.westWall].UnlockSpellID;
    break;
  }
  return spell;
}

/*
BOOL LEVEL::haveTimeEvent(int x, int y, int day, int hour, int minute, GameEvent **timeEvent)
{
  BOOL TE_Found = FALSE;
  if (timeEvent == NULL) return FALSE;
  
  // scan for first time event that should trigger
  for (int i=0; (i<MAX_TIME_EVENTS) && (!TE_Found); i++)
  {
    switch (timeEvents[i].triggerType)
    {
    case TIME_EVENT::ABSOLUTE_TIME:
      {
        if (   (day == timeEvents[i].day)
            && (hour == timeEvents[i].hour)
            && (minute == timeEvents[i].minute))
        {
          // if event data is present...
          if (eventData.IsValidEvent(timeEvents[i].timeEvent))
          {
            *timeEvent = eventData.GetEvent(timeEvents[i].timeEvent);
            TE_Found=TRUE;
          }
        }
      }
      break;
    case TIME_EVENT::REPEAT_TIME:
      {        
        if (   (timeEvents[i].day > 0)
            || (timeEvents[i].hour > 0)
            || (timeEvents[i].minute > 0))
        {
          // at least one of them is configured with a trigger
          BOOL dayTrigger = TRUE;
          BOOL hourTrigger = TRUE;
          BOOL minuteTrigger = TRUE;

          if (timeEvents[i].day > 0)
            dayTrigger = ((timeEvents[i].day % day) == 0);

          if (timeEvents[i].hour > 0) // change hour to 1-24 (from 0-23)
            hourTrigger = ((timeEvents[i].hour % (hour+1)) == 0);

          if (timeEvents[i].minute > 0) // change minute to 1-60 (from 0-59)
            minuteTrigger = ((timeEvents[i].minute % (minute+1)) == 0);

          if ( dayTrigger && hourTrigger && minuteTrigger )
          {
            // if event data is present...
            if (eventData.IsValidEvent(timeEvents[i].timeEvent))
            {
              *timeEvent = eventData.GetEvent(timeEvents[i].timeEvent);
              TE_Found=TRUE;
            }
          }
        }
      }
      break;
    }
  }
  
  return TE_Found;
}
*/

BOOL LEVEL::haveStepEvent(int x, int y, int stepCount, GameEvent **stepEvent)
{
  BOOL SE_Found = FALSE;
  int zone = GetCurrZone(x,y);
  if (stepEvent == NULL) return FALSE;
  // scan for first step event that should trigger
  for (int i=0; (i<MAX_STEP_EVENTS) && (!SE_Found); i++)
  {
    // if this zone can trigger and count is multiple of step count
    //if (   (stepEvents[i].stepTrigger[zone] > 0) // does this zone trigger?
    if (   ((stepEvents[i].zoneMask & (1 << zone)) > 0) // does this zone trigger?
        && (stepEvents[i].stepCount > 0)         // must have non-zero trigger count
        && ((stepCount % stepEvents[i].stepCount) == 0) )
    {
      // if event data is present...
      if (eventData.IsValidEvent(stepEvents[i].stepEvent))
      {
        *stepEvent = eventData.GetEvent(stepEvents[i].stepEvent);
        SE_Found=TRUE;
      }
    }
  }

  return SE_Found;
}

BOOL LEVEL::haveRestEvent(int x, int y, GameEvent **restEvent)
{
  BOOL RE_Found = FALSE;

  int zone = GetCurrZone(x, y);
  if (restEvent == NULL) return FALSE;

  if (   (eventData.IsValidEvent(zoneData.zones[zone].restEvent.restEvent))
      && (zoneData.zones[zone].restEvent.everyMin > 0)
      && (zoneData.zones[zone].restEvent.chance > 0))
  {
    if (zoneData.zones[zone].restEvent.prevMinChecked >= zoneData.zones[zone].restEvent.everyMin)
    {
      zoneData.zones[zone].restEvent.prevMinChecked = 0;
      int result = RollDice(100, 1, 0);
    
      WriteDebugString("Rest event chance %i, result %i\n",
                       zoneData.zones[zone].restEvent.chance, result);
    
      if (result <= zoneData.zones[zone].restEvent.chance)
      {
        *restEvent = eventData.GetEvent(zoneData.zones[zone].restEvent.restEvent);
        RE_Found=TRUE;
      }
    }
  }

  return RE_Found;
}
#ifdef UAFEDITOR
// event descriptive text for editor
/*
void LEVEL::GetTimeEventText(int num, char *ptext) 
{ 
  if ((num >= 0) && (num < MAX_TIME_EVENTS))
  {
    sprintf(ptext, "Time Event %u: %s", 
      num+1, 
      (LPCSTR)GetEventIdDescription(timeEvents[num].timeEvent, LevelEventSrc));
  }
}
*/
void LEVEL::GetStepEventText(int num, char *ptext) 
{ 
  if ((num >= 0) && (num < MAX_STEP_EVENTS))
  {
    //sprintf(ptext, "Step Event %u: %s", 
    //        num+1, 
    //        (LPCSTR)GetEventIdDescription(stepEvents[num].stepEvent, LevelEventSrc));
    sprintf(ptext, "(%u)%s: %s", 
            num+1, (LPCSTR)stepEvents[num].name,
            (LPCSTR)GetEventIdDescription(stepEvents[num].stepEvent, LevelEventSrc));
  }
}
void LEVEL::GetRestEventText(int zone, char *ptext) 
{ 
  if ((zone >= 0) && (zone < MAX_ZONES))
  {
    zoneData.zones[zone].GetEventText(ptext);
  }
}
#endif
BOOL LEVEL::ValidAreaMapCoords(int x, int y)
{
  return ((x >= 0) && (x < area_width) && (y >= 0) && (y < area_height));
}

///////////////////////////////////////////////////////////////////////


BOOL saveLevel(LEVEL &data, int LevelIndex)
{
   BOOL success = FALSE;  

   CFile myFile;
   CFileException e;

#ifdef SIMPLE_STRUCTURE
   CString fullPath;
   fullPath.Format("%sLevel%.3i.lvl", rte.DataDir(), LevelIndex+1);
#else
   char temp[4];
   char fullPath[_MAX_PATH+1];
   GetDesignPath(fullPath);
   strcat(fullPath, "Level");
   sprintf(temp, "%.3i", LevelIndex);
   strcat(fullPath, temp);
   strcat(fullPath, ".lvl");
#endif

   SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
   success = myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive, &e);

   if (!success)
      WriteDebugString("Unable to open level file %s, error %i\n", fullPath, e.m_cause);

   if (success)
   {
     // put signal value and version at start of file
     __int64 hdr=0xFABCDEFABCDEFABF;
     double ver = PRODUCT_VER;
     myFile.Write(&hdr, sizeof(hdr));
     myFile.Write(&ver, sizeof(double));

     CAR ar(&myFile, CArchive::store);
     //ar.Compress(true); // qqqqq
   
     try 
     {
        data.Serialize(ar, ver);
        success = TRUE;
     }
     catch (...) 
     {
        WriteDebugString("Unable to save level file %s\n", ar.GetFile()->GetFileName());
        success = FALSE;
     }

     ar.Close();
   }

   return success;
}

BOOL saveLevel(int LevelIndex)
{
   //ASSERT( LevelIndex == globalData.currLevel ); // won't hold true when importing UA levels
   EditorStatusMsg("Saving level data...");

   levelData.GetSlots();

   BOOL success = saveLevel(levelData, LevelIndex);

   if (success)
      EditorStatusMsg("Level data saved.");
   else
      EditorStatusMsg("Unable to save level data.");

#ifdef UAFEDITOR
   EditorOptionsData.m_LastSaveSuccess=success;
   EditorOptionsData.m_LastDesign = GetDesignFolderRoot();
   EditorOptionsData.m_LastDesignLevel = globalData.currLevel;
#endif

   return success;
}

BOOL LoadLevel(LEVEL &data, int LevelIndex)
{
   BOOL success = FALSE;
   CFile myFile;
   double ver=0.0;
   char fullPath[_MAX_PATH+1];
   char temp[4];

   sprintf(temp, "%.3i", LevelIndex+1);

   strcpy(fullPath, rte.DataDir());
   strcat(fullPath, "Level");
   strcat(fullPath, temp);
   strcat(fullPath, ".lvl");   

   success = myFile.Open(fullPath, CFile::modeRead);

   if (!success)
   {
     WriteDebugString("Unable to open level file %s\n", fullPath);
     return FALSE;
   }

   data.Clear();

   /*
   int x;
   for (x=0; x<MAX_WALLSETS; x++)
     data.m_WallSets[x].Clear();

   for (x=0; x<MAX_BACKGROUNDS; x++)
     data.m_BackgroundSets[x].Clear();
    */

  // check for version
  __int64 hdr=0;
  myFile.Read(&hdr, sizeof(hdr));
  if (hdr == 0xFABCDEFABCDEFABF)
  {
    myFile.Read(&ver, sizeof(double));
  }
  else
  {
    // special hdr not present, so reset archive to beginning
    myFile.SeekToBegin();
    // set version to last build that didn't save version
    ver = _VERSION_0572_;
  }

  data.version = ver;

  // if loading older level data, don't use compression archive
  if (ver < _VERSION_0573_)
  {
    CArchive ar(&myFile, CArchive::load);
    try 
    {  
        data.Serialize(ar, ver);
        success = TRUE;
     }
     catch (...) 
     {
        success = FALSE;
        WriteDebugString("Unable to load level data file %s\n", fullPath);
     }

    ar.Close();
  }
  else
  {
    CAR ar(&myFile, CArchive::load);
    //ar.Compress(true); // qqqqq
    try 
    {
      data.Serialize(ar, ver);
      success = TRUE;
    }
    catch (...) 
    {
       success = FALSE;
       WriteDebugString("Unable to load level data file %s\n", fullPath);
    }
    ar.Close();
  } 
  
   if (success)
   {
     data.area_height = globalData.levelInfo.stats[LevelIndex].area_height;
     data.area_width = globalData.levelInfo.stats[LevelIndex].area_width;
     data.version = ver;
   }

   return success;
}

BOOL LoadLevel(int LevelIndex, BOOL loadArt)
{
   EditorStatusMsg("Loading level data...");

   BOOL success = LoadLevel(levelData, LevelIndex);

   if (success)
   {   
     globalData.currLevel = LevelIndex;
     StopAllSounds();
     deleteAllLevelArt(LevelIndex);
     deleteAllLevelSound(LevelIndex);

     levelData.SetSlots();
     if (loadArt)
     {
        WriteDebugString("Start loading WallSetSurfaces\n");
        if (!loadWallSetSurfaces())
        {
          debugSeverity = 7;
          WriteDebugString("Failed to load WallSetSurfaces\n");
        };
     }
     EditorStatusMsg("Level data loaded.");
     WriteDebugString("Level %u loaded, %ix%i, ver %4.7f\n",
                      LevelIndex+1, 
                      levelData.area_width,levelData.area_height,
                      levelData.version);
   }
   else
     EditorStatusMsg("Unable to load level data.");

#ifdef UAFEDITOR
   EditorOptionsData.m_LastLoadSuccess=success;
   EditorOptionsData.m_LastDesign = GetDesignFolderRoot();
   EditorOptionsData.m_LastDesignLevel = globalData.currLevel;
   UndoData.RemoveAll();
#endif
   return success;
}


#if (defined UAFEDITOR)

BOOL FirstTimeFileAction = TRUE;

/*
 // We should never again use CArchive to save a design
BOOL saveDesign(CArchive& ar)
{
   BOOL success = TRUE;
   int i;
   
   EditorStatusMsg("Saving design data...");
#ifdef SIMPLE_STRUCTURE
#else
   SetCurrentDirectory(global_designDir);
#endif

   try 
   {
      WriteDebugString("Deleting level and global art\n"); 
      for (i=0;i<MAX_LEVELS;i++)
      {
        deleteAllLevelArt(i);
        deleteAllLevelSound(i);
        if (!globalData.levelInfo.stats[i].used)
          deleteLevel(i);
      }

      deleteAllLevelArt(GLOBAL_ART);
      deleteAllLevelSound(GLOBAL_SOUND);
      
     if (success)
     {
       EditorStatusMsg("Writing art files...");

       WriteDebugString("SaveArt(globalData)\n");
       saveArt(globalData);
       WriteDebugString("SaveArt(itemData)\n");
       saveArt(itemData);
       WriteDebugString("SaveArt(monsterData)\n");
       saveArt(monsterData);
       WriteDebugString("SaveArt(spellData)\n");
       saveArt(spellData);
       // need to guarantee default monster icon is in design folder
       CString MI_Temp;
#ifdef SIMPLE_STRUCTURE
       MI_Temp = ede.TemplateIconArtDir() + DEFAULT_MONSTER_ICON;
#else
       MI_Temp.Format("%s%s", EditorArt,DEFAULT_MONSTER_ICON);
#endif
       ::SaveArt(MI_Temp, IconDib, GLOBAL_ART, TRUE, rte.IconArtDir());

       levelData.GetSlots();

       if (   (globalData.currLevel >= 0)
           && (globalData.levelInfo.stats[globalData.currLevel].used))
       {
         levelData.SaveArt(globalData.currLevel);
       }

       WriteRequiredArtList();

       EditorStatusMsg("Writing sound files...");
       
       saveSounds(globalData);
       saveSounds(itemData);
       saveSounds(monsterData);
       saveSounds(spellData);

       WriteRequiredSoundList();
     }
     WriteDebugString(" Start Serialize globalData\n");
     if (success)
       globalData.Serialize(ar);   
     WriteDebugString(" End Serialize globalData - Start Serialize current level\n");
     // save current level data
     if (   (success)
         && (globalData.currLevel >= 0)
         && (globalData.levelInfo.stats[globalData.currLevel].used))
     {
       success = saveLevel(globalData.currLevel);
     }
     WriteDebugString(" Start ConfirmUsedLevels\n");
     if (success)
       globalData.ConfirmUsedLevels();

     WriteDebugString(" Start saveData(itemData)\n");
     if (success)
      success = saveData(itemData);

     WriteDebugString(" Start saveData(monsterData)\n");
     if (success)
       success = saveData(monsterData);

     WriteDebugString(" Start saveData(spellData)\n");
     if (success)
        success = saveData(spellData);

     WriteDebugString(" Start saveData(spellgroupData)\n");
     if (success)
        success = saveData(spellgroupData);

     WriteDebugString(" Start saveData(raceData)\n");
     if (success)
       success = saveData(raceData);

#ifdef USE_TRAITS
     WriteDebugString(" Start saveData(traitData)\n");
     if (success)
       success = saveData(traitData);
#endif


     WriteDebugString(" Start saveData(abilityData)]\n");
     if (success)
       success = saveData(abilityData);

     WriteDebugString(" Start saveData(specialAbilityData)\n");
     if (success)
       success = saveData(specialAbilitiesData);

     WriteDebugString(" Start saveData(baseclassData)\n");
     if (success)
       success = saveData(baseclassData);

     WriteDebugString(" Start saveData(classData)\n");
     if (success)
       success = saveData(classData);

     // make sure GameExec exists
     BOOL PreSaveExecStatus = success;
     CString temp;
     if (success)
     {
#ifdef SIMPLE_STRUCTURE
       temp = rte.ExecutableDir();
#else
       success = ConfigFile.FindToken("Game_Exec", temp, false);
       
       if (success)
       {
         if (!FileExists(temp))
         {
            success = FALSE;
            temp += " - game exec file does not exist\n";
            MsgBoxError(temp, "Save Design Error");
         }
       }
       else
         MsgBoxError("Game_Exec token not found in config.txt", "Save Design Error");
#endif
     }

     CString saveExec;
     CString file;
#ifdef SIMPLE_STRUCTURE
#else
     char path[_MAX_PATH+1];
     if (success)
     {
        EditorStatusMsg("Saving game shortcut...");
        int index = temp.ReverseFind('\\');

        if (index > 2)
          saveExec = temp.Right(temp.GetLength()-(index+1));
        else
          saveExec = temp;
        
        GetDesignPath(path);
        file.Format("%sStart.lnk", path);
        char *pCh;
        if ((pCh = strrchr(path, '\\')) != NULL)
          *pCh = '\0';
        CShellLinkInfo sli1;
        sli1.m_sTarget = _T(temp);
        file = global_designDir;
        file = "\"";
        file += path;
        file += "\"";
        sli1.m_sArguments = _T(file);
        strcpy(path,global_designDir);
        sli1.m_sWorkingDirectory = _T(path);
        sli1.m_sDescription = _T("Play Design");
        sli1.m_sIconLocation = _T(temp);
        sli1.m_nIconIndex = 0;
        sli1.m_nShowCmd = SW_MAXIMIZE;

        CShellLink sl1;
        if (sl1.Create(sli1))
        {
          strcat(path, "\\");
          file.Format("%sStart.lnk", path);          
          SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
          MyDeleteFile(file);
          if (!sl1.Save(_T(file)))
            success = FALSE;
        }
        else
          success = FALSE;

        if (!success)
        {
           CString error;
           error.Format("Could not create game exec shortcut for %s", temp);
           MsgBoxError(error, "Save Design Error");
        }
     }
#endif
     if (success)
     {
       saveExec = ConfigFile.GetFilename();

       if (!FileExists(saveExec))
       {
         temp.Format("Could not locate config.txt file to add to design");
         MsgBoxError(temp, "Save Design Error");
       }
       else
       {
#ifdef SIMPLE_STRUCTURE
#else
         GetDesignPath(path);
         file.Format("%s%s", path, "config.txt");

         // Only update config.txt if editor version is more
         // recent than the design version. This will ensure that
         // that the design gets the latest config.txt file after 
         // the user upgrades DC.

          BOOL copy = ((!FileExists(file)) || (PreSaveDesignVer != PRODUCT_VER));

          if (copy)
          {
            SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);

            success = MyCopyFile(saveExec, file, FALSE);

            SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);

            if (!success)
            {
               temp.Format("Could not copy config file %s to %s", saveExec, file);
               MsgBoxError(temp, "Save Design Error");
            }
          }
#endif
       }
     }

     // ignore config file copy errors after letting the user know about it
     success = PreSaveExecStatus;
   }
   catch (...) 
   {
      WriteDebugString("Unable to save design %s\n", ar.GetFile()->GetFileName());
      success = FALSE;
   }
      
   WriteDebugString(" Start Validate Event Data)\n");
   if (success)
   {
     // The validation code checks that sound, etc files exist.
     // It often checks the same file hundreds of times.
     // We will provide a way of maintaining a list.
     // The function 'FileExists' will use this list if we put it there.
     A_CStringInteger_L fileValidationList;
     fileExistsList = &fileValidationList;
     EditorStatusMsg("Validating event data...");
     if (!levelData.eventData.Validate())
       EditorStatusMsg("Event validation failed");

     EditorStatusMsg("Design data saved.");
     fileExistsList = NULL;
   }
   else
      EditorStatusMsg("Unable to save design data.");

   FirstTimeFileAction=FALSE;
   WriteDebugString("Exiting savedesign(CArchive&)\n");
   return success;
}
*/





BOOL saveDesign(CAR& car)
{
   BOOL success = TRUE;
   int i;
   
   EditorStatusMsg("Saving design data...");
#ifdef SIMPLE_STRUCTURE
#else
   SetCurrentDirectory(global_designDir);
#endif

   try 
   {
      WriteDebugString("Deleting level and global art\n"); 
      for (i=0;i<MAX_LEVELS;i++)
      {
        deleteAllLevelArt(i);
        deleteAllLevelSound(i);
        if (!globalData.levelInfo.stats[i].used)
          deleteLevel(i);
      }

      deleteAllLevelArt(GLOBAL_ART);
      deleteAllLevelSound(GLOBAL_SOUND);
      
     if (success)
     {
       EditorStatusMsg("Writing art files...");

       WriteDebugString("SaveArt(globalData)\n");
       saveArt(globalData);
       WriteDebugString("SaveArt(itemData)\n");
       saveArt(itemData);
       WriteDebugString("SaveArt(monsterData)\n");
       saveArt(monsterData);
       WriteDebugString("SaveArt(spellData)\n");
       saveArt(spellData);
       // need to guarantee default monster icon is in design folder
       CString MI_Temp;
#ifdef SIMPLE_STRUCTURE
       MI_Temp = ede.TemplateIconArtDir() + DEFAULT_MONSTER_ICON;
#else
       MI_Temp.Format("%s%s", EditorArt,DEFAULT_MONSTER_ICON);
#endif
       ::SaveArt(MI_Temp, IconDib, GLOBAL_ART, TRUE, rte.IconArtDir());

       levelData.GetSlots();

       if (   (globalData.currLevel >= 0)
           && (globalData.levelInfo.stats[globalData.currLevel].used))
       {
         levelData.SaveArt(globalData.currLevel);
       }

       WriteRequiredArtList();

       EditorStatusMsg("Writing sound files...");
       
       saveSounds(globalData);
       saveSounds(itemData);
       saveSounds(monsterData);
       saveSounds(spellData);

       WriteRequiredSoundList();
     }
     WriteDebugString(" Start Serialize globalData\n");
     if (success)
       globalData.Serialize(car);   
     WriteDebugString(" End Serialize globalData - Start Serialize current level\n");
     // save current level data
     if (   (success)
         && (globalData.currLevel >= 0)
         && (globalData.levelInfo.stats[globalData.currLevel].used))
     {
       success = saveLevel(globalData.currLevel);
     }
     WriteDebugString(" Start ConfirmUsedLevels\n");
     if (success)
       globalData.ConfirmUsedLevels();

     WriteDebugString(" Start saveData(itemData)\n");
     if (success)
      success = saveData(itemData);

     WriteDebugString(" Start saveData(monsterData)\n");
     if (success)
       success = saveData(monsterData);

     WriteDebugString(" Start saveData(spellData)\n");
     if (success)
        success = saveData(spellData);

     WriteDebugString(" Start saveData(spellgroupData)\n");
     if (success)
        success = saveData(spellgroupData);

     WriteDebugString(" Start saveData(raceData)\n");
     if (success)
       success = saveData(raceData);

#ifdef USE_TRAITS
     WriteDebugString(" Start saveData(traitData)\n");
     if (success)
       success = saveData(traitData);
#endif


     WriteDebugString(" Start saveData(abilityData)]\n");
     if (success)
       success = saveData(abilityData);

     WriteDebugString(" Start saveData(specialAbilityData)\n");
     if (success)
       success = saveData(specialAbilitiesData);

     WriteDebugString(" Start saveData(baseclassData)\n");
     if (success)
       success = saveData(baseclassData);

     WriteDebugString(" Start saveData(classData)\n");
     if (success)
       success = saveData(classData);

     // make sure GameExec exists
     BOOL PreSaveExecStatus = success;
     CString temp;
     if (success)
     {
#ifdef SIMPLE_STRUCTURE
       temp = rte.ExecutableDir();
#else
       success = ConfigFile.FindToken("Game_Exec", temp, false);
       
       if (success)
       {
         if (!FileExists(temp))
         {
            success = FALSE;
            temp += " - game exec file does not exist\n";
            MsgBoxError(temp, "Save Design Error");
         }
       }
       else
         MsgBoxError("Game_Exec token not found in config.txt", "Save Design Error");
#endif
     }

     CString saveExec;
     CString file;
#ifdef SIMPLE_STRUCTURE
#else
     char path[_MAX_PATH+1];
     if (success)
     {
        EditorStatusMsg("Saving game shortcut...");
        int index = temp.ReverseFind('\\');

        if (index > 2)
          saveExec = temp.Right(temp.GetLength()-(index+1));
        else
          saveExec = temp;
        
        GetDesignPath(path);
        file.Format("%sStart.lnk", path);
        char *pCh;
        if ((pCh = strrchr(path, '\\')) != NULL)
          *pCh = '\0';
        CShellLinkInfo sli1;
        sli1.m_sTarget = _T(temp);
        file = global_designDir;
        file = "\"";
        file += path;
        file += "\"";
        sli1.m_sArguments = _T(file);
        strcpy(path,global_designDir);
        sli1.m_sWorkingDirectory = _T(path);
        sli1.m_sDescription = _T("Play Design");
        sli1.m_sIconLocation = _T(temp);
        sli1.m_nIconIndex = 0;
        sli1.m_nShowCmd = SW_MAXIMIZE;

        CShellLink sl1;
        if (sl1.Create(sli1))
        {
          strcat(path, "\\");
          file.Format("%sStart.lnk", path);          
          SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
          MyDeleteFile(file);
          if (!sl1.Save(_T(file)))
            success = FALSE;
        }
        else
          success = FALSE;

        if (!success)
        {
           CString error;
           error.Format("Could not create game exec shortcut for %s", temp);
           MsgBoxError(error, "Save Design Error");
        }
     }
#endif
     if (success)
     {
       saveExec = ConfigFile.GetFilename();

       if (!FileExists(saveExec))
       {
         temp.Format("Could not locate config.txt file to add to design");
         MsgBoxError(temp, "Save Design Error");
       }
       else
       {
#ifdef SIMPLE_STRUCTURE
#else
         GetDesignPath(path);
         file.Format("%s%s", path, "config.txt");

         // Only update config.txt if editor version is more
         // recent than the design version. This will ensure that
         // that the design gets the latest config.txt file after 
         // the user upgrades DC.

          BOOL copy = ((!FileExists(file)) || (PreSaveDesignVer != PRODUCT_VER));

          if (copy)
          {
            SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);

            success = MyCopyFile(saveExec, file, FALSE);

            SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);

            if (!success)
            {
               temp.Format("Could not copy config file %s to %s", saveExec, file);
               MsgBoxError(temp, "Save Design Error");
            }
          }
#endif
       }
     }

     // ignore config file copy errors after letting the user know about it
     success = PreSaveExecStatus;
   }
   catch (...) 
   {
      WriteDebugString("Unable to save design %s\n", car.GetFile()->GetFileName());
      success = FALSE;
   }
      
   WriteDebugString(" Start Validate Event Data)\n");
   if (success)
   {
     // The validation code checks that sound, etc files exist.
     // It often checks the same file hundreds of times.
     // We will provide a way of maintaining a list.
     // The function 'FileExists' will use this list if we put it there.
     A_CStringInteger_L fileValidationList;
     fileExistsList = &fileValidationList;
     EditorStatusMsg("Validating event data...");
     if (!levelData.eventData.Validate())
       EditorStatusMsg("Event validation failed");

     EditorStatusMsg("Design data saved.");
     fileExistsList = NULL;
   }
   else
      EditorStatusMsg("Unable to save design data.");

   FirstTimeFileAction=FALSE;
   WriteDebugString("Exiting savedesign(CArchive&)\n");
   return success;
}









BOOL saveDesign()
{
  HANDLE EngineEvent = CreateEvent(NULL, FALSE, FALSE, ENGINE_APP_EVENT);
  if (ERROR_ALREADY_EXISTS == GetLastError())
  {    
    CloseHandle(EngineEvent);

    // engine is running, but check to see if it is using
    // the same design files were are about to overwrite
    CString dest;      
    dest = rte.DataDir() + "game.dat";

    if (IsFileAlreadyOpen(dest))
    {
      MsgBoxError("You cannot save the design while the engine is using the same files");
      FirstTimeFileAction=FALSE;
      return FALSE;
    }
  }
  else
    CloseHandle(EngineEvent);





  if (ede.TemplateDir() == rte.DesignDir())
  {
    /* Make it difficult to edit the template.
     * Post a warning and
     * Force the user to enter a key.
     */
    CImportWarning dlg;
    int answer;
    dlg.m_warning = "You are asking to save the 'TemplateDesign'.  This is the\r\n"
                    "design that we copy to your design when you create a new design.\r\n"
                    "It was intended that the 'TemplateDesign'be treated as part of\r\n"
                    "the Editor and only changed when a new version of the Editor\r\n"
                    "is released.  In other words, we expected the TemplateDesign\r\n"
                    "to be treated as a read-only resource and not changed by the user\r\n"
                    "of this editor.  If you wish to proceed, you may.  But be careful.";
    if (dlg.DoModal() != IDOK)
    {
	    MsgBoxInfo("That is a wise choice");
	    return FALSE;
    };
    if ( (sscanf(dlg.m_numeric,"%d",&answer) != 1) || (answer!=dlg.m_i))
    {
        MsgBoxInfo("Wrong answer....sorry");
        return FALSE;
    };
    //return true;
  };




   if (EditorOptionsData.m_LastDesign.CompareNoCase(GetDesignName())!=0)
     EditorOptionsData.m_LastSaveSuccess=FALSE; // not the same design

   BOOL success = FALSE;
   
   CFile myFile;

     CString fullPath;

   try
   {
     if (!CreateDesignDirectory())
     {
       WriteDebugString("Failed to create design directory\n");
       EditorOptionsData.m_LastSaveSuccess=FALSE;
       FirstTimeFileAction=FALSE;
       return FALSE;
     }

     PreSaveDesignVer = globalData.version;

     if (globalData.version != PRODUCT_VER)
     {       
       WriteDebugString("Changing design ver from %4.7f to %4.7f\n",
                        globalData.version, PRODUCT_VER);
       globalData.version = PRODUCT_VER;                 
     }

     // backup the current files if we know that
     // they have been validated by loading/saving
     if ((FirstTimeFileAction)||(EditorOptionsData.m_LastSaveSuccess))
     {
       WriteDebugString("Calling 'BackupDesignFiles'");
       BackupDesignFiles();
     };

      CString src;
#ifdef USE_BASS      
      CString dest;
      WriteDebugString("Copy bass.dll\n");
      dest = rte.DataDir() + "bass.dll";
      src = ede.TemplateDataDir() + "bass.dll";
      SetFileAttributes(dest, FILE_ATTRIBUTE_NORMAL);

      // if engine is running, bass.dll will be in use and this
      // function will fail.
      //
      // try this operation before opening game.dat so we don't
      // lose any design data.
      if (!CopyFileIfDifferent(src, dest, FALSE))
      {
        WriteDebugString("Error %u: Failed to copy BASS.DLL to design folder\n", GetLastError());

        MsgBoxError("You cannot save the design while the engine is using the same files");        
        FirstTimeFileAction=FALSE;
        return FALSE;
      }
#endif
      WriteDebugString("Copy Readme.txt\n");
      {
        CString dst;
        dst = rte.DesignDir() + "ReadMe.txt";
        MyDeleteFile(dst);
        src = ede.TemplateDir() + "ReadMe.txt";
        MyCopyFile(src, dst, FALSE);
      };

     fullPath = rte.DataDir() + "game.dat";

     CFileException e;
     SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
     if (!myFile.Open(fullPath, (CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive), &e))
     {
        WriteDebugString("Unable to open design file %s, error %i\n", fullPath, e.m_cause);
        EditorOptionsData.m_LastSaveSuccess=FALSE;
        FirstTimeFileAction=FALSE;
        return FALSE;
     }   
  
     //CArchive ar(&myFile, CArchive::store);
     //success = saveDesign(ar);


     // put signal value and version at start of file
     __int64 hdr=0xFABCDEFABCDEFABF;
     double ver = PRODUCT_VER;
     myFile.Write(&hdr, sizeof(hdr));
     myFile.Write(&ver, sizeof(double));

     CAR car(&myFile, CArchive::store);
     car.Compress(true);


     success = saveDesign(car);
     //ar.Close();
     car.Close();
   }
   catch(...)
   {
     WriteDebugString("Unable to save design %s\n", fullPath);
     success = FALSE;
   }

   EditorOptionsData.m_LastSaveSuccess=success;
   FirstTimeFileAction=FALSE;
   return success;
}

BOOL CheckSaveChangedDesign(char *msgIfFailure)
{
  if (!globalData.designName.IsEmpty() && IS_MODIFIED)
  {
    //if (globalData.designName.CompareNoCase("Default Design")!=0)
    //{
    /*  Removed 20130515.  This was temporary until editor was stable.
    MsgBoxError("The changes are lost until the editor has been better tested","Changes Lost");
    return TRUE;
    */
      CString msg;
      msg = "You have made changes to this design.\n"
            "Do you want to save it?";
      int response = MsgBoxYesNo(msg, "Design Warning");
      if (response == IDYES)
      {
        PumpWinMsgs();
        CWaitCursor cursor;
        if (!saveDesign())
        {
          msg="Failed to save design.  ";
          msg += msgIfFailure;
          return MsgBoxYesNo(msg, "Save Error") == IDYES;
        }
        else
          CLEAR_MODIFIED;
      }
    //}
  }
  return TRUE;
}


void ForceReloadOfConfigFiles()
{
  CUAFWinEdApp *pApp = reinterpret_cast<CUAFWinEdApp*>(AfxGetApp());
  pApp->LoadDefaultDBFiles();     	
}
#endif // UAFEDITOR


BOOL loadDesign(CArchive *ar, CAR *car)
{
   BOOL success = TRUE;
   int dataCount;

   CString dataPath;
   if (ar == NULL)
   {
      StripOneFolderFromFilenamePath(car->GetFile()->GetFilePath(), dataPath);
   }
   else
   {
      StripOneFolderFromFilenamePath(ar->GetFile()->GetFilePath(), dataPath);
   };

   EditorStatusMsg("Loading design data...");

   //clearDesign(FALSE);

   global_SmallPicImport.RemoveAll();
   global_IconPicImport.RemoveAll();

   WriteDebugString("Finished clearing old design\n");
   try 
   {
      char fullPath[_MAX_PATH+1];    

      strcpy(fullPath, dataPath);
      strcat(fullPath, SPECAB_DB_NAME);

      if (loadData(specialAbilitiesData, fullPath) <= 0)
          success = FALSE;
      WriteDebugString("Finished loading SpecialAbilitiesDB\n");

      if (success)
      {



#ifdef SIMPLE_STRUCTURE
        strcpy(fullPath, dataPath);
#else
        GetDesignPath(fullPath);
#endif
        strcat(fullPath, ITEM_DB_NAME);

        if (loadData(itemData, fullPath) <= 0)
          success = FALSE;
        WriteDebugString("Finished loading ItemDB\n");
      };

      if (success)
      {
#ifdef SIMPLE_STRUCTURE
        strcpy(fullPath, dataPath);
#else
        GetDesignPath(fullPath);
#endif
        strcat(fullPath, MONSTER_DB_NAME);

        if (loadData(monsterData, fullPath) <= 0)
           success = FALSE;
        WriteDebugString("Finished loading MonsterDB\n");
     }

     if (success)
     {
#ifdef SIMPLE_STRUCTURE
        strcpy(fullPath, dataPath);
#else
        GetDesignPath(fullPath);
#endif
        strcat(fullPath, SPELL_DB_NAME);

        if (loadData(spellData, fullPath) <= 0)
           success = FALSE;
        WriteDebugString("Finished loading SpellDB\n");
     }

     if (success)
     {
#ifdef SIMPLE_STRUCTURE
        strcpy(fullPath, dataPath);
#else
        GetDesignPath(fullPath);
#endif
        strcat(fullPath, RACE_DB_NAME);

        dataCount=loadData(raceData, fullPath);
        if (dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using defaults\n",fullPath);
          loadUADefaults(raceData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n", dataCount, fullPath);
        };
     }

     if (success)
     {
        strcpy(fullPath, dataPath);
        strcat(fullPath, BASE_CLASS_DB_NAME);

        dataCount=loadData(baseclassData, fullPath);
        if ( dataCount <= 0)          
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
#ifdef UAFEDITOR
          loadUADefaults(baseclassData);
#else
          die("Unable to Load Baseclass Database");
#endif
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }



     if (success)
     {
        strcpy(fullPath, dataPath);
        strcat(fullPath, CLASS_DB_NAME);

        dataCount=loadData(classData, fullPath);
        if ( dataCount<= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
#ifdef UAFEDITOR
          loadUADefaults(classData);
#else
          die("Cannot Load Class Database");
#endif
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n", dataCount, fullPath);
        };
     }

     if (success)
     {
#ifdef SIMPLE_STRUCTURE
        strcpy(fullPath, dataPath);
#else
        GetDesignPath(fullPath);
#endif
        strcat(fullPath, ABILITY_DB_NAME);

        dataCount=loadData(abilityData, fullPath);
        if ( dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(abilityData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }
/*  No spellgroup database, please.  PRS 20130305
     if (success)
     {
#ifdef SIMPLE_STRUCTURE
        strcpy(fullPath, dataPath);
#else
        GetDesignPath(fullPath);
#endif
        strcat(fullPath, SPELLGROUP_DB_NAME);

        dataCount=loadData(spellgroupData, fullPath);
        if ( dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(spellgroupData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }
*/
#ifdef USE_TRAITS
     if (success)
     {
#ifdef SIMPLE_STRUCTURE
        strcpy(fullPath, dataPath);
#else
        GetDesignPath(fullPath);
#endif
        strcat(fullPath, TRAIT_DB_NAME);

        dataCount=loadData(traitData, fullPath);
        if ( dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(traitData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }
#endif


#ifdef UAFEDITOR
     if ((success)&&(globalData.version < _VERSION_0882_))
     {
       // Recent changes to the config files require the latest
       // files to be reloaded, rather than relying on the older
       // data stored in the DAT files (just loaded prior to this).
       // 
       // globalData.version is initialized prior to this even though
       // globalData.serialize() is called after this.
       ForceReloadOfConfigFiles();
     }
#endif
     if (ar == NULL)
     {
       globalData.Serialize(*car);
     }
     else
     {
       globalData.Serialize(*ar);
     };
     WriteDebugString("Finished loading globalData\n");

     if (success)
     {      
        party.Posx = globalData.startX;
        party.Posy = globalData.startY;
        party.facing = globalData.startFacing;
     }
     else 
     {
        party.Posx = 0;
        party.Posy = 0;
        party.facing = FACE_EAST;
     }
   }
   catch (...)
   {
     success = FALSE;
   }
  
   if (success)
   {
      EditorStatusMsg("Design data loaded.");
      WriteDebugString("Design %s loaded, version %4.7f\n", 
                       globalData.designName, globalData.version);         
   }
   else
      EditorStatusMsg("Unable to load design data.");

#if (defined UAFEDITOR)
   EditorOptionsData.m_LastLoadSuccess=success;
   UndoData.RemoveAll();
   FirstTimeFileAction=FALSE;
#endif

   //if (success)
   //{
   //  for (int i=0;i<MAX_LEVELS;i++)
   //  {
   //    if (!globalData.levelInfo.stats[i].used)
   //      deleteLevel(i);
   //  };
   //}

   return success;
}

#ifdef UAFEDITOR
BOOL CheckLevelVersions(const char *name)
{
  int i;
  LEVEL level;
  double version;
  int messageCount;
  char flags[MAX_LEVELS+1];
  int missingFiles, missingIncrementedFiles, extraFiles, extraIncrementedFiles;
  memset(flags,0,sizeof(flags));
  for (i=0; i<MAX_LEVELS+1; i++)
  {
    CString fname;
    fname.Format("%sData\\Level%03d.lvl", name, i);
    if (FileExists(fname))
    {
      flags[i] = 1;
    };
  };
  missingFiles = 0;
  missingIncrementedFiles = 0;
  extraFiles = 0;
  extraIncrementedFiles = 0;
  for (i=0; i<MAX_LEVELS; i++)
  {
    if ((flags[i+1] != 0) && !globalData.levelInfo.stats[i].used)
    {
      extraFiles++;
    };
    if ((flags[i+1] == 0) && globalData.levelInfo.stats[i].used)
    {
      missingFiles++;
    };
    if ((flags[i] != 0) && !globalData.levelInfo.stats[i].used)
    {
      extraIncrementedFiles++;
    };
    if ((flags[i] == 0) && globalData.levelInfo.stats[i].used)
    {
      missingIncrementedFiles++;
    };
  };
  if ((extraFiles > 0) || (missingFiles > 0))
  {
    if ( (extraIncrementedFiles==0) && (missingIncrementedFiles==0))
    {
      int ans;
      // Perhaps we can rename the files for the user.
      ans = MessageBox(NULL, "Older designs named the level files starting at 000 but new "
                 "designs start at 001.  It appears that if we rename all your "
                 "level files by adding one to each file name, we would make the "
                 "files agree with the new naming scheme.\n\nShould we do this?",
                 "Question",
                 MB_OKCANCEL);
      if (ans == IDOK)
      {
        for (i=MAX_LEVELS-1; i>=0; i--)
        {
          if (flags[i] > 0)
          {
            CString oldName, newName;
            oldName.Format("%sData\\Level%03d.lvl", name, i);
            newName.Format("%sData\\Level%03d.lvl", name, i+1);
            rename(oldName, newName);
          };
        };
      };
    };
  };
  messageCount = 0;
  for (i=0; i<MAX_LEVELS; i++)
  {
    if (!globalData.levelInfo.stats[i].used) continue;
    {
      CString fname, backupName;
      fname.Format("%sData\\Level%03d.lvl", name, i+1);
      if (FileExists(fname))
      {
        FILE *f;
        f = fopen(fname, "rb");
        if (f == NULL)
        {
          CString msg;
          msg.Format("Cannot open level file to check version.\nName=%s", fname);
          MsgBoxInfo(msg);
          continue;
        };
        version = level.GetVersion(f);
        fclose(f);
        if (version < 0)
        {
          CString msg;
          msg.Format("Error attempting to determine version of %s", fname);
          return FALSE;
        };
        if (version >= VersionSpellNames)
        {
          continue;
        };
        {
          CTime ctime = CTime::GetCurrentTime();
          CString backupTime;
          CString msg;
          backupTime = ctime.Format("%Y%m%d%H%M%S");
          backupName.Format("%sData\\Level%03d.%s", name, i+1, backupTime);
          if ( messageCount == 0)
          {
            msg.Format("The level definition file:\n'%s'\n must be converted to the "
                       "latest format.  We will rename the existing file to:\n"
                       "%s\n as a backup and create a new, updated level file.\n"
                       "Is that OK with you?", fname, backupName);
          }
          else if (messageCount == 1)
          {
            msg.Format("Additional levels need to be updated in the same way.\n"
                       "Is that OK with you?");
          }
          else
          {
            msg.Empty();
          };
          messageCount++;
          if (!msg.IsEmpty())
          {
            if (MessageBox(NULL, msg, "Information", MB_OKCANCEL) != IDOK)
            {
              return FALSE;
            };
          };
          {
            if (!LoadLevel(level, i))
            {
              MsgBoxInfo("Error loading the level");
              return FALSE;
            };
          };
          if (rename(fname, backupName) != 0)
          {
            MsgBoxInfo("Unable to rename that file to back it up");
            return FALSE;
          };
          saveLevel(level, i);
        };
      }
      else
      {
        CString msg;
        msg.Format("Cannot open file '%s'", fname);
        MsgBoxInfo(msg, "Level File Error");
      };
    };
  };
  return TRUE;
}
#endif

BOOL loadDesign(LPCSTR name)
{
   BOOL success = TRUE;
   CFile myFile;
   char fullPath[_MAX_PATH+1];
   memset(fullPath, 0, sizeof(fullPath));

   clearDesign(FALSE);
  
   if (name != NULL)
   {
     if (strlen(name) == 0)
     {
        WriteDebugString("Empty design name in loadDesign()\n");
        EditorStatusMsg("Unable to load design data."); 
        success = FALSE;
     }
     else
     {
       strcpy(fullPath, name);
       strcat(fullPath, "Data\\game.dat");
     }
   }
   else
   {
     strcpy(fullPath, rte.DataDir());
     strcat(fullPath, "game.dat");
   }

   double DesignVersion = GetDesignVersion(fullPath);
#ifdef UAFEDITOR
   if (   (DesignVersion >= VersionSpellNames)
        &&(DesignVersion <= 0.9988))
   {
     MsgBoxError("This editor will reliably load designs created with editor version less than 0.998101 or greater than 0.9988", "Warning",MB_OK);
   };
#endif
   BOOL VersionError = FALSE;

   if ((success) && (DesignVersion > PRODUCT_VER))
   {
     // version in design file is greater than the
     // version of this build
     VersionError = TRUE;
     WriteDebugString("Version Check: design name %s, curr ver %4.7f, design ver %4.7f\n",
                      fullPath, PRODUCT_VER, DesignVersion);
   }
   else if (DesignVersion < 0.5)
   {
     // something is wrong with game.dat file
     MsgBoxError("The Game.dat file appears to be missing or invalid - Load failed", "Load Error");
     success = FALSE;
   }
#ifdef UAFEngine
   else if (DesignVersion < VersionSpellNames)
   {
     CString msg;
     msg.Format("This game was created with an old editor.  You must load it with editor version %4.7f or later and save it again.", VersionSpellNames);
     MsgBoxError(msg, "Load Error");
     success = FALSE;
   }
#endif
   else
     WriteDebugString("Loading design %s, version %4.10f...\n", fullPath, DesignVersion);

   try
   {
     if (success)
     {
       globalData.version = DesignVersion;
       if (!myFile.Open(fullPath, CFile::modeRead))
       {
          WriteDebugString("Unable to open design file %s\n", fullPath);
          CString errorMsg;
          errorMsg.Format("Unable to open design file %s, load failed", fullPath);                       
          MsgBoxError(errorMsg, "Design Load Failed");
          success = FALSE;
       }   
       else
       {  
         if (DesignVersion < VersionSpellNames)
         {
           CArchive ar(&myFile, CArchive::load);  
           success = loadDesign(&ar, NULL);
           ar.Close();
         }
         else
         {
           CAR car(&myFile, CArchive::load);
           success = loadDesign(NULL, &car);
           car.Close();
         };
       }
     }
   }
   catch (...)
   {
      WriteDebugString("Unable to load design %s\n", fullPath);
      success = FALSE;
   }

   if ((!success) && (VersionError))
   {
#ifdef UAFEDITOR
     EditorStatusMsg("Design version incompatible");
     CString errorMsg;
     errorMsg.Format("You are trying to load a design saved with UAF version %4.7f,\n"
                     "which is not compatible with your UAF version %4.7f.\n"
                     "You must upgrade to version %4.7f or higher to load this design",
                     DesignVersion, PRODUCT_VER, DesignVersion);
     MsgBoxError(errorMsg, "Design Version Incompatible");
#else
     miscError = DesignVersionError;
#endif
   }

#ifdef UAFEDITOR
   if (success)
   {
     success = CheckLevelVersions(name);
   }
#endif
#ifdef UAFEDITOR
   EditorOptionsData.m_LastLoadSuccess=success;
   FirstTimeFileAction=FALSE;
#endif        
   return success;
}

void clearDesign(BOOL FillDefaults)
{
   int i;  
   globalData.Clear();
   levelData.Clear();

   for (i=0;i<MAX_LEVELS;i++)
   {
      deleteAllLevelArt(i);
      deleteAllLevelSound(i);
   }

   deleteAllLevelArt(GLOBAL_ART);
   deleteAllLevelSound(GLOBAL_SOUND);

   clearArtSlots();

   global_SmallPicImport.RemoveAll();
   global_IconPicImport.RemoveAll();

#if (defined UAFEDITOR)

   CopyEventData.Clear();
   UndoData.RemoveAll();

   HBarVPArt.name             = /*ede.EditorWindowArtDir() + */DEFAULT_HB;
   VBarVPArt.name             = /*ede.EditorWindowArtDir() + */DEFAULT_VB;
   FrameVPArt.name            = /*ede.EditorWindowArtDir() + */DEFAULT_FR;
   VBarCombArt.name           = /*ede.EditorWindowArtDir() + */DEFAULT_VB;
   HBarCombArt.name           = /*ede.EditorWindowArtDir() + */DEFAULT_HB;
   CombatWallArt.name         = /*ede.DesignCombatArtDir() + */DEFAULT_CWD;
   CombatCursorArt.name       = /*ede.DesignCursorArtDir() + */DEFAULT_CC;
   CombatDeathIconArt.name    = /*ede.DesignCombatArtDir() + */DEFAULT_CDI;
   CombatPetrifiedIconArt.name= /*ede.DesignCombatArtDir() + */DEFAULT_CPI;
   CombatDeathArt.name        = /*ede.DesignCombatArtDir() + */DEFAULT_CD;
   CursorArt.filename         = /*ede.DesignCursorArtDir() + */DEFAULT_CURSOR;
   CursorArt.FrameHeight = 32;
   CursorArt.FrameWidth = 32;
   CursorArt.NumFrames = 15;
   CursorArt.picType = TransBufferDib;
   CursorArt.style = PIC_DATA::AS_None;
   CursorArt.timeDelay = 0;
   
   // load up the default icons/sprites that
   // are in the editor/DefaultArt folder.
   //
   // Loads all icons named Icon1.pcx..Icon50.pcx
   // Loads all icons named SPic1.pcx..SPic50.pcx

   i=1;
   CString temp;
   CString path;
   PIC_DATA pic;
   pic.picType = SmallPicDib;
   pic.SetDefaults();
#ifdef SIMPLE_STRUCTURE
   {
     long result, handle;
     _finddata_t fd;
      for (handle=result=_findfirst(ede.TemplateSmallPicDir() + "*.png", &fd); result!=-1; result=_findnext(handle,&fd))
      {
        if (memcmp(fd.name,"prt_",4) == 0)
        {
          pic.filename = fd.name;
          global_SmallPicImport.AddTail(pic);
        };
        if (memcmp(fd.name,"cn_",3) == 0)
        {
          pic.filename = fd.name;
          global_IconPicImport.AddTail(pic);
        };
      };
      if (handle!=-1) _findclose(handle);
   };
#else
   while (i <= 50)
   {
     temp.Format("prt_SPic%u.png", i);
     // uses root name to search for any matches
     // using valid image extensions (bmp, pcx, etc)
     if (FindImageWithValidExt(temp, ede.DesignSmallPicDir()))
     {
       pic.filename = temp;
       global_SmallPicImport.AddTail(pic);
     }
     i++;
   }

   i=1;
   pic.Clear();
   pic.picType = IconDib;
   pic.SetDefaults();
   while (i <= 50)
   {
     temp.Format("cn_Icon%u.png", i);
     if (FindImageWithValidExt(temp, ede.DesignIconArtDir()))
     {
       pic.filename = temp;
       global_IconPicImport.AddTail(pic);
     }
     i++;
   }
#endif
   currX = globalData.startX;
   currY = globalData.startY;
   if ((currX < 0) || (currX >= globalData.levelInfo.stats[globalData.startLevel].area_width))
   {
     currX  = max(0, currX);
     currX  = min(globalData.levelInfo.stats[globalData.startLevel].area_width-1, currX);
   }
   if ((currY < 0) || (currY >= globalData.levelInfo.stats[globalData.startLevel].area_height))
   {
     currY  = max(0, currY);
     currY  = min(globalData.levelInfo.stats[globalData.startLevel].area_height-1, currY);
   }
   currFacing = globalData.startFacing;
   party.facing = currFacing;
   party.Posx = currX;
   party.Posy = currY;

   globalData.levelInfo.stats[globalData.startLevel].SetDefaults();

   if (FillDefaults)
   {
     fillDefaultArtSlots(levelData,globalData.levelInfo.stats[globalData.startLevel].overland);
     levelData.SetSlots();
     if (loadWallSetSurfaces())
     {
       currBlockage = BlockedBlk;
       currWallSlot = FindFirstLoadedWall();
       currBkGnd = FindFirstLoadedBackground();
     }

     fillDefaultLevelData(levelData,
                          globalData.startLevel, 
                          globalData.levelInfo.stats[globalData.startLevel].area_width, 
                          globalData.levelInfo.stats[globalData.startLevel].area_height, 
                          globalData.levelInfo.stats[globalData.startLevel].overland, 
                          TRUE);
     levelData.GetSlots();

   }

   m_mapType = (globalData.levelInfo.stats[globalData.startLevel].overland ? OVERLAND_TYPE : DUNGEON_TYPE);

   if (m_mapType == OVERLAND_TYPE)
   {
     if (m_mode == WALL_MODE)
       m_mode = BLOCKAGE_MODE;
   }
   else
   {
     if (m_mode == BLOCKAGE_MODE)
       m_mode = WALL_MODE;
   }
#endif

   /*
  for (i=0;i<MAX_LEVELS;i++)
  {
    if (!globalData.levelInfo.stats[i].used)
      deleteLevel(i);
  }
  */
#ifdef SIMPLE_STRUCTURE
#else
  m_designFolder[0]='\0';
#endif
#ifdef UAFEDITOR
  CLEAR_MODIFIED;
#endif
}


BOOL CheckLevelForWallSlot(int slot)
{
  if (!globalData.levelInfo.stats[slot].used)
    return FALSE;

  for (int i=0;i < globalData.levelInfo.stats[slot].area_height;i++)
  {
    for (int j=0;j < globalData.levelInfo.stats[slot].area_width;j++)
    {
       if (   (levelData.area[i][j].northWall == slot)
           || (levelData.area[i][j].southWall == slot)
           || (levelData.area[i][j].eastWall == slot)
           || (levelData.area[i][j].westWall == slot))
       {
          return TRUE;
       }
    }
  }
  return FALSE;
}

BOOL CheckLevelForBackgroundSlot(int slot)
{
  if (!globalData.levelInfo.stats[slot].used)
    return FALSE;

  for (int i=0;i < globalData.levelInfo.stats[slot].area_height;i++)
  {
    for (int j=0;j < globalData.levelInfo.stats[slot].area_width;j++)
    {
      if (levelData.area[i][j].northBG == slot)
        return TRUE;
      if (levelData.area[i][j].eastBG == slot)
        return TRUE;
      if (levelData.area[i][j].southBG == slot)
        return TRUE;
      if (levelData.area[i][j].westBG == slot)
        return TRUE;
    }
  }
  return FALSE;
}

int GetAreaWidth(LEVEL& data)
{
  return data.area_width;
}

int GetAreaHeight(LEVEL& data)
{
  return data.area_height;
}

int GetAreaBlockages(LEVEL& data, int x, int y, int dir)
{
  return data.area[y][x].blockages(dir);
}

BOOL deleteLevel(int LevelIndex)
{   
   char temp[4];
   sprintf(temp, "%.3i", LevelIndex+1);

#ifdef SIMPLE_STRUCTURE
   CString fullPath;
   fullPath = rte.DataDir() + "Level" + temp + ".lvl";
#else
   char fullPath[_MAX_PATH+1];
   GetDesignPath(fullPath);
   strcat(fullPath, "Level");
   strcat(fullPath, temp);
   strcat(fullPath, ".lvl");
#endif
   
   globalData.levelInfo.Clear(LevelIndex);
   if (LevelIndex == globalData.currLevel)
     levelData.Clear();

   // remove any art this level may have been using
   deleteAllLevelArt(LevelIndex);

   if (!FileExists(fullPath))
     return TRUE;

   // remove read-only flags
   SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);

   return remove(fullPath);
}

BOOL levelExists(int i)
{
  BOOL success = TRUE;
  char fullPath[_MAX_PATH+1];
  char temp[9];
  sprintf(temp, "%.3i", i+1);
#ifdef SIMPLE_STRUCTURE
  strcpy(fullPath, rte.DataDir());
#else
  GetDesignPath(fullPath);
#endif
  strcat(fullPath, "Level");
  strcat(fullPath, temp);
  strcat(fullPath, ".lvl");

  if (!FileExists(fullPath))
    success = FALSE;

  return success;
}


#ifdef UAFEDITOR
void LEVEL_STATS::CrossReference(CR_LIST *pCRList, int level)
{
  LEVEL lvlData;
  CR_REFERENCE crRef;
  crRef.m_referenceName.Format("Level%02d", level);
  crRef.m_referenceType = CR_TYPE_level;
  pCRList->CR_AddSoundReference(StepSound, &crRef);
  pCRList->CR_AddSoundReference(BumpSound, &crRef);
  bgSounds.CrossReference(pCRList, &crRef);
  LoadLevel(lvlData, level);
  {
    CR_EVENT_INFO crEI;
    crEI.m_pCRList = pCRList;
    crEI.m_level = level;
    crEI.m_CRReference = crRef;
    crEI.m_pEventList = &lvlData.eventData;
    lvlData.eventData.CrossReference(&crEI);
    lvlData.zoneData.CrossReference(pCRList, &crEI.m_CRReference);
	int i;
    for (i=0; i<MAX_STEP_EVENTS;i++)
    {
      lvlData.stepEvents[i].CrossReference(&crEI);
    };
    lvlData.blockageKeys.CrossReference(pCRList, &crEI.m_CRReference);
    for (i=0; i<MAX_BACKGROUNDS; i++)
    {
      lvlData.m_BackgroundSets[i].CrossReference(pCRList, &crEI.m_CRReference);
    };
    for (i=0; i<MAX_WALLSETS; i++)
    {
      lvlData.m_WallSets[i].CrossReference(pCRList, &crEI.m_CRReference);
    };
  };
}
#endif

