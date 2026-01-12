CC = gcc
LDFLAGS = $(shell curl-config --libs)
EXEC = csepta

build: $(EXEC)

run : $(EXEC)
	./$< JSON_data/valid_requests.json

$(EXEC):
	$(CC) all.c -o $(EXEC) $(LDFLAGS)

clean:
	rm -rf $(EXEC)

