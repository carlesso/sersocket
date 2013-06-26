SerSocket
=========

SerSocket is a QT5 implementation of a Serial/Socket proxy. It simply creates a TcpSocket, opens a Serial Port and cross-connect the Signals/Slots.

It's basically a QT porting of [SerProxy](http://www.lspace.nildram.co.uk/freeware.html) project.

That's it. Pretty simple. Any (custom) number of clients can be connected in the same time (the logs at level debug will show the number of clients connected
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

# First group of binding, you can add as many as you want. Listed valutes are the defaults
[proxy1]
tcp_port = 5331               # Tcp Port
max_tcp_clients = 0           # Max number of clients allowed, 0 is unlimited
serial_port = /dev/ttySP0     # The Serial Port device, should be something like /dev/ttyS* or /dev/ttyUSB* if using a USB-Serial converter (pl2303 tested)
baud_rate = 115200            # The BaudRate of the Serial Port
parity = N                    # The Parity to use, can be [N]one or [E]ven
stop_bits = 1                 # StopBits of the serial port
```
