#ifndef REGIONFREE_H
#define REGIONFREE_H

#include <3ds.h>

#include "menu.h"
#include "smdh.h"

#define REGIONFREE_PATH "regionfree:/"

extern bool regionFreeAvailable;
extern bool regionFreeGamecardIn;

extern menuEntry_s gamecardMenuEntry;

Result regionFreeInit();
Result regionFreeExit();
Result regionFreeRun();
Result regionFreeRun2(u32 pid_low, u32 pid_high, u8 mediatype, u8 flag);

void regionFreeUpdate();

Result loadGamecardIcon(smdh_s* out);

#endif
