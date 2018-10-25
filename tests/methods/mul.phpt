--TEST--
Decimal::mul
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * OP1, OP2, Expected Result, Expected Precision
 */
$tests = [
    [
        new Decimal("1.2345678901234567890123456789" /* 28 */), // 1.234567890123456789012345679
        new Decimal("9.8765432109876543210987654321" /* 28 */), // 9.876543210987654321098765432
                   "12.19326311370217952261850327",    28
    ],
    [
        new Decimal("1.2345678901234567890123456789",   20), // 1.234567890123456789
        new Decimal("9.8765432109876543210987654321",   12), // 9.87654321099
                   "12.193263113705075422",            20
    ],
    [
        new Decimal("1.2345678901234567890123456789",    5), // 1.2346
        new Decimal("9.8765432109876543210987654321",   20), // 9.8765432109876543211
                   "12.193580248285358025",            20
    ],
    [
        new Decimal("4.3333",    1), // 4
        new Decimal("3.3333",    2), // 3.3
                   "13",        2
    ],
    [
        new Decimal("0.3" /* 28 */),
        new Decimal("0.1" /* 28 */),
                   "0.03",  28
    ],
    [
        new Decimal("-0.3" /* 28 */),
        new Decimal( "0.1" /* 28 */),
                   "-0.03",  28
    ],
    [
        new Decimal("15"), 10, "150", 28
    ],
    [
        new Decimal("15", 2), 10, "1.5E+2", 2
    ],
    [
        new Decimal("15", 50), 10, "150", 50
    ],
    [
        new Decimal("0.619207", 6), 10000000, "6.19207E+6", 6
    ],
    [
        new Decimal("0.619207"), 10000000, "6192070.000000", 28
    ],
    [
        $a = new Decimal("7", 50), $a, "49", 50
    ],

    /* Special numbers */
    [new Decimal( "NAN"),  "NAN", (string)  (NAN *  NAN), 28],
    [new Decimal( "NAN"),  "INF", (string)  (NAN *  INF), 28],
    [new Decimal( "NAN"), "-INF", (string)  (NAN * -INF), 28],
    [new Decimal( "INF"),  "NAN", (string)  (INF *  NAN), 28],
    [new Decimal( "INF"),  "INF", (string)  (INF *  INF), 28],
    [new Decimal( "INF"), "-INF", (string)  (INF * -INF), 28],
    [new Decimal("-INF"),  "NAN", (string) (-INF *  NAN), 28],
    [new Decimal("-INF"),  "INF", (string) (-INF *  INF), 28],
    [new Decimal("-INF"), "-INF", (string) (-INF * -INF), 28],

    [new Decimal( "NAN"), 0, "NAN", 28],
    [new Decimal( "INF"), 0, "NAN", 28],
    [new Decimal("-INF"), 0, "NAN", 28],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $precision) = $test;

    $results = [
        $op1->mul($op2),

        $op1 * $op2,
        $op2 * $op1,
    ];

    foreach ($results as $result) {
        if ([(string) $result, $result->precision()] !== [$expect, $precision]) {
            print_r(compact("index", "op1", "op2", "result", "expect", "precision"));
            break;
        }
    }
}

/* Test that op does not modify the original */
$number = new Decimal("2");
$number->mul(5);
$number * 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECTF--
Warning: Loss of data on string conversion in %s on line 9

Warning: Loss of data on string conversion in %s on line 10

Warning: Loss of data on string conversion in %s on line 14

Warning: Loss of data on string conversion in %s on line 15

Warning: Loss of data on string conversion in %s on line 19

Warning: Loss of data on string conversion in %s on line 20

Warning: Loss of data on string conversion in %s on line 24

Warning: Loss of data on string conversion in %s on line 25

Warning: Loss of data on integer conversion in %s on line 77

Warning: Loss of data on integer conversion in %s on line 79

Warning: Loss of data on integer conversion in %s on line 80
