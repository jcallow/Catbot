KERNEL_TREE := /opt/raspberrypi/linux/
PWD := $(shell pwd)

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

obj-m := sainsmart_lcd_driver.o

default:
	@[ -d ${KERNEL_TREE} ] || { echo "Edit Makefile to set KERNEL_TREE to point at your kernel"; exit 1; }
	@[ -e ${KERNEL_TREE}/Module.symvers ] || { echo "KERNEL_TREE/Module.symvers does not exist, you need to configure and compile your kernel"; exit 1; }
	make -C ${KERNEL_TREE} M=$(PWD) modules

clean:
	make -C ${KERNEL_TREE} M=$(PWD) clean
