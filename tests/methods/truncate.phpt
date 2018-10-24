--TEST--
Decimal::truncate
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

var_dump((string) decimal()->truncate());

var_dump((string) decimal("1234.5678")->truncate());
var_dump((string) decimal("1E-500")->truncate());

var_dump((string) decimal( "NAN")->truncate());
var_dump((string) decimal( "INF")->truncate());
var_dump((string) decimal("-INF")->truncate());

/**
 * Check that truncate does not modify the original
 */
$obj = decimal("1.234");
$obj->truncate();
var_dump((string) $obj);

?>
--EXPECT--
string(1) "0"
string(4) "1234"
string(1) "0"
string(3) "NAN"
string(3) "INF"
string(4) "-INF"
string(5) "1.234"
