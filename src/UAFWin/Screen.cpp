/******************************************************************************
* Filename: Screen.cpp
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


#include "globaldata.h"
#include "soundmgr.h"
#include "graphics.h"
#include "viewport.h"
#include "gameevent.h"
#include "Disptext.h"
#include "CharStatsForm.h"
#include "GameMenu.h"
#include "Path.h"
#include "Drawtile.h"
#include "party.h"
#include "FormattedText.h"
#include "../Shared/GPDLOpCodes.h"
#include "gpdlexec.h"
#include "Combatants.h"
#include "MouseThread.h"

#include "Screen.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


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



const int IconPickX = 100;
const int IconPickY = 100;

// Decides what to draw in viewport
// If currPic has something to draw it will be done
// draw viewport frame
// draw viewport contents if needed (no pic? then walls or map)
// draw currPic data
void blitView(DWORD flags, BOOL drawViewportPic, PIC_DATA *pPic)
{
  //if ((globalData.BackgroundSurf >= 0) && (currPic.key > 0))
  //{
  //  // so that pic area is clear of background image
  //  RECT rect = { 0, 0, SCREEN_WIDTH, HorzBarMidY };
  //  GraphicsMgr.ClearSurface(BACKBUFFERNUM, BACKGROUND_FILL, &rect, FALSE);
  //}
  drawBorder();
  
  if ((currPic.key > 0) && (currPic.picType == BigPicDib) && (flags & BigPicDib))
    drawViewport(FALSE, FALSE, drawViewportPic, TRUE);
  else
    drawViewport(TRUE, TRUE, drawViewportPic, TRUE);
  
  if (drawViewportPic)
  {
    //if (currPic.key > 0)
    if (pPic->key > 0)
    {
      //if ((currPic.picType == BigPicDib) && (flags & BigPicDib))
      if ((pPic->picType == BigPicDib) && (flags & BigPicDib))
      {
        //GraphicsMgr.BlitSprite(currPic.sx, currPic.sy, currPic.key);
        GraphicsMgr.BlitSprite(pPic->sx, pPic->sy, pPic->key);
      }
      //else if ((currPic.picType == SpriteDib) && (flags & SpriteDib))
      else if ((pPic->picType == SpriteDib) && (flags & SpriteDib))
      {
        updateViewport(party.Posx, party.Posy, party.facing, BACKBUFFERNUM, TRUE);
        //GraphicsMgr.BlitSprite(currPic.sx, currPic.sy, currPic.key);
        GraphicsMgr.BlitSprite(pPic->sx, pPic->sy, pPic->key);
      }
      //else if ((currPic.picType == SmallPicDib) && (flags & SmallPicDib))
      else if ((pPic->picType == SmallPicDib) && (flags & SmallPicDib))
      {
        updateViewport(party.Posx, party.Posy, party.facing, BACKBUFFERNUM, FALSE);
        //GraphicsMgr.BlitSprite(currPic.sx, currPic.sy, currPic.key);
        GraphicsMgr.BlitSprite(pPic->sx, pPic->sy, pPic->key);
      }
      else
        updateViewport(party.Posx, party.Posy, party.facing, BACKBUFFERNUM, FALSE);
    }
    else
      updateViewport(party.Posx, party.Posy, party.facing, BACKBUFFERNUM, FALSE);
  };
}

void DisplayTextAndFlip(void)
{  
  TEXT_DISPLAY_DATA *pText;

  if (errorTextData.numLines > 0)
    pText = &errorTextData;
  else
    pText = &textData;

  if (pText->NeedsFrontBuffer())
  {
    GraphicsMgr.FlipSurfaces();
    DisplayFormattedText(*pText);
  }
  else
  {
    DisplayFormattedText(*pText);
    GraphicsMgr.FlipSurfaces();
  }
}

void UpdateBigSmallSprite(BOOL drawViewportPic)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);  
  blitView(SmallPicDib | BigPicDib | SpriteDib, drawViewportPic, &currPic);
  menu.DisplayMenu();
  displayPartyNames(party, false, true);
  DisplayTextAndFlip();
}

void UpdateSmall(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(SmallPicDib, TRUE, &currPic);
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateBig(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(BigPicDib, TRUE, &currPic);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateSmallSprite(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(SmallPicDib | SpriteDib, TRUE, &currPic);
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void DisplayCombatData(BOOL ShowCursor=TRUE, BOOL FullIcon=TRUE)
{
  GraphicsMgr.ClearSurface(BACKBUFFERNUM, BACKGROUND_COMBAT_FILL); 
  displayCombatWalls();    
  DisplayAnimateList();
  combatData.DisplayCombatants();
  combatData.DisplayAURAs();
  if (ShowCursor)
    combatData.displayCursor(FullIcon);
  displayCombatBorders();
}

void DisplayCombatDataWithSprite(BOOL ShowCursor=TRUE, BOOL FullIcon=TRUE)
{
  GraphicsMgr.ClearSurface(BACKBUFFERNUM, BACKGROUND_COMBAT_FILL); 
  displayCombatWalls();    
  DisplayAnimateList();
  combatData.DisplayCombatants();
  combatData.DisplayAURAs();
  if (ShowCursor) 
    combatData.displayCursor(FullIcon);
  combatSprite.Display();
  displayCombatBorders();
}

///////////////////////////////////////
// Load and Display Credits Screen
///////////////////////////////////////
BOOL Credits()
{  
  WriteDebugString("Loading credits screen and music\n");
  return GraphicsMgr.FadeInImage(globalData.CreditsBgArt, TitleDib, rte.CreditsArtDir());
}

void UpdateStartScreen(void)
{
  int offsetX, offsetY;
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displayPartyNames(party, true, true);
  menu.DisplayMenu();

  // format version as text
  CString DesignVersionInfo;
  DesignVersionInfo.Format("%s  ver %01.6f", 
                           globalData.designName,
                           ((float)globalData.version));

  // determine coords for top/center of screen
  if ((dsgnVerX>=0)&&(dsgnVerY>=0))
  {
    offsetX=dsgnVerX;
    offsetY=dsgnVerY;
  }
  else
  {
    offsetX = (SCREEN_WIDTH/2) - 
            (GraphicsMgr.GetTextWidth(DesignVersionInfo, DesignVersionInfo.GetLength())/2) - 1;
    offsetY = -1; //DLD 12/20/00 change due to new art layout //offsetY = 1;
  }
  // blit to primary surface
  displayText(offsetX, offsetY, DesignVersionInfo, White, FALSE, FALSE, TRUE);
  DisplayTextAndFlip();
}

void UpdateIconPickScreen(void)
{
  int offsetX=0, offsetY=0;
  int width, height, bits, temp;
  //RECT rect;

  if (GraphicsMgr.GetSpriteSurfaceStats(character.icon.key, width, height, bits))
  {  
    temp = width / COMBAT_TILE_WIDTH;
    temp /= (character.icon.NumFrames/2);
    if (temp > 4)
      offsetX = 0;
    else if (temp == 4)
      offsetX = COMBAT_TILE_WIDTH;
    else if (temp == 2)
      offsetX = 2*COMBAT_TILE_WIDTH;
    else 
    {
      offsetX = 0;
      WriteDebugString("Invalid icon width for GS_IconPick in UpdateIconPickScreen\n");
    };
    temp = height / COMBAT_TILE_HEIGHT;
    if (temp == 3)
       offsetY = 0;
    else if (temp == 2)
       offsetY = COMBAT_TILE_HEIGHT;
    else if (temp == 1)
       offsetY = 2*COMBAT_TILE_HEIGHT;
    else 
    {
       WriteDebugString("Invalid icon height for GS_IconPick in UpdateIconPickScreen\n");
       offsetY = 0;
    }
  }

  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);

  drawBorder();
  if (SpriteBkGndArt > 0)
     GraphicsMgr.BlitImage(IconPickX, IconPickY, SpriteBkGndArt, NULL);

  if (character.icon.key >= 0)
  {
    character.icon.SetFrame(0);
    GraphicsMgr.BlitSprite( IconPickX+offsetX, 
                            IconPickY+offsetY, 
                            character.icon.key);  
    character.icon.SetFrame(1);
    offsetX +=  (COMBAT_TILE_WIDTH + (width/2));
    GraphicsMgr.BlitSprite( IconPickX+offsetX, 
                            IconPickY+offsetY, 
                            character.icon.key);  
  };
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateSmallPicScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  if (character.smallPic.key > 0)
     GraphicsMgr.BlitSprite(IconPickX, IconPickY, character.smallPic.key);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateAdventureScreen(void)
{
  DETAILTRACE(0x4c3321,0);
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  DETAILTRACE(0x4c3322,0);
  drawBorder();
  DETAILTRACE(0x4c3323,0);
  drawViewport(TRUE, TRUE, TRUE, TRUE);
  DETAILTRACE(0x4c3324,0);
  updateViewport(party.Posx, party.Posy, party.facing, BACKBUFFERNUM, FALSE);
  DETAILTRACE(0x4c3325,0);
  displayPartyNames(party, false, true);         

  DETAILTRACE(0x4c3326,0);
  menu.DisplayMenu();
  DETAILTRACE(0x4c3327,0);
  DisplayTextAndFlip();
}         

void UpdateTextEventScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateQuestionYesNoScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateQuestionListScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateQuestionButtonScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateVaultScreen(void)
{
  UpdateSmallSprite();
}

void UpdateGiveTreasureScreen(void)
{
  UpdateSmallSprite();
}

void UpdateViewCharacterScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewportFrame(VPFrameXOffset, VPFrameY+VPFrameYOffset);
  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
       GraphicsMgr.BlitSprite(VPFrameXOffset+(ViewportX-VPFrameX), VPFrameY+VPFrameYOffset+(ViewportY-VPFrameY), currPic.key);
  }
  menu.DisplayMenu();
  //if (errorTextData.numLines == 0)
    displayCharStats(party.getActiveChar());
  DisplayTextAndFlip();
}

void UpdateCombatViewCharacterScreen(void)
{  
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewportFrame(VPFrameXOffset, VPFrameY+VPFrameYOffset);
  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
       GraphicsMgr.BlitSprite(VPFrameXOffset+(ViewportX-VPFrameX), VPFrameY+VPFrameYOffset+(ViewportY-VPFrameY), currPic.key);
  }
  menu.DisplayMenu();
  if (errorTextData.numLines == 0)
    displayCharStats(*combatData.getActiveCombatCharacter().m_pCharacter);
  DisplayTextAndFlip();
}

void UpdateEncampScreen(void)
{
  UpdateSmallSprite();
}

void UpdateTransferMenuScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}
    
void UpdateTransferMsgScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport(TRUE, TRUE, TRUE, TRUE);
  updateViewport(party.Posx, party.Posy, party.facing, BACKBUFFERNUM, FALSE);
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateTavernMenuScreen(void)
{
  UpdateSmallSprite();
}

void UpdateGiveDamageScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateGainExpScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateHealPartyScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateWhoPaysScreen(void)
{
  UpdateSmall();
}

void UpdatePassTimeScreen(void)
{
  UpdateSmall();
}

void UpdateSmallTownScreen(void)
{
  UpdateSmall();
}
void UpdateRandomEventScreen(void)
{
  UpdateSmall();
}
void UpdateAddNPCScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateRemoveNPCScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateNPCSaysScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateShopScreen(void)
{
  UpdateSmallSprite();
}

void UpdateSpecialItemKeyScreen(void)
{
  UpdateSmall();
}


void UpdateGPDLScreen(GPDL *pGPDL, PIC_DATA *pPic)
{
  GPDL_STATE dscState;
  dscState=pGPDL->State();
  if (dscState!=GPDL_UNINITIALIZED) 
  {
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    blitView(SmallPicDib | BigPicDib | SpriteDib, TRUE, pPic);
    displayPartyNames(party, false, true);
    menu.DisplayMenu();
       
    if (errorTextData.numLines == 0)
    {
    // DisplayFormattedText(textData, White);
      int x = textboxX;
      int y = textboxY;
      //discourse.Display();
      GraphicsMgr.ResetFontColorTags();
      GraphicsMgr.EnableFontColorTags(TRUE);
      for (int i=0; i<discourseLine.GetSize(); i++)
      {
        GraphicsMgr.ResetFontColorTags();
        displayText(x,y+18*i, discourseLine[i], White, FALSE, FALSE, TRUE);
      };
      GraphicsMgr.FlipSurfaces();
      GraphicsMgr.EnableFontColorTags(FALSE);
    }
    else
      DisplayTextAndFlip();    
  }; // if (!GPDL_UNINITIALIZED)
}

void UpdateTakePartyItemsScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateTrainHallWelcomeScreen(void)
{
  UpdateSmall();
}

void UpdateWhoTriesScreen(void)
{
  UpdateSmall();
}

void UpdateTempleScreen(void)
{
  UpdateSmallSprite();
}

void UpdateTempleGiveScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(SmallPicDib | SpriteDib, TRUE, &currPic);
  displayPartyNames(party,false, true);
  menu.DisplayMenu();
  if (errorTextData.numLines == 0) displayPoolGoldTotal();
  DisplayTextAndFlip();
}

void UpdatePasswordScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateEncounterScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateQuestScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateGuidedTourScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateGuidedTourMoveScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport(TRUE, TRUE, TRUE, TRUE);
  updateViewport(party.Posx, party.Posy, party.facing, BACKBUFFERNUM, FALSE);
  displayPartyNames(party, false, true);
  DisplayTextAndFlip();
}

void UpdateSpeedCombatScreen(void)
{
  DisplayCombatData();
  menu.DisplayMenu();
  displayCombatStats(getCombatantInCell(combatData.m_iCursorX, combatData.m_iCursorY, 1, 1));
  DisplayTextAndFlip();  
}

void UpdateSpeedScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateCombatRoundDelayScreen(void)
{
  DisplayCombatData();
  DisplayTextAndFlip();
}

void UpdateCombatTimeDelayMsg(int dude)
{
  DETAILTRACE(0x2ac700,dude);
  DisplayCombatDataWithSprite();
  menu.DisplayMenu();
  if (dude < 0) dude = combatData.GetCurrCombatant();
  displayCombatStats(dude);
  DisplayTextAndFlip();
}

void UpdateSpriteAnimateScreen(int dude)
{
  DETAILTRACE(0x2ac701,dude);
  DisplayCombatDataWithSprite(FALSE); // no cursor
  menu.DisplayMenu();
  displayCombatStats(dude);
  DisplayTextAndFlip();
}

void UpdateCombatMoveScreen(void)
{
  DisplayCombatData(FALSE); // no cursor
  menu.DisplayMenu();
  displayCombatStats(combatData.GetCurrCombatant());
  DisplayTextAndFlip();
}

void UpdateCombatAimManualScreen(void)
{
  DisplayCombatData(TRUE,FALSE);
  menu.DisplayMenu();
  
  int dude = getCombatantInCell(combatData.m_iCursorX, combatData.m_iCursorY, 1,1);
  if (dude == NO_DUDE)
    dude = getDeadCombatantInCell(combatData.m_iCursorX, combatData.m_iCursorY, 1,1);

  displayCombatStats(dude);
  
#ifdef _DEBUG
  displayCursorStats();
#endif
  DisplayTextAndFlip();
}

void UpdateCombatAimScreen(void)
{
  DisplayCombatData();
  menu.DisplayMenu();
  displayCombatStats(getCombatantInCell(combatData.m_iCursorX, combatData.m_iCursorY, 1, 1));
  DisplayTextAndFlip();
}

void UpdateCombatMainMenuScreen(void)
{
  DETAILTRACE(0x2ab080,0);
  DisplayCombatData();
  DETAILTRACE(0x2ab081,0);
  menu.DisplayMenu();
  DETAILTRACE(0x2ab082,0);
  displayCombatStats(getCombatantInCell(combatData.m_iCursorX, combatData.m_iCursorY, 1, 1));  
  DETAILTRACE(0x2ab083,0);
  DisplayTextAndFlip();
  DETAILTRACE(0x2ab084,0);
}
/*
void UpdateHumanChangeClassScreen(void)
{
  UpdateBigSmallSprite(FALSE);
}
*/
void UpdateTradeTakerSelectScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateCombatTradeTakerSelectScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport(TRUE, TRUE, TRUE, TRUE);

  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
       GraphicsMgr.BlitSprite(ViewportX, ViewportY, currPic.key);
  }
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateAlterGameScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateAlterOrderScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdatePartyDiedScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  // don't have a picture for this yet
  //blitView(SmallPicDib | BigPicDib | SpriteDib);
  //if (currPic.picType != BigPicDib)
  //   displayPartyNames(party, false, true);
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateCombatResultsScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  //blitView(SmallPicDib | BigPicDib | SpriteDib);
  //if (currPic.picType != BigPicDib)
  //   displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateCombatMagicMenuScreen(void)
{
  DisplayCombatData();
  menu.DisplayMenu();
  displayCombatStats(getCombatantInCell(combatData.m_iCursorX, combatData.m_iCursorY, 1, 1));  
  DisplayTextAndFlip();
}

void UpdateMagicMenuScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}

void UpdateScribeMenuScreen(void)
{
  UpdateBigSmallSprite(TRUE);
}
/*
void UpdateCombatDisplaySpellEffectsMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateDisplaySpellEffectsMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/

/*
void UpdateMemorizeVerifyMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateCombatCastMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateCastMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateMemorizeMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  // draw middle horz bar, but 
  // not viewport and middle ver bar
  drawViewport(FALSE, FALSE);
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
void UpdateRestMenuScreen(void)
{

}

void UpdateAppraiseSelectMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateAppraiseEvaluateMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateAskYesNoMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
/*
// this screen is displayed before a design is loaded, which
// means that the usual art files are not loaded - frames,
// cursors, horz/vert bars, etc.
//
void UpdateDesignSelectMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  displaySpells(); // design list built using spell list code
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
*/
void UpdateTargetSelectNonCombatScreen(void)
{
  UpdateSmall();
}

void UpdateDisplayPartyJournalScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  

  if (journalTextData.NeedsFrontBuffer())
  {
    GraphicsMgr.FlipSurfaces();
    DisplayJournalText(journalTextData);
  }
  else
  {
    DisplayJournalText(journalTextData);
    GraphicsMgr.FlipSurfaces();
  }
}

void UpdateMovieBigPicWindow(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport(FALSE, FALSE, TRUE, TRUE);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateMovieSmallPicWindow(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport(TRUE, TRUE, TRUE, TRUE);
  displayPartyNames(party, false, true); // useWX=0, ShowNPC=1
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateRaceMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  //int totalListItems = spellListText.GetCount();
  int totalListItems = spellListText.races.GetSize();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO RACES AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;

  while (currItem < totalListItems)
  {
    //raceType rt = (raceType)spellListText.GetSpellIndex(currItem);    
    RACE_ID raceID = spellListText.GetRace(currItem);    
    CString raceName = raceID.UniqueName();
    //displayText(18, shopY, raceData.GetRaceName(rt), White, shouldHighlight(currItem));
    displayText(18, shopY, raceName, White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    //InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(spellListText.GetSpellText(currItem), strlen(spellListText.GetSpellText(currItem)));
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(raceName, strlen(raceName));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateGenderMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  int totalListItems = spellListText.GetGenderCount();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO GENDERS AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;

  while (currItem < totalListItems)
  {
    //genderType type = (genderType)spellListText.GetSpellIndex(currItem);    
    genderType type = spellListText.GetGender(currItem);    
    CString genderName;
    genderName = GetGenderName(type);
    displayText(18, shopY, genderName, White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(genderName, strlen(genderName));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
void UpdateClassMenuScreen(void)
{
  // some class names can trigger color codes: 'Cleric/Ranger'
  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);

  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  int totalListItems = spellListText.GetClassCount();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO CLASSES AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;

  while (currItem < totalListItems)
  {
    //classType ct = (classType)spellListText.GetSpellIndex(currItem);        
    CLASS_ID classID = spellListText.GetClass(currItem);        
    CString className;
    className = classID.UniqueName();
    displayText(18, shopY, className , White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(className, strlen(className));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);

  menu.DisplayMenu();  
  DisplayTextAndFlip();  
}
void UpdateAlignmentMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  int totalListItems = spellListText.GetAlignmentCount();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO ALIGNMENTS AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;
  /*
  enum alignmentType { LawfulGood,    NeutralGood, ChaoticGood, 
                       LawfulNeutral, TrueNeutral, ChaoticNeutral,  
                       LawfulEvil,    NeutralEvil, ChaoticEvil };
  */

  while (currItem < totalListItems)
  {
    //alignmentType type = (alignmentType)spellListText.GetSpellIndex(currItem);    
    alignmentType type = spellListText.GetAlignment(currItem);    
    CString alignmentName;
    alignmentName = GetAlignmentName(type);
    displayText(18, shopY, alignmentName, White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(alignmentName, strlen(alignmentName));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
