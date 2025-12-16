# Music player in C

## Compiling `play.c`

Use:

```
gcc play.c -o play -ldl -lpthread -lm
```

### What does the flags mean?

1. `-ldl` (Dynamic Loading Library)

    Link Dynamic Loader.

    Purpose: This library provides functions (like dlopen, dlsym, and dlclose) that allow a program to load and use shared libraries (like .so files on Linux or .dll files on Windows) at runtime.

    Why it's needed here: The miniaudio library often uses this to dynamically access necessary audio drivers on different Linux systems, ensuring it can connect to various audio servers (ALSA, PulseAudio, etc.) without being hard-coded to one.

2. `-lpthread` (POSIX Threads Library)

    Link POSIX Threads.

    Purpose: This library provides the standard C API for multithreading. Threads allow a program to do multiple things simultaneously (e.g., one thread handles the audio playback in the background while the main thread waits for you to press "Enter").

    Why it's needed here: Audio playback must be done on a separate thread (a background process) so that your main program can continue running and waiting for input. Without this, your terminal would freeze until the song ended.

3. `-lm` (Math Library)

    Link Math.

    Purpose: This library contains implementations for advanced mathematical functions (like sin(),cos(),log(),pow(), and sqrt()).

    Why it's needed here: While you didn't call any math functions directly, sound processing often involves complex mathematics (e.g., calculating sampling rates, filtering, or handling floating-point audio data). MP3 decoding, in particular, relies on functions found in the math library.

## Audio decoding speed, latency, stability

`play` (compiled with Level-1 optimization), `play_O3` (level-3 optimization) and `play_control` (level-1 optimization) have audio stuttering issues about a minute into playing the song. `play_O3`, being much more optimized, fares much better than the other two here, but the song still ends up stuttering.

Gemini suggested changing the `config.periodSizeInMilliseconds` to 250 to increase the audio buffer size.

That's `play_stable_O3` (also optimized with level-3 for good measure) but the song manages to survive much, much longer, but it also still ends up stuttering. This proves that increasing the audio buffer size helps with the problem though. The question now is just: what's the optimal audio buffer size?

### Gemini explains latency in audio programming

> In audio programming, latency is the delay between when the computer calculates a sound and when you actually hear it from the speakers.
> 
> Think of it like a buffer (waiting room) for audio data.
> 
>     Low Latency (e.g., 10ms):
> 
>         The Buffer is tiny.
> 
>         Pros: If you press a key in a video game, the gunshot sound happens instantly.
> 
>         Cons: The computer must refill the buffer constantly. If the CPU gets busy for even a split second (decoding a complex MP3 frame), the buffer empties before the CPU can refill it. Result: Stuttering (silence).
> 
>     High Latency (e.g., 250ms - 500ms):
> 
>         The Buffer is huge.
> 
>         Pros: The CPU can take a break or handle a complex task without the music stopping, because there is plenty of "saved up" audio in the buffer to keep the speakers busy.
> 
>         Cons: If you press "Pause", the music might keep playing for half a second before stopping (because the buffer has to finish playing out). For a music player, this is acceptable.

It also mentions that:

> To play music without stuttering, your computer must decode audio faster than real-time.
> 
>     If a song is 60 seconds long, your CPU must be able to decode it in less than 60 seconds.
> 
>     If your CPU takes 65 seconds to decode 60 seconds of audio, it is mathematically impossible to play it without pausing to buffer.
