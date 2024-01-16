BUILD_NAME = explorer
BUILD_DIR = build
SRC_DIR = src

PATHS = $(shell find $(SRC_DIR) -type d)

# Collect all headers and source files inside every subfolder
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
HEADERS = $(shell find $(SRC_DIR) -name '*.hpp')

# Collect all object files
OBJECTS = $(SOURCES:%= $(BUILD_DIR)/%.o)

# Collect all directories
DIRS = $(shell find $(SRC_DIR) -type d)

$(BUILD_NAME): $(OBJECTS) 
	g++ -I $() -pthread  $(OBJECTS) -lncurses -o $@ 

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	g++ -I $(SRC_DIR) -pthread -c $< -lncurses -o $@

run: explorer
	./explorer

echo: 
	@echo $(BUILD_DIR)/$*.cpp.o
	@echo $(SOURCES)
	@echo $(HEADERS)
	@echo $(OBJECTS)

delete:
	rm -rf $(BUILD_DIR)
	rm -f $(BUILD_NAME)

clean:
	rm -f $(BUILD_NAME) $(OBJECTS)