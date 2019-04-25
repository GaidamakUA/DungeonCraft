#ifndef __STRING_TABLES_H__
#define __STRING_TABLES_H__
/******************************************************************************
* Filename: FRUAStringTables.h
* Copyright (c) 2013, UAF Development Team (email CocoaSpud@hotmail.com)
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

#include "stdafx.h"

const char *FRUAGetItemNameText(int nameCode);
const char *FRUAGetSpellText(int spellCode);
const char *FRUAGetSpecAbText(int specAbCode);
const char *FRUAGetMonsterNameText(int monsterCode);
const char *FRUAGetSoundNameText(int soundCode);
const char *FRUAGetEventTypeText(int eventType);
const char *FRUAGetWallSlotArtName(int slot);
const char *FRUAGetDungeonCombatSlotArtName(int slot);
const char *FRUAGetWildernessCombatSlotArtName(int slot);
const char *FRUAGetOverlandWildernessSlotArtName(int slot);
const char *FRUAGetBackgroundSlotArtName(int slot);
const char *FRUAGetWallBlockageText(int blockage);
const char *FRUAGetClassText(int classCode);
const char *FRUAGetRaceText(int raceCode);
const char *FRUAGetDirectionText(int dirCode);
const char *FRUAGetCostModifierText(int value);

const char *FRUAGetBigSmallSpritePicText(int picNum);
const char *FRUAGetBigSmallSpritePicOriginalFilenameText(int picNum);
const int FRUATranslateBigSmallSpritePicIndexToPicNum(int index);
int GetNumBigSmallSpritePics();

#endif // __STRING_TABLES_H__