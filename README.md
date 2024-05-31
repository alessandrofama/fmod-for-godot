# FMOD for Godot

[![Build](https://github.com/alessandrofama/fmod-for-godot/workflows/Build/badge.svg)](https://github.com/alessandrofama/fmod-for-godot/actions/workflows/build_all.yml) [![Discord](https://img.shields.io/discord/1083520667451920394?label=Discord)](https://discord.gg/XQbvRdQcNn)

# <em>This project is no longer maintained and is not under active development.</em>

Welcome! This repository provides an integration of [FMOD Studio's audio middleware](https://www.fmod.com/) into the [Godot game engine](https://godotengine.org). At the moment, **FMOD version 2.02.17** and **Godot 4.1.1 stable** are supported.

<img src="https://alessandrofama.com/wp-content/uploads/sites/3/fmod-for-godot-1920.png" width="720">

## Features

* Use the FMOD Studio API on different platforms (Windows, macOS, Linux, iOS and Android) with a [GDExtension library](https://docs.godotengine.org/en/latest/tutorials/scripting/gdextension/what_is_gdextension.html) that seamlessly integrates with Godot and GDScript.
* Modify and build the library for any target platform with ease using SCons, a flexible and cross-platform software construction tool.
* Streamlined audio authoring workflow with FMOD Studio Live Update, which lets you sync your audio project with your game in the editor or in a build. Experiment and iterate faster without reloading or recompiling your game.
* Custom blocking I/O implementation that uses Godot's `FileAccess` API. This ensures compatibility across different platforms.
* Custom FMOD nodes, such as StudioEventEmitter3D, StudioEventEmitter2D, StudioBankLoader, StudioListener3D, StudioListener2D, StudioParameterTrigger and StudioGlobalParameterTrigger. Create an immersive audio experience for players with little code.
* Event **Timeline Marker** and **Timeline Beat** callback support to easily synchronize your game with your audio's logic. You can use these callbacks to trigger animations or gameplay events based on the timing of your audio events.
* Generate unique identifiers (GUIDs) for GDScript to maintain organization and consistency in your project. GUIDs will help you reference FMOD assets without hard-coding paths or names. 
* Search and preview Events in the Godot editor with the **FMOD Project Browser**. This handy tool lets you browse through all the Events in your project and play them back directly in the editor.
* Customize **FMOD settings** for each platform in the Godot Project Settings. You can adjust settings such as **sample rate**, **speaker mode**, **real and virtual channels**, **DSP buffer size** and more to optimize performance and quality for each platform.
* Studio API wrapper unit tests ensuring a reliable and stable integration.
* Custom performance monitors to track the performance of FMOD in the editor.

## Getting started

If you are a game developer that would like to integrate FMOD into your Godot project, go to the [releases](https://github.com/alessandrofama/fmod-for-godot/releases) page to download the addon and read the [getting started](https://alessandrofama.com/tutorials/fmod/godot/getting-started) guide.

If you would like to modify the code of the FMOD addon or build it yourself, visit the short [Building](https://alessandrofama.com/tutorials/fmod/godot/building) tutorial for build instructions.

## Tutorials

Extensive tutorials for the integration, including custom nodes usage and code examples can be found on the [tutorials](https://alessandrofama.com/tutorials/fmod/godot/) section of my website. 

## Known issues

Known issues are being logged into the [issues page](https://github.com/alessandrofama/fmod-for-godot/issues) of this repo. Please check this page first, then log a new issue if you haven't found it listed.

## Contributing

Any contribution is welcome. Please send pull-requests with fixes or useful additions!

## License

The integration files, code and scripts in this repository are released under the [MIT license](https://github.com/alessandrofama/fmod-for-godot/blob/master/LICENSE).

While this project is licensed under MIT, FMOD itself is not free and has different license schemes depending on the type and budget of your project.

You can find out more about FMOD licensing [here](https://www.fmod.com/licensing). Please make sure to obtain a license for your commercial game.
