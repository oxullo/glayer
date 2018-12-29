# Glayer

AKA a kitty mp3 player for kiddos.

![glayer](https://raw.githubusercontent.com/oxullo/glayer/master/media/glayer.jpg)

Glayer is an MP3 player with a minimalist interface suitable for kids.

Playlists are selected using RFID badges, while tracks can be skimmed with two buttons.

The buttons can be also used to set the volume.

## Components

Revision A is composed of the following components:

* Adafruit Feather M0 (Atmel SAMD21G) basic proto
* Adafruit Music maker featherwing (VLSI VS1053)
* Adafruit TPA2016 amplifier breakout
* Seeed studio NFC grove (NXP PN532)
* Adafruit Arcade button
* Stereo Enclosed Speaker Set - 3W 4 Ohm
* Light barrier (Vishay TCST2103)
* DTP 2000mAh 3.7V LiPo battery
* Mifare classic (ISO14443A) RFID transponder
* Transcend 8GB SDHC

![glayer FBD](https://raw.githubusercontent.com/oxullo/glayer/master/media/fbd.png)

## Firmware

The firmware can be found in the folder _firmware/glayer_ and it can be compiled using [platformio](https://platformio.org).

Most of the constants are defined at the beginning of _main.cpp_.

Check also platformio.ini for build flags that affect the debugging verbosity of the running firmware.

## SD template

The firmware expects the content to be organized in folders. Each folder represents a playlist.

Each RFID has a unique code (UID) which is associated to a specific playlist. Hence the folders must be named with the hexadecimal representation of the RFIDs.

Both folders and files should comply with the 8+3 FAT limitation.

Content must be supported by the VS1053, but at the moment a simple filter prevents any file without "mp3" extension to be loaded into the playlist.

The maximum number of files per playlist is 32 (it can be easily increased).

The files list is sorted alphanumerically.

Example:

- root
    - 12ab45cd
        - 01_foo.mp3
        - 02_bar.mp3
        - 03_baz.mp3
    - deadbeef
        - foobar.mp3
        - whatever.mp3



## Enclosure

The enclosure is custom made of Okoume plywood, then dyed with water-based paint.

A quite critical component is the RFID slot, which has been assembled with tight tolerances in order to offer a pleasant insertion feeling and a sufficient retention force.

Both assemblies are documented in the folder _drawings_

## Disclaimer

These plans are offered primarily for educational purposes and "as is". When building toys for kids, care must be taken in order to avoid potential hazards.

Check LICENSE.txt §16.

