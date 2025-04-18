DEBUG = TRUE

E = @echo

GXX = g++
LD 	= g++

GXXFLAGS	= -Wall -Wextra -Werror
LDFLAFS		= 

ifeq ($(DEBUG), TRUE)
	GXXFLAGS += -O0 -g
else
	GXXFLAGS += -Os
endif

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

BINDIR = .
EXE = nace

.PHONY: all
all: $(EXE)

$(EXE): $(OBJS)
	mkdir -p $(BINDIR)
	$(LD) $^ -o $(BINDIR)/$@ $(LDFLAFS)

%.o: %.cpp
	$(GXX) $(GXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINDIR)/$(EXE)
