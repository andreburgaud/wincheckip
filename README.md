# wincheckip

Windows CLI tool to fetch and print a machine external IP address

It is a re-implementation of https://github.com/andreburgaud/checkip but written in C++ WinRT for Windows.

## Usages


### Default Usage

Execute `wincheckip` without arguments displays the machine external IP address:

```
> wincheckip
209.85.234.100
```

By default, `wincheckip` fetch the API https://checkip.amazonaws.com/ and displays the result.

### Change API Provider

You can use a different API by providing a URL at the command line:

```
> wincheckip --url https://ifconfig.co
209.85.234.100
```

Or:

```
> wincheckip --url https://api64.ipify.org
209.85.234.100
```

### Add Debug Information

Add option `--debug` or `-d` to make the output more verbose.


### Display Help or Version

```
> wincheckip --help
```

```
> wincheckip --version
```

## License

The Wincheckip code is published under the MIT license (see LICENSE file in the project).

The source code also include `color.h` based on https://cplusplus.com/articles/Eyhv0pDG/ which is published under an MIT license.
