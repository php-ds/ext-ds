--TEST--
Decimal cast to string, bool, int, float
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

    /**
     * STRING
     */
    [(string) decimal(),                "0"],
    [(string) decimal("-0"),           "-0"],
    [(string) decimal("5.2"),           "5.2"],

    [(string) decimal( "NAN"),          "NAN"],
    [(string) decimal( "INF"),          "INF"],
    [(string) decimal("-INF"),         "-INF"],

    /**
     * BOOL
     */
    [(bool) decimal(),                  true],
    [(bool) decimal( "0.1"),            true],
    [(bool) decimal("-0.1"),            true],

    [(bool) (decimal( "NAN")),          true],
    [(bool) (decimal( "INF")),          true],
    [(bool) (decimal("-INF")),          true],

    [(bool) decimal( "1E-1000"),        true],
    [(bool) decimal("-1E-1000"),        true],

    /**
     * INT
     */
    [(int) decimal(),                   0],
    [(int) decimal( "1E-1000"),         0],
    [(int) decimal("-1E-1000"),         0],

    [(int) decimal(PHP_INT_MAX),        PHP_INT_MAX],
    [(int) decimal(PHP_INT_MIN),        PHP_INT_MIN],

    [(int) decimal( NAN),               0],
    [(int) decimal( INF),               0],
    [(int) decimal(-INF),               0],

    /**
     * FLOAT
     */
    [(float) decimal(),                         0.0],
    [(float) decimal("-0"),                    -0.0],
    [(float) decimal("-0.0"),                  -0.0],
    [(float) decimal( "0.1"),                   0.1],
    [(float) decimal( "0.5"),                   0.5],

    [(float) decimal("0.11111111111111111111"), 0.111111111111111111], // Truncated

    [(float) decimal("1234.5678E+9"),           1.2345678E+12],
    [(float) decimal("1234.5678E+90"),          1.2345678E+93],

    [(string) (float) decimal( "NAN"),          "NAN"],
    [(string) (float) decimal( "INF"),          "INF"],
    [(string) (float) decimal("-INF"),         "-INF"],

    [(float) decimal( INF),  INF],
    [(float) decimal(-INF), -INF],

    /**
     * ARRAY
     */
    [(array) decimal(), []],
];

foreach ($tests as $test) {
    list($result, $expect) = $test;

    if ($result !== $expect) {
        print_r(compact("result", "expect"));
    }
}

try {
    (int) decimal("1E+1000");
} catch (OverflowException $e) {
    printf("%s\n", $e->getMessage());
}

try {
    (float) decimal("1E-1000");
} catch (UnderflowException $e) {
    printf("%s\n", $e->getMessage());
}

try {
    (float) decimal("-1E-1000");
} catch (UnderflowException $e) {
    printf("%s\n", $e->getMessage());
}

?>
--EXPECT--
Integer overflow
Floating point underflow
Floating point underflow
