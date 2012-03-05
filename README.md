
Intent and Purposes
===================

This application is meant to serve two purposes:

1) interface IB/TWS with Microsoft Office Excel 2007/2010/2011/onwards via TCP (remote or local): Excel can use 'web queries' to keep its copy of the collected data up to date in 'almost real time'.

   We don't want to depend on COM or other Windows specifics as we intend to port this bugger (and the rest of the trade environment) to UNIX.

   Our experiments have shown that Excel is a very useful tool, but is a lot of bother when used as part of a real-time trade system; it's nice for analyzing data, but the trade logic and trade decisions require another type of flexibility and _speed_ of operation, which leads us to:
   
2) collect stock, etc. data, process it using public and proprietary means and derive trade decision hinting and activity from that.

   As we want a 'one click' type of user process, the easiest way to accomplish that is integrate the entire collect-analyse-advise-trade pipeline in this application, so that a user can easily control it via a web or thin application UI interface.

   The technical 'tough part' is migrating all our existing logic / glue / process to this application, while ensuring we adhere to the 'should also run on UNIX' portability requirement as closely as possible: in the end, we want to be able to run the process on both Win32/64 and UNIX platforms without any loss of functionality either way.


R & D
-----

As we want to 'eat our own dog food' future R&D will largely be founded on top of this software; this will also ensure that we'll be able to detect (and fix) bugs at an early stage in the development process with minimal transition risk from development to production environment.



TODO / Roadmap
--------------

- prioritizing outgoing requests, e.g. ORDERs have priority over historical data requests, and requests for recent historical data have priority over requests for older data.

- use a priority queue for the above plus a 'idle time' delay to prevent hammering the TWS machines with historical data requests: only fire those when the interface has been 'quiet' for X seconds

- store/cache historical data; use 'smart' code to request consecutive and _large_ chunks of historical data to be cached: one request, served many times (from local cache)

- async TWS TX/RX: push requests asap, using a 'telnet' TCP setting (you don't want orders to wait for a TCP buffer fill timeout!): single thread/connection connected to TWS,
  all requests are posted in a 'response queue' (so we know which responses are for whom) upon transmission --> true full duplex communication instead of the standard TWS sample
  which uses the TCP connection as a half-duplex connect (as it waits for the response to the request before firing another).

- as you can be (almost) certain that a single TWS backend connect is handled in a single thread/task at the backend, it is opportune to have two connections open to TWS:
  one for scanner/historical/misc. data and one for real-time data. This should ensure that the real-time ticker data arrives ASAP at our location.
