<?php
namespace Ds\Tests\Pair;

trait _clone
{
    public function testClone()
    {
        $instance = $this->getPair('a', 1);

        $clone = clone $instance;

        $this->assertEquals(get_class($instance), get_class($clone));
        $this->assertEquals($instance->toArray(), $clone->toArray());
        $this->assertFalse($clone === $instance);
    }
}
