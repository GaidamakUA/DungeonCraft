
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class QUESTION_YES_NO : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  labelPostChainOptionsType YesChainAction;
  labelPostChainOptionsType NoChainAction; 
  DWORD YesChain;
  DWORD NoChain;
  long Unused;
  int sx,sy;
};




void Event_QuestionYesNo(JReader& event, JReader& details, JWriter& jw)
{
  QUESTION_YES_NO question_yes_no;
  question_yes_no.Read(event, details);
  question_yes_no.Export(jw);
}


void QUESTION_YES_NO::Export(JWriter& jw)
{
    GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
    jw.NameAndEnum(JKEY_YESCHAINACTION, YesChainAction);
    jw.NameAndNonZero(JKEY_YESCHAIN, YesChain);
    jw.NameAndEnum(JKEY_NOCHAINACTION, NoChainAction);
    jw.NameAndNonZero(JKEY_NOCHAIN, NoChain);
  jw.EndList();
}

void QUESTION_YES_NO::Read(JReader& event, JReader& details)
{
    GameEvent::Read(event, QuestionYesNo);
    details.NameAndEnum("yes_action", YesChainAction);
    details.NameAndNonZero("yes_event", YesChain);
    details.NameAndEnum("no_action", NoChainAction);
    details.NameAndNonZero("no_event", NoChain);
    details.NameAndValue("text", text);
    details.NameAndValue("yes_text", text2); 
    details.NameAndValue("no_text", text3);
    details.NameAndValue("picture", pic.filename);
    if (pic.filename.IsEmpty())
    {
      pic.picType = BogusDib;
    }
    else
    {
      pic.picType = SmallPicDib;
    };
    return;
}
