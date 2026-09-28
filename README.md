# RefMatch 0.5.76 — A/B Progress Flash Fix

This build keeps the 0.5.74 reference-capture start fix and the 0.5.75 paused/stopped progress freeze.

New in 0.5.76:
- prevents the B reference progress bar/current-time display from flashing to 0:00 during A/B switching
- ignores the brief transient zero-position MediaRemote can publish during the transport hand-off
- preserves the last valid reference position unless the track genuinely changes or the user explicitly seeks/resets

Plugin/CMake version: 0.5.76.
