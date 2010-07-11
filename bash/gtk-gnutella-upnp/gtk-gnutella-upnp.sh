#!/bin/bash
upnpc="$(which upnpc)";
gnet_port="$(grep listen_port ~/.gtk-gnutella/config_gnet | grep -o [0-9]* | head -n 1)";
test "$gnet_port" -gt 1 || upnpc=true;
"$upnpc" -r "$gnet_port" TCP "$gnet_port" TCP;
"$upnpc" -r "$gnet_port" UDP "$gnet_port" UDP;
echo -e '\n======================= GTK-GNUTELLA =======================\n'
echo "listen_port=$gnet_port"
gtk-gnutella $@;
echo -e '\n===================== GTK-GNUTELLA-END =====================\n'
"$upnpc" -d "$gnet_port" TCP "$gnet_port" UDP;
