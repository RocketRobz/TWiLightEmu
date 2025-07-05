#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>		// access
#include <sys/stat.h>

u8 sysRegion = CFG_REGION_USA;
u64 appID = 0;

int main()
{
	amInit();
	romfsInit();

	u8 consoleModel = 0;

	Result res = cfguInit();
	if (R_SUCCEEDED(res)) {
		CFGU_SecureInfoGetRegion(&sysRegion);
		CFGU_GetSystemModel(&consoleModel);
		cfguExit();
	}

	s64 isCitra = 0;
	svcGetSystemInfo(&isCitra, 0x20000, 0); // Check if running on Citra

	aptInit();
	APT_GetProgramID(&appID);
	aptExit();

	gfxInit(GSP_RGBA8_OES,GSP_RGBA8_OES,false);
	consoleInit(GFX_BOTTOM, NULL);

	iprintf("To be developed...");
	while (aptMainLoop() && !(hidKeysHeld() & KEY_B)) {
		hidScanInput();
		gspWaitForVBlank();
	}

	gfxExit();
	romfsExit();
	amExit();

	return 0;
}