--TEST--
Seq::reduce - reduce to a single value
--FILE--
<?php
// Sum with initial value
$s = new Ds\Seq([1, 2, 3, 4]);
$result = $s->reduce(function($carry, $value) {
    return $carry + $value;
}, 0);
var_dump($result);

// Product with initial value
$s = new Ds\Seq([1, 2, 3, 4]);
$result = $s->reduce(function($carry, $value) {
    return $carry * $value;
}, 1);
var_dump($result);

// Without initial value (defaults to null)
$s = new Ds\Seq([1, 2, 3]);
$result = $s->reduce(function($carry, $value) {
    return ($carry ?? 0) + $value;
});
var_dump($result);

// String concatenation
$s = new Ds\Seq(["a", "b", "c"]);
$result = $s->reduce(function($carry, $value) {
    return $carry . $value;
}, "");
var_dump($result);

// Empty seq with initial
$s = new Ds\Seq();
$result = $s->reduce(function($carry, $value) {
    return $carry + $value;
}, 42);
var_dump($result);

// Empty seq without initial returns null
$s = new Ds\Seq();
$result = $s->reduce(function($carry, $value) {
    return $carry + $value;
});
var_dump($result);

// Single element
$s = new Ds\Seq([10]);
$result = $s->reduce(function($carry, $value) {
    return $carry + $value;
}, 5);
var_dump($result);

// Original unchanged
$s = new Ds\Seq([1, 2, 3]);
$s->reduce(function($carry, $value) { return $carry + $value; }, 0);
var_dump($s->toArray());
?>
--EXPECT--
int(10)
int(24)
int(6)
string(3) "abc"
int(42)
NULL
int(15)
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
