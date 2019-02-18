<?php

$a = new Vector();

$a[] = "a";
$a[] = "b";
$a[] = "c";

/**
 * Simple iteration, no writes.
 */
printf("---\n");
foreach ($a as $k => $v) {
    printf("key=%s, val=%s\n", $k, $v);
}

/**
 * Simple iteration, except we write to the object being iterated. The traversal
 * should NOT include the values being added.
 */
printf("---\n");
foreach ($a as $k => $v) {
    $a[] = $k;
    printf("key=%s, val=%s\n", $k, $v);
}

/**
 * Objects should still preserve object semantics (not full copy-on-write).
 */
$b = $a;
$b[] = "x";

printf("---\n");
var_dump($a);
var_dump($b);
