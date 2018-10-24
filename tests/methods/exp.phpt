--TEST--
Decimal::exp
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
    [decimal("-0.1", 50), "0.90483741803595957316424905944643662119470536098040", 50],
    [decimal( "0.1", 50), "1.1051709180756476248117078264902466682245471947375", 50],

    [decimal(), "1", 28],

    [decimal("-INF"), "0",   28],
    [decimal( "NAN"), "NAN", 28],
    [decimal( "INF"), "INF", 28],
];

foreach ($tests as $test) {
    list($op1, $expect, $precision) = $test;

    $result = $op1->exp();

    if ((string) $result !== $expect || $result->precision() !== $precision) {
        print_r(compact("number", "result", "expect", "precision"));
    }
}

/* Test that exp does not modify the original */
$number = decimal("2");
$result = $number->exp();

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
