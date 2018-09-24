# foo_np_adv
Based on the popular foobar2000 [Now Playing Simple](http://skipyrich.com/wiki/Foobar2000:Now_Playing_Simple) plugin, this one offers the same functionality with one key difference: multiple instances can be defined, each with its own unique set of options. First instance may write what is currently playing, second logging playback, third writing a progress bar and so on..

Since [NPS](http://skipyrich.com/wiki/Foobar2000:Now_Playing_Simple) source was nowhere to be found, this had to be written from scratch. This project is still in the early stages. If you experience any issues or want your commit merged, feel free to submit [here](https://github.com/nixnodes/foo_np_adv/issues).

## Features
* Multiple instances
* Logging mode
* Write delay
* Write on exit
* Asynchronous writes
* Character encoding
  - [ANSI](https://en.wikipedia.org/wiki/Windows_code_page#ANSI_code_page)
  - [UTF-8](https://en.wikipedia.org/wiki/UTF-8) (header optional)
  - [UCS-2](https://en.wikipedia.org/wiki/Universal_Coded_Character_Set) (header optional, big/little endian)
* Trigger events
  - Playback start/pause/seek
  - Playback stop (except when changing track)
  - Playback time (once per second)
  - New track  
  - Track edited
  - Dynamic info
  - Volume change
* Variables
  - `%volume%` (dB)
  - `%datetime%` (full date and time)
  - `%time%` (time only)
* Functions
  - `$strftime('<format>')` (see [here](http://www.cplusplus.com/reference/ctime/strftime/) for format specifiers, modifiers not supported)
  
## Installation
Download the latest zipped release from [here](https://github.com/nixnodes/foo_np_adv/releases).
* Manual
  - Unzip dll to `<foobar install path>\components`.
* Via foobar preferences
  - Open `Components` page, click `Install`, select the zip file and click `Apply`.

## Compiling
VS2017 project files are included
* Clone the repository and all submodules
  - `git clone --recurse-submodules -j8 https://github.com/nixnodes/foo_np_adv`
* Make sure v141_xp toolset for Visual Studio is installed
* Load `foo_np_adv/foo_np_advanced.sln` and build 
