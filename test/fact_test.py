import unittest
import os
import subprocess

def run(arg = None):
    cmd = os.environ.get("BIN")
    try:
       out = subprocess.check_output([cmd, arg] if arg else [cmd])   
       return 0, out.strip()
    except subprocess.CalledProcessError as ex:                                                                                                   
        return ex.returncode, ex.output.strip()
    
'''
  Integration tests that can be use for both implementations,
  C++ and C#
'''
class TestFact(unittest.TestCase):
    def test_should_fail_on_empty_arg(self):
        self.assertEqual(1, run()[0])

    def test_should_fail_on_string_arg(self):
        self.assertEqual(1, run("str")[0])

    def test_should_fail_on_wrong_format_arg(self):
        self.assertEqual(1, run("123l")[0])

    def test_return_nan_for_negative(self):
        ret, res = run("-1")
        self.assertEqual(0, ret)
        self.assertEqual("nan", res)

    def test_return_one_for_zero(self):
        self.assertEqual("1", run("0")[1])

    def test_calculate_fact_10(self):
        self.assertEqual("3628800", run("10")[1])

    def test_calculate_fact_100(self):
        self.assertEqual("9.3326215443944102e+157", run("100")[1])

    def test_return_inf_for_large_number(self):
        self.assertEqual("inf", run("5000")[1])

if __name__ == '__main__':
    unittest.main()
