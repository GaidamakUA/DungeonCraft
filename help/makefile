CHE = c:\bin\cheetah9
HCW = c:\bin\HCW /C /E

# Convert .CHE files to .RTF files
# and create .CNT files
#
UAForever.hlp: UAForever.hpj \
               UAForever.cnt \
               UAF.rtf       \
               GPDL.rtf
    del UAForever.hlp
    ECHO abc > UAForever.hlp
    $(HCW)  UAForever.hpj

UAForever.cnt: root.cnt UAF.cnt GPDL.cnt
    copy root.cnt+UAF.cnt+GPDL.cnt UAForever.cnt


UAF.cnt UAF.rtf: UAF.che
    $(CHE) UAF.che
    pause

GPDL.cnt GPDL.rtf: GPDL.che
    $(CHE) GPDL.che
    pause


