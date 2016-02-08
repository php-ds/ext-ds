<?php
namespace Ds\Tests\Stack;

trait push
{
    public function pushDataProvider()
    {
        return $this->basicDataProvider();
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushVariadic(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $instance->push(...$values);

        $this->assertToArray($expected, $instance);
        $this->assertCount(count($expected), $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPush(array $values, array $expected)
    {
        $instance = $this->getInstance();

        foreach ($values as $value) {
            $instance->push($value);
        }

        $this->assertToArray($expected, $instance);
        $this->assertCount(count($expected), $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testArrayAccessPush(array $values, array $expected)
    {
        $instance = $this->getInstance();

        foreach ($values as $value) {
            $instance[] = $value;
        }

        $this->assertToArray($expected, $instance);
        $this->assertCount(count($expected), $instance);
    }
}
