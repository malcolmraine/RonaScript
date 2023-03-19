from typing import List


TAB_STR = "    "


class TestXmlObject(object):
    def __init__(self, tag: str):
        self.attributes = dict()
        self.detail = ""
        self.tag = tag

    def to_xml_string(self, tabs=0) -> str:
        result = TAB_STR * tabs
        result += f"<{self.tag}"

        for key in self.attributes:
            result += f" {key}=\"{self.attributes[key]}\""

        if self.detail:
            result += ">" + self.detail + "\n" + TAB_STR * tabs
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


class TestSkipped(TestXmlObject):
    def __init__(self, message=""):
        super().__init__("skipped")
        self.attributes["message"] = message


class TestProperty(TestXmlObject):
    def __init__(self, name, value):
        super().__init__("property")
        self.attributes["name"] = name
        self.attributes["value"] = value


class TestCase(TestXmlObject):
    def __init__(self, test_case_id, test_case_name, total_time):
        super().__init__("testcase")
        self.attributes["id"] = test_case_id
        self.attributes["name"] = test_case_name
        self.attributes["time"] = round(total_time, 6)
        self.failures: List[TestFailure] = []
        self.skipped: List[TestSkipped] = []
        self.properties: List[TestProperty] = []

    def add_failure(self, message, failure_type, detail):
        self.failures.append(TestFailure(message, failure_type, detail))

    def failed(self) -> int:
        return len(self.failures) > 0

    def to_xml_string(self, tabs=0) -> str:
        if self.properties:
            self.detail += f"\n{TAB_STR * (tabs + 1)}<properties>"
            for prop in self.properties:
                self.detail += "\n" + prop.to_xml_string(tabs + 2)
            self.detail += f"\n{TAB_STR * (tabs + 1)}</properties>"

        for skip in self.skipped:
            self.detail += "\n" + skip.to_xml_string(tabs + 1)

        for failure in self.failures:
            self.detail += "\n" + failure.to_xml_string(tabs + 1)
        return super().to_xml_string(tabs)


class TestSuite(TestXmlObject):
    def __init__(self, test_suite_id: str, test_name: str, total_time):
        super().__init__("testsuite")
        self.attributes["id"] = test_suite_id
        self.attributes["name"] = test_name
        self.attributes["time"] = round(total_time, 6)
        self.attributes["tests"] = 0
        self.attributes["failures"] = 0
        self.attributes["skipped"] = 0
        self.test_cases: List[TestCase] = []
        self.properties: List[TestProperty] = []

    def failed_count(self) -> int:
        return len([_ for _ in self.test_cases if _.failed()])

    def pass_count(self) -> int:
        return len(self.test_cases) - self.failed_count()

    def skipped_count(self) -> int:
        return sum([1 for _ in self.test_cases if _.skipped])

    def test_count(self) -> int:
        return len(self.test_cases)

    def total_time(self) -> float:
        return sum([float(_.attributes["time"]) for _ in self.test_cases])

    def add_testcase(self, testcase: TestCase):
        self.attributes["tests"] += 1
        if testcase.failed():
            self.attributes["failures"] += 1
        self.test_cases.append(testcase)

    def to_xml_string(self, tabs=0) -> str:
        if self.properties:
            self.detail += f"\n{TAB_STR * (tabs + 1)}<properties>"
            for prop in self.properties:
                self.detail += "\n" + prop.to_xml_string(tabs + 2)
            self.detail += f"\n{TAB_STR * (tabs + 1)}</properties>"

        for testcase in self.test_cases:
            self.detail += "\n" + testcase.to_xml_string(tabs + 1)
        return super().to_xml_string(tabs)


class JUnitReporter(TestXmlObject):
    def __init__(self):
        super().__init__("testsuites")
        self.test_suites: List[TestSuite] = []
        self.attributes["id"] = ""
        self.attributes["name"] = ""
        self.attributes["tests"] = 0
        self.attributes["failures"] = 0
        self.attributes["skipped"] = 0
        self.attributes["time"] = 0
        self.attributes["timestamp"] = ""

    def pass_count(self) -> int:
        return sum([_.pass_count() for _ in self.test_suites])

    def failed_count(self) -> int:
        return sum([_.failed_count() for _ in self.test_suites])

    def total_time(self) -> float:
        return sum([float(_.total_time()) for _ in self.test_suites])

    def write(self, filename):
        with open(filename, "w") as file:
            file.write(self.to_xml())

    def to_xml(self) -> str:
        self.attributes["tests"] = sum([_.test_count() for _ in self.test_suites])
        self.attributes["failures"] = sum([_.failed_count() for _ in self.test_suites])
        self.attributes["skipped"] = sum([_.skipped_count() for _ in self.test_suites])
        self.attributes["time"] = sum([_.total_time() for _ in self.test_suites])
        result = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"

        for test_suite in self.test_suites:
            self.detail += "\n" + test_suite.to_xml_string(1) + "\n"
        result += super().to_xml_string()
        return result
