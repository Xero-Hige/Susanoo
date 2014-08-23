include mkinc.mak
CFLAGS=-Iinclude

all: libstemmer.o

libstemmer.o: $(snowball_sources:.c=.o)
	$(AR) -cru $@ $^

clean:
	rm -f stemwords *.o src_c/*.o runtime/*.o libstemmer/*.o
