#include "audio.h"
#include <windows.h>

void initAudio(void) {
}

void playRotateSound(void) {
    PlaySound(SOUND_FILE, NULL, SND_FILENAME | SND_ASYNC);
}