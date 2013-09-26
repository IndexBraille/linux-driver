cupsfilterdir = $(libdir)/cups/filter
dist_cupsfilter_SCRIPTS = filter/indexbraille.sh

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

ppd/Index-Braille/4WavPRO.ppd ppd/Index-Braille/4x4PRO.ppd ppd/Index-Braille/BasicD.ppd ppd/Index-Braille/BasicS.ppd ppd/Index-Braille/Everest.ppd: driver/indexbraille.drv
	$(PPDC) -d ppd/Index-Braille/ $<
