--TEST--
Nested: Seq containing Map instances
--FILE--
<?php
$m1 = new Ds\Map(["a" => 1]);
$m2 = new Ds\Map(["b" => 2]);
$s = new Ds\Seq([$m1, $m2]);

echo "seq count: " . $s->count() . "\n";
echo "m1 a: " . $s->get(0)->get("a") . "\n";
echo "m2 b: " . $s->get(1)->get("b") . "\n";

// Iterate through seq of maps
foreach ($s as $map) {
    foreach ($map as $key => $value) {
        echo "  $key => $value\n";
    }
}
?>
--EXPECT--
seq count: 2
m1 a: 1
m2 b: 2
  a => 1
  b => 2
