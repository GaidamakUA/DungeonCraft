#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int




void Event_Vault(JReader& event, JReader& details, JWriter& jw)
{
  VAULT_EVENT_DATA vault_event_data;
  vault_event_data.Read(event, details);
  vault_event_data.Export(jw);
}


void VAULT_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_FORCEBACKUP, ForceBackup);
  jw.NameAndValue(JKEY_VAULTID, WhichVault);
  jw.EndList();
}
 
void VAULT_EVENT_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, Vault);
  details.NameAndBool("force_backup_one_step", ForceBackup);
  //details.NameAndValue(JKEY_VAULTID, WhichVault);
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
  return;
}
