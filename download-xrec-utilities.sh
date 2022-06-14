#!/bin/sh

set -e

XOAS_FILES="xrec-utilities.tar.gz"
XOAS_FILES_URL="https://collaboration.cmc.ec.gc.ca/science/outgoing/goas/xrec-utilities.tar.gz"
XOAS_FILES_MD5SUM="7dfcc58e18f423ecb203a9995ece5b16"

printUsage() {
    echo -e "Download xrec utilities needed to run xoas"
    echo -e "Usage:"
    echo -e "./$(basename $0) <XOAS-GIT-DIR>\n"
    echo -e "Usually, XOAS-GIT-DIR is the current directory, so use:"
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
    tarballPath="${1}/${XOAS_FILES}"
    if [[ -x $(which wget) ]]; then
        wget ${XOAS_FILES_URL} -O "${tarballPath}";
    elif [[ -x $(which curl) ]]; then
        curl -o "${tarballPath}" ${XOAS_FILES_URL}
    else
        echo "Error: cannot download using wget or curl."
        echo "Please download database at: ${XOAS_FILES_URL}" 
        exit 1
    fi
fi

if checkMd5 "$tarballPath" "$XOAS_FILES_MD5SUM"; then
    echo "MD5 check OK"
else
    echo "The MD5 of $XOAS_FILES does not match what was expected.  The file might be corrupted."
    exit 1
fi
tar -xzvf ${tarballPath} -C "$1"
