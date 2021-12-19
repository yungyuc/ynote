import unittest

import modpyb
import modbpy


class ItemTest(unittest.TestCase):

    def setUp(self):

        self.ipyb = modpyb.ItemPyb()
        self.ibpy = modbpy.ItemBpy()

    def test_pyb(self):

        # pyb
        self.ipyb.value = 10
        self.ipyb.take_pyb(modpyb.ItemPyb(11))
        self.assertIsInstance(self.ipyb.make_pyb(), modpyb.ItemPyb)
        self.assertEqual(11, self.ipyb.make_pyb().value)

        # bpy
        self.ipyb.value = 10
        self.ipyb.take_bpy(modbpy.ItemBpy(11))
        self.assertIsInstance(self.ipyb.make_bpy(), modbpy.ItemBpy)
        self.assertEqual(11, self.ipyb.value)

    def test_bpy(self):

        # pyb
        self.ibpy.value = 10
        self.ibpy.take_pyb(modpyb.ItemPyb(11))
        self.assertIsInstance(self.ibpy.make_pyb(), modpyb.ItemPyb)
        self.assertEqual(11, self.ibpy.make_pyb().value)

        # bpy
        self.ibpy.value = 10
        self.ibpy.take_bpy(modbpy.ItemBpy(11))
        self.assertIsInstance(self.ibpy.make_bpy(), modbpy.ItemBpy)
        self.assertEqual(11, self.ibpy.make_bpy().value)


class ContainerTest(unittest.TestCase):

    def test_container_pyb(self):

        pyb = modpyb.ItemPyb(100)
        bpy = modbpy.ItemBpy(101)
        pybss = modpyb.ItemPybSS(102)
        bpybs = modbpy.ItemBpyBS(103)
        cpyb = modpyb.ContainerPyb(pyb, bpy, pybss, bpybs)

        self.assertEqual(100, cpyb.pyb.value)
        self.assertEqual(101, cpyb.bpy.value)
        self.assertEqual(102, cpyb.pybss.value)
        self.assertEqual(103, cpyb.bpybs.value)

        self.assertNotEqual(pyb.address, cpyb.pyb.address)
        self.assertNotEqual(bpy.address, cpyb.bpy.address)
        self.assertEqual(pybss.address, cpyb.pybss.address)
        self.assertEqual(bpybs.address, cpyb.bpybs.address)

        cpyb.take_pyb(modpyb.ItemPyb(200))
        cpyb.take_bpy(modbpy.ItemBpy(201))

        pybss2 = modpyb.ItemPybSS(202)
        bpybs2 = modbpy.ItemBpyBS(203)
        cpyb.set_pybss(pybss2)
        cpyb.set_bpybs(bpybs2)

        self.assertEqual(200, cpyb.pyb.value)
        self.assertEqual(201, cpyb.bpy.value)
        self.assertEqual(202, cpyb.pybss.value)
        self.assertEqual(203, cpyb.bpybs.value)

        self.assertNotEqual(pyb.address, cpyb.pyb.address)
        self.assertNotEqual(bpy.address, cpyb.bpy.address)
        self.assertNotEqual(pybss.address, cpyb.pybss.address)
        self.assertNotEqual(bpybs.address, cpyb.bpybs.address)
        self.assertEqual(pybss2.address, cpyb.pybss.address)
        self.assertEqual(bpybs2.address, cpyb.bpybs.address)

        cpyb.set_pybss_ref(pybss)
        cpyb.set_bpybs_ref(bpybs)

        self.assertEqual(pybss.address, cpyb.pybss.address)
        self.assertEqual(bpybs.address, cpyb.bpybs.address)
        self.assertNotEqual(pybss2.address, cpyb.pybss.address)
        self.assertNotEqual(bpybs2.address, cpyb.bpybs.address)

        self.assertEqual("overload_pyb_noss", cpyb.overload_pyb(pyb))
        self.assertEqual("overload_pyb_ss", cpyb.overload_pyb(pybss))
        self.assertEqual("overload_bpy_nobs", cpyb.overload_bpy(bpy))
        self.assertEqual("overload_bpy_bs", cpyb.overload_bpy(bpybs))
        with self.assertRaises(TypeError):
            cpyb.overload_pyb(pyb=pyb)
        with self.assertRaises(TypeError):
            cpyb.overload_pyb(pybss=pybss)
        with self.assertRaises(TypeError):
            cpyb.overload_bpy(bpy=bpy)
        with self.assertRaises(TypeError):
            cpyb.overload_bpy(bpybs=bpybs)

        self.assertEqual("overload_pyb_noss", cpyb.overload2_pyb(pyb))
        self.assertEqual("overload_pyb_ss", cpyb.overload2_pyb(pybss))
        self.assertEqual("overload_bpy_nobs", cpyb.overload2_bpy(bpy))
        self.assertEqual("overload_bpy_bs", cpyb.overload2_bpy(bpybs))
        self.assertEqual("overload_pyb_noss", cpyb.overload2_pyb(pyb=pyb))
        self.assertEqual("overload_pyb_ss", cpyb.overload2_pyb(pybss=pybss))
        self.assertEqual("overload_bpy_nobs", cpyb.overload2_bpy(bpy=bpy))
        self.assertEqual("overload_bpy_bs", cpyb.overload2_bpy(bpybs=bpybs))

    def test_container_bpy(self):

        pyb = modpyb.ItemPyb(100)
        bpy = modbpy.ItemBpy(101)
        pybss = modpyb.ItemPybSS(102)
        bpybs = modbpy.ItemBpyBS(103)
        cbpy = modbpy.ContainerBpy(pyb, bpy, pybss, bpybs)

        self.assertEqual(100, cbpy.pyb.value)
        self.assertEqual(101, cbpy.bpy.value)
        self.assertEqual(102, cbpy.pybss.value)
        self.assertEqual(103, cbpy.bpybs.value)

        self.assertNotEqual(pyb.address, cbpy.pyb.address)
        self.assertNotEqual(bpy.address, cbpy.bpy.address)
        self.assertEqual(pybss.address, cbpy.pybss.address)
        self.assertEqual(bpybs.address, cbpy.bpybs.address)

        cbpy.take_pyb(modpyb.ItemPyb(200))
        cbpy.take_bpy(modbpy.ItemBpy(201))

        pybss2 = modpyb.ItemPybSS(202)
        bpybs2 = modbpy.ItemBpyBS(203)
        cbpy.set_pybss(pybss2)
        cbpy.set_bpybs(bpybs2)

        self.assertEqual(200, cbpy.pyb.value)
        self.assertEqual(201, cbpy.bpy.value)
        self.assertEqual(202, cbpy.pybss.value)
        self.assertEqual(203, cbpy.bpybs.value)

        self.assertNotEqual(pyb.address, cbpy.pyb.address)
        self.assertNotEqual(bpy.address, cbpy.bpy.address)
        self.assertNotEqual(pybss.address, cbpy.pybss.address)
        self.assertNotEqual(bpybs.address, cbpy.bpybs.address)
        self.assertEqual(pybss2.address, cbpy.pybss.address)
        self.assertEqual(bpybs2.address, cbpy.bpybs.address)

        cbpy.set_pybss_ref(pybss)
        cbpy.set_bpybs_ref(bpybs)

        self.assertEqual(pybss.address, cbpy.pybss.address)
        self.assertEqual(bpybs.address, cbpy.bpybs.address)
        self.assertNotEqual(pybss2.address, cbpy.pybss.address)
        self.assertNotEqual(bpybs2.address, cbpy.bpybs.address)

        self.assertEqual("overload_pyb_noss", cbpy.overload_pyb(pyb))
        self.assertEqual("overload_pyb_ss", cbpy.overload_pyb(pybss))
        self.assertEqual("overload_bpy_nobs", cbpy.overload_bpy(bpy))
        self.assertEqual("overload_bpy_bs", cbpy.overload_bpy(bpybs))
        with self.assertRaisesRegex(Exception, "Python argument types in"):
            cbpy.overload_pyb(pyb=pyb)
        with self.assertRaisesRegex(Exception, "Python argument types in"):
            cbpy.overload_pyb(pybss=pybss)
        with self.assertRaisesRegex(Exception, "Python argument types in"):
            cbpy.overload_bpy(bpy=bpy)
        with self.assertRaisesRegex(Exception, "Python argument types in"):
            cbpy.overload_bpy(bpybs=bpybs)

        self.assertEqual("overload_pyb_noss", cbpy.overload2_pyb(pyb))
        self.assertEqual("overload_pyb_ss", cbpy.overload2_pyb(pybss))
        self.assertEqual("overload_bpy_nobs", cbpy.overload2_bpy(bpy))
        self.assertEqual("overload_bpy_bs", cbpy.overload2_bpy(bpybs))
        self.assertEqual("overload_pyb_noss", cbpy.overload2_pyb(pyb=pyb))
        self.assertEqual("overload_pyb_ss", cbpy.overload2_pyb(pybss=pybss))
        self.assertEqual("overload_bpy_nobs", cbpy.overload2_bpy(bpy=bpy))
        self.assertEqual("overload_bpy_bs", cbpy.overload2_bpy(bpybs=bpybs))

# vim: set fenc=utf8 ff=unix et sw=4 ts=4 sts=4:
