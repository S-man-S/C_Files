GCC = gcc -Wall -Wextra -Werror

all: cipher logging_cipher

clean:
	rm -rf *.o ../build/*

rebuild: clean all

cipher:
	$(GCC) -o ../build/cipher cipher.c

logging_cipher:
	$(GCC) -o ../build/logging_cipher cipher.c logger.c -DLOG