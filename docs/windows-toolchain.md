# Building newer C++ 10.2.0 Windows toolchain for longan nano

## Building on Ubuntu

This is how I had success to build newer toolchain. If there is better way to do it, comments are welcome.

It is based on tutorial here: <https://shadowcode.io/compile-risc-v-gnu-toolchain-for-windows/>.

Create new user riscvnucleibuilder and add it into sudo group:

```bash
sudo adduser riscvnucleibuilder
sudo usermod -aG sudo riscvnucleibuilder
```

Log in as riscvnucleibuilder and execute:

```bash
sudo apt-get update
sudo apt-get install -y gcc g++ gperf bison flex texinfo help2man make libncurses5-dev  python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 xz-utils unzip patch libstdc++6 rsync git mingw-w64 gettext python
cd
git clone --recursive https://github.com/stephanosio/zephyr-crosstool-ng ct-ng
cd ~/ct-ng
cp -r samples/riscv32-unknown-elf/ samples/x86_64-w64-mingw32,riscv-nuclei-elf/
echo "CT_CANADIAN=y" >>samples/x86_64-w64-mingw32,riscv-nuclei-elf/crosstool.config
echo "CT_HOST=\"x86_64-w64-mingw32\"" >>samples/x86_64-w64-mingw32,riscv-nuclei-elf/crosstool.config
./bootstrap
mkdir ~/zt-ng
./configure --prefix=/home/riscvnucleibuilder/zt-ng
make
make install
cd ~/zt-ng/bin/
./ct-ng x86_64-w64-mingw32,riscv-nuclei-elf
./ct-ng menuconfig

# “Target options” submenu. Select “Build a multilib toolchain”.
# “C-Library” submenu. Here select “Newlib” as the C library
# Version of newlib set to 3.0.0,
# because "%USERPROFILE%\.platformio\packages\toolchain-gd32vold\riscv-nuclei-elf\include\_newlib_version.h"
# version is 3.0.0
#  “Enable newlib nano variant”
# “C Compiler” submenu, scroll to the bottom and select “C++”
# Debug facilities  --->gdb (Build a static cross gdb)
# Exit and save

./ct-ng build
cd
nano renamebin.sh
chmod +x renamebin.sh
sudo ./renamebin.sh
tar cvzf win-riscv-nuclei-elf.tar.gz x-tools/HOST-x86_64-w64-mingw32/riscv32-unknown-elf/
scp win-riscv-nuclei-elf.tar.gz user@host:/directory
```

```bash
cat renamebin.sh
#!/bin/bash
USER_HOME=$(getent passwd ${SUDO_USER:-$USER} | cut -d: -f6)
pushd "$USER_HOME/x-tools/HOST-x86_64-w64-mingw32/riscv32-unknown-elf/bin/"
for name in riscv32-unknown-elf*
do
    newname=riscv-nuclei-elf"$(echo "$name" | cut -c20-)"
    mv "$name" "$newname"
    echo "$name --> $newname"
done
popd
```

On windows computer execute:

updtoolchain.bat

```cmd
@echo off
echo Make sure toolchain-gd32v.old directory contains original g++9.2 toolchain-gd32v
pause
if NOT exist %USERPROFILE%\.platformio\packages\toolchain-gd32v.old exit /b
cd %USERPROFILE%\.platformio\packages
rmdir /S /Q toolchain-gd32v
tar -xzvf W:\tiit\win-riscv-nuclei-elf.tar.gz
move x-tools\HOST-x86_64-w64-mingw32\riscv32-unknown-elf toolchain-gd32v

7z x win-riscv-nuclei-elf.tar
move x-tools\HOST-x86_64-w64-mingw32\riscv32-unknown-elf toolchain-gd32v

Xcopy toolchain-gd32v.old\riscv-nuclei-elf toolchain-gd32v\riscv32-unknown-elf /E /H /C /I /Y
copy toolchain-gd32v.old\.piopm toolchain-gd32v
copy toolchain-gd32v.old\package.json toolchain-gd32v
```

## Using On Windows

Add to platform.ini:

```ini
build_flags = -Wl,-u,_printf_float,-u,_scanf_float
    -std=gnu++2a
build_unflags =
    -std=gnu++17
```

Add to main.cpp some code:

```c++
struct base {
 virtual int func() const = 0;
};

struct derived: base {
 constexpr int func() const override { return 2; }
};

constexpr auto func(const base &b) {
 return b.func();
}


consteval int add(int a, int b) { return a+b; }
constexpr int r = add(100, 300);


void setup() {
 constexpr derived d{};
 static_assert(func(d) == 2);
 static_assert(add(2,3) == 5);
}
```

## Links

<https://shadowcode.io/compile-risc-v-gnu-toolchain-for-windows/>