CurDir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"
OSS_PALMPRINT_DATA_PREFIX=https://blog-images-1257621236.cos.ap-shanghai.myqcloud.com/edcc_palmprints
DATA_USING_IN_SAMPLE_ZIP_NAME=data_using_in_sample.zip

download_zip() {
    url=${OSS_PALMPRINT_DATA_PREFIX}/${DATA_USING_IN_SAMPLE_ZIP_NAME}
    dst=${CurDir}/$1
    echo "Downloading $1......"
    curl -o ${dst} ${url} >/dev/null
    echo "Download $1 success!"
}

extract_images() {
    zip_file_name=$1
    unzip ${zip_file_name}
    rm -rf ${zip_file_name}
}

SAVE_AS=data.zip
download_zip ${SAVE_AS}
extract_images ${SAVE_AS}
