# Builds every example into ./bin as <folder>_<file>
#   make        - build all
#   make run    - build and run all
#   make clean  - remove binaries

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -O1
LDFLAGS  ?= -pthread

SOURCES := $(shell find . -name '*.cpp' -not -path './build/*' | sort)
# ./01-SOLID/O-open-closed/good.cpp -> bin/O-open-closed_good
BINARIES := $(foreach s,$(SOURCES),bin/$(notdir $(patsubst %/,%,$(dir $(s))))_$(basename $(notdir $(s))))

.PHONY: all run clean

all: $(BINARIES)

define BUILD_RULE
bin/$(notdir $(patsubst %/,%,$(dir $(1))))_$(basename $(notdir $(1))): $(1) | bin
	$$(CXX) $$(CXXFLAGS) $$< -o $$@ $$(LDFLAGS)
endef
$(foreach s,$(SOURCES),$(eval $(call BUILD_RULE,$(s))))

bin:
	mkdir -p bin

run: all
	@for b in $(BINARIES); do echo "==================== $$b"; ./$$b; echo; done

clean:
	rm -rf bin
