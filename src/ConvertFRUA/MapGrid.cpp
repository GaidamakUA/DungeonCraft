#include "stdafx.h"
#include "Translate.h"


#define Str mapGrid.Fetch_String
#define Int mapGrid.Fetch_Int

CELL cells[100][100];
//CArray <CELL, CELL&> cells;

int WallNum(const CString& name, CString *wallNames)
{
  int i;
  for (i=0; i<16; i++)
  {
    if (name == wallNames[i]) return i;
  };
  if (name == "none") return 0;
  printf("Cannot determine wall graphic mapping for %s\n", name);
  return 0;
}

const char *WallBlockage(const CString& name)
{
  if (name == "Open") return "Open";
  if (name == "Blocked") return "Blocked";

  
  if (name == "Secret")               return "Open, Secret";
  if (name == "Locked")               return "Locked";
  if (name == "Locked Secret")        return "Locked, Secret";
  if (name == "Wizard Locked")        return "Locked, Wizard";
  if (name == "Wizard Locked Secret") return "Locked, Wizard, Secret";
  if (name == "Locked Key 1")         return "Locked, Key 1";
  if (name == "Locked Key 2")         return "Locked, Key 2";
  if (name == "Locked Key 3")         return "Locked, Key 3";
  if (name == "Locked Key 4")         return "Locked, Key 4";
  if (name == "Locked Key 5")         return "Locked, Key 5";
  if (name == "Locked Key 6")         return "Locked, Key 6";
  if (name == "Locked Key 7")         return "Locked, Key 7";
  if (name == "Locked Key 8")         return "Locked, Key 8";
  if (name == "Blocked Secret")       return "False Door";
  return "????";
}


void MapGrid(JReader& mapGrid, CString *wallNames, CString *backdropNames, int width, int height)
{
  CString backdrop;
  int backdropNumber = -1;
  mapGrid.NameAndValue("backdrop", backdrop);
  if (backdrop == "none")
  {
    backdrop = "";
  }
  if (!backdrop.IsEmpty())
  {
    int i;
    for (i=0; i<5; i++)
    {
      if (backdrop == backdropNames[i])
      {
        backdropNumber = i;
        break;
      };
    };
  }
  else
  {
    backdropNumber = 0;
  };
  if (backdropNumber == -1)
  {
    printf("Cannot map backdrop name '%s'\n", backdrop);
    backdropNumber = 0;
  };
    {
      int cellX, cellY;
      CELL *pCell;
      int i[2];
      i[0] = mapGrid.Fetch_Int("x");
      i[1] = mapGrid.Fetch_Int("y");

      cellX = i[0];
      cellY = i[1];
      if ((cellX > 99) || (cellY > 99))
      {
        die("Cell outside map boundaries");
      };
      pCell = &cells[cellX][cellY];

      pCell->x = cellX;
      pCell->y = cellY;

      mapGrid.NameAndValue("event_id", pCell->eventID);

      pCell->backdrop.Format("%d", backdropNumber);
      pCell->zone = mapGrid.Fetch_Int("zone");
      pCell->jsonLine = mapGrid.LineNum();
      {
        pCell->wallNorth          = WallNum(Str("wall_north"), wallNames)+1;
        pCell->wallNorthBlockage  = WallBlockage(Str("wall_north_blockage"));
      };
      {
        pCell->wallEast          = WallNum(Str("wall_east"), wallNames)+1;
        pCell->wallEastBlockage  = WallBlockage(Str("wall_east_blockage"));
      };
      {
        pCell->wallSouth          = WallNum(Str("wall_south"), wallNames)+1;
        pCell->wallSouthBlockage  = WallBlockage(Str("wall_south_blockage"));
      };
      {
        pCell->wallWest          = WallNum(Str("wall_west"), wallNames)+1;
        pCell->wallWestBlockage  = WallBlockage(Str("wall_west_blockage"));
      };
    
    };
}

/*
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
  if (c->event  != 0)      allDefault = false;
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
    if (c->event != 0)            jw.NameAndValue(JKEY_EVID, c->event);
    if (c->zone != 0)             jw.NameAndValue(JKEY_ZONE, c->zone);
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
*/