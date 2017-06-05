program_name = lab07
obj_list = $(program_name).o
compiler = cc

spg: $(obj_list)
	$(compiler) -std=c99  $(obj_list) -o $(program_name)
.o:
	$(compiler) -c -std=c99 $^
clean: clean_obj
	rm $(program_name)
clean_obj:
	rm *.o
test: spg
	./666
	kill -1 $(pidof '666')
	cat output.log
