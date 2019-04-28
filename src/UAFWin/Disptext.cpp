/******************************************************************************
* Filename: Disptext.cpp
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

//#include "externs.h"
#include "Dungeon.h"


#include "items.h"
#include "char.h"
#include "class.h"
#include "level.h"
#include "globaldata.h"
#include "gameevent.h"
#include "party.h"
#include "graphics.h"
#include "Disptext.h"
//#include "GameMenu.h"
#include "Combatants.h"
#include "Drawtile.h"
#include "monster.h"
#include "Treas.h"
#include "viewport.h"
#include "FormattedText.h"
//#include "Getinput.h"


#ifdef TRACE_TIMER_DETAILS
class DETAIL_TRACE
{
  FILE *f;
public:
  DETAIL_TRACE(void);
  ~DETAIL_TRACE(void);
//  DWORD    m_id[1000];
//  DWORD    m_data[1000];
//  DWORD    m_time[1000];
//  DWORD    m_virtualTime[1000];
//  int      m_nextIndex;
  void     trace (DWORD id, DWORD data);
};
extern DETAIL_TRACE detailTrace;
#endif


#ifdef TRACE_TIMER_DETAILS
#define DETAILTRACE(a,b) detailTrace.trace((a),(b));
#else
#define DETAILTRACE(a,b)
#endif






#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int ItemsOnPage = 0;
int startItem;

RECT InventoryRects[Items_Per_Page];
RECT PartyNameRects[MAX_PARTY_MEMBERS];

CString CombatMsg;
CString timeDelayMsg; // appears in menu-bar area for x seconds

// error strings will be wrapped to fit message box, but
// make sure they fit in one box. There is no processing
// to let the player "Press Return to Continue" to see
// multiple boxes of error text continuation.
//
char *miscErrorText[MAX_MISC_ERROR_MSGS] = 
{
   "No Error",
   "UNKNOWN ERROR",
   "THE CHARACTER IS DEAD",
   "THE CHARACTER IS UNCONSCIOUS",
   "THE CHARACTER HAS FLED",
   "THE CHARACTER IS PETRIFIED",
   "THE CHARACTER IS GONE",
   "THE CHARACTER IS ANIMATED",
   "THE CHARACTER IS TEMPORARILY GONE",
   "THE CHARACTER IS RUNNING AWAY",
   "THE CHARACTER IS DYING",
   "NO MONEY",
   "NOT ENOUGH MONEY",
   "YOU HAVE THE MAXIMUM ITEM TYPES THAT YOU CAN CARRY",
   "YOU ARE NOT STRONG ENOUGH TO CARRY ANY MORE",
   "THIS ITEM TAKES TWO HANDS TO USE",
   "NOT USABLE BY YOUR CLASS",
   "A SIMILAR ITEM IS ALREADY READIED",
   "NO ICONS AVAILABLE",
   "NO SMALL PICTURES AVAILABLE",
   "NO DESIGNS IN DIRECTORY",
   "FAILED TO LOAD LEVEL FROM DISK",
   "INVALID GAME STATE ENCOUNTERED",
   "INVALID EVENT TYPE ENCOUNTERED",
   "INVALID MENU STATE ENCOUNTERED",
   "INVALID TASK STATE ENCOUNTERED",
   "YOUR PARTY CANNOT CONTAIN MORE CHARACTERS",
   "YOUR PARTY CANNOT CONTAIN MORE NPC's",
   "ITEM MUST NOT BE READIED",
   "SOME MONSTER COMBAT ICONS CANNOT BE FOUND",
   "GENERAL GRAPHICS ERROR",
   "DESIGN VERSION HIGHER THAN RUNTIME, LOAD FAILED, PLEASE UPGRADE",
   "SAVED GAME VERSION HIGHER THAN RUNTIME, LOAD FAILED, PLEASE UPGRADE",
   "SAVED CHAR VERSION ERROR, LOAD FAILED",
   "UNABLE TO LOAD FILE",
   "UNABLE TO SAVE FILE",
   "A CHARACTER WITH THIS NAME ALREADY EXISTS",
   "NOT ENOUGH FREE HANDS",
   "CHARACTER COULD NOT BE CREATED",
   "LastErrorType Placeholder"
};

miscErrorType miscError = NoError;
CString errorText;




//*****************************************************************************
// NAME: clearCharItemState
//
// PURPOSE:  
//*****************************************************************************
void clearCharItemState()
{
   startItem = 0;   
   party.activeItem = 0;
   ItemsOnPage = 0;
   charItemListText.Clear();
}


//*****************************************************************************
// NAME: clearTreasureItemState
//
// PURPOSE:  
//*****************************************************************************
void clearTreasureItemState()
{
   treasureItemListText.Clear();
   startItem = 0;   
   party.activeItem = 0;
   ItemsOnPage = 0;
}


//*****************************************************************************
// NAME: clearShopItemState
//
// PURPOSE:  
//*****************************************************************************
void clearShopItemState()
{
   itemListText.Clear();
   ItemsOnPage = 0;
   startItem = 0;   
   party.activeItem = 0;
}


//*****************************************************************************
// NAME: clearSpellItemState
//
// PURPOSE:  
//*****************************************************************************
void clearSpellItemState()
{
   spellListText.Clear();
   ItemsOnPage = 0;
   startItem = 0;   
   party.activeItem = 0;
}


//*****************************************************************************
// NAME: setCharItemTextState
//
// PURPOSE:  
//*****************************************************************************
void setCharItemTextState(CHARACTER &dude, 
                          BOOL (*pFilter)(ITEM  *pItem, void *pFilterPkt), 
                          void *pFilterPkt,
                          int flags)
{
   //if (charItemListText.GetCount() <= 0)
   clearCharItemState();

   flags |= QTY | RDY;
   if (party.PartyIsDetectingMagic())
     flags |= DETECTING_MAGIC;

   if ((flags & SPECIAL_ONLY) == 0)
   {
      charItemListText.FillItemListText(dude.myItems, 
                                        flags,
                                        Normal,
                                        pFilter,
                                        pFilterPkt);
      dude.addCharMoneyToItemList(charItemListText);
   };
   addSpecialItemsToItemList(charItemListText);
   charItemListText.FormatItemListTitle("YOU HAVE NO ITEMS");

   int cnt = charItemListText.GetCount();
   ItemsOnPage = min(Items_Per_Page,cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= charItemListText.GetCount())
        party.activeItem = (BYTE)(charItemListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
     clearCharItemState();
}


//*****************************************************************************
// NAME: setShopItemState
//
// PURPOSE:  
//*****************************************************************************
void setShopItemState(ITEM_LIST &list, costFactorType type)
{
   //if (itemListText.GetCount() <= 0)
     clearShopItemState();

   UINT flags = COST | QTY | MONEY;
   itemListText.FillItemListText(list, flags, type);
   itemListText.FormatItemListTitle("THIS SHOP HAS NO ITEMS");
   int cnt = itemListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= itemListText.GetCount())
        party.activeItem = (BYTE)(itemListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}


//*****************************************************************************
// NAME: setSpellItemState
//
// PURPOSE:  
//*****************************************************************************
// PRS July 2009void setSpellItemState(CHARACTER &cdata,  spellBookType *spellbook, DWORD flags, costFactorType type)
void setSpellItemState(CHARACTER &cdata, DWORD flags, costFactorType type)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();   // Clear spellListText

   //if (spellbook.SpellsKnown() > 0)
   //       spellbook.SortSpells();
   // PRS Jul 2009 spellListText.FillSpellListText(cdata, spellbook, flags, type);
   spellListText.FillSpellListText(cdata, flags, type);
   spellListText.FormatSpellListTitle("NO SPELLS AVAILABLE");
   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}

void setSpellNewLevelItemState(CHARACTER &data)
{
   clearSpellItemState();

   if (data.SpellsKnown() > 0)
     //data.SortSpells();
   
   spellListText.FillNewSpellLevelListText(data);

   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}

void setSpellMemorizeItemState(CHARACTER &data)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   //if (data.SpellsKnown() > 0)
     //data.SortSpells();
   spellListText.FillMemorizeSpellListText(&data);
   int cnt = spellListText.memSum.GetMemorizeItemCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}

void setSpellTempleCastItemState(CHARACTER &data, costFactorType type)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   //if (data.SpellsKnown() > 0)
     //data.SortSpells();
   spellListText.FillTempleCastSpellListText(data, type);
   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
   }
}

void setSpellCastItemState(CHARACTER &data, CASTING_ENVIRONMENT ce)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   //if (data.SpellsKnown() > 0)
     //data.SortSpells();
   spellListText.FillCastSpellListText(data, ce);
   int cnt = spellListText.GetIntegerCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetIntegerCount())
        party.activeItem = (BYTE)(spellListText.GetIntegerCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
   }
}

void setSpellEffectItemState(CHARACTER &data)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   //if (data.SpellsKnown() > 0)
     //data.SortSpells();
   spellListText.FillSpellEffectsListText(data, CHAR_VIEW_DISPLAY);
   int cnt = spellListText.GetEffectCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetEffectCount())
        party.activeItem = (BYTE)(spellListText.GetEffectCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
   }
}

//*****************************************************************************
// NAME: setTreasureEventState
//
// PURPOSE:  
//*****************************************************************************
void setTreasureEventState(GameEvent *event)
{
   //if (treasureItemListText.GetCount() <= 0)
   clearTreasureItemState();

   getTreasureEventItems(event);
   int cnt = treasureItemListText.GetCount();
   ItemsOnPage = min(Items_Per_Page,cnt);
  
   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= treasureItemListText.GetCount())
        party.activeItem = (BYTE)(treasureItemListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}


//*****************************************************************************
// NAME: nextTreasurePage
//
// PURPOSE:  
//*****************************************************************************
void nextTreasurePage()
{
  if (startItem+Items_Per_Page < treasureItemListText.GetCount())
    startItem += Items_Per_Page;  
  int cnt = (treasureItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevTreasurePage
//
// PURPOSE:  
//*****************************************************************************
void prevTreasurePage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (treasureItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: allowTreasureItemNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowTreasureItemNextPage()
{
  // only one page of data
  if (treasureItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (treasureItemListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowTreasureItemPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowTreasureItemPrevPage()
{
  // only one page of data
  if (treasureItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: nextCharItemsPage
//
// PURPOSE:  
//*****************************************************************************
void nextCharItemsPage()
{
  if (startItem+Items_Per_Page < charItemListText.GetCount())
    startItem += Items_Per_Page;
  int cnt = (charItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevCharItemsPage
//
// PURPOSE:  
//*****************************************************************************
void prevCharItemsPage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (charItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);
  
  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: allowCharItemNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowCharItemNextPage()
{
  // only one page of data
  if (charItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (charItemListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowCharItemPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowCharItemPrevPage()
{
  // only one page of data
  if (charItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: nextShopPage
//
// PURPOSE:  
//*****************************************************************************
void nextShopPage()
{
  if (startItem+Items_Per_Page < itemListText.GetCount())    
    startItem += Items_Per_Page;
  int cnt = (itemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevShopPage
//
// PURPOSE:  
//*****************************************************************************
void prevShopPage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (itemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}

//*****************************************************************************
// NAME: allowShopItemNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowShopItemNextPage()
{
  // only one page of data
  if (itemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (itemListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowShopItemPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowShopItemPrevPage()
{
  // only one page of data
  if (itemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: nextSpellPage
//
// PURPOSE:  
//*****************************************************************************
void nextSpellPage()
{
  if (startItem+Items_Per_Page < spellListText.GetCount())    
    startItem += Items_Per_Page;
  int cnt = (spellListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevSpellPage
//
// PURPOSE:  
//*****************************************************************************
void prevSpellPage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (spellListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}

//*****************************************************************************
// NAME: nextLinePage
//
// PURPOSE:  
//*****************************************************************************
void nextLinePage()
{
  if (startItem+Items_Per_Page < lineListText.GetCount())    
    startItem += Items_Per_Page;
  int cnt = (lineListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevLinePage
//
// PURPOSE:  
//*****************************************************************************
void prevLinePage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (lineListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: allowSpellNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowSpellNextPage()
{
  // only one page of data
  if (spellListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (spellListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowSpellPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowSpellPrevPage()
{
  // only one page of data
  if (spellListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: currSpellTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currSpellTextIndex()
{
  return startItem+party.activeItem;
}

//*****************************************************************************
// NAME: currLineTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currLineTextIndex()
{
  return startItem+party.activeItem;
}


//*****************************************************************************
// NAME: currShopTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currShopTextIndex()
{
  return startItem+party.activeItem;
}


//*****************************************************************************
// NAME: currCharItemsTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currCharItemsTextIndex()
{
  return startItem+party.activeItem;
}


//*****************************************************************************
// NAME: currTreasureTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currTreasureTextIndex()
{
  return startItem+party.activeItem;
}


BOOL shouldHighlight(int index)
{
  return ((index-startItem)== party.activeItem);
}

//*****************************************************************************
// NAME: displayPoolGoldTotal
//
// PURPOSE:  
//*****************************************************************************
void displayPoolGoldTotal()
{
  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);

  CString temp;
  int qty;

  if (party.moneyPooled)
    qty = party.GetPoolGoldValue();
  else
    qty = party.getActiveChar().poolCharacterGold();

  temp.Format("AVAILABLE %s: %u", globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()), qty);
  displayText(textboxX+(TEXTBOX_WIDTH/3), textboxY+20, temp);

  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
}

//*****************************************************************************
// NAME: displayCharacterHP
//
// PURPOSE: Displays character name and hitpoints in message box window
//          during temple spell casting.
//*****************************************************************************
void displayCharacterHP(CHARACTER &dude)
{
  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);

  COLORREF color;
  CString temp;
  int hp = dude.GetAdjHitPoints();
  temp.Format("%s     %i/%i", dude.GetName(), hp, dude.GetMaxHitPoints());
  
  if (hp <= 0)
    color = Red;
  else if (hp < dude.GetMaxHitPoints())
     color=Yellow;
  else if (dude.GetAdjReadyToTrain())
     color = Blue;
  else
     color=Green;
  displayText(textboxX+(TEXTBOX_WIDTH/3), textboxY+20, temp, color);

  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
}

// figure out if x,y is within one of the displayed party member RECTs
int IntersectPointWithInventory(long x, long y)
{
// If you ain't selecting, don't call!
  POINT pt = { x, y };

  for (int i=0; i < Items_Per_Page; i++)
  {
    if (PtInRect(&InventoryRects[i], pt))
      return i;
  }

  return -1;
}

// figure out if x,y is within one of the displayed party member RECTs
int IntersectPointWithParty(long x, long y)
{
  // Don't call me if you ain't selecting party members!
  POINT pt = { x, y };

  for (int i=0; i < party.numCharacters; i++)
  {
    if (PtInRect(&PartyNameRects[i], pt))
      return i;
  }

  return NO_DUDE;
}

//*****************************************************************************
// NAME: displayPartyNames
//
// PURPOSE:  
//*****************************************************************************
void displayPartyNames(PARTY &pty, bool useWX, bool showNPC)
{  
  DETAILTRACE(0x4c1120,0);
  memset(PartyNameRects, 0, sizeof(PartyNameRects));

   // a couple of cases when party data isn't drawn
   if (   (globalData.levelInfo.stats[globalData.currLevel].overland)
       && (globalData.useAreaView))
     return;

   if (currPic.picType == BigPicDib)
     return;

  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  DETAILTRACE(0x4c1121,0);
  GraphicsMgr.EnableFontColorTags(FALSE);
  DETAILTRACE(0x4c1122,0);
  int LineHeight = GraphicsMgr.GetMaxCharacterHeight()+2;

   int i,x;
   int y;                //DLD 12/20/00 //int y=18;
   int wx=PartyNames_wx; //const int wx=18;
   int nx=PartyNames_nx; //const int nx=275;
   int wy=PartyNames_wy; //
   int ny=PartyNames_ny; //end change/add
   
   COLORREF color;
   COLORREF namecolor;
   CString temp;   

   x = useWX ? wx : nx;
   y = useWX ? wy : ny; //DLD 12/20/00
   
   displayText(x, y, "NAME", White);
   DETAILTRACE(0x4c1123,0);
   displayText(x + 225, y, " AC", White); //DLD 12/20/00 //displayText(500, y, " AC", White);
   displayText(x + 300, y, " HP", White); //displayText(575, y, " HP", White);
   
   for (i=0; i<pty.numCharacters;i++)
   {
      if ((!showNPC) && (pty.characters[i].GetType() == NPC_TYPE))
        continue;

      int hp = pty.characters[i].GetAdjHitPoints();
      if (hp <= 0)
        color = Red;
      else if (hp < pty.characters[i].GetMaxHitPoints())
         color=Yellow;
      else
         color=Green;

      if (pty.characters[i].GetAdjReadyToTrain())
        namecolor=Blue;
      else
        namecolor=Green;

      y+=LineHeight;//20;
      
      if (pty.activeCharacter == i)
         displayText(x,y, pty.characters[i].GetName(), namecolor, TRUE);
      else
         displayText(x,y, pty.characters[i].GetName(), namecolor);
      
      {
        int adjAC, effAC;
        adjAC = pty.characters[i].GetAdjAC();
        effAC = pty.characters[i].GetEffectiveAC();
        temp.Format("%3i", effAC);
        displayText(x + 225,y, temp, (effAC==adjAC)?namecolor:Yellow); //DLD 12/20/00 //displayText(500,y, temp, color);
      };
      
      temp.Format("%3i", hp);
      displayText(x + 300,y, temp, color); //DLD 12/20/00 //displayText(575,y, temp, color);

      PartyNameRects[i].left = x;
      PartyNameRects[i].top = y;
      PartyNameRects[i].right = x + 300 + GraphicsMgr.GetTextWidth(temp, temp.GetLength()); //DLD 12/20/00 //PartyNameRects[i].right = 575 + GraphicsMgr.GetTextWidth(temp, strlen(temp));
      PartyNameRects[i].bottom = y + GraphicsMgr.GetMaxCharacterHeight();
   }
     
   if (x == nx)
   {
      temp.Format("%i,%i", pty.Posx, pty.Posy);
      displayText(LocationText_x, LocationText_y, temp, White); //DLD 12/20/00 //displayText(275, 280, temp, White);
      
      if (!globalData.levelInfo.stats[globalData.currLevel].overland)
      {
        switch (pty.facing) 
        {
        case FACE_NORTH:
           temp = "North";
           break;
        case FACE_EAST:
           temp = "East";
           break;
        case FACE_SOUTH:
           temp = "South";
           break;
        case FACE_WEST:
           temp = "West";
           break;
        }      
        displayText(LocationText_x + 25 + 40, LocationText_y, temp, White); //DLD 12/20/00 //displayText(340, 280, temp, White);
      }
      
      if (pty.searching)
         displayText(LocationText_x + 25 + 85, LocationText_y, "Searching", White); //DLD 12/20/00 //displayText(385, 280, "Searching", White);
      
      temp.Format("Day %i %02i:%02i", pty.days, pty.hours, pty.minutes);
            displayText(LocationText_x + 125 + 70, LocationText_y, temp, White); //DLD 12/20/00 displayText(470, 280, temp, White);
   }
   DETAILTRACE(0x4c1123,0);
   GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
   DETAILTRACE(0x4c1124,0);
}


DisplayStatusPkt::~DisplayStatusPkt(void)
{
  for (int i=0; i<m_numString; i++)  delete m_pStrings[i]; 
  free(m_pStrings);
  m_pStrings = NULL;
  m_numString = 0;
}


void DisplayStatusPkt::AddString(const CString& str)
{
  m_numString++;
  m_pStrings = (CString **)realloc(m_pStrings, m_numString*sizeof(CString *));
  m_pStrings[m_numString-1] = new CString(str);
}

CBRESULT ScriptCallback_CharDisplayStatus(CBFUNC func, CString *str, void *pkt)
{
  DisplayStatusPkt *pResult;
  pResult = (DisplayStatusPkt *)pkt;
  if (func == CBF_EXAMINESCRIPT)
  {
    if (!str->IsEmpty())
    {
        pResult->AddString((*p_hook_parameters)[6]);
        pResult->AddString((*p_hook_parameters)[7]);
    };
  };
  return CBR_CONTINUE;
}




//*****************************************************************************
// NAME: displayCombatStats
//
// PURPOSE:  
//*****************************************************************************
void displayCombatStats(int dude)
{ 
   if ((dude < 0) || (dude >= combatData.NumCombatants()))
      return;


  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);
  int LineHeight = GraphicsMgr.GetMaxCharacterHeight()+2;

  int x = COMBAT_SCREEN_WIDTH+(VertBarSrc.right-VertBarSrc.left)+2;
  int y = 18;
  CString wp, dmg, temp;
  COLORREF color, color2; 
  const COLORREF AdjColor = RGB(255,128,0); // Bright Orange, for stats currently modified by active spells   
   
  if (combatData.GetCombatant(dude)->GetHitPoints() <= 0)
    color2 = CombatRed;
  else if (combatData.GetCombatant(dude)->GetIsFriendly())
    color2 = CombatBlue;
  else
    color2 = CombatYellow;

  int hp = combatData.GetCombatant(dude)->GetAdjHitPoints();
  if (hp <= 0)
     color = CombatRed;
  else if (hp < combatData.GetCombatant(dude)->GetMaxHitPoints())
     color = CombatYellow;
  else
     color = CombatGreen;
   
  // display name
#ifdef _DEBUG
  CString DisplayName;   
  DisplayName.Format("%s (%i:%i,%i)",
            combatData.GetCombatant(dude)->GetName(), 
            dude,
            combatData.GetCombatant(dude)->x,
            combatData.GetCombatant(dude)->y);
  displayText(x,y, DisplayName, color2);
#else
  displayText(x,y, combatData.GetCombatant(dude)->GetName(), color2);
#endif
   
  // display current hit points
  y += LineHeight;
  displayText(x,y, "HITPOINTS ", CombatYellow);
  temp.Format("%3i", hp);
  int hpx = x + GraphicsMgr.GetTextWidth("HITPOINTS ", strlen("HITPOINTS "));
  if (hp!=combatData.GetCombatant(dude)->GetHitPoints())
    displayText(hpx,y, temp, AdjColor);
  else
    displayText(hpx,y, temp, color);
   
  // display current armor class
  y += LineHeight;//18;
  displayText(x,y, "AC ", CombatYellow);
  int currAC = combatData.GetCombatant(dude)->GetEffectiveAC();

/*  Deleted 28 Feb 2010; We re-defined CHARACTER.AC;
    // Added 29 July 2009 PRS
   if (    (combatData.GetCombatant(dude).GetType() == MONSTER_TYPE)
        ||     (combatData.GetCombatant(dude).GetType() == NPC_TYPE)// Added 28 Feb 2010 PRS
            &&  !combatData.GetCombatant(dude).friendly             // Added 28 Feb 2010 PRS
      )
   {
      currAC += combatData.GetCombatant(dude).myItems.GetProtectModForRdyItems();
   };
    //
    */

  temp.Format("%3i", currAC);
  if (currAC != combatData.GetCombatant(dude)->GetEffectiveAC())
    displayText(x+25, y, temp, AdjColor);
  else
    displayText(x+25, y, temp, CombatYellow);
  y += LineHeight;//18;

   // display current readied weapon
  combatData.getCharWeaponText(dude, wp, dmg);
  if ((!wp.IsEmpty()) && (wp.CompareNoCase("NONE")!=0))
  {
     displayText(x,y, wp, CombatYellow);
     y += LineHeight;//18;
  }

#ifdef _DEBUG
  temp.Format("Attacks: %f", combatData.GetCombatant(dude)->AttacksRemaining());
  displayText(x,y, temp, White);
  y += LineHeight;//18;
  temp.Format("Cursor: %u, %u", combatData.m_iCursorX, combatData.m_iCursorY);
  displayText(x,y, temp, White);
  y += LineHeight;//18;

#endif

  /*
  POSITION pos = combatMsgs.msgs.GetHeadPosition();
  while (pos != NULL)
  {
    displayText(x,y, combatMsgs.msgs.GetNext(pos), White);
    y += 18;
  }
  */

  {
    // We moved this here from OnStartCombatAction so that
    // targets will also have their stats displayed.

    HOOK_PARAMETERS hookParameters;
    SCRIPT_CONTEXT scriptContext;
    ActorType actor;
    int i;
    DisplayStatusPkt results;
    COMBATANT *pCombatant;
    pCombatant = combatData.GetCombatant(dude);
    SetCharContext(&actor);
    scriptContext.SetCharacterContext(pCombatant->m_pCharacter);
    scriptContext.SetCombatantContext(pCombatant);

    hookParameters[5] = "COMBAT";

    {
        //For Each dude->SpellEffect
      POSITION p;
      //GLOBAL_SPELL_ID gsID;
      SPELL_ID spellID;
      SPELL_DATA *pSpell;
      const SPELL_EFFECTS_DATA *pSpellEffect;
      //pCombatant->SortSpells();
      //if (virtualGameTime == 0xd110)
      //{
      //  int kkk=1;
      //};
      ; // access ordered list
      for ( p = pCombatant->m_pCharacter->m_spellEffects.GetHeadPosition();
            p != NULL; 
            pCombatant->m_pCharacter->m_spellEffects.GetNext(p))
      {
         int oldCount;
         pSpellEffect = pCombatant->m_pCharacter->m_spellEffects.PeekAt(p);
         //gsID = pSpellEffect->SourceSpell_ID();
         spellID = pSpellEffect->SourceSpell_ID();
         //pSpell = spellData.GetSpellData(gsID);
         pSpell = spellData.GetSpell(spellID);
         if (pSpell == NULL) continue;
  
         oldCount = results.m_numString;
         pSpell->RunSpellScripts(CHAR_DISPLAY_STATUS,
                 ScriptCallback_CharDisplayStatus,
                 &results);
        if (results.m_numString == oldCount)
        {  // If nothing added, use spell name.
          results.AddString("");
          results.AddString(pSpell->Name);
        };
      };       
    };

    pCombatant->m_pCharacter->RunCharacterScripts(CHAR_DISPLAY_STATUS, 
                                    ScriptCallback_CharDisplayStatus, 
                                    &results);
    pCombatant->RunCombatantScripts(CHAR_DISPLAY_STATUS, 
                                    ScriptCallback_CharDisplayStatus, 
                                    &results);
    ClearCharContext();
    for (i=1; i<results.m_numString; i+=2)
    {
      CString *msg;
      msg = results.m_pStrings[i];
      if (!msg->IsEmpty())
      {
        displayText(x, y, *msg, White);
        y += LineHeight;//18;
      };
    };
  };
  y+=(LineHeight*2);
  for (int i=0;i<combatTextData.numLines;i++)
  {
    //displayText(x,y, combatTextData.text[i], White);
    displayText(x,y, combatTextData.GetTextLine(i)->text, White);
    y += LineHeight;//18;
  }
  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
} 

#ifdef _DEBUG
// debug only, shows details about terrain under cursor
// while aiming manually
void displayCursorStats()
{
   BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
   GraphicsMgr.EnableFontColorTags(FALSE);

   const int x = COMBAT_SCREEN_WIDTH+18;
   int y = 150; // closer to the bottom
   
   TILE_DATA data;
   int cellNum = GetCellNum(combatData.m_iCursorX,combatData.m_iCursorY);
   data = GetTileData(combatData.m_iCursorX,combatData.m_iCursorY);

   CString tmp;

   tmp.Format("Cursor X,Y: %i,%i", combatData.m_iCursorX,combatData.m_iCursorY);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("Cell: %i", cellNum);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("enab: %i", data.tile_enabled);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("X,Y: %i,%i", data.tile_posx, data.tile_posy);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("visi: %i", data.tile_invisible);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("obst: %i", data.tile_passable);
   displayText(x,y, tmp, White);
   
   GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
} 
#endif
