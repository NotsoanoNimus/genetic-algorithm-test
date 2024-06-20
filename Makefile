# Makefile
#
# Primary instructions on creating the target executable.
#
# The -print versions include all printed game details and decisions.
#   They are not included by default because it skews the total computation metrics.
#

.PHONY: default clean release

CC = gcc
CFLAGS = -Wall

SRCS = main.c genetic.c
OBJS = $(SRCS:.c=.o)

TARGET = genetic


default:
	$(MAKE) clean
	$(MAKE) $(TARGET)

release:
	$(MAKE) clean
	$(MAKE) release-sub

release-sub: CFLAGS += -O3
release-sub: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -f $(OBJS) $(TARGET)
