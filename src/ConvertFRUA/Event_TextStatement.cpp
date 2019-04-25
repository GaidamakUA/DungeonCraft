#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int
#define Bol details.Fetch_Bool

class TEXT_EVENT_DATA : public GameEvent
{
public:
  TEXT_EVENT_DATA(void){WaitForReturn=HighlightText=ForceBackup=KillSound=FALSE;};
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);
  BOOL WaitForReturn;
  BOOL HighlightText;
  BOOL ForceBackup;
  BOOL KillSound;
  CString sound;
  int hSound;
  eventDistType distance;  
};




void Event_TextStatement(JReader& event, JReader& details, JWriter& jw)
{
  TEXT_EVENT_DATA text_event_data;
  text_event_data.Read(event, details);
  text_event_data.Export(jw);
}

void TEXT_EVENT_DATA::Export(JWriter& jw)
{
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  jw.NameAndBool(JKEY_WAITFORRETURN, WaitForReturn);
  jw.NameAndBool(JKEY_FORCEBACKUP, ForceBackup);
  jw.NameAndBool(JKEY_HIGHLIGHTTEXT, HighlightText);
  jw.NameAndEnum(JKEY_DISTANCE, distance);
  jw.NameAndNonBlank(JKEY_SOUND, sound);
  jw.EndList();
}

void TEXT_EVENT_DATA::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, TextStatement);
  {
    bool highlight = false;
    TEXT text1, text2, text3, text4, text5;
    text1.text = details.Fetch_String("message_1_text");
    text2.text = details.Fetch_String("message_2_text");
    text3.text = details.Fetch_String("message_3_text");
    text4.text = details.Fetch_String("message_4_text");
    text5.text = details.Fetch_String("message_5_text");
    text1.needReturn = details.Fetch_Bool("message_1_hit_return");
    text2.needReturn = details.Fetch_Bool("message_2_hit_return");
    text3.needReturn = details.Fetch_Bool("message_3_hit_return");
    text4.needReturn = details.Fetch_Bool("message_4_hit_return");
    text5.needReturn = details.Fetch_Bool("message_5_hit_return");
    text1.highlight = details.Fetch_Bool("message_1_highlight");
    text2.highlight = details.Fetch_Bool("message_2_highlight");
    text3.highlight = details.Fetch_Bool("message_3_highlight");
    text4.highlight = details.Fetch_Bool("message_4_highlight");
    text5.highlight = details.Fetch_Bool("message_5_highlight");

    if (!text1.text.IsEmpty())
    {
      if (text1.highlight ^ highlight)
      {
        if (text1.highlight)
        {
          text += "/b";
        }
        else
        {
          text += "/w";
        };
      };
      highlight = text1.highlight;
      text += text1.text;
      if (text1.needReturn)
      {
        text += "/n";
      };
    };
    if (!text2.text.IsEmpty())
    {
      if (text2.highlight ^ highlight)
      {
        if (text2.highlight)
        {
          text += "/b";
        }
        else
        {
          text += "/w";
        };
      };
      highlight = text2.highlight;
      text += text2.text;
      if (text2.needReturn)
      {
        text += "/n";
      };
    };
    if (!text3.text.IsEmpty())
    {
      if (text3.highlight ^ highlight)
      {
        if (text3.highlight)
        {
          text += "/b";
        }
        else
        {
          text += "/w";
        };
      };
      highlight = text3.highlight;
      text += text3.text;
      if (text3.needReturn)
      {
        text += "/n";
      };
    };
    if (!text4.text.IsEmpty())
    {
      if (text4.highlight ^ highlight)
      {
        if (text4.highlight)
        {
          text += "/b";
        }
        else
        {
          text += "/w";
        };
      };
      highlight = text4.highlight;
      text += text4.text;
      if (text4.needReturn)
      {
        text += "/n";
      };
    };
    if (!text5.text.IsEmpty())
    {
      if (text5.highlight ^ highlight)
      {
        if (text5.highlight)
        {
          text += "/b";
        }
        else
        {
          text += "/w";
        };
      };
      highlight = text5.highlight;
      text += text5.text;
      if (text5.needReturn)
      {
        text += "/n";
      };
    };

  }
  {
    CString dist;
    pic.Clear();
    details.NameAndBool("force_backup_one_step", ForceBackup);
    dist = details.Fetch_String("picture_distance");
    pic.ReadSmallPicFile(details, "picture");
    if (dist == "close") distance = UpClose;
    else if (dist == "nearby") distance = Nearby;
    else if (dist == "FarAway") distance = FarAway;
    else if (dist == "far away") distance = FarAway;
    else details.PrintError("Cannot determine Picture distance");
  };
  return;
}
