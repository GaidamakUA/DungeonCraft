#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class QUESTION_LIST_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  long UserResult;
  QLIST_DATA buttons;
  int sx,sy;
};



 
void Event_QuestionList(JReader& event, JReader& details, JWriter& jw)
{
  QUESTION_LIST_DATA question_list_data;
  question_list_data.Read(event, details);
  question_list_data.Export(jw);
}

void QUESTION_LIST_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  buttons.Export(jw);
  jw.EndList();
}

void QUESTION_LIST_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, QuestionList);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
  buttons.Read(details);
  return;
}
