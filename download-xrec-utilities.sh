#!/bin/bash

set -e

XREC_FILES="xrec-utilities.tar.gz"
XREC_FILES_URL="https://collaboration.cmc.ec.gc.ca/science/outgoing/goas/xrec-utilities.tar.gz"
XREC_FILES_MD5SUM="7b4e9b9bfa33652ed715cb44a141ba7c"

printUsage() {
    echo -e "Download xrec utilities needed to run xrec"
    echo -e "Usage:"
    echo -e "./$(basename $0) <XREC-GIT-DIR>\n"
    echo -e "Usually, XREC-GIT-DIR is the current directory, so use:"
    echo -e "./$(basename $0) ."
}

checkMd5() {
    # $1 File path
    # $2 Expected MD5
    # Return: 0 if matching; 1 otherwise
    md5=$(md5sum "$1" | cut -d' ' -f1)
    [[ "$md5" = "$2" ]]
}

if [[ ! -d "$1" ]]; then
    printUsage
    exit 1
fi

if [[ $# -eq 2 ]]; then
    if [[ ! -r "$2" ]]; then
        echo "Can't read ${2} !"
        exit 1
    else
        tarballPath="$2"
    fi
else
    tarballPath="${1}/${XREC_FILES}"
    if [[ -x $(which wget) ]]; then
        wget ${XREC_FILES_URL} -O "${tarballPath}";
    elif [[ -x $(which curl) ]]; then
        curl -o "${tarballPath}" ${XREC_FILES_URL}
    else
        echo "Error: cannot download using wget or curl."
        echo "Please download database at: ${XREC_FILES_URL}" 
        exit 1
    fi
fi

if checkMd5 "$tarballPath" "$XREC_FILES_MD5SUM"; then
    echo "MD5 check OK"
else
    echo "The MD5 of $XREC_FILES does not match what was expected.  The file might be corrupted."
    exit 1
fi
tar -xzvf ${tarballPath} -C "$1"
