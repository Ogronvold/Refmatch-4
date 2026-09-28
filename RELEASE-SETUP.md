# RefMatch 0.5.74 — Release setup

Build the RefMatch project with the existing GitHub Actions workflow or the existing JUCE/CMake macOS build process. The expected visible plugin version is `v0.5.74`.

After installing the AU/VST3 build, verify:

1. B/reference playback still holds its displayed timeline position while paused.
2. Press RECORD REF while B is paused: playback starts and reference capture advances beyond 0.0 s once system audio arrives.
3. Press RECORD REF while B is already playing: playback is not restarted and capture advances normally.
4. Stop RECORD REF: the captured duration remains available for Match.

If macOS has revoked Screen & System Audio Recording permission for the DAW/AU host, re-enable that permission and reopen the host before testing.
