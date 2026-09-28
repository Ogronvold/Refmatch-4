# RefMatch 0.5.75 — Release setup

Build the RefMatch project with the existing GitHub Actions workflow or the existing JUCE/CMake macOS build process. The expected visible plugin version is `v0.5.75`.

Regression checks for this build:

1. Start B/reference playback and confirm current time + seek bar advance normally.
2. Pause/stop B and confirm both freeze immediately at the last playback position.
3. Leave B paused for several seconds and confirm neither time nor seek bar continues counting.
4. Resume playback and confirm the display continues naturally from the current real position.
5. Seek while paused and confirm the UI reflects the explicit seek without drifting afterward.
6. Change track and confirm metadata/artwork/position update for the new track.
7. Confirm RECORD REF still starts system-audio capture correctly (0.5.74 fix retained).
