--TEST--
Map::jsonSerialize - encode map as JSON object
--FILE--
<?php
// String keys encode as JSON object
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
echo "string keys: " . json_encode($m) . "\n";

// Integer keys also produce JSON object
$m2 = new Ds\Map([0 => "x", 1 => "y", 2 => "z"]);
echo "int keys: " . json_encode($m2) . "\n";

// Empty map
$empty = new Ds\Map();
echo "empty: " . json_encode($empty) . "\n";

// Mixed value types
$m3 = new Ds\Map([
    "int" => 1,
    "float" => 1.5,
    "str" => "hello",
    "bool" => true,
    "null" => null,
    "arr" => [1, 2, 3],
]);
echo "mixed: " . json_encode($m3) . "\n";

// Nested map
$inner = new Ds\Map(["x" => 1]);
$outer = new Ds\Map(["inner" => $inner]);
echo "nested: " . json_encode($outer) . "\n";

// Single element
$m4 = new Ds\Map(["key" => "value"]);
echo "single: " . json_encode($m4) . "\n";

// Non-sequential integer keys produce JSON object
$m5 = new Ds\Map([5 => "a", 10 => "b"]);
echo "non-seq int: " . json_encode($m5) . "\n";
?>
--EXPECT--
string keys: {"a":1,"b":2,"c":3}
int keys: {"0":"x","1":"y","2":"z"}
empty: {}
mixed: {"int":1,"float":1.5,"str":"hello","bool":true,"null":null,"arr":[1,2,3]}
nested: {"inner":{"x":1}}
single: {"key":"value"}
non-seq int: {"5":"a","10":"b"}
