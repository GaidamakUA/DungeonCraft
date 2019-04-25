#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


class WHO_PAYS_EVENT_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL impossible;
  passwordActionType successAction;
  passwordActionType failAction;
  int gems;
  int jewels;
  int platinum;
  itemClassType moneyType; // which money type to take (plat,gold,etc)
  long Unused;
  DWORD successChain;
  DWORD failChain;
  TRANSFER_DATA successTransferData;
  TRANSFER_DATA failTransferData;
};




void Event_WhoPays(JReader& event, JReader& details, JWriter& jw)
{
  WHO_PAYS_EVENT_DATA who_pays_event_data;
  who_pays_event_data.Read(event, details);
  who_pays_event_data.Export(jw);
}


void WHO_PAYS_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_POSSIBLE, !impossible);
  jw.NameAndValue(JKEY_GEMS, gems);
  jw.NameAndValue(JKEY_JEWELS, jewels);
  jw.NameAndValue(JKEY_PLATINUM, platinum);
  jw.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jw.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  if (successAction == Teleport)
  {
    successTransferData.Export(jw, JKEY_SUCCESSTELEPORT);
  };
  jw.NameAndValue(JKEY_FAILCHAIN, failChain);
  jw.NameAndEnum(JKEY_FAILACTION, failAction);
  if (failAction == Teleport)
  {
    failTransferData.Export(jw, JKEY_FAILTELEPORT);
  };
  //jw.NameAndEnum(JKEY_MONEYTYPE, moneyType);
  jw.NameAndValue(JKEY_MONEYTYPE, "Platinum");
  jw.EndList();
}

void WHO_PAYS_EVENT_DATA::Read(JReader& event, JReader& details)
{
  bool possible = true;
  GameEvent::Read(event, WhoPays);
  {
    CString check;
    int amount;
    details.NameAndValue("success_check", check);
    details.NameAndValue("success_amount", amount);
    impossible = false;
    gems = 0;
    jewels = 0;
    platinum = 0;
    if (check == "impossible") {impossible = true;};
    if (check == "sufficient gems") {gems = amount;};
    if (check == "sufficient jewelry") {jewels = amount;};
    if (check == "sufficient platinum") {platinum = amount;};
  };
  //details.NameAndBool(JKEY_POSSIBLE, possible);
  //details.NameAndValue(JKEY_GEMS, gems);
  //details.NameAndValue(JKEY_JEWELS, jewels);
  //details.NameAndValue(JKEY_PLATINUM, platinum);

  //details.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  //details.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  details.NameAndValue(JKEY_SUCCESSEVENT, successChain);
  {
    CString temp;
    details.NameAndValue(JKEY_success_action, temp);
    if (temp == "teleport") successAction = Teleport;
    if (temp == "no action") successAction = NoAction;
    if (temp == "chain event") successAction = ChainEvent;
  };

  successTransferData.Read(details, "teleport_dest_event_execute", "teleport_x", "teleport_y", "teleport_dest_facing");
  details.NameAndValue(JKEY_FAILEVENT, failChain);
  {
    CString temp;
    details.NameAndValue(JKEY_fail_action, temp);
    if (temp == "teleport") failAction = Teleport;
    if (temp == "no action") failAction = NoAction;
    if (temp == "chain event") failAction = ChainEvent;
  };
  failTransferData.Read(details, "teleport_event_execute", "teleport_x", "teleport_y", "teleport_dest_facing");
  details.NameAndValue("text", text);
  details.NameAndValue("fail_text", text3);
  details.NameAndValue("success_text", text2);
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
