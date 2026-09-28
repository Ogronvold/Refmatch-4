# RefMatch 0.5.77 — Release setup

Build and install using the same macOS JUCE/GitHub Actions workflow as prior releases.

Expected visible plugin version: `v0.5.77`.

Regression checks:
- Switch repeatedly between A and B while B is mid-track: the B seek bar must not flash to 0.
- Pause/stop B: seek bar and current time must freeze at the last position.
- Resume B: position must continue from the real playback location.
- RECORD REF must still receive system audio and advance from LISTENING 0.0 s when audio is present.
- Genuine track changes and manual seeks must still update the mini-player normally.
