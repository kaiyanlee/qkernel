#!/bin/bash

# This will create a hard disk that we can use to test our kernel.

grub-mkrescue -v --fonts="" --locales="" --themes="" -o ../qkernel.iso ../root
