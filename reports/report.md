# Peer 2 Peer RAM Sharing

## Nicholas Colby Sherman, George Nicholas Steelman, Prava Dhulipalla, Nate "Not here" Sampo

## Project Summary

You know what's wild? The RAM on other machines on the same network is closer than your disk.

Our aim for this project was to create a system that takes advantage of this fact for better storage. If your computer runs out of memory, in a likelihood there are plenty of computers on the same network that have free space.

Thus, with a proper system, a computer that runs out of memory can outsource storage to free computers on the same network rather than paying the expensive operation of going to disk. This project was supposed to be faster than accessing a HDD, but was expected to be slower than accessing a SSD. Upon completion, we found that it was in fact significantly slower than a SSD but that it was at least 10x faster than accessing a HDD.

## Breakdown
When implementing this project, there are 3 key players that make this interaction possible:
- The computer that requests data storage
- The computer that handles requests and routes data to the correct place
- The computer that offers its memory for use

The interactions break down as follows:
![Code Diagram](p2p.png)

### Requester
**Key Functions**
- become_requester: DESCRIPTION
- store: DESCRIPTION
- retrieve: DESCRIPTION

### Server
**Key Functions**
- add_requester: DESCRIPTION
- add_donor: DESCRIPTION
- store: DESCRIPTION
- retrieve: DESCRIPTION

### Donor
**Key Functions**
- become_donor: DESCRIPTION
- store: DESCRIPTION
- retrieve: DESCRIPTION

## Implementation Challenges
When implementing this project, there are two main challenges that should be considered: that of networking and that of the multithreading required to host a viable server.
### Networking
Networking was done through the Transmission Control Protocol (TCP). This protocol is the most commonly implemented in networking as it is very reliable and ensures that packets are read in order. The server was constantly running, and clients who wanted to store data or act as a data host would query the server accordingly. If the client wanted to be a data host, they would be saved as a donor so that the server could reference it in the future. If a client wanted to have data hosted, they would be added to the hosts dictionary and then given a reference number so they could later request their data back. The overarching data structure (what we called the "server") can be seen below.

```c
typedef struct {
  hashmap* requestors;
  hashmap* donors;
  hashmap* locations;
}server;
```

We tried to make the user experience as intuitive as possible, however anyone who wants to interact with the server much know the current IP address that the server is being hosted on and will be prompted for it.

## Threading

## Resources
A lot of our first set of resources came from prior projects in this class or other projects that we have located. A list of referenced projects are:
- https://github.com/shrutiyer/SoftSysServerClient/blob/master/reports/report.md
- https://github.com/ericasaywhat/SoftSysNybbleRoom/blob/master/reports/report.md  

Additionally, we needed resources when learning how to set up servers and in multithreading. The resources we used for server setup include:
- https://ops.tips/blog/a-tcp-server-in-c/
- https://greenteapress.com/wp/semaphores/  
- Beej's Guide https://beej.us/guide/bgnet/html/multi/index.html

Some other resources that we found that helped us are:  
- This introduction on concurrent servers: https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/  
- This introduction on coding a TCP/IP Stack: http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/
- Think OS: http://greenteapress.com/wp/think-os/  
- Head First C
- StackOverflow

The final resource we needed was something to prove that this is a good idea for people to implement. That resource was given in class and can be found at:
- https://people.eecs.berkeley.edu/~rcs/research/interactive_latency.html

## Reflection

Our learning goals for this project were to:
- Learn how to network
- Prove that Peer to Peer Ram sharing is what everyone with a HDD should use
- Learn how to code while working with multiple different systems
- Learn more and get practice with about multithreading/concurrency
- Practice coding with a full team of software engineers
- Practice good git hygiene
- Practice good error handling

In terms of the lower and upper bounds identified in the project proposal, our project landed settled down somewhere in-between. As mentioned, the goal of actually implementing the 'Peer to peer RAM sharing' using networks and servers was fulfilled
