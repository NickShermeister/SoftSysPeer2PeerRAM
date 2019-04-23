# Project Update on Peer 2 Peer RAM Sharing

## Nicholas Colby Sherman, Nate Sampo, George Nicholas Steelman, Prava Dhulipalla

## Goal of the Project

Our aim for this project is to create a system that takes advantage of the fact that often the RAM of other machines on the same network as a computer is quicker to access than that computer’s disk drive.  

Thus a computer that runs out of memory can outsource storage to free computers on the same network rather than paying the expensive operation of going to disk.  

A MVP would be an application that can accept data from a machine on a network and route that data through a central server which will dictate where that data is stored form the list of available machines. If completed successfully, we would then build on this to check latency of reads and and streamline it to have as little latency as possible.  

## Learning Goals

As a team, we identified a list of goals that some or all of us would like to achieve. The full list is as follows:  
- Learn how to network
- Prove that Peer to Peer Ram sharing is what everyone with a HDD should use
- Learn how to code while working with multiple different systems
- Learn more and get practice with about multithreading/concurrency
- Practice coding with a full team of software engineers
- Practice good git hygiene
- Practice good error handling
<!--
 Have the most poppin’ Trello board
 No puns allowed throughout the duration of this project
-->

## Steps Towards Goals
As we are a larger group of 4, we are splitting up into two smaller groups in order to pursue initial tasks. Therefore, there are two areas of focus for our project.

### Networking -- Nate Sampo & Nicholas Colby Sherman
- Determine potential ways to host a server and to connect with clients. This can be considered done once we have at least three different types of ways to connect servers with clients.
- Finalize which protocol we will use for server-client communication. This can be considered done once we have a single protocol we will be using and all information regarding that protocol so we can implement it.
- Create and host a server and prove that it can be accessed from another computer. This is considered done if we can ping a server we create from another computer.
- Create a client that interfaces with the Server. This is considered done if we have a client that is able to pass data back and forth to the already created server.

### Multithreading -- George Nicholas Steelman & Prava
- Figure out how to quickly process storage requests and choose from a list of possible RAM donors. This is considered done when we have chosen/implemented a way to multiple process requests effectively.
- Figure out how to store data, what data is stored where, and quickly access this to efficiently process data retrieval requests. This is considered done when we have a data storage/retrieval system that works relatively fast.
- Figure out how to handle threading in the system so that data is safe and modified correctly. This is considered done when we have mapped out how we want our necessary functions to lock.

## Resources
A lot of our first set of resources will come from prior projects in this class or other projects that we have located. A list of them are:
- https://github.com/shrutiyer/SoftSysServerClient/blob/master/reports/report.md
- https://github.com/ericasaywhat/SoftSysNybbleRoom/blob/master/reports/report.md  

Additionally, we need resources in learning how to set up servers and in multithreading. The resources we have for server setup include:
- https://ops.tips/blog/a-tcp-server-in-c/
- https://greenteapress.com/wp/semaphores/  
- Beej's Guide https://beej.us/guide/bgnet/html/multi/index.html

Some other resources that we found that might help us are:  
- This introduction on concurrent servers: https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/  
- This introduction on coding a TCP/IP Stack: http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/
- Think OS: http://greenteapress.com/wp/think-os/  
- Head First C (we have various physical and electronic copies)
- StackOverflow
- Victoria Marie McDermott's Github

The final resource we need is something to prove that this is a good idea. That resource was given in class and can be found at:
- https://people.eecs.berkeley.edu/~rcs/research/interactive_latency.html

## Progress on First Steps

### Multithreading Subteam
After abstracting and mapping out our required functions we realized that we needed a quick Hashmap implementation for checking requester, donors, and finding locations of data. Since this has to be fast, we eliminated overhead by implementing our own Hashmap representation just just the functions, types and checks that we need.

With the Hashmap implemented, we created functions with appropriate mutexes to add/delete in hashmap of requesters, add/delete in a donors, add/delete from a hashmap of data locations.

With those functions implemented, we developed a single function that accepts incoming requests, parses them to check for type of request, and hands it off to the appropriate function.

At this point, we successfully enabled requesters and donors to add and remove themselves from the server. Requesters can also send a string to the server, who will choose from a list of donors and send the string to the donor who can print it out.

Future stuff:
1. Enable the donors to track and store the data from different places via IDs
1. Enable the server to request the data from the donors
1. Enable the requesters to track the ID of its stored data, request it from the server, and interpret the response.

### Networking Subteam
The networking subteam started by successfully implementing a TCP/IP server using the socket library. We had completed some research in other potential protocols but ruled that this would be the best protocol for the project. After this, we created a basic server that could multithread in order to allow connections to multiple clients. At this point, we handed off the server to the multithreading team. Although we will be aiding in networking, they will be working with the core of the server as it is where all the multithreaded magic happens. After this, we have been working closely with the multithreading team to ensure that we are providing the necessary information and that the client-side view of things remains as simple and robust as possible.

Moving forward, the Networking subteam has two primary goals:
1. Create the structures to be passed between donors and requestors across the network as data packets.
2. Increase error handling. Right now, we have relatively weak error handling, which needs to be immensely improved as we are dealing with networking.

We will also be working closely with Multithreading to continue good server-client relations.

So long, and thanks for reading.
