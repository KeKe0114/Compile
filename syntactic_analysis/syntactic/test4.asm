.data
$str1: .asciiz "\n"
$str2: .space 2
.text
add $fp, $zero, $sp
jal main
li $v0, 10
syscall
.data
arr: .space 4000
tmp: .space 4000
.text
merge:
sw $ra, 4($sp)
sw $fp, 8($sp)
sw $sp, 12($sp)
add $fp, $sp, $zero
add $sp, $sp, 316
la $t0, 44($fp)
li $t1, 0
sw $t1, 0($t0)
lw $t0, 44($fp)
la $t1, 32($fp)
sw $t0, 0($t1)
la $t0, 48($fp)
li $t1, 0
sw $t1, 0($t0)
lw $t0, 48($fp)
la $t1, 36($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 52($fp)
sw $t0, 0($t1)
lw $t0, 52($fp)
la $t1, 40($fp)
sw $t0, 0($t1)
Label1:
lw $t0, 32($fp)
la $t1, 56($fp)
sw $t0, 0($t1)
lw $t0, 20($fp)
la $t1, 60($fp)
sw $t0, 0($t1)
lw $t0, 56($fp)
lw $t1, 60($fp)
slt $t9, $t0, $t1
bnez $t9, Label2
lw $t0, 36($fp)
la $t1, 64($fp)
sw $t0, 0($t1)
lw $t0, 28($fp)
la $t1, 68($fp)
sw $t0, 0($t1)
lw $t0, 64($fp)
lw $t1, 68($fp)
slt $t9, $t0, $t1
beqz $t9, Label3
lw $t0, 16($fp)
la $t1, 72($fp)
sw $t0, 0($t1)
lw $t0, 32($fp)
la $t1, 76($fp)
sw $t0, 0($t1)
lw $t0, 72($fp)
lw $t1, 76($fp)
la $t2, 80($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 80($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 84($fp)
sw $t1, 0($t2)
lw $t0, 24($fp)
la $t1, 88($fp)
sw $t0, 0($t1)
lw $t0, 36($fp)
la $t1, 92($fp)
sw $t0, 0($t1)
lw $t0, 88($fp)
lw $t1, 92($fp)
la $t2, 96($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 96($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 100($fp)
sw $t1, 0($t2)
lw $t0, 84($fp)
lw $t1, 100($fp)
sgt $t9, $t0, $t1
beqz $t9, Label4
lw $t0, 40($fp)
la $t1, 104($fp)
sw $t0, 0($t1)
lw $t0, 24($fp)
la $t1, 108($fp)
sw $t0, 0($t1)
lw $t0, 36($fp)
la $t1, 112($fp)
sw $t0, 0($t1)
lw $t0, 108($fp)
lw $t1, 112($fp)
la $t2, 116($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 116($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 120($fp)
sw $t1, 0($t2)
la $t0, tmp
la $t1, 104($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 120($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
lw $t0, 36($fp)
la $t1, 124($fp)
sw $t0, 0($t1)
la $t0, 128($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 124($fp)
lw $t1, 128($fp)
la $t2, 132($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 132($fp)
la $t1, 36($fp)
sw $t0, 0($t1)
j Label5
Label4:
lw $t0, 40($fp)
la $t1, 136($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 140($fp)
sw $t0, 0($t1)
lw $t0, 32($fp)
la $t1, 144($fp)
sw $t0, 0($t1)
lw $t0, 140($fp)
lw $t1, 144($fp)
la $t2, 148($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 148($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 152($fp)
sw $t1, 0($t2)
la $t0, tmp
la $t1, 136($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 152($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
lw $t0, 32($fp)
la $t1, 156($fp)
sw $t0, 0($t1)
la $t0, 160($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 156($fp)
lw $t1, 160($fp)
la $t2, 164($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 164($fp)
la $t1, 32($fp)
sw $t0, 0($t1)
Label5:
lw $t0, 40($fp)
la $t1, 168($fp)
sw $t0, 0($t1)
la $t0, 172($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 168($fp)
lw $t1, 172($fp)
la $t2, 176($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 176($fp)
la $t1, 40($fp)
sw $t0, 0($t1)
j Label6
Label3:
Label7:
lw $t0, 32($fp)
la $t1, 180($fp)
sw $t0, 0($t1)
lw $t0, 20($fp)
la $t1, 184($fp)
sw $t0, 0($t1)
lw $t0, 180($fp)
lw $t1, 184($fp)
slt $t9, $t0, $t1
bnez $t9, Label8
lw $t0, 40($fp)
la $t1, 188($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 192($fp)
sw $t0, 0($t1)
lw $t0, 32($fp)
la $t1, 196($fp)
sw $t0, 0($t1)
lw $t0, 192($fp)
lw $t1, 196($fp)
la $t2, 200($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 200($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 204($fp)
sw $t1, 0($t2)
la $t0, tmp
la $t1, 188($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 204($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
lw $t0, 40($fp)
la $t1, 208($fp)
sw $t0, 0($t1)
la $t0, 212($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 208($fp)
lw $t1, 212($fp)
la $t2, 216($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 216($fp)
la $t1, 40($fp)
sw $t0, 0($t1)
lw $t0, 32($fp)
la $t1, 220($fp)
sw $t0, 0($t1)
la $t0, 224($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 220($fp)
lw $t1, 224($fp)
la $t2, 228($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 228($fp)
la $t1, 32($fp)
sw $t0, 0($t1)
j Label7
Label8:
Label6:
j Label1
Label2:
Label9:
lw $t0, 36($fp)
la $t1, 232($fp)
sw $t0, 0($t1)
lw $t0, 28($fp)
la $t1, 236($fp)
sw $t0, 0($t1)
lw $t0, 232($fp)
lw $t1, 236($fp)
slt $t9, $t0, $t1
bnez $t9, Label10
lw $t0, 40($fp)
la $t1, 240($fp)
sw $t0, 0($t1)
lw $t0, 24($fp)
la $t1, 244($fp)
sw $t0, 0($t1)
lw $t0, 36($fp)
la $t1, 248($fp)
sw $t0, 0($t1)
lw $t0, 244($fp)
lw $t1, 248($fp)
la $t2, 252($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 252($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 256($fp)
sw $t1, 0($t2)
la $t0, tmp
la $t1, 240($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 256($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
lw $t0, 40($fp)
la $t1, 260($fp)
sw $t0, 0($t1)
la $t0, 264($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 260($fp)
lw $t1, 264($fp)
la $t2, 268($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 268($fp)
la $t1, 40($fp)
sw $t0, 0($t1)
lw $t0, 36($fp)
la $t1, 272($fp)
sw $t0, 0($t1)
la $t0, 276($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 272($fp)
lw $t1, 276($fp)
la $t2, 280($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 280($fp)
la $t1, 36($fp)
sw $t0, 0($t1)
j Label9
Label10:
lw $t0, 16($fp)
la $t1, 284($fp)
sw $t0, 0($t1)
lw $t0, 284($fp)
la $t1, 32($fp)
sw $t0, 0($t1)
Label11:
lw $t0, 32($fp)
la $t1, 288($fp)
sw $t0, 0($t1)
lw $t0, 40($fp)
la $t1, 292($fp)
sw $t0, 0($t1)
lw $t0, 288($fp)
lw $t1, 292($fp)
slt $t9, $t0, $t1
bnez $t9, Label12
lw $t0, 32($fp)
la $t1, 296($fp)
sw $t0, 0($t1)
lw $t0, 32($fp)
la $t1, 300($fp)
sw $t0, 0($t1)
la $t0, 300($fp)
la $t1, tmp
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 304($fp)
sw $t1, 0($t2)
la $t0, arr
la $t1, 296($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 304($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
la $t0, 308($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 32($fp)
lw $t1, 308($fp)
la $t2, 312($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 312($fp)
la $t1, 32($fp)
sw $t0, 0($t1)
j Label11
Label12:
lw $sp, 12($fp)
lw $fp, 8($sp)
lw $ra, 4($sp)
jr $ra
inner_merge_sort:
sw $ra, 4($sp)
sw $fp, 8($sp)
sw $sp, 12($sp)
add $fp, $sp, $zero
add $sp, $sp, 192
lw $t0, 20($fp)
la $t1, 32($fp)
sw $t0, 0($t1)
la $t0, 36($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 32($fp)
lw $t1, 36($fp)
sle $t9, $t0, $t1
beqz $t9, Label13
lw $sp, 12($fp)
lw $fp, 8($sp)
lw $ra, 4($sp)
jr $ra
Label13:
lw $t0, 20($fp)
la $t1, 40($fp)
sw $t0, 0($t1)
la $t0, 44($fp)
li $t1, 2
sw $t1, 0($t0)
lw $t0, 40($fp)
lw $t1, 44($fp)
seq $t9, $t0, $t1
beqz $t9, Label14
lw $t0, 16($fp)
la $t1, 48($fp)
sw $t0, 0($t1)
la $t0, 48($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 52($fp)
sw $t1, 0($t2)
lw $t0, 16($fp)
la $t1, 56($fp)
sw $t0, 0($t1)
la $t0, 60($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 56($fp)
lw $t1, 60($fp)
la $t2, 64($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 64($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 68($fp)
sw $t1, 0($t2)
lw $t0, 52($fp)
lw $t1, 68($fp)
sgt $t9, $t0, $t1
beqz $t9, Label15
lw $t0, 16($fp)
la $t1, 72($fp)
sw $t0, 0($t1)
la $t0, 72($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 76($fp)
sw $t1, 0($t2)
lw $t0, 76($fp)
la $t1, 24($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 80($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 84($fp)
sw $t0, 0($t1)
la $t0, 88($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 84($fp)
lw $t1, 88($fp)
la $t2, 92($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
la $t0, 92($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 96($fp)
sw $t1, 0($t2)
la $t0, arr
la $t1, 80($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 96($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
lw $t0, 16($fp)
la $t1, 100($fp)
sw $t0, 0($t1)
la $t0, 104($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 100($fp)
lw $t1, 104($fp)
la $t2, 108($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 24($fp)
la $t1, 112($fp)
sw $t0, 0($t1)
la $t0, arr
la $t1, 108($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 112($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
Label15:
lw $sp, 12($fp)
lw $fp, 8($sp)
lw $ra, 4($sp)
jr $ra
Label14:
lw $t0, 20($fp)
la $t1, 116($fp)
sw $t0, 0($t1)
la $t0, 120($fp)
li $t1, 2
sw $t1, 0($t0)
lw $t0, 116($fp)
lw $t1, 120($fp)
la $t2, 124($fp)
div $t0, $t1
mflo $t3
sw $t3, 0($t2)
lw $t0, 124($fp)
la $t1, 28($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 128($fp)
sw $t0, 0($t1)
lw $t0, 128($fp)
sw $t0, 16($sp)
lw $t0, 28($fp)
la $t1, 132($fp)
sw $t0, 0($t1)
lw $t0, 132($fp)
sw $t0, 20($sp)
jal inner_merge_sort
lw $t0, 16($fp)
la $t1, 136($fp)
sw $t0, 0($t1)
lw $t0, 28($fp)
la $t1, 140($fp)
sw $t0, 0($t1)
lw $t0, 136($fp)
lw $t1, 140($fp)
la $t2, 144($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 144($fp)
sw $t0, 16($sp)
lw $t0, 20($fp)
la $t1, 148($fp)
sw $t0, 0($t1)
lw $t0, 28($fp)
la $t1, 152($fp)
sw $t0, 0($t1)
lw $t0, 148($fp)
lw $t1, 152($fp)
la $t2, 156($fp)
sub $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 156($fp)
sw $t0, 20($sp)
jal inner_merge_sort
lw $t0, 16($fp)
la $t1, 160($fp)
sw $t0, 0($t1)
lw $t0, 160($fp)
sw $t0, 16($sp)
lw $t0, 28($fp)
la $t1, 164($fp)
sw $t0, 0($t1)
lw $t0, 164($fp)
sw $t0, 20($sp)
lw $t0, 16($fp)
la $t1, 168($fp)
sw $t0, 0($t1)
lw $t0, 28($fp)
la $t1, 172($fp)
sw $t0, 0($t1)
lw $t0, 168($fp)
lw $t1, 172($fp)
la $t2, 176($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 176($fp)
sw $t0, 24($sp)
lw $t0, 20($fp)
la $t1, 180($fp)
sw $t0, 0($t1)
lw $t0, 28($fp)
la $t1, 184($fp)
sw $t0, 0($t1)
lw $t0, 180($fp)
lw $t1, 184($fp)
la $t2, 188($fp)
sub $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 188($fp)
sw $t0, 28($sp)
jal merge
lw $sp, 12($fp)
lw $fp, 8($sp)
lw $ra, 4($sp)
jr $ra
main:
sw $ra, 4($sp)
sw $fp, 8($sp)
sw $sp, 12($sp)
add $fp, $sp, $zero
add $sp, $sp, 92
.data
$str3: .asciiz "17373248"
.text
la $a0, $str3
li $v0, 4
syscall
la $a0, $str1
li $v0, 4
syscall
li $v0, 5
syscall
la $t0, 16($fp)
sw $v0, 0($t0)
la $t0, 28($fp)
li $t1, 0
sw $t1, 0($t0)
lw $t0, 28($fp)
la $t1, 20($fp)
sw $t0, 0($t1)
Label16:
lw $t0, 20($fp)
la $t1, 32($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 36($fp)
sw $t0, 0($t1)
lw $t0, 32($fp)
lw $t1, 36($fp)
slt $t9, $t0, $t1
bnez $t9, Label17
li $v0, 5
syscall
la $t0, 24($fp)
sw $v0, 0($t0)
lw $t0, 20($fp)
la $t1, 40($fp)
sw $t0, 0($t1)
lw $t0, 24($fp)
la $t1, 44($fp)
sw $t0, 0($t1)
la $t0, arr
la $t1, 40($fp)
mul $t1, $t1, 4
add $t0, $t0, $t1
la $t2, 44($fp)
lw $t1, 0($t2)
sw $t1, 0($t0)
la $t0, 48($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 20($fp)
lw $t1, 48($fp)
la $t2, 52($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 52($fp)
la $t1, 20($fp)
sw $t0, 0($t1)
j Label16
Label17:
la $t0, 56($fp)
li $t1, 0
sw $t1, 0($t0)
lw $t0, 56($fp)
sw $t0, 16($sp)
lw $t0, 16($fp)
la $t1, 60($fp)
sw $t0, 0($t1)
lw $t0, 60($fp)
sw $t0, 20($sp)
jal inner_merge_sort
la $t0, 64($fp)
li $t1, 0
sw $t1, 0($t0)
lw $t0, 64($fp)
la $t1, 20($fp)
sw $t0, 0($t1)
Label18:
lw $t0, 20($fp)
la $t1, 68($fp)
sw $t0, 0($t1)
lw $t0, 16($fp)
la $t1, 72($fp)
sw $t0, 0($t1)
lw $t0, 68($fp)
lw $t1, 72($fp)
slt $t9, $t0, $t1
bnez $t9, Label19
lw $t0, 20($fp)
la $t1, 76($fp)
sw $t0, 0($t1)
la $t0, 76($fp)
la $t1, arr
mul $t1, $t1, 4
add $t0, $t0, $t1
lw $t1, 0($t0)
la $t2, 80($fp)
sw $t1, 0($t2)
lw $a0, 80($fp)
li $v0, 1
syscall
la $a0, $str1
li $v0, 4
syscall
la $t0, 84($fp)
li $t1, 1
sw $t1, 0($t0)
lw $t0, 20($fp)
lw $t1, 84($fp)
la $t2, 88($fp)
add $t3, $t0, $t1
sw $t3, 0($t2)
lw $t0, 88($fp)
la $t1, 20($fp)
sw $t0, 0($t1)
j Label18
Label19:
lw $sp, 12($fp)
lw $fp, 8($sp)
lw $ra, 4($sp)
jr $ra
