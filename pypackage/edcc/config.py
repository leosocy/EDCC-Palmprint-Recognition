# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.


class EncoderConfig(object):
    """Config which is used to initialize `Encoder`"""

    def __init__(
        self, image_size, gabor_kernel_size, laplace_kernel_size, gabor_directions
    ):
        self.image_size = image_size
        self.gabor_kernel_size = gabor_kernel_size
        self.laplace_kernel_size = laplace_kernel_size
        self.gabor_directions = gabor_directions


default_encoder_config = EncoderConfig(29, 5, 5, 10)
