<?php
namespace Ds\Tests\Set;

trait _isset
{
    public function testArrayAccessIsset()
    {
        $set = $this->getInstance();
        $this->expectArrayAccessUnsupportedException();
        isset($set['a']);
    }
}
