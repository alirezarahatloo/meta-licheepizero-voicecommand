SUMMARY = "Voice Command Application"
DESCRIPTION = "Simple C++ voice command application"
LICENSE = "CLOSED"

SRC_URI = "\
    file://main.cpp \
    file://mfcc.cpp \
    file://mfcc.h \
    file://wav_reader.cpp \
    file://wav_reader.h \
"

S = "${WORKDIR}"

DEPENDS += "libsndfile1"

do_compile() {
    ${CXX} ${CPPFLAGS} ${CXXFLAGS} \
        ${S}/main.cpp \
        ${S}/mfcc.cpp \
        ${S}/wav_reader.cpp \
        ${LDFLAGS} \
        -lsndfile \
        -o voicecommand
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 voicecommand ${D}${bindir}/voicecommand
}

FILES:${PN} += "${bindir}/voicecommand"
