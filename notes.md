# Study Note

## What is a socket.

It's a way to talk to other programs using standard Unix file descriptors. When Unix do any I/O, they do it by writing to a file descriptors. File descriptors is an integer associated with an open file. The file can lowk be anything tho. Everything in UNIX is a file.

To get a file descriptor for network communication, you call socket() system routine. It returns the scoket descriptor and you communicate through it using specialised send() annd recv().

They are a lot of kind of socker such as DARPA Internet addresses, path names on a local node, CCITT X.25 and many more but we just gonna deal with Internet sockets.

### 2 Types of Internet Sockets.

There is 2 (but Raw Socket is powerful) which is:
  - Stream Sockets (SOCK_STREAM)
    - Reliable two-way connected communication streams. 
    - They will certainly be error free.
    - Lowk tho is this TCP.
    - Used by telnet or ssh apps.
    - Web browser used Hypertext Transfer Protocol which uses stream sockets to get pages.
    - You need them in order.
    - If you telnet to a web site on port 80 and type "GET / HTTP/1.0" and hit RETURN twice, it'll dump the html back at you.
  - Datagram Sockets (SOCK_DGRAM)
    - Connectionless, Unreliable
    - If you send datagram, it may arrive but it may arrive out of order and you don't need to maintain open connection compared to stream socket.
    - Used either when TCP stack is unavailable or a few dropped packets here and there don't mean the end of the universe.
    - Sample app:
      - TFTP (trivial file transfer protocol)
      - dhcpcd (a DHCP client)
      - multiplayer games
      - streaming audio
      - video conferencing
    - tftp and dhcp have their own protocol on top of udp.
      - TFTP protocol sent an ACK package to acknowledge that they got the original packet.
      - If they dont got it, they retransmit.
    - other unreliable app, you lowk just ignore it.
    - Its faster, thats why they used it for streaming and game.

### Low level nonsense and Network theory.

When you wanna send a packet, you wrapped it with a first protocol (TFTP i.e) before you wrapped it with the next protocol (UDP) which then wrapped in IP protocol which is wrapped in the final hardware protocol. 

When another computer received said packet, it unwrapped it. Stripped it of the Ethernet header (hardware does this), the kernel stripped it IP and UDP headers and the TFTP program stripped TFTP header to get the data.

Layered Network Model:
Physical -> Data Link -> Network -> Transport -> Session -> Presentation -> Application.

Application Layer (Telnet, ftp, etc)
Host-to-host Transport Layer (TCP, UDP)
Internet Layer (IP and Routing)
Network Access Layer (Physical/Hardware Layer)

All we have to do to stream socket is send() the data out. All you have to do for the datagram socket is encapsulate the packet in the method of your choosing and sendto() it out. Kernel build Transport and Internet Layer and Hardware does the Physical Layer

## IP Addresses, structs and Data Munging.

We used to use IPv4 but since theres more devices than atom in universe know, we pivot to IPv6.

### Subnets

For organizational reasons, it sometimes convenient to declares that "the first part of IP address up through this bit is the network portion of IP address and the remainder is host portion."

For example:

192.0.100.67 - We could say the first 3 bytes (192.0.100) is the network portion and the last byte (67) is the host. When referring to 192.0.100.67 we are referring to host 67 on network 192.0.100.

They used to be this archaic class of subnets where class A network can only have one byte for network so they have a lot more for their host and so on but this is well archaic.

The network portion of the IP address is described by something called the netmask which you bitwise-AND with the IP address to get the network number out of it. The netmask usually looks something like 255.255.255.0. (E.g with that netmask your ip is 192.0.2.12 then your network is 192.0.2.12 AND 255.255.255.0 which gives 192.0.2.0).

Basically you perform bitwise-AND your ip addess and netmask to get the network ip.

Since theres not enough IP address for Class C, netmask is allowed to be any number of bits. You can see it on CIDR notation for example: 192.0.2.12/30

### Port Numbers

You know this. Its like a local address used by TCP and UDP. 16 bit number that is like a local address for the connection.

### Byte Orders

I don't get it :(

Okay apparently host byte order can be something that is not Big-Endian sometime which meant that we need to convert the byte that come first.

| Function | Description |
| -------------- | --------------- |
| htons() | host to network short |
| hotnl() | host to network long  |
| ntohs() | network to host short |
| ntohl() | network to host long  |

basically you want to convert the numbers to network byte order before they go out on the wire, and convert them to host byte order as they come in off the wire.

### structs

Data types used by socket interface.

A socket descriptor is the following type 

```c
int
```

Just a regular int.

First struct - `struct addrinfo` used to prep the socket address structures for subsequent use. Its also used in host name lookups. That'll make sense later/

```c
struct addrinfo {
  int             ai_flags;       // AI_PASSIVE, AI_CANONNAME, etx.
  int             ai_family;      // AF_INET, AF_INET6, AF_UNSPEC
  int             ai_socktype;    // SOCK_STREAM, SOCK_DGRAM
  int             ai_protocol;    // use 0 for "any"
  size_t          ai_addrlen;     // size of ai_addr in bytes
  struct sockaddr *ai_addr;       // struct sockaddr_in or _in6
  char            *ai_canonname;  // full canonical hostname

  struct addrinfo *ai_next;        // linked list, next node
};
```



