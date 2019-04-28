#ifndef __GLOBALS_H__
#define __GLOBALS_H__
/******************************************************************************
* Filename: globals.h
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
#include "FRUAStructs.h"

// force 1 byte packing alignment for the FRUA data structs
#include <pshpack1.h>

struct SImportFRUASpell
{
   enum { MAX_SPELL_NAME=50 };
   char spellName[MAX_SPELL_NAME+1];
   ImportUASpell spellData;
};

// index 0 is the 'none' spell, 1..MAX_FRUA_SPELLS are genuine
extern SImportFRUASpell FRUASpells[MAX_FRUA_SPELLS];

extern ImportUAItem Items[MAX_FRUA_ITEMS];
extern ImportUAItems ItemClasses[MAX_FRUA_ITEM_CLASSES];

extern ImportFRUAMonster MonstersAndNPCs[MAX_FRUA_MONSTERS];

extern UALevelData Levels[MAX_FRUA_LEVELS];

// restore default packing alignment
#include <poppack.h>



//extern CString ArtFiles[MAX_FRUA_ART_FILES];
//extern CString SoundFiles[MAX_FRUA_SOUND_FILES];

extern const char *NoneString;
extern const char *UnknownString;
extern char DesignFolder[_MAX_PATH];
extern char FRUAFolder[_MAX_PATH];

#endif 
