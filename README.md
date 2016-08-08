# Data Structures for PHP 7

[![Build Status](https://travis-ci.org/php-ds/extension.svg?branch=master)](https://travis-ci.org/php-ds/extension)
[![Build status](https://ci.appveyor.com/api/projects/status/dbcssp6flml2gher?svg=true)](https://ci.appveyor.com/project/rtheunissen/extension)
[![PECL](https://img.shields.io/badge/PECL-1.1.3-blue.svg)](https://pecl.php.net/package/ds)

An extension providing specialized data structures as efficient alternatives to the PHP array.

## Documentation

Documentation is available on [php.net](http://docs.php.net/manual/en/book.ds.php). You should also include the [polyfill](https://github.com/php-ds/polyfill) in your project for compatibility and IDE integration. The polyfill will not be loaded if the extension is enabled

## Installation

The easiest way to install the extension is to use PECL:

```
pecl install ds
```


You can also build directly from source:

```bash
# Dependencies you might need to install
# sudo apt-get install git build-essential php7.0-dev

git clone https://github.com/php-ds/extension "php-ds"
cd php-ds

# Build and install the extension
phpize
./configure
make 
make install

# Clean up the build files
make clean
phpize --clean
```

If you're on Windows, you can download a compiled .dll on [PECL](https://pecl.php.net/package/ds) or under [releases](https://github.com/php-ds/extension/releases).

## Enable

You'll need to add `extension=ds.so`  (or .dll if you're on Windows) to your primary *ini* file.
You can also create a separate *ini* file for the extension, which allows you to control load order.

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

## Testing

There is a suite of PHPUnit tests that can be installed using [**Composer**](https://getcomposer.org/doc/00-intro.md#installation-linux-unix-osx).

``` bash
composer install
composer test
```

## Compatibility

It's highly recommended that you include the [polyfill](https://github.com/php-ds/polyfill) as a dependency in your project. This allows your codebase to still function in an environment where the extension is not installed. 

## Contributing

Please see [CONTRIBUTING](CONTRIBUTING.md) for more information.

## Credits

- [Rudi Theunissen](https://github.com/rtheunissen)
- [Joe Watkins](https://github.com/krakjoe)

## License

The MIT License (MIT). Please see [LICENSE](LICENSE.md) for more information.
