"""this is the main part of the assignment"""

# AUTHOR ? ?@bu.edu
# AUTHOR ? ??@bu.edu
# AUTHOR ??? ???@bu.edu
import unittest
import subprocess

#please change this to valid author emails
AUTHORS = ['?@bu.edu', '??@bu.edu', '???@bu.edu']

PROGRAM_TO_TEST = "test_program"

def runprogram(program, args, inputstr):
    coll_run = subprocess.run(
        [program, *args],
        input=inputstr.encode(),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)

    ret_code = coll_run.returncode
    program_output = coll_run.stdout.decode()
    program_errors = coll_run.stderr.decode()
    return (ret_code, program_output, program_errors)


class CollisionTestCase(unittest.TestCase):
    "empty class - write this"
    def test_programname(self):
        self.assertTrue(PROGRAM_TO_TEST.endswith('.py'),"wrong program name")

def main():
    "show how to use runprogram"

    print(runprogram('./test_program.py', ["4", "56", "test"], "my input"))
    unittest.main()

if __name__ == '__main__':
    main()

