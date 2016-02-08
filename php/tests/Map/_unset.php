<?php
namespace Ds\Tests\Map;

trait _unset
{

    public function testArrayAccessUnset()
    {
        $instance = $this->getInstance(['a' => 1]);
        unset($instance['a']);
        $this->assertToArray([], $instance);
    }

    public function testArrayAccessUnsetByReference()
    {
        $instance = $this->getInstance(['a' => [1]]);
        unset($instance['a'][0]);

        $this->assertToArray(['a' => []], $instance);
    }
}
