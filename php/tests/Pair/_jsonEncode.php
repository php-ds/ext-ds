<?php
namespace Ds\Tests\Pair;

trait _jsonEncode
{
    public function testJsonEncode()
    {
        $instance = $this->getPair('a', 1);
        $this->assertEquals(json_encode(['a', 1]), json_encode($instance));
    }
}
