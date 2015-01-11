
QT       += core #gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minigo
TEMPLATE = app

HEADERS += \
    src/Board.h \
    src/boardview.h \
    src/MiniGo.h \
    src/Point.h \
    src/SetupDialog.h \
    src/State.h \
    src/time.h \
    src/rules.h

SOURCES += \
    src/boardview.cpp \
    src/MiniGo.cpp \
    src/SetupDialog.cpp \
    src/State.cpp \
    src/Board.cpp \
    src/time.cpp \
    src/main.cpp \
    src/Point.cpp

FORMS += \
    MiniGo.ui \
    SetupDialog.ui

# cuda source
#CUDA_SOURCES += src/rules.cu
#CUDA_SOURCES += src/Point.cu src/rules.cu

# Path to cuda toolkit install
CUDA_DIR = /usr/local/cuda-5.5

INCLUDEPATH += /home/andrew/boost_1_55_0 \ $$CUDA_DIR/include

# library directories
QMAKE_LIBDIR += $$CUDA_DIR/lib64

# GPU architecture
CUDA_ARCH = sm_21

QMAKE_CXXFLAGS += -fopenmp -Wall

#CUDALIBS += -lcudart
#CUDALIBS += -lcudart -lcuda

#LIBS += -fopenmp
LIBS += -fopenmp $$CUDALIBS

OTHER_FILES += $$CUDA_SOURCES

# join the includes in a line
#CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')
CUDA_INC = $$join($$CUDA_DIR/include,' -I','-I',' ')
#cuda.commands = nvcc -g -G -O0 -arch=$$CUDA_ARCH -rdc=true -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
cuda.commands = nvcc -g -G -O0 -arch=$$CUDA_ARCH -c $$CUDA_INC $$CUDALIBS ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
cuda.dependcy_type = TYPE_C
#cuda.depend_command = nvcc -O0 -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}

cuda.input = CUDA_SOURCES
#cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}.o
cuda.output = ${QMAKE_FILE_BASE}_cuda.o

# Tell Qt that we want add more stuff to the Makefile
QMAKE_EXTRA_COMPILERS += cuda
