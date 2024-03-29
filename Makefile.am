#
# Excelfore Universal Base Library
# Copyright (C) 2019 Excelfore Corporation (https://excelfore.com)
#
# This file is part of Excelfore-unibase.
#
# Excelfore-unibase is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Excelfore-unibase is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Excelfore-unibase.  If not, see
# <https://www.gnu.org/licenses/old-licenses/gpl-2.0.html>.
#
PACKAGENAME=@PACKAGE_NAME@
VERSION=@PACKAGE_VERSION@
DESCRIPTION=@PACKAGE_DESC@
lib_LTLIBRARIES = libx4unibase.la
libx4unibase_la_SOURCES = \
unibase.h unibase_private.h unibase.c \
ub_logging.h ub_logging.c \
ub_esarray.h ub_esarray.c \
ub_strutils.h ub_strutils.c \
ub_llist.h ub_llist.c \
ub_confutils.h ub_confutils.c

pkginclude_HEADERS = \
unibase.h \
unibase_macros.h \
ub_logging.h \
ub_esarray.h \
ub_strutils.h \
ub_llist.h \
ub_confutils.h

pkgincludedir=$(includedir)/$(PACKAGENAME)
docdir=${datarootdir}/doc/$(PACKAGENAME)
pkgdatadir=${datarootdir}/$(PACKAGENAME)

AM_CFLAGS = -I. -Wall -Wextra -Wno-unused-parameter \
	-DXL4PKGVERSION=\"$(VERSION)-`cd $(srcdir);echo opensrc`\"
ACLOCAL_AMFLAGS = -I m4

if UNITTEST_MEMTEST
AM_CFLAGS += -DUB_OVERRIDE_H=\"ub_cmocka_unittest.h\"
endif

UNITY_SRCDIR = $(srcdir)/Unity/src
check_PROGRAMS = ub_logging_unittest ub_esarray_unittest unibase_macros_unittest \
ub_llist_unittest ub_genconf_unittest ub_confutils_unittest ub_strutils_unittest
ub_logging_unittest_SOURCES = ub_logging_unittest.c
ub_logging_unittest_LDADD = libx4unibase.la
ub_logging_unittest_LDFLAGS = -lcmocka
ub_esarray_unittest_SOURCES = ub_esarray_unittest.c
ub_esarray_unittest_LDADD = libx4unibase.la
ub_esarray_unittest_LDFLAGS = -lcmocka
unibase_macros_unittest_SOURCES=unibase_macros_unittest.c
unibase_macros_unittest_LDADD = libx4unibase.la
unibase_macros_unittest_LDFLAGS = -lcmocka
ub_llist_unittest_SOURCES = ub_llist_unittest.c
ub_llist_unittest_LDADD = libx4unibase.la
ub_llist_unittest_LDFLAGS = -lcmocka
ub_strutils_unittest_SOURECE = ub_strutils_unittest.c
ub_strutils_unittest_LDADD = libx4unibase.la
ub_strutils_unittest_LDFLAGS = -lcmocka

if POSIX_PLATFORM
libx4unibase_la_SOURCES += \
unibase_binding.h \
unibase_posix.c
pkginclude_HEADERS += \
unibase_binding.h
check_PROGRAMS += ub_logging_posix_unittest ub_esarray_posix_unittest
AM_MAKEFLAGS = CMOCKA_TEST_ABORT='1'
ub_logging_posix_unittest_SOURCES = ub_logging_posix_unittest.c
ub_logging_posix_unittest_LDADD = libx4unibase.la
ub_logging_posix_unittest_LDFLAGS = -lpthread -lcmocka
ub_esarray_posix_unittest_SOURCES = ub_esarray_posix_unittest.c
ub_esarray_posix_unittest_LDADD = libx4unibase.la
ub_esarray_posix_unittest_LDFLAGS = -lpthread -lcmocka
endif

pkgconfig_DATA=$(PACKAGENAME).pc

ubconftest_conf.h ubconftest_conf.c ubconftest_values.c: ubconftest_def.h ub_genconf.py
	$(srcdir)/ub_genconf.py -i $(srcdir)/ubconftest_def.h -c ubconftest_conf.c \
	-d ubconftest_conf.h -v ubconftest_values.c -p ubconftest

ub_genconf_unittest.c: ubconftest_conf.c ubconftest_values.c
	echo "#include \"ubconftest_def.h\"" > $@
	echo "#include \"ubconftest_conf.h\"" >> $@
	echo "extern int ubconftestconf_values_test(void);" >> $@
	echo "int main(void){return ubconftestconf_values_test();}" >> $@

ub_genconf_unittest_SOURCES = ub_genconf_unittest.c ubconftest_conf.c ubconftest_values.c

ub_confutils_unittest_SOURCES = ub_confutils_unittest.c ubconftest_conf.c ubconftest_values.c
ub_confutils_unittest_LDADD = libx4unibase.la
ub_confutils_unittest_LDFLAGS = -lpthread -lcmocka

TESTS = $(check_PROGRAMS)

DOC_HTML_INDEX=$(srcdir)/docoutput/html/index.html
DOC_LATEC_REFMAN=$(srcdir)/docoutput/latex/refman.pdf
TARGET_PDF=$(srcdir)/docoutput/unibase_apis.pdf
TEX_TEMPLATE = $(srcdir)/doc/xl4_header_template.tex
doc: $(TARGET_PDF)
$(srcdir)/xl4_header.tex: $(TEX_TEMPLATE) Makefile.am
	cp $(TEX_TEMPLATE) $@
	DOCDATE=`cd $(srcdir); git show -s --format=%ci HEAD | sed -r "s/(\S*).*/\1/"`; \
	sed -i "s/XL4-DOCUMENT-DATE/$${DOCDATE}/" $@
	sed -i "s/XL4-DOCUMENT-NAME/$(PACKAGENAME)/" $@
	sed -i "s/XL4-DOCUMENT-VERSION/$(VERSION)/" $@
	sed -i "s:EXCELFORE_LOGO_PNG:$(abs_srcdir)/doc/excelfore_logo.png:" $@
	sed -i "s:EXCELFORE_LOGO_L_PNG:$(abs_srcdir)/doc/excelfore_logo_L.png:" $@

$(TARGET_PDF): $(DOC_LATEC_REFMAN)
	$(INSTALL_DATA) $^ $@

$(DOC_LATEC_REFMAN): $(DOC_HTML_INDEX)
	make -C $(srcdir)/docoutput/latex

docoutput/html $(DOC_HTML_INDEX): $(srcdir)/xl4_header.tex $(pkginclude_HEADERS) README.md
	cd $(srcdir); UNIBASE_VERSION=$(VERSION)-`date +%Y%m%d` doxygen unibase_doxygen.conf

install-html-local: docoutput/html $(DOC_HTML_INDEX)
	install -d $(DESTDIR)$(docdir)
	cp -r $< $(DESTDIR)$(docdir)/
uninstall-html:
	rm -rf $(DESTDIR)$(docdir)/html
	if [ -d $(DESTDIR)$(docdir) -a -z "`ls -A $(DESTDIR)$(docdir)`" ]; then \
		rmdir $(DESTDIR)$(docdir); \
	fi

install-pdf-local: $(TARGET_PDF)
	install -d $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $^ $(DESTDIR)$(docdir)/
uninstall-pdf: $(TARGET_PDF)
	rm -rf $(DESTDIR)$(docdir)/*.pdf
	if [ -d $(DESTDIR)$(docdir) -a -z "`ls -A $(DESTDIR)$(docdir)`" ]; then \
		rmdir $(DESTDIR)$(docdir); \
	fi

DOC_FILES= doc/excelfore_logo_L.png doc/excelfore_logo.png \
doc/xl4_brochure_template.tex doc/xl4_header_template.tex \
doc/xl4_usersguide_template.org

install-data-local: debian.make ub_genconf.py $(DOC_FILES)
	install -d $(DESTDIR)$(pkgdatadir)
	$(INSTALL) $^ $(DESTDIR)$(pkgdatadir)/
uninstall-local:
	rm -f $(DESTDIR)$(pkgdatadir)/debian.make
	rm -f $(DESTDIR)$(pkgdatadir)/ub_genconf.py
	rm -f $(DESTDIR)$(pkgdatadir)/*.png
	rm -f $(DESTDIR)$(pkgdatadir)/*.tex
	if [ -d $(DESTDIR)$(pkgdatadir) -a -z "`ls -A $(DESTDIR)$(pkgdatadir)`" ]; then \
		rmdir $(DESTDIR)$(pkgdatadir); \
	fi

deb:
	cd $(srcdir); make -f debian.make

CPPCHECK_TST_TARGET = $(libx4unibase_la_SOURCES)
-include $(srcdir)/make_cppcheck.mk
