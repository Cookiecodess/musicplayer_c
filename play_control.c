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
    ma_sound sound; // We need a persistent sound object to control playback

    // 1. Initialize the engine
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.\n");
        return -1;
    }

    // 2. Load the sound file into the sound object
    // We use MA_SOUND_FLAG_DECODE to decode partially for better seeking/control, 
    // or stream from disk (default behavior without flags).
    result = ma_sound_init_from_file(&engine, argv[1], 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        printf("Failed to load sound file: %s\n", argv[1]);
        ma_engine_uninit(&engine);
        return -1;
    }

    printf("Playing: %s\n", argv[1]);
    printf("Controls:\n");
    printf("  [p] + Enter -> Play/Pause\n");
    printf("  [r] + Enter -> Rewind\n");
    printf("  [q] + Enter -> Quit\n");

    // 3. Start playback initially
    ma_sound_start(&sound);

    char input = 0;
    while (input != 'q') {
        // We use " %c" (space before %c) to skip any leftover whitespace/newlines
        // The space before %c is a special command that tells C: 
        // "Before you read a character, eat up and destroy any whitespace 
        // (spaces, tabs, or newlines) sitting in the buffer."
        scanf(" %c", &input);

        if (input == 'p') {
            if (ma_sound_is_playing(&sound)) {
                ma_sound_stop(&sound);
                printf("Paused.\n");
            } else {
                ma_sound_start(&sound);
                printf("Resumed.\n");
            }
        } 
        else if (input == 'r') {
            ma_sound_seek_to_pcm_frame(&sound, 0);
            printf("Rewound to start.\n");
        }
        else if (input == 'q') {
            printf("Quitting...\n");
        }
    }

    // 4. Cleanup
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);

    return 0;
}
