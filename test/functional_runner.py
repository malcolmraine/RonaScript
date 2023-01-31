import subprocess
import sys

rn_executable = sys.argv[1]


class Test(object):
    def __init__(self, name, source_dir: str, args: list):
        self.stdout = ""
        self.stderr = ""
        self.source_dir = source_dir
        self.source_file = f"{source_dir}/source.rn"
        self.expected_output = f"{source_dir}/expected_output.txt"
        self.args = args
        self.timeout = 10
        self.returncode = -1
        self.name = name
        self.log_file = f"{self.source_dir}/test.results"
        open(self.log_file, "w").close()

    def log(self, *args):
        with open(self.log_file, "a+") as file:
            for arg in args:
                file.write(arg)
            file.write("\n")

    def check_output(self):
        with open(self.expected_output, "r") as file:
            expected = file.read().strip("\n").strip()
            actual = self.stdout.strip("\n").strip()
            if expected != actual:
                print(f"FAILED - {self.name}")
                self.log(f"FAILED - {self.name}")
            else:
                print(f"PASSED - {self.name}")
                self.log(f"PASSED - {self.name}")

            self.log("=========================================================")
            self.log("Expected")
            self.log("=========================================================")
            self.log(expected)
            self.log()
            self.log("=========================================================")
            self.log("Actual")
            self.log("=========================================================")
            self.log(actual)

    def run(self):
        s = subprocess.Popen([rn_executable, self.source_file, *self.args], stderr=subprocess.PIPE,
                             stdout=subprocess.PIPE)
        self.returncode = s.returncode
        self.stdout, self.stderr = s.communicate()
        self.stdout = self.stdout.decode("utf-8")
        self.check_output()


class TestRunner(object):
    def __init__(self):
        self.tests = []

    def add_test(self, test: Test):
        self.tests.append(test)

    def run(self):
        for test in self.tests:
            test.run()


if __name__ == "__main__":
    runner = TestRunner()
    runner.add_test(Test("Recursive GCF", "functional/recursive_gcf", []))
    runner.add_test(Test("Class Creation", "functional/class_creation", []))
    runner.run()
