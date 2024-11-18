# Power Management System
### A C++ application implementing a First-In-First-Out (FIFO) resource allocation system to manage power distribution across devices in a constrained power supply system.

# Requirements

- Implement FIFO - used queue datastructure
- track power consumption - used global variable to track the power
- redistrubte power - done by iterating through the queue in FIFO order, allocating available power to devices.
- connection of new device - adds the new device at the end of the connection i.e, It is added to the end of the queue.

