import contextlib
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

import sunc_runner


def suite_log(failed=()):
    lines = [f"SUNCIO-PLACE {sunc_runner.PLACE_ID}"]
    for index in range(sunc_runner.EXPECTED_CHECKS):
        symbol = "\u274c" if index in failed else "\u2705"
        lines.append(f"2026-11-14T00:00:00Z [FLog::CreatorOutput] {symbol} function_{index}")
    passed = sunc_runner.EXPECTED_CHECKS - len(failed)
    percent = passed * 100 // sunc_runner.EXPECTED_CHECKS
    lines += [f"Passed the test with {percent}%\u200c success rate ({passed} \u200cout \u200cof \u200c86)",
              "SUNCIO-DONE"]
    return "\n".join(lines)


class ReportValidationTests(unittest.TestCase):
    def test_complete_unicode_player_log_passes(self):
        report = sunc_runner.validate_report(suite_log())
        self.assertEqual((report["passed"], report["failed"], report["percent"]), (86, 0, 100))

    def test_full_score_followed_by_disconnect_is_invalid(self):
        output = suite_log() + "\n[FLog::Network] Client has been disconnected with reason: kicked"
        with self.assertRaisesRegex(ValueError, "disconnected"):
            sunc_runner.validate_report(output)
        report = sunc_runner.failure_report(output, "disconnected")
        self.assertEqual(report["status"], "invalid")
        self.assertNotIn("percent", report)

    def test_missing_check_cannot_claim_full_score(self):
        output = "\n".join(line for line in suite_log().splitlines()
                           if not line.endswith("\u2705 function_40"))
        with self.assertRaisesRegex(ValueError, "85/86"):
            sunc_runner.validate_report(output)

    def test_duplicate_result_cannot_replace_missing_function(self):
        output = suite_log().replace("function_40", "function_39")
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            sunc_runner.validate_report(output)

    def test_results_from_another_place_are_invalid(self):
        output = suite_log().replace(str(sunc_runner.PLACE_ID), "79035306837882")
        self.assertEqual(sunc_runner.failure_report(output, "wrong place")["status"], "invalid")

    def test_out_of_order_completion_is_invalid(self):
        output = "SUNCIO-DONE\n" + suite_log().removesuffix("SUNCIO-DONE")
        self.assertEqual(sunc_runner.failure_report(output, "wrong order")["status"], "invalid")

    def test_complete_failure_preserves_actual_score(self):
        output = suite_log(failed=(20,))
        with self.assertRaisesRegex(ValueError, "85/86"):
            sunc_runner.validate_report(output)
        report = sunc_runner.failure_report(output, "failed test")
        self.assertEqual((report["status"], report["passed"], report["failed"]), ("failed", 85, 1))
        self.assertEqual(report["failed_checks"], ["function_20"])

    def test_inconsistent_percentage_is_invalid(self):
        output = suite_log(failed=(20,)).replace("98%", "100%")
        self.assertEqual(sunc_runner.failure_report(output, "bad summary")["status"], "invalid")

    def test_incomplete_run_has_no_score(self):
        report = sunc_runner.failure_report(f"SUNCIO-PLACE {sunc_runner.PLACE_ID}", "timeout")
        self.assertEqual((report["status"], report["checks_received"]), ("invalid", 0))
        self.assertNotIn("percent", report)

    def test_failed_rerun_replaces_previous_success_report(self):
        with tempfile.TemporaryDirectory() as folder:
            output = Path(folder) / "result.log"
            output.write_text(suite_log(), encoding="utf-8")
            output.with_suffix(".json").write_text('{"status":"passed","percent":100}', encoding="utf-8")
            with patch.object(sys, "argv", ["sunc_runner.py", "--output", str(output)]), \
                    patch.object(sunc_runner, "run_suite", side_effect=RuntimeError("initialization failed")), \
                    contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(sunc_runner.main(), 1)
            report = json.loads(output.with_suffix(".json").read_text(encoding="utf-8"))
            self.assertEqual(report["status"], "invalid")
            self.assertEqual(report["checks_received"], 0)
            self.assertNotIn("percent", report)


if __name__ == "__main__":
    unittest.main()
