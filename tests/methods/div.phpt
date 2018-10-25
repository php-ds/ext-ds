--TEST--
Decimal::div
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
                    "0.1249999988609375000142382813",   28
    ],
    [
        new Decimal("1.2345678901234567890123456789",   20), // 1.234567890123456789
        new Decimal("9.8765432109876543210987654321",   12), // 9.87654321099
                    "0.12499999886090781251",           20
    ],
    [
        new Decimal("1.2345678901234567890123456789",    5), // 1.2346
        new Decimal("9.8765432109876543210987654321",   20), // 9.8765432109876543211
                    "0.12500324998593713438",           20
    ],
    [
        new Decimal("1.1111",    1), // 1
        new Decimal("3.3333",    2), // 3.3
                    "0.30",      2
    ],
    [
        new Decimal("0.3" /* 28 */),
        new Decimal("0.1" /* 28 */),
                    "3",     28
    ],
    [
        new Decimal("-0.3" /* 28 */),
        new Decimal( "0.1" /* 28 */),
                    "-3",     28
    ],
    [
        new Decimal("15"), 10, "1.5", 28
    ],
    [
        new Decimal("15", 2), 10, "1.5", 2
    ],
    [
        new Decimal("15", 50), 10, "1.5", 50
    ],
    [
        $a = new Decimal("7", 50), $a, "1", 50
    ],

    /* Special numbers */
    [new Decimal( "NAN"),  "NAN", (string)  (NAN /  NAN), 28],
    [new Decimal( "NAN"),  "INF", (string)  (NAN /  INF), 28],
    [new Decimal( "NAN"), "-INF", (string)  (NAN / -INF), 28],
    [new Decimal( "INF"),  "NAN", (string)  (INF /  NAN), 28],
    [new Decimal( "INF"),  "INF", (string)  (INF /  INF), 28],
    [new Decimal( "INF"), "-INF", (string)  (INF / -INF), 28],
    [new Decimal("-INF"),  "NAN", (string) (-INF /  NAN), 28],
    [new Decimal("-INF"),  "INF", (string) (-INF /  INF), 28],
    [new Decimal("-INF"), "-INF", (string) (-INF / -INF), 28],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $precision) = $test;

    $results = [
        $op1->div($op2),
        $op1 / $op2,
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
$number->div(5);
$number / 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/* Check division by zero */
try {
    new Decimal() / 0;
} catch (DivisionByZeroError $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    new Decimal(1) / 0;
} catch (DivisionByZeroError $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    new Decimal(NAN) / 0;
} catch (DivisionByZeroError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    new Decimal(INF) / 0;
} catch (DivisionByZeroError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    new Decimal(-INF) / 0;
} catch (DivisionByZeroError $e) {
    printf("E %s\n", $e->getMessage());
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
A Division by zero
B Division by zero
C Division by zero
D Division by zero
E Division by zero
