#include "StdAfx.h"
#include "Translate.h"
//
extern FILE *f;
extern CString jsonPath;
extern CString executablePath;
extern CString FRUAExportDate, FRUAExportTime;
extern CString ConvertFRUAVersion;
void Line(const char *name, const char *value);
void Line(const char *name, int value);

#define Str(a)   spell.Fetch_String(a)
#define Int(a)   spell.Fetch_Int(a)
#define YesNo(a) spell.Fetch_YesNo(a)


struct DEFAULT_SPELL_DATA
{
  CArray<CString, CString&> key;
  CArray<CString, CString&> value;
  void Add(const char *newKey, int newValue)
  {
    CString temp;
    temp.Format("%d", newValue);
    Add(newKey, temp);
  };
  void Add(const char *newKey, const CString& newValue)
  {
    int n = key.GetSize();
    key.SetSize(n+1); value.SetSize(n+1);
    key[n] = newKey; value[n] = newValue;
  };
  void operator =(const DEFAULT_SPELL_DATA &src)
  {
    key.RemoveAll(); value.RemoveAll();
    key.Append(src.key); value.Append(src.value);
  };
};



CArray<DEFAULT_SPELL_DATA, DEFAULT_SPELL_DATA&>defaultSpellData;

/*
CONFIGID spellIDs[] =
{
CONFIGID ("",CONFIG_DATA_object,spellObject),
DEFID ("name"               ,string     ,Name),
DEFID ("Key"                ,long       ,Key),
DEFID ("casting sound"      ,file       ,CastingSound),
DEFID ("cast msg"           ,string     ,CastMsg),
DEFID ("missile sound"      ,file       ,MissileSound),
DEFID ("coverage sound"     ,file       ,CoverageSound),
DEFID ("hit sound"          ,file       ,HitSound),
DEFID ("linger sound"       ,file       ,LingerSound),
DEFID ("cast art"           ,spritedata ,CastArt),
DEFID ("missile art"        ,spritedata ,MissileArt),
DEFID ("target cover art"   ,spritedata ,TargCoverArt),
DEFID ("target hit art"     ,spritedata ,TargHitArt),
DEFID ("target linger art"  ,spritedata ,TargLingerArt),
DEFID ("school"             ,string     ,School),
DEFID ("cast by"            ,flags      ,CastByClass),
DEFID ("can target friend"  ,enum       ,TargetFriend),
DEFID ("can target enemy"   ,enum       ,TargetEnemy),
DEFID ("cumulative"         ,enum       ,Cumulative),
//DEFID ("usable by party"    ,enum       ,UsableByParty),
DEFID ("can memorize"       ,enum       ,CanMemorize),
//DEFID ("can scribe"         ,enum       ,CanScribe),
DEFID ("allow scribe"       ,enum       ,AllowScribe),
DEFID ("auto scribe"        ,enum       ,AutoScribe),
//DEFID ("combat only"        ,enum       ,CombatOnly),      // If not in camp   // Sets NOT_IN_CAMP
//DEFID ("camp only"          ,enum       ,CampOnly),        // If not in combat // Sets NOT_IN_COMBAT
DEFID ("in camp"            ,enum       ,InCamp),    
DEFID ("in combat"          ,enum       ,InCombat),  
DEFID ("can be dispelled"   ,enum       ,CanBeDispelled),
DEFID ("lingers"            ,enum       ,Lingers),
DEFID ("linger affects once",enum       ,LingerAffectsOnce),
DEFID ("save versus"        ,enum       ,SaveVersus),
DEFID ("save result"        ,enum       ,SaveResult),
DEFID ("Special Ability"    ,string     ,SpecialAbility),

DEFID ("targeting type"     ,enum       ,TargetingType),
DEFID ("target width"       ,dicePlus   ,TargetWidth),
DEFID ("target height"      ,dicePlus   ,TargetHeight),
DEFID ("target length"      ,dicePlus   ,TargetLength),
DEFID ("target quantity"    ,dicePlus   ,TargetQuantity),
DEFID ("target range"       ,dicePlus   ,TargetRange),
DEFID ("target radius"      ,dicePlus   ,TargetRadius),
DEFID ("target number"      ,dicePlus   ,TargetNumber),

DEFID ("level"              ,long       ,Level),
DEFID ("casting time"       ,long       ,CastingTime),
DEFID ("casting time units" ,enum       ,CastingTimeUnits),
DEFID ("duration"           ,dicePlus   ,Duration),
DEFID ("duration units"     ,enum       ,DurationUnits),
DEFID ("target duration"    ,dicePlus   ,TargetDuration),
DEFIDz ("attack bonus"      ,exDice     ,AttackBonus, true), // read-only
DEFIDz ("damage"            ,exDice     ,Damage, true), // read-only
DEFIDz ("protection bonus"  ,exDice     ,ProtectionBonus, true), // read-only
DEFIDz ("healing"           ,exDice     ,Healing, true), // read-only
DEFID ("price"              ,long       ,Price),
DEFID ("priority"           ,long       ,Priority),
DEFIDg ("spell effect"      ,spellEffect,SpellEffect),
DEFIDg ("activate script"   ,string     ,SpellActivateScript),
DEFIDg ("modify script"     ,string     ,SpellModifyScript),
DEFIDg ("saving throw script",string    ,SpellSavingThrowScript),
DEFIDg ("saving throw failed script",string    ,SpellSavingThrowFailedScript),
DEFIDg ("saving throw succeeded script",string    ,SpellSavingThrowSucceededScript),
DEFID ("spell attribute"    ,string     ,SpellAttribute),
DEFID ("begin script"       ,string     ,SpellBeginScript),
DEFID ("end script"         ,string     ,SpellEndScript),
CONFIGID( "",CONFIG_DATA_end,NULL)
};
*/


void ConvertOneSpell(JReader& spell)
{
  spell.StartList(DBGID_oneSpell);
  Line("Key"              ,Int("id"));
  {
    CString name, spellBook, level;
    name = Str("name");
    spellBook = Str("spell_book");
    level = Str("spell_level");
    if (name.IsEmpty())
    {
      name.Format("ID");
    };
    Line("name", name + "|" + spellBook + "|" + level + "|" + Str("id"));
  };

  {
    CString temp, v;
/*
   case 0: currSpell["save_result"] = "no_save_possible"; break;
   case 1: currSpell["save_result"] = "save_negates"; break;
   case 2: currSpell["save_result"] = "save_for_half_dmg"; break;
   case 3: currSpell["save_result"] = "special_save"; break;
   default: currSpell["save_result"] = Unknownevictring; break;
    
    
*/
    temp = Str("save_result");
    v = "none";
    if (temp == "no_save_possible")  v = "none";
    if (temp == "save_negates")      v = "negates";
    if (temp == "save_for_half_dmg") v = "half damage";
    if (temp == "special_save")      v = "use THAC0";
    Line("save result",   v);
  };


  Line("level", Int("spell_level"));


  {
    Line("target width",  Int("target_area_square_size"));
    Line("target height", Int("target_area_square_size"));
  };
  {
    Line("target number"       , Int("target_min_count"));
    Line("target quantity"     , Int("target_min_count"));
  };
  //Line("unsure_byte"            , Int("unsure_byte"));
  WriteLine(f, "casting sound"       , "");
  WriteLine(f, "Key"                 , "0");
  WriteLine(f, "cast msg"            , "");
  WriteLine(f, "missile sound"       , "");
  WriteLine(f, "coverage sound"      , "");
  WriteLine(f, "hit sound"           , "");
  WriteLine(f, "linger sound"        , "");
  WriteLine(f, "cast art"            , "");
  WriteLine(f, "missile art"         , "");
  WriteLine(f, "target cover art"    , "");
  WriteLine(f, "target hit art"      , "");
  WriteLine(f, "target linger art"   , "");
  {
    CString school, castBy;
    school = Str("spell_book");
    Line("school"                        , school);
    if (school == "cleric")     castBy = "cleric";
    if (school == "magic-user") castBy = "magicUser";
    if (school == "druid")      castBy = "cleric";
    if (school == "device")     castBy = "magicUser";
    WriteLine(f, "cast by"             , castBy);
  };
  Line("can target friend"            , YesNo("target_friendly"));
  WriteLine(f, "cumulative"          , "");
  WriteLine(f, "can memorize"        , "");
  WriteLine(f, "allow scribe"        , "");
  WriteLine(f, "auto scribe"         , "");
  WriteLine(f, "can be dispelled"    , "");
  WriteLine(f, "lingers"             , "");
  WriteLine(f, "linger affects once" , "");

  {
    CString temp, v;
    /*
    CONFIGID( "spell"           ,Sp),
    CONFIGID( "Par/Pois/DM"     ,ParPoiDM),
    CONFIGID( "Pet/Poly"        ,PetPoly),
    CONFIGID( "Rod/Staff/Wand"  ,RodStaffWand),
    CONFIGID( "Breath Weapon"   ,BreathWeapon),
    CONFIGID( ""      ,0)

      currSpell["save_vs"] = "none";
   }
   else
   {
      switch (spell.spellData.saveVs)
      {
      case 0: currSpell["save_vs"] = "save_vs_poison"; break;
      case 4: currSpell["save_vs"] = "save_vs_spell"; break;
      default: currSpell["save_vs"] = UnknownString; break;
      }
   }
    
    
    */
    temp = Str("save_vs");
    if (temp == "save_vs_poison") v = "Par/Pois/DM";
    if (temp == "save_vs_spell")  v = "spell";
    if (!v.IsEmpty()) Line("save versus", v);
  };
  Line("Special Ability"               , Str("special_ability"));
  {
    CString temp, v;
/*
      currSpell["target_type"] = "self";
      currSpell["target_type"] = "select_by_target_count";
      currSpell["target_type"] = "select_by_target_count";
      currSpell["target_type"] = "select_by_target_count";
      currSpell["target_type"] = "select_by_target_count";
      currSpell["target_type"] = "select_by_target_count";
      currSpell["target_type"] = "select_by_target_count";
      currSpell["target_type"] = "select_by_target_count";
         currSpell["target_type"] = "select_area_line_end";
         currSpell["target_type"] = "select_area_cone";
         currSpell["target_type"] = "select_area_square";                  
         currSpell["target_type"] = "select_area_circle";
      currSpell["target_type"] = "select_area_square";
      currSpell["target_type"] = "select_area_square";
      currSpell["target_type"] = "select_area_circle";
      currSpell["target_type"] = "none";

    CONFIGID( "self"              ,Self),
    CONFIGID( "selectByCount"     ,SelectedByCount),
    CONFIGID( "party"             ,WholeParty),
    CONFIGID( "touch"             ,TouchedTargets),
    CONFIGID( "areacircle"        ,AreaCircle),
    CONFIGID( "arealinepickstart" ,AreaLinePickStart),
    CONFIGID( "arealinepickend"   ,AreaLinePickEnd),
    CONFIGID( "areasquare"        ,AreaSquare),
    CONFIGID( "areacone"          ,AreaCone),
    CONFIGID( "SelectByHitDice"   ,SelectByHitDice),
    CONFIGID( ""                  ,0)

    
    */
    temp = Str("target_type");
    if (temp == "self" )                   v = "self";
    if (temp == "select_by_target_count" ) v = "selectByCount";
    if (temp == "select_area_line_end" )   v = "arealinepickend";
    if (temp == "select_area_cone" )       v = "areacone";
    if (temp == "select_area_square" )     v = "areasquare";
    if (temp == "select_area_circle" )     v = "areacircle";
    Line("targeting type", v);
  }
  Line("casting time"                  , Int("cast_time_in_segments"));
  WriteLine(f, "casting time units"     , "");
  {
    CString duration;
    int baseDuration, levelDuration;
    baseDuration = Int("duration_base_rounds");
    levelDuration = Int("duration_rounds_added_per_level");
    duration.Format("%d+%d*level", baseDuration, levelDuration);
    WriteLine(f, "target duration"       , duration);
  };
  WriteLine(f, "duration units"        ,"");
  
  //Line("Number of targets"               , Int("target_max_count"));
  Line("priority"                        , Int("cast_priority"));


  {
    CString temp, v;
/*
   case 0: currSpell["non_combat_targeting"] = "none";  break;
   case 1: currSpell["non_combat_targeting"] = "self";  break;
   case 2: currSpell["non_combat_targeting"] = "selected_party_members";  break;
   case 3: currSpell["non_combat_targeting"] = "entire_party";  break;
*/
    temp = Str("non_combat_targeting");
    if (temp == "none") v = "no"; else v = "yes";
    Line("in camp"                      , v);
    Line("in combat"                    , "yes");
  };
  {
    CString range;
    int baseRange, levelRange;
    baseRange = Int("range_base_squares");
    levelRange = Int("range_squares_added_per_level");
    range.Format("%d+%d*level", baseRange, levelRange);
    WriteLine(f, "target range", range);
  };
  WriteLine(f, "attack bonus"                 ,"0");
  WriteLine(f, "damage"                       ,"0");
  WriteLine(f, "protection bonus"             ,"0");
  WriteLine(f, "healing"                      ,"0");
  WriteLine(f, "price"                        ,"0");
  WriteLine(f, "spell effect"                 ,"");
  WriteLine(f, "activate script"              ,"");
  WriteLine(f, "modify script"                ,"");
  WriteLine(f, "saving throw script"          ,"");
  WriteLine(f, "saving throw failed script"   ,"");
  WriteLine(f, "saving throw succeeded script","");
  //WriteLine(f, "spell attribute"              ,"");
  WriteLine(f, "begin script"                 ,"");
  WriteLine(f, "end script"                   ,"");

  spell.EndList(DBGID_oneSpell);
};


void ReadDCDefaultSpells(FILE *out)
{
  FILE *f;
  char *line;
  DEFAULT_SPELL_DATA *pSpell;
  int len;
  bool copyComments = true;
  f = fopen(executablePath + "DC_default_spells.txt", "r");
  if (f == NULL)
  {
    char line[200];
    printf("Cannot open \"DC_default_spells.txt\"\n");
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
      int i;
      i = defaultSpellData.GetSize();
      defaultSpellData.SetSize(i + 1);
      defaultSpellData[i].key.SetSize(1);
      defaultSpellData[i].value.SetSize(1);
      defaultSpellData[i].key[0] = "//JSON Line";
      defaultSpellData[i].value[0] = "????";
      while (fgets(line, 99999, f) != NULL)
      {
        int col, len, n;
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
        n = defaultSpellData[i].key.GetSize();
        defaultSpellData[i].value.SetSize(n+1);
        defaultSpellData[i].key.SetSize(n+1);
        while (*equal == ' ') equal++;
        while (*line == ' ') line++;
        defaultSpellData[i].key[n] = CString(line);
        defaultSpellData[i].value[n] = CString(equal);
      };
      pSpell = &defaultSpellData[i];
      //RenameLine("classes", "class");  
    };
  };
  free(line);
};



void ConvertSpells(JReader& spells)
{
  if (f != NULL)
  {
    die("Internal error in Spells.cpp.  f not null");
  };
  f = fopen(jsonPath + "spells.txt", "w");
  fprintf(f,"// Spell database file\n");
  fprintf(f,"// The line above **must** be the first line of the monsters file!!!\n\n");
  fprintf(f,"// FRUA Export Version %s:%s\n", FRUAExportDate, FRUAExportTime);
  fprintf(f,"// ConverFRUA version %s\n", ConvertFRUAVersion);

  while (spells.Optional(), spells.NextEntry())
  {
    JReader spell(spells);
    fprintf(f,"\n\\(BEGIN)\n");
    ConvertOneSpell(spell);
    fprintf(f,"\\(END)\n");
  };
  ReadDCDefaultSpells(f);
  {
    int i, n;
    n = defaultSpellData.GetSize();
    for (i=0; i<n; i++)
    {
      fprintf(f,"\n\\(BEGIN)\n");
      {
        int j, m;
        m = defaultSpellData[i].key.GetSize();
        for (j=0; j<m; j++)
        {
          fprintf(f,"%s = %s\n", defaultSpellData[i].key[j], defaultSpellData[i].value[j]);
        };
      };
      fprintf(f,"\\(END)\n");
    };
  };
  fclose(f);
  f = NULL;
}
