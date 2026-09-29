# RefMatch 0.5.78

Reference progress freeze fix.

- B/reference current-time and seek-bar now freeze when playback is paused/stopped.
- No stale MediaRemote elapsed clock is allowed to advance the mini-player while transport is not playing.
- Track changes and explicit seeks still update correctly.
- Reference capture-start fix from 0.5.74 is retained.
