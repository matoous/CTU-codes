#!/bin/sh
# Clone busybox
git clone git://git.busybox.net/busybox
cd busybox
make defconfig
make
cd ../

# Clone Linux
git clone https://kernel.googlesource.com/pub/scm/linux/kernel/git/stable/linux-stable.git
cd linux-stable
make defconfig
make -j$(nproc)
cd ../

# Install busybox
cd busybox
make install

# Copy lib
mkdir _install/lib
mkdir _install/lib64
cp /usr/lib/libm.so.6 /usr/lib/libc.so.6 /usr/lib64/ld-linux-x86-64.so.2 _install/lib
cp /usr/lib64/ld-linux-x86-64.so.2 _install/lib64

# Boot
mkdir _install/{dev,etc,proc,sys}
sudo cp -a /dev/tty? _install/dev
ln -s bin/busybox _install/init
(cd _install; find . | cpio -o -H newc | gzip) > ramdisk

# Run
cd ../
qemu-system-x86_64 -serial stdio -append console=ttyS0 -kernel linux-stable/arch/x86/boot/bzImage -initrd busybox/ramdisk
