# 24-08-2011
# billy

EXE = telefons # Executable
OBJ = telefons.o # Objectes
COM = g++ $(LIB) # Compilador
LIB = -I/usr/include/mysql -I/usr/include/mysql++ -L/usr/local/lib -lmysqlpp -lmysqlclient -lnsl -lz -lm # Llibreries (-l*, -L*, -I*)
AVS = -W -Wall -Wextra -ansi -pedantic # Avisos
OPT = -march=native -O2 -pipe # Optimitzacio
DEP = -g # Depuracio, no recomanable junt amb $(OPT)
OPC = $(OPT) $(AVS) -std=c++14 # Opcions
DIR = ~/bin # Directori per a instalar

all: $(EXE)

$(EXE): $(OBJ)
	@echo Enllaçant $(OBJ) per a crear $(EXE)
	$(COM) $(OBJ) -o $@

telefons.o: telefons.c++
	@echo Compilant $<
	$(COM) $(OPC) -c $<

exe: all
	./$(EXE)

install: all
	mkdir -p $(DIR)
	cp $(EXE) $(DIR)

clean:
	@echo Netejant...
	rm -rf $(EXE) $(OBJ) *~
