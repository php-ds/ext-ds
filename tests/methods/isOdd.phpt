--TEST--
Decimal::isOdd
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

$tests = [
    /* Number, expected */

    [ "1E-50",  false],
    ["-1E-50",  false],

    [ "3.0000000000000000000000000001",  true],  // truncated, warning
    ["-3.0000000000000000000000000001",  true],  // truncated, warning
    [ "3.000000000000000000000000001",   false], // not truncated
    ["-3.000000000000000000000000001",   false], // not truncated

    [0,         false],
    [1,         true],
    [2,         false],
    [3,         true],

    [-1,        true],
    [-2,        false],
    [-3,        true],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    ["-INF",    false],
    ["INF",    false],
    ["NAN",     false],
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = (new Decimal($number))->isOdd();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECTF--
Warning: Loss of data on string conversion in %s on line %d

Warning: Loss of data on string conversion in %s on line %d
