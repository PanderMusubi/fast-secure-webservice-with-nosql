if [ $# -eq 0 ]
then
    echo "Error: specify a C source code file"
    exit 1
fi
splint -I. -I/usr/include/x86_64-linux-gnu -Ihtparse -Ioniguruma -Ievthr -Ibuild/compat +posixstrictlib $1
