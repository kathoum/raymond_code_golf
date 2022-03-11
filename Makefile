sources = main.c v1.c v2.c v3.c v4.c v5d.c v6d.c v7d.c v8.c v9d.c vad.c vb.c vcd.c
output = program

$(output): $(sources)
	$(CC) -Wall -Wextra -std=c17 -Os -march=native $^ -o $@

run: $(output)
	./$(output)

test: $(output)
	./$(output) 8
	./$(output) 16
	./$(output) 24
	./$(output) 0
	./$(output) 1
	./$(output) 2
	./$(output) 255
	./$(output) 256

clean:
	$(RM) $(output)
