## TCP Simultaneous Open
Implementation of [TCP Simultaneous Open](https://ttcplinux.sourceforge.net/documents/one/tcpstate/tcpstate.html#:~:text=Simultaneous%20Open,active%20open%20on%20both%20sides%22.) in C

### Procedure
1. Prepare two machines that are not behind [Symmetric NAT](https://en.wikipedia.org/wiki/Network_address_translation).
2. Check the external IP address and port of each with the following command:
  ```
  $ gcc ./src/tcp-stun/client.c
  $ ./a.out 44444 18.191.223.12 3478
  ```
  This example uses stun.stunprotocol.org:3478 as STUN server. You can use whatever servers as long as it's available on TCP (though STUN Server supporting TCP is not really common). You can also specify other local ports than 44444.

3. Run src/main.c with the local port, peer's external address and port
  ```
  $ gcc ./src/main.c
  $ ./a.out 0.0.0.0 [Local Port] [Peer's External Address] [Peer's External Port]
  ```

When the connection is successfully established the following prompt will be shown:
```
Waiting for xxxxx microseconds
Connecting...
Connection Established
Received: Hello :)
```

### Environment
Ubuntu, FreeBSD, MacOS have been tested so far
