# RefMatch 0.5.67 — Release setup

This package keeps the same source/build structure as 0.5.66.

Build the RefMatch project with the existing GitHub Actions workflow or the existing JUCE/CMake macOS build process. The expected visible plugin version is `v0.5.67`.

UI verification:
1. Create a Match.
2. Confirm HEAR MATCHED is visually centred beneath MATCHED.
3. Switch to HEAR ORIGINAL and confirm the Match EQ correction/Matched legend become softly grey/dimmed while remaining visible.
4. Switch back to HEAR MATCHED and confirm the normal colours return.
