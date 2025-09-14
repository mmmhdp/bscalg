//a0 - a7 preserved for func args
//a0 - a1 handle return value

foo:
        slli    a5,a1,32
// seem like a1 - is a kind of an index
// and a0 is a pointer to array
	//a5 = a1  << 32
        srli    t4,a5,30
	//t4 = a5  >> 30
	//so t4 = a1 << 2 = a1 * 4
        add     t4,a0,t4
	//t4 = t4 + a0
        lw      t3,0(t4)
	//t3 = mem[t4 + 0] = *(a0 + a1 * 4 + 0)
//t3 = arr[li]
        mv      a6,a0
	//a6 = a0
        sext.w  a7,a2
	// maybe a2 was unsigned of char
	//ext to word size
	//a7 = (bit 32) a2

// logically it's should be first 
// check before initialization 
        bgeu    a1,a2,epilog
	// if a1 >= a2 => jump

        slli    t1,a7,2
// a7 = ri
	// t1 = a7 << 2
	// r_idx * 4 
        add     a5,a0,t1
	// a5 = a0 + t1
// a5 - is a pointer on same arr, but 
// we point to a5 = arr + ri 
        mv      a4,a7
	// a4 = a7
// a7 = a2 = ri

loop:
        lw      a3,0(a5)
	// a3 = mem[a5 + 0]
        add     a2,a6,t1
	// a2 = a6 + t1
// a2 = a0 + 4 <=> a0+=1 
// arr pointer update
        addiw   a4,a4,-1
	// a4 -= 1
// right index is reduced by 1

        ble     a3,t3,epilog2
	// if a3 <= t3 => jump
// here are values from array under
// updated indexes are compared

// seems like below is a swap of values
// with usage of tmp value
// right into orig array value 
// under right_idx
        addiw   a7,a7,-1

        lw      a0,0(a2)
	// a0 = mem[a2 + 0]
	// a7 -= 1
// right index is reduced by 1
        sw      a3,0(a2)
	// mem[a2 + 0] = a3 
        sw      a0,0(a5)
	// mem[a5 + 0] = a0

        slli    t1,a7,2
	// t1 = a7 << 2

epilog2:
        addi    a5,a5,-4
	// a5 -= 4
// it's minus 4 byte for int
// so basically it's same as idx -= 1
        bltu    a1,a4,loop
	// if unsigned  a1 < unsigned a4 => jump

// offset prep 
        lw      t3,0(t4)
	// t3 = [t4 + 0]
        add     a6,a6,t1
	// a6 += t1

// seems like below is a swap of values with 
// extention of value with usage of tmp value
        lw      a5,0(a6)
	// a5 = mem[a6 + 0]
        sw      t3,0(a6)
	// mem[a6 + 0] = t3
        sext.w  a0,a7
	// a0 = (int 32) a7
        sw      a5,0(t4)
	// mem[t4 + 0] = a5

        ret

epilog:
        slli    t1,a7,2
	// t1 = a7 << 2
// if jump from beginning, a7 = a2
// t1 = a7 * 2

// below is a swap of values with 
// intermediate value
        add     a6,a6,t1
// a6 = original arr
	// a6 += t1
        lw      a5,0(a6)
	// a5 = mem[a6 + 0] = tmp = arr[ri]
        sw      t3,0(a6)
	// mem[a6 + 0] = t3
	// arr[ri] = arr[il]
        sw      a5,0(t4)
	// mem[t4 + 0] = a5
	// arr[li] = tmp

// a0 is return value register
        mv      a0,a2
	// a0 = a2

        ret
