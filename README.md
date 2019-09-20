# i3x-status
**i3x-status** means "extensible i3status".

[i3status](https://github.com/i3/i3status) has done a great job on generating status bar for i3bar, though to extend the functionality is rather complex.

Here i3x-status struggles to hold as a server provided with basic info, and read info sent by other extensions by FIFO or something else.

## Implementation and Usage

**i3x-status** natively support basic status info including battery and time. Besides, it read from the FIFO "/tmp/i3x.status" with option, O\_NONBLOCK. The protocol for FIFO is very simple as "{name}|{info}\n".

So if extra status info is needed, just write another script to write the info into the FIFO.

### Example

- [spotify playtrack](https://github.com/inhzus/linux-config/blob/master/scripts/spotify-status.py)

- [volume control](https://github.com/inhzus/linux-config/blob/master/scripts/volume.sh)

## Consideration

### Why start the project

The original idea was that I wanted to show the Spotify playtrack in the status bar. Then I figured out how the i3bar and i3status work. However, the project of i3status is not that friendly to make additional functionality. So I came up with the idea of making a extensible status bar.

### FIFO communication isolation

As mentioned in the book, Unix Network Programming, Volume 2: Interprocess Communications, the atomicity of *write* operation is guaranteed if the size of buffer is less than PIPE\_BUF. According to the standard manual, PIPE\_BUF must be greater than 512 bytes and always between 1024 to 5120 bytes. The info sent to the FIFO is always less than the threshold, hence it is undoubted that atomicity is guranteed.
