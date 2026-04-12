--TEST--
Seq::join - join values as string
--FILE--
<?php
// With glue
$s = new Ds\Seq([1, 2, 3]);
echo $s->join(", ") . "\n";

// Without glue (no separator)
$s = new Ds\Seq(["a", "b", "c"]);
echo $s->join() . "\n";

// Empty string glue
$s = new Ds\Seq([1, 2, 3]);
echo $s->join("") . "\n";

// Single element
$s = new Ds\Seq(["only"]);
echo $s->join(", ") . "\n";

// Empty seq
$s = new Ds\Seq();
echo "'" . $s->join(", ") . "'\n";

// Mixed types
$s = new Ds\Seq([1, "two", 3.5, true]);
echo $s->join("-") . "\n";
?>
--EXPECT--
1, 2, 3
abc
123
only
''
1-two-3.5-1
