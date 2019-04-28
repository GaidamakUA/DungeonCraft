#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int



class SPECIAL_ITEM_KEY_EVENT_DATA : public GameEvent
{
public:
//  DECLARE_SERIAL( SPECIAL_ITEM_KEY_EVENT_DATA )
  void Clear();
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& details);

  BOOL WaitForReturn;
  BOOL forceExit;
  SPECIAL_OBJECT_EVENT_LIST items;
};




void Event_SpecialItems(JReader& event, JReader& details, JWriter& jw)
{
  SPECIAL_ITEM_KEY_EVENT_DATA special_item_key_event_data;
  special_item_key_event_data.Read(event, details);
  special_item_key_event_data.Export(jw);
};

void SPECIAL_ITEM_KEY_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  {
    jw.NameAndBool(JKEY_WAITFORRETURN, WaitForReturn);
    jw.NameAndBool(JKEY_FORCEEXIT, forceExit);
    items.Export(jw);
  };
  jw.EndList();
}



void SPECIAL_ITEM_KEY_EVENT_DATA::Read(JReader& event, JReader& details)
{
  SPECIAL_OBJECT_EVENT_DATA itemData;
  CString itemName;
  CString actionName;
  GameEvent::Read(event, SpecialItem);
  //jr.NameAndBool(JKEY_WAITFORRETURN, WaitForReturn);
  WaitForReturn = FALSE;
  details.NameAndValue("special_item_key_quest", itemName);
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
  details.NameAndValue("action", actionName);
  if (actionName == "take from party")
  {
    itemData.operation = SPECIAL_OBJECT_TAKE;
  }
  else
  {
    itemData.operation = SPECIAL_OBJECT_GIVE;
  };
  if (itemName.Left(12) == "special item")
  {
    itemData.ItemType = ITEM_FLAG;
    itemData.index = atoi(itemName.Right(itemName.GetLength()-13));
  }
  else if (itemName.Left(11) == "special key")
  {
    itemData.ItemType = KEY_FLAG;
    itemData.index = atoi(itemName.Right(itemName.GetLength()-12));
  }
  else if (itemName.Left(13) == "special quest")
  {
    itemData.ItemType = 0;
    itemData.index = atoi(itemName.Right(itemName.GetLength()-14));
  }
  else
  {
    details.PrintError("cannot determine special item type");
    itemData.index = -1;
  };
  itemData.id = 1;
  items.list.Add(itemData);

  //jr.NameAndBool(JKEY_FORCEEXIT, forceExit);
  forceExit = FALSE;
  return;
}


