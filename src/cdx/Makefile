### Generated by Winemaker 0.8.4
###
### Invocation command line was
### /usr/local/bin/winemaker --nobackup -idll --mfc .


SRCDIR                = .
DLLS                  =
LIBS                  =
EXES                  = cdx \
			cdx_vs2005 \
			cdx_vs2008



### Common settings

CEXTRA                = -mno-cygwin
CXXEXTRA              =
RCEXTRA               =
DEFINES               =
INCLUDE_PATH          = -I. \
			$(MFC_INCLUDE_PATH)
DLL_PATH              =
DLL_IMPORTS           = dll
LIBRARY_PATH          =
LIBRARIES             =


### Extra source lists

EXTRA_C_SRCS          =
EXTRA_CXX_SRCS        = ddutils.cpp \
			dxerrors.cpp
EXTRA_RC_SRCS         =

EXTRA_OBJS            = $(EXTRA_C_SRCS:.c=.o) \
			$(EXTRA_CXX_SRCS:.cpp=.o)



### cdx sources and settings

cdx_MODULE            = cdx
cdx_C_SRCS            =
cdx_CXX_SRCS          = Cdxinput.cpp \
			Cdxmusic.cpp \
			Cdxtile.cpp \
			cdxdebug.cpp \
			cdxfile.cpp \
			cdximage.cpp \
			cdximagebase.cpp \
			cdximagebmp.cpp \
			cdximagejpg.cpp \
			cdximagepcx.cpp \
			cdximagepng.cpp \
			cdximagepsd.cpp \
			cdximagetga.cpp \
			cdxiso.cpp \
			cdxlayer.cpp \
			cdxlink.cpp \
			cdxlog.cpp \
			cdxmap.cpp \
			cdxmapcell.cpp \
			cdxmidi.cpp \
			cdxmusiccd.cpp \
			cdxpacker.cpp \
			cdxresource.cpp \
			cdxscreen.cpp \
			cdxsound.cpp \
			cdxsoundbuffer.cpp \
			cdxsprite.cpp \
			cdxspritelist.cpp \
			cdxsurface.cpp \
			cdxtemplate.cpp
cdx_RC_SRCS           =
cdx_LDFLAGS           = -mwindows \
			-mno-cygwin
cdx_ARFLAGS           =
cdx_DLL_PATH          = $(MFC_LIBRARY_PATH)
cdx_DLLS              = odbc32 \
			ole32 \
			oleaut32 \
			winspool \
			odbccp32 \
			mfc.dll
cdx_LIBRARY_PATH      = $(MFC_LIBRARY_PATH)
cdx_LIBRARIES         = uuid \
			mfc

cdx_OBJS              = $(cdx_C_SRCS:.c=.o) \
			$(cdx_CXX_SRCS:.cpp=.o) \
			$(cdx_RC_SRCS:.rc=.res)



### cdx_vs2005 sources and settings

cdx_vs2005_MODULE     = cdx_vs2005
cdx_vs2005_C_SRCS     =
cdx_vs2005_CXX_SRCS   =
cdx_vs2005_RC_SRCS    =
cdx_vs2005_LDFLAGS    = -mwindows \
			-mno-cygwin
cdx_vs2005_ARFLAGS    =
cdx_vs2005_DLL_PATH   = $(MFC_LIBRARY_PATH)
cdx_vs2005_DLLS       = odbc32 \
			ole32 \
			oleaut32 \
			winspool \
			odbccp32 \
			mfc.dll
cdx_vs2005_LIBRARY_PATH= $(MFC_LIBRARY_PATH)
cdx_vs2005_LIBRARIES  = uuid \
			mfc

cdx_vs2005_OBJS       = $(cdx_vs2005_C_SRCS:.c=.o) \
			$(cdx_vs2005_CXX_SRCS:.cpp=.o) \
			$(cdx_vs2005_RC_SRCS:.rc=.res)



### cdx_vs2008 sources and settings

cdx_vs2008_MODULE     = cdx_vs2008
cdx_vs2008_C_SRCS     =
cdx_vs2008_CXX_SRCS   =
cdx_vs2008_RC_SRCS    =
cdx_vs2008_LDFLAGS    = -mwindows \
			-mno-cygwin
cdx_vs2008_ARFLAGS    =
cdx_vs2008_DLL_PATH   = $(MFC_LIBRARY_PATH)
cdx_vs2008_DLLS       = odbc32 \
			ole32 \
			oleaut32 \
			winspool \
			odbccp32 \
			mfc.dll
cdx_vs2008_LIBRARY_PATH= $(MFC_LIBRARY_PATH)
cdx_vs2008_LIBRARIES  = uuid \
			mfc

cdx_vs2008_OBJS       = $(cdx_vs2008_C_SRCS:.c=.o) \
			$(cdx_vs2008_CXX_SRCS:.cpp=.o) \
			$(cdx_vs2008_RC_SRCS:.rc=.res)



### Global source lists

C_SRCS                = $(cdx_C_SRCS) \
			$(cdx_vs2005_C_SRCS) \
			$(cdx_vs2008_C_SRCS) \
			$(EXTRA_C_SRCS)
CXX_SRCS              = $(cdx_CXX_SRCS) \
			$(cdx_vs2005_CXX_SRCS) \
			$(cdx_vs2008_CXX_SRCS) \
			$(EXTRA_CXX_SRCS)
RC_SRCS               = $(cdx_RC_SRCS) \
			$(cdx_vs2005_RC_SRCS) \
			$(cdx_vs2008_RC_SRCS) \
			$(EXTRA_RC_SRCS)


### Tools

CC = winegcc
CXX = wineg++
RC = wrc
AR = ar


### Generic targets

all: $(DLLS:%=%.so) $(LIBS) $(EXES)

### Build rules

.PHONY: all clean dummy

$(SUBDIRS): dummy
	@cd $@ && $(MAKE)

# Implicit rules

.SUFFIXES: .cpp .cxx .rc .res
DEFINCL = $(INCLUDE_PATH) $(DEFINES) $(OPTIONS)

.c.o:
	$(CC) -c $(CFLAGS) $(CEXTRA) $(DEFINCL) -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.rc.res:
	$(RC) $(RCFLAGS) $(RCEXTRA) $(DEFINCL) -fo$@ $<

# Rules for cleaning

CLEAN_FILES     = y.tab.c y.tab.h lex.yy.c core *.orig *.rej \
                  \\\#*\\\# *~ *% .\\\#*

clean:: $(SUBDIRS:%=%/__clean__) $(EXTRASUBDIRS:%=%/__clean__)
	$(RM) $(CLEAN_FILES) $(RC_SRCS:.rc=.res) $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)
	$(RM) $(DLLS:%=%.so) $(LIBS) $(EXES) $(EXES:%=%.so)

$(SUBDIRS:%=%/__clean__): dummy
	cd `dirname $@` && $(MAKE) clean

$(EXTRASUBDIRS:%=%/__clean__): dummy
	-cd `dirname $@` && $(RM) $(CLEAN_FILES)

### Target specific build rules
DEFLIB = $(LIBRARY_PATH) $(LIBRARIES) $(DLL_PATH) $(DLL_IMPORTS:%=-l%)

$(cdx_MODULE): $(cdx_OBJS)
	$(CXX) $(cdx_LDFLAGS) -o $@ $(cdx_OBJS) $(cdx_LIBRARY_PATH) $(cdx_DLL_PATH) $(DEFLIB) $(cdx_DLLS:%=-l%) $(cdx_LIBRARIES:%=-l%)


$(cdx_vs2005_MODULE): $(cdx_vs2005_OBJS)
	$(CXX) $(cdx_vs2005_LDFLAGS) -o $@ $(cdx_vs2005_OBJS) $(cdx_vs2005_LIBRARY_PATH) $(cdx_vs2005_DLL_PATH) $(DEFLIB) $(cdx_vs2005_DLLS:%=-l%) $(cdx_vs2005_LIBRARIES:%=-l%)


$(cdx_vs2008_MODULE): $(cdx_vs2008_OBJS)
	$(CXX) $(cdx_vs2008_LDFLAGS) -o $@ $(cdx_vs2008_OBJS) $(cdx_vs2008_LIBRARY_PATH) $(cdx_vs2008_DLL_PATH) $(DEFLIB) $(cdx_vs2008_DLLS:%=-l%) $(cdx_vs2008_LIBRARIES:%=-l%)


