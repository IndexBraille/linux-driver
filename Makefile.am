cupsfilterdir = $(libdir)/cups/filter
dist_cupsfilter_SCRIPTS = filter/indexbraille.sh

cupsmimedir=$(datadir)/cups/mime
dist_cupsmime_DATA = mime/indexbraille.types

cupsdrvdir = $(datadir)/cups/drv
dist_cupsdrv_DATA = \
	driver/indexbraille.drv \
	driver/indexbraille-media.defs \
	driver/indexbraille-filter.defs

cupsppddir = $(datadir)/cups/ppd/Index-Braille
dist_cupsppd_DATA = \
	ppd/Index-Braille/4WavPRO.ppd \
	ppd/Index-Braille/4x4PRO.ppd \
	ppd/Index-Braille/BasicD.ppd \
	ppd/Index-Braille/BasicS.ppd \
	ppd/Index-Braille/Everest.ppd

ppd/Index-Braille:
	mkdir -p $@

ppd/Index-Braille/4WavPRO.ppd ppd/Index-Braille/4x4PRO.ppd ppd/Index-Braille/BasicD.ppd ppd/Index-Braille/BasicS.ppd ppd/Index-Braille/Everest.ppd: driver/indexbraille.drv driver/indexbraille-filter.defs driver/indexbraille-media.defs ppd/Index-Braille
	$(PPDC) -d ppd/Index-Braille/ $<

cupsfilter_PROGRAMS=index-direct-braille

index_direct_braille_SOURCES=filter/index-direct-braille.c
