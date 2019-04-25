/******************************************************************************
* Filename: globals.cpp
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
#include <string>

#include "globals.h"

// index 0 is the 'none' spell, 1..MAX_FRUA_SPELLS are genuine
SImportFRUASpell FRUASpells[MAX_FRUA_SPELLS];

ImportUAItem Items[MAX_FRUA_ITEMS];
ImportUAItems ItemClasses[MAX_FRUA_ITEM_CLASSES];

ImportFRUAMonster MonstersAndNPCs[MAX_FRUA_MONSTERS];

UALevelData Levels[MAX_FRUA_LEVELS];

//CString ArtFiles[MAX_FRUA_ART_FILES];

//CString SoundFiles[MAX_FRUA_SOUND_FILES];

const char *NoneString = "none";
const char *UnknownString = "unknown";
char DesignFolder[_MAX_PATH];
char FRUAFolder[_MAX_PATH];