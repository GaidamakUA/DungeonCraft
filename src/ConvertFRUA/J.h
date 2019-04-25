#define JWTAB 3
#define maxTab 199

enum DEBUG_ID
{
  DBGID_singleEvent = 1,
  DBGID_singleArtEntry,
  DBGID_FRUAExport,
  DBGID_BigSmallSpritePics,
  DBGID_singleSoundEntry,
  DBGID_globalData,
  DBGID_healEffects,
  DBGID_healAdjustements,
  DBGID_monsterEventEntry,
  DBGID_randomChain,
  DBGID_restEvent,
  DBGID_smallTown,
  DBGID_tale,
  DBGID_drink,
  DBGID_trainableBaseclass,
  DBGID_singleItem,
  DBGID_singleLevelConfig,
  DBGID_singleLevelString,
  DBGID_entryPoint,
  DBGID_zone,
  DBGID_levelConfigArt,
  DBGID_artSlots1,
  DBGID_artSlots2,
  DBGID_artSlots3,
  DBGID_mapGrid,
  DBGID_details,
  DBGID_levelEntry,
  DBGID_transfer,
  DBGID_tourStep,
  DBGID_backgroundSound,
  DBGID_spellbook,
  DBGID_gemType,
  DBGID_item,
  DBGID_sound,
  DBGID_qbutton,
  DBGID_picData,
  DBGID_moneySack,
  DBGID_qList,
  DBGID_specialItem,
  DBGID_characterSpell,
  DBGID_encounterButton,
  DBGID_oneMonster,
  DBGID_oneSpell,
  DBGID_stepEvent,
  DBGID_TownShopItems,
};




extern const char *ChainTriggerText[];
extern const char *EventTriggerText[];
extern const char *DistanceText[];
extern const char *DirectionText[];
extern const char *TurnModText[];
extern const char *ButtonPostChainOptionText[];
extern const char *PasswordActionText[];
extern const char *AffectsWhoText[];
extern const char *SaveEffectText[];
extern const char *SaveVsText[];
extern const char *TakeItemsAffectsText[];
extern const char *TakeItemsQtyText[];
extern const char *TaleOrderTypeText[];
extern const char *costFactorText[];
extern const char *MathOperationText[];
extern const char *MultiItemCheckText[];
extern const char *genderText[];
extern const char *alignmentText[];
extern const char *FacingText[];
extern const char *StepText[];
extern const char *ButtonOptionText[];
extern const char *GlobalAVStyleText[];
extern const char *BlockageText[];
extern const char *itemClassText[];
extern const char *EventListText[];


enum eventType        // implementation status
{                     //
   // Status:
   //        Dlg: Editor dialog has been created to allow user to configure this event
   //       Game: Engine has the code to process this event
   //   ChainIds: Indicates an event which stores references to other events
   //             using event keys, not including the chainEventHappen/chainEventNotHappen
   //             events which all events have. These events need special handling
   //             for situations such as event deletion, since the referenced events
   //             must also be deleted.
   //      
   //             Events with ChainIds must over-ride the following virtual functions
   //             and provide proper handling for each chain event.
   //
   //               IsChained
   //               ChainToEventId
   //               UnchainToEventId
   //               GetEVChainText
   //               GetEVChain
   //               GetChainCount
   //
   //             Special handling must also be added to these functions which
   //             are shared by all events.
   //
   //               DeleteEvent
   //               CopyEvent
   //
   //             And of course each chain event must be processed in 
   //             functions such as Serialize and operator=.
   //
   //             The list of events with ChainIds currently includes:
   //
   //               ChainEventType,
   //               EncounterEvent,
   //               EnterPassword,
   //               QuestStage,
   //               QuestionButton,
   //               QuestionList,
   //               QuestionYesNo,
   //               SmallTown,
   //               TavernEvent,
   //               TempleEvent,
   //               WhoPays,
   //               WhoTries,
   //               LogicBlock,
   //               RandomEvent
   //
                      // Dlg? Game? ChainIds
   NoEvent,           //  -     -     -
   AddNpc,            //  x     x     -
   Camp,              //  x     x     -
   ChainEventType,    //  x     x     x
   Combat,            //  x     x     -
   CombatTreasure,    //  x     x     - <--- CombatTreasure=GiveTreasure
   Damage,            //  x     x     -
   EncounterEvent,    //  x     x     x
   EnterPassword,     //  x     x     x
   GainExperience,    //  x     x     -
   GiveTreasure,      //  x     x     - <--- CombatTreasure=GiveTreasure
   GuidedTour,        //  x     x     -
   InnEvent,          //  -     -     - <--- obsoleted, WhoPays+CampEvent
   NPCSays,           //  x     x     -
   PassTime,          //  x     x     -
   PickOneCombat,     //  -     -     - <--- obsoleted, use option in Combat event
   QuestStage,        //  x     x     x
   QuestionButton,    //  x     x     x
   QuestionList,      //  x     x     x
   QuestionYesNo,     //  x     x     x
   RemoveNPCEvent,    //  x     x     - 
   ShopEvent,         //  x     x     -
   SmallTown,         //  x     x     x
   Sounds,            //  x     x     -
   SpecialItem,       //  x     x     -
   Stairs,            //  x     x     - <--- Stairs=Teleporter=TransferModule
   TavernEvent,       //  x     x     x
   TavernTales,       //  -     -     - <--- obsoleted, included in TavernEvent
   Teleporter,        //  x     x     - <--- Stairs=Teleporter=TransferModule
   TempleEvent,       //  x     x     x
   TextStatement,     //  x     x     - 
   TrainingHallEvent, //  x     x     -
   TransferModule,    //  x     x     - <--- Stairs=Teleporter=TransferModule
   Utilities,         //  x     x     -
   Vault,             //  x     x     -
   WhoPays,           //  x     x     x
   WhoTries,          //  x     x     x
   TakePartyItems,    //  x     x     -
   HealParty,         //  x     x     -
   LogicBlock,        //  x     x     x
   GPDLEvent,         //        x     -
   RandomEvent,       //  x     x     x
   PlayMovieEvent,    //  x     x     -
   JournalEvent,      //  x     x     -
   NUM_EVENT_TYPES,
};

enum eventDistType { UpClose, Nearby, FarAway };
const int NUM_DIST_TYPES = 3;

enum eventType;
enum chainTriggerType;

enum EventSourceType { LevelEventSrc=0, GlobalEventSrc=1, CopyEventSrc=2 };

enum chainTriggerType { AlwaysChain, IfEventHappen, IfEventNotHappen };
const DWORD NUM_CHAINTRIG_TYPES = 3;

const int MAX_BUTTONS = 5; // both horiz or vertical
const int MAX_TOUR_STEPS = 24;
const int MAX_TALES = 20;
const int MAX_DRINKS = 5;

enum eventTriggerType 
{  
   AlwaysTrigger, 
   PartyHaveItem, PartyNotHaveItem,
   Daytime, Nighttime,
   RandomChance,
   PartySearching, PartyNotSearching,
   FacingDirection,
   QuestComplete, QuestFailed, QuestInProgress,
   PartyDetectingTraps, PartyNotDetectingTraps,
   PartySeeInvisible, PartyNotSeeInvisible,
   ClassInParty, ClassNotInParty,
   RaceInParty, RaceNotInParty,
   QuestPresent, QuestNotPresent,
   GenderInParty, GenderNotInParty,
   FacingDirectionAnyTime,
   NPCInParty,NPCNotInParty,
   ExecuteGPDL,
   SpellMemorized,
   PartyAtXY,
   BaseclassInParty, BaseclassNotInParty,
   PartyHaveSpecialItem, PartyNotHaveSpecialItem,
   PartyHaveSpecialKey, PartyNotHaveSpecialKey,
   LAST_EVENT_TRIGGER


};
const int NUM_EVENTTRIG_TYPES = LAST_EVENT_TRIGGER;

enum eventFacingType { FaceNorth, FaceEast, FaceSouth, FaceWest, FaceUnchanged };
const int NUM_FACING_TYPES = 5;

enum eventStepType { TStep_NoMove, TStep_Pause, TStep_Forward, TStep_Left, TStep_Right };
const int NUM_STEP_TYPES = 5;

enum encounterButtonResultType
{
   NoResult, DecreaseRange, CombatNoSurprise,
   CombatSlowPartySurprised, CombatSlowMonsterSurprised,
   Talk, EscapeIfFastPartyElseCombat, ChainToEvent
};
const int NUM_ENCOUNTER_BUTTON_OPTION_TYPES = 8;

enum labelPostChainOptionsType
{
   LabelDoNothing,
   LabelReturnToQuestion,
   LabelBackupOneStep
};
const int NUM_LABEL_POST_CHAIN_OPTION_TYPES = 3;

enum eventDirType
{
   Any, North, South, East, West,
   N_S, N_E, N_W, S_E, S_W, E_W,
   N_S_E, N_S_W, N_W_E, W_S_E, InFront
};
const int NUM_DIR_TYPES = 16;

enum eventSurpriseType { Neither, PartySurprised, MonsterSurprised };
const int NUM_SURPRISE_TYPES = 3;

enum eventTurnUndeadModType { None, Hard, Difficult, Impossible };
const int NUM_TURN_MOD_TYPES = 4;

enum passwordActionType { NoAction, ChainEvent, Teleport, BackupOneStep };
const int NUM_PASSACTION_TYPES = 4;

enum eventPartyAffectType
{
   NoPartyMember, EntireParty, ActiveChar,
   OneAtRandom, ChanceOnEach
};
const int NUM_AFFECT_TYPES = 5;

enum spellSaveEffectType { NoSave, SaveNegates, SaveForHalf, UseTHAC0 };
const int NUM_SAVE_EFFECT_TYPES = 4;

enum spellSaveVsType { ParPoiDM, PetPoly, RodStaffWand, Sp, BreathWeapon };
const int  NUM_SAVE_VS_TYPES = 5;

enum takeItemQtyType { TakeSpecifiedQty, TakeRndQty, TakePcntQty, TakeAll };
const int NUM_TAKEITEMQTY_TYPES = 4;

enum takeItemsAffectsType { TakeAffectsParty, TakeAffectsRndChar, TakeAffectsActiveChar };
const int NUM_TAKEAFFECTS_TYPES = 3;

enum QuestAcceptType { QA_Impossible, QA_OnYes, QA_OnNo, QA_OnYesOrNo, 
                       QA_ImpossibleAuto, QA_AutoAccept };
const int MAX_QUEST_ACCEPT_TYPES = 6;

// special item, key, quest flags
//const WORD ITEM_FLAG = 0x01;
//const WORD KEY_FLAG = 0x02;
//const WORD QUEST_FLAG = 0x04;
// Changed to enum july 2009 - PRS
enum QUEST_TYPE
{
  NO_FLAG    = 0x00,
  ITEM_FLAG  = 0x01,
  KEY_FLAG   = 0x02,
  QUEST_FLAG = 0x04
  };


enum taleOrderType { InOrderTales, RandomTales };
const int NUM_TALEORDER_TYPES = 2;

enum costFactorType { Free,
                      Div100,Div50,Div20,Div10,Div5,Div4,Div3,Div2,Div1_5,
                      Normal,
                      Mult1_5,Mult2,Mult3,Mult4,Mult5,Mult10,Mult20,Mult50,Mult100 };
const int NUM_COST_FACTOR_TYPES = 20;

enum MathOperationType  { MO_NoOperation, MO_StoredIn, MO_AddedTo, MO_SubtractFrom };
const int MAX_MATH_OPERATION_TYPES = 4;

enum MultiItemCheckType { MIC_None, MIC_AllItems, MIC_AtLeastOneItem };
const int MAX_MULTI_ITEM_CHECK_TYPES = 3;

enum GlobalAreaViewStyle { AnyView, OnlyAreaView, Only3DView };
const int MAX_GLOBAL_AVSTYLES = 3;

enum BlockageType { OpenBlk,
                    OpenSecretBlk,
                    BlockedBlk,
                    FalseDoorBlk, //( secret + blocked )
                    LockedBlk,
                    LockedSecretBlk, // ( secret + locked )
                    LockedWizardBlk,
                    LockedWizardSecretBlk, // ( secret + wizard )
                    LockedKey1, LockedKey2,
                    LockedKey3, LockedKey4,
                    LockedKey5, LockedKey6,
                    LockedKey7, LockedKey8 };
const int NUM_BLOCKAGE_TYPES = 16;

enum alignmentType;
const int NUM_ALIGNMENT_TYPES = 9;

enum creatureSizeType;
const int NUM_SIZE_TYPES = 3;

enum QuestStateType;
const int NUM_QUEST_STATE_TYPE = 4;

enum MappingType { AreaMapping, AutoMapping, NoMapping };
const int NUM_MAPPING_TYPES = 3;

enum itemClassType { itemType=0,
                     PlatinumType=1,
                     ElectrumType=2,
                     GoldType=3,
                     SilverType=4,
                     CopperType=5,
                     GemType=6,
                     JewelryType=7,
                     SpecialItemType=8,
                     SpecialKeyType=9,
                     QuestType=10,
                     CoinType6=12,
                     CoinType7=13,
                     CoinType8=14,
                     CoinType9=15,
                     CoinType10=16,
                     BogusItemType=11,
                     EquipmentSetType = 17
                    };
const int NUM_ITEMCLASS_TYPES = 18;

enum genderType 
{ 
  Male=0, 
  Female=1,
  Bishop=2,
};
const int NUM_GENDER_TYPES = 2;

enum SurfaceType { BogusDib=0, CommonDib=1, CombatDib=2, WallDib=4, DoorDib=8,
                   BackGndDib=16, OverlayDib=32, IconDib=64, OutdoorCombatDib=128,
                   BigPicDib=256, MapDib=512, SmallPicDib=1024, SpriteDib=2048,
                   TitleDib=4096, BufferDib=8192, FontDib=16384, MouseDib=32768,
                   TransBufferDib=65536};
const int NUM_SURFACE_TYPES = 17;

enum ES
{
  ES_ok = 0,
  ES_EOF,
  ES_syntax,
  ES_missingColon,
  ES_newlineInString,
  ES_missingList,
  ES_missingArray,
  ES_illegalValue,
  ES_missingData,
};





class JWriter
{
  CFile *m_f;
  int indent;
  bool linefeed;
  ES   errorStatus;
  CArray<int, int> lineCount; // lineCount[indent];
  char tabs[maxTab+1];
  void OpenBrace(void);
  void OpenBracket(void);
  void Colon(void);
  void Name(const char *name);
  void String(const char *str);
  void MaybeComma(void);
  void Tab(void);
 public:
  JWriter(CFile *cf){m_f=cf; indent=0;memset(tabs,' ',199);tabs[199]=0;linefeed=true;};
  void NameAndColon(const char *name);
  void QuotedString(const char *str);
  void NameAndValue(const char *name, const CString& value);
  void NameAndValue(const char *name, const unsigned char& value);
  void NameAndBinary(const char *name, const CString& value);
  void NameAndQuotedValue(const char *name, const CString& value);
  void NameAndNonBlank(const char *name, const CString& value);
  void NameAndQuotedNonBlank(const char *name, const CString& value);
  void NameAndValue(const char *name, const int& value);
  void NameAndValue(const char *name, const long& value);
  void NameAndValue(const char *name, const WORD& value);
  void NameAndNonZero(const char *name, const int& value){if(value!=0)NameAndValue(name,value);};
  void NameAndNonNegative(const char *name, const int& value){if(value>=0)NameAndValue(name,value);};
  void NameAndValue(const char *name, const DWORD& value);
  void NameAndNonZero(const char *name, const DWORD& value){if(value!=0)NameAndValue(name,value);};
  void NameAndValue(const char *name, const LONGLONG& value);
  void NameAndNonZero(const char *name, const LONGLONG& value){if(value!=0)NameAndValue(name,value);};
  void NameAndValue(const char *name, const double& value);
  void NameAndValue(const char *name, const float& value);
  void NameAndBool(const char *name, const bool& value);
  void NameAndEnum(const char *name, const DWORD& value, DWORD numValue, const char *text[]);
  void NameAndEnum(const char *name, const int& value, DWORD numValue, const char *text[]);
  void NameAndEnum(const char *name, const BYTE& value, DWORD numValue, const char *text[]);
  void NameAndEnum(const char *name, const chainTriggerType& value);
  void NameAndEnum(const char *name, const itemClassType& value);
  void NameAndEnum(const char *name, const eventTriggerType& value);
  void NameAndEnum(const char *name, const genderType& value);
  void NameAndEnum(const char *name, const creatureSizeType& value);
  void NameAndEnum(const char *name, const alignmentType& value);
  void NameAndEnum(const char *name, const eventFacingType& value);
  void NameAndEnum(const char *name, const eventStepType& value);
  void NameAndEnum(const char *name, const encounterButtonResultType& value);
  void NameAndEnum(const char *name, const labelPostChainOptionsType& value);
  void NameAndEnum(const char *name, const eventDistType& value);
  void NameAndEnum(const char *name, const eventDirType& value);
  void NameAndEnum(const char *name, const eventSurpriseType& value);
  void NameAndEnum(const char *name, const eventTurnUndeadModType& value);
  void NameAndEnum(const char *name, const passwordActionType& value);
  void NameAndEnum(const char *name, const eventPartyAffectType& value);
  void NameAndEnum(const char *name, const spellSaveEffectType& value);
  void NameAndEnum(const char *name, const spellSaveVsType& value);
  void NameAndEnum(const char *name, const takeItemsAffectsType& value);
  void NameAndEnum(const char *name, const takeItemQtyType& value);
  void NameAndEnum(const char *name, const taleOrderType& value);
  void NameAndEnum(const char *name, const costFactorType& value);
  void NameAndEnum(const char *name, const MathOperationType& value);
  void NameAndEnum(const char *name, const MultiItemCheckType& value);
  void NameAndEnum(const char *name, const GlobalAreaViewStyle& value);
  void NameAndEnum(const char *name, const BlockageType& value);
  void NameAndEnum(const char *name, const SurfaceType& value);
  void NameAndEnum(const char *name, const QuestStateType& value);
  void NameAndEnum(const char *name, const MappingType& value);
  void NameAndEnum(const char *name, const QuestAcceptType& value);
  void NameAndFlags(const char *name, const BYTE& value, int num, const char *text[]);
  void NameAndFlags(const char *name, const DWORD& value, int num, const char *text[]);
  void NameAndBool(const char *name, const BOOL& value){NameAndBool(name, value!=0);};
  void NameAndDice(const char *name, const int& n, const int& s, const int& b);
  void NameAndValue(const char *name, const int *val, int num);
  void NameAndValue(const char *name, const char *val, int num);
  void NewLine(void);

  void StartList(void);
  void StartList(const char *name);
  void EndList(void);

  void StartArray(void);
  void StartArray(const char *name);
  void ListEntry(void)  {NextEntry(); StartList();};
  void ArrayEntry(void) {NextEntry(); StartArray();};
  void NextEntry();
  void EndArray(void);

  void Linefeed(bool lf){linefeed=lf;};
};

struct JR_DATA;

struct JR_LIST
{
  JR_DATA      *first;
  JR_DATA      *last;
  JR_DATA      *current;
  int          lineNum;
  JR_LIST(void){first=last=current=NULL;};
  ~JR_LIST(void);
};

struct JR_DATA
{
  CString       name;
  CString       stringValue;
  JR_LIST      *listValue; // If non-null then value is another list
  JR_LIST      *arrayValue; // A list of nameless values
  JR_DATA      *next;
  int           lineNum;
  JR_DATA(void) {next = NULL; listValue = NULL; arrayValue = NULL;};
  ~JR_DATA(void);
};

// Ensure that the 'optional' flag is removed by any function
// that wants to test for optional data.
// Now the instance variable 'm_optional' should never be referenced
// by the code.
class OPT
{
public:
  bool opt;
  OPT(bool *optional)
  {
    opt = *optional;
    *optional = false;
  };
};

#define USEOPTIONAL  OPT opt(&m_optional);
#define ISOPTIONAL   (opt.opt)


class JReader
{
  enum TT
  {
    TT_String,
    TT_Colon,
    TT_OpenBrace,
    TT_CloseBrace,
    TT_OpenBracket,
    TT_CloseBracket,
    TT_Comma,
  };
public:
  CFile *m_f;
private:
  FILE    *errors;
  CString token;
  TT      tokenType;
  ES      errStat;
  CString errMessage;
  void    SetError(ES e);
  JR_DATA *pCurRoot;  // A named data point
  JR_DATA *pCurArrayEntry;
  BOOL    nextCharValid;
  void    GetChar(void);
  BOOL    Whitespace(void);
  void    GetToken(void);
  void    GetParseList(JR_LIST **pList);
  void    GetParseArray(JR_LIST **pList);
  char    thisChar;
  char    nextChar;
  CArray<char, char>tokenArray;
  int     tokenSize;
  void    JRError(const char *msg);
  JR_DATA *m_root;
  int     lineNum;
  bool    m_optional;
  int     errLine;
  int     rootStackSize;
  CArray<JR_DATA *, JR_DATA *>rootStack;
  bool    OpenBrace(JR_DATA *pNewRoot, DEBUG_ID dbid);
  bool    OpenBracket(JR_DATA *pNewRoot);
  void    DeleteData(JR_DATA **first);
  bool    m_thisIsACopy;
  char    inBuf[1000];
  int     inBufSize;
  int     inputCol;
public:
  JReader(CFile *cf)
  {
    m_f = cf; 
    pCurArrayEntry=m_root=pCurRoot=NULL;
    m_optional=false;
    rootStackSize=0;
    m_thisIsACopy=false;
    errLine = 0;
    lineNum = 0;
    errors = fopen(jsonPath + "JReaderErrors.txt","w");
  };
  JReader(const JReader& jr);
  ~JReader(void);
  void    operator =(const JReader& jr);
  void    Clear(void);
  void    Rewind(void);
  ES      GetError(void);
  void    PrintError(const char *msg) const;
  JR_DATA *Locate(const char *name) const;
  bool    Fetch_String(const char *name, CString *result) const;
  CString Fetch_String(const char *name) const;
  bool    Fetch_Int(const char *name, int *result) const;
  int     Fetch_Int(const char *name) const;
  bool    Fetch_Bool(const char *name, bool *result) const;
  bool    Fetch_Bool(const char *name) const;
  bool    Fetch_Float(const char *name, double *result) const;
  double  Fetch_Float(const char *name) const;
  CString Fetch_YesNo(const char *name) const;
  CString GetErrorMessage(void);
  int     Initialize(void);
  inline  void Optional(){m_optional = true;};
  void    EndList(DEBUG_ID dbid);
  void    Colon(void);
  void    Linefeed(bool){return;}; // Just so read looks more like write
  bool    Name(const char *name);
  void    String(const char *str);
  void    QuotedString(char term);
  bool    NameAndValue(const char *name,  CString& value);
  bool    NameAndBinary(const char *name, CString& value);
  bool    NameAndQuotedNonBlank(const char *name,  CString& value)
          {
            Optional();
            if (NameAndValue(name, value)) return true;
            value.Empty(); return false;
          };
  bool    NameAndNonBlank(const char *name,  CString& value)
          {
            Optional();
            if (NameAndValue(name, value)) return true;
            value.Empty(); return false;
          };
  bool    NameAndQuotedValue(const char *name, CString& value){return NameAndValue(name, value);};
  bool    NameAndValue(const char *name,  unsigned char& value);
  bool    NameAndValue(const char *name,  char& value);
  bool    NameAndValue(const char *name, int& value);
  bool    NameAndValue(const char *name, long& value);
  bool    NameAndValue(const char *name, WORD& value);
  bool    NameAndNonZero(const char *name, int& value)
          {
            Optional();
            if (NameAndValue(name, value)) return true;
            value = 0; return false;
          };
  bool    NameAndNonNegative(const char *name, int& value)
          {
            Optional();
            if (NameAndValue(name, value)) return true;
            value = -1; return false;
          };
  bool    NameAndNonZero(const char *name, DWORD& value)
          {
            Optional();
            if (NameAndValue(name, value)) return true;
            value = 0; return false;
          };
  bool    NameAndValue(const char *name, DWORD& value);
  bool    NameAndValue(const char *name, LONGLONG& value);
  bool    NameAndValue(const char *name, double& valuef);
  bool    NameAndValue(const char *name, float& value);
  bool    NameAndBool(const char *name, bool& value);
  bool    NameAndBool(const char *name, BYTE& value);
  bool    NameAndEnum(const char *name, DWORD& value, DWORD numValue, const char *text[]);
  bool    NameAndEnum(const char *name, int& value, DWORD numValue, const char *text[]);
  bool    NameAndEnum(const char *name, BYTE& value, DWORD numValue, const char *text[]);
  bool    NameAndEnum(const char *name, eventDistType& value);
  bool    NameAndEnum(const char *name, itemClassType& value);
  bool    NameAndEnum(const char *name, genderType& value);
  bool    NameAndEnum(const char *name, creatureSizeType& value);
  bool    NameAndEnum(const char *name, alignmentType& value);
  bool    NameAndEnum(const char *name, chainTriggerType& value);
  bool    NameAndEnum(const char *name, takeItemsAffectsType& value);
  bool    NameAndEnum(const char *name, takeItemQtyType& value);
  bool    NameAndEnum(const char *name, eventTriggerType& value);
  bool    NameAndEnum(const char *name, eventType& value);
  bool    NameAndEnum(const char *name, eventPartyAffectType& value);
  bool    NameAndEnum(const char *name, eventFacingType& value);
  bool    NameAndEnum(const char *name, eventStepType& value);
  bool    NameAndEnum(const char *name, eventDirType& value);
  bool    NameAndEnum(const char *name, eventTurnUndeadModType& value);
  bool    NameAndEnum(const char *name, eventSurpriseType& value);
  bool    NameAndEnum(const char *name, encounterButtonResultType& value);
  bool    NameAndEnum(const char *name, labelPostChainOptionsType& value);
  bool    NameAndEnum(const char *name, spellSaveEffectType& value);
  bool    NameAndEnum(const char *name, spellSaveVsType& value);
  bool    NameAndEnum(const char *name, passwordActionType& value);
  bool    NameAndEnum(const char *name, MathOperationType& value);
  bool    NameAndEnum(const char *name, MultiItemCheckType& value);
  bool    NameAndEnum(const char *name, taleOrderType& value);
  bool    NameAndEnum(const char *name, costFactorType& value);
  bool    NameAndEnum(const char *name, GlobalAreaViewStyle& value);
  bool    NameAndEnum(const char *name, BlockageType& value);
  bool    NameAndEnum(const char *name, SurfaceType& value);
  bool    NameAndEnum(const char *name, QuestStateType& value);
  bool    NameAndEnum(const char *name, MappingType& value);
  bool    NameAndEnum(const char *name, QuestAcceptType& value);
  bool    NameAndFlags(const char *name, BYTE& value, int num, const char *text[]);
  bool    NameAndFlags(const char *name, DWORD& value, int num, const char *text[]);
  bool    NameAndBool(const char *name, BOOL& value);
  bool    NameAndDice(const char *name, int& n, int& s, int& b);

  bool    StartList(const char *name, DEBUG_ID dbid);
  bool    StartList(DEBUG_ID dbid);
  CString CurrentTag(void);
  CString CurrentValue(void);
  void    NextTag(void);

  bool    StartArray(const char *name);
  bool    StartArray(void);
  bool    ListEntry(DEBUG_ID dbid) {return NextEntry() && StartList(dbid);};
  int     LineNum(void);
  bool    ArrayEntry() {return NextEntry() && StartArray();};
  bool    NextEntry();
  bool    EndArray(void);

  bool    NameAndValue(const char *name, int *val, int num);
  bool    NameAndValue(const char *name, char *val, int num);
};

