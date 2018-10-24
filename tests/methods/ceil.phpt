--TEST--
Decimal::ceil
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
    ["-0.1",       "-0"],
    [ "0.1",        "1"],

    [ "1E-50",      "1"],
    ["-1E-50",     "-0"],

    [ "NAN",        "NAN"],
    [ "INF",        "INF"],
    ["-INF",       "-INF"],
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = decimal($number)->ceil();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}

/* Test that ceil does not modify the original */
$number = decimal("-1.5");
$result = $number->ceil();

if ((string) $number !== "-1.5") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
