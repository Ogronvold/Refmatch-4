## 0.5.75

- Fixed the B/reference mini-player progress bar continuing to move after Spotify/reference playback had paused or stopped.
- Current-time and seek-bar position now freeze at the last trustworthy playback position whenever the transport is not actually playing.
- Playback metadata and duration can still refresh while paused without advancing the displayed position.
- A genuine track change can still reset/update the displayed position immediately.
- Explicit seeks while paused remain reflected in the mini-player.
- Preserved the 0.5.74 reference-capture start fix.
- Updated visible plugin version and CMake project version to 0.5.75.
