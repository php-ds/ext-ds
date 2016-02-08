<?php
namespace Ds\Tests\PriorityQueue;

trait _unset
{
    public function testArrayAccessUnset()
    {
        $instance = $this->getInstance();
        $this->expectArrayAccessUnsupportedException();
        unset($instance['?']);
    }
}
