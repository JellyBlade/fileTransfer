CC = g++
TESTLIBS = -lgtest  -lgtest_main -lpthread
OBJS = headerManager.o test_headerManager.o

testSH: $(OBJS)
	$(CC) -o $@  $(OBJS) $(TESTLIBS)

%.o : %.cc
	$(CC) $(CCFLAGS) -c $<
	$(CC) $(CCFLAGS) -MM -MP -MT $@ $< > $(basename $@).d


.PHONY : clean
clean :
	rm -f *.o *~ *.d


## include the generated dependency files
-include $(addsuffix .d,$(basename $(OBJS)))
