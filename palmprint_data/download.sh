#! /bin/bash

CurDir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"
OSS_EDCC_PART_BASE_DIR=https://blog-images-1257621236.cos.ap-shanghai.myqcloud.com/edcc_data/for_test_and_example
OSS_EDCC_ALL_BASE_DIR=https://blog-images-1257621236.cos.ap-shanghai.myqcloud.com/edcc_data/all

do_download() {
    url=$1
    dst=$2
    echo "Downloading ${url}......"
    curl -o ${dst} ${url} 1>/dev/null 2>&1
    echo "Download ${url} success!"
}

download_type=$1

if [ "_${download_type}" = "_all" ]; then
  download_list=$(cat ${CurDir}/downloads.all.lst)
  url_base_dir=${OSS_EDCC_ALL_BASE_DIR}
else
  download_list=$(cat ${CurDir}/downloads.lst)
  url_base_dir=${OSS_EDCC_PART_BASE_DIR}
fi

for item in ${download_list[@]}; do
    if [ ! -f ${CurDir}/${item} ]; then
        do_download ${url_base_dir}/${item} ${CurDir}/${item}
    fi
done
