--TEST--
Decimal constants
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Decimal;

var_dump(Decimal::DEFAULT_PRECISION);
var_dump(Decimal::DEFAULT_ROUNDING);

var_dump(Decimal::MIN_PRECISION);
var_dump(Decimal::MAX_PRECISION);

var_dump(Decimal::ROUND_UP);
var_dump(Decimal::ROUND_DOWN);
var_dump(Decimal::ROUND_CEILING);
var_dump(Decimal::ROUND_FLOOR);
var_dump(Decimal::ROUND_HALF_UP);
var_dump(Decimal::ROUND_HALF_DOWN);
var_dump(Decimal::ROUND_HALF_EVEN);
var_dump(Decimal::ROUND_HALF_ODD);
?>
--EXPECTF--
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
int(%d)
