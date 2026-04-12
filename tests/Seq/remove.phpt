--TEST--
Seq::remove - remove and return element at index
--FILE--
<?php
// Remove from middle
$s = new Ds\Seq(["a", "b", "c", "d"]);
var_dump($s->remove(1));
var_dump($s->toArray());

// Remove first
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->remove(0));
var_dump($s->toArray());

// Remove last
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->remove(2));
var_dump($s->toArray());

// Remove only element
$s = new Ds\Seq([42]);
var_dump($s->remove(0));
var_dump($s->isEmpty());

// Negative index throws
$s = new Ds\Seq([1, 2, 3]);
try {
    $s->remove(-1);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: negative\n";
}

// Index equal to count throws
$s = new Ds\Seq([1, 2, 3]);
try {
    $s->remove(3);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: equal to count\n";
}

// Empty seq throws
$s = new Ds\Seq();
try {
    $s->remove(0);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: empty\n";
}

// Remove preserves order
$s = new Ds\Seq([1, 2, 3, 4, 5]);
$s->remove(2); // remove 3
var_dump($s->toArray());
?>
--EXPECT--
string(1) "b"
array(3) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "c"
  [2]=>
  string(1) "d"
}
int(1)
array(2) {
  [0]=>
  int(2)
  [1]=>
  int(3)
}
int(3)
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}
int(42)
bool(true)
OutOfRangeException: negative
OutOfRangeException: equal to count
OutOfRangeException: empty
array(4) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(4)
  [3]=>
  int(5)
}
