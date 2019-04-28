#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class QUESTION_BUTTON_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  void Convert(void);
  long UserResult;
  BOOL    ForceBackup[5];
  BOOL    Present[5];
  BOOL    ReturnToQuestion[5];
  int     Chain[5];
  CString Label[5];
  QBUTTON_DATA buttons;
  int sx,sy;
};

void QUESTION_BUTTON_DATA::Convert(void)
{
  int i;
  for (i=0; i<5; i++)
  {
    buttons.buttons[i].present = Present[i];
    buttons.buttons[i].label = Label[i];
    buttons.buttons[i].postChainAction = LabelDoNothing;
    if (ReturnToQuestion[i]) buttons.buttons[i].postChainAction = LabelReturnToQuestion;
    if (ForceBackup[i]) buttons.buttons[i].postChainAction = LabelBackupOneStep;
    buttons.buttons[i].chain = Chain[i];
  };
  buttons.numListButtons = 5;
};

void Event_QuestionButton(JReader& event, JReader& details, JWriter& jw)
{
  QUESTION_BUTTON_DATA question_button_data;
  question_button_data.Read(event, details);

  question_button_data.Convert();

  question_button_data.Export(jw);
}

void QUESTION_BUTTON_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  buttons.Export(jw);
  jw.EndList();
}

void QUESTION_BUTTON_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, QuestionButton);
  details.NameAndBool("button_1_force_backup_one_step", ForceBackup[0]);
  details.NameAndBool("button_2_force_backup_one_step", ForceBackup[1]);
  details.NameAndBool("button_3_force_backup_one_step", ForceBackup[2]);
  details.NameAndBool("button_4_force_backup_one_step", ForceBackup[3]);
  details.NameAndBool("button_5_force_backup_one_step", ForceBackup[4]);

  details.NameAndValue("button_1_chain_event", Chain[0]);
  details.NameAndValue("button_2_chain_event", Chain[1]);
  details.NameAndValue("button_3_chain_event", Chain[2]);
  details.NameAndValue("button_4_chain_event", Chain[3]);
  details.NameAndValue("button_5_chain_event", Chain[4]);

  details.NameAndValue("button_1_label", Label[0]);
  details.NameAndValue("button_2_label", Label[1]);
  details.NameAndValue("button_3_label", Label[2]);
  details.NameAndValue("button_4_label", Label[3]);
  details.NameAndValue("button_5_label", Label[4]);

  details.NameAndBool("button_1_present", Present[0]);
  details.NameAndBool("button_2_present", Present[1]);
  details.NameAndBool("button_3_present", Present[2]);
  details.NameAndBool("button_4_present", Present[3]);
  details.NameAndBool("button_5_present", Present[4]);

  details.NameAndBool("button_1_return_to_questions", ReturnToQuestion[0]);
  details.NameAndBool("button_2_return_to_questions", ReturnToQuestion[1]);
  details.NameAndBool("button_3_return_to_questions", ReturnToQuestion[2]);
  details.NameAndBool("button_4_return_to_questions", ReturnToQuestion[3]);
  details.NameAndBool("button_5_return_to_questions", ReturnToQuestion[4]);

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

  return ;
}
