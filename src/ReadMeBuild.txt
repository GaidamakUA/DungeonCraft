You will need to install the DirectX 8 SDK before compiling DungeonCraft. The compiler settings must then be updated to point to the DX8 include and lib folders. This project links to ddraw.lib so in theory any version of DxSDK that is version 8+ which also includes this lib file might work.

VS2008 - Note that the DX8 include/lib folders should be arranged last in the VS2008 directories configuration, else you might get this error

	error C2146: syntax error : missing ';' before identifier 'PVOID64'
	
	which is due to one of the dependencies including Winnt.h before including Windows.h
	
	
VS2013 - The minimum supported operating system is WindowsXP so the WINVER=0x0501 instead of WINVER=0x0500
	