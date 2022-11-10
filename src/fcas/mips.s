addiu $sp,$sp,-9
sw    $ra, 4($sp)
sw    $30, 0($sp)
move  $30, $sp
lui   $1, 15258
ori   $3,$1,51711
