# Data structures for PHP 7

[![Build Status](https://travis-ci.org/php-ds/ds.svg?branch=master)](https://travis-ci.org/php-ds/ds)
[![Build status](https://ci.appveyor.com/api/projects/status/in5p00vw6rk5f27q?svg=true)](https://ci.appveyor.com/project/krakjoe/ds)

[**Blog post**](https://medium.com/@rtheunissen/efficient-data-structures-for-php-7-9dda7af674cd) that covers the behaviour and performance benefits of each data structure.


## Installation

```bash
# Dependencies you might need to install
# sudo add-apt-repository ppa:ondrej/php
# sudo apt-get update
# sudo apt-get install git build-essential php7.0-dev

git clone https://github.com/php-ds/ds "php-ds"
cd php-ds

# Build and install the extension
phpize
./configure
sudo make install

# Clean up the build files
make clean
phpize --clean
```

## Usage

##### Enabling the extension

The best way to enable the extension is to create an *ini* file.

```bash
# To see where additional .ini files are located
php -i | grep "dir for additional .ini files"

# Create a new .ini file for the extension
echo "extension=ds.so" > /path/to/ini/files/30-ds.ini
```

---

You can also enable the extension temporarily using the command line:

```bash
php -d extension=ds.so
```

## Documentation

See [/php/include](/php/include).

## Testing

There is a suite of PHPUnit tests that can be installed using [**Composer**](https://getcomposer.org/doc/00-intro.md#installation-linux-unix-osx).

The extension has to be installed to run the tests.

#### Running the tests

``` bash
composer install
./vendor/bin/phpunit
```

## Contributing

Please see [CONTRIBUTING](CONTRIBUTING.md) for more information.

## Credits

- [Rudi Theunissen](https://github.com/rtheunissen)
- [Joe Watkins](https://github.com/krakjoe)

## License

The MIT License (MIT). Please see [LICENSE](LICENSE.md) for more information.
