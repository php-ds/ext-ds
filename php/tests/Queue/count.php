<?php
namespace Ds\Tests\Queue;

trait count
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testCount(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertCount(count($expected), $instance);
    }

    public function testCountEmpty()
    {
        $instance = $this->getInstance();
        $this->assertCount(0, $instance);
    }
}
