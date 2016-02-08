<?php
namespace Ds\Tests\Stack;

trait _isset
{
    public function testArrayAccessIsset()
    {
        $set = $this->getInstance();
        $this->expectArrayAccessUnsupportedException();
        isset($set['a']);
    }
}
