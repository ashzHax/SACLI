# by     : ashz
# reason : compiling SACLI code 
AUTHORS=ashz

INCLUDE_DIR=$(shell pwd)/include
LIB_DIR=$(shell pwd)/lib
LIBS:=-ljansson

CC=gcc
FLAGS=-Wall -Werror -g

TARGET=svm
OBJ=\
    global.o \
    log.o \
    add.o \
    remove.o \
    show.o \
    comment.o \
    commit.o \
    clear.o \
    overwrite.o \
    edit.o \
    revert.o \
    rollback.o \
    main.o

DEFINES=-DPROG_NAME=$(TARGET) \
		-DMODE_DEBUG \
		-DAUTHOR=$(AUTHORS)

all: create_dir copy_header install_lib $(TARGET)

create_dir:
	mkdir -p $(LIB_DIR)
	mkdir -p $(INCLUDE_DIR)

copy_header:
	cp ./*.h $(INCLUDE_DIR)

# Github repository: https://github.com/akheron/jansson
# Use version: Release 2.14
install_lib_jansson:
	cd $(LIB_DIR); \
	[ -e ./jansson-2.14 ] || { \
		[ -e ./jansson-2.14.tar.gz ] && rm -rf jansson-2.14.tar.gz; \
		wget https://github.com/akheron/jansson/releases/download/v2.14/jansson-2.14.tar.gz; \
		tar -zxvf jansson-2.14.tar.gz; \
	}; \
	cd jansson-2.14; \
	./configure; \
	make; \
	cp ./src/*.h $(INCLUDE_DIR)/; \
	cp ./src/.libs/libjansson.so.4.14.0 $(LIB_DIR)/; \
	cd ..; \
	rm libjansson.so; \
	rm libjansson.so.4; \
	ln -s libjansson.so.4.14.0 libjansson.so; \
	ln -s libjansson.so.4.14.0 libjansson.so.4

clean_lib_jansson:
	cd $(LIB_DIR); \
	rm libjansson.so libjansson.so.4 libjansson.so.4.14.0; \
	cd ./jansson-2.14; \
	make clean

install_lib: \
	install_lib_jansson
	@echo -e "\n\n\n\n\nFinished installing all libraries!\n\n\n\n\n"

clean_lib: \
	clean_lib_jansson
	@echo -e "\n\n\n\n\nFinished cleaning all libraries!\n\n\n\n\n"

%.o : %.c $(G_DEPS)
	$(CC) -c -o $@ $< $(FLAGS) $(DEFINES) -I$(INCLUDE_DIR)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -L$(LIB_DIR) $(LIBS)

install:
	mkdir -p /usr/bin/
	sudo cp ./$(TARGET) /usr/bin/$(TARGET)
	sudo cp $(LIB_DIR)/libjansson.so.4.14.0 /lib/x86_64-linux-gnu/
	# TODO need a better implementation of this, need to get architecture by system var
	[ -e /lib/x86_64-linux-gnu/libjansson.so.4 ] || sudo ln -s /lib/x86_64-linux-gnu/libjansson.so.4.14.0 /lib/x86_64-linux-gnu/libjansson.so.4

uninstall:
	sudo rm /usr/bin/$(TARGET)

clean: clean_lib
	rm -rf $(OBJ)
	rm -rf $(TARGET)
