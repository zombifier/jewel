# Jewel Tracker Player

![Main GUI](https://raw.githubusercontent.com/zombifier/jewel/master/pic.png)

A simple music player using libopenmpt to play tracker files. Currently includes support for notes visualization, muting individual channels and changing tempo for faster/slower playback. The GUI uses Qt, in particular Qt Quick Controls 2 with a purple Material Theme.

## TODO:
- Make the song progress bar not overshoot in case of looping songs (quite tough, since not even openmpt::module::get\_position\_seconds does this)
- Prettier GUI
- More features
- Bug testing
