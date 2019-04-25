/******************************************************************************
* Filename: FRUASoundFiles.cpp
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
#include "FRUASoundFiles.h"
#include "Utilities.h"
#include "FRUAStringTables.h"


void ReadSoundFiles(Json::Value &root)
{
   printf("Exporting sound files\n");

   // we're not going to decode any of the sound file contents.
   //
   // we're only going to make a list of all sound files in the design
   // and put that list in the json file for reference.

   //for (int i=0;i<MAX_FRUA_SOUND_FILES;i++)
   //   SoundFiles[i] = "";

   std::list<CString> SoundFileList;
   std::list<CString>::iterator SoundFileListIt;

   CFileFind finder1;
   CFileFind finder2;
   CFileFind finder3;
   //CString searchPath = "*.voc;*.mid;*.xmi";
   CString searchPath = DesignFolder;
   searchPath += "\\*.voc";

   BOOL found = finder1.FindFile(searchPath);

   //int fileCount = 0;

   while (found)
   {
      found = finder1.FindNextFile();

      if (finder1.IsDots())
         continue;
      if (finder1.IsDirectory())
         continue;

      SoundFileList.push_back(finder1.GetFileName());
      //SoundFiles[fileCount] = finder1.GetFileName();
      //fileCount++;
   }

   searchPath = DesignFolder;
   searchPath += "\\*.xmi";

   found = finder2.FindFile(searchPath);   
   while (found)
   {
      found = finder2.FindNextFile();

      if (finder2.IsDots())
         continue;
      if (finder2.IsDirectory())
         continue;

      SoundFileList.push_back(finder2.GetFileName());
      //SoundFiles[fileCount] = finder2.GetFileName();
      //fileCount++;
   }

   searchPath = DesignFolder;
   searchPath += "\\*.mid";

   found = finder3.FindFile(searchPath);   
   while (found)
   {
      found = finder3.FindNextFile();

      if (finder3.IsDots())
         continue;
      if (finder3.IsDirectory())
         continue;

      SoundFileList.push_back(finder3.GetFileName());
      //SoundFiles[fileCount] = finder3.GetFileName();
      //fileCount++;
   }

   if (SoundFileList.size() > 0)
   {
      //Json::Value &soundFileCollection = root["sounds"];
      //CString strIdKey;
      int count = 0;
      Json::Value soundArray(Json::arrayValue);

      //for (int i=0; i<fileCount; i++)
      SoundFileListIt = SoundFileList.begin();
      while (SoundFileListIt != SoundFileList.end())
      {
         Json::Value sound;         
         sound["name"] = CStringOrEmptyString((*SoundFileListIt));
         //sound["name"] = CStringOrEmptyString(SoundFiles[i]);
         soundArray.append(sound);

         ++SoundFileListIt;
      }

      root["sounds"] = soundArray;
      root["sounds_count"] = IntToString(SoundFileList.size());
   }
   else
   {
      Json::Value soundArray(Json::arrayValue);
      root["sounds"] = soundArray;
      root["sounds_count"] = "0";
   }
}