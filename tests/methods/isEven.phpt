--TEST--
Decimal::isEven
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

    [ "2.0000000000000000000000000001",  true],  // truncated
    ["-2.0000000000000000000000000001",  true],  // truncated
    [ "2.000000000000000000000000001",   false], // not truncated
    ["-2.000000000000000000000000001",   false], // not truncated

    [0,         true],
    [1,         false],
    [2,         true],
    [3,         false],

    [-1,        false],
    [-2,        true],
    [-3,        false],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    ["-INF",    false],
    ["INF",     false],
    ["NAN",     false],
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = (new Decimal($number))->isEven();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECTF--
Warning: Loss of data on string conversion in %s on line %d

Warning: Loss of data on string conversion in %s on line %d
