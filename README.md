# lib-installer-example-arduino

A small library made to try [lib-installer](https://github.com/nimaltd/lib-installer) on, and a starting point for making your own library installable for Arduino.

It does one useful thing. `example_due()` says yes once every `EXAMPLE_PERIOD_MS` milliseconds and never blocks, so `loop()` can blink an LED, read a sensor or print a line on a rhythm without `delay()` holding everything else up. `example_hello()` prints `EXAMPLE_GREETING` on Serial. Both settings are in `example_config.h`. Its `uptime` option adds `example_uptime()`, the time since start up in hours, minutes and seconds. It builds for any board.

It also has a step for the installer to run, so you can see one work: installed into a sketch, `make-id` makes `example_id.h` in the sketch's folder, the board's own ID, a random number. The installer shows what it runs and asks first. Its script stays in the library's folder, named `{library}/tools/make_id.py`, which is the library's folder wherever it went.

---

## Try the installer with it

Install the installer once:

```bash
pip install lib-installer
```

It needs lib-installer 4.0.0 or newer. `pip install --upgrade lib-installer` brings an older one up to date.

Then, from your sketch's folder, the one holding `blink.ino` for a sketch called `blink`:

```bash
lib-installer nimaltd/lib-installer-example-arduino
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

**Give the board an ID.** Installed into a sketch, the library's step runs last, once you say yes:

```
Run
  make-id      Make this board's own ID, example_id.h
               python src/example/tools/make_id.py example_id.h
               Run it? [Y/n]:
Made example_id.h: device B5B29845.
               done
```

`example_id.h` is now in the sketch's folder, beside `blink.ino`, which is where a sketch finds a header of its own:

```cpp
#include "src/example/src/example.h"
#include "example_id.h"     // made by the make-id step, this board's own

void setup()
{
    Serial.begin(115200);
    Serial.println(EXAMPLE_DEVICE_ID, HEX);
}
```

Run the install again, and the step is not asked about: `make-id skipped, example_id.h is already there`, so the ID never changes under a board already in use. Delete the file, and the next install asks again. `--run make-id` runs it without asking, `--no-run make-id` leaves it, and `--yes` takes every answer offered, for a script. In the libraries folder, which belongs to no sketch, it is left unrun, and the install goes ahead: `make-id not run, there is no project folder here to run it in`.

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
lib-installer nimaltd/lib-installer-example-arduino --with uptime
```

```cpp
#include "example_uptime.h"

example_uptime_t up = example_uptime();   /* up.hours, up.minutes, up.seconds */
```

**Uninstall it.** From the sketch it is in, or from anywhere for the libraries folder. Its files go, and a configuration header with your settings in it is saved beside them first, as a `.bak`. `example_id.h` stays in the sketch, since it is yours:

```bash
lib-installer --uninstall example
```

**Without internet.** Download this repository as a zip, from the green **Code** button, **Download ZIP**, and give the installer the zip instead of the name:

```bash
lib-installer D:/Downloads/lib-installer-example-arduino-main.zip
```

---

## Make your own library installable

Copy [`installer.yml`](installer.yml) to the root of your library's repository, and change what is yours: its name, its description, its repository, its files, and the `arduino` part that `library.properties` is written from. Every key the installer reads is in it, with a comment saying what it does; the ones this library has no use for are commented out. Only `name` and its `headers` or `sources` are required.

Then anyone installs your library with:

```bash
lib-installer yourname/yourlibrary
```

Keep the code in `src/`, the headers beside the code that includes them, and `extern "C"` in every header of C code, as this one has. `run` lists the steps run after an install into a sketch, with the script in [`tools/make_id.py`](tools/make_id.py). The [lib-installer README](https://github.com/nimaltd/lib-installer#for-someone-writing-a-library) has the rest.

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
