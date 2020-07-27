# Makefile for Meta-Object Code (MOC) generation
# Run this before invoking CDT's build process.

Q_OBJECTS = signeditor/SignEditor.h signeditor/SignWidget.h \
	signeditor/SignTreeDetailsWidget.h signeditor/pages/PageBlinkAnimation.h \
	signeditor/pages/PageCompose.h signeditor/pages/PageDisplay.h \
	signeditor/pages/PageFill.h signeditor/pages/PageMarqueeAnimation.h \
	signeditor/pages/PageSign.h signeditor/pages/PageSplit.h \
	signeditor/pages/PageText.h signeditor/pages/PageEmptyCell.h \
	signeditor/SignTreeWidget.h signeditor/SignTreeQtModel.h \
	signeditor/SignTreeContextMenuProvider.h \
	fontstudio/FontStudio.h fontstudio/FontVisualEditor.h \
	fontstudio/FontQtModel.h fontstudio/CharacterListWidget.h \
	fontstudio/NoSuchCharWidget.h \
	SiganimMainWindow.h SiganimUICore.h

MOCS = $(join $(dir $(Q_OBJECTS)), $(addsuffix .cpp, $(addprefix moc_, $(basename $(notdir $(Q_OBJECTS))))))

DEFINES = 

INCPATH = 

all: $(MOCS)

moc_%.cpp: %.h
	moc $(DEFINES) $(INCPATH) $< -o $@
