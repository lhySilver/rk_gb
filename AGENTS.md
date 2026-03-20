# AGENTS.md - Developer Guide for RC0240 IPC Project

## Project Overview

This is an embedded C/C++ project for RK1106 dual-sensor IPC cameras. The codebase uses CMake as the build system with a custom `build.sh` wrapper script. It targets ARM Rockchip processors with a cross-compilation toolchain.

## Build Commands

### Main Build Script

```bash
# Full build (middleware + app + image)
./build.sh all

# Debug build
./build.sh debug

# Build app only
./build.sh app

# Build middleware only
./build.sh mid

# Clean all build artifacts
./build.sh clean

# Clean and rebuild middleware
./build.sh mid-clean && ./build.sh mid

# Build image only
./build.sh image
```

### Individual Component Builds

```bash
# Daemon (uses Make)
make -C daemon

# Burn tool (uses Make)
make -C burntool
```

### Build Configuration

The default build configuration uses:
- **Board**: RC0240_LGV10
- **Chip**: RV1106_DUAL_IPC
- **BLE**: AIC8800DL
- **Mode**: Release (use `./build.sh debug` for debug)

You can modify these in `build.sh`:
```bash
CHIP_TYPE=RV1106_DUAL_IPC
BOARD_TYPE=RC0240_LGV10
BLE_TYPE=AIC8800DL
DMODE=release  # or debug
```

### Cross-Compilation

The project uses ARM Rockchip toolchain. Source `envsetup.sh` to set up the environment:
```bash
source envsetup.sh
```

This exports `CROSS=arm-rockchip830-linux-uclibcgnueabihf-` and adds the toolchain to PATH.

## Code Style Guidelines

### Formatting

- **Follow existing `.clang-format`** in `Middleware/libmpp/rkipc/.clang-format`
- Key settings:
  - BasedOnStyle: LLVM
  - ColumnLimit: 100
  - IndentWidth: 4
  - TabWidth: 4
  - PointerAlignment: Right
  - Cpp11BracedListStyle: true
  - Standard: Cpp11

### Naming Conventions

- **Functions**: camelCase (e.g., `CreateDetachedThread`, `delete_file`)
- **Classes**: PascalCase (e.g., `CSofia`, `CInfra`)
- **Member variables**: `m_` prefix (e.g., `m_gbClientSdk`)
- **Global variables**: `_g` suffix (e.g., `DeviceMode_g`)
- **Constants**: SCREAMING_SNAKE_CASE
- **Macros/Defines**: SCREAMING_SNAKE_CASE

### Code Organization

- Headers go in `Include/` or component-specific directories
- Source files go in `App/`, `Middleware/`, `daemon/`, `burntool/`
- Use `#ifndef __FILENAME_H__ / #define __FILENAME_H__` guards for headers

### Imports

- System includes first (`<stdio.h>`, `<stdlib.h>`, etc.)
- Project includes second (local headers with quotes)
- Order matters - match existing patterns in each file

### Error Handling

- Return error codes (typically `-1` for failure, `0` for success)
- Check return values explicitly
- Use goto-based cleanup pattern for resource management:
```cpp
int function() {
    int ret = -1;
    resource_t *res = NULL;
    
    res = alloc();
    if (!res) goto EXIT;
    
    ret = do_work(res);
    
EXIT:
    if (res) free(res);
    return ret;
}
```

### Logging

- Use provided logging macros (e.g., `AppErr`, `AppInfo`)
- Check existing files for logging patterns

## Testing

**No unit tests exist in this project.** Testing is done manually on the target hardware.

If adding tests:
- Create test binaries in a `test/` directory
- Use the existing build system (Make or CMake)
- Link against built libraries from `Lib/`

## Project Structure

```
./
├── App/                  # Main application code
│   ├── Common.cpp/h      # Common utilities
│   ├── Main.cpp          # Application entry point
│   ├── Manager/          # Manager modules
│   ├── Media/            # Media handling
│   ├── Net/              # Network code
│   ├── Onvif/            # ONVIF support
│   ├── Rtsp/             # RTSP server
│   └── tuya/             # Tuya SDK integration
├── Include/              # Public headers
├── Middleware/           # Third-party middleware (libmpp)
│   └── libmpp/           # Rockchip media processing
├── daemon/               # Background daemon
├── burntool/             # Flash burning tool
├── driver/               # Kernel drivers
├── third_party/          # External dependencies
└── packaging-*/          # Image packaging scripts
```

## Important Notes

1. **Embedded target**: Code runs on ARM with limited resources. Avoid dynamic memory in critical paths.
2. **No exceptions**: The codebase uses C++98/03 patterns with manual error handling.
3. **Threading**: Uses pthreads. See `CreateDetachedThread`, `CreateDefaultThread` helpers.
4. **Singletons**: Many managers use singleton pattern (e.g., `PATTERN_SINGLETON_IMPLEMENT`).
5. **Configuration**: JSON-based config in `CONFIG_DIR/Json` (typically `/mnt/app/conf/`).

## Known Build Dependencies

- arm-rockchip830-linux-uclibcgnueabihf-gcc/g++
- CMake >= 3.0
- Make
- libevent
- OpenSSL
- zlib
