#include "stdafx.h"
#include "ConvertFRUA.h"
#include "Translate.h"


extern CString FRUAExportDate, FRUAExportTime;
extern CString ConvertFRUAVersion;
#define HIGHEST_CHARACTER_PRIME 25

FILE *classFile;

struct HIT_DICE_LEVEL_BONUS
{
  CString ability;
  CString baseclassID;
  char bonusValues[HIGHEST_CHARACTER_PRIME];
  bool operator ==(const HIT_DICE_LEVEL_BONUS& src) const;
  void Export(void);
};


class CLASS
{
public:
  CLASS(void){};
  ~CLASS(void){};
  CLASS& operator =(const CLASS& src);
  void Clear(void);
  void DefaultFighter(const CString& startingEquipment);
  void DefaultCleric(const CString& startingEquipment);
  void DefaultRanger(const CString& startingEquipment);
  void DefaultMagicUser(const CString& startingEquipment);
  void DefaultPaladin(const CString& startingEquipment);
  void DefaultThief(const CString& startingEquipment);
  void Export(void);
  void AddStartingEquipment(char *item);

  
  CString strengthBonusDice;
  CString m_name;
  CString hitDiceBaseclassID;
  CArray <CString, CString&> m_baseclasses;
  CArray <CString, CString&> m_specialAbilities;
  CArray <CString, CString&> m_startingEquipment;
  CString hitDiceLevelBonus;
};

void CLASS::Clear(void)
{
  strengthBonusDice.Empty();
  m_name.Empty();
  hitDiceBaseclassID.Empty();
  m_baseclasses.RemoveAll();
  m_specialAbilities.RemoveAll();
  m_startingEquipment.RemoveAll();
}

void CLASS::AddStartingEquipment(char *item)
{
  m_startingEquipment.Add(CString(item));
}

void CLASS::Export(void)
{
  int i;
  fprintf(classFile, "\\(BEGIN)\n");
  fprintf(classFile, "name = %s\n", m_name);
  fprintf(classFile, "HitDiceLevelBonus = %s\n", hitDiceLevelBonus);
  fprintf(classFile, "HitDiceBaseclass = %s\n", hitDiceBaseclassID);
  fprintf(classFile, "StrengthBonusDice = %s\n", strengthBonusDice);
  for (i=0; i<m_startingEquipment.GetSize(); i++)
  {
    fprintf(classFile, "Starting Equipment = %s\n", m_startingEquipment[i]);
  };
  for (i=0; i<m_baseclasses.GetSize(); i++)
  {
    fprintf(classFile, "Baseclass = %s\n", m_baseclasses[i]);
  };
  fprintf(classFile,"\\(END)\n");
}




// Class database file
// The line above **must** be the first line of the classes file!!!


void CLASS::DefaultFighter(const CString& startingEquip)
{
  Clear();
  m_name = "Fighter";
  hitDiceLevelBonus = "fighter,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,3,4";
  hitDiceBaseclassID = "*Average*";
  strengthBonusDice = "1D100";
  if (startingEquip == "poor")
  {
    AddStartingEquipment("Ring Mail");
    AddStartingEquipment("Spear");
  }
  else if (startingEquip == "Modest")
  {
    AddStartingEquipment("Shield");
    AddStartingEquipment("Scale Mail");
    AddStartingEquipment("Broad Sword");
  }
  else if (startingEquip == "  Average")
  { 
    AddStartingEquipment("Arrows");
    AddStartingEquipment("Long Bow");
    AddStartingEquipment("Shield");
    AddStartingEquipment("Banded Mail");
    AddStartingEquipment("Long Sword");
  }
  else if (startingEquip == "Prosperous")
  {
    AddStartingEquipment("20 Arrows");
    AddStartingEquipment("Composite Long Bow");
    AddStartingEquipment("Plate Mail");
    AddStartingEquipment("Two-Handed Sword");
  }
  else if (startingEquip == "prosperous +1")
  {
    AddStartingEquipment("Helm +1");
    AddStartingEquipment("Shield +1");
    AddStartingEquipment("10 Arrows +1");
    AddStartingEquipment("Composite Long Bow +1");
    AddStartingEquipment("Plate Mail +1");
    AddStartingEquipment("Long Sword +1");
  }
  else if (startingEquip == "prosperous +2")
  {
    AddStartingEquipment("Helm +2");
    AddStartingEquipment("Composite Long Bow +2");
    AddStartingEquipment("40 Arrows +2");
    AddStartingEquipment("Shield +2");
    AddStartingEquipment("Plate Mail +2");
    AddStartingEquipment("Long Sword +2");
  }
  else if (startingEquip == "prosperous +3")
  {
    AddStartingEquipment("Helm +3");
    AddStartingEquipment("Shield +3");
    AddStartingEquipment("Composite Long Bow +3");
    AddStartingEquipment("10 Arrows +3");
    AddStartingEquipment("Plate Mail +3");
    AddStartingEquipment("Long Sword +3");
  }
  else if (startingEquip == "prosperous +4")
  {
    AddStartingEquipment("Composite Long Bow +4");
    AddStartingEquipment("10 Arrows +4");
    AddStartingEquipment("Helm +4");
    AddStartingEquipment("Shield +4");
    AddStartingEquipment("Plate Mail +4");
    AddStartingEquipment("Long Sword +4");
  };
  m_baseclasses.Add(CString("fighter"));
  m_specialAbilities.Add(CString("FighterAttacksPerRound"));
  m_specialAbilities.Add(CString("NoSpells"));
}
/*
\(BEGIN)
name = Cleric
HitDiceBonus = Cleric
HitDiceLevelBonus = cleric,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2
HitDiceBaseclass = *Average*
StrengthBonusDice = 
Starting Equipment = 
Baseclass = cleric
\(END)
*/
void CLASS::DefaultCleric(const CString& startingEquip)
{
  Clear();
  m_name = "Cleric";
  hitDiceLevelBonus = "cleric,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2";
  hitDiceBaseclassID = "*Average*";
  strengthBonusDice = "";
  if (startingEquip == "poor")
  {
    AddStartingEquipment("Helm");
    AddStartingEquipment("Club");
  }
  else if (startingEquip == "Modest")
  {
    AddStartingEquipment("Shield");
    AddStartingEquipment("Ring Mail");
    AddStartingEquipment("Mace");
  }
  else if (startingEquip == "  Average")
  { 
    AddStartingEquipment("Hammer");
    AddStartingEquipment("Shield");
    AddStartingEquipment("Chain Mail");
    AddStartingEquipment("Mace");
  }
  else if (startingEquip == "prosperous")
  {
    AddStartingEquipment("Shield");
    AddStartingEquipment("Plate Mail");
    AddStartingEquipment("Staff Sling");
    AddStartingEquipment("Flail");
  }
  else if (startingEquip == "prosperous +1")
  {
    AddStartingEquipment("Staff Sling +1");
    AddStartingEquipment("Shield +1");
    AddStartingEquipment("Banded Mail +1");
    AddStartingEquipment("Mace +1");
  }
  else if (startingEquip == "prosperous +2")
  {
    AddStartingEquipment("Helm +2");
    AddStartingEquipment("Composite Long Bow +2");
    AddStartingEquipment("40 Arrows +2");
    AddStartingEquipment("Shield +2");
    AddStartingEquipment("Plate Mail +2");
    AddStartingEquipment("Long Sword +2");
  }
  else if (startingEquip == "prosperous +3")
  {
    AddStartingEquipment("Helm +3");
    AddStartingEquipment("Shield +3");
    AddStartingEquipment("Composite Long Bow +3");
    AddStartingEquipment("10 Arrows +3");
    AddStartingEquipment("Plate Mail +3");
    AddStartingEquipment("Long Sword +3");
  }
  else if (startingEquip == "prosperous +4")
  {
    AddStartingEquipment("Composite Long Bow +4");
    AddStartingEquipment("10 Arrows +4");
    AddStartingEquipment("Helm +4");
    AddStartingEquipment("Shield +4");
    AddStartingEquipment("Plate Mail +4");
    AddStartingEquipment("Long Sword +4");
  };
  m_baseclasses.Add(CString("cleric"));
}

/*
\(BEGIN)
name = Ranger
HitDiceBonus = Ranger
HitDiceLevelBonus = ranger,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2
HitDiceBaseclass = *Average*
StrengthBonusDice = 1D100
Starting Equipment = 
Baseclass = ranger
Special Ability = RangerAttacksPerRound
\(END)
*/
void CLASS::DefaultRanger(const CString& startingEquip)
{
  Clear();
  m_name = "Ranger";
  hitDiceLevelBonus = "ranger,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2";
  hitDiceBaseclassID = "*Average*";
  strengthBonusDice = "1D100";
  if (startingEquip == "poor")
  {
    AddStartingEquipment("Ring Mail");
    AddStartingEquipment("Spear");
  }
  else if (startingEquip == "Modest")
  {
    AddStartingEquipment("Shield");
    AddStartingEquipment("Scale Mail");
    AddStartingEquipment("Broad Sword");
  }
  else if (startingEquip == "  Average")
  { 
    AddStartingEquipment("Arrows");
    AddStartingEquipment("Long Bow");
    AddStartingEquipment("Shield");
    AddStartingEquipment("Banded Mail");
    AddStartingEquipment("Long Sword");
  }
  else if (startingEquip == "prosperous")
  {
    AddStartingEquipment("20 Arrows");
    AddStartingEquipment("Composite Long Bow");
    AddStartingEquipment("Plate Mail");
    AddStartingEquipment("Two-Handed Sword");
  }
  else if (startingEquip == "prosperous +1")
  {
    AddStartingEquipment("Helm +1");
    AddStartingEquipment("Shield +1");
    AddStartingEquipment("10 Arrows +1");
    AddStartingEquipment("Composite Long Bow +1");
    AddStartingEquipment("Plate Mail +1");
    AddStartingEquipment("Long Sword +1");
  }
  else if (startingEquip == "prosperous +2")
  {
    AddStartingEquipment("Helm +2");
    AddStartingEquipment("Composite Long Bow +2");
    AddStartingEquipment("40 Arrows +2");
    AddStartingEquipment("Shield +2");
    AddStartingEquipment("Plate Mail +2");
    AddStartingEquipment("Long Sword +2");
  }
  else if (startingEquip == "prosperous +3")
  {
    AddStartingEquipment("Helm +3");
    AddStartingEquipment("Shield +3");
    AddStartingEquipment("Composite Long Bow +3");
    AddStartingEquipment("10 Arrows +3");
    AddStartingEquipment("Plate Mail +3");
    AddStartingEquipment("Long Sword +3");
  }
  else if (startingEquip == "prosperous +4")
  {
    AddStartingEquipment("Composite Long Bow +4");
    AddStartingEquipment("10 Arrows +4");
    AddStartingEquipment("Helm +4");
    AddStartingEquipment("Shield +4");
    AddStartingEquipment("Plate Mail +4");
    AddStartingEquipment("Long Sword +4");
  };
  m_baseclasses.Add(CString("ranger"));
  m_specialAbilities.Add(CString("RangerAttacksPerRound"));
}
/*
\(BEGIN)
name = Paladin
HitDiceBonus = Paladin
HitDiceLevelBonus = paladin,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2
HitDiceBaseclass = *Average*
StrengthBonusDice = 1D100
Starting Equipment = 
Baseclass = paladin
Special Ability = IsProtectedEvil
Special Ability = PaladinAttacksPerRound
Special Ability = PaladinImmuneDisease
\(END)
*/
void CLASS::DefaultPaladin(const CString& startingEquip)
{
  Clear();
  m_name = "Paladin";
  hitDiceLevelBonus = "paladin,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2";
  hitDiceBaseclassID = "*Average*";
  strengthBonusDice = "1D100";
  if (startingEquip == "poor")
  {
    AddStartingEquipment("Ring Mail");
    AddStartingEquipment("Spear");
  }
  else if (startingEquip == "Modest")
  {
    AddStartingEquipment("Shield");
    AddStartingEquipment("Scale Mail");
    AddStartingEquipment("Broad Sword");
  }
  else if (startingEquip == "  Average")
  { 
    AddStartingEquipment("Arrows");
    AddStartingEquipment("Long Bow");
    AddStartingEquipment("Shield");
    AddStartingEquipment("Banded Mail");
    AddStartingEquipment("Long Sword");
  }
  else if (startingEquip == "prosperous")
  {
    AddStartingEquipment("20 Arrows");
    AddStartingEquipment("Composite Long Bow");
    AddStartingEquipment("Plate Mail");
    AddStartingEquipment("Two-Handed Sword");
  }
  else if (startingEquip == "prosperous +1")
  {
    AddStartingEquipment("Helm +1");
    AddStartingEquipment("Shield +1");
    AddStartingEquipment("10 Arrows +1");
    AddStartingEquipment("Composite Long Bow +1");
    AddStartingEquipment("Plate Mail +1");
    AddStartingEquipment("Long Sword +1");
  }
  else if (startingEquip == "prosperous +2")
  {
    AddStartingEquipment("Helm +2");
    AddStartingEquipment("Composite Long Bow +2");
    AddStartingEquipment("40 Arrows +2");
    AddStartingEquipment("Shield +2");
    AddStartingEquipment("Plate Mail +2");
    AddStartingEquipment("Long Sword +2");
  }
  else if (startingEquip == "prosperous +3")
  {
    AddStartingEquipment("Helm +3");
    AddStartingEquipment("Shield +3");
    AddStartingEquipment("Composite Long Bow +3");
    AddStartingEquipment("10 Arrows +3");
    AddStartingEquipment("Plate Mail +3");
    AddStartingEquipment("Long Sword +3");
  }
  else if (startingEquip == "prosperous +4")
  {
    AddStartingEquipment("Composite Long Bow +4");
    AddStartingEquipment("10 Arrows +4");
    AddStartingEquipment("Helm +4");
    AddStartingEquipment("Shield +4");
    AddStartingEquipment("Plate Mail +4");
    AddStartingEquipment("Long Sword +4");
  };
  m_baseclasses.Add(CString("paladin"));
  m_specialAbilities.Add(CString("IsProtectedEvil"));
  m_specialAbilities.Add(CString("PaladinAttacksPerRound"));
  m_specialAbilities.Add(CString("PaladinImmuneDisease"));
}
/*
\(BEGIN)
name = Magic User
HitDiceBonus = Magic User
HitDiceLevelBonus = magicUser,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2
HitDiceBaseclass = *Average*
StrengthBonusDice = 
Starting Equipment = 
Baseclass = magicUser
\(END)
*/
void CLASS::DefaultMagicUser(const CString& startingEquip)
{
  Clear();
  m_name = "Magic-User";
  hitDiceLevelBonus = "magicUser,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2";
  hitDiceBaseclassID = "*Average*";
  strengthBonusDice = "";
  if (startingEquip == "poor")
  {
    AddStartingEquipment("Club");
  }
  else if (startingEquip == "Modest")
  {
    AddStartingEquipment("Leather Armor");
    AddStartingEquipment("Short Sword");
  }
  else if (startingEquip == "  Average")
  { 
    AddStartingEquipment("Sling,");
    AddStartingEquipment("Leather Armor");
    AddStartingEquipment("Broad Sword");
  }
  else if (startingEquip == "prosperous")
  {
    AddStartingEquipment("Elfin Chain");
    AddStartingEquipment("20 Arrows");
    AddStartingEquipment("Composite Short Bow");
    AddStartingEquipment("Long Sword");
  }
  else if (startingEquip == "prosperous +1")
  {
    AddStartingEquipment("10 Arrows +1");
    AddStartingEquipment("Composite Short Bow +1");
    AddStartingEquipment("Elfin Chain +1");
    AddStartingEquipment("Long Sword +1");
  }
  else if (startingEquip == "prosperous +2")
  {
    AddStartingEquipment("Elfin Chain +2");
    AddStartingEquipment("Sling +2");
    AddStartingEquipment("Long Sword +2");
  }
  else if (startingEquip == "prosperous +3")
  {
    AddStartingEquipment("Elfin Chain +3");
    AddStartingEquipment("Sling +3");
    AddStartingEquipment("Long Sword +3");
  }
  else if (startingEquip == "prosperous +4")
  {
    AddStartingEquipment("Cloak of Prot. +4");
    AddStartingEquipment("Bracers of AC 2");
    AddStartingEquipment("Sling +4");
    AddStartingEquipment("Long Sword +4");
  };
  m_baseclasses.Add(CString("magicUser"));
}

/*
\(BEGIN)
name = Thief
HitDiceBonus = Thief
HitDiceLevelBonus = thief,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2
HitDiceBaseclass = *Average*
StrengthBonusDice = 
Starting Equipment = 
Baseclass = thief
Special Ability = NoSpells
\(END)
*/
void CLASS::DefaultThief(const CString& startingEquip)
{
  Clear();
  m_name = "Thief";
  hitDiceLevelBonus = "thief,Constitution,0,0,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,1,2,2,2";
  hitDiceBaseclassID = "*Average*";
  strengthBonusDice = "";
  if (startingEquip == "poor")
  {
    AddStartingEquipment("Ring Mail");
    AddStartingEquipment("Spear");
  }
  else if (startingEquip == "Modest")
  {
    AddStartingEquipment("Shield");
    AddStartingEquipment("Scale Mail");
    AddStartingEquipment("Broad Sword");
  }
  else if (startingEquip == "  Average")
  { 
    AddStartingEquipment("Arrows");
    AddStartingEquipment("Long Bow");
    AddStartingEquipment("Shield");
    AddStartingEquipment("Banded Mail");
    AddStartingEquipment("Long Sword");
  }
  else if (startingEquip == "prosperous")
  {
    AddStartingEquipment("20 Arrows");
    AddStartingEquipment("Composite Long Bow");
    AddStartingEquipment("Plate Mail");
    AddStartingEquipment("Two-Handed Sword");
  }
  else if (startingEquip == "prosperous +1")
  {
    AddStartingEquipment("Helm +1");
    AddStartingEquipment("Shield +1");
    AddStartingEquipment("10 Arrows +1");
    AddStartingEquipment("Composite Long Bow +1");
    AddStartingEquipment("Plate Mail +1");
    AddStartingEquipment("Long Sword +1");
  }
  else if (startingEquip == "prosperous +2")
  {
    AddStartingEquipment("Helm +2");
    AddStartingEquipment("Composite Long Bow +2");
    AddStartingEquipment("40 Arrows +2");
    AddStartingEquipment("Shield +2");
    AddStartingEquipment("Plate Mail +2");
    AddStartingEquipment("Long Sword +2");
  }
  else if (startingEquip == "prosperous +3")
  {
    AddStartingEquipment("Helm +3");
    AddStartingEquipment("Shield +3");
    AddStartingEquipment("Composite Long Bow +3");
    AddStartingEquipment("10 Arrows +3");
    AddStartingEquipment("Plate Mail +3");
    AddStartingEquipment("Long Sword +3");
  }
  else if (startingEquip == "prosperous +4")
  {
    AddStartingEquipment("Composite Long Bow +4");
    AddStartingEquipment("10 Arrows +4");
    AddStartingEquipment("Helm +4");
    AddStartingEquipment("Shield +4");
    AddStartingEquipment("Plate Mail +4");
    AddStartingEquipment("Long Sword +4");
  };
  m_baseclasses.Add(CString("thief"));
  m_specialAbilities.Add(CString("NoSpells"));
}


void ConvertClasses(JReader& globals)
{
  CLASS tempClass;
  CString startEquip;
  classFile = fopen(jsonPath + "Classes.txt", "w");
  if (classFile == NULL)
  {
    printf("Cannot open file Classes.txt for writing\n");
  };
  fprintf(classFile,"// Class database file\n");
  fprintf(classFile,"// The line above **must** be the first line of the classes file!!!\n");
  fprintf(classFile,"// FRUA Export Version %s:%s\n", FRUAExportDate, FRUAExportTime);
  fprintf(classFile,"// ConvertFRUA version %s\n", ConvertFRUAVersion);

  startEquip = globals.Fetch_String("start_equipment_level");
  tempClass.DefaultFighter(startEquip);
  tempClass.Export();
  tempClass.DefaultCleric(startEquip);
  tempClass.Export();
  tempClass.DefaultRanger(startEquip);
  tempClass.Export();
  tempClass.DefaultMagicUser(startEquip);
  tempClass.Export();
  tempClass.DefaultPaladin(startEquip);
  tempClass.Export();
  tempClass.DefaultThief(startEquip);
  tempClass.Export();
  fclose(classFile);
}
