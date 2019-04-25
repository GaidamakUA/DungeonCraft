/******************************************************************************
* Filename: FRUAGameDat.cpp
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
#include "FRUAGameDat.h"
#include "FRUAStructs.h"
#include "Utilities.h"
#include "FRUAStringTables.h"


bool OpenGameDat(const char *rootFolder, Json::Value &root)
{
   std::string designFilename = rootFolder; 
   designFilename += "\\game001.dat";

   char TextBuffer[50];
   unsigned int FourBytes;
   unsigned char  OneByte;
   std::string strTemp;

   CFile file;   
   bool success = false;

   try
   {
      if (file.Open(designFilename.c_str(), CFile::modeRead))
      {
         // name of design
         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 32) == 32)
            root["global"]["design_name"] = _strupr(TextBuffer);

         if (file.Read(&FourBytes, 4) == 4)
            root["global"]["start_experience"] = IntToString(FourBytes);
         if (file.Read(&FourBytes, 4) == 4) 
            root["global"]["start_platinum"] = IntToString(FourBytes);
         if (file.Read(&FourBytes, 4) == 4)
            root["global"]["start_gem"] = IntToString(FourBytes);
         if (file.Read(&FourBytes, 4) == 4)
            root["global"]["start_jewelry"] = IntToString(FourBytes);
         if (file.Read(&OneByte, 1) == 1)
            root["global"]["start_level"] = IntToString(OneByte);
         if (file.Read(&OneByte, 1) == 1)
            root["global"]["start_entrypoint"] = IntToString(OneByte);

         if (file.Read(&OneByte, 1) == 1)
         {
            switch (OneByte)
            {
            case 0: root["global"]["start_equipment_level"] = "none"; break;
            case 1: root["global"]["start_equipment_level"] = "poor"; break;
            case 2: root["global"]["start_equipment_level"] = "modest"; break;
            case 3: root["global"]["start_equipment_level"] = "average"; break;
            case 4: root["global"]["start_equipment_level"] = "prosperous"; break;
            case 5: root["global"]["start_equipment_level"] = "prosperous +1"; break;
            case 6: root["global"]["start_equipment_level"] = "prosperous +2"; break;
            case 7: root["global"]["start_equipment_level"] = "prosperous +3"; break;
            case 8: root["global"]["start_equipment_level"] = "prosperous +4"; break;
            default: root["global"]["start_equipment_level"] = "none"; break;
            }
         }

         if (file.Read(&OneByte, 1) == 1)
         {
            // unused byte, assignment quiets compiler
         }

         // All text in FRUA is upper case...

         // special key names 1-8
         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_001"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_002"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_003"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_004"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_005"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_006"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_007"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_key_008"] = _strupr(TextBuffer);


         // special item names 1-12
         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_001"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_002"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_003"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_004"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_005"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_006"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_007"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_008"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_009"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_010"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_011"] = _strupr(TextBuffer);

         memset(TextBuffer, 0, sizeof(TextBuffer));
         if (file.Read(TextBuffer, 16) == 16)
            root["global"]["special_item_012"] = _strupr(TextBuffer);

         // design password
         if (file.Read(TextBuffer, 15) == 15)
            root["global"]["password"] = TextBuffer;      

         success = true;
      }
   }
   catch (...)
   {
      printf("Exception encountered while exporting game001.dat\n");
   }

   if (file.m_hFile != CFile::hFileNull)
      file.Close();

   return success;
}

void ReadGameDat(Json::Value &root)
{
   printf("Exporting global design data\n");

   if (!OpenGameDat(DesignFolder, root))
   {
      if (!OpenGameDat(".", root))
      {
         printf("Game001.dat NOT IMPORTED - UNABLE TO OPEN FILE\n");
      }
   }
}
