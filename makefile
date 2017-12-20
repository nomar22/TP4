# Compile/Link 
GCC = gcc 
OPTMIZE = -g -O3 
LIBS = -lm -pthread
#Objetos 
OBJS =  tp4.c grafo.c lista.c 
# Nome do aplicativo 
APPNAME = tp4
release : ; $(GCC) $(OPTMIZE) $(OBJS) -o $(APPNAME)  $(LIBS)
clean : rm -rf *.o 
run :
	make 
	./tp4 input.txt output.txt
