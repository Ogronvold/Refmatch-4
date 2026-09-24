# RefMatch 0.5.44 — Match workflow + verification

This release keeps the existing RefMatch audio path and focuses on the core Match EQ workflow.

- MATCH now requires at least 8.0 seconds of both MIX and REF capture.
- Capture status shows a check mark once each side has enough data.
- MATCH shows `ANALYZING...` during processing and `MATCHED ✓` when complete.
- Match EQ status is moved out of the YOUR MIX card and sits with the match workflow.
- Max Correction is now visible and host-automatable; it is the final absolute correction ceiling, including Amount above 100%.
- A subtle `Residual tonal error ... dB avg` readout estimates the remaining level-normalised tonal difference over the active target range after the currently applied Match EQ curve.
- Existing Auto Gain, source switching, reference artwork/player, Loop and Tone EQ behaviour are otherwise preserved.

See `BUILD-NOTES.md` for implementation notes and `RELEASE-SETUP.md` for the commercial release pipeline.


### 0.5.44 workflow polish
- Record MIX/REF buttons now fill to 100% at the 8-second Match-ready threshold while capture may continue.
- STREAM reference capture arms before playback and starts a paused stream without restarting an already-playing source.
- Auto Gain is a compact tertiary control in the A card.
- Amount 0–200% is intentional: 100% is the full learned correction, >100% is deliberate over-match; Max Correction remains an absolute safety clamp.
- ORIGINAL/MATCHED compare is labelled separately from the Match EQ ON/OFF toggle.
- Residual tonal error is kept as advanced tooltip information rather than primary UI.
