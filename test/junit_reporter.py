from typing import List


class TestXmlObject(object):
    def __init__(self, tag):
        self.attributes = dict()
        self.detail = ""
        self.tag = tag

    def to_xml_string(self, tabs=0):
        result = "\t" * tabs
        result += f"<{self.tag}"

        for key in self.attributes:
            result += f" {key}=\"{self.attributes[key]}\""

        if self.detail:
            result += ">" + self.detail + "\n" + "\t" * tabs
            result += f"</{self.tag}>"
        else:
            result += "/>"

        return result


class TestFailure(TestXmlObject):
    def __init__(self, message, failure_type, detail):
        super().__init__("failure")
        self.attributes["message"] = message
        self.attributes["type"] = failure_type
        self.detail = "\n" + detail


class TestCase(TestXmlObject):
    def __init__(self, test_case_id, test_case_name, total_time):
        super().__init__("testcase")
        self.attributes["id"] = test_case_id
        self.attributes["name"] = test_case_name
        self.attributes["time"] = round(total_time, 6)
        self.failures = []

    def add_failure(self, message, failure_type, detail):
        self.failures.append(TestFailure(message, failure_type, detail))

    def failed(self):
        return len(self.failures) > 0

    def to_xml_string(self, tabs=0):
        for failure in self.failures:
            self.detail += "\n" + failure.to_xml_string(tabs + 1)
        return super().to_xml_string(tabs)


class TestSuite(TestXmlObject):
    def __init__(self, test_suite_id, test_name, total_time):
        super().__init__("testsuite")
        self.attributes["id"] = test_suite_id
        self.attributes["name"] = test_name
        self.attributes["time"] = round(total_time, 6)
        self.attributes["tests"] = 0
        self.attributes["failures"] = 0
        self.test_cases: List[TestCase] = []

    def failed_count(self):
        return len([_ for _ in self.test_cases if _.failed()])

    def pass_count(self):
        return len(self.test_cases) - self.failed_count()

    def total_time(self):
        return sum([float(_.attributes["time"]) for _ in self.test_cases])

    def add_testcase(self, testcase: TestCase):
        self.attributes["tests"] += 1
        if testcase.failed():
            self.attributes["failures"] += 1
        self.test_cases.append(testcase)

    def to_xml_string(self, tabs=0):
        for testcase in self.test_cases:
            self.detail += "\n" + testcase.to_xml_string(tabs + 1)
        return super().to_xml_string(tabs)


class JUnitReporter(object):
    def __init__(self):
        self.test_suites: List[TestSuite] = []
        self.id = ""
        self.name = ""

    def pass_count(self):
        return sum([_.pass_count() for _ in self.test_suites])

    def failed_count(self):
        return sum([_.failed_count() for _ in self.test_suites])

    def total_time(self):
        return sum([float(_.total_time()) for _ in self.test_suites])

    def to_xml(self):
        pass_count = self.pass_count()
        fail_count = self.failed_count()
        total_count = pass_count + fail_count
        result = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
        result += f"<testsuites " \
                  f"id=\"{self.id}\" " \
                  f"name=\"{self.name}\" " \
                  f"tests=\"{total_count}\" " \
                  f"failures=\"{fail_count}\" " \
                  f"time=\"{round(self.total_time(), 6)}\">"
        for test_suite in self.test_suites:
            result += "\n" + test_suite.to_xml_string(1) + "\n"
        result += "</testsuites>"
        return result
