echo "AUTH"
echo $BINSTAR_AUTH
echo "WORKER"
echo $BINSTAR_WORKER
echo "START"
nohup anaconda -t $BINSTAR_AUTH worker run $BINSTAR_WORKER > $BINSTAR_WORKER.log &
