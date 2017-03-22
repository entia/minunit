CFLAGS += -I../ -I. -g
EXEC = minunit_example.out

$(EXEC):
	$(CC) $(CFLAGS) minunit_example.c -o $(EXEC)

all: $(EXEC)

clean:
	$(RM) *.out

test: $(EXEC)
	echo "n\n" | ./$(EXEC) > result.txt
	diff expected.txt result.txt
