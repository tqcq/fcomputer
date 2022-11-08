REG_NUM=64
echo "  mov r64,0"

for ((i = 0; i < $REG_NUM; ++i));
do
    echo "  mov r$i,$i"
done

echo "clear_loop:"
echo "  add r64,r64,64"
for ((i = 0; i < $REG_NUM; ++i));
do
    echo "  str [r$i],r$i"
    if [ $i == 32 ]; then
        echo "test r64,2048"
    fi
    echo "  add r$i,r$i,64"
done
echo "  jl clear_loop"
