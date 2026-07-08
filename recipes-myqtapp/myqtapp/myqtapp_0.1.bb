SUMMARY = "Qt application to connect to SQL Server"
LICENSE = "CLOSED"

#Add whatever you need here
DEPENDS += " qtbase qtserialport"

# Source files
SRC_URI = "file://myqtapp.cpp \
           file://myqtapp.pro"

S = "${WORKDIR}"

# Install the application
do_install() {
    install -d ${D}${bindir}
    install -m 0755 myqtapp ${D}${bindir}
}

FILES_${PN} += "${bindir}/myqtapp"
# Use qmake to build the application
inherit qmake5
