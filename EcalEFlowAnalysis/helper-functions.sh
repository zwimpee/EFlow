findtaskdir() {
    echo "Looking for last folder of for task $1"
    taskId=`find jobs -type d | grep $1 | sort -r -n | head -n1 | awk -F'/'  '{ print $NF}'`
#    return $taskdir
}

countJobsTask() {
    nTaskjobs=`ls -1 jobs/${taskId}/*.sh | wc -l`
#    return $njobs
}

isTaskDone() {
    countJobsTask
    nJobsDone=`ls -1 logs/${taskId}/*.log | wc -l`
    nErrors=`ls -1 logs/${taskId}/*jobs*error | wc -l`

    echo "nJobs for task ${taskId} are ${nTaskjobs}"
    echo "nJobsDone for task ${taskId} are ${nJobsDone}"
    echo "task ${taskId} have ${nErrors} errors"

    if [ "${nErrors}" != "0" ]; then
	taskStatus="ERROR"
	return
    fi

    if [ "${nJobsDone}" = "${nTaskjobs}" ]; then
	taskStatus="YES"
	return
    fi

    taskStatus="NO"
}
