--TEST--
Decimal::sqrt
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
    [decimal(),             "0",                                                       28],
    [decimal("0.1", 5),     "0.31623",                                                  5],
    [decimal("0.1"),        "0.3162277660168379331998893544",                          28],
    [decimal("0.1", 50),    "0.31622776601683793319988935444327185337195551393252",    50],

    [decimal("144"),        "12",                               28],
    [decimal("145", 3),     "12.0",                              3],
    [decimal("145"),        "12.04159457879229548012824103",    28],

    [decimal( "1E-50"),     "1E-25",    28],
    [decimal("-1E-50"),     "NAN",      28],

    [decimal( "NAN"),       "NAN",      28],
    [decimal( "INF"),       "INF",      28],
    [decimal("-INF"),      "-INF",      28],
];

foreach ($tests as $test) {
    list($op1, $expect, $precision) = $test;

    $result = $op1->sqrt();

    if ((string) $result !== (string) $expect || $result->precision() !== $precision) {
        print_r(compact("number", "result", "expect", "precision"));
    }
}

/* Test that ceil does not modify the original */
$number = decimal("1.5");
$result = $number->sqrt();

if ((string) $number !== "1.5") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
