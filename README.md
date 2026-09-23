# RefMatch 0.5.33 — Live Reference Spectrum

- B/reference now gets a live purple spectral metric during normal reference playback, not only during REF capture.
- Selecting B starts the existing system-audio analysis capture when needed; reference audio remains analysis-only and is never routed through the plug-in output.
- A keeps the existing orange/cyan before/after live spectral metric.
- No Match EQ DSP, gain, routing, transport, Loop, or Tone EQ behaviour was changed.


## 0.5.42

- Auto Gain UI spacing polish.
- Auto Gain requires active MIX playback before the 5-second measurement can begin.
- Added 5-second AUTO GAIN / level match for A vs system reference B.
- Uses gated accumulated RMS energy rather than instantaneous peak matching.
- Automatically updates A Gain and shows the resulting dB offset in the UI.
- Starts system-reference analysis as needed and restores MIX after the measurement when appropriate.
