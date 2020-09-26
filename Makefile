all:
	gcc -o process_vm_read process_vm_read.c
	gcc -o process process.c
clean:
	rm -f process_vm_read process
