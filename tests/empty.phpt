--TEST--
Decimal empty
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Decimal;

var_dump(empty(new Decimal()));
var_dump(empty(new Decimal(0)));
var_dump(empty(new Decimal(1)));

var_dump(empty(new Decimal( "1E-1000")));
var_dump(empty(new Decimal("-1E-1000")));

var_dump(empty(new Decimal( "NAN")));
var_dump(empty(new Decimal( "INF")));
var_dump(empty(new Decimal("-INF")));
?>
--EXPECT--
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
