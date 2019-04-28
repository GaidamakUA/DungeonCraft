/********************************************************************
*
*   Author:			Robert Karlsson
*
*   Module:			Movie.cpp
*
*   $Workfile: $
*   $Revision: 1.5 $
*   $Date: 2014/03/09 17:10:31 $
*
*   Description:		Implementation of the Movie class.
*
*   Usage:				>>!! DONT FORGET TO LINK WITH amstrmid.lib !!<<
*							
*                   1. Create an instance of the class
*							2. Invoke Create() with filename of movie and with pointer to CDXSurface
*								that the movie will render to. It will also scale the movie to fit the
*								size of the CDXSurface. Remember to create CDXSurface first!
*							3. Invoke Play()
*							4. In your main game loop, call Update() and then draw the surface to the screen.
*							5. You can stop the movie by calling Stop(), or wait until Update() returns false(movie done.)
*							6. Invoke Destroy()
*							7. Delete instance.
*
*   Dependencies:	
*
********************************************************************/

/*** Include files **************************************************/
#include "../Shared/stdafx.h"

#include "cdx.h"
#include "Movie.h"
#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void WriteDebugString(const char *ptext, ... );
extern int debugSeverity;
extern int ExitSignaled;


/*** Defines ********************************************************/

/*** Macros *********************************************************/

/*** Data types definitions and typedefs ****************************/

/*** Global variables ***********************************************/

/********************************************************************/
/*** Class constructor/destructor ***********************************/
/********************************************************************/
/*********************************************************************
*
*   Purpose:			
*
*   Inparameters:	
*
*   Outparameters:	
*
*   Return value:
*
*   Note:
*
*/
Movie::Movie(CDXScreen* pScreen)
{
  m_pScreen = pScreen;
  m_pSurface=NULL;
  m_pMMStream=NULL;
  m_pPrimaryVidStream=NULL;
  m_pDDStream=NULL;
  m_pDDSurface=NULL;
  m_pDDStreamSample=NULL;
  memset(&m_rectClipping,0,sizeof(m_rectClipping));
  memset(&m_srcRect, 0, sizeof(m_srcRect));
}

/*********************************************************************
*
*   Purpose:			
*
*   Inparameters:	
*
*   Outparameters:	
*
*   Return value:
*
*   Note:
*
*/
Movie::~Movie()
{
  Stop();
  Destroy();
}

/********************************************************************/
/*** Public Member Functions ****************************************/
/********************************************************************/
/*********************************************************************
*
*   Purpose:			
*
*   Inparameters:	szFilename			Filename of the movie.
*							pSurface				Surface to render movie on.
*
*   Outparameters:	
*
*   Return value:	bool					true or false
*
*   Note:
*
*/
bool Movie::Create(const char *szFilename, CDXSurface *pSurface)
{
  if (pSurface==NULL) return false;
  if (strlen(szFilename)==0) return false;

  //CoInitialize( NULL );		// Initialize COM
  m_pSurface = pSurface;
  
  if (!OpenMMStream(szFilename))
    return false;

  PAVIFILE aviFile=NULL;

  if (AVIFileOpen(&aviFile, szFilename, OF_SHARE_DENY_NONE, NULL)==0)
  {
    AVIFILEINFO aviInfo;
    memset(&aviInfo,0,sizeof(aviInfo));    
    if (AVIFileInfo(aviFile, &aviInfo, sizeof(aviInfo))==0)
    {
      m_srcRect.left=m_srcRect.top=0;
      m_srcRect.right = aviInfo.dwWidth;
      m_srcRect.bottom = aviInfo.dwHeight;
      WriteDebugString("Src movie size: %u width, %u height\n", 
                        m_srcRect.right,m_srcRect.bottom);
    }
  }
  return true;
}

/*********************************************************************
*
*   Purpose:			
*
*   Inparameters:	
*
*   Outparameters:	
*
*   Return value:
*
*   Note:
*
*/
bool Movie::Play()
{
  if (m_pMMStream==NULL) return false;

  HRESULT rval;
  
  rval = m_pMMStream->GetMediaStream(MSPID_PrimaryVideo, &m_pPrimaryVidStream);
  if(FAILED(rval)) { WriteDebugString("Failed GetMediaStream() in movie playback\n"); return false; }
  
  rval = m_pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&m_pDDStream);
  if(FAILED(rval)) { WriteDebugString("Failed QueryInterface() in movie playback\n"); return false; }
  
  rval = m_pDDStream->CreateSample(NULL, NULL, 0, &m_pDDStreamSample);
  if(FAILED(rval)) { WriteDebugString("Failed CreateSample() in movie playback\n"); return false; }
  
  rval = m_pDDStreamSample->GetSurface(&m_pDDSurface, &m_rectClipping);
  if(FAILED(rval)) { WriteDebugString("Failed GetSurface() in movie playback\n"); return false; }

  rval = m_pMMStream->SetState(STREAMSTATE_RUN);
  if(FAILED(rval)) { WriteDebugString("Failed SetState(run) in movie playback\n"); return false; }
  
  return true;
}

/*********************************************************************
*
*   Purpose:			Actual drawing to the surface must be done somewhere, right? :-)
*
*   Inparameters:	
*
*   Outparameters:	
*
*   Return value:	bool				true or false. true if movie done, else false.
*
*/
bool Movie::Update(RECT *pRect)
{
  if (m_pDDStreamSample==NULL) return false;
  if (m_pSurface==NULL) return false;

  HRESULT rval;
  
  if (m_pDDStreamSample->Update(0, NULL, NULL, 0) != S_OK)
  {
    WriteDebugString("Failed to update movie\n");
    return false;
  }

  rval = m_pSurface->GetDDS()->Blt(pRect, // Destination rect, NULL = whole surface
                                   (CDX_LPDIRECTDRAWSURFACE) m_pDDSurface,
                                   &m_rectClipping,
                                   DDBLT_WAIT,
                                   NULL);

  if(FAILED(rval)) { WriteDebugString("Failed blit in update movie\n"); return false; }    
  return true;
}

/*********************************************************************
*
*   Purpose:			Clean up
*
*   Inparameters:	
*
*   Outparameters:	
*
*   Return value:
*
*   Note:
*
*/
bool Movie::Destroy()
{
  RELEASE(m_pMMStream);
  RELEASE(m_pPrimaryVidStream);
  RELEASE(m_pDDStream);
  RELEASE(m_pDDSurface);
  RELEASE(m_pDDStreamSample);
  
  m_pMMStream=NULL;
  m_pPrimaryVidStream=NULL;
  m_pDDStream=NULL;
  m_pDDSurface=NULL;
  m_pDDStreamSample=NULL;

  //CoUninitialize();
  
  return true;
}


/*********************************************************************
*
*   Purpose:			Guess!
*
*   Inparameters:	
*
*   Outparameters:	
*
*   Return value:
*
*   Note:
*
*/
bool Movie::Stop()
{
  if (m_pMMStream==NULL) return false;
  HRESULT rval;  
  rval = m_pMMStream->SetState(STREAMSTATE_STOP);  
  if(FAILED(rval)) { WriteDebugString("Failed setting movie stop state\n"); return false; }  
  return true;  
}

/********************************************************************/
/*** Protected Member Functions *************************************/
/********************************************************************/

/********************************************************************/
/*** Private Member Functions ***************************************/
/********************************************************************/
/*********************************************************************
*
*   Purpose:			
*
*   Inparameters:	
*
*   Outparameters:	
*
*   Return value:
*
*   Note:
*
*/
bool Movie::OpenMMStream(const char* szFilename)
{
  if (m_pScreen==NULL) return false;

  HRESULT					rval;
  IAMMultiMediaStream*	pAMStream;
  WCHAR	   				wPath[MAX_PATH+1];  
  
  // Create COM object
  rval = CoCreateInstance(CLSID_AMMultiMediaStream,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IAMMultiMediaStream,
                          (void **)&pAMStream
                          );
  
  if(FAILED(rval)) { WriteDebugString("Failed CoCreateInstance() for Movie playback\n"); return false; }
  m_pMMStream = pAMStream;

  rval = pAMStream->Initialize(STREAMTYPE_READ, /*0*/AMMSF_NOGRAPHTHREAD, NULL);
  if(FAILED(rval)) { WriteDebugString("Failed Initialize for Movie playback\n"); return false; }
  
  rval = pAMStream->AddMediaStream(m_pScreen->GetDD(), &MSPID_PrimaryVideo, 0, NULL);
  if(FAILED(rval)) { WriteDebugString("Failed AddMediaStream(video) for Movie playback\n"); return false; }
  
  rval = pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL);
  if(FAILED(rval)) { WriteDebugString("Failed AddMediaStream(audio) for Movie playback\n"); /*return false;*/ }
  
  if(0 == MultiByteToWideChar(CP_ACP, 0, szFilename, -1, wPath, sizeof(wPath)/sizeof(wPath[0])))
  {
    WriteDebugString("Failed MultiByteToWideChar() for Movie playback: %s\n", szFilename);
    return false;
  }
  
  rval = pAMStream->OpenFile(wPath, 0);
  if(FAILED(rval)) { WriteDebugString("Failed OpenFile(\'%s\') in Movie playback\n",szFilename); return false; }
    
  // These two lines are unnecessary? Looks like that to me
  //pAMStream->AddRef();
  //RELEASE(pAMStream);  
  return true;
}

/* EOF */





