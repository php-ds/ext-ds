<?php
namespace Ds\Tests\Sequence;

trait insert
{
    public function insertDataProvider()
    {
        $s = $this->sample();

        $h = count($s) / 2;

        // initial, index, values
        return [
            [[0], 0, [1]],
            [[0], 1, [1]],

            [['a', 'b'], 0, ['c', 'd']],
            [['a', 'b'], 1, ['c', 'd']],
            [['a', 'b'], 2, ['c', 'd']],

            // Inserting many values at the front of an empty sequence
            [[], 0, $s],

            // Inserting many values at the front of a non-empty sequence
            [$s, 0, $s],

            // Inserting many values at the front of a non-empty sequence
            [$s, $h, $s],
        ];
    }


    /**
     * @dataProvider insertDataProvider
     */
    public function testInsertVariadic(array $initial, $index, array $values)
    {
        $expected = $initial;
        array_splice($expected, $index, 0, $values);

        $instance = $this->getInstance($initial);
        $instance->insert($index, ...$values);

        $this->assertEquals(count($expected), count($instance));
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider insertDataProvider
     */
    public function testInsert(array $initial, $index, array $values)
    {
        $expected = $initial;
        array_splice($expected, $index, 0, array_reverse($values));

        $instance = $this->getInstance($initial);

        foreach ($values as $value) {
            $instance->insert($index, $value);
        }

        $this->assertEquals(count($expected), count($instance));
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider outOfRangeDataProvider
     */
    public function testInsertIndexOutOfRange($initial, $index)
    {
        $instance = $this->getInstance($initial);
        $this->expectIndexOutOfRangeException();
        $instance->insert($index);
    }

    /**
     * @dataProvider badIndexDataProvider
     */
    public function testInsertIndexBadIndex($initial, $index)
    {
        $instance = $this->getInstance();
        $this->expectWrongIndexTypeException();
        $instance->insert($index);
    }
}
