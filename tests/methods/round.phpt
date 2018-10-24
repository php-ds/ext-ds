--TEST--
Decimal::round
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

$tests = [
    /* number, decimal places, rounding mode(s), expected result */

    /* HALF UP, .5 AWAY FROM ZERO */
    [ "1.1", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "2"],
    [ "1.9", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-2"],
    ["-1.9", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-INF"],


    /* HALF DOWN, .5 TOWARDS ZERO */
    [ "1.1", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1"],
    [ "1.9", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1"],
    ["-1.9", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-INF"],


    /* HALF EVEN, .5 TO NEAREST EVEN NUMBER */
    [ "1.1", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "2"],
    [ "1.9", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-2"],
    ["-1.9", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-INF"],


    /* HALF ODD, .5 TO NEAREST ODD NUMBER */
    [ "1.1", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1"],
    [ "1.9", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1"],
    ["-1.9", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-INF"],


    /* UP, AWAY FROM ZERO */
    [ "1.1", 0, [Decimal::ROUND_UP],  "2"],
    [ "1.5", 0, [Decimal::ROUND_UP],  "2"],
    [ "1.9", 0, [Decimal::ROUND_UP],  "2"],
    ["-1.1", 0, [Decimal::ROUND_UP], "-2"],
    ["-1.5", 0, [Decimal::ROUND_UP], "-2"],
    ["-1.9", 0, [Decimal::ROUND_UP], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_UP],  "1.235"],
    [ "1.2345", 3, [Decimal::ROUND_UP],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_UP],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_UP], "-1.235"],
    ["-1.2345", 3, [Decimal::ROUND_UP], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_UP], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_UP],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_UP], "-1.235"],

    [ "0", 0, [Decimal::ROUND_UP],  "0"],
    [ "3", 0, [Decimal::ROUND_UP],  "3"],
    [ "4", 0, [Decimal::ROUND_UP],  "4"],
    ["-3", 0, [Decimal::ROUND_UP], "-3"],
    ["-4", 0, [Decimal::ROUND_UP], "-4"],

    [ "NAN", 0, [Decimal::ROUND_UP],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_UP],  "INF"],
    ["-INF", 0, [Decimal::ROUND_UP], "-INF"],


    // /* DOWN, TOWARDS ZERO */
    [ "1.1", 0, [Decimal::ROUND_DOWN],  "1"],
    [ "1.5", 0, [Decimal::ROUND_DOWN],  "1"],
    [ "1.9", 0, [Decimal::ROUND_DOWN],  "1"],
    ["-1.1", 0, [Decimal::ROUND_DOWN], "-1"],
    ["-1.5", 0, [Decimal::ROUND_DOWN], "-1"],
    ["-1.9", 0, [Decimal::ROUND_DOWN], "-1"],

    [ "1.2341", 3, [Decimal::ROUND_DOWN],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_DOWN],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.2341", 3, [Decimal::ROUND_DOWN], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_DOWN], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "1.23455", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "0", 0, [Decimal::ROUND_DOWN],  "0"],
    [ "3", 0, [Decimal::ROUND_DOWN],  "3"],
    [ "4", 0, [Decimal::ROUND_DOWN],  "4"],
    ["-3", 0, [Decimal::ROUND_DOWN], "-3"],
    ["-4", 0, [Decimal::ROUND_DOWN], "-4"],

    [ "NAN", 0, [Decimal::ROUND_DOWN],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_DOWN],  "INF"],
    ["-INF", 0, [Decimal::ROUND_DOWN], "-INF"],


    /* CEILING, TOWARDS +INF */
    [ "1.1", 0, [Decimal::ROUND_CEILING],  "2"],
    [ "1.5", 0, [Decimal::ROUND_CEILING],  "2"],
    [ "1.9", 0, [Decimal::ROUND_CEILING],  "2"],
    ["-1.1", 0, [Decimal::ROUND_CEILING], "-1"],
    ["-1.5", 0, [Decimal::ROUND_CEILING], "-1"],
    ["-1.9", 0, [Decimal::ROUND_CEILING], "-1"],

    [ "1.2341", 3, [Decimal::ROUND_CEILING],  "1.235"],
    [ "1.2345", 3, [Decimal::ROUND_CEILING],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_CEILING],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_CEILING], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_CEILING], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_CEILING], "-1.234"],

    [ "1.23455", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "0", 0, [Decimal::ROUND_CEILING],  "0"],
    [ "3", 0, [Decimal::ROUND_CEILING],  "3"],
    [ "4", 0, [Decimal::ROUND_CEILING],  "4"],
    ["-3", 0, [Decimal::ROUND_CEILING], "-3"],
    ["-4", 0, [Decimal::ROUND_CEILING], "-4"],

    [ "NAN", 0, [Decimal::ROUND_CEILING],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_CEILING],  "INF"],
    ["-INF", 0, [Decimal::ROUND_CEILING], "-INF"],


    /* FLOOR, TOWARDS -INF */
    [ "1.1", 0, [Decimal::ROUND_FLOOR],  "1"],
    [ "1.5", 0, [Decimal::ROUND_FLOOR],  "1"],
    [ "1.9", 0, [Decimal::ROUND_FLOOR],  "1"],
    ["-1.1", 0, [Decimal::ROUND_FLOOR], "-2"],
    ["-1.5", 0, [Decimal::ROUND_FLOOR], "-2"],
    ["-1.9", 0, [Decimal::ROUND_FLOOR], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_FLOOR],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_FLOOR],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_FLOOR],  "1.234"],
    ["-1.2341", 3, [Decimal::ROUND_FLOOR], "-1.235"],
    ["-1.2345", 3, [Decimal::ROUND_FLOOR], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_FLOOR], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "0", 0, [Decimal::ROUND_FLOOR],  "0"],
    [ "3", 0, [Decimal::ROUND_FLOOR],  "3"],
    [ "4", 0, [Decimal::ROUND_FLOOR],  "4"],
    ["-3", 0, [Decimal::ROUND_FLOOR], "-3"],
    ["-4", 0, [Decimal::ROUND_FLOOR], "-4"],

    [ "NAN", 0, [Decimal::ROUND_FLOOR],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_FLOOR],  "INF"],
    ["-INF", 0, [Decimal::ROUND_FLOOR], "-INF"],


    /* TRUNCATE */
    [ "1.1", 0, [Decimal::ROUND_TRUNCATE],  "1"],
    [ "1.5", 0, [Decimal::ROUND_TRUNCATE],  "1"],
    [ "1.9", 0, [Decimal::ROUND_TRUNCATE],  "1"],
    ["-1.1", 0, [Decimal::ROUND_TRUNCATE], "-1"],
    ["-1.5", 0, [Decimal::ROUND_TRUNCATE], "-1"],
    ["-1.9", 0, [Decimal::ROUND_TRUNCATE], "-1"],

    [ "1.2341", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    ["-1.2341", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],

    [ "1.23455", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],

    [ "0", 0, [Decimal::ROUND_TRUNCATE],  "0"],
    [ "3", 0, [Decimal::ROUND_TRUNCATE],  "3"],
    [ "4", 0, [Decimal::ROUND_TRUNCATE],  "4"],
    ["-3", 0, [Decimal::ROUND_TRUNCATE], "-3"],
    ["-4", 0, [Decimal::ROUND_TRUNCATE], "-4"],

    [ "NAN", 0, [Decimal::ROUND_TRUNCATE],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_TRUNCATE],  "INF"],
    ["-INF", 0, [Decimal::ROUND_TRUNCATE], "-INF"],

    /* ROUND NEGATIVE FIGURES */
    ["1.2345", -3, [Decimal::ROUND_HALF_UP], "0E+3"],
    ["12345",  -3, [Decimal::ROUND_HALF_UP], "1.2E+4"],

    /* ROUND TO CURRENT SIGNIFICANT FIGURES */
    ["0.12345", 5, [Decimal::ROUND_HALF_UP], "0.12345"],
    ["1.2345",  5, [Decimal::ROUND_HALF_UP], "1.2345"],
    ["12.345",  5, [Decimal::ROUND_HALF_UP], "12.345"],
    ["123.45",  5, [Decimal::ROUND_HALF_UP], "123.45"],
    ["1234.5",  5, [Decimal::ROUND_HALF_UP], "1234.5"],
    ["12345",   5, [Decimal::ROUND_HALF_UP], "12345"],

    /* ROUND BEYOND CURRENT SIGNIFICANT FIGURES */
    ["0.12345", 10, [Decimal::ROUND_HALF_UP], "0.12345"],
    ["1.2345",  10, [Decimal::ROUND_HALF_UP], "1.2345"],
    ["12.345",  10, [Decimal::ROUND_HALF_UP], "12.345"],
    ["123.45",  10, [Decimal::ROUND_HALF_UP], "123.45"],
    ["1234.5",  10, [Decimal::ROUND_HALF_UP], "1234.5"],
    ["12345",   10, [Decimal::ROUND_HALF_UP], "12345"],

    /* ROUND BEYOND PRECISION */
    ["0.12345", 10, [Decimal::ROUND_HALF_UP], "0.12345"],
    ["1.2345",  10, [Decimal::ROUND_HALF_UP], "1.2345"],
    ["12.345",  10, [Decimal::ROUND_HALF_UP], "12.345"],
    ["123.45",  10, [Decimal::ROUND_HALF_UP], "123.45"],
    ["1234.5",  10, [Decimal::ROUND_HALF_UP], "1234.5"],
    ["12345",   10, [Decimal::ROUND_HALF_UP], "12345"],
];

foreach ($tests as $test) {
    list($number, $places, $modes, $expect) = $test;

    /* Check to see if there is an equivalent PHP constant */
    foreach ($modes as $mode) {
        $result = (string) (new Decimal($number))->round($places, $mode);

        if ($result !== $expect) {
            print_r(compact("number", "places", "mode", "result", "expect"));
        }
    }
}

/* Test that round does not modify the original */
$number = new Decimal("1.2345");
$result = $number->round();

if ((string) $number !== "1.2345") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
