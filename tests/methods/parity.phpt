--TEST--
Decimal::parity
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

var_dump((string) decimal()->parity());

var_dump((string) decimal("0")->parity());
var_dump((string) decimal("1")->parity());
var_dump((string) decimal("2")->parity());
var_dump((string) decimal("3")->parity());

var_dump((string) decimal("-0")->parity());
var_dump((string) decimal("-1")->parity());
var_dump((string) decimal("-2")->parity());
var_dump((string) decimal("-3")->parity());

var_dump((string) decimal("0.5")->parity());
var_dump((string) decimal("1.5")->parity());
var_dump((string) decimal("2.5")->parity());
var_dump((string) decimal("3.5")->parity());

var_dump((string) decimal( "NAN")->parity());
var_dump((string) decimal( "INF")->parity());
var_dump((string) decimal("-INF")->parity());

?>
--EXPECT--
string(1) "0"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "1"
string(1) "1"
string(1) "1"
