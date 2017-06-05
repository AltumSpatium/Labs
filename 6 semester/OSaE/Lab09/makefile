program_name = lab09
obj_list = $(program_name).o
compiler = cc

spg: $(obj_list)
	$(compiler) $(obj_list) -o $(program_name) -lrt -pthread 
.o:
	$(compiler) -c $^
clean: clean_obj
	rm $(program_name)
clean_obj:
	rm *.o
