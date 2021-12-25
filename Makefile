
BIN = bin
SRCS = $(wildcard *.c)
BINS = $(patsubst %.c, $(BIN)/%.out, $(SRCS))
LOGFILE = log
CFLAGS = -std=c11 -march=native -Ofast -Wall -Werror -pedantic -Wno-unused-result
CC = clang

all: $(BINS)

$(BIN)/%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

run: $(BINS)
	@rm -f $(LOGFILE)
	@bash -c "TIMEFORMAT='%E'; \
		duration=0; \
		for f in \$$(ls $(BIN)); do \
			fname=\$$(basename \$$f); \
			input=input/\$${fname/"out"/"txt"}; \
			elapsed=\$$(time (./$(BIN)/\$$f < \$$input >> $(LOGFILE)) 2>&1); \
			echo \$$fname: \$$elapsed\ | tee -a $(LOGFILE); \
			duration=\$$(echo \"\$$duration + \$$elapsed\" | bc); \
		done; \
		duration=\$$(printf %.3f \$$duration); \
		echo total: \$$duration"

clean:
	rm -f $(BIN)/*.out $(LOGFILE)