import subprocess
import sys
import threading

rn_executable = sys.argv[1]


class Test(object):
    def __init__(self, name, source_dir: str, args: list):
        self.stdout = ""
        self.stderr = ""
        self.source_dir = source_dir
        self.source_file = f"{source_dir}/source.rn"
        self.expected_output = f"{source_dir}/expected_output.txt"
        self.args = args
        self.timeout = 5
        self.returncode = -1
        self.name = name
        self.log_file = f"{self.source_dir}/test.results"
        open(self.log_file, "w").close()
        self.process = None
        self.timeout_occurred = False

    def log(self, *args):
        with open(self.log_file, "a+") as file:
            for arg in args:
                file.write(arg)
            file.write("\n")

    def check_output(self, returncode=0):
        with open(self.expected_output, "r") as file:
            expected = file.read().strip("\n").strip()
            actual = self.stdout.strip("\n").strip()
            msg = ""
            if self.timeout_occurred:
                msg = "TIMEOUT"
            elif expected != actual:
                msg = "FAILED"
            else:
                msg = "PASSED"

            print(f"{msg} - {self.name}")
            self.log(f"{msg} - {self.name}")
            self.log(f"return code: {self.returncode}")
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
        def target():
            self.process = subprocess.Popen([rn_executable, self.source_file, *self.args], stderr=subprocess.PIPE,
                                            stdout=subprocess.PIPE)
            self.process.communicate()

        thread = threading.Thread(target=target)
        thread.start()

        thread.join(self.timeout)
        if thread.is_alive():
            self.process.terminate()
            self.timeout_occurred = True
            thread.join()
        self.returncode = self.process.returncode
        self.stdout, self.stderr = self.process.communicate()
        self.stdout = self.stdout.decode("utf-8")
        self.check_output(self.returncode)


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
    runner.add_test(Test("Simple Array", "functional/simple_array", []))
    runner.add_test(Test("Loop Timeout", "functional/loop_timeout", []))
    runner.run()
