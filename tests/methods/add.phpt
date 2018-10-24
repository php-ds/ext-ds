--TEST--
Decimal::add
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
        new Decimal("9.0E+90000" /* 28 */),
        new Decimal("9.0E+90000" /* 28 */),
                    "1.80E+90001",  28,
    ],
    [
        new Decimal("1.1111", 1), // 1.0, warning
        new Decimal("3.3333", 2), // 3.3, warning
                    "4.3",    2
    ],
    [
        new Decimal("0.1" /* 28 */),
        new Decimal("0.1" /* 28 */),
                    "0.2",   28
    ],
    [
        new Decimal("-0.1" /* 28 */),
        new Decimal( "0.1" /* 28 */),
                     "0.0",   28
    ],
    [
        new Decimal( "0.1" /* 28 */),
        new Decimal("-0.1" /* 28 */),
                     "0.0",   28
    ],
    [
        new Decimal("-0.1" /* 28 */),
        new Decimal("-0.1" /* 28 */),
                    "-0.2",   28
    ],
    [
        new Decimal("0.11111111111111111111111111111", 20), // 0.11111111111111111111
        new Decimal("0.22222222222222222222222222222", 12), // 0.222222222222
                    "0.33333333333311111111",          20,
    ],
    [
        new Decimal("0.11111111111111111111111111111",  2), // 0.11
        new Decimal("0.22222222222222222222222222222", 16), // 0.2222222222222222
                    "0.3322222222222222",              16,
    ],
    [
        new Decimal("5"), 10, "15", 28
    ],
    [
        new Decimal("5", 2), 10, "15", 28
    ],
    [
        new Decimal("5", 50), 10, "15", 50
    ],
    [
        $a = new Decimal("7", 50), $a, "14", 50
    ],
    [
        new Decimal(PHP_INT_MAX), new Decimal(PHP_INT_MIN), "-1", 28,
    ],

    /* Special numbers */
    [new Decimal( "NAN"),  "NAN", (string)  (NAN +  NAN), 28],
    [new Decimal( "NAN"),  "INF", (string)  (NAN +  INF), 28],
    [new Decimal( "NAN"), "-INF", (string)  (NAN + -INF), 28],
    [new Decimal( "INF"),  "NAN", (string)  (INF +  NAN), 28],
    [new Decimal( "INF"),  "INF", (string)  (INF +  INF), 28],
    [new Decimal( "INF"), "-INF", (string)  (INF + -INF), 28],
    [new Decimal("-INF"),  "NAN", (string) (-INF +  NAN), 28],
    [new Decimal("-INF"),  "INF", (string) (-INF +  INF), 28],
    [new Decimal("-INF"), "-INF", (string) (-INF + -INF), 28],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $precision) = $test;

    $results = [
        $op1->add($op2),

        $op1 + $op2,
        $op2 + $op1,
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
$number->add(5);
$number + 5;

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
