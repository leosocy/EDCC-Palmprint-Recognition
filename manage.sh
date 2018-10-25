#! /bin/bash

CurDir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"

######## envs ########

DOCKER_REGISTRY=registry.cn-hangzhou.aliyuncs.com/leosocy
IMAGE=${DOCKER_REGISTRY}/edcc
BASE_IMAGE_TAG=${IMAGE}:base-${TRAVIS_COMMIT:0:8}
PYTHON_IMAGE_TAG=${IMAGE}:python-${TRAVIS_COMMIT:0:8}
OPENCV_CI_IMAGE=${DOCKER_REGISTRY}/opencv:ci
CPPCHECK_CI_IMAGE=${DOCKER_REGISTRY}/cppcheck:1.83

CACHED_IMAGES=(${OPENCV_CI_IMAGE} ${CPPCHECK_CI_IMAGE})


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
    check_exec_success "$?" "pull ${OPENCV_CI_IMAGE}"
    docker run -it --rm -v $(pwd):/app -w /app ${OPENCV_CI_IMAGE} /bin/sh -ec """
    mkdir -p build; cd build;
    cmake ../test; make -j; ./test_edcc;
    lcov -b . -d edcc -c -o cov.info > /dev/null;
    lcov -r cov.info \"/usr/*\" \"*/thirdparty/*\" \"*/test/*\" -o cov.info -q;
    lcov -l cov.info;
    genhtml -o cov_result cov.info > /dev/null; rm -rf ../cov_result; mv -f cov_result ..
    echo ""
    echo ""
    echo \"==========Generated code coverage report under ./cov_result directory.==========\"
    """
    check_exec_success "$?" "run test"
}


lint() {
    load_images ${CPPCHECK_CI_IMAGE}
    if [ $? -ne 0 ]; then
        docker pull ${CPPCHECK_CI_IMAGE} > /dev/null
    fi
    check_exec_success "$?" "pull ${CPPCHECK_CI_IMAGE}"
    docker run -it --rm -v $(pwd):/app -w /app ${CPPCHECK_CI_IMAGE} /bin/sh -ec """
    cppcheck --enable=warning --error-exitcode=1 -I source/include source/src
    """
    check_exec_success "$?" "run lint"
}


test_and_lint() {
    test
    lint
}


run_py_sample() {
    docker pull ${OPENCV_CI_IMAGE} > /dev/null
    check_exec_success "$?" "pull ${OPENCV_CI_IMAGE}"
    docker run -it --rm -v $(pwd):/app -w /app ${OPENCV_CI_IMAGE} /bin/sh -ec """
    mkdir -p build_sample; cd build_sample;
    cmake ..; make -j; make install;
    make -j run_py_sample
    """
    check_exec_success "$?" "run py sample"
}

enter_env() {
    docker pull ${OPENCV_CI_IMAGE} > /dev/null
    check_exec_success "$?" "pull ${OPENCV_CI_IMAGE}"
    docker run -it --rm -v $(pwd):/app -w /app ${OPENCV_CI_IMAGE} /bin/sh -ec """
    mkdir -p build_install; cd build_install;
    cmake ..; make -j; make install;
    cd ..; rm -rf build_install
    sh
    """
}


######## below functions are used for travis-ci ########

upload_codecov() {
    if [ -z ${CODECOV_TOKEN} ]; then
        echo "Please set CODECOV_TOKEN value"
        exit 1
    fi
    docker run -d --rm -v $(pwd):/app -w /app/build -e CODECOV_TOKEN=${CODECOV_TOKEN} ${OPENCV_CI_IMAGE} /bin/bash -ec "$(curl -s https://codecov.io/bash)"
    check_exec_success "$?" "upload codecov"
}

build_images() {
    docker build -t ${BASE_IMAGE_TAG} -f Dockerfile.base .
    check_exec_success "$?" "build ${BASE_IMAGE_TAG}"
    docker build -t ${PYTHON_IMAGE_TAG} -f Dockerfile.python .
    check_exec_success "$?" "build ${PYTHON_IMAGE_TAG}"
}

upload_images() {
    if [ -z ${ALIYUN_DOCKER_REGISTRY_USERNAME} ]; then
        echo "Please set ALIYUN_DOCKER_REGISTRY_USERNAME value"
        exit 1
    fi
    if [ -z ${ALIYUN_DOCKER_REGISTRY_PASSWD} ]; then
        echo "Please set ALIYUN_DOCKER_REGISTRY_PASSWD value"
        exit 1
    fi
    echo "${ALIYUN_DOCKER_REGISTRY_PASSWD}" | docker login -u "${ALIYUN_DOCKER_REGISTRY_USERNAME}" --password-stdin ${DOCKER_REGISTRY}
    check_exec_success "$?" "docker login"
    docker push ${IMAGE}
    check_exec_success "$?" "push images"
}

image_exist() {
    docker images -a --filter='dangling=false' --format '{{.Repository}}:{{.Tag}} {{.ID}}' | grep $1 &> /dev/null
    return $?
}

save_images() {
    for image in "${CACHED_IMAGES[@]}"; do
        image_exist ${image}
        if [ $? -eq 0 ]; then
            mkdir -p ${HOME}/docker && docker images -a --filter='dangling=false' --format '{{.Repository}}:{{.Tag}} {{.ID}}' | grep ${image} | xargs -n 2 -t sh -c 'test -e $HOME/docker/$1.tar.gz || docker save $0 | gzip -2 > ${HOME}/docker/$1.tar.gz'
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
    lint) lint ;;
    test_and_lint) test_and_lint ;;
    run_py_sample) run_py_sample ;;
    upload_codecov) upload_codecov ;;
    env) enter_env ;;
    build_images) build_images ;;
    upload_images) upload_images ;;
    save_images) save_images ;;
    load_images) load_images $2 ;;
    *)
        echo "Usage:"
        echo "./manage.sh test"
        echo "./manage.sh lint"
        echo "./manage.sh test_and_lint"
        echo "./manage.sh run_py_sample"
        echo "./manage.sh env"
        exit 1
        ;;
esac

exit 0

