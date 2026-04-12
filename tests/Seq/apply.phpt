--TEST--
Seq::apply - transform values in-place
--FILE--
<?php
// Basic apply: double each value
$s = new Ds\Seq([1, 2, 3, 4]);
$s->apply(function($value) {
    return $value * 2;
});
var_dump($s->toArray());

// Apply with strings
$s = new Ds\Seq(["hello", "world"]);
$s->apply(function($value) {
    return strtoupper($value);
});
var_dump($s->toArray());

// Apply on empty seq
$s = new Ds\Seq();
$s->apply(function($value) {
    return $value * 2;
});
var_dump($s->toArray());

// Apply changes type
$s = new Ds\Seq([1, 2, 3]);
$s->apply(function($value) {
    return "v" . $value;
});
var_dump($s->toArray());

// Apply with single element
$s = new Ds\Seq([42]);
$s->apply(function($value) {
    return $value + 8;
});
var_dump($s->toArray());
?>
--EXPECT--
array(4) {
  [0]=>
  int(2)
  [1]=>
  int(4)
  [2]=>
  int(6)
  [3]=>
  int(8)
}
array(2) {
  [0]=>
  string(5) "HELLO"
  [1]=>
  string(5) "WORLD"
}
array(0) {
}
array(3) {
  [0]=>
  string(2) "v1"
  [1]=>
  string(2) "v2"
  [2]=>
  string(2) "v3"
}
array(1) {
  [0]=>
  int(50)
}
