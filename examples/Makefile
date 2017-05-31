override CFLAGS += -Wall -Wextra -I../ -I. -g
FILE = minunit_example.c
EXEC = minunit_example.out

$(EXEC): $(FILE) ../minunit.h
	$(CC) $(CFLAGS) $(FILE) -o $@

all: $(EXEC)

clean:
	$(RM) *.out

test: $(EXEC)
	echo "n\n" | ./$(EXEC) > result.txt
	diff expected.txt result.txt
