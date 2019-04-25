
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class QUEST_EVENT_DATA : public GameEvent
{
public:
//  DECLARE_SERIAL( QUEST_EVENT_DATA )
  QUEST_EVENT_DATA(void)
  {
    evType = QuestStage;
    m_quest = 0;
    stage = 0;
    acceptChain = 0;
    rejectChain = 0;
  };
  void Clear();
  void Export(JWriter& jw);
  void Read(JReader& events, JReader& details);

  QuestAcceptType accept;
  BOOL completeOnAccept;
  BOOL failOnRejection;
//-----
private: // PRS July 2009 - Encode quest type in top 4 bits (quest, item, key)
         // These four functions are sufficient to manipulate m_quest.
  int m_quest;
public:
  inline int EncodeQuestTypeAndID(QUEST_TYPE type, int id){return ((type&7)<<28)|id;};
  inline void SetQuestTypeAndID(QUEST_TYPE type, int id)
  {
    if (id < 0) 
    {
      m_quest = -1;
    }
    else
    {
      m_quest=EncodeQuestTypeAndID(type,id);
    };
  };
//  inline void SetQuestTypeAndID(int taid){m_quest=taid;};
  inline QUEST_TYPE QuestType(void)
  {
    if (m_quest < 0) return NO_FLAG;
    if ((m_quest & 0x70000000) == 0) return QUEST_FLAG; // Default
    return (QUEST_TYPE)((m_quest>>28) & 0x7);
  };
  inline int QuestID(void){return m_quest&0xfffffff;};
//-----
  WORD stage;
  DWORD acceptChain;
  DWORD rejectChain;
};


void Event_Quest(JReader& event, JReader& details, JWriter& jw)
{
  QUEST_EVENT_DATA quest_event_data;
  quest_event_data.Read(event, details);
  quest_event_data.Export(jw);
}

void QUEST_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);

  jw.NameAndEnum(JKEY_ACCEPTTYPE, accept);
  jw.NameAndBool(JKEY_FAILONREJECT, failOnRejection);
  jw.NameAndBool(JKEY_COMPLETEONACCEPT, completeOnAccept);
  jw.NameAndValue(JKEY_QUESTNUM, m_quest);
  jw.NameAndValue(JKEY_QUESTSTAGE, stage);
  jw.NameAndValue(JKEY_ACCEPTCHAIN, acceptChain);
  jw.NameAndValue(JKEY_REJECTCHAIN, rejectChain);
  
// Jadefang says:
//For FRUA importation, a FRUA quest stage set at 128 
//should be turned into a DC quest stage set to 65001 
//but should trigger if the current stage of the quest 
//is 127. And a FRUA utilities event that sets a quest
//to 128 should probably be translated into a DC 65001
  jw.EndList();

}

void QUEST_EVENT_DATA::Read(JReader& event, JReader& details)
{
  CString temp;
  GameEvent::Read(event, QuestStage);
  details.NameAndValue("quest_accepted_on", temp);
  {
    if (temp == "impossible")             accept = QA_Impossible;
    if (temp == "yes")                    accept = QA_OnYes; 
    if (temp == "no")                     accept = QA_OnNo;
    if (temp == "yes or no")              accept = QA_OnYesOrNo;                    ;
    if (temp == "impossible no question") accept = QA_ImpossibleAuto;
    if (temp == "automatic no question")  accept = QA_AutoAccept;
  }
  details.NameAndBool("quest_fails_on_reject", failOnRejection);
  details.NameAndBool("quest_completes_on_accept", completeOnAccept);
  details.NameAndValue("special_item_key_quest", temp);
  //need quest number
  if (memcmp(temp, "special quest ",14) == 0)
  {
    m_quest = atoi((LPCTSTR)temp+14);
  };
  details.NameAndValue("accept_event", acceptChain);
  details.NameAndValue("reject_event", rejectChain); 
  details.NameAndValue("stage", stage);
  details.NameAndValue("text", text);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
}
