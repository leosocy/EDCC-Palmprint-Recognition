// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <edcc/facade.h>
#include <edcc/status.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

using edcc::EdccFacade;
using edcc::Status;

#define EXAMPLES_ROOT "../../"
#define PALMPRINT_DATA_DIR EXAMPLES_ROOT "palmprint_data/"
#define POLYU_G_DIR PALMPRINT_DATA_DIR "PolyU/Multispectral_G/"

class PalmprintImage {
 public:
  PalmprintImage(const std::string& person_name, const std::string& filepath)
      : person_name_(person_name), filepath_(filepath) {}
  const char* person_name() { return person_name_.c_str(); }
  const char* filepath() { return filepath_.c_str(); }

 private:
  std::string person_name_;
  std::string filepath_;
};

class PalmID {
 public:
  PalmID(const std::string& name, EdccFacade::eid encoder_id) {
    name_ = name;
    encoder_id_ = encoder_id;
  }
  ~PalmID() {
    for (auto it = codes_.begin(); it != codes_.end(); ++it) {
      delete[] it->second;
    }
  }
  void RegisterPalmprint(std::shared_ptr<PalmprintImage> image) {
    Status s;
    auto inst = EdccFacade::Instance();
    size_t buffer_size = inst->GetSizeOfCodeBufferRequired(encoder_id_);
    char* code_buffer = new char[buffer_size];
    inst->EncodePalmprint(encoder_id_, image->filepath(), code_buffer, buffer_size, &s);
    codes_.emplace(image, code_buffer);
  }
  std::shared_ptr<PalmprintImage> FindMostSimilar(const char* code_buffer, double* max_score) {
    Status s;
    auto inst = EdccFacade::Instance();
    std::shared_ptr<PalmprintImage> max_image;
    for (auto it = codes_.begin(); it != codes_.end(); ++it) {
      double score = inst->CalcCodeSimilarity(code_buffer, it->second, &s);
      if (score > *max_score) {
        *max_score = score;
        max_image = it->first;
      }
    }
    return max_image;
  }

 private:
  std::string name_;
  EdccFacade::eid encoder_id_;
  std::map<std::shared_ptr<PalmprintImage>, char*> codes_;
};

std::vector<std::shared_ptr<PalmprintImage>> training_images, testing_images;
std::map<std::string, std::shared_ptr<PalmID>> traning_palm_ids;

void init_polyu_training_and_testing_data(EdccFacade::eid encoder_id, const char* base_dir,
                                          int training_images_person) {
  for (int person = 0; person < 450; ++person) {
    int traning_added_cnt = 0;
    for (int num = 0; num < 6; ++num) {
      for (int session = 0; session < 2; ++session) {
        char person_name[8];
        char filepath[128];
        snprintf(person_name, 7, "%03d", person + 1);
        snprintf(filepath, 127, "%s%03d/%d_%02d_s.bmp", base_dir, person + 1, session + 1, num + 1);
        auto image = std::shared_ptr<PalmprintImage>(new PalmprintImage(person_name, filepath));
        if (traning_added_cnt < training_images_person) {
          training_images.emplace_back(image);
          if (traning_palm_ids.find(person_name) == traning_palm_ids.end()) {
            auto palm_id = std::shared_ptr<PalmID>(new PalmID(person_name, encoder_id));
            traning_palm_ids.emplace(person_name, palm_id);
          }
          ++traning_added_cnt;
        } else {
          testing_images.emplace_back(image);
        }
      }
    }
  }
}

int main() {
  Status s;
  auto eid = EdccFacade::Instance()->NewEncoderWithConfig(29, 5, 5, 10, &s);
  init_polyu_training_and_testing_data(eid, POLYU_G_DIR, 8);
  // training
  for (auto image : training_images) {
    auto palm_id = traning_palm_ids.at(image->person_name());
    palm_id->RegisterPalmprint(image);
  }
  // testing
  int correct_cnt = 0;
  for (auto test_image : testing_images) {
    size_t buffer_size = EdccFacade::Instance()->GetSizeOfCodeBufferRequired(eid);
    char* code_buffer = new char[buffer_size];
    EdccFacade::Instance()->EncodePalmprint(eid, test_image->filepath(), code_buffer, buffer_size, &s);
    double max_score = .0;
    std::shared_ptr<PalmprintImage> max_image;
    for (auto palm_id_it = traning_palm_ids.begin(); palm_id_it != traning_palm_ids.end(); ++palm_id_it) {
      double score = .0;
      auto image = palm_id_it->second->FindMostSimilar(code_buffer, &score);
      if (score > max_score) {
        max_score = score;
        max_image = image;
      }
    }
    bool correct = std::string(max_image->person_name()) == std::string(test_image->person_name());
    if (correct) {
      correct_cnt++;
    }
    printf("%s\tTesting Image:%s\tTraing Image:%s\tScore:%lf\n", correct ? "✅" : "❌", test_image->filepath(),
           max_image->filepath(), max_score);
    delete[] code_buffer;
  }
  printf("Accurate:%lf\n", double(correct_cnt) / testing_images.size());
  return 0;
}
