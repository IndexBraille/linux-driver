AUTOMAKE_OPTIONS = foreign

cupsfilterdir = @FILTER_PATH@
dist_cupsfilter_SCRIPTS = filter/index-braille.sh

cupsmimedir=@MIME_PATH@
dist_cupsmime_DATA = mime/indexbraille.types

cupsdrvdir = @DRV_PATH@
dist_cupsdrv_DATA = \
	driver/indexbraille.drv \
	driver/indexbraille-media.defs \
	driver/indexbraille-filter.defs

PPD_FILES=$(shell awk '/PCFileName/{printf "ppd/Index-Braille/%s\n", $$2}' $(top_builddir)/driver/indexbraille.drv | xargs)
PPD_FILES_GZ=$(subst .ppd,.ppd.gz, $(PPD_FILES))

cupsppddir = @PPD_PATH@
dist_cupsppd_DATA = \
    $(PPD_FILES_GZ)

%.ppd.gz: %.ppd
	test ! -z "$(PPD_FILTER)" && sed -i .orig -e "$(PPD_FILTER)" $< || true
	gzip -c $< > $@

$(top_builddir)/ppd/Index-Braille:
	mkdir -p $@

$(PPD_FILES): $(top_builddir)/driver/indexbraille.drv $(top_srcdir)/driver/indexbraille-filter.defs $(top_srcdir)/driver/indexbraille-media.defs $(top_builddir)/ppd/Index-Braille
	$(PPDC) -d ppd/Index-Braille/ -I $(top_srcdir)/driver $<

cupsfilter_PROGRAMS=index-direct-braille

index_direct_braille_SOURCES=filter/index-direct-braille.c

clean-local:
	rm -rf ppd
	rm -rf dist-deb
	rm -rf dist-osx

dist-osx:
	mkdir -p dist-osx
	pkgbuild \
		--identifier com.indexbraille.printer-driver.Library \
		--version @VERSION@ \
		--install-location /Library \
		--root staged-dir/Library \
		dist-osx/@PACKAGE@-library-@VERSION@.pkg
	productbuild \
		--distribution osx-installer/printer-driver-indexbraille.xml \
		--resource osx-installer \
		--package-path dist-osx \
		dist-osx/@PACKAGE@-@VERSION@.pkg

pkg:
	make clean
	make CFLAGS="-arch i386 -arch x86_64" LDFLAGS="-arch i386 -arch x86_64"

	make DESTDIR=staged-dir install
	make dist-osx
	rm -rf staged-dir

deb: mkdeb.sh
	./mkdeb.sh

pkg-uninstall:
	(cd /Library && pkgutil --only-files --files com.indexbraille.printer-driver.Library | sudo xargs rm)
	sudo pkgutil --forget com.indexbraille.printer-driver.Library

print-%  : ; @echo $* = $($*)