# foo_np_adv
Based on the popular foobar2000 [Now Playing Simple](http://skipyrich.com/wiki/Foobar2000:Now_Playing_Simple) plugin, this one offers the same functionality with one key difference: multiple instances can be defined, each with its own unique set of options. First instance may write what is currently playing, second logging playback, third writing a progress bar and so on..

Since [NPS](http://skipyrich.com/wiki/Foobar2000:Now_Playing_Simple) source was nowhere to be found, this had to be written from scratch. The project is still in its early stages. If you find any problems please open an issue [here](https://github.com/nixnodes/foo_np_adv/issues). If you want to submit a pull request, feel free to do so.

Only tested on 1.4 so far.

## Features
* Multiple instances
* Logging mode
* Write delay
* Write on exit
* Asynchronous writes
* Import/export configuration in JSON format
* Output to clipboard
* Character encoding
  - [ANSI](https://en.wikipedia.org/wiki/Windows_code_page#ANSI_code_page)
  - [UTF-8](https://en.wikipedia.org/wiki/UTF-8) (header optional)
  - [UCS-2](https://en.wikipedia.org/wiki/Universal_Coded_Character_Set) (header optional, big/little endian)
  - [OEM](https://en.wikipedia.org/wiki/Windows_code_page#OEM_code_page)
  - [MAC](https://en.wikipedia.org/wiki/Code_page#Macintosh_emulation_code_pages_2)
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
  - `%playing_playlist_name%` (playlist from which playing items are taken from)
  - `%active_playlist_name%`
* Functions
  - `$strftime('<format>')` - custom formatted time (see [here](http://www.cplusplus.com/reference/ctime/strftime/) for format specifiers, modifiers not supported)
 
## Examples
#### Basic now playing
Format: `$if(%isplaying%,$if(%ispaused%,||,|>) $if($meta_test(artist,title),%artist% - %title% ,%title%))`  
Events: New track, Playback stop, Playback pause  
Output:  
  - Playing: `|> Muzzy - Junction Seven`
  - Paused:  `|| Muzzy - Junction Seven`
  - Empty when stopped
  
#### Playback logging
Format: `$strftime('[%d/%m/%y %T]') $if($meta_test(artist,title),%artist% - %title% ,%title%)`  
Events: New track  
Options: Log mode enabled, Changes only  
Output: `[10/9/18 12:00:01] Muzzy - Junction Seven`

#### Progress bar with time
Format: `$if(%isplaying%,$progress(%_time_elapsed_seconds%,%_time_total_seconds%,20,$if(%ispaused%,|,#),'=') %playback_time%/%length%)`  
Events: New track, Playback stop, Playback pause, Playback time  
Output:
  - Playing: `==============#===== 3:40/5:05`
  - Paused:  `==============|===== 3:40/5:05`
  - Empty when stopped  

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
