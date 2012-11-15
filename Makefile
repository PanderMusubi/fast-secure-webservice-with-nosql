PROGRAMS = store store-user-location

all: ${PROGRAMS}
clean:
	rm -f ${PROGRAMS}

${PROGRAMS}: %: %.c
	cc -O3 -Wall -W -Werror $< -o $@ $(shell pkg-config --cflags --libs libmongo-client) $(shell pkg-config --cflags --libs libssl)

.PHONY: all clean
