OBJS = $(patsubst %c,%o,$(SRCS))
SHARED_OBJS = $(patsubst %c,%shared_o,$(SRCS))
OBJS_FIND_FROM_HERE_RECURSIVELY := $(foreach dir, $(SUB_DIRS), $(wildcard $(dir)/*.o))
SHARED_OBJS_FIND_FROM_HERE_RECURSIVELY := $(foreach dir, $(SUB_DIRS), $(wildcard $(dir)/*.shared_o))

INCLUDE += -I$(PROJECT_ROOT_DIR) -I$(PROJECT_ROOT_DIR)/util/inc

ifdef WIN32
ifeq ($(strip $(WIN32)),TRUE)
DEFINE += -DWIN32
DEFINE_FOR_SHARED_OBJS +=
endif
endif

CFLAGS_FOR_OBJS += $(INCLUDE) $(DEFINE)
CFLAGS_FOR_SHARED_OBJS += $(INCLUDE) $(DEFINE) $(DEFINE_FOR_SHARED_OBJS)

build_objs:
	@echo "=================================================="
	@echo "   Preparing to compile source files..."
	@echo "=================================================="
	@make .o.depend
	@echo "=================================================="
	@echo "   Start compiling source files..."
	@echo "=================================================="
	@make build_objs_real

build_objs_real: $(OBJS)

build_shared_objs:
	@echo "=================================================="
	@echo "   Preparing to compile source files..."
	@echo "=================================================="
	@make .shared_o.depend
	@echo "=================================================="
	@echo "   Start compiling source files..."
	@echo "=================================================="
	@make build_shared_objs_real

build_shared_objs_real: $(SHARED_OBJS)

build_subdir:
	@for dir in $(SUB_DIRS); do \
	  echo "=================================================="; \
	  echo "   Building $$dir"; \
	  echo "=================================================="; \
		cd $$dir; \
		make all; \
		cd ..; \
	done

clean_subdir:
	@for dir in $(SUB_DIRS); do \
	  echo "=================================================="; \
	  echo "   Cleaning $$dir"; \
	  echo "=================================================="; \
		cd $$dir; \
		make clean; \
		cd ..; \
	done

install_subdir:
	@for dir in $(SUB_DIRS); do \
		cd $$dir; \
		make install; \
		cd ..; \
	done

.o.depend: $(SRCS)
	@echo "=================================================="
	@echo "   Checking dependencies..."
	@echo "=================================================="
	@$(CC) $(CFLAGS_FOR_OBJS) -M $(SRCS) > $@

.shared_o.depend: $(SRCS)
	@echo "=================================================="
	@echo "   Checking dependencies..."
	@echo "=================================================="
	@$(CC) $(CFLAGS_FOR_SHARED_OBJS) -M $(SRCS) > $@.$$$$; \
	$(SED) 's,\($*\).o[ :]*,\1.shared_o : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$

%.o: %.c
	@echo "Compile $<..."
	@$(CC) $(CFLAGS_FOR_OBJS) -Wall -g3 -c -o $@ $<

%.shared_o: %.c
	@echo "Compile $<..."
	@$(CC) $(CFLAGS_FOR_SHARED_OBJS) -fPIC -Wall -g3 -c -o $@ $<

.libs/lib$(MODULE).so.$(MAJOR_VERSION).$(MINOR_VERSION): $(SHARED_OBJS_FIND_FROM_HERE_RECURSIVELY)
	@echo "=================================================="
	@echo "   Creating lib$(MODULE).so..."
	@echo "=================================================="
	@$(MKDIR) -p .libs
	@$(CC) -shared -Wl,-soname,lib$(MODULE).so.$(MAJOR_VERSION) -Wl,--out-implib,.libs/lib$(MODULE).a -o .libs/lib$(MODULE).so.$(MAJOR_VERSION).$(MINOR_VERSION) $(SHARED_OBJS_FIND_FROM_HERE_RECURSIVELY) $(LIBS)
	@$(LN) -sf .libs/lib$(MODULE).so.$(MAJOR_VERSION).$(MINOR_VERSION) .libs/lib$(MODULE).so.$(MAJOR_VERSION)
	@$(LN) -sf .libs/lib$(MODULE).so.$(MAJOR_VERSION).$(MINOR_VERSION) .libs/lib$(MODULE).so

.libs/lib$(MODULE).a: $(OBJS_FIND_FROM_HERE_RECURSIVELY)
	@echo "=================================================="
	@echo "   Creating lib$(MODULE).a..."
	@echo "=================================================="
	@$(MKDIR) -p .libs
	@$(AR) -ru .libs/lib$(MODULE).a $(OBJS_FIND_FROM_HERE_RECURSIVELY)

clean_objs:
	@$(RM) -f $(OBJS) $(SHARED_OBJS) *~ *.bak .o.depend .shared_o.depend

clean_libs:
	@$(RM) -rf .libs

install_module_shared_library:
	@$(MKDIR) -p $(INSTALL_PREFIX)\lib
	@$(CP) -dpR .libs\lib$(MODULE).so.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_PREFIX)\lib
	@$(CP) -dpR .libs\lib$(MODULE).so.$(MAJOR_VERSION) $(INSTALL_PREFIX)\lib
	@$(CP) -dpR .libs\lib$(MODULE).so $(INSTALL_PREFIX)\lib

install_module_library:
	@$(MKDIR) -p $(INSTALL_PREFIX)\lib
	@$(CP) -dpR .libs\lib$(MODULE).a $(INSTALL_PREFIX)\lib

ifeq (.o.depend,$(wildcard .o.depend))
include .o.depend
endif

ifeq (.shared_o.depend,$(wildcard .shared_o.depend))
include .shared_o.depend
endif
