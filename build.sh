#!/bin/bash

function ftp_transport () {
ftp -n <<END
open ${ARMADILLO_ADRESS}
user ftp
cd pub
put main.arm
put facer.py
END
}

echo "Starting build"
make
if [ $? = 0 ]; then
    echo -e "\033[0;32mBuild Success\033[0;39m"
    echo "Starting transport"
    ftp_transport
else
    echo -e "\033[0;31mビルドに失敗しました。\033[0;39m"
fi

