<?php
namespace Ds\Tests\Map;

trait reduce
{
    public function reduceDataProvider()
    {
        // values, initial, callback, expected
        return [

            // Test reducing an empty map returns the initial
            [
                [],
                1,
                function(){},
                1
            ],

            // Test reducing strings by concatenating them.
            [
                ['a', 'b', 'c'],
                '/',
                function ($c, $k, $v) { return join('-', func_get_args()); },
                '/-0-a-1-b-2-c',
            ],

            // Test reducing mixed numeric values by multiplying them.
            [
                [1, 2.5, '3'],
                2,
                function ($c, $k, $v) { return $c * $v; },
                15,
            ],
        ];
    }

    public function reduceWithoutInitialDataProvider()
    {
        // values, callback, expected
        return [

            // Test reducing an empty map returns the initial
            [
                [],
                function(){},
                null,
            ],

            // Test reducing strings by concatenating them.
            [
                ['a', 'b', 'c'],
                function ($c, $k, $v) { return join('-', func_get_args()); },
                '-0-a-1-b-2-c',
            ],

            // Test reducing mixed numeric values by multiplying them.
            [
                [1, 2.5, '3'],
                function ($c, $k, $v) { return $c * $v; },
                0,
            ],
        ];
    }

    /**
     * @dataProvider reduceDataProvider
     */
    public function testReduce(array $values, $initial, callable $callback, $expected)
    {
        $instance = $this->getInstance($values);

        $reduced = $instance->reduce($callback, $initial);

        $this->assertToArray($values, $instance);
        $this->assertEquals($expected, $reduced);
    }

    /**
     * @dataProvider reduceWithoutInitialDataProvider
     */
    public function testReduceWithoutInitial(array $values, callable $callback, $expected)
    {
        $instance = $this->getInstance($values);

        $reduced = $instance->reduce($callback);

        $this->assertToArray($values, $instance);
        $this->assertEquals($expected, $reduced);
    }

    public function testReduceCallbackThrowsException()
    {
        $instance = $this->getInstance([1, 2, 3]);
        $result = null;

        try {
            $result = $instance->reduce(function($carry, $key, $value) {
                throw new \Exception();
            });
        } catch (\Exception $e) {
            $this->assertToArray([1, 2, 3], $instance);
            $this->assertNull($result);
            return;
        }

        $this->fail('Exception should have been caught');
    }

    public function testReduceCallbackThrowsExceptionLaterOn()
    {
        $instance = $this->getInstance([1, 2, 3]);
        $result = null;

        try {
            $result = $instance->reduce(function($carry, $key, $value) {
                if ($value === 3) {
                    throw new \Exception();
                }
            });
        } catch (\Exception $e) {
            $this->assertToArray([1, 2, 3], $instance);
            $this->assertNull($result);
            return;
        }

        $this->fail('Exception should have been caught');
    }
}
