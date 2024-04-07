CC = g++
OPT = -O3
#OPT = -g
OPT = -std=c++11 #-g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# List all your .cc/.cpp files here (source files, excluding header files)
SIM_SRC = sim_bp.cc predictor.cc chooser.cc

# List corresponding compiled object files here (.o files)
SIM_OBJ = sim_bp.o predictor.o chooser.o

#################################

# default rule

all: sim
	@echo "my work is done here..."


# rule for making sim

sim: $(SIM_OBJ)
	$(CC) -o sim $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH sim-----------"


# generic rule for converting any .cpp file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc

.cpp.o:
	$(CC) $(CFLAGS)  -c $*.cpp


# type "make clean" to remove all .o files plus the sim binary

clean:
	rm -f *.o sim


# type "make clobber" to remove all .o files (leaves sim binary)

clobber:
	rm -f *.o


# Validation run with bimodal prediction
brun:
	./sim bimodal 6 gcc_trace.txt 

brun1:
	./sim bimodal 6 gcc_trace.txt > bimodal_run1_output.txt
bdiff1:
	diff -iw bimodal_run1_output.txt ./validation/val_bimodal_1.txt || exit 0

brun2:
	./sim bimodal 12 gcc_trace.txt > bimodal_run2_output.txt
bdiff2:
	diff -iw bimodal_run2_output.txt ./validation/val_bimodal_2.txt || exit 0

brun3:
	./sim bimodal 4 jpeg_trace.txt > bimodal_run3_output.txt
bdiff3:
	diff -iw bimodal_run3_output.txt ./validation/val_bimodal_3.txt || exit 0

brun4:
	./sim bimodal 5 perl_trace.txt > bimodal_run4_output.txt
bdiff4:
	diff -iw bimodal_run4_output.txt ./validation/val_bimodal_4.txt || exit 0



# Validation run with gshare prediction
grun:
	./sim gshare 9 3 gcc_trace.txt

grun1:
	./sim gshare 9 3 gcc_trace.txt > gshare_run1_output.txt
gdiff1:
	diff -iw gshare_run1_output.txt ./validation/val_gshare_1.txt || exit 0

grun2:
	./sim gshare 14 8 gcc_trace.txt > gshare_run2_output.txt
gdiff2:
	diff -iw gshare_run2_output.txt ./validation/val_gshare_2.txt || exit 0

grun3:
	./sim gshare 11 5 jpeg_trace.txt > gshare_run3_output.txt
gdiff3:
	diff -iw gshare_run3_output.txt ./validation/val_gshare_3.txt || exit 0

grun4:
	./sim gshare 10 6 perl_trace.txt > gshare_run4_output.txt
gdiff4:
	diff -iw gshare_run4_output.txt ./validation/val_gshare_4.txt || exit 0



# Validation run with hybrid prediction
hrun:
	./sim hybrid 8 14 10 5 gcc_trace.txt

hrun1:
	./sim hybrid 8 14 10 5 gcc_trace.txt > hybrid_run1_output.txt
hdiff1:
	diff -iw hybrid_run1_output.txt ./validation/val_hybrid_1.txt || exit 0

hrun2:
	./sim hybrid 5 10 7 5 jpeg_trace.txt > hybrid_run2_output.txt
hdiff2:
	diff -iw hybrid_run2_output.txt ./validation/val_hybrid_2.txt || exit 0