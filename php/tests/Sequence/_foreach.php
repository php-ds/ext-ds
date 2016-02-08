<?php
namespace Ds\Tests\Sequence;

trait _foreach
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testForEach(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertForEach($expected, $instance);
    }
}
