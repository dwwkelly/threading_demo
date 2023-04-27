# Prompt

You're given the task to write an application that receives integers on UDP port 5000 and then sends back the largest prime factor of that number.  Finding the largest prime factor is reasonably computationally difficult (O(sqrt(n)) so it likely the simplest approach is not the best one.

# Client application

The provided client application, written in Python, opens a UDP socket on port 5000 and then starts sending random uint64_t's until the user types CTRL-C

# Naive C Application
The naive C application opens a UDP socket, binds to port 5000, and then waits for integers to factor.  When an integer arrives the application factors it, and then waits for the next one.

![C single threaded CPU utilization]("naive.jpg)

# Threaded C Application

The threaded C application opens a UDP socket, binds to port 5000, initializes a queue, spawns several threads and then waits for integers to arrive.  Each thread runs the Process() function.  The Process() function blocks on `queue_pop()` until there are items in the queue to process.  When there's an item it is pop'ed from the queue processed.

Multiple threads are able to access the queue's location in memory in random order.  In order to prevent the queue from being corrupted this access must be synchronized.  The synchronization technique used here is `pthread_mutex_t`.  Calling lock() and unlock() allows the system to ensure that only on thread at a time is accessing the shared memory.

However, there is a special case that must be accounted for: when the queue is empty.  When the queue is empty all the threads access the queue, find nothing is there and then immediately attempt to access the queue again.  This uses the CPU without acheiving anything useful.  A conditional variable is used to tell the threads to block unwil the non-empty condition is met.

![C multithreaded CPU utilization]("threaded_n4.jpg)

# Threaded Python Application

The threaded python application has the same design as the threaded C application: Bind a UDP socket, setup a thread safe queue, spin off several threads.  However the performance of the python application is significantly worse than the C application, why?  This is due to Python's Global Interpreter Lock (GIL).  The GIL is a lock that all Python processes have that essentially serializes the process.  A single Python process is not able about utilize multicore processes, unlick C/C++.

![Python multithreaded CPU utilization]("python_threaded.jpg)

# Building the Code

There are no significant dependencies for this code.

``` 
make 
```
