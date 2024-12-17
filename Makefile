
CC = gcc


CFLAGS = -Wall -Wextra -std=c11


TARGET = DungeonDweller


SRCS = main.c
HEADERS = main.h


OBJS = $(SRCS:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build complete! Run with ./$(TARGET)"


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET) $(OBJS)
	@echo "Clean complete!"


run: all
	./$(TARGET)
