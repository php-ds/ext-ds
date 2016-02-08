<?php
namespace Ds\Tests\Sequence;

trait unshift
{
    public function unshiftDataProvider()
    {
        return $this->basicDataProvider();
    }

    /**
     * @dataProvider unshiftDataProvider
     */
    public function testUnshiftVariadic(array $initial, array $values)
    {
        $instance = $this->getInstance($initial);

        $instance->unshift(...$values);
        $expected = array_merge($values, $initial);

        $this->assertToArray($expected, $instance);
        $this->assertEquals(count($expected), count($instance));
    }

    /**
     * @dataProvider unshiftDataProvider
     */
    public function testUnshift(array $initial, array $values)
    {
        $instance = $this->getInstance($initial);

        foreach ($values as $value) {
            $instance->unshift($value);
        }

        $expected = array_merge(array_reverse($values), $initial);

        $this->assertToArray($expected, $instance);
        $this->assertEquals(count($expected), count($instance));
    }
}
