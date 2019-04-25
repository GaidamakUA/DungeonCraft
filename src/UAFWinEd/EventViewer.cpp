/******************************************************************************
* Filename: EventViewer.cpp
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
#include "class.h"
#include "GameEvent.h"
#include "resource.h"

//#include "UAFWinEd.h"
#include "globtext.h"
//#include "Char.h"
//#include "items.h"
#include "spell.h"
#include "GlobalData.h"
//#include "Level.h"

#include "ItemDlg.h"
#include "ItemKeyQuestDlg.h"
#include "EditText.h"

// includes for all event dialogs
#include "AddNPC.h"
#include "CampEvnt.h"
#include "CombatEvent.h"
#include "cbattres.h"
#include "givedmg.h"
#include "encnter.h"
#include "password.h"
#include "GiveExp.h"
#include "tour.h"
#include "NPCSays.h"
#include "QListOptions.h"
#include "QButtonOptions.h"
#include "QuestionYesNoData.h"
#include "RemovNPC.h"
#include "ShopDlg.h"
#include "TextEvent.h"
#include "TransferEvent.h"
#include "ChurchDlg.h"
#include "TavDlg.h"
#include "TavernTalesDlg.h"
#include "WhoPaysDlg.h"
#include "WhoTriesDlg.h"
#include "SpecialItemDlg1.h"
#include "PassTimeDlg.h"
#include "VaultEvent.h"
#include "StepEventDlg.h"
#include "ZoneDlg.h"
#include "TrainDlg.h"
#include "SmallTownEvent.h"
#include "QuestEventData.h"
#include "UtilitiesEventData.h"
#include "SoundEventDlg.h"
#include "TakePartyItems.h"
#include "HealParty.h"
#include "LogicBlock.h"
#include "RandomEvent.h"
#include "PlayMovieDlg.h"
#include "JournalAddEventDlg.h"
#include "ChooseNPC.h"
#include "GPDLOpCodes.h"
#include "gpdlcomp.h"
#include "SearchParamDlg.h"
#include "SpellDlg.h"
#include "CombatTreasureDlg.h"

#include "EventViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MAX_NODE_TEXT_LEN = 512;
char RootText[MAX_NODE_TEXT_LEN+1];
static HTREEITEM srchpos = NULL;

//*****************************************************************************
//    NAME: addJournalEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addJournalEvent(GameEvent *event, UINT& result)
{
  JOURNAL_EVENT *data = (JOURNAL_EVENT*)(event);

  CJournalAddEventDlg dlg(*data);
  result = dlg.DoModal();   
  if (result == IDOK)
    dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addPlayMovieEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addPlayMovieEvent(GameEvent *event, UINT& result)
{
  PLAY_MOVIE_DATA *data = (PLAY_MOVIE_DATA*)(event);

  PlayMovieDlg dlg(*data);
  result = dlg.DoModal();   
  if (result == IDOK)
    dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addRandomEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addRandomEvent(GameEvent *event, UINT& result)
{
  RANDOM_EVENT_DATA *data = (RANDOM_EVENT_DATA*)(event);

  CRandomEvent dlg(*data);
  result = dlg.DoModal();   
  if (result == IDOK)
    dlg.GetData(*data);
}
//*****************************************************************************
//    NAME: addSoundEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addSoundEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(SOUND_EVENT)));
   SOUND_EVENT *data = (SOUND_EVENT*)(event);

   CSoundEventDlg dlg(*data);
   result = dlg.DoModal();   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addUtilitiesEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addUtilitiesEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(UTILITIES_EVENT_DATA)));
   UTILITIES_EVENT_DATA *data = (UTILITIES_EVENT_DATA*)(event);

   CUtilitiesEventData dlg(*data);
   result = dlg.DoModal();   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addQuestEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addQuestEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUEST_EVENT_DATA)));
   QUEST_EVENT_DATA *data = (QUEST_EVENT_DATA*)(event);

   CQuestEventData dlg(*data);   
   result = dlg.DoModal();   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addSmallTownEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addSmallTownEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(SMALL_TOWN_DATA)));
   SMALL_TOWN_DATA *data = (SMALL_TOWN_DATA*)(event);

   CSmallTownEvent dlg(*data);   
   result = dlg.DoModal();   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addNPCEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addNPCEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(ADD_NPC_DATA)));
   ADD_NPC_DATA *data = (ADD_NPC_DATA*)(event);

   AddNPC dlg(*data);   
   result = dlg.DoModal();   
   if (result == IDOK)
      dlg.GetNPCData(*data);
}

//*****************************************************************************
//    NAME: addVaultEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addVaultEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(VAULT_EVENT_DATA)));
   VAULT_EVENT_DATA *data = (VAULT_EVENT_DATA*)(event);

  CVaultEvent dlg(*data);
   result = dlg.DoModal();
   
  if (result == IDOK)
     dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addCampEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addCampEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(CAMP_EVENT_DATA)));
   CAMP_EVENT_DATA *data = (CAMP_EVENT_DATA*)(event);

   CCampEvent dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetCampData(*data);
}

//*****************************************************************************
//    NAME: addCombatEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addCombatEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(COMBAT_EVENT_DATA)));
   COMBAT_EVENT_DATA *data = (COMBAT_EVENT_DATA*)(event);

   CCombat dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetCombatData(*data);
}


//*****************************************************************************
//    NAME: addCombatTreasureEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addCombatTreasureEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GIVE_TREASURE_DATA)));
   COMBAT_TREASURE *data = (COMBAT_TREASURE*)(event);

   CCombatTreasure dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetTreasureData(*data);
}



//*****************************************************************************
//    NAME: addTreasureEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTreasureEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GIVE_TREASURE_DATA)));
   GIVE_TREASURE_DATA *data = (GIVE_TREASURE_DATA*)(event);

   CCombatTreas dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetTreasureData(*data);
}

//*****************************************************************************
//    NAME: addGiveDamageEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addGiveDamageEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GIVE_DAMAGE_DATA)));
   GIVE_DAMAGE_DATA *data = (GIVE_DAMAGE_DATA*)(event);

   GiveDmg dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetDamageData(*data);
}

//*****************************************************************************
//    NAME: addEncounterEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addEncounterEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(ENCOUNTER_DATA)));
   ENCOUNTER_DATA *data = (ENCOUNTER_DATA*)(event);

   CEncounter dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetEncounterData(*data);
}

//*****************************************************************************
//    NAME: addPasswordEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addPasswordEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(PASSWORD_DATA)));
   PASSWORD_DATA *data = (PASSWORD_DATA*)(event);

   CPassword dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetPasswordData(*data);
}

//*****************************************************************************
//    NAME: addGiveExpEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addGiveExpEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GAIN_EXP_DATA)));
   GAIN_EXP_DATA *data = (GAIN_EXP_DATA*)(event);

   GiveExp dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetExpData(*data);
}

//*****************************************************************************
//    NAME: addTourEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTourEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(GUIDED_TOUR)));
   GUIDED_TOUR *data = (GUIDED_TOUR*)(event);

   Tour dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetTourData(*data);
}

//*****************************************************************************
//    NAME: addNPCSaysEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addNPCSaysEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(NPC_SAYS_DATA)));
   NPC_SAYS_DATA *data = (NPC_SAYS_DATA*)(event);

   CNPCSays dlg(*data);
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetNPCSaysData(*data);
}

//*****************************************************************************
//    NAME: addQListEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addQListEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_LIST_DATA)));
   QUESTION_LIST_DATA *data = (QUESTION_LIST_DATA*)(event);

   CQListOptions dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addQButtonEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addQButtonEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_BUTTON_DATA)));
   QUESTION_BUTTON_DATA *data = (QUESTION_BUTTON_DATA*)(event);

   CQButtonOptions dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addQYesNoEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addQYesNoEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_YES_NO)));
   QUESTION_YES_NO *data = (QUESTION_YES_NO*)(event);

   CQuestionYesNoData dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addRemoveNPCEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addRemoveNPCEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(REMOVE_NPC_DATA)));
   REMOVE_NPC_DATA *data = (REMOVE_NPC_DATA*)(event);

   RemoveNPC dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetRemoveNPCData(*data);
}

//*****************************************************************************
//    NAME: addShopEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addShopEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(SHOP)));
   SHOP *data = (SHOP*)(event);

   CShop dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetShopData(*data);
}


//*****************************************************************************
//    NAME: addTavernEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTavernEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TAVERN)));
   TAVERN *data = (TAVERN*)(event);

   CTavern dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetTavernData(*data);
}


//*****************************************************************************
//    NAME: addTavernTalesEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTavernTalesEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TAVERN)));
   TAVERN_TALES *data = (TAVERN_TALES*)(event);

   CTavernTales dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetTavernTalesData(*data);
}


//*****************************************************************************
//    NAME: addTempleEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTempleEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TEMPLE)));
   TEMPLE *data = (TEMPLE*)(event);

   CChurch dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetTempleData(*data);
}

//*****************************************************************************
//    NAME: addTextEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTextEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TEXT_EVENT_DATA)));
   TEXT_EVENT_DATA *data = (TEXT_EVENT_DATA*)(event);

   CTextEvent dlg(*data);
   
   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addTransferEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTransferEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
   TRANSFER_EVENT_DATA *data = (TRANSFER_EVENT_DATA*)(event);

   CTransferEvent dlg(*data, TRUE);

   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addTeleporterEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTeleporterEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
   TRANSFER_EVENT_DATA *data = (TRANSFER_EVENT_DATA*)(event);

   CTransferEvent dlg(*data, FALSE);

   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addStairsEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addStairsEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
   TRANSFER_EVENT_DATA *data = (TRANSFER_EVENT_DATA*)(event);

   CTransferEvent dlg(*data, FALSE);

   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addWhoPaysEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addWhoPaysEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_PAYS_EVENT_DATA)));
   WHO_PAYS_EVENT_DATA *data = (WHO_PAYS_EVENT_DATA*)(event);

   CWhoPaysDlg dlg(*data);

   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addWhoTriesEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addWhoTriesEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_TRIES_EVENT_DATA)));
   WHO_TRIES_EVENT_DATA *data = (WHO_TRIES_EVENT_DATA*)(event);

   CWhoTriesDlg dlg(*data);

   result = dlg.DoModal();
   
   if (result == IDOK)
      dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addSpecialItemEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addSpecialItemEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(SPECIAL_ITEM_KEY_EVENT_DATA)));
   SPECIAL_ITEM_KEY_EVENT_DATA *data = (SPECIAL_ITEM_KEY_EVENT_DATA*)(event);

  CSpecialItemDlg dlg(*data);

  result = dlg.DoModal();

  if (result == IDOK)
    dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addPassTimeEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addPassTimeEvent(GameEvent *event, UINT& result)
{
   //ASSERT(event->IsKindOf(RUNTIME_CLASS(PASS_TIME_EVENT_DATA)));
   PASS_TIME_EVENT_DATA *data = (PASS_TIME_EVENT_DATA*)(event);

  CPassTimeDlg dlg(*data);

  result = dlg.DoModal();

  if (result == IDOK)
    dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addTrainingHallEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTrainingHallEvent(GameEvent *event, UINT& result)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(TRAININGHALL)));
  TRAININGHALL *data = (TRAININGHALL*)(event);

  CTrainingHall dlg(*data);

  result = dlg.DoModal();

  if (result == IDOK)
    dlg.GetHallData(*data);
}

//*****************************************************************************
//    NAME: addTakePartyItemsEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addTakePartyItemsEvent(GameEvent *event, UINT& result)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(TAKE_PARTY_ITEMS_DATA)));
  TAKE_PARTY_ITEMS_DATA *data = (TAKE_PARTY_ITEMS_DATA*)(event);

  CTakePartyItems dlg(*data);

  result = dlg.DoModal();

  if (result == IDOK)
    dlg.GetData(*data);
}


//*****************************************************************************
//    NAME: addHealPartyEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addHealPartyEvent(GameEvent *event, UINT& result)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(HEAL_PARTY_DATA)));
  HEAL_PARTY_DATA *data = (HEAL_PARTY_DATA*)(event);

  CHealParty dlg(*data);

  result = dlg.DoModal();

  if (result == IDOK)
    dlg.GetData(*data);
}

//*****************************************************************************
//    NAME: addLogicBlockEvent
//
// PURPOSE: 
//
//*****************************************************************************
static void addLogicBlockEvent(GameEvent *event, UINT& result)
{
  //ASSERT(event->IsKindOf(RUNTIME_CLASS(LOGIC_BLOCK_DATA)));
  LOGIC_BLOCK_DATA *data = (LOGIC_BLOCK_DATA*)(event);

  CLogicBlock dlg(*data);

  result = dlg.DoModal();

  if (result == IDOK)
    dlg.GetData(*data);
}

/////////////////////////////////////////////////////////////////////////////
// CEventViewer dialog


//*****************************************************************************
//    NAME: CEventViewer::CEventViewer
//
// PURPOSE: 
//
//*****************************************************************************
CEventViewer::CEventViewer(int x, int y, CWnd* pParent /*=NULL*/)
	: CDialog(CEventViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventViewer)
	m_OnceOnly = FALSE;
	m_Chance = 0;
	m_DeleteAllChained = FALSE;
	m_PartyX = 0;
	m_PartyY = 0;
	//}}AFX_DATA_INIT
  m_x = x;
  m_y = y;
  m_hCurrItem = NULL;  
  m_CurrEvent = NULL;
  m_CurrFlags = 0;
  m_SingleEvent=FALSE;
  m_SingleEventRoot=0;
  m_SingleEventRootLabel="";
  m_SelectedEventId=0;
}

//*****************************************************************************
//    NAME: CEventViewer::CEventViewer
//
// PURPOSE: 
//
//*****************************************************************************
CEventViewer::CEventViewer(CString label, DWORD SingleEvent, CWnd* pParent /*=NULL*/)
	: CDialog(CEventViewer::IDD, pParent)
{
	m_OnceOnly = FALSE;
	m_Chance = 0;
	m_DeleteAllChained = FALSE;
  m_x = -1;
  m_y = -1;
  m_hCurrItem = NULL;  
  m_CurrEvent = NULL;
  m_CurrFlags = 0;
  m_SingleEvent=TRUE;
  m_SingleEventRoot=SingleEvent;
  m_SingleEventRootLabel=label;
  m_SelectedEventId=0;
}

//*****************************************************************************
//    NAME: CEventViewer::DoDataExchange
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventViewer)
	DDX_Control(pDX, IDC_SPELLTRIGGER, m_SpellTrigger);
	DDX_Control(pDX, IDC_GPDLTRIGGER, m_GPDLTrigger);
	DDX_Control(pDX, IDC_NPCTRIGGER, m_NPCTrigger);
	DDX_Control(pDX, IDC_PASTEEVENTS, m_Paste);
	DDX_Control(pDX, IDC_COPYEVENTS, m_Copy);
	DDX_Control(pDX, IDC_CHOOSEGENDER, m_Gender);
	DDX_Control(pDX, IDC_CONFIGZONES, m_ConfigZones);
	DDX_Control(pDX, IDC_CONFIGSTEPEVENTS, m_ConfigStepEvents);
	DDX_Control(pDX, IDC_EVENT_TYPE, m_EventType);
	DDX_Control(pDX, IDC_TRIGGERSPECKEY, m_SpKeyTrigger);
	DDX_Control(pDX, IDC_TRIGGERSPECITEM, m_SpItemTrigger);
	DDX_Control(pDX, IDC_TRIGGERITEM, m_ItemTrigger);
	DDX_Control(pDX, IDC_QUESTTRIGGER, m_QuestTrigger);
	DDX_Control(pDX, IDC_EVENTTRIGGER, m_EventTrigger);
	DDX_Control(pDX, IDC_CHOOSERACE, m_Race);
	DDX_Control(pDX, IDC_CHOOSEFACING, m_Facing);
	DDX_Control(pDX, IDC_CHOOSECLASS, m_Class);
	DDX_Control(pDX, IDC_CHAINTRIGGER, m_ChainTrigger);
	DDX_Control(pDX, IDC_INSERTEVENT, m_Insert);
	DDX_Control(pDX, IDC_DELETEEVENT, m_Delete);
	DDX_Control(pDX, IDC_EDITEVENT, m_Edit);
	DDX_Control(pDX, IDC_EVENTTREE, m_EventTree);
	DDX_Check(pDX, IDC_ONCEONLY, m_OnceOnly);
	DDX_Text(pDX, IDC_PCNTCHANCE, m_Chance);
	DDX_Check(pDX, IDC_DELETEALLCHAINED, m_DeleteAllChained);
	DDX_Text(pDX, IDC_PARTYXTRIGGER, m_PartyX);
	DDX_Text(pDX, IDC_PARTYYTRIGGER, m_PartyY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventViewer, CDialog)
	//{{AFX_MSG_MAP(CEventViewer)
	ON_BN_CLICKED(IDC_INSERTEVENT, OnInsertevent)
	ON_BN_CLICKED(IDC_EDITEVENT, OnEditevent)
	ON_BN_CLICKED(IDC_DELETEEVENT, OnDeleteevent)
	ON_NOTIFY(NM_DBLCLK, IDC_EVENTTREE, OnDblclkEventtree)
	ON_BN_CLICKED(IDC_TRIGGERSPECKEY, OnTriggerspeckey)
	ON_BN_CLICKED(IDC_TRIGGERSPECITEM, OnTriggerspecitem)
	ON_BN_CLICKED(IDC_TRIGGERITEM, OnTriggeritem)
	ON_BN_CLICKED(IDC_QUESTTRIGGER, OnQuesttrigger)
	ON_CBN_SELCHANGE(IDC_CHAINTRIGGER, OnSelchangeChaintrigger)
	ON_NOTIFY(TVN_SELCHANGED, IDC_EVENTTREE, OnSelchangedEventtree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_EVENTTREE, OnSelchangingEventtree)
	ON_CBN_SELCHANGE(IDC_EVENTTRIGGER, OnSelchangeEventtrigger)
	ON_BN_CLICKED(IDC_CONFIGSTEPEVENTS, OnConfigstepevents)
	ON_BN_CLICKED(IDC_CONFIGZONES, OnConfigzones)
	ON_BN_CLICKED(IDC_DELETEALLCHAINED, OnDeleteallchained)
	ON_BN_CLICKED(IDC_EXPANDALL, OnExpandall)
	ON_BN_CLICKED(IDC_COLLAPSEALL, OnCollapsall)
	ON_BN_CLICKED(IDC_PASTEEVENTS, OnPasteevents)
	ON_BN_CLICKED(IDC_COPYEVENTS, OnCopyevents)
	ON_BN_CLICKED(IDC_NPCTRIGGER, OnNpctrigger)
	ON_BN_CLICKED(IDC_GPDLTRIGGER, OnGpdltrigger)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_SPELLTRIGGER, OnSpelltrigger)
	//}}AFX_MSG_MAP

  ON_MESSAGE(MSG_FINDNEXT, OnMsgFindNext) 
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEventViewer message handlers

//*****************************************************************************
//    NAME: CEventViewer::CreateNewEvent
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::CreateNewEvent()
{
  // get current event settings
  UpdateData(TRUE);

  int sel = m_EventType.GetCurSel();
  if (m_SingleEvent)
    m_CurrEvent = globalData.eventData.CreateNewEvent(m_EventType.GetItemData(sel));
  else
    m_CurrEvent = levelData.eventData.CreateNewEvent(m_EventType.GetItemData(sel));
  
  if (m_CurrEvent == NULL)
    return;

  EditorOptionsData.m_LastEventTypeUsed = m_EventType.GetItemData(sel);
  
  if (m_EventTree.GetParentItem(m_hCurrItem) == NULL)
  {
    if (m_hCurrItem!=NULL)
    {
      DWORD val;
      if (m_MapXY.Lookup(m_hCurrItem, val))
      {
        m_x = ((val >> 16) & 0x0000FFFF);
        m_y = (val & 0x0000FFFF);
        m_MapXY.RemoveKey(m_hCurrItem);
      }
    }

    WriteDebugString("Creating new root event at %u,%u\n", m_x, m_y);
    m_CurrEvent->x = m_x;
    m_CurrEvent->y = m_y;
  }
  else // is a chained event
  {
    WriteDebugString("Creating new chain event\n");
    m_CurrEvent->x = -1;
    m_CurrEvent->y = -1;
  }

  GetControls(m_CurrEvent);
}

//*****************************************************************************
//    NAME: CEventViewer::EditEvent
//
// PURPOSE: 
//
//*****************************************************************************
UINT CEventViewer::EditEvent(GameEvent *event)
{
  UINT result = !IDOK;

  if (event == NULL)
    return result;

  // invoke the appropriate event dialog
  switch (event->GetEventType())
  {
  case AddNpc:
    addNPCEvent(event, result);
    break;    
  case Camp:
    addCampEvent(event, result);
    break;    
  case Combat:
    addCombatEvent(event, result);
    break;    
  case CombatTreasure:
    addCombatTreasureEvent(event, result);
    break;    
  case Damage:
    addGiveDamageEvent(event, result);
    break;    
  case EncounterEvent:
    addEncounterEvent(event, result);
    break;    
  case EnterPassword:
    addPasswordEvent(event, result);
    break;    
  case GainExperience:
    addGiveExpEvent(event, result);
    break;    
  case GiveTreasure:
    addTreasureEvent(event, result);
    break;    
  case GuidedTour:
    addTourEvent(event, result);
    break;    
  case NPCSays:
    addNPCSaysEvent(event, result);
    break;    
  case QuestionList:
    addQListEvent(event, result);
    break;    
  case QuestionButton:
    addQButtonEvent(event, result);
    break;    
  case PassTime:
    addPassTimeEvent(event, result);
    break;    
  case QuestionYesNo:
    addQYesNoEvent(event, result);
    break;    
  case RemoveNPCEvent:
    addRemoveNPCEvent(event, result);
    break;    
  case ShopEvent:
    addShopEvent(event, result);
    break;    
  case TempleEvent:
    addTempleEvent(event, result);
    break;    
  case TavernEvent:
    addTavernEvent(event, result);
    break;  
  case TavernTales:
    addTavernTalesEvent(event, result);
    break;
  case Stairs:
    addStairsEvent(event, result);
    break;    
  case Teleporter:
    addTeleporterEvent(event, result);
    break;    
  case TextStatement:
    addTextEvent(event, result);
    break;    
  case TransferModule:
    addTransferEvent(event, result);
    break;    
  case WhoPays:
    addWhoPaysEvent(event, result);
    break;    
  case WhoTries:
    addWhoTriesEvent(event, result);
    break;    
  case SpecialItem:
    addSpecialItemEvent(event, result);
    break;    
  case Vault:
    addVaultEvent(event, result);
    break;  
  case TrainingHallEvent:
    addTrainingHallEvent(event, result);
    break;
  case SmallTown:
    addSmallTownEvent(event, result);
    break;
  case ChainEventType:
    // no data other than event chain and triggers
    result = IDOK;
    break;
  case QuestStage:
    addQuestEvent(event, result);
    break;
  case Sounds:
    addSoundEvent(event, result);
    break;
  case Utilities:
    addUtilitiesEvent(event, result);
    break;    
  case TakePartyItems:
    addTakePartyItemsEvent(event, result);
    break;
  case HealParty:
    addHealPartyEvent(event, result);
    break;
  case LogicBlock:
    addLogicBlockEvent(event, result);
    break;
  case RandomEvent:
    addRandomEvent(event, result);
    break;
  case PlayMovieEvent:
    addPlayMovieEvent(event, result);
    break;
  case JournalEvent:
    addJournalEvent(event, result);
    break;
  case PickOneCombat:
  //case InnEvent:    
  //case TavernTales:
  default:
    WriteDebugString("Unhandled event type %i in NewEvent::EventData()\n",
                     event->GetEventType());
    break;
  }

  return result;
}

//
// Also gets called to add a new event
//
//*****************************************************************************
//    NAME: CEventViewer::OnEditevent
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnEditevent() 
{
  // must be pointing to a tree node before event
  // edit/insert can occur
  //
  LookupCurrEvent();

  if (m_hCurrItem == NULL)
    return;

  bool NewEvent = false;

  // add new event, unused event node is selected
  if (m_CurrEvent == NULL )
  {
    NewEvent = true;
    CreateNewEvent();
  }
  
  if (m_CurrEvent == NULL)
    return;

  UINT result = EditEvent(m_CurrEvent);

  if (result==IDOK)
    SET_MODIFIED;

  if ((result == IDOK) && (NewEvent))
  {
    if (m_SingleEvent)
      globalData.eventData.AddEvent(m_CurrEvent);
    else
      levelData.eventData.AddEvent(m_CurrEvent);

    BYTE ChainIdx = GetChainIndex(m_hCurrItem);

    if (m_CurrEvent->IsAChainEvent())
    {
      // all rest and step events always have x,y = -1,
      // so would be considered chain events

      if (ChainIdx != NOT_A_CHAIN)
      {
        // the new event is a chained event,
        // so we need to chain new event to parent
        DWORD ParentId = GetParentEventId(m_hCurrItem);

        GameEvent *ParentEvent;
        if (m_SingleEvent)
          ParentEvent = globalData.eventData.GetEvent(ParentId);
        else
          ParentEvent = levelData.eventData.GetEvent(ParentId);

        if (ParentEvent != NULL)
        {
          ASSERT(ParentEvent->GetEventId() != m_CurrEvent->GetEventId()); // no looping
          ParentEvent->ChainToEventId(ChainIdx, m_CurrEvent->GetEventId());
        }
        else
        {
          // if root step or rest event, update levelData
          switch (m_CurrFlags)    
          {
          case IS_TIME_EVENT:
            ASSERT (m_SingleEvent==FALSE);
            //levelData.timeEvents[ChainIdx].timeEvent = m_CurrEvent->GetEventId();
            break;
          case IS_STEP_EVENT:
            ASSERT (m_SingleEvent==FALSE);
            levelData.stepEvents[ChainIdx].stepEvent = m_CurrEvent->GetEventId();
            break;
          case IS_REST_EVENT:
            ASSERT (m_SingleEvent==FALSE);
            levelData.zoneData.zones[ChainIdx].restEvent.restEvent = m_CurrEvent->GetEventId();
            break;
          case IS_SINGLE_EVENT:
            m_SingleEventRoot = m_CurrEvent->GetEventId();
            break;

          default:
            // do nothing
            break;
          }
        }
      }
    }

    PopulateTree(m_CurrEvent->GetEventId());
    UpdateControls(m_CurrEvent);
    SetControlStates();
  }
  else
  {
    if ((m_CurrEvent != NULL) && (NewEvent))
    {
      WriteDebugString("Deleting '%s (%u)' event in EventViewer - canceled new event add\n",
                       GetEventDescription(m_CurrEvent->GetEventType()),m_CurrEvent->GetEventId());

      delete m_CurrEvent;
      m_CurrEvent = NULL;
    }
  }
}

//*****************************************************************************
//    NAME: CEventViewer::OnDeleteevent
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnDeleteevent() 
{
  int numChainCleared;
  DWORD chainId = 0;
  GameEvent *parentEvent;

  LookupCurrEvent();

  BYTE flags = GetFlags(m_hCurrItem);
  BYTE chainIdx = GetChainIndex(m_hCurrItem);
  DWORD myID = GetEventId(m_hCurrItem);

  DWORD parentID = GetParentEventId(m_hCurrItem);

  // delete root event and all events
  // that are chained to it
  if (m_CurrEvent != NULL)
  {
    if (m_DeleteAllChained)
    {
      if (m_SingleEvent)
        globalData.eventData.DeleteEvents(m_CurrEvent);
      else
        levelData.eventData.DeleteEvents(m_CurrEvent);
      m_CurrEvent=NULL;
    }
    else
    {
      // lookup current chain event id
      switch (m_CurrEvent->control.chainTrigger)
      {
      case AlwaysChain:
      case IfEventHappen:
        chainId = m_CurrEvent->chainEventHappen;
        break;
      case IfEventNotHappen:
        chainId = m_CurrEvent->chainEventNotHappen;
        break;
      }

      if (m_SingleEvent)
        globalData.eventData.DeleteEvent(m_CurrEvent);
      else
        levelData.eventData.DeleteEvent(m_CurrEvent);
      m_CurrEvent=NULL;
    }
  }

  // A little overkill here because GetEvent returns NULL if not valid.
  BOOL ev_valid;
  if (m_SingleEvent)
    ev_valid = globalData.eventData.IsValidEvent(parentID);
  else
    ev_valid = levelData.eventData.IsValidEvent(parentID);

  if (ev_valid)
  {
    if (m_SingleEvent)
      parentEvent=globalData.eventData.GetEvent(parentID);
    else
      parentEvent=levelData.eventData.GetEvent(parentID);

    // The event is gone.  Nobody better be pointing at it.
    if (parentEvent!=NULL) 
    {
      m_CurrEvent=parentEvent;
      // either deleted event has no chain, or all chained
      // events under deleted event were deleted
      if (chainId == 0)
      {
        numChainCleared=parentEvent->UnchainToEventId(chainIdx,myID);
        ASSERT(numChainCleared==1);
      }
      else
      {
        // only selected event was deleted, now make
        // parent event point to chained event
        ASSERT(parentEvent->GetEventId() != chainId); // no looping
        parentEvent->ChainToEventId(chainIdx, chainId);
      }
    }
  }
  else
  {
    // there was no parent event
    if (chainId > 0)
    {
      // deleted event was a root, if there is a chained event
      // it needs to be the root now.
      GameEvent *pEvent;
      if (m_SingleEvent)
        pEvent = globalData.eventData.GetEvent(chainId);
      else
        pEvent = levelData.eventData.GetEvent(chainId);

      if (pEvent != NULL)
      {
        pEvent->x = m_x;
        pEvent->y = m_y;
        m_CurrEvent=pEvent;
      }

      // chained event is now root
      // update levelData
      switch (flags)
      {
      case IS_TIME_EVENT_ROOT:
      case IS_REST_EVENT_ROOT:
      case IS_STEP_EVENT_ROOT:
      case IS_SINGLE_EVENT_ROOT:
        // shouldn't get here
        ASSERT(FALSE);
        break;
      case IS_REST_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.zoneData.zones[chainIdx].restEvent.restEvent = chainId;
        break;
      case IS_STEP_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.stepEvents[chainIdx].stepEvent = chainId;
        break;
      case IS_TIME_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        //levelData.timeEvents[chainIdx].timeEvent = chainId;
        break;
      case IS_SINGLE_EVENT:
        m_SingleEventRoot = chainId;
        break;
      }
    }
    else
    {
      // no parent and no chain
      // clear event in levelData
      switch (flags)
      {
      case IS_TIME_EVENT_ROOT:
      case IS_REST_EVENT_ROOT:
      case IS_STEP_EVENT_ROOT:
      case IS_SINGLE_EVENT_ROOT:
        // shouldn't get here
        ASSERT(FALSE);
        break;
      case IS_REST_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.zoneData.zones[chainIdx].restEvent.restEvent = 0;
        break;
      case IS_STEP_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.stepEvents[chainIdx].stepEvent = 0;
        break;
      case IS_TIME_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        //levelData.timeEvents[chainIdx].timeEvent = 0;
        break;
      case IS_SINGLE_EVENT:
        m_SingleEventRoot = 0;
        break;
      }
    }
  }

  UpdateControls(m_CurrEvent);
  PopulateTree(parentID);
  UpdateControls(m_CurrEvent);
  SetControlStates();
  SET_MODIFIED;
}

//*****************************************************************************
//    NAME: CEventViewer::OnInsertevent
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnInsertevent() 
{
  // save current node handle
  LookupCurrEvent();
  
  // need handle to tree item to continue
  if (m_hCurrItem == NULL)
    return;

  BYTE flags = GetFlags(m_hCurrItem);
  BYTE chainIdx = GetChainIndex(m_hCurrItem);
  DWORD origID = GetEventId(m_hCurrItem);

  // get id for parent of insertion point
  DWORD ParentId = GetParentEventId(m_hCurrItem);

  // create new event
  CreateNewEvent();

  // edit event data
  UINT result = EditEvent(m_CurrEvent);

  // if keep event
  if (result == IDOK)
  {
    // add new event to list
    if (m_SingleEvent)
      globalData.eventData.AddEvent(m_CurrEvent);
    else
      levelData.eventData.AddEvent(m_CurrEvent);

    // chain new event to existing event
    // using normal chain, and set chain control
    // to always chain
    m_CurrEvent->control.chainTrigger = AlwaysChain; 
    UpdateControls(m_CurrEvent);
    ASSERT(m_CurrEvent->GetEventId() != origID); // no looping
    m_CurrEvent->ChainToEventId(0, origID);

    // if there is a parent event
    if (ParentId > 0)
    {
      GameEvent *ParentEvent;
      if (m_SingleEvent)
        ParentEvent = globalData.eventData.GetEvent(ParentId);
      else
        ParentEvent = levelData.eventData.GetEvent(ParentId);

      if (ParentEvent != NULL)
      {
        // chain parent to new event
        ASSERT(ParentEvent->GetEventId() != m_CurrEvent->GetEventId()); // no looping
        ParentEvent->ChainToEventId(chainIdx, m_CurrEvent->GetEventId());
      }
    }
    else
    {
      // new event is now root and not a chained event
      m_CurrEvent->x = m_x;
      m_CurrEvent->y = m_y;

      // make original root a chain event
      GameEvent *origEvent;
      if (m_SingleEvent)
        origEvent = globalData.eventData.GetEvent(origID);
      else
        origEvent = levelData.eventData.GetEvent(origID);

      if (origEvent != NULL)
      {
        origEvent->x = -1;
        origEvent->y = -1;
      }

      // if root step or rest event, update levelData
      switch (flags)    
      {
      case IS_REST_EVENT_ROOT:
      case IS_STEP_EVENT_ROOT:
      case IS_SINGLE_EVENT_ROOT:
      case IS_TIME_EVENT_ROOT:
        // shouldn't get here
        ASSERT(FALSE); // not sure, just put this here
        break;
      case IS_STEP_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.stepEvents[chainIdx].stepEvent = m_CurrEvent->GetEventId();
        break;
      case IS_TIME_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        //levelData.timeEvents[chainIdx].timeEvent = m_CurrEvent->GetEventId();
        break;
      case IS_REST_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.zoneData.zones[chainIdx].restEvent.restEvent = m_CurrEvent->GetEventId();
        break;
      case IS_SINGLE_EVENT:
        m_SingleEventRoot = m_CurrEvent->GetEventId();
        break;

      default:
        // do nothing
        break;
      }
    }

    PopulateTree(m_CurrEvent->id);
    UpdateControls(m_CurrEvent);
    SetControlStates();
    SET_MODIFIED;
  }
  else
  {
    // user hit cancel or creation failed
    if (m_CurrEvent != NULL)
    {
      WriteDebugString("Deleting '%s (%u)' event in EventViewer - canceled new event insert\n",
                       GetEventDescription(m_CurrEvent->GetEventType()),m_CurrEvent->GetEventId());
      delete m_CurrEvent;
      m_CurrEvent = NULL;
    }
  }
}

//*****************************************************************************
//    NAME: CEventViewer::GetParentEventId
//
// PURPOSE: 
//
//*****************************************************************************
DWORD CEventViewer::GetParentEventId(HTREEITEM hCurr)
{  
  if (hCurr == NULL)
    return 0;

  HTREEITEM hParent = m_EventTree.GetParentItem(hCurr);

  if (hParent != NULL)
  {
    DWORD id;
    BYTE chainIdx, flags;
    DWORD data = m_EventTree.GetItemData(hParent);
    SplitEventKey(data, id, chainIdx, flags);
    return id;
  }
  else
    return 0;
}

//*****************************************************************************
//    NAME: CEventViewer::GetChainIndex
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CEventViewer::GetChainIndex(HTREEITEM hCurr)
{  
  if (hCurr == NULL)
    return NOT_A_CHAIN;

  DWORD id;
  BYTE chainIdx, flags;
  DWORD data = m_EventTree.GetItemData(hCurr);
  SplitEventKey(data, id, chainIdx, flags);

  return chainIdx;
}

//*****************************************************************************
//    NAME: CEventViewer::GetFlags
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CEventViewer::GetFlags(HTREEITEM hCurr)
{  
  if (hCurr == NULL)
    return 0;

  DWORD id;
  BYTE chainIdx, flags;
  DWORD data = m_EventTree.GetItemData(hCurr);
  SplitEventKey(data, id, chainIdx, flags);

  return flags;
}

//*****************************************************************************
//    NAME: CEventViewer::GetEventId
//
// PURPOSE: 
//
//*****************************************************************************
DWORD CEventViewer::GetEventId(HTREEITEM hCurr)
{  
  if (hCurr == NULL)
    return 0;

  DWORD id;
  BYTE chainIdx, flags;
  DWORD data = m_EventTree.GetItemData(hCurr);
  SplitEventKey(data, id, chainIdx, flags);

  return id;
}

//*****************************************************************************
//    NAME: CEventViewer::MakeEventKey
//
// PURPOSE: 
//
//*****************************************************************************
DWORD CEventViewer::MakeEventKey(DWORD EventId, BYTE ChainIndex, BYTE Flags)
{
  EventId &= MAX_EVENTS; 
  ChainIndex &= 0x0F;
  Flags &= 0xF0;
  ChainIndex = (ChainIndex | Flags);

  DWORD key = ((ChainIndex << 24) | EventId);
  return key;
}

//*****************************************************************************
//    NAME: CEventViewer::SplitEventKey
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::SplitEventKey(DWORD key, DWORD &EventId, BYTE &ChainIndex, BYTE &Flags)
{
  EventId = (key & MAX_EVENTS);
  BYTE data = ((key & 0xFF000000) >> 24);
  ChainIndex = data & 0x0F;
  Flags = data & 0xF0;
}

//*****************************************************************************
//    NAME: CEventViewer::LookupCurrEvent
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::LookupCurrEvent()
{
  m_hCurrItem = m_EventTree.GetSelectedItem();

  if (m_hCurrItem != NULL)
  {
    DWORD data = m_EventTree.GetItemData(m_hCurrItem);
    
    DWORD id;
    BYTE chainIdx, flags;
    SplitEventKey(data, id, chainIdx, flags);

    m_CurrFlags = flags;

    if (id > 0)
    {
      if (m_SingleEvent)
        m_CurrEvent = globalData.eventData.GetEvent(id);
      else
        m_CurrEvent = levelData.eventData.GetEvent(id);
    }
    else
      m_CurrEvent = NULL;

     switch (m_CurrFlags)
     {
     case IS_TIME_EVENT_ROOT:
     case IS_STEP_EVENT_ROOT:
     case IS_REST_EVENT_ROOT:
     case IS_SINGLE_EVENT_ROOT:
     case IS_TIME_EVENT:
     case IS_STEP_EVENT:
     case IS_REST_EVENT:
     case IS_SINGLE_EVENT:
       m_x = -1; 
       m_y = -1;
       return; // these never have map coords
       break;
     default:
       // do nothing here
       break;
     }

    if (m_CurrEvent != NULL)
    {
      // update m_x and m_y with current data
      if ((m_CurrEvent->x != -1) && (m_CurrEvent->y != -1))
      {
        m_x = m_CurrEvent->x;
        m_y = m_CurrEvent->y;
      }
      else
      {
        // find the root event for this event so we
        // can update m_x and m_y
        
        BOOL found = FALSE;
        HTREEITEM hParent;
        HTREEITEM hItem = m_hCurrItem;
        do 
        {
          hParent = m_EventTree.GetParentItem(hItem);
          if (hParent != NULL)
              hItem = hParent;
        } while (hParent != NULL);

        if (hItem != NULL)
        {
          DWORD ParentId = GetEventId(hItem);
          GameEvent *ParentEvent;
          if (m_SingleEvent)          
            ParentEvent = globalData.eventData.GetEvent(ParentId);
          else
            ParentEvent = levelData.eventData.GetEvent(ParentId);

          if (ParentEvent != NULL)
          {
            if ((ParentEvent->x != -1) && (ParentEvent->y != -1))
            {
              found = TRUE;
              m_x = ParentEvent->x;
              m_y = ParentEvent->y;
            }
          }
        }
        // the parent should always be able to be found
        ASSERT(found);
      }
    }
    else
    {
      // This is used to track the map x,y coords associated
      // with an empty root tree item. The item itself does not store
      // it. There should really only be one such item to track with
      // each invocation of this dialog.
      // 
      // Ex: - User clicks on map square 2,1 with no event.
      //     - EventViewer starts up with m_x,m_y = 2,1.
      //     - EventViewer displays empty root tree node for 2,1.
      //     - Before adding event to root 2,1, user clicks on 
      //         another root tree item that does have an event.
      //     - This user action changes m_x,m_y to reflect values 
      //         for the selected event.
      //     - User clicks on empty root tree item for 2,1 again.
      //     - m_x,m_y still has values for existing event and there
      //         is no way to determine that x,y for empty root item
      //         should be 2,1 instead of x,y of existing event.
      //     - User creates new event, and now the new event will
      //         replace the previously existing event
      //
      // Solution: Keep track of the x,y for empty root items
      //
      DWORD val;
      if (!m_MapXY.Lookup(m_hCurrItem,val))
        m_MapXY[m_hCurrItem] = ((m_x << 16)|(m_y));
    }
  }
}

//*****************************************************************************
//    NAME: CEventViewer::OnOK
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnOK() 
{
	LookupCurrEvent();
  GetControls(m_CurrEvent);	
  DestroySearchDlg();
	CDialog::OnOK();
}


   
void CEventViewer::PopulateClassBaseclassList(GameEvent *pEvent)
{ 
  int indx;
  int currentIndex, currentSel;
  currentSel = m_Class.GetCurSel();
  currentIndex = m_Class.GetItemData(currentSel);
  if (m_CurrEvent == NULL) return;
  m_Class.ResetContent();
  switch (m_CurrEvent->GetEventControl().eventTrigger) 
  {      
  case ClassInParty:
  case ClassNotInParty:
    {
      int i, n;
      n = classData.GetCount();
      for (i=0; i<n; i++)
      {
        if (!(classData.PeekClass(i)->ClassID() == "$$Help"))
        {
          int idx;
          idx = m_Class.AddString(classData.PeekClass(i)->ClassID());
          m_Class.SetItemData(idx, i);
        };
      };
    }
    m_Class.EnableWindow(TRUE);
    {
      CWnd *x = GetDlgItem(IDC_Class_Baseclass);
      x->SetWindowText("Class");
    };
    indx = m_Class.FindStringExact(0, pEvent->control.classBaseclassID.classID);
    break;
  case BaseclassInParty:
  case BaseclassNotInParty:
    {
      int i, n;
      n = baseclassData.GetCount();
      for (i=0; i<n; i++)
      {
        if (!(baseclassData.PeekBaseclass(i)->BaseclassID() == "$$Help"))
        {
          int idx;
          idx = m_Class.AddString(baseclassData.PeekBaseclass(i)->BaseclassID());
          m_Class.SetItemData(idx, i);
        };
      };
    }
    m_Class.EnableWindow(TRUE);
    {
      CWnd *x = GetDlgItem(IDC_Class_Baseclass);
      x->SetWindowText("Baseclass");
    };
    indx = m_Class.FindStringExact(0, pEvent->control.classBaseclassID.baseclassID);
    break;
  default:      
    m_Class.EnableWindow(FALSE);
    return;
  };
  if (indx >= 0)
  {
    m_Class.SetCurSel(indx);
  }
  else
  {
    m_Class.SetCurSel(0);
  };
}




//*****************************************************************************
//    NAME: CEventViewer::OnInitDialog
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CEventViewer::OnInitDialog() 
{
   CDialog::OnInitDialog();
   DWORD i;
   int j, n;
   
   m_EventType.ResetContent();
   m_EventTrigger.ResetContent();
   m_ChainTrigger.ResetContent();
   m_Race.ResetContent();
   m_Class.ResetContent();
   m_Facing.ResetContent();
   m_Gender.ResetContent();
   
   for (i=0;i<NUM_EVENT_TYPES;i++)
   {
     BOOL used;
     if (m_SingleEvent)
       used = (    globalData.eventData.IsEventTypeUsed((eventType)i)
               &&  AllowedAsGlobalEvent((eventType)i));
     else
       used = levelData.eventData.IsEventTypeUsed((eventType)i);

      if (used)
      {
        int index = m_EventType.AddString(EventListText[i]);
        m_EventType.SetItemData(index, i);
      }
   }
            
   for (i=0;i<NUM_EVENTTRIG_TYPES;i++)
   {
      int index = m_EventTrigger.AddString(EventTriggerText[i]);
      m_EventTrigger.SetItemData(index, i);
   }
   
   for (i=0;i<NUM_CHAINTRIG_TYPES;i++)
      m_ChainTrigger.AddString(ChainTriggerText[i]);
   
   //for (i=0;i<NUM_RACE_TYPES;i++)
   //   m_Race.AddString(RaceText[i]);
   n = raceData.GetCount();
   for (j=0; j<n; j++)
   {
     const RACE_DATA *pRace;
     pRace = raceData.PeekRace(j);
     if (pRace->Name() == "$$Help") continue;
     m_Race.AddString(pRace->Name());
   };
   for (i=0;i<NUM_DIR_TYPES-1;i++) // exclude InFront dirType
      m_Facing.AddString(DirectionText[i]);  

   for (i=0;i<NUM_GENDER_TYPES;i++)
      m_Gender.AddString(genderText[i]);  
   
   m_EventType.SetCurSel(0);

   for (i=0;i<(unsigned)m_EventType.GetCount();i++)
   {
     if (m_EventType.GetItemData(i) == EditorOptionsData.m_LastEventTypeUsed)
       m_EventType.SetCurSel(i);
   }  

   m_EventTrigger.SetCurSel(0);
   m_Race.SetCurSel(0);
   m_Facing.SetCurSel(0);
   m_Class.SetCurSel(0);
   m_Gender.SetCurSel(0);
   m_ChainTrigger.SetCurSel(0);
   m_OnceOnly = FALSE;
   m_Chance = 100;
   m_PartyX = 0;
   m_PartyY = 0;

   PopulateTree(0);
   LookupCurrEvent();
   UpdateControls(m_CurrEvent);
   SetControlStates();

   GotoDlgCtrl(GetDlgItem(IDC_EVENT_TYPE));
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//*****************************************************************************
//    NAME: CEventViewer::OnDblclkEventtree
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnDblclkEventtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//LookupCurrEvent();
  //OnEditevent();
	*pResult = 0;
}

//*****************************************************************************
//    NAME: CEventViewer::OnSelchangedEventtree
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnSelchangedEventtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

  // get selected event
  LookupCurrEvent();

  // transfer event settings into
  // dialog controls
  UpdateControls(m_CurrEvent);

  // update button states
  SetControlStates();

  m_EventTree.EnsureVisible(m_EventTree.GetSelectedItem());

  srchpos = m_EventTree.GetSelectedItem();

	*pResult = 0;
}

//*****************************************************************************
//    NAME: CEventViewer::OnSelchangingEventtree
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnSelchangingEventtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  
  // get selected event
  LookupCurrEvent();

  // transfer current dialog settings
  // into selected event
  GetControls(m_CurrEvent);

	*pResult = 0;
}

//*****************************************************************************
//    NAME: CEventViewer::OnTriggerspeckey
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnTriggerspeckey() 
{
  // get selected event
  LookupCurrEvent();

  // transfer event settings into
  // dialog controls
  UpdateControls(m_CurrEvent);

  if (m_CurrEvent == NULL)
    return;

  CItemKeyQuestDlg dlg(KEY_FLAG);
	
  if (dlg.DoModal() == IDOK)
  {
    if (dlg.m_selectedID >= 0)
      m_CurrEvent->GetEventControl().specialKey = dlg.m_selectedID;
    else
      m_CurrEvent->GetEventControl().specialKey = -1;
  }	

	SetControlStates();	
}

//*****************************************************************************
//    NAME: CEventViewer::OnTriggerspecitem
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnTriggerspecitem() 
{
  // get selected event
  LookupCurrEvent();

  // transfer event settings into
  // dialog controls
  UpdateControls(m_CurrEvent);

  if (m_CurrEvent == NULL)
    return;

  CItemKeyQuestDlg dlg(ITEM_FLAG);

  if (dlg.DoModal() == IDOK)
  {
    if (dlg.m_selectedID >= 0)
      m_CurrEvent->GetEventControl().specialItem = dlg.m_selectedID;
    else
      m_CurrEvent->GetEventControl().specialItem = -1;
  }	

	SetControlStates();	
}

//*****************************************************************************
//    NAME: CEventViewer::OnTriggeritem
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnTriggeritem() 
{
  // get selected event
  LookupCurrEvent();

  // transfer event settings into
  // dialog controls
  UpdateControls(m_CurrEvent);

  if (m_CurrEvent == NULL)
    return;

   ITEM_LIST list;
   list.Clear();

   //ITEM_DATA *data = itemData.GetItemData(m_CurrEvent->GetEventControl().m_giID);
   ITEM_DATA *data = itemData.GetItem(m_CurrEvent->GetEventControl().itemID);

   if (data != NULL)
   {
     ITEM temp;

 	   //temp.m_giID = m_CurrEvent->GetEventControl().m_giID; 
 	   temp.itemID = m_CurrEvent->GetEventControl().itemID; 
	   temp.ready = FALSE;
	   temp.qty = max(1, data->Bundle_Qty);
	   temp.charges = data->Num_Charges;
	   temp.identified = FALSE;

     list.AddItem(temp);
   }

   CItemDlg dlg(list, 1);
   dlg.m_AllItemsAreIdentified = TRUE;

   UINT result = dlg.DoModal();
 
   if (result == IDOK)
   {
     dlg.GetItemList(list);
     if (list.GetCount() > 0)
     {
        POSITION pos = list.GetHeadPosition();
        //m_CurrEvent->GetEventControl().m_giID = list.PeekAtPos(pos).m_giID;
        m_CurrEvent->GetEventControl().itemID = list.PeekAtPos(pos).itemID;
     }
     else
       //m_CurrEvent->GetEventControl().m_giID.Clear();
       m_CurrEvent->GetEventControl().itemID.Clear();

   }

   SetControlStates();   
}

//*****************************************************************************
//    NAME: CEventViewer::OnQuesttrigger
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnQuesttrigger() 
{
  // get selected event
  LookupCurrEvent();

  // transfer event settings into
  // dialog controls
  UpdateControls(m_CurrEvent);

  if (m_CurrEvent == NULL)
    return;

  CItemKeyQuestDlg dlg(QUEST_FLAG);

  if (dlg.DoModal() == IDOK)
  {
    if (dlg.m_selectedID >= 0)
      m_CurrEvent->GetEventControl().quest = dlg.m_selectedID;
    else
      m_CurrEvent->GetEventControl().quest = -1;
  }	

  // update button states
	SetControlStates();	
}

//*****************************************************************************
//    NAME: CEventViewer::OnNpctrigger
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnNpctrigger() 
{
  // get selected event
  LookupCurrEvent();

  // transfer event settings into
  // dialog controls
  UpdateControls(m_CurrEvent);

  if (m_CurrEvent == NULL)
    return;

	//CChooseNPC dlg(m_CurrEvent->GetEventControl().npc);
	CChooseNPC dlg(m_CurrEvent->GetEventControl().characterID);
	
	if (dlg.DoModal() == IDOK)
    //m_CurrEvent->GetEventControl().npc = dlg.m_CharNum;
    m_CurrEvent->GetEventControl().characterID = dlg.m_characterID;

  // update button states
	SetControlStates();	
}

//*****************************************************************************
//    NAME: CEventViewer::OnConfigstepevents
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnConfigstepevents() 
{
  ASSERT (m_SingleEvent==FALSE);
  if (m_SingleEvent) return;
  LookupCurrEvent();
  BYTE idx = GetChainIndex(m_hCurrItem);
  
  CStepEventDlg dlg(levelData, idx+1); // always level data

  if (dlg.DoModal() == IDOK)
    dlg.GetData(levelData);
}

//*****************************************************************************
//    NAME: CEventViewer::OnConfigzones
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnConfigzones() 
{
  ASSERT (m_SingleEvent==FALSE);
  if (m_SingleEvent) return;
  LookupCurrEvent();
  BYTE idx = GetChainIndex(m_hCurrItem);

	CZoneDlg dlg(levelData.zoneData, idx); // always level data
	if (dlg.DoModal() == IDOK)
    dlg.GetData(levelData.zoneData);
}

//*****************************************************************************
//    NAME: CEventViewer::OnSelchangeChaintrigger
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnSelchangeChaintrigger() 
{   
  // get selected event
  LookupCurrEvent();
  if (m_CurrEvent == NULL) return;

  // lookup current chain event id
  DWORD EventId = 0;
  switch (m_CurrEvent->control.chainTrigger)
  {
  case AlwaysChain:
  case IfEventHappen:
    EventId = m_CurrEvent->chainEventHappen;
    break;
  case IfEventNotHappen:
    EventId = m_CurrEvent->chainEventNotHappen;
    break;
  }   
  
  // if there is a chain event
  if (EventId != NOT_A_CHAIN)
  {
    // get new chain trigger setting
    GetControls(m_CurrEvent);    
    // set chained event
    ASSERT(m_CurrEvent->GetEventId() != EventId); // no looping
    m_CurrEvent->ChainToEventId(0, EventId);
    // update gui
    UpdateControls(m_CurrEvent);
    SetControlStates();
  } 
}

//*****************************************************************************
//    NAME: CEventViewer::OnSelchangeEventtrigger
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnSelchangeEventtrigger() 
{
  // get selected event
  LookupCurrEvent();

  // transfer event settings into
  // dialog controls
  GetControls(m_CurrEvent);
  UpdateControls(m_CurrEvent);

  // update button states
  SetControlStates();
}

//*****************************************************************************
//    NAME: CEventViewer::UpdateControls
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::UpdateControls(GameEvent *pEvent)
{
  if (pEvent == NULL) return;


  DWORD currType = pEvent->GetEventType();
  int i=0, currSel=NoEvent;
  BOOL found = FALSE;

  while ((i<m_EventType.GetCount()) && (!found))
  {
    if (m_EventType.GetItemData(i) == currType)
    {
      currSel = i;
      found = TRUE;
    }
    i++;
  }

  m_EventType.SetCurSel(currSel);

  i=0;
  found = FALSE;
  currType = pEvent->GetEventControl().eventTrigger;

  while ((i<m_EventTrigger.GetCount()) && (!found))
  {
    if (m_EventTrigger.GetItemData(i) == currType)
    {
      currSel = i;
      found = TRUE;
    }
    i++;
  }

  m_EventTrigger.SetCurSel(currSel);

  //m_Race.SetCurSel((int)pEvent->GetEventControl().race);
  m_Race.SetCurSel(raceData.LocateRace(pEvent->GetEventControl().raceID));
  m_Facing.SetCurSel((int)pEvent->GetEventControl().facing);
  //m_Class.SetCurSel((int)pEvent->GetEventControl().charClass);
  //m_Class.SetCurSel(classData.LocateClass(pEvent->GetEventControl().classID));
  m_Gender.SetCurSel((int)pEvent->GetEventControl().gender);
  m_ChainTrigger.SetCurSel((int)pEvent->GetEventControl().chainTrigger);
  m_OnceOnly = pEvent->GetEventControl().onceOnly;
  m_Chance = pEvent->GetEventControl().chance;
  m_PartyX = pEvent->GetEventControl().partyX;
  m_PartyY = pEvent->GetEventControl().partyY;

  PopulateClassBaseclassList(pEvent);
  UpdateData(FALSE);
}

//*****************************************************************************
//    NAME: CEventViewer::GetControls
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::GetControls(GameEvent *event)
{
  if (event == NULL)
    return;

  // update member vars
  UpdateData(TRUE);

  // transfer to event vars
  event->control.onceOnly = m_OnceOnly;
  event->control.chance = m_Chance;
  event->control.partyX = m_PartyX;
  event->control.partyY = m_PartyY;

  int selection = m_ChainTrigger.GetCurSel();
  if (selection >= 0)
    event->control.chainTrigger = (chainTriggerType)selection;


  event->control.classBaseclassID.baseclassID.Empty();
  event->control.classBaseclassID.classID.Empty();
  selection = m_Class.GetCurSel();
  if (selection >= 0)
  {
    int data;
    data = m_Class.GetItemData(selection);
    if (    (event->control.eventTrigger == BaseclassInParty)
         || (event->control.eventTrigger == BaseclassNotInParty)
       )
    {
       event->control.classBaseclassID.baseclassID = baseclassData.PeekBaseclass(data)->BaseclassID();
    }
    else if (    (event->control.eventTrigger == ClassInParty)
              || (event->control.eventTrigger == ClassNotInParty)
            )
    {
       event->control.classBaseclassID.classID = classData.PeekClass(data)->ClassID();
    };      ;
  };


  selection = m_EventTrigger.GetCurSel();
  if (selection >= 0)
  {
    event->control.eventTrigger = (eventTriggerType) m_EventTrigger.GetItemData(selection);
  }

  selection = m_Facing.GetCurSel();
  if (selection >= 0)
    event->control.facing = (eventDirType) selection;

  selection = m_Race.GetCurSel();
  if (selection >= 0)
    //event->control.race = (raceType) selection;
    event->control.raceID = raceData.PeekRace(selection)->RaceID();

    //event->control.charClass = (classType) selection; 
  //  event->control.classID = classData.PeekClass(selection)->ClassID(); 

  selection = m_Gender.GetCurSel();
  if (selection >= 0)
    event->control.gender = (genderType) selection; 

  // set using other dialogs
  //event->control.item = m_ItemTrigger;
  //event->control.quest = m_QuestTrigger;
  //event->control.specialItem = m_SpItemTrigger;
  //event->control.specialKey = m_SpKeyTrigger;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: CEventViewer::PopulateTree
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::PopulateTree(DWORD SelectedEventId)
{  
  m_EventTree.DeleteAllItems();
  m_hCurrItem = NULL;
  m_CurrEvent = NULL;
  m_SelectedEventId = SelectedEventId;

  if (!m_SingleEvent)
  {
    int x, y;
    GameEvent *event;
    for (y=0; y < levelData.area_height; y++)
    {
      for (x=0; x < levelData.area_width; x++)
      {
      if ((event = levelData.eventData.GetFirstEvent(x, y)) != NULL)
        {
          if (!event->IsAChainEvent())
          {
            // there is a root event at this location
            levelData.area[y][x].eventExists = TRUE;
            PopulateTreeFromRootEvent(event);
          }
        }
        else // no events at this location
          levelData.area[y][x].eventExists = FALSE;
      }
    }

    if ((m_x >= 0) && (m_y >= 0))
    {
      // if no root event for this map location,
      // insert empty event label
      if (levelData.eventData.GetFirstEvent(m_x, m_y) == NULL)
          m_hCurrItem=AddBlankRoot(m_x, m_y);  
    }

    PopulateStepEvents();
    PopulateRestEvents();
    //PopulateTimeEvents();
  }
  else
    PopulateSingleEvents();

  if (m_hCurrItem != NULL)
  {
    m_EventTree.EnsureVisible(m_hCurrItem);
    m_EventTree.Select(m_hCurrItem, TVGN_CARET);
  }
}

//*****************************************************************************
//    NAME: CEventViewer::AddBlankRoot
//
// PURPOSE: 
//
//*****************************************************************************
HTREEITEM CEventViewer::AddBlankRoot(int x, int y)
{
  TVINSERTSTRUCT tvInsert;
  HTREEITEM hRoot;
  sprintf(RootText, "%s: %i,%i", (LPCSTR)GetEventDescription(NoEvent), x, y);

  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = RootText;
  hRoot = m_EventTree.InsertItem(&tvInsert);
  m_EventTree.SetItemData(hRoot, MakeEventKey(0,NOT_A_CHAIN));
  return hRoot;
}

//*****************************************************************************
//    NAME: CEventViewer::PopulateTreeFromRootEvent
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::PopulateTreeFromRootEvent(GameEvent *event)
{
  TVINSERTSTRUCT tvInsert;
  HTREEITEM hRoot;

  if (event == NULL)
    return;

  event->GetEVRootText(RootText);
  CString MapLoc;
  MapLoc.Format(": %u,%u", event->x, event->y);
  strcat(RootText, MapLoc);

  // Insert a root item using the structure. We must
  // initialize a TVINSERTSTRUCT structure and pass its
  // address to the call. 
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;  
  tvInsert.item.pszText = RootText;
  hRoot = m_EventTree.InsertItem(&tvInsert);
  m_EventTree.SetItemData(hRoot, MakeEventKey(event->id, NOT_A_CHAIN));

  if ((m_SelectedEventId > 0) && (m_SelectedEventId == event->id))
    m_hCurrItem = hRoot;

  PopulateTreeChildren(hRoot, event);

  // if no preference on selected event,
  // use root event at current map location
  if (m_SelectedEventId <= 0)
  {
    // root event at selected map square
    if ((event->x == m_x) && (event->y == m_y))
      m_hCurrItem = hRoot;
  }
}

TVINSERTSTRUCT PTC_Insert;

//*****************************************************************************
//    NAME: CEventViewer::PopulateTreeChildren
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::PopulateTreeChildren(HTREEITEM &rootTree, GameEvent *rootEvent, BYTE flags)
{
  BYTE index=0;
  DWORD id=0;
  HTREEITEM hCurr=NULL;
  GameEvent *PTC_ChainEvent;

  if ((rootEvent == NULL) || (rootTree == NULL))
    return;

  //rootEvent->GetEVRootText(RootText);
  //m_EventTree.SetItemText(rootTree, RootText);

  RootText[0] = '\0';
  // fill in all chain event labels for this node

  // this function returns true if there is a chain label
  // for the given index number 0..num chains
  while (rootEvent->GetEVChainText(index, RootText))
  {     
    if (strlen(RootText) <= 0)
      strcpy(RootText, "Unknown Event Name");

    PTC_Insert.hParent = rootTree;
    PTC_Insert.hInsertAfter = hCurr;
    PTC_Insert.item.mask = TVIF_TEXT;
    PTC_Insert.item.pszText = RootText;
    hCurr = m_EventTree.InsertItem(&PTC_Insert);    

    // this function returns the chain event id for the
    // given index. Zero means no event is chained.
    if (rootEvent->GetEVChain(index, id))
    { 
      // The following ASSERT was put here when it was discovered that
      // we were not always clearing the chain word when chained events
      // were deleted.  In particular, the Button chains of the encounter
      // events were not getting cleared.  I think there are a lot more.
      // In general, this will do no harm in the release version because
      // we have changed the ID assignment to only use each ID once (until
      // 16 million have been used!) and the code pretty much ignores
      // chained event IDs that point at nothing.
      // So...when debugging, you can ignore the ASSERTion failure if
      // you don't feel like fixing it.  No harm.
//#ifdef _DEBUG
      if (m_SingleEvent)
      {
        //ASSERT (globalData.eventData.IsValidEventChainWord(id)); // Zero is ok.
        if (!globalData.eventData.IsValidEventChainWord(id))
        {
          BOOL fix;
          fix = EventChainError("Single event has invvalid chain word", true);
          if (fix)
          {
            rootEvent->UnchainToEventId(index, id);
          };
        };
      }
      else
      {
        //ASSERT (levelData.eventData.IsValidEventChainWord(id)); // Zero is ok.
        if (!levelData.eventData.IsValidEventChainWord(id))
        {
          BOOL fix;
          fix = EventChainError("Non-single event has invvalid chain word", true);
          if (fix)
          {
            rootEvent->UnchainToEventId(index, id);
          };
        };
      };
//#endif
      if ((m_SelectedEventId > 0) && (m_SelectedEventId == id))
        m_hCurrItem = hCurr;

      // if a chained event exists, add a root placeholder for it,
      // then recurse through it's chained events.
      if (m_SingleEvent)
        PTC_ChainEvent = globalData.eventData.GetEvent(id);
      else
        PTC_ChainEvent = levelData.eventData.GetEvent(id);
      if (PTC_ChainEvent != NULL)
      {
        ASSERT(PTC_ChainEvent != rootEvent); // no looping
        // add event id to tree node data
        m_EventTree.SetItemData(hCurr, MakeEventKey(id, index, flags));
        PopulateTreeChildren(hCurr, PTC_ChainEvent, flags);
      }
      else // no event id for chain
        m_EventTree.SetItemData(hCurr, MakeEventKey(0, index, flags));
    }
    else  // no event id for chain
      m_EventTree.SetItemData(hCurr, MakeEventKey(0, index, flags));
    
    RootText[0] = '\0';
    index++;
  }
}

//*****************************************************************************
//    NAME: CEventViewer::PopulateStepEvents
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::PopulateStepEvents()
{
  ASSERT (m_SingleEvent==FALSE);
  if (m_SingleEvent) return;
  TVINSERTSTRUCT tvInsert;
  HTREEITEM hRoot, hChild;

  sprintf(RootText, "Step Events");
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = RootText;
  hRoot = m_EventTree.InsertItem(&tvInsert);
  m_EventTree.SetItemData(hRoot, MakeEventKey(0,NOT_A_CHAIN,IS_STEP_EVENT_ROOT));

  RootText[0] = '\0';
  hChild = NULL;
  DWORD id;

  for (int i=0;i<MAX_STEP_EVENTS; i++)
  {
    levelData.GetStepEventText(i,RootText);

    tvInsert.hParent = hRoot;
    tvInsert.hInsertAfter = hChild;
    tvInsert.item.mask = TVIF_TEXT;
    tvInsert.item.pszText = RootText;
    hChild = m_EventTree.InsertItem(&tvInsert);

    id = levelData.stepEvents[i].stepEvent;

    if ((m_SelectedEventId > 0) && (m_SelectedEventId == id))
        m_hCurrItem = hChild;

    m_EventTree.SetItemData(hChild, MakeEventKey(id,i, IS_STEP_EVENT));
    PopulateTreeChildren(hChild, levelData.eventData.GetEvent(id), IS_STEP_EVENT);

    RootText[0] = '\0';
  }   
}

//*****************************************************************************
//    NAME: CEventViewer::PopulateRestEvents
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::PopulateRestEvents()
{
  ASSERT (m_SingleEvent==FALSE);
  if (m_SingleEvent) return;
  TVINSERTSTRUCT tvInsert;
  HTREEITEM hRoot, hChild;

  sprintf(RootText, "Rest Events");
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = RootText;
  hRoot = m_EventTree.InsertItem(&tvInsert);
  m_EventTree.SetItemData(hRoot, MakeEventKey(0,NOT_A_CHAIN,IS_REST_EVENT_ROOT));

  RootText[0] = '\0';
  hChild = NULL;
  DWORD id;

  for (int i=0;i<MAX_ZONES; i++)
  {
    levelData.GetRestEventText(i,RootText);

    tvInsert.hParent = hRoot;
    tvInsert.hInsertAfter = hChild;
    tvInsert.item.mask = TVIF_TEXT;
    tvInsert.item.pszText = RootText;
    hChild = m_EventTree.InsertItem(&tvInsert);

    id = levelData.zoneData.zones[i].restEvent.restEvent;

    if ((m_SelectedEventId > 0) && (m_SelectedEventId == id))
        m_hCurrItem = hChild;

    m_EventTree.SetItemData(hChild, MakeEventKey(id,i, IS_REST_EVENT));
    PopulateTreeChildren(hChild, levelData.eventData.GetEvent(id), IS_REST_EVENT);
    RootText[0] = '\0';
  }   
}

//*****************************************************************************
//    NAME: CEventViewer::PopulateTimeEvents
//
// PURPOSE: 
//
//*****************************************************************************
/*
void CEventViewer::PopulateTimeEvents()
{
  ASSERT (m_SingleEvent==FALSE);
  if (m_SingleEvent) return;
  TVINSERTSTRUCT tvInsert;
  HTREEITEM hRoot, hChild;
  
  sprintf(RootText, "Time Events");
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = RootText;
  hRoot = m_EventTree.InsertItem(&tvInsert);
  m_EventTree.SetItemData(hRoot, MakeEventKey(0,NOT_A_CHAIN,IS_TIME_EVENT_ROOT));
  
  RootText[0] = '\0';
  hChild = NULL;
  DWORD id;
  
  for (int i=0;i<MAX_TIME_EVENTS; i++)
  {
    levelData.GetTimeEventText(i,RootText);
    
    tvInsert.hParent = hRoot;
    tvInsert.hInsertAfter = hChild;
    tvInsert.item.mask = TVIF_TEXT;
    tvInsert.item.pszText = RootText;
    hChild = m_EventTree.InsertItem(&tvInsert);
    
    id = levelData.timeEvents[i].timeEvent;
    
    if ((m_SelectedEventId > 0) && (m_SelectedEventId == id))
      m_hCurrItem = hChild;
    
    m_EventTree.SetItemData(hChild, MakeEventKey(id,i, IS_TIME_EVENT));
    PopulateTreeChildren(hChild, levelData.eventData.GetEvent(id), IS_TIME_EVENT);
    
    RootText[0] = '\0';
  }   
}
*/
//*****************************************************************************
//    NAME: CEventViewer::PopulateSingleEvents
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::PopulateSingleEvents()
{
  ASSERT(m_SingleEvent == TRUE);
  if (!m_SingleEvent) return;
  TVINSERTSTRUCT tvInsert;
  HTREEITEM hRoot, hChild;

  strcpy(RootText, m_SingleEventRootLabel);
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = RootText;
  hRoot = m_EventTree.InsertItem(&tvInsert);
  m_EventTree.SetItemData(hRoot, MakeEventKey(0,NOT_A_CHAIN,IS_SINGLE_EVENT_ROOT));

  RootText[0] = '\0';
  hChild = NULL;

  DWORD id = m_SingleEventRoot;
  GameEvent *pEvent = globalData.eventData.GetEvent(id);
  if (pEvent == NULL)
    strcpy(RootText, GetEventDescription(NoEvent));
  else
    pEvent->GetEVRootText(RootText);

  tvInsert.hParent = hRoot;
  tvInsert.hInsertAfter = hChild;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = RootText;
  hChild = m_EventTree.InsertItem(&tvInsert);
  
  m_EventTree.SetItemData(hChild, MakeEventKey(id,0, IS_SINGLE_EVENT));
  PopulateTreeChildren(hChild, globalData.eventData.GetEvent(id), IS_SINGLE_EVENT);
 
  m_hCurrItem = hRoot;
}

//*****************************************************************************
//    NAME: CEventViewer::SetControlStates
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::SetControlStates(void)
{
   m_ItemTrigger.EnableWindow(FALSE);
   m_SpItemTrigger.EnableWindow(FALSE);
   m_SpKeyTrigger.EnableWindow(FALSE);
   m_QuestTrigger.EnableWindow(FALSE);
   m_NPCTrigger.EnableWindow(FALSE);
   m_Race.EnableWindow(FALSE);
   m_Facing.EnableWindow(FALSE);
   m_Class.EnableWindow(FALSE);  
   m_Gender.EnableWindow(FALSE);
   m_Delete.EnableWindow(FALSE);
   m_Insert.EnableWindow(FALSE);
   m_EventType.EnableWindow(TRUE);
   m_ConfigStepEvents.EnableWindow(FALSE);
   m_ConfigZones.EnableWindow(FALSE);   
   m_GPDLTrigger.EnableWindow(FALSE);
   m_SpellTrigger.EnableWindow(FALSE);

   if (CopyEventData.GetCount() > 0)
     m_Paste.EnableWindow(TRUE);
   else
     m_Paste.EnableWindow(FALSE);

   CWnd *pChanceWnd = GetDlgItem(IDC_PCNTCHANCE);
   if (pChanceWnd != NULL)
      pChanceWnd->EnableWindow(FALSE);

   CWnd *pPartyXWnd = GetDlgItem(IDC_PARTYXTRIGGER);
   if (pPartyXWnd != NULL)
     pPartyXWnd->EnableWindow(FALSE);

   CWnd *pPartyYWnd = GetDlgItem(IDC_PARTYYTRIGGER);
   if (pPartyYWnd != NULL)
     pPartyYWnd->EnableWindow(FALSE);
   
   m_Edit.SetWindowText("Add");
   
   switch (m_CurrFlags)
   {
   case IS_TIME_EVENT_ROOT:
   case IS_STEP_EVENT_ROOT:
   case IS_REST_EVENT_ROOT:
   case IS_SINGLE_EVENT_ROOT:
     m_Edit.EnableWindow(FALSE);
     m_Paste.EnableWindow(FALSE);
     break;
   case IS_TIME_EVENT:
     m_Edit.EnableWindow(TRUE);
     //m_ConfigStepEvents.EnableWindow(TRUE);
     break;
   case IS_STEP_EVENT:
     m_Edit.EnableWindow(TRUE);
     m_ConfigStepEvents.EnableWindow(TRUE);
     break;
   case IS_REST_EVENT:
     m_Edit.EnableWindow(TRUE);
     m_ConfigZones.EnableWindow(TRUE);
     break;
   case IS_SINGLE_EVENT:
     m_Edit.EnableWindow(TRUE);
     break;

   default:
     m_Edit.EnableWindow(TRUE);
     break;
   }

   if (m_CurrEvent == NULL)
   {
     m_Chance = 100;
     m_PartyX = 0;
     m_PartyY = 0;
     UpdateData(FALSE);
     return;
   }

   if (m_CurrEvent->GetEventType() != NoEvent)
   {
     // there is an event here
     m_Delete.EnableWindow(TRUE);
     m_Insert.EnableWindow(TRUE);
     m_Edit.SetWindowText("Edit");
   }

   switch (m_CurrEvent->GetEventControl().eventTrigger) 
   {      
   case RandomChance:
     //m_CurrEvent->GetEventControl().m_giID.Clear();
     m_CurrEvent->GetEventControl().itemID.Clear();
     m_CurrEvent->GetEventControl().specialItem = -1;
     m_CurrEvent->GetEventControl().specialKey = -1;
     m_CurrEvent->GetEventControl().quest = -1;
     m_CurrEvent->GetEventControl().partyX = 0;
     m_CurrEvent->GetEventControl().partyY = 0;

     if (pChanceWnd != NULL)
       pChanceWnd->EnableWindow(TRUE);
     break;

   case PartyAtXY:
     //m_CurrEvent->GetEventControl().m_giID.Clear();
     m_CurrEvent->GetEventControl().itemID.Clear();
     m_CurrEvent->GetEventControl().specialItem = -1;
     m_CurrEvent->GetEventControl().specialKey = -1;
     m_CurrEvent->GetEventControl().quest = -1;
     m_CurrEvent->GetEventControl().chance = 100;

     if (pPartyXWnd != NULL)
       pPartyXWnd->EnableWindow(TRUE);
     if (pPartyYWnd != NULL)
       pPartyYWnd->EnableWindow(TRUE);
     break;

   case PartyHaveItem:
   case PartyNotHaveItem:
      m_ItemTrigger.EnableWindow(TRUE);
      //m_SpItemTrigger.EnableWindow(TRUE);
      //m_SpKeyTrigger.EnableWindow(TRUE);
      //m_QuestTrigger.EnableWindow(TRUE);
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      break;
      
   case PartyHaveSpecialItem:
   case PartyNotHaveSpecialItem:
      //m_ItemTrigger.EnableWindow(TRUE);
      m_SpItemTrigger.EnableWindow(TRUE);
      //m_SpKeyTrigger.EnableWindow(TRUE);
      //m_QuestTrigger.EnableWindow(TRUE);
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      break;
      
   case PartyHaveSpecialKey:
   case PartyNotHaveSpecialKey:
      //m_ItemTrigger.EnableWindow(TRUE);
      //m_SpItemTrigger.EnableWindow(TRUE);
      m_SpKeyTrigger.EnableWindow(TRUE);
      //m_QuestTrigger.EnableWindow(TRUE);
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      break;
      
   case FacingDirection:
   case FacingDirectionAnyTime:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      m_Facing.EnableWindow(TRUE);
      break;
      
   case QuestComplete:
   case QuestFailed:
   case QuestInProgress:
   case QuestPresent:
   case QuestNotPresent:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      //m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      m_QuestTrigger.EnableWindow(TRUE);
      break;
      
   case NPCInParty:
   case NPCNotInParty:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
     m_NPCTrigger.EnableWindow(TRUE);
     break;

   case ClassInParty:
   case ClassNotInParty:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      m_Class.EnableWindow(TRUE);
      {
        CWnd *x = GetDlgItem(IDC_Class_Baseclass);
        x->SetWindowText("Class");
      };
      break;
      
   case BaseclassInParty:
   case BaseclassNotInParty:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      m_Class.EnableWindow(TRUE);
      {
        CWnd *x = GetDlgItem(IDC_Class_Baseclass);
        x->SetWindowText("Baseclass");
      };
      break;
      
   case RaceInParty:
   case RaceNotInParty:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      m_Race.EnableWindow(TRUE);
      break;

   case GenderInParty:
   case GenderNotInParty:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
      m_Gender.EnableWindow(TRUE);
     break;

   case ExecuteGPDL:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
     m_GPDLTrigger.EnableWindow(TRUE);
     break;

   case SpellMemorized:
     //m_CurrEvent->GetEventControl().m_giID.Clear();
     m_CurrEvent->GetEventControl().itemID.Clear();
     m_CurrEvent->GetEventControl().specialItem = -1;
     m_CurrEvent->GetEventControl().specialKey = -1;
     m_CurrEvent->GetEventControl().quest = -1;
     m_CurrEvent->GetEventControl().chance = 100;
     m_CurrEvent->GetEventControl().partyX = 0;
     m_CurrEvent->GetEventControl().partyY = 0;
     m_SpellTrigger.EnableWindow(TRUE);
     break;

   default:
      //m_CurrEvent->GetEventControl().m_giID.Clear();
      m_CurrEvent->GetEventControl().itemID.Clear();
      m_CurrEvent->GetEventControl().specialItem = -1;
      m_CurrEvent->GetEventControl().specialKey = -1;
      m_CurrEvent->GetEventControl().quest = -1;
      m_CurrEvent->GetEventControl().chance = 100;
      m_CurrEvent->GetEventControl().partyX = 0;
      m_CurrEvent->GetEventControl().partyY = 0;
     break;
   }  

   char name[MAX_MEDITBUTTON_TEXT+1];

   //if (m_CurrEvent->GetEventControl().m_giID.IsValidItem())
 	 //  getBaseName(itemData.GetItemIdName(m_CurrEvent->GetEventControl().m_giID), name, MAX_MEDITBUTTON_TEXT); 
   if (m_CurrEvent->GetEventControl().itemID.IsValidItem())
 	   getBaseName(itemData.GetItemIdName(m_CurrEvent->GetEventControl().itemID), name, MAX_MEDITBUTTON_TEXT); 
   else
     strcpy(name, "No Item");
   
   m_ItemTrigger.SetWindowText(name);

   if (m_CurrEvent->GetEventControl().specialItem >= 0)
 	  getBaseName(GetSpecialItemName(m_CurrEvent->GetEventControl().specialItem),name, MAX_MEDITBUTTON_TEXT);
   else
     strcpy(name, "No Item");

   m_SpItemTrigger.SetWindowText(name);

   if (m_CurrEvent->GetEventControl().specialKey >= 0)
    getBaseName(GetSpecialKeyName(m_CurrEvent->GetEventControl().specialKey), name, MAX_MEDITBUTTON_TEXT);   
   else
     strcpy(name, "No Item");

   m_SpKeyTrigger.SetWindowText(name);

   if (m_CurrEvent->GetEventControl().quest >= 0)
    getBaseName(globalData.questData.GetName(m_CurrEvent->GetEventControl().quest), name, MAX_MEDITBUTTON_TEXT);   
   else
     strcpy(name, "No Quest");

   m_QuestTrigger.SetWindowText(name);

   //if (globalData.charData.HaveKey(m_CurrEvent->GetEventControl().npc))
   if (globalData.charData.LocateCharacter(m_CurrEvent->GetEventControl().characterID) >= 0)
     getBaseName(globalData.charData.GetCharacterName(m_CurrEvent->GetEventControl().characterID), name, MAX_MEDITBUTTON_TEXT);    
   else
     strcpy(name, "Nobody");

   m_NPCTrigger.SetWindowText(name);

  //if (m_CurrEvent->GetEventControl().memorizedSpellDbKey.IsValidSpell())
  // getBaseName(spellData.GetSpellName(m_CurrEvent->GetEventControl().memorizedSpellDbKey), name, MAX_MEDITBUTTON_TEXT);
  if (m_CurrEvent->GetEventControl().memorizedSpellID.IsValidSpell())
   getBaseName(spellData.GetSpellName(m_CurrEvent->GetEventControl().memorizedSpellID), name, MAX_MEDITBUTTON_TEXT);
  else
    strcpy(name, "No Spell");

  m_SpellTrigger.SetWindowText(name);     

  UpdateData(FALSE);
}


//*****************************************************************************
//    NAME: CEventViewer::OnDeleteallchained
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnDeleteallchained() 
{
  m_DeleteAllChained = !m_DeleteAllChained;	
}

//*****************************************************************************
//    NAME: CEventViewer::ExpandChildren
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::ExpandChildren(HTREEITEM hParent)
{
  if (m_EventTree.ItemHasChildren(hParent))
  {
     HTREEITEM hNextItem;
     HTREEITEM hChildItem = m_EventTree.GetChildItem(hParent);
   
     while (hChildItem != NULL)
     {
        hNextItem = m_EventTree.GetNextItem(hChildItem, TVGN_NEXT);
        m_EventTree.Expand(hChildItem, TVE_EXPAND);
        ExpandChildren(hChildItem);
        hChildItem = hNextItem;
     }
  }
}

//*****************************************************************************
//    NAME: CEventViewer::CollapseChildren
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::CollapseChildren(HTREEITEM hParent)
{
  if (m_EventTree.ItemHasChildren(hParent))
  {
     HTREEITEM hNextItem;
     HTREEITEM hChildItem = m_EventTree.GetChildItem(hParent);
   
     while (hChildItem != NULL)
     {
        hNextItem = m_EventTree.GetNextItem(hChildItem, TVGN_NEXT);
        m_EventTree.Expand(hChildItem, TVE_COLLAPSE);
        CollapseChildren(hChildItem);
        hChildItem = hNextItem;
     }
  }
}

//*****************************************************************************
//    NAME: CEventViewer::OnExpandall
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnExpandall() 
{
  HTREEITEM hParent = m_hCurrItem;
  m_EventTree.Expand(hParent, TVE_EXPAND);
  ExpandChildren(hParent);
  m_EventTree.EnsureVisible(hParent);
}

//*****************************************************************************
//    NAME: CEventViewer::OnCollapsall
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnCollapsall() 
{
  HTREEITEM hParent = m_hCurrItem;
  m_EventTree.Expand(hParent, TVE_COLLAPSE);
  CollapseChildren(hParent);
  m_EventTree.EnsureVisible(hParent);
}

//*****************************************************************************
//    NAME: CEventViewer::OnPasteevents
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnPasteevents() 
{
  if (m_hCurrItem == NULL) return;  
	if (CopyEventData.GetCount() <= 0) return;

  // some things to keep in mind when pasting events
  //
  // - the pasted event can be a single event or an entire chain of events
  //
  // - if the pasted event was a chain, and it is pasted to the root, the
  //   x,y,chainflags must be adjusted to indicate it's root status
  //
  // - if pasted event was a root, and it is becoming a chained event, the
  //   pasted event's x,y,chainflags must be adjusted to indicate it's chain status
  //
  // - if the pasted event is being inserted at the root, any existing events
  //   at the root must be chained to the last event in the pasted chain.
  //
  // - if the pasted event is being added to the end of a chain, the pasted event
  //   must be chained properly to the existing event.
  //
  // - if the pasted event is inserted between two events, the pasted event
  //   must be chained to the parent event, and the previous child event must
  //   be chained to the last event in the pasted event chain.
  //
  // - event id's in the CopyEventData list have no meaning in the
  //   destination event list, as the events are copied to the dest list
  //   new events are created with new id's.
  //
  // - if the pasted events become root in a rest/step/time chain, update the
  //   zone/step/time event records

  DWORD DstParentId;
  DWORD DstChildId;  
  DWORD SrcParentId;
  DWORD NewDstParentId;
  DWORD TailId;
  GameEvent *pDstParent=NULL;
  GameEvent *pTail=NULL;
  GameEvent *pSrcParent=NULL;
  GameEvent *pNewDstParent=NULL;

  POSITION pos = CopyEventData.GetHeadPosition();
  if (pos==NULL) return;

  // get the parent/child id's for paste insertion point
  DstParentId = GetParentEventId(m_hCurrItem);
  BYTE chainIdx = GetChainIndex(m_hCurrItem);
  if (m_CurrEvent!=NULL) 
    DstChildId=m_CurrEvent->GetEventId();
  else
    DstChildId=NO_EVENT;

  pSrcParent = CopyEventData.GetAtPos(pos);
  if (pSrcParent==NULL) return;

  pSrcParent->x=-1;
  pSrcParent->y=-1;
  SrcParentId = pSrcParent->GetEventId();

  // add pasted events to destination event list, chaining pasted events
  // to parent event
  if (m_SingleEvent)
  {
    NewDstParentId=DstParentId;
    pDstParent = globalData.eventData.GetEvent(DstParentId);
    globalData.eventData.CopyEvent(NewDstParentId, SrcParentId, &CopyEventData);
    pNewDstParent = globalData.eventData.GetEvent(NewDstParentId);
    ASSERT(pNewDstParent!=NULL);
    // get the tail id for pasted chain
    TailId = globalData.eventData.GetLastEventInNormalChain(NewDstParentId);
    pTail = globalData.eventData.GetEvent(TailId);
  }
  else
  {
    NewDstParentId=DstParentId;
    pDstParent = levelData.eventData.GetEvent(DstParentId);
    levelData.eventData.CopyEvent(NewDstParentId, SrcParentId, &CopyEventData);
    pNewDstParent = levelData.eventData.GetEvent(NewDstParentId);
    ASSERT(pNewDstParent!=NULL);
    // get the tail id for pasted chain
    TailId = levelData.eventData.GetLastEventInNormalChain(NewDstParentId);
    pTail = levelData.eventData.GetEvent(TailId);
  }

  // chain copied events into existing event chain  
  if (pDstParent!=NULL)
  {
    // chain copied event chain to parent event
    pDstParent->ChainToEventId(chainIdx, NewDstParentId);
    WriteDebugString("Pasting event chain to parent id %u, chain index %u\n",
                     NewDstParentId, chainIdx);
  }
  else
  {
    ASSERT(pNewDstParent!=NULL);

    // no parent, make the copied event a root    
    if (pNewDstParent!=NULL)
    {
      WriteDebugString("Pasting event chain as root, chain index %u\n", chainIdx);

      pNewDstParent->x = m_x;
      pNewDstParent->y = m_y;

      BYTE flags = GetFlags(m_hCurrItem);

      // if root step or rest event, update levelData
      switch (flags)    
      {
      case IS_TIME_EVENT_ROOT:
      case IS_REST_EVENT_ROOT:
      case IS_STEP_EVENT_ROOT:
      case IS_SINGLE_EVENT_ROOT:
        // shouldn't get here
        ASSERT(FALSE); // not sure, just put this here
        break;
      case IS_TIME_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        //levelData.timeEvents[chainIdx].timeEvent = pNewDstParent->GetEventId();
        break;
      case IS_STEP_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.stepEvents[chainIdx].stepEvent = pNewDstParent->GetEventId();
        break;
      case IS_REST_EVENT:
        ASSERT(chainIdx!=NOT_A_CHAIN);
        ASSERT (m_SingleEvent==FALSE);
        levelData.zoneData.zones[chainIdx].restEvent.restEvent = pNewDstParent->GetEventId();
        break;
      case IS_SINGLE_EVENT:
        m_SingleEventRoot = pNewDstParent->GetEventId();
        break;

      default:
        // do nothing
        break;
      }
    }
  }

  // 
  // chain tail event of pasted chain to child event
  //
  // the tail in this case will always point to a
  // normal chain event, and will be the last event
  // in the pasted chain which follows the normal
  // chain trail. No other chains are followed to
  // determine the tail event, just normal chains.
  //
  if ((pTail!=NULL) && (DstChildId!=NO_EVENT))
  {
    GameEvent *pChildEvent;
    if (m_SingleEvent)
      pChildEvent = globalData.eventData.GetEvent(DstChildId);
    else
      pChildEvent = levelData.eventData.GetEvent(DstChildId);

    ASSERT(pChildEvent!=NULL);
    if (pChildEvent!=NULL)
    {
      ASSERT(pTail->GetEventId() != pChildEvent->GetEventId()); // no looping
      // it's a chained event now
      pChildEvent->x=-1;
      pChildEvent->y=-1;
      pTail->ChainToEventId(0,DstChildId); // use normal chain  
    }
  }

  PopulateTree(NewDstParentId);
  UpdateControls(pNewDstParent);
  LookupCurrEvent();
  SetControlStates();
  SET_MODIFIED;
}

//*****************************************************************************
//    NAME: CEventViewer::OnCopyevents
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnCopyevents() 
{
  // must be pointing to a tree node before event
  // copy can occur
  //
  if (m_hCurrItem == NULL) return;  
  if (m_CurrEvent == NULL) return;

  CopyEventData.Clear();
  CopyEventData.SetEventSource(CopyEventSrc);
  DWORD RootId = NO_EVENT;
  GameEventList *pList;

  if (m_SingleEvent)
    pList = &globalData.eventData;
  else
    pList = &levelData.eventData;
  
  CopyEventData.CopyEvent(RootId, m_CurrEvent->GetEventId(), pList);

  if (CopyEventData.GetCount() > 0)
  {
    m_Paste.EnableWindow(TRUE);
    WriteDebugString("CopyEventData has %u events\n", CopyEventData.GetCount());
  }
  else
    m_Paste.EnableWindow(FALSE);
}


//*****************************************************************************
//    NAME: CEventViewer::OnGpdltrigger
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnGpdltrigger() 
{
  // get selected event
  LookupCurrEvent();
  // transfer event settings into
  // dialog controls
  UpdateControls(m_CurrEvent);
  if (m_CurrEvent == NULL) return;
 
  CString script;
  CString start("$PUBLIC $FUNC evcntrltest() { ");
  CString end(" } evcntrltest;");
  CString func = "evcntrltest";

  ScriptList list;
  list.SetSize(1);
  list[0].name = "Event Trigger Script";
  list[0].script = m_CurrEvent->GetEventControl().gpdlData;

  CEditText dlg(start, end, func, true, list);
  dlg.m_ScriptFilter = EVENT_USAGE;

  if (dlg.DoModal()==IDOK)
    m_CurrEvent->GetEventControl().gpdlData = dlg.m_list[0].script;

  // update button states
	SetControlStates();	
}

//*****************************************************************************
//    NAME: CEventViewer::GetTreeRoot
//
// PURPOSE: 
//
//*****************************************************************************
HTREEITEM CEventViewer::GetTreeRoot(HTREEITEM hCurrItem)
{
  if (hCurrItem == NULL)
    return NULL;
  
  HTREEITEM hParent;
  HTREEITEM hItem = hCurrItem;
  do 
  {
    hParent = m_EventTree.GetParentItem(hItem);
    if (hParent != NULL)
        hItem = hParent;
  } while (hParent != NULL);

  return hItem;
}

//*****************************************************************************
//    NAME: EventViewerSearchCallback
//
// PURPOSE: 
//
//*****************************************************************************
void EventViewerSearchCallback(CString value, WORD type, DWORD userdata)
{
  static CString SrchValue;

  SrchValue=value;
  HWND hWnd = (HWND)userdata;

  ::PostMessage(hWnd, 
                MSG_FINDNEXT, 
                (WPARAM)&SrchValue, 
                (LPARAM)type);
    
}

//*****************************************************************************
//    NAME: CEventViewer::OnSearch
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnSearch() 
{
  srchpos = NULL;
  CSearchParamDlg *pSearchDlg = GetSearchDlg(ENABLE_EVENT_SRCH_FLAGS);
  pSearchDlg->SetSearchCallback(EventViewerSearchCallback, (DWORD)this->m_hWnd);
  pSearchDlg->m_ShowEventFlags=true;
  pSearchDlg->ShowWindow(SW_SHOWNORMAL);
  pSearchDlg->SetForegroundWindow();
}

//*****************************************************************************
//    NAME: CEventViewer::AdvanceToNextSearchTree
//
// PURPOSE: 
//
//*****************************************************************************
HTREEITEM CEventViewer::AdvanceToNextSearchTree(HTREEITEM hCurrItem)
{
  HTREEITEM hItem = GetTreeRoot(hCurrItem);
  if (hItem != NULL)
  {
    // found the root of this branch, now find sibling root
    hItem = m_EventTree.GetNextSiblingItem(hItem);
  }
  return hItem;
}

//*****************************************************************************
//    NAME: CEventViewer::AdvanceToNextSearchItem
//
// PURPOSE: 
//
//*****************************************************************************
HTREEITEM CEventViewer::AdvanceToNextSearchItem(HTREEITEM hCurrItem)
{
  // advance to next item, so next Find operation won't
  // simply return the same item we found this time
  // (since srchpos is static)
  //
  if (hCurrItem != NULL)
  {
    if (m_EventTree.ItemHasChildren(hCurrItem))
      hCurrItem = m_EventTree.GetNextItem(hCurrItem,TVGN_CHILD);
    else
      hCurrItem = m_EventTree.GetNextSiblingItem(hCurrItem);
  }

  // after all this, if srchpos is still NULL, reset it
  // back to the root node
  //if (srchpos == NULL)
  //  srchpos = m_EventTree.GetRootItem();

  return hCurrItem;
}

//*****************************************************************************
//    NAME: CEventViewer::OnMsgFindNext
//
// PURPOSE: 
//
//*****************************************************************************
LRESULT CEventViewer::OnMsgFindNext(WPARAM wp, LPARAM lp) 
{  
  CString *pValue = (CString*)wp;
  WORD type = (WORD)lp;

  HTREEITEM MarkPoint = m_EventTree.GetRootItem();
  HTREEITEM StartPos = srchpos;
  int circle = 0;

  if (StartPos == NULL) // start search at selected item
    StartPos = m_EventTree.GetSelectedItem();

  bool found = false;
  while ((StartPos != NULL) && (!found))
  {
    if (MarkPoint == StartPos)
      circle++;

    if (circle >= 2)
    {
      // searched full circle, so bail out
      MessageBox("Search text not found", "Search Result");
      srchpos=NULL;
      break;
    }

    StartPos = FindItemFromString(StartPos, *pValue, type);

    if (StartPos != NULL)
    {
      m_EventTree.EnsureVisible(StartPos);
      m_EventTree.Select(StartPos, TVGN_CARET); 
      found = true;

      // try for child item, if present
      StartPos = AdvanceToNextSearchItem(StartPos);

      // else find root of next tree
      if (StartPos == NULL)
        StartPos = AdvanceToNextSearchTree(srchpos);
    }
    else
    {
      // no child item matches, go to next tree
      StartPos = AdvanceToNextSearchTree(srchpos);
      srchpos = StartPos;

      if (srchpos == NULL)
      {
        // no matches found from selection to end of list
        // reset to start
        srchpos = m_EventTree.GetRootItem();
        StartPos = srchpos;
      }
    }
  }

  srchpos = StartPos;
  
  if (srchpos == NULL)
  {
    
  }

  return 1;
}

//*****************************************************************************
//    NAME: IsSearchTextAMatch
//
// PURPOSE: 
//
//*****************************************************************************
BOOL IsSearchTextAMatch(CString &str1, CString &str2, WORD type)
{
  CString strTemp1(str1);
  CString strTemp2(str2);

  if (!MatchCaseInSearch(type))
  {
    strTemp1.MakeLower();
    strTemp2.MakeLower();
  }          

  if (MatchEntireInSearch(type))
  {
    if (strTemp2.Compare(strTemp1) == 0)
        return TRUE;          
  }
  else
  {
    if (strTemp2.Find(strTemp1) >= 0)
      return TRUE;
  }          
  return FALSE;
}

//*****************************************************************************
//    NAME: CEventViewer::FindItemFromString
//
// PURPOSE: 
//
//*****************************************************************************
HTREEITEM CEventViewer::FindItemFromString(HTREEITEM hItem, LPCTSTR szItemName, WORD type)
{
    if (hItem == NULL)
      return NULL;

    CString szBuffer;
    while (hItem != NULL)
    {     
        szBuffer = m_EventTree.GetItemText(hItem);

        CString srchval(szItemName);
    
        // check item label first,
        // then check event text if enabled

        // Did we find it?
        if (IsSearchTextAMatch(srchval, szBuffer, type))
          return hItem;

        if (MatchEventIdInSearch(type))
        {
          DWORD id = GetEventId(hItem);
          if (id == (DWORD)atol(srchval))
            return hItem;
        }

        if (MatchEventTextInSearch(type))
        {
          // check event text 1,2,3
          // check pic name
          
          DWORD id = GetEventId(hItem);

          GameEvent *pEvent;
          if (m_SingleEvent)
            pEvent = globalData.eventData.GetEvent(id);
          else
            pEvent = levelData.eventData.GetEvent(id);

          if (pEvent != NULL)
          {
            if (IsSearchTextAMatch(srchval, pEvent->GetEventText(), type))
              return hItem;
            if (IsSearchTextAMatch(srchval, pEvent->GetEventText2(), type))
              return hItem;
            if (IsSearchTextAMatch(srchval, pEvent->GetEventText3(), type))
              return hItem;
            if (IsSearchTextAMatch(srchval, pEvent->GetEventPic().filename, type))
              return hItem;
            if (IsSearchTextAMatch(srchval, pEvent->GetEventPic2().filename, type))
              return hItem;
          }
        }

        // Check whether we have child items.
        if (m_EventTree.ItemHasChildren(hItem))
        {
            // Recursively traverse child items.
            HTREEITEM hItemFound, hItemChild;

            hItemChild = m_EventTree.GetNextItem(hItem,TVGN_CHILD);

            hItemFound = FindItemFromString(hItemChild, szItemName, type);

            // Did we find it?
            if (hItemFound != NULL)
                return hItemFound;
        }

        // Go to next sibling item.
        hItem = m_EventTree.GetNextSiblingItem(hItem);
    }

    // Not found.
    return NULL;
}

//*****************************************************************************
//    NAME: CEventViewer::OnSpelltrigger
//
// PURPOSE: 
//
//*****************************************************************************
void CEventViewer::OnSpelltrigger() 
{
  if (m_CurrEvent == NULL)
    return;

  spellBookType m_books;
  m_books.spellLimits.SetToMaxLimits();
  
  // add single spell to book
  //if (m_CurrEvent->GetEventControl().memorizedSpellDbKey.IsValidSpell())
  if (m_CurrEvent->GetEventControl().memorizedSpellID.IsValidSpell())
  {
    //m_books.KnowSpell(m_CurrEvent->GetEventControl().memorizedSpellClass,
    //                  m_CurrEvent->GetEventControl().memorizedSpellClass,                       
    //                  m_CurrEvent->GetEventControl().memorizedSpellID, 
    //                  m_CurrEvent->GetEventControl().memorizedSpellLevel,
    //                  TRUE);
    SPELL_ID spellID;
    spellID = m_CurrEvent->GetEventControl().memorizedSpellID;
    const SPELL_DATA *pSpell;
    pSpell = spellData.PeekSpell(spellID);
    //m_books.KnowSpell(baseclassData.PeekBaseclass(baseclassData.PeekBaseclass(0)->BaseclassID()),
    //                  pSpell->schoolID,
    //                  spellID, 
    //                  pSpell->Level,
    //                  TRUE);
    NotImplemented(0xc3a891, false);
  }
  
  // allow any spell class to be chosen
  CSpell dlg(m_books, 1);
  
  if (dlg.DoModal() == IDOK)
  {
    m_CurrEvent->GetEventControl().memorizedSpellClass = 0;
    m_CurrEvent->GetEventControl().memorizedSpellLevel = 0;
    //m_CurrEvent->GetEventControl().memorizedSpellDbKey.Clear();      
    m_CurrEvent->GetEventControl().memorizedSpellID.Clear();      
    
    dlg.GetSpellList(m_books);
/*    
    POSITION pos = m_books.GetHeadPosition();
    if (pos != NULL)
    {
      SPELL_DATA *data;
      if ((data = spellData.GetSpellData(m_books.PeekAtPos(pos).spell)) != NULL)
      {
        m_CurrEvent->GetEventControl().memorizedSpellClass = data->schoolMask;
        m_CurrEvent->GetEventControl().memorizedSpellLevel = data->Level;
        m_CurrEvent->GetEventControl().memorizedSpellDbKey = m_books.PeekAtPos(pos).spell;      
      }
    }
*/
    {
      int i, n;
      n = m_books.GetCount();
      for (i=0; i<n; i++)
      {        
        const SPELL_DATA *pSpellData;
        if ((pSpellData = spellData.PeekSpell(m_books.PeekCharacterSpell(i)->spellID)) != NULL)
        {
          //m_CurrEvent->GetEventControl().memorizedSpellClass = pSpellData->schoolMask;
          //m_CurrEvent->GetEventControl().memorizedSpellLevel = pSpellData->Level;
          m_CurrEvent->GetEventControl().memorizedSpellID = m_books.PeekCharacterSpell(i)->spellID;      
        }
      }
    };
    // update button states
    SetControlStates();	
  }	
}
