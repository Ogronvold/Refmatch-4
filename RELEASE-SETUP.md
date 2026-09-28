# RefMatch 0.5.76 — Release setup

Build the RefMatch project with the existing GitHub Actions workflow or existing JUCE/CMake macOS build process.

Expected visible plugin version: `v0.5.76`.

Primary regression test:
1. Start B playback at a non-zero position.
2. Switch B -> A -> B several times.
3. Confirm the mini-player progress bar and current-time label never flash to 0:00 during the hand-off.
4. Pause B and confirm progress/time remain frozen.
5. Resume B and confirm progress continues from the real position.
6. Verify RECORD REF still captures system audio as in 0.5.74.
