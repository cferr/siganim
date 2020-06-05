# Makefile for Meta-Object Code (MOC) generation
# Run this before invoking CDT's build process.

Q_OBJECTS = SignEditor.h FontStudio.h SignWidget.h SiganimMainWindow.h \
	SignTreeDetailsWidget.h pages/PageBlinkAnimation.h pages/PageCompose.h \
	pages/PageDisplay.h pages/PageFill.h pages/PageMarqueeAnimation.h \
	pages/PageSign.h pages/PageSplit.h pages/PageText.h pages/PageEmptyCell.h

MOCS = $(join $(dir $(Q_OBJECTS)), $(addsuffix .cpp, $(addprefix moc_, $(basename $(notdir $(Q_OBJECTS))))))

DEFINES = 

INCPATH = 

all: $(MOCS)

moc_%.cpp: %.h
	moc $(DEFINES) $(INCPATH) $< -o $@
