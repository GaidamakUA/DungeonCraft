class EVENT_CONTROL //: public CObject
{
public:
   void Clear(void);
   void Export(JWriter& jw);
   void Read(JReader& jr);
   
   BOOL onceOnly;
   chainTriggerType chainTrigger;
   eventTriggerType eventTrigger;
   eventDirType facing;
   CString raceID;
   CLASS_BASECLASS_ID classBaseclassID;
   genderType gender; // stored as attribute
   CString itemID;
   int quest;
   int chance;
   int specialItem;
   int specialKey;
   CString characterID;
   int partyX, partyY;
   CString memorizedSpellID;
   DWORD memorizedSpellClass;
   DWORD memorizedSpellLevel;
   CString gpdlData;
   BOOL gpdlIsBinary;

};





class GameEvent //: public CObject
{
public:
//  DECLARE_SERIAL( GameEvent )
  GameEvent(void){Clear();};
  void Clear(void);
  void Export(JWriter& jw);
  void Read(JReader& jr, eventType type);

  int jsonLineNum;
  const char *Name(void);
  eventType evType;
  DWORD id;
  int x, y;
  EVENT_CONTROL control;
  PIC_DATA pic;
  PIC_DATA pic2;
  CString text;
  CString text2;
  CString text3;
  DWORD chainEventHappen;
  DWORD chainEventNotHappen;
  LONGLONG timer;  // virtualGameTime at which an OnTimer event should be posted. (or zero).
  UINT timerID;     
  EventSourceType m_source;
};





class TALE //: public CObject
{
public:
  void Export(JWriter& jw, int index);
  bool Read(JReader& jr, int index);
  CString tale;
  int count;
};

class DRINK //: public CObject
{
public:
  void Export(JWriter& jw, int index);
  bool Read(JReader& jr, int index);
  CString name;
  int points;
};




class TAVERN : public GameEvent
{
public:
//  DECLARE_SERIAL( TAVERN )
  TAVERN(void):GameEvent()
  {
    forceExit=allowFights=allowDrinks=EachTaleOnceOnly=FALSE;
    fightChain=drinkChain=0;
    inflation=0;
    barkeep=0;
    drinkPointTrigger=0;
    taleOrder=InOrderTales;
    evType = TavernEvent;
  };
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
	BOOL forceExit;
  BOOL allowFights;
  BOOL allowDrinks;
  BOOL EachTaleOnceOnly;
	int inflation;
	int barkeep;
  DWORD fightChain;
  DWORD drinkChain;
  int drinkPointTrigger;
  long Unused;
  taleOrderType taleOrder;
  TALE tales[MAX_TALES];
  DRINK drinks[MAX_DRINKS];
};




class TEMPLE : public GameEvent
{
public:
  TEMPLE(void);
  TEMPLE::~TEMPLE(void);
  void Clear(void);
  CString characterID;
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  long m_state;
	BOOL forceExit;
  BOOL allowDonations;
  enum costFactorType costFactor;
  int maxLevel;
  int donationTrigger;
  int totalDonation;
  DWORD donationChain;
  spellBookType& templeSpells;
};

class VAULT_EVENT_DATA : public GameEvent
{
public:
  VAULT_EVENT_DATA(void)
  {
    evType = Vault;
    WhichVault = 0;
    ForceBackup = FALSE;
  };
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BYTE WhichVault;
  BOOL ForceBackup;
};

class CAMP_EVENT_DATA : public GameEvent
{
public:
  CAMP_EVENT_DATA(void)
  {
    evType = Camp;
    ForceExit = FALSE;
  };
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL ForceExit;
};


class SHOP : public GameEvent
{
public:
  SHOP(void)
  {
    evType = ShopEvent;
    forceExit = FALSE;
    costFactor = Free;
    costToId = 0;
    buybackPercentage = 0;
    buyItemsSoldOnly = FALSE;
    sellprice = 0;
    canId = FALSE;
    canApprGems = FALSE;
    canApprJewels = FALSE;
  };
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
	BOOL forceExit;
  costFactorType costFactor;
  int costToId;
  int buybackPercentage;
  BOOL buyItemsSoldOnly;
  int sellprice;
  BOOL canId;
  BOOL canApprGems;
  BOOL canApprJewels;
	ITEM_LIST itemsAvail;
};

struct TRAINABLE_BASECLASS
{
  CString baseclassID;
  int minLevel;
  int maxLevel;
  CString whateverthosenutsmightwant;
  void Export(JWriter& jw) const;
  bool Read(JReader& jr);
};


class TRAININGHALL : public GameEvent
{
public:
  TRAININGHALL(void)
  {
    evType = TrainingHallEvent;
    forceExit = FALSE;
    Cost = 0;
  };
  void Export(JWriter& jw);
  bool Read(JReader& event, JReader& jr);
  void AddBaseclass(const char *baseclassName)
  {
    TRAINABLE_BASECLASS tb;
    tb.baseclassID = baseclassName;
    tb.maxLevel = 40;
    tb.minLevel = 1;
    tb.whateverthosenutsmightwant.Empty();
    trainableBaseclasses.Add(tb);
  }
	BOOL forceExit;
  CArray<TRAINABLE_BASECLASS, TRAINABLE_BASECLASS&> trainableBaseclasses;
	int Cost;  
};


