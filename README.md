getaddr
=======

Tiny IP address utility, using getaddrinfo(3).

This is IP address resolver using getaddrinfo(3).
Getaddr makes you to resolve addresses defined in /etc/hosts, and you can write hostname instead of raw IP address in /etc/rc.conf file.

ex:

for FreeBSD, /etc/hosts

    10.9.8.7	vpn-addr

/etc/rc.conf

    cloned_interfaces="tap0 bridge0"
    ifconfig_bridge0="addm tap0 up"
    ipv4_addrs_bridge0="`/root/bin/getaddr vpn-addr`/24"
    ifconfig_tap0="promisc up"
