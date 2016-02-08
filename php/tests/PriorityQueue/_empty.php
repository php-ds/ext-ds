<?php
namespace Ds\Tests\PriorityQueue;

trait _empty
{
    public function testArrayAccessEmpty()
    {
        $instance = $this->getInstance();
        $this->expectArrayAccessUnsupportedException();
        empty($instance['?']);
    }
}
