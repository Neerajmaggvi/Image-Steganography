
# Image Steganography in C

A C-based Image Steganography project that hides secret text files inside BMP images using the Least Significant Bit (LSB) technique.

The project supports encoding secret data into an image and decoding the hidden information to retrieve the original file.

## Features

- Hide secret files inside BMP images.
- Extract hidden data from encoded BMP images.
- LSB-based encoding and decoding.
- Magic string verification.
- Automatic extraction of secret file extension and size.
- Command-line interface.
- Binary file handling using C.

## Technologies Used

- **Language:** C
- **Compiler:** GCC
- **Concepts:** Pointers, Structures, Arrays, Bitwise Operations, File Handling
- **Technique:** Least Significant Bit (LSB) Steganography
- **IDE:** Visual Studio Code

## Project Structure

```text
Image-Steganography/
│
├── main.c
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── color.h
├── Makefile
├── beautiful.bmp
├── secret.txt
└── README.md
```

## Working Principle

### 1. Encoding Process

The encoding process embeds secret file information into a BMP image.

The following information is encoded:

1. Magic string
2. Secret file extension size
3. Secret file extension
4. Secret file size
5. Secret file data

Each bit of the secret data is embedded into the Least Significant Bit of the image data bytes.

### 2. Decoding Process

The decoding process extracts the hidden information from the encoded BMP image.

The following steps are performed:

1. Open the encoded BMP image.
2. Verify the magic string.
3. Decode the secret file extension size.
4. Decode the secret file extension.
5. Decode the secret file size.
6. Extract and reconstruct the original secret file.

## Compilation

Clone the repository:

```bash
git clone https://github.com/Neerajmaggvi/Image-Steganography.git
```

Navigate to the project directory:

```bash
cd Image-Steganography
```

Compile the project:

```bash
make
```

## Execution

### Encoding

```bash
./stego -e beautiful.bmp secret.txt stego.bmp
```

**Arguments:**

- `-e` : Encoding operation
- `beautiful.bmp` : Source BMP image
- `secret.txt` : Secret file to hide
- `stego.bmp` : Output image containing hidden data

### Decoding

```bash
./stego -d stego.bmp recovered
```

**Arguments:**

- `-d` : Decoding operation
- `stego.bmp` : Encoded BMP image
- `recovered` : Output filename prefix

The decoded file extension is automatically appended to the output filename.

For example, the output file will be:

```text
recovered.txt
```

## Output Screenshots

### Encoding Successful

The screenshot below demonstrates successful encoding of secret data into a BMP image.

![Encoding Successful](screenshots/encoding-success.png)

### Decoding Successful

The screenshot below demonstrates successful extraction of the hidden secret data.

![Decoding Successful](screenshots/decoding-success.png)

## Learning Outcomes

- Understanding bitwise operations.
- Implementing LSB-based data encoding and decoding.
- Working with file pointers and binary file operations.
- Understanding structures and pointers in C.
- Handling command-line arguments.
- Applying C programming concepts to a practical project.

## Author

**Neeraj Maggavi**

GitHub: [Neerajmaggvi](https://github.com/Neerajmaggvi)

**Project:** Image Steganography using C
