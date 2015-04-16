/**
 *  \file IMP/rmf/Category.h
 *  \brief Handle read/write of Model data from/to files.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */
#include <RMF/exceptions.h>
#include <IMP/exception.h>
#include <RMF/utility.h>

int main(int, char * []) {
  try {
    RMF::test_throw_exception();
  }
  catch (RMF::Exception& e) {
    std::cout << RMF::get_message(e) << std::endl;
    return 0;
  }
  return 1;
}
