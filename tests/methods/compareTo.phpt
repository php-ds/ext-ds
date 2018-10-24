--TEST--
Decimal::compareTo
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

$decimal = decimal("1234.5678E+9");

$tests = [

    /**
     * METHOD Decimal::compareTo
     *
     * op1, op2, expected result.
     */
    [$decimal, $decimal, 0],

    [decimal(),     0,      0],
    [decimal(),     null,   1],
    [decimal(),     false,  1],
    [decimal(),     0.0,    0],

    [decimal(1),    0,      1],
    [decimal(1),    null,   1],
    [decimal(1),    false,  1],
    [decimal(1),    0.0,    1],

    [decimal(NAN),  0,      1],
    [decimal(NAN),  null,   1],
    [decimal(NAN),  false,  1],
    [decimal(NAN),  0.0,    1],

    [decimal(),     1,     -1],
    [decimal(),     true,   0], // Objects are always equal to true.
    [decimal(),     1.0,   -1],
    [decimal(),     "1",   -1],

    [decimal(1),    1,      0],
    [decimal(1),    true,   0], // Objects are always equal to true.
    [decimal(1),    1.0,    0],
    [decimal(1),    "1",    0],

    [decimal(NAN),  1,      1],
    [decimal(NAN),  true,   0], // Objects are always equal to true.
    [decimal(NAN),  1.0,    1],
    [decimal(NAN),  "1",    1],

    [decimal(0),    NAN,    1],
    [decimal(1),    NAN,    1],

    [decimal("0.1"),     0.1,   0],
    [decimal("0.2"),     0.2,   0],
    [decimal("0.200"),   0.2,   0],
    [decimal("0.200"), 0.200,   0],
    [decimal("1E-10"), 1E-10,   0],
    [decimal("1E+10"), 1E+10,   0],

    [decimal("0.1000000000000000000001"), 0.1,   1],
    [decimal("0.2000000000000000000001"), 0.2,   1],

    [decimal("0.0000000000000000000001"), 0.1,  -1],
    [decimal("0.0000000000000000000001"), 0.0,   1],

    [decimal("0.123", 3), "0.123",   0],
    [decimal("0.123", 3), "0.1234", -1], // Precision of op1 not used
    [decimal("0.123", 3), "0.12  ",  1],

    [decimal("0.123", 3), decimal("0.1234", 3),  0], // Warning, lost data
    [decimal("0.123", 3), decimal("0.1234", 4), -1],

    /* Parse beyond the default precision. Should use max prec, not default.
       0.123
       0.12300...001
                  ^default prec cutoff
    */
    [decimal("0.123", 3), "0.123" . str_repeat("0", Decimal::DEFAULT_PRECISION) . "1", -1],

    [decimal("-0.123", 3), "-0.122", -1],
    [decimal("-0.123", 3), "-0.123",  0],
    [decimal("-0.123", 3), "-0.124",  1], // Numeric value, not lexicographic

    [decimal("-0.1"),     -0.1, 0],
    [decimal("-0.2"),     -0.2, 0],
    [decimal("-1E-10"), -1E-10, 0],
    [decimal("-1E+10"), -1E+10, 0],

    [decimal("0.1"),     "0.01",      1],
    [decimal("0.1"),     "0.1",       0],
    [decimal("0.1"),     "0.2",      -1],
    [decimal("0.2"),     "0.1",       1],
    [decimal("0.01"),    "0.1",      -1],

    [decimal("0.1"),     decimal("0.01"),      1],
    [decimal("0.1"),     decimal("0.1"),       0],
    [decimal("0.1"),     decimal("0.2"),      -1],
    [decimal("0.2"),     decimal("0.1"),       1],
    [decimal("0.01"),    decimal("0.1"),      -1],

    [decimal("0.1", 2),  decimal("0.1", 1),    0],
    [decimal("0.1", 2),  decimal("0.1", 2),    0],
    [decimal("0.1", 2),  decimal("0.1", 3),    0],

    [decimal( "INF"),    decimal( "NAN"),      INF <=>  NAN],
    [decimal( "INF"),    decimal( "INF"),      INF <=>  INF],
    [decimal( "INF"),    decimal("-INF"),      INF <=> -INF],
    [decimal("-INF"),    decimal( "NAN"),     -INF <=>  NAN],
    [decimal("-INF"),    decimal( "INF"),     -INF <=>  INF],
    [decimal("-INF"),    decimal("-INF"),     -INF <=> -INF],
    [decimal( "NAN"),    decimal( "NAN"),      NAN <=>  NAN],
    [decimal( "NAN"),    decimal( "INF"),      NAN <=>  INF],
    [decimal( "NAN"),    decimal("-INF"),      NAN <=> -INF],

    [decimal( "INF"),    NAN,                  INF <=>  NAN],
    [decimal( "INF"),    INF,                  INF <=>  INF],
    [decimal( "INF"),   -INF,                  INF <=> -INF],
    [decimal("-INF"),    NAN,                 -INF <=>  NAN],
    [decimal("-INF"),    INF,                 -INF <=>  INF],
    [decimal("-INF"),   -INF,                 -INF <=> -INF],
    [decimal( "NAN"),    NAN,                  NAN <=>  NAN],
    [decimal( "NAN"),    INF,                  NAN <=>  INF],
    [decimal( "NAN"),   -INF,                  NAN <=> -INF],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->compareTo($op2),
        $op1 <=> $op2,
    ];

    foreach ($results as $result) {
        if ($result !== $expect) {
            var_dump(compact("index", "op1", "op2", "result", "expect"));
            break;
        }
    }
}

/**
 * Check throws for undefined comparison
 */
try {
    decimal() <=> "abc";
} catch (DomainException $e) {
    printf("%s\n", $e->getMessage());
}

try {
    decimal() <=> "";
} catch (DomainException $e) {
    printf("%s\n", $e->getMessage());
}
?>
--EXPECTF--
Warning: Loss of data on string conversion in %s on line %d
