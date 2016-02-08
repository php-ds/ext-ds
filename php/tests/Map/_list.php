<?php
namespace Ds\Tests\Map;

trait _list
{
    public function testList()
    {
        // list is useless in this context, because it tries to access indices
        // 0, 1, 2... which will return the value for each index as key,
        // rather than the first N values in the map.
    }
}
