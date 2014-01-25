#!/bin/sh

fastboot -i 0x283b flash kernel ./arch/arm/boot/zImage
fastboot -i 0x283b reboot
