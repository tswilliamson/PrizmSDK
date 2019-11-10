An example Project is included in projects/example or you can use the Makefile
and make.bat provided as examples to create your own setup.

The basic steps for compiling if you chose to use a build system other than make
are provided below.
# Compile
sh3eb-elf-gcc -m4a-nofpu -mb -Os -mhitachi -Wall -nostdlib -Iinclude -lfxcg -lgcc -Llib -Tcommon/prizm.ld -Wl,-static -o addin.bin projects/common/crt0.s addin.c

# Make g3a with a few localized names and icons
mkg3a -i uns:unselected.bmp -i sel:selected.bmp -n "en:My First Addin" -n "es:José" addin.bin MyFirstAddin.g3a