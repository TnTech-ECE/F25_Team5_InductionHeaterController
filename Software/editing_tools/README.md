## Shift References
#### What the software does
Shifts References to splice in a new reference in IEEE format.
#### All dependencies
 - node.js
 - fs
 - path
#### How to Install
 1. Install node.js [https://nodejs.org/en/download](https://nodejs.org/en/download).
 2. Open `./Software/editing_tools` in a terminal.
 3. To install dependencies run: 
```bash
npm i
```
#### How to run/How to use
 1. In references add a reference with [<number>#] ie [23#]. <number> will be at the reference you want it to be and the program will shift it to splice in that number.
 2. Run:
```bash
npm run shift -- "relative path from editing_tools or static path"
``` 
#### Notes
only does one reference at a time atm will throw an error.