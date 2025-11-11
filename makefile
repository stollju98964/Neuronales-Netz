CC = gcc
CFLAGS = -g -Wall

ifeq ($(OS),Windows_NT)
	include makefile_windows.variables
else
	UNAME = $(shell uname)
	ifeq ($(UNAME),Linux)
		include makefile_linux.variables
	else
		include makefile_mac.variables
	endif
endif


raylibfolder = ./raylib
unityfolder = ./unity

# --------------------------
# Initiales Programm bauen (zum ausprobieren)
# --------------------------
mnist_initial: $(BINARIES)/libmnist_complete.a
	$(CC) -o mnist $(BINARIES)/libmnist_complete.a $(BINARIES)/libraylib.a ${LDFLAGS}

# --------------------------
# Selbst implementiertes Programm bauen
# --------------------------
mnist: main.o matrix.o neuralNetwork.o imageInput.o mnistVisualization.o $(BINARIES)/libraylib.a
	$(CC) $(CFLAGS) -I$(raylibfolder) -o mnist main.o matrix.o neuralNetwork.o imageInput.o mnistVisualization.o $(BINARIES)/libraylib.a ${LDFLAGS}

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

matrix.o: matrix.c
	$(CC) -c $(CFLAGS) matrix.c

neuralNetwork.o: neuralNetwork.c
	$(CC) -c $(CFLAGS) neuralNetwork.c

imageInput.o: imageInput.c
	$(CC) -c $(CFLAGS) imageInput.c

mnistVisualization.o: mnistVisualization.c
	$(CC) -c $(CFLAGS) -I$(raylibfolder) mnistVisualization.c

# --------------------------
# Unit Tests
# --------------------------
matrixTests: matrix.o matrixTests.c $(unityfolder)/unity.c
	$(CC) $(CFLAGS) -I$(unityfolder) -o runMatrixTests matrixTests.c matrix.o $(unityfolder)/unity.c

neuralNetworkTests: neuralNetwork.o matrix.o neuralNetworkTests.c $(unityfolder)/unity.c
	$(CC) $(CFLAGS) -I$(unityfolder) -o runNeuralNetworkTests neuralNetworkTests.c matrix.o neuralNetwork.o $(unityfolder)/unity.c ${LDFLAGS}

imageInputTests: imageInput.o imageInputTests.c $(unityfolder)/unity.c
	$(CC) $(CFLAGS) -I$(unityfolder) -o runImageInputTests imageInputTests.c imageInput.o $(unityfolder)/unity.c

# --------------------------
# Clean
# --------------------------
clean:
ifeq ($(OS),Windows_NT)
	del /f *.o *.exe
else
	rm -f *.o mnist runMatrixTests runNeuralNetworkTests runImageInputTests
endif

