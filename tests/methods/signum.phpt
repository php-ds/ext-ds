--TEST--
Decimal::signum
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

var_dump(decimal()->signum());

var_dump(decimal("1234.5678E+9")->signum());
var_dump(decimal("1234.5678E-9")->signum());

var_dump(decimal("-1234.5678E+9")->signum());
var_dump(decimal("-1234.5678E-9")->signum());

var_dump(decimal( "NAN")->signum());
var_dump(decimal( "INF")->signum());
var_dump(decimal("-INF")->signum());
?>
--EXPECT--
int(1)
int(1)
int(1)
int(-1)
int(-1)
int(1)
int(1)
int(-1)
