# C&C Bot and Server for BSY

This is simple bot and server/commander implementation for BSY.
The bot and server communicate using IRC. The communication is
first base64 encoded and then [rot8000](http://rot8000.com/) is applied
to make the communication look like chinese. Even though this method
is very simple it provides at least some default level of seccurity
by obscurity and the commands can not be direcctly seen.

The bot can execute arbitary commands on the host machine and thus is
limited only by the bash skills of commander.
