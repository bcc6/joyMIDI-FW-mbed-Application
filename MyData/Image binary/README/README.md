#lcd-image-converter
	http://www.riuson.com/lcd-image-converter/others/command-line/hex2bin

1. Open bitmap file (128 x 32 pixel, Mono color)

2. Options > Conversion > Prepare > scanning
	Left to Right
	Forward
	Bands: 8px
	Use custom script:
		```
		var bandY = 0;
		do {
		    for (var x = 0; x < image.width; x++) {
		        for (var y = (image.bandSize - 1); y >= 0; y--) {
		            image.addPoint(x, bandY + y);
		        }
		    }
		    bandY += image.bandSize;
		} while (bandY < image.height);
		```

3. Options > Conversion > Prepare > Preprocessing
	Monochrome

4. Options > Conversion > Image > Data
	Prefix: "0x"
	Suffix: ""
	Delimiter: " "  <-- SPACE

5. File > Convert
	Output xxx.c file.

6. Convert Hex file (xxx.c) to binary file.
	> .\lcd-image-converter.exe --mode=hex2bin --input=test.c --output=test.bin

#HHD Hex Editor Neo

7. Strip head(4bytes) and tail(1bytes) unused bytes.

8. Merge files

