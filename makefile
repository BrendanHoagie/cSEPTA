CC = gcc
LDFLAGS = $(shell curl-config --libs)
EXEC = csepta

build: | clean $(EXEC)

debug: | clean
	$(CC) -g -o debug all.c $(LDFLAGS)

run : clean $(EXEC)
	./$(EXEC)

$(EXEC):
	$(CC) all.c -o $(EXEC) $(LDFLAGS)

clean:
	rm -rf $(EXEC)

