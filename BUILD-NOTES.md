## 0.5.73

- Fixed the B/reference mini-player progress bar visually resetting to 0:00 when playback is paused/stopped.
- Added a UI-side last-known-position cache for transient zero/invalid MediaRemote position reports while paused.
- Explicit seeks continue to update the cached display position immediately.
- No DSP, Match EQ, Auto Gain, loop, Tone EQ, or audio playback behavior was changed.
- Updated visible plugin version and CMake project version to 0.5.73.
