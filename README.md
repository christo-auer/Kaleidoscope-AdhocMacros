# Kaleidoscope-AdhocMacros

AdhocMacros lets you record a series of keystrokes and then replay them. A
replay can be executed multiple times by prepending the number of repetitions.

AdhocMacros was inspired by Vim's ingenious [macro
feature](http://vim.wikia.com/wiki/Macros) and makes it possible to use apply
this idea to any application.


## Using the plugin
To use the plugin, you need to setup an array that will contain the key stroke
events. To control recording and replaying, there are several extension methods
that you have to call by using macros (see the TODO example).

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-AdhocMacros.h>

void setup() {
  Kaleidoscope.setup();
  Kaleidoscope.use(&AdhocMacros);

  // setup the macro array in which the key events are stored
  static kaleidoscope::AdhocMacros::AdhocKeyEvent adhocMacro[100];
  kaleidoscope::AdhocMacros::adhocMacro = adhocMacro;

  // set the maximum length of an adhoc macro
  kaleidoscope::AdhocMacros::maxAdhocMacroLength = 100;

  // starting, stopping, replaying recordings
  // NOTE you must call these methods by using Kaleidoscope macros (for now)
  // to toggle recording:
  AdhocMacros.toggleRecording();

  // to toggle replaying
  AdhocMacros.toggleReplay();

  // add a digit to the repeat counter, e.g., repeat 50 times
  AdhocMacros.repeatAddDigit(5);
  AdhocMacros.repeatAddDigit(0);

  // reset repeat counter
  AdhocMacros.resetRepeat();
}
```
## Plugin Methods

### `.flashCol`, `.flashRow`, `.flashColor`

> Column, row and color of the key that starts flashing while recording.
> By default `.flashCol=0`, `.flashRow=0` and `.flashColor = CRGB(255,0,0)`.

### `.startRecording`, `.stopRecording`, `.toggleRecording`

> Control recording, where `.toggleRecording` is useful for using only one key
> to control the whole recording process. Note, these methods don't react if a
> replay is running.

### `.replay`, `.abortReplay`, `.toggleReplay`

> Control replay, where each replay is executed according to the repeat value,
> which is 1 by default.
>
> Note: A replay always executes a full cycle, the whole macro that you have
> recorded and can only be aborted between cycles, i.e., `.abortReplay` and
> `.toggleReplay` have effect only between repetitions of the adhoc macro.

### `.repeatAddDigit`, `.resetRepeat`

> Control repetition value, which is 1 by default. You can add digits to the
> repetition counter by calling `.repeatAddDigit` whose argument is a single
> digit from (0..9). The maximum amount of repetitions is 1000. To reset the
> repetition counter to 1, call `.resetRepeat`.

## Further reading

Starting from the TODO example is the recommended way of getting started with the plugin.

