/******************************************************************************
* Filename: Dbutils.cpp
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

#include "..\Shared\stdafx.h"
//#include "..\Shared\Version.h"
#include "resource.h"

#include <afxpriv.h>
#include "UAFWinEd.h"
#include "items.h"
#include "monster.h"
#include "spell.h"
#include "class.h"
#include "itemdb.h"
#include "DbUtils.h"
#include "ConfigFile.h"
#include "ItemEditor.h"
#include "MonsterEditor.h"
#include "SpellEditor.h"
#include "ImportWarning.h"
#include "EditAttributes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbUtils dialog

CDbUtils::CDbUtils(CWnd* pParent /*=NULL*/)
	: CDialog(CDbUtils::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDbUtils)
	m_SpellSize = _T("");
	m_MonsterSize = _T("");
	m_ItemSize = _T("");
  m_SpecAbSize = _T("");
	m_ClassSize = _T("");
	m_BaseclassSize = _T("");
	m_RaceSize = _T("");
	m_AbilitySize = _T("");
	//}}AFX_DATA_INIT
}


void CDbUtils::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbUtils)
	DDX_Text(pDX, IDC_SPELLSIZE, m_SpellSize);
	DDX_Text(pDX, IDC_MONSTERSIZE, m_MonsterSize);
	DDX_Text(pDX, IDC_ITEMSIZE, m_ItemSize);
	DDX_Text(pDX, IDC_SPECABSIZE, m_SpecAbSize);
	DDX_Text(pDX, IDC_CLASSSIZE, m_ClassSize);
	DDX_Text(pDX, IDC_BASECLASSSIZE, m_BaseclassSize);
	DDX_Text(pDX, IDC_RACESIZE, m_RaceSize);
	DDX_Text(pDX, IDC_ABILITYSIZE, m_AbilitySize);
	DDX_Check(pDX, IDC_TestMode, m_testMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDbUtils, CDialog)
	//{{AFX_MSG_MAP(CDbUtils)
  ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CONVERTITEMDB, OnConvertitemdb)
	ON_BN_CLICKED(IDC_CONVERTMONSTERDB, OnConvertmonsterdb)
	ON_BN_CLICKED(IDC_CONVERTSPELLDB, OnConvertspelldb)
	ON_BN_CLICKED(IDC_EXPORTITEMDB, OnExportitemdb)
	ON_BN_CLICKED(IDC_EXPORTBASECLASSDB, OnExportbaseclassdb)
	ON_BN_CLICKED(IDC_EXPORTMONSTERDB, OnExportmonsterdb)
	ON_BN_CLICKED(IDC_EXPORTSPELLDB, OnExportspelldb)
	ON_BN_CLICKED(IDC_EXPORTCLASSDB, OnExportclassdb)
	ON_BN_CLICKED(IDC_EXPORTRACEDB, OnExportracedb)
	ON_BN_CLICKED(IDC_LOADDEFITEMS, OnLoaddefitems)
	ON_BN_CLICKED(IDC_LOADDEFMONSTERS, OnLoaddefmonsters)
	ON_BN_CLICKED(IDC_LOADDEFSPELLS, OnLoaddefspells)
	ON_BN_CLICKED(IDC_EDITITEMTEXT, OnEdititemtext)
	ON_BN_CLICKED(IDC_EDITMONSTERTEXT, OnEditmonstertext)
	ON_BN_CLICKED(IDC_EDITSPELLTEXT, OnEditspelltext)
	ON_BN_CLICKED(IDC_CONVERTCLASSDB, OnConvertclassdb)
	ON_BN_CLICKED(IDC_CONVERTBASECLASSDB, OnConvertbaseclassdb)
	ON_BN_CLICKED(IDC_CONVERTRACEDB, OnConvertracedb)
	ON_BN_CLICKED(IDC_CONVERTABILITYDB, OnConvertabilitydb)
	ON_BN_CLICKED(IDC_EXPORTABILITY, OnExportabilitydb)
	ON_BN_CLICKED(IDC_IMPORTALLTEXT, OnImportalltext)
	ON_BN_CLICKED(IDC_ExportGlobalSettings, OnExportGlobalSettings)
	ON_BN_CLICKED(IDC_ImportGlobalSettings, OnImportGlobalSettings)
	ON_BN_CLICKED(IDC_ExportCharacters, OnExportCharacters)
	ON_BN_CLICKED(IDC_ImportCharacters, OnImportCharacters)

#ifdef OLD_SPECIAL_ABILITIES
#else
	ON_BN_CLICKED(IDC_EXPORTSPECABDB, OnExportspecabdb)
	ON_BN_CLICKED(IDC_IMPORTSPECABDB, OnImportSpecAbDB)
#endif
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbUtils message handlers

bool CDbUtils::LiveDangerously(void)
{
   CImportWarning dlg;
   int answer;
   dlg.m_warning = "What you are doing can be very dangerous.  Replacing\r\n "
			   "a database can cause references to the database to\r\n"
			   "become useless.  For example, the starting equipment\r\n"
			   "for a Cleric specifies equipment in the Items Database\r\n"
			   "Rearranging the Items Database can cause all the\r\n"
         "starting equipment for a cleric to be totally different\r\n"
         "because the references are to locations within the\r\n"
         "database rather than to the names of the items in the\r\n"
         "database.  If you  know what you are doing or like\r\n"
         "adventure, then enter the number you see below and press 'OK'.";
   if (dlg.DoModal() != IDOK)
   {
	   MsgBoxInfo("That is a wise choice");
	   return false;
   };
   if ( (sscanf(dlg.m_numeric,"%d",&answer) != 1) || (answer!=dlg.m_i))
   {
       MsgBoxInfo("Wrong answer....sorry");
       return false;
   };
   return true;
}


void CDbUtils::OnLoaddefitems() 
{
   CString filename;
   filename = rte.DataDir()+ITEM_DB_NAME;
   if (GetFilename(filename, "DAT", TRUE))
   {   
      AfxGetApp()->DoWaitCursor(1);
      loadData(itemData, filename);   
      AfxGetApp()->DoWaitCursor(-1);

     if (itemData.GetCount() == 0)
     {
        MsgBoxError("Operation Failed!", "Dat File Error");
        TRACE("Empty Item DB\n");
     }
     else 
     { 
        SET_MODIFIED;
        CString temp;
        temp.Format("Num Items in DB = %i\n", itemData.GetCount());
        TRACE(temp);
        MsgBoxInfo("Operation Successful", "Dat File Message");
     }
     m_ItemSize.Format("%i", itemData.GetCount());
     UpdateData(FALSE);
   }
}

void CDbUtils::OnLoaddefmonsters() 
{
   CString filename;
   filename = rte.DataDir()+MONSTER_DB_NAME;
   if (GetFilename(filename, "DAT", TRUE))
   {   
     AfxGetApp()->DoWaitCursor(1);
     loadData(monsterData, filename);
     AfxGetApp()->DoWaitCursor(-1);

     if (monsterData.GetCount() <= 0)
     {
        MsgBoxError("Operation Failed!", "Dat File Error");
        TRACE("Empty Monster DB\n");
     }
     else 
     {
        SET_MODIFIED;
        CString temp;
        temp.Format("Num Monsters in DB = %i\n", monsterData.GetCount());
        TRACE(temp);
        MsgBoxInfo("Operation Successful", "Dat File Message");
     }   

     m_MonsterSize.Format("%i", monsterData.GetCount());
     UpdateData(FALSE);
   }
}

void CDbUtils::OnLoaddefspells() 
{
   CString filename;
   filename = rte.DataDir()+SPELL_DB_NAME;
   if (GetFilename(filename, "DAT", TRUE))
   {   
     AfxGetApp()->DoWaitCursor(1);
     loadData(spellData, filename);
     AfxGetApp()->DoWaitCursor(-1);

     if (spellData.GetCount() <= 0)
     {
       MsgBoxError("Operation Failed!", "Dat File Error");
       TRACE("Empty Spell DB\n");
     }
     else 
     {
       SET_MODIFIED;
       CString temp;
       temp.Format("Num Spells in DB = %i\n", spellData.GetCount());
       TRACE(temp);
       MsgBoxInfo("Operation Successful", "Dat File Message");
     }

     m_SpellSize.Format("%i", spellData.GetCount());
     UpdateData(FALSE);
   }
}

BOOL CDbUtils::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_ItemSize.Format("%i", itemData.GetCount());
   m_MonsterSize.Format("%i", monsterData.GetCount());
   m_SpellSize.Format("%i", spellData.GetCount());
   m_ClassSize.Format("%i", classData.GetCount());
   m_BaseclassSize.Format("%i", baseclassData.GetCount());
   m_RaceSize.Format("%i", raceData.GetCount());
   m_AbilitySize.Format("%i", abilityData.GetCount());
   m_SpecAbSize.Format("%i", specialAbilitiesData.GetCount());

   UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDbUtils::OnConvertitemdb() 
{
   CString filename;

   if (!LiveDangerously()) return;

   filename = rte.DataDir()+ITEM_CONFIG_NAME;
   if (GetFilename(filename, "TXT", TRUE))
   {
     SET_MODIFIED;
     
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadItemDB(filename, &itemData);
     AfxGetApp()->DoWaitCursor(-1);
     CString msg;
     switch (status)
     {
     case 0:
       msg = "Items database file read successfully";
       break;
     case 1:
       msg = "Minor errors in Items database";
       break;
     case -1:
       msg = "Cannot read Items database file";
       break;
     case -2:
       msg = "Items database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading items database file";
       break;
     default:
       msg = "Unknown error reading items database file";
       break;
     };   

     WriteDebugString("%s\n", msg);
     MsgBoxInfo(msg, "Database Status");

     m_ItemSize.Format("%i", itemData.GetCount());
     UpdateData(FALSE);
   }
}

void CDbUtils::OnConvertmonsterdb() 
{
   CString filename;

   if (!LiveDangerously()) return;
   filename = rte.DataDir()+MONSTER_CONFIG_NAME;

   if (GetFilename(filename, "TXT", TRUE))
   {
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadMonsterDB(filename, &monsterData);
     AfxGetApp()->DoWaitCursor(-1);
     CString msg;
     switch (status)
     {
     case 0:
       msg = "Monsters database file read successfully";
       break;
     case 1:
       msg = "Minor errors in Monsters database";
       break;
     case -1:
       msg = "Cannot read Monsters database file";
       break;
     case -2:
       msg = "Monsters database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading monsters database file";
       break;
     default:
       msg = "Unknown error reading monsters database file";
       break;
     };

     WriteDebugString("%s\n", msg);
     MsgBoxInfo(msg, "Database Status");

     m_MonsterSize.Format("%i", monsterData.GetCount());
     UpdateData(FALSE);
   }
}

void CDbUtils::OnConvertspelldb() 
{
   CString filename;

   if (!LiveDangerously()) return;

   filename = rte.DataDir()+SPELL_CONFIG_NAME;
   if (GetFilename(filename, "TXT", TRUE))
   {
     SET_MODIFIED;
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadSpellDB(filename, &spellData);
     AfxGetApp()->DoWaitCursor(-1);

     CString msg;
     switch (status)
     {
     case 0:
       msg = "Spell database read successfully";
       break;
     case 1:
       msg = "Minor errors in Spells database";
       break;
     case -1:
       msg = "Cannot read Spells database file";
       break;
     case -2:
       msg = "Spells database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading spells database file";
       break;
     default:
       msg = "Unknown error reading spells database file";
       break;
     };

     WriteDebugString("%s\n", msg);
     MsgBoxInfo(msg, "Database Status");
 
     m_SpellSize.Format("%i", spellData.GetCount());
     UpdateData(FALSE);
   }
}

void CDbUtils::OnConvertclassdb() 
{
   CString filename;
   filename = rte.DataDir()+CLASS_CONFIG_NAME;
   if (GetFilename(filename, "TXT", TRUE))
   {
     SET_MODIFIED;
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadClassDB(filename, &classData);
     AfxGetApp()->DoWaitCursor(-1);

     CString msg;
     switch (status)
     {
     case 0:
       msg = "Class database read successfully";
       break;
     case 1:
       msg = "Minor errors in Class database";
       break;
     case -1:
       msg = "Cannot read Class database file";
       break;
     case -2:
       msg = "Class database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading Class database file";
       break;
     default:
       msg = "Unknown error reading Class database file";
       break;
     };

     WriteDebugString("%s\n", msg);
     MsgBoxInfo(msg, "Database Status");
 
     m_ClassSize.Format("%i", classData.GetCount());
     UpdateData(FALSE);
   }
}


void CDbUtils::OnExportitemdb() 
{
   CString filename;
   UpdateData();
   filename = rte.DataDir()+ITEM_CONFIG_NAME;
   if (GetFilename(filename, "TXT", FALSE))
   {
     CONFIG_FILE ConfFile;
     AfxGetApp()->DoWaitCursor(1);
     BOOL success = (WriteItemDB(filename, &itemData, &ConfFile)==0);
     if (m_testMode)
     {
       ITEM_DATA_TYPE itemDataTest;
       if (ReadItemDB(filename, &itemDataTest, &ConfFile) == 0)
       {
         itemDataTest.version = itemData.version;
         if (!(itemDataTest == itemData))
         {
           MsgBoxError("Data Mismatch when importing the file we wrote", "Export/Import error");
         };
       }
       else
       {
         MsgBoxError("Error reading back the file we wrote", "Export/Import error"); 
       };
     };
     AfxGetApp()->DoWaitCursor(-1);

     if (!success)
        MsgBoxError("Operation Failed!", "Database Error");
     else
        MsgBoxInfo("Operation Successful", "Database Message");
   }
   m_testMode = FALSE;
   UpdateData(FALSE);
}

void CDbUtils::OnExportmonsterdb() 
{
   CString filename;
   UpdateData();
   filename = rte.DataDir()+MONSTER_CONFIG_NAME;
   if (GetFilename(filename, "TXT", FALSE))
   {
     CONFIG_FILE ConfFile;
     AfxGetApp()->DoWaitCursor(1);
     BOOL success = (WriteMonsterDB(filename, &monsterData, &ConfFile)==0);
     if (m_testMode)
     {
       MONSTER_DATA_TYPE monsterDataTest;
       if (ReadMonsterDB(filename, &monsterDataTest, &ConfFile) == 0)
       {
         monsterDataTest.version = monsterData.version;
         if (!(monsterDataTest == monsterData))
         {
           MsgBoxError("Data Mismatch when importing the file we wrote", "Export/Import error");
         };
       }
       else
       {
         MsgBoxError("Error reading back the file we wrote", "Export/Import error"); 
       };
     };

     AfxGetApp()->DoWaitCursor(-1);

     if (!success)
        MsgBoxError("Operation Failed!", "Database Error");
     else
        MsgBoxInfo("Operation Successful", "Database Message");
   }
   m_testMode = FALSE;
   UpdateData(FALSE);
}

void CDbUtils::OnExportbaseclassdb() 
{
   CString filename;
   UpdateData();
   filename = rte.DataDir()+BASE_CLASS_CONFIG_NAME;
   if (GetFilename(filename, "TXT", FALSE))
   {
     CONFIG_FILE ConfFile;
     AfxGetApp()->DoWaitCursor(1);
     BOOL success = (WriteBaseclassDB(filename, &baseclassData, &ConfFile)==0);

     if (m_testMode)
     {
       BASE_CLASS_DATA_TYPE baseclassDataTest;
       if (ReadBaseclassDB(filename, &baseclassDataTest, &ConfFile) == 0)
       {
         //baseclassDataTest.version = baseclassData.version;
         if (!(baseclassDataTest == baseclassData))
         {
           MsgBoxError("Data Mismatch when importing the file we wrote", "Export/Import error");
         };
       }
       else
       {
         MsgBoxError("Error reading back the file we wrote", "Export/Import error"); 
       };
     };

     
     
     AfxGetApp()->DoWaitCursor(-1);

     if (!success)
        MsgBoxError("Operation Failed!", "Database Error");
     else
        MsgBoxInfo("Operation Successful", "Database Message");
   }
   m_testMode = FALSE;
   UpdateData(FALSE);
}

#ifdef OLD_SPECIAL_ABILITIES
#else
void CDbUtils::OnExportspecabdb() 
{
   CString filename;
   UpdateData();
   filename = rte.DataDir()+SPECAB_CONFIG_NAME;
   if (GetFilename(filename, "TXT", FALSE))
   {
     CONFIG_FILE ConfFile;
     AfxGetApp()->DoWaitCursor(1);
     BOOL success = (WriteSpecAbDB(filename, &specialAbilitiesData, &ConfFile)==0);

     
     if (m_testMode)
     {
       A_SPECABILITY_DEF_L specAbDataTest;
       if (ReadSpecialAbilitiesDB(filename, &specAbDataTest, &ConfFile) == 0)
       {
         //specAbDataTest.version = specialAbilitiesData.version;
         if (!(specAbDataTest == specialAbilitiesData))
         {
           MsgBoxError("Data Mismatch when importing the file we wrote", "Export/Import error");
         };
       }
       else
       {
         MsgBoxError("Error reading back the file we wrote", "Export/Import error"); 
       };
     };
     
     
     AfxGetApp()->DoWaitCursor(-1);

     if (!success)
        MsgBoxError("Operation Failed!", "Database Error");
     else
        MsgBoxInfo("Operation Successful", "Database Message");
   }
   m_testMode = FALSE;
   UpdateData(FALSE);
}

void CDbUtils::OnImportSpecAbDB() 
{
   CString filename;

   filename = rte.DataDir()+SPECAB_CONFIG_NAME;

   if (!LiveDangerously()) return;

   if (GetFilename(filename, "TXT", TRUE))
   {
     SET_MODIFIED;
     
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadSpecialAbilitiesDB(filename, &specialAbilitiesData);
     AfxGetApp()->DoWaitCursor(-1);
     CString msg;
     switch (status)
     {
     case 0:
       msg = "Special Abilities database file read successfully";
       break;
     case 1:
       msg = "Minor errors in Special Abilities database";
       break;
     case -1:
       msg = "Cannot read Special Abilities database file";
       break;
     case -2:
       msg = "Special Abilities database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading Special Abilities database file";
       break;
     default:
       msg = "Unknown error reading Special Abilities database file";
       break;
     };   

     WriteDebugString("%s\n", msg);
     MsgBoxInfo(msg, "Database Status");

     m_SpecAbSize.Format("%i", specialAbilitiesData.GetCount());
     UpdateData(FALSE);
   }
};

#endif

void CDbUtils::OnExportspelldb() 
{
   CString filename;
   UpdateData();
   filename = rte.DataDir()+SPELL_CONFIG_NAME;
   if (GetFilename(filename, "TXT", FALSE))
   {
     CONFIG_FILE ConfFile;
     AfxGetApp()->DoWaitCursor(1);
     BOOL success = (WriteSpellDB(filename, &spellData, &ConfFile)==0);

     if (m_testMode)
     {
       SPELL_DATA_TYPE spellDataTest;
       if (ReadSpellDB(filename, &spellDataTest, &ConfFile) == 0)
       {
         spellDataTest.version = spellData.version;
         if (!(spellDataTest == spellData))
         {
           MsgBoxError("Data Mismatch when importing the file we wrote", "Export/Import error");
         };
       }
       else
       {
         MsgBoxError("Error reading back the file we wrote", "Export/Import error"); 
       };
     };
     
     
     AfxGetApp()->DoWaitCursor(-1);

     if (!success)
        MsgBoxError("Operation Failed!", "Database Error");
     else
        MsgBoxInfo("Operation Successful", "Database Message");
   }
   m_testMode = FALSE;
   UpdateData(FALSE);
}

void CDbUtils::OnExportclassdb() 
{
   CString filename;
   UpdateData();
   filename = rte.DataDir()+CLASS_CONFIG_NAME;
   if (GetFilename(filename, "TXT", FALSE))
   {
     CONFIG_FILE ConfFile;
     AfxGetApp()->DoWaitCursor(1);
     BOOL success = (WriteClassDB(filename, &classData, &ConfFile)==0);

     
     
     if (m_testMode)
     {
       CLASS_DATA_TYPE classDataTest;
       if (ReadClassDB(filename, &classDataTest, &ConfFile) == 0)
       {
         //classDataTest.version = classData.version;
         if (!(classDataTest == classData))
         {
           MsgBoxError("Data Mismatch when importing the file we wrote", "Export/Import error");
         };
       }
       else
       {
         MsgBoxError("Error reading back the file we wrote", "Export/Import error"); 
       };
     };
     
     
     
     AfxGetApp()->DoWaitCursor(-1);

     if (!success)
        MsgBoxError("Operation Failed!", "Database Error");
     else
        MsgBoxInfo("Operation Successful", "Database Message");
   }
   m_testMode = FALSE;
   UpdateData(FALSE);
}

void CDbUtils::OnExportracedb() 
{
   CString filename;
   UpdateData();
   filename = rte.DataDir()+RACE_CONFIG_NAME;
   if (GetFilename(filename, "TXT", FALSE))
   {
     CONFIG_FILE ConfFile;
     AfxGetApp()->DoWaitCursor(1);
     BOOL success = (WriteRaceDB(filename, &raceData, &ConfFile)==0);
     AfxGetApp()->DoWaitCursor(-1);

     if (!success)
        MsgBoxError("Operation Failed!", "Database Error");
     else
        MsgBoxInfo("Operation Successful", "Database Message");
   }
   m_testMode = FALSE;
   UpdateData(FALSE);
}

void CDbUtils::OnEdititemtext() 
{
	// TODO: Add your control notification handler code here
  CString filename;
   filename = rte.DataDir()+ITEM_CONFIG_NAME;
  if (!GetFilename(filename, "TXT", TRUE))
    return;

  ITEM_DATA_TYPE tempItemData;
  CONFIG_FILE ConfFile;

  ReadItemDB(filename,&tempItemData,&ConfFile);

  CItemEditor dlg;
  dlg.m_pItemData=&tempItemData;
  if (dlg.DoModal()==IDCANCEL) return;  // Forget all changes
  // Rename old file to .bak
  // First form a .BAK filename
  if (backup(filename)!=0) return; // Message already printed.
  WriteItemDB(filename,&tempItemData, &ConfFile);
}

void CDbUtils::OnEditmonstertext() 
{
	// TODO: Add your control notification handler code here
  CString filename;
   filename = rte.DataDir()+MONSTER_CONFIG_NAME;
  if (!GetFilename(filename, "TXT", TRUE))
    return;

  MONSTER_DATA_TYPE tempMonsterData;
  CONFIG_FILE ConfFile;

  ReadMonsterDB(filename,&tempMonsterData,&ConfFile);

  CMonsterEditor dlg;
  dlg.m_pMonsterData=&tempMonsterData;
  if (dlg.DoModal()==IDCANCEL) return;  // Forget all changes
  // Rename old file to .bak
  // First form a .BAK filename
  if (backup(filename)!=0) return; // Message already printed.
  WriteMonsterDB(filename,&tempMonsterData, &ConfFile);
  
}


void CDbUtils::OnEditspelltext() 
{
	// TODO: Add your control notification handler code here
  CString filename;
   filename = rte.DataDir()+ITEM_CONFIG_NAME;
  if (!GetFilename(filename, "TXT", TRUE))
    return;

  SPELL_DATA_TYPE tempSpellData;
  CONFIG_FILE ConfFile;

  ReadSpellDB(filename,&tempSpellData,&ConfFile);

  CSpellEditor dlg;
  dlg.m_pSpellData=&tempSpellData;
  if (dlg.DoModal()==IDCANCEL) return;  // Forget all changes
  // Rename old file to .bak
  // First form a .BAK filename
  if (backup(filename)!=0) return; // Message already printed.
  WriteSpellDB(filename,&tempSpellData, &ConfFile);
}



void CDbUtils::OnConvertbaseclassdb() 
{
	// TODO: Add your control notification handler code here
	
   CString filename;
   filename = rte.DataDir()+BASE_CLASS_CONFIG_NAME;
   if (GetFilename(filename, "TXT", TRUE))
   {
     SET_MODIFIED;
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadBaseclassDB(filename, &baseclassData);
     AfxGetApp()->DoWaitCursor(-1);

     CString msg;
     switch (status)
     {
     case 0:
       msg = "Baseclass database read successfully";
       break;
     case 1:
       msg = "Minor errors in Baseclass database";
       break;
     case -1:
       msg = "Cannot read Baseclass database file";
       break;
     case -2:
       msg = "Baseclass database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading Baseclass database file";
       break;
     default:
       msg = "Unknown error reading Baseclass database file";
       break;
     };

     WriteDebugString("%s\n", msg);
     MsgBoxInfo(msg, "Database Status");
 
     m_BaseclassSize.Format("%i", baseclassData.GetCount());
     UpdateData(FALSE);
   }
}

void CDbUtils::OnConvertracedb() 
{
	// TODO: Add your control notification handler code here
	
   CString filename;
   filename = rte.DataDir()+RACE_CONFIG_NAME;
   if (GetFilename(filename, "TXT", TRUE))
   {
     SET_MODIFIED;
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadRaceDB(filename, &raceData);
     AfxGetApp()->DoWaitCursor(-1);

     CString msg;
     switch (status)
     {
     case 0:
       msg = "Race database read successfully";
       break;
     case 1:
       msg = "Minor errors in Race database";
       break;
     case -1:
       msg = "Cannot read Race database file";
       break;
     case -2:
       msg = "Race database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading Race database file";
       break;
     default:
       msg = "Unknown error reading Race database file";
       break;
     };

     WriteDebugString("%s\n", msg);
     MsgBoxInfo(msg, "Database Status");
 
     m_RaceSize.Format("%i", raceData.GetCount());
     UpdateData(FALSE);
   }
	
}

void CDbUtils::OnExportabilitydb(void)
{
  CString filename;
  UpdateData();
  filename = rte.DataDir() + ABILITY_CONFIG_NAME;
  if (GetFilename(filename, "TXT", FALSE))
  {
    CONFIG_FILE ConfFile;
    AfxGetApp()->DoWaitCursor(1);
    BOOL success = (WriteAbilityDB(filename, &abilityData, &ConfFile)==0);

    AfxGetApp()->DoWaitCursor(-1);
     if (!success)
       MsgBoxError("Operation Failed!", "Database Error");
    else
       MsgBoxInfo("Operation Successful", "Database Message");
  }
  m_testMode = FALSE;
  UpdateData(FALSE);
}

void CDbUtils::OnConvertabilitydb() 
{
	// TODO: Add your control notification handler code here
	
   CString filename;
   filename = rte.DataDir()+ABILITY_CONFIG_NAME;
   if (GetFilename(filename, "TXT", TRUE))
   {
     SET_MODIFIED;
     AfxGetApp()->DoWaitCursor(1);
     int status = ReadAbilityDB(filename, &abilityData);
     AfxGetApp()->DoWaitCursor(-1);

     CString msg;
     switch (status)
     {
     case 0:
       msg = "Ability database read successfully";
       break;
     case 1:
       msg = "Minor errors in Ability database";
       break;
     case -1:
       msg = "Cannot read Ability database file";
       break;
     case -2:
       msg = "Ability database file appears to be the wrong kind of file";
       break;
     case -3:
       msg = "Internal program error reading Ability database file";
       break;
     default:
       msg = "Unknown error reading Ability database file";
       break;
     };

     WriteDebugString("%s\n", msg);
     MsgBoxInfo( msg, "Database Status");
 
     m_AbilitySize.Format("%i", abilityData.GetCount());
     UpdateData(FALSE);
   }
}

void CDbUtils::OnImportalltext() 
{
  CString result;
  CUAFWinEdApp *pApp = reinterpret_cast<CUAFWinEdApp*>(AfxGetApp());
  if (!LiveDangerously()) return;
  result = pApp->LoadDefaultDBFiles();  
  MessageBox(result, "Load All Databases", MB_OK);

  m_ItemSize.Format("%i", itemData.GetCount());
  m_MonsterSize.Format("%i", monsterData.GetCount());
  m_SpellSize.Format("%i", spellData.GetCount());
  m_ClassSize.Format("%i", classData.GetCount());
  m_BaseclassSize.Format("%i", baseclassData.GetCount());
  m_RaceSize.Format("%i", raceData.GetCount());
  m_AbilitySize.Format("%i", abilityData.GetCount());
  m_SpecAbSize.Format("%i", specialAbilitiesData.GetCount());

  UpdateData(FALSE);
}

void ExportGlobal(void);
void ImportGlobal(CString filename); 

void CDbUtils::OnExportGlobalSettings()
{
  ExportGlobal();
}

void CDbUtils::OnImportGlobalSettings()
{
  ImportGlobal("");
}

void ExportCharacters(void);
void ImportCharacters(void);


void CDbUtils::OnExportCharacters()
{
  ExportCharacters();
}

void CDbUtils::OnImportCharacters()
{
  ImportCharacters();
}



void CDbUtils::DisplaySpecialAbilities(const SPECIAL_ABILITIES *pSpecialAbilities, const CString& databaseName)
{
  CString title;
  title.Format("Examine Special Abilities for $$Help in %s Database", databaseName);
  CEditAttributes dlg(*pSpecialAbilities, title);
  dlg.m_peekOnly = true;
  dlg.DoModal();
}


BOOL CDbUtils::OnHelpInfo(HELPINFO* pHelpInfo)
{ 
  if (pHelpInfo->iContextType == HELPINFO_WINDOW)
  {
    switch (pHelpInfo->iCtrlId)
    {
    case IDC_EXPORTCLASSDB:
      {
        CLASS_DATA *pClass;
        CLASS_ID classID;
        classID = "$$Help";
        //pClass = classData.GetClass("$$Help");
        pClass = classData.GetClass(classID);
        if (pClass == NULL)
        {
          MessageBox("Class data has no class named '$$Help'", "Info", MB_OK);
          return TRUE;
        };
        DisplaySpecialAbilities(&pClass->m_specialAbilities, "Class");
      };
      break;
    case IDC_EXPORTRACEDB:
      {
        RACE_DATA *pRace;
        RACE_ID raceID;
        raceID = "$$Help";
        //pRace = raceData.GetRace("$$Help");
        pRace = raceData.GetRace(raceID);
        if (pRace == NULL)
        {
          MessageBox("Race data has no race named '$$Help'", "Info", MB_OK);
          return TRUE;
        };
        DisplaySpecialAbilities(&pRace->m_specAbs, "Race");
      };
      break;
    case IDC_EXPORTBASECLASSDB:
      {
        BASE_CLASS_DATA *pBaseClass;
        BASECLASS_ID baseclassID;
        baseclassID = "$$Help";
        //pBaseClass = baseclassData.GetBaseclass("$$Help");
        pBaseClass = baseclassData.GetBaseclass(baseclassID);
        if (pBaseClass == NULL)
        {
          MessageBox("BaseClass data has no baseclass named '$$Help'", "Info", MB_OK);
          return TRUE;
        };
        DisplaySpecialAbilities(&pBaseClass->m_specAbs, "BaseClass");
      };
      break;
    case IDC_EXPORTSPECABDB:
      {
        SPECIAL_ABILITIES helpAbilities(false);
        SPECABILITY_DEF *pSpecialAbility;
        const ASLENTRY *pScript;
        POSITION pos;
        pSpecialAbility = specialAbilitiesData.FindAbility("$$Help");
        pos = pSpecialAbility->GetStartPosition();
        while (pos != NULL)
        {
          pSpecialAbility->GetNextEntry(pos, &pScript);
          helpAbilities.InsertAbility(pScript->Key(), pScript->Value(),"","");
        };
        DisplaySpecialAbilities(&helpAbilities, "Special Ability");
      };
      break;
    case IDC_EXPORTSPELLDB:
      {
        SPELL_DATA *pSpell;
        SPELL_ID spellID;
        spellID = "$$Help";
        pSpell = spellData.GetSpell(spellID);
        if (pSpell == NULL)
        {
          MessageBox("Spell data has no spell named '$$Help'", "Info", MB_OK);
          return TRUE;
        };
        DisplaySpecialAbilities(&pSpell->specAbs, "Spell");
      };
      break;
    case IDC_EXPORTMONSTERDB:
      {
        MONSTER_DATA *pMonster;
        MONSTER_ID monsterID;
        monsterID = "$$Help";
        //pMonster = monsterData.GetMonster("$$Help");
        pMonster = monsterData.GetMonster(monsterID);
        if (pMonster == NULL)
        {
          MessageBox("Monster data has no monster named '$$Help'", "Info", MB_OK);
          return TRUE;
        };
        DisplaySpecialAbilities(&pMonster->specAbs, "Monster");
      };
      break;
    case IDC_EXPORTITEMDB:
      {
        ITEM_DATA *pItem;
        ITEM_ID itemID;
        itemID = "$$Help";
        pItem = itemData.GetItem(itemID);
        if (pItem == NULL)
        {
          MessageBox("Item data has no item named '$$Help'", "Info", MB_OK);
          return TRUE;
        };
        DisplaySpecialAbilities(&pItem->specAbs, "Item");
      };
      break;
    default:
      MessageBox("Sorry....only those controls marked with '?' have help",
                 "Information",
                 MB_OK);
    };
  }
  return TRUE;
}


struct SKILL_SUMMARY
{
  SKILL_ID skillID;
  BASECLASS_ID baseclassID; // Where defined.
  RACE_ID raceID;           // where defined.
};

class CHECK_SKILLS
{
  CArray<SKILL_SUMMARY, SKILL_SUMMARY&> existingSkills;
  bool ListBaseclassSkills(const BASE_CLASS_DATA *);
  bool ListRaceSkills(const RACE_DATA *);
  bool CheckBaseclass(const BASE_CLASS_DATA *pBaseclass);
  bool CheckBaseclassAdjustment(const SKILL_ADJUSTMENT_BASECLASS *pAdj);
  bool CheckRace(const RACE_DATA *pRace);
  bool CheckRaceAdjustment(const SKILL_ADJUSTMENT_RACE *pAdj);
  bool CheckAbilityAdjustment(const SKILL_ADJUSTMENT_ABILITY *pAdj);
  bool DoesBaseclassExist(const BASECLASS_ID& baseclassID);
  bool DoesRaceExist(const RACE_ID& raceID);
  bool DoesSkillExist(const SKILL_ID& skillID);
  bool DoesAbilityExist(const ABILITY_ID& abilityID);

  void WriteError(const CString& msg);
  bool ForEachBaseclass(bool (CHECK_SKILLS::*func)(const BASE_CLASS_DATA *));
  bool ForEachRace(bool (CHECK_SKILLS::*func)(const RACE_DATA *));
public:
  bool check(void);
};


void CHECK_SKILLS::WriteError(const CString& msg)
{
  WriteDebugString("%s", msg);
}

bool CHECK_SKILLS::ListRaceSkills(const RACE_DATA *pRace)
{
  bool error = false;
  int i, n;
  n = pRace->GetSkillCount();
  for (i=0; i<n; i++)
  {
    SKILL_SUMMARY skillSummary;
    skillSummary.skillID = pRace->PeekSkill(i)->skillID;
    skillSummary.raceID = pRace->RaceID();
    existingSkills.Add(skillSummary);
  };
  return error;
}

bool CHECK_SKILLS::ListBaseclassSkills(const BASE_CLASS_DATA *pBaseclass)
{
  bool error = 0;
  int i, n;
  n = pBaseclass->GetSkillCount();
  for (i=0; i<n; i++)
  {
    SKILL_SUMMARY skillSummary;
    skillSummary.skillID = pBaseclass->PeekSkill(i)->skillID;
    skillSummary.baseclassID = pBaseclass->BaseclassID();
    existingSkills.Add(skillSummary);
  };
  return error;
}

bool CHECK_SKILLS::DoesAbilityExist(const ABILITY_ID& abilityID)
{
  if (abilityID == "Intelligence") return false;
  if (abilityID == "Strength") return false;
  if (abilityID == "Dexterity") return false;
  if (abilityID == "Wisdom") return false;
  if (abilityID == "Charisma") return false;
  if (abilityID == "Constitution") return false;
  return true;
}

bool CHECK_SKILLS::DoesBaseclassExist(const BASECLASS_ID& baseclassID)
{
  bool error = false;
  if (baseclassData.LocateBaseclass(baseclassID) < 0)
  {
    CString err;
    err.Format("Baseclass does not exist;  baseclass = %s\n", baseclassID);
    WriteError(err);
    error = true;
  };
  return error;
}

bool CHECK_SKILLS::DoesRaceExist(const RACE_ID& raceID)
{
  bool error = false;
  if (raceData.LocateRace(raceID) < 0)
  {
    CString err;
    err.Format("Race does not exist;  race = %s\n", raceID);
    WriteError(err);
    error = true;
  };
  return error;
}

bool CHECK_SKILLS::DoesSkillExist(const SKILL_ID& skillID)
{
  int i, n;
  n = existingSkills.GetSize();
  for (i=0; i<n; i++)
  {
    if (existingSkills[i].skillID == skillID)
    {
      return false;
    };
  };
  {
    CString err;
    err.Format("Skill does not exist;  skill = %s\n", skillID);
    WriteError(err);
    return true;
  };
}

bool CHECK_SKILLS::CheckBaseclassAdjustment(const SKILL_ADJUSTMENT_BASECLASS *pAdj)
{
  bool error = false;
  switch (pAdj->adjType)
  {
  case '-':
  case '+':
  case '=':
    break;
  default: 
    {
      CString err;
      err.Format("Illegal adjustment type in baseclass adjustment\n");
      WriteError(err);
      error = true;
    };
  };
  error = error || DoesBaseclassExist(pAdj->baseclassID);
  error = error || DoesSkillExist(pAdj->skillID);
  if (error)
  {
    CString err;
    err.Format("  Skill name = %s; Baseclass name = %s;  Adjustment type=%c\n", 
                   pAdj->skillID, pAdj->baseclassID, pAdj->adjType);
    WriteError(err);
  };
  return error;
}

bool CHECK_SKILLS::CheckRaceAdjustment(const SKILL_ADJUSTMENT_RACE *pAdj)
{
  bool error = false;
  switch (pAdj->adjType)
  {
  case '-':
  case '+':
  case '=':
    break;
  default: 
    {
      CString err;
      err.Format("Illegal adjustment type in race adjustment\n");
      WriteError(err);
      error = true;
    };
  };
  error = error || DoesRaceExist(pAdj->raceID);
  error = error || DoesSkillExist(pAdj->skillID);
  if (error)
  {
    CString err;
    err.Format("  Skill name = %s; Race name = %s;  Adjustment type=%c\n", 
                   pAdj->skillID, pAdj->raceID, pAdj->adjType);
    WriteError(err);
  };
  return error;
}

bool CHECK_SKILLS::CheckAbilityAdjustment(const SKILL_ADJUSTMENT_ABILITY *pAdj)
{
  bool error = false;
  switch (pAdj->adjType)
  {
  case '-':
  case '+':
    break;
  default: 
    {
      CString err;
      err.Format("Illegal adjustment type in ability adjustment\n");
      WriteError(err);
      error = true;
    };
  };
  error = error || DoesAbilityExist(pAdj->abilityID);
  error = error || DoesSkillExist(pAdj->skillID);
  if (error)
  {
    CString err;
    err.Format("  Skill name = %s; Ability name = %s;  Adjustment type=%c\n", 
                   pAdj->skillID, pAdj->abilityID, pAdj->adjType);
    WriteError(err);
  };
  return error;
}


bool CHECK_SKILLS::CheckBaseclass(const BASE_CLASS_DATA *pBaseclass)
{
  bool error = false;
  int i, n;
  n = pBaseclass->GetSkillAdjBaseclassCount();
  for (i=0; i<n; i++)
  {
    const SKILL_ADJUSTMENT_BASECLASS *pAdj;
    pAdj = pBaseclass->PeekSkillAdjBaseclass(i);
    if (CheckBaseclassAdjustment(pAdj) != 0)
    {
      CString err;
      err.Format("   Error occurred in definition of baseclass %s\n", pBaseclass->BaseclassID());
      error = true;
    };
  };
  n = pBaseclass->GetSkillAdjRaceCount();
  for (i=0; i<n; i++)
  {
    const SKILL_ADJUSTMENT_RACE *pAdj;
    pAdj = pBaseclass->PeekSkillAdjRace(i);
    if (CheckRaceAdjustment(pAdj))
    {
      CString err;
      err.Format("   Error occurred in definition of baseclass %s\n", pBaseclass->BaseclassID());
      error = true;
    };
  };
  n = pBaseclass->GetSkillAdjAbilityCount();
  for (i=0; i<n; i++)
  {
    const SKILL_ADJUSTMENT_ABILITY *pAdj;
    pAdj = pBaseclass->PeekSkillAdjAbility(i);
    if (CheckAbilityAdjustment(pAdj))
    {
      CString err;
      err.Format("   Error occurred in definition of baseclass %s\n", pBaseclass->BaseclassID());
      error = true;
    };
  };
  return error;
}

bool CHECK_SKILLS::CheckRace(const RACE_DATA *pRace)
{
  bool error = false;
  int i, n;
  n = pRace->GetSkillAdjBaseclassCount();
  for (i=0; i<n; i++)
  {
    const SKILL_ADJUSTMENT_BASECLASS *pAdj;
    pAdj = pRace->PeekSkillAdjBaseclass(i);
    if (CheckBaseclassAdjustment(pAdj) != 0)
    {
      CString err;
      err.Format("   Error occurred in definition of race %s\n", pRace->RaceID());
      error = true;
    };
  };
  n = pRace->GetSkillAdjRaceCount();
  for (i=0; i<n; i++)
  {
    const SKILL_ADJUSTMENT_RACE *pAdj;
    pAdj = pRace->PeekSkillAdjRace(i);
    if (CheckRaceAdjustment(pAdj))
    {
      CString err;
      err.Format("   Error occurred in definition of race %s\n", pRace->RaceID());
      error = true;
    };
  };
  n = pRace->GetSkillAdjAbilityCount();
  for (i=0; i<n; i++)
  {
    const SKILL_ADJUSTMENT_ABILITY *pAdj;
    pAdj = pRace->PeekSkillAdjAbility(i);
    if (CheckAbilityAdjustment(pAdj))
    {
      CString err;
      err.Format("   Error occurred in definition of race %s\n", pRace->RaceID());
      error = true;
    };
  };
  return error;
}

bool CHECK_SKILLS::ForEachBaseclass(bool (CHECK_SKILLS::*func)(const BASE_CLASS_DATA *))
{
  bool error = false;
  int i, n;
  n = baseclassData.GetBaseclassCount();
  for (i=0; i<n; i++)
  {
    const BASE_CLASS_DATA *pBaseclass;
    pBaseclass = baseclassData.PeekBaseclass(i);
    error = error || (this->*func)(pBaseclass);
  };
  return error;
}

bool CHECK_SKILLS::ForEachRace(bool (CHECK_SKILLS::*func)(const RACE_DATA *pRace))
{
  bool error = false;
  int i, n;
  n = raceData.GetRaceCount();
  for (i=0; i<n; i++)
  {
    const RACE_DATA *pRace;
    pRace = raceData.PeekRace(i);
    error = error || (this->*func)(pRace);
  };
  return error;
}


bool CHECK_SKILLS::check(void)
{
  bool error = false;
  error = error || ForEachBaseclass(&CHECK_SKILLS::ListBaseclassSkills);
  error = error || ForEachRace(&CHECK_SKILLS::ListRaceSkills);
//int CheckSkillAdjustmentHasValidSkill(void);
  error = error || ForEachBaseclass(&CHECK_SKILLS::CheckBaseclass);
  error = error || ForEachRace(&CHECK_SKILLS::CheckRace);
//int CheckValidRaces(void);  // And Indirect references from baselclasses.
//int CheckValidBaseclasses(void);
//int CheckValidAbilityNAmes(void);
//int CheckLegalAdjustmentTypes(void)
  return error;
};


void CheckSkills(void)
{
  //Check that every skill adjustment refers to
  // a valid skill.
  CHECK_SKILLS checkSkills;
  int numError;
  numError = checkSkills.check();
  //numError += CheckSkillAdjustmentHasValidSkill();
  //numError += CheckValidRaces();  // And Indirect references from baselclasses.
  //numError += CheckValidBaseclasses();
  //numError += CheckValidAbilityNAmes();
  //numError += CheckLegalAdjustmentTypes();
  if (numError != 0)
  {
    MessageBox(NULL, "There are errors - See the editor log","Warning", MB_OK);
  }
  else
  {
    MessageBox(NULL, "No Obvious Errors", "Info", MB_OK);
  };
}
