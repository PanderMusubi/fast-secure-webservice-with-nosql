PROGRAMS = store store-user-location drop-and-create-collections insert-tests-into-collections

all: ${PROGRAMS}
clean:
	rm -f ${PROGRAMS}

${PROGRAMS}: %: %.c
	cc -O3 -Wall -W -Werror -Wshadow $< -o $@ $(shell pkg-config --cflags --libs libmongo-client)
# $(shell pkg-config --cflags --libs libssl)

.PHONY: all clean
