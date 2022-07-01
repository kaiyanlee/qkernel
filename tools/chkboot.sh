#/usr/bin/sh

# Check to see if grub can detect multiboot.

if grub-file --is-x86-multiboot root/boot/qkernel; then
  echo "Multiboot: Yes"
else
  echo "Multiboot: No"
fi
