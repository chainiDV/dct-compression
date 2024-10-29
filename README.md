## Introduction
Basic Discrete Cosine Transform (DCT) with quantization and Run Length Encoder(RLE) implementation

## Input
Binary expects two filenames as arguments:

|             	| Argument 0                                 	|                     Argument 1                    	|
|-------------	|--------------------------------------------	|:-------------------------------------------------:	|
| name        	| Pixel Matrix                               	|                Quantization matrix                	|
| format      	| Plain-text                                 	|                     Plain-text                    	|
| description 	| 8x8 Pixel matrix to be transformed  by DCT 	| Quantization matrix to apply to  the DCTed matrix 	|

##Workflow
8x8 Matrix is transformed by Discrete Cosine Transform. Afterwards, these DCT coefficients are quantized using the quantization matrix. Finally, Run Length Encoder is applied


### DCT
A DCT is a Fourier-related transform similar to the discrete Fourier transform (DFT), but using only real numbers. Its usage in image compression lies in its property that, for a typical image, most of the visually significant information about the image is concentrated in just a few coefficients of the DCT. 

```math
\begin{equation}
DCT(k, l) = \frac{1}{4} \, C(k) C(l) \sum_{x=0}^{N-1} \sum_{y=0}^{N-1} \text{matrix}(x, y) \cos\left( \frac{(2x + 1) k \pi}{2N} \right) \cos\left( \frac{(2y + 1) l \pi}{2N} \right)
\end{equation}
```


```math
\begin{equation}
C(u) = 
\begin{cases} 
\frac{1}{\sqrt{2}} & \text{si } u = 0, \\
1 & \text{si } u \neq 0.
\end{cases}
\end{equation}
```

### Quantize Matrix
In this step, DCT coefficients are quantizied using the quantization matrix to obtain greater compression by discarding non-significant visual information. These are the higher frequencies, which the human visual system is less sensitive, that are quantized more.  The result are quantized DCT coefficients. 
Note: This is the most lossy operation.

### Run Length Encoder
The last step is to apply the Run Length Encoder (RLE). It's a form of lossless data compression in which runs of data (consecutive occurrences of the same data value) are stored as a single occurrence of that data value and a count of its consecutive occurrences. Components are read in zig-zag order to maximize the length of runs of zeroes.

##Examples
Two 8x8 pixel matrix and two 8x8 quantization matrix are provided.
