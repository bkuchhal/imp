/**
 *  \file link_macros.h
 *  \brief macros for display classes
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPRMF_LINK_MACROS_H
#define IMPRMF_LINK_MACROS_H
#include "links.h"
#include "associations.h"
#include <IMP/base/Pointer.h>
#include <RMF/FileHandle.h>

#define IMP_DEFINE_INTERNAL_LINKERS(Name, name,args, cargs,          \
                                    create_args, create_cargs           \
                                    )                                   \
  namespace {                                                           \
  Name##SaveLink *get_##name##_save_link args {                         \
    int index=IMP::rmf::get_save_linker_index(#name);                   \
    if (!fh.get_has_associated_data(index)) {                           \
      IMP::rmf::SaveLinkAssociationType psl                             \
        = new Name##SaveLink create_args;                               \
      IMP::rmf::set_linker(fh, index, psl);                             \
    }                                                                   \
    IMP::rmf::SaveLinkAssociationType ln                                \
      = IMP::rmf::get_save_linker(fh, index);                           \
    return dynamic_cast<Name##SaveLink*>(ln.get());                     \
  }                                                                     \
  Name##LoadLink *get_##name##_load_link cargs {                        \
    int index=IMP::rmf::get_load_linker_index(#name);                   \
    if (!fh.get_has_associated_data(index)) {                           \
      IMP::rmf::LoadLinkAssociationType psl                             \
        = new Name##LoadLink create_cargs;                              \
      IMP::rmf::set_linker(fh, index, psl);                             \
    }                                                                   \
    IMP::rmf::LoadLinkAssociationType pt                                \
      = IMP::rmf::get_load_linker(fh, index);                           \
    return dynamic_cast<Name##LoadLink*>(pt.get());                     \
  }                                                                     \
  }                                                                     \


#define IMP_DECLARE_LINKERS(Name, name, names, InType, InTypes,         \
                            OutType, OutTypes,args, cargs)              \
  IMPRMFEXPORT void add_##names(RMF::FileHandle fh,                     \
                                const OutTypes& hs);                    \
  IMPRMFEXPORT void add_##name(RMF::FileHandle fh, OutType hs);         \
  IMPRMFEXPORT InTypes create_##names cargs;                            \
  IMPRMFEXPORT void link_##names(RMF::FileConstHandle fh,               \
                                 const InTypes &hs)


#define IMP_DEFINE_LINKERS(Name, name, names, InType, InTypes,          \
                           OutType, OutTypes,args, cargs,               \
                           create_args, create_cargs, create_cargs_from) \
  IMP_DEFINE_INTERNAL_LINKERS(Name, name, args, cargs,                  \
                              create_args, create_cargs);               \
  void add_##names(RMF::FileHandle fh,                                  \
                   const OutTypes& hs) {                                \
    if (hs.empty()) return;                                             \
    Name##SaveLink* hsl= get_##name##_save_link(fh);                    \
    hsl->add(fh.get_root_node(), hs);                                   \
    hsl->save(fh, 0);                                                   \
  }                                                                     \
  void add_##name(RMF::FileHandle fh, OutType hs) {                     \
    add_##names(fh, OutTypes(1, hs));                                   \
  }                                                                     \
  InTypes create_##names cargs {                                        \
    Name##LoadLink* rsl= get_##name##_load_link create_cargs;           \
    InTypes ret= rsl->create(fh.get_root_node());                       \
    rsl->load(fh, 0);                                                   \
    return ret;                                                         \
  }                                                                     \
  void link_##names(RMF::FileConstHandle fh,                            \
                        const InTypes &hs) {                            \
    if(hs.empty()) return;                                              \
    base::Pointer<Name##LoadLink> pll                                   \
        = get_##name##_load_link create_cargs_from;                     \
    pll->link(fh.get_root_node(), hs);                                  \
  }

#endif /* IMPRMF_LINK_MACROS_H */
