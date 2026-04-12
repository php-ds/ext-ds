--TEST--
Set: mutation during iteration is safe (COW)
--FILE--
<?php
$s = new Ds\Set([1, 2, 3]);
$result = [];
foreach ($s as $value) {
    $result[] = $value;
    if ($value === 1) {
        $s->add(4);
        $s->remove(3);
    }
}
echo "iterated: " . implode(",", $result) . "\n";
echo "count: " . $s->count() . "\n";
echo "has 4: " . var_export($s->contains(4), true) . "\n";
echo "has 3: " . var_export($s->contains(3), true) . "\n";
?>
--EXPECT--
iterated: 1,2,3
count: 3
has 4: true
has 3: false
