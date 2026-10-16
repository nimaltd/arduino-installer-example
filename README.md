# arduino-installer-example

A small library made to try [arduino-installer](https://github.com/nimaltd/arduino-installer) on, and a starting point for making your own library installable for Arduino.

It does one useful thing. `example_due()` says yes once every `EXAMPLE_PERIOD_MS` milliseconds and never blocks, so `loop()` can blink an LED, read a sensor or print a line on a rhythm without `delay()` holding everything else up. `example_hello()` prints `EXAMPLE_GREETING` on Serial. Both settings are in `example_config.h`. Its `uptime` option adds `example_uptime()`, the time since start up in hours, minutes and seconds. It builds for any board.

---

## Try the installer with it

Install the installer once:

```bash
pip install arduino-installer
```

Then, from your sketch's folder, the one holding `blink.ino` for a sketch called `blink`:

```bash
arduino-installer nimaltd/arduino-installer-example
```

It asks where the library goes. Press Enter, and it goes into the libraries folder of your sketchbook, where Library Manager installs to, for every sketch:

```
This is the sketch blink. Where do the libraries go?
  1  the libraries folder, for every sketch: C:/Users/you/Documents/Arduino/libraries
  2  this sketch's src folder, for this sketch only
Install into [1]:
```

Then use it in your sketch:

```cpp
#include "example.h"

example_t tick;

void setup()
{
    Serial.begin(115200);
    example_init(&tick);
}

void loop()
{
    if (example_due(&tick))
    {
        example_hello(&tick);
    }
}
```

Upload it, and open the Serial Monitor at 115200 baud:

```
Hello from example 1
Hello from example 2
Hello from example 3
```

### Things to try

**Put it in one sketch only.** Answer `2`, or give `--into sketch`, and the library goes into the sketch's `src` folder. Arduino builds it with that sketch alone, which then includes it by its path:

```cpp
#include "src/example/src/example.h"
```

**Keep a setting through an update.** Open `example_config.h`, in `Arduino/libraries/example/src/`, or in your sketch's `src/example/src/`, and change the period between its `USER CODE` lines:

```c
/* USER CODE BEGIN EXAMPLE_CONFIGURATION */
#define EXAMPLE_PERIOD_MS           250U
#define EXAMPLE_GREETING            "Hi"
/* USER CODE END EXAMPLE_CONFIGURATION */
```

Run the install command again: every file is replaced, and your settings are still there. In the libraries folder they are the settings of every sketch; in a sketch, of that sketch alone.

**Add a part, and take it out.** `--with uptime` adds `example_uptime.h` and `example_uptime.cpp`. `--without uptime` takes them out again.

```bash
arduino-installer nimaltd/arduino-installer-example --with uptime
```

```cpp
#include "example_uptime.h"

example_uptime_t up = example_uptime();   /* up.hours, up.minutes, up.seconds */
```

**Uninstall it.** From the sketch it is in, or from anywhere for the libraries folder. Its files go, and a configuration header with your settings in it is saved beside them first, as a `.bak`:

```bash
arduino-installer --uninstall example
```

**Without internet.** Download this repository as a zip, from the green **Code** button, **Download ZIP**, and give the installer the zip instead of the name:

```bash
arduino-installer D:/Downloads/arduino-installer-example-main.zip
```

---

## Make your own library installable

Copy [`installer.yml`](installer.yml) to the root of your library's repository, and change what is yours: its name, its description, its repository, its files, and the `arduino` part that `library.properties` is written from. Every key the installer reads is in it, with a comment saying what it does; the ones this library has no use for are commented out. Only `name` and `files` are required.

Then anyone installs your library with:

```bash
arduino-installer yourname/yourlibrary
```

Keep the code in `src/`, the headers beside the code that includes them, and `extern "C"` in every header of C code, as this one has. The [arduino-installer README](https://github.com/nimaltd/arduino-installer#for-someone-writing-a-library) has the rest.

---

## API

| Function | What it does |
|---|---|
| `void example_init(example_t *handle)` | Start a period of `EXAMPLE_PERIOD_MS` from now |
| `bool example_due(example_t *handle)` | Whether the period has passed. Returns at once, and starts the next one when it has |
| `uint32_t example_count(const example_t *handle)` | How many times `example_due()` has said yes |
| `void example_hello(const example_t *handle)` | Print `EXAMPLE_GREETING` on Serial, and how many periods have passed |
| `example_uptime_t example_uptime(void)` | With the `uptime` option: hours, minutes and seconds since start up |

Declare one `example_t` for each thing that repeats on its own rhythm. A loop that comes back late is caught up, so the rhythm does not drift.

---

## License

Apache License 2.0. See [LICENSE.md](LICENSE.md).
