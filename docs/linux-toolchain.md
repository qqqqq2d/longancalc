# Building newer C++ 10.2.0 Linux toolchain for longan nano

## Building on Ubuntu


Create new user linuxbuilder and add it into sudo group:

```bash
sudo adduser linuxbuilder
sudo usermod -aG sudo linuxbuilder
exit
```

Log in as linuxbuilder and execute:

```bash
cd
git clone --recursive https://github.com/stephanosio/zephyr-crosstool-ng ct-ng
cd ct-ng/
cp -r samples/riscv32-unknown-elf/ samples/riscv-nuclei-elf/
./bootstrap
./configure --prefix=/home/linuxbuilder/zt-ng
make
make install
cd ~/zt-ng/bin/
./ct-ng riscv-nuclei-elf
./ct-ng menuconfig
# “Target options” submenu. Select “Build a multilib toolchain”.
# “C-Library” submenu. Here select “Newlib” as the C library
# Version of newlib set to 3.0.0,
# because ".platformio\packages\toolchain-gd32v.old\riscv-nuclei-elf\include\_newlib_version.h"
# version is 3.0.0
#  “Enable newlib nano variant”
# “C Compiler” submenu, scroll to the bottom and select “C++”
# Debug facilities  --->gdb (Build a static cross gdb)
# Exit and save
./ct-ng build
cd
nano renamebin.sh
cat renamebin.sh

#!/bin/bash
USER_HOME=$(getent passwd ${SUDO_USER:-$USER} | cut -d: -f6)
pushd "$USER_HOME/x-tools/riscv32-unknown-elf/bin/"
for name in riscv32-unknown-elf*
do
    newname=riscv-nuclei-elf"$(echo "$name" | cut -c20-)"
    mv "$name" "$newname"
    echo "$name --> $newname"
done
popd

chmod +x renamebin.sh
sudo ./renamebin.sh

tar cvzf linux-riscv-nuclei-elf.tar.gz x-tools/riscv32-unknown-elf/
scp linux-riscv-nuclei-elf.tar.gz user@host:/directory
```

On Linux platformio development computer execute:

```bash
cd ~/.platformio/packages/
tar xzf linux-riscv-nuclei-elf.tar.gz
mv toolchain-gd32v toolchain-gd32v.old #rename original toochain
mv riscv32-unknown-elf/ toolchain-gd32v
sudo cp toolchain-gd32v.old/.piopm toolchain-gd32v/.
sudo cp toolchain-gd32v.old/package.json toolchain-gd32v/..
sudo cp -r toolchain-gd32v.old/riscv-nuclei-elf/* toolchain-gd32v/riscv32-unknown-elf/
mv toolchain-gd32v.old/ ../ #move old toolchain to upper directory for avoiding conflict since .piopm and package.json files are the same
```

## Using On Linux

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
