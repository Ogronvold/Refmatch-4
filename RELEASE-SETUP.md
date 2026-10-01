# RefMatch 0.5.85 — Release setup

Expected visible plugin version: `v0.5.85`.

Build and install using the same GitHub Actions/macOS workflow as previous RefMatch versions.

Regression check after install:
1. Create a Match.
2. Turn Tone ON and apply a visible manual Tone boost/cut.
3. Set Match Amount to 0%.
4. `Matched (Applied)` must remain perfectly flat at 0 dB, while Tone may still be audible separately.
5. Verify 50%, 100% and 200% scale the applied Match curve progressively.
