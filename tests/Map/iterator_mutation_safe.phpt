--TEST--
Map: mutation during iteration is safe (COW)
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$result = [];
foreach ($m as $key => $value) {
    $result[] = "$key=$value";
    if ($key === "a") {
        $m->put("d", 4);
        $m->remove("c");
    }
}
// Iterator sees original data
echo "iterated: " . implode(",", $result) . "\n";
// Map was mutated
echo "count: " . $m->count() . "\n";
echo "has d: " . var_export($m->hasKey("d"), true) . "\n";
echo "has c: " . var_export($m->hasKey("c"), true) . "\n";
?>
--EXPECT--
iterated: a=1,b=2,c=3
count: 3
has d: true
has c: false
