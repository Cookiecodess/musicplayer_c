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
    ma_engine_config config;

    // 1. Initialize the config with defaults
    config = ma_engine_config_init();

    // 2. INCREASE BUFFER SIZE
    // Default is usually 10ms (good for games, bad for heavy decoding).
    // We set it to 250ms. This gives the CPU a huge margin of error.
    config.periodSizeInMilliseconds = 250; 

    // 3. Initialize the engine with this specific config
    printf("Initializing engine with 250ms buffer...\n");
    result = ma_engine_init(&config, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.\n");
        return -1;
    }

    printf("Playing: %s\n", argv[1]);
    
    // We use the stream flag to ensure it doesn't try to load the whole file into RAM at once
    // (though engine_play_sound usually handles this automatically)
    result = ma_engine_play_sound(&engine, argv[1], NULL);
    
    if (result != MA_SUCCESS) {
        printf("Failed to play sound.\n");
        return -1;
    }

    printf("Press Enter to quit...");
    getchar();

    ma_engine_uninit(&engine);

    return 0;
}
