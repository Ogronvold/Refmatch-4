## 0.5.74

- Fixed a regression where RECORD REF could remain at `LISTENING... 0.0 s` while B playback was audible.
- Removed the eager Screen/System Audio capture request that ran whenever the editor opened.
- RECORD REF now owns capture startup and cleanly restarts a still-pending capture request before arming reference learning.
- Preserved the 0.5.73 B-player position-hold behaviour when playback is paused/stopped.
- No DSP, Match EQ, Auto Gain, Loop, Tone or transport-layout changes.
- Updated visible plugin version and CMake project version to 0.5.74.
