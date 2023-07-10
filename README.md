# SPHINXHash

## Introduction

This project is dedicated to the world community as an Open-source Post-quantum SPHINXChain blockchain, means anyone can join and contribute based on his/ her passion and skills. SPHINX is a blockchain protocol designed to provide secure and scalable solutions in the post-quantum era.

This repository contains code for the SPHINXHash project, which is a mining module for the SPHINX blockchain framework. The SPHINXHash aims to provide a `Hash function` in the blockchain systems

## Components

### A lattice-based construction

In cryptography, SWIFFT is a collection of provably secure hash functions based on the concept of the fast Fourier transform (FFT). It distinguishes itself by providing a mathematical proof of its security and utilizing the LLL basis reduction algorithm. SWIFFT's security is linked to the difficulty of finding short vectors in cyclic/ideal lattices, making collision finding a challenging task. This property offers a stronger security guarantee compared to most other cryptographic hash functions.

Despite its provable security and desirable cryptographic and statistical properties, SWIFFT is not designed as a general-purpose hash function. It does not function as a pseudorandom function and is not suitable for applications requiring a random oracle. While SWIFFT achieves a reasonable throughput of 40Mbit/s on a 3.2 GHz Intel Pentium 4, it is less efficient than traditional hash functions that lack provable collision-resistance. As a result, SWIFFT finds practical use in scenarios where the proof of collision-resistance holds significant value, such as long-term trustworthy digital signatures.

A modified version of SWIFFT called SWIFFTX was proposed as a candidate for the SHA-3 function in the NIST hash function competition. However, it was not selected in the first round of evaluations. Today we known that `Lattice-Based` cryptographic algorithms are gaining attention as promising solutions to mitigate security risks posed by quantum computers.

[SPHINXHash](https://github.com/ChyKusuma/SPHINXHash) utilizes the [SWIFFTX](https://github.com/ChyKusuma/SPHINXHash/blob/main/SWIFFTX_Report.pdf) as main hash function, its involves iteratively applying a compression function to the input data. The compression function in SWIFFTX is based on ideal lattices, which are mathematical structures with desirable properties for cryptographic purposes.

[SWIFFTX](https://github.com/ChyKusuma/SPHINXHash/blob/main/SWIFFTX_Report.pdf) rely on the HAIFA (HAsh Iterative FrAmework) construction, combined with the use of ideal lattices, allows SWIFFTX to achieve its cryptographic goals, such as collision resistance and preimage resistance.

During the NIST hash function competition in 2008, SWIFFTX was submitted as a candidate along with other hash functions, as mentioned earlier. While it did not advance to the final round of the competition, SWIFFTX's utilization of ideal lattices and the HAIFA construction contributed to the broader research and understanding of hash function design.

The SWIFFTX compression functions are designed with simplicity and mathematical elegance, which facilitates analysis and optimization. Notably, they possess two unique characteristics:

Asymptotic Security Proof: It is formally proven that finding a collision in a randomly-selected SWIFFTX compression function is at least as difficult as finding short vectors in cyclic/ideal lattices in the worst-case scenario. This provides a strong foundation for its security guarantees.

High Parallelizability: SWIFFTX's compression function enables efficient implementations on modern microprocessors, even without relying on multi-core capabilities. This is achieved through an innovative cryptographic utilization of the Fast Fourier Transform (FFT).

The core component of SWIFFTX is the SWIFFT family of compression functions, initially introduced in the 2008 workshop on Fast Software Encryption (Lyubashevsky et al., FSE’08). Careful attention was given to address a major drawback of SWIFFT, which is linearity, while preserving its provable collision resistance.

Specifically, the SWIFFTX compression function maps 2048 input bits to 520 output bits. The chosen mode of operation is HAIFA (Biham and Dunkelman, 2007), resulting in a hash function capable of accommodating inputs of any length up to 264 − 1 bits. The resulting message digests align with the required lengths of SHA-3, including 224, 256, 384, and 512 bits.


### The main component of the SWIFFTX compression function is the SWIFFT function. 

It takes a specific input format and produces a sequence of values as output. Here is a description of the SWIFFT function:

Input Format:
The SWIFFT function takes `m = 32` (or in one special case, `m = 25`) 64-bit words, denoted as `x1,...,xm,` resulting in a total of 2048 (or 1600) bits. These words are indexed by fixed "randomizer" elements `a1,0, . . . , am,63 ∈ Z257,` which are uniformly random integers modulo 257.

Mathematical Description:
`Let rev: {0,...,63} → {0,...,63}` be the "bit-reversal" function, which rearranges the bits of a 6-bit binary number. SWIFFT operates as follows:

1. Permute the bits `xj,0, . . . , xj,63` in each word `xj` using the bit-reversal function to obtain `xj,rev(0), . . . , xj,rev(63)`.
2. Interpret each (permuted) word `xj,rev(0),...,xj,rev(63`) as coefficients of a polynomial `pj(α)` of degree (at most) `63:
   pj(α) = xj,rev(0) + xj,rev(1) · α + ... + xj,rev(63) · α^63`.
3. Evaluate each polynomial `pj(·)` on all the odd powers of `ω = 42` modulo `257: ω, ω^3, ω^5, ..., ω^127`.
4. Multiply each resulting value `pj(ω^2i+1)` by `aj,i` and `sum` them over all `j` to obtain `zi' = a1,i · p1(ω^2i+1) + ... + am,i · pm(ω^2i+1)`.

Evaluating `pj(α)` on all odd powers of `ω` can be performed in various ways, with one efficient method involving pre-multiplication of coefficients by `ω^i` and subsequent evaluation on all powers of `ω^2`.

### Fast Fourier Transform (FFT) algorithm. 

The choice of parameters, specifically `ω = 42` and modulus `257`, was primarily driven by efficiency considerations. Notably, the cryptographic strength of the SWIFFTX function is largely independent of these specific parameter values. In fact, during the post-processing stage of SWIFFTX, an instantiation of SWIFFT with a different modulus is employed.

Since `257` is a prime number, the `rin`g of integers `modulo 257` forms a finite field. The multiplicative group of this field is cyclic and consists of exactly 256 elements. It is worth noting that any generator of the `multiplicative` group has an order of `256`. In the context of SWIFFTX, we require an element of order `128`, which can be obtained by squaring any generator from the `multiplicative` group. The value `ω = 42` serves as one such element with an order of `128`. Additionally, `ω = 42` possesses additional properties that facilitate highly optimized implementations.

###  Converted to bytes
The output of the SWIFFT function is comprised of elements in `Z257`, we need a function that converts them into binary quantities for further use. We perform a simple change of base from `257` to `256` by taking groups of 8 elements `z0′ ,...,z7′ ∈ Z257` and producing 8 elements `z0,...,z7 in Z256` and a bit `b ∈ {0,1}.

In the SWIFFTX compression function, the ConvertToBytes procedure combines bits from 8 groups to form one byte. Consequently, ConvertToBytes is an injective function that maps 64 elements of `Z257` into 65 bytes.

The linearity of the SWIFFT functions in the inner and outer layers of SWIFFTX is disrupted by two factors: the base change performed by ConvertToBytes and an S-Box operation. The S-Box is a simple permutation that maps one byte to another byte, ensuring it is a permutation and lacking any trapdoors. Notably, the S-Box is constructed using the digits of `'e'` in a specific manner.

### SWIFFTX Compression Function (Putting Everything Together)

The SWIFFTX compression function takes a 2048-bit input and applies three SWIFFT functions with distinct randomizers. The FFT operation needs to be performed only once for each set of input blocks. The resulting intermediate values are computed as follows:

for `i` in range`(7)`:
    `zi·257 = zi·256 + b·256`

Here, `'b'` represents the bit from each group of 8. The computed values are then used in subsequent computations.

The output of the three SWIFFT functions is fed into the ConvertToBytes function, resulting in a total of 195 bytes `(3 × 65)`. Each byte is then processed through the S-box, completing the linearity disruption in SWIFFTX.

The arrangement of these 195 bytes, 5 bytes corresponding to S-box(0) are appended, resulting in a total of 200 bytes or 1600 bits.

This 1600-bit output serves as the input for the subsequent SWIFFT. The output of this SWIFFT is then passed through the ConvertToBytes function, resulting in 520 bits. These bits can either be fed into the next compression function or directed to the FinalTransform function (described below).


More detil about SWIFFTX you can find the document here [SWIFFTX](https://github.com/ChyKusuma/SPHINXHash/blob/main/SWIFFTX_Report.pdf)


### FinalTransform

The output of SWIFFTX, while almost regularly distributed over the domain `Z64`, needs to be uniformly distributed over `Z512` for the entire hash function. However, when converted to 65 bytes using the ConvertToBytes function, the resulting 520 bits exhibit statistical bias. To address this, after processing the final block of the input, it is necessary to convert these 520 skewed bits into 512 uniformly-distributed bits. Our objective is to preserve the security proof while achieving this transformation, which is achieved by performing an operation equivalent to evaluating an additional SWIFFT function with a 520-bit input.

To accomplish this, 520 bits to 576 bits by padding them with zero bits. Next, we break these bits into 9 groups of 64 bits, treating each group as a polynomial  `'xi'` of degree at most 63. We leverage the 576 randomizer elements that were previously created and create 9 polynomials `'pi'`. Then, we compute the expression `x0p0 + x1p1 + ... + x8p8` over the ring `Z256[α]/(α64 + 1)`. The result is a polynomial of degree 63 whose coefficients are elements modulo 256, representing bytes. This process ensures the generation of the required 512 bits for the final transformed output.


### NOTE

The code in the repository is a part of the SPHINX blockchain algorithm, which is currently in development and not fully integrated or extensively tested for functionality. The purpose of this repository is to provide a framework and algorithm for the `main hash function` scheme in the SPHINX blockchain project.

As the project progresses, further updates and enhancements will be made to ensure the code's stability and reliability. We encourage contributors to participate in improving and refining the `SPHINXHash` algorithm by submitting pull requests and providing valuable insights.

We appreciate your understanding and look forward to collaborative efforts in shaping the future of the SPHINX blockchain project.

## Getting Started
To get started with the SPHINX blockchain project, follow the instructions below:

1. Clone the repository: `git clone https://github.com/ChyKusuma/SPHINXChain.git`
2. Install the necessary dependencies (List the dependencies or provide a link to the installation guide).
3. Explore the codebase to understand the project structure and components.
4. Run the project or make modifications as needed.


## Contributing
We welcome contributions from the developer community to enhance the SPHINX blockchain project. If you are interested in contributing, please follow the guidelines below:

1. Fork the repository on GitHub.
2. Create a new branch for your feature or bug fix: `git checkout -b feature/your-feature-name` or `git checkout -b bugfix/your-bug-fix`.
3. Make your modifications and ensure the code remains clean and readable.
4. Write tests to cover the changes you've made, if applicable.
5. Commit your changes: `git commit -m "Description of your changes"`.
6. Push the branch to your forked repository: `git push origin your-branch-name`.
7. Open a pull request against the main repository, describing your changes and the problem it solves.
8. Insert your information (i.e name, email) in the authors space.

## License
Specify the license under which the project is distributed (MIT License).

## Contact
If you have any questions, suggestions, or feedback regarding the SPHINX blockchain project, feel free to reach out to us at [sphinxfounders@gmail.com](mailto:sphinxfounders@gmail.com).
