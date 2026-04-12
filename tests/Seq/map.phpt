--TEST--
Seq::map - return new Seq with transformed values
--FILE--
<?php
// Basic map
$s = new Ds\Seq([1, 2, 3]);
$mapped = $s->map(function($v) { return $v * 10; });
var_dump($mapped->toArray());

// Original unchanged
var_dump($s->toArray());

// Map returns Ds\Seq
echo get_class($mapped) . "\n";

// Map changing types
$s = new Ds\Seq([1, 2, 3]);
$mapped = $s->map(function($v) { return "v" . $v; });
var_dump($mapped->toArray());

// Empty seq
$s = new Ds\Seq();
$mapped = $s->map(function($v) { return $v; });
var_dump($mapped->toArray());

// Single element
$s = new Ds\Seq([42]);
$mapped = $s->map(function($v) { return $v * 2; });
var_dump($mapped->toArray());

// Map to constant
$s = new Ds\Seq([1, 2, 3, 4]);
$mapped = $s->map(function($v) { return null; });
var_dump($mapped->toArray());
?>
--EXPECT--
array(3) {
  [0]=>
  int(10)
  [1]=>
  int(20)
  [2]=>
  int(30)
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
Ds\Seq
array(3) {
  [0]=>
  string(2) "v1"
  [1]=>
  string(2) "v2"
  [2]=>
  string(2) "v3"
}
array(0) {
}
array(1) {
  [0]=>
  int(84)
}
array(4) {
  [0]=>
  NULL
  [1]=>
  NULL
  [2]=>
  NULL
  [3]=>
  NULL
}
