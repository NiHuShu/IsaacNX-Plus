# Playing: Switch install, controls, PC saves and mods

This assumes you have built the NRO (and, for PC play, the `gl` executable)
following [BUILDING.md](BUILDING.md). Paths use the same variables:

```sh
R=$PWD                                   # this repository
U=$R/build/browsing-port/repo            # upstream checkout
GAME="$HOME/.local/share/Steam/steamapps/common/The Binding of Isaac Rebirth"
```

## Installing on the Switch

You need a Switch running custom firmware (tested with Atmosphère on a
Switch Lite) and a homebrew launcher that can start NROs in **Application
mode**: hbmenu opened by holding R while launching a game, or Sphaira (tested
with 1.0.8). Applet mode (hbmenu opened from the Album) does not have enough
memory, and the NRO refuses to start there.

The SD card layout:

```
sdmc:/switch/isaac-repentance-ns1-recomp.nro           the NRO you built
sdmc:/switch/isaac-recomp/
  guest/                                  game files from your PC install
    resources/packed/*.a
    resources/scripts/...
    savedatapath.txt
    mods/<mod folder>/                    optional, one folder per mod
  saves/                                  your saves (created at first start)
  settings.ini                            Switch options (created at first start)
  browsing-last-run.txt                   what the last start did, for troubleshooting
```

Copy the files with an SD card reader or over FTP (Sphaira has an FTP
server; ftpd and others work too):

1. `build/browsing-native/switch/isaac-repentance-ns1-recomp.nro` → `sdmc:/switch/isaac-repentance-ns1-recomp.nro`.
2. From your Steam folder (`$GAME`): `resources/` and `savedatapath.txt` →
   `sdmc:/switch/isaac-recomp/guest/`. The NRO only reads the 22 files listed
   in its `instance_index.json` (about 1.07 GB): the ten `resources/packed/*.a`
   archives, `resources/scripts/` and `savedatapath.txt`. Copying the whole
   `resources/` folder (about 1.5 GB, with the language packs) also works; the
   extra files are not used.
3. Optional: your PC saves into `saves/` and your mods into `guest/mods/`
   (sections below).

Then launch **ISAAC REPENTANCE+ NS1 RECOMP** from the homebrew launcher. Quit with
HOME → Close: the game shuts down normally and writes its saves.

The NRO contains the memory image of your executable. Do not share it.

Choose **Both** in the Builder or pass `--format both` to copy the installer
and the direct-launch game to the same SD card. Run
`isaac-recomp-installer.nro` from the Homebrew Menu and press **A** to
install the HOME Menu title. **IsaacNX+** remains available to launch from
the Homebrew Menu in Application mode, including after an installer error.
Pressing **X** after a successful install deletes the install payload and
keeps the direct-launch game files.

Both launch methods use the same `sdmc:/switch/isaac-recomp/saves/` folder.
The HOME Menu title keeps mods in `sdmc:/switch/isaac-recomp/mods/`, and the
direct-launch game reads `sdmc:/switch/isaac-recomp/guest/mods/`.

## Controls

The pad goes through the game's own controller support, so the game's
controller layout applies. Two things differ from an Xbox pad:

| Button | In a run | In menus |
| --- | --- | --- |
| A, B, X, Y | shoot by position: right, down, left, up | A accepts, B goes back (by position if `a_accepts=0`) |
| + (tap) | pause, when released | START |
| + (hold about 2 s) | restart the run (the game's own restart) | START |
| − | BACK | BACK |
| L, R, ZL, ZR, sticks, D-pad | their Xbox equivalents | their Xbox equivalents |

Mod Config Menu (opened with a click of the right stick) counts as a menu
although the run keeps going under it: A accepts and B goes back.

The buttons drawn on screen are the Switch ones (`switch_glyphs=1`), using
the Switch art that ships inside the PC game. The controls drawing on the
first room's floor shows the in-run layout.

## Game Options

Press **X** on the launcher. Use the D-pad to select or change a setting,
**A** to save, **B** to discard, and **X** to select the performance defaults.
These changes apply when the game launches.

A fresh setup uses an internal render scale of **1×** and disables caustics,
bloom, shockwaves and room color correction. Lighting, the water surface and
vertical sync stay on. **1×** is the lowest supported internal render scale.
Higher render scales can sharpen text and small sprites. **Frame skip** has
three choices. **Adaptive** skips extra frames as needed, **30 FPS** always
skips the in-between frame to give busy rooms more time, and **60 FPS** draws
every frame. The launcher reads existing settings. To use the preset on an existing setup,
press **X**, then **A**.

The menu writes the game's `options.ini` inside the save folder and keeps a
Switch-local `saves/resources/config.ini` override for graphics effects.
Frame skip is saved as `frame_skip` in the homebrew's `settings.ini`.

**Skip water reflections** is an experimental option. It skips recording
reflected entities in flooded rooms. Other water effects still run. Mods
that draw during the reflection pass may look different. The option starts
off and applies the next time you launch the game.

## Launcher artwork and font

The builder extracts the Repentance+ logo from your installed game files. If
your Steam library cache has the Isaac banner, the builder uses it automatically.
You can also put the [official Steam library banner](https://shared.steamstatic.com/store_item_assets/steam/apps/250900/library_hero.jpg?t=1722458407)
in `launcher-art/library_hero.jpg` at the root of your checkout before building.
Put your copy of Motiva Sans Regular in the same folder as
`MotivaSans-Regular.ttf` or `MotivaSans-Regular.woff` to use Steam's typeface.
The launcher uses the Switch system font when it has no readable Motiva Sans
file.

You can also copy the font onto the SD card as
`sdmc:/switch/isaac-recomp/ui/MotivaSans-Regular.ttf` (or `.woff`) for an
existing build. The artwork and font are kept out of the source repository.

## settings.ini

`sdmc:/switch/isaac-recomp/settings.ini` is created with the defaults the
first time the game starts. Edit it with any text editor; `1` is on and `0`
is off. Changes apply at the next start.

```ini
# Draw Nintendo Switch buttons instead of the Xbox ones.
switch_glyphs=1

# In menus A accepts and B goes back. 0: by position like an Xbox pad
# (the bottom button, B, accepts). In play the buttons always shoot by
# position.
a_accepts=1

# The game updates 30 times a second and draws a second frame between
# two updates. Where that does not fit in the time (heavy rooms, the
# water floors) the whole game slows down. 1: skip the in-between frame
# only then, so the game keeps its speed. 2: always skip it (30 fps,
# the most headroom). 0: always draw it.
frame_skip=1

# Feed the GPU from a second CPU core while the game runs on the first.
# 0: everything on one core, as older builds did.
render_thread=1

# Before the game, a check that its files are on the card and none was
# cut short by a copy; Play stays off when they are not. 0: no check.
check_game_files=1

# Options -> Fullscreen in the game zooms in on the room, as the
# console releases do; Fullscreen off is the PC view. How much: 100
# shows 426x240 points of the room instead of 480x270, the most the
# game allows; 0 makes Fullscreen look like the PC view.
zoom=100
```

**Zoom.** Fullscreen starts off in a fresh setup and uses the wider PC view.
In the game's Options menu, turn **Fullscreen** on to use the zoomed console
view. You can switch views during a run. The `zoom` value controls how far
Fullscreen zooms in.

The launch screen names what is missing. Only what cannot work counts: a file that
is not there or is empty, or an archive of 1 MiB or more with over 1 % gone. Other
size differences are normal (a Steam patch moves the archives a little) and only
appear in `browsing-last-run.txt`. A copy cut within its last 1 % is not noticed.

These are part of the host, not mods: they do not appear in the mods menu,
and with `switch_glyphs=0` the game's files are not touched at all.

A `settings.ini` written by an older build has no `frame_skip`,
`render_thread`, `steam_sync`, `sync_prompt` or `launcher` line; the game then uses `1`
for all of them, so the Steam Cloud sync is already on. Add a line to change it.

`frame_skip` matters on the stock clocks. In the measured heavy room (36
enemies on Flooded Caves, CPU 1020 MHz) drawing both frames took 55 ms per
update, so the game ran at about 60 % speed. Skipping the in-between frame
took 33 ms, which is full speed at 30 fps. Rooms that fit keep 60 fps with
`1`.

`render_thread` moves the graphics driver to another core. In the same room
a tick then takes 25 ms with `frame_skip=1`, so the game keeps its speed
with room to spare. It adds one frame of input latency; set it to `0` if a
problem shows up only with it on.

## Moving your PC saves

On PC, Repentance+ keeps its three save slots in Steam Cloud:
`<Steam>/userdata/<your id>/250900/remote/rep+persistentgamedataN.dat` and
`rep+gamestateN.dat` (the run in progress). The native build has Steam
stubbed and reads the same files under their local names. The content is the
same, only the name changes. `import_pc_saves.py` builds a save folder from
your Steam profile:

```sh
python3 host/import_pc_saves.py --out build/browsing-native/pc-saves
```

It writes:

```
pc-saves/
  documents/my games/binding of isaac repentance+/
    persistentgamedata1-3.dat     your three slots (unlocks, progress)
    gamestate1-3.dat              runs in progress, if any
    options.ini                   native defaults plus your PC preferences
  data/<mod directory>/saveN.dat  each installed mod's own save data
```

- The defaults are for Steam on Linux with the game under Proton. On Windows
  or with another Steam folder, pass `--steam <folder with userdata/>`,
  `--game <game folder>` and `--pc-options <Documents/My Games/Binding of
  Isaac Repentance+/options.ini>`. If you have several Steam accounts, pick
  one with `--steam-user <id>`.
- `options.ini` is not your PC file copied as-is: it starts from the native
  defaults (`SteamCloud=0`, so the local slots are read; windowed 960x540)
  and takes your preferences (volume, language, HUD, `EnableMods`…) from the
  PC file. Do not replace it with the PC one.
- Steam may rewrite the cloud files when the game closes or syncs. Close the
  game, let Steam finish syncing, then import.

For the Switch, copy the **contents** of the output folder (`documents/` and
`data/`) into `sdmc:/switch/isaac-recomp/saves/`. If that folder already has
saves you care about, back it up first: the files replace the console's.
On PC, point `ISAAC_SAVE_DIR` at the output folder.

The game writes back into the same folder: the slots, `options.ini`,
`save_backups/`, the game's `log.txt`, mod data and `mod-state/` (see below).
To keep both machines in step automatically, see [Steam Cloud saves](#steam-cloud-saves)
below. Copying saves from the Switch back to a PC by hand has not been tested. If you try
it, the names have to go back to `rep+…` in Steam's `remote/` folder, and you
should back up both sides first.

## Steam Cloud saves

The console can keep its saves in your account's Steam Cloud by talking to
Steam itself: no PC, no Steam client running, no local network.

The first time, the launch screen shows a QR code: scan it with the Steam app on
your phone and approve the sign-in. **−** on that screen signs out again, which
also ends the session on Steam; your saves stay on the card. Press B on the code
to play without syncing.

Once signed in, the screen says whether the account has the game and its
expansions. If it does not have the game the saves do not sync, and you can still
play. Building with `--no-steam` ([BUILDING.md](BUILDING.md#6-switch-nro)) leaves all
of this out.

**Uploading works on the console since 2026-11-10** (from the panel and when the
game is quit from its own menu). One upload at exit was refused once by Steam's
storage (HTTP 403) before a fix to how the upload URL is read; if an upload fails,
the panel says so and the save stays marked to upload.

Once signed in it behaves like this:

- at start, a launch screen in the style of the Steam library shows the sync
  step by step and each save file; saves that changed only on Steam are
  downloaded (the copy they replace is backed up); if both sides changed, a
  dialog asks which copy to keep. The game starts when you press A on **Play**
  (+ leaves, Y retries a sync that failed);
- at exit, the slots you played are uploaded: quit from the game's own menu
  (B on the title screen, then **Yes** to "Are you sure you want me to
  die?"), or close it with HOME → Close Software, which lets it quit
  normally and upload before the console ends it (it has 15 seconds for
  that);
- in the game, **hold − and press +** for the Steam Cloud panel: **A** uploads
  the saves that changed right now and says whether it worked (a missing
  connection shows as "Upload failed" with the reason), **X** turns the upload
  when the game closes on or off, **B** (or − and + again) closes it. A run
  pauses when the panel opens. If the upload when the game last closed failed,
  the panel says so the next time.

`settings.ini` has `steam_sync=1` (0 turns it off), `sync_prompt=1` (0 makes
the newest copy win without asking), `auto_upload=1` (0: nothing is uploaded
when the game closes, only from the panel) and `launcher=1` (0 starts the game
straight away, with no screen, and then only a token copied from a PC signs in).

[STEAM-CLOUD.md](STEAM-CLOUD.md) has the sign-in details, the rules, what was
measured, what is not tested yet, and the risks of using an unofficial Steam
client with your account.

## Mods

Mods from the Steam Workshop work, including Lua mods. They were tested with
17 mods at once (EID, Mod Config Menu, Enhanced Boss Bars, Planetarium
Chance, Boss Rush Wave Counter and others) together with their saved data.

### Steam Workshop on Switch

Press **Y** at the launcher to open **Workshop**. Choose **Browse** and use
**Y** for the Switch keyboard, **X** to change sort, and **−** to enter a Steam
tag such as `Lua` or `Items`. Browse loads up to 24 results per page in a
two-column grid. Hold the D-pad or left stick to move between cards and across
page boundaries, or use **ZL / ZR** to jump between pages. If prompted,
use **Y** for the existing Steam QR sign-in. After signing in, open Workshop
again from the launcher.

Open a listing with **A** to see its description, required items, and install
or update action. Press **+** on a Browse card to start a local install or
queue it behind the current download. The Browse screen shows download
progress and the number waiting. **B** cancels the active download and clears
the remaining queue. Use up and down to scroll the description. The scrollbar
shows where you are. Supported descriptions show headings, emphasis, lists
and quotes, and render embedded images and GIFs from supported image hosts.
The selected card and the details page animate GIF previews. Long animations sample frames across the full loop.
Recently visited pages and thumbnails stay cached until you leave Workshop
or the cache fills. Changing the search, tag, or sort loads a fresh set of pages.

**Y** on the details screen reviews missing dependencies. The mod page shows
installation progress as a percentage and verified MiB. Press **B** to cancel
during an install or return to the list once it finishes. A failed or cancelled
replacement leaves the working copy available.

Use **L / R** to switch tabs. **Installed** lists Workshop and manually
copied mods. **X** changes the selected mod's enable setting for the next
launch. **Y** checks Workshop versions and opens a review for individual or
bulk updates. **−** removes a selected Workshop-managed mod after confirmation.
The play launcher remains available via **B** when you finish.

To compare the console with your Steam account, open **Sync** and select
**Sync Steam Mods** with **A**. **X** cycles the sync direction on that tab.
Mark or skip changes with **A** in the review, then press **X** to continue
to the confirmation screen.

- **Steam to Switch** proposes installs and updates from your subscriptions.
  Removal of local copies for items you no longer subscribe to is optional.
- **Switch to Steam** offers subscriptions for your local Workshop mods. It
  leaves existing subscriptions in place.
- **Two-way** shows both sets of differences. For an item present only on
  Steam, you can explicitly choose to unsubscribe instead of installing it.

Local removals and Steam subscription changes start deselected. Installing
locally and subscribing on Steam are separate actions. Sync runs only after
you confirm a plan.

[Steam Workshop protocol, storage and limits](STEAM-WORKSHOP.md).

### Adding mods

1. On PC, subscribe in the Workshop and start the game once: it places each
   mod in `$GAME/mods/<name>_<workshop id>/`. A mod is a folder with a
   `metadata.xml`; folders without one are ignored by the game.
2. Copy each mod folder **as is** (keep the folder name) to:
   - Switch: `sdmc:/switch/isaac-recomp/guest/mods/<folder>/`
   - PC: `<instance>/mods/<folder>/` (see [Playing on PC](#playing-on-pc))
3. Mod save data (Mod Config Menu settings, EID options, …) is not in the mod
   folder. As on PC it is in `data/<directory>/`, where `<directory>` is the
   one in the mod's `metadata.xml` (`mod config menu` for the folder
   `mod config menu_3701683951`). `import_pc_saves.py` copies it; by hand,
   copy `$GAME/data/<directory>/` to `saves/data/<directory>/`. Builds before
   2026-10-27 kept it in `data/<mod folder>/`; such a folder is still read
   when `data/<directory>/` does not exist yet, and the game then saves under
   the new name.

On the Switch, play mode always mounts `guest/mods/`.

### Turning mods on and off

Use the game's own **Mods** menu. The choice applies at the next start, as
on PC, and the host remembers it as an empty file in the save folder:
`mod-state/<folder>.off` or `mod-state/<folder>.on`. That file wins over
anything else, so:

- to turn a mod off without the menu, create `saves/mod-state/<folder>.off`;
- a mod folder that contains `disable.it` starts off until you turn it on in
  the menu;
- to remove a mod, delete its folder from `mods/` (and, if you like, its
  `mod-state` file and `data/` folder).

A disabled mod is still listed in the menu, greyed out, so it can be turned
back on; none of its files are loaded.

### Limits

- Mods run on the game's Lua 5.3.3 API, as on PC. A mod that fails logs the
  error to the game's `log.txt` (in `saves/documents/my games/binding of
  isaac repentance+/`) and the game continues, as on PC.
- Mods that need REPENTOGON or anything else that patches the Windows
  executable are not supported: there is no Windows executable here.
- The game has 768 MiB of heap. Leaving the mods menu reloads every mod's
  resources while the old ones are still in memory: 17 mods peaked at
  421.5 MiB, so very large mod collections may not fit.
- Replacing game graphics by dropping loose files into `resources/` does not
  work: the engine reads its packed archives first. Graphics have to be
  replaced through a mod's own `resources/` folder, as on PC.

## Playing on PC

The `gl` profile opens a 960x540 window with keyboard (WASD moves, arrows
shoot, E bomb, space item) and the first gamepad through the game's XInput.
Closing the window quits normally.

Without mods, the upstream game bundle is the instance:

```sh
ISAAC_NATIVE_WINDOW=1 ISAAC_SAVE_DIR=build/browsing-native/pc-saves \
  build/browsing-native/gl/isaac_native \
  "$U/output/recomp/host/isaac.segs.bin" "$U/.scratch/game-bundle" main
```

With mods, make an instance folder that shares the bundle's resources and
has its own `mods/`:

```sh
I=build/browsing-native/pc-instance
mkdir -p "$I/mods"
ln -s "$U/.scratch/game-bundle/resources" "$I/resources"
cp "$U/.scratch/game-bundle/savedatapath.txt" "$I/"
cp -r "$GAME/mods/." "$I/mods/"
ISAAC_NATIVE_WINDOW=1 ISAAC_NATIVE_MODS=1 ISAAC_SAVE_DIR=build/browsing-native/pc-saves \
  build/browsing-native/gl/isaac_native "$U/output/recomp/host/isaac.segs.bin" "$I" main
```

Add `ISAAC_NATIVE_SWITCH_GLYPHS=1` to see the Switch prompts on PC. Other
variables are listed in [DESIGN.md](DESIGN.md#environment-variables).

## Troubleshooting

`sdmc:/switch/isaac-recomp/browsing-last-run.txt` records each stage of the
last start (settings read, heap state, game started, how it ended). Look
there first.

- **It returns to the launcher at once.** If the log's `end:` line says
  `WRONG_MODE`, the launcher is in applet mode: start it by holding R while
  launching a game, or use Sphaira as a title.
- **"The software was closed because an error occurred" right at launch,
  after another homebrew ran.** Older builds of this NRO left GPU memory
  behind in hbloader's shared heap; current builds release it and fence off
  what other NROs leave. If it still happens, close the launcher completely
  (HOME → Close) and start it again.
- **Play is grey and says "NO GAME FILES".** Copy the files it names to `guest/`
  from your PC install. If the check is wrong about your card, `check_game_files=0`
  in `settings.ini` turns it off, and the log lists what it found.
- **It stops while booting.** Check that the game files are under `guest/`
  exactly as in the layout above (`guest/resources/packed/…`, not
  `guest/The Binding of Isaac Rebirth/resources/…`).
- **Vade Retro crashes at room clear in Greedier.** The room-clear fix passed
  the normal, boss, and optional angel waves in a desktop Greed run. The
  original Greedier report still needs a Switch test. If it recurs, keep
  `browsing-last-run.txt` and the game's `log.txt` and record the seed, wave,
  item use, and enabled mods.
- **A mod does not appear.** Its folder needs `metadata.xml` at the top
  level, directly under `mods/<folder>/`.
- **Mod settings are gone.** Copy the mod's data folder into
  `saves/data/<directory>/` (see [Adding mods](#adding-mods)).
