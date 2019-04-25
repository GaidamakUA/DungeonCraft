
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int


enum passwordMatchType { ExactMatch, PswdInTyped, TypedInPswd };
const int NUM_PASSMATCH_TYPES = 3;


class PASSWORD_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& details);
  CString password;
  passwordActionType successAction;
  passwordActionType failAction;
  passwordMatchType matchCriteria;
  BOOL matchCase;
  int nbrTries;
  int currTry;
  long Unused;  
  DWORD successChain;
  DWORD failChain; 
  TRANSFER_DATA successTransferData;
  TRANSFER_DATA failTransferData;
};



void Event_Password(JReader& event, JReader& details, JWriter& jw)
{
  PASSWORD_DATA password_data;
  password_data.Read(event, details);
  password_data.Export(jw);
}

void PASSWORD_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, EnterPassword);
  details.NameAndValue(JKEY_PASSWORD, password);
  //details.NameAndValue(JKEY_NUMBEROFTRIES, nbrTries);
  details.NameAndValue(JKEY_NUMATTEMPTSALLOWED, nbrTries);
  //details.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  details.NameAndValue(JKEY_SUCCESSEVENT, successChain);
  //details.NameAndValue(JKEY_FAILCHAIN, failChain);
  details.NameAndValue(JKEY_FAILEVENT, failChain);
  //details.NameAndEnum(JKEY_SUCCESSACTION, successAction);

  {
    CString v;
    details.NameAndValue(JKEY_success_action, v);
    successAction = NoAction;
    if (v == "No Action")       successAction = NoAction;
	  if (v == "chain Event")     successAction = ChainEvent;
	  if (v == "teleport")        successAction = Teleport;
	  if (v == "backup one step") successAction = BackupOneStep;
  };
  //details.NameAndEnum(JKEY_FAILACTION, failAction);
  details.NameAndEnum(JKEY_fail_action, failAction);
  details.NameAndValue("text", text);
  details.NameAndValue("success_text", text2);
  details.NameAndValue("fail_text", text3);
  details.NameAndValue("picture", pic.filename);
  if (pic.filename.IsEmpty())
  {
    pic.picType = BogusDib;
  }
  else
  {
    pic.picType = SmallPicDib;
  };
  successTransferData.Read(details, "teleport_dest_event_execute", "teleport_x", "teleport_y", "teleport_dest_facing");
  failTransferData.Read(details, "dest_event_execute", "dest_x", "dest_y", "dest_facing");
}

void PASSWORD_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndValue(JKEY_PASSWORD, password);
  jw.NameAndValue(JKEY_NUMBEROFTRIES, nbrTries);
  jw.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jw.NameAndValue(JKEY_FAILCHAIN, failChain);
  jw.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  jw.NameAndEnum(JKEY_FAILACTION, failAction);
  jw.NameAndValue(JKEY_MATCHCASE, "false");

  successTransferData.Export(jw, JKEY_SUCCESSDESTINATION);
  failTransferData.Export(jw, JKEY_FAILDESTINATION);
  jw.EndList();
}
