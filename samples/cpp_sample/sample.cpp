#include <stdio.h>
#include <stdlib.h>
#include <edcc.h>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Usage:\n\t./run_cpp_sample image_path_1 image_path_2\n");
        return -1;
    }
    const char* palmprint_image_path_1 = argv[1];
    const char* palmprint_image_path_2 = argv[2];
    const char* config_file_path = "../config.json";
    #define CODE_BUFFER_MAX_LEN 1024
    unsigned char code_buffer_1[CODE_BUFFER_MAX_LEN] = {0};
    unsigned char code_buffer_2[CODE_BUFFER_MAX_LEN] = {0};
    size_t buffer_len_1 = 0;
    size_t buffer_len_2 = 0;
    int ret = GetEDCCCoding(palmprint_image_path_1,
                            config_file_path,
                            CODE_BUFFER_MAX_LEN,
                            code_buffer_1,
                            &buffer_len_1);
    ret = GetEDCCCoding(palmprint_image_path_2,
                        config_file_path,
                        CODE_BUFFER_MAX_LEN,
                        code_buffer_2,
                        &buffer_len_2);
    double coding_matching_score = 0.0;
    double image_matching_score = 0.0;
    GetTwoPalmprintCodingMatchScore(code_buffer_1,
                                    code_buffer_2,
                                    &coding_matching_score);
    GetTwoPalmprintMatchScore(palmprint_image_path_1,
                              palmprint_image_path_2,
                              config_file_path,
                              &image_matching_score);
    if (coding_matching_score == image_matching_score)
    {
        printf("-----------------------------------------------------------\n");
        printf("PalmprintImage1:%s\nPalmprintImage2:%s\nMatchingScore:%lf\n", palmprint_image_path_1, palmprint_image_path_2, image_matching_score);
        printf("-----------------------------------------------------------\n");
    }
    return 0;
}
