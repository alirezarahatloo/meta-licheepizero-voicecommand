SUMMARY = "UDP Packet Receiver"
DESCRIPTION = "A program to receive UDP packets on a specific port"
LICENSE = "CLOSED"

SRC_URI = "file://read_sql.c"

S = "${WORKDIR}"

DEPENDS = "freetds"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -o read_sql read_sql.c -lsybdb
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 read_sql ${D}${bindir}/read_sql
}

FILES_${PN} = "${bindir}/read_sql"








