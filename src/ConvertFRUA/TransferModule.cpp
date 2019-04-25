#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool

class TRANSFER_EVENT_DATA : public GameEvent
{
public:
  TRANSFER_EVENT_DATA(void){Clear();};
  void Clear(void);
  void Export(JWriter& jw);
  void ReadTeleporter(JReader& event, JReader& jr);
  void ReadStairs(JReader& event, JReader& jr);
  void ReadTransferModule(JReader& event, JReader& jr);
  BOOL askYesNo;
  BOOL transferOnYes;
  BOOL destroyDrow;
  BOOL activateBeforeEntry;
  TRANSFER_DATA transferData;
};

void TRANSFER_EVENT_DATA::Clear(void)
{
  transferData.Clear();
}




void TRANSFER_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_ASKYESNO, askYesNo);
  jw.NameAndBool(JKEY_TRANSFER, transferOnYes);
  jw.NameAndBool(JKEY_DESTROYDROW, destroyDrow);
  jw.NameAndBool(JKEY_ACTIVATEBEFOREENTRY, activateBeforeEntry);
  transferData.Export(jw, JKEY_TRANSFERLOCATION);
  jw.EndList();
}

void TRANSFER_EVENT_DATA::ReadStairs(JReader& event, JReader& details)
{
  GameEvent::Read(event, TransferModule);
  CString textAsk;
  details.NameAndBool("ask_yes_no", askYesNo);
  details.NameAndBool("transfer_on_yes_or_no", transferOnYes);
  textAsk = details.Fetch_String("text_ask");
  //details.NameAndBool("destroy_drow_equipment_after_transfer", destroyDrow);
  destroyDrow = false;
  this->text = textAsk;
  //details.NameAndBool(JKEY_ACTIVATEBEFOREENTRY, activateBeforeEntry);
  activateBeforeEntry = true;
  transferData.Read(details, "dest_event_execute", "dest_x", "dest_y", "dest_facing");
  pic.ReadSmallPicFile(details, "picture");
  details.NameAndValue("text_enter", text2);
  details.NameAndValue("text_ask", text);
  return;
}

void TRANSFER_EVENT_DATA::ReadTransferModule(JReader& event, JReader& details)
{
  GameEvent::Read(event, TransferModule);
  CString textAsk;
  details.NameAndBool("ask_yes_no", askYesNo);
  details.NameAndBool("transfer_on_yes_no", transferOnYes);
  textAsk = details.Fetch_String("text_1");
  details.NameAndBool("destroy_drow_equipment_after_transfer", destroyDrow);
  this->text = textAsk;
  this->text2 = details.Fetch_String("text_2");
  //details.NameAndBool(JKEY_ACTIVATEBEFOREENTRY, activateBeforeEntry);
  {
    CString temp;
    details.NameAndValue("transfer_when", temp);
    activateBeforeEntry = true;
    if (temp == "party in cell") activateBeforeEntry = false;
  };
  transferData.Read(details, "dest_event_execute", "dest_x", "dest_y", "dest_facing");
  pic.ReadSmallPicFile(details, "picture");
  return;
}

void TRANSFER_EVENT_DATA::ReadTeleporter(JReader& event, JReader& details)
{
  GameEvent::Read(event, TransferModule);
  CString textAsk;
  details.NameAndBool("ask_yes_no", askYesNo);
  {
    CString value;
    details.NameAndValue("transfer_on_yes_no", value);
    transferOnYes = value == "yes";
  };
  textAsk = details.Fetch_String("text_1");
  //details.NameAndBool("destroy_drow_equipment_after_transfer", destroyDrow);
  destroyDrow = false;
  this->text2 = details.Fetch_String("text_2");;
  //details.NameAndBool(JKEY_ACTIVATEBEFOREENTRY, activateBeforeEntry);
  activateBeforeEntry = true;
  transferData.Read(details, "dest_event_execute", "dest_x", "dest_y", "dest_facing");
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
  details.NameAndValue("text_1", text);
  return;
}
