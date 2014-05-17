MasterLog
=========

Simple JSON based Reliable Logging Service

1) This is an independent server process with which other processes communicate.

This server is intended to be "Unix Simple": it does this thing that it does, and bells and whistles
must be provided by partner processes.

The other processes exchange JSON objects with this server over a TCP link or over UDP.

This server is Reliable up to a point: it makes no effort to protect against failing disks. Because
disk I/O under Unix is inherently broken, it is also possible to lose a small time-window of data
under power-failure conditions. MasterLog makes no heroic efforts to protect against such loss.
Reliability beyond this point should be provided by Replication, which is easy to configure.

** Reliability may be enhanced at some future release. **

Logging is used in a general sense that includes but extends traditional "/var/log/" logging.

This process Logs each data object received, assigning it a sequence number. It combines all logged
objects into a single log. It provides Logged objects to other processes in sequence, starting
at a selected point either in time or by sequence number.

2) This is an insecure system which should be run only within a protected environment.

A Server may be configured to listen on Loopback (localhost; 127.0.0.1) ports or on TCP ports.
Each individual port may be configured to accept control commands, accept data Objects to Log,
or to provide Logs. It may also be configured to connect to another TCP host:port combination
to provide or request logs.

In these first releases, it is the responsibility of the User to limit access to such ports,
for instance by using Trusted LANs, VPNs, firewall rules, and static routes.

** In the future it may become a secured system, but one thing at a time. **

3) Unless the configuration of the server has been frozen by command, it may be adjusted at any time.

Each server has a set of configurable variables.

When a server launches it applies configuration files and command line arguments in a specified order.
&lt;Exact Order has not yet been specified&gt;. It simply overwrites the configurable variables
with each new configuration received. If at any point the configuration "frozen":true is received
then no new configuration will be accepted after that point.

Only after all configurations have been applied the server will begin opening ports and files and
accepting requests.

Many configurable variables may be modified after launch by use of a control JSON object received
over a control-enabled port.
