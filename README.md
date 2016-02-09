# Efficient data structures for PHP 7

[![Build Status](https://travis-ci.org/php-ds/ds.svg?branch=master)](https://travis-ci.org/php-ds/ds)
[![Build status](https://ci.appveyor.com/api/projects/status/in5p00vw6rk5f27q?svg=true)](https://ci.appveyor.com/project/krakjoe/ds)

## Installation

```bash
# Dependencies you might need to install
#
# sudo apt-get update
# sudo apt-get install -y build-essential autoconf
#
# sudo add-apt-repository ppa:ondrej/php
# sudo apt-get install -y php7.0-dev

git clone https://github.com/php-ds/ds "php-ds"
cd php-ds

# Build and install the extension
phpize
./configure
make
sudo make install

# Clean up the build files
phpize --clean
make clean
```

##### What about Windows?

Windows will be supported when a stable release is on PECL.

## Usage

##### Enabling the extension

```bash
php -d extension=ds.so 
```

---


You can use the `Ds` namespace when using a specific structure:

```php
<?php

use Ds\Set;

$set = new Set();
$set->add('a', 2, new \stdClass());

print_r($set);

/*
Ds\Set Object
(
    [0] => a
    [1] => 2
    [2] => stdClass Object
        (
        )
)
*/
```

You can also use the `ds` static API:

```php
<?php

$vector = ds::vector();
$vector->push('a', 'c', 'd');
$vector->insert(1, 'b');

print_r($vector);

/*
Ds\Vector Object
(
    [0] => a
    [1] => b
    [2] => c
    [3] => d
)
*/
```

## Docs

See [/php/include](/php/include). 

## Testing

`php-ds` comes with a suite of `PHPUnit` tests requiring `composer`:

``` bash
composer install
./vendor/bin/phpunit
```

## Contributing

Please see [CONTRIBUTING](CONTRIBUTING.md) for details.

## Credits

- [Rudi Theunissen](https://github.com/rtheunissen)
- [Joe Watkins](https://github.com/krakjoe)

## License

The MIT License (MIT). Please see [LICENSE](LICENSE.md) for more information.
