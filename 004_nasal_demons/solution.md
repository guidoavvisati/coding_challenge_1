# Solutions: Nasal demons

## One
```
    #define MIN(a,b) (a < b) ? (a) : (b)
```
`#define` directives should not be used for this purpose. Instead, we are 
better off defining a (possibly inline) function. This line can cause
obscure bugs because it enforces literal substitution of the expression in MIN
before the compiler runs. 
`int min(int a, int b){(a < b) ? return a; : return b;}`

## Two
```
    int xto3(volatile int *x){
        return *x * *x * *x;
    }
```
Since x is a pointer to a `int volatile`, we know that the content pointed to by x
can change at any time. Therefore, we risk to fetch 3 different values by dereferencing
x. According to me, one can either store the first value `int a = *x; return a*a*a;`
or directly drop the `*` and use the copied value of x 
`int xto3(volatile int x){return x*x*x;}`. This last option does change the signature,
therefore client code could be affected, so probably the first is a better option.

## Three

```
    int *align_to_bits(int *addr, int bits){
        return ((uintptr_t)addr) & (uintptr_t)((1<<bits)-1);
    }
```

## Four
```
    int return0(int *a, int *b) {
        *b=0;
        *a=42;
        return *b;
    }
```
Although this code works fine, I think it may just be a bad smell.
The function name does not reflect all operations that are going on
because the value of `a` is also being set to a non zero value. This
goes against the single-responsabilty principle.
