--TEST--
Set::join - joins values into a string
--FILE--
<?php
$s = new Ds\Set([1, 2, 3]);

// With glue
echo "comma: " . $s->join(",") . "\n";
echo "dash: " . $s->join("-") . "\n";
echo "space: " . $s->join(" ") . "\n";
echo "multi-char: " . $s->join(" | ") . "\n";

// Without glue
echo "no glue: " . $s->join() . "\n";

// Empty set
$empty = new Ds\Set();
echo "empty with glue: '" . $empty->join(",") . "'\n";
echo "empty no glue: '" . $empty->join() . "'\n";

// Single element
$single = new Ds\Set(["only"]);
echo "single: " . $single->join(",") . "\n";

// String values
$s2 = new Ds\Set(["hello", "world"]);
echo "strings: " . $s2->join(" ") . "\n";

// Empty string glue
echo "empty glue: " . $s->join("") . "\n";

// Mixed types
$s3 = new Ds\Set([1, "two", 3.14, true]);
echo "mixed: " . $s3->join(",") . "\n";
?>
--EXPECT--
comma: 1,2,3
dash: 1-2-3
space: 1 2 3
multi-char: 1 | 2 | 3
no glue: 123
empty with glue: ''
empty no glue: ''
single: only
strings: hello world
empty glue: 123
mixed: 1,two,3.14,1
