COMPILER_NAME                = g++
COMPILER_FLAGS               = -W -Wall -Wno-unused-parameter -g -O2 -std=c++11 -I/usr/include/libxml2
LINKER_NAME                  = g++
LINKER_FLAGS                 = -L/usr/lib -lxml2 -lpthread
INCLUDES_DIRECTORY           = src/include
INCLUDES_EXTENSION           = .hpp
INCLUDES_NAME                = $(wildcard $(INCLUDES_DIRECTORY)/*$(INCLUDES_EXTENSION))
SOURCES_DIRECTORY            = src
SOURCES_EXTENSION            = .cpp
SOURCES_NAME                 = $(wildcard $(SOURCES_DIRECTORY)/*$(SOURCES_EXTENSION))
SOURCES_MAIN_DIRECTORY       = src/main
SOURCES_MAIN_EXTENSION       = .cpp
SOURCES_MAIN_NAME            = $(wildcard $(SOURCES_MAIN_DIRECTORY)/*$(SOURCES_MAIN_EXTENSION))
OBJECTS_DIRECTORY            = build
OBJECTS_EXTENSION            = .o
OBJECTS_NAME                 = $(addprefix $(OBJECTS_DIRECTORY)/, $(notdir $(SOURCES_NAME:$(SOURCES_EXTENSION)=$(OBJECTS_EXTENSION))))
OBJECTS_MAIN_DIRECTORY       = build/main
OBJECTS_MAIN_EXTENSION       = .o
OBJECTS_MAIN_NAME            = $(addprefix $(OBJECTS_MAIN_DIRECTORY)/, $(notdir $(SOURCES_MAIN_NAME:$(SOURCES_MAIN_EXTENSION)=$(OBJECTS_MAIN_EXTENSION))))
BINARIES_DIRECTORY           = bin
BINARIES_EXTENSION           = 
BINARIES_NAME                = $(addprefix $(BINARIES_DIRECTORY)/, $(notdir $(OBJECTS_MAIN_NAME:$(OBJECTS_MAIN_EXTENSION)=$(BINARIES_EXTENSION))))

all: create $(BINARIES_NAME)

$(BINARIES_DIRECTORY)/%$(BINARIES_EXTENSION): $(OBJECTS_NAME) $(OBJECTS_MAIN_DIRECTORY)/%$(OBJECTS_MAIN_EXTENSION)
	@$(LINKER_NAME) -o $@ $^ $(LINKER_FLAGS)

$(OBJECTS_DIRECTORY)/%$(OBJECTS_EXTENSION): $(SOURCES_DIRECTORY)/%$(SOURCES_EXTENSION)
	@$(COMPILER_NAME) -o $@ -c $< $(COMPILER_FLAGS)

$(OBJECTS_MAIN_DIRECTORY)/%$(OBJECTS_MAIN_EXTENSION): $(SOURCES_MAIN_DIRECTORY)/%$(SOURCES_MAIN_EXTENSION)
	@$(COMPILER_NAME) -o $@ -c $< $(COMPILER_FLAGS)

.PRECIOUS: $(OBJECTS_NAME) $(OBJECTS_MAIN_NAME)

.PHONY: create mrproper clean install

create:
	@mkdir -p $(OBJECTS_DIRECTORY)
	@mkdir -p $(OBJECTS_MAIN_DIRECTORY)
	@mkdir -p $(BINARIES_DIRECTORY)

mrproper:
	@rm -f $(OBJECTS_NAME)
	@rm -f $(OBJECTS_MAIN_NAME)
	@rm -f $(BINARIES_NAME)

clean:
	@rm -f $(addsuffix ~, $(SOURCES_NAME))
	@rm -f $(addsuffix ~, $(SOURCES_MAIN_NAME))
	@rm -f $(addsuffix ~, $(INCLUDES_NAME))
	@rm -rf $(OBJECTS_DIRECTORY)
	@rm -rf $(BINARIES_DIRECTORY)

install:
	@sudo cp $(BINARIES_DIRECTORY)/osmtorus /usr/sbin
	@sudo mkdir /etc/osmtorus
	@sudo cp daemon_init/osmtorus.conf /etc/osmtorus
	@sudo cp daemon_init/osmtorus /etc/init.d
	@sudo chmod +x /etc/init.d/osmtorus

uninstall:
	@sudo /etc/init.d/osmtorus stop
	@sudo rm /etc/init.d/osmtorus
	@sudo rm -rf /etc/osmtorus
	@sudo rm /usr/sbin/osmtorus

startup:
	@sudo update-rc.d osmtorus defaults

startdown:
	@sudo update-rc.d -f osmtorus remove
