
#include "stdafx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class SOUND_EVENT : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);

  CList<CString, CString&> sounds;
};





void Event_Sound(JReader& event, JReader& details, JWriter& jw)
{
  SOUND_EVENT sound_event;
  sound_event.Read(event, details);
  sound_event.Export(jw);
}

void SOUND_EVENT::Export(JWriter& jw)
{
  CString temp;
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  POSITION pos = sounds.GetHeadPosition();
  jw.StartArray(JKEY_SOUNDS);
  while (pos != NULL)
  {
    temp = sounds.GetAt(pos);
    jw.NextEntry();
    jw.NameAndValue(NULL, temp);
    sounds.GetNext(pos);
  }
  jw.EndArray();
  jw.EndList();
}

void SOUND_EVENT::Read(JReader& event, JReader& details)
{
  int i;
  GameEvent::Read(event, Sounds);
  for (i=1; i<=10; i++)
  {
    CString key, value;
    key.Format("sound_%02d", i);
    if (details.NameAndValue(key, value))
    {
      sounds.AddTail(value);
    };
  };
  return;
}
