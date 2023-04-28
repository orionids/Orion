~/.minttyrc : $(MATE_ROOT)/src/minttyrc.mk
	@echo -e "BoldAsFont=-1\n\
FontHeight=16\n\
CursorType=block\n\
CursorBlinks=no\n\
CopyOnSelect=no\n\
CopyAsRTF=no\n\
Columns=61\n\
Rows=33\n\
Scrollbar=none\n\
Blue=66,139,210\n\
BoldBlue=80,170,255" > ~/.minttyrc
