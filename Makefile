CC 	 = clang
CXX	 = clang++
LD	 = clang++

CCFLAGS  =
CXXFLAGS =
LDFLAGS  = -g

Target = teapot
Objs   = main.o

all: $(Target)

$(Target) : $(Objs)
	@echo "[LD]" $(Target) "<-" $(Objs)
	@$(LD) -o $(Target) $(Objs)

run: $(Target)
	@echo [RUN] $(Target)
	@./$(Target)

debug: $(Target)
	@echo [GDB] $(Target)
	@gdb $(Target)
clean:
	@echo [Clean] $(Target) $(Objs)
	@rm $(Target) $(Objs)
%.o: %.c
	@echo [CC] $@ "<-" $<
	@$(CC) $(CCFLAGS) -c -o $@ $<
%.o: %.cxx
	@echo [CXX] $@ "<-" $<
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	
	


