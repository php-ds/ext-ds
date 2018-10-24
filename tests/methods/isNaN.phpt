--TEST--
Decimal::isNAN
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * Shortcut to construct a new decimal.
 */
function decimal(...$args) { return new Decimal(...$args); }

$tests = [
    /* Number, expected */

    [ "1E-50",  false],
    ["-1E-50",  false],

    [0,         false],
    [1,         false],
    [2,         false],
    [3,         false],

    [-1,        false],
    [-2,        false],
    [-3,        false],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    ["-INF",    false],
    ["INF",    false],
    ["NAN",     true],
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = decimal($number)->isNAN();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
