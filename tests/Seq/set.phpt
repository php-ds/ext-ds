--TEST--
Seq::set - set value at index
--FILE--
<?php
// Basic set
$s = new Ds\Seq([1, 2, 3]);
$s->set(1, 20);
var_dump($s->toArray());

// Set first
$s = new Ds\Seq(["a", "b", "c"]);
$s->set(0, "x");
var_dump($s->toArray());

// Set last
$s = new Ds\Seq(["a", "b", "c"]);
$s->set(2, "z");
var_dump($s->toArray());

// Set to different type
$s = new Ds\Seq([1, 2, 3]);
$s->set(1, "hello");
var_dump($s->get(1));

// Set to null
$s = new Ds\Seq([1, 2, 3]);
$s->set(0, null);
var_dump($s->get(0));

// Returns void
$s = new Ds\Seq([1, 2, 3]);
$result = $s->set(0, 10);
var_dump($result);

// Negative index throws
$s = new Ds\Seq([1, 2, 3]);
try {
    $s->set(-1, 0);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: negative\n";
}

// Index equal to count throws
try {
    $s->set(3, 0);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: equal to count\n";
}

// Empty seq throws
$s = new Ds\Seq();
try {
    $s->set(0, 1);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: empty\n";
}

// Does not change count
$s = new Ds\Seq([1, 2, 3]);
$s->set(1, 99);
var_dump($s->count());
?>
--EXPECT--
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(20)
  [2]=>
  int(3)
}
array(3) {
  [0]=>
  string(1) "x"
  [1]=>
  string(1) "b"
  [2]=>
  string(1) "c"
}
array(3) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
  [2]=>
  string(1) "z"
}
string(5) "hello"
NULL
NULL
OutOfRangeException: negative
OutOfRangeException: equal to count
OutOfRangeException: empty
int(3)
