<?php
namespace Ds\Tests\Set;

trait _jsonEncode
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testJsonEncode(array $initial, array $expected)
    {
        $instance = $this->getInstance($initial);
        $this->assertEquals(json_encode($expected), json_encode($instance));
    }
}
