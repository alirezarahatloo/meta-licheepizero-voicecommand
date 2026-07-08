SUMMARY = "FreeTDS - A free implementation of Sybase's DB-Library, CT-Library and ODBC"
HOMEPAGE = "http://www.freetds.org/"
SECTION = "libs"
LICENSE = "LGPLv2+"
LIC_FILES_CHKSUM = "file://COPYING.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "https://www.freetds.org/files/stable/freetds-1.3.10.tar.gz"
SRC_URI[md5sum] = "5f011b9333a57802b11bb86d35473405"


inherit autotools pkgconfig

# Enable OpenSSL & unixODBC
PACKAGECONFIG ??= "openssl unixodbc"

PACKAGECONFIG[openssl] = "--with-openssl=${STAGING_DIR_TARGET}/usr,--without-openssl,openssl"
PACKAGECONFIG[unixodbc] = "--with-unixodbc=${STAGING_DIR_TARGET}/usr,--without-unixodbc,unixodbc"

DEPENDS += "openssl unixodbc"

EXTRA_OECONF = "--disable-libiconv"


