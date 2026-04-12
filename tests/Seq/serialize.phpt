--TEST--
Seq::serialize/unserialize - round-trip serialization
--FILE--
<?php
// Basic round-trip
$s = new Ds\Seq([1, 2, 3]);
$serialized = serialize($s);
$unserialized = unserialize($serialized);
var_dump($unserialized->toArray());
echo get_class($unserialized) . "\n";

// Empty seq
$s = new Ds\Seq();
$unserialized = unserialize(serialize($s));
var_dump($unserialized->toArray());
var_dump($unserialized->isEmpty());

// Mixed types
$s = new Ds\Seq([1, "two", 3.5, null, true, false]);
$unserialized = unserialize(serialize($s));
var_dump($unserialized->toArray());

// Preserves count
$s = new Ds\Seq([1, 2, 3, 4, 5]);
$unserialized = unserialize(serialize($s));
var_dump($unserialized->count());

// Unserialized is independent
$s = new Ds\Seq([1, 2, 3]);
$u = unserialize(serialize($s));
$u->push(4);
var_dump($s->count());
var_dump($u->count());

// Objects survive serialization
$obj = new stdClass();
$obj->name = "test";
$s = new Ds\Seq([$obj]);
$u = unserialize(serialize($s));
var_dump($u->get(0)->name);

// __serialize and __unserialize
$s = new Ds\Seq([10, 20, 30]);
$data = $s->__serialize();
var_dump($data);

$s2 = new Ds\Seq();
$s2->__unserialize([40, 50, 60]);
var_dump($s2->toArray());
?>
--EXPECT--
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
Ds\Seq
array(0) {
}
bool(true)
array(6) {
  [0]=>
  int(1)
  [1]=>
  string(3) "two"
  [2]=>
  float(3.5)
  [3]=>
  NULL
  [4]=>
  bool(true)
  [5]=>
  bool(false)
}
int(5)
int(3)
int(4)
string(4) "test"
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
  int(40)
  [1]=>
  int(50)
  [2]=>
  int(60)
}
