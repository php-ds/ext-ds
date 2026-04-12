--TEST--
Seq::insert - insert values at index
--FILE--
<?php
// Insert in the middle
$s = new Ds\Seq([1, 2, 5]);
$s->insert(2, 3, 4);
var_dump($s->toArray());

// Insert at beginning
$s = new Ds\Seq([2, 3]);
$s->insert(0, 1);
var_dump($s->toArray());

// Insert at end (index === count is valid for append)
$s = new Ds\Seq([1, 2]);
$s->insert(2, 3);
var_dump($s->toArray());

// Insert multiple values at once
$s = new Ds\Seq([1, 5]);
$s->insert(1, 2, 3, 4);
var_dump($s->toArray());

// Insert single value
$s = new Ds\Seq(["a", "c"]);
$s->insert(1, "b");
var_dump($s->toArray());

// Invalid index: negative
$s = new Ds\Seq([1, 2, 3]);
try {
    $s->insert(-1, 0);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: negative\n";
}

// Invalid index: greater than count
$s = new Ds\Seq([1, 2, 3]);
try {
    $s->insert(4, 0);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: beyond count\n";
}

// Insert into empty at index 0
$s = new Ds\Seq();
$s->insert(0, 1, 2, 3);
var_dump($s->toArray());
?>
--EXPECT--
array(5) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
  [4]=>
  int(5)
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(5) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
  [4]=>
  int(5)
}
array(3) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
  [2]=>
  string(1) "c"
}
OutOfRangeException: negative
OutOfRangeException: beyond count
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
