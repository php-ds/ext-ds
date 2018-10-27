--TEST--
Decimal::__construct
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

$tests = [

    /**
     * Constructor args, expected precision, expected value
     *
     * Decimal::__construct(value [, precision])
     */
    [[], 28, "0"],

    [[123,                   /* 28 */ ],    28,     "123"],
    [["0.2000",              /* 28 */ ],    28,     "0.2000"],
    [["1E+900",              /* 28 */ ],    28,     "1E+900"],
    [[new Decimal("0.1"),    /* 28 */ ],    28,     "0.1"],

    [[123,                          10],    10,     "123"],
    [["0.2000",                     10],    10,     "0.2000"],
    [["1E+900",                     10],    10,     "1E+900"],
    [[new Decimal("0.1"),           10],    28,     "0.1"],

    [[150,                           1],     1,     "2E+2"], // Warning
    [["0.2000",                      1],     1,     "0.2"],  // No warning, result is exact
    [["0.20001",                     1],     1,     "0.2"],  // Warning, data lost
    [["1E+900",                      1],     1,     "1E+900"],
    [[new Decimal("0.1"),            1],    28,     "0.1"],

    /* Precision is considered a minimum, to preserve precision of decimals. */
    [[new Decimal("0.12345", 5),     1],     5,     "0.12345"],

    /* Use given decimal precision if greater than default. */
    [[new Decimal("0.1234", 50), /* 28 */], 50,     "0.1234"],

    [[ "nan"],  28,  "NAN"],
    [[ "inf"],  28,  "INF"],
    [["-inf"],  28, "-INF"],

    /* While float is not actually supported, these constants are an exception
       for the sake of developer sanity and convenience. */
    [[ NAN],  28,  "NAN"],
    [[ INF],  28,  "INF"],
    [[-INF],  28, "-INF"],

    /* Check precision bounds */
    [[0, Decimal::MAX_PRECISION], Decimal::MAX_PRECISION, "0"],
    [[0, Decimal::MIN_PRECISION], Decimal::MIN_PRECISION, "0"],
];

foreach ($tests as $index => $test) {
    list($args, $precision, $expect) = $test;

    $result = new Decimal(...$args);

    if ($result->precision() !== $precision || (string) $result !== $expect) {
        var_dump(compact("index", "args", "result", "precision", "expect"));
    }
}


try {
    new Decimal(" 1");
} catch (DomainException $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    new Decimal("1 ");
} catch (DomainException $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    new Decimal(1.5);
} catch (TypeError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    new Decimal(null);
} catch (TypeError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    new Decimal(0, "b");
} catch (TypeError $e) {
    printf("E %s\n", $e->getMessage());
}

try {
    new Decimal(0, null);
} catch (TypeError $e) {
    printf("F %s\n", $e->getMessage());
}

try {
    new Decimal(0, 0);
} catch (OutOfRangeException $e) {
    printf("G %s\n", $e->getMessage());
}

try {
    new Decimal(0, -1);
} catch (OutOfRangeException $e) {
    printf("H %s\n", $e->getMessage());
}

/* Check max precision */
try {
    new Decimal(0, Decimal::MAX_PRECISION + 1);
} catch (OutOfRangeException $e) {
    printf("I %s\n", $e->getMessage());
}

try {
    (new Decimal())->__construct();
} catch (BadMethodCallException $e) {
    printf("J %s\n", $e->getMessage());
}

?>
--EXPECTF--
Warning: Loss of data on integer conversion in %s on line %d

Warning: Loss of data on string conversion in %s on line %d
A Failed to parse string as decimal: " 1"
B Failed to parse string as decimal: "1 "
C Decimal\Decimal::__construct() expected parameter 1 to be a string, integer, or decimal, float given
D Decimal\Decimal::__construct() expected parameter 1 to be a string, integer, or decimal, null given
E Argument 2 passed to Decimal\Decimal::__construct() must be of the type int%s string given
F Argument 2 passed to Decimal\Decimal::__construct() must be of the type int%s null given
G Decimal precision out of range
H Decimal precision out of range
I Decimal precision out of range
J Decimal objects are immutable
