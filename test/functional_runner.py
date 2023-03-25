import datetime
import glob
import json
import os
import subprocess
import sys
import threading
import time
from difflib import ndiff, SequenceMatcher
import junit_reporter


rn_executable = sys.argv[1]

if os.name == "nt":
    os.system("colors")


def remove_ansi_codes(s: str) -> str:
    return s.replace("\0[33m", "") \
        .replace("\033[31m", "") \
        .replace("\033[0m", "")


class Test(object):
    fixed_args = []

    def __init__(self,
                 name,
                 test_id,
                 source_dir: str,
                 expected_output_file: str = "",
                 args: list = None,
                 timeout: int = 5,
                 invoke_count=1,
                 enabled=False,
                 similarity=1.0):
        self.id = test_id
        self.stdout = []
        self.stderr = []
        self.source_dir = source_dir
        self.source_file = f"{source_dir}/source.rn"
        self.expected_output = f"{source_dir}/{expected_output_file}"
        self.enabled = enabled
        self.similarity_threshold = similarity
        self.passed = False
        self.timestamp = ""

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
        self.similarity_scores = []
        self.msg = ""

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
        if not self.enabled:
            return

        with open(self.expected_output, "r") as file:
            expected = file.read().strip("\n").strip()
            self.passed = True
            invalid_output = ""

            for output in self.stdout:
                actual = remove_ansi_codes(output.strip("\n").strip())
                similarity = SequenceMatcher(None, expected, actual).ratio()
                self.similarity_scores.append(similarity)

                if self.similarity_threshold < 1.0:
                    if similarity < self.similarity_threshold:
                        self.passed = False
                        invalid_output = actual
                        break
                elif expected not in actual or len(expected) == 0 and len(actual) != 0:
                    self.passed = False
                    invalid_output = actual
                    break

            if self.returncode != 0:
                self.passed = False

            if self.timeout_occurred:
                self.msg = "TIMEOUT"
            elif not self.passed:
                self.msg = "FAILED"
            else:
                self.msg = "PASSED"

            if self.passed and not self.timeout_occurred:
                print(f"\033[92m{self.msg} ({round(self.runtime, 6)}s) - {self.name}\033[0m")
            else:
                print(f"\033[91m{self.msg} ({round(self.runtime, 6)}s) - {self.name}\033[0m")

            self.log(f"Test: {self.name}")
            self.log(f"Status: {self.msg}")
            self.log(f"Return code: {self.returncode}")
            self.log(f"Timestamp: {datetime.datetime.now()}")
            self.log(f"Similarities: {self.similarity_scores}")
            self.log(f"Runtime: {round(self.runtime, 6)}s")
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
            if not self.passed:
                self.log(invalid_output)
            elif len(self.stdout) > 0:
                self.log(self.stdout[0])
            else:
                self.log()
                self.log()
            self.log_header("Diff")
            if not self.passed:
                self.log("\n".join(ndiff(expected.splitlines(), invalid_output.splitlines())))

    def run(self):
        self.timestamp = str(datetime.datetime.now().replace(microsecond=0).isoformat())
        if not self.enabled:
            self.msg = "DISABLED"
            self.passed = True
            print(f"{self.msg} ({round(self.runtime, 6)}s) - {self.name}\033[0m")
            return

        def target():
            self.process = subprocess.Popen(
                [rn_executable, self.source_file, *self.fixed_args, *self.args],
                stderr=subprocess.PIPE,
                stdout=subprocess.PIPE)
            self.process.communicate()

        self.similarity_scores = []
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
        self.enabled_count = 0
        self.disabled_count = 0
        self.passed_count = 0
        self.timeout_count = 0
        self.failed_count = 0
        self.total_runtime = 0.0

    def add_test(self, test: Test):
        self.tests.append(test)
        if test.enabled:
            self.enabled_count += 1
        else:
            self.disabled_count += 1

    def run(self):
        for test in self.tests:
            test.run()
            self.total_runtime += test.runtime
            if test.timeout_occurred:
                self.timeout_count += 1
            elif test.enabled:
                if test.passed:
                    self.passed_count += 1
                else:
                    self.failed_count += 1
        print("-------------------------------------------------------")
        print(f"Total tests: {len(self.tests)}")
        print(f"Enabled/Disabled: {self.enabled_count}/{self.disabled_count}")
        print(f"Passed: {self.passed_count}")
        print(f"Failed: {self.failed_count}")
        print(f"Timed Out: {self.timeout_count}")
        print(f"Total runtime: {round(self.total_runtime, 3)}s")
        print(f"Avg. runtime: {round(self.total_runtime / (self.enabled_count or 1), 3)}s")
        print(f"\nPass rate: {round(self.passed_count / (self.enabled_count or 1), 3)}")


if __name__ == "__main__":
    runner = TestRunner()
    Test.fixed_args.append("--no-validation")

    for file in glob.glob("functional/**/manifest.json"):
        with open(file, "r") as manifest_file:
            manifest = json.load(manifest_file)
        runner.add_test(Test(manifest.get("title"),
                             manifest.get("name"),
                             os.path.dirname(file),
                             expected_output_file=manifest.get("expected_output", "expected_output.txt"),
                             args=[*manifest.get("args", [])],
                             timeout=manifest.get("timeout", 5),
                             invoke_count=manifest.get("invoke_count", 1),
                             enabled=manifest.get("enabled", False),
                             similarity=manifest.get("similarity_threshold", 1.0)))
    timestamp = str(datetime.datetime.now().replace(microsecond=0).isoformat())
    runner.run()
    reporter = junit_reporter.JUnitReporter()
    reporter.attributes["timestamp"] = timestamp
    suite = junit_reporter.TestSuite("1", "Functional Tests", runner.total_runtime)
    suite.attributes["timestamp"] = timestamp

    for arg in Test.fixed_args:
        suite.properties.append(junit_reporter.TestProperty("argument", arg))

    for test in runner.tests:
        testcase = junit_reporter.TestCase(test.id, test.name, round(test.runtime, 6))
        testcase.attributes["timestamp"] = test.timestamp

        for arg in test.args:
            testcase.properties.append(junit_reporter.TestProperty("argument", arg))

        if not test.enabled:
            testcase.skipped.append(junit_reporter.TestSkipped())
        elif not test.passed:
            failure_detail = f"Return code: {test.returncode}"
            testcase.failures.append(junit_reporter.TestFailure(test.msg, "Error", failure_detail))
        suite.add_testcase(testcase)

    reporter.test_suites.append(suite)
    reporter.write("rona_test.junit.xml")
