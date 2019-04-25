#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

extern const char *QuestTypeText[];

class UTILITIES_EVENT_DATA : public GameEvent
{
public:
   void Export(JWriter& jw);
   void Read(JReader& event, JReader& jr);
  BOOL endPlay;
  MathOperationType operation;
  MultiItemCheckType itemCheck;  
  BYTE mathItemType;
  BYTE resultItemType;
  WORD mathAmount;
  int  mathItemIndex;  
  int  resultItemIndex;
  SPECIAL_OBJECT_EVENT_LIST items;
};



void Event_Utilities(JReader& event, JReader& details, JWriter& jw)
{
  UTILITIES_EVENT_DATA utilities_event_data;
  utilities_event_data.Read(event, details);
  utilities_event_data.Export(jw);
}


void UTILITIES_EVENT_DATA::Export(JWriter& jw)
{
// Jadefang says:
//For FRUA importation, a FRUA quest stage set at 128 
//should be turned into a DC quest stage set to 65001 
//but should trigger if the current stage of the quest 
//is 127. And a FRUA utilities event that sets a quest
//to 128 should probably be translated into a DC 65001
  GameEvent::Export(jw);
  CString type;
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_ENDPLAY, endPlay);
  jw.NameAndEnum(JKEY_OPERATION, operation);
  jw.NameAndEnum(JKEY_ITEMCHECK, itemCheck);
  jw.NameAndValue(JKEY_MATHAMOUNT, mathAmount);
  if (mathItemType == SpecialKeyType)
  {
    type = "Key";
  };
  if (mathItemType == SpecialItemType)
  {
    type = "Item";
  };
  if (mathItemType == QuestType)
  {
    type = "Quest";
  };
  jw.StartArray("mathItemType"); jw.Linefeed(false); 
  jw.NextEntry(); jw.QuotedString(type); 
  jw.EndArray(); jw.Linefeed(true);
  jw.NameAndValue(JKEY_MATHITEMINDEX, mathItemIndex);
  if (resultItemType == SpecialKeyType)
  {
    type = "Key";
  };
  if (resultItemType == SpecialItemType)
  {
    type = "Item";
  };
  if (resultItemType == QuestType)
  {
    type = "Quest";
  };
  jw.StartArray("resultItemType"); jw.Linefeed(false); 
  jw.NextEntry(); jw.QuotedString(type); 
  jw.EndArray(); jw.Linefeed(true);
  jw.NameAndValue(JKEY_RESULTITEMINDEX, resultItemIndex);
  items.Export(jw);
  jw.EndList();
}

/*
case 0: details["single_item_operation"] = NoneString; break;
   case 1: details["single_item_operation"] = "store in"; break;
   case 2: details["single_item_operation"] = "add to"; break;
   case 3: details["single_item_operation"] = "subtract from"; break;

   details["single_item_target"] = CStringOrEmptyString(GetSpecialItemKeyQuestText(mathTarget));
   details["single_item_operand"] = IntToString(mathValue);

   switch (multiItemCheck)
   {
   case 0: details["multi_item_check"] = NoneString; break;
   case 4: details["multi_item_check"] = "all present"; break;
   case 8: details["multi_item_check"] = "one or more present"; break;
   default: details["multi_item_check"] = UnknownString; break;
   }

*/

/*
            "particular":{
               "endPlay":"false",
               "operation":"Added To",
               "itemCheck":"All Items",
               "mathAmount":"3",
               "mathItemType":["Key"],
               "mathItemIndex":"1",
               "resultItemType":["Key"],
               "resultItemIndex":"8",
               "specialItems":[
                  "specialObject":{
                     "type":"",
                     "operation":"",
                     "index":"9",
                     "itemID":"1"
                  },
                  "specialObject":{
                     "type":"item",
                     "operation":"",
                     "index":"3",
                     "itemID":"1"
                  }
               ]
            }
*/

void UTILITIES_EVENT_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, Utilities);
  CString temp;
  details.NameAndValue("single_item_operation", temp);
  operation = MO_NoOperation;
  if (temp == "store in") operation = MO_StoredIn;
  if (temp == "add to") operation = MO_AddedTo;
  if (temp == "subtract from") operation = MO_SubtractFrom;
  details.NameAndBool("force_game_exit", endPlay);
  details.NameAndValue("multi_item_check", temp);
  details.NameAndValue("single_item_operand", mathAmount);
  if (temp == "none") itemCheck = MIC_None;
  if (temp == "all present") itemCheck = MIC_AllItems;
  if (temp == "one or more present") itemCheck = MIC_AtLeastOneItem;
  //details.NameAndValue(JKEY_MATHAMOUNT, mathAmount);
  //details.NameAndFlags(JKEY_MATHITEMTYPE, mathItemType, 3, QuestTypeText);
  //details.NameAndValue(JKEY_MATHITEMINDEX, mathItemIndex);
  //details.NameAndFlags(JKEY_RESULTITEMTYPE, resultItemType, 3, QuestTypeText);
  //details.NameAndValue(JKEY_RESULTITEMINDEX, resultItemIndex);
  {
    CString itemName;
    int col;
    details.NameAndValue("multi_item_target", itemName);
    if (itemName.Left(11) == "special key")
    {
      col = 12;
      resultItemType = SpecialKeyType;
    };
    if (itemName.Left(12) == "special item")
    {
      col = 13;
      resultItemType = SpecialItemType;
    };
    if (itemName.Left(13) == "special quest")
    {
      col = 14;
      resultItemType = QuestType;
    };
    sscanf(itemName.GetBuffer(0)+col, "%d", &resultItemIndex);
  };
  {
    CString itemName;
    int col;
    details.NameAndValue("single_item_target", itemName);
    if (itemName.Left(11) == "special key")
    {
      col = 12;
      mathItemType = SpecialKeyType;
    };
    if (itemName.Left(12) == "special item")
    {
      col = 13;
      mathItemType = SpecialItemType;
    };
    if (itemName.Left(13) == "special quest")
    {
      col = 14;
      mathItemType = QuestType;
    };
    sscanf(itemName.GetBuffer(0)+col, "%d", &mathItemIndex);
  };

  {
    int i;
    for (i=1; i<=6; i++)
    {
      CString key, value;
      SPECIAL_OBJECT_EVENT_DATA item;
      details.Optional();
      key.Format("multi_item_%d", i);
      value = Str(key);
      if (value.IsEmpty()) continue;
      {
        CString itemName;
        int col;
        details.NameAndValue(key, itemName);
        if (itemName.Left(11) == "special key")
        {
          col = 12;
          item.ItemType = KEY_FLAG;
        };
        if (itemName.Left(12) == "special item")
        {
          col = 13;
          item.ItemType = ITEM_FLAG;
        };
        if (itemName.Left(13) == "special quest")
        {
          col = 14;
          item.ItemType = QUEST_FLAG;
        };
        sscanf(itemName.GetBuffer(0)+col, "%d", &item.index);
      };
      items.list.Add(item);
    };
  }
  return;
}
