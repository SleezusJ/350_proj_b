#!/bin/bash

# Kyle Snow 10/16/19
#
# Bash script to compile a given kernel.c and flash it to 
# a new disk diskc.img. 
#---------------------------------------------------------
# Requires: bootload.asm && project applicable kernel.asm
#


# assemble bootload.asm
FILE=bootload
if [ -e "$FILE" ]; then
    echo -e "\e[96mbootload exists. Skipping assembly."
else
    echo -e "\e[96mAssembling bootload..."
    nasm bootload.asm
fi


# If directory artifacts does not exist, create one
mkdir -p artifacts

#create a blank diskc.img and append the bootloader
echo -e "\e[96m(Re-)Creating diskc.img..."
dd if=/dev/zero of=diskc.img bs=512 count=1000
echo -e "\e[96mAppending bootloader..."
dd if=bootload of=diskc.img bs=512 count=1 conv=notrunc
if [ $? -ne 0 ];then
    echo -e "\e[96mFailed to append bootloader. do you have bootload?"
    exit 1
fi

#Start compilation/linking of kernel.c
echo -e "\e[96mCreating kernel and associated artifacts...\e[33m"

bcc kernel.c -ansi -c -o kernel.o
if [ $? -ne 0 ]; then
     echo -e "\e[31mFailed on compilation of kernel.o"
     exit 2
fi

as86 kernel.asm -o kernel_asm.o
if [ $? -ne 0 ]; then
     echo -e "\e[31mFailed on assembly of kernel_asm.o"
     exit 3

fi
ld86 -o kernel -d kernel.o kernel_asm.o
if [ $? -ne 0 ]; then
     echo -e "\e[31mFailed on linking"
     exit 4
fi


echo -e "\e[96mMaking a backup..."
cp diskc.img diskc.img.old

echo -e "\e[96mOverwriting diskc.img..."
dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3


echo -e "\e[96mCleaning up..."
mv diskc.img.old artifacts
mv kernel.o artifacts
mv kernel_asm.o artifacts



echo -e "\e[96mDone."
echo ""

exit 0
