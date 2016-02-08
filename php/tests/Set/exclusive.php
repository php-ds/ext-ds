<?php
namespace Ds\Tests\Set;

trait exclusive
{
    public function exclusiveDataProvider()
    {
        // Values in either A or B, but not both.
        // A, B, expected result
        return [
            [[],          [],           []],
            [['a'],       ['b'],        ['a', 'b']],
            [['a', 'b'],  ['b'],        ['a']],
            [['a', 'b'],  ['b', 'a'],   []],
        ];
    }

    /**
     * @dataProvider exclusiveDataProvider
     */
    public function testExclusive(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $b = $this->getInstance($b);

        $this->assertEquals($expected, $a->exclusive($b)->toArray());
    }

    /**
     * @dataProvider exclusiveDataProvider
     */
    public function testExclusiveOperator(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $b = $this->getInstance($b);

        $this->assertEquals($expected, ($a ^ $b)->toArray());
    }

    /**
     * @dataProvider exclusiveDataProvider
     */
    public function testExclusiveOperatorAssign(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $b = $this->getInstance($b);

        $a ^= $b;
        $this->assertEquals($expected, $a->toArray());
    }
}
