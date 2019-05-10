cc := g++
ifeq ($(ver), debug)
prom:= testdns_d
CXXFLAGS := -m32 -g -Ddebug
else
prom:= testdns
CXXFLAGS := -m32
endif
deps := $(wildcard *.h)
src := $(wildcard *.cpp)
obj := $(patsubst %.cpp, %.o, $(src)) 

$(prom): $(obj)
	$(cc) $(CXXFLAGS) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)
