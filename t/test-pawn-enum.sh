#!/bin/sh
#
# Test for enum generation

. ./t/test-lib.sh

# Compare generated file with expected file
diff -u -w ${top_srcdir}/expected_test_enum.pb-pawn.inc ${top_builddir}/test_enum.pb-pawn.inc
test $? -eq 0 || die "Generated enum file differs from expected."

echo "Enum generation test passed."
exit 0 