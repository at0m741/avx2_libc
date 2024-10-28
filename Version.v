SIMPL_0.1 {
	global:
		strlen;
		strcmp;
		strncmp;

		memset;
		memcpy;
		memmove;


		malloc;
		free;
	local:
		*;
};

SIMPL_0.2 {
	global:
		printf;
} SIMPL_0.1;

