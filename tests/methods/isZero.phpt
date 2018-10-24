--TEST--
Decimal::isZero
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

    [0,         true],
    [1,         false],
    [2,         false],
    [3,         false],

    [-1,        false],
    [-2,        false],
    [-3,        false],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    [ "NAN",    false],
    [ "INF",    false],
    ["-INF",    false],
];

foreach ($tests as $pair) {
    $number = $pair[0];
    $expect = $pair[1];
    $result = decimal($number)->isZero();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
