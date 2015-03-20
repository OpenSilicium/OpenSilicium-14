HTARGET	:= hello_world
ETARGET	:= e_hello_world
WARN	:= -Wall

ESDK=${EPIPHANY_HOME}
LIBSPATH=${ESDK}/tools/host/lib
LIBS=-le-hal
INCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf
ELIBS=-le-lib
EHDF=${EPIPHANY_HDF}

CC      := gcc
ECC	:= e-gcc
EOBJCOP := e-objcopy

all: ${HTARGET} ${ETARGET}.srec

${HTARGET}: ${HTARGET}.c
	${CC} ${WARN} ${HTARGET}.c -I ${INCS} -L ${LIBSPATH} ${LIBS} -o $@

${ETARGET}.srec: ${ETARGET}
	${EOBJCOP} --srec-forceS3 --output-target srec ${ETARGET} ${ETARGET}.srec

${ETARGET}: ${ETARGET}.c
	${ECC} ${WARN} -T ${ELDF} ${ETARGET}.c ${ELIBS} -o $@

run: ${ETARGET}.srec ${HTARGET}
	sudo -E LD_LIBRARY_PATH=${LIBSPATH} EPIPHANY_HDF=${EPIPHANY_HDF} ./${HTARGET}
	# envvar a cause de sudo

clean:
	rm -rf *.o ${HTARGET}  ${ETARGET} ${ETARGET}.srec
