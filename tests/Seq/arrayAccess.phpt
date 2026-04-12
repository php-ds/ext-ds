--TEST--
Seq ArrayAccess - bracket syntax for get, set, isset, unset
--FILE--
<?php
// offsetGet: $s[index]
$s = new Ds\Seq(["a", "b", "c"]);
var_dump($s[0]);
var_dump($s[1]);
var_dump($s[2]);

// offsetSet: $s[index] = value
$s = new Ds\Seq([1, 2, 3]);
$s[1] = 20;
var_dump($s->toArray());

// offsetSet with null key: $s[] = value (append)
$s = new Ds\Seq([1, 2]);
$s[] = 3;
var_dump($s->toArray());

// offsetExists: isset($s[index])
$s = new Ds\Seq([1, null, 3]);
var_dump(isset($s[0]));
var_dump(isset($s[1])); // null: isset returns false
var_dump(isset($s[2]));
var_dump(isset($s[3])); // out of range
var_dump(isset($s[-1])); // negative

// offsetUnset: unset($s[index])
$s = new Ds\Seq(["a", "b", "c", "d"]);
unset($s[1]);
var_dump($s->toArray());

// Unset first
$s = new Ds\Seq([1, 2, 3]);
unset($s[0]);
var_dump($s->toArray());

// Unset last
$s = new Ds\Seq([1, 2, 3]);
unset($s[2]);
var_dump($s->toArray());

// offsetGet out of range
$s = new Ds\Seq([1, 2, 3]);
try {
    $v = $s[5];
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: get\n";
}

// offsetGet negative
try {
    $v = $s[-1];
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: get negative\n";
}

// offsetSet out of range
try {
    $s[5] = 99;
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: set\n";
}

// Append on empty seq
$s = new Ds\Seq();
$s[] = "first";
var_dump($s->toArray());

// Multiple appends
$s = new Ds\Seq();
$s[] = 1;
$s[] = 2;
$s[] = 3;
var_dump($s->toArray());
?>
--EXPECT--
string(1) "a"
string(1) "b"
string(1) "c"
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
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
bool(true)
bool(false)
bool(true)
bool(false)
bool(false)
array(3) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "c"
  [2]=>
  string(1) "d"
}
array(2) {
  [0]=>
  int(2)
  [1]=>
  int(3)
}
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}
OutOfRangeException: get
OutOfRangeException: get negative
OutOfRangeException: set
array(1) {
  [0]=>
  string(5) "first"
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
