#-------------------------------------------------
#
# Project created by QtCreator 2020-01-06T14:40:55
#
#-------------------------------------------------

QT       += core gui
QT	 += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ELAN_v2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QCUSTOMPLOT_USE_OPENGL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"

SOURCES += \
    AddEventDialog.cpp \
    AssociateEegPosFileWindow.cpp \
    ButterflyOrganization.cpp \
    ButtonID.cpp \
    CentralMiddleLayerDisplayer.cpp \
    CentralMiddleLayerEvents.cpp \
    CentralMiddleLayerManager.cpp \
    ChangeMultipleAmplitudeScales.cpp \
    ChannelLabels.cpp \
    ChannelLabelsDisplayer.cpp \
    CheckboxID.cpp \
    ChooseActiveEventFileWindow.cpp \
    ChooseCursorWindow.cpp \
    ChooseFileWindow.cpp \
    ComboboxID.cpp \
    DataContainers/ChannelData.cpp \
    DataContainers/PageData.cpp \
    DataContainers/PlotData.cpp \
    DataContainers/PresentationData.cpp \
    DataContainers/ProjectData.cpp \
    EegDataStorage.cpp \
    EventDataStorage.cpp \
    EventLabels.cpp \
    EventLabelsDisplayer.cpp \
    GenerateMultipleButterfly.cpp \
    LineEditID.cpp \
    LoadProjectWindow.cpp \
    ProjectManager.cpp \
    RecodeCurrentEventDialog.cpp \
    RecodeEventDialog.cpp \
    SaveProjectWindow.cpp \
    SelectAlignmentReferenceWindow.cpp \
    SelectChannelWindow.cpp \
    SetCursorPositionWindow.cpp \
    SpinBoxID.cpp \
    UnloadEegFileWindow.cpp \
    UnloadEventFileWindow.cpp \
    main.cpp \
    MainWindow.cpp \
    CentralBottomLayer.cpp \
    CentralTopLayer.cpp \
    qcustomplot.cpp \
    ScaleWindow.cpp \
    SettingWindow.cpp \
    Wrapper.cpp \
    FileReader.cpp \
    EventCatcherTopLayer.cpp \
    EventWindow.cpp

HEADERS += \
    AddEventDialog.h \
	AssociateEegPosFileWindow.h \
	ButterflyOrganization.h \
    ButtonID.h \
    CentralMiddleLayerDisplayer.h \
	CentralMiddleLayerEvents.h \
    CentralMiddleLayerManager.h \
	ChangeMultipleAmplitudeScales.h \
    ChannelLabels.h \
    ChannelLabelsDisplayer.h \
    CheckboxID.h \
	ChooseActiveEventFileWindow.h \
	ChooseCursorWindow.h \
	ChooseFileWindow.h \
	ComboboxID.h \
	DataContainers/ChannelData.h \
	DataContainers/PageData.h \
	DataContainers/PlotData.h \
	DataContainers/PresentationData.h \
	DataContainers/ProjectData.h \
    EegDataStorage.h \
    EventDataStorage.h \
    EventLabels.h \
    EventLabelsDisplayer.h \
	GenerateMultipleButterfly.h \
	LineEditID.h \
    LoadProjectWindow.h \
    MainWindow.h \
    CentralBottomLayer.h \
    CentralTopLayer.h \
    ProjectManager.h \
    RecodeCurrentEventDialog.h \
    RecodeEventDialog.h \
    SaveProjectWindow.h \
	SelectAlignmentReferenceWindow.h \
    SelectChannelWindow.h \
	SetCursorPositionWindow.h \
    SpinBoxID.h \
	UnloadEegFileWindow.h \
	UnloadEventFileWindow.h \
    qcustomplot.h \
    ScaleWindow.h \
    SettingWindow.h \
    Wrapper.h \
#    ../../../usr/include/hdf5/serial/hdf5.h \
    /usr/include/hdf5/serial/hdf5.h \
    FileReader.h \
    EventCatcherTopLayer.h \
    EventWindow.h

FORMS += \
    AddEventDialog.ui \
    AssociateEegPosFileWindow.ui \
    ButterflyOrganization.ui \
    ChangeMultipleAmplitudeScales.ui \
    ChooseActiveEventFileWindow.ui \
    ChooseCursorWindow.ui \
    ChooseFileWindow.ui \
    GenerateMultipleButterfly.ui \
    LoadProjectWindow.ui \
    MainWindow.ui \
    RecodeCurrentEventDialog.ui \
    RecodeEventDialog.ui \
    SaveProjectWindow.ui \
    ScaleWindow.ui \
    SelectAlignmentReferenceWindow.ui \
    SelectChannelWindow.ui \
    SetCursorPositionWindow.ui \
    SettingWindow.ui \
    EventWindow.ui \
    UnloadEegFileWindow.ui \
    UnloadEventFileWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/release/ -lelanfile -lgen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/debug/ -lelanfile -lgen
else:unix: LIBS += -L$$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/ -lelanfile -lgen

INCLUDEPATH += $$PWD/../ELAN_GIT/Ubuntu_18.x86_64/include
DEPENDPATH += $$PWD/../ELAN_GIT/Ubuntu_18.x86_64/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/release/libelanfile.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/debug/libelanfile.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/release/elanfile.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/debug/elanfile.lib
else:unix: PRE_TARGETDEPS += $$PWD/../ELAN_GIT/Ubuntu_18.x86_64/lib/libelanfile.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial/release/ -lhdf5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial/debug/ -lhdf5
else:unix: LIBS += -L/usr/lib/x86_64-linux-gnu/hdf5/serial/ -lhdf5

INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial
DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial/release/libhdf5.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial/debug/libhdf5.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial/release/hdf5.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/hdf5/serial/debug/hdf5.lib
else:unix: PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5.a

DISTFILES += \
	Ressources/amplitudeUnitScales

RESOURCES = resources.qrc
