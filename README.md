# Ferry Ticket Booking & Management System

A menu-driven, file-persisted command-line system for a ferry operator, written in
standard C11 with no third-party dependencies. Built as a Principles of Programming
course project.

Routes and bookings are held in dynamically allocated arrays of structs that grow via
`realloc` as records are added, and persist to binary data files between runs.

## Features

**Admin mode**

- Add a new ferry route (rejects duplicate route IDs, empty island names,
  non-positive prices, and non-positive capacity)
- Update an existing route (refuses to shrink capacity below the number of
  seats already booked)
- Remove a route — refused while the route still has active bookings
- View all routes as a formatted table
- Search by route ID or by destination (case-insensitive)
- Sort routes by price, destination, departure date, or available seats,
  ascending or descending
- View all passenger bookings
- View a booking summary: total routes, active bookings, tickets sold, revenue
  collected, and the most-booked route

**Passenger mode**

- View, search, and sort available routes
- Book one or more tickets — validates that the route exists, that enough seats
  are free, and that payment covers the total; shows the shortfall if it doesn't
  and the change if it does
- Look up a booking confirmation by booking ID
- Cancel a booking by ID — returns the seats to the route and refuses to
  cancel the same booking twice

All searching and sorting is hand-written (linear search, bubble sort); no library
sort is used.

## Requirements

- A C compiler supporting C11 — GCC, Clang, or MSVC
- CMake 3.16 or newer
- A build tool: `make`/`ninja` on Linux and macOS, or Visual Studio / MinGW on Windows

CMake is the recommended path because it configures the compiler flags and include
paths for you, but the project is six `.c` files and compiles fine with a single
`gcc` command — see [Building without CMake](#building-without-cmake).

## Installing the tools

### Linux

Most distributions ship CMake and GCC in their default repositories.

```bash
# Fedora / RHEL
sudo dnf install cmake gcc make

# Debian / Ubuntu
sudo apt update && sudo apt install cmake build-essential

# Arch
sudo pacman -S cmake base-devel
```

### macOS

Install the Apple command-line developer tools first — this provides Clang and
`make`:

```bash
xcode-select --install
```

Then install CMake with [Homebrew](https://brew.sh):

```bash
brew install cmake
```

If you don't use Homebrew, download the macOS `.dmg` from
[cmake.org/download](https://cmake.org/download/), drag CMake to Applications, and
add it to your `PATH`:

```bash
sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
```

### Windows

You need both CMake and a C compiler. The simplest combination is CMake plus the
Visual Studio Build Tools.

**Using winget** (built into Windows 10/11):

```powershell
winget install Kitware.CMake
winget install Microsoft.VisualStudio.2022.BuildTools
```

When the Build Tools installer opens, tick the **Desktop development with C++**
workload — CMake needs the MSVC compiler that this installs.

**Manual install:** download the Windows x64 installer from
[cmake.org/download](https://cmake.org/download/) and, during setup, choose
**Add CMake to the system PATH**. Then install
[Visual Studio Community](https://visualstudio.microsoft.com/downloads/) or the
standalone Build Tools with the *Desktop development with C++* workload.

**Alternative — MSYS2/MinGW,** if you'd rather use GCC on Windows: install
[MSYS2](https://www.msys2.org/), then from the MSYS2 UCRT64 terminal run

```bash
pacman -S mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make
```

### Verifying the install

Open a new terminal (so `PATH` changes take effect) and check:

```bash
cmake --version
gcc --version     # or: clang --version   /   cl   on Windows with MSVC
```

## Building and running

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

This produces the `ferry_booking` executable.

**Linux and macOS:**

```bash
./build/ferry_booking
```

**Windows** — Visual Studio is a multi-configuration generator, so the executable
lands in a configuration subdirectory:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\ferry_booking.exe
```

With MinGW/MSYS2 the layout matches Linux:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/ferry_booking.exe
```

### Building without CMake

The application is plain C11 with no dependencies beyond the standard library, so a
single compiler invocation works. From the project root:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude -o ferry_booking \
    src/main.c src/routes.c src/bookings.c src/files.c src/utilities.c src/validation.c
```

Substitute `clang` for `gcc` on macOS if you prefer. The equivalent from a
Visual Studio Developer Command Prompt:

```
cl /std:c11 /W4 /Iinclude /Fe:ferry_booking.exe src\main.c src\routes.c src\bookings.c src\files.c src\utilities.c src\validation.c
```

The project compiles warning-free under `-Wall -Wextra -Wpedantic` and `/W4`.

## Data files

On startup the program loads `route.dat` and `bookings.dat`, and it rewrites both
files after every change — adding, updating, or removing a route, and booking or
cancelling a ticket. Nothing is lost if the program is closed between operations.

Both files are **created in whatever directory you launch the program from**, not
next to the executable. Running `./build/ferry_booking` from the project root leaves
the data files in the project root.

The first time you run with no existing `route.dat`, the program seeds five sample
Male'-hub routes and two bookings so the menus aren't empty, and prints
`No existing data found -- loaded sample routes and bookings.` To start from scratch
again, delete both files:

```bash
rm route.dat bookings.dat
```

The files are written as binary records — a record count followed by the raw structs
— so they aren't human-readable and aren't portable between machines with different
architectures or compilers.

## Using the program

The program opens on the main menu and keeps returning there until you choose Exit:

```
1. Admin Mode
2. Passenger Mode
3. Exit
```

Enter the number for your choice and press Enter. Invalid selections and
non-numeric input are re-prompted rather than crashing the program, at every menu
and every field.

A typical passenger booking: choose `2` for Passenger Mode, then `1` to view the
available routes and note a route ID, then `4` to book. You'll be asked for your
name, phone number, the route ID, how many tickets, and how much you're paying. If
the payment covers the total you get a confirmation with a booking ID and your
change; keep the booking ID, since cancelling (option `6`) and viewing the
confirmation (option `5`) both need it.

Choose `3` from the main menu to exit cleanly — this releases all allocated memory.

## Project structure

```
include/          Public headers, one per module
  routes.h          FerryRoute and RouteList structs, route API
  bookings.h        Booking, BookingList, BookingSummary structs, booking API
  files.h           Save/load API and file I/O error codes
  utilities.h       String helpers and interactive input prompts
  validation.h      Parsing and validation predicates

src/
  main.c            Menu loop and display formatting only — no business logic
  routes.c          Add/update/remove/search/sort routes (bubble sort)
  bookings.c        Book/cancel/look up bookings, compute the summary
  files.c           Binary persistence for routes and bookings
  utilities.c       copy_str, read_line, prompt_* helpers, case-insensitive compare
  validation.c      parse_int, parse_double, non-empty and positive checks

CMakeLists.txt    Build configuration
```

Business logic is kept out of `main.c`: every function in `routes.c`, `bookings.c`,
`files.c`, and `validation.c` takes its inputs as parameters and returns a status
code or writes through an output pointer, with no interactive prompting of its own.
`main.c` collects input, dispatches to those functions, and formats the result.
