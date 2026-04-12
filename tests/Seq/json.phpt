--TEST--
Seq::jsonSerialize - JSON encoding
--FILE--
<?php
// Basic JSON encode
$s = new Ds\Seq([1, 2, 3]);
echo json_encode($s) . "\n";

// Empty seq
$s = new Ds\Seq();
echo json_encode($s) . "\n";

// Strings
$s = new Ds\Seq(["a", "b", "c"]);
echo json_encode($s) . "\n";

// Mixed types
$s = new Ds\Seq([1, "two", 3.5, null, true, false]);
echo json_encode($s) . "\n";

// Nested structure
$s = new Ds\Seq([[1, 2], [3, 4]]);
echo json_encode($s) . "\n";

// Single element
$s = new Ds\Seq([42]);
echo json_encode($s) . "\n";

// Nested Seq
$inner = new Ds\Seq([1, 2]);
$outer = new Ds\Seq([$inner, 3]);
echo json_encode($outer) . "\n";

// jsonSerialize returns array
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->jsonSerialize());
?>
--EXPECT--
[1,2,3]
[]
["a","b","c"]
[1,"two",3.5,null,true,false]
[[1,2],[3,4]]
[42]
[[1,2],3]
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
