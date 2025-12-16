#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <file.mp3>\n", argv[0]);
        return -1;
    }

    ma_result result;
    ma_engine engine;

    printf("Initializing audio engine...\n");
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.\n");
        return -1;
    }

    printf("Playing: %s\n", argv[1]);
    result = ma_engine_play_sound(&engine, argv[1], NULL);
    if (result != MA_SUCCESS) {
        printf("Failed to play sound. (Check if file exists)\n");
        return -1;
    }

    printf("Press Enter to quit...");
    getchar(); // Wait for user input to keep the program running while audio plays

    ma_engine_uninit(&engine);

    return 0;
}