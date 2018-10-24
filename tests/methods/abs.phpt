--TEST--
Decimal::abs
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
    ["-0.1", "0.1"],
    ["+0.1", "0.1"],
    [ "0.1", "0.1"],

    ["-1", "1"],
    [ "1", "1"],

    ["-INF", "INF"],
    ["INF", "INF"],
    ["NAN",  "NAN"],
];

/* Test expected output */
foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = decimal($number)->abs();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}

/* Test that abs does not modify the original */
$number = decimal("-1");
$result = $number->abs();

if ((string) $number !== "-1") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
