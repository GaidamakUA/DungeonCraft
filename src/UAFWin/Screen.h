/******************************************************************************
* Filename: Screen.h
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
#ifndef DGNSCRN_H_
#define DGNSCRN_H_

class GPDL;

void blitView(DWORD flags, BOOL drawViewportPic, PIC_DATA *pPic);
BOOL Splash();
BOOL Credits();

void UpdateSmall(void);
void UpdateBig(void);

void DisplayTextAndFlip(void);
void UpdateStartScreen(void);
void UpdateIconPickScreen(void);
void UpdateSmallPicScreen(void);
void UpdateAdventureScreen(void);
void UpdateMoveScreen(void);
void UpdateTextEventScreen(void);
void UpdateQuestionYesNoScreen(void);
void UpdateVaultScreen(void);
void UpdateGiveTreasureScreen(void);
void UpdateViewCharacterScreen(void);
void UpdateCombatViewCharacterScreen(void);
void UpdateEncampScreen(void);
void UpdateTransferMenuScreen(void);
void UpdateTavernMenuScreen(void);
void UpdateGiveDamageScreen(void);
void UpdateGainExpScreen(void);
void UpdateHealPartyScreen(void);
void UpdateWhoPaysScreen(void);
void UpdatePassTimeScreen(void);
void UpdateSmallTownScreen(void);
void UpdateAddNPCScreen(void);
void UpdateRemoveNPCScreen(void);
void UpdateNPCSaysScreen(void);
void UpdateShopScreen(void);
void UpdateSpecialItemKeyScreen(void);
void UpdateGPDLScreen(GPDL *pGPDL, PIC_DATA *pPic);
void UpdateTakePartyItemsScreen(void);
void UpdateTrainHallWelcomeScreen(void);
void UpdateWhoTriesScreen(void);
void UpdateTempleScreen(void);
void UpdateTempleGiveScreen(void);
void UpdatePasswordScreen(void);
void UpdateQuestionListScreen(void);
void UpdateQuestionButtonScreen(void);
void UpdateEncounterScreen(void);
void UpdateQuestScreen(void);
void UpdateGuidedTourScreen(void);
void UpdateGuidedTourMoveScreen(void);
void UpdateSpeedCombatScreen(void);
void UpdateSpeedScreen(void);
void UpdateCombatRoundDelayScreen(void);
void UpdateCombatTimeDelayMsg(int dude=-1);
void UpdateSpriteAnimateScreen(int dude);
void UpdateCombatMoveScreen(void);
void UpdateCombatAimManualScreen(void);
void UpdateCombatAimScreen(void);
void UpdateCombatMainMenuScreen(void);
void UpdateHumanChangeClassScreen(void);
void UpdateTradeTakerSelectScreen(void);
void UpdateCombatTradeTakerSelectScreen(void);
void UpdateAlterGameScreen(void);
void UpdateAlterOrderScreen(void);
void UpdatePartyDiedScreen(void);
void UpdateCombatResultsScreen(void);
void UpdateMagicMenuScreen(void);
void UpdateScribeMenuScreen(void);
void UpdateCombatMagicMenuScreen(void);
void UpdateRestMenuScreen(void);
void UpdateAppraiseSelectMenuScreen(void);
void UpdateAppraiseEvaluateMenuScreen(void);
void UpdateAskYesNoMenuScreen(void);
void UpdateTargetSelectNonCombatScreen(void);
void UpdateRandomEventScreen(void);
void UpdateDisplayPartyJournalScreen(void);
void UpdateMovieBigPicWindow(void);
void UpdateMovieSmallPicWindow(void);
void UpdateRaceMenuScreen(void);
void UpdateGenderMenuScreen(void);
void UpdateClassMenuScreen(void);
void UpdateAlignmentMenuScreen(void);
#endif
