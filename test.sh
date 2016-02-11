#!/bin/bash

# Produces a basic confirmation prompt
function confirm {
    read -p "$1 (y/n): " prompt
    if [[ ! $prompt =~ ^[yY]$ ]]; then
        return 1
    fi
}

# Determines if a command or program is valid
function installed {
    command -v composer > /dev/null 2>&1 || return 1
}

# Installs Composer globally
function install_composer {
    curl -sS https://getcomposer.org/installer | php
    sudo mv composer.phar /usr/local/bin/composer
}

# Verifies that Composer is installed
function verify_composer {
    if ! installed composer; then
        if confirm "Composer is not installed. Install it now?"; then
            install_composer
        else
            echo "Composer is required to run the tests."
            return 1
        fi
    fi
}

# Verifies that PHPUnit is installed
function verify_phpunit {
    if [ ! -f ./vendor/bin/phpunit ]; then
        composer install
    fi
}

# Checks that all dependencies are in place to run the tests.
function setup {
    verify_composer
    verify_phpunit
}

if setup; then
    php -dextension=ds.so ./vendor/bin/phpunit $@
else
    exit 1
fi
