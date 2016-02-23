525Ass1MF : dberror.o storage_mgr.o test_assign1_1.o
	gcc dberror.o storage_mgr.o test_assign1_1.o -o 525Ass1MF

dberror.o : dberror.c dberror.h
	gcc -c dberror.c -o dberror.o

storage_mgr.o : storage_mgr.c storage_mgr.h
	gcc -c storage_mgr.c -o storage_mgr.o

test_assign1_1.o : test_assign1_1.c test_helper.h
	gcc -c test_assign1_1.c -o test_assign1_1.o

clean:
	rm -rf *.o 525AssMF
