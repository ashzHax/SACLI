# by     : ashz
# reason : compiling SACLI code 

COMPILER=gcc
FLAGS=-Wall -Werror -I -g
LIBS:=

V_AUTHORS=ashz leegh

S_OBJECT=main.o
S_OUTPUT_NAME=svm

CONSTANT_VARIABLES=-DPROG_NAME=$(S_OUTPUT_NAME) \
                   -DAUTHOR=$(V_AUTHORS)

%.o : %.c $(G_DEPS)
	$(COMPILER) -c -o $@ $< $(FLAGS) 

$(S_OUTPUT_NAME): $(S_OBJECT)
	$(COMPILER) -o $(S_OUTPUT_NAME) $(S_OBJECT) $(LIBS)

install:
	mkdir -p /usr/bin/
	sudo cp ./$(S_OUTPUT_NAME) /usr/bin/$(S_OUTPUT_NAME)

uninstall:
	sudo rm /usr/bin/$(S_OUTPUT_NAME)

clean:
	rm -rf $(S_OBJECT)
	rm -rf $(S_OUTPUT_NAME)
