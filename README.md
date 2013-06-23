SerSocket
=========

SerSocket is a QT5 implementation of a Serial/Socket proxy. It simply creates a TcpSocket, opens a Serial Port and cross-connect the Signals/Slots.

That's it. Pretty simple. Any number of clients can be connected in the same time (the logs at level debug will show the number of clients connected
at every connect/disconnect message).


Compile
=======

Run qmake into project root, it will create a Makefile, then run make.

Usage
=====

```
sersocket [-f /path/to/serconfig.ini]
```

Configuration File
==================

The File configuration, in .ini format (parsed by [QSettings](http://qt-project.org/doc/qt-5.0/qtcore/qsettings.html))

```
# Log File configuration
# defaults to STDOUT
log_file = /var/log/sersocket.log

# Log Level
# defaults to 1
# 0 Debug, 1 Info, 2 Warn, 3 Error, 4 Fatal
log_level = 0

# First group of binding, you can add as many as you want
[proxy1]
tcp_port = 5331
serial_port = /dev/ttySP0
baud_rate = 115200
parity = N
stop_bits = 1

```
