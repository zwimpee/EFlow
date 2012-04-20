findtaskdir() {
    taskdir=`ls -t jobs | head -n1`
    return $taskdir
}

countJobsTask() {
    njobs=`ls -1 jobs/$1/*.sh | wc -l`
    return $njobs
}

isTaskDone() {
    nTaskJobs=countJobsTask $1
    nJobsDone=`ls -t logs/$1/*jobs.success | head -n1 | xargs wc -l | awk '{print $1}'`
    nJobsError=`ls -t logs/$1/*jobs*error | head -n 2 | xargs wc -l | tail -n 1 | awk '{print $1}'`

    if [ ${nJobsError} -e 0 ]; then
       return  "ERRORS"
    fi

    if [ ${nJobsDone} -eq ${nTaskJobs} ]; then
	return "YES"
    fi

    return "NO"
}
