#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <edcc.h>

void PrintUsage()
{
    printf("Usage:\t./run_cpp_sample [-c config path] [-i image_1 image_2]\n");
    printf("e.g.\t./run_cpp_sample -c ./config.json -i ./Palmprint1.jpg ./Palmprint2.jpg\n");
}

bool CheckOptions(int argc, char** argv)
{
    if (argc < 6)
    {
        PrintUsage();
        return false;
    }
    return true;
}

int main(int argc, char** argv)
{
    if (!CheckOptions(argc, argv))
    {
        return 1;
    }
    const char* palmprint_image_path_1 = NULL;
    const char* palmprint_image_path_2 = NULL;
    const char* config_file_path = NULL;
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            config_file_path = argv[++i];
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            palmprint_image_path_1 = argv[++i];
            palmprint_image_path_2 = argv[++i];
        }
        ++i;
    }
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
        printf("\n-----------------------------------------------------------\n");
        printf("PalmprintImage1:%s\nPalmprintImage2:%s\nMatchingScore:%lf\n", palmprint_image_path_1, palmprint_image_path_2, image_matching_score);
        printf("-----------------------------------------------------------\n\n");
    }
    return 0;
}

