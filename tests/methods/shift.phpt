--TEST--
Decimal::shift
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
    /* A * B = C */

    ["1",       -3,      "0.001"],
    ["1",       -2,      "0.01"],
    ["1",       -1,      "0.1"],
    ["1",        0,      "1"],
    ["1",        1,      "1E+1"],
    ["1",        2,      "1E+2"],
    ["1",        3,      "1E+3"],

    ["1",       -27,      "1E-27"],
    ["1",        27,      "1E+27"],

    ["0.5",     -3,      "0.0005"],
    ["0.5",     -2,      "0.005"],
    ["0.5",     -1,      "0.05"],
    ["0.5",      0,      "0.5"],
    ["0.5",      1,      "5"],
    ["0.5",      2,      "5E+1"],
    ["0.5",      3,      "5E+2"],

    ["1",       -10000,  "1E-10000"],
    ["1",        10000,  "1E+10000"],

    [ "NAN",    2,       "NAN"],
    [ "INF",    2,       "INF"],
    ["-INF",    2,      "-INF"],
];

foreach ($tests as $test) {
    list($number, $exponent, $expect) = $test;

    $result = decimal($number)->shift($exponent);

    if ((string) $result !== $expect || $result->precision() !== 28) {
        print_r(compact("number", "exponent", "result", "expect"));
    }
}
?>
--EXPECT--
