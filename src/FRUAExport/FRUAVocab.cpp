/******************************************************************************
* Filename: FRUAVocab.cpp
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
#include "FRUAVocab.h"
#include "Utilities.h"
#include "FRUAStringTables.h"

void InsertFRUAVocabulary(Json::Value &root)
{
   printf("Exporting FRUA vocabulary\n");

   // We will insert much of the FRUA vocabulary srings with
   // their default table offsets. The art will include default filename.

   Json::Value artArray(Json::arrayValue);

   // skip the first index='none' and last index='none'
   for (int i=1; i<(GetNumBigSmallSpritePics()-1); i++)
   {
      int picIndex = FRUATranslateBigSmallSpritePicIndexToPicNum(i);

      Json::Value art;                

      art["name"] = CStringOrEmptyString(FRUAGetBigSmallSpritePicText(picIndex));
      art["index"] = IntToString(picIndex);
      art["filename"] = CStringOrEmptyString(FRUAGetBigSmallSpritePicOriginalFilenameText(picIndex));
      artArray.append(art);
   }

   root["Vocab_BigSmallSpritePics"] = artArray;
   root["Vocab_BigSmallSpritePics_Count"] = IntToString(GetNumBigSmallSpritePics());
}


