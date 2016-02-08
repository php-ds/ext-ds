<?php
namespace Ds\Tests\Stack;

trait clear
{
    public function testClear()
    {
        $instance = $this->getInstance($this->sample());
        $instance->clear();

        $this->assertToArray([], $instance);
        $this->assertCount(0, $instance);
    }
}
