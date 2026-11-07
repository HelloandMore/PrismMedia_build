# Screen-discovery compatibility patch — experimental (2026-11-07)

This patch is designed for the reported ETS2 1.62 Scania S case. Capture and
media uploads were working at ~48 FPS, but the old per-draw picker saw zero
final draws. The 16-slot survey sampled several 1024x1024 BC3-sRGB textures
(format 78) in slot 6, and an RGBA8 1024x512 texture (format 29). Neither
qualified under the old 1024x1024 RGBA8 slot-6-only filter.

**This is a diagnostic/fix candidate, not an in-game-validated fix.** The survey
could not establish which resource is the physical Scania display. Do **not**
select a candidate merely because the dimensions match. Use visual or numbered
preview and confirm the intended screen before selecting. No replacement occurs
on broad discovery alone.

## Build

Use GitHub Actions > Build Windows 4.0.0 > Run workflow on your fork. The
`final_draw_discovery_test.cpp` filter tests run before compiling. Download the
resulting `PrismMedia-4.0.0` artifact. Keep PrismMediaClient files in the
existing adjacent `PrismMedia/` helper directory. Copy the *new* DLL and
`PrismDiagnosticConsole.exe` together. No new dependency DLLs are required.

## Test, window capture only

1. Start ETS2 in the Scania cockpit, leave a normal File Explorer window open.
2. In PrismMedia enable exactly **one Custom display** with **Window capture**
   set to `explorer.exe`; disable other custom displays. The source upload must
   be live (`displays`: enabled=1 live=1).
3. Start the diagnostic console. Type `route scan broad` and leave the cockpit
   visible for 16 seconds, then use `route status` and `route candidates`.
4. If stable candidates exist, use **System > Pick truck screen**. It will
   probe each candidate without modifying other displays permanently. Confirm
   ONLY the correct physical GPS. The numbered fallback can help when several
   candidates overlap.
5. If no candidates, try `route scan slot 6` for a focused full-rate survey
   (also sampled at one in four plausible draws), then `route scan wide` to
   permit larger indexed meshes. Avoid repeating these experimental passes
   during normal driving because discovery is CPU-intensive.
6. After selection, check `route status` for `active` and increasing `routed`.
   If no candidate paints the screen, this Scania screen likely uses another
   draw kind, shader-stage binding or render path; a new RenderDoc/targeted
   capture will be needed. This patch must not blindly rewrite its native GPS.
7. `route scan strict` reverts the in-session search rules to stock behavior.
   The mode is intentionally not saved across game restarts.

Commands:

```
route scan status
route scan strict
route scan broad
route scan wide
route scan slot 6
route status
route candidates
route retrain
route preview 0 10
route preview off
```

## How it differs from stock

- Stock *strict* mode retains original 1024x1024 RGBA slot-6 detection.
- Explicit exploratory mode checks common color texture formats including
  BC1/BC2/BC3/BC7, BGRA8 and RGBA8, on pixel-shader slots 0 through 15.
  Resource and shader-view format families must be compatible, dimensions must
  be power-of-two between 256 and 2048, and the view must be a shader-readable
  `Texture2D`. Only plausible `DrawIndexed` calls with triangle-list-like
  index counts are sampled. Broad mode caps the count at 256; wide at 4096.
- All new scanning is bounded, sampled at 1 in 4 eligible draws, with at most
  24 distinct signatures per shader slot and 256 total. New discovery sampling
  stops after 16 seconds; candidate preview continues to work. Regular capture
  and active routing keep their original
  low-overhead behavior.
- The existing exact-signature checks, 3-hit stability rule, grouped passes,
  explicit picker/selection, resource restoration and safe fallback remain.
  A persisted selection isn't applied automatically during experimental scan.

## Known limitations

- 16-slot sampling is not exhaustive; the actual Scania screen may have a
  different texture size or rendering technique and still produce no candidates.
- A candidate can represent another cockpit surface. Do not select an
  unverified preview, especially in a modded truck.
- No live Windows/ETS2 build or functional in-game test was possible here;
  only cross-platform discovery-predicate tests and source-level checks were run.
