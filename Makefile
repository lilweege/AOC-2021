
BIN = bin
SRCS = $(wildcard *.c) $(wildcard *.cpp)
BINS = $(patsubst %.c, $(BIN)/%.out, $(wildcard *.c)) $(patsubst %.cpp, $(BIN)/%.out, $(wildcard *.cpp))
LOGFILE = log
CFLAGS = -Ofast

all: $(BINS)

$(BIN)/%.out: %.c
	gcc $(CFLAGS) $< -o $@ 2>/dev/null

$(BIN)/%.out: %.cpp
	g++ $(CFLAGS) $< -o $@ 2>/dev/null

run: $(BINS)
	@rm -f $(LOGFILE)
	@bash -c "TIMEFORMAT='%U'; \
		duration=0; \
		for f in \$$(ls $(BIN)); do \
			elapsed=\$$(time (./$(BIN)/\$$f >> $(LOGFILE);) 2>&1); \
			echo \$$(basename \$$f): \$$elapsed\ | tee -a $(LOGFILE); \
			duration=\$$(echo \"\$$duration + \$$elapsed\" | bc); \
		done; \
		duration=\$$(printf %.3f \$$duration); \
		echo total: \$$duration"

clean:
	rm -f $(BIN)/*.out $(LOGFILE)