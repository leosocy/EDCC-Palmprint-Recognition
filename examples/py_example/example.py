# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

import os
import edcc

TEST_PALMPRINT_DATA_DIR = "../../palmprint_data"
TEST_A_01_PALMPRINT_IMAGE = os.path.join(TEST_PALMPRINT_DATA_DIR, "a_01.bmp")
TEST_A_02_PALMPRINT_IMAGE = os.path.join(TEST_PALMPRINT_DATA_DIR, "a_02.bmp")
TEST_B_01_PALMPRINT_IMAGE = os.path.join(TEST_PALMPRINT_DATA_DIR, "b_01.bmp")
TEST_B_02_PALMPRINT_IMAGE = os.path.join(TEST_PALMPRINT_DATA_DIR, "b_02.bmp")


if __name__ == "__main__":
    config = edcc.EncoderConfig(29, 5, 5, 10)
    encoder = edcc.create_encoder(config)
    one_palmprint_code = encoder.encode_using_file(TEST_A_01_PALMPRINT_IMAGE)
    another_palmprint_code = encoder.encode_using_file(TEST_B_01_PALMPRINT_IMAGE)
    similarity_score = one_palmprint_code.compare_to(another_palmprint_code)
    print(
        "{} <-> {} similarity score:{}".format(
            TEST_A_01_PALMPRINT_IMAGE, TEST_B_01_PALMPRINT_IMAGE, similarity_score
        )
    )
