# iOS reversing challenge

Author: Aneesh Maganti

This challenge prevents a login screen to the user with the username set as "OSIRIS" and the password blank. While not directly specified, the challenger can assume the 'ipa' extension corresponds to an iPhone app.

## Solution

A [.ipa](https://en.wikipedia.org/wiki/.ipa) archive should be extracted to get to the actual application. From here, I will be using IDA Pro but other decompilers should work. Tracing from the entrypoint doesn't immediately help but one of the first functions you encounter starting from the begining of the file (this behavior was consistent on IDA, Ghidra, and Binary Ninja) is the relevant function which handles the password checking. 

![IDA Function](https://github.com/user-attachments/assets/68b2a8c5-0b80-4298-8815-3e0bb67dacc2)

The data is big-endian so the words are reversed but, as seen above, after doing a check for the word "OSIRIS" for the username, a string will be constructed with the output of a function with "nxhtrnslfuuqj", "purple", "csaw_ctf{", and "}". The only thing from here is to figure out what the function is doing to that phrase. Descending into that function shows iteration over a string and performing operations involving ["Unicode Scalars"](https://developer.apple.com/documentation/swift/unicode/scalar). Even if it's too difficult to read through the code directly, the unicode scalar is a strong hint that there's some ceaser ciphering occuring (and that's correct); the phase is ciphered five characters forwards so just undo that to get "iscomingapple". Combine the pieces to get the flag.
