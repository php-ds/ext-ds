<?php
namespace Ds\Tests\Sequence;

trait toArray
{
    public function toArrayDataProvider()
    {
        return $this->basicDataProvider();
    }

    /**
     * @dataProvider toArrayDataProvider
     */
    public function testToArray(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertToArray($expected, $instance);
    }
}
