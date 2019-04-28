#include "StdAfx.h"
#include "Translate.h"
#include "Events.h"

#define Str details.Fetch_String
#define Int details.Fetch_Int

class GUIDED_TOUR : public GameEvent
{
public:
  void Export(JWriter& jw);
  void Read(JReader& event, JReader& jr);

  int tour_x,tour_y;  // start location
  int facing; // start facing
  BOOL executeEvent;
  BOOL useStartLoc;
  int currStep;
  TOUR_STEP steps[MAX_TOUR_STEPS];
};



void Event_GuidedTour(JReader& event, JReader& details, JWriter& jw)
{
  GUIDED_TOUR guided_tour;
  guided_tour.Read(event,details);
  guided_tour.Export(jw);
}

void GUIDED_TOUR::Export(JWriter& jw)
{
  int i[2];
  GameEvent::Export(jw);
  jw.StartList(JKEY_PARTICULAR);
  i[0] = tour_x;
  i[1] = tour_y;
  jw.NameAndValue(JKEY_XY, i, 2);
  jw.NameAndValue(JKEY_FACING, facing);
  jw.NameAndBool(JKEY_USESTARTLOCATION, useStartLoc);   
  jw.NameAndBool(JKEY_EXECUTEEVENT, executeEvent);

  jw.StartArray(JKEY_STEPS);
  for (int j=0;j<MAX_TOUR_STEPS;j++)
  {
    steps[j].Export(jw, j);
  };
  jw.EndArray();
  jw.EndList();
}

void GUIDED_TOUR::Read(JReader& event, JReader& details)
{
  GameEvent::Read(event, GuidedTour);
  details.NameAndValue ("start_facing", facing);
  details.NameAndBool  ("start_loc_use", useStartLoc);   
  details.NameAndBool  ("dest_event_execute", executeEvent);
  details.NameAndValue ("start_loc_x", tour_x);
  details.NameAndValue ("start_loc_y", tour_y);

  {
    int i;
    for (i = 1; i<=MAX_TOUR_STEPS; i++)
    {
      CString key, value;
      int stepNum;     
      key.Format("step_%02d_action", i);
      value = Str(key);
      if (value == "none")   steps[i-1].step = TStep_NoMove;
      if (value == "Pause")  steps[i-1].step = TStep_Pause;
      if (value == "Left")   steps[i-1].step = TStep_Left;
      if (value == "Right")  steps[i-1].step = TStep_Right;
      if (value == "Forward")steps[i-1].step = TStep_Forward;
      key.Format("text_%d_displayed_before_step", i);
      details.Optional();
      if (details.NameAndValue(key, stepNum))
      {
        CString text;
        key.Format("text_%d", i);
        details.NameAndValue(key, text);
        steps[stepNum-1].text = text;
      };
    };
  };
  return;
}
