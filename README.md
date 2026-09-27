# RefMatch 0.5.73 — Reference Position Hold

This version keeps the B/reference player timeline visually parked at the last known playback position when the reference is paused/stopped, instead of briefly jumping back to 0:00.

- Seek/progress bar keeps the last trustworthy position while paused.
- Current-time display stays at that same position.
- Playback itself is unchanged; starting again continues to follow the live player position.
- Genuine track changes still reset normally.
- Explicit user seeks still update the displayed position immediately.

Plugin/CMake version: 0.5.73.
