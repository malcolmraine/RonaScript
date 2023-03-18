import datetime
import glob
import json
import os
import subprocess
import sys
import threading
import time
from difflib import ndiff

rn_executable = sys.argv[1]

if os.name == "nt":
    os.system("colors")


def strip_ansi_codes(s: str) -> str:
    return s.strip("\0[33m") \
        .strip("\0[33[0m") \
        .strip("\0[31m") \
        .strip("\0[31[0m")


class Test(object):
    def __init__(self,
                 name,
                 source_dir: str,
                 expected_output_file: str = "",
                 args: list = None,
                 timeout: int = 5,
                 invoke_count=1,
                 enabled=False):
        self.stdout = []
        self.stderr = []
        self.source_dir = source_dir
        self.source_file = f"{source_dir}/source.rn"
        self.expected_output = f"{source_dir}/{expected_output_file}"
        self.enabled = enabled

        if args is None:
            self.args = []
        else:
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

    def log_header(self, header: str):
        self.log("=" * 80)
        self.log(" " + header)
        self.log("=" * 80)

    def check_output(self):
        with open(self.expected_output, "r") as file:
            expected = file.read().strip("\n").strip()
            passed = True
            invalid_output = ""

            for output in self.stdout:
                if expected not in output.strip("\n").strip():
                    passed = False
                    invalid_output = output.strip("\n").strip()
                    break

            if self.returncode != 0:
                passed = False

            if self.timeout_occurred:
                msg = "TIMEOUT"
            elif not passed:
                msg = "FAILED"
            else:
                msg = "PASSED"

            if passed and not self.timeout_occurred:
                print(f"\033[92m{msg} ({round(self.runtime, 6)}s) - {self.name}\033[0m")
            else:
                print(f"\033[91m{msg} ({round(self.runtime, 6)}s) - {self.name}\033[0m")

            self.log(f"Test: {self.name}")
            self.log(f"Status: {msg}")
            self.log(f"Return code: {self.returncode}")
            self.log(f"Timestamp: {datetime.datetime.now()}")
            self.log()
            self.log_header("stderr")
            if self.stderr != self.stdout:
                for output in self.stderr:
                    if output:
                        self.log(output)
            self.log()
            self.log_header("Expected")
            self.log(expected)
            self.log()
            self.log_header("Actual")
            if not passed:
                self.log(invalid_output)
            elif len(self.stdout) > 0:
                self.log(self.stdout[0])
            else:
                self.log()
                self.log()
            self.log_header("Diff")
            if not passed:
                self.log("\n".join(ndiff(expected.splitlines(), invalid_output.splitlines())))

    def run(self):
        if not self.enabled:
            # print(f"DISABLED ({round(self.runtime, 6)}s) - {self.name}\033[0m")
            return

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
    for file in glob.glob("functional/**/manifest.json"):
        with open(file, "r") as manifest_file:
            manifest = json.load(manifest_file)
        runner.add_test(Test(manifest.get("title"),
                             file.replace("manifest.json", ""),
                             expected_output_file=manifest.get("expected_output", "expected_output.txt"),
                             args=manifest.get("args", []),
                             timeout=manifest.get("timeout", 5),
                             invoke_count=manifest.get("invoke_count", 1),
                             enabled=manifest.get("enabled", False)))
    runner.run()
