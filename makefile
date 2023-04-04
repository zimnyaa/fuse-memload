

FILENAME := fuse-memfile
MOUNTPOINT := ./mountp

all: build run

build:
	@echo make: building and encoding the example binary 
	@gcc -O3 -s executable-example.c -o executable-example
	@python3 xor.py executable-example $(MOUNTPOINT) > encfile.h
	@echo make: compiling the driver
	@gcc fuse-memfile.c -D_FILE_OFFSET_BITS=64 -w -lfuse -o $(FILENAME)

run:
	@echo make: creating the mountpoint at $(MOUNTPOINT)
	@mkdir $(MOUNTPOINT)
	@echo make: running the FS
	@./$(FILENAME)

clean:
	@echo make: unmounting
	@fusermount -u $(MOUNTPOINT)
	@echo make: killin the driver
	@killall $(FILENAME)
	@echo make: removing artifacts
	@rm executable-example
	@rm $(FILENAME)
	@rm encfile.h
	@rmdir $(MOUNTPOINT)
