--TEST--
Decimal::precision
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

var_dump(decimal()->precision());

var_dump(decimal(5)->precision());
var_dump(decimal(5, 5)->precision());

decimal(0, 0);
?>
--EXPECTF--
int(28)
int(28)
int(5)

Fatal error: Uncaught OutOfRangeException: Decimal precision out of range: 1 <= P <= %d, 0 given in %s:%d
Stack trace:
#0 %s(%d): Decimal\Decimal->__construct(0, 0)
#1 %s(%d): decimal(0, 0)
#2 {main}
  thrown in %s on line %d
