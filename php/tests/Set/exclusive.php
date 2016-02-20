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

    /**
     * @dataProvider exclusiveDataProvider
     */
    public function testExclusiveWithSelf(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $this->assertEquals([], $a->exclusive($a)->toArray());
    }

    /**
     * @dataProvider exclusiveDataProvider
     */
    public function testExclusiveOperatorWithSelf(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $this->assertEquals([], ($a ^ $a)->toArray());
    }

    /**
     * @dataProvider exclusiveDataProvider
     */
    public function testExclusiveOperatorAssignWithSelf(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);

        $a ^= $a;
        $this->assertEquals([], $a->toArray());
    }
}
