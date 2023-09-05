# by     : ashz
# reason : compiling SACLI code 
AUTHORS=ashz
DEFINES=-DMODE_DEBUG

INCLUDE_DIR=$(shell pwd)/include
LIB_DIR=$(shell pwd)/lib
LIBS:=-ljansson

CC=gcc
FLAGS=-Wall -Werror -I -g

TARGET=svm
OBJ=\
    global.o \
    main.o

CONSTANT_VARIABLES=\
                   -DPROG_NAME=$(TARGET) \
                   -DAUTHOR=$(AUTHORS)

all: create_dir install_lib $(TARGET)

create_dir:
	mkdir -p $(LIB_DIR)
	mkdir -p $(INCLUDE_DIR)

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
	cp ./src/.libs/libjansson.so $(LIB_DIR)/; \
	cp ./src/*.h $(INCLUDE_DIR)/

clean_lib_jansson:
	cd $(LIB_DIR)/jansson-2.14; \
	make clean

install_lib: \
	install_lib_jansson
	@echo -e "\n\n\n\n\nFinished installing all libraries!\n\n\n\n\n"

clean_lib: \
	clean_lib_jansson
	@echo -e "\n\n\n\n\nFinished cleaning all libraries!\n\n\n\n\n"

%.o : %.c $(G_DEPS)
	$(CC) -c -o $@ $< $(FLAGS) $(DEFINES)

$(TARGET): $(OBJ)
	cp ./*.h $(INCLUDE_DIR)
	$(CC) -o $(TARGET) $(OBJ) -L$(LIB_DIR) $(LIBS) -I$(INCLUDE_DIR)

install:
	mkdir -p /usr/bin/
	sudo cp ./$(TARGET) /usr/bin/$(TARGET)

uninstall:
	sudo rm /usr/bin/$(TARGET)

clean: clean_lib
	rm -rf $(OBJ)
	rm -rf $(TARGET)
