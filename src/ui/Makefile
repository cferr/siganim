# Makefile for Meta-Object Code (MOC) generation
# Run this before invoking CDT's build process.

Q_OBJECTS = SignWidget.h SiganimMainWindow.h

MOCS = $(addprefix moc_, $(Q_OBJECTS:.h=.cpp))

DEFINES = 

INCPATH = 

all: $(MOCS)

moc_%.cpp: %.h
	moc $(DEFINES) $(INCPATH) $< -o $@
