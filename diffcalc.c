#include <stdio.h>
#include <stdint.h>
#include "cipher.h"

uint16_t pow2(uint16_t num);

uint16_t best_dA = 0;
uint16_t best_dB = 0;
uint16_t best_diff_score = 0;

int main(int argc, char **argv)
{
	//Take every diff. dA (2^12 combs), every diff. dB (2^12 combs) , iterate through every value A0 and A1 with diff. dA (2^12 combs), if it gets a diff. of dB as output ++ the score of dA -> dB, if higher than best diff. set new best diff.
	
	for(uint16_t dA = 1; dA < pow2(12); dA++)
	{
		printf("Running for dA: %d\nBest diff so far: %d -> %d\nHits: %d out of %d", dA, best_dA, best_dB, best_diff_score, pow2(12));
		fflush(stdout);
		for(uint16_t dB = 0; dB < pow2(12); dB++)
		{
			uint16_t current_score = 0;
			for(uint16_t A0 = 0; A0 < pow2(12); A0++)
			{
				//XOR A0 with the input differential to get a B0 such that A0 ^ B0 = dA (this is our chosen pair basically)
				uint8_t B0 = A0 ^ dA;
				
				uint8_t A0_0 = A0 % 16; //First 4 bits
				uint8_t A0_1 = (A0 >> 4) % 16; //Next 4 bits
				uint8_t A0_2 = (A0 >> 8) % 16; //Final 4 bits
				uint8_t B0_0 = B0 % 16;
				uint8_t B0_1 = (B0 >> 4) % 16;
				uint8_t B0_2 = (B0 >> 8) % 16;
				
				perform_ks_rounds(&A0_0, &A0_1, &A0_2);
				perform_ks_rounds(&B0_0, &B0_1, &B0_2);
				
				//Reconstruct A0 and B0
				A0 = (A0_0) | (A0_1 << 4) | (A0_2 << 8);
				B0 = (B0_0) | (B0_1 << 4) | (B0_2 << 8);
				
				if(A0 ^ B0 == dB)
					current_score++;
			}
			
			if(current_score > best_diff_score)
			{
				best_dA = dA;
				best_dB = dB;
				best_diff_score = current_score;
			}
		}
	}
	
	return 0;
}

uint16_t pow2(uint16_t num)
{
	return (1 << num);
}
