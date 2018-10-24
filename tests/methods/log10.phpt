--TEST--
Decimal::log10
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
    [decimal("0.001"), "-3",    28],
    [decimal("0.01"),  "-2",    28],
    [decimal("0.1"),   "-1",    28],
    [decimal("1"),      "0",    28],
    [decimal("10"),     "1",    28],
    [decimal("100"),    "2",    28],
    [decimal("1000"),   "3",    28],

    [decimal("-0.001"), "NAN",  28],
    [decimal("-0.01"),  "NAN",  28],
    [decimal("-0.1"),   "NAN",  28],
    [decimal("-1"),     "NAN",  28],
    [decimal("-10"),    "NAN",  28],
    [decimal("-100"),   "NAN",  28],
    [decimal("-1000"),  "NAN",  28],

    [decimal("123.456"),            "2.091512201627771681069399777",                       28],
    [decimal("123.456", 50),        "2.0915122016277716810693997770679057946535841876887", 50],

    [decimal("0.000123456"),       "-3.908487798372228318930600223",                       28],
    [decimal("0.000123456", 50),   "-3.9084877983722283189306002229320942053464158123113", 50],

    [decimal("-INF"), (string) log10(-INF), 28],
    [decimal( "NAN"), (string) log10( NAN), 28],
    [decimal( "INF"), (string) log10( INF), 28],
    [decimal(   "0"), (string) log10(   0), 28],
    [decimal(  "-1"), (string) log10(  -1), 28],
];

foreach ($tests as $test) {
    list($op1, $expect, $precision) = $test;

    $result = $op1->log10();

    if ((string) $result !== $expect || $result->precision() !== $precision) {
        print_r(compact("number", "result", "expect", "precision"));
    }
}

/* Test that log10 does not modify the original */
$number = decimal("2");
$result = $number->log10();

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
