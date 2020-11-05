NAME = list.out
MAIN_DIR = mainFiles
OBJ_DIR = objectsFiles
COMPILATOR = clang


$(NAME): $(OBJ_DIR)/listMain.o $(OBJ_DIR)/listFuncs.o
	$(COMPILATOR) $(OBJ_DIR)/listMain.o $(OBJ_DIR)/listFuncs.o -o $(NAME)

$(OBJ_DIR)/listMain.o: $(MAIN_DIR)/listMain.cpp
	$(COMPILATOR) -c $(MAIN_DIR)/listMain.cpp -o $(OBJ_DIR)/listMain.o

$(OBJ_DIR)/listFuncs.o: $(MAIN_DIR)/listFuncs.cpp
	$(COMPILATOR) -c $(MAIN_DIR)/listFuncs.cpp -o $(OBJ_DIR)/listFuncs.o