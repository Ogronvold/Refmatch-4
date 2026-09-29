## 0.5.79

- Auto Gain can now be armed while the DAW/MIX is stopped.
- The Auto Gain pill shows `PLAY MIX` until fresh non-silent MIX audio is detected.
- Measurement begins automatically when MIX audio appears; no second Auto Gain click is required.
- Reference/System Audio capture is started when measurement begins.
- Spotify/reference playback is started automatically for the measurement when needed, but an already-playing stream is not restarted.
- If B is already selected but paused, Auto Gain sends PLAY directly instead of switching away from B.
- Completed `AUTO GAIN | <dB>`, `MEASURING`, and `RETRY` states are preserved.
- Preserved reference capture, pause-freeze, A/B progress-hold, and paused-seek fixes from 0.5.74–0.5.78.
- Updated visible plugin version and CMake project version to 0.5.79.
