
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class WHO_TRIES_EVENT_DATA : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL alwaysSucceeds;
  BOOL alwaysFails;
  BOOL checkSTR;
  BOOL checkINT;
  BOOL checkWIS;
  BOOL checkDEX;
  BOOL checkCON;
  BOOL checkCHA;
  BOOL checkPP;
  BOOL checkOL;
  BOOL checkFT;
  BOOL checkMS;
  BOOL checkHS;
  BOOL checkHN;
  BOOL checkCW;
  BOOL checkRL;
  BYTE strBonus;
  BOOL compareToDie; // if false, then compareDie is actual compare value
  passwordActionType successAction;
  passwordActionType failAction;
  int  compareDie;   // dice sides, or actual compare value
  int  NbrTries;
  int  currTry;
  long Unused;
  DWORD successChain;
  DWORD failChain;
  TRANSFER_DATA successTransferData;
  TRANSFER_DATA failTransferData;
};





void Event_WhoTries(JReader& event, JReader& details, JWriter& jw)
{
  WHO_TRIES_EVENT_DATA who_tries_event_data;
  who_tries_event_data.Read(event, details);
  who_tries_event_data.Export(jw);
}

void WHO_TRIES_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_ALWAYSSUCCEEDS, alwaysSucceeds);
  jw.NameAndBool(JKEY_ALWAYSFAILS, alwaysFails);
  jw.NameAndBool(JKEY_CHECKSTR, checkSTR);
  jw.NameAndBool(JKEY_CHECKINT, checkINT);
  jw.NameAndBool(JKEY_CHECKWIS, checkWIS);
  jw.NameAndBool(JKEY_CHECKDEX, checkDEX);
  jw.NameAndBool(JKEY_CHECKCON, checkCON);
  jw.NameAndBool(JKEY_CHECKCHA, checkCHA);
  jw.NameAndBool(JKEY_CHECKPP, checkPP);
  jw.NameAndBool(JKEY_CHECKOL, checkOL);
  jw.NameAndBool(JKEY_CHECKFT, checkFT);
  jw.NameAndBool(JKEY_CHECKMS, checkMS);
  jw.NameAndBool(JKEY_CHECKHS, checkHS);
  jw.NameAndBool(JKEY_CHECKHN, checkHN);
  jw.NameAndBool(JKEY_CHECKCW, checkCW);
  jw.NameAndBool(JKEY_CHECKRL, checkRL);
  jw.NameAndValue(JKEY_STRBONUS, strBonus);
  jw.NameAndBool(JKEY_COMPARETODIE, compareToDie);
  jw.NameAndValue(JKEY_DIESIDES, compareDie);
  jw.NameAndValue(JKEY_NUMBEROFTRIES, NbrTries);
  jw.NameAndEnum(JKEY_SUCCESSACTION, successAction);
  jw.NameAndValue(JKEY_SUCCESSCHAIN, successChain);
  jw.NameAndEnum(JKEY_FAILACTION, failAction);
  jw.NameAndValue(JKEY_FAILCHAIN, failChain);
  successTransferData.Export(jw, JKEY_SUCCESSTRANSFER);
  failTransferData.Export(jw, JKEY_FAILTRANSFER);
  jw.EndList();
}

void WHO_TRIES_EVENT_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, WhoTries);
  alwaysSucceeds = FALSE; //details.NameAndBool(JKEY_ALWAYSSUCCEEDS, alwaysSucceeds);
  alwaysFails = FALSE;    //details.NameAndBool(JKEY_ALWAYSFAILS, alwaysFails);

  {
    CString op;

    
/*
   case 0: details["check_operation"] = "always succeed"; break;
   case 4: details["check_operation"] = "always fail"; break;
   case 8: details["check_operation"] = "compare strength"; break;
   case 12: details["check_operation"] = "compare intelligence"; break;
   case 16: details["check_operation"] = "compare wisdom"; break;
   case 20: details["check_operation"] = "compare dexterity"; break;
   case 24: details["check_operation"] = "compare constitution"; break;
   case 28: details["check_operation"] = "compare charisma"; break;
   case 32: details["check_operation"] = "compare thief pick pocket"; break;
   case 36: details["check_operation"] = "compare thief open lock"; break;
   case 40: details["check_operation"] = "compare thief find remove trap"; break;
   case 44: details["check_operation"] = "compare thief move silently"; break;
   case 48: details["check_operation"] = "compare thief hide in shadow"; break;
   case 52: details["check_operation"] = "compare thief hear noise"; break;
   case 56: details["check_operation"] = "compare thief climb wall"; break;
   case 60: details["check_operation"] = "compare thief read language"; break;
   default: details["check_operation"] = UnknownString; break;
*/    
   details.NameAndValue("check_operation", op); 
   if (op == "always succeed"){alwaysSucceeds = true;}
   else if (op == "always fail"){alwaysFails = true;}
   else if (op == "compare strength") checkSTR=true;
   else if (op == "compare intelligence") checkINT=true;
   else if (op == "compare wisdom") checkWIS=true;
   else if (op == "compare dexterity") checkDEX=true;
   else if (op == "compare constitution") checkCON=true;
   else if (op == "compare charisma") checkCHA=true;
   else if (op == "compare thief pick pocket") checkPP=true;
   else if (op == "compare thief open lock") checkOL=true;
   else if (op == "compare thief find remove trap") checkFT=true;
   else if (op == "compare thief move silently") checkMS=true;
   else if (op == "compare thief hide in shadow") checkHS=true;
   else if (op == "compare thief hear noise") checkHN=true;
   else if (op == "compare thief climb wall") checkCW=true;
   else if (op == "compare thief read language") checkRL=true;
     else details.PrintError("Cannot determine WHO TRIES operation");
  };

  //details.NameAndBool(JKEY_CHECKSTR, checkSTR);
  //details.NameAndBool(JKEY_CHECKINT, checkINT);
  //details.NameAndBool(JKEY_CHECKWIS, checkWIS);
  //details.NameAndBool(JKEY_CHECKDEX, checkDEX);
  //details.NameAndBool(JKEY_CHECKCON, checkCON);
  //details.NameAndBool(JKEY_CHECKCHA, checkCHA);
  //details.NameAndBool(JKEY_CHECKPP, checkPP);
  //details.NameAndBool(JKEY_CHECKOL, checkOL);
  //details.NameAndBool(JKEY_CHECKFT, checkFT);
  //details.NameAndBool(JKEY_CHECKMS, checkMS);
  //details.NameAndBool(JKEY_CHECKHS, checkHS);
  //details.NameAndBool(JKEY_CHECKHN, checkHN);
  //details.NameAndBool(JKEY_CHECKCW, checkCW);
  //details.NameAndBool(JKEY_CHECKRL, checkRL);
  details.NameAndValue("check_value", strBonus);
  compareToDie = 1; //details.NameAndBool(JKEY_COMPARETODIE, compareToDie);
  compareDie =  12; //details.NameAndValue(JKEY_DIESIDES, compareDie);

  details.NameAndValue("num_attempts_allowed", NbrTries);
  details.NameAndValue(JKEY_SUCCESSEVENT, successChain);
  {
    CString temp;
    details.NameAndValue(JKEY_success_action, temp);
    if (temp == "teleport") successAction = Teleport;
    if (temp == "no action") successAction = NoAction;
    if (temp == "chain event") successAction = ChainEvent;
  };
  details.NameAndValue("success_event", successChain);
  details.NameAndValue(JKEY_SUCCESSEVENT, successChain);
  if (successAction == Teleport)
  {
    successTransferData.Read(details, "teleport_dest_event_execute", "teleport_x", "teleport_y", "teleport_dest_facing");
  };
  {
    CString temp;
    details.NameAndValue(JKEY_fail_action, temp);
    if (temp == "teleport") failAction = Teleport;
    if (temp == "no action") failAction = NoAction;
    if (temp == "chain event") failAction = ChainEvent;
  };
  details.NameAndValue("fail_event", failChain);
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
  
  //successTransferData.Read(details, JKEY_SUCCESSTRANSFER);
  //failTransferData.Read(details, JKEY_FAILTRANSFER);
  return;
}
