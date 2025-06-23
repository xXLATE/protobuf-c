#!/bin/sh
#
# Test for message generation

. ./t/test-lib.sh

# Compare generated file with expected file
diff -u -w ${top_srcdir}/expected_test_message.pb-pawn.inc ${top_builddir}/test_message.pb-pawn.inc
test $? -eq 0 || die "Generated message file differs from expected."

echo "Message generation test passed."
exit 0 