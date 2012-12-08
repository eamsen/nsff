SRCDIR:=src
BINDIR:=bin
OBJDIR:=bin/obj
CXX:=g++ -std=c++0x
CFLAGS:=-Wall -O3
LIBS:=-lpthread
BINS:=nsff

OBJS:=$(notdir $(basename $(wildcard $(SRCDIR)/*.cc)))
OBJS:=$(addsuffix .o, $(filter-out $(BINS), $(OBJS)))
OBJS:=$(addprefix $(OBJDIR)/, $(OBJS))
BINS:=$(addprefix $(BINDIR)/, $(BINS))

compile: makedirs $(BINS)
	@echo "compiled all"

makedirs:
	@mkdir -p bin/obj

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f $(BINS)
	@echo "cleaned"

.PRECIOUS: $(OBJS) $(TSTOBJS)
.PHONY: compile makedirs clean

$(BINDIR)/%: $(OBJS) $(SRCDIR)/%.cc
	@$(CXX) $(CFLAGS) -o $(OBJDIR)/$(@F).o -c $(SRCDIR)/$(@F).cc
	@$(CXX) $(CFLAGS) -o $(BINDIR)/$(@F) $(OBJDIR)/$(@F).o $(OBJS) $(LIBS)
	@echo "compiled $(BINDIR)/$(@F)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(SRCDIR)/%.h
	@$(CXX) $(CFLAGS) -o $(OBJDIR)/$(@F) -c $<
