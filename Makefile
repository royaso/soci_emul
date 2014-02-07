soci_emul: emul_def.o
	gcc -o $@ main.c emul_def.o

emul_def.o:
	gcc -c -o $@ emul_def.c

.PHONY: clean
clean:
	rm -rf soci_emul emul_def.o

