#!/usr/bin/env python

# general imports
from numpy import *
from random import uniform


# imp general
import IMP

# our project
from IMP.isd import Scale, JeffreysRestraint

# unit testing framework
import IMP.test


class Tests(IMP.test.TestCase):

    def setUp(self):
        IMP.test.TestCase.setUp(self)
        # IMP.base.set_log_level(IMP.MEMORY)
        IMP.base.set_log_level(0)
        self.m = IMP.kernel.Model()
        self.sigma = Scale.setup_particle(IMP.kernel.Particle(self.m), 1.0)
        self.DA = IMP.DerivativeAccumulator()
        self.J = IMP.isd.JeffreysRestraint(self.m, self.sigma)

    def testValueP(self):
        "Test if JeffreysRestraint probability is 1/scale"
        for i in xrange(100):
            no = uniform(0.1, 100)
            self.sigma.set_scale(no)
            self.assertAlmostEqual(self.J.get_probability(),
                                   1.0 / no, delta=0.001)

    def testValueE(self):
        "Test if JeffreysRestraint score is log(scale)"
        for i in xrange(100):
            no = uniform(0.1, 100)
            self.sigma.set_scale(no)
            self.assertAlmostEqual(self.J.unprotected_evaluate(self.DA),
                                   log(no), delta=0.001)

    def testDerivative(self):
        "Test the derivative of JeffreysRestraint"
        self.m.add_restraint(self.J)
        for i in xrange(100):
            no = uniform(0.1, 100)
            self.sigma.set_scale(no)
            self.m.evaluate(True)
            self.assertAlmostEqual(self.sigma.get_scale_derivative(),
                                   1.0 / no, delta=0.001)

    def test_get_inputs(self):
        "Test JeffreysRestraint::get_inputs()"
        self.assertEqual([x.get_name() for x in self.J.get_inputs()],
                         [self.sigma.get_name()])

    def testNonzeroE(self):
        "JeffreysRestraint raise ValueError on evaluate with zero scale"
        self.sigma.set_scale(0.0)
        self.assertRaises(
            IMP.base.ModelException,
            self.J.unprotected_evaluate,
            self.DA)

    def testNegativeE(self):
        "JeffreysRestraint raise ValueError on evaluate with negative scale"
        self.sigma.set_scale(-1.0)
        self.assertRaises(
            IMP.base.ModelException,
            self.J.unprotected_evaluate,
            self.DA)

    def testNonzeroP(self):
        "JeffreysRestraint raise ValueError on get_prob with zero scale"
        self.sigma.set_scale(0.0)
        self.assertRaises(IMP.base.ModelException, self.J.get_probability)

    def testNegativeP(self):
        "JeffreysRestraint raise ValueError on get_prob with negative scale"
        self.sigma.set_scale(-1.0)
        self.assertRaises(IMP.base.ModelException, self.J.get_probability)

    def testSanityEP(self):
        "Test if JeffreysRestraint score is -log(prob)"
        for i in xrange(100):
            no = uniform(0.1, 100)
            self.sigma.set_scale(no)
            self.assertAlmostEqual(self.J.unprotected_evaluate(self.DA),
                                   -log(self.J.get_probability()))

    def testSanityPE(self):
        "Test if JeffreysRestraint prob is exp(-score)"
        for i in xrange(100):
            no = uniform(0.1, 100)
            self.sigma.set_scale(no)
            self.assertAlmostEqual(self.J.get_probability(),
                                   exp(-self.J.unprotected_evaluate(self.DA)))


if __name__ == '__main__':
    IMP.test.main()
