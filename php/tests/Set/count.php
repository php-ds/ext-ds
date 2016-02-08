<?php
namespace Ds\Tests\Set;

trait count
{
    public function testCount()
    {
        list($unique, $duplicates) = $this->getUniqueAndDuplicateData();

        $instance = $this->getInstance($unique);
        $this->assertCount(count($unique), $instance);

        $instance = $this->getInstance($duplicates);
        $this->assertCount(count($unique), $instance);
    }

    public function testCountEmpty()
    {
        $instance = $this->getInstance();
        $this->assertCount(0, $instance);
    }
}
