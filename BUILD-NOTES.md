# RefMatch 0.5.85 — Matched Applied Graph Sync Fix

## Changed
- `Matched (Applied)` now renders the Match EQ correction only, excluding manual Tone EQ.
- At Amount 0%, `Matched (Applied)` is exactly flat at 0 dB.
- 50%, 100% and 200% continue to represent half, full and double Match correction respectively.
- Target Range weighting remains shared with the DSP path.
- Added a DSP regression test proving the Match-only display stays flat at 0% even while Tone EQ is active.

## Unchanged
- Tone EQ DSP and UI.
- A/B playback, reference capture, Auto Gain, Loop and Match workflow.
- Your Mix / Reference analysis traces.

Visible plugin version and CMake project version: 0.5.85.
