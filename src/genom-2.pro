QT += widgets

HEADERS += \
    Matrix.hpp \
    procedures.hpp \
    saveSequence.hpp \
    Sequence.hpp \
    user_interaction.hpp \
    utility.hpp \
    window.hpp \
    matrixFromSequence.hpp \
    sequenceFromMatrix.hpp \
    resultsWindow.hpp \
    genomic_coordinates.hpp \
    askBaseProb.hpp

SOURCES += \
    mainQt.cpp \
    Matrix.cpp \
    procedures.cpp \
    saveSequence.cpp \
    Sequence.cpp \
    user_interactionQt.cpp \
    utility.cpp \
    window.cpp \
    matrixFromSequence.cpp \
    sequenceFromMatrix.cpp \
    resultsWindow.cpp \
    genomic_coordinates.cpp \
    askBaseProb.cpp

FORMS += \
    saveSequence.ui \
    window.ui \
    matrixFromSequence.ui \
    sequenceFromMatrix.ui \
    resultsWindow.ui \
    askBaseProb.ui
