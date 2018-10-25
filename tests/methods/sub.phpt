--TEST--
Decimal::sub
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
        new Decimal("8E+90000" /* 28 */),
        new Decimal("5E+90000" /* 28 */),
                    "3E+90000",   28,
    ],
    [
        new Decimal("4.3333", 1), // 4
        new Decimal("3.3333", 2), // 3.3
                    "0.7",    2
    ],
    [
        new Decimal("0.3" /* 28 */),
        new Decimal("0.1" /* 28 */),
                    "0.2",   28
    ],
    [
        new Decimal("-0.3" /* 28 */),
        new Decimal( "0.1" /* 28 */),
                    "-0.4",   28
    ],
    [
        new Decimal( "0.1" /* 28 */),
        new Decimal("-0.1" /* 28 */),
                     "0.2",   28
    ],
    [
        new Decimal("-0.1" /* 28 */),
        new Decimal("-0.1" /* 28 */),
                     "0.0",   28
    ],
    [
        new Decimal("0.33333333333333333333333333333", 20), // 0.33333333333333333333
        new Decimal("0.22222222222222222222222222222", 12), // 0.222222222222
                    "0.11111111111133333333",          20,
    ],
    [
        new Decimal("0.33333333333333333333333333333",  2), // 0.33
        new Decimal("0.22222222222222222222222222222", 16), // 0.2222222222222222
                    "0.1077777777777778",              16,
    ],
    [
        new Decimal("15"), 10, "5", 28
    ],
    [
        new Decimal("15", 2), 10, "5", 2
    ],
    [
        new Decimal("15", 50), 10, "5", 50
    ],
    [
        $a = new Decimal("7", 50), $a, "0", 50
    ],

    /* Special numbers */
    [new Decimal( "NAN"),  "NAN", (string)  (NAN -  NAN), 28],
    [new Decimal( "NAN"),  "INF", (string)  (NAN -  INF), 28],
    [new Decimal( "NAN"), "-INF", (string)  (NAN - -INF), 28],
    [new Decimal( "INF"),  "NAN", (string)  (INF -  NAN), 28],
    [new Decimal( "INF"),  "INF", (string)  (INF -  INF), 28],
    [new Decimal( "INF"), "-INF", (string)  (INF - -INF), 28],
    [new Decimal("-INF"),  "NAN", (string) (-INF -  NAN), 28],
    [new Decimal("-INF"),  "INF", (string) (-INF -  INF), 28],
    [new Decimal("-INF"), "-INF", (string) (-INF - -INF), 28],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $precision) = $test;

    $results = [
        $op1->sub($op2),
        $op1 - $op2,
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
$number->sub(5);
$number - 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECTF--
Warning: Loss of data on string conversion in %s on line 14

Warning: Loss of data on string conversion in %s on line 15

Warning: Loss of data on string conversion in %s on line 39

Warning: Loss of data on string conversion in %s on line 40

Warning: Loss of data on string conversion in %s on line 44

Warning: Loss of data on string conversion in %s on line 45
