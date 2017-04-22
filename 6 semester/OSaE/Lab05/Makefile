TARGET = lab05
SRCS = lab05.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r $(TARGET) $(OBJS)

