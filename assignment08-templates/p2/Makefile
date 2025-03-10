CC=     gcc
LD=     ld
CFLAGS= -g -W -Wall -Werror -std=c17 -lrt
TARGET= message_queue
SRC=    message_queue.c testlib.c main.c
OBJ=    $(SRC:.c=.o)

message_queue: $(OBJ)
	$(CC) -o $@ $(OBJ) -lrt

.PHONY: $(TARGET)-solution.o
$(TARGET)-sol: $(subst $(TARGET),$(TARGET)-solution,$(OBJ))
	$(CC) -o $@ $^ -lrt

$(TARGET)-solution.o:
	@test -f $(TARGET)-solution.c \
	|| { echo "Please download solution." >&2; false; }
	$(CC) $(CFLAGS) -c $(TARGET)-solution.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

sol: $(TARGET)-sol

clean:
	rm -rf $(TARGET) $(TARGET)-sol *.o

showsrc:
	@echo message_queue.c

zip:
	cd ..; ./makezip.py

