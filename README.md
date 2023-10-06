# Kernel-x8086
Preemptive multitasking kernel for operating systems running on x8086 based processors. Concepts: thread, pcb, semaphore, event.


The goal of this project is the realization of a small but fully functional kernel - an operating system that supports threads (multithreaded operating system) with time sharing.


As part of this project, one subsystem of this core will be implemented - the thread management subsystem. This subsystem should provide the concept of threads, as well as the services of creating and starting threads, then the concept of semaphores and events related to interruption, and support for time sharing.


The core should be implemented so that the user program (application) and the core itself share the same address space, that is, they represent a single program. Competitive the processes created inside the application will actually be only "light" processes, ie. threads.


The application and the kernel should be compiled and linked into the same program, i.e. written as "unique" source code.


For the purpose of demonstration and easier development, testing and experimentation, the kernel and the application should be executed on a PC computer under the operating system Windows (any 32-bit), as a console application.
