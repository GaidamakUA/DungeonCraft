#include "StdAfx.h"
#include "Translate.h"
//
extern FILE *f;
extern CString jsonPath;
extern CString executablePath;
extern CString FRUAExportDate, FRUAExportTime;
extern CString ConvertFRUAVersion;

enum weaponClassType 
{ 
                  // CanAttack?   AttackAdjacent?   AttackRanged?   NeedsAmmoReadied?  ConsumesAmmo?
                  //
  NotWeapon=0,    //   No              No               No                N/A              N/A
  HandBlunt=1,    //   Yes             Yes              Yes               No               No
  HandCutting=2,  //   Yes             Yes              Yes               No               No
  HandThrow=3,    //   Yes             Yes              Yes               No               if range > 1
  SlingNoAmmo=4,  //   Yes             No               Yes               No               No
  Bow=5,          //   Yes             No               Yes               Yes              Yes
  Crossbow=6,     //   Yes             No               Yes               Yes              Yes
  Throw=7,        //   Yes             No               Yes               No               Yes
  Ammo=8,         //   No              N/A              No                N/A              N/A
  SpellCaster     //   Yes             Yes              Yes               No               Yes

  //
  //  CanAttack = isWeapon()
  //
  //  AttackRanged = WpnCanAttackAtRange(Range>1)
  //
  //  AttackAdjacent = WpnCanAttackAtRange(Range=1)
  //
  //  NeedsAmmoReadied = WpnUsesAmmo()
  //
  //  ConsumesAmmo = WpnConsumesAmmoAtRange(Range)
  //
};



struct CONFIGID
{
  CONFIGID(const char *n, weaponClassType t){text=n; type=t;};
  CString text;
  weaponClassType type;
};

  CONFIGID typeIDs[] =
  {
    CONFIGID( "not weapon"            ,NotWeapon),
    CONFIGID( "hand held blunt"       ,HandBlunt),
    CONFIGID( "hand held cutting"     ,HandCutting),
    CONFIGID( "hand held or thrown"   ,HandThrow),
    CONFIGID( "sling"                 ,SlingNoAmmo),
    CONFIGID( "bow"                   ,Bow),
    CONFIGID( "crossbow"              ,Crossbow),
    CONFIGID( "thrown only"           ,Throw),
    CONFIGID( "ammo"                  ,Ammo),
    CONFIGID( "SpellCaster"           ,SpellCaster),
    CONFIGID( ""          ,NotWeapon)
  };
  const int numWeaponType = sizeof(typeIDs)/sizeof(typeIDs[0]);


#define base38(a,b,c,d,e,f) ( (((((unsigned)a*38+b)*38+c)*38+d)*38+e)*38+f )
#define BASE38(a,b,c,d,e,f) (base38((a-'A'+12),(b-'A'+12),(c-'A'+12),(d-'A'+12),(e-'A'+12),(f-'A'+12)));
typedef DWORD itemReadiedType;
#define blank ('A'-11)

CString CStringFromBase38(unsigned int v)
{
  switch(v)
  {
  case 0: return "WEAPON";
  case 1: return "SHIELD";
  case 2: return "ARMOR";
  case 3: return "HANDS";
  case 4: return "HEAD";
  case 5: return "WAIST";
  case 6: return "ROBE";
  case 7: return "CLOAK";
  case 8: return "FEET";
  case 9: return "FINGER";
  case 10: return "QUIVER";
  case 11: return "CANNOT";
  default:
    {
      int i;
      char result[6];
      for (i=5; i>=0; i--)
      {
        int remainder;
        remainder = v%38;
        v /= 38;
        if (remainder < 1) result[i] = '?';
        else if (remainder < 2) result[i] = ' ';
        else if (remainder < 12) result[i] = '0' + remainder - 2;
        else result[i] = remainder - 12 + 'A';
      };
      return CString(result,6);
    };
  };
}




const itemReadiedType WeaponHand = BASE38('W','E','A','P','O','N');
const itemReadiedType ShieldHand = BASE38('S','H','I','E','L','D');
const itemReadiedType BodyArmor  = BASE38('A','R','M','O','R',blank);
const itemReadiedType AmmoQuiver = BASE38('Q','U','I','V','E','R');
const itemReadiedType Hands      = BASE38('H','A','N','D','S',blank);
const itemReadiedType Head       = BASE38('H','E','A','D',blank,blank);
const itemReadiedType Waist      = BASE38('W','A','I','S','T',blank);
const itemReadiedType BodyRobe   = BASE38('R','O','B','E',blank,blank);
const itemReadiedType Back       = BASE38('C','L','O','A','K',blank);
const itemReadiedType Feet       = BASE38('F','E','E','T',blank,blank);
const itemReadiedType Fingers    = BASE38('F','I','N','G','E','R');
const itemReadiedType Cannot     = BASE38('C','A','N','N','O','T');

enum itemRechargeRate 
{ 
  irrNever, // fixed qty of charges, once gone no 'Use' is possible
  irrDaily  // full qty of charges restored each day
};



class ITEM_DATA //: public CObject
{
public:

//  DECLARE_SERIAL( ITEM_DATA )
  ITEM_DATA(void){Clear(TRUE);};
  void operator =(const ITEM_DATA& src);
  void Clear(BOOL);
  void Read(JReader& jr);
  void Export(void);
  int inputLineNum;
  CString  m_uniqueName;                                                 //        **
	CString  m_commonName; 
  CString fruaID;//        **
  //CString  Id_Name;                                                    //        **
  CString  m_idName;                                                     //        **
  CString  AmmoType;
  CString  HitSound;
  CString  MissSound;
  CString  LaunchSound;
  long  Experience;
  long	Cost;
	long	Encumbrance;
	long	Attack_Bonus;
	BOOL	Cursed;
	long	Bundle_Qty;
	long	Num_Charges;
	itemReadiedType	Location_Readied;  // A base-38 name!
	long	Hands_to_Use;  
	long	Dmg_Dice_Sm;
	long	Nbr_Dice_Sm;
	long	Dmg_Bonus_Sm;
	long	Dmg_Dice_Lg;
	long	Nbr_Dice_Lg;
	long	Dmg_Bonus_Lg;
	double ROF_Per_Round;
  long  Protection_Base;
	long	Protection_Bonus;
  weaponClassType	Wpn_Type;
  int   m_usageFlags;
enum 
{
  USAGE_usable     = 0x00000001,
  USAGE_scribable  = 0x00000002
};
  BOOL  CanBeHalvedJoined;
  BOOL  CanBeTradeDropSoldDep;
	//long	Usable_by_Class;
  CArray<CString, CString&> usableByBaseclass;
  long	Range;
  //GLOBAL_SPELL_ID  m_gsID;
  CString spellID;
  //long  Spell_Level;
  //long  Spell_Class;
  DWORD m_useEvent;
  DWORD ExamineEvent;
  CString ExamineLabel;
  CString attackMsg;
  CString Recharge_Rate; //itemRechargeRate Recharge_Rate;
  BOOL IsNonLethal;
  CString specAbs;
  CString MissileArt; //PIC_DATA  MissileArt; // in-route sprite
  CString HitArt;     //PIC_DATA  HitArt;     // target hit sprite

  CString item_asl;
  BOOL  canHalveJoin;
};

void ITEM_DATA::Clear(BOOL ctor)
{
  //if (event > 0)
  //  globalData.eventData.DeleteEvents(event);  
  //if (ExamineEvent > 0)
  //  globalData.eventData.DeleteEvents(ExamineEvent);
  //key=0;
  //m_giID.Clear();
  m_useEvent=0;
  ExamineEvent=0;
  ExamineLabel="EXAMINE";
  attackMsg="attacks";
  m_uniqueName="";
  m_commonName = "";
  m_idName="";
  fruaID = "";
  HitSound = "";
  MissSound = "";
  LaunchSound = "";
  AmmoType="";
  Experience = 0;
  Cost = 0;
  Encumbrance = 0;
  Attack_Bonus = 0;
  Cursed = FALSE;
  Bundle_Qty = 1;
  Num_Charges = 0;
  Location_Readied = WeaponHand;
  Hands_to_Use = 1;  
  Dmg_Dice_Sm = 99;
  Nbr_Dice_Sm = 99;
  Dmg_Bonus_Sm = 99;
  Dmg_Dice_Lg = 99;
  Nbr_Dice_Lg = 99;
  Dmg_Bonus_Lg = 99;
  ROF_Per_Round = 1.0;
  Protection_Base = 0;
  Protection_Bonus = 0;
  Wpn_Type = NotWeapon;
  m_usageFlags = 0;
  //Usable_by_Class = FighterFlag;
  usableByBaseclass.RemoveAll();
  Recharge_Rate = irrNever;
  IsNonLethal=FALSE;
  CanBeHalvedJoined=TRUE;
  CanBeTradeDropSoldDep=TRUE;
  Range = 0;
  //m_gsID.Clear();
#ifdef UAFEDITOR
//  preVersionSpellNames_giID = -1;
  preVersionSpellNames_gsID = -1;
#endif
  spellID.Empty();
  //Spell_Level = 1; 

  //Spell_Class = MagicUserFlag;


  item_asl.Empty();
  specAbs.Empty();
}


void ITEM_DATA::operator =(const ITEM_DATA& src)
{
  inputLineNum = src.inputLineNum;
  m_uniqueName = src.m_uniqueName;                                                 //        **
	m_commonName = src.m_commonName;
  m_idName = src.m_idName;
  fruaID = src.fruaID; //        **
  AmmoType = src.AmmoType;
  HitSound = src.HitSound;
  MissSound = src.MissSound;
  LaunchSound = src.LaunchSound;
  Experience = src.Experience;
  Cost = src.Cost;
	Encumbrance = src.Encumbrance;
	Attack_Bonus = src.Attack_Bonus;
	Cursed = src.Cursed;
	Bundle_Qty = src.Bundle_Qty;
  canHalveJoin = src.canHalveJoin;
	Num_Charges = src.Num_Charges;
	Location_Readied = src.Location_Readied;  // A base-38 name!
	Hands_to_Use = src.Hands_to_Use;  
	Dmg_Dice_Sm = src.Dmg_Dice_Sm;
	Nbr_Dice_Sm = src.Nbr_Dice_Sm;
	Dmg_Bonus_Sm = src.Dmg_Bonus_Sm;
	Dmg_Dice_Lg = src.Dmg_Dice_Lg;
	Nbr_Dice_Lg = src.Nbr_Dice_Lg;
	Dmg_Bonus_Lg = src.Dmg_Bonus_Lg;
	ROF_Per_Round = src.ROF_Per_Round;
  Protection_Base = src.Protection_Base;
	Protection_Bonus = src.Protection_Bonus;
  Wpn_Type = src.Wpn_Type;
  m_usageFlags = src.m_usageFlags;
  CanBeHalvedJoined = src.CanBeHalvedJoined;
  CanBeTradeDropSoldDep = src.CanBeTradeDropSoldDep;
  usableByBaseclass.RemoveAll(); usableByBaseclass.Append(src.usableByBaseclass);
  Range = src.Range;
  spellID = src.spellID;
  m_useEvent = src.m_useEvent;
  ExamineEvent = src.ExamineEvent;
  ExamineLabel = src.ExamineLabel;
  attackMsg = src.attackMsg;
  Recharge_Rate = src.Recharge_Rate; //itemRechargeRate Recharge_Rate;
  IsNonLethal = src.IsNonLethal;
  specAbs = src.specAbs;
  MissileArt = src.MissileArt; //PIC_DATA  MissileArt; // in-route sprite
  HitArt = src.HitArt;     //PIC_DATA  HitArt;     // target hit sprite
  item_asl = src.item_asl;
}

#define NOTIMPLEMENTED 24681357

void Line(const char *tag, const char *val);
void Line(const char *tag, int v);
#define DEFID(tag,type,val) tag = val;

#define Str(tag) jr.Fetch_String(tag)
#define Int(tag) jr.Fetch_Int(tag)
#define Bol(tag) jr.Fetch_Bool(tag)
#define Flt(tag) jr.Fetch_Float(tag)

/*
void ITEM_DATA::Read(JReader& jr)
{
  jr.StartList(DBGID_singleItem);
  inputLineNum = jr.LineNum();
  m_commonName = jr.Fetch_String("name");
  m_idName = jr.Fetch_String("identified_name");
  fruaID = jr.Fetch_String("id");
  m_uniqueName = m_idName;

  Cost              = Int("price_platinum");
  Hands_to_Use      = Int("hands_needed");
  Cursed            = Bol("cursed");
  Num_Charges       = Int("num_charges");
  Protection_Base   = Int("protection_base");
  Protection_Bonus  = Int("protection_bonus");
  Range             = Int("range");
  ROF_Per_Round     = Flt("rate");
  specAbs           = Str("special_ability");
  Dmg_Dice_Lg       = Int("vs_large_numSides");
  Nbr_Dice_Lg       = Int("vs_large_numDice");
  Dmg_Bonus_Lg      = Int("vs_large_bonus");
  Dmg_Dice_Sm       = Int("vs_small_numSides");
  Nbr_Dice_Sm       = Int("vs_small_numDice");
  Dmg_Bonus_Sm      = Int("vs_small_bonus");
  Encumbrance       = Int("encumbrance_coins");
  {
    CString loc;
    loc = Str("ready_slot"); // For backward compatibility
    if      (loc == "weapon_hand") Location_Readied = WeaponHand;
    else if (loc = "none")         Location_Readied = Cannot;
    else
    {
      CString msg;
      msg.Format("Cannot determine readied location of %s", loc);
      jr.PrintError(msg);
    };
  };
  {
    CString wtype;
    wtype = Str("weapon_type");
    if      (wtype == "hand_thrown") Wpn_Type = HandThrow;
    else if (wtype == "none")        Wpn_Type = NotWeapon;
    else if (wtype == "hand_cutting")Wpn_Type = HandCutting;
    else if (wtype == "crossbow")    Wpn_Type = Crossbow;
    else if (wtype == "hand_blunt")  Wpn_Type = HandBlunt;
    else if (wtype == "thrown")      Wpn_Type = Throw;
    else if (wtype == "sling")       Wpn_Type = SlingNoAmmo;
    else if (wtype == "ammo")        Wpn_Type = Ammo;
    else if (wtype == "bow")         Wpn_Type = Bow;
    else if (wtype == "fine_bow")    Wpn_Type = Bow;
    else
    {
      CString msg;
      msg.Format("Cannot determine weapon type of \"%s\"", wtype);
      jr.PrintError(msg);
    };
  };
         if (Str("useable_cleric") == "1")
         {
           usableByBaseclass.Add(CString("cleric"));
         };
         if (Str("useable_fighter")        == "1")
         {
           usableByBaseclass.Add(CString("fighter"));
         };
         if (Str("useable_knight")         == "1")
         {
           usableByBaseclass.Add(CString("knight"));
         };
         if (Str("useable_magic_user")     == "1")
         {
           usableByBaseclass.Add(CString("magic user"));
         };
         if (Str("useable_paladin_ranger") == "1")
         {
           usableByBaseclass.Add(CString("ranger"));
         };
          if (Str("useable_paladin_ranger") == "1")
         {
           usableByBaseclass.Add(CString("paladin"));
         };
         if (Str("useable_thief")          == "1")
         {
           usableByBaseclass.Add(CString("thief"));
         };


  LaunchSound      = "none";
  HitSound         = "none";
  MissSound        = "none";
  MissileArt       = "none";
  HitArt           = "none";
  Attack_Bonus     =  0;
  Experience       =  0;
  IsNonLethal      =  1;
  item_asl         = "";
  AmmoType         = "none";
  {
    Bundle_Qty        = Int("bundle_quantity");
    if (Bundle_Qty == 0)
    {
      Bundle_Qty = 1;
      canHalveJoin = false;
    }
    else
    {
      canHalveJoin = true;
    };
    m_usageFlags     =  0;
      // Part of usage flags Scribtable       =  NOTIMPLEMENTED;
      // Part of usage flags CanDrop          = "NOTIMPLEMENTED";
      // Part of usage flags CanHalveJoin     = "NOTIMPLEMENTED";
  };
  ExamineEvent     = 0;
  ExamineLabel     = "NOTIMPLEMENTED";
  Recharge_Rate    = "never";

         //Str("ready_slot" : "ammo_quiver",
//DEFID ( "readied location"        ,DWORD       ,ReadiedLocation);
         //Str("special_ability" : "none",

         //Str("spell_number_1" : "none",
         //Str("spell_number_2" : "none",
         //Str("spell_number_3" : "none",
//DEFID ( "spell effect"            ,string      ,SpellEffect);

//DEFID ( "weapon type"             ,enum        ,WeaponType);


  jr.EndList(DBGID_singleItem);

}
*/
/*


*/

#undef DEFID
void Line(const char *key, const char *value); 
#define DEFID(tag, type, value) Line(tag, value); 

void ITEM_DATA::Export(void)
{
  fprintf(f,"// jsonLineNumber = %d\n" ,inputLineNum);
DEFID ( "name"                    ,string      ,m_uniqueName);
//DEFID ( "Key"                     ,long        ,Key);
DEFID ( "id name"                 ,string      ,m_idName);
DEFID ( "ROF per round"           ,float       ,ROF_Per_Round);
DEFID ( "combat range"            ,long        ,Range);
DEFID ( "cost"                    ,long        ,Cost);
DEFID ( "hands to carry"          ,long        ,Hands_to_Use);
{
  int i;
  for (i=0; i<usableByBaseclass.GetSize(); i++)
  {
    DEFID ( "baseclass"               ,flags       ,usableByBaseclass[i]);
  };
};
{
  //DEFID ( "small/medium damage dice",dice        ,SmallDamageDice);
  CString dd;
  dd.Format("%dd%d%+d", Nbr_Dice_Sm, Dmg_Dice_Sm, Dmg_Bonus_Sm);
  DEFID("small/medium damage dice", string, dd);
};
{
  //DEFID ( "large damage dice"       ,dice        ,LargeDamageDice);
  CString dd;
  dd.Format("%dd%d%+d", Nbr_Dice_Lg, Dmg_Dice_Lg, Dmg_Bonus_Lg);
  DEFID("large damage dice", string, dd);
};
DEFID ( "AC base"                 ,long        ,Protection_Base);
DEFID ( "AC bonus"                ,long        ,Protection_Bonus);
DEFID ( "encumbrance"             ,long        ,Encumbrance);
DEFID ( "cursed"                  ,BOOL        ,(Cursed==0)?"no":"yes");
DEFID ( "readied location"        ,DWORD       ,CStringFromBase38(Location_Readied));
DEFID ( "bundle quantity"         ,long        ,Bundle_Qty);
DEFID ( "CanHalveJoin"            ,BOOL        ,canHalveJoin?"yes":"no");
DEFID ( "Special Ability"         ,string      ,specAbs);
{
  int i;
  CString text = "unknown";
  for (i=0; i<numWeaponType; i++)
  {
    if (typeIDs[i].type == Wpn_Type)
    {
      text = typeIDs[i].text;
      break;
    };
  };
  DEFID ("weapon type", string, text);
};
DEFID ( "number of charges"       ,long        ,Num_Charges);
DEFID ( "Examine Event ID"        ,long        ,ExamineEvent);
DEFID ( "Examine Label"           ,string      ,ExamineLabel);
DEFID ( "recharge rate"           ,enum        ,Recharge_Rate);
if (!spellID.IsEmpty())
{
  DEFID ( "spell effect"            ,string      ,spellID);
};
DEFID ( "launch sound"            ,file        ,LaunchSound);
DEFID ( "hit sound"               ,file        ,HitSound);
DEFID ( "miss sound"              ,file        ,MissSound);
DEFID ( "missile sprite"          ,spritedata  ,MissileArt);
DEFID ( "hit sprite"              ,spritedata  ,HitArt);
DEFID ( "attack bonus"            ,long        ,Attack_Bonus);
DEFID ( "experience"              ,long        ,Experience);
DEFID ( "non lethal"              ,BOOL        ,IsNonLethal?"yes":"no");
DEFID ( "location carried"        ,DWORD       ,"none"); // For backward compatibility
DEFID ( "ASL"                     ,string      ,"fake,zero,0x0"); //item_asl);
DEFID ( "ammo type"               ,string      ,AmmoType);
DEFID ( "usable"                  ,BOOL        ,"no"); //m_usageFlags);
//DEFID ( "scribtable"              ,BOOL        ,Scribable);
//DEFID ( "can drop"                ,BOOL        ,Droppable);
//DEFID ( "CanHalveJoin"            ,BOOL        ,CanHalveJoin);
  
  
  
  
};


CArray <ITEM_DATA, ITEM_DATA&> itemData;
LINE_DATABASE defaultItemData;
LINE_DATABASE jsonItemData;
LINE_DATA *pItem;


void ReadJsonItemData(JReader& jr)
{
  LINE_DATA itemData;
  int inputLineNum;
  CString fruaID;
  jr.StartList(DBGID_singleItem);
  inputLineNum = jr.LineNum();
  itemData.Add("// jsonLineNumber",inputLineNum);

  {
    CString commonName, idName;
    int plus;
    commonName = jr.Fetch_String("name");
    idName = jr.Fetch_String("identified_name");
    if ((plus = idName.Find('+')) > 0)
    {
      int bonus, len;
      bonus = 0;
      len = idName.GetLength();
      for (plus++; plus<len; plus++)
      {
        if ((idName[plus] < '0') || (idName[plus] > '9')) break;
        bonus = 10*bonus + idName[plus] - '0';
      };
      if (plus == len)
      {
        commonName.Format("%s|%d", commonName, bonus);
      };
    };
    itemData.Add("name", commonName);
    itemData.Add("id name", idName);
  };
  fruaID = jr.Fetch_String("id");
  itemData.Add("//FRUA id", fruaID);
  //m_uniqueName = m_idName;

  int     cost            = Int("price_platinum");
  itemData.Add("cost", cost);
  int     handsToUse    = Int("hands_needed");
  itemData.Add("hands to carry", handsToUse);
  bool    cursed          = Bol("cursed");
  itemData.Add("cursed", cursed?"yes":"no");
  int     numCharges      = Int("num_charges");
  itemData.Add("number of charges", numCharges);
  int     protectionBase  = Int("protection_base");
  itemData.Add("AC base", protectionBase);
  int     protectionBonus = Int("protection_bonus");
  itemData.Add("AC bonus", protectionBonus);
  int     range           = Int("range");
  itemData.Add("combat range", range);
  int     ROF             = Flt("rate");
  itemData.Add("ROF per round", ROF);
  CString specAbs         = Str("special_ability");
  itemData.Add("Special Ability", specAbs);
  {
    CString ldd;
    int     dmgDiceLg       = Int("vs_large_numSides");
    int     nbrDiceLg       = Int("vs_large_numDice");
    int     dmgBonusLg      = Int("vs_large_bonus");
    ldd.Format("%dd%d%+d", nbrDiceLg, dmgDiceLg, dmgBonusLg);
    itemData.Add("large damage dice", ldd);
  };
  {
    CString sdd;
    int     dmgDiceSm       = Int("vs_small_numSides");
    int     nbrDiceSm       = Int("vs_small_numDice");
    int     dmgBonusSm      = Int("vs_small_bonus");
    int     encumbrance     = Int("encumbrance_coins");
    sdd.Format("%dd%d%+d", nbrDiceSm, dmgDiceSm, dmgBonusSm);
    itemData.Add("small/medium damage dice", sdd);
  };
  {
    CString loc;
    CString locationReadied;
    loc = Str("ready_slot"); // For backward compatibility
    if      (loc == "weapon_hand") locationReadied = "WEAPON";
    else if (loc = "none")         locationReadied = "CANNOT";
    else
    {
      CString msg;
      msg.Format("Cannot determine readied location of %s", loc);
      jr.PrintError(msg);
    };
    itemData.Add("readied location", locationReadied);
  };
  {
    CString wtype, wpnType;
    wtype = Str("weapon_type");
    if      (wtype == "hand_thrown") wpnType = "hand held or thrown";
    else if (wtype == "none")        wpnType = "not weapon";
    else if (wtype == "hand_cutting")wpnType = "hand held cutting";
    else if (wtype == "crossbow")    wpnType = "crossbow";
    else if (wtype == "hand_blunt")  wpnType = "hand held blunt";
    else if (wtype == "thrown")      wpnType = "thrown only";
    else if (wtype == "sling")       wpnType = "sling";
    else if (wtype == "ammo")        wpnType = "ammo";
    else if (wtype == "bow")         wpnType = "bow";
    else if (wtype == "fine_bow")    wpnType = "bow";
    else
    {
      CString msg;
      msg.Format("Cannot determine weapon type of \"%s\"", wtype);
      jr.PrintError(msg);
    };
    itemData.Add("weapon type", wpnType);
  };
         if (Str("useable_cleric") == "1")
         {
           //usableByBaseclass.Add(CString("cleric"));
           itemData.Add("baseclass", "cleric");
         };
         if (Str("useable_fighter")        == "1")
         {
           //usableByBaseclass.Add(CString("fighter"));
           itemData.Add("baseclass", "fighter");
         };
         if (Str("useable_knight")         == "1")
         {
           //usableByBaseclass.Add(CString("knight"));
           itemData.Add("baseclass", "knight");
         };
         if (Str("useable_magic_user")     == "1")
         {
           //usableByBaseclass.Add(CString("magic user"));
           itemData.Add("baseclass", "magic user");
         };
         if (Str("useable_paladin_ranger") == "1")
         {
           //usableByBaseclass.Add(CString("ranger"));
           itemData.Add("baseclass", "ranger");
         };
          if (Str("useable_paladin_ranger") == "1")
         {
           //usableByBaseclass.Add(CString("paladin"));
           itemData.Add("baseclass", "paladin");
         };
         if (Str("useable_thief")          == "1")
         {
           //usableByBaseclass.Add(CString("thief"));
           itemData.Add("baseclass", "thief");
         };


  //LaunchSound      = "none";
  //HitSound         = "none";
  //MissSound        = "none";
  //MissileArt       = "none";
  //HitArt           = "none";
  //Attack_Bonus     =  0;
  //Experience       =  0;
  //IsNonLethal      =  1;
  //item_asl         = "";
  //AmmoType         = "none";
  {
    char *canHalveJoin;
    int bundleQty        = Int("bundle_quantity");
    if (bundleQty == 0)
    {
      bundleQty = 1;
      canHalveJoin = "no";
    }
    else
    {
      canHalveJoin = "yes";
    };
    itemData.Add("CanHalveJoin", canHalveJoin);
    //m_usageFlags     =  0;
      // Part of usage flags Scribtable       =  NOTIMPLEMENTED;
      // Part of usage flags CanDrop          = "NOTIMPLEMENTED";
      // Part of usage flags CanHalveJoin     = "NOTIMPLEMENTED";
  };
  //ExamineEvent     = 0;
  itemData.Add("Examine Event ID", "0");
  itemData.Add("Examine Label", "NOTIMPLEMENTED");
  itemData.Add("recharge rate","never");

         //Str("ready_slot" : "ammo_quiver",
//DEFID ( "readied location"        ,DWORD       ,ReadiedLocation);
         //Str("special_ability" : "none",

         //Str("spell_number_1" : "none",
         //Str("spell_number_2" : "none",
         //Str("spell_number_3" : "none",
//DEFID ( "spell effect"            ,string      ,SpellEffect);

//DEFID ( "weapon type"             ,enum        ,WeaponType);


  jr.EndList(DBGID_singleItem);
  jsonItemData.Add(itemData);
}

void ReadDCDefaultItems(FILE *out)
{
  FILE *f;
  char *line;
  int len;
  bool copyComments = true;
  f = fopen(executablePath + "DC_default_items.txt", "r");
  if (f == NULL)
  {
    char line[200];
    printf("Cannot open \"DC_default_items.txt\"\n");
    printf("Press Enter ---->");
    gets(line);
    printf("\n");
    return;
  };
  line = (char *)malloc(100000);
  while (fgets(line, 99999, f) != NULL)
  {
    len = strlen(line);
    while ( (len > 0) && (line[len-1] == '\n'))
    {
      len--;
      line[len] = 0;
    };
    if (len < 2) continue;
    if ((line[0] == '/') && (line[1] == '/'))
    {
      if (copyComments)
      {
        fprintf(out, "%s\n", line);
      };
      continue;
    }
    else
    {
      copyComments = false;
    };
    if (len < 8) continue;
    if (memcmp(line, "\\(BEGIN)", 8) == 0)
    {
      LINE_DATA newItem;
      newItem.Add("//JSON Line", "????");
      while (fgets(line, 99999, f) != NULL)
      {
        int col, len;
        char *equal;
        len = strlen(line);
        while ((len > 0) && ((line[len-1] == '\n')||(line[len-1] == ' ')))
        {
          len--;
          line[len] = 0;
        };
        if ( (len == 6) && (memcmp(line, "\\(END)", 6) == 0)) 
        {
          break;
        };
        equal = strchr(line, '=');
        if (equal == NULL) continue;
        col = equal - line ;
        line[col] = 0;
        equal++;
        while ((col > 0) && (line[col-1] == ' ')) line[--col] = 0;
        if (col < 2) continue;
        while (*equal == ' ') equal++;
        while (*line == ' ') line++;
        newItem.Add(line, equal);
      };
      pItem = defaultItemData.Add(newItem);
    };
  };
  free(line);
};



void ConvertOneItem(JReader& item)
{
  //ITEM_DATA tempItem;
  //tempItem.Read(item);
  //itemData.Add(tempItem);
  ReadJsonItemData(item);
}

void RenameJsonItems(void)
{
  // if a json item name does not exist in the default database
  //     AND
  // its identified name exists in the default database
  //     THEN
  // rename the json item
  int i, n;
  n = jsonItemData.GetSize();
  for (i=0; i<n; i++)
  {
    LINE_DATA *pJsonData;
    LINE_DATA *pDefaultData;
    LINE_ENTRY *pJsonNameLine;
    LINE_ENTRY *pJsonIdNameLine;
    LINE_ENTRY *pDefaultNameLine;
    pJsonData = jsonItemData[i];
    pJsonIdNameLine = (*pJsonData)["id name"];
    if (pJsonIdNameLine == NULL) continue;  // Impossible to rename
    pDefaultData = defaultItemData.Find("id name", pJsonIdNameLine->value);
    if (pDefaultData == NULL) continue;
    pDefaultNameLine = (*pDefaultData)["name"];
    if (pDefaultNameLine == NULL) continue; // Nothing to name it to!
    pJsonNameLine = (*pJsonData)["name"];
    if (pJsonNameLine == NULL) continue;
    if (pJsonIdNameLine->value == "Bracers AC 2")
    {
      ASSERT(true);
    };
    pJsonNameLine->value = pDefaultNameLine->value;
    ASSERT(true);
  };
}

void ConvertItems(JReader& items)
{
  if (f != NULL)
  {
    die("Internal error in items.cpp.  f not null");
  };
  f = fopen(jsonPath + "items.txt", "w");
  fprintf(f,"// Item database file\n");
  fprintf(f,"// The line above **must** be the first line of the items file!!!\n\n");
  fprintf(f,"// FRUA Export Version %s:%s\n", FRUAExportDate, FRUAExportTime);
  fprintf(f,"// ConverFRUA version %s\n", ConvertFRUAVersion);

  while (items.Optional(), items.NextEntry())
  {
    JReader item(items);
    ConvertOneItem(item);
  };
  ReadDCDefaultItems(f);
  RenameJsonItems();  // For example "Javelin +2"  --->  "Javelin|2"
  {
    // Combine the default items with
    // the FRUA items.
    int i, n;
    n = defaultItemData.GetSize();
    for (i=0; i<n; i++)
    {
      LINE_DATA *pDefaultData;
      LINE_ENTRY *pDefaultNameLine;
      pDefaultData = defaultItemData[i];
      pDefaultNameLine = (*pDefaultData)["name"];
      if(pDefaultNameLine->value == "Bracers|2")
      {
        ASSERT(true);
        };
//      for (j=0; j<m; j++)
      {
        LINE_DATA *pJsonData;
        pJsonData = jsonItemData.Find("name", pDefaultNameLine->value);
        if (pJsonData != NULL)
        {
          //Overlay the default with the json values.
          int i, n;
          n = pJsonData->GetSize();
          for (i=0; i<n; i++)
          {
            LINE_ENTRY *pJsonLine;
            LINE_ENTRY *pDefaultLine;
            pJsonLine = (*pJsonData)[i];
            if (pJsonLine->value.IsEmpty()) continue; // Ignore (especiall the "name" field!
            pDefaultLine = (*pDefaultData)[pJsonLine->key];
            if (pDefaultLine != NULL)
            {
              pDefaultLine->value = pJsonLine->value;
            }
            else
            {
              pDefaultData->Add(pJsonLine->key, pJsonLine->value);
      };
            (*pJsonData)["name"]->value.Empty();
          };
        };
      };
    };
  };
  
  {
    int i, n;
    n = defaultItemData.GetSize();
    for (i=0; i<n; i++)
    {
      LINE_DATA *pDefaultData;
      int j, m;
      fprintf(f,"\n\\(BEGIN)\n");
      pDefaultData = defaultItemData[i];
      m = pDefaultData->GetSize();
      for (j=0; j<m; j++)
      {
        LINE_ENTRY *pDefaultLine;
        pDefaultLine = (*pDefaultData)[j];
        fprintf(f,"%s = %s\n", pDefaultLine->key, pDefaultLine->value);
      };
      fprintf(f,"\\(END)\n");
    };
  };
  {
    int i, n;
    n = jsonItemData.GetSize();
    for (i=0; i<n; i++)
    {
      LINE_DATA *pJsonData;
      LINE_ENTRY *pJsonNameLine;
      pJsonData = jsonItemData[i];
      pJsonNameLine = (*pJsonData)["name"];
      if (pJsonNameLine == NULL) continue; // Skip unnamed items.
      if (pJsonNameLine->value.IsEmpty()) continue; // Skip 'removed' entries.
      fprintf(f,"\n\\(BEGIN)\n");
      {
        int j, m;
        m = pJsonData->GetSize();
        for (j=0; j<m; j++)
        {
          LINE_ENTRY *pJsonLine;
          pJsonLine = (*pJsonData)[j];
          fprintf(f,"%s = %s\n", pJsonLine->key, pJsonLine->value);
        };
      };
      fprintf(f,"\\(END)\n");
    };
  };
  fclose(f);
  f = NULL;
}

bool FindItem(const CString& name)
{
  int i;
  for (i=0; i<itemData.GetSize(); i++)
  {
    if (itemData[i].m_uniqueName == name) return true;
  };
  return false;
}
