#!/bin/sh

XOASFILES="xrec-utilities.tar.gz"
GOASSITE="http://collaboration.cmc.ec.gc.ca/science/outgoing/goas/xrec-utilities.tar.gz"

if [ -x /usr/bin/wget ];
then
    /usr/bin/wget ${GOASSITE};
elif [ -x /usr/bin/curl ];
then
    /usr/bin/curl -O ${GOASSITE}
else
    echo "Error: cannot download tar file using wget or curl."
    echo "Please download database at: ${GOASSITE}" 
    exit 1
fi

if [ -f ${XOASFILES} ];
then
    tar -xzvf  ${XOASFILES}
else
    echo "Error. Cannot find database tar in this directory."
fi
