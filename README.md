**High DPC Simulator**

This driver is intended for learning purposes.  It simulates the behavior of a driver that might induce high DPC load on a Windows system.

**Purpose**

To provide a hands-on example for understanding the concepts of DPCs and to allow IT specialists to experiment with tools and techniques for diagnosing performance issues related to kernel-mode drivers.

**System Behavior**

When this driver is running, you will likely observe the following:

1. Reduced system responsiveness: DPCs and ISRs execute at a high priority level, so excessive workload within these routines can disrupt normal system operations.

1. Spikes in CPU usage: This is caused by the computationally intensive loop within the ExampleDpcRoutine.

1. When the driver is loaded and running, it will periodically execute a high-load computational task in its DPC routine. 

**Understanding DPCs**

A DPC is a mechanism in Windows for deferring lower-priority tasks to be executed at a later time, but still at a higher priority level than most user-mode code. DPCs are frequently used by drivers to handle tasks that must be completed promptly but can't be done directly within the context of an ISR, e.g. allowing hardware interrupts to be processed quickly while deferring other processing to a later time.

**Code Structure**

- The driver entry point (DriverEntry) initializes a timer and a DPC.
- The timer is set, upon which it queues a DPC (ExampleTimerRoutine).
- The queued DPC then performs a computationally intensive task (ExampleDpcRoutine), simulating a high-load scenario.
- The driver ensures proper cleanup by canceling the timer and freeing allocated resources upon unload (ExampleUnloadDriver).

**How to Use**

1. Compile the driver. Using WDK and Kernel mode UMDF framework.
2. Load the driver with service control manager.
3. Observe system behavior using tools such as Task Manager or Windows Performance Recorder/Windows Performance Analyzer.
4. If you see the system freezing, reduce the timer interval or computational loop:
    
```
volatile ULONG64 accumulator = 0;
    for (ULONG i = 0; i < 700000; i++) { // Adjust loop count here
        accumulator += i * i;
    }
```

**License**

This code is licensed under the MIT License.

**Disclaimer**

This driver is not designed for use in production systems. Its purpose is solely to demonstrate concepts for educational exploration.
