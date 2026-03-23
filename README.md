# QtCalculator

## Linux

Install dependencies (Ubuntu/Debian):
```bash
sudo apt install -y cmake build-essential qt6-base-dev
```

Install dependencies (Arch):
```bash
sudo pacman -S --needed cmake base-devel qt6-base
```

Install dependencies (Fedora):
```bash
sudo dnf install -y cmake gcc-c++ make qt6-qtbase-devel
```

Build:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Run:
```bash
./build/main
```

## macOS

Install dependencies (Homebrew example):
```bash
brew install cmake qt@6
```

Build:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6)"
cmake --build build -j
```

Run:
```bash
./build/main
```

## Windows

Install dependencies:
- CMake
- Qt 6 (MinGW 64-bit kit)

Install Qt 6 on Windows:
1. Install Qt Online Installer from qt.io.
2. In the installer, select Qt 6.x and select the kit `MinGW 64-bit`.
3. Make sure the MinGW toolchain is selected in the installer (Tools section).
4. Note the install path, for example `C:/Qt/6.x.x/mingw_64`.

Build (PowerShell):
```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
cmake --build build -j
```

Run:
```powershell
.\build\main.exe
```

