# QKernel - Tiny Educational Kernel

This is a hobby kernel used for educational purposes: it will be a small monolithic kernel that I can use to learn more about operating systems and C/C++. Not meant for external usage outside QEMU.

Currently a big work in progress (only x86 supported and many stubs).

## Roadmap / Notes

### Stage 1/3

Focus on reading through the wiki material. Become familiar with terminology.

```
- [x] Learn some operating systems theory
- [x] Learn enough x86 assembly (Intel or AT&T) to be comfortable
- [x] Create the file stubs / structure the directories
- [x] Get the build system setup (Makefile)
- [x] Finish the 'Barebones' and 'Meaty Skeleton' examples
- [x] By the end we should print "Hello, World" in freestanding mode
```

### Stage 2/3

Stage 2 will focus on getting something to work beyond the basic tutorial. First, we need to place some constraints so this project does not exceed its capabilities:

```
- No POSIX compatibility
- Start with x86
- No support for real devices or drivers
- No UEFI support
- No Unicode support
- No VGA or frame-buffer support; only terminal I/O
- Nothing too complicated (keep it simple)
```

Then focus on getting these done:

```
- [x] Switch over to CMake
- [x] Get the build system to work with QEMU and Grub2
- [x] Create a standard library (one for kernel and one for user-space)
- [x] Global C++ constructors/destructors
- [x] Disable exceptions
- [x] Add undefined behavior sanitizer
- [x] Heap memory allocation via new/delete
- [x] Logger inspired by Qt6
- [x] Get I/O working for the shell using serial ports
- [x] Enable FPU (math coprocessor)
- [x] Enable conversion from integers/floats/doubles to strings
- [x] List CPU features using CPUID
- [x] Complete a basic kernel-space shell
- [x] Handle CPU exceptions and IRQs
- [x] Setup GDT and IDT
- [x] Get interrupts working
- [x] Setup basic system call interface
```

### Stage 3/3

Complete the project by adding user-space, multitasking, privileges, file systems, etc... This will most likely take a very long time.

```
- [ ] Support more complicated memory layouts (paging)
- [ ] ELF parser/loader for executables
- [ ] IDE controller / ATA driver
- [ ] Actually implement the sys calls instead of using stubs
- [ ] Add graphics support
- [ ] Virtual file system
- [ ] FAT32 filesystem support
- [ ] Persistent disk storage
- [ ] Setup privileges
- [ ] Legacy power management via APM
- [ ] Multi-process architecture
- [ ] Date/time support
- [ ] Add networking support
- [ ] Thread-safety
- [ ] Should implement some clones of GNU Core Utilities
- [ ] Upgrade to Multiboot2 or use Limine with Stivale2
- [ ] Basic networking support
- [ ] Enter long mode
- [ ] Port code to x86_64 and aarch64
- [ ] SATA driver
- [ ] Ability to display some graphics
- [ ] UEFI support
```

## Dependencies

- Clang
- lld
- Grub2
- QEMU

## Build and Test

```
git clone https://github.com/kaiyanlee/qkernel
mkdir build
cd build
cmake ..
make && make start-kernel
```

## Credits

This project was bootstrapped by reading wiki.osdev.org and going through the Meaty Skeleton example. I then studied other resources:

- [wiki.osdev.org](https://wiki.osdev.org/)
- [The Little Book About OS Development](https://littleosbook.github.io/#the-serial-ports)
- [fwsGonzo/barebones](https://github.com/fwsGonzo/barebones)
- [SamyPesse/How-to-Make-a-Computer-Operating-System](https://github.com/SamyPesse/How-to-Make-a-Computer-Operating-System)
- [cfenollosa/os-tutorial](https://github.com/cfenollosa/os-tutorial)
- [AlgorithMan-de/wyoos](https://github.com/AlgorithMan-de/wyoos)

## License

The parts written by me have been licensed under GPLv3; I do not own the code written by others.
