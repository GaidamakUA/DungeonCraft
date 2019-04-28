
#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int



void TRAINABLE_BASECLASS::Export(JWriter& jw) const
{
  jw.StartList();
  jw.Linefeed(false);
  jw.NameAndValue(JKEY_ID, baseclassID);
  jw.NameAndValue(JKEY_MINLEVEL, minLevel);
  jw.NameAndValue(JKEY_MAXLEVEL, maxLevel);
  jw.NameAndValue(JKEY_WHATEVER, whateverthosenutsmightwant);
  jw.EndList();
  jw.Linefeed(true);
}




void Event_TrainingHall(JReader& event, JReader& details, JWriter& jw)
{
  TRAININGHALL traininghall;
  traininghall.Read(event, details);
  traininghall.Export(jw);
}



void TRAININGHALL::Export(JWriter& jw)
{
  int i;
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
  jw.NameAndValue(JKEY_COST, Cost);
  jw.StartArray(JKEY_BASECLASSES);
  for (i=0; i<trainableBaseclasses.GetSize(); i++)
  {
    const TRAINABLE_BASECLASS *pBaseclass;
    pBaseclass = &trainableBaseclasses[i];
    jw.NextEntry();
    pBaseclass->Export(jw);
  };
  jw.EndArray();
  jw.EndList();
}

bool TRAININGHALL::Read(JReader& event, JReader& details)
{
  BOOL trainable;
  TRAINABLE_BASECLASS tb;
  tb.minLevel = 0;
  tb.maxLevel = 999;

  GameEvent::Read(event, TrainingHallEvent);
  trainableBaseclasses.RemoveAll();
  details.NameAndBool("force_backup_one_step", forceExit);
  details.NameAndValue("cost", Cost);
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
  if (details.NameAndBool("train_cleric", trainable)) 
  {
    if (trainable)
    {
      tb.baseclassID = "cleric";
      trainableBaseclasses.Add(tb); 
    };
  };
  if (details.NameAndBool("train_fighter", trainable)) 
  {
    if (trainable)
    {
      tb.baseclassID = "fighter";
      trainableBaseclasses.Add(tb);; 
    };
  };
  if (details.NameAndBool("train_mage", trainable)) 
  {
    if (trainable)
    {
      tb.baseclassID = "mage";
      trainableBaseclasses.Add(tb);; 
    };
  };
  if (details.NameAndBool("train_paladin", trainable)) 
  {
    if (trainable)
    {
      tb.baseclassID = "paladin";
      trainableBaseclasses.Add(tb);; 
    };
  };
  if (details.NameAndBool("train_ranger", trainable)) 
  {
    if (trainable)
    {
      tb.baseclassID = "ranger";
      trainableBaseclasses.Add(tb);; 
    };
  };
  if (details.NameAndBool("train_thief", trainable)) 
  {
    if (trainable)
    {
      tb.baseclassID = "thief";
      trainableBaseclasses.Add(tb);; 
    };
  };
  return true;
}
