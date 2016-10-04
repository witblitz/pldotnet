BINDIR = /usr/lib/postgresql/9.5/bin
DOCDIR = /usr/share/doc/postgresql-doc-9.5
HTMLDIR = /usr/share/doc/postgresql-doc-9.5
INCLUDEDIR = /usr/include/postgresql
PKGINCLUDEDIR = /usr/include/postgresql
INCLUDEDIR-SERVER = /usr/include/postgresql/9.5/server
LIBDIR = /usr/lib/x86_64-linux-gnu
PKGLIBDIR = /usr/lib/postgresql/9.5/lib
LOCALEDIR = /usr/share/locale
MANDIR = /usr/share/postgresql/9.5/man
SHAREDIR = /usr/share/postgresql/9.5
SYSCONFDIR = /etc/postgresql-common
PGXS = /usr/lib/postgresql/9.5/lib/pgxs/src/makefiles/pgxs.mk
CONFIGURE = '--with-tcl' '--with-perl' '--with-python' '--with-pam' '--with-openssl' '--with-libxml' '--with-libxslt' '--with-tclconfig=/usr/lib/x86_64-linux-gnu/tcl8.6' '--with-includes=/usr/include/tcl8.6' 'PYTHON=/usr/bin/python' '--mandir=/usr/share/postgresql/9.5/man' '--docdir=/usr/share/doc/postgresql-doc-9.5' '--sysconfdir=/etc/postgresql-common' '--datarootdir=/usr/share/' '--datadir=/usr/share/postgresql/9.5' '--bindir=/usr/lib/postgresql/9.5/bin' '--libdir=/usr/lib/x86_64-linux-gnu/' '--libexecdir=/usr/lib/postgresql/' '--includedir=/usr/include/postgresql/' '--enable-nls' '--enable-integer-datetimes' '--enable-thread-safety' '--enable-tap-tests' '--enable-debug' '--disable-rpath' '--with-uuid=e2fs' '--with-gnu-ld' '--with-pgport=5432' '--with-system-tzdata=/usr/share/zoneinfo' 'CFLAGS=-g -O2 -fstack-protector-strong -Wformat -Werror=format-security -I/usr/include/mit-krb5 -fPIC -pie -fno-omit-frame-pointer' 'LDFLAGS=-Wl,-z,relro -Wl,-z,now -Wl,--as-needed -L/usr/lib/mit-krb5 -L/usr/lib/x86_64-linux-gnu/mit-krb5' '--with-krb5' '--with-gssapi' '--with-ldap' '--with-selinux' 'CPPFLAGS=-D_FORTIFY_SOURCE=2'
CC = gcc
CPPFLAGS = -D_FORTIFY_SOURCE=2 -D_GNU_SOURCE -I/usr/include/libxml2 -I/usr/include/tcl8.6
CFLAGS = -Wall -Wmissing-prototypes -Wpointer-arith -Wdeclaration-after-statement -Wendif-labels -Wmissing-format-attribute -Wformat-security -fno-strict-aliasing -fwrapv -fexcess-precision=standard -g -g -O2 -fstack-protector-strong -Wformat -Werror=format-security -I/usr/include/mit-krb5 -fPIC -pie -fno-omit-frame-pointer
CFLAGS_SL = -fpic
LDFLAGS = -L../../../src/common -Wl,-z,relro -Wl,-z,now -Wl,--as-needed -L/usr/lib/mit-krb5 -L/usr/lib/x86_64-linux-gnu/mit-krb5 -Wl,--as-needed
LDFLAGS_EX = 
LDFLAGS_SL = 
LIBS = -lpgcommon -lpgport -lselinux -lxslt -lxml2 -lpam -lssl -lcrypto -lgssapi_krb5 -lz -ledit -lrt -lcrypt -ldl -lm 
VERSION = PostgreSQL 9.5.4
