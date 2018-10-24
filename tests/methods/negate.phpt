--TEST--
Decimal::negate
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
if (!extension_loaded("json"))    echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * Shortcut to construct a new decimal.
 */
function decimal(...$args) { return new Decimal(...$args); }

var_dump((string) decimal()->negate());

var_dump((string) decimal("-0")->negate());
var_dump((string) decimal( "0")->negate());
var_dump((string) decimal( "1")->negate());
var_dump((string) decimal("-1")->negate());

var_dump((string) decimal( "NAN")->negate());
var_dump((string) decimal( "INF")->negate());
var_dump((string) decimal("-INF")->negate());

/**
 * Check that negate does not modify the original
 */
$obj = decimal("1.234");
$obj->negate();
var_dump((string) $obj);

?>
--EXPECT--
string(2) "-0"
string(1) "0"
string(2) "-0"
string(2) "-1"
string(1) "1"
string(3) "NAN"
string(4) "-INF"
string(3) "INF"
string(5) "1.234"
