CC = gcc
RUSTC = rustc

C_SRC = src/tictactoe.c
RUST_SRC = src/tictactoe.rs
C_OUT = tictactoeC
RUST_OUT = tictactoeRUST

all: $(C_OUT) $(RUST_OUT)
	./src/script.sh

$(C_OUT): $(C_SRC)
	$(CC) -o $(C_OUT) $(C_SRC)

$(RUST_OUT): $(RUST_SRC)
	$(RUSTC) -o $(RUST_OUT) $(RUST_SRC)

clean:
	rm -f $(C_OUT) $(RUST_OUT)
