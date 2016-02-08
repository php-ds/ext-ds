<?php
namespace Ds\Tests\Map;

trait sort
{
    public function sortDataProvider()
    {
        return [
            [[

            ]],
            [[
                'a' => 3,
                'c' => 1,
                'b' => 2,
            ]],
            [[
                3 => 'd',
                0 => 'a',
                1 => 'b',
                4 => 'e',
                2 => 'c',
            ]],
        ];
    }

    /**
     * @dataProvider sortDataProvider
     */
    public function testSort(array $values)
    {
        $instance = $this->getInstance($values);

        $expected = $values; ksort($expected);

        $this->assertToArray($expected, $instance->sort());
        $this->assertToArray($values, $instance);
    }

    /**
     * @dataProvider sortDataProvider
     */
    public function testSortedUsingComparator(array $values)
    {
        $instance = $this->getInstance($values);

        $sorted = $instance->sort(function($a, $b) {
            return $b->value <=> $a->value;
        });

        $expected = $values;
        ksort($expected);

        $this->assertToArray($expected, $sorted);
        $this->assertToArray($values, $instance);
    }

    public function testSortIsStable()
    {
        $values = range(1, self::MANY);
        $instance = $this->getInstance($values);

        $sorted = $instance->sort(function($a, $b) {
            return $b[0] <=> $a[0];
        });

        $this->assertToArray($values, $sorted);
    }
}
