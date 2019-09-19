#!/usr/bin/python
# -*- coding: utf-8 -*-

import dbus
import time
import sys
import signal


fifo = open('/tmp/i3x.status', 'w')
signal.signal(signal.SIGINT, lambda: fifo.write('spotify|\n'))

try:
    session_bus = dbus.SessionBus()
    spotify_bus = session_bus.get_object("org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2")
    spotify_properties = dbus.Interface(spotify_bus, "org.freedesktop.DBus.Properties")

    while True:
        metadata = spotify_properties.Get("org.mpris.MediaPlayer2.Player", "Metadata")
        title = str(metadata[dbus.String('xesam:title')])
        artist = str(metadata[dbus.String('xesam:artist')][0])
        s = 'spotify|{} |> {}\n'.format(artist, title)
#        print(s, end='')
        fifo.write(s)
        fifo.flush()
        time.sleep(5)
except Exception:
    fifo.write('spotify|\n')
    print('exit caused by exception')
