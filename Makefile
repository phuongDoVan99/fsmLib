OBJECTS = fsm.o fsm_demo.o
SOURCES = fsm.c fsm_demo.c
#LIB = fsm.so
EXEC = fsm_exec

CFLAGS += -Wall
CPPFLAGS += -DA2D

all: $(EXEC)

$(EXEC):
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $(SOURCES)

clean:
	$(RM) *.o *.exe $(EXEC)

run: clean all
	./$(EXEC)
