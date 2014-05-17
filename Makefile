PKGNAME=mstrlg

LIBRARIES=\
	config \
	logging \
	jsonparse \
	$(END_OF_LIST)

BINARIES=\
	masterlog \
	$(END_OF_LIST)

SYSTEM_LIBRARIES=

include makedefs/RULES
