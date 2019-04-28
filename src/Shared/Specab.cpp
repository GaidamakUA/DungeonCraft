/******************************************************************************
* Filename: Specab.cpp
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
#include "../Shared/stdafx.h"
//#include "..\Shared\Version.h"
#include "GPDLOpCodes.h"
#include "Specab.h"
#include "GPDLcomp.h"
#include "GPDLexec.h"

#ifdef UAFEngine
#include "Char.h"
#include "../UAFWin/Combatant.h"
extern A_CStringPAIR_L globalSA_debug;
extern int globalLoggingFlags;
#endif

#include "class.h"
#include "GlobalData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

A_SPECABILITY_DEF_L specialAbilitiesData;




#ifdef UAFEDITOR
BOOL IsSyntaxAndSemanticsValid(const char *pText, const char *pFuncName, CString *pMsg, DWORD context);
#endif

GPDL specAbGPDL;
CString ActivationScriptName("Activation Script");
CString DeActivationScriptName("DeActivation Script");
extern CString emptyString;


SCRIPT_CONTEXT NULL_SCRIPT_CONTEXT;
SCRIPT_CONTEXT *pScriptContext = &NULL_SCRIPT_CONTEXT;

/*
CString SpecialAbilityContext::DeleteAbility(const CString& name)
{
  return pSpecialAbilities->DeleteAbility(name);
}

void SpecialAbilityContext::SetParam(const CString& name, const CString& value)
{
  if (pSpecialAbilities == NULL)
  {
    WriteDebugString("Attempt to insert Special Ability %s(%s)in non-existent context\n",name, value);
    return;
  };

  pSpecialAbilities->InsertAbility(name, value, emptyString, "SpecialAbilityContext");
}
*/

void SCRIPT_CONTEXT::MsgBoxError(const char *pContext)
{
  CString msg;
  msg.Format("$%sContext not defined\nType=%s\nName=%s\nSA = %s\nScript=%s",
               pContext, sourceType, sourceName, specAb.Key(), scriptName);
  ::MsgBoxError(msg, "Error", 2);
};

void SCRIPT_CONTEXT::SetParam(const CString& value)
{
  if (context == NULL) return;
  context->InsertAbility(specAb.Key(), value, emptyString, emptyString);
  specAb.Value(value);
}

void SCRIPT_CONTEXT::SetItemContext(const ITEM_DATA *pItem)
{
  pItemContext = pItem;
  if (pItem != NULL)
  {
    //SetSpellContext(pItem->m_gsID);
    SetSpellContext(pItem->SpellID());
  };
}

void SCRIPT_CONTEXT::SetMonsterTypeContext(const MONSTER_DATA *pMonster)
{
  pMonstertypeContext = pMonster;
}

//void SCRIPT_CONTEXT::SetItemContext(GLOBAL_ITEM_ID giID)
void SCRIPT_CONTEXT::SetItemContext(const ITEM_ID& itemID)
{
  //if (giID.IsValidItem())
  if (itemID.IsValidItem())
  {
    //SetItemContext(itemData.GetItemData(giID));
    SetItemContext(itemData.GetItem(itemID));
  };
  itemKey = -1;
}

void SCRIPT_CONTEXT::SetRaceContext(const RACE_DATA *pRace)
{
  pRaceContext = pRace;
}

void SCRIPT_CONTEXT::SetClassContext(const CLASS_DATA *pClass)
{
  pClassContext = pClass;
}

void SCRIPT_CONTEXT::SetClassContext(const CLASS_ID classID)
{
  pClassContext = classData.PeekClass(classID);
}

void SCRIPT_CONTEXT::SetBaseClassContext(const BASE_CLASS_DATA *pBaseclass)
{
  pBaseclassContext = pBaseclass;
}

void SCRIPT_CONTEXT::SetSpellContext(const SPELL_DATA *pSpell)
{
  pSpellContext = pSpell;
}

void SCRIPT_CONTEXT::SetToHitComputationContext(const ToHitComputation *pToHitCC)
{
  pToHitComputationContext = pToHitCC;
}


//void SCRIPT_CONTEXT::SetSpellContext(GLOBAL_SPELL_ID gsID)
void SCRIPT_CONTEXT::SetSpellContext(const SPELL_ID& spellID)
{
  //if (gsID.IsValidSpell())
  if (spellID.IsValidSpell())
  {
    //SetSpellContext(spellData.GetSpellData(gsID));
    SetSpellContext(spellData.GetSpell(spellID));
  };
}

void SCRIPT_CONTEXT::SetCharacterContext(const CHARACTER *pChar)
{
  ActorType actor;
  pChar->GetContext(&actor);
  characterContext = actor.ToString();
}

void SCRIPT_CONTEXT::SetCreatedCharacterContext(CHARACTER *pChar)
{
  SetCharacterContext(pChar);
  pCreatedCharacterContext = pChar;
}

void SCRIPT_CONTEXT::SetTargetContext(const CHARACTER *pChar)
{
  ActorType actor;
  pChar->GetContext(&actor);
  targetContext = actor.ToString();
}

void SCRIPT_CONTEXT::SetAttackerContext(const CHARACTER *pChar)
{
  ActorType actor;
  pChar->GetContext(&actor);
  attackerContext = actor.ToString();
}




//COMBATANT *SCRIPT_CONTEXT::GetAttackerCombatant(void)
//{
//  ActorType actor;
//  CHARACTER *pCharacter;
//  actor.FromString(attackerContext);
//  pCharacter = GetCurrentlyActiveContext(&actor);
//  return pCharacter->GetCombatant();
//}

//COMBATANT *SCRIPT_CONTEXT::GetTargetCombatant(void)
//{
//  ActorType actor;
//  CHARACTER *pCharacter;
//  actor.FromString(targetContext);
//  pCharacter = GetCurrentlyActiveContext(&actor);
//  return pCharacter->GetCombatant();
//}

COMBATANT *SCRIPT_CONTEXT::GetCombatant(void)
{
  ActorType actor;
  CHARACTER *pCharacter;
  if (combatantContext.IsEmpty())
  {
    WriteDebugString("Attempt to get non-existent Combatant Context\n");
    return NULL;
  };
  actor.FromString(combatantContext);
  pCharacter = GetCurrentlyActiveContext(&actor);
  return pCharacter->m_pCombatant;
}

CHARACTER *SCRIPT_CONTEXT::GetCharacter(void)
{
  ActorType actor;
  CHARACTER *pCharacter;
  actor.FromString(characterContext);
  pCharacter = GetCurrentlyActiveContext(&actor);
  return pCharacter;
}


#ifdef UAFEngine
void SCRIPT_CONTEXT::SetAttackerContext(const COMBATANT *pCombatant)
{
  ActorType actor;
  pCombatant->GetContext(&actor);
  attackerContext = actor.ToString();
}

void SCRIPT_CONTEXT::SetTargetContext(const COMBATANT *pCombatant)
{
  ActorType actor;
  pCombatant->GetContext(&actor);
  targetContext = actor.ToString();
}

void SCRIPT_CONTEXT::SetTargetContext(const ActorType *pActor)
{
  targetContext = pActor->ToString();
}

void SCRIPT_CONTEXT::SetCombatantContext(const COMBATANT *pCombatant)
{
  ActorType actor;
  pCombatant->GetContext(&actor);
  combatantContext = actor.ToString();
}


#endif

ITEM_DATA bogusItem;
const ITEM_DATA  *SCRIPT_CONTEXT::GetItemContext(void) const
{
  if (pItemContext == NULL) return &bogusItem;
  return pItemContext;
};
RACE_DATA bogusRace;
const RACE_DATA  *SCRIPT_CONTEXT::GetRaceContext(void)     
{
  if (pRaceContext == NULL) return &bogusRace;
  return pRaceContext;
};
CString SCRIPT_CONTEXT::GetAttackerContext(void) 
{
  return attackerContext;
};
CString  SCRIPT_CONTEXT::GetTargetContext(void)   
{
  return targetContext;
};
CHARACTER  *SCRIPT_CONTEXT::GetCreatedCharacterContext(void)   
{
  return pCreatedCharacterContext;
};
CString SCRIPT_CONTEXT::GetCharacterContext(void)
{
  return characterContext;
};
CString SCRIPT_CONTEXT::GetCombatantContext(void)
{
  return combatantContext;
};

CLASS_DATA bogusClass;
const CLASS_DATA *SCRIPT_CONTEXT::GetClassContext(void)    
{
  if (pClassContext == NULL) return &bogusClass;
  return pClassContext;
}
BASE_CLASS_DATA bogusBaseclass;
const BASE_CLASS_DATA *SCRIPT_CONTEXT::GetBaseclassContext(void)    
{
  if (pBaseclassContext == NULL) return &bogusBaseclass;
  return pBaseclassContext;
}
const MONSTER_DATA *SCRIPT_CONTEXT::GetMonstertypeContext(void) const   
{
  return pMonstertypeContext;
}
ABILITY_DATA *SCRIPT_CONTEXT::GetAbilityContext(void)    
{
  return pAbilityContext;
}


#ifdef USE_TRAITS
TRAIT_DATA *SCRIPT_CONTEXT::GetTraitContext(void)    
{
  return pTraitContext;
}
#endif


SPELL_DATA bogusSpell;
const SPELL_DATA *SCRIPT_CONTEXT::GetSpellContext(void) const   
{
  if (pSpellContext == NULL)
  {
    return &bogusSpell;
  };
  return pSpellContext;
}

SPELLGROUP_DATA *SCRIPT_CONTEXT::GetSpellgroupContext(void)    
{
  return pSpellgroupContext;
}

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetItemSAContext(void) const
{
  const ITEM_DATA *pItem;
  pItem = GetItemContext();
  if (pItem == NULL) return NULL;
  return &pItem->specAbs;
};

//SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetAttackerSAContext(void)
//{
//#ifdef UAFEngine
//  COMBATANT *pAttacker;
//  pAttacker = GetAttackerCombatant();
//  if (pAttacker == NULL) return NULL;
//  return &pAttacker->combatantSA;
//#else
//  return NULL;
//#endif
//};
//SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetTargetSAContext(void)
//{
//#ifdef UAFEngine
//  COMBATANT *pTarget;
//  pTarget = GetTargetCombatant();
//  if (pTarget == NULL) return NULL;
//  return &pTarget->combatantSA;
//#else
//  return NULL;
//#endif
//};
SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetCharacterSAContext(void)
{
  CHARACTER *pCharacter = NULL;
  if (!characterContext.IsEmpty())
  {
    pCharacter = GetCharacter();
    if (pCharacter == NULL) return NULL;
    return &pCharacter->specAbs;
  };
  if (combatantContext.IsEmpty())
  {
    pCharacter = GetCharacter(); // To provide error message and such.
    if (pCharacter == NULL) return NULL;
    return &pCharacter->specAbs;
  };
#ifdef UAFEngine
  COMBATANT *pCombatant;
  pCombatant = GetCombatant();
  if (pCombatant == NULL)
#endif
  {
    pCharacter = GetCharacter();
    if (pCharacter == NULL) return NULL;
    return &pCharacter->specAbs;
  };
#ifdef UAFEngine
  pCharacter = pCombatant->m_pCharacter;
  return &pCharacter->specAbs;
#endif
};

#ifdef UAFEngine
SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetCombatantSAContext(void)
{
  COMBATANT *pCombatant;
  pCombatant = GetCombatant();
  if (pCombatant == NULL) return NULL;
  return &pCombatant->combatantSA;
};
#endif

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetClassSAContext(void)
{
  const CLASS_DATA *pClass;
  pClass = GetClassContext();
  if (pClass == NULL) return NULL;
  return &pClass->m_specialAbilities;
};

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetBaseclassSAContext(void)
{
  const BASE_CLASS_DATA *pBaseclass;
  pBaseclass = GetBaseclassContext();
  if (pBaseclass == NULL) return NULL;
  return &pBaseclass->m_specAbs;
};

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetSpellSAContext(void) const
{
  const SPELL_DATA *pSpell;
  pSpell = GetSpellContext();
  if (pSpell == NULL) return NULL;
  return &pSpell->specAbs;
};

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetSpellgroupSAContext(void)
{
  SPELLGROUP_DATA *pSpellgroup;
  pSpellgroup = GetSpellgroupContext();
  if (pSpellgroup == NULL) return NULL;
  return &pSpellgroup->m_specAbs;
};

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetMonstertypeSAContext(void) const
{
#ifdef UAFEngine
  //MONSTER_DATA *pMonster;
  const MONSTER_DATA *pMonster;
  pMonster = GetMonstertypeContext();
  if (pMonster == NULL) return NULL;
  return &pMonster->specAbs;
#else
  return NULL;
#endif
};

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetRaceSAContext(void)
{
  const RACE_DATA *pRace;
  pRace = GetRaceContext();
  if (pRace == NULL) return NULL;
  return &pRace->m_specAbs;
};

const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetAbilitySAContext(void)
{
  const ABILITY_DATA *pAbility;
  pAbility = GetAbilityContext();
  if (pAbility == NULL) return NULL;
  return &pAbility->m_specAbs;
};


#ifdef USE_TRAITS
const SPECIAL_ABILITIES *SCRIPT_CONTEXT::GetTraitSAContext(void)
{
  const TRAIT_DATA *pTrait;
  pTrait = GetTraitContext();
  if (pTrait == NULL) return NULL;
  return &pTrait->m_specAbs;
};
#endif


void SCRIPT_CONTEXT::Clear(void)
{
  pItemContext        = NULL;
  attackerContext    = "";
  characterContext   = "";
  targetContext      = "";
  combatantContext   = "";
  pClassContext       = NULL;
  pSpellContext       = NULL;
  pSpellgroupContext  = NULL;
  pMonstertypeContext = NULL;
  pRaceContext        = NULL;
  pAbilityContext     = NULL;
#ifdef USE_TRAITS
  pTraitContext       = NULL;
#endif
  context             = NULL;

  specAb.Clear();
}


void SCRIPT_CONTEXT::ClearAbility(void)
{
  specAb.Clear();
}

void SCRIPT_CONTEXT::SetAbility(const SPECIAL_ABILITIES *pSpecAb, const CStringPAIR *pAb)
{
  context = const_cast<SPECIAL_ABILITIES*>(pSpecAb);
  if (pAb != NULL)  specAb = *pAb;
  else
  {
    specAb.Clear();
  };
}


void SCRIPT_CONTEXT::SetSA_Source_Type(LPCSTR source_Type)
{
  sourceType = source_Type;
}

void SCRIPT_CONTEXT::SetSA_Source_Name(LPCSTR source_Name)
{
  sourceName = source_Name;
}

void SCRIPT_CONTEXT::SetSA_ScriptName(LPCSTR name)
{
  scriptName = name;
}



const CStringPAIR *SCRIPT_CONTEXT::GetAbility(void) const
{
  return &specAb;
}

CString SCRIPT_CONTEXT::RemoveAbility(void)
{
  CString result;
  result = NO_SUCH_SA;
  if (context != NULL)
  {
    if (specAb.Key() != emptyString)
    {
      result = context->DeleteAbility(specAb.Key());
    };
  };
  return result;
}



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#ifdef UAFEngine
void SPECIAL_ABILITIES::EnableAllFor(CHARACTER *pChar)
{
  ActorType actor;
  pChar->GetContext(&actor);
  SetCharContext(&actor);
  SetTargetContext(&actor);


  if (GetCount() != 0)
  {
    NotImplemented(0xc892d, FALSE);
  //for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
  //  specAbData[i].ExecuteScript();
  };
  
  ClearCharContext();
  ClearTargetContext();
}


void SPECIAL_ABILITIES::DisableAllFor(CHARACTER *pChar)
{
  ActorType actor;
  pChar->GetContext(&actor);
  SetCharContext(&actor);
  SetTargetContext(&actor);

  if (GetCount() != 0)
  {
    NotImplemented(0xc891a, FALSE);
     //for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
       //specAbData[i].ExecuteDAScript();
  };
  
  ClearCharContext();
  ClearTargetContext();
}



#endif



const char *ConvertRuntimeIfTextToSpecAb(const CString& str)
{
  NotImplemented(0xa2cf8, FALSE);
  return "abc";
}



CString ConvertSpecAbToRuntimeIfText(const char *txt)
{
  NotImplemented(0xf4a9bc, FALSE);
  return "abc";
}

int ConvertSpecAbFlagToIndex(DWORD flag)
{
  switch (flag)
  {
  case SPECAB_MSG_NONE: return 0;
  case SPECAB_MSG_CASTING: return 1;
  case SPECAB_MSG_ACTIVATESPELL: return 2;
  case SPECAB_MSG_FLEE: return 3;
  case SPECAB_MSG_TURNUNDEAD: return 4;
  case SPECAB_MSG_BANDAGE: return 5;
  case SPECAB_MSG_GUARD: return 6;
  case SPECAB_MSG_ATTACK: return 7;
  case SPECAB_MSG_MOVE: return 8;
  case SPECAB_MSG_ENDTURN: return 9;
  case SPECAB_MSG_DELAY: return 10;
  case SPECAB_MSG_READY: return 11;
  }
  return 0;
}



//***********************************************************************  SPECABILITY_DEF

int SPECABILITY_DEF::Compare(const SPECABILITY_DEF& specAbDef) const
{
  return m_specAbName.Compare(specAbDef.m_specAbName);
}

SPECABILITY_DEF::SPECABILITY_DEF(void)
{
}

void SPECABILITY_DEF::Swap(SPECABILITY_DEF& specAbDef)
{
  m_abilityStrings.Swap(specAbDef.m_abilityStrings);
}

SPECABILITY_DEF::~SPECABILITY_DEF(void)
{
}

const CString& SPECABILITY_DEF::Lookup(LPCSTR name) const 
{
  return m_abilityStrings.Lookup(name);
}

ASLENTRY *SPECABILITY_DEF::Find(LPCSTR name) const 
{
  return m_abilityStrings.Find(name);
}

void SPECABILITY_DEF::Clear(void)
{
  m_specAbName = emptyString;
  m_abilityStrings.Clear();
}

CString SPECABILITY_DEF::Delete(const CString& name)
{
  return m_abilityStrings.Delete(name);
}

void SPECABILITY_DEF::Copy(const SPECABILITY_DEF& specAbDef)
{
  m_specAbName = specAbDef.m_specAbName;
  m_abilityStrings.Copy(specAbDef.m_abilityStrings);
}

void SPECABILITY_DEF::Copy(const SPECABILITY_DEF& specAbDef, const CString& newName)
{
  m_specAbName = newName;
  m_abilityStrings.Copy(specAbDef.m_abilityStrings);
}


bool SPECABILITY_DEF::operator ==(const SPECABILITY_DEF& specAb) const
{
  if (m_specAbName != specAb.m_specAbName) return false;
  if (m_abilityStrings == specAb.m_abilityStrings) return true;
  return false;
}

int SPECABILITY_DEF::Compare(LPCSTR name) const
{
  return m_specAbName.Compare(name);
}

POSITION SPECABILITY_DEF::GetStartPosition(void) const
{
  return m_abilityStrings.GetStartPosition();
}

void SPECABILITY_DEF::GetNextEntry(POSITION& pos, const ASLENTRY **ppEntry) const
{
  m_abilityStrings.GetNextAssoc(pos, ppEntry);
}

void SPECABILITY_DEF::Insert(const CString& key, const CString& value, unsigned char flags)
{
  m_abilityStrings.Insert(key, value, flags);
}

void SPECABILITY_DEF::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    car << m_specAbName;
    m_abilityStrings.Serialize(car,"SPECIAL_ABILITIES_DB");
  }
  else
  {
    NotImplemented(0xfda2324, FALSE);
  };
}

//*****************************************************************************
// NAME: loadData
//
// PURPOSE:  
//*****************************************************************************
int loadData(A_SPECABILITY_DEF_L& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success=TRUE;

  if (globalData.version < _SPECIAL_ABILITIES_VERSION_) return success;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open special abilities db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  data.Clear();


  {
    WriteDebugString("Loading special abilities DB\n");
    CAR ar(&myFile, CArchive::load);
    try
    {
       data.Serialize(ar);
    }
    catch (...)
    {
       WriteDebugString("Unable to load special abilities data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }  
  
  if (data.GetCount() == 0)
     WriteDebugString("No special abilities in special abilities db file\n");

  return success; //data.GetCount();
}



/*
//******************************************************************  SPECABILITY

const CString& SPECABILITY::Name(void) const
{
  return m_specAbName;
}

bool SPECABILITY::operator ==(const SPECABILITY& specAb) const
{
  if (m_specAbName != specAb.m_specAbName) return false;
  if (m_parameters == specAb.m_parameters) return true;
  return false;
}

//const CString& SPECABILITY::Binary(void) const
//{
//  NotImplemented(0x4f4d4a, FALSE);
//  return emptyString;
//}

//const CString& SPECABILITY::Script(void) const
//{
//  NotImplemented(0x6a3cb7, FALSE);
//  return emptyString;
//}


//void ABILITY_DELETE(int a)
//{
//  NotImplemented(0x43ea12,FALSE);
//}
*/

SPECIAL_ABILITIES::SPECIAL_ABILITIES(bool readOnly)
{
  m_readOnly = readOnly;
}



SPECIAL_ABILITIES::SPECIAL_ABILITIES(const SPECIAL_ABILITIES& spab)
{
  NotImplemented(0x5ffdab, FALSE);
}


void SPECIAL_ABILITIES::InsertAbility_RO(const CString& name, const CString& param, LPCSTR d1, LPCSTR d2)
{
  m_specialAbilities.Insert(name, param);
#ifdef UAFEngine
  if (globalSA_debug.Find(name) != NULL)
  {
    WriteDebugString("@@SA InsertAbility \"%s\" %s%s\n", name, d1, d2);
  };
#endif
}


void SPECIAL_ABILITIES::InsertAbility(const CString& name, const CString& param, LPCSTR d1, LPCSTR d2)
{
#ifdef UAFEngine
  if (m_readOnly)
  {
    if (!debugStrings.AlreadyNoted(CString("ATISAIROS")))
          writeDebugDialog = WRITE_DEBUG_DIALOG_WARNING;
          WriteDebugString("Attempt to Insert SA in read-only structure\n");
    return;
  };
#endif
  InsertAbility_RO(name, param, d1, d2);
}


CString SPECIAL_ABILITIES::DeleteAbility_RO(const CString& name)
{
  return m_specialAbilities.Delete(name);
}


CString SPECIAL_ABILITIES::DeleteAbility(const CString& name)
{
#ifdef UAFEngine
  if (m_readOnly)
  {
    if (!debugStrings.AlreadyNoted(CString("ATISAIROS")))
          writeDebugDialog = WRITE_DEBUG_DIALOG_WARNING;
          WriteDebugString("Attempt to Insert SA in read-only structure\n");
    return NO_SUCH_SA;
  };
#endif
  return DeleteAbility_RO(name);
}

CStringPAIR *SPECIAL_ABILITIES::FindAbility(specialAbilitiesType sa) const
{
  return m_specialAbilities.Find(sa);
}


void SPECIAL_ABILITIES::EnableSpecAb(specialAbilitiesType sa, const CString& ascript, const CString& dscript)
{
  /*
  CStringPAIR *pData;
  pData = FindAbility(sa);
  if (pData==NULL) return;
  //if (ascript.IsEmpty()) ascript = GenerateDefaultSpecAbScript(sa);
  //if (dscript.IsEmpty()) dscript = GenerateDefaultSpecAbDAScript(sa);  
  if (!ascript.IsEmpty()) pData->Insert(ActivationScriptName, ascript, SPECAB_SCRIPT);
  if (!dscript.IsEmpty()) pData->Insert(ActivationScriptName, dscript, SPECAB_SCRIPT);
  //CompileSpecAbScript(sa);  
  //CompileSpecAbDAScript(sa);  
  */
}

CString SPECIAL_ABILITIES::GenerateSpecAbFuncName(specialAbilitiesType sa) const
{
  CString name;
  name = "SA_";
  name = name + sa;
  name.Replace(' ', '_');
  name.Replace('(', '_');
  name.Replace(')', '_');
  return name;
}

/*
CString SPECIAL_ABILITIES::GenerateSpecAbFuncStart(specialAbilitiesType sa) const
{
  CString name;
  CString script;
  name = GenerateSpecAbFuncName(sa);
  if (name.IsEmpty()) return "";
  script.Format("$PUBLIC $FUNC %s() \n{ ", name);
  return script;
}

CString SPECIAL_ABILITIES::GenerateSpecAbFuncEnd(specialAbilitiesType sa) const
{
  CString name;
  CString script;

  name = GenerateSpecAbFuncName(sa);
  if (name.IsEmpty()) return "";
  script.Format(" } %s;", name);
  return script;
}
*/
bool  SPECIAL_ABILITIES::HaveSpecAb(specialAbilitiesType sa) const
{
  NotImplemented(0xde1cc, FALSE);
  return FALSE;
}

bool SPECIAL_ABILITIES::HaveAtLeastOneSpecAb() const
{
  NotImplemented(0xde1cd, FALSE);
  return FALSE;
}

void SPECIAL_ABILITIES::CrossReference(CR_LIST *pCRList) const
{
  NotImplemented(0xde1ce, FALSE);
}

void SPECIAL_ABILITIES::SetMsgTypes(specialAbilitiesType sa, DWORD types)
{
  NotImplemented(0xf63ae, FALSE);
}


const CStringPAIR *SPECIAL_ABILITIES::GetNextData(POSITION& pos) const
{
  const CStringPAIR *pEntry;
  m_specialAbilities.GetNextAssoc(pos, &pEntry);
  return pEntry;
}



void SPECIAL_ABILITIES::DisableSpecAb(specialAbilitiesType sa)
{
  NotImplemented(0xa4216b, FALSE);
}


const CString& SPECIAL_ABILITIES::GetSpecAbUserDAScript(specialAbilitiesType sa) const
{
  return GetString(sa);
}

const CString& SPECIAL_ABILITIES::GetString(specialAbilitiesType sa) const
{
  static CString noSuch = NO_SUCH_SA;
  const CStringPAIR *pEntry;
  pEntry = m_specialAbilities.Find(sa);
  if (pEntry == NULL) return noSuch;
  return pEntry->Value();
}

const CString& SPECIAL_ABILITIES::GetSpecAbUserScript(specialAbilitiesType sa) const
{
  return GetString(sa);
}


bool SPECIAL_ABILITIES::operator ==(const SPECIAL_ABILITIES& pSA) const
{
  return (m_specialAbilities == pSA.m_specialAbilities)?true:false;
}

//SPECIAL_ABILITIES& SPECIAL_ABILITIES::operator =(const SPECIAL_ABILITIES& pSA)
//{
//  m_specialAbilities = pSA.m_specialAbilities;
//  return *this;
//}

void SPECIAL_ABILITIES::Copy(const SPECIAL_ABILITIES& pSA)
{
  m_specialAbilities.Copy(pSA.m_specialAbilities);
}

void SPECIAL_ABILITIES::Copy(const A_ASLENTRY_L& pasl)
{
  m_specialAbilities.Copy(pasl);
}


void SPECIAL_ABILITIES::Clear(void)
{
  m_specialAbilities.Clear();
}


SPECIAL_ABILITIES::~SPECIAL_ABILITIES(void)
{
  Clear();
}

void SPECIAL_ABILITIES::Serialize(CArchive& ar, double version, LPCSTR objName, LPCSTR objType)
{
  if ((version <= 0.920) && !ar.IsStoring())
  {
#ifdef UAFEDITOR
    {
      int count;
      if (version < _VERSION_0850_)
      {
        // load the old way, just an array of integers
        WORD temp; 
        int i;
        CArray<WORD, WORD> specAbs;
  
        ar >> count;
        if (count < 0) count = 0;
        //if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;
  
        specAbs.SetSize(count);
        for (i=0; i<count; i++)
        {
          ar >> temp;
          specAbs[i] = temp;
        }

        for (i=0; i<specAbs.GetSize(); i++)
        {
          if (specAbs[i] > 0)
            EnableSpecAb((specialAbilitiesType)specAbs[i], emptyString, emptyString); 
        }
      }
      else
      {
        ar >> count;
        // if either of these two are invalid, the loading
        // has gone wrong somewhere, or, possibly loading a
        // file that was saved as a newer version
        if (count < 0) count = 0;
        //if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;
        for (int i=0; i<count; i++)
        {
          // Convert old special ability to new
          {
            CString script, binary;
            CString da_script, da_binary;
            BOOL DisplayOnce;
            DWORD MsgTypes;
            CString Msgs[NUM_SPECAB_MSGS];
            int empty = 0;
            //*******************************
            if (version == _VERSION_0850_)
            {
              int unused;
              ar >> unused;
            }
            //*******************************
            DAS(ar, script);
            empty += script.GetLength();
            //*****************************
            if (version >= _VERSION_0851_)
             DAS(ar, binary);
            empty += binary.GetLength();
            //*******************************
            if (version >= _VERSION_0852_)
            {
              DAS(ar, da_script);
              empty += da_script.GetLength();
              DAS(ar, da_binary);
              empty += da_binary.GetLength();
            }
            //*******************************
            if (version >= _VERSION_0870_)
            {
              int cnt;
              CString msg;
              ar >> MsgTypes;
              if (MsgTypes != 0) empty++;
              ar >> DisplayOnce;
              ar >> cnt;      
              if (cnt > NUM_SPECAB_MSGS)
              {
                WriteDebugString("Invalid SpecAbMsg count %i in Serialize\n", cnt);
                die(0xab537);
              }
              //*******************************
              for (int idx=0;idx<cnt;idx++)
              {
                DAS(ar, msg);
                empty += msg.GetLength();
                //totalSpecAbMsg++;
                if (idx < NUM_SPECAB_MSGS)
                  Msgs[idx] = msg;          
              }
            };
            if (empty != 0)
            {
              CString abname;
              int j;
              abname = CString(objType) + '_' + objName + '_' + spellAbilitiesText[i];
              m_specialAbilities.Insert(abname, emptyString);
              // Now we need to create the special ability definition.
              specialAbilitiesData.InsertAbility(abname);
              // And finally add the definition strings
              if (script.GetLength() != 0)
              {
                //entry.Key("ActivationScript");
                //entry.Value(script);
                //entry.Flags(SPECAB_SCRIPT);
                specialAbilitiesData.InsertString(abname, ActivationScriptName,script, SPECAB_SCRIPT);
              };
              if (da_script.GetLength() != 0)
              {
                specialAbilitiesData.InsertString(abname, DeActivationScriptName, da_script, SPECAB_SCRIPT);
              };
              for (j=0; j<NUM_SPECAB_MSGS; j++)
              {
                if (Msgs[j].GetLength() != 0)
                {
                  CString stringName;
                  stringName.Format("%s Msg", SpecAbMsgText[j]);
                  specialAbilitiesData.InsertString(abname, stringName, Msgs[j], SPECAB_CONSTANT);
                };
              };
            };
          };
        };
      };
    };
#else
    {
      int count;
      if (version < _VERSION_0850_)
      {
        // load the old way, just an array of integers
        WORD temp; 
        int i;
        CArray<WORD, WORD> specAbs;
  
        ar >> count;
        if (count < 0) count = 0;
        //if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;
  
        specAbs.SetSize(count);
        for (i=0; i<count; i++)
        {
          ar >> temp;
          specAbs[i] = temp;
        }

        for (i=0; i<specAbs.GetSize(); i++)
        {
          if (specAbs[i] > 0)
            EnableSpecAb((specialAbilitiesType)specAbs[i], emptyString, emptyString); 
        }
      }
      else
      {
        ar >> count;
        // if either of these two are invalid, the loading
        // has gone wrong somewhere, or, possibly loading a
        // file that was saved as a newer version
        if (count < 0) count = 0;
        //if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;
        for (int i=0; i<count; i++)
        {
          // Convert old special ability to new
          {
            CString script, binary;
            CString da_script, da_binary;
            BOOL DisplayOnce;
            DWORD MsgTypes;
            CString Msgs[NUM_SPECAB_MSGS];
            int empty = 0;
            //*******************************
            if (version == _VERSION_0850_)
            {
              int unused;
              ar >> unused;
            }
            //*******************************
            DAS(ar, script);
            empty += script.GetLength();
            //*****************************
            if (version >= _VERSION_0851_)
             DAS(ar, binary);
            empty += binary.GetLength();
            //*******************************
            if (version >= _VERSION_0852_)
            {
              DAS(ar, da_script);
              empty += da_script.GetLength();
              DAS(ar, da_binary);
              empty += da_binary.GetLength();
            }
            //*******************************
            if (version >= _VERSION_0870_)
            {
              int msgCnt;
              CString msg;
              ar >> MsgTypes;
              if (MsgTypes != 0) empty++;
              ar >> DisplayOnce;
              ar >> msgCnt;
              if (msgCnt > NUM_SPECAB_MSGS)
              {
                WriteDebugString("Invalid SpecAbMsg count %i in Serialize\n", msgCnt);
                die(0xab538);
              }
              //*******************************
              {
                for (int j = 0; j < msgCnt; j++)
                {
                  DAS(ar, msg);
                  empty += msg.GetLength();
                  //totalSpecAbMsg++;
                  if (j < NUM_SPECAB_MSGS)
                    Msgs[j] = msg;
                };
              };
            };
            if (empty != 0)
            {
              NotImplemented(0x42ad78,FALSE);
              /*
              CString abname;
              int j;
              abname = CString(objType) + '_' + objName + '_' + spellAbilitiesText[i];
              m_specialAbilities.Insert(abname, emptyString);
              // Now we need to create the special ability definition.
              specialAbilitiesData.InsertAbility(abname);
              // And finally add the definition strings
              if (script.GetLength() != 0)
              {
                //entry.Key("ActivationScript");
                //entry.Value(script);
                //entry.Flags(SPECAB_SCRIPT);
                specialAbilitiesData.InsertString(abname, ActivationScriptName,script, SPECAB_SCRIPT);
              };
              if (da_script.GetLength() != 0)
              {
                specialAbilitiesData.InsertString(abname, DeActivationScriptName, da_script, SPECAB_SCRIPT);
              };
              for (j=0; j<NUM_SPECAB_MSGS; j++)
              {
                if (Msgs[j].GetLength() != 0)
                {
                  CString stringName;
                  stringName.Format("%s Msg", SpecAbMsgText[j]);
                  specialAbilitiesData.InsertString(abname, stringName, Msgs[j], SPECAB_CONSTANT);
                };
              }; */
            };
          };
        };
      };
    };
#endif
  }
  else
  {
    m_specialAbilities.Serialize(ar);
  };
}

void SPECIAL_ABILITIES::Serialize(CAR& ar, double version, LPCSTR objName, LPCSTR objType)
{
  if ((version <= 0.920) && !ar.IsStoring())
  {
#ifdef UAFEDITOR
    {
      int count;
      if (version < _VERSION_0850_)
      {
        // load the old way, just an array of integers
        WORD temp; 
        int i;
        CArray<WORD, WORD> specAbs;
  
        ar >> count;
        if (count < 0) count = 0;
        //if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;
  
        specAbs.SetSize(count);
        for (i=0; i<count; i++)
        {
          ar >> temp;
          specAbs[i] = temp;
        }

        for (i=0; i<specAbs.GetSize(); i++)
        {
          if (specAbs[i] > 0)
            EnableSpecAb((specialAbilitiesType)specAbs[i], emptyString, emptyString); 
        }
      }
      else
      {
        ar >> count;
        // if either of these two are invalid, the loading
        // has gone wrong somewhere, or, possibly loading a
        // file that was saved as a newer version
        if (count < 0) count = 0;
        //if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;
        for (int i=0; i<count; i++)
        {
          // Convert old special ability to new
          {
            CString script, binary;
            CString da_script, da_binary;
            BOOL DisplayOnce;
            DWORD MsgTypes;
            CString Msgs[NUM_SPECAB_MSGS];
            int empty = 0;
            //*******************************
            if (version == _VERSION_0850_)
            {
              int unused;
              ar >> unused;
            }
            //*******************************
            DAS(ar, script);
            empty += script.GetLength();
            //*****************************
            if (version >= _VERSION_0851_)
             DAS(ar, binary);
            empty += binary.GetLength();
            //*******************************
            if (version >= _VERSION_0852_)
            {
              DAS(ar, da_script);
              empty += da_script.GetLength();
              DAS(ar, da_binary);
              empty += da_binary.GetLength();
            }
            //*******************************
            if (version >= _VERSION_0870_)
            {
              int cnt;
              CString msg;
              ar >> MsgTypes;
              if (MsgTypes != 0) empty++;
              ar >> DisplayOnce;
              ar >> cnt;      
              if (cnt > NUM_SPECAB_MSGS)
              {
                WriteDebugString("Invalid SpecAbMsg count %i in Serialize\n", cnt);
                die(0xab539);
              }
              //*******************************
              for (int idx=0;idx<cnt;idx++)
              {
                DAS(ar, msg);
                empty += msg.GetLength();
                //totalSpecAbMsg++;
                if (idx < NUM_SPECAB_MSGS)
                  Msgs[idx] = msg;          
              }
            };
            if ((globalData.version < _SPECIAL_ABILITIES_VERSION_) && (empty != 0))
            {
              CString abname;
              int j;
              abname = CString(objType) + '_' + objName + '_' + spellAbilitiesText[i];
              m_specialAbilities.Insert(abname, emptyString);
              // Now we need to create the special ability definition.
              specialAbilitiesData.InsertAbility(abname);
              // And finally add the definition strings
              if (script.GetLength() != 0)
              {
                //entry.Key("ActivationScript");
                //entry.Value(script);
                //entry.Flags(SPECAB_SCRIPT);
                specialAbilitiesData.InsertString(abname, ActivationScriptName,script, SPECAB_SCRIPT);
              };
              if (da_script.GetLength() != 0)
              {
                specialAbilitiesData.InsertString(abname, DeActivationScriptName, da_script, SPECAB_SCRIPT);
              };
              for (j=0; j<NUM_SPECAB_MSGS; j++)
              {
                if (Msgs[j].GetLength() != 0)
                {
                  CString stringName;
                  stringName.Format("%s Msg", SpecAbMsgText[j]);
                  specialAbilitiesData.InsertString(abname, stringName, Msgs[j], SPECAB_CONSTANT);
                };
              };
            };
          };
        };
      };
    };
#else
    if (ar.IsStoring())
    {
      NotImplemented(0x8da3f, FALSE);
    }
    else
    {
      NotImplemented(0x54fa23,FALSE);
    };
#endif
  }
  else
  {
    m_specialAbilities.Serialize(ar);
  };
}


#ifdef UAFEDITOR
void SPECIAL_ABILITIES::Export(JWriter& jw)
{
  m_specialAbilities.Export(jw);
};
void SPECIAL_ABILITIES::Import(JReader& jr)
{
  m_specialAbilities.Import(jr);
};
#endif


const CString& SPECIAL_ABILITIES::GetNext(POSITION &pos) const
{
  const CStringPAIR *pEntry;
  pEntry = GetNextData(pos);
  return pEntry->Key();
}

void SPECIAL_ABILITIES::SetMsgFormat(specialAbilitiesType sa, DWORD type, CString &format)
{
  NotImplemented(0x35a901, FALSE);
}

CString  SPECIAL_ABILITIES::GetMsgFormat(specialAbilitiesType sa, DWORD type) const
{
  return CString("NotImplemented 4kd4x6");
}


DWORD SPECIAL_ABILITIES::GetMsgTypes(specialAbilitiesType sa) const
{
  CString sTypes;
  int iTypes = 0;
  sTypes = GetString(sa);
  sscanf(sTypes, "%d", &iTypes);
  return iTypes;
}


bool IsDigit(char c)
{
  return (c >= '0') && (c <= '9');
}

int ScriptAtoI(const char **p, int min, int max)
{
  int n = 0;
  int sign = 1;
  if (**p == '-')
  {
    sign = -1;
    (*p)++;
  }
  else if (**p == '+')
  {
    (*p)++;
  };
  while (IsDigit(**p))
  {
    n = 10* n + **p - '0';
    (*p)++;
  };
  n *= sign;
  if (n < min) n = min;
  if (n > max) n = max;
  return n;
}

CBRESULT ScriptCallback_MinMax(CBFUNC func, CString *scriptResult, void *pkt)
{
  int *minmax = (int *)pkt;
  int n;
  LPCTSTR pStr;
  if (func != CBF_EXAMINESCRIPT) return CBR_CONTINUE;
  pStr = (LPCTSTR)*scriptResult;
  // If it starts with a number we simply move the
  // max down to that number and the min up to that number.
  if (IsDigit(pStr[0]))
  {
    n = ScriptAtoI(&pStr, minmax[0], minmax[1]);
    if (n > minmax[0]) minmax[0] = n;
    if (n < minmax[1]) minmax[1] = n;
    return CBR_CONTINUE;
  };
  for (;;)
  {
    if (pStr[0] == '=')
    {
      n = ScriptAtoI(&pStr, minmax[0], minmax[1]);
      if (n > minmax[0]) minmax[0] = n;
      if (n < minmax[1]) minmax[1] = n;
      continue;
    };
    if (pStr[0] == '<')
    {
      pStr++;
      n = ScriptAtoI(&pStr, minmax[0], 0x7fffffff);
      minmax[1] = n;
      continue;
    };
    if (pStr[0] == '>')
    {
      pStr++;
      n = ScriptAtoI(&pStr, 0, minmax[1]);
      minmax[0] = n;
      continue;
    };
    return CBR_CONTINUE;
  };
  //return CBR_CONTINUE;
}


CBRESULT ScriptCallback_RunAllScripts(CBFUNC func, CString *scriptResult, void *pkt)
{
  return CBR_CONTINUE;
}

CBRESULT ScriptCallback_LookForChar(CBFUNC func, CString *scriptResult, void *pkt)
{
  char *lookFor = (char *)pkt;
  int indx;
  switch(func)
  {
  case CBF_EXAMINESCRIPT:
    indx = scriptResult->FindOneOf(lookFor);
    if (indx < 0)
    {
      return CBR_CONTINUE;
    };
    break;
  case CBF_ENDOFSCRIPTS:
    *scriptResult = emptyString;
    return CBR_STOP;
  default: 
    return CBR_CONTINUE;
  };
  {
    *scriptResult = scriptResult->Mid(indx, 1);
  };
  return CBR_STOP;
}

CBRESULT ScriptCallback_YessesAndNoes(CBFUNC func, CString *scriptResult, void *pkt)
{
  char *pResult = (char *)pkt;
  switch(func)
  {
  case CBF_EXAMINESCRIPT:
    {
      int i;
      int len = scriptResult->GetLength();
      if (len > 5) len = 5;
      for (i=0; i<len; i++)
      {
        char c;
        c = scriptResult->GetAt(i);
        if (c == 'Y') pResult[i] = 'Y';
        if (c == 'N') pResult[i] = 'N';
      };
    };
    break;
  case CBF_ENDOFSCRIPTS:
    *scriptResult = emptyString;
    return CBR_STOP;
  default: 
    return CBR_CONTINUE;
  };
  return CBR_STOP;
}

CBRESULT ScriptCallback_RelOrAbs(CBFUNC func, CString *scriptResult, void *pkt)
{
  int *relAbs = (int *)pkt;
  const char *pC;
  int i, n, sign = 0;
  pC = (LPCTSTR) *scriptResult;
  if (scriptResult->IsEmpty()) return CBR_CONTINUE;
  if (func == CBF_EXAMINESCRIPT)
  {
    if (*pC == '+')
    {
      sign = +1;
      pC++;
    }
    else
    {
      if (*pC == '-') 
      {
        sign = -1;
        pC++;
      }
    };
    i = 0;
    n = 0;
    while (IsDigit(*pC))
    {
      i++;
      n = 10* n + *pC -'0';
      pC++;
    };
    if (i == 0) return CBR_CONTINUE;
    if (sign == 0) relAbs[1] = n;
    else relAbs[0] = sign * n;
  };
  return CBR_CONTINUE;
}




CString frontEnd = "$PUBLIC $FUNC SA(){";
CString backEnd = "\n} SA ;";
const char *SAentries[2] = {"SA", 0};
HOOK_PARAMETERS *p_hook_parameters = NULL;

void ClearHookParameters(void)
{
  int i;
  for (i=0; i<10; i++)
  {
    (*p_hook_parameters)[i] = emptyString;
  };
}

CString SPECIAL_ABILITIES::RunScripts(LPCSTR     scriptName, 
                                      CBRESULT (*fnc)(CBFUNC func, CString *scriptResult, void *pkt), 
                                      void      *pkt,
                                      LPCSTR     SA_Source_Type, 
                                      LPCSTR     SA_Source_Name) const
{
  ASLENTRY *scripts[MAX_SPEC_AB];
  CStringPAIR  saAbility[MAX_SPEC_AB];
  CString binScript;
  int numScript = 0;
  int i;
  const CStringPAIR *pAbility = NULL;
  SPECABILITY_DEF  *pSpecAb;
  ASLENTRY *pSpecString;
  POSITION pos;

  pos = GetHeadPosition();
  while (pos != NULL)
  {
    pAbility = GetNextData(pos);
    if (pAbility == NULL) break;  // Why would this happen?
    pSpecAb = specialAbilitiesData.FindAbility(pAbility->Key());
    if (pSpecAb == NULL) continue;
    pSpecString = pSpecAb->Find(scriptName);
    if (pSpecString == NULL) continue;
    if (numScript >= MAX_SPEC_AB) continue;
    if (pSpecString->Flags() == SPECAB_SCRIPT)
    {
      GPDLCOMP gpdlcomp;
      binScript = gpdlcomp.CompileScript(frontEnd+pSpecString->Value()+backEnd, SAentries);
      if (binScript[0] != 0)
      {
        WriteDebugString("* * * * Script Error in %s[%s]\n", pAbility->Key(),scriptName);
        pSpecString->Value(binScript);
        pSpecString->Flags(SPECAB_SCRIPTERROR);
        pSpecAb->Insert(scriptName, binScript, SPECAB_SCRIPTERROR);
        continue;
      };
      pSpecString->Value(binScript);
      pSpecString->Flags(SPECAB_BINARYCODE);
      pSpecAb->Insert(scriptName, binScript, SPECAB_BINARYCODE);
    };
    if (pSpecString->Flags() != SPECAB_BINARYCODE) continue;
    saAbility[numScript] = *pAbility;
    scripts[numScript++] = pSpecString;
  };
  if (numScript != 0)
  {
    CBRESULT callbackResult;
    for (i=0; i<numScript;  i++)
    {
      pSpecString = scripts[i];
      pScriptContext->SetAbility(this, &saAbility[i]);
      pScriptContext->SetSA_Source_Type(SA_Source_Type);
      pScriptContext->SetSA_Source_Name(SA_Source_Name);
      pScriptContext->SetSA_ScriptName(pSpecString->Key());
      gpdlStack.Push();
      (*p_hook_parameters)[0] = gpdlStack.activeGPDL()->ExecuteScript(scripts[i]->Value(),1, NULL, 0); 
      gpdlStack.Pop();
      pScriptContext->ClearAbility();
      callbackResult = (*fnc)(CBF_EXAMINESCRIPT, &(*p_hook_parameters)[0], pkt);
#ifdef UAFEngine
      if ((globalLoggingFlags&1) || (globalSA_debug.Find(saAbility[i].Key()) != NULL))
      {
        WriteDebugString("@@SA \"%s\" Script \"%s\": %s%s returned \"%s\"\n",
                         saAbility[i].Key(), 
                         scripts[i]->Key(), 
                         SA_Source_Type, 
                         SA_Source_Name, 
                         (*p_hook_parameters)[0]); 
      };
#endif
      if (callbackResult == CBR_STOP)
      {
        return (*p_hook_parameters)[0];
      };
    };
    (*fnc)(CBF_ENDOFSCRIPTS, &(*p_hook_parameters)[0], pkt);
  }
  else
  {
    (*fnc)(CBF_DEFAULT, &(*p_hook_parameters)[0], pkt);
  };
  return (*p_hook_parameters)[0];
}

#ifdef UAFEngine
int IntegerTableLookup(const CString& SAname, 
                       const CString& tableName, 
                       int   iValue,
                       const CString& function)
{
  CString binTable;
//  const CStringPAIR *pAbility = NULL;
  SPECABILITY_DEF  *pSpecAb;
  ASLENTRY *pSpecString;


  pSpecAb = specialAbilitiesData.FindAbility(SAname);
  if (pSpecAb != NULL)
  {
    pSpecString = pSpecAb->Find(tableName);
    if (pSpecString != NULL)
    {
      if (pSpecString->Flags() == SPECAB_INTEGERTABLE)
      {
        //convert text to binTable
        CString text;
        const char *pText;
        int *pArray = NULL;
        int numEnt = 0;
        int col = 0, len;
        int intVal;
        text = pSpecString->Value();
        pText = LPCTSTR(text);
        len = text.GetLength();
        while (col < len)
        {
          if (sscanf(pText+col,"%d", &intVal) == 1)
          {
            pArray = (int *)realloc(pArray, 4*(((numEnt + 10)/10)*10));
            pArray[numEnt++] = intVal;
            while ((col < len) && (pText[col] != '\n'))
            {
              col++;
            };
            col++;
          };
        };
        //if (error) 
        //{
        //  WriteDebugString("* * * * Script Error in %s[%s]\n", pSpecAb->Name(),tableName);
        //  pSpecString->Value(binTable);
        //  pSpecString->Flags(SPECAB_TABLEERROR);
        //  pSpecAb->Insert(tableName, binTable, SPECAB_TABLEERROR);
        //}
        //else
        {
          binTable = CString((char *)pArray, 4*numEnt);
          pSpecString->Value(binTable);
          pSpecString->Flags(SPECAB_BINARYTABLE);
          pSpecAb->Insert(tableName, binTable, SPECAB_BINARYTABLE);
        };
        if (pArray != NULL) free(pArray);
      };
      if (pSpecString->Flags() == SPECAB_BINARYTABLE)
      {
        CString result;
        const int *pArray;
        int i, numEnt;
        pArray = (const int *)LPCSTR(pSpecString->Value());
        numEnt = pSpecString->Value().GetLength() / 4;
        switch (function.GetAt(0))
        {
        case 'I':
          if ((iValue < numEnt) && (iValue >= 0)) return pArray[iValue];
          return -1;
        case 'E':
          for (i=0; i<numEnt; i++)
          {
            if (pArray[i] == iValue) return i;
          };
          return -1;
        case 'G':
          for (i=0; i<numEnt; i++)
          {
            if (pArray[i] > iValue) break;
          };
          return i;
        case 'L':
          for (i=0; i<numEnt; i++)
          {
            if (pArray[i] < iValue) break;
          };
          return i;
        };
        //Search binTable
        return -5;
      }
      else
      {
        return -2;
      };
    }
    else
    {
      return -3;
    };
  }
  else
  {
    return -4;  
  };
}
#endif


CString RunGlobalScript(const char *SAname, const char *scriptName, bool nullSA)
{
//  HOOK_PARAMETERS hookParameters;
  CStringPAIR  saAbility[MAX_SPEC_AB];
  CString binScript;
//  const CStringPAIR *pAbility = NULL;
  SPECABILITY_DEF  *pSpecAb;
  ASLENTRY *pSpecString;


  pSpecAb = specialAbilitiesData.FindAbility(SAname);
  if (pSpecAb != NULL)
  {
    pSpecString = pSpecAb->Find(scriptName);
    if (pSpecString != NULL)
    {
      if (pSpecString->Flags() == SPECAB_SCRIPT)
      {
        GPDLCOMP gpdlcomp;
        binScript = gpdlcomp.CompileScript(frontEnd+pSpecString->Value()+backEnd, SAentries);
        if (binScript[0] != 0)
        {
          WriteDebugString("* * * * Script Error in %s[%s]\n", pSpecAb->Name(),scriptName);
          pSpecString->Value(binScript);
          pSpecString->Flags(SPECAB_SCRIPTERROR);
          pSpecAb->Insert(scriptName, binScript, SPECAB_SCRIPTERROR);
        }
        else
        {
          pSpecString->Value(binScript);
          pSpecString->Flags(SPECAB_BINARYCODE);
          pSpecAb->Insert(scriptName, binScript, SPECAB_BINARYCODE);
        };
      };
      if (pSpecString->Flags() == SPECAB_BINARYCODE)
      {
        CStringPAIR origSA;
        SPECIAL_ABILITIES *pOrigContext;
        gpdlStack.Push();
        if (nullSA)
        {
          CStringPAIR SAcontext(SAname, "");
          origSA = pScriptContext->specAb;
          pOrigContext = pScriptContext->context;
          pScriptContext->SetAbility(NULL, &SAcontext);
          pScriptContext->SetSA_ScriptName(scriptName);
        };
        (*p_hook_parameters)[0] = gpdlStack.activeGPDL()->ExecuteScript(pSpecString->Value(),1, NULL, 0); 
        //hookParameters[0] = gpdlStack[++activeGPDL].ExecuteScript(pSpecString->Value(),1, NULL, 0); 
        gpdlStack.Pop();
        if (nullSA)
        {
          pScriptContext->specAb = origSA;
        };
        //callbackResult = ScriptCallback_RunAllScripts(CBF_EXAMINESCRIPT, &(*p_hook_parameters)[0], NULL);
#ifdef UAFEngine
        if (globalSA_debug.Find(SAname) != NULL)
        {
          WriteDebugString("@@SA \"%s\" Script \"%s\": %s%s\n", SAname, scriptName, NULL, NULL); 
        };
#endif
      }
    };
  }
  else
  {
    ScriptCallback_RunAllScripts(CBF_DEFAULT, &(*p_hook_parameters)[0], NULL);
  };
  return (*p_hook_parameters)[0];
}



#ifdef UAFEDITOR

void TestOneScript(const CString& abilityName, const ASLENTRY *pASL)
{
  CString start("$PUBLIC $FUNC SpecAbTest() { ");
  CString end("\n } SpecAbTest;");
  CString func = "SpecAbTest";
  GPDLCOMP gpdlComp;
  CString script;
  CString msg;
  script = start + pASL->Value() + end;
  msg.Format("Testing %s[%s]\n", abilityName, pASL->Key());
  WriteDebugString(msg);
  msg.Empty();
	IsSyntaxAndSemanticsValid(script, func, &msg, 0xffffffff);
  if (!msg.IsEmpty())
  {
    WriteDebugString("%s", msg);
  };
}

void TestOneSpecialAbility(const SPECABILITY_DEF *pSpecAb)
{
  POSITION pos;
  const ASLENTRY *pASL;
  pos = pSpecAb->GetStartPosition();
  while (pos != NULL)
  {
    pSpecAb->GetNextEntry(pos, &pASL);
    if (pASL->Flags() != SPECAB_SCRIPT) continue;
    TestOneScript(pSpecAb->Name(), pASL);
  };
}

void TestAllSpecialAbilities(A_SPECABILITY_DEF_L *pAbilities)
{
    POSITION pos;
    const SPECABILITY_DEF* pSaData;
    pos = pAbilities->GetStartPosition();
    while (pos != NULL)
    {
      pSaData = pAbilities->GetNext(pos);
      TestOneSpecialAbility(pSaData);
    };
};

#endif
