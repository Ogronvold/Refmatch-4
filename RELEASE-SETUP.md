# RefMatch 0.5.73 — Release setup

Build the RefMatch project with the existing GitHub Actions workflow or the existing JUCE/CMake macOS build process. The expected visible plugin version is `v0.5.73`.

Primary verification: play B/reference to a non-zero position, pause it, and confirm both the seek bar and current-time label remain at that position. Resume playback and confirm the UI continues naturally from the live position.
