
BIN = bin
SRCS = $(wildcard *.c) $(wildcard *.cpp)
BINS = $(patsubst %.c, $(BIN)/%.out, $(SRCS)) $(patsubst %.cpp, $(BIN)/%.out, $(SRCS))
LOGFILE = log

all: $(BINS)

$(BIN)/%.out: %.c
	gcc -Ofast $< -o $@

$(BIN)/%.out: %.cpp
	g++ -Ofast $< -o $@

run:
	@rm -f $(LOGFILE)
	@bash -c "TIMEFORMAT='%U'; \
		duration=0; \
		for f in \$$(ls bin); do \
			elapsed=\$$(time (./bin/\$$f >> $(LOGFILE);) 2>&1); \
			echo \$$(basename \$$f): \$$elapsed\ | tee -a $(LOGFILE); \
			duration=\$$(echo \"\$$duration + \$$elapsed\" | bc); \
		done; \
		duration=\$$(printf %.3f \$$duration); \
		echo total: \$$duration"

clean:
	rm -f $(BIN)/*.out $(LOGFILE)