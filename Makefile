CFLAGS:=-Wall -Wextra -g
EXTENSION:=

TEST_FLAGS:=

TEST_SAFE_SOURCES=test.c munit/munit.c
TEST_SAFE_HEADERS=safe.h munit/munit.h
TEST_SAFE=$(TEST_SAFE_SOURCES) $(TEST_SAFE_HEADERS)

test-safe$(EXTENSION): $(TEST_SAFE)
	$(CC) $(CFLAGS) -o $@ $(TEST_SAFE_SOURCES)

test-safe-portable$(EXTENSION): $(TEST_SAFE)
	$(CC) -DSAFE_FORCE_PORTABLE $(CFLAGS) -o $@ $(TEST_SAFE_SOURCES)

test-safe-no-promotions$(EXTENSION): $(TEST_SAFE)
	$(CC) -DSAFE_FORCE_PORTABLE -DSAFE_NO_PROMOTIONS $(CFLAGS) -o $@ $(TEST_SAFE_SOURCES)

test-full: test-safe$(EXTENSION) test-safe-portable$(EXTENSION) test-safe-no-promotions$(EXTENSION)
	./test-safe $(TEST_FLAGS)
	./test-safe-portable $(TEST_FLAGS)
	./test-safe-no-promotions $(TEST_FLAGS)

test: test-safe
	./test-safe$(EXTENSION) $(TEST_FLAGS)

clean:
	rm -f test-safe test-safe-portable test-safe-no-promotions

all: test-safe
