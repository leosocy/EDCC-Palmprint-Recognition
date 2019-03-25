#! /bin/bash

CurDir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"

######## envs ########

DOCKER_REGISTRY=registry.cn-hangzhou.aliyuncs.com/leosocy
OPENCV_CI_IMAGE=${DOCKER_REGISTRY}/opencv:ci
CPPCHECK_CI_IMAGE=${DOCKER_REGISTRY}/cppcheck:1.83

CACHED_IMAGES=(${OPENCV_CI_IMAGE} ${CPPCHECK_CI_IMAGE})

APP_TEST_NAME=test-edcc

######## functions ########

check_exec_success() {
    if [ "$1" != "0" ]; then
        echo "[ERROR] $2 failed!"
        exit 1
    else
        echo "[INFO] $2 success!"
    fi
}


test() {
    load_images ${OPENCV_CI_IMAGE}
    image_exist ${OPENCV_CI_IMAGE}
    if [ $? -ne 0 ]; then
        docker pull ${OPENCV_CI_IMAGE} > /dev/null
    fi
    check_exec_success "$?" "pulling ${OPENCV_CI_IMAGE} image"
    docker run -it --rm -v ${CurDir}:/app -w /app ${OPENCV_CI_IMAGE} /bin/sh -ec """
        mkdir -p build_test; cd build_test;
        cmake .. -DEDCC_BUILD_TESTS=ON; make -j2 build_and_test;
        lcov -b . -d . -c -o cov.info > /dev/null;
        lcov -r cov.info \"/usr/*\" \"*/thirdparty/*\" \"*/test/*\" \"*/build_test/*\" \"/**/b64.*\" -o cov.info -q;
        lcov -l cov.info;
        genhtml -o cov_result cov.info > /dev/null; rm -rf ../cov_result; mv cov_result ..;
        echo ""
        echo ""
        echo \"==========Generated code coverage report under ./cov_result directory.==========\"
    """
    check_exec_success "$?" "run test"
}


gdbtest() {
    load_images ${OPENCV_CI_IMAGE}
    image_exist ${OPENCV_CI_IMAGE}
    if [ $? -ne 0 ]; then
        docker pull ${OPENCV_CI_IMAGE} > /dev/null
    fi
    check_exec_success "$?" "pulling ${OPENCV_CI_IMAGE} image"
    docker run -it --rm -v ${CurDir}:/app -w /app \
    --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
    ${OPENCV_CI_IMAGE} /bin/sh -ec """
        mkdir -p build_test; cd build_test;
        cmake .. -DEDCC_BUILD_TESTS=ON; make -j2; gdb ${APP_TEST_NAME}
    """
    check_exec_success "$?" "gdb test"
}


lint() {
    load_images ${CPPCHECK_CI_IMAGE}
    if [ $? -ne 0 ]; then
        docker pull ${CPPCHECK_CI_IMAGE} > /dev/null
    fi
    check_exec_success "$?" "pulling ${CPPCHECK_CI_IMAGE} image"
    docker run -it --rm -v ${CurDir}:/app -w /app ${CPPCHECK_CI_IMAGE} /bin/sh -ec """
	    cppcheck --enable=warning,performance --error-exitcode=1 include src
    """
    check_exec_success "$?" "run lint"
}


test_and_lint() {
    test
    lint
}


runenv() {
    load_images ${OPENCV_CI_IMAGE}
    image_exist ${OPENCV_CI_IMAGE}
    if [ $? -ne 0 ]; then
        docker pull ${OPENCV_CI_IMAGE} > /dev/null
    fi
    check_exec_success "$?" "pulling ${OPENCV_CI_IMAGE} image"
    docker run -it --rm -v ${CurDir}:/app -w /app ${OPENCV_CI_IMAGE} /bin/sh -ec """
    mkdir -p build_install; cd build_install;
    cmake ..; make install;
    bash
    """
}

######## below functions are used for travis-ci ########

upload_codecov() {
    if [ -z ${CODECOV_TOKEN} ]; then
        echo "Please set CODECOV_TOKEN value"
        exit 1
    fi
    docker run -it --rm -v ${CurDir}:/app -w /app/build_test \
    -e CODECOV_TOKEN=${CODECOV_TOKEN} \
    ${OPENCV_CI_IMAGE} /bin/bash -ec "$(curl -s https://codecov.io/bash)"
    check_exec_success "$?" "upload codecov"
}

image_exist() {
    docker images -a --filter='dangling=false' --format '{{.Repository}}:{{.Tag}} {{.ID}}' | grep $1 &> /dev/null
    return $?
}

save_images() {
    for image in "${CACHED_IMAGES[@]}"; do
        image_exist ${image}
        if [ $? -eq 0 ]; then
            mkdir -p ${HOME}/docker && docker images -a \
            --filter='dangling=false' --format '{{.Repository}}:{{.Tag}} {{.ID}}' | grep ${image} \
            | xargs -n 2 -t sh -c 'test -e $HOME/docker/$1.tar.gz || docker save $0 | gzip -2 > ${HOME}/docker/$1.tar.gz'
        fi
    done
}

load_images() {
    image_exist $1
    if [ $? -eq 0 ]; then
        return 0
    fi
    if [[ -d ${HOME}/docker ]]; then
        ls ${HOME}/docker/*.tar.gz | xargs -I {file} sh -c "zcat {file} | docker load";
    fi
}

######## script start ########

case "$1" in
    test) test ;;
    gdbtest) gdbtest ;;
    lint) lint ;;
    test_and_lint) test_and_lint ;;
    env) runenv ;;
    upload_codecov) upload_codecov ;;
    save_images) save_images ;;
    load_images) load_images $2 ;;
    *)
        echo "Usage:"
        echo "./manage.sh test | gdbtest"
        echo "./manage.sh lint"
        echo "./manage.sh test_and_lint"
        echo "./manage.sh env"
        exit 1
        ;;
esac

exit 0
