
# Custom Memory Allocator in C

## Overview

This project is an exploration into the development of a custom memory allocator in C. The allocator overrides standard dynamic memory management functions (`malloc`, `free`, `calloc`, and `realloc`) with custom implementations. This README documents the process, observations, and lessons learned throughout the development and testing phases. It serves as a guide for anyone interested in the inner workings of memory allocators.

## Objectives

- To understand the principles behind dynamic memory management in C.
- To implement custom versions of memory management functions.
- To ensure the custom allocator is thread-safe and memory is correctly aligned.
- To rigorously test the allocator under various conditions for reliability and efficiency.

## Implementation Highlights

- **Custom Data Structures**: Used a header structure to track memory block size and allocation status, ensuring alignment.
- **Thread Safety**: Employed `pthread_mutex_lock` and `pthread_mutex_unlock` to protect global list modifications, addressing concurrency concerns.
- **Memory Alignment**: Ensured allocated memory adheres to architecture-specific alignment requirements for optimal access and performance.

## Challenges and Solutions

### Memory Alignment

Ensuring that allocated memory is properly aligned was critical. Misalignment can lead to inefficient memory access and crashes on architectures that require strict alignment.

### Thread Safety

The allocator uses mutexes to protect its internal structures, making it thread-safe. However, challenges arose with the inherent non-thread-safe nature of `sbrk()`. The solution was meticulous use of locks and considering alternatives like `mmap` for future improvements.

### Heap Management

Determining when to expand or shrink the heap, and managing free blocks efficiently, was complex. Initial versions attempted to return memory to the system aggressively, leading to issues.

## Building the Project

### Prerequisites

- GCC compiler
- GNU Make (optional for building)
- GDB for debugging

### Compiling the Allocator

Compile the allocator into a shared library (`memalloc.so`):

```bash
gcc -o memalloc.so -fPIC -shared memalloc.c -pthread -g
```

This command enables position-independent code (`-fPIC`), thread safety (`-pthread`), and includes debug symbols (`-g`).

## Usage

### Running Applications

To use the custom allocator with an application:

```bash
env LD_PRELOAD=./memalloc.so <application>
```

For example, to use it with `ls`:

```bash
env LD_PRELOAD=./memalloc.so ls
```

### Testing

A test program (`allocator_test.c`) is provided. Compile and run it with the custom allocator:

```bash
gcc -o allocator_test allocator_test.c -g
env LD_PRELOAD=./memalloc.so ./allocator_test
```

## Debugging

If issues arise, such as segmentation faults, use GDB to debug:

```bash
gdb --args env LD_PRELOAD=./memalloc.so ./allocator_test
```

Use `run` to start, and `bt` to get a backtrace if a crash occurs.

## Lessons Learned

- **Debugging Skills**: Enhanced debugging skills, especially in low-level programming and memory management.
- **Memory Allocator Intricacies**: Gained insights into the complexities of designing and implementing a memory allocator, including thread safety and memory alignment challenges.
- **Tool Proficiency**: Improved proficiency with development tools like GDB and the GCC compiler.

## Future Directions

- **Incorporate `mmap`**: Explore using `mmap` for memory allocation to improve thread safety and efficiency.
- **Optimization**: Investigate strategies for reducing fragmentation and improving allocation speed.
- **Comprehensive Testing**: Develop more extensive tests, including stress tests and multi-threaded scenarios.

## Conclusion

This project has been a deep dive into the challenges and considerations of creating a custom memory allocator. It has provided valuable hands-on experience with low-level system programming concepts and debugging techniques.
