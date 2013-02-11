COMPILER_NAME                = g++
COMPILER_FLAGS               = -W -Wall -g -O2 -std=c++0x
LINKER_NAME                  = g++
LINKER_FLAGS                 = -lpthread
INCLUDES_DIRECTORY           = src/include
INCLUDES_EXTENSION           = .hpp
INCLUDES_NAME                = $(wildcard $(INCLUDES_DIRECTORY)/*$(INCLUDES_EXTENSION))
INCLUDES_LIBRARIES_DIRECTORY = src/lib/include
INCLUDES_LIBRARIES_EXTENSION = .hpp
INCLUDES_LIBRARIES_NAME      = $(wildcard $(INCLUDES_LIBRARIES_DIRECTORY)/*$(INCLUDES_LIBRARIES_EXTENSION))
SOURCES_DIRECTORY            = src
SOURCES_EXTENSION            = .cpp
SOURCES_NAME                 = $(wildcard $(SOURCES_DIRECTORY)/*$(SOURCES_EXTENSION))
SOURCES_LIBRARIES_DIRECTORY  = src/lib
SOURCES_LIBRARIES_EXTENSION  = .cpp
SOURCES_LIBRARIES_NAME       = $(wildcard $(SOURCES_LIBRARIES_DIRECTORY)/*$(SOURCES_LIBRARIES_EXTENSION))
SOURCES_MAIN_DIRECTORY       = src/main
SOURCES_MAIN_EXTENSION       = .cpp
SOURCES_MAIN_NAME            = $(wildcard $(SOURCES_MAIN_DIRECTORY)/*$(SOURCES_MAIN_EXTENSION))
OBJECTS_DIRECTORY            = build
OBJECTS_EXTENSION            = .o
OBJECTS_NAME                 = $(addprefix $(OBJECTS_DIRECTORY)/, $(notdir $(SOURCES_NAME:$(SOURCES_EXTENSION)=$(OBJECTS_EXTENSION))))
OBJECTS_LIBRARIES_DIRECTORY  = build/lib
OBJECTS_LIBRARIES_EXTENSION  = .o
OBJECTS_LIBRARIES_NAME       = $(addprefix $(OBJECTS_LIBRARIES_DIRECTORY)/, $(notdir $(SOURCES_LIBRARIES_NAME:$(SOURCES_LIBRARIES_EXTENSION)=$(OBJECTS_LIBRARIES_EXTENSION))))
OBJECTS_MAIN_DIRECTORY       = build/main
OBJECTS_MAIN_EXTENSION       = .o
OBJECTS_MAIN_NAME            = $(addprefix $(OBJECTS_MAIN_DIRECTORY)/, $(notdir $(SOURCES_MAIN_NAME:$(SOURCES_MAIN_EXTENSION)=$(OBJECTS_MAIN_EXTENSION))))
BINARIES_DIRECTORY           = bin
BINARIES_EXTENSION           = 
BINARIES_NAME                = $(addprefix $(BINARIES_DIRECTORY)/, $(notdir $(OBJECTS_MAIN_NAME:$(OBJECTS_MAIN_EXTENSION)=$(BINARIES_EXTENSION))))

all: create mrproper $(BINARIES_NAME)

$(BINARIES_DIRECTORY)/%$(BINARIES_EXTENSION): $(OBJECTS_NAME) $(OBJECTS_LIBRARIES_NAME) $(OBJECTS_MAIN_DIRECTORY)/%$(OBJECTS_MAIN_EXTENSION)
	@$(LINKER_NAME) -o $@ $^ $(LINKER_FLAGS)

$(OBJECTS_DIRECTORY)/%$(OBJECTS_EXTENSION): $(SOURCES_DIRECTORY)/%$(SOURCES_EXTENSION)
	@$(COMPILER_NAME) -o $@ -c $< $(COMPILER_FLAGS)

$(OBJECTS_LIBRARIES_DIRECTORY)/%$(OBJECTS_LIBRARIES_EXTENSION): $(SOURCES_LIBRARIES_DIRECTORY)/%$(SOURCES_LIBRARIES_EXTENSION)
	@$(COMPILER_NAME) -o $@ -c $< $(COMPILER_FLAGS)

$(OBJECTS_MAIN_DIRECTORY)/%$(OBJECTS_MAIN_EXTENSION): $(SOURCES_MAIN_DIRECTORY)/%$(SOURCES_MAIN_EXTENSION)
	@$(COMPILER_NAME) -o $@ -c $< $(COMPILER_FLAGS)

.PRECIOUS: $(OBJECTS_NAME) $(OBJECTS_LIBRARIES_NAME) $(OBJECTS_MAIN_NAME)

.PHONY: create mrproper clean install

create:
	@mkdir -p $(OBJECTS_DIRECTORY)
	@mkdir -p $(OBJECTS_LIBRARIES_DIRECTORY)
	@mkdir -p $(OBJECTS_MAIN_DIRECTORY)
	@mkdir -p $(BINARIES_DIRECTORY)

mrproper:
	@rm -f $(OBJECTS_NAME)
	@rm -f $(OBJECTS_LIBRARIES_NAME)
	@rm -f $(OBJECTS_MAIN_NAME)
	@rm -f $(BINARIES_NAME)

clean:
	@rm -f $(addsuffix ~, $(SOURCES_NAME))
	@rm -f $(addsuffix ~, $(SOURCES_LIBRARIES_NAME))
	@rm -f $(addsuffix ~, $(SOURCES_MAIN_NAME))
	@rm -f $(addsuffix ~, $(INCLUDES_NAME))
	@rm -f $(addsuffix ~, $(INCLUDES_LIBRARIES_NAME))
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
