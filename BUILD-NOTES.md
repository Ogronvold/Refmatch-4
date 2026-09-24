# RefMatch 0.5.45

- Action-row spacing normalized to 12 px between Record Mix, Record Ref, Match, Reset, Loop, quick toggle, and EQ controls.
- MATCH remains horizontally centered on the same axis as the A/B switch.
- Auto Gain behavior and UI are unchanged from 0.5.41.


## 0.5.41
- Compacted the Auto Gain pill from the left while keeping the result value anchored in place.
- Centered the MATCH action on the same horizontal center as the A/B switch control.
# RefMatch 0.5.40

- Auto Gain pill spacing tightened; result value moved inward for balanced padding.
- Auto Gain now refuses to start until A/MIX has fresh audible signal and shows PLAY MIX instead.

- Replaced 5 s raw-RMS Auto Gain with a K-weighted, LUFS-style parallel A/B level match.
- Requires fresh overlapping audio from both A and B for roughly 2 s of the 5 s window.
- Uses robust median + outlier rejection for repeatable results.
- Refuses unstable/extreme matches instead of applying wild gain; automatic range is limited to ±12 dB.
- A Gain now ramps over 200 ms to avoid abrupt gain jumps.

# RefMatch 0.5.6 validation

## Architecture change

ReferenceAnalysis owns the only system-audio FIFO consumer, on an internal JUCE thread.
It pulls complete stereo blocks at 48 kHz for reference spectrum, peak and learning.
processBlock now only measures/learns MIX and processes output audio. releaseResources
no longer stops external capture. Thread shutdown is joined before capture destruction.
Capture buffer fill is checked before consumption; partial packets are not padded into
learned profiles. REF spectra are displayed and remapped using their actual 48 kHz rate.
MIX and REF have separate LearnCapture accumulators and share no FFT working storage.

Smooth uses a continuous Gaussian target blur before fitting. Three broad parametric
stages follow the 20-band match bank; coefficient changes retain the 20 ms ramp.
All controls are appended to the parameter list for existing-session compatibility.
Tone gains/frequencies are APVTS parameters. Old sessions get neutral Tone defaults.

## Local validation

UTF-8 and delimiter checks, CMake source/test path checks and workflow shell syntax.
Source review of the single FIFO consumer, independent analysis lifecycle, safe PLAY
routing and seek-on-release. Layout sketch/coordinate check, not a native screenshot.
ZIP integrity and required source/workflow/test checks.

No Apple developer toolchain is installed. C++ tests, native build, Logic playback,
ScreenCaptureKit on this host and actual new UI rendering have NOT been executed locally.
The new analysis ownership is a material change that requires the acceptance tests below.

## Added automated regressions (GitHub Actions)

- ReferenceAnalysisState records REF and updates meters with no host processBlock calls.
- Smooth reduces alternating narrow correction changes.
- Manual Tone remains active at zero Match Amount and produces the expected 3 dB sine boost.
- Playhead hit testing takes priority over coincident loop edges; both edges remain reachable.
- Existing fade ordering, transport cancellation, playback feedback, captures, stereo power,
  transparent neutral EQ, full-range Amount response and timeline range tests are retained.

## Logic acceptance

1. While A plays MIX, press PLAY: the mix fades out before the reference starts; B is selected.
   Repeat while Logic is stopped, then resume Logic. There must be no simultaneous MIX output.
2. Stop Logic while Spotify plays. REF peak, spectrum and RECORD REF must continue. MIX clears
   when no callback/input is available. Test 44.1/48/96 kHz host sample rates.
3. Capture MIX and REF, stop them, MATCH, change Smooth from Fine to Broad; no re-recording.
4. Open Tone, change all frequencies/gains, audition EQ ON/OFF, Amount 0/100, RESET TONE.
   Confirm actual audible response, bypass ramp and persistence on reopening the project.
5. Drag white playhead, then release; check no region edit. Resize loop edges and draw a new
   region. Seek/skip outside an active loop turns it off. Test short selections and track changes.
6. Deny capture permission, quit source, pause source, suspend/resume audio and remove plugin
   while REF recording. No crash, stale meter, shared FIFO consumer or A/B dependency on meters.
7. Verify typography, orange/purple palette and expanded/collapsed Tone heights in Logic.

Private MediaRemote APIs remain optional and fallible. No helpers, subprocesses, or new
external audio routes are introduced. Existing live-analyser audio-thread locks remain for
MIX; this version does not claim hard realtime certification or sample-accurate Spotify seek.

## 0.5.6 focused changes and acceptance

Tone enable is appended as a parameter. Only the three manual stages ramp to identity;
learned EQ and Tone values remain unchanged. DSP tests added for bypassed actual audio,
retention of learned match response and exact restoration of Tone values.
Plot scaling no longer reads either full-match or Tone curves. Only the range selector
changes the scale; out-of-range curves are explicitly marked. Range persists in state.

Local checks: source/UTF-8, CMake paths, workflow shell, control coordinates and archive.
New C++ tests and Logic audition have not run locally (no Apple developer tools).
Verify Smooth around 95.2-95.5%, all Tone controls, Amount, Tone on/off, range choices,
clipped-curve label and save/reopen. Axis labels must remain unchanged during EQ edits.

## 0.5.6 focused changes

PersistentLoop is a pure state machine shared by production and runtime tests.
Tests cover metadata outage/recovery, refused and unconfirmed seek with backoff,
A/B/pause, shorter tracks, invalid edits, in-loop seeking and explicit OFF. The timer
never calls enable(false). The indicator distinguishes confirmed active from waiting.

Capture scratch buffer belongs to each stream delegate on its serial callback queue.
Both workflow YAML files parse with Ruby Psych, all workflow shell blocks pass bash -n,
and release-macos.sh passes bash -n. Source/UTF-8 and archive checks are also performed.
No certificates were supplied, no installer was built and no notary submission occurred.
The release template still requires credentialed testing and clean-Mac installation.

C++ suites have NOT run locally: no Apple developer toolchain. In Logic, record REF
repeatedly inside a short loop; stop/start Logic and Spotify; alternate A/B; hide the
editor; scrub; change songs; temporarily lose metadata. LOOP must stay selected and
recover without being re-enabled. For a shorter track, adjust bounds while WAITING.

## 0.5.6 additions
- Main-screen persistent LOOP toggle reuses the most recently defined range.
- BEFORE/AFTER on A keeps Mix Gain active while bypassing/enabling Match EQ + Tone EQ.
- Match EQ low/high draggable range handles with soft transition outside the selected range.
- Applied EQ graph now overlays a smoothed live before/after spectrum.
- Tone EQ frequency ranges are focused: LOW 30–300 Hz, MID 200 Hz–6 kHz, HIGH 3–20 kHz.
- LOW/HIGH can switch between Shelf and Bell; MID adds adjustable Q.
- Capture guidance recommends at least 8 seconds of representative audio.
- Max Correction is now respected in MatchEQ curve scaling/processing.

Build note: full macOS AU/VST3 compilation must run on macOS because the project uses Objective-C++/ScreenCaptureKit. The Linux validation environment cannot provide the Objective-C++ compiler/runtime required for the plugin target.

## 0.5.37 — Auto Gain pill UI
- Redesigned AUTO GAIN to match the requested compact mastering-style pill.
- Added orange level icon, fixed AUTO GAIN label, divider, and separate result/countdown field.
- Moved the control to the lower-right of the A card; DSP and 5-second matching behavior are unchanged.


## 0.5.45
- Progress-style MIX/REF capture buttons; 8 s = Match-ready fill, capture can continue.
- STREAM REF capture starts paused playback only when needed and keeps capture armed before PLAY.
- Compact Auto Gain moved to the top-right of A card.
- ORIGINAL/MATCHED compare wording separated from Match EQ toggle semantics.
- Residual tonal error moved out of primary UI into Match tooltip.
- Amount is now 0–100% and is the sole Match EQ strength control. Max Correction has been removed.

## 0.5.45 cleanup
- Removed Max Correction parameter/UI/DSP clamp.
- Amount clamps to 0–100%, including restored legacy projects.
- READY states are visually quieter; Match remains the primary workflow action.
- Fixed UTF-8 UI strings and clarified Loop / Match EQ / Tone states.
