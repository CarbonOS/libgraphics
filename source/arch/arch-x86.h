#ifndef __ASM_I386_H__
#define __ASM_I386_H__

/* Extern Symbol */
#define EXTERN(x) \
.extern x;

/* Function Start */
#define ENTRY(x) \
.globl x; \
.type x, @function; x:

/* Function Start */
#define	END(entry) \
.size entry, . - entry

#endif /* !__ASM_I386_H__ */
