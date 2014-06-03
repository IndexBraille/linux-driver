cupsfilterdir = @FILTER_PATH@
dist_cupsfilter_SCRIPTS = filter/indexbraille.sh

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

# Update this in order to "upgrade" older package.
OSX_PKG_VERSION=1
pkg:
	make DESTDIR=staged-dir install
	pkgbuild \
		--identifier com.Index-Braille.Index-direct-Braille \
		--version $(OSX_PKG_VERSION) \
		--install-location /Library \
		--root staged-dir \
		@PACKAGE@.pkg
	rm -rf staged-dir
