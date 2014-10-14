VAL=0

for f in ./runTests*
do
    ./$f
    rc=$?
    if [[ $rc != 0 ]] ; then
        VAL=1
    fi
done

exit $VAL
