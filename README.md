# RefMatch 0.5.74 — Reference Capture Start Fix

This build fixes the reference-recording regression where RECORD REF could stay at `LISTENING... 0.0 s` even though B was playing.

System-audio capture is no longer requested just because the editor opens. RECORD REF starts capture on demand, and if an earlier asynchronous start is still pending it is restarted cleanly before reference learning begins.

The B/reference timeline position-hold behaviour from 0.5.73 is preserved.

Plugin/CMake version: 0.5.74.
