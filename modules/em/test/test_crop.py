import unittest
import os
import IMP
import IMP.em
import IMP.test
import IMP.core

class CropTest(IMP.test.TestCase):
    """Class to test EM correlation restraint"""

    def load_density_maps(self):
        mrw = IMP.em.MRCReaderWriter()
        self.scene= IMP.em.read_map(self.get_input_file_name("1z5s_20.imp.mrc"), mrw)
        self.scene.get_header_writable().set_resolution(20.)
        self.scene.update_voxel_size(3.)
        self.scene.calcRMS()


    def setUp(self):
        """Build test model and optimizer"""
        IMP.test.TestCase.setUp(self)
        self.imp_model = IMP.Model()
        self.load_density_maps()

    def test_crop_map(self):
        """Test the cropping functionality"""
        t=0.
        cropped_scene=self.scene.get_cropped(0.)
        cropped_bb=IMP.em.get_bounding_box(cropped_scene)
        scene_bb=IMP.em.get_bounding_box(self.scene,t)
        #check that the scene and cropped scene have the same bounding box
        for i in range(2):
            self.assertAlmostEqual(IMP.algebra.get_distance(
                scene_bb.get_corner(i),
                cropped_bb.get_corner(i)),0,2)
        IMP.em.write_map(cropped_scene,"test2.mrc",IMP.em.MRCReaderWriter())
        dmap3=IMP.em.read_map("test2.mrc",IMP.em.MRCReaderWriter())
        coarse_cc=IMP.em.CoarseCC()
        IMP.set_log_level(IMP.VERBOSE)
        #check that the center stays in the same place
        print self.scene.get_centroid()
        print cropped_scene.get_centroid()
        IMP.em.write_map(cropped_scene,"temp.mrc",IMP.em.MRCReaderWriter())
        self.assertAlmostEqual(IMP.algebra.get_distance(
            self.scene.get_centroid(),
            cropped_scene.get_centroid()),0,2)
        #check that the spacing is correct
        self.assertAlmostEqual(dmap3.get_spacing(),3,2)
        #check that the dmin and dmax are the same
        self.assertAlmostEqual(cropped_scene.get_min_value()-self.scene.get_min_value(),0.,2)
        self.assertAlmostEqual(cropped_scene.get_max_value()-self.scene.get_max_value(),0.,2)
        os.unlink("test2.mrc")
if __name__ == '__main__':
    unittest.main()
