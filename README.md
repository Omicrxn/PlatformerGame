# Paracelsus

## Description

Fantasy 2D platformer game developed by 3 students at CITM. In the game you are a magician who can control the elements which are learned after defeating the elemental enemies.

## Key Features

 - XML config file loading
 - Load/Save game state using XML file
 - Tiled TMX map loading and drawing (orthographic)
 - Map collisions detection (platforms)
 - Map navigation: player movement and jumping
 - Logo screen with fade-in fade-out
 - Title screen (press ENTER to start)
 - Player walk/jump/die animations
 - Ending screen (on dying)
 - Background with parallax
 - DEBUG keys
 - Walking and Flying enemy types that can pathfind the player.
 - Load/Save includes enemy states.
 - Game is capped to stable 60 frames per second without vsync.
 - Game has all its movement normalized using dt (deltaTime).
 - The player can destroy enemies shooting to them.
 - Game item (heart) to recover lifes.
 - Game collectable item (coin) to accumulate points.
 - Map Checkpoints autosave with feedback and on passing and teleportation.

## Controls

### Player movement

|Key|Action|
|---|------|
|W / SPACE|Jump|
|A|Move left|
|D|Move right|

### DEBUG Keys

|Key|Action|
|---|------|
|F1|Start from the first level|
|F3| Start from the beginning of the current level|
|F5|Save the current game state|
|F6|Load the previous state (even across levels)|
|F9|View colliders / logic|
|F10|God Mode (fly around, cannot be killed)|
|F11|Enable/Disable FPS cap to 60|
|C|Start from the last completed checkpoint|

## Developers

 - Alejandro Ávila [Omicrxn](https://github.com/Omicrxn) - Developer & Designer
 - Bosco Barber [boscobarberesbert](https://github.com/boscobarberesbert) - Developer & Designer
 - Yeray Tarifa [yeraytm](https://github.com/yeraytm) - Developer & Designer

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

### Additional Licenses
The assets' work of this project is licensed under the Creative Commons Attribution 4.0 International License.
For map assets license, check [LICENSE_MAP](/Output/Assets/maps/LICENSE_MAP) for the artists' work link.
