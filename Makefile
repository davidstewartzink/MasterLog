PKGNAME=mstrlg

LIBRARIES=\
	todo \
	config \
    network \
    protocol \
    listener \
    connector \
	identification \
	nameresolution \
	stringcentral \
	logging \
    storage \
	jsonparse \
	$(END_OF_LIST)

BINARIES=\
	masterlog \
	$(END_OF_LIST)

SYSTEM_LIBRARIES=

default-target: bin/masterlog
	bin/masterlog -f samples/etc/masterlog.conf -f samples/etc/masterlog.conf

include makedefs/RULES
