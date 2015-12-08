echo "AUTH"
echo $BINSTAR_AUTH
echo "WORKER"
echo $BINSTAR_WORKER
echo "QUEUE"
echo $BINSTAR_QUEUE
anaconda -t $BINSTAR_AUTH worker register $BINSTAR_QUEUE -n $BINSTAR_WORKER
