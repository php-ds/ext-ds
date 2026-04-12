--TEST--
Pair::__debugInfo - var_dump output format
--FILE--
<?php
// Basic var_dump
$p = new Ds\Pair("hello", 42);
var_dump($p);

// With null values
$p2 = new Ds\Pair(null, null);
var_dump($p2);

// With array value
$p3 = new Ds\Pair("data", [1, 2]);
var_dump($p3);

// With boolean
$p4 = new Ds\Pair(true, false);
var_dump($p4);
?>
--EXPECTF--
object(Ds\Pair)#%d (2) {
  ["key"]=>
  string(5) "hello"
  ["value"]=>
  int(42)
}
object(Ds\Pair)#%d (2) {
  ["key"]=>
  NULL
  ["value"]=>
  NULL
}
object(Ds\Pair)#%d (2) {
  ["key"]=>
  string(4) "data"
  ["value"]=>
  array(2) {
    [0]=>
    int(1)
    [1]=>
    int(2)
  }
}
object(Ds\Pair)#%d (2) {
  ["key"]=>
  bool(true)
  ["value"]=>
  bool(false)
}
