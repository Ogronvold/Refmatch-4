# RefMatch 0.5.78 — Release setup

Build the AU/VST3/Standalone targets through the existing macOS GitHub Actions workflow.

Expected visible plugin version: `v0.5.78`.

Primary regression test:
1. Play a Spotify/reference track in B.
2. Pause it.
3. Drag or click the B progress bar to another position.
4. Confirm the current-time/progress UI moves immediately and stays there.
5. Confirm Spotify remains paused.
6. Press Play and confirm playback resumes from the selected position.
