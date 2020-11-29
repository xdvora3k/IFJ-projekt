#!/bin/bash
#
# Run test_expression against multiple of .go files
#
# Synopsis:
#     test_expression
#

[[ -f "test-exppression" ]] || make build-test-expression || exit 1

./test-expression