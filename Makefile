EXEC=./scheduler
NUM_PROCS=2
MPICC=mpicc ${CFLAGS}

all: ${EXEC}

ubuntu_install_deps:
	sudo apt update && sudo apt-get install libopenmpi-dev openmpi-bin

scheduler: scheduler.c
	${MPICC} -o scheduler scheduler.c

run: all
	mpirun -n ${NUM_PROCS} ${EXEC}

clean:
	rm -f ${EXEC}