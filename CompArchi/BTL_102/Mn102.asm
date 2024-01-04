#-------------------------------------------------------------------------------
# Merge Sort
# Input: n : so luong phan tu trong mang
#	 filename : dia chi file de doc	 
# Output: xuat ra man hinh mang da sap xep 
#-------------------------------------------------------------------------------
.macro copyToArr %filename
	la   $a0, filename 				
	addi   $a1, $zero, 0 	#flag=0:read only		
	addi   $v0, $zero, 13  			
	syscall    			
	#bltz	$v0,baoloi
	sw	$v0,fdescr		
	# read to file
	 # 60 byte so thuc
	lw   $a0,fdescr
	la   $a1, %filename   		
	addi   $a2, $zero, 60   
	addi   $v0, $zero, 14      		
	syscall     
		         		
  	# dong file
	lw	$a0,fdescr
	addi	$v0,$zero,16
	syscall
.end_macro
.data
n:			.word	15
str_sort:		.asciiz "\nSort:  "
str_sorted_array:	.asciiz	"\nSorted Array:  "
str_unsorted_array: 	.asciiz "\nUnsorted Array:  "
str_left:		.asciiz	"\nLeft: "
str_right:		.asciiz	"\nRight: "
str_mid:		.asciiz	"\nMid: "
space:			.asciiz	" "
filename: 		.asciiz "FLOAT15.BIN" #"D:\\Computer Science\\ComputerArchitecture\\MARS\\FLOAT15.BIN" # this must be the absolute address
fdescr:			.word 0
array:			.space 60  # float array[15] || this changed after mergesort
result_arr:		.space	60 # for printing
.text                                                                                                                                                                                                                 
.globl main
main:
	copyToArr array
	copyToArr result_arr
	lw	$s0, n			# s0 = n = 20
	jal 	printUnsorted
	jal 	InitValue
	jal 	MergeSort		
	jal 	printSorted		
	j 	Exit			
#Merge Sort
InitValue:
	la	$a1, array	# $a1 = address of array
	lw	$a2, n		# $a2 = n = 20
	subi	$a2, $a2, 1	# $a2-- (n--)
	li	$a3, 0
	jr 	$ra
#-----------------------------------------------------------------------------------------
# Ham MergeSort:  mergesort( $a0, $s2, $s3 ) = mergesort( adrress of array, start index, end index)
# Input: $a0=addr(a[0]), $a1 = addr(a[0] + i)
# Output: none
#-----------------------------------------------------------------------------------------
MergeSort: 
 	addi	$sp, $sp, -20  		# create Stack
 	sw  	$ra, 16($sp)   		# save return adress
 	sw  	$s1, 12($sp)   		# save array address
 	sw  	$s2, 8($sp)  		# save right
 	sw  	$s3, 4($sp)   		# save low
 	sw  	$s4, 0($sp)   		# save mid
 	or  	$s1, $zero, $a1  	# $s1 <- array address	
 	or  	$s2, $zero, $a2  	# $s2 <- right	
 	or  	$s3, $zero, $a3  	# $s3 <- left	
 	slt 	$t3, $s3, $s2   	# low < high? ($s3 < $s2)?: $t3 = 1; $t3 = 0
 	beq 	$t3, $zero, DONE  	# if $t3 == 0, DONE
 	add 	$s4, $s3, $s2  		# left + right
 	div 	$s4, $s4, 2   		# $s4 <- (left+right)/2
 	or  	$a2, $zero, $s4 	# $a2 = mid
 	or  	$a3, $zero, $s3 	# $a3 = left
 	jal 	MergeSort   		# recursive call for (array, left, mid)
# mergesort (arr, mid+1, high)
 	addi	$t4, $s4, 1   		# $t4 = $s4+1 = mid + 1 
 	or  	$a3, $zero, $t4 	# $a3 = $t4 = mid + 1
 	or  	$a2, $zero, $s2 	# $a2 = right
 	jal 	MergeSort   		# recursive call for (array, mid+1, high) 
 	or 	$a1, $zero, $s1 	# a1 = arr adress
 	or 	$a2, $zero, $s2 	# a2 = high 	 
 	or 	$a3, $zero, $s3 	# a3 = low 	
 	or 	$a0, $zero, $s4 	# a0 = mid 	
 	jal 	Merge   		# jump to merge (array, high, low, mid) 
DONE:	
 	lw 	$ra, 16($sp)  		# load return address 	
 	lw 	$s1, 12($sp)  		# load array address	
	lw 	$s2, 8($sp)  		# load right	
 	lw 	$s3, 4($sp)  		# load left 	
 	lw 	$s4, 0($sp)  		# load mid 	
 	addi 	$sp, $sp,  20 		# delete stack 	
 	jr 	$ra    			# jump to return address
#-------------------------------------------------------------------------------
# Ham merge: mergesort(address of arr, start index, mid index, end index)
# Input: $s1 - array, $s2 - start, $s3 - end, $s4 - mid
# Output: none
#-------------------------------------------------------------------------------
Merge:  
	addi	$sp, $sp, -20  		# create stack 	
 	sw  	$ra, 16($sp)   		# save return adress 	
 	sw  	$s1, 12($sp)   		# save array address	
 	sw  	$s2, 8($sp)  		# save right 	
 	sw  	$s3, 4($sp)   		# save low 	
 	sw  	$s4, 0($sp)   		# save mid 	
 	or  	$s1, $zero, $a1  	# $s1 = array address 	
 	or  	$s2, $zero, $a2  	# $s2 <- right 	
 	or 	$s3, $zero, $a3  	# $s3 <- left  			
 	or 	$s4, $zero, $a0  	# $s4 <- mid		
# Print last merge
 	la 	$a0, str_sort		# print string	
	li 	$v0, 4
	syscall	
	la	$a1, result_arr		# $a1 = address of arraykq
	addi	$t1, $0, 0
	jal	printArray
	
# print left, mid, right
	la	$a1, result_arr
	addi	$t1, $0, 0		# int i
 	
 	la 	$a0, str_left		# print string
	li 	$v0, 4
	syscall	
	
	addi 	$a0, $s3, 0		# print left = $s3	
	li 	$v0, 1	
	syscall	
	
	la 	$a0, str_mid		# print string
	li 	$v0, 4
	syscall
	
	move	$a0, $s4		# print mid = $s4	
	li 	$v0, 1	
	syscall
	
	la 	$a0, str_right		# print string
	li 	$v0, 4
	syscall
	
	move	$a0, $s2		# print right = $s2	
	li 	$v0, 1
	syscall
continue:
# update
	or  	$a1, $zero, $s1  	# arr address 	
 	or  	$a2, $zero, $s2 	# $a2 = $s2 = right 	
 	or  	$a3, $zero, $s3  	# $a3 = $s3 = left 	
 	or  	$a0, $zero, $s4  	# $a0 = $s4 = mid 	
 	or  	$t1, $zero, $s3  	# $t1 = i = left 	
 	or  	$t2, $zero, $s4  	# $t2 = j = mid	 		
 	addi	$t2, $t2, 1   	 	# $t2 = j = mid + 1 	 		
 	or  	$t3, $zero, $a3  	# $t3 = k = left = count variables of arraykq	 								
# i -> mid and j -> right ( arrays )
while: 
# while condition: i <= mid va j <= right	
 	slt  	$t4, $s4, $t1  		# mid < i (i <= mid)  	
 	bne  	$t4, $zero, while2 	# go to while 2 if i >= mid 	
 	slt  	$t5, $s2, $t2  		# high < j (j <= high) 	
 	bne  	$t5, $zero, while2 	# && go to while2 if j >= right 	
 	sll  	$t6, $t1, 2  		# t6 = i * 4  	
 	add  	$t6, $s1, $t6 		# $t6 = address a[i]	 	 		
 	lwc1	$f5, 0($t6) 		# $f5 = a[i] 	
	sll  	$t7, $t2, 2  		# j * 4 	
 	add  	$t7, $s1, $t7 		# $t7 = address a[j] 		
 	lwc1	$f6, 0($t7)		# $f6 = a[j] 		
 	c.lt.s	$f5, $f6 	
 	bc1f	Else 	
 	sll  	$t8, $t3, 2  		# k * 4 	
 	la   	$a0, result_arr  		# $a0 = dia chi arraykq 	
 	add  	$t8, $a0, $t8 		# $t8 = addr c[k] 	
 	swc1   	$f5, 0($t8)  		# c[k] = a[i]   	
 	addi 	$t3, $t3, 1  		# k++	arr_result 	
 	addi 	$t1, $t1, 1  		# i++	arr low -> mid
 	j 	while 

Else:   # a[i] >= a[j}
 	sll  	$t8, $t3, 2  		# i = i * 4
 	la   	$a0, result_arr  	# $a0 = address arr_result
 	add  	$t8, $a0, $t8 		# $t8 = address c[k]
 	swc1  	$f6, 0($t8)  		# c[k] = a[j]
 	addi 	$t3, $t3, 1  		# k++ 	temp 
 	addi 	$t2, $t2, 1  		# j++	arr mid + 1 -> high 
 	j 	while

# Add from low to mid
while2: 
	
 	slt  	$t4, $s4, $t1  		# mid < i (i <= mid)
 	bne  	$t4, $zero, while3 	# go to while3 if i >= mid  	
 	sll  	$t6, $t1, 2  		# i * 4 	
 	add  	$t6, $s1, $t6 		# $t6 = address a[i] 	
 	lw  	$s5, 0($t6)  		# $s5 = a[i]  	
 	sll  	$t8, $t3, 2  		# i * 4 	
 	la   	$a0, result_arr  	# $a0 = address arr_ressult	
 	add  	$t8, $a0, $t8 		# $t8 = address c[k] 	
 	sw   	$s5, 0($t8)  		# c[k] = a[i] 	
 	addi 	$t3, $t3, 1  		# k++  	
 	addi 	$t1, $t1, 1  		# i++ 	
 	j while2

# Add from mid + 1 to high
while3: 
	slt  	$t5, $s2, $t2  		# high < j (j >= right) 	
 	bne  	$t5, $zero, start 	# go to for loop if j >= right  	
	sll  	$t7, $t2, 2  		# i*4	
 	add  	$t7, $s1, $t7 		# $t7 = address a[j] 	
 	lw   	$s6, 0($t7)  		# $s6 = a[j]  	
 	sll  	$t8, $t3, 2  		# i*4 	
 	la   	$a0, result_arr   	# $a0 = address arr_ressult	
 	add  	$t8, $a0, $t8 		# $t8 = address c[k] 	
 	sw   	$s6, 0($t8)  		# c[k] = a[j]	
	addi 	$t3, $t3, 1  		# k++ 	
 	addi 	$t2, $t2, 1  		# j++ 	
	j 	while3

# Update what's left 
start:
	or   	$t1, $zero, $s3 	# i <- left
forloop:	
 	slt  	$t5, $t1, $t3  		# i < k 	
 	beq  	$t5, $zero, DONE 	# complete 
 	sll  	$t6, $t1, 2 		# i * 4 
 	add  	$t6, $s1, $t6 		# $t6 = address a[i]
 	sll  	$t8, $t1, 2  		# i * 4
 	la   	$a0, result_arr  	# $a0 = dia chi array_res 
 	add  	$t8, $a0, $t8  		# $t8 = address array_res[i] 
 	lw   	$s7, 0($t8)  		# $s7 = array_res[i]
 	sw   	$s7, 0($t6)  		# a[i]=	array_res[i] 
 	addi 	$t1, $t1, 1  		# i++ 	
 	j 	forloop
  
printUnsorted:	
	la	$a1, array		# $a1 = address of array	
	addi 	$t1, $0, 0		# $t1 = 0	
	la 	$a0, str_unsorted_array		
	li 	$v0, 4
	syscall	
	j	printArray

printSorted:
	la	$a1, array	
	addi	$t1, $0, 0
	la 	$a0, str_sorted_array		
	li 	$v0, 4
	syscall	
	
printArray:
# If loopNum > 20 => break;	
	slt 	$t2, $t1, $s0	
	beq 	$t2, $0, Break
# Print array[i]	
	lwc1 	$f12, ($a1)		# load to f12
	li 	$v0, 2			# print float
	syscall
# print space	
	la	$a0, space	
	li 	$v0, 4
	syscall
#  (i++)	
	addi 	$t1, $t1, 1
	addi 	$a1, $a1, 4	
	j 	printArray
Break:	
	jr	$ra	
# End of nightmare
	Exit:	
	li 	$v0, 10
	syscall
