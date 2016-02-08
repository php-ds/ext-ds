<?php
namespace Ds\Tests\Set;

trait add
{
    public function addDataProvider()
    {
        list($unique, $duplicates) = $this->getUniqueAndDuplicateData();

        // initial, input, expected
        return [
            [[], [], []],

            [[],    ['a'],      ['a']],
            [['a'], ['a'],      ['a']],
            [['a'], ['a', 'b'], ['a', 'b']],

            [[],    ['a'],      ['a']],
            [['a'], ['a'],      ['a']],
            [['a'], ['a', 'b'], ['a', 'b']],

            // Test that numeric strings are not treated as int.
            [['1'], [ 1 ], ['1', 1 ]],
            [[ 1 ], ['1'], [ 1, '1']],

            // Test adding sample values to empty set adds all.
            [[], $unique, $unique],

            // Test adding duplicates values to empty set adds sample only.
            [[], $duplicates, $unique],

            // Test adding sample values to sample set set adds nothing.
            [$unique, $unique, $unique],

            // Test adding duplicates to sample values adds nothing.
            [$unique, $duplicates, $unique]
        ];
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAdd(
        array $initial,
        array $values,
        array $expected
    ) {
        $instance = $this->getInstance($initial);

        foreach($values as $value) {
            $instance->add($value);
        }

        $this->assertEquals(count($expected), count($instance));
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testArrayAccessAdd(
        array $initial,
        array $values,
        array $expected
    ) {
        $instance = $this->getInstance($initial);

        foreach($values as $value) {
            $instance[] = $value;
        }

        $this->assertEquals(count($expected), count($instance));
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddVariadic(
        array $initial,
        array $values,
        array $expected
    ) {
        $instance = $this->getInstance($initial);
        $instance->add(...$values);

        $this->assertEquals(count($expected), count($instance));
        $this->assertToArray($expected, $instance);
    }

    public function testAddCircularReference()
    {
        $instance = $this->getInstance();
        $instance->add($instance);
        $this->assertToArray([$instance], $instance);
    }

    public function testAddIndirectCircularReference()
    {
        $a = $this->getInstance();
        $b = $this->getInstance();

        $a->add($b);
        $b->add($a);

        $this->assertToArray([$b], $a);
        $this->assertToArray([$a], $b);
    }
}
