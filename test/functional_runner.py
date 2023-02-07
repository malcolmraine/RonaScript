import subprocess
import sys
import threading
import time
import os
import datetime

rn_executable = sys.argv[1]

if os.name == "nt":
    os.system("colors")


def strip_ansi_codes(s: str) -> str:
    return s.strip("\0[33m") \
            .strip("\0[33[0m")\
            .strip("\0[31m")\
            .strip("\0[31[0m")


class Test(object):
    def __init__(self, name, source_dir: str, args: list, timeout: int = 5, invoke_count=1):
        self.stdout = []
        self.stderr = []
        self.source_dir = source_dir
        self.source_file = f"{source_dir}/source.rn"
        self.expected_output = f"{source_dir}/expected_output.txt"
        self.args = args
        self.timeout = timeout
        self.returncode = -1
        self.name = name
        self.log_file = f"{self.source_dir}/test.results"
        open(self.log_file, "w").close()
        self.process = None
        self.timeout_occurred = False
        self.invoke_count = invoke_count
        self.runtime = 0

    def log(self, *args):
        with open(self.log_file, "a+") as file:
            for arg in args:
                file.write(arg)
            file.write("\n")

    def check_output(self, returncode=0):
        with open(self.expected_output, "r") as file:
            expected = file.read().strip("\n").strip()
            passed = True
            invalid_output = ""

            for output in self.stdout:
                if expected not in output.strip("\n").strip():
                    passed = False
                    invalid_output = output.strip("\n").strip()
                    break

            if self.timeout_occurred:
                msg = "TIMEOUT"
            elif not passed:
                msg = "FAILED"
            else:
                msg = "PASSED"

            if passed:
                print(f"\033[92m{msg} ({round(self.runtime, 6)}s) - {self.name}")
            else:
                print(f"\033[91m{msg} ({round(self.runtime, 6)}s) - {self.name}")

            self.log(f"Test: {self.name}")
            self.log(f"Status: {msg}")
            self.log(f"Return code: {self.returncode}")
            self.log(f"Timestamp: {datetime.datetime.now()}")
            self.log()
            self.log("=========================================================")
            self.log("stderr")
            self.log("=========================================================")
            if self.stderr != self.stdout:
                for output in self.stderr:
                    if output:
                        self.log(output)
            self.log()
            self.log("=========================================================")
            self.log("Expected")
            self.log("=========================================================")
            self.log(expected)
            self.log()
            self.log("=========================================================")
            self.log("Actual")
            self.log("=========================================================")
            if not passed:
                self.log(invalid_output)
            elif len(self.stdout) > 0:
                self.log(self.stdout[0])
            else:
                self.log()

    def run(self):
        def target():
            self.process = subprocess.Popen([rn_executable, self.source_file, *self.args], stderr=subprocess.PIPE,
                                            stdout=subprocess.PIPE)
            self.process.communicate()

        start = time.time()
        for n in range(self.invoke_count):
            thread = threading.Thread(target=target)
            thread.start()

            thread.join(self.timeout)
            if thread.is_alive():
                self.process.terminate()
                self.timeout_occurred = True
                thread.join()
                break

            self.returncode = self.process.returncode
            stdout, stderr = self.process.communicate()
            self.stderr.append(stderr.decode("utf-8"))
            self.stdout.append(stdout.decode("utf-8"))
        end = time.time()
        self.runtime = end - start
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
    runner.add_test(Test("Command Line Arguments", "functional/commandline_args", ["-h"], timeout=1, invoke_count=100))
    runner.add_test(Test("Recursive GCF", "functional/recursive_gcf", [], timeout=2, invoke_count=3))
    runner.add_test(Test("Class Creation", "functional/class_creation", [], timeout=1, invoke_count=3))
    runner.add_test(Test("Simple Array", "functional/simple_array", [], invoke_count=3))
    runner.add_test(Test("Loop Timeout", "functional/loop_timeout", []))
    runner.add_test(Test("Matrix Multiplication", "functional/matrix_multiplication", [], invoke_count=3))
    runner.add_test(Test("Var Declaration Stress", "functional/var_decl_stress_test", [], timeout=3))
    runner.add_test(Test("Multiple Invokes", "functional/multiple_invokes", [], timeout=10, invoke_count=100))
    runner.run()
