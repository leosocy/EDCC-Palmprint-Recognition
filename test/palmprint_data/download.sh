#! /bin/bash

CurDir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"
DOWNLOADS_LIST=$(cat ${CurDir}/downloads.lst)
OSS_PALM_IMG_PREFIX=https://blog-images-1257621236.cos.ap-shanghai.myqcloud.com/edcc_palmprints

download_img() {
    url=${OSS_PALM_IMG_PREFIX}/$1
    dst=${CurDir}/$1
    echo "Downloading $1......"
    curl -o ${dst} ${url} &>/dev/null
    echo "Download $1 success!"
}

for img in ${DOWNLOADS_LIST[@]}; do
    if [ ! -f ${CurDir}/${img} ]; then
        download_img ${img}
    fi
done
