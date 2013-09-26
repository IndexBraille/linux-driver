cupsfilterdir = $(libdir)/cups/filter
dist_cupsfilter_SCRIPTS = filter/indexbraille.sh

cupsdrvdir = $(datadir)/cups/drv
dist_cupsdrv_DATA = \
	driver/indexbraille.drv \
	driver/indexbraille-media.defs \
	driver/indexbraille-filter.defs

cupsppddir = $(datadir)/cups/ppd
dist_cupsppd_DATA = \
	ppd/4WavPRO.ppd \
	ppd/4x4PRO.ppd \
	ppd/BasicD.ppd \
	ppd/BasicS.ppd \
	ppd/Everest.ppd

ppd/4WavPRO.ppd ppd/4x4PRO.ppd ppd/BasicD.ppd ppd/BasicS.ppd ppd/Everest.ppd: driver/indexbraille.drv
	$(PPDC) $<
