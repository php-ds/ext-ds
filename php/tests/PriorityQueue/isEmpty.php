<?php
namespace Ds\Tests\PriorityQueue;

trait isEmpty
{
    public function testIsEmpty()
    {
        $instance = $this->getInstance();
        $this->assertTrue($instance->isEmpty());

        $instance->push('a', 1);
        $this->assertFalse($instance->isEmpty());

        $instance->pop();
        $this->assertTrue($instance->isEmpty());
    }
}
