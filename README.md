# i3x-status
**i3x-status** means "extensible i3status".

[i3status](https://github.com/i3/i3status) has done a great job on generating status bar for i3bar, though to extend the functionality is rather complex.

Here i3x-status struggles to hold as a server provided with basic info, and read info sent by other extensions by FIFO or something else.

## Implementation

**i3x-status** natively support basic status info including battery and time. Besides, it read from the FIFO "/tmp/i3x.status" with option, O_NONBLOCK. The protocol for FIFO is very simple as "{name}|{info}\n".

So if extra status info is needed, just write another script to write the info into the FIFO. There has been a spotify status [example](https://github.com/inhzus/i3x-status/blob/master/scripts/spotify-status.py) in the directory *scripts*.
