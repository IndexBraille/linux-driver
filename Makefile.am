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
	ppd/Index-Braille/4WavPRO.ppd.gz \
	ppd/Index-Braille/4x4PRO.ppd.gz \
	ppd/Index-Braille/BasicD.ppd.gz \
	ppd/Index-Braille/BasicS.ppd.gz \
	ppd/Index-Braille/Everest.ppd.gz

%.ppd.gz: %.ppd
	gzip -c $< > $@

ppd/Index-Braille:
	mkdir -p $@

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
