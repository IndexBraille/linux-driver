cupsfilterdir = $(libdir)/cups/filter
dist_cupsfilter_SCRIPTS = filter/indexbraille.sh

EXTRA_DIST = \
	driver/indexbraille.drv \
	driver/media.defs \
	driver/filter.defs

cupsppddir = $(datadir)/cups/ppd
dist_cupsppd_DATA = \
	ppd/4WavPRO.ppd \
	ppd/4x4PRO.ppd \
	ppd/BasicD.ppd \
	ppd/BasicS.ppd \
	ppd/Everest.ppd

ppd/4WavPRO.ppd ppd/4x4PRO.ppd ppd/BasicD.ppd ppd/BasicS.ppd ppd/Everest.ppd: driver/indexbraille.drv
	$(PPDC) $<
