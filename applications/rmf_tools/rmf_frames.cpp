/**
 * Copyright 2007-2012 IMP Inventors. All rights reserved.
 */
#include <RMF/FileHandle.h>
#include <sstream>
#include "common.h"

std::string description("Print out information about frames");


int main(int argc, char **argv) {
  try {
    IMP_ADD_INPUT_FILE("rmf");
    process_options(argc, argv);


    RMF::FileConstHandle rh= RMF::open_rmf_file_read_only(input);
    for (unsigned int i=0; i< rh.get_number_of_frames(); ++i) {
      std::string cmt=rh.get_frame_comment(i);
      if (!cmt.empty()) {
        std::cout << i << ": " << cmt << std::endl;
      }
    }
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
