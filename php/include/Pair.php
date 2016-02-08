<?php
namespace Ds;

/**
 * A pair which represents a key, and an associated value.
 *
 * The key can be accessed using $p[0], $p['key'], or $p->key.
 * The value can be accessed using $p[1], $p['value'], or $p->value.
 */
final class Pair
{
    /**
     * @param mixed $key The pair's key
     */
    public $key;

    /**
     * @param mixed $value The pair's value
     */
    public $value;

    /**
     * @inheritDoc
     */
    public function toArray(): array {}
}
