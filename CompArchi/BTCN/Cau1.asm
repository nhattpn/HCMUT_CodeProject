.macro writeStr(%str,%length)
    li   $v0, 15       # system call for write to file
    move $a0, $s1      # file descriptor 
    la   $a1, %str   # address of buffer from which to write
    li   $a2, %length       # hardcoded buffer length
    syscall            # write to file
.end_macro
.data
    one: .float 1.0
    four:.float 4.0
    pi:  .float 0.0
    pi_int: .word 0
    pi_frac: .word 0
    count_point: .space 0
    count:     .word 100000
    random_x: .float 0.0
    random_y: .float 0.0
    str_count_point: .space 5
    str_pi_int: .space 2
    str_dot: .asciiz "."
    str_pi_frac: .space 6
    endl: .asciiz "\n"
    file_out:  .asciiz "PI.txt"
    point_msg:     .asciiz "So diem nam trong hinh tron: " #29
    pi_msg:        .asciiz "\nSo PI tinh duoc: " #18
.text
main:
    # Set the seed for random number generation (optional)
    li $v0, 40            # Syscall number for setting the seed
    li $a0, 42           # Seed value (can be any value)
    lw $a1, count
    mtc1 $a1, $f6         # f6 contain COUNT
    li $t0, 0
    li $t1, 0
    syscall
###########################################################################
counting: 
    beq $t0, $a1, end_program
    addi $t0, $t0, 1
    # Generate two random floats in the range (0, 1)
    jal generate   # Call the function to generate a random float, result in $f0
    s.s $f12, random_x
    jal generate   # Call the function to generate another random float, result in $f1
    s.s $f12, random_y
    
    ################################### Calculate x^2 + y^2
    l.s $f12, random_x
    mul.s $f12, $f12, $f12   # x^2
    l.s $f13, random_y
    mul.s $f13, $f13, $f13   #y^2    
    add.s $f1, $f12, $f13   # x^2 + y^2
    
    #################################### Compare the result with 1.0    
    l.s $f2, one       # Load 1.0
    c.lt.s $f1, $f2       # Compare x^2 + y^2 with 1.0
    bc1t in_range         # Branch if both conditions are true
    j counting 		#loop

###########################################################################
end_program:
    sw $t1, count_point
##################################### t1->f12 and mul numpi 4 time
    mtc1 $t1, $f12
    div.s $f12, $f12, $f6
    l.s $f2, four
    mul.s $f12, $f12, $f2
    swc1 $f12, pi
    
    # save int part of pi
    l.s $f12, pi
    cvt.w.s $f0, $f12
    mfc1 $t0, $f0
    sw $t0, pi_int 
    # save frac part of pi
    l.s $f0, pi
    li $t0, 1000000
    mtc1 $t0,$f2
    cvt.s.w $f2, $f2
    mul.s $f0, $f0, $f2 # get 6 digit
    cvt.w.s $f1, $f0 # Convert to integer
    mfc1 $t0,$f1
    addi $t0,$t0,-3000000
    sw $t0, pi_frac
    # convert point incircle to string
    la $a0, count_point
    lw $a0, 0($a0)
    la $a1, str_count_point
    jal  int2str
    # convert int part of pi to string
    la   $a0, pi_int # $a0 = address of int to convert
    lw   $a0, 0($a0) # Load the integer at the address into $a0convert
    la   $a1, str_pi_int             # $a1 = address of string where converted number will be kept
    jal  int2str
    # convert frac part of pi to string
    la   $a0, pi_frac # $a0 = address of int to convert
    lw   $a0, 0($a0) # Load the integer at the address into $a0convert
    la   $a1, str_pi_frac             # $a1 = address of string where converted number will be kept
    jal  int2str
    
    # open fileout
    li $v0, 13	
    la $a0, file_out
    li $a1, 1
    li $a2, 0
    syscall
    move $s1, $v0
    # close file
    writeStr(point_msg,29)
    writeStr(str_count_point,5)
    writeStr(pi_msg,18)
    writeStr(str_pi_int,1)
    writeStr(str_dot,1)
    writeStr(str_pi_frac,6)
    li $v0, 16		
    move $a0, $s1
    syscall
    ##########################
    li $v0, 4
    la $a0, point_msg
    syscall 	#print "point"
    li $v0, 1
    move $a0, $t1
    syscall
    #########################
    li $v0, 4
    la $a0, pi_msg
    syscall 	#print "pi"
    li $v0, 2
    syscall
    ####################
 
    li $v0, 10            # Syscall number for program exit
    syscall

##################################################################### count point in range
in_range:
    addi $t1, $t1, 1
    j counting
################################# Function to generate a random float in the range (0, 1)
generate:
    li $a0, 1
    li $v0, 42           # Syscall number for generating a random number    
    syscall 
    mtc1 $a0, $f12
    div.s $f12, $f12, $f6    
    jr $ra
#################################
int2str:
    addi $sp, $sp, -4         # to avoid headaches save $t- registers used in this procedure on stack
    sw   $t0, ($sp)           # so the values don't change in the caller. We used only $t0 here, so save that.
    bltz $a0, neg_num         # is num < 0 ?
    j    next0                # else, goto 'next0'
################################
neg_num:                  # body of "if num < 0:"
    li   $t0, '-'
    sb   $t0, ($a1)           # *str = ASCII of '-' 
    addi $a1, $a1, 1          # str++
    li   $t0, -1
    mul  $a0, $a0, $t0        # num *= -1
##############################
next0:
    li   $t0, -1
    addi $sp, $sp, -4         # make space on stack
    sw   $t0, ($sp)           # and save -1 (end of stack marker) on MIPS stack
################################
push_digits:
    blez $a0, next1           # num < 0? If yes, end loop (goto 'next1')
    li   $t0, 10              # else, body of while loop here
    div  $a0, $t0             # do num / 10. LO = Quotient, HI = remainder
    mfhi $t0                  # $t0 = num % 10
    mflo $a0                  # num = num // 10  
    addi $sp, $sp, -4         # make space on stack
    sw   $t0, ($sp)           # store num % 10 calculated above on it
    j    push_digits          # and loop
###############################
next1:
    lw   $t0, ($sp)           # $t0 = pop off "digit" from MIPS stack
    addi $sp, $sp, 4          # and 'restore' stack
    
    bltz $t0, neg_digit       # if digit <= 0, goto neg_digit (i.e, num = 0)
    j   pop_digits           # else goto popping in a loop
###############################
neg_digit:
    li   $t0, '0'
    sb   $t0, ($a1)           # *str = ASCII of '0'
    addi $a1, $a1, 1          # str++
    j    next2                # jump to next2
##############################
pop_digits:
    bltz $t0, next2           # if digit <= 0 goto next2 (end of loop)
    addi $t0, $t0, '0'        # else, $t0 = ASCII of digit
    sb   $t0, ($a1)           # *str = ASCII of digit
    addi $a1, $a1, 1          # str++
lw   $t0, ($sp)           # digit = pop off from MIPS stack 
addi $sp, $sp, 4          # restore stack
j    pop_digits           # and loop
#############################
next2:
sb  $zero, ($a1)          # *str = 0 (end of string marker)

lw   $t0, ($sp)           # restore $t0 value before function was called
addi $sp, $sp, 4          # restore stack
jr  $ra                   # jump to caller