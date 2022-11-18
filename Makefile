CC = g++
TESTLIBS = -lgtest  -lgtest_main -lpthread
OBJS = $(wildcard *.cpp)

testSH: $(OBJS)
	$(CC) -o $@ $(OBJS) $(TESTLIBS)

%.o : %.cc
	$(CC) $(CCFLAGS) -c $<
	$(CC) $(CCFLAGS) -MM -MP -MT $@ $< > $(basename $@).d


.PHONY : clean
clean :
	rm -f *.o *~ *.d testSH *.gcov *.gcno *.gcda


## include the generated dependency files
-include $(addsuffix .d,$(basename $(OBJS)))
