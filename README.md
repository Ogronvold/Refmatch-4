# RefMatch 0.5.77 — A/B Progress Hold

Incremental source release based on 0.5.76.

New in 0.5.77:
- Keeps the last valid B/reference position through transient invalid MediaRemote reads during A/B switching.
- Prevents the mini-player seek bar/current-time display from flashing to 0 before the real position returns.
- Genuine valid track changes, seeks and real position updates are still accepted normally.
- Preserves the reference-capture start fix from 0.5.74 and pause/stop progress freeze from 0.5.75.

Plugin/CMake version: 0.5.77.
