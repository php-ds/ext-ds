<?php
namespace Ds\Tests\Stack;

trait _serialize
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testSerialize(array $values, array $expected)
    {
        // Stack has to be serialized in push order, so that values can be
        // pushed when being unserialized.
        $expected = array_reverse($expected);

        $instance = $this->getInstance($values);
        $this->assertSerialized($expected, $instance, false);
    }
}
