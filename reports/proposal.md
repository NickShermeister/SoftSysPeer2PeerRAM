# Peer2PeerRAM (Peer to Peer RAM Sharing)
Prava Dhulipalla , Nick Sherman, Nick Steelman, Nate Sampo
https://github.com/NickShermeister/SoftSysPeer2PeerRAM
## Project Goals
Our aim for this project is to create a system that takes advantage of the fact that often the RAM of other machines on the same network as a computer is quicker to access than that computer’s disk drive.  

Thus a computer that runs out of memory can outsource storage to free computers on the same network rather than paying the expensive operation of going to disk.  

A MVP would be an application that can accept data from a machine on a network and route that data through a central server which will dictate where that data is stored form the list of available machines. If completed successfully, we would then build on this to check latency of reads and and streamline it to have as little latency as possible.  
## Personal Learning Goals
As a team, we identified a list of goals that some or all of us would like to achieve. The full list is as follows:  
- Learn how to network
- Prove that Peer to Peer Ram sharing is what everyone with a HDD should use
- Learn how to code while working with multiple different systems
- Learn more and get practice with about multithreading/concurrency
- Practice coding with a full team of software engineers
- Practice good git hygiene
<!--
 Have the most poppin’ Trello board
 No puns allowed throughout the duration of this project
-->

## Resources Needed
A lot of our first set of resources will come from prior projects in this class or other projects that we have located. A list of them are:
- https://github.com/shrutiyer/SoftSysServerClient/blob/master/reports/report.md
- https://github.com/ericasaywhat/SoftSysNybbleRoom/blob/master/reports/report.md  

Additionally, we need resources in learning how to set up servers and in multithreading. The resources we have for server setup include:
- https://ops.tips/blog/a-tcp-server-in-c/
- https://greenteapress.com/wp/semaphores/  

Some other resources that we found that might help us are:  
- This introduction on concurrent servers: https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/  
- This introduction on coding a TCP/IP Stack: http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/
- Think OS: http://greenteapress.com/wp/think-os/  

The final resource we need is something to prove that this is a good idea. That resource was given in class and can be found at:
- https://people.eecs.berkeley.edu/~rcs/research/interactive_latency.html

## First Steps
As we are a larger group of 4, we are splitting up into two smaller groups in order to pursue initial tasks. Therefore, there are two areas of focus for our project.
- Networking -- Nate Sampo & Nicholas Sherman
 - Determine potential ways to host a server and to connect with clients. This can be considered done once we have at least three different types of ways to connect servers with clients.
 - Finalize which protocol we will use for server-client communication. This can be considered done once we have a single protocol we will be using and all information regarding that protocol so we can implement it.
 - Create and host a server and prove that it can be accessed from another computer. This is considered done if we can ping a server we create from another computer.
 - Create a client that interfaces with the Server. This is considered done if we have a client that is able to pass data back and forth to the already created server.
- Multithreading -- The Nick Steelman & Prava
 - Figure out how to quickly process storage requests and choose from a list of possible RAM donors. This is considered done when we have chosen/implemented a way to multiple process requests effectively.
 - Figure out how to store data, what data is stored where, and quickly access this to efficiently process data retrieval requests. This is considered done when we have a data storage/retrieval system that works relatively fast.
 - Figure out how to handle threading in the system so that data is safe and modified correctly. This is considered done when we have mapped out how we want our necessary functions to lock.
