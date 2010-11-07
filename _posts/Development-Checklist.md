UI Checklist before releasing 2.0 final:

* Check that EVERY control has a text label with a mnemonic
* Test tab navigation on Windows, Unix, and (Ctrl+Fn+F1) full keyboard accessibility on Mac
* Check initial focus
* Check wxTAB_TRAVERSAL on all the dialogs
* Be careful with Raise(), Show(), and SetFocus()
* Check logical tab order (left to right, top to bottom)
* Check shift-Tab as inverse
* Make sure that there's visible focus at each stop in the tab order
* Don't do any custom mouse event handling (or check for parallel keyboard handling)
* Keep radio buttons sequential