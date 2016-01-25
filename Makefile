CFLAGS:=-Wall -Wextra -g
CC:=cc

TEST_FLAGS:=

TEST_SAFE_SOURCES=test.c munit/munit.c
TEST_SAFE_HEADERS=safe.h munit/munit.h
TEST_SAFE=$(TEST_SAFE_SOURCES) $(TEST_SAFE_HEADERS)

test-safe: $(TEST_SAFE)
	$(CC) $(CFLAGS) -o $@ $(TEST_SAFE_SOURCES)

test-safe-portable: $(TEST_SAFE)
	$(CC) -DSAFE_FORCE_PORTABLE $(CFLAGS) -o $@ $(TEST_SAFE_SOURCES)

test-safe-no-promotions: $(TEST_SAFE)
	$(CC) -DSAFE_FORCE_PORTABLE -DSAFE_NO_PROMOTIONS $(CFLAGS) -o $@ $(TEST_SAFE_SOURCES)

test-full: test-safe test-safe-portable test-safe-no-promotions
	./test-safe $(TEST_FLAGS)
	./test-safe-portable $(TEST_FLAGS)
	./test-safe-no-promotions $(TEST_FLAGS)

test: test-safe
	./test-safe $(TEST_FLAGS)

all: test-safe
