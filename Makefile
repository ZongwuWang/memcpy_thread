EXECUTABLE:= main
LIBDIR:=
LIBS:=pthread
INCLUDES:=.
SRCDIR:=./src

CC:=gcc
CXX:=g++
# CXX:=aarch64-linux-gnu-g++
CFLAGS:= -g -Wall -O0 -static -static-libgcc -static-libstdc++
CFLAGS+= -std=c++11
CFLAGS+= $(addprefix -I,$(INCLUDES))
CFLAGS+= -I.
CFLAGS+= -MMD
CXXFLAGS+= $(CFLAGS)

RM-F:= rm -f

SRCS:= $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR)))
OBJS:= $(patsubst %.cpp,%.o,$(SRCS))
DEPS:= $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS:= $(filter-out $(wildcard $(DEPS)),$(DEPS))
#MISSING_DEPS_SOURCES:= $(wildcard $(patsubst %.d,%.cpp,$(MISSING_DEPS)))


.PHONY : all deps objs clean
all:$(EXECUTABLE)
deps:$(DEPS)

objs:$(OBJS)
clean:
	# @$(RM-F) ${SRCDIR}/*.o
	# @$(RM-F) ${SRCDIR}/*.d
	@$(RM-F) $(OBJS)
	@$(RM-F) $(DEPS)
	@$(RM-F) $(EXECUTABLE)

ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS):
	@$(RM-F) $(patsubst %.d,%.o,$@)
endif
-include $(DEPS)
$(EXECUTABLE) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJS) -Wl,--whole-archive  $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))  -Wl,--no-whole-archive
	@$(RM-F) $(OBJS)
	@$(RM-F) $(DEPS)


