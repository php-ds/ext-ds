--TEST--
Decimal::ln
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
    [decimal("0.001"), "-6.907755278982137052053974364",    28],
    [decimal("0.01"),  "-4.605170185988091368035982909",    28],
    [decimal("0.1"),   "-2.302585092994045684017991455",    28],
    [decimal("1"),      "0",                                28],
    [decimal("10"),     "2.302585092994045684017991455",    28],
    [decimal("100"),    "4.605170185988091368035982909",    28],
    [decimal("1000"),   "6.907755278982137052053974364",    28],

    [decimal("-0.001"), "NAN",  28],
    [decimal("-0.01"),  "NAN",  28],
    [decimal("-0.1"),   "NAN",  28],
    [decimal("-1"),     "NAN",  28],
    [decimal("-10"),    "NAN",  28],
    [decimal("-100"),   "NAN",  28],
    [decimal("-1000"),  "NAN",  28],

    [decimal("123.456"),            "4.815884817283263883109232105",                       28],
    [decimal("123.456", 50),        "4.8158848172832638831092321051665255771721581355055", 50],

    [decimal("0.000123456"),       "-8.999625740681010220998716623",                       28],
    [decimal("0.000123456", 50),   "-8.9996257406810102209987166229396596684344507962672", 50],

    [decimal("-INF"), (string) log(-INF), 28],
    [decimal( "NAN"), (string) log( NAN), 28],
    [decimal( "INF"), (string) log( INF), 28],
    [decimal(   "0"), (string) log(   0), 28],
    [decimal(  "-1"), (string) log(  -1), 28],
];

foreach ($tests as $test) {
    list($op1, $expect, $precision) = $test;

    $result = $op1->ln();

    if ((string) $result !== $expect || $result->precision() !== $precision) {
        print_r(compact("number", "result", "expect", "precision"));
    }
}

/* Test that log10 does not modify the original */
$number = decimal("2");
$result = $number->ln();

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
