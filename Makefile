BIN=markandsweep

.PHONY : clean

$(BIN) : main.c
	$(CC) -ggdb -std=c11 main.c -o $(BIN)

clean :
	rm -f $(BIN) *~

run : $(BIN)
	valgrind --leak-check=yes ./$(BIN)
