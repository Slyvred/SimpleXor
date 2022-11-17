# SimpleXor
This command line tool allows for file encryption using a xor and also features a basic frequency analysis tool.

## Usage:

```console
foo@bar:~$ ./SimpleXor {file} {key} // Xoring a file
foo@bar:~$ ./SimpleXor {file} // Analyzing a file
```

## Notes:  
- When you enter a password to encrypt/decrypt a file the password is hashed using the sha256 algorithm.
- This tools works for file up to ~1.5 Gb in size.
- Do not use this to protect actual sensitive informations, this project was intented to make time pass and is probably poorly coded and secured.
