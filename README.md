# RefMatch 0.5.48 — Capture state polish

## 0.5.48 capture buttons
- 0–8 s capture now fills the Record Mix/Record Ref button itself from left to right.
- Live capture text reads `LISTENING...  x.x s`.
- At 8 s the button switches to a stable `MIX READY ✓` / `REF READY ✓` completed state without stopping capture.
- Continued capture shows a small status below, e.g. `11.4 s captured · keep listening`.
- Capture/DSP/stream logic is otherwise unchanged from 0.5.45.


This release keeps the existing RefMatch audio path and focuses on the core Match EQ workflow.

- MATCH now requires at least 8.0 seconds of both MIX and REF capture.
- Capture status shows a check mark once each side has enough data.
- MATCH shows `ANALYZING...` during processing and `MATCHED ✓` when complete.
- Match EQ status is moved out of the YOUR MIX card and sits with the match workflow.
- A subtle `Residual tonal error ... dB avg` readout estimates the remaining level-normalised tonal difference over the active target range after the currently applied Match EQ curve.
- Existing Auto Gain, source switching, reference artwork/player, Loop and Tone EQ behaviour are otherwise preserved.

See `BUILD-NOTES.md` for implementation notes and `RELEASE-SETUP.md` for the commercial release pipeline.


### 0.5.45 cleanup
- Record MIX/REF buttons now fill to 100% at the 8-second Match-ready threshold while capture may continue.
- STREAM reference capture arms before playback and starts a paused stream without restarting an already-playing source.
- Auto Gain is a compact tertiary control in the A card.
- Amount is intentionally 0–100% and is the only Match EQ strength control: 0% = no learned correction, 100% = the full calculated correction.
- ORIGINAL/MATCHED compare is labelled separately from the Match EQ ON/OFF toggle.
- Residual tonal error is kept as advanced tooltip information rather than primary UI.

### 0.5.45 cleanup
- Removed Max Correction from the public parameter set and UI.
- Amount is clamped to 0–100% everywhere, including legacy state migration.
- Cleaned Match EQ states, UTF-8 labels, Loop/Match EQ labeling, Tone-off dimming, and stale artwork handling.