##
## Main.
##

##
RM = rm -v -f

##
ALLLIBS = \
	engine.a

##
ALLPROGS = \
	tp1 \
	tp2 \
	tp3 \
	tp4 \
	tp5

##
all: stamp $(ALLLIBS) $(ALLPROGS)
	@echo "Done."

##
stamp:
	./mkversion

##
engine.a:
	(cd engine && make -f Makefile.engine all)

##
tp1: $(ALLLIBS)
	(cd tp1 && make -f Makefile.tp1 all)

##
tp2: $(ALLLIBS)
	(cd tp2 && make -f Makefile.tp2 all)

##
tp3: $(ALLLIBS)
	(cd tp3 && make -f Makefile.tp3 all)

##
tp4: $(ALLLIBS)
	(cd tp4 && make -f Makefile.tp4 all)

##
tp5: $(ALLLIBS)
	(cd tp5 && make -f Makefile.tp5 all)

##
clean:
	(cd engine && make -f Makefile.engine clean)
	(cd tp1 && make -f Makefile.tp1 clean)
	(cd tp2 && make -f Makefile.tp2 clean)
	(cd tp3 && make -f Makefile.tp3 clean)
	(cd tp4 && make -f Makefile.tp4 clean)
	(cd tp5 && make -f Makefile.tp5 clean)
	$(RM) doxygenwarn.txt
	$(RM) dmake.output
	$(RM) libs/engine.a

##
cex:
	(cd engine && make -f Makefile.engine cex)
	(cd tp1 && make -f Makefile.tp1 cex)
	(cd tp2 && make -f Makefile.tp2 cex)
	(cd tp3 && make -f Makefile.tp3 cex)
	(cd tp4 && make -f Makefile.tp4 cex)
	(cd tp5 && make -f Makefile.tp5 cex)

##
lexers:
	(cd engine && make -f Makefile.engine lexers)

##
splint:
	
	(cd engine && make -f Makefile.engine splint)

##
preset:
	(cd engine && make -f Makefile.engine preset)
	(cd tp1 && make -f Makefile.tp1 preset)
	(cd tp2 && make -f Makefile.tp2 preset)
	(cd tp3 && make -f Makefile.tp3 preset)
	(cd tp4 && make -f Makefile.tp4 preset)
	(cd tp5 && make -f Makefile.tp5 preset)

## End.
