#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>		// access
#include <sys/stat.h>

#include "ndsheaderbanner.h"
#include "ds_system.h"

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

	iprintf("Opening NDS file...\n");

	sNDSHeaderExt NDSHeader;

	FILE *f_nds_file = fopen("romfs:/hello_world.nds", "rb");
	if (f_nds_file) {
		iprintf("Loading NDS file...\n");
		fread(&NDSHeader, 1, sizeof(NDSHeader), f_nds_file);

		// Load header and ARM binaries to emulated main RAM
		memcpy(dsMainRam+0x3FFE00, &NDSHeader, 0x160); // 0x023FFE00

		const bool arm9dstValid = (NDSHeader.arm9destination >= 0x02000000 && NDSHeader.arm9destination < 0x02400000);
		const bool arm7dstValid = (NDSHeader.arm7destination >= 0x02000000 && NDSHeader.arm7destination < 0x02400000);

		if (arm9dstValid) {
			fseek(f_nds_file, NDSHeader.arm9romOffset, SEEK_SET);
			fread(dsMainRam + (NDSHeader.arm9destination - 0x02000000), 1, NDSHeader.arm9binarySize, f_nds_file);
		} else {
			iprintf("ARM9 destination is not valid!\n");
		}
		if (arm7dstValid) {
			fseek(f_nds_file, NDSHeader.arm7romOffset, SEEK_SET);
			fread(dsMainRam + (NDSHeader.arm7destination - 0x02000000), 1, NDSHeader.arm7binarySize, f_nds_file);
		} else {
			iprintf("ARM7 destination is not valid!\n");
		}

		fclose(f_nds_file);
	} else {
		iprintf("Not found!\n");
	}

	iprintf("\nTo be developed...\n");
	while (aptMainLoop() && !(hidKeysHeld() & KEY_B)) {
		hidScanInput();
		gspWaitForVBlank();
	}

	gfxExit();
	romfsExit();
	amExit();

	return 0;
}