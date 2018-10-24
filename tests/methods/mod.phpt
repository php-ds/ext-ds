--TEST--
Decimal::mod
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

/**
 * OP1, OP2, Expected Result, Expected Precision
 */
$tests = [
    [decimal("0"),        "1",    0 %  1,    28],
    [decimal("0"),       "-1",    0 % -1,    28],

    [decimal( "1"),       "3",    1 %  3,    28],
    [decimal( "1"),      "-3",    1 % -3,    28],
    [decimal("-1"),      "-3",   -1 % -3,    28],
    [decimal("-1"),      "-3",   -1 % -3,    28],

    [decimal( "7"),       "4",    7 %  4,    28],
    [decimal( "7"),      "-4",    7 % -4,    28],
    [decimal("-7"),       "4",   -7 %  4,    28],
    [decimal("-7"),      "-4",   -7 % -4,    28],

    /* mod is an integer operation*/
    [decimal("5.678"),   "1.234",    "0",    28],
    [decimal("5.678"),   "2.234",    "1",    28],
    [decimal("5.678"),   "3.234",    "2",    28],

    [decimal("5"),       "1.5",      "0",    28], // 5 % 1
    [decimal("5"),       "2.5",      "1",    28], // 5 % 2
    [decimal("5"),       "3.5",      "2",    28], // 5 % 3

    [
        decimal("5.678", 5),
        decimal("3.234", 5),
                "2",     5
    ],

    [decimal( "NAN"),  "NAN",   "NAN", 28],
    [decimal( "NAN"),  "INF",   "NAN", 28],
    [decimal( "NAN"), "-INF",   "NAN", 28],
    [decimal( "INF"),  "NAN",   "NAN", 28],
    [decimal( "INF"),  "INF",   "NAN", 28], // <-- No exception, technically invalid.
    [decimal( "INF"), "-INF",   "NAN", 28], // <-- No exception, technically invalid.
    [decimal("-INF"),  "NAN",   "NAN", 28],
    [decimal("-INF"),  "INF",   "NAN", 28], // <-- No exception, technically invalid.
    [decimal("-INF"), "-INF",   "NAN", 28], // <-- No exception, technically invalid.

    [decimal( "NAN"),  1,       "NAN", 28],
    [decimal( "INF"),  1,       "NAN", 28], // <-- No exception, technically invalid.
    [decimal("-INF"),  1,       "NAN", 28], // <-- No exception, technically invalid.
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $precision) = $test;

    $results = [
        $op1->mod($op2),
        $op1 % $op2,
    ];

    foreach ($results as $result) {
        if ([(string) $result, $result->precision()] !== [(string) $expect, $precision]) {
            print_r(compact("index", "op1", "op2", "result", "expect", "precision"));
            break;
        }
    }
}

/* Test that op does not modify the original */
$number = decimal("2");
$number->mod(5);
$number % 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/* Check division by zero */
try {
    decimal() % 0;
} catch (DivisionByZeroError $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    decimal(1) % 0;
} catch (DivisionByZeroError $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    decimal(NAN) % 0;
} catch (DivisionByZeroError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    decimal(INF) % 0;
} catch (DivisionByZeroError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    decimal(-INF) % 0;
} catch (DivisionByZeroError $e) {
    printf("E %s\n", $e->getMessage());
}

?>
--EXPECT--
A Division by zero
B Division by zero
C Division by zero
D Division by zero
E Division by zero
