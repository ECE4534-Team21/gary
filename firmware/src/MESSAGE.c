#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct Message {
	unsigned char from;
	unsigned char purpose;
	unsigned int  message;
};

unsigned long encode(unsigned char from, unsigned char purpose, unsigned int message)	{
	
	unsigned long encodedMSG = (unsigned long)(from << 24) | (unsigned long)(purpose << 16) | (unsigned long) message;
	return encodedMSG;
}

struct Message decode(unsigned long message)	{

	struct Message msg;

	msg.from = (unsigned char) (message >> 24);
	msg.purpose = (unsigned char) (message >> 16);
	msg.message = (unsigned int) (message & 0xFFFF);

	return msg;
}

/* Message use example
 int main()
{
	FILE *f = fopen("encode_decode.txt", "w");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    return;
	}

	srand(time(NULL));

	int i = 0, match_counter = 0, mismatch_counter = 0;

	struct Message m1;
	unsigned char msg_from, msg_purpose;
	unsigned int msg;
	unsigned long result;

	fprintf(f, "Encode Inputs:  From   Purpose   Message  |  Encode Outputs:  Encoded Msg  |||  Decode Inputs: Encoded Msg  |  Decode Outputs:  From   Purpose   Message\n");
	fprintf(f, "________________________________________________________________________________________________________________________________________________________\n");

	for (i = 0; i < 100; i ++)	{

		msg_from = (char) (rand() % 256);
		msg_purpose = (char) (rand() % 256);
		msg = rand() % 65535;

		result = encode(msg_from, msg_purpose, msg);
		m1 = decode(result);

		fprintf(f, "\n                %-5d  %-7d   %-8u |                   %-13u|||                 %-13u|                   %-5d  %-7d   %-8u", msg_from, msg_purpose, msg, result, result, m1.from, m1.purpose, m1.message);
		if (msg_from == m1.from && msg_purpose == m1.purpose && msg == m1.message)
			match_counter += 1;
		else
			mismatch_counter += 1;
	}

	fprintf(f, "\n\nThese were %i matches AND %i mismatches.", match_counter, mismatch_counter);
	fclose(f);

    return 0;
}
 */
