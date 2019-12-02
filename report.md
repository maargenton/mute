test/test_require.cpp:17: enter: Scenario: Require is section allows further section
test/test_require.cpp:21: enter: given a first section before any failed require
test/test_require.cpp:22: enter: then the section is executed
test/test_require.cpp:23: failed: v < 123 (0x7b,'{')
    value: 123 (0x7b,'{')
test/test_require.cpp:22: leave: then the section is executed
test/test_require.cpp:21: leave: given a first section before any failed require
test/test_require.cpp:17: leave: Scenario: Require is section allows further section


test/test_require.cpp:23: error: expected v < 123 (0x7b,'{')
    value: 123 (0x7b,'{')
test/test_require.cpp:17: note: Scenario: Require is section allows further section
test/test_require.cpp:21: note: given a first section before any failed require
test/test_require.cpp:22: note: then the section is executed

```
Scenario: Require is section allows further section                     [FAILED]
  given a first section before any failed require                       [FAILED]
    then the section is executed                                        [FAILED]
      expected v < 123 (0x7b,'{')                                       [FAILED]
```

[PASSED]
[FAILED]

```
Scenario: All checks are executed regardless of success                 [FAILED]
  false == true                                                         [FAILED]
  false == true                                                         [FAILED]
  given something                                                       [FAILED]
    false == true                                                       [FAILED]
    false == true                                                       [FAILED]
    when doing something                                                [FAILED]
      false == true                                                     [FAILED]
      false == true                                                     [FAILED]
    then something happens                                              [FAILED]
      false == true                                                     [FAILED]
      false == true                                                     [FAILED]
```


| Description  | Status |
|--------|------|
|Scenario: All checks are executed regardless of successare executed regardless of successare executed regardless of successare executed regardless of successare executed regardless of successare executed regardless of successare executed regardless of successare executed regardless of successare executed regardless of successare executed regardless of success | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;given something | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;when doing something | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;then something happens | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`false == true` | **FAILED** |


test/test_checks.cpp:14: leave: then something happens
test/test_checks.cpp:11: leave: when doing something
test/test_checks.cpp:8: leave: given something
test/test_checks.cpp:5: leave: Scenario: All checks are executed regardless of success

test/test_checks.cpp:5: enter: Scenario: All checks are executed regardless of success
test/test_checks.cpp:6: failed: false == true
test/test_checks.cpp:7: failed: false == true
test/test_checks.cpp:8: enter: given something
test/test_checks.cpp:9: failed: false == true
test/test_checks.cpp:10: failed: false == true
test/test_checks.cpp:11: enter: when doing something
test/test_checks.cpp:12: failed: false == true
test/test_checks.cpp:13: failed: false == true
test/test_checks.cpp:18: enter: then something else happens
test/test_checks.cpp:19: failed: false == true
test/test_checks.cpp:20: failed: false == true
test/test_checks.cpp:18: leave: then something else happens
test/test_checks.cpp:11: leave: when doing something
test/test_checks.cpp:8: leave: given something
test/test_checks.cpp:5: leave: Scenario: All checks are executed regardless of success
