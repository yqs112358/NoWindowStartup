# No-window-startup

> A simple tool to help you start a program with its window hidden (Windows platform)

## Usage

```shell
./NoWindowStartup.exe --program=string [options...]
options:
  -p, --program       set program path (string)
  -w, --workingdir    set program working directory (string [=])
  -o, --output        set program output redirect target file (string [=])
```

## Tips

- `-p / --program` argument must be filled to give the target program to launch.
- If you want to redirect target's `stdout` & `stderr` to a file, use `-o / --output` argument
- If any path in command contains space (like `C:/Program Files/Google/...`), you must include them in quotation marks
- If any path in command contains `\`，you must use escape them with `\\`, or the program will report error

## Error output

If any error occurred, you will find error output in `error.log` generated in the same directory of this program.

## Examples

```shell
NoWindowStartup.exe --program "C:\\Users\\Tom Joshson\\Desktop\\a.exe"
```

```shell
NoWindowStartup.exe --program "C:\\Users\\Tom Joshson\\Desktop\\b.exe --arg1 --arg2"
```

```shell
NoWindowStartup.exe --program "C:\\Users\\Tom Joshson\\Desktop\\c.exe" --workingdir "C:\\Users\\Tom Joshson\\Desktop" -o "C:\\Users\\Tom Joshson\\Desktop\\logs.txt"
```

