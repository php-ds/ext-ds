--TEST--
Decimal::floor
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
    ["-0.1",       "-1"],
    [ "0.1",        "0"],

    [ "1E-50",      "0"],
    ["-1E-50",     "-1"],

    [ "NAN",      "NAN"],
    [ "INF",      "INF"],
    ["-INF",     "-INF"],

    /* Example from PHP documentation on floating point precision */
    [decimal("0.1")->add("0.7")->mul(10)->floor(), "8"]
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = decimal($number)->floor();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}

/* Test that floor does not modify the original */
$number = decimal("2.5");
$result = $number->floor();

if ((string) $number !== "2.5") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
