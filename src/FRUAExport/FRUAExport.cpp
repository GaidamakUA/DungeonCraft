/******************************************************************************
* Filename: FRUAExport.cpp
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

// FRUAExport.cpp : Defines the entry point for the console application.
//


//#include "stdafx.h"
#include <string>

//#include "Externs.h"


#include "json.h"
#include "FRUAStringTables.h"
#include "FRUAStructs.h"
#include "Utilities.h"
#include "globals.h"

#include "FRUAItems.h"
#include "FRUAGameDat.h"
#include "FRUASpells.h"
#include "FRUAMonster.h"
#include "FRUAArtFiles.h"
#include "FRUASoundFiles.h"
#include "FRUALevels.h"
#include "FRUAVocab.h"

void ShowHelp()
{
   printf("\n");
   printf("FRUAExport\n");
   printf("\t-d <design folder.dsn> - design folder to import\n");
   printf("\t-u <UA folder> - specify root folder for UA, default is current folder\n");
   printf("\t-h - show help and exit\n");
   printf("\n");
   printf("This executable should be located in the root UA folder\n");
   printf("\n");
   printf("Example Usage: FRUAExport -d default.dsn\n");
   printf("\n");
   printf("If you are exporting a hacked design then ensure the hacks are\n");
   printf("applied before running this program\n");
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
   // if you set this to true, then make sure your folder has ALL of the monst*.dat files!
   bool writeDefaultFiles = false;
#endif

   printf("FRUA Export\n");
   printf("\n");
   printf("Reads existing design files created by FRUA and\n");
   printf("exports the data to a text file in JSON format\n");
   printf("\n");
   printf("Many thanks to the folks dedicated to decoding the bits\n");
   printf("in the FRUA files and documenting them in the hackdocs!\n");
   printf("\n");
  
   memset(DesignFolder, 0, sizeof(DesignFolder));
   memset(FRUAFolder, 0, sizeof(FRUAFolder));

#ifdef _DEBUG
   // RBT: this will force writing new default files
    //writeDefaultFiles = true;
    //printf("READING EXISTING FILES AND CREATING FRUAExport DEFAULT FILES\n");
#endif

   for (int i=1; i < argc; i++)
   {
      if ((strncmp(argv[i], "-h", 2) == 0) || (strncmp(argv[i], "-H", 2) == 0))
      {
         ShowHelp();
         exit(0);
      }

      if ((strncmp(argv[i], "-d", 2) == 0) || (strncmp(argv[i], "-D", 2) == 0))
      {
         i++;
         if (i < argc)
         {            
            strcpy(DesignFolder, argv[i]);
         }
      }
      else if ((strncmp(argv[i], "-u", 2) == 0) || (strncmp(argv[i], "-U", 2) == 0))
      {
         i++;
         if (i < argc)
         {            
            strcpy(FRUAFolder, argv[i]);
         }
      }
   }

   // if no args provided on cmd line, then show help just
   // in case we have a new user
   if (strlen(DesignFolder) == 0)
   {
      printf("DESIGN FOLDER NOT SPECIFIED!\n");
      ShowHelp();
      exit(0);
   }

   // if the FRUA root folder is not supplied then
   // assume the current folder
   if (strlen(FRUAFolder) == 0)
      strcpy(FRUAFolder, ".");

   memset(Items, 0, sizeof(Items));
   memset(ItemClasses, 0, sizeof(ItemClasses));  
   memset(FRUASpells, 0, sizeof(FRUASpells));
   memset(MonstersAndNPCs, 0, sizeof(MonstersAndNPCs));
   memset(Levels, 0, sizeof(Levels));
   
   printf("Design Folder: \'%s\'\n", DesignFolder);
   printf("    UA Folder: \'%s\'\n", FRUAFolder);

   // this will hold the entire design hierarchy
   Json::Value root;

   Json::Value exportVersionArray(Json::arrayValue);

   Json::Value exportVersionValue;
   exportVersionValue["Build Date"] = __DATE__;
   CString strTimestamp(__TIME__);
   strTimestamp += " EST";
   exportVersionValue["Build Time"] = (const char*)strTimestamp;
   exportVersionArray.append(exportVersionValue);
   root["FRUAExport"] = exportVersionArray;

   ReadGameDat(root);
   ReadSpells(root);
   ReadItemsDat(root);  
   ReadMonstersDat(root);
   ReadArtTlb(root);
   ReadSoundFiles(root);
   ReadLevelsDat(root);
   InsertFRUAVocabulary(root);
   
#ifdef _DEBUG
   if (writeDefaultFiles)
   {
      WriteItemDefaultsFile();
      WriteMonsterDefaultsFile();
      WriteSpellDefaultsFile();
      return 0;
   }
#endif

   // everything has been read from the design and converted to json, now format to a string and write it out 
   Json::StyledWriter writer;
   std::string formattedOutput = writer.write( root );

   // make sure this key value is set
   if (root["global"]["design_name"].type() == Json::stringValue)
   {
      // FRUA apparently allows characters in the design name that are not valid for Windows filenames.
      // Switch to using the input design folder name.
      //CString strOutputFile = root["global"]["design_name"].asCString();
      CString strOutputFile = DesignFolder;
      strOutputFile.Trim();
      //if (strOutputFile.GetLength() == 0)
      //   strOutputFile = "game001";
   
      strOutputFile += ".json";

      printf("Writing json data to \'%s\'\n", (const char*)strOutputFile);

      CFile outputFile;
      if (outputFile.Open(strOutputFile, CFile::modeWrite|CFile::modeCreate))
      {
         outputFile.Write(formattedOutput.c_str(), formattedOutput.size());
         outputFile.Close();
      }
      else
      {
         printf("failed to open output file\n");
      }
   }
   else
   {
      printf("Error while parsing design, no json file created\n");
   }
   return 1;
}

