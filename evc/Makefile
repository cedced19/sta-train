SOURCE	= EVC.c Libs_Unirail/CAN/canLinux.c Libs_Unirail/CAN/MESCAN1_Utilitaire.c Libs_Unirail/CAN/MESCAN1_VarStatusTrain.c Libs_Unirail/CAN/MESCAN1_VarTrain.c
CC = gcc
FLAGS=-std=gnu11 -Wall
LFLAGS= -lpthread -lm -lrt

# Add -lwiringPi on Raspberry

all: EVC

EVC: $(SOURCE)
	$(CC) $(SOURCE) -o EVC.exe $(LFLAGS) -lwiringPi $(FLAGS)

EVC-fake:
	$(CC) EVC-fake.c -o EVC.exe $(LFLAGS) $(FLAGS)

test: EVC
	./EVC.exe 192.168.1.101

test-fake: EVC-fake
	./EVC.exe 127.0.0.1 4242
