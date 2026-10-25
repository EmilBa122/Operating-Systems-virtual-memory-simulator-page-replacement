src = $(wildcard ./sourcefiles/*.c)
obj = $(src:.c=.o)
CC = gcc

LDFLAGS = -lm

mementries: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) mementries
