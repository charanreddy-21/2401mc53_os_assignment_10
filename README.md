# Operating Systems — Assignment 10: Memory Management & Paging in xv6

**Course:** Operating Systems

**Student Name:** Konyala Sri Charan Reddy

**Roll Number:** **2401MC53**

**Institution:** Indian Institute of Technology Patna

---

## 1. Overview

This repository contains the completed implementation for **Assignment 10** in **MIT**'s xv6 operating system. The assignment focuses on four key operating system concepts:

1. **Physical Memory Management:** Tracking total allocated, freed, and currently available physical memory pages via `memstats()`.

2. **Virtual-to-Physical Address Translation:** Walking two-level page tables via `vtop()` to map virtual addresses to physical frame addresses with 12-bit offsets.

3. **Page Protection & Fault Interception:** Inspecting page permissions (`pageinfo()`) and enforcing write protection (`setreadonly()`) to dynamically handle hardware protection page faults (`T_PGFLT`).

4. **Page Replacement Algorithms:** Simulating and evaluating **FIFO**, **LRU**, and Optimal page replacement strategies across 3, 4, and 5 frame allocations.

---

## 2. Environment & Prerequisites

- **Target Architecture:** 32-bit x86 (`qemu-system-i386`)

- **Host Platform:** Apple Silicon macOS (macOS Sonoma / Ventura)
- **Cross-Compiler:** `x86_64-elf-gcc` (**GCC** 16.2.0)
- **Emulator:** **QEMU** 11.1.0

### Key Build Flags & Workarounds Applied

- **`-mgeneral-regs-only`:** Added to `**CFLAGS**` in `Makefile` to disable vector/**SSE** instruction generation, preventing `#UD` (Invalid Opcode) exception loops before `**CR4**.**OSFXSR**` setup.
- **Bootblock Stripping:** Added `-fno-asynchronous-unwind-tables` and `-fno-ident` to keep the boot sector within the hard **510**-byte limit while preserving `-j .rodata`.
- **Cross-Platform Script Execution:** Explicitly invoked `perl` (`perl sign.pl`, `perl vectors.pl`) in `Makefile` to bypass macOS quarantine attributes.

---

## 3. Building and Running

### Build Kernel and File System Image

### Launch Interactive Console in QEMU

*To exit **QEMU**, press `Ctrl+A` then `X`.*

---

## 4. Kernel Modifications & System Calls

| System Call | Syscall No. | Core Files Modified | Description |
| --- | --- | --- | --- |
| `memstats()` | 24 | `kalloc.c`, `sysproc.c`, `defs.h`, `user.h`, `syscall.h`, `syscall.c`, `usys.S` | Calculates total allocated pages, total freed pages, current allocated pages, and available free 4KB frames on `kmem.freelist`.

---

## 5. User Programs & Output Verification

All test executables are included in `fs.img` and can be run directly from the xv6 shell prompt (`$`):

### A. Physical Memory Allocation Tracking (`memtest`)

Tests memory tracking by lazily allocating pages via `sbrk()`, touching them to trigger frame allocation, printing statistics, and freeing them via `sbrk(-3 * **4096**)`.


### B. Virtual-to-Physical Address Translation (`vttest`)

Demonstrates address translation for page-aligned virtual addresses, offset preservation, unmapped lazy pages, and out-of-bounds boundaries.


### C. Page Protection & Protection Fault Handling (`pagetest`)

Verifies permission flag inspection, write-protection enforcement (`setreadonly`), and process termination upon write-fault interception.


### D. Page Replacement Algorithm Simulation (`pagereplace`)

Simulates **FIFO**, **LRU**, and Optimal algorithms on reference sequence: `7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`.


---
