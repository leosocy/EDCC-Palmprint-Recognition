##################
# edcc lib image #
##################

FROM registry.cn-hangzhou.aliyuncs.com/leosocy/opencv:base-3.4.2

RUN apk add --no-cache --virtual build-dependencies make cmake git \
    && cd /tmp \
    && git clone https://github.com/Leosocy/EDCC-Palmprint-Recognition.git \
    && cd EDCC-Palmprint-Recognition \
    && mkdir build && cd build \
    && cmake .. && make -j && make install \
    && rm -rf /tmp/* \
    && apk del build-dependencies