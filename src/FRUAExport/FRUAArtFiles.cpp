/******************************************************************************
* Filename: FRUAArtFiles.cpp
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
#include <list>

#include "globals.h"
#include "FRUAArtFiles.h"
#include "Utilities.h"
#include "FRUAStringTables.h"

void ReadArtTlb(Json::Value &root)
{
   printf("Exporting art files\n");

   // we're not going to decode any of the art file contents.
   //
   // we're only going to make a list of all art files in the design
   // and put that list in the json file for reference.

   //for (int i=0;i<MAX_FRUA_ART_FILES;i++)
   //   ArtFiles[i] = "";

   CFileFind finder;
   CString searchPath = DesignFolder; 
   searchPath += "\\*.tlb";

   BOOL found = finder.FindFile(searchPath);

   //int artFileCount = 0;
   std::list<CString> ArtFileList;
   std::list<CString>::iterator ArtFileListIt;

   while (found) 
   {
      found = finder.FindNextFile();

      if (finder.IsDots())
         continue;
      if (finder.IsDirectory())
         continue;

      ArtFileList.push_back(finder.GetFileName());

      //ArtFiles[artFileCount] = finder.GetFileName();
      //artFileCount++;
   }

   //if (artFileCount > 0)
   if (ArtFileList.size() > 0)
   {
      int count = 0;
      Json::Value artArray(Json::arrayValue);

      //for (int i=0; i<artFileCount; i++)
      ArtFileListIt = ArtFileList.begin();
      while (ArtFileListIt != ArtFileList.end())
      {
         Json::Value art;       
         //art["name"] = CStringOrEmptyString(ArtFiles[i]);
         art["name"] = CStringOrEmptyString((*ArtFileListIt));
         artArray.append(art);
         count++;

         ++ArtFileListIt;
      }

      root["art"] = artArray;
      root["art_Count"] = IntToString(count);
   }
   else
   {
      Json::Value artArray(Json::arrayValue);
      root["art"] = artArray;
      root["art_Count"] = "0";
   }
}


