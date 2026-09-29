# RefMatch 0.5.79 — Release setup

Build the AU/VST3/Standalone targets using the existing GitHub Actions macOS workflow.

Expected visible plugin version: `v0.5.79`.

Smoke test Auto Gain:
1. Stop Logic playback.
2. Press AUTO GAIN — the pill should show `PLAY MIX`.
3. Start Logic playback — the pill should automatically change to `MEASURING` and fill from left to right.
4. Spotify/reference playback should start automatically if it was paused; an already-playing reference must not restart.
5. On completion the pill should show `AUTO GAIN | <dB>`.
