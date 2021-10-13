SOURCE	= avance_train.c Libs_Unirail/CAN/canLinux.c Libs_Unirail/CAN/MESCAN1_Utilitaire.c Libs_Unirail/CAN/MESCAN1_VarStatusTrain.c Libs_Unirail/CAN/MESCAN1_VarTrain.c
CC = gcc
FLAGS=-std=gnu11 -Wall
LFLAGS= -lpthread -lm -lwiringPi -lrt

all: EVC


EVC: $(SOURCE)
	$(CC) $(SOURCE) -o avance_train.exe $(LFLAGS) $(FLAGS)

