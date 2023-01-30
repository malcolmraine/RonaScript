import subprocess
import sys
rn_executable = sys.argv[1]


class Test(object):
    def __init__(self, name, source_dir: str, args: list):
        self.stdout = ""
        self.stderr = ""
        self.source_file = f"{source_dir}/source.rn"
        self.expected_output = f"{source_dir}/expected_output.txt"
        self.args = []
        self.timeout = 10
        self.returncode = -1
        self.name = name

    def run(self):
        s = subprocess.Popen([rn_executable, self.source_file, *self.args], stderr=subprocess.PIPE,
                             stdout=subprocess.PIPE)
        self.returncode = s.returncode
        self.stdout, self.stderr = s.communicate()
        self.stdout = self.stdout.decode("utf-8")


class TestRunner(object):
    def __init__(self):
        self.tests = []

    def add_test(self, test: Test):
        self.tests.append(test)

    def run(self):
        for test in self.tests:
            test.run()
            with open(test.expected_output, "r") as file:
                if file.read().strip("\n").strip() != test.stdout.strip("\n").strip():
                    print(f"FAILED - {test.name}")
                else:
                    print(f"PASSED - {test.name}")


runner = TestRunner()
runner.add_test(Test("Recursive GCF", "functional/recursive_gcf", []))
runner.add_test(Test("Class Creation", "functional/class_creation", []))
runner.run()
