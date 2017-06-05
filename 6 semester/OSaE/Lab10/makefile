program_name = lab10
obj_list = $(program_name).o
compiler = cc

all: $(obj_list)
	$(compiler) $(obj_list) -o $(program_name) -lrt -pthread
.o:
	$(compiler) -c $^
clean: clean_obj
	rm $(program_name)
clean_obj:
	rm *.o
