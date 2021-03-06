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

cupsppddir = @PPD_PATH@
dist_cupsppd_DATA = \
	ppd/Index-Braille/$(PPD_PREFIX)4WavPRO.ppd.gz \
	ppd/Index-Braille/$(PPD_PREFIX)4x4PRO.ppd.gz \
	ppd/Index-Braille/$(PPD_PREFIX)BasicD.ppd.gz \
	ppd/Index-Braille/$(PPD_PREFIX)BasicS.ppd.gz \
	ppd/Index-Braille/$(PPD_PREFIX)Everest.ppd.gz

%.ppd.gz: %.ppd
	test ! -z "$(PPD_FILTER)" && sed -i .orig -e "$(PPD_FILTER)" $< || true
	gzip -c $< > $@

ppd/Index-Braille:
	mkdir -p $@

$(PPD_PREFIX)%.ppd.gz: %.ppd.gz
	mv $< $@

ppd/Index-Braille/4WavPRO.ppd \
ppd/Index-Braille/4x4PRO.ppd \
ppd/Index-Braille/BasicD.ppd \
ppd/Index-Braille/BasicS.ppd \
ppd/Index-Braille/Everest.ppd: driver/indexbraille.drv driver/indexbraille-filter.defs driver/indexbraille-media.defs ppd/Index-Braille
	$(PPDC) -d ppd/Index-Braille/ $<

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
